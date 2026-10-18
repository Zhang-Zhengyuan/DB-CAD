#pragma once

// HttpTransport —— HTTP 传输抽象（架构 v2 核心库）。
//
// CollabBackend 的所有 HTTP 调用经由本接口发出；生产环境用 QtHttpTransport
// （QNetworkAccessManager + 本地事件循环，M1 保持同步语义，异步化留给 M2），
// 单元测试注入假传输（内存实现 json-delta 线协议语义），实现
// "对 MemoryStore/假服务器的协议测试"（REFACTOR_PLAN M1.4 验收）。

#include <QByteArray>
#include <QList>
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
    virtual HttpResponse send(const HttpRequest& request) = 0;
};

// 生产实现：QNetworkAccessManager，10 秒超时，同步返回（嵌套事件循环）。
// 注意：调用期间会处理事件（M2 将把网络调用移出 GUI 线程）。
class QtHttpTransport : public HttpTransport {
public:
    HttpResponse send(const HttpRequest& request) override;
};

} // namespace dbcad
