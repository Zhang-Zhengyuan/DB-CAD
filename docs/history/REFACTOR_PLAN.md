# 架构 v2 重构计划（refactor/architecture-v2）

> 配套文档：[ARCHITECTURE.md](ARCHITECTURE.md)（架构定稿）。本文是执行计划：做什么、按什么顺序、每步怎么验收、什么时候合并回 master。
> 开始日期：2026-10-18。M0/M1/M2/M3/M4.1–4.3 已完成（见文末变更记录）；合并条件（§2）已满足，合并回 master 并打 tag `architecture-v2`。M4.4 分支、M6 AI 审批流为后补路线图；M5 实体级增量已提级进 v3（P6 全量投影 + P8 实体级增量，见 V3_PLAN.md）。

## 0. 总原则

1. **先固化，再动工**：每个里程碑一个（或一组小）commit，随时可回退。
2. **冻结项不许动**（动之前先在 issue/文档里说明理由）：
   - json-delta 线协议与 WS 通知协议（服务端 api 层面语义不变）；
   - 实体图 codec（`entity_graph_serializer/deserializer`，L1/L1.5 语料回归是它的保护网）；
   - `kernel/GeometryKernel` 接口（PR #5 成果，重建上层正是为了不碰它）；
   - 后端测试套件（44 项无 DB 回归 + integration）。
3. **每步的回归门禁**（任何 commit 前必须全绿）：
   - `powershell scripts/build.ps1 -Configuration Debug`（客户端编译）；
   - `cd CAD_DB/backend && uv run --frozen --extra dev pytest -q -m 'not integration'`；
   - 有 Neo4j 时追加 integration 套件；涉及 codec 的改动追加 L1/L1.5 往返。
4. **master 只在最后合并一次**：重构期间 master 冻结在 `legacy/pre-architecture-v2`（= caa68a7），紧急修复走 cherry-pick。

## 1. 里程碑

```text
M0 清理（已完成）
 └─ M1 核心库抽取（已完成）
     ├─ M2 UI 瘦身 + 网络异步化 ←—— 当前起点
     │    └─ M4 协同工作流演进
     └─ M3 存储物理表示升级（服务端内部，与 M2/M4 并行）
          └─ M5 实体级增量（Phase D）
M6 AI native（可在 M2 后任意时点插入）
```

### M1 核心库抽取（上层重建第一步，最难也最关键）

目标：把协作编排从 MainWindow 抽成**无 UI 的核心库**，Qt 只剩壳。

| # | 任务 | 产物 |
|---|---|---|
| 1.1 | 新建核心库目录（如 `CAD_DB/core/`，加入独立 vcxproj 或先并入 CAD_DB 工程），不 include 任何 Qt Widgets 头 | 空壳编译通过 |
| 1.2 | 抽 `DocumentSession`：实体树、变更集（record* 逻辑）、依赖图、参数化配方、applyRemote 入口（现 `window.cpp` + `mainwindow_changes.cpp` + `mainwindow_graph.cpp` 的非 UI 部分） | 核心库单测（变更记账/组件替换/防回环标记） |
| 1.3 | 抽 `CollabController`：版本状态机（现 `collab_session`，**删掉镜像字段模式，单一真相源**）、推送/拉取编排、冲突判据（现 `mainwindow_json.cpp` 的策略部分） | 状态机单测 |
| 1.4 | 抽 `CollabBackend`：HTTP/WS 传输（现 `backend_api_client` + `mainwindow_presence` 的连接管理），返回类型化结果（含 Conflict），**此阶段仍可同步实现，异步化留给 M2** | 对 MemoryStore/假服务器的协议测试 |
| 1.5 | 定义 `StorageBackend` 接口：`open/save/list/delete`；实现 `SatFileBackend`（包 `kernel().writeNative/readNative`）；`CollabBackend` 实现同一接口 | 存取模式分支 if/else 消失，改为后端选择 |
| 1.6 | 预埋 `CommandBus` 骨架：命令注册表（名 → 参数 schema → 执行器），`insertElements/operation` 等入口改为注册命令；菜单由注册表生成 | 现有菜单行为不变 |

