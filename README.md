# DBCAD — 多人协作 CAD 数据库系统

Qt/C++ CAD 桌面客户端，使用 ACIS 几何内核、FastAPI 服务与 Neo4j 存储。支持几何建模、版本化存取与多人协作。

## 新成员上手

使用 coding agent 前先读 [AGENTS.md](AGENTS.md)；团队按 [Issue → 分支 → 验证 → PR → 人工审核](docs/ai/WORKFLOW.md) 协作。项目按需导航见 [AI 项目地图](docs/ai/PROJECT_CONTEXT.md)。

完整步骤见 [复现与部署指南](docs/GETTING_STARTED.md)。客户端需要授权 ACIS SDK 与配套依赖；源码不包含 SDK。依赖布局见 [依赖说明](docs/DEPENDENCIES.md)；SDK 已迁移到官方 ACIS 2026.1.0.1（纯公有 API，无定制头文件），编译契约与行为差异见 [ACIS 2026 迁移指南](docs/ACIS_2026_MIGRATION.md)。

准备依赖和本机配置后，在仓库根目录执行：

```powershell
.\scripts\check-environment.ps1
.\scripts\build.ps1 -Configuration Release
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
```

只验证后端，不需要 CAD SDK 或数据库：

```powershell
cd CAD_DB/backend
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

## 功能与模式

- ACIS 几何创建、曲线/曲面、布尔运算、渲染、文件存取与撤销/重做。
- 唯一协作链路 Mode1：JSON 组件携带 SAT 备份，HTTP Push/Pull、保存和历史版本共用 Neo4j 版本链。
- 支持的组件优先 JSON 重建，失败按 UUID 使用 SAT；已知有损组件直接走 SAT。
- WebSocket 只提供版本通知、在线成员和重连；Mode0 与 C++ Bridge 已移除。
- base_version 冲突检查、稳定 UUID、增量拓扑组件合并和本地未提交修改保护。
- Neo4j 管理 WebUI，包含项目、版本、图及按需查看原始提交。
- 直接 Neo4j 与 PostgreSQL 实验功能保留。

## 几何内核隔离

业务/UI 通过 `GeometryKernel` 调用建模、变换、网格与存取能力，文档使用稳定 UUID 和会话 `ShapeHandle`；ACIS 原生对象、SDK 调用、许可初始化、混合 JSON/SAT 编解码集中在独立的 `DbcadAcis` 静态库。显示层仅消费坐标、索引、颜色和包围盒。

当前采用编译时适配：接入其他内核需要实现接口、替换工厂与构建依赖，并处理已有 ACIS 数据的兼容读取。尚未接入第二内核，不承诺直接无损读取 SAT，也没有运行时插件加载。

实现路径、对象所有权、撤销与协作流程、允许保留的专用模块，以及 [issue #4](https://github.com/Zhang-Zhengyuan/DB-CAD/issues/4) 的逐项验收状态，见 [内核隔离与替换指南](docs/KERNEL_ISOLATION.md)。当前已完成主要调用隔离；自动依赖防护、无 SDK 业务测试构建和第二内核原型仍待完成。

## 目录

```text
CAD_DB.sln                唯一 Visual Studio 解决方案
CAD_DB/
  CAD_DB.vcxproj          客户端业务/UI 构建目标
  DbcadAcis.vcxproj       ACIS 适配器与专用存储兼容静态库
  kernel/                中立接口、值类型、文档记录与 ACIS 适配实现
  mainwindow.cpp         窗口初始化；其他实现位于 ui/
  ui/                    连接、协作、消息、文档、编辑
  access.cpp             SAT 文件与诊断；图存储位于 storage/
  storage/               旧 Neo4j 图/历史兼容、几何编码（编入适配库）
  backend/app/
    api/                 HTTP/WS 入口
    collaboration/       通知协议与版本摘要
    neo4j_webui/          管理 API
    static/webui/         HTML/CSS/JS
  backend/tests/         回归与集成检查
  deploy/lib/            配置、Docker、进程、打包
scripts/                 环境检查与便携构建
docs/                   上手、依赖、架构、验证记录
```

## 启停

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -PrepareOnly
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -SkipClient
.\CAD_DB\deploy\start_dbcad_two_clients.cmd
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

服务默认提供 `/health`、`/docs`、`/webui`，端口 8000。参数和密码管理见 [部署说明](CAD_DB/DEPLOYMENT.md)。重置会删除数据，正常启动无需重置。

[架构](docs/ARCHITECTURE.md) · [贡献说明](CONTRIBUTING.md) · [后端说明](CAD_DB/backend/README.md) · [验证记录](docs/REFACTOR_VALIDATION.md)

本轮设计与测试入口见 [统一协作链路说明](docs/HYBRID_COLLABORATION.md)。
