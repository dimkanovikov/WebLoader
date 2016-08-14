#include "networkqueue.h"

NetworkQueue::NetworkQueue()
{
    //
    // В нужном количестве создадим WebLoader'ы
    // И сразу же соединим их со слотом данного класса, обозначающим завершение
    //
    for (int i = 0; i != qMax(QThread::idealThreadCount(), 4); ++i) {
        freeWL.push_back(new WebLoader(this));
        connect(freeWL.back(), SIGNAL(downloadComplete(WebLoader*)), this, SLOT(downloadComplete(WebLoader*)));
    }
}

NetworkQueue* NetworkQueue::getInstance() {
    static NetworkQueue queue;
    return &queue;
}

void NetworkQueue::put(NetworkRequestInternal *request) {
    mtx.lock();
    //
    // Положим в очередь пришедший запрос
    //
    queue.push_back(request);
    inQueue.insert(request);

    //
    // В случае, если есть свободный WebLoader
    // Извлечем пришедший запрос из очереди и начнем выполнять его
    //
    if (!freeWL.empty()) {
        pop();
    }
    mtx.unlock();
}

void NetworkQueue::pop() {
    //
    // Извлечем свободный WebLoader
    //
    WebLoader* wl = freeWL.front();
    freeWL.pop_front();

    //
    // Извлечем первый запрос на обработку
    //
    NetworkRequestInternal *r = queue.front();
    queue.pop_front();
    inQueue.remove(r);

    //
    // Настроим WebLoader на запрос
    //
    usedWL[wl] = r;
    setWLParams(wl, r);

    //
    // Соединим сигналы WebLoader'а с сигналами класса запроса
    //
    connect(wl, SIGNAL(downloadComplete(QByteArray)), r, SIGNAL(downloadComplete(QByteArray)));
    connect(wl, SIGNAL(downloadComplete(QString)), r, SIGNAL(downloadComplete(QString)));
    connect(wl, SIGNAL(uploadProgress(int)), r, SIGNAL(uploadProgress(int)));
    connect(wl, SIGNAL(downloadProgress(int)), r, SIGNAL(downloadProgress(int)));
    connect(wl, SIGNAL(error(QString)), r, SIGNAL(error(QString)));
    connect(wl, SIGNAL(finished()), r, SIGNAL(finished()));

    //
    // Загружаем!
    //
    wl->loadAsync(r->m_request->urlToLoad(), r->m_request->urlReferer());
}

void NetworkQueue::stop(NetworkRequestInternal *internal) {
    mtx.lock();
    if (inQueue.contains(internal)) {
        //
        // Либо запрос еще в очереди
        // Тогда его нужно оттуда удалить
        //
        queue.removeAll(internal);
        inQueue.remove(internal);
    }
    else {
        //
        // Либо запрос уже обрабатывается
        //
        for (auto iter = usedWL.begin(); iter != usedWL.end(); ++iter) {
            //
            // Найдем запрос в списке обрабатывающихся
            //
            if (iter.value() == internal) {

                //
                // Отключим все сигналы
                // Обязательно сначала отключить сигналы, а затем остановить. Не наоборот!
                //
                disconnect(iter.key(), SIGNAL(downloadComplete(QByteArray)), iter.value(), SLOT(downloadComplete(QByteArray)));
                disconnect(iter.key(), SIGNAL(downloadComplete(QString)), iter.value(), SLOT(downloadComplete(QString)));
                disconnect(iter.key(), SIGNAL(uploadProgress(int)), iter.value(), SLOT(uploadProgress(int)));
                disconnect(iter.key(), SIGNAL(downloadProgress(int)), iter.value(), SLOT(downloadProgress(int)));
                disconnect(iter.key(), SIGNAL(error(QString)), iter.value(), SLOT(error(QString)));
                disconnect(iter.key(), SIGNAL(finished()), iter.value(), SLOT(finished()));

                //
                // Остановим запрос
                //
                iter.key()->stop();

                //
                // Удалим из списка используемых
                // К списку свободных WebLoader'ов припишет слот downloadComplete
                //
                usedWL.erase(iter);

                break;
            }
        }
    }
    mtx.unlock();
}

void NetworkQueue::setWLParams(WebLoader *wl, NetworkRequestInternal *r) {
    wl->setCookieJar(r->cookieJar);
    wl->setRequestMethod(r->method);
    wl->setLoadingTimeout(r->loadingTimeout);
    wl->setWebRequest(r->m_request);
}

void NetworkQueue::downloadComplete(WebLoader *wl) {
    mtx.lock();
    if (usedWL.contains(wl)) {
        //
        // Если запрос отработал до конца (не был прерван методом stop),
        // то необходимо отключить сигналы
        // и удалить из списка используемых
        //
        NetworkRequestInternal *r = usedWL[wl];

        disconnect(wl, SIGNAL(downloadComplete(QByteArray)), r, SLOT(downloadComplete(QByteArray)));
        disconnect(wl, SIGNAL(downloadComplete(QString)), r, SLOT(downloadComplete(QString)));
        disconnect(wl, SIGNAL(uploadProgress(int)), r, SLOT(uploadProgress(int)));
        disconnect(wl, SIGNAL(downloadProgress(int)), r, SLOT(downloadProgress(int)));
        disconnect(wl, SIGNAL(error(QString)), r, SLOT(error(QString)));
        disconnect(wl, SIGNAL(finished()), r, SLOT(finished()));

        usedWL.remove(wl);
    }

    //
    // Добавляем WebLoader в список свободных
    //
    freeWL.push_back(wl);

    //
    //Смотрим, надо ли что еще выполнить из очереди
    //
    if (!queue.empty()) {
        pop();
    }
    mtx.unlock();
}
