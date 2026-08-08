# DBCAD 多人协作系统 — 技术路线与方案全览

> 最后更新：2026-08-03 | 阶段里程碑：客户端-服务端增量同步 + 删除闭环端到端跑通

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
│  ┌──────────────────────┐ ┌──────────────────┐  │
│  │EntityGraphSerializer│ │StorageBridge     │  │
│  │(ACIS → JSON)        │ │(C++ → Neo4j)     │  │
│  └──────────────────────┘ └──────────────────┘  │
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
| `submit_entity_graph` | 提交增量实体图（含 `content.delta_bodies[]` 单 body SAT + `fullSat`） |
| `sync_now` | 请求服务器推送最新版本 |
| `ping` | 心跳保活 |

#### 服务器 → 客户端

| type | 用途 |
|------|------|
| `model_saved` | 广播：新版本已保存（trigger=manual-push/sync_now/...） |
| `entity_graph_saved` | 广播：增量实体图已保存 |
| `submit_rejected` | 提交被拒绝（版本冲突） |
| `submit_accepted` | 提交已被接受 |
| `presence_snapshot` | 当前在线协作者完整列表 |
| `collaborator_joined` | 有人加入 |
| `collaborator_left` | 有人离开 |
| `pong` | 心跳响应 |

### 3.3 增量提交 payload (`submit_entity_graph`)

```json
{
  "type": "submit_entity_graph",
  "project_id": "...",
  "author": "...",
  "base_version": 1,
  "request_id": "...",
  "content": {
    "delta_bodies": [
      {
        "uuid": "868930b8-587d-4e0f-b442-3353cf3045b1",
        "op": "ADD",
        "name": "立方体",
        "sat": "<单 body SAT 文本 ~3.6KB>"
      }
    ],
    "deleted_uuids": [],
    "fullSat": "<全量 SAT ~3.6KB>",
    "_entity_graph_version": 1
  }
}
```

### 3.4 HTTP REST API

| 方法 | 路径 | 用途 |
|------|------|------|
| POST | `/projects` | 创建协作项目 |
| GET | `/projects/{id}/models/latest` | 获取最新版本 |
| POST | `/projects/{id}/models` | HTTP 直存新版本 |
| GET | `/projects/{id}/models/{version}` | 获取指定版本 |
| GET | `/projects/{id}/models/versions` | 分页列出版本历史 |
| POST | `/projects/{id}/entities` | 存储实体图版本 |
| GET | `/projects/{id}/entities/diff?a=&b=` | 两个实体图版本的 diff |
| GET | `/projects/{id}/entities/{version}` | 获取实体图版本 |

---

## 四、同步机制

### 4.1 手动 Push / Pull（仿 Git 协作模型）

协作面板提供两个按钮，用户手动触发：

- **推送(Push)**：`submitEntityGraphIncremental` 把本地 `pendingEntityChanges` 一次性发到服务器，写为新版本并通过 `entity_graph_saved` 广播给其他 client。每个 ADD 变更附带该 body 的独立 SAT 文本。
- **拉取(Pull)**：`requestFastAPISyncNow` 从服务器拉取远端最新版本，通过 `entity_graph_saved` 广播；接收端走 `applyRemoteIncrementalDelta` 增量合并到本地画布。**不 clear 本地画布**。

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

## 五、增量协作方案（核心）

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

### 5.2 单 body SAT 序列化

`serializeBodyToSat(ENTITY* body)` 把单个 body 序列化为 SAT 文本：

```cpp
ENTITY_LIST el;
el.add(body);
QTemporaryFile tempFile(QDir::tempPath() + "/dbcad_delta_XXXXXX.sat");
tempFile.setAutoRemove(false);
if (!tempFile.open()) return QString();
const QString tempPath = tempFile.fileName();
tempFile.close();   // 释放 Qt 排他锁
FILE* f = fopen(tempPath.toStdString().c_str(), "wb");
api_set_int_option("sequence_save_files", 1);   // 写 schema-version header
api_save_entity_list(f, true, el);              // text_mode=true
fclose(f);
// 读字节流作为字符串返回
```

要点：
- **必须先 close() QTemporaryFile 再 fopen**——Qt 的排他锁会让 ACIS 后续 `fopen` 失败（"打开文件失败"根因）
- **`sequence_save_files=1`** 让 ACIS 写 SAT schema header（schema-version / product-id），否则接收端 `acis_restore_entity_list` 读不到合法 header 直接 abort

