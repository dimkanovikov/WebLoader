/*
* Copyright (C) 2015 Dimka Novikov, to@dimkanovikov.pro
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3 of the License, or any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* Full license: http://dimkanovikov.pro/license/LGPLv3
*/

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

    void done();

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
