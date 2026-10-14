# DBCAD FastAPI 服务

公共入口保持 `uvicorn app.main:app`。项目、SAT 版本、JSON delta、实体图与 WebSocket 路径保留。管理界面在 `/` 或 `/webui`；文档在 `/docs`。

## 开发测试

```powershell
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

无数据库测试使用内存适配器。integration 检查需要真实服务；跳过集成测试不代表全栈验收。

测试默认按"无 API 密码"运行：`tests/conftest.py` 会在加载任何 app 模块前把 `CAD_DB_API_PASSWORD` 置空（环境变量优先级高于 `.env`），避免本机 export 过密码或 `.env` 配了密码时全部请求误报 401。integration 测试对活体服务联调时，密码改用 `CAD_DB_INTEGRATION_API_PASSWORD` 传入。

## 独立启动

复制 `.env.example` 为 `.env`，填写 Neo4j 地址、用户名、密码和 API 密码。先启动数据库：

```powershell
uv sync --frozen
uv run --frozen uvicorn app.main:app --host 127.0.0.1 --port 8000
```

`CAD_DB_DIRECT_JSON_MODE=true` 使用直接 Neo4j 路径，Mode1 不需要 C++ Bridge。旧路径设置 false 与 `CAD_DB_STORAGE_BRIDGE_URL`，先启动 `CAD_DB.exe --storage-bridge`。全栈部署优先使用 `CAD_DB/deploy/start_dbcad_fullstack.cmd`。

## 协议

| 路径 | 功能 |
|---|---|
| POST /projects | 创建项目 |
| GET /projects/{id}、/projects/by-name/{name} | 项目查询 |
| POST /projects/{id}/models | 保存 Mode0 |
| GET /projects/{id}/models/latest、/models/{version}、/models/versions | 版本查询 |
| POST/GET /projects/{id}/json-delta | Mode1 Push/Pull |
| /projects/{id}/entities | 保留实体版本接口 |
| WS /ws/projects/{id} | 协作；stream=mode0 或 mode1 |
| /api/neo4j/* | 管理 API |

HTTP 鉴权头为 `X-API-Password`，WS 使用 password 查询参数。模式、事件字段和 base_version 是协议约定。部署默认生成非空密码；独立开发的旧空密码行为保留，管理界面要求设置密码。

装配位于 app/main.py；模块边界见 [架构说明](../../docs/ARCHITECTURE.md)。静态资源随 app/ 打包，不依赖源码工作目录。
