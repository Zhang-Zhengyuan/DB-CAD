# DBCAD — 多人协作 CAD 数据库系统

Qt/C++ CAD 桌面客户端，使用 ACIS 几何内核、FastAPI 服务与 Neo4j 存储。支持几何建模、版本化存取与多人协作。

## 新成员上手

完整步骤见 [复现与部署指南](docs/GETTING_STARTED.md)。客户端需要授权 ACIS SDK 与配套依赖；源码不包含 SDK。依赖布局见 [依赖说明](docs/DEPENDENCIES.md)。

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
- Mode0：SAT 全量版本，保留 C++ Storage Bridge 路径。
- Mode1：客户端序列化实体图，JSON delta Push/Pull，服务端直接写 Neo4j；默认启动不需要 Bridge。
- WebSocket 通知、在线成员、重连及按模式隔离的同步通道。
- base_version 冲突检查、稳定 UUID、增量拓扑组件合并和本地未提交修改保护。
- Neo4j 管理 WebUI，包含项目、版本、图及按需查看原始提交。
- 直接 Neo4j 与 PostgreSQL 实验功能保留。

## 目录

```text
CAD_DB.sln                唯一 Visual Studio 解决方案
CAD_DB/
  CAD_DB.vcxproj          唯一客户端构建清单
  mainwindow.cpp         窗口初始化；其他实现位于 ui/
  ui/                    连接、协作、消息、文档、编辑
  access.cpp             SAT 文件与诊断；图存储位于 storage/
  storage/               图保存/恢复、历史、delta、几何编码
  backend/app/
    api/                 HTTP/WS 入口
    services/            版本写入与事件
    collaboration/       协议、快照、提交
    neo4j_webui/          管理 API
    static/webui/         HTML/CSS/JS
  backend/tests/         回归与集成检查
  deploy/lib/            配置、Docker、进程、打包
  assignments/           后端教学练习
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
