#include "core/http_transport.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>

namespace dbcad {

HttpResponse QtHttpTransport::send(const HttpRequest& request) {
    HttpResponse result;

    if (!request.url.isValid()) {
        result.error = QString::fromUtf8("FastAPI地址无效");
        return result;
    }

    QNetworkRequest networkRequest(request.url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    for (const auto& header : request.headers) {
        networkRequest.setRawHeader(header.first, header.second);
    }
    QNetworkAccessManager manager;
    QNetworkReply* reply = nullptr;
    if (request.method == QLatin1String("GET")) {
        reply = manager.get(networkRequest);
    } else if (request.method == QLatin1String("POST")) {
        reply = manager.post(networkRequest, request.payload);
    } else {
        result.error = QString::fromUtf8("不支持的HTTP方法");
        return result;
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, [&]() {
        if (reply->isRunning()) {
            reply->abort();
            result.error = QString::fromUtf8("连接FastAPI超时（10秒）");
        }
    });
    timer.start(10000);
    loop.exec();
    timer.stop();

    result.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    result.body = reply->readAll();
    if (reply->error() != QNetworkReply::NoError && result.error.isEmpty()) {
        result.error = reply->errorString();
    }
    reply->deleteLater();
    return result;
}

} // namespace dbcad
