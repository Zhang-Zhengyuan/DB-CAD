#include "core/collab_backend.h"

#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStringConverter>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QUuid>
#include <QtWebSockets/QWebSocket>

#include <utility>

namespace dbcad {

namespace {
QString extractErrorDetail(const QByteArray& body) {
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
        const QJsonValue raw = doc.object().value("detail");
        QString detail;
        if (raw.isObject())
            detail = QString::fromUtf8(QJsonDocument(raw.toObject()).toJson(QJsonDocument::Compact));
        else if (raw.isArray())
            detail = QString::fromUtf8(QJsonDocument(raw.toArray()).toJson(QJsonDocument::Compact));
        else
            detail = raw.toString().trimmed();
        if (!detail.isEmpty()) {
            return detail;
        }
    }
    return {};
}

void appendFastApiErrorLog(const QString& method, const QString& url, int statusCode, const QString& networkError,
                           const QByteArray& body) {
    QFile file("fastapi_error.log");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << "[" << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) << "] " << method << " " << url
           << " status=" << statusCode;
    if (!networkError.trimmed().isEmpty()) {
        stream << " network_error=" << networkError;
    }
    const QString detail = extractErrorDetail(body);
    if (!detail.isEmpty()) {
        stream << " detail=" << detail;
    } else {
        QString raw = QString::fromUtf8(body).trimmed();
        if (raw.size() > 800) {
            raw = raw.left(800) + "...";
        }
        if (!raw.isEmpty()) {
            stream << " body=" << raw;
        }
    }
    stream << "\n";
}

QString formatHttpErrorMessage(const QString& prefix, int statusCode, const QByteArray& body) {
    const QString detail = extractErrorDetail(body);
    if (!detail.isEmpty()) {
        return QString::fromUtf8("%1，HTTP状态码：%2，详情：%3").arg(prefix).arg(statusCode).arg(detail);
    }
    return QString::fromUtf8("%1，HTTP状态码：%2").arg(prefix).arg(statusCode);
}

QString compactJson(const QJsonObject& object) {
    return QString::fromUtf8(QJsonDocument(object).toJson(QJsonDocument::Compact));
}
} // namespace

CollabBackend::CollabBackend(QObject* parent) : QObject(parent) {
    defaultTransport_ = std::make_unique<QtHttpTransport>();
    transport_ = defaultTransport_.get();
    clientId_ = QUuid::createUuid().toString(QUuid::WithoutBraces);

    heartbeatTimer_ = new QTimer(this);
    heartbeatTimer_->setInterval(10000);
    connect(heartbeatTimer_, &QTimer::timeout, this, [this]() {
        if (socket_ != nullptr && socket_->isValid()) {
            socket_->sendTextMessage(QStringLiteral("ping"));
        }
    });

    reconnectTimer_ = new QTimer(this);
    reconnectTimer_->setInterval(3000);
    connect(reconnectTimer_, &QTimer::timeout, this, [this]() {
        const bool policyAllows = reconnectPolicy_ ? reconnectPolicy_() : true;
        if (policyAllows && !channelProjectId_.isEmpty()) {
            openChannel(channelProjectId_);
        } else {
            reconnectTimer_->stop();
        }
    });
}

CollabBackend::~CollabBackend() = default;

void CollabBackend::setTransport(HttpTransport* transport) {
    transport_ = transport;
}

void CollabBackend::configure(const QString& baseUrl, const QString& author, const QString& apiPassword) {
    baseUrl_ = baseUrl.trimmed();
    author_ = author.trimmed();
    apiPassword_ = apiPassword.trimmed();
    while (baseUrl_.endsWith('/')) {
        baseUrl_.chop(1);
    }
}

bool CollabBackend::isConfigured() const {
    return !baseUrl_.isEmpty();
}

