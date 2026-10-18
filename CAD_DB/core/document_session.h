#pragma once

// DocumentSession —— 无 UI 的文档会话（架构 v2 核心库）。
//
// 职责（自 REFACTOR_PLAN M1.2）：
//   * 业务实体树（ENTITY_TREE_ITEM 列表、单调索引、依赖图 index_base/index_support）；
//   * 组件级变更记账（record* 逻辑，供 json-delta 增量提交）；
//   * 远端图合并入口 applyRemoteGraph（组件替换、删除、追加，防回环标记 RAII）；
//   * 参数化配方驱动的下游重建（rebuildDownstream）；
//   * 脏标记（dirty）。
//
// 边界：不 include 任何 Qt Widgets 头；只依赖 QtCore 与几何内核接口。
// 视图（Window/GLWidget）在本类之上做刷新，通过 retiredDisplayData 取回需要
// 释放的显示缓存；消息经由 messageSink 上抛（默认丢弃）。

#include <functional>
#include <optional>
#include <vector>

#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QSet>
#include <QString>
#include <QStringList>

#include "core/entity_change.h"
#include "kernel/document_model.h"
#include "kernel/geometry_kernel.h"

namespace dbcad {

class DocumentSession {
public:
    explicit DocumentSession(GeometryKernel& geometryKernel);

    // ---------- 视图回调 ----------
    // 供视图设置的状态消息出口（原 showMessage 路径）。核心库内不做任何 UI。
    void setMessageSink(std::function<void(const QString&)> sink) { messageSink_ = std::move(sink); }
    // M4.2 在场感知：本地记录 ADD/MODIFY 时回调（每 uuid 一次，直到 forgetEntityEdit）。
    void setEntityEditSink(std::function<void(const QString& uuid)> sink) { entityEditSink_ = std::move(sink); }
    void forgetEntityEdit(const QString& uuid) { announcedEdits_.remove(uuid); }
    void forgetAllEntityEdits() { announcedEdits_.clear(); }

    // ---------- 实体树 ----------
    ENTITY_TREE_ITEM* addEntity(ShapeHandle shape, const std::string& name, int subOperatorType);
    ENTITY_TREE_ITEM* addEntity(ShapeHandle shape, const std::string& name, int subOperatorType,
                                std::vector<int> indexBase, OPERATOR_TYPES ot = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR);
    ENTITY_TREE_ITEM* addEntity(ShapeHandle shape, const std::string& name, int subOperatorType,
                                HandleGrid& handles);
    ENTITY_TREE_ITEM* addEntity(ShapeHandle shape, const std::string& name, int subOperatorType,
                                HandleGrid& handles, std::vector<int> indexBase,
                                OPERATOR_TYPES ot = OPERATOR_TYPES::OPERATOR_CONSTRUCTOR);

    ENTITY_TREE_ITEM* itemByIndex(int index);
    const ENTITY_TREE_ITEM* itemByIndex(int index) const;
    const std::vector<ENTITY_TREE_ITEM>& entities() const { return entityTree_; }
    // 视图对条目表现字段（可见性、名称、显示类型）的就地编辑入口。
    std::vector<ENTITY_TREE_ITEM>& entitiesMutable() { return entityTree_; }

    ShapeList shapeList() const;
    ShapeList selectedShapeList(const std::vector<int>& selectedIndices) const;

    int allocateIndex() { return latestIndex_++; }
    int nextIndex() const { return latestIndex_; }

    // 清空文档：擦除内核几何、记 REMOVE 账、清树（单调索引不回退）。
    // 返回被淘汰的显示缓存，视图负责 delete。
    std::vector<GmeMesh::DisplayData*> clearAll();

    // 协作友好的级联删除（原 deleteEntityByIndexForCollaboration 的数据部分）：
    // 递归删除依赖该实体的所有下游，修正剩余条目的依赖引用。
    struct CascadeResult {
        bool ok = false;
        std::vector<int> removedIndices;
        std::vector<GmeMesh::DisplayData*> retiredDisplayData;
    };
    CascadeResult removeEntityCascade(int index);

