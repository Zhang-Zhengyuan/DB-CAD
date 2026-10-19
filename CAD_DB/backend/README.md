# DBCAD FastAPI 服务

公共入口 `uvicorn app.main:app`。v3 服务端权威网关：接收浏览器命令，转发 ACIS worker 执行，管理版本链与资产。Web 客户端在 `/web`，管理界面在 `/` 或 `/webui`，接口文档在 `/docs`。

## 开发测试

```powershell
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

无数据库测试使用内存适配器。integration 检查需要真实 Neo4j / worker；跳过集成测试不代表全栈验收。

撤销/重做仍追加新版本，`base_version` 保持实际提交父链；版本内部的
`restored_from` 记录恢复来源，连续撤销沿逻辑建模历史逐步回退。该字段随
Neo4j 版本保存，旧版本缺失字段时按普通历史版本读取，无需迁移。重做栈仍
仅在进程内保存，服务重启后清空；成功的新建模命令或导入会清除重做分支，
失败操作保留重做机会。旧版已产生的撤销记录不会被追溯改写。

撤销来源的数据库往返回归使用 `tests/test_v1_undo_neo4j.py`：必须显式配置
`CAD_DB_UNDO_TEST_NEO4J_URI` 指向隔离测试库；需要认证时另设
`CAD_DB_UNDO_TEST_NEO4J_PASSWORD`，未配置 URI 时跳过，不使用开发库。

测试默认按"无 API 密码"运行：`tests/conftest.py` 会在加载任何 app 模块前把 `CAD_DB_API_PASSWORD` 置空（环境变量优先级高于 `.env`），避免本机 export 过密码或 `.env` 配了密码时全部请求误报 401。integration 测试对活体服务联调时，密码改用 `CAD_DB_INTEGRATION_API_PASSWORD` 传入。

## 独立启动

复制 `.env.example` 为 `.env`，填写 Neo4j 地址、用户名、密码和 API 密码。先启动数据库：

```powershell
uv sync --frozen
uv run --frozen uvicorn app.main:app --host 127.0.0.1 --port 8000
```

worker（`x64/<Configuration>/dbcad_worker.exe`）由 FastAPI 的 GeometryPool 按项目自动拉起，一个项目一个进程；路径由环境变量配置。全栈部署使用 `CAD_DB/deploy/start_dbcad_fullstack.cmd`。

## 协议（/api/v1）

| 路径 | 功能 |
|---|---|
| GET/POST /projects、GET /projects/{id} | 项目查询与创建 |
| GET/POST /projects/{id}/versions、GET /versions/{version} | 版本提交（base_version 乐观锁）与历史；另有 /scene 场景快照、/branches 分支 |
| POST /projects/{id}/commands | 几何命令（worker 执行并产出版本）；/commands/dry-run 预演、/undo /redo、GET /schema/commands |
| POST /projects/{id}/files/import | SAT/STEP 导入（组件树拆分入库）；GET /files/export 导出 SAT/GLB/STL |
| GET /projects/{id}/entities、/diff | 实体级细档查询、两版本实体差异；POST /admin/compact-entities 压实 |
| GET /assets/{hash}.sat/.glb | 内容寻址资产（不可变强缓存） |
| WS /api/v1/events | 版本通知与在场感知（edit_intent） |
| /api/neo4j/* | 管理 API |

HTTP 鉴权头为 `X-API-Password`，WS 使用 password 查询参数。部署默认生成非空密码；独立开发的空密码行为保留，管理界面要求设置密码。模式、事件字段和 base_version 是协议约定。

装配位于 app/main.py；模块边界见 [架构说明](../../docs/ARCHITECTURE.md)。静态资源随 app/ 打包，不依赖源码工作目录。v2 的 json-delta/WS 模型提交协议已随 Qt 客户端移除，历史见 [docs/history/](../../docs/history/README.md)。
