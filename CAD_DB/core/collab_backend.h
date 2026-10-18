#pragma once

// CollabBackend —— 协作传输后端（架构 v2 核心库，REFACTOR_PLAN M1.4 + M1.5 + M2.1）。
//
// 职责：
//   * HTTP：json-delta 线协议的客户端封装，返回类型化结果（含 Conflict）；
//     全部异步（回调式），传输经 HttpTransport 抽象（生产 QNAM 原生异步，
//     无嵌套事件循环；测试注入内存实现）。
//   * WS：连接管理（重连/心跳定时器、URL 构造、原始消息转发）与在场名单维护；
//     UI 反应一律通过信号交还上层。
//   * 实现 StorageBackend 接口（open/save/list/remove，同样异步回调式）。
//
// 回调约定：恰好调用一次；同步前置校验失败时同步回调。回调在 GUI 线程。
// 线协议冻结：POST/GET /projects/{id}/json-delta 与 WS 通知语义不变。

#include <functional>
#include <memory>
#include <optional>

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

    // ---------- HTTP（异步，类型化结果） ----------
    struct ProjectLookup {
        bool found = false;
        bool failed = false; // 网络或服务器错误（error 有效）
        ProjectRef project;
        QString error;
    };
    // 404 → found=false 且 failed=false；网络/其他错误 → failed=true。
    void findProject(const QString& projectName, std::function<void(const ProjectLookup&)> callback);
    // 409（已存在）→ ok=false 且 error 为空。
    struct ProjectCreate {
        bool ok = false;
        ProjectRef project;
        QString error;
    };
    void createProject(const QString& projectName, std::function<void(const ProjectCreate&)> callback);
    void getOrCreateProject(const QString& projectName, std::function<void(const ProjectLookup&)> callback);

    // 推送 json-delta；409 → status=Conflict。
    struct PushResult {
        enum class Status { Created, Conflict, Error };
        Status status = Status::Error;
        int version = 0;
        QString error;
    };
    void pushJsonDelta(const QString& projectId, const QString& author, const QString& entityGraphJson,
                       const QString& changesJson, const QStringList& removedIds, std::optional<int> baseVersion,
                       std::function<void(const PushResult&)> callback, const QString& sourceClientId = QString());

    struct FetchResult {
        enum class Status { Ok, NotFound, Error };
        Status status = Status::Error;
        JsonGraphPayload payload;
        QString error;
    };
    void fetchJsonDelta(const QString& projectId, int baseVersion,
                        std::function<void(const FetchResult&)> callback);
    void fetchJsonVersion(const QString& projectId, int version,
                          std::function<void(const FetchResult&)> callback);

    // "测试连接"：收到任何 HTTP 应答即成功（404 也算连通）。
    void testConnection(std::function<void(bool ok, const QString& error)> callback);

    // ---------- WS 通道 ----------
    // 打开（或重开）项目的实时通道；project 为空或未配置则保持关闭。
    void openChannel(const QString& projectId);
    void closeChannel();
    bool channelReady() const;
    bool channelOpening() const { return socket_ != nullptr; }
    bool hasChannelProject() const { return !channelProjectId_.isEmpty(); }
    void sendSyncNow();
    // 重连策略由上层注入（"存取后端为协作且项目非空"）；返回 false 时重连定时器停止。
    void setReconnectPolicy(std::function<bool()> policy) { reconnectPolicy_ = std::move(policy); }

    // 在场名单：presence_* 消息经 processPresenceMessage 更新。
    QHash<QString, QString> collaborators() const { return collaborators_; }
    void processPresenceMessage(const QJsonObject& root, const QString& messageType);

    // ---------- StorageBackend ----------
    QString backendName() const override { return QStringLiteral("fastapi-collab"); }
    bool isCollaborative() const override { return true; }
    void open(const QString& location, std::function<void(const StorageOpenResult&)> callback) override;
    void save(const QString& location, const SaveRequest& request,
              std::function<void(const StorageSaveResult&)> callback) override;
    void list(const QString& scope, std::function<void(const StorageListResult&)> callback) override;
    void remove(const QString& location, std::function<void(const StorageRemoveResult&)> callback) override;

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
    void sendJsonRequest(const QString& method, const QString& path, const QByteArray& payload,
                         std::function<void(const HttpCall&)> callback);
    QUrl buildChannelUrl(const QString& projectId) const;

    HttpTransport* transport_ = nullptr;
    std::unique_ptr<class QtHttpTransport> defaultTransport_; // 无注入时使用
    QString baseUrl_;
    QString author_;
    QString apiPassword_;
    QString clientId_;

    QWebSocket* socket_ = nullptr;
    QTimer* heartbeatTimer_ = nullptr;
    QTimer* reconnectTimer_ = nullptr;
    QString channelProjectId_;
    std::function<bool()> reconnectPolicy_;
    QHash<QString, QString> collaborators_;
};

} // namespace dbcad