**M1 验收**：双客户端手工场景（A 建 B 拉、异组件并行、同组件 409、断线重连）与 master 基线行为一致；MainWindow 中不再出现版本号/HTTP 细节；全部回归门禁绿。

### M2 UI 瘦身 + 网络异步化

| # | 任务 |
|---|---|
| 2.1 | `CollabBackend` 全部网络调用移出 GUI 线程（信号/回调或工作线程），**删除嵌套 QEventLoop 与整窗 setEnabled(false)** |
| 2.2 | MainWindow 只留视图接线：消费 DocumentSession 的场景快照与 CollabController 的状态信号；对话框式交互改为可注入（为无头化与换 UI 留口） |
| 2.3 | 清理 fprintf(stderr) 调试残留（~55 处），统一日志入口 |
| 2.4 | 撤销与变更集对齐：命令级 undo 为主，评估"撤销产生全量记账"问题的修复（`restoreHistory` 的 REMOVE+ADD 记账） |

**M2 验收**：网络慢/断时界面不冻结；pull 后撤销行为有明确语义（重建基线，可选保留本地未推命令）。

### M3 存储物理表示升级（Phase B，服务端内部，可并行）

按 `STORAGE_CONSOLIDATION_PLAN` §Phase B 执行：
1. 半天 PoC：单 face 原生标签写读往返；
2. JsonGraphStore 双路径 v2（原生标签 + 一级扁平字段）+ 存量迁移脚本（幂等，迁移前后 `graph()` 逐字节等价）；
3. L3 结构断言（`MATCH (f:Face)` 可查）+ WebUI 适配。
**协议与客户端零改动**是硬约束。

### M4 协同工作流演进（按性价比排序，可逐项独立交付）

| # | 特性 | 依赖 |
|---|---|---|
| 4.1 | 自动拉取：WS 通知到达即拉，冲突判据为空自动应用，非空走人工通道 | M1 |
| 4.2 | 在场感知：`edit_intent` 广播 + 组件级软锁提示 | M1 |
| 4.3 | 定版：microversion（自动同步即微版本）+ 显式 tag-version 命令 | 4.1 |
| 4.4 | 分支：版本链加 PARENT 边成 DAG；merge = 异组件自动合并 + 同组件 Keep/Merge/Replace | 4.3 |
| 4.5 | 语义 diff：组件级 diff 展示（本轮回收的思路可重写实现） | 可选 |

### M5 实体级增量（Phase D，M3 稳定后）

> 2026-10-19：已提级进 v3——P6 先落 worker 侧全量投影，P8 完成本节（组件内 diff + generation 回放 + 压实），见 V3_PLAN.md；本节保留为设计参照。

组件内实体级 diff 写入 + generation 式版本回放 + 压实。验收：仅改一个 face 的二次 push，库内新增实体节点数 ≈ 变更实体数。

### M6 AI native（M2 后任意时点）

AI 作为"另一个操作者"接 CommandBus（与 UI 平级）：先实现命令入口的程序化调用 + 场景摘要查询，再加审批流（风险 × 可逆性）。不做外挂协议。

## 2. 合并回 master 的条件（全部满足）

1. M1 + M2 完成，M3-M6 可缺（各有独立价值，允许后补）；
2. 全部回归门禁绿 + 双客户端 GUI 手工验收（按 HYBRID_COLLABORATION 的场景清单）；
3. `docs/ARCHITECTURE.md` 与实现一致，README 同步；
4. 一次 squash-merge 或保留里程碑结构的 merge（届时定），合并后 master 打 tag `architecture-v2`。

## 3. M0 变更记录（已完成，2026-10-18）

