#ifndef NETWORKREQUESTINTERNAL_H
#define NETWORKREQUESTINTERNAL_H

#include <QObject>

#include "WebLoader.h"

/*!
 * \brief Внутренний класс для хранения в очереди запросов
 * Используется только классами NetworkRequest и NetworkQueue
 * Для упрощения взаимодействия данные класса являются public
 */
class NetworkRequestInternal : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequestInternal(QObject *parent = 0, QNetworkCookieJar *jar = 0);
    ~NetworkRequestInternal();

    QUrl urlToLoad;
    QUrl referer;
    QNetworkCookieJar *cookieJar;
    WebLoader::RequestMethod method;
    int loadingTimeout;
    WebRequest *m_request;

signals:
    /*!
     * \brief Прогресс отправки запроса на сервер
     */
    void uploadProgress(int);
    /*!
     * \brief Прогресс загрузки данных с сервера
     */
    void downloadProgress(int);
    /*!
     * \brief Данные загружены
     */
    void downloadComplete(QByteArray);
    void downloadComplete(QString);
    /*!
     * \brief Сигнал об ошибке
     */
    void error(QString);
    void finished();

public slots:
};

#endif // NETWORKREQUESTINTERNAL_H
