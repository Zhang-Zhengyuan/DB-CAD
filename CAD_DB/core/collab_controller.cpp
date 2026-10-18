#include "core/collab_controller.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSet>
#include <QTimer>

#include <algorithm>

#include "core/document_session.h"

namespace dbcad {

CollabController::CollabController(DocumentSession& session, CollabBackend& backend, QObject* parent)
    : QObject(parent), session_(session), backend_(backend) {}

void CollabController::emitInfo(const QString& message, int duration) {
    emit infoMessage(message, duration);
}

// ---------- 项目绑定 ----------

void CollabController::bindProject(const QString& projectId, const QString& projectName) {
    state_.setProjectId(projectId);
    projectName_ = projectName;
    emit stateChanged();
}

void CollabController::leaveProject() {
    disconnectChannel();
    state_.setProjectId(QString());
    state_.setModelVersion(0);
    projectName_.clear();
    emit stateChanged();
}

// ---------- 连接编排 ----------

void CollabController::connectProject() {
    // 先断开：状态机复位 + 关旧 socket；真正转 Connected 发生在 WS 连上时
    // （channelConnected → onReconnect，由视图接线）。
    disconnectChannel();
    backend_.openChannel(state_.projectId());
}

void CollabController::disconnectChannel() {
    state_.onDisconnected();
    backend_.closeChannel();
    emit stateChanged();
}

void CollabController::onChannelConnected() {
    // onReconnect 把状态机转 Connected_Idle，确保 Pull 触发 sync_now 应答后
    // tryBeginSubmit 不会因 state == Disconnected 被拒。
    state_.onReconnect(!state_.projectId().isEmpty());
    emit stateChanged();
}

// ---------- 本地编辑策略（原 notifyModelChangedForCollaboration） ----------

void CollabController::onLocalEdit() {
    if (session_.applyingRemoteSnapshot()) {
        return;
    }
    if (state_.projectId().isEmpty() || projectName_.isEmpty()) {
        return;
    }
    // 本地编辑只改变协作状态；用户经协作面板显式推送，这里绝不在编辑操作内发起网络请求。
    if (state_.pendingRemoteVersion() > state_.modelVersion()) {
        emitInfo(QString::fromUtf8("远端有未拉取版本，请先点击「拉取(Pull)」再推送本地修改"), 4000);
        state_.onRemotePending(state_.pendingRemoteVersion());
    }
    if (state_.isSubmitInFlight()) {
        state_.setLastPublishReason(QStringLiteral("local-change"));
        state_.onUserEditDuringInFlight();
    } else {
        state_.onUserEdit();
    }
    emit stateChanged();
}

// ---------- WS 通知策略（原 handleDeltaSyncEvent） ----------

void CollabController::onRemoteVersionNotified(int remoteVersion) {
    // 永远不在 WS 通知里直接应用远端图；Pull 经 HTTP 只取变更的拓扑组件。
    if (remoteVersion > state_.modelVersion()) {
        state_.onRemotePending(remoteVersion);
        emitInfo(QString::fromUtf8("远端有 JSON 协作版本 v%1，请点击 Pull 合并").arg(remoteVersion), 5000);
        emit stateChanged();
    }
}

bool CollabController::canPullNow(QString* reason) const {
    if (state_.isSubmitInFlight()) {
        if (reason != nullptr)
            *reason = QString::fromUtf8("正在提交，请稍后拉取");
        return false;
    }
    return true;
}

// ---------- 推送（原 submitMode1JsonDelta） ----------

CollabController::Outcome CollabController::push(const QString& reason) {
    Outcome outcome;
    if (state_.projectId().isEmpty() || !state_.isConnected()) {
        outcome.message = QString::fromUtf8("协作通道未连接");
        return outcome;
    }
    if (session_.pendingChanges().isEmpty()) {
        outcome.ok = true;
        outcome.nothingToDo = true;
        outcome.messageDuration = 2000;
        outcome.message = QString::fromUtf8("没有未推送的本地修改");
        return outcome;
    }
    // 同一实体多次变更只保留最后一笔；已不在树上的实体按 REMOVE 结算。
    QHash<QString, EntityChange> lastChange;
    for (const auto& change : session_.pendingChanges())
        lastChange.insert(change.uuid, change);
    QSet<QString> liveIds;
    for (const auto& item : session_.entities())
        liveIds.insert(QString::fromStdString(item.uuid));
    for (auto it = lastChange.begin(); it != lastChange.end(); ++it) {
        if (!liveIds.contains(it.key()))
            it->changeType = EntityChangeType::REMOVE;
    }
    const QList<EntityChange> submittedChanges = lastChange.values();
    const auto decision = state_.tryBeginSubmit(reason);
    if (decision.kind != CollabState::SubmitDecision::Allow) {
        outcome.message = decision.reason;
        return outcome;
    }

    // 只序列化发生变更的顶级实体。稳定 UUID 按紧凑位置作键，而不是画布的单调索引。
    QSet<QString> changedRoots;
    for (const auto& change : submittedChanges) {
        if (change.changeType != EntityChangeType::REMOVE)
            changedRoots.insert(change.uuid);
    }
    ShapeList entities;
    QHash<int, QString> serializerUuidByPosition;
    for (const auto& item : session_.entities()) {
        const QString uuid = QString::fromStdString(item.uuid);
        if (!changedRoots.contains(uuid))
            continue;
        serializerUuidByPosition.insert(static_cast<int>(entities.size()), uuid);
        entities.push_back(item.shape);
    }
    QJsonObject deltaGraph;
    try {
        deltaGraph = session_.kernel().encodeGraph(serializerUuidByPosition, entities);
    } catch (const std::exception& ex) {
        state_.rollbackSubmit();
        outcome.message = QString::fromUtf8("序列化失败：%1").arg(QString::fromUtf8(ex.what()));
        return outcome;
    }
    QHash<QString, QString> rootNames;
    for (const auto& item : session_.entities())
        rootNames.insert(QString::fromStdString(item.uuid), QString::fromStdString(item.name));
    QJsonArray namedNodes = deltaGraph.value("nodes").toArray();
    for (int i = 0; i < namedNodes.size(); ++i) {
        auto node = namedNodes[i].toObject();
        const QString id = node.value("id").toString();
        if (!rootNames.contains(id))
            continue;
        auto props = node.value("props").toObject();
        props.insert("display_name", rootNames.value(id));
        node.insert("props", props);
        namedNodes[i] = node;
    }
    deltaGraph.insert("nodes", namedNodes);
    const QString graphJson = QString::fromUtf8(QJsonDocument(deltaGraph).toJson(QJsonDocument::Compact));
    const QString changesJson = session_.exportEntityChangesJson(submittedChanges);
    QJsonParseError changesError;
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8(), &changesError);
    const QJsonArray changes = changesDoc.object().value("changes").toArray();
    const QString jsonOnlyChangesText = QString::fromUtf8(QJsonDocument(changes).toJson(QJsonDocument::Compact));
    QStringList removedIds;
    for (const QJsonValue& value : changes) {
        const QJsonObject change = value.toObject();
        if (change.value("changeType").toString() == QStringLiteral("REMOVE")) {
            const QString uuid = change.value("uuid").toString().trimmed();
            if (!uuid.isEmpty())
                removedIds.append(uuid);
        }
    }