CollabBackend::HttpCall CollabBackend::sendJsonRequest(const QString& method, const QString& path,
                                                       const QByteArray& payload) {
    HttpCall call;
    lastHttpStatus_ = -1;

    if (baseUrl_.isEmpty()) {
        call.networkError = QString::fromUtf8("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        lastError_ = call.networkError;
        return call;
    }

    const QUrl url(baseUrl_ + path);
    if (!url.isValid()) {
        call.networkError = QString::fromUtf8("FastAPI地址无效");
        lastError_ = call.networkError;
        return call;
    }

    HttpRequest request;
    request.method = method;
    request.url = url;
    request.payload = payload;
    if (!apiPassword_.isEmpty()) {
        request.headers.append({QByteArrayLiteral("X-API-Password"), apiPassword_.toUtf8()});
    }
    const HttpResponse response = transport_->send(request);
    call.statusCode = response.statusCode;
    call.body = response.body;
    call.networkError = response.error;
    lastHttpStatus_ = response.statusCode;

    if (response.statusCode >= 400 || !response.error.isEmpty()) {
        appendFastApiErrorLog(method, url.toString(), response.statusCode, response.error, response.body);
    }
    return call;
}

CollabBackend::ProjectLookup CollabBackend::findProject(const QString& projectName) {
    ProjectLookup lookup;
    lastError_.clear();
    const QString encodedName = QString::fromUtf8(QUrl::toPercentEncoding(projectName));
    const HttpCall response = sendJsonRequest(QStringLiteral("GET"),
                                              QString::fromUtf8("/projects/by-name/%1").arg(encodedName));

    if (response.networkFailed()) {
        lookup.failed = true;
        lookup.error = response.networkError;
        lastError_ = response.networkError;
        return lookup;
    }
    if (response.statusCode == 404) {
        return lookup; // found=false, failed=false
    }
    if (response.statusCode != 200) {
        lookup.failed = true;
        lookup.error = formatHttpErrorMessage(QString::fromUtf8("查询项目失败"), response.statusCode, response.body);
        lastError_ = lookup.error;
        return lookup;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        lookup.failed = true;
        lookup.error = QString::fromUtf8("查询项目返回数据格式错误");
        lastError_ = lookup.error;
        return lookup;
    }
    const QJsonObject obj = doc.object();
    lookup.project.id = obj.value("id").toString();
    lookup.project.name = obj.value("name").toString();
    if (lookup.project.id.isEmpty() || lookup.project.name.isEmpty()) {
        lookup.failed = true;
        lookup.project = ProjectRef{};
        lookup.error = QString::fromUtf8("查询项目返回数据缺少必要字段");
        lastError_ = lookup.error;
        return lookup;
    }
    lookup.found = true;
    return lookup;
}

CollabBackend::ProjectCreate CollabBackend::createProject(const QString& projectName) {
    ProjectCreate create;
    lastError_.clear();

    QJsonObject payload;
    payload.insert("name", projectName);
    const HttpCall response = sendJsonRequest(QStringLiteral("POST"), QStringLiteral("/projects"),
                                              QJsonDocument(payload).toJson(QJsonDocument::Compact));

    if (response.networkFailed()) {
        create.error = response.networkError;
        lastError_ = response.networkError;
        return create;
    }
    if (response.statusCode == 409) {
        return create; // 已存在：ok=false 且 error 为空（与旧语义一致）
    }
    if (response.statusCode != 201) {
        create.error = formatHttpErrorMessage(QString::fromUtf8("创建项目失败"), response.statusCode, response.body);
        lastError_ = create.error;
        return create;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        create.error = QString::fromUtf8("创建项目返回数据格式错误");
        lastError_ = create.error;
        return create;
    }
    const QJsonObject obj = doc.object();
    create.project.id = obj.value("id").toString();
    create.project.name = obj.value("name").toString();
    if (create.project.id.isEmpty() || create.project.name.isEmpty()) {
        create.project = ProjectRef{};
        create.error = QString::fromUtf8("创建项目返回数据缺少必要字段");
        lastError_ = create.error;
        return create;
    }
    create.ok = true;
    return create;
}

CollabBackend::ProjectLookup CollabBackend::getOrCreateProject(const QString& projectName) {
    ProjectLookup lookup = findProject(projectName);
    if (lookup.found || lookup.failed) {
        return lookup;
    }
    const ProjectCreate create = createProject(projectName);
    if (create.ok) {
        lookup.found = true;
        lookup.project = create.project;
        return lookup;
    }
    lookup.failed = true;
    lookup.error = create.error.isEmpty() ? QString::fromUtf8("项目已存在但无法查询，请检查后端接口状态")
                                          : create.error;
    lastError_ = lookup.error;
    return lookup;
}

CollabBackend::PushResult CollabBackend::pushJsonDelta(const QString& projectId, const QString& author,
                                                       const QString& entityGraphJson, const QString& changesJson,
                                                       const QStringList& removedIds, std::optional<int> baseVersion,
                                                       const QString& sourceClientId) {
    PushResult result;
    lastError_.clear();
    QJsonParseError parseError;
    const QJsonDocument graphDoc = QJsonDocument::fromJson(entityGraphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !graphDoc.isObject()) {
        result.error = QString::fromUtf8("Entity Graph JSON 格式错误");
        lastError_ = result.error;
        return result;
    }
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || (!changesDoc.isArray() && !changesDoc.isObject())) {
        result.error = QString::fromUtf8("changes JSON 格式错误");
        lastError_ = result.error;
        return result;
    }
    QJsonArray changes;
    if (changesDoc.isArray())
        changes = changesDoc.array();
    else
        changes = changesDoc.object().value("changes").toArray();

    QJsonArray removed;
    for (const QString& id : removedIds)
        removed.append(id);
    QJsonObject payload;
    payload.insert("author", author);
    payload.insert("entity_graph", graphDoc.object());
    payload.insert("changes", changes);
    payload.insert("removed_ids", removed);
    payload.insert("base_version", baseVersion.has_value() ? QJsonValue(*baseVersion) : QJsonValue(QJsonValue::Null));
    if (!sourceClientId.isEmpty())
        payload.insert("source_client_id", sourceClientId);

    const HttpCall response = sendJsonRequest(
        QStringLiteral("POST"), QString::fromUtf8("/projects/%1/json-delta").arg(projectId),
        QJsonDocument(payload).toJson(QJsonDocument::Compact));
    if (response.networkFailed()) {
        result.error = response.networkError;
        lastError_ = response.networkError;
        return result;
    }
    if (response.statusCode == 409) {
        result.status = PushResult::Status::Conflict;
        result.error = formatHttpErrorMessage(QString::fromUtf8("保存 JSON delta 失败"), response.statusCode,
                                              response.body);
        lastError_ = result.error;
        return result;
    }
    if (response.statusCode != 201) {
        result.error = formatHttpErrorMessage(QString::fromUtf8("保存 JSON delta 失败"), response.statusCode,
                                              response.body);
        lastError_ = result.error;
        return result;
    }
    const QJsonDocument resultDoc = QJsonDocument::fromJson(response.body, &parseError);
    const int version = resultDoc.object().value("version").toInt(0);
    if (parseError.error != QJsonParseError::NoError || version <= 0) {
        result.error = QString::fromUtf8("保存 JSON delta 返回版本无效");
        lastError_ = result.error;
        return result;
    }
    result.status = PushResult::Status::Created;
    result.version = version;
    return result;
}

