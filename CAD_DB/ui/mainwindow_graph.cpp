#include "mainwindow_internal.h"
#include <QScopeGuard>

// Reconstruct every incoming component before changing the visible document.
// The decoder chooses fine JSON or the matching SAT backup per stable UUID.
bool MainWindow::pullACISEntityGraph(int version, const QJsonObject &graph, bool deltaGraph,
                                     const QStringList &removedIds, const QHash<QString, QString> &remoteNames) {
    if (!curWindow || fastapi_project_id.isEmpty())
        return false;
    DeserializedEntityGraph decoded{nullptr, {}};
    QString error;
    if (!graph.value("nodes").toArray().isEmpty() && !deserializeACISEntityGraph(graph, &decoded, &error)) {
        qWarning() << "[Collab] reconstruction failed:" << error;
        return false;
    }
    QHash<QString, QString> names = remoteNames;
    for (const auto &value : graph.value("nodes").toArray()) {
        const auto node = value.toObject();
        const QString name = node.value("props").toObject().value("display_name").toString();
        if (!name.isEmpty())
            names.insert(node.value("id").toString(), name);
    }
    auto &session = CollabSession::instance();
    const bool localDirty = curWindow->getIsModified();
    session.setApplyingRemoteSnapshot(true);
    const auto restoreApply = qScopeGuard([&session]() { session.setApplyingRemoteSnapshot(false); });
    auto &tree = curWindow->getEntityTree();
    QSet<QString> incoming;
    for (auto it = decoded.uuidToEntity.constBegin(); it != decoded.uuidToEntity.constEnd(); ++it)
        incoming.insert(it.key());
    QList<ENTITY *> replaced;
    // Keep indices, visibility, dependencies and display settings on updates.
    for (int i = static_cast<int>(tree.size()) - 1; i >= 0; --i) {
        auto &item = tree[static_cast<size_t>(i)];
        const QString uuid = QString::fromStdString(item.uuid);
        if (incoming.contains(uuid)) {
            replaced.append(item.ptrEntity);
            item.ptrEntity = static_cast<ENTITY *>(decoded.uuidToEntity.value(uuid));
            delete item.ptrDisplayData;
            item.ptrDisplayData = nullptr;
            if (names.contains(uuid))
                item.name = names.value(uuid).toStdString();
            entityIndexToUuid.insert(item.index, uuid);
            incoming.remove(uuid);
        } else if (removedIds.contains(uuid) || (!deltaGraph && !decoded.uuidToEntity.contains(uuid))) {
            replaced.append(item.ptrEntity);
            delete item.ptrDisplayData;
            entityIndexToUuid.remove(item.index);
            tree.erase(tree.begin() + i);
        }
    }
    for (const auto &uuid : incoming) {
        ENTITY_TREE_ITEM item;
        item.index = curWindow->latest_index++;
        item.uuid = uuid.toStdString();
        item.name = names.value(uuid, tr("远端实体")).toStdString();
        item.ptrEntity = static_cast<ENTITY *>(decoded.uuidToEntity.value(uuid));
        item.visible = true;
        item.subOperatorType = -1;
        tree.push_back(item);
        entityIndexToUuid.insert(item.index, uuid);
    }
    QSet<int> validIndices;
    for (const auto &item : tree)
        validIndices.insert(item.index);
    for (auto &item : tree) {
        auto missing = [&](int index) { return !validIndices.contains(index); };
        item.index_base.erase(std::remove_if(item.index_base.begin(), item.index_base.end(), missing),
                              item.index_base.end());
        item.index_support.erase(std::remove_if(item.index_support.begin(), item.index_support.end(), missing),
                                 item.index_support.end());
    }
    ENTITY_LIST oldEntities;
    for (auto *entity : replaced)
        if (entity)
            oldEntities.add(entity);
    if (oldEntities.count()) {
        API_NOP_BEGIN;
        api_del_entity_list(oldEntities);
        API_NOP_END;
    }
    delete decoded.entities;
    curWindow->ptrCurrentTreeItem = nullptr;
    curWindow->clearSelectedEntities();
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();
    curWindow->setIsModified(localDirty && !pendingEntityChanges.isEmpty());
    return true;
}