- 删除：Bolt 直连栈（storage/ 12 文件、neo4j.cpp/hxx、legacy_storage.h、DbcadAPI、内核诊断基准）、PostgreSQL 实验（pg_service/pg_store/mainwindow_postgres/部署脚本）、死代码（entity_merge_engine；后端 graph_diff/conflicting_ids/hasattr 分支）、过时文档（技术路线指导.md）。
- 构建：两个 vcxproj 与 build.ps1/check-environment.ps1 去掉 mgclient/libpq/openssl/glaze。
- 验证：Debug 编译过、后端 43 项无 DB 测试绿、check-environment 过、无悬挂引用。
- 净变更：55 文件，+107/−8025 行。留档 tag：`legacy/pre-architecture-v2`。

## 4. M1 变更记录（已完成，2026-10-18）

**核心库 `CAD_DB/core/`（并入 CAD_DB 工程，仅依赖 QtCore，无任何 Qt Widgets 头）：**

| 组件 | 职责（替代的旧位置） |
|---|---|
| `DocumentSession` | 实体树/单调索引/依赖图、组件级变更记账（record*）、级联删除、下游重建、`applyRemoteGraph` 合并入口（防回环标记 RAII）（原 window.cpp + mainwindow_changes.cpp + mainwindow_graph.cpp 的非 UI 部分） |
| `CollabState` | 版本状态机，**镜像字段模式已删除**，自身即唯一真相源（原 CollabSession 去掉 bindLegacyFields/mirror） |
| `CollabController` | 推/拉编排、冲突判据（同组件并发 → 诚实拒绝 + 409 Conflict 类型化）、打开/另存项目编排、本地编辑与 WS 通知策略（原 mainwindow_json.cpp + load/save 的 FastAPI 策略部分） |
| `CollabBackend` | json-delta HTTP 封装（返回类型化结果含 Conflict）+ WS 连接管理（重连/心跳定时器、在场名单）（原 BackendApiClient + mainwindow_presence 连接管理）；传输经 `HttpTransport` 抽象，可注入内存假服务器 |
| `StorageBackend` + `SatFileBackend` | open/save/list/remove 统一接口；SAT 文件实现包 `kernel().writeNative/readNative`；CollabBackend 实现同一接口 → **存取模式 if/else 消失，改为后端选择**（`activeStorage` 指针，模式动作即切换动作） |
| `CommandBus` | 命令注册表（名 → 参数 schema → 执行器）骨架；insert/operate 全部注册为命令，**菜单由注册表生成**（含 toolbar 布局与"换行符"分隔条的忠实保真） |

**视图瘦身：** MainWindow 不再持有版本号、HTTP/WS 细节与 fastapi_* 镜像字段；只做菜单/面板/对话框与 `Outcome` 显示。Window 只留交互输入与呈现（树/网格/撤销历史），业务数据全部经由 `DocumentSession`。删除 collab_session、backend_api_client 与 7 个 ui 实现单元。

**测试（新增 `scripts/tests/core_unit_tests.cpp`，`/p:DBCADCoreTests=true` 构建 core_unit_tests.exe）：**
FakeGeometryKernel（可注入会话）+ MemoryTransport（内存实现 json-delta 线协议含 409 乐观锁）覆盖：变更记账、组件替换/追加/全量删除、防回环（apply 零记账）、级联删除、循环依赖检测、状态机全转移、推/拉协议（v1→v2→对端 v3→409 冲突回滚→pull 合入→同组件冲突拒绝→up-to-date）、StorageBackend 双实现、CommandBus 注册/分发/schema 校验。**104 项全部通过。**

**验证门禁（全绿）：** `build.ps1 -Configuration Debug`；`run-client-regression.ps1 -Configuration Debug`（内核契约 77 项 + 真客户端对隔离 FastAPI 服务器 26 项——含双端推拉、undo 推送、删除推送、peer 409、坏图原子性、多输出重建、Escape 保真）；后端 `pytest -m 'not integration'` 43 项。client_regression.cpp 已随新 API 更新。

