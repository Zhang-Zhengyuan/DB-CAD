#include "mainwindow_internal.h"

void MainWindow::beginEntityChangeTracking() {
    isTrackingEntityChanges = true;
    entityChangeTrackingStartTime = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.clear();
}

void MainWindow::recordEntityAdded(const QString &uuid, const QString &name, const QString &entityType, int index) {
    if (!isTrackingEntityChanges)
        return;
    MainWindow::EntityChange change;
    change.uuid = uuid;
    change.name = name;
    change.entityType = entityType;
    change.changeType = MainWindow::EntityChangeType::ADD;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.append(change);
    entityIndexToUuid[index] = uuid;
}

void MainWindow::recordEntityRemoved(int index) {
    if (!isTrackingEntityChanges)
        return;
    // apply remote snapshot 期间 Window::clear() 会触发本函数，必须过滤掉，
    // 否则会把"远端 apply 时的 clear"误记成本地 REMOVE 变更。
    auto &session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot()) {
        return;
    }
    QString uuid = entityIndexToUuid.value(index, "");
    MainWindow::EntityChange change;
    change.uuid = uuid;
    if (curWindow != nullptr) {
        const auto &tree = curWindow->getEntityTree();
        for (const auto &item : tree) {
            if (item.index == index) {
                change.name = QString::fromStdString(item.name);
                change.entityType = change.name;
                break;
            }
        }
    }
    change.changeType = MainWindow::EntityChangeType::REMOVE;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.append(change);
    entityIndexToUuid.remove(index);
}

// 协作友好的本地删除：UI 右键 → 真正 ACIS api_del_entity + 清理依赖 + 记账 REMOVE + 刷新 UI。
// 仅记 pendingEntityChanges，不自动 Push；与添加实体保持同一行为契约（待 Push 按钮）。
void MainWindow::deleteEntityByIndexForCollaboration(int index) {
    qDebug().noquote() << "[Collab][Delta] deleteEntityByIndexForCollaboration ENTER index=" << index;
    if (curWindow == nullptr)
        return;
    auto &etiList = curWindow->getEntityTree();
    dbcad::ShapeHandle entity;
    int foundPos = -1;
    for (int i = 0; i < (int)etiList.size(); ++i) {
        if (etiList[i].index == index) {
            entity = etiList[i].shape;
            foundPos = i;
            break;
        }
    }
    if (foundPos < 0) {
        statusBar()->showMessage(tr("未找到该实体"), 3000);
        return;
    }

    // 收集下游依赖（递归）：index_base 指向 idx 的实体本身也要删，避免孤儿节点。
    std::vector<int> toRemoveIndices;
    dbcad::ShapeList toRemoveEntities;
    std::vector<QString> toRemoveUuids;
    QSet<int> visited;
    std::function<void(int)> collectDeps = [&](int idx) {
        if(visited.contains(idx)) return;
        visited.insert(idx);
        for (auto &eti : etiList) {
            bool dependsOnIdx = false;
            for (int b : eti.index_base) {
                if (b == idx) {
                    dependsOnIdx = true;
                    break;
                }
            }
            if (!dependsOnIdx)
                continue;
            bool alreadyQueued = false;
            for (int r : toRemoveIndices) {
                if (r == eti.index) {
                    alreadyQueued = true;
                    break;
                }
            }
            if (alreadyQueued || visited.contains(eti.index))
                continue;
            collectDeps(eti.index);
            toRemoveIndices.push_back(eti.index);
            toRemoveEntities.push_back(eti.shape);
            toRemoveUuids.push_back(QString::fromStdString(eti.uuid));
        }
    };
    collectDeps(index);
    toRemoveIndices.push_back(index);
    toRemoveEntities.push_back(entity);
    toRemoveUuids.push_back(QString::fromStdString(etiList[foundPos].uuid));

    try { dbcad::kernel().erase(toRemoveEntities); }
    catch(const std::exception& e) { showMessage(QString::fromUtf8(e.what()),5000); return; }
    for(auto removed:toRemoveIndices) recordEntityRemoved(removed);
    for(auto it=etiList.begin();it!=etiList.end();) {
        if(std::find(toRemoveIndices.begin(),toRemoveIndices.end(),it->index)!=toRemoveIndices.end()) {
            curWindow->removeFromSelectedEntities(it->index);
            delete it->ptrDisplayData; it=etiList.erase(it);
        } else ++it;
    }
    curWindow->ptrCurrentTreeItem=nullptr;
    curWindow->setCurrentEntity({});

    // 3. 修正剩余 entity 的 index_base / index_support：清掉指向已删除 index 的位置
    for (auto &eti : etiList) {
        std::vector<int> newBase;
        newBase.reserve(eti.index_base.size());
        for (int b : eti.index_base) {
            bool stillExists = false;
            for (auto &e : etiList) {
                if (e.index == b) {
                    stillExists = true;
                    break;
                }
            }
            if (stillExists)
                newBase.push_back(b);
        }
        eti.index_base = std::move(newBase);

        std::vector<int> newSupport;
        newSupport.reserve(eti.index_support.size());
        for (int s : eti.index_support) {
            bool stillExists = false;
            for (auto &e : etiList) {
                if (e.index == s) {
                    stillExists = true;
                    break;
                }
            }
            if (stillExists)
                newSupport.push_back(s);
        }
        eti.index_support = std::move(newSupport);
    }

    curWindow->captureHistory();

    // 4. 刷新 UI（tree widget 必须重绘，仅 updateMeshData 不够）
    curWindow->updateTreeWidget();
    curWindow->updateMeshData();
    if (curWindow->getEntityTree().empty()) {
        curWindow->glWidget->clear();
    } else {
        curWindow->glWidget->update();
    }

    // 5. 状态栏：仅记入 pendingEntityChanges，由用户点 Push 按钮统一提交（含 REMOVE）
    {
        int pendingRemoves = 0;
        int pendingAdds = 0;
        for (const MainWindow::EntityChange &ch : pendingEntityChanges) {
            if (ch.changeType == MainWindow::EntityChangeType::REMOVE)
                ++pendingRemoves;
            else if (ch.changeType == MainWindow::EntityChangeType::ADD)
                ++pendingAdds;
        }
        if (pendingRemoves > 0) {
            statusBar()->showMessage(
                tr("已删除 %1 个实体（待 Push，含 %2 个删除）").arg((int)toRemoveIndices.size()).arg(pendingRemoves),
                3000);
        } else {
            statusBar()->showMessage(tr("已删除 %1 个实体（待 Push）").arg((int)toRemoveIndices.size()), 3000);
        }
    }
    updateCollabPanelUi();
}

