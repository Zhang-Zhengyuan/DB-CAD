#include "core/http_transport.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

namespace dbcad {

QString HttpTransport::timeoutErrorText() {
    return QString::fromUtf8("连接FastAPI超时（10秒）");
}

void QtHttpTransport::send(const HttpRequest& request, std::function<void(const HttpResponse&)> callback) {
    if (!request.url.isValid()) {
        callback(HttpResponse{-1, {}, QString::fromUtf8("FastAPI地址无效")});
        return;
    }

    QNetworkRequest networkRequest(request.url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    for (const auto& header : request.headers) {
        networkRequest.setRawHeader(header.first, header.second);
    }
    // QNAM 在信号回调到达前保持活动；超时通过挂在 reply 上的 QTimer 中止。
    QNetworkReply* reply = nullptr;
    if (request.method == QLatin1String("GET")) {
        reply = manager_.get(networkRequest);
    } else if (request.method == QLatin1String("POST")) {
        reply = manager_.post(networkRequest, request.payload);
    } else {
        callback(HttpResponse{-1, {}, QString::fromUtf8("不支持的HTTP方法")});
        return;
    }

    auto* timer = new QTimer(reply);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, reply, [reply]() {
        if (reply->isRunning()) {
            reply->abort();
        }
    });
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback]() {
        HttpResponse result;
        result.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        result.body = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            result.error = reply->error() == QNetworkReply::OperationCanceledError
                               ? HttpTransport::timeoutErrorText()
                               : reply->errorString();
        }
        callback(result);
        reply->deleteLater();
    });
    timer->start(10000);
}

} // namespace dbcad