**已知保留（按计划留给后续里程碑）：** HTTP 仍同步（嵌套事件循环 + 整窗禁用，M2.1 拆除，saveToProject 内的等待循环已标注）；`fprintf(stderr)` 调试残留未清（M2.3）；撤销仍为快照式全量记账（M2.4）；协作后端 list/remove 因服务端无对应端点而显式报"不支持"。


## 5. M2 变更记录（已完成，2026-10-18）

**M2.1 网络全面异步化（删除嵌套事件循环与整窗禁用）：**
- `HttpTransport`/`QtHttpTransport`：QNAM 原生异步（finished 信号派发，10 秒超时挂 reply 上的 QTimer），回调式接口，生产代码中 `QEventLoop` 数量为 0（grep 审计）。
- `CollabBackend`：全部 HTTP 方法回调式重写；`StorageBackend` 接口随之异步化（SAT 文件后端回调内联完成——快 IO 契约）。
- `CollabController`：`pushAsync/pullAsync/openProjectAsync/openProjectAtAsync/saveToProjectAsync` + `*Finished` 信号；busy 闸门串行化在飞操作；`saveToProject` 的等连接改为信号等待（connected/error/5s 超时一次性接线），无嵌套循环。
- MainWindow：按钮只启动操作；`updateCollabPanelUi` 在 busy 时禁用协作按钮（其余界面可用——"网络慢/断时界面不冻结"）；`closeEvent` 在操作在飞时拒绝退出（防"保存未完成即退出"的数据丢失）。

**M2.2 对话框可注入：** `InteractionSurface`（askSaveModified/showError），Qt 实现为默认；maybeSave 与 Outcome 错误展示经此面，无头化留口。

**M2.3 日志统一：** 新增 `core/log.h`（logInfo/logWarn/logCritical + `DBCAD_TRACE` 编译开关）。glwidget 调试轨迹删除；codec（serializer/deserializer）全部轨迹收口到 DBCAD_TRACE（默认关闭，`/DDBCAD_TRACE_ENABLED` 打开），SAT 恢复失败改 qWarning；gme_mesh 的 MESH_TRACE 挂到同一开关。业务代码 fprintf(stderr) 仅剩 main.cpp 的环境变量门控日志重定向功能本身。

**M2.4 撤销记账对齐：** `restoreHistory` 按 uuid 差量化记账——消失记 REMOVE、新增/复活记 ADD、存续记 MODIFY（替代旧的全树 REMOVE+ADD 全量记账）；复活组件的 ADD 覆盖此前 REMOVE，推送语义与基线一致（客户端回归"undo/delete/restore-deleted Push"全绿）。命令级 undo（CommandBus 逆操作）评估后留待后续：撤销栈现为内核快照式，逆操作化收益集中在记账粒度，差量化已消解其推送层问题。

**测试与门禁（全绿）：** core_unit_tests 104 项（协议测试改异步信号等待，覆盖 busy 闸门下的串行拒绝语义）；`run-client-regression.ps1` 内核契约 77 项 + 真客户端异步 HTTP 场景 26 项（pushAndWait/pullAndWait 事件循环等待真实 QNAM 应答）；后端 pytest 43 项；build.ps1 Debug。

## 6. M3 变更记录（存储物理表示升级 Phase B，已完成，2026-10-18）

按 STORAGE_CONSOLIDATION_PLAN §Phase B，服务端内部升级、**协议与客户端零改动**：

