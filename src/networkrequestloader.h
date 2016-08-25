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
    static void loadAsync(const QString& _urlToLoad, QObject* _object,
                                    const char* _slot, const QUrl& _referer = QUrl())
    {
        loadAsync(QUrl(_urlToLoad), _object, _slot, _referer);
    }

    static void loadAsync(const QUrl& _urlToLoad, QObject* _object,
                                   const char* _slot, const QUrl& _referer = QUrl())
    {
        NetworkRequest* request = new NetworkRequest;
        QObject::connect(request, SIGNAL(downloadComplete(QByteArray, QUrl)), _object, _slot);
        QObject::connect(request, &NetworkRequest::finished, request, &NetworkRequest::deleteLater);
        request->loadAsync(_urlToLoad, _referer);
    }

    template<typename Func>
    static void loadAsync(const QString& _urlToLoad, Func _func, const QUrl& _referer = QUrl())
    {
        loadAsync(QUrl(_urlToLoad), _func, _referer);
    }

    template<typename Func>
    static void loadAsync(const QUrl& _urlToLoad, Func _func, const QUrl& _referer = QUrl())
    {
        NetworkRequest* request = new NetworkRequest;
        QObject::connect(request, static_cast<void (NetworkRequest::*)(QByteArray, QUrl)>(&NetworkRequest::downloadComplete),
                _func);
        QObject::connect(request, &NetworkRequest::finished, request, &NetworkRequest::deleteLater);
        request->loadAsync(_urlToLoad, _referer);
    }

    static QByteArray loadSync(const QString& _urlToLoad, const QUrl& _referer = QUrl())
    {
        return loadSync(QUrl(_urlToLoad), _referer);
    }

    static QByteArray loadSync(const QUrl& _urlToLoad, const QUrl& _referer = QUrl())
    {
        NetworkRequest request;
        return request.loadSync(_urlToLoad, _referer);
    }
};

#endif // NETWORKREQUESTLOADER_H