void MainWindow::recordEntityModified(int index) {
    if (!isTrackingEntityChanges)
        return;
    auto &session = CollabSession::instance();
    if (session.isApplyingRemoteSnapshot()) {
        return;
    }
    QString uuid = entityIndexToUuid.value(index, "");
    if (uuid.isEmpty())
        return;
    MainWindow::EntityChange change;
    change.uuid = uuid;
    if (curWindow) {
        for (const auto &item : curWindow->getEntityTree()) {
            if (item.index == index) {
                change.name = QString::fromStdString(item.name);
                break;
            }
        }
    }
    change.changeType = MainWindow::EntityChangeType::MODIFY;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingEntityChanges.append(change);
}

QList<MainWindow::EntityChange> MainWindow::endEntityChangeTracking() {
    isTrackingEntityChanges = false;
    return pendingEntityChanges;
}

void MainWindow::clearEntityChanges() {
    pendingEntityChanges.clear();
    isTrackingEntityChanges = false;
}

QString MainWindow::exportEntityChangesToJson(const QList<MainWindow::EntityChange> &changes) {
    QJsonArray changesArray;
    for (const auto &change : changes) {
        QJsonObject obj;
        obj["uuid"] = change.uuid;
        obj["name"] = change.name;
        obj["entityType"] = change.entityType;
        switch (change.changeType) {
        case MainWindow::EntityChangeType::ADD:
            obj["changeType"] = "ADD";
            break;
        case MainWindow::EntityChangeType::REMOVE:
            obj["changeType"] = "REMOVE";
            break;
        case MainWindow::EntityChangeType::MODIFY:
            obj["changeType"] = "MODIFY";
            break;
        }
        obj["entityIndex"] = change.entityIndex;
        obj["timestamp"] = change.timestamp;
        // SAT lives once in the graph component; changes carry only metadata.
        changesArray.append(obj);
    }
    QJsonObject root;
    root["changes"] = changesArray;
    root["trackingStartTime"] = entityChangeTrackingStartTime;
    root["exportTime"] = QDateTime::currentMSecsSinceEpoch();
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}