### 5.3 增量回放（接收端 `applyRemoteIncrementalDelta`）

**完整工作流**（这是 2026-07-31 阶段里程碑跑通的端到端路径）：

```cpp
1. 收集本地 entity_tree 里所有 uuid，构成 localUuids: QSet<QString>
2. 解析 content.delta_bodies[] 和 content.deleted_uuids
3. 对每个 delta_bodies[i]（op=ADD）：
   a) uuid 已在 localUuids → skip
   b) 写远端 SAT 到 QTemporaryFile（setAutoRemove(false)，先 close 释放排他锁）
   c) fopen("rb") → acis_restore_entity_list(el, f, 2, 0, 1)
      注意：不要在外面套 API_NOP_BEGIN/END——wrapper 内部已 API_BEGIN/END，
      嵌套会让 ACIS 内部状态机错乱，导致 entity 不进 active list 也不进 el 数组
   d) 取 el[0]（restore 后的第一个 ENTITY*）
   e) curWindow->addEntity(el[0], tr("远端增量%1").arg(idx).toStdString(), -1)
   f) entityIndexToUuid[idx] = uuid   // 注册到本地索引
4. 对每个 deleted_uuids[i]：如果本地有，调用 ACIS api_del_entity + 从 entity_tree 删除
5. 不调用 clear()，不删除任何本地 body（本地未提交修改完整保留）
```

关键点逐条说明：

| 步骤 | 关键点 |
|---|---|
| QTemporaryFile | `setAutoRemove(false)`，**close 后再 fopen**——Qt 排他锁 |
| acis_restore_entity_list | **不要嵌套 API_NOP_BEGIN/END**（详见第六节 bug 修复记录） |
| el[0] | restore 成功后 el.count() >= 1，第一个 ENTITY* 即 BODY |
| addEntity | isApplyingRemote 标志位让 addEntity 不再记录增量 change（避免回环推送） |

### 5.4 SAT 导出来源修正（已修复）

**问题**：pull 后再 push，远端看不到之前的实体。

**根因**：`exportCurrentModelToSat` 使用 `acis_get_noattrib_toplevel_active_entities()` 从 ACIS API 获取实体列表，但 `api_restore_entity_list` 恢复的实体没有正确进入 ACIS 的 active model，导致 ACIS API 返回的实体不完整。

**修复**：改用 `Window::getEntityList()` 从 Qt 的 `entity_tree` 直接获取实体列表，不再依赖 ACIS API。

```cpp
// 修复前（有问题）
ENTITY_LIST el;
acis_get_noattrib_toplevel_active_entities(el);  // pull 后实体可能不在 active model 里

// 修复后
ENTITY_LIST el = curWindow->getEntityList();     // 从 entity_tree 获取，准确性有保证
```

### 5.5 增量同步端到端验证（2026-07-31）

测试场景：A 端 push 立方体 → B 端 pull → B 端本地加球体 → B 端 push → A 端自动收到。

**A 端日志关键帧**（清理后）：
```
[Collab] handleFastAPISyncMessage type= entity_graph_saved ...
[Collab] shouldApply decision: isPullResponse= false trigger= manual-push final= false
[Collab] onCollabPullButtonClicked: requesting sync_now
[Collab] handleFastAPISyncMessage type= model_saved hasTrigger= sync_now
[Collab] model_saved try incremental delta path, + 1  - 0
```

**B 端日志关键帧**（清理后）：
```
[FINGERPRINT-2026-07-31] applyRemoteIncrementalDelta CALLED
[Collab][Delta] >>> applyRemoteIncrementalDelta ENTER delta_bodies.size=1 deleted_uuids.size=0
[Collab][Delta]   body[0] temp SAT ready, path= ... bytes=...
[CreateMeshFromEntity] e=BODY
[CreateMeshFromEntity] direct topology: lumps=1 shells=1 faces=6
[Window::updateMeshData] display_data=1 totalFaces=6
[GLWidget::uploadMeshDataToGpu] uploaded face=216 floats  ← 立方体 GPU 上传成功
[Collab][Delta] applyRemote: + 1 / 1   - 0 / 0
[Collab] remoteApply | Connected_Idle | v=1
```

### 5.6 删除闭环端到端验证（2026-08-03）

测试场景：A 端 push 立方体 → A 端删除立方体 → B 端 Pull → B 端 tree widget 同步删除。