CollabBackend::FetchResult CollabBackend::fetchJsonDelta(const QString& projectId, int baseVersion) {
    FetchResult result;
    lastError_.clear();
    const HttpCall response = sendJsonRequest(
        QStringLiteral("GET"), QString::fromUtf8("/projects/%1/json-delta?base_version=%2").arg(projectId).arg(baseVersion));
    if (response.networkFailed()) {
        result.error = response.networkError;
        lastError_ = response.networkError;
        return result;
    }
    if (response.statusCode == 404) {
        result.status = FetchResult::Status::NotFound;
        return result;
    }
    if (response.statusCode != 200) {
        result.error = formatHttpErrorMessage(QString::fromUtf8("获取 JSON delta 失败"), response.statusCode,
                                              response.body);
        lastError_ = result.error;
        return result;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        result.error = QString::fromUtf8("获取 JSON delta 返回格式错误");
        lastError_ = result.error;
        return result;
    }
    const QJsonObject root = doc.object();
    JsonGraphPayload& payload = result.payload;
    payload.version = root.value("version").toInt(0);
    if (!root.value("entity_graph").isObject()) {
        result.error = QString::fromUtf8("获取 JSON delta 缺少实体图");
        lastError_ = result.error;
        return result;
    }
    payload.graphJson = compactJson(root.value("entity_graph").toObject());
    payload.changesJson = compactJson(root.value("changes").toObject());
    if (root.value("changes").isArray())
        payload.changesJson = QString::fromUtf8(QJsonDocument(root.value("changes").toArray()).toJson(QJsonDocument::Compact));
    for (const QJsonValue& value : root.value("removed_ids").toArray()) {
        const QString id = value.toString().trimmed();
        if (!id.isEmpty())
            payload.removedIds.append(id);
    }
    payload.delta = root.value("delta").toBool(false);
    payload.graphHash = root.value("graph_hash").toString();
    if (payload.version < 0 || payload.graphJson.isEmpty()) {
        result.error = QString::fromUtf8("获取 JSON delta 版本无效");
        lastError_ = result.error;
        return result;
    }
    result.status = FetchResult::Status::Ok;
    return result;
}

