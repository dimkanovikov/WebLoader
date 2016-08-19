#ifndef NETWORKREQUESTINTERNAL_H
#define NETWORKREQUESTINTERNAL_H

#include <QObject>

#include "WebLoader_p.h"

/*!
 * \brief Внутренний класс для хранения в очереди запросов
 * Используется только классами NetworkRequest и NetworkQueue
 * Для упрощения взаимодействия данные класса являются public
 */

class NetworkRequestInternal : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequestInternal(QObject* _parent = 0, QNetworkCookieJar* _jar = 0);
    ~NetworkRequestInternal();

    QUrl m_urlToLoad;
    QUrl m_referer;
    QNetworkCookieJar* m_cookieJar;
    WebLoader::RequestMethod m_method;
    int m_loadingTimeout;
    WebRequest* m_request;

signals:
    /*!
     * \brief Прогресс отправки запроса на сервер
     */
    void uploadProgress(int, QUrl);
    /*!
     * \brief Прогресс загрузки данных с сервера
     */
    void downloadProgress(int, QUrl);
    /*!
     * \brief Данные загружены
     */
    void downloadComplete(QByteArray, QUrl);
    void downloadComplete(QString, QUrl);
    /*!
     * \brief Сигнал об ошибке
     */
    void error(QString, QUrl);
    void errorDetails(QString, QUrl);
    void finished();

public slots:
};

#endif // NETWORKREQUESTINTERNAL_H
