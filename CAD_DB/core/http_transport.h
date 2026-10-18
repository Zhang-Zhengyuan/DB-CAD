#pragma once

// HttpTransport —— HTTP 传输抽象（架构 v2 核心库）。
//
// CollabBackend 的所有 HTTP 调用经由本接口发出；生产环境用 QtHttpTransport
// （QNetworkAccessManager 原生异步，回调经 finished 信号在 GUI 线程派发，
// M2 起没有嵌套事件循环），单元测试注入假传输（内存实现 json-delta 线协议
// 语义）。

#include <functional>

#include <QByteArray>
#include <QList>
#include <QNetworkAccessManager>
#include <QPair>
#include <QString>
#include <QUrl>

namespace dbcad {

struct HttpRequest {
    QString method; // "GET" | "POST"
    QUrl url;       // 完整 URL（含 query）
    QByteArray payload;
    QList<QPair<QByteArray, QByteArray>> headers;
};

struct HttpResponse {
    int statusCode = -1; // -1 表示网络层失败（未收到 HTTP 应答）
    QByteArray body;
    QString error; // 网络层错误描述；HTTP 应答到达时为空
};

class HttpTransport {
public:
    virtual ~HttpTransport() = default;
    // 异步发送；回调在发起线程被调用（生产实现经 Qt 信号派发）。
    // 实现保证回调恰好调用一次。
    virtual void send(const HttpRequest& request, std::function<void(const HttpResponse&)> callback) = 0;

protected:
    static QString timeoutErrorText(); // 统一超时文案（10 秒）
};

// 生产实现：QNetworkAccessManager，10 秒超时，回调经 finished 信号派发。
class QtHttpTransport : public HttpTransport {
public:
    QtHttpTransport() = default;
    void send(const HttpRequest& request, std::function<void(const HttpResponse&)> callback) override;

private:
    QNetworkAccessManager manager_;
};

} // namespace dbcad
