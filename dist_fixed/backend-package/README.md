# CAD DB Store Backend (FastAPI)

## 当前阶段能力
- 项目创建与查询
- CAD 模型版本化存储（每次保存生成新版本）
- 版本冲突检测（`base_version`）
- 最新版本与历史版本查询
- WebSocket 同步通知（模型保存后广播）
- Python 仅作为服务层，底层存储由 C++ Storage Bridge + Neo4j 实现

## 快速启动（推荐 uv）
1. 安装 uv（若未安装）
   ```powershell
   powershell -ExecutionPolicy Bypass -c "irm https://astral.sh/uv/install.ps1 | iex"
   ```

2. 同步依赖并启动
   ```powershell
   cd CAD_DB/backend
   uv sync
   uv run uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
   ```

3. 打开文档
   - Swagger: `http://localhost:8000/docs`

## 跨电脑部署
- 后端机器启动：
  - `uvicorn app.main:app --host 0.0.0.0 --port 8000`
- 前端机器 `fastapi_connect_info.conf` 填后端地址，例如：
  - `http://192.168.1.100:8000`
- 需放通网络策略：
  - HTTP 端口（默认 8000）
  - WebSocket 路径：`/ws/projects/{project_id}`（与 HTTP 同端口）

也可直接用一键脚本（仓库根目录）：

```powershell
.\CAD_DB\deploy\server\deploy_backend_uv.ps1 -HostAddress 0.0.0.0 -Port 8000 -StorageBackend neo4j
```

## 核心 API
- `POST /projects` 创建项目
- `GET /projects/{project_id}` 查询项目
- `POST /projects/{project_id}/models` 保存模型版本
- `GET /projects/{project_id}/models/latest` 获取最新版本
- `GET /projects/{project_id}/models/versions` 分页列出版本
- `GET /health` 健康检查
- `WS /ws/projects/{project_id}` 订阅项目更新

## 存储架构（关键）
FastAPI 不直接操作 Neo4j 几何存储，必须通过 C++ Storage Bridge。后端只支持 Neo4j 路径。

需要配置：

- `CAD_DB_STORAGE_BACKEND=neo4j`
- `CAD_DB_STORAGE_BRIDGE_URL`（例如 `http://127.0.0.1:8100`）
- `CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS`

- `CAD_DB_NEO4J_URI`（例如 `bolt://127.0.0.1:7687`）
- `CAD_DB_NEO4J_USER`
- `CAD_DB_NEO4J_PASSWORD`
- `CAD_DB_NEO4J_DATABASE`（默认 `neo4j`）

示例（PowerShell）：

```powershell
$env:CAD_DB_STORAGE_BACKEND = "neo4j"
$env:CAD_DB_STORAGE_BRIDGE_URL = "http://127.0.0.1:8100"
$env:CAD_DB_NEO4J_URI = "bolt://127.0.0.1:7687"
$env:CAD_DB_NEO4J_USER = "neo4j"
$env:CAD_DB_NEO4J_PASSWORD = "your_password"
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

启动 C++ Storage Bridge（与 Qt 同一可执行程序）：

```powershell
CAD_DB.exe --storage-bridge --bridge-host 127.0.0.1 --bridge-port 8100 --neo4j-host 127.0.0.1 --neo4j-port 7687 --neo4j-user neo4j --neo4j-password your_password
```
