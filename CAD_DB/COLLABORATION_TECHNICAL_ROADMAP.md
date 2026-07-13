# DBCAD 多人协作系统 — 技术路线与方案全览

> 更新日期：2026-07-13 | 分支：zhangzhengyuan-dev

---

## 一、总体架构

DBCAD 多人协作采用 **客户端-服务器** 架构，分五层：

```
┌─────────────────────────────────────────────────┐
│  Qt C++ 客户端 (CAD_DB.exe)                      │
│  ┌────────────┐ ┌──────────┐ ┌───────────────┐  │
│  │MainWindow  │ │Window    │ │CollabSession   │  │
│  │(协作面板)  │ │(实体树)  │ │(状态机)        │  │
│  └────────────┘ └──────────┘ └───────────────┘  │
│  ┌──────────────────┐ ┌──────────────────────┐  │
│  │BackendApiClient  │ │EntityChange Tracking │  │
│  │(HTTP/WS 客户端)  │ │(增量变更追踪)        │  │
│  └──────────────────┘ └──────────────────────┘  │
└────────────────────┬────────────────────────────┘
                     │  HTTP REST + WebSocket (端口 8000)
┌────────────────────┴────────────────────────────┐
│  Python FastAPI 后端                             │
│  ┌──────────┐ ┌───────────┐ ┌────────────────┐  │
│  │main.py   │ │sync.py    │ │crud.py         │  │
│  │(路由/WS) │ │(连接管理) │ │(CRUD 编排)     │  │
│  └──────────┘ └───────────┘ └────────────────┘  │
│  ┌──────────────────┐ ┌──────────────────────┐  │
│  │storage_bridge.py │ │neo4j_entity_store.py │  │
│  │(HTTP→C++ Bridge) │ │(实体图版本存储)      │  │
│  └──────────────────┘ └──────────────────────┘  │
└────────────────────┬────────────────────────────┘
                     │  HTTP (端口 8100)
┌────────────────────┴────────────────────────────┐
│  C++ Storage Bridge (同一 CAD_DB.exe 进程)       │
│  ┌───────────────────────┐ ┌─────────────────┐  │
│  │storage_bridge_service │ │neo4j.cpp        │  │
│  │(TCP HTTP Server)      │ │(mgclient→Neo4j) │  │
│  └───────────────────────┘ └─────────────────┘  │
└────────────────────┬────────────────────────────┘
                     │  Bolt (端口 7687)
┌────────────────────┴────────────────────────────┐
│  Neo4j 图数据库 (Docker)                         │
│  - BridgeProject / BridgeVersion 节点            │
│  - entity_graph_version / entity_node 节点       │
└─────────────────────────────────────────────────┘
```

---

## 二、客户端协作状态机 (CollabSession)

### 2.1 设计目标

`CollabSession` 是协作系统的**唯一真相源 (Single Source of Truth)**，将所有协作状态集中管理，替代早期散落在 MainWindow 中的 ad-hoc bool 标志位。

### 2.2 状态定义 (9 个状态)

| 状态 | 含义 |
|------|------|
| `Disconnected` | WebSocket 未连接 |
| `Connected_NoProject` | 已连接但未打开项目 |
| `Connected_Idle` | 正常空闲，无待处理事项 |
| `Connected_LocalDirty` | 本地有未提交修改 |
| `Connected_SubmitInFlight` | 提交已发送，等待服务器确认 |
| `Connected_SubmitInFlight_Dirty` | 提交在空中 + 又有新的本地修改 |
| `Connected_RemotePending_Dirty` | 远端有待同步版本 + 本地也有修改 |
| `Connected_ApplyingRemote` | 正在应用远端快照 |
| `Connected_PublishingDirect` | 正在通过 HTTP 直发（非 WebSocket） |

### 2.3 决策接口

```cpp
SubmitDecision  tryBeginSubmit(reason)      // 提交前检查
ApplyDecision   tryBeginApplyRemote(ver)     // 应用远端前检查
PublishDecision tryBeginHttpPublish()        // HTTP 直发前检查
```

每个接口返回 `Allow` 或具体的 `Reject*` 原因，调用方根据结果决定是否继续。
对应的回滚接口：`rollbackSubmit()`, `rollbackApply()`, `rollbackHttpPublish()`

### 2.4 核心状态转移

```
UserEdit:      Idle → LocalDirty
SubmitStart:   LocalDirty → SubmitInFlight
SubmitOk:      SubmitInFlight → Idle (dirty-during-submit → LocalDirty)
SubmitReject:  SubmitInFlight → RemotePending_Dirty (冲突)
RemotePending: Idle → RemotePending_Dirty
ApplyStart:    → ApplyingRemote
ApplyEnd:      ApplyingRemote → Idle
```

