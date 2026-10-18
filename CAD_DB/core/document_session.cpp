#include "core/document_session.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>

#include <algorithm>
#include <set>

namespace dbcad {

DocumentSession::DocumentSession(GeometryKernel& geometryKernel) : kernel_(geometryKernel) {
    // 变更追踪由视图在就绪后显式开启（beginChangeTracking）。
}

void DocumentSession::emitMessage(const QString& message) const {
    if (messageSink_)
        messageSink_(message);
}

void DocumentSession::retireDisplayData(ENTITY_TREE_ITEM& item,
                                        std::vector<GmeMesh::DisplayData*>& retired) {
    if (item.ptrDisplayData != nullptr) {
        retired.push_back(item.ptrDisplayData);
        item.ptrDisplayData = nullptr;
    }
}

// ---------- 实体树 ----------

ENTITY_TREE_ITEM* DocumentSession::addEntity(ShapeHandle shape, const std::string& name, int subOperatorType) {
    HandleGrid handles;
    std::vector<int> indexBase;
    return addEntity(shape, name, subOperatorType, handles, indexBase);
}

ENTITY_TREE_ITEM* DocumentSession::addEntity(ShapeHandle shape, const std::string& name, int subOperatorType,
                                             std::vector<int> indexBase, OPERATOR_TYPES ot) {
    HandleGrid handles;
    return addEntity(shape, name, subOperatorType, handles, indexBase, ot);
}

ENTITY_TREE_ITEM* DocumentSession::addEntity(ShapeHandle shape, const std::string& name, int subOperatorType,
                                             HandleGrid& handles) {
    std::vector<int> indexBase;
    return addEntity(shape, name, subOperatorType, handles, indexBase);
}

ENTITY_TREE_ITEM* DocumentSession::addEntity(ShapeHandle shape, const std::string& name, int sot,
                                             HandleGrid& handles, std::vector<int> indexBase, OPERATOR_TYPES ot) {
    if (!shape)
        return nullptr;
    ENTITY_TREE_ITEM eti;
    eti.uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    eti.name = name;
    // Index is an identity used by dependency lists and collaboration
    // bookkeeping. entityTree_.size() is not stable after a deletion (it
    // would reuse an existing index and make two rows address the same
    // entity), so allocate from the monotonic counter instead.
    eti.index = latestIndex_++;
    eti.shape = shape;
    eti.handles = handles;
    eti.visible = true;
    eti.operatorType = ot;
    eti.subOperatorType = sot;
    eti.index_base = indexBase;
    entityTree_.push_back(eti);
    for (auto index : indexBase) {
        for (auto& e : entityTree_) {
            if (e.index == index)
                e.index_support.push_back(eti.index);
        }
    }
    // 只有在"非 apply remote snapshot"时才记录增量变更，
    // 否则会把远端实体的"添加"动作记成本地 ADD change，下次提交造成回环上传。
    if (!applyingRemoteSnapshot_) {
        recordEntityAdded(QString::fromStdString(eti.uuid), QString::fromStdString(eti.name),
                          QString::fromStdString(eti.name), eti.index);
    }
    // `ENTITY_TREE_ITEM::index` is a monotonic logical identity. It is not
    // the current std::vector position after an entity has been deleted.
    return &entityTree_.back();
}

ENTITY_TREE_ITEM* DocumentSession::itemByIndex(int index) {
    for (auto& e : entityTree_)
        if (e.index == index)
            return &e;
    return nullptr;
}

const ENTITY_TREE_ITEM* DocumentSession::itemByIndex(int index) const {
    for (auto& e : entityTree_)
        if (e.index == index)
            return &e;
    return nullptr;
}

ShapeList DocumentSession::shapeList() const {
    ShapeList el;
    for (const auto& eti : entityTree_)
        el.push_back(eti.shape);
    return el;
}

ShapeList DocumentSession::selectedShapeList(const std::vector<int>& selectedIndices) const {
    ShapeList el;
    for (auto selectedIndex : selectedIndices) {
        const auto* entity = itemByIndex(selectedIndex);
        if (entity)
            el.push_back(entity->shape);
    }
    return el;
}

std::vector<GmeMesh::DisplayData*> DocumentSession::clearAll() {
    std::vector<GmeMesh::DisplayData*> retiredDisplayData;
    kernel_.erase(shapeList());
    for (auto& item : entityTree_) {
        recordEntityRemoved(item.index);
        retireDisplayData(item, retiredDisplayData);
    }
    entityTree_.clear();
    dirty_ = true;
    return retiredDisplayData;
}

DocumentSession::CascadeResult DocumentSession::removeEntityCascade(int index) {
    CascadeResult result;
    // 收集下游依赖（递归）：index_base 指向 idx 的实体本身也要删，避免孤儿节点。
    ShapeHandle entity;
    const ENTITY_TREE_ITEM* found = itemByIndex(index);
    if (found == nullptr)
        return result;
    entity = found->shape;

    std::vector<int> toRemoveIndices;
    ShapeList toRemoveEntities;
    QSet<int> visited;
    std::function<void(int)> collectDeps = [&](int idx) {
        if (visited.contains(idx))
            return;
        visited.insert(idx);
        for (const auto& eti : entityTree_) {
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
        }
    };
    collectDeps(index);
    toRemoveIndices.push_back(index);
    toRemoveEntities.push_back(entity);

    try {
        kernel_.erase(toRemoveEntities);
    } catch (const std::exception& e) {
        emitMessage(QString::fromUtf8(e.what()));
        return result;
    }
    for (auto removed : toRemoveIndices)
        recordEntityRemoved(removed);
    for (auto it = entityTree_.begin(); it != entityTree_.end();) {
        if (std::find(toRemoveIndices.begin(), toRemoveIndices.end(), it->index) != toRemoveIndices.end()) {
            retireDisplayData(*it, result.retiredDisplayData);
            it = entityTree_.erase(it);
        } else {
            ++it;
        }
    }

    // 修正剩余 entity 的 index_base / index_support：清掉指向已删除 index 的位置
    for (auto& eti : entityTree_) {
        std::vector<int> newBase;
        newBase.reserve(eti.index_base.size());
        for (int b : eti.index_base) {
            if (itemByIndex(b) != nullptr)
                newBase.push_back(b);
        }
        eti.index_base = std::move(newBase);

        std::vector<int> newSupport;
        newSupport.reserve(eti.index_support.size());
        for (int s : eti.index_support) {
            if (itemByIndex(s) != nullptr)
                newSupport.push_back(s);
        }
        eti.index_support = std::move(newSupport);
    }

    result.ok = true;
    result.removedIndices = std::move(toRemoveIndices);
    return result;
}

// ---------- 内核操作 ----------

ShapeHandle DocumentSession::createPrimitive(int primitiveId, HandleGrid& handles) {
    // 非交互路径：样条无控制点视为无效配方（交互式草绘留在视图）。
    if (primitiveId == E_BSPLINE && handles.empty())
        return {};
    try {
        auto candidate = handles;
        ShapeHandle shape = kernel_.create(primitiveId, candidate);
        handles = std::move(candidate);
        return shape;
    } catch (const std::exception& e) {
        emitMessage(QString::fromUtf8(e.what()));
        return {};
    }
}

bool DocumentSession::operate(OPERATOR_TYPES type, int subType, std::vector<int>& ids, ShapeList& outputs,
                              bool updateDocument, std::vector<GmeMesh::DisplayData*>& retiredDisplayData) {
    if (type != OPERATOR_DEFEATURE && ids.size() != 2) {
        emitMessage(QString::fromUtf8("此运算需要恰好两个实体"));
        return false;
    }
    if (std::set<int>(ids.begin(), ids.end()).size() != ids.size()) {
        emitMessage(QString::fromUtf8("不能重复选择同一实体"));
        return false;
    }
    if (updateDocument && type == OPERATOR_BOOLEAN) {
        for (auto id : ids) {
            if (const auto* item = itemByIndex(id); item && !item->index_support.empty()) {
                emitMessage(QString::fromUtf8("实体仍有依赖结果，请先删除依赖结果再进行布尔运算"));
                return false;
            }
        }
    }
    ShapeList inputs;
    for (auto id : ids) {
        const auto* item = itemByIndex(id);
        if (item && item->shape)
            inputs.push_back(item->shape);
    }
    try {
        outputs = kernel_.operate(type, subType, inputs);
    } catch (const std::exception& e) {
        emitMessage(QString::fromUtf8(e.what()));
        return false;
    }
    if (outputs.empty()) {
        emitMessage(QString::fromUtf8("运算未产生结果"));
        return false;
    }
    if (updateDocument) {
        if (type == OPERATOR_BOOLEAN) {
            try {
                kernel_.erase({inputs[0], inputs[1]});
            } catch (const std::exception& e) {
                kernel_.erase(outputs);
                outputs.clear();
                emitMessage(QString::fromUtf8(e.what()));
                return false;
            }
            for (auto it = entityTree_.begin(); it != entityTree_.end();) {
                if (std::find(ids.begin(), ids.begin() + 2, it->index) != ids.begin() + 2) {
                    recordEntityRemoved(it->index);
                    retireDisplayData(*it, retiredDisplayData);
                    it = entityTree_.erase(it);
                } else {
                    ++it;
                }
            }
            // Remove dependency references to consumed input rows.
            for (auto& item : entityTree_) {
                auto consumed = [&](int id) { return id == ids[0] || id == ids[1]; };
                std::erase_if(item.index_base, consumed);
                std::erase_if(item.index_support, consumed);
            }
        } else {
            for (auto id : ids) {
                if (auto* item = itemByIndex(id))
                    item->visible = false;
            }
        }
    }
    return true;
}

bool DocumentSession::rebuildDownstream(std::vector<int> indices,
                                        std::vector<GmeMesh::DisplayData*>& retiredDisplayData) {
    std::set<int> affected, visited, active;
    bool cycle = false;
    std::function<void(int)> collect = [&](int index) {
        if (!affected.insert(index).second)
            return;
        if (const auto* row = itemByIndex(index)) {
            for (auto child : row->index_support)
                collect(child);
        }
    };
    for (auto index : indices)
        collect(index);
    std::function<void(int)> rebuild = [&](int index) {
        if (visited.contains(index))
            return;
        if (!active.insert(index).second) {
            cycle = true;
            return;
        }
        const ENTITY_TREE_ITEM* item = itemByIndex(index);
        const auto bases = item ? item->index_base : std::vector<int>{};
        for (auto base : bases) {
            if (affected.contains(base))
                rebuild(base);
        }
        active.erase(index);
        if (cycle)
            return;
        if (!visited.insert(index).second)
            return;
        item = itemByIndex(index);
        if (!item || !item->shape || !item->isAutoUpdate)
            return;
        auto metadata = *item;
        std::vector<int> siblings{index};
        if (metadata.rebuildGroup >= 0) {
            siblings.clear();
            for (const auto& row : entityTree_) {
                if (row.rebuildGroup == metadata.rebuildGroup) {
                    siblings.push_back(row.index);
                    visited.insert(row.index);
                }
            }
        }
        ShapeList results;
        if (metadata.operatorType == OPERATOR_CONSTRUCTOR) {
            ShapeHandle shape = createPrimitive(metadata.subOperatorType, metadata.handles);
            if (shape)
                results.push_back(shape);
        } else {
            auto opBases = metadata.index_base;
            std::vector<GmeMesh::DisplayData*> opRetired;
            if (!operate(metadata.operatorType, metadata.subOperatorType, opBases, results, false, opRetired)) {
                retiredDisplayData.insert(retiredDisplayData.end(), opRetired.begin(), opRetired.end());
                return;
            }
            retiredDisplayData.insert(retiredDisplayData.end(), opRetired.begin(), opRetired.end());
        }
        if (results.empty())
            return;
        for (size_t i = 0; i < results.size(); ++i) {
            const auto* row = i < siblings.size() ? itemByIndex(siblings[i]) : nullptr;
            kernel_.copyColor(metadata.shape, results[i]);
            kernel_.translate(results[i], row ? row->trans : metadata.trans, true);
        }
        for (size_t i = 0; i < results.size(); ++i) {
            if (i < siblings.size()) {
                auto* row = itemByIndex(siblings[i]);
                if (!row)
                    continue;
                auto old = row->shape;
                row->shape = results[i];
                kernel_.erase({old});
                retireDisplayData(*row, retiredDisplayData);
                recordEntityModified(row->index);
            } else {
                auto* row =
                    addEntity(results[i], metadata.name, metadata.subOperatorType, metadata.index_base, metadata.operatorType);
                if (row) {
                    row->rebuildGroup = metadata.rebuildGroup;
                    row->trans = metadata.trans;
                }
            }
        }
        for (size_t i = results.size(); i < siblings.size(); ++i) {
            auto cascade = removeEntityCascade(siblings[i]);
            retiredDisplayData.insert(retiredDisplayData.end(), cascade.retiredDisplayData.begin(),
                                      cascade.retiredDisplayData.end());
        }
    };
    for (auto index : affected) {
        rebuild(index);
        if (cycle)
            break;
    }
    if (cycle)
        emitMessage(QString::fromUtf8("检测到循环依赖，已停止重建"));
    return !cycle;
}

// ---------- 变更记账 ----------

void DocumentSession::beginChangeTracking() {
    trackingEntityChanges_ = true;
    changeTrackingStartTime_ = QDateTime::currentMSecsSinceEpoch();
    pendingChanges_.clear();
}

QList<EntityChange> DocumentSession::endChangeTracking() {
    trackingEntityChanges_ = false;
    return pendingChanges_;
}

void DocumentSession::stopChangeTracking() {
    pendingChanges_.clear();
    trackingEntityChanges_ = false;
}

void DocumentSession::resetChangesFromTree() {
    pendingChanges_.clear();
    indexToUuid_.clear();
    for (const auto& item : entityTree_)
        indexToUuid_.insert(item.index, QString::fromStdString(item.uuid));
}

void DocumentSession::recordEntityAdded(const QString& uuid, const QString& name, const QString& entityType,
                                        int index) {
    if (!trackingEntityChanges_)
        return;
    EntityChange change;
    change.uuid = uuid;
    change.name = name;
    change.entityType = entityType;
    change.changeType = EntityChangeType::ADD;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingChanges_.append(change);
    indexToUuid_[index] = uuid;
}

void DocumentSession::recordEntityRemoved(int index) {
    if (!trackingEntityChanges_)
        return;
    // apply remote snapshot 期间 clearAll() 会触发本函数，必须过滤掉，
    // 否则会把"远端 apply 时的 clear"误记成本地 REMOVE 变更。
    if (applyingRemoteSnapshot_) {
        return;
    }
    QString uuid = indexToUuid_.value(index, "");
    EntityChange change;
    change.uuid = uuid;
    for (const auto& item : entityTree_) {
        if (item.index == index) {
            change.name = QString::fromStdString(item.name);
            change.entityType = change.name;
            break;
        }
    }
    change.changeType = EntityChangeType::REMOVE;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingChanges_.append(change);
    indexToUuid_.remove(index);
}

void DocumentSession::recordEntityModified(int index) {
    if (!trackingEntityChanges_)
        return;
    if (applyingRemoteSnapshot_) {
        return;
    }
    QString uuid = indexToUuid_.value(index, "");
    if (uuid.isEmpty())
        return;
    EntityChange change;
    change.uuid = uuid;
    for (const auto& item : entityTree_) {
        if (item.index == index) {
            change.name = QString::fromStdString(item.name);
            break;
        }
    }
    change.changeType = EntityChangeType::MODIFY;
    change.entityIndex = index;
    change.timestamp = QDateTime::currentMSecsSinceEpoch();
    pendingChanges_.append(change);
}

QString DocumentSession::exportEntityChangesJson(const QList<EntityChange>& changes) const {
    QJsonArray changesArray;
    for (const auto& change : changes) {
        QJsonObject obj;
        obj["uuid"] = change.uuid;
        obj["name"] = change.name;
        obj["entityType"] = change.entityType;
        switch (change.changeType) {
        case EntityChangeType::ADD:
            obj["changeType"] = "ADD";
            break;
        case EntityChangeType::REMOVE:
            obj["changeType"] = "REMOVE";
            break;
        case EntityChangeType::MODIFY:
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
    root["trackingStartTime"] = changeTrackingStartTime_;
    root["exportTime"] = QDateTime::currentMSecsSinceEpoch();
    return QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Compact));
}

// ---------- 远端应用 ----------

DocumentSession::ApplyRemoteResult DocumentSession::applyRemoteGraph(
    int version, const QJsonObject& graph, bool deltaGraph, const QStringList& removedIds,
    const QHash<QString, QString>& remoteNames) {
    ApplyRemoteResult result;
    // Reconstruct every incoming component before changing the visible document.
    // The decoder chooses fine JSON or the matching SAT backup per stable UUID.
    DecodedGraph decoded;
    QString error;
    if (!graph.value("nodes").toArray().isEmpty() && !kernel_.decodeGraph(graph, decoded, error)) {
        qWarning() << "[Collab] reconstruction failed:" << error << "version=" << version;
        return result;
    }
    QHash<QString, QString> names = remoteNames;
    for (const auto& value : graph.value("nodes").toArray()) {
        const auto node = value.toObject();
        const QString name = node.value("props").toObject().value("display_name").toString();
        if (!name.isEmpty())
            names.insert(node.value("id").toString(), name);
    }
    const bool localDirty = dirty_;
    // 防回环：apply 期间所有 add/remove 记账被抑制，避免远端动作回流上传。
    applyingRemoteSnapshot_ = true;
    struct FlagReset {
        DocumentSession& session;
        ~FlagReset() { session.applyingRemoteSnapshot_ = false; }
    } flagReset{*this};

    auto tree = entityTree_;
    auto uuidMapping = indexToUuid_;
    int nextIndex = latestIndex_;
    std::vector<GmeMesh::DisplayData*> retiredMeshes;
    bool committed = false;
    const auto discardDecoded = [&]() {
        if (!committed) {
            ShapeList abandoned;
            for (auto h : decoded.shapes)
                abandoned.push_back(h);
            try {
                kernel_.erase(abandoned);
            } catch (...) {
            }
        }
    };
    QSet<QString> incoming;
    for (auto it = decoded.shapes.constBegin(); it != decoded.shapes.constEnd(); ++it)
        incoming.insert(it.key());
    ShapeList replaced;
    // Keep indices, visibility, dependencies and display settings on updates.
    for (int i = static_cast<int>(tree.size()) - 1; i >= 0; --i) {
        auto& item = tree[static_cast<size_t>(i)];
        const QString uuid = QString::fromStdString(item.uuid);
        if (incoming.contains(uuid)) {
            replaced.push_back(item.shape);
            item.shape = decoded.shapes.value(uuid);
            // Incoming geometry replaces the old local construction recipe.
            item.operatorType = OPERATOR_IMPORT;
            item.subOperatorType = -1;
            item.handles.clear();
            item.trans = {};
            item.index_base.clear();
            item.rebuildGroup = -1;
            if (item.ptrDisplayData != nullptr) {
                retiredMeshes.push_back(item.ptrDisplayData);
                item.ptrDisplayData = nullptr;
            }
            if (names.contains(uuid))
                item.name = names.value(uuid).toStdString();
            uuidMapping.insert(item.index, uuid);
            incoming.remove(uuid);
        } else if (removedIds.contains(uuid) || (!deltaGraph && !decoded.shapes.contains(uuid))) {
            replaced.push_back(item.shape);
            if (item.ptrDisplayData != nullptr) {
                retiredMeshes.push_back(item.ptrDisplayData);
                item.ptrDisplayData = nullptr;
            }
            uuidMapping.remove(item.index);
            tree.erase(tree.begin() + i);
        }
    }
    for (const auto& uuid : incoming) {
        ENTITY_TREE_ITEM item;
        item.index = nextIndex++;
        item.uuid = uuid.toStdString();
        item.name = names.value(uuid, QString::fromUtf8("远端实体")).toStdString();
        item.shape = decoded.shapes.value(uuid);
        item.visible = true;
        item.subOperatorType = -1;
        item.operatorType = OPERATOR_IMPORT;
        tree.push_back(item);
        uuidMapping.insert(item.index, uuid);
    }
    QSet<int> validIndices;
    for (const auto& item : tree)
        validIndices.insert(item.index);
    for (auto& item : tree) {
        auto missing = [&](int index) { return !validIndices.contains(index); };
        item.index_base.erase(std::remove_if(item.index_base.begin(), item.index_base.end(), missing),
                              item.index_base.end());
        item.index_support.clear();
    }
    for (const auto& item : tree) {
        for (auto base : item.index_base) {
            for (auto& source : tree) {
                if (source.index == base)
                    source.index_support.push_back(item.index);
            }
        }
    }
    try {
        kernel_.erase(replaced);
    } catch (const std::exception& e) {
        emitMessage(QString::fromUtf8(e.what()));
        discardDecoded();
        return result;
    }
    entityTree_ = std::move(tree);
    indexToUuid_ = std::move(uuidMapping);
    latestIndex_ = std::max(latestIndex_, nextIndex);
    committed = true;
    discardDecoded();
    result.ok = true;
    result.retiredDisplayData = std::move(retiredMeshes);
    dirty_ = localDirty && !pendingChanges_.isEmpty();
    return result;
}

} // namespace dbcad
