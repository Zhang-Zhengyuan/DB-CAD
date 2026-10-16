#include <QScopeGuard>
#include "mainwindow_internal.h"

bool MainWindow::submitMode1JsonDelta(const QString &reason) {
    auto &session = CollabSession::instance();
    if (curWindow == nullptr || session.projectId().isEmpty() || !session.isConnected()) {
        statusBar()->showMessage(tr("协作通道未连接"), 4000);
        return false;
    }
    if (pendingEntityChanges.isEmpty()) {
        statusBar()->showMessage(tr("没有未推送的本地修改"), 2000);
        return true;
    }
    QHash<QString, EntityChange> lastChange;
    for (const auto &change : pendingEntityChanges)
        lastChange.insert(change.uuid, change);
    const QList<EntityChange> submittedChanges = lastChange.values();
    const auto decision = session.tryBeginSubmit(reason);
    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        statusBar()->showMessage(decision.reason, 4000);
        return false;
    }

    // Serialize only changed top-level entities. Stable UUIDs are keyed by
    // compact positions in this list, not by the canvas' monotonic indices.
    QSet<QString> changedRoots;
    for (const auto &change : submittedChanges)
        if (change.changeType != EntityChangeType::REMOVE)
            changedRoots.insert(change.uuid);
    ENTITY_LIST entities;
    QHash<int, QString> serializerUuidByPosition;
    for (const auto &item : curWindow->getEntityTree()) {
        const QString uuid = QString::fromStdString(item.uuid);
        if (!changedRoots.contains(uuid))
            continue;
        serializerUuidByPosition.insert(entities.count(), uuid);
        entities.add(item.ptrEntity);
    }
    QJsonObject deltaGraph;
    try {
        deltaGraph = serializeACISEntityGraph(serializerUuidByPosition, entities);
    } catch (const std::exception &ex) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("序列化失败：%1").arg(QString::fromUtf8(ex.what())), 6000);
        return false;
    }
    QHash<QString, QString> rootNames;
    for (const auto &item : curWindow->getEntityTree())
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
    const QString changesJson = exportEntityChangesToJson(submittedChanges);
    QJsonParseError changesError;
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8(), &changesError);
    const QJsonArray changes = changesDoc.object().value("changes").toArray();
    const QString jsonOnlyChangesText = QString::fromUtf8(QJsonDocument(changes).toJson(QJsonDocument::Compact));
    QStringList removedIds;
    for (const QJsonValue &value : changes) {
        const QJsonObject change = value.toObject();
        if (change.value("changeType").toString() == QStringLiteral("REMOVE")) {
            const QString uuid = change.value("uuid").toString().trimmed();
            if (!uuid.isEmpty())
                removedIds.append(uuid);
        }
    }

    BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                            QString::fromStdString(fastapi_password));
    const int base = session.modelVersion();
    const bool wasEnabled = isEnabled();
    setEnabled(false);
    const auto result = client.saveJsonDelta(session.projectId(), QString::fromStdString(fastapi_author), graphJson,
                                             jsonOnlyChangesText, removedIds,
                                             base > 0 ? std::optional<int>(base) : std::nullopt, fastapi_client_id);
    setEnabled(wasEnabled);
    if (!result.has_value()) {
        session.rollbackSubmit();
        if (client.lastStatusCode() == 409)
            requestFastAPISyncNow();
        statusBar()->showMessage(tr("Mode1 JSON Push 失败：%1").arg(client.lastError()), 6000);
        return false;
    }

    const int version = *result;
    session.onSubmitAccepted(version);
    session.setPushedVersion(version);
    pendingEntityChanges.clear();
    if (curWindow != nullptr)
        curWindow->setIsModified(false);
    fastapi_model_version = session.modelVersion();
    fastapi_pending_remote_version = session.pendingRemoteVersion();
    updateCollabPanelUi();
    statusBar()->showMessage(tr("Mode1 JSON Push 成功 v%1").arg(version), 4000);
    return true;
}

