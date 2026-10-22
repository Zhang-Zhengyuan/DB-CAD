# 按改动选择验证

记录执行目录、命令、退出状态、结果摘要和未运行原因。历史验证记录不是本次证据；无服务测试不等于全栈验收。不修改公共/真实模型库来制造测试环境。

| 改动 | 最低验证 | 额外验收 |
|---|---|---|
| 文档、agent 入口、模板 | `git diff --check`、相对链接/命令路径核对 | 确认事实与当前代码一致 |
| 后端 Python / HTTP / WebUI | 下方后端基线 + 相关回归 | 鉴权、非法输入、409 冲突；管理台人工检查 |
| worker 命令/内核 / 网格 | 下方 worker selftest + mesh_tests | 真栈集成测试；GLB 用 `web/scripts/validate-assets.mjs` 校验 |
| 实体图 / 投影 / 存储 | 后端基线 + `tests/test_v1_projection_neo4j.py`（integration） | `/entities`、`/diff` 查询；数据影响与回滚说明 |
| 导入导出（SAT/STEP） | 后端基线 + `tests/test_v1_step_import_integration.py` | 往返等价（entity_key 集合相等口径） |
| 前端 web/ | `npm run build`（tsc + vite） | `npx playwright test` 真栈 e2e |
| 部署脚本/打包 | 相关 PowerShell 检查 | 独立目录部署包启动、健康检查和静态资源 |
| CI / 流程检查 | `python scripts/tests/test-pr-policy.py`（仓库根，需要 Node.js）、YAML 解析 | 真实 PR 事件验证；首次合入后确认状态检查名称 |

后端（工作目录 `CAD_DB/backend`）：

```powershell
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

真实服务集成（同目录，先按后端 README 配隔离服务及 `CAD_DB_INTEGRATION_API_PASSWORD`）：

```powershell
uv run --frozen pytest -q -m integration
```

worker 与内核（仓库根目录，先按 GETTING_STARTED 配授权 SDK）：

```powershell
.\scripts\build.ps1 -Configuration Release
.\x64\Release\dbcad_worker.exe selftest
msbuild CAD_DB\CAD_DB.vcxproj /p:DBCADMeshTests=true   # mesh_tests 回归门禁
```

前端（工作目录 `web`）：

```powershell
npm run build        # tsc --noEmit + vite build
npx playwright test  # 真浏览器 + 真后端 + 真 worker（e2e-server.cmd，端口 8765）
```

e2e 会自建隔离后端（临时组件库目录），不污染开发实例。格式沿用 `.clang-format` / `.editorconfig`；当前 pyproject 未配置 Ruff 依赖，不能把未安装的 lint 命令声称为既有 CI。

源码启动器回归：在仓库根执行 `powershell -NoProfile -ExecutionPolicy Bypass -File scripts/tests/test-fullstack-launcher.ps1` 和 `CAD_DB/deploy/start_dbcad_fullstack.cmd -PrepareOnly`。Web e2e 使用隔离内存存储和测试密码，覆盖登录、资产/导入导出认证及双会话同步，不代表 Neo4j 持久化验收。
