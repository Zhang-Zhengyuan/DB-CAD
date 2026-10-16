#include "mainwindow_internal.h"
#include <QScopeGuard>

// Reconstruct every incoming component before changing the visible document.
// The decoder chooses fine JSON or the matching SAT backup per stable UUID.
bool MainWindow::pullEntityGraph(int version, const QJsonObject &graph, bool deltaGraph,
                                     const QStringList &removedIds, const QHash<QString, QString> &remoteNames) {
    if (!curWindow)
        return false;
    dbcad::DecodedGraph decoded;
    QString error;
    if (!graph.value("nodes").toArray().isEmpty() && !dbcad::kernel().decodeGraph(graph, decoded, error)) {
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
    auto& currentTree=curWindow->getEntityTree();
    auto tree=currentTree;
    auto uuidMapping=entityIndexToUuid;
    int nextIndex=curWindow->latest_index;
    std::vector<GmeMesh::DisplayData*> retiredMeshes;
    bool committed=false;
    const auto discardDecoded=qScopeGuard([&]() {
        if(!committed) {
            dbcad::ShapeList abandoned;
            for(auto h:decoded.shapes) abandoned.push_back(h);
            try { dbcad::kernel().erase(abandoned); } catch(...) {}
        }
    });
    QSet<QString> incoming;
    for (auto it = decoded.shapes.constBegin(); it != decoded.shapes.constEnd(); ++it)
        incoming.insert(it.key());
    dbcad::ShapeList replaced;
    // Keep indices, visibility, dependencies and display settings on updates.
    for (int i = static_cast<int>(tree.size()) - 1; i >= 0; --i) {
        auto &item = tree[static_cast<size_t>(i)];
        const QString uuid = QString::fromStdString(item.uuid);
        if (incoming.contains(uuid)) {
            replaced.push_back(item.shape);
            item.shape = decoded.shapes.value(uuid);
            // Incoming geometry replaces the old local construction recipe.
            item.operatorType=OPERATOR_IMPORT; item.subOperatorType=-1;
            item.handles.clear(); item.trans={}; item.index_base.clear(); item.rebuildGroup=-1;
            retiredMeshes.push_back(item.ptrDisplayData);
            item.ptrDisplayData = nullptr;
            if (names.contains(uuid))
                item.name = names.value(uuid).toStdString();
            uuidMapping.insert(item.index, uuid);
            incoming.remove(uuid);
        } else if (removedIds.contains(uuid) || (!deltaGraph && !decoded.shapes.contains(uuid))) {
            replaced.push_back(item.shape);
            retiredMeshes.push_back(item.ptrDisplayData);
            uuidMapping.remove(item.index);
            tree.erase(tree.begin() + i);
        }
    }
    for (const auto &uuid : incoming) {
        ENTITY_TREE_ITEM item;
        item.index = nextIndex++;
        item.uuid = uuid.toStdString();
        item.name = names.value(uuid, tr("远端实体")).toStdString();
        item.shape = decoded.shapes.value(uuid);
        item.visible = true;
        item.subOperatorType = -1;
        item.operatorType = OPERATOR_IMPORT;
        tree.push_back(item);
        uuidMapping.insert(item.index, uuid);
    }
    QSet<int> validIndices;
    for (const auto &item : tree)
        validIndices.insert(item.index);
    for (auto &item : tree) {
        auto missing = [&](int index) { return !validIndices.contains(index); };
        item.index_base.erase(std::remove_if(item.index_base.begin(), item.index_base.end(), missing),
                              item.index_base.end());
        item.index_support.clear();
    }
    for(const auto& item:tree) for(auto base:item.index_base)
        for(auto& source:tree) if(source.index==base) source.index_support.push_back(item.index);
    try { dbcad::kernel().erase(replaced); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return false; }
    currentTree=std::move(tree);
    entityIndexToUuid=std::move(uuidMapping);
    curWindow->latest_index=nextIndex;
    for(auto mesh:retiredMeshes) delete mesh;
    committed=true;
    // Remote edits invalidate old local undo snapshots, which must not undo a colleague's changes.
    curWindow->resetDocumentHistory();
    curWindow->currentEntityIndex = -1;
    curWindow->clearSelectedEntities();
    curWindow->setCurrentEntity({});
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();
    curWindow->setIsModified(localDirty && !pendingEntityChanges.isEmpty());
    return true;
}
