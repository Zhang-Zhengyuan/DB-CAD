#pragma once

// StorageBackend —— 可换的存储后端接口（架构 v2 核心库，REFACTOR_PLAN M1.5）。
//
// 设计不变量（ARCHITECTURE.md §3）：存储就两条链路——SAT 全量（本地文件，
// 内核原生读写）与序列化 JSON（经上层同步到服务端）。两者实现同一接口后，
// 存取模式分支的 if/else 消失，改为后端选择。
//
// 语义约定：
//   * location：SAT 后端是文件路径；协作后端是项目名。
//   * open：SAT 后端读文件返回内核实体；协作后端解析项目并取最新 delta。
//   * save：SAT 后端全量写文件；协作后端推送 json-delta（返回新版本号）。
//   * list/remove：SAT 后端按目录列出/删除；协作后端由服务端能力决定。
// 所有实现以 error 出参描述失败原因（面向用户的中文文案）。

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

class StorageBackend {
public:
    virtual ~StorageBackend() = default;

    virtual QString backendName() const = 0;
    virtual bool isConfigured() const = 0;
    // 协作后端需要项目解析/WS 连接等编排；视图据此决定工作流分支。
    virtual bool isCollaborative() const = 0;

    // 打开文档。失败时返回空并写 *error。
    virtual std::optional<StorageDocument> open(const QString& location, QString* error) = 0;
    // 保存文档。返回值：协作后端为新版本号；SAT 后端成功时返回 0。
    virtual std::optional<int> save(const QString& location, const SaveRequest& request, QString* error) = 0;
    virtual QStringList list(const QString& scope, QString* error) = 0;
    virtual bool remove(const QString& location, QString* error) = 0;
};

} // namespace dbcad
