/*
* Copyright (C) 2016 Alexey Polushkin, armijo38@yandex.ru
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

#ifndef NETWORKREQUESTLOADER_H
#define NETWORKREQUESTLOADER_H

#include <QByteArray>
#include <QUrl>
#include "NetworkRequest.h"

class NetworkRequestLoader {
public:
	template<typename Func>
	static void loadAsync(const QString& _urlToLoad, Func _func)
	{
		loadAsync(QUrl(_urlToLoad), _func);
	}

	template<typename Func>
	static void loadAsync(const QUrl& _urlToLoad, Func _func)
	{
		NetworkRequest* request = new NetworkRequest;
		QObject::connect(request, static_cast<void (NetworkRequest::*)(QByteArray, QUrl)>(&NetworkRequest::downloadComplete), _func);
		QObject::connect(request, &NetworkRequest::finished, request, &NetworkRequest::deleteLater);
		request->loadAsync(_urlToLoad);
	}

	template<typename Func>
	static void loadAsync(const QString& _urlToLoad, const typename QtPrivate::FunctionPointer<Func>::Object* _reciever = 0, Func _slot = 0)
	{
		loadAsync(QUrl(_urlToLoad), _reciever, _slot);
	}

	template<typename Func>
	static void loadAsync(const QUrl& _urlToLoad, const typename QtPrivate::FunctionPointer<Func>::Object* _reciever = 0, Func _slot = 0)
	{
		NetworkRequest* request = new NetworkRequest;
		QObject::connect(request, static_cast<void (NetworkRequest::*)(QByteArray, QUrl)>(&NetworkRequest::downloadComplete), _reciever, _slot);
		QObject::connect(request, &NetworkRequest::finished, request, &NetworkRequest::deleteLater);
		request->loadAsync(_urlToLoad);
	}

	static QByteArray loadSync(const QString& _urlToLoad)
	{
		return loadSync(QUrl(_urlToLoad));
	}

	static QByteArray loadSync(const QUrl& _urlToLoad)
	{
		NetworkRequest request;
		return request.loadSync(_urlToLoad);
	}
};

#endif // NETWORKREQUESTLOADER_H
