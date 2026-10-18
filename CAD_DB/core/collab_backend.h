#pragma once

// CollabBackend —— 协作传输后端（架构 v2 核心库，REFACTOR_PLAN M1.4 + M1.5）。
//
// 职责：
//   * HTTP：json-delta 线协议的客户端封装，返回类型化结果（含 Conflict）；
//     传输经 HttpTransport 抽象发出（生产 QtHttpTransport，测试注入内存实现）。
//   * WS：连接管理（重连/心跳定时器、URL 构造、原始消息转发）与在场名单维护，
//     移植自原 mainwindow_presence.cpp；UI 反应一律通过信号交还上层。
//   * 实现 StorageBackend 接口（open/save/list/remove）。
//
// 本阶段 HTTP 保持同步（嵌套事件循环），异步化是 M2 的事。
// 线协议冻结：POST/GET /projects/{id}/json-delta 与 WS 通知语义不变。

#include <memory>
#include <optional>
#include <functional>

#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>

#include "core/http_transport.h"
#include "core/storage_backend.h"

class QWebSocket;
class QTimer;

namespace dbcad {

// json-delta 载荷（与线协议字段一一对应）。
struct JsonGraphPayload {
    int version = 0;
    QString graphJson;
    QString changesJson;
    QStringList removedIds;
    bool delta = false;
    QString graphHash;
};

struct ProjectRef {
    QString id;
    QString name;
};

class CollabBackend : public QObject, public StorageBackend {
    Q_OBJECT
public:
    // transport 归调用方所有（默认创建 QtHttpTransport）。
    explicit CollabBackend(QObject* parent = nullptr);
    ~CollabBackend() override;

    void setTransport(HttpTransport* transport); // 所有权不转移，须保证生命周期覆盖本对象
    void configure(const QString& baseUrl, const QString& author, const QString& apiPassword);

    bool isConfigured() const override;
    QString baseUrl() const { return baseUrl_; }
    QString author() const { return author_; }
    QString clientId() const { return clientId_; }
    QString lastError() const { return lastError_; }
    int lastStatusCode() const { return lastHttpStatus_; }

    // ---------- HTTP（类型化结果） ----------
    struct ProjectLookup {
        bool found = false;
        bool failed = false; // 网络或服务器错误（error 有效）
        ProjectRef project;
        QString error;
    };
    // 404 → found=false 且 failed=false；网络/其他错误 → failed=true。
    ProjectLookup findProject(const QString& projectName);
    // 409（已存在）→ ok=false 且 error 为空（与旧 BackendApiClient 语义一致）。
    struct ProjectCreate {
        bool ok = false;
        ProjectRef project;
        QString error;
    };
    ProjectCreate createProject(const QString& projectName);
    ProjectLookup getOrCreateProject(const QString& projectName);

    // 推送 json-delta；409 → status=Conflict，其余失败 → Error。
    struct PushResult {
        enum class Status { Created, Conflict, Error };
        Status status = Status::Error;
        int version = 0;
        QString error;
    };
    PushResult pushJsonDelta(const QString& projectId, const QString& author, const QString& entityGraphJson,
                             const QString& changesJson, const QStringList& removedIds,
                             std::optional<int> baseVersion, const QString& sourceClientId = QString());

    struct FetchResult {
        enum class Status { Ok, NotFound, Error };
        Status status = Status::Error;
        JsonGraphPayload payload;
        QString error;
    };
    FetchResult fetchJsonDelta(const QString& projectId, int baseVersion);
    FetchResult fetchJsonVersion(const QString& projectId, int version);

    // "测试连接"：与旧 getProjectByName 探测语义一致（收到任何 HTTP 应答即成功）。
    bool testConnection(QString* error);

    // ---------- WS 通道 ----------
    // 打开（或重开）项目的实时通道；project 为空或未配置则保持关闭。
    void openChannel(const QString& projectId);
    void closeChannel();
    bool channelReady() const;
    bool channelOpening() const { return socket_ != nullptr; }
    bool hasChannelProject() const { return !channelProjectId_.isEmpty(); }
    void sendSyncNow();
    // 重连策略由上层注入（原"模式为 FastAPI 且项目非空"判断）；返回 false 时重连定时器停止。
    void setReconnectPolicy(std::function<bool()> policy) { reconnectPolicy_ = std::move(policy); }

    // 在场名单：presence_* 消息经 processPresence 更新。
    QHash<QString, QString> collaborators() const { return collaborators_; }
    void processPresenceMessage(const QJsonObject& root, const QString& messageType);

    // ---------- StorageBackend ----------
    QString backendName() const override { return QStringLiteral("fastapi-collab"); }
    bool isCollaborative() const override { return true; }
    std::optional<StorageDocument> open(const QString& location, QString* error) override;
    std::optional<int> save(const QString& location, const SaveRequest& request, QString* error) override;
    QStringList list(const QString& scope, QString* error) override;
    bool remove(const QString& location, QString* error) override;

signals:
    void channelStateChanged(const QString& stateKey); // Connecting / Connected / Reconnecting / NotConnected
    void channelConnected();
    void channelDisconnected();
    void channelError(const QString& message);
    void textMessageReceived(const QString& message); // 原始 WS 文本，上层做模式门控与分发
    void collaboratorsChanged();

private:
    struct HttpCall {
        int statusCode = -1;
        QByteArray body;
        QString networkError;
        bool networkFailed() const { return statusCode <= 0; }
    };
    HttpCall sendJsonRequest(const QString& method, const QString& path, const QByteArray& payload = {});
    QUrl buildChannelUrl(const QString& projectId) const;

    HttpTransport* transport_ = nullptr;
    std::unique_ptr<class QtHttpTransport> defaultTransport_; // 无注入时使用
    QString baseUrl_;
    QString author_;
    QString apiPassword_;
    QString clientId_;
    mutable QString lastError_;
    mutable int lastHttpStatus_ = -1;

    QWebSocket* socket_ = nullptr;
    QTimer* heartbeatTimer_ = nullptr;
    QTimer* reconnectTimer_ = nullptr;
    QString channelProjectId_;
    std::function<bool()> reconnectPolicy_;
    QHash<QString, QString> collaborators_;
};

} // namespace dbcad
