#include "NetworkQueue_p.h"

NetworkQueue::NetworkQueue()
{
    //
    // В нужном количестве создадим WebLoader'ы
    // И сразу же соединим их со слотом данного класса, обозначающим завершение
    //
    for (int i = 0; i != qMax(QThread::idealThreadCount(), 4); ++i) {
        m_freeLoaders.push_back(new WebLoader(this));
        connect(m_freeLoaders.back(), SIGNAL(finished()), this, SLOT(downloadComplete()));
    }
}

NetworkQueue* NetworkQueue::getInstance() {
    static NetworkQueue queue;
    return &queue;
}

void NetworkQueue::put(NetworkRequestInternal* _request) {
    m_mtx.lock();
    //
    // Положим в очередь пришедший запрос
    //
    m_queue.push_back(_request);
    m_inQueue.insert(_request);

    //
    // В случае, если есть свободный WebLoader
    // Извлечем пришедший запрос из очереди и начнем выполнять его
    //
    if (!m_freeLoaders.empty()) {
        pop();
    }
    m_mtx.unlock();
}

void NetworkQueue::pop() {
    //
    // Извлечем свободный WebLoader
    //
    WebLoader* loader = m_freeLoaders.front();
    m_freeLoaders.pop_front();

    //
    // Извлечем первый запрос на обработку
    //
    NetworkRequestInternal *request = m_queue.front();
    m_queue.pop_front();
    m_inQueue.remove(request);

    //
    // Настроим WebLoader на запрос
    //
    m_busyLoaders[loader] = request;
    setLoaderParams(loader, request);

    //
    // Соединим сигналы WebLoader'а с сигналами класса запроса
    //
    connect(loader, SIGNAL(downloadComplete(QByteArray, QUrl)),
            request, SIGNAL(downloadComplete(QByteArray, QUrl)));
    connect(loader, SIGNAL(downloadComplete(QString, QUrl)),
            request, SIGNAL(downloadComplete(QString, QUrl)));
    connect(loader, SIGNAL(uploadProgress(int, QUrl)),
            request, SIGNAL(uploadProgress(int, QUrl)));
    connect(loader, SIGNAL(downloadProgress(int, QUrl)),
            request, SIGNAL(downloadProgress(int, QUrl)));
    connect(loader, &WebLoader::error,
            request, &NetworkRequestInternal::error);
    connect(loader, &WebLoader::errorDetails,
            request, &NetworkRequestInternal::errorDetails);

    //
    // Загружаем!
    //
    loader->loadAsync(request->m_request->urlToLoad(), request->m_request->urlReferer());
}

void NetworkQueue::stop(NetworkRequestInternal* _internal) {
    m_mtx.lock();
    if (m_inQueue.contains(_internal)) {
        //
        // Либо запрос еще в очереди
        // Тогда его нужно оттуда удалить
        //
        m_queue.removeAll(_internal);
        m_inQueue.remove(_internal);
    }
    else {
        //
        // Либо запрос уже обрабатывается
        //
        for (auto iter = m_busyLoaders.begin(); iter != m_busyLoaders.end(); ++iter) {
            //
            // Найдем запрос в списке обрабатывающихся
            //
            if (iter.value() == _internal) {

                //
                // Отключим все сигналы
                // Обязательно сначала отключить сигналы, а затем остановить. Не наоборот!
                //
                disconnectLoaderRequest(iter.key(), iter.value());

                //
                // Остановим запрос
                //
                iter.key()->stop();

                iter.value()->done();
                //
                // Удалим из списка используемых
                // К списку свободных WebLoader'ов припишет слот downloadComplete
                //
                m_busyLoaders.erase(iter);

                break;
            }
        }
    }
    m_mtx.unlock();
}

void NetworkQueue::setLoaderParams(WebLoader* _loader, NetworkRequestInternal* request)
{
    _loader->setCookieJar(request->m_cookieJar);
    _loader->setRequestMethod(request->m_method);
    _loader->setLoadingTimeout(request->m_loadingTimeout);
    _loader->setWebRequest(request->m_request);
}

void NetworkQueue::disconnectLoaderRequest(WebLoader* _loader, NetworkRequestInternal* _request)
{
    disconnect(_loader, SIGNAL(downloadComplete(QByteArray, QUrl)),
               _request, SIGNAL(downloadComplete(QByteArray, QUrl)));
    disconnect(_loader, SIGNAL(downloadComplete(QString, QUrl)),
               _request, SIGNAL(downloadComplete(QString, QUrl)));
    disconnect(_loader, SIGNAL(uploadProgress(int, QUrl)),
               _request, SIGNAL(uploadProgress(int, QUrl)));
    disconnect(_loader, SIGNAL(downloadProgress(int, QUrl)),
               _request, SIGNAL(downloadProgress(int, QUrl)));
    disconnect(_loader, &WebLoader::error,
               _request, &NetworkRequestInternal::error);
}

void NetworkQueue::downloadComplete()
{
    m_mtx.lock();
    WebLoader* loader = qobject_cast<WebLoader*>(sender());
    if (m_busyLoaders.contains(loader)) {
        //
        // Если запрос отработал до конца (не был прерван методом stop),
        // то необходимо отключить сигналы
        // и удалить из списка используемых
        //
        NetworkRequestInternal* request = m_busyLoaders[loader];

        disconnectLoaderRequest(loader, request);
        request->done();

        m_busyLoaders.remove(loader);
    }

    //
    // Добавляем WebLoader в список свободных
    //
    m_freeLoaders.push_back(loader);

    //
    //Смотрим, надо ли что еще выполнить из очереди
    //
    if (!m_queue.empty()) {
        pop();
    }
    m_mtx.unlock();
}
