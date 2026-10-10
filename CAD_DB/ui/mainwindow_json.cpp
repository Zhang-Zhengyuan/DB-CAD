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
    const auto decision = session.tryBeginSubmit(reason);
    if (decision.kind != CollabSession::SubmitDecision::Allow) {
        statusBar()->showMessage(decision.reason, 4000);
        return false;
    }

    const ENTITY_LIST &entities = curWindow->getEntityList();
    // ENTITY_LIST uses compact vector positions, while entityIndexToUuid is
    // keyed by the monotonic ENTITY_TREE_ITEM::index. They diverge after a
    // deletion, so translate positions explicitly before serializing.
    QHash<int, QString> serializerUuidByPosition;
    const auto &entityTreeForSerialization = curWindow->getEntityTree();
    for (int position = 0; position < static_cast<int>(entityTreeForSerialization.size()); ++position) {
        const int logicalIndex = entityTreeForSerialization[static_cast<size_t>(position)].index;
        const QString uuid = entityIndexToUuid.value(logicalIndex).trimmed();
        if (!uuid.isEmpty())
            serializerUuidByPosition.insert(position, uuid);
    }
    const QJsonObject graphObject = serializeACISEntityGraph(serializerUuidByPosition, entities);
    // Only transmit complete topology components rooted at changed stable
    // body UUIDs. The server expands these components into its canonical
    // snapshot; unchanged bodies never cross the network on this request.
    QSet<QString> changedRoots;
    for (const auto &change : pendingEntityChanges) {
        const QString id = change.uuid.trimmed();
        if (!id.isEmpty() && change.changeType != EntityChangeType::REMOVE)
            changedRoots.insert(id);
    }
    const QSet<QString> topologyTypes = {
        QStringLiteral("body_lump"),      QStringLiteral("body_wire"),       QStringLiteral("body_transform"),
        QStringLiteral("lump_shell"),     QStringLiteral("lump_next"),       QStringLiteral("lump_body"),
        QStringLiteral("shell_next"),     QStringLiteral("shell_face"),      QStringLiteral("shell_wire"),
        QStringLiteral("shell_lump"),     QStringLiteral("wire_coedge"),     QStringLiteral("wire_next"),
        QStringLiteral("wire_owner"),     QStringLiteral("face_loop"),       QStringLiteral("face_next"),
        QStringLiteral("face_shell"),     QStringLiteral("loop_face"),       QStringLiteral("loop_start"),
        QStringLiteral("loop_next"),      QStringLiteral("coedge_next"),     QStringLiteral("coedge_previous"),
        QStringLiteral("coedge_partner"), QStringLiteral("coedge_edge"),     QStringLiteral("coedge_owner"),
        QStringLiteral("edge_start"),     QStringLiteral("edge_end"),        QStringLiteral("edge_coedge"),
        QStringLiteral("vertex_edge"),    QStringLiteral("edge_vertex"),     QStringLiteral("face_geometry"),
        QStringLiteral("edge_geometry"),  QStringLiteral("vertex_geometry"), QStringLiteral("body_face"),
        QStringLiteral("body_edge"),      QStringLiteral("body_vertex")};
    QHash<QString, QSet<QString>> adjacency;
    const QJsonArray graphRels = graphObject.value("rels").toArray();
    for (const QJsonValue &value : graphRels) {
        const QJsonObject rel = value.toObject();
        const QString relType = rel.value("type").toString().toLower();
        if (!topologyTypes.contains(relType) && !relType.endsWith(QStringLiteral("_ptr")))
            continue;
        const QString start = rel.value("start").toString();
        const QString end = rel.value("end").toString();
        if (!start.isEmpty() && !end.isEmpty()) {
            adjacency[start].insert(end);
            adjacency[end].insert(start);
        }
    }
    QSet<QString> componentIds;
    QList<QString> pendingRoots = changedRoots.values();
    while (!pendingRoots.isEmpty()) {
        const QString current = pendingRoots.takeLast();
        if (componentIds.contains(current))
            continue;
        componentIds.insert(current);
        for (const QString &neighbour : adjacency.value(current))
            pendingRoots.append(neighbour);
    }
    QJsonObject deltaGraph;
    QJsonArray deltaNodes;
    for (const QJsonValue &value : graphObject.value("nodes").toArray()) {
        const QJsonObject node = value.toObject();
        if (componentIds.contains(node.value("id").toString()))
            deltaNodes.append(node);
    }
    QJsonArray deltaRels;
    for (const QJsonValue &value : graphRels) {
        const QJsonObject rel = value.toObject();
        if (componentIds.contains(rel.value("start").toString()) &&
            componentIds.contains(rel.value("end").toString())) {
            deltaRels.append(rel);
        }
    }
    deltaGraph.insert("nodes", deltaNodes);
    deltaGraph.insert("rels", deltaRels);
    for (auto it = graphObject.constBegin(); it != graphObject.constEnd(); ++it) {
        if (it.key() != QStringLiteral("nodes") && it.key() != QStringLiteral("rels")) {
            deltaGraph.insert(it.key(), it.value());
        }
    }
    const QString graphJson = QString::fromUtf8(QJsonDocument(deltaGraph).toJson(QJsonDocument::Compact));
    const QString changesJson = exportEntityChangesToJson(pendingEntityChanges);
    QJsonParseError changesError;
    const QJsonDocument changesDoc = QJsonDocument::fromJson(changesJson.toUtf8(), &changesError);
    const QJsonArray changes = changesDoc.object().value("changes").toArray();
    // Pure JSON Mode1 never sends SAT. The client serializes ACIS into the
    // entity graph; SAT is legacy fallback data and must not leak into the
    // JSON delta metadata or Neo4j snapshots.
    QJsonArray jsonOnlyChanges;
    for (const QJsonValue &value : changes) {
        QJsonObject item = value.toObject();
        item.remove("sat");
        jsonOnlyChanges.append(item);
    }
    const QString jsonOnlyChangesText =
        QString::fromUtf8(QJsonDocument(jsonOnlyChanges).toJson(QJsonDocument::Compact));
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
    const auto result = client.saveJsonDelta(session.projectId(), QString::fromStdString(fastapi_author), graphJson,
                                             jsonOnlyChangesText, removedIds,
                                             base > 0 ? std::optional<int>(base) : std::nullopt, fastapi_client_id);
    if (!result.has_value()) {
        session.rollbackSubmit();
        statusBar()->showMessage(tr("Mode1 JSON Push 失败：%1").arg(client.lastError()), 6000);
        return false;
    }

    const int version = *result;
    session.onSubmitAccepted(version);
    session.setPushedVersion(version);
    api_advance_delta_since(collabCtx);
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
        if (node.value("labels").toArray().contains(QJsonValue(QStringLiteral("body")))) {
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
    QString graphError;
    if (!pullACISEntityGraph(result->version, graphDoc.object(), QString(), result->delta, result->removedIds,
                             remoteNames)) {
        statusBar()->showMessage(tr("Mode1 JSON Pull 重建 ACIS 失败"), 6000);
        return;
    }
    session.onRemoteApplied(result->version);
    session.setPushedVersion(result->version);
    fastapi_model_version = session.modelVersion();
    fastapi_pending_remote_version = session.pendingRemoteVersion();
    updateCollabPanelUi();
    statusBar()->showMessage(tr("Mode1 JSON Pull 完成 v%1").arg(result->version), 4000);
}