CollabBackend::FetchResult CollabBackend::fetchJsonVersion(const QString& projectId, int version) {
    FetchResult result;
    lastError_.clear();
    const HttpCall response = sendJsonRequest(QStringLiteral("GET"),
                                              QString::fromUtf8("/projects/%1/models/%2").arg(projectId).arg(version));
    if (response.networkFailed()) {
        result.error = response.networkError;
        lastError_ = response.networkError;
        return result;
    }
    if (response.statusCode == 404) {
        result.status = FetchResult::Status::NotFound;
        return result;
    }
    if (response.statusCode != 200) {
        result.error = formatHttpErrorMessage(QString::fromUtf8("读取历史版本失败"), response.statusCode, response.body);
        lastError_ = result.error;
        return result;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(response.body, &parseError);
    const QJsonObject content = doc.object().value("content").toObject();
    if (parseError.error != QJsonParseError::NoError || !content.value("entity_graph").isObject()) {
        result.error = QString::fromUtf8("历史版本缺少实体图");
        lastError_ = result.error;
        return result;
    }
    result.payload.version = doc.object().value("version").toInt();
    result.payload.graphJson = compactJson(content.value("entity_graph").toObject());
    result.payload.changesJson =
        QString::fromUtf8(QJsonDocument(content.value("changes").toArray()).toJson(QJsonDocument::Compact));
    result.status = FetchResult::Status::Ok;
    return result;
}

bool CollabBackend::testConnection(QString* error) {
    const ProjectLookup lookup = findProject(QStringLiteral("__dbcad_connection_test__"));
    if (lookup.failed) {
        if (error != nullptr)
            *error = lookup.error;
        return false;
    }
    return true;
}

// ---------- WS 通道 ----------

QUrl CollabBackend::buildChannelUrl(const QString& projectId) const {
    QString wsBaseUrl = baseUrl_.trimmed();
    while (wsBaseUrl.endsWith('/')) {
        wsBaseUrl.chop(1);
    }
    if (wsBaseUrl.startsWith("https://")) {
        wsBaseUrl.replace(0, 5, QStringLiteral("wss"));
    } else if (wsBaseUrl.startsWith("http://")) {
        wsBaseUrl.replace(0, 4, QStringLiteral("ws"));
    } else if (!wsBaseUrl.startsWith("ws://") && !wsBaseUrl.startsWith("wss://")) {
        wsBaseUrl = QStringLiteral("ws://") + wsBaseUrl;
    }

    QString wsPath = QStringLiteral("/ws/projects/") + projectId;
    QStringList queryItems;
    queryItems << QStringLiteral("client_id=") + QString::fromUtf8(QUrl::toPercentEncoding(clientId_));
    queryItems << QStringLiteral("author=") + QString::fromUtf8(QUrl::toPercentEncoding(author_));
    queryItems << QStringLiteral("stream=mode1");
    if (!apiPassword_.isEmpty()) {
        queryItems << QStringLiteral("password=") + QString::fromUtf8(QUrl::toPercentEncoding(apiPassword_));
    }
    wsPath += QStringLiteral("?") + queryItems.join(QLatin1Char('&'));
    return QUrl(wsBaseUrl + wsPath);
}

void CollabBackend::openChannel(const QString& projectId) {
    // 先关旧连接（与旧 reconnectFastAPISync 入口处 disconnectFastAPISync 一致）。
    closeChannel();
    channelProjectId_ = projectId;

    if (channelProjectId_.isEmpty() || baseUrl_.isEmpty()) {
        emit channelStateChanged(QStringLiteral("NotConnected"));
        return;
    }

    emit channelStateChanged(QStringLiteral("Connecting"));

    const QUrl wsUrl = buildChannelUrl(channelProjectId_);
    socket_ = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);

    connect(socket_, &QWebSocket::textMessageReceived, this, &CollabBackend::textMessageReceived);
    connect(socket_, &QWebSocket::connected, this, [this]() {
        if (reconnectTimer_ != nullptr) {
            reconnectTimer_->stop();
        }
        // Git 语义：重连后不主动 sync_now，避免一连接就把远端的 entity_graph_saved
        // 自动 apply 下来。远端历史版本等待用户主动 Pull。
        if (heartbeatTimer_ != nullptr) {
            heartbeatTimer_->start();
        }
        emit channelConnected();
        emit channelStateChanged(QStringLiteral("Connected"));
    });
    // 捕获"创建时那一把 socket"的指针，避免旧 socket 在 close 时被 deleteLater
    // 触发的 disconnected 事件误启动重连定时器 / 误报"已断开"。
    QWebSocket* const createdSocket = socket_;
    connect(socket_, &QWebSocket::disconnected, this, [this, createdSocket]() {
        if (createdSocket != socket_) {
            return; // stale socket 的关闭事件
        }
        if (heartbeatTimer_ != nullptr) {
            heartbeatTimer_->stop();
        }
        if (!channelProjectId_.isEmpty()) {
            emit channelStateChanged(QStringLiteral("Reconnecting"));
            if (reconnectTimer_ != nullptr && !reconnectTimer_->isActive()) {
                reconnectTimer_->start();
            }
        } else {
            emit channelStateChanged(QStringLiteral("NotConnected"));
        }
        emit channelDisconnected();
    });
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred), this,
            [this](QAbstractSocket::SocketError) {
                emit channelError(socket_ ? socket_->errorString() : QString());
            });

    socket_->open(wsUrl);
}