### 2.5 遗留字段兼容层 (Legacy Mirror)

通过 `bindLegacyFields()` 将状态机内部 snapshot 同步到 MainWindow 的 `fastapi_*` 字段，实现渐进式迁移。所有写入必须通过状态机，禁止直接修改 `fastapi_*` 字段。

---

## 三、通信协议

### 3.1 WebSocket 连接

```
ws://<host>:8000/ws/projects/{project_id}?client_id=<uuid>&author=<name>&password=<token>
```

### 3.2 消息类型

#### 客户端 → 服务器

| type | 用途 |
|------|------|
| `submit_model` | 提交 SAT 全量快照（含 `base_version` 乐观锁） |
| `submit_entity_graph` | 提交增量实体图 |
| `sync_now` | 请求服务器推送最新版本 |
| `ping` | 心跳保活 |

#### 服务器 → 客户端

| type | 用途 |
|------|------|
| `model_saved` | 广播：新版本已保存 |
| `entity_graph_saved` | 广播：增量实体图已保存 |
| `submit_rejected` | 提交被拒绝（版本冲突） |
| `submit_accepted` | 提交已被接受 |
| `presence_snapshot` | 当前在线协作者完整列表 |
| `collaborator_joined` | 有人加入 |
| `collaborator_left` | 有人离开 |
| `pong` | 心跳响应 |

### 3.3 HTTP REST API

| 方法 | 路径 | 用途 |
|------|------|------|
| POST | `/projects` | 创建协作项目 |
| GET | `/projects/{id}/models/latest` | 获取最新版本 |
| POST | `/projects/{id}/models` | HTTP 直存新版本 |
| GET | `/projects/{id}/models/{version}` | 获取指定版本 |
| GET | `/projects/{id}/models/versions` | 分页列出版本历史 |
| POST | `/projects/{id}/entities` | 存储实体图版本 |
| GET | `/projects/{id}/entities/diff?a=&b=` | 两个实体图版本的 diff |

---

## 四、同步机制

### 4.1 手动 Push / Pull（仿 Git 协作模型）

协作面板提供两个按钮，用户手动触发：

- **推送(Push)**：`submitEntityGraphIncremental` 把本地 `pendingEntityChanges` 一次性发到服务器，写为新版本并通过 `entity_graph_saved` 广播给其他 client。每个 ADD 变更附带该 body 的独立 SAT 文本。
- **拉取(Pull)**：`requestFastAPISyncNow` 从服务器拉取远端最新版本，通过 `entity_graph_saved` 广播；接收端走 `applyRemoteEntityGraphIncremental` 增量合并到本地画布。**不 clear 本地画布**。

冲突处理：
- 如果 `pendingRemoteVersion > modelVersion`，Push 按钮提示先 Pull。
- Pull 时如果本地有未推送修改，REMOTE 新增的 body 会增量合并进来，不影响本地 dirty 状态。

### 4.2 定时器策略

| 定时器 | 间隔 | 用途 |
|--------|------|------|
| `fastapiHeartbeatTimer` | 10s | WebSocket ping 保活 |
| `fastapiReconnectTimer` | 3s | 断线自动重连 |
| `fastapiSyncTimer` | — | 不自动触发，仅重连时一次性 `requestFastAPISyncNow` |
| `fastapiPublishTimer` | — | 已废弃，不使用 |

---

## 五、增量协作方案

### 5.1 实体变更追踪

MainWindow 维护基于 Entity UUID 的变更追踪系统：

```cpp
enum class EntityChangeType { ADD, REMOVE, MODIFY };

struct EntityChange {
    QString uuid;           // 实体唯一标识
    QString name;
    QString entityType;
    EntityChangeType changeType;
    int entityIndex;
    qint64 timestamp;
    // ADD 变更有效：body 单独序列化的 SAT 文本
    QString sat;
};
```

追踪生命周期：

```
MainWindow 构造 → beginEntityChangeTracking()  // 一次性开启
addEntity / removeEntity → recordEntity*()     // 期间累积
Push → submitEntityGraphIncremental 提交后清空
```

### 5.2 实体图导出

`exportEntityGraphToJson()` 将当前场景序列化为图结构：

```json
{
  "nodes": [
    {
      "id": "<uuid>",
      "labels": ["<entity-type>"],
      "props": {
        "index": 0, "name": "...",
        "operatorType": 1, "subOperatorType": 0,
        "transform": [...], "index_base": [...],
        "index_support": [...], "visible": true
      }
    }
  ],
  "rels": [
    { "type": "DEPENDS_ON", "start": "<uuid>", "end": "<dep-uuid>" }
  ]
}
```

