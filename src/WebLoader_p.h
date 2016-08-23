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

#ifndef WEBLOADER_H
#define WEBLOADER_H

#include "WebRequest_p.h"

#include <QtCore/QThread>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

class QNetworkAccessManager;
class QNetworkCookieJar;


/*!
  \class WebLoader

  \brief Класс для работы с http-протоколом
  */
class WebLoader : public QThread
{
	Q_OBJECT

public:
	/*!
	  \enum Метод запроса
	  */
	enum RequestMethod {
		Undefined, /*!< Метод не установлен */
		Get,
		Post
	};

public:
	/*!
	  \fn Конструктор
	  */
    explicit WebLoader(QObject* _parent = 0, QNetworkCookieJar* _jar = 0);
	/*!
	  \fn Деструктор
	  */
	virtual ~WebLoader();

	/*!
	  \fn Установка куков для сессии загрузчика
	  \param cookie - коллекция куков
	  */
    void setCookieJar(QNetworkCookieJar* _cookieJar);
	/*!
	  \fn Установка метода запроса
	  \param method - метод запроса
	  */
    void setRequestMethod(RequestMethod _method);

    /**
     * @brief Установить таймаут загрузки
     */
    void setLoadingTimeout(int _msecs);
    void setWebRequest(WebRequest* _request);

	/*!
      \fn Отправка запроса (асинхронное выполнение)
	  \param urlToLoad - ссылка для запроса
	  \param referer   - реферальная ссылка
      */
    void loadAsync(QUrl _urlToLoad, QUrl _referer = QUrl());

    /**
     * @brief Остановить выполнение
     */
    void stop();

signals:
	/*!
	  \fn Прогресс отправки запроса на сервер
	  \param Процент отправленных данных
	  */
    void uploadProgress(int, QUrl);
	/*!
	  \fn Прогресс загрузки данных с сервера
	  \param Процент загруженных данных
	  */
    void downloadProgress(int, QUrl);
	/*!
	  \fn Данные загружены
	  \param Загруженные данные
	  */
    void downloadComplete(WebLoader*);
    void downloadComplete(QByteArray, QUrl);
    void downloadComplete(QString, QUrl);
	/*!
	  \fn Сигнал об ошибке
	  \param Текст ошибки
	  */
    void error(QString, QUrl);
    void errorDetails(QString, QUrl);


//*****************************************************************************
// Внутренняя реализация класса

private:
	/*!
	  \fn Работа потока
	  */
	void run();


private slots:
	/*!
	  \fn Прогресс отправки запроса на сервер
	  \param uploadedBytes - отправлено байт
	  \param totalBytes - байт к отправке
	  */
    void uploadProgress(qint64 _uploadedBytes, qint64 _totalBytes);
	/*!
	  \fn Прогресс загрузки данных с сервера
	  \param recievedBytes загружено байт
	  \param totalBytes - байт к отправке
	  */
    void downloadProgress(qint64 _recievedBytes, qint64 _totalBytes);
	/*!
	  \fn Окончание загрузки страницы
	  \param reply - ответ сервера
	  */
    void downloadComplete(QNetworkReply* _reply);
	/*!
	  \fn Ошибка при загрузки страницы
	  \param networkError - ошибка
	  */
    void downloadError(QNetworkReply::NetworkError _networkError);
	/*!
	 * \fn Ошибки при защищённом подключении
	 * \param Список ошибок
	 */
	void downloadSslErrors(const QList<QSslError>& _errors);


//*****************************************************************************
// Методы доступа к данным класса, а так же вспомогательные
// методы для работы с данными класса
private:
	void initNetworkManager();

// Данные класса
private:
    QNetworkAccessManager* m_networkManager;
    QNetworkCookieJar* m_cookieJar;
    WebRequest* m_request;
	RequestMethod m_requestMethod;
	bool m_isNeedRedirect;
    QUrl m_initUrl;

    /**
     * @brief Таймаут загрузки ссылки
     */
    int m_loadingTimeout;

	QByteArray m_downloadedData;
	QString m_lastError;
	QString m_lastErrorDetails;
};

#endif // WEBLOADER_H