void CollabBackend::closeChannel() {
    if (heartbeatTimer_ != nullptr) {
        heartbeatTimer_->stop();
    }
    collaborators_.clear();
    if (reconnectTimer_ != nullptr) {
        reconnectTimer_->stop();
    }
    channelProjectId_.clear();
    if (socket_ != nullptr) {
        socket_->disconnect(this);
        socket_->close();
        socket_->deleteLater();
        socket_ = nullptr;
    }
}

bool CollabBackend::channelReady() const {
    return socket_ != nullptr && socket_->isValid();
}

void CollabBackend::sendSyncNow() {
    if (channelReady()) {
        socket_->sendTextMessage(QStringLiteral("sync_now"));
    }
}

void CollabBackend::processPresenceMessage(const QJsonObject& root, const QString& messageType) {
    if (messageType == QStringLiteral("presence_snapshot")) {
        collaborators_.clear();
        const QJsonArray members = root.value("members").toArray();
        for (const auto& memberValue : members) {
            if (!memberValue.isObject()) {
                continue;
            }
            const QJsonObject member = memberValue.toObject();
            const QString memberClientId = member.value("client_id").toString();
            if (memberClientId.isEmpty()) {
                continue;
            }
            collaborators_.insert(memberClientId, member.value("author").toString());
        }
        emit collaboratorsChanged();
        return;
    }
    if (messageType == QStringLiteral("collaborator_joined")) {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            collaborators_.insert(memberClientId, root.value("author").toString());
            emit collaboratorsChanged();
        }
        return;
    }
    if (messageType == QStringLiteral("collaborator_left")) {
        const QString memberClientId = root.value("client_id").toString();
        if (!memberClientId.isEmpty()) {
            collaborators_.remove(memberClientId);
            emit collaboratorsChanged();
        }
        return;
    }
}

