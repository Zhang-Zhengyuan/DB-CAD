# DBCAD FastAPI 服务

公共入口保持 `uvicorn app.main:app`。协作统一使用 JSON + SAT 组件版本链。管理界面在 `/` 或 `/webui`；文档在 `/docs`。

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

服务直接连接 Neo4j，不需要服务器 ACIS 或 C++ Bridge。全栈部署使用 `CAD_DB/deploy/start_dbcad_fullstack.cmd`。

## 协议

| 路径 | 功能 |
|---|---|
| POST /projects | 创建项目 |
| GET /projects/{id}、/projects/by-name/{name} | 项目查询 |
| GET /projects/{id}/models/latest、/models/{version}、/models/versions | 同一 JSON 版本链的只读查询 |
| POST/GET /projects/{id}/json-delta | 唯一写入入口 / 增量 Pull |
| WS /ws/projects/{id} | 版本通知和在线成员；stream=mode1 |
| /api/neo4j/* | 管理 API |

HTTP 鉴权头为 `X-API-Password`，WS 使用 password 查询参数。模式、事件字段和 base_version 是协议约定。部署默认生成非空密码；独立开发的旧空密码行为保留，管理界面要求设置密码。

装配位于 app/main.py；模块边界见 [架构说明](../../docs/ARCHITECTURE.md)。静态资源随 app/ 打包，不依赖源码工作目录。

Mode0、`POST /models`、旧实体版本写入和 WS 模型提交已删除；旧模式的数据库记录仅在管理审计中保留。迁移与验收见 [说明](../../docs/HYBRID_COLLABORATION.md)。
