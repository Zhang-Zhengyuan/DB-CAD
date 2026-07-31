# DBCAD — 多人协作 CAD 数据库系统

> 阶段里程碑：客户端-服务端增量协作同步已端到端跑通（2026-07-31）

DBCAD（Database CAD）是一个**支持多人实时协作**的 ACIS 内核 CAD 客户端，对接 FastAPI 后端 + Neo4j 图数据库。多个用户能在不同机器上打开同一项目，互相同步新增/删除的实体（立方体、球体、布尔运算体等），并保留各自未提交的本地修改。

## 核心能力

- **ACIS 几何内核**（Qt C++ 客户端）：立方体/球体/锥/环/扫掠/布尔运算等
- **多人实时协作**：WebSocket 推送 + Pull 拉取，UUID 增量去重，本地未提交修改不被覆盖
- **乐观锁版本控制**：每次提交携带 `base_version`，冲突拒绝
- **可插拔存储**：Neo4j（图）/ PostgreSQL（实验性 SAT 直存）
- **协作状态机**（`CollabSession`）：9 状态 + 决策接口，避免散落 bool 标志位
- **ACIS Entity Graph 序列化**：JSON 表达 BODY/LUMP/SHELL/FACE/EDGE/VERTEX 拓扑 + 几何

## 仓库结构

```
DB-CAD-master/
├── CAD_DB/                                  # Qt C++ 客户端主项目
│   ├── mainwindow.cpp/h                     # 主窗口 + 协作面板 + 实体图 push/pull
│   ├── window.cpp/h                         # 实体树 + addEntity
│   ├── collab_session.h/cpp                 # 协作状态机（单一真相源）
│   ├── backend_api_client.cpp/h             # HTTP/WS 客户端
│   ├── entity_graph_serializer.cpp/h        # ACIS 拓扑 → JSON
│   ├── storage_bridge_service.cpp/h         # C++ → Neo4j 桥服务（嵌入 CAD_DB.exe）
│   ├── neo4j.cpp/h                          # mgclient Bolt 客户端
│   ├── access.cpp/hxx                       # ACIS restore/save wrapper
│   ├── glwidget.cpp/h                       # OpenGL 渲染
│   ├── gme_mesh.cpp/hxx                     # ACIS → 三角面片
│   ├── deploy/                              # 一键部署脚本
│   ├── backend/                             # FastAPI 后端
│   ├── assignments/                         # 后端三份教学作业
│   ├── COLLABORATION_TECHNICAL_ROADMAP.md   # 协作技术路线（必读）
│   └── DEPLOYMENT.md                        # 部署说明
├── CAD_DB.sln / CAD_DB.vcxproj              # Visual Studio 解决方案
├── x64/                                     # 构建输出
└── neo4j-runtime/                           # Neo4j Docker 数据持久化（自动生成）
```

## 快速开始

### 一键启动（Neo4j + FastAPI + 双客户端协作）

```powershell
# 构建客户端
msbuild CAD_DB.sln /p:Configuration=Release /p:Platform=x64

# 启动全栈（首次会创建 Neo4j Docker + 写 dbcad.local.env）
.\CAD_DB\deploy\start_dbcad_fullstack.cmd

# 启动两个客户端（连接到同一个 FastAPI 项目）
.\CAD_DB\deploy\start_dbcad_two_clients.cmd
```

### 停止 / 重置

```powershell
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
.\CAD_DB\deploy\reset_dbcad_deployment.cmd           # 清理旧部署 + 旧 Docker 数据
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -ResetNeo4jDockerData   # 重新生成 Neo4j + 启动
```

## 架构

```
┌────────────────────────────────────────────────────────┐
│  Qt C++ 客户端 (CAD_DB.exe) × N                         │
│  ┌────────────┐ ┌──────────┐ ┌──────────────────────┐  │
│  │MainWindow  │ │Window    │ │CollabSession          │  │
│  │(协作面板)  │ │(实体树)  │ │(状态机 + 决策接口)    │  │
│  └────────────┘ └──────────┘ └──────────────────────┘  │
│  ┌──────────────────┐ ┌─────────────────────────────┐  │
│  │BackendApiClient  │ │EntityGraphSerializer         │  │
│  │HTTP / WebSocket  │ │ACIS 拓扑 → JSON              │  │
│  └──────────────────┘ └─────────────────────────────┘  │
└────────────────────────┬───────────────────────────────┘
                         │ HTTP REST + WebSocket (8000)
┌────────────────────────┴───────────────────────────────┐
│  Python FastAPI (uvicorn app.main:app)                  │
│  ┌─────────┐ ┌──────────┐ ┌──────────────────────────┐ │
│  │main.py  │ │sync.py   │ │crud.py                   │ │
│  │路由/WS  │ │连接管理  │ │业务规则                   │ │
│  └─────────┘ └──────────┘ └──────────────────────────┘ │
└────────────────────────┬───────────────────────────────┘
                         │ HTTP (8100)
┌────────────────────────┴───────────────────────────────┐
│  C++ Storage Bridge (CAD_DB.exe --storage-bridge)       │
│  ┌────────────────────────┐ ┌────────────────────────┐ │
│  │storage_bridge_service  │ │neo4j.cpp (mgclient)    │ │
│  └────────────────────────┘ └────────────────────────┘ │
└────────────────────────┬───────────────────────────────┘
                         │ Bolt (7687)
┌────────────────────────┴───────────────────────────────┐
│  Neo4j 5.15 + APOC (Docker)                             │
│  BridgeProject / BridgeVersion / entity_graph_version   │
└────────────────────────────────────────────────────────┘
```

## 文档导航

| 文档 | 用途 |
|---|---|
| [`CAD_DB/COLLABORATION_TECHNICAL_ROADMAP.md`](CAD_DB/COLLABORATION_TECHNICAL_ROADMAP.md) | **协作同步技术路线（核心）**：状态机、协议、增量合并、bug 修复记录 |
| [`CAD_DB/DEPLOYMENT.md`](CAD_DB/DEPLOYMENT.md) | 部署、密码、Docker Neo4j、常用命令 |
| [`CAD_DB/deploy/README.md`](CAD_DB/deploy/README.md) | PostgreSQL 实验性 demo + 主入口参考 |
| [`CAD_DB/backend/README.md`](CAD_DB/backend/README.md) | FastAPI 后端 API 文档 |
| [`CAD_DB/assignments/README.md`](CAD_DB/assignments/README.md) | 后端三份教学作业 |

## 里程碑

| 日期 | 节点 |
|---|---|
| 2026-07-13 | 协作面板 + CollabSession 状态机 |
| 2026-07-14 | Entity Graph JSON 序列化层 |
| 2026-07-31 | **客户端-服务端增量同步端到端跑通（A push 立方体 → B pull 恢复并渲染；B 加球体 → 推到 A）** |