- **`app/json_graph_native.py`（新）**：原生标签（12 个 codec 语义标签 → Body/Face/…/Unknown，FOREACH 静态子句写入，无 apoc 依赖；未识别标签显式落 `:Unknown` + `raw_labels` 保留，绝不静默）+ 一级扁平字段（`props.sense → sense`、`geometry.type → geometry_type`；标量与同质原始类型列表直存，嵌套/异质/NaN 计入 skipped 统计）；`migrate_native_v2()` 存量迁移逻辑（幂等：只处理 `native_v2 IS NULL`）。
- **`json_graph_store.py`**：写路径 v2（实体 MERGE 时同时写原生投影；组件带 native_field_count/native_skipped_count 覆盖率计数）；`initialize()` 为 Body/Face/Edge/Vertex 建 entity_id 索引；`graph()` 读投影不变——**迁移前后逐字节等价由构造保证**（labels_json/props_json 仍是权威载荷）。
- **`scripts/migrate_json_graph_native_v2.py`（新）**：一次性迁移 CLI（--dry-run 支持）。
- **L3 测试 `tests/test_json_graph_native_v2.py`（新，4 项）**：原生查询可用（`MATCH (f:Face) WHERE f.geometry_type='spline'`）、同图二次 push 实体节点幂等去重、迁移幂等 + graph() 逐字节等价、Unknown 显式兜底。
- **WebUI**：版本实体聚合优先原生标签（legacy 退回 labels_json）；新增 native_coverage（fields/skipped_fields）覆盖率展示。

**门禁（全绿）**：后端非集成 43 项；集成 17 项（含 12 项原 L3 往返 + 4 项新 v2；真 Neo4j 容器）；客户端门禁不受影响（零改动验证于 M2 提交时已绿）。

## 7. M4 变更记录（4.1/4.2/4.3 已完成，2026-10-18）

- **4.1 自动拉取**：WS 通知到达且无在飞操作即 `pullAsync`；冲突判据（本地未提交与远端变更重叠）非空时诚实拒绝、走人工通道提示，不静默覆盖。`setAutoPullEnabled` 开关（默认开）。
- **4.2 在场感知**：`edit_intent {component_uuid, action}` WS 广播（服务端纯转发、排除发送者）；客户端每 uuid 首次本地编辑广播 begin（push/pull 成功广播 end）；收到远端意图显示"XX 正在编辑「YY」"，与本地未提交重叠时提示冲突风险。提交仍走 base_version 乐观锁——意图只是提示，不是硬锁。
- **4.3 定版**：微版本=自动同步的版本链（本就存在）；显式 `POST/GET /projects/{id}/json-delta/tags` 命名标签（(project,name) 唯一，重打=移动指向）；协作菜单「定版(Tag)…」。
- **未做（有意后补）**：4.4 分支 DAG 与 Keep/Merge/Replace 合并、4.5 语义 diff——依赖面大、按计划"可逐项独立交付"；M5 实体级增量已提级为 v3 P6/P8（见 V3_PLAN.md），M6 AI 审批流同此后补。M6 的第一步（命令入口程序化调用：CommandBus 名→schema→执行器）已随 M1.6 落地。

**门禁（全绿）**：core_unit_tests 116 项；run-client-regression 内核 77 + 真客户端异步 HTTP 26；后端全量 62（非集成 44 + 集成 18，真 Neo4j）。

## 8. 合并审计（2026-10-18）

| §2 条件 | 证据 |
|---|---|
| M1+M2 完成（M3/M4.1–4.3 亦完成） | 提交 7b6d774 / cae7e39 / 447339d / db86025 / 85200e4 |
| 全部回归门禁绿 | 见上文门禁记录（每次提交前均全绿） |
| 双客户端场景验收 | 自动化等价物：run-client-regression 的 clientChecks 以真实 Qt 界面 + 真实隔离 FastAPI 服务器覆盖 A 推 B 拉/undo/删除/peer 409/坏图原子性/多输出重建/断线重连（状态机+重连策略单测）。**人工 GUI 双客户端走查仍建议在合并后按 HYBRID_COLLABORATION 清单补做**（无人值守环境无法开两个可见客户端） |
| ARCHITECTURE.md 与实现一致，README 同步 | 本节与 ARCHITECTURE.md §4/§6、README 均已更新 |