void MainWindow::pullMode1JsonDelta() {
    auto &session = CollabSession::instance();
    if (curWindow == nullptr || session.projectId().isEmpty() || !session.isConnected()) {
        statusBar()->showMessage(tr("协作通道未连接"), 4000);
        return;
    }
    const int base = session.modelVersion();
    BackendApiClient client(QString::fromStdString(fastapi_base_url), QString::fromStdString(fastapi_author),
                            QString::fromStdString(fastapi_password));
    // BackendApiClient runs a nested Qt event loop. Prevent edits and repeated
    // Pull clicks until the response has been merged into this same document.
    const bool wasEnabled = isEnabled();
    setEnabled(false);
    const auto restoreUi = qScopeGuard([this, wasEnabled]() { setEnabled(wasEnabled); });
    const auto result = client.getJsonDelta(session.projectId(), base);
    if (!result.has_value()) {
        statusBar()->showMessage(tr("Mode1 JSON Pull 失败：%1").arg(client.lastError()), 6000);
        return;
    }
    if (result->version <= base) {
        statusBar()->showMessage(tr("Mode1 JSON 已是最新版本 v%1").arg(result->version), 3000);
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument graphDoc = QJsonDocument::fromJson(result->graphJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !graphDoc.isObject()) {
        statusBar()->showMessage(tr("Mode1 JSON Pull 的图格式无效"), 6000);
        return;
    }
    QSet<QString> remoteChangedIds;
    for (const QJsonValue &value : graphDoc.object().value("nodes").toArray()) {
        const QJsonObject node = value.toObject();
        if (node.value("labels").toArray().contains(QJsonValue(QStringLiteral("body"))) ||
            node.value("props").toObject().value("top_level").toBool()) {
            const QString id = node.value("id").toString().trimmed();
            if (!id.isEmpty())
                remoteChangedIds.insert(id);
        }
    }
    for (const QString &id : result->removedIds)
        remoteChangedIds.insert(id);
    QHash<QString, QString> remoteNames;
    QJsonParseError changesParseError;
    const QJsonDocument changesDoc = QJsonDocument::fromJson(result->changesJson.toUtf8(), &changesParseError);
    QJsonArray remoteChanges;
    if (changesParseError.error == QJsonParseError::NoError) {
        remoteChanges = changesDoc.isArray() ? changesDoc.array() : changesDoc.object().value("changes").toArray();
    }
    for (const QJsonValue &value : remoteChanges) {
        const QJsonObject change = value.toObject();
        const QString uuid = change.value("uuid").toString().trimmed();
        const QString name = change.value("name").toString().trimmed();
        if (!uuid.isEmpty() && !name.isEmpty())
            remoteNames.insert(uuid, name);
    }
    for (const auto &localChange : pendingEntityChanges) {
        if (remoteChangedIds.contains(localChange.uuid)) {
            statusBar()->showMessage(tr("Pull 与本地未提交修改冲突：实体 %1，请先处理冲突").arg(localChange.uuid),
                                     6000);
            return;
        }
    }
    if (!pullACISEntityGraph(result->version, graphDoc.object(), true, result->removedIds, remoteNames)) {
        statusBar()->showMessage(tr("Mode1 JSON Pull 重建 ACIS 失败"), 6000);
        return;
    }
    session.onRemoteApplied(result->version);
    session.setPushedVersion(result->version);
    if (!pendingEntityChanges.isEmpty())
        session.onUserEdit();
    curWindow->setIsModified(!pendingEntityChanges.isEmpty());
    fastapi_model_version = session.modelVersion();
    fastapi_pending_remote_version = session.pendingRemoteVersion();
    updateCollabPanelUi();
    statusBar()->showMessage(tr("Mode1 JSON Pull 完成 v%1").arg(result->version), 4000);
}
