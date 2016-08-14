#include "networkrequest.h"

NetworkRequest::NetworkRequest(QObject * _parent, QNetworkCookieJar * _jar)
    : QObject(_parent), internal(_parent, _jar)
{
    //
    // Соединим сигналы от internal с сигналами/слотами этого класса
    connect(&internal, SIGNAL(downloadComplete(QByteArray)), this, SIGNAL(downloadComplete(QByteArray)));
    connect(&internal, SIGNAL(downloadComplete(QString)), this, SIGNAL(downloadComplete(QString)));
    connect(&internal, SIGNAL(downloadProgress(int)), this, SIGNAL(downloadProgress(int)));
    connect(&internal, SIGNAL(uploadProgress(int)), this, SIGNAL(uploadProgress(int)));
    connect(&internal, SIGNAL(error(QString)), this, SLOT(slotError(QString)));
    connect(&internal, SIGNAL(finished()), this, SIGNAL(finished()));

}

NetworkRequest::~NetworkRequest()
{

}

void NetworkRequest::setCookieJar(QNetworkCookieJar * cookieJar)
{
    stop();
    internal.cookieJar = cookieJar;
}

QNetworkCookieJar* NetworkRequest::getCookieJar()
{
    return internal.cookieJar;
}

void NetworkRequest::setRequestMethod(WebLoader::RequestMethod method)
{
    stop();
    internal.method = method;
}

WebLoader::RequestMethod NetworkRequest::getRequestMethod() const
{
    return internal.method;
}

void NetworkRequest::setLoadingTimeout(int loadingTimeout)
{
    stop();
    internal.loadingTimeout = loadingTimeout;
}

int NetworkRequest::getLoadingTimeout() const
{
    return internal.loadingTimeout;
}

void NetworkRequest::clearRequestAttributes()
{
    stop();
    internal.m_request->clearAttributes();
}

void NetworkRequest::addRequestAttribute(QString name, QVariant value)
{
    stop();
    internal.m_request->addAttribute(name, value);
}

void NetworkRequest::addRequestAttributeFile(QString name, QString filePath)
{
    stop();
    internal.m_request->addAttribute(name, filePath);
}

void NetworkRequest::loadAsync(QString urlToLoad, QUrl referer)
{
    loadAsync(QUrl(urlToLoad), referer);
}

void NetworkRequest::loadAsync(QUrl urlToLoad, QUrl referer)
{
    //
    // Получаем инстанс очереди
    //
    NetworkQueue *nq = NetworkQueue::getInstance();

    //
    // Останавливаем в очереди запрос, связанный с данным классом (если имеется)
    //
    nq->stop(&internal);

    //
    // Настраиваем параметры и кладем в очередь
    internal.m_request->setUrlToLoad(urlToLoad);
    internal.m_request->setUrlReferer(referer);
    nq->put(&internal);
}

QByteArray NetworkRequest::loadSync(QString urlToLoad, QUrl referer)
{
    return loadSync(QUrl(urlToLoad), referer);
}

QByteArray NetworkRequest::loadSync(QUrl urlToLoad, QUrl referer)
{
    //
    // Для синхронного запроса используем QEventLoop и асинхронный запрос
    //
    QEventLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&internal, SIGNAL(downloadComplete(QByteArray)), this, SLOT(downloadCompleteData(QByteArray)));
    loadAsync(urlToLoad, referer);
    loop.exec();

    return m_downloadedData;
}

QUrl NetworkRequest::url() const
{
    return internal.m_request->urlToLoad();
}

void NetworkRequest::downloadCompleteData(QByteArray data)
{
    m_downloadedData = data;
}

void NetworkRequest::slotError(QString errorStr)
{
    m_lastError = errorStr;
    emit error(errorStr);
}

QString NetworkRequest::lastError() const
{
    return m_lastError;
}

void NetworkRequest::stop()
{
    NetworkQueue *nq = NetworkQueue::getInstance();
    nq->stop(&internal);
}

//QString NetworkRequest::lastErrorDetails() const;
