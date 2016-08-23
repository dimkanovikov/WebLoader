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

#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QEventLoop>
#include <QTimer>

#include "NetworkQueue_p.h"
#include "NetworkRequestPrivate_p.h"

/*!
 * \brief Пользовательский класс для создания GET и POST запросов
 */

class NetworkRequest : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief Конструктор
     */
    explicit NetworkRequest(QObject* _parent = 0, QNetworkCookieJar* _jar = 0);
    /*!
     * \brief Деструктор
     */
    virtual ~NetworkRequest();

    /*!
     * \brief Установка cookie для загрузчика
     */
    void setCookieJar(QNetworkCookieJar* _cookieJar);
    /*!
     * \brief Получение cookie загрузчика
     */
    QNetworkCookieJar* getCookieJar();
    /*!
     * \brief Установка метода запроса
     */
    void setRequestMethod(WebLoader::RequestMethod _method);
    /*!
     * \brief Получение метода запроса
     */
    WebLoader::RequestMethod getRequestMethod() const;
    /*!
     * \brief Установка таймаута загрузки
     */
    void setLoadingTimeout(int _loadingTimeout);
    /*!
     * \brief Получение таймаута загрузки
     */
    int getLoadingTimeout() const;
    /*!
     * \brief Очистить все старые атрибуты запроса
     */
    void clearRequestAttributes();
    /*!
     * \brief Добавление атрибута в запрос
     */
    void addRequestAttribute(const QString& _name, const QVariant& _value);
    /*!
     * \brief Добавление файла в запрос
     */
    void addRequestAttributeFile(const QString& _name, const QString& _filePath);
    /*!
     * \brief Асинхронная загрузка запроса
     */
    void loadAsync(const QString& _urlToLoad, const QUrl& _referer = QUrl());
    /*!
     * \brief Асинхронная загрузка запроса
     */
    void loadAsync(const QUrl& _urlToLoad, const QUrl& _referer = QUrl());
    static void loadAsyncS(const QString& _urlToLoad, QObject* _object,
                           const char* _slot, const QUrl& _referer = QUrl());
    static void loadAsyncS(const QUrl& _urlToLoad, QObject* _object,
                           const char* _slot, const QUrl& _referer = QUrl());
    /*!
     * \brief Синхронная загрузка запроса
     */
    QByteArray loadSync(const QString& _urlToLoad, const QUrl& _referer = QUrl());
    /*!
     * \brief Синхронная загрузка запроса
     */
    QByteArray loadSync(const QUrl& _urlToLoad, const QUrl& _referer = QUrl());
    static QByteArray loadSyncS(const QUrl& _urlToLoad, const QUrl& _referer = QUrl());
    static QByteArray loadSyncS(const QString& _urlToLoad, const QUrl& _referer = QUrl());
    /*!
     * \brief Получение загруженного URL
     */
    QUrl url() const;
    /*!
     * \brief Получение строки с последней ошибкой
     */
    QString lastError() const;
    QString lastErrorDetails() const;

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
    void finished();

private:

    /*!
     * \brief Объект, используемый в очереди запросов
     */
    NetworkRequestPrivate m_internal;
    /*!
     * \brief Загруженные данные в случае, если используется синхронная загрузка
     */
    QByteArray m_downloadedData;
    /*!
     * \brief Строка, содержащая описание последней ошибки
     */
    QString m_lastError;
    QString m_lastErrorDetails;

    /*!
     * \brief Остановка выполнения запроса, связанного с текущим объектом
     * и удаление запросов, ожидающих в очереди, связанных с текущим объектом
     */
    void stop();

private slots:
    /*!
     * \brief Данные загружены. Используется при синхронной загрузке
     */
    void downloadCompleteData(const QByteArray&);
    /*!
     * \brief Ошибка при получении данных
     */
    void slotError(const QString&, const QUrl&);
    void slotErrorDetails(const QString&);
};

#endif // NETWORKREQUEST_H
