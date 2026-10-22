# DBCAD — 多人协作 CAD 数据库系统

服务端权威（server-authoritative）的多人协作 CAD：几何计算在服务端 C++ ACIS worker 执行，FastAPI 网关管理命令队列与版本链，Neo4j 存储 SAT/GLB 资产与实体级图投影，浏览器（React + three.js）是唯一客户端。参照 Onshape 的架构分层，文件被数据库替代。

## 新成员上手

使用 coding agent 前先读 [AGENTS.md](AGENTS.md)；团队按 [Issue → 分支 → 验证 → PR → 人工审核](docs/ai/WORKFLOW.md) 协作。项目按需导航见 [AI 项目地图](docs/ai/PROJECT_CONTEXT.md)。

完整步骤见 [复现与部署指南](docs/GETTING_STARTED.md)。worker 构建需要授权 ACIS SDK 与配套依赖；源码不包含 SDK。依赖布局见 [依赖说明](docs/DEPENDENCIES.md)；SDK 已迁移到官方 ACIS 2026.1.0.1（纯公有 API，无定制头文件），编译契约与行为差异见 [ACIS 2026 迁移指南](docs/ACIS_2026_MIGRATION.md)。

准备依赖和本机配置后，在仓库根目录执行：

```powershell
.\scripts\check-environment.ps1
.\scripts\build.ps1 -Configuration Release
Push-Location web
npm ci
npm run build
Pop-Location
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
```

只验证后端，不需要 CAD SDK 或数据库：

```powershell
cd CAD_DB/backend
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

## 功能与模式（v3，2026-10-19 定稿）

- **服务端权威命令**：建模（曲线/曲面/布尔/抽壳/变换）、撤销/重做全部在 ACIS worker 执行，浏览器只提交命令与消费网格（ADR-2：每项目一 worker 进程，命令串行队列即版本链）。
- **版本链与协作**：commit 带 base_version 乐观锁（过期 409 并返回实体级冲突清单）；组件不可变、按 `hash_sat` 全局去重；WebSocket 推送版本事件与在场感知（edit_intent）。
- **三级存储（ADR-3）**：SAT 资产 = 几何真相（内容寻址，还原字节级无损）；GLB = 渲染网格；实体图投影 = 可查询细档索引——原生标签 + 扁平字段，`DERIVED_FROM` 血缘链、`/entities` 与 `/diff` 实体级查询，内容寻址实体跨组件去重（ABC 压测实测 60.5%）。
- **导入导出**：SAT 与 STEP（Spatial InterOp）导入，SAT/GLB/STL 导出；STEP 装配体按组件树拆分入库。
- **渲染全覆盖**：可视化网格 + 边折线；无面体输出纯线框 GLB，退化面计入 `faces_skipped` 诚实上报（ABC 200/200 零失败；CNC 车床装配 248 组件 / 48.8 万三角形实测流畅渲染）。
- Neo4j 管理 WebUI（项目/版本/图/原始提交审计）。
- Qt 桌面客户端、C++ Bridge、客户端 Bolt 直连与 PostgreSQL 实验均已移除（历史见 `docs/history/` 与 tag `legacy/pre-architecture-v2`）。

## 几何内核隔离

ACIS 只存在于 worker 进程：许可初始化与 SDK 调用集中在 `CAD_DB/acis.cpp` 与 `worker/`、`kernel/` 的适配实现；`kernel/model_types.h` 定义中立值类型，`glb_writer`/`mesh_extractor` 输出与内核无关的网格资产。网关与浏览器不接触任何内核类型。

当前为编译时适配：接入其他内核需要实现 worker 命令层与网格提取，并处理已有 SAT 数据的兼容读取。尚未接入第二内核，不承诺无损读取 SAT，也没有运行时插件加载。设计取舍见 [内核隔离与替换指南](docs/KERNEL_ISOLATION.md)。

## 目录

```text
CAD_DB.sln                唯一 Visual Studio 解决方案（默认目标 dbcad_worker）
CAD_DB/
  acis.cpp/common.cpp     ACIS 许可初始化与错误处理
  worker/                 ACIS worker：命令执行、会话、实体图序列化
  kernel/                 网格提取、GLB 写入、中立类型
  backend/app/
    api/v1/               HTTP/WS 入口（项目/版本/命令/资产/实体/事件）
    v1_store.py           版本链 + 实体投影存储（Memory/Neo4j 双实现）
    neo4j_webui/          管理 API；static/webui/ 管理台前端
  backend/tests/          回归与集成检查（API 契约 / 真内核 / STEP / Neo4j 投影）
  backend/scripts/        abc 压测、投影重建工具
  deploy/                 一键启停、Docker、打包
web/                      React + three.js 客户端（Vite）
scripts/                  环境检查与便携构建
docs/                     架构、计划、基准、上手指南；docs/history/ 为 v2 归档
```

## 启停

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

启动链路：Neo4j → FastAPI → 浏览器 `/web/`，worker 由后端按需启动。默认端口 8000，以启动器输出地址为准；登录使用 `CAD_DB/deploy/dbcad.local.env` 中的 `DBCAD_PASSWORD`。重建与同机双客户端测试见[上手指南](docs/GETTING_STARTED.md#修改代码后重新运行)，参数见[部署说明](CAD_DB/DEPLOYMENT.md)。

[架构](docs/ARCHITECTURE.md) · [v3 计划](docs/V3_PLAN.md) · [压测基准](docs/BENCHMARK_v3.md) · [贡献说明](CONTRIBUTING.md) · [后端说明](CAD_DB/backend/README.md)
