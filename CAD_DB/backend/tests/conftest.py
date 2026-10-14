"""测试会话的环境固定（在任何 app 模块 import 之前执行）。

本套件按"无 API 密码"模式编写。`app.config.settings` 是 import 时创建的
单例，`api_password` 取自环境变量（优先级最高）或 `backend/.env`。因此只要
shell 里 export 过 `CAD_DB_API_PASSWORD`，或 `.env` 里配了它，TestClient
的全部请求都会拿到 401——看起来像大规模回归，实际只是鉴权环境泄漏。

这里把环境变量显式置空而不是删除，是为了同时覆盖两种来源：置空的环境变量
优先级高于 .env 文件。pytest 进程内的修改不影响开发者 shell，也不改 .env。
"""

import os

os.environ["CAD_DB_API_PASSWORD"] = ""
