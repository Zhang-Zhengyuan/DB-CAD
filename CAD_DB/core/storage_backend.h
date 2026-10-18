#pragma once

// StorageBackend —— 可换的存储后端接口（架构 v2 核心库，REFACTOR_PLAN M1.5）。
//
// 设计不变量（ARCHITECTURE.md §3）：存储就两条链路——SAT 全量（本地文件，
// 内核原生读写）与序列化 JSON（经上层同步到服务端）。两者实现同一接口后，
// 存取模式分支的 if/else 消失，改为后端选择。
//
// 全接口异步（回调式）：协作后端的 open/save 走网络；本地文件后端的回调
// 在调用内同步完成（快 IO，契约保证）。回调恰好调用一次，error 为面向
// 用户的中文文案。
//
// location 语义：SAT 后端是文件路径；协作后端是项目名。

#include <functional>
#include <optional>

#include <QString>
#include <QStringList>

#include "kernel/geometry_kernel.h"
#include <QJsonObject>

namespace dbcad {

// open 的返回：后端按自身能力填充字段。
struct StorageDocument {
    QString location;    // 解析后的位置（文件绝对路径或项目名）
    QString projectId;   // 协作后端：项目 ID
    int version = 0;     // 协作后端：打开时的版本（0 = 空项目）
    ShapeList shapes;    // SAT 后端：已恢复的内核实体（调用方接管）
    QJsonObject graph;   // 协作后端：实体图对象（空对象 = 空项目）
    QStringList removedIds;
};

// save 的入参：后端按自身能力取用字段。
struct SaveRequest {
    QByteArray satPayload;      // SAT 后端：内核原生字节
    QJsonObject deltaGraph;     // 协作后端：变更组件图
    QString changesJson;        // 协作后端：元数据变更列表 JSON
    QStringList removedIds;     // 协作后端：整替删除的组件 UUID
    std::optional<int> baseVersion; // 协作后端：乐观锁基线版本
    QString author;
    QString sourceClientId;
};

struct StorageOpenResult {
    bool ok = false;
    StorageDocument document;
    QString error;
};
struct StorageSaveResult {
    bool ok = false;
    int version = 0; // 协作后端：新版本号
    QString error;
};
struct StorageListResult {
    bool ok = false;
    QStringList entries;
    QString error;
};
struct StorageRemoveResult {
    bool ok = false;
    QString error;
};

class StorageBackend {
public:
    virtual ~StorageBackend() = default;

    virtual QString backendName() const = 0;
    virtual bool isConfigured() const = 0;
    // 协作后端需要项目解析/WS 连接等编排；视图据此决定工作流分支。
    virtual bool isCollaborative() const = 0;

    virtual void open(const QString& location, std::function<void(const StorageOpenResult&)> callback) = 0;
    virtual void save(const QString& location, const SaveRequest& request,
                      std::function<void(const StorageSaveResult&)> callback) = 0;
    virtual void list(const QString& scope, std::function<void(const StorageListResult&)> callback) = 0;
    virtual void remove(const QString& location, std::function<void(const StorageRemoveResult&)> callback) = 0;
};

} // namespace dbcad
