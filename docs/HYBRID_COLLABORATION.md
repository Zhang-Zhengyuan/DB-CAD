# 统一多人协作链路（2026-10-16）

> 本文保留协作收敛阶段记录。后续内核隔离将 UI 的 ACIS 调用迁入适配器，并将本地撤销改为文档/几何快照；当前边界见 [内核隔离与替换指南](KERNEL_ISOLATION.md)，后续验证见 [修复记录](KERNEL_ISOLATION_FIX_20261016.md)。

## 原有设计与本轮结果

学长的主要改动确实位于底层序列化：完整细粒度拓扑/几何与风险组件 SAT 片段共存。协作层此前仍有 Mode0 SAT/Bridge、Mode1 JSON 和独立实体图版本入口，版本读取也未完全统一。

现在只保留 Mode1 写入：

```text
ACIS 顶级实体（稳定 UUID）
  → 共用拓扑/几何序列化 + 每组件 SAT 备份
  → POST /projects/{id}/json-delta（base_version）
  → Neo4j 不可变组件 + JsonGraphVersion 清单
  → GET json-delta / models/{version}
  → 客户端按组件重建与 UUID 合并
```

Push、Ctrl+S、另存新项目、Pull、最新打开、历史打开共用这条链。WebSocket 只通知版本及在线成员，不传输或提交模型。服务端不需要 ACIS，不启动 Bridge。

## 粗细粒度如何选择

顶级节点 `props` 同时包含 `sat_fragment` 和 `fallback`。正常 Body 使用 `on_failure`：先按 JSON 创建骨架、几何和拓扑，失败后使用同节点的 SAT。沿用学长风险识别的组件使用 `sat_fragment`：属性链、未覆盖过程曲面、未知几何等已知有损情况直接经内核恢复，避免“构造成功但信息丢失”。独立面、边等顶级实体也保存稳定 UUID 和 SAT。

所有组件完成恢复后才交给画布；JSON 与 SAT 都失败时不应用部分结果、不推进本地版本。按 UUID 替换组件，保留未修改组件和无冲突本地编辑。不同组件的并发修改可以先 Pull 再 Push；相同 UUID 的冲突明确阻止 Pull，需要人工保留/放弃本地修改（本轮未增加自动几何合并或冲突编辑器）。

Push 仅序列化并发送当前变更组件，SAT 不在 changes 元数据中重复携带。服务端以项目写锁和 base_version 拒绝过期提交；新版本清单复用未变化组件。修复了同一子节点 ID 再次提交导致旧关系重复保留的问题。删除最后一个实体也可提交空模型。

## 删除与保留

已删除 Mode0 客户端模式开关、旧 WS 模型提交/ACK/全量发布和替换流程、C++ Storage Bridge 及其项目/模型/版本/delta 实现、Python Bridge 适配器、独立 EntityGraph 版本写入、Bridge 专用 delta 计算器、旧启动配置/进程逻辑、无效自动跟随选项和对应旧教学补丁/测试。构建清单和部署同步更新。

仍由菜单实际调用的单机 Neo4j 保存/恢复/历史、ACIS 文件存取与 PostgreSQL 实验功能保留。管理 WebUI 中旧 Mode0/EG 数据的只读审计保留，并标为历史数据；它们不能再通过协作接口写入。

## 历史数据与升级

- 旧 Mode1 / JsonGraphVersion 历史继续读取。原先无 SAT 的旧组件只能使用已有细粒度信息；服务端无法补造 SAT。新客户端修改后提交该组件时才补上备份。
- 新客户端输出 v2，顶级组件必须有非空 SAT。模型还有未更新的旧组件时，整体清单暂用 v1，组件自己的 SAT 策略照常生效；全部更新后成为 v2。
- 旧 Mode0 / 独立 EG 数据不自动迁移、不删除，也不与新版本号混用。如需继续协作，请从旧版导出 ACIS 文件，在新版导入后另存为新的协作项目。
- 请使用新后端配合新客户端。`-LegacyBridgeMode`、`-BridgeHost`、`-BridgePort` 和旧环境开关已移除。`-KeepExistingServices` 会拒绝复用旧协议后端。

## 已执行验证

- 客户端 Release 编译通过（ACIS 头文件和既有导入符号仍有警告）。
- 后端无数据库回归：44 项通过。覆盖唯一写入入口、SAT 保留、旧历史升级、跨版本 Pull、删除/重新添加、过期提交、独立根组件、WS 通知和旧协议拒绝。
- ACIS 18 组往返：全部通过。覆盖既有几何/拓扑组及新增的纯 JSON 成功、JSON 失败或拓扑被截断后 SAT 恢复、无备份拒绝、损坏 SAT 拒绝、多组件失败不返回部分结果、独立面/边 UUID 恢复。体积、包围盒、拓扑数和边长按用例比较。这是已测样本的证据，不代表任意模型的全面等价证明。
- 部署脚本语法和项目 XML 检查通过；7 项可执行文件选择回归通过；PrepareOnly 打包和 27 个运行库/Qt 插件检查通过。
- 当前 `127.0.0.1:7687` 未运行 Neo4j，13 项数据库集成测试跳过；本轮未执行双客户端 GUI 操作验收。

日志保存在仓库 `artifacts/collab-*.log`，独立准备包在 `artifacts/collab-package`。下次正常启动会重新同步正式部署包。

## 下一轮手工测试

使用仓库根目录 PowerShell：

```powershell
.\scripts\build.ps1 -Configuration Release
.\CAD_DB\deploy\start_dbcad_two_clients.cmd -Configuration Release
```

1. 两客户端选择 FastAPI 存储，打开相同的新项目名。A 创建方块并 Push；B 收到通知后 Pull，核对模型和版本号。
2. A、B 分别编辑不同 UUID：先各自保留本地修改，一方 Push，另一方 Pull 后确认本地修改未丢，再 Push，最后双方 Pull 一致。
3. 同一 UUID 并发编辑：过期 Push 应被拒绝，Pull 应报告本地冲突，不能悄悄覆盖。
4. 删除一个实体、删除最后一个实体、Ctrl+S、删除后重新添加、多版本未拉取后的 Pull、断线重连逐项验证。
5. 使用扫掠/放样/带属性的模型验证 SAT 路径，核对几何和下游编辑；独立面/边也测试一次。
6. 用“打开”对话框指定历史版本，确认读取同一条链且后续 Push 必须基于最新版本；确认新版本没有产生 Mode0 记录或 Bridge 进程。
7. 检查单机 ACIS 文件、Neo4j 菜单和现有撤销/重做行为。当前 GUI 撤销使用业务元数据与几何快照，不能将它等同于数据库版本回退；旧 Neo4j 增量历史另有 ACIS 专用兼容实现。

后端复测：

```powershell
cd CAD_DB/backend
uv run --frozen --extra dev pytest -q
```

真实 Neo4j 可用后集成用例会运行，不能用跳过结果替代数据库验收。