**A 端日志关键帧**：
```
[Collab][Delta] deleteEntityByIndexForCollaboration ENTER index=1
[Collab]    # 删除只入 pendingEntityChanges，未自动 publish
[Collab][Delta] submitIncrementalDelta ENTER pendingEntityChanges.size=1 (ADD=0 REMOVE=1)
[Collab][Delta] submitIncrementalDelta SENT delta_bodies=0 deleted=1
[Collab]    submitOk v=3
```

**B 端日志关键帧**：
```
[Collab] onCollabPullButtonClicked: requesting sync_now
[Collab][Delta] applyRemoteIncrementalDelta ENTER delta_bodies.size=0 deleted_uuids.size=1
[Collab][Delta]   del[0] uuid=... localHas=true
[Collab][Delta]   afterDelete: updateMeshData + updateTreeWidget called, entityTree.size=0
[Collab][Delta] <<< applyRemoteIncrementalDelta EXIT appliedDelete=1 skippedDelete=0
[Collab] remoteApply | Connected_Idle | v=3
```

**关键不变量**：
- 删除后**没有自动 Push**——本地 `modelVersion` 在删除那刻保持不变
- 删除被正确记到 `pendingEntityChanges`（REMOVE=1），点 Push 后才上去
- 远端 Pull 走 `applyRemoteIncrementalDelta` → `appliedDelete=1` → `updateTreeWidget` 重绘

---

## 六、关键 Bug 修复记录

### 6.1 B 端 `acis_restore_entity_list` 后 `el.count()=1` 但 `activeList.count()=0`（2026-07-31）

**症状**：
- B 端 pull 后进程**不崩**，applyRemoteIncrementalDelta EXIT success
- 版本号正常升 v=1
- 但画布空，GLWidget::uploadMeshDataToGpu 没被调用
- 旧版本 log 显示 `activeList.count=0` → `no BODY found in activeList, skip`（该 fallback 路径已在 2026-08-03 清理中移除）

**尝试 1**：放弃 `el[]`，改走 `acis_get_noattrib_toplevel_active_entities`——失败，因为 active list 本身是空的，绕了一圈还是 0。

**真正根因**：在 `acis_restore_entity_list(el, f, 2, 0, 1)`（wrapper 函数，内部已 `API_BEGIN/END`）**外面又嵌套了 `API_NOP_BEGIN/END`**——ACIS 内部状态机在嵌套 API_NOP 时进入异常状态，导致 restore 完后 entity 进了 ref/history 列表而**没进 active list**，也没进 `el` 内部数组（实测 `el.count()=1` 但解引用 `el[0]` 时同样异常）。

**修复**：去掉外面嵌套的 `API_NOP_BEGIN/END`，只保留 try/catch 守 C++ 异常，让 wrapper 内部自己的 `API_BEGIN/END` 单独工作。

```cpp
// 修复前（bug）
try {
    API_NOP_BEGIN;
    acis_restore_entity_list(el, f, 2, 0, 1);
    API_NOP_END;
    restoreOk = true;
} catch (...) { ... }

// 修复后
try {
    acis_restore_entity_list(el, f, 2, 0, 1);  // wrapper 内部 API_BEGIN
    restoreOk = true;
} catch (...) { ... }
```

### 6.2 删除-不再自动推（行为契约 2026-08-03）

**问题**：早期 `deleteEntityByIndexForCollaboration` 在 ACIS 删除后会**自动**调度 `publishFastAPIAutoSnapshot`，导致：

- 删除一个 body 立刻把远端版本拉过来一把冲掉本地画布（远端还没该 REMOVE）
- 与添加实体的"按 Push 提交"行为契约不一致
- 调试期日志里 `entity_graph_saved ... parking as pending` 频繁出现，浪费一次往返

**修复**：删除只把 REMOVE 写入 `pendingEntityChanges`，**不调度任何自动 publish**。与添加完全对齐，全部由用户点 Push 按钮统一提交。

```cpp
// 修复后：删除只入 pendingEntityChanges，由 Push 按钮统一提交
for (int i = 0; i < (int)toRemoveIndices.size(); ++i) {
    recordEntityRemoved(toRemoveIndices[i]);   // 同步 entityIndexToUuid
}
// 之后不再调 publishFastAPIAutoSnapshot / scheduleFastAPIAutoPublish
```

