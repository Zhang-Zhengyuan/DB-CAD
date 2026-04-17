# CAD DB Store Backend (FastAPI)

## 当前阶段能力
- 项目创建与查询
- CAD 模型版本化存储（每次保存生成新版本）
- 版本冲突检测（`base_version`）
- 最新版本与历史版本查询
- WebSocket 同步通知（模型保存后广播）

## 快速启动
1. 安装依赖
   ```powershell
   cd CAD_DB/backend
   python -m venv .venv
   .\.venv\Scripts\Activate.ps1
   pip install -r requirements.txt
   ```

2. 启动服务
   ```powershell
   uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
   ```

3. 打开文档
   - Swagger: `http://localhost:8000/docs`

## 核心 API
- `POST /projects` 创建项目
- `GET /projects/{project_id}` 查询项目
- `POST /projects/{project_id}/models` 保存模型版本
- `GET /projects/{project_id}/models/latest` 获取最新版本
- `GET /projects/{project_id}/models/versions` 分页列出版本
- `GET /health` 健康检查
- `WS /ws/projects/{project_id}` 订阅项目更新

## 数据库
- 默认 SQLite：`cad_store.db`
- 可通过环境变量 `CAD_DB_DATABASE_URL` 覆盖

## 存储后端切换（关键）
为支持“前后端分离 + FastAPI 统一服务层 + Neo4j 数据库存储”，后端支持双模式：

- `CAD_DB_STORAGE_BACKEND=sqlite`（默认）
- `CAD_DB_STORAGE_BACKEND=neo4j`

当使用 Neo4j 模式时，还需要配置：

- `CAD_DB_NEO4J_URI`（例如 `bolt://127.0.0.1:7687`）
- `CAD_DB_NEO4J_USER`
- `CAD_DB_NEO4J_PASSWORD`
- `CAD_DB_NEO4J_DATABASE`（默认 `neo4j`）

示例（PowerShell）：

```powershell
$env:CAD_DB_STORAGE_BACKEND = "neo4j"
$env:CAD_DB_NEO4J_URI = "bolt://127.0.0.1:7687"
$env:CAD_DB_NEO4J_USER = "neo4j"
$env:CAD_DB_NEO4J_PASSWORD = "your_password"
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```
