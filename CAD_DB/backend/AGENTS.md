# 后端补充规则

继承根与 CAD_DB/AGENTS.md 中适用规则；Python 服务不使用客户端 SDK。

- 工作目录为本目录；先读 README 和 pyproject。用 `uv sync --frozen --extra dev` 与 `uv run --frozen pytest -q -m 'not integration'`。
- 公共入口 `app.main:app`；HTTP/WS 路由在 `app/api/`，业务验证/闭包在 `json_collab.py`，组件版本持久化在 `json_graph_store.py`。避免把新业务塞回 main.py。
- 保持 schema、HTTP 错误、鉴权与 WS 通知兼容；输入校验失败不产生半次写入。并发提交必须保留 base_version 检查与事务一致性。
- SAT 作为组件数据存储，不在后端引入 ACIS/Bridge；WS 不承担模型提交。
- 单元验证用已有内存适配器；integration 明确需要隔离的真实服务。conftest 会清空单元测试 API 密码；联调用独立变量，不能为了通过测试放松生产鉴权。
- 更改路由/schema 同步 C++ 调用、WebUI 和回归；静态资源应随包可访问，不依赖源码工作目录。新增依赖同步锁文件，禁止手改 lock 的解析结果。