    const int base = state_.modelVersion();
    const auto result = backend_.pushJsonDelta(state_.projectId(), backend_.author(), graphJson, jsonOnlyChangesText,
                                               removedIds, base > 0 ? std::optional<int>(base) : std::nullopt,
                                               backend_.clientId());
    if (result.status != CollabBackend::PushResult::Status::Created) {
        state_.rollbackSubmit();
        if (result.status == CollabBackend::PushResult::Status::Conflict) {
            outcome.conflict = true;
            // 409：对端先推了，立刻刷新远端版本通知，引导用户走 Pull。
            backend_.sendSyncNow();
        }
        outcome.messageDuration = 6000;
        outcome.message = QString::fromUtf8("Mode1 JSON Push 失败：%1").arg(result.error);
        return outcome;
    }

    state_.onSubmitAccepted(result.version);
    state_.setPushedVersion(result.version);
    session_.discardPendingChanges();
    session_.setDirty(false);
    outcome.ok = true;
    outcome.version = result.version;
    outcome.message = QString::fromUtf8("Mode1 JSON Push 成功 v%1").arg(result.version);
    emit stateChanged();
    return outcome;
}

// ---------- 拉取（原 pullMode1JsonDelta） ----------

CollabController::Outcome CollabController::pull() {
    Outcome outcome;
    if (state_.projectId().isEmpty() || !state_.isConnected()) {
        outcome.message = QString::fromUtf8("协作通道未连接");
        return outcome;
    }
    const int base = state_.modelVersion();
    const auto result = backend_.fetchJsonDelta(state_.projectId(), base);
    if (result.status != CollabBackend::FetchResult::Status::Ok) {
        outcome.messageDuration = 6000;
        outcome.message = QString::fromUtf8("Mode1 JSON Pull 失败：%1").arg(result.error);
        return outcome;
    }
    if (result.payload.version <= base) {
        outcome.ok = true;
        outcome.nothingToDo = true;
        outcome.version = result.payload.version;
        outcome.messageDuration = 3000;
        outcome.message = QString::fromUtf8("Mode1 JSON 已是最新版本 v%1").arg(result.payload.version);
        return outcome;
    }

    QJsonParseError parseError;
    const QJsonDocument graphDoc = QJsonDocument::fromJson(result.payload.graphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !graphDoc.isObject()) {
        outcome.message = QString::fromUtf8("Mode1 JSON Pull 的图格式无效");
        return outcome;
    }
    QSet<QString> remoteChangedIds;
    for (const QJsonValue& value : graphDoc.object().value("nodes").toArray()) {
        const QJsonObject node = value.toObject();
        if (node.value("labels").toArray().contains(QJsonValue(QStringLiteral("body"))) ||
            node.value("props").toObject().value("top_level").toBool()) {
            const QString id = node.value("id").toString().trimmed();
            if (!id.isEmpty())
                remoteChangedIds.insert(id);
        }
    }
    for (const QString& id : result.payload.removedIds)
        remoteChangedIds.insert(id);
    QHash<QString, QString> remoteNames;
    QJsonParseError changesParseError;
    const QJsonDocument changesDoc = QJsonDocument::fromJson(result.payload.changesJson.toUtf8(), &changesParseError);
    QJsonArray remoteChanges;
    if (changesParseError.error == QJsonParseError::NoError) {
        remoteChanges =
            changesDoc.isArray() ? changesDoc.array() : changesDoc.object().value("changes").toArray();
    }
    for (const QJsonValue& value : remoteChanges) {
        const QJsonObject change = value.toObject();
        const QString uuid = change.value("uuid").toString().trimmed();
        const QString name = change.value("name").toString().trimmed();
        if (!uuid.isEmpty() && !name.isEmpty())
            remoteNames.insert(uuid, name);
    }
    // 诚实拒绝：同组件并发修改不做拓扑级自动合并。
    for (const auto& localChange : session_.pendingChanges()) {
        if (remoteChangedIds.contains(localChange.uuid)) {
            outcome.conflict = true;
            outcome.message =
                QString::fromUtf8("Pull 与本地未提交修改冲突：实体 %1，请先处理冲突").arg(localChange.uuid);
            return outcome;
        }
    }
    const auto apply = session_.applyRemoteGraph(result.payload.version, graphDoc.object(), true,
                                                 result.payload.removedIds, remoteNames);
    outcome.retiredDisplayData = apply.retiredDisplayData;
    if (!apply.ok) {
        outcome.message = QString::fromUtf8("Mode1 JSON Pull 重建 ACIS 失败");
        return outcome;
    }
    state_.onRemoteApplied(result.payload.version);
    state_.setPushedVersion(result.payload.version);
    if (!session_.pendingChanges().isEmpty())
        state_.onUserEdit();
    session_.setDirty(!session_.pendingChanges().isEmpty());
    outcome.ok = true;
    outcome.version = result.payload.version;
    outcome.message = QString::fromUtf8("Mode1 JSON Pull 完成 v%1").arg(result.payload.version);
    emit stateChanged();
    return outcome;
}