**附带教训**（6.1 + 6.2 共享）：
- `QTemporaryFile::setAutoRemove(false)` + 先 `close()` 再 `fopen` 是必须的
- **不要在 ACIS wrapper 外面再嵌套 `API_NOP_BEGIN/END`**——wrapper 已经做了
- 调试时在每个 ACIS 调用前后加 `fprintf(stderr, ...)` 写 stderr，比 `qDebug()` 更可靠（崩溃时 qDebug 消息可能未 flush，stderr 直接落盘）

---

## 七、Storage Bridge 架构

### 7.1 设计动机

C++ 客户端和 Neo4j 之间需要中间层：ACIS 内核是 C++ 独占的（SAT ↔ Entity 转换必须在 C++ 侧完成），而 FastAPI (Python) 不能直接调用 ACIS API。

### 7.2 双模式可执行文件

`CAD_DB.exe` 支持两种运行模式：

| 模式 | 启动参数 | 角色 |
|------|----------|------|
| GUI 客户端 | (无参数) | 完整 CAD 应用程序 |
| Bridge 服务 | `--storage-bridge` | 无头 HTTP 服务，监听 8100 端口 |

### 7.3 Bridge REST API

| 方法 | 路径 | 说明 |
|------|------|------|
| GET | `/health` | 健康检查 |
| POST | `/projects` | 创建项目 |
| GET | `/projects/{id}` | 获取项目详情 |
| POST | `/projects/{id}/models` | 创建模型版本（SAT→ACIS→Neo4j） |
| GET | `/projects/{id}/models/latest` | 获取最新版本（Neo4j→ACIS→SAT） |
| GET | `/projects/{id}/models/{version}` | 获取指定版本 |
| GET | `/projects/{id}/models/versions` | 列出版本 |

### 7.4 乐观锁并发控制

创建模型版本时校验 `base_version`（客户端声明）与服务器最新版本是否匹配，不匹配返回 409 Conflict。

---

## 八、在线协作者管理 (Presence)

`ProjectSyncManager` 管理 WebSocket 连接池，每个项目一个 `asyncio.Lock` 用于写序列化。

MainWindow 维护 `fastapi_collaborators: QHash<QString, QString>` (client_id → author)，实时处理 `collaborator_joined` / `collaborator_left` 增量更新，UI 面板展示在线成员。

---

## 九、ACIS Entity Graph 序列化层（entity_graph_serializer）

### 9.1 设计动机

现有协作的"推送"和"拉取"路径主用 **SAT 全量文本**：
- 推送：客户端导出完整 ACIS 模型为 SAT 字符串 → 后端存 Neo4j → 通过 WebSocket 广播 SAT
- 拉取：远端 `entity_graph_saved` 事件携带 `content.sat`，接收端走 `applyRemoteIncrementalDelta` 单 body 增量合并

为此引入独立的 **ACIS Entity Graph JSON 序列化层**（`entity_graph_serializer.h/cpp`），把 ACIS 拓扑 + 几何信息直接序列化成 JSON，与后端 `neo4j_entity_store.py` 的 `entity_graph_version / entity_node` 图结构对齐。

### 9.2 JSON 格式

```json
{
  "nodes": [
    { "id": "<uuid>", "labels": ["body"], "props": { "entity_type": "body", "transform": {...} } },
    { "id": "n_0x..._FACE_ID", "labels": ["face"], "props": { "sense": 1, "geometry": {"type": "plane", "root_point": [...], "normal": [...], "uv_range": [...]} } },
    { "id": "n_0x..._EDGE_ID", "labels": ["edge"], "props": { "geometry": {"type": "straight", "root_point": [...], "direction": [...], "range": [...]} } }
  ],
  "rels": [
    { "type": "body_lump", "start": "<body-uuid>", "end": "n_0x..._LUMP_ID" },
    { "type": "face_geometry", "start": "n_0x..._FACE_ID", "end": "n_0x..._PLANE_ID" }
  ]
}
```

### 9.3 双轨协作语义

推送（Push）：
1. `serializeBodyToSat(body)` → 单 body SAT 字符串加入 `delta_bodies[i].sat`
2. `exportCurrentModelToSat()` → 全量 SAT 字符串加入 `content.fullSat`（fallback）
3. WebSocket 发 `submit_entity_graph`，`request_id` 一致

拉取（Pull）：
1. 收到 `entity_graph_saved` 是自己的提交应答 → `session.onSubmitAccepted()`
2. 是 sync_now 应答 / 其他 client broadcast → 走 `applyRemoteIncrementalDelta(content.delta_bodies, content.deleted_uuids)`
3. 对每个 ADD 变更：写 SAT 到临时文件 → `acis_restore_entity_list` → `el[0]` → `addEntity`