// ---------- StorageBackend ----------

std::optional<StorageDocument> CollabBackend::open(const QString& location, QString* error) {
    const ProjectLookup lookup = findProject(location);
    if (lookup.failed) {
        if (error != nullptr)
            *error = lookup.error.isEmpty() ? QString::fromUtf8("查询协作项目失败") : lookup.error;
        return std::nullopt;
    }
    if (!lookup.found) {
        if (error != nullptr)
            *error = QString::fromUtf8("名为%1的项目(FastAPI)不存在。").arg(location);
        return std::nullopt;
    }
    const FetchResult fetch = fetchJsonDelta(lookup.project.id, 0);
    if (fetch.status != FetchResult::Status::Ok) {
        if (error != nullptr)
            *error = QString::fromUtf8("读取 Mode1 JSON 项目失败：%1").arg(fetch.error);
        return std::nullopt;
    }
    StorageDocument document;
    document.location = location;
    document.projectId = lookup.project.id;
    document.version = fetch.payload.version;
    QJsonParseError parseError;
    const QJsonDocument graphDoc = QJsonDocument::fromJson(fetch.payload.graphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !graphDoc.isObject()) {
        if (error != nullptr)
            *error = QString::fromUtf8("获取 JSON delta 返回格式错误");
        return std::nullopt;
    }
    document.graph = graphDoc.object();
    document.removedIds = fetch.payload.removedIds;
    return document;
}

std::optional<int> CollabBackend::save(const QString& location, const SaveRequest& request, QString* error) {
    ProjectRef project;
    const ProjectLookup lookup = findProject(location);
    if (lookup.failed) {
        if (error != nullptr)
            *error = lookup.error;
        return std::nullopt;
    }
    if (lookup.found) {
        // 保存到当前打开的项目允许已有版本；换名保存要求目标为空项目。
        if (lookup.project.id != channelProjectId_) {
            const FetchResult head = fetchJsonDelta(lookup.project.id, 0);
            if (head.status != FetchResult::Status::Ok) {
                if (error != nullptr)
                    *error = head.error;
                return std::nullopt;
            }
            if (head.payload.version > 0) {
                if (error != nullptr)
                    *error = QString::fromUtf8("目标项目已有版本，请先打开并 Pull，或使用新的项目名");
                return std::nullopt;
            }
        }
        project = lookup.project;
    } else {
        const ProjectCreate create = createProject(location);
        if (!create.ok) {
            if (error != nullptr)
                *error = create.error;
            return std::nullopt;
        }
        project = create.project;
    }

    const PushResult push = pushJsonDelta(project.id, request.author.isEmpty() ? author_ : request.author,
                                          compactJson(request.deltaGraph), request.changesJson, request.removedIds,
                                          request.baseVersion, request.sourceClientId);
    if (push.status != PushResult::Status::Created) {
        if (error != nullptr)
            *error = push.error;
        return std::nullopt;
    }
    return push.version;
}

QStringList CollabBackend::list(const QString& scope, QString* error) {
    Q_UNUSED(scope);
    if (error != nullptr)
        *error = QString::fromUtf8("服务端未提供项目列表接口");
    return {};
}

bool CollabBackend::remove(const QString& location, QString* error) {
    Q_UNUSED(location);
    if (error != nullptr)
        *error = QString::fromUtf8("服务端未提供项目删除接口");
    return false;
}

} // namespace dbcad