    // ---------- 内核操作（原 Window::createEntity/operation 的非交互部分） ----------
    // 交互式输入（样条草绘、输入模式切换）留在视图；这里只做纯内核创建。
    ShapeHandle createPrimitive(int primitiveId, HandleGrid& handles);
    // 布尔/运算：updateDocument=false 时只算内核结果不动树。
    bool operate(OPERATOR_TYPES type, int subType, std::vector<int>& ids, ShapeList& outputs,
                 bool updateDocument, std::vector<GmeMesh::DisplayData*>& retiredDisplayData);

    // 依赖驱动的下游重建（原 Window::updateEntites）。返回 false 表示检测到循环依赖。
    bool rebuildDownstream(std::vector<int> indices, std::vector<GmeMesh::DisplayData*>& retiredDisplayData);

    // ---------- 变更记账（原 MainWindow record* 逻辑） ----------
    void beginChangeTracking();
    QList<EntityChange> endChangeTracking();
    // 停止追踪并清空（原 clearEntityChanges）。
    void stopChangeTracking();
    // 只清空待提交列表，追踪保持开启（提交成功后的动作）。
    void discardPendingChanges() { pendingChanges_.clear(); }
    // 清空 index→uuid 映射（导入路径：清空后由 addEntity/applyRemoteGraph 重建）。
    void clearIndexToUuid() { indexToUuid_.clear(); }
    // 清空待提交列表并按当前树重建 index→uuid 映射（远端打开/导入后的动作）。
    void resetChangesFromTree();

    void recordEntityAdded(const QString& uuid, const QString& name, const QString& entityType, int index);
    void recordEntityRemoved(int index);
    void recordEntityModified(int index);

    const QList<EntityChange>& pendingChanges() const { return pendingChanges_; }
    QString exportEntityChangesJson(const QList<EntityChange>& changes) const;
    qint64 changeTrackingStartTime() const { return changeTrackingStartTime_; }
    bool isTrackingEntityChanges() const { return trackingEntityChanges_; }

    // ---------- 远端应用（原 pullEntityGraph 的合并核心） ----------
    struct ApplyRemoteResult {
        bool ok = false;
        std::vector<GmeMesh::DisplayData*> retiredDisplayData;
    };
    // version 仅用于错误消息；graph 为组件图对象；deltaGraph=false 表示全量快照。
    ApplyRemoteResult applyRemoteGraph(int version, const QJsonObject& graph, bool deltaGraph,
                                       const QStringList& removedIds, const QHash<QString, QString>& remoteNames);

    // ---------- 防回环标记 ----------
    // applyRemoteGraph 内部以 RAII 置位；外部也可查询。
    bool applyingRemoteSnapshot() const { return applyingRemoteSnapshot_; }

    // ---------- 脏标记 ----------
    bool isDirty() const { return dirty_; }
    void setDirty(bool value) { dirty_ = value; }

    GeometryKernel& kernel() const { return kernel_; }

private:
    void emitMessage(const QString& message) const;
    void retireDisplayData(ENTITY_TREE_ITEM& item, std::vector<GmeMesh::DisplayData*>& retired);

    GeometryKernel& kernel_;
    std::function<void(const QString&)> messageSink_;

    std::vector<ENTITY_TREE_ITEM> entityTree_;
    int latestIndex_ = 0;

    QList<EntityChange> pendingChanges_;
    QHash<int, QString> indexToUuid_;
    bool trackingEntityChanges_ = false;
    qint64 changeTrackingStartTime_ = 0;

    bool applyingRemoteSnapshot_ = false;
    bool dirty_ = false;
    std::function<void(const QString& uuid)> entityEditSink_;
    QSet<QString> announcedEdits_;

    void announceEntityEdit(const QString& uuid);
};

} // namespace dbcad