### 5.3 增量回放

`applyRemoteEntityGraphIncremental(remoteGraph, remoteChanges, reason)` 工作流：

1. 收集本地 `entity_tree` 里所有 uuid，构成 `localUuids: QSet<QString>`
2. 解析 `content.changes`，对每个变更：
   - **ADD**：uuid 不在 `localUuids` 时，用变更携带的 `sat` 文本创建临时文件，调 `acis_restore_entity_list` 恢复为 `ENTITY_LIST`，取第一个 body 调 `addEntity` 注册到本地 tree
   - **REMOVE**：跳过，保留本地未推送的实体
   - **MODIFY**：当前不处理
3. 不调用 `clear()`，不删除任何本地 body

### 5.4 SAT 导出来源修正（Bug 修复记录）

**问题**：pull 后再 push，远端看不到之前的实体。

**根因**：`exportCurrentModelToSat` 使用 `acis_get_noattrib_toplevel_active_entities()` 从 ACIS API 获取实体列表，但 `api_restore_entity_list` 恢复的实体没有正确进入 ACIS 的 active model，导致 ACIS API 返回的实体不完整。

**修复**：改用 `Window::getEntityList()` 从 Qt 的 `entity_tree` 直接获取实体列表，不再依赖 ACIS API。`entity_tree` 是 Qt 层维护的列表，始终与场景同步。

```cpp
// 修复前（有问题）
ENTITY_LIST el;
acis_get_noattrib_toplevel_active_entities(el);  // pull 后实体可能不在 active model 里

// 修复后
ENTITY_LIST el = curWindow->getEntityList();     // 从 entity_tree 获取，准确性有保证
```

---

## 六、Storage Bridge 架构

### 6.1 设计动机

C++ 客户端和 Neo4j 之间需要中间层：ACIS 内核是 C++ 独占的（SAT ↔ Entity 转换必须在 C++ 侧完成），而 FastAPI (Python) 不能直接调用 ACIS API。

### 6.2 双模式可执行文件

`CAD_DB.exe` 支持两种运行模式：

| 模式 | 启动参数 | 角色 |
|------|----------|------|
| GUI 客户端 | (无参数) | 完整 CAD 应用程序 |
| Bridge 服务 | `--storage-bridge` | 无头 HTTP 服务，监听 8100 端口 |

### 6.3 Bridge REST API

| 方法 | 路径 | 说明 |
|------|------|------|
| GET | `/health` | 健康检查 |
| POST | `/projects` | 创建项目 |
| GET | `/projects/{id}` | 获取项目详情 |
| POST | `/projects/{id}/models` | 创建模型版本（SAT→ACIS→Neo4j） |
| GET | `/projects/{id}/models/latest` | 获取最新版本（Neo4j→ACIS→SAT） |
| GET | `/projects/{id}/models/{version}` | 获取指定版本 |
| GET | `/projects/{id}/models/versions` | 列出版本 |

### 6.4 乐观锁并发控制

创建模型版本时校验 `base_version`（客户端声明）与服务器最新版本是否匹配，不匹配返回 409 Conflict。

---

## 七、在线协作者管理 (Presence)

`ProjectSyncManager` 管理 WebSocket 连接池，每个项目一个 `asyncio.Lock` 用于写序列化。

MainWindow 维护 `fastapi_collaborators: QHash<QString, QString>` (client_id → author)，实时处理 `collaborator_joined` / `collaborator_left` 增量更新，UI 面板展示在线成员。

---

## 八、存储模式对比

| 模式 | 存储位置 | 协议 | 增量支持 | 多人协作 |
|------|----------|------|----------|----------|
| SAT 文件 (ACIS) | 本地 .sat 文件 | 文件系统 | ❌ | ❌ |
| Neo4j 全量 | Neo4j 图数据库 | Bolt | ❌ | ❌ (单机) |
| Neo4j 增量 | Neo4j 图数据库 | Bolt | ✅ | ❌ (单机) |
| FastAPI 远程 | Neo4j (via Bridge) | HTTP + WS | ✅ | ✅ |
| PostgreSQL | PostgreSQL 表 | libpq | ❌ | ❌ (实验) |

---

## 九、部署架构

```
docker run neo4j:5.15 (端口 7687/7474)
     +
CAD_DB.exe --storage-bridge (端口 8100)
     +
uvicorn app.main:app (端口 8000)
     +
CAD_DB.exe (GUI 客户端) × N
```

启动脚本：`deploy/start_dbcad_fullstack.cmd`

---