### 9.4 当前阶段状态

- ✅ **单 body SAT 序列化** `serializeBodyToSat()`：完整实现，与 push 端对齐
- ✅ **增量回放** `applyRemoteIncrementalDelta()`：2026-07-31 端到端跑通
- ✅ **UUID 去重 + 本地索引映射**：`entityIndexToUuid[idx] = uuid`
- ✅ **Entity Graph JSON 序列化** `serializeACISEntityGraph()`：完整实现 BODY / LUMP / SHELL / FACE / LOOP / COEDGE / EDGE / VERTEX / TRANSFORM 遍历
- ✅ **Entity Graph 反序列化** `deserializeACISEntityGraph()`：真正实现（2026-08-05），两遍遍历重建 ACIS 实体

---

## 十、存储模式对比

| 模式 | 存储位置 | 协议 | 增量支持 | 多人协作 |
|------|----------|------|----------|----------|
| SAT 文件 (ACIS) | 本地 .sat 文件 | 文件系统 | ❌ | ❌ |
| Neo4j 全量 | Neo4j 图数据库 | Bolt | ❌ | ❌ (单机) |
| Neo4j 增量 | Neo4j 图数据库 | Bolt | ✅ | ❌ (单机) |
| FastAPI 远程 | Neo4j (via Bridge) | HTTP + WS | ✅ | ✅ |
| PostgreSQL | PostgreSQL 表 | libpq | ❌ | ❌ (实验) |

---

## 十一、后续工作

1. ~~`deserializeACISEntityGraph()` 真正实现~~ —— ✅ 已完成（2026-08-05）
2. **`applyRemoteIncrementalDelta` 扩展支持 MODIFY 变更**——目前只处理 ADD 和 REMOVE
3. **冲突可视化**——当两个客户端同时改同一 UUID 的 body 时，目前后到者 reject，前端需提示用户
4. **撤销栈同步**——本地 Undo/Redo 暂未通过协作广播
5. **`api_for_all` / `api_get_entities` 之外的 active entity 收集路径**——保留为 fallback

### 已完成（2026-08-05 清理）
- ✅ **Entity Graph JSON 反序列化** `deserializeACISEntityGraph()` 真正实现（2026-08-05）
  - 两遍遍历：先创建实体再链接拓扑
  - 修复 VERTEX 坐标设置问题
  - 补充缺失的拓扑关系链接
  - 修复 parseInterval 数组索引问题
  - Pull 路径已启用：优先尝试 entity_graph，失败则 fallback 到 SAT

### 已完成（2026-08-03 清理）
- ✅ 删除-不再自动推：删除只入 `pendingEntityChanges`，由 Push 按钮统一提交
- ✅ 删除闭环端到端（带 `updateTreeWidget`）跑通，A/B 两端 v=5，画布一致
- ✅ `[Collab][DEBUG]` 调试日志全部收敛到 `[Collab]` / `[Collab][Delta]` 两个语义前缀
- ✅ FINGERPRINT 日期从 2026-07-31 推到 2026-08-03
- ✅ `deleteEntityByIndexForCollaboration` 顶部 6 条"修复要点"长注释合并为 1 行任务说明

---

## 附录 A：日志约定

协作相关日志使用统一前缀（2026-08-03 清理后）：

| 前缀 | 含义 |
|---|---|
| `[CollabSession]` | 状态机转移（按 event 限频去重） |
| `[Collab]` | 通用协作事件（WS 连接、Pull / Push 入口、关键路由决策） |
| `[Collab][Delta]` | 增量同步细节（submitIncrementalDelta / applyRemoteIncrementalDelta 路径） |
| `[CreateMeshFromEntity]` | ACIS → 三角面片 |
| `[GLWidget::*]` | 渲染上传 |
| `[Window::addEntity]` / `[Window::updateMeshData]` | Qt 层实体树管理 |
| `[FINGERPRINT-YYYY-MM-DD]` | 代码指纹——快速确认运行的是最新二进制 |

调试时优先看：
- `[FINGERPRINT-2026-08-03]` 在不在 → 新代码生效没
- `[Collab][Delta]` applyRemoteIncrementalDelta 段 → 增量同步走到哪一步
- `[CollabSession]` 状态转移 → 协作状态机是否正常
- `[Collab] Conflict merge` → 冲突合并路径触发节点
- `[Collab] onCollabPullButtonClicked` / `Pull applyRemoteSat` → Pull 链路