// ---------- 打开/另存 ----------

CollabController::Outcome CollabController::openProject(const QString& projectName) {
    Outcome outcome;
    outcome.showDialog = true;
    if (!backend_.isConfigured()) {
        outcome.message = QString::fromUtf8("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        return outcome;
    }
    const auto lookup = backend_.findProject(projectName);
    if (lookup.failed) {
        outcome.message = lookup.error.isEmpty() ? QString::fromUtf8("查询协作项目失败") : lookup.error;
        return outcome;
    }
    if (!lookup.found) {
        const auto created = backend_.createProject(projectName);
        if (!created.ok) {
            outcome.message = QString::fromUtf8("创建协作项目失败：%1").arg(created.error);
            return outcome;
        }
        outcome.retiredDisplayData = session_.clearAll();
        // clearAll 记了 REMOVE 账；新项目从零开始，丢弃这些记录并清映射。
        session_.discardPendingChanges();
        session_.clearIndexToUuid();
        bindProject(created.project.id, projectName);
        connectProject();
        state_.setModelVersion(0);
        outcome.ok = true;
        outcome.showDialog = false;
        outcome.message = QString::fromUtf8("已创建并加入协作项目（当前无版本，请先保存模型）");
        return outcome;
    }
    const auto fetch = backend_.fetchJsonDelta(lookup.project.id, 0);
    if (fetch.status != CollabBackend::FetchResult::Status::Ok) {
        outcome.message = QString::fromUtf8("读取 Mode1 JSON 项目失败：%1").arg(fetch.error);
        return outcome;
    }
    bool restored = true;
    QJsonObject graphObject;
    if (fetch.payload.version > 0) {
        QJsonParseError graphError;
        const QJsonDocument graphDoc = QJsonDocument::fromJson(fetch.payload.graphJson.toUtf8(), &graphError);
        restored = graphError.error == QJsonParseError::NoError && graphDoc.isObject();
        if (restored) {
            graphObject = graphDoc.object();
            if (!graphObject.value("nodes").toArray().isEmpty()) {
                const auto apply =
                    session_.applyRemoteGraph(fetch.payload.version, graphObject, false, fetch.payload.removedIds, {});
                outcome.retiredDisplayData = apply.retiredDisplayData;
                restored = apply.ok;
            } else {
                // 显式为空的最新快照是一个合法项目。
                outcome.retiredDisplayData = session_.clearAll();
            }
        }
    } else {
        outcome.retiredDisplayData = session_.clearAll();
    }
    if (!restored) {
        outcome.message = QString::fromUtf8("Mode1 JSON 图无法重建，请检查序列化器兼容性");
        return outcome;
    }
    session_.resetChangesFromTree();
    bindProject(lookup.project.id, projectName);
    state_.setModelVersion(fetch.payload.version);
    connectProject();
    outcome.ok = true;
    outcome.showDialog = false;
    outcome.version = fetch.payload.version;
    return outcome;
}

CollabController::Outcome CollabController::openProjectAt(const QString& projectName, int generation) {
    Outcome outcome;
    outcome.showDialog = true;
    {
        outcome.retiredDisplayData = session_.clearAll();
        session_.discardPendingChanges();
        session_.clearIndexToUuid();
    }
    if (!backend_.isConfigured()) {
        outcome.message = QString::fromUtf8("FastAPI地址未配置，请先设置fastapi_connect_info.conf");
        return outcome;
    }
    const auto lookup = backend_.findProject(projectName);
    if (lookup.failed) {
        outcome.message = lookup.error;
        return outcome;
    }
    if (!lookup.found) {
        outcome.message = QString::fromUtf8("名为%1的项目(FastAPI)不存在。").arg(projectName);
        return outcome;
    }
    const auto model = backend_.fetchJsonVersion(lookup.project.id, generation);
    if (model.status == CollabBackend::FetchResult::Status::NotFound) {
        outcome.message = QString::fromUtf8("协作项目存在，但指定版本不存在。");
        return outcome;
    }
    if (model.status != CollabBackend::FetchResult::Status::Ok) {
        outcome.message = model.error;
        return outcome;
    }
    QJsonParseError parseError;
    const auto doc = QJsonDocument::fromJson(model.payload.graphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        outcome.message = QString::fromUtf8("历史版本无法重建");
        return outcome;
    }
    const auto apply = session_.applyRemoteGraph(model.payload.version, doc.object(), false, {}, {});
    outcome.retiredDisplayData.insert(outcome.retiredDisplayData.end(), apply.retiredDisplayData.begin(),
                                      apply.retiredDisplayData.end());
    if (!apply.ok) {
        outcome.message = QString::fromUtf8("历史版本无法重建");
        return outcome;
    }
    bindProject(lookup.project.id, projectName);
    connectProject();
    state_.setModelVersion(model.payload.version);
    outcome.ok = true;
    outcome.showDialog = false;
    outcome.version = model.payload.version;
    return outcome;
}

CollabController::Outcome CollabController::saveToProject(const QString& projectName) {
    Outcome outcome;
    if (projectName != projectName_ || state_.projectId().isEmpty()) {
        const auto lookup = backend_.getOrCreateProject(projectName);
        if (!lookup.found) {
            outcome.message = lookup.error;
            return outcome;
        }
        const auto head = backend_.fetchJsonDelta(lookup.project.id, 0);
        if (head.status != CollabBackend::FetchResult::Status::Ok) {
            outcome.message = head.error;
            return outcome;
        }
        if (head.payload.version > 0) {
            outcome.message = QString::fromUtf8("目标项目已有版本，请先打开并 Pull，或使用新的项目名");
            return outcome;
        }
        disconnectChannel();
        bindProject(lookup.project.id, projectName);
        state_.setModelVersion(0);
        // 新项目收录全部本地 Body，包括先前已保存过的。
        session_.discardPendingChanges();
        for (const auto& item : session_.entities()) {
            session_.recordEntityAdded(QString::fromStdString(item.uuid), QString::fromStdString(item.name),
                                       QStringLiteral("body"), item.index);
        }
        connectProject();
    }
    // M2 将删除这个嵌套事件循环：等待 WS 真正连上（最多 5 秒）。
    if (!state_.isConnected() && backend_.channelOpening()) {
        QEventLoop wait;
        QTimer timeout;
        timeout.setSingleShot(true);
        QObject::connect(&timeout, &QTimer::timeout, &wait, &QEventLoop::quit);
        QObject::connect(&backend_, &CollabBackend::channelConnected, &wait, &QEventLoop::quit);
        QObject::connect(&backend_, &CollabBackend::channelError, &wait, &QEventLoop::quit);
        timeout.start(5000);
        wait.exec();
    }
    if (!state_.isConnected()) {
        outcome.message = QString::fromUtf8("协作通道正在连接，请连接成功后再次保存");
        return outcome;
    }
    return push(QStringLiteral("save"));
}

} // namespace dbcad
