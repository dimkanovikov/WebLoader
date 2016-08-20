#include "NetworkRequest.h"

NetworkRequest::NetworkRequest(QObject* _parent, QNetworkCookieJar* _jar)
    : QObject(_parent), m_internal(_parent, _jar)
{
    //
    // Соединим сигналы от internal с сигналами/слотами этого класса
    connect(&m_internal, SIGNAL(downloadComplete(QByteArray, QUrl)),
            this, SIGNAL(downloadComplete(QByteArray, QUrl)));
    connect(&m_internal, SIGNAL(downloadComplete(QString, QUrl)),
            this, SIGNAL(downloadComplete(QString, QUrl)));
    connect(&m_internal, &NetworkRequestInternal::downloadProgress,
            this, &NetworkRequest::downloadProgress);
    connect(&m_internal, &NetworkRequestInternal::uploadProgress,
            this, &NetworkRequest::uploadProgress);
    connect(&m_internal, &NetworkRequestInternal::error,
            this, &NetworkRequest::slotError);
    connect(&m_internal, &NetworkRequestInternal::errorDetails,
            this, &NetworkRequest::slotErrorDetails);
    connect(&m_internal, &NetworkRequestInternal::finished,
            this, &NetworkRequest::finished);

}

NetworkRequest::~NetworkRequest()
{

}

void NetworkRequest::setCookieJar(QNetworkCookieJar* _cookieJar)
{
    stop();
    m_internal.m_cookieJar = _cookieJar;
}

QNetworkCookieJar* NetworkRequest::getCookieJar()
{
    return m_internal.m_cookieJar;
}

void NetworkRequest::setRequestMethod(WebLoader::RequestMethod _method)
{
    stop();
    m_internal.m_method = _method;
}

WebLoader::RequestMethod NetworkRequest::getRequestMethod() const
{
    return m_internal.m_method;
}

void NetworkRequest::setLoadingTimeout(int _loadingTimeout)
{
    stop();
    m_internal.m_loadingTimeout = _loadingTimeout;
}

int NetworkRequest::getLoadingTimeout() const
{
    return m_internal.m_loadingTimeout;
}

void NetworkRequest::clearRequestAttributes()
{
    stop();
    m_internal.m_request->clearAttributes();
}

void NetworkRequest::addRequestAttribute(QString _name, QVariant _value)
{
    stop();
    m_internal.m_request->addAttribute(_name, _value);
}

void NetworkRequest::addRequestAttributeFile(QString _name, QString _filePath)
{
    stop();
    m_internal.m_request->addAttribute(_name, _filePath);
}

void NetworkRequest::loadAsync(QString _urlToLoad, QUrl _referer)
{
    loadAsync(QUrl(_urlToLoad), _referer);
}

void NetworkRequest::loadAsync(QUrl _urlToLoad, QUrl _referer)
{
    //
    // Получаем инстанс очереди
    //
    NetworkQueue* nq = NetworkQueue::getInstance();

    //
    // Останавливаем в очереди запрос, связанный с данным классом (если имеется)
    //
    nq->stop(&m_internal);

    //
    // Настраиваем параметры и кладем в очередь
    m_internal.m_request->setUrlToLoad(_urlToLoad);
    m_internal.m_request->setUrlReferer(_referer);
    nq->put(&m_internal);
}

void NetworkRequest::loadAsyncS(QString _urlToLoad, QObject *_object,
                                const char* _slot, QUrl _referer)
{
    loadAsyncS(QUrl(_urlToLoad), _object, _slot, _referer);
}

void NetworkRequest::loadAsyncS(QUrl _urlToLoad, QObject *_object,
                               const char* _slot, QUrl _referer)
{
    NetworkRequest* request = new NetworkRequest;
    connect(request, SIGNAL(downloadComplete(QByteArray, QUrl)), _object, _slot);
    connect(request, SIGNAL(finished()), request, SLOT(deleteLater()));
    //request->m_internal.m_networkRequest = request;
    request->loadAsync(_urlToLoad, _referer);
}

QByteArray NetworkRequest::loadSyncS(QString _urlToLoad, QUrl _referer)
{
    return loadSyncS(QUrl(_urlToLoad), _referer);
}

QByteArray NetworkRequest::loadSyncS(QUrl _urlToLoad, QUrl _referer)
{
    NetworkRequest request;
    return request.loadSync(_urlToLoad, _referer);
}

QByteArray NetworkRequest::loadSync(QString _urlToLoad, QUrl _referer)
{
    return loadSync(QUrl(_urlToLoad), _referer);
}

QByteArray NetworkRequest::loadSync(QUrl _urlToLoad, QUrl _referer)
{
    //
    // Для синхронного запроса используем QEventLoop и асинхронный запрос
    //
    QEventLoop loop;
    connect(this, &NetworkRequest::finished,
            &loop, &QEventLoop::quit);
    connect(&m_internal, SIGNAL(downloadComplete(QByteArray, QUrl)),
            this, SLOT(downloadCompleteData(QByteArray)));
    loadAsync(_urlToLoad, _referer);
    loop.exec();

    return m_downloadedData;
}

QUrl NetworkRequest::url() const
{
    return m_internal.m_request->urlToLoad();
}

void NetworkRequest::downloadCompleteData(QByteArray _data)
{
    m_downloadedData = _data;
}

void NetworkRequest::slotError(QString _errorStr, QUrl _url)
{
    m_lastError = _errorStr;
    emit error(_errorStr, _url);
}

void NetworkRequest::slotErrorDetails(QString _errorStr)
{
    m_lastErrorDetails = _errorStr;
}

QString NetworkRequest::lastError() const
{
    return m_lastError;
}

void NetworkRequest::stop()
{
    NetworkQueue* nq = NetworkQueue::getInstance();
    nq->stop(&m_internal);
}

QString NetworkRequest::lastErrorDetails() const
{
    return m_lastErrorDetails;
}
