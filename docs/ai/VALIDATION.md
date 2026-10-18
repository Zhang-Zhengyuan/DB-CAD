# 按改动选择验证

记录执行目录、命令、退出状态、结果摘要和未运行原因。历史验证记录不是本次证据；无服务测试不等于全栈验收。不修改公共/真实模型库来制造测试环境。

| 改动 | 最低验证 | 额外验收 |
|---|---|---|
| 文档、agent 入口、模板 | `git diff --check`、相对链接/命令路径核对 | 确认事实与当前代码一致 |
| 后端 Python / HTTP / WebUI | 下方后端基线 + 相关回归 | 鉴权、非法输入、冲突；UI 人工检查 |
| C++ UI/文档/内核 | 环境检查、Debug/Release 构建、对应回归 | 实际 GUI 操作；对象生命周期/撤销重做 |
| JSON/SAT、UUID、协作 | 后端 + 客户端相关回归 | 两客户端 Push/Pull、冲突、本地修改保护、失败不推进版本、保存重开、往返几何 |
| 数据库存储 | 单元/内存适配器回归 + 隔离数据库 integration | 版本/组件一致性、旧数据读取，数据影响与回滚说明 |
| 部署脚本/打包 | 相关 PowerShell 检查 + PrepareOnly | 独立目录部署包启动、健康检查和静态资源 |
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

客户端（仓库根目录，先按 GETTING_STARTED/LOCAL_SETUP 配授权 SDK、Qt 和本地 props）：

```powershell
.\scripts\check-environment.ps1
.\scripts\build.ps1 -Configuration Debug
.\scripts\build.ps1 -Configuration Release
.\scripts\tests\run-client-regression.ps1 -Configuration Debug
.\scripts\tests\run-client-regression.ps1 -Configuration Release
```

部署（仓库根目录，按改动范围选择）：

```powershell
.\scripts\tests\test-deploy-resolution.ps1
.\scripts\tests\test-deploy-window-style.ps1
.\scripts\tests\test-client-runtime.ps1
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -PrepareOnly
```

PrepareOnly 仍可能生成本机配置/部署文件，需要正确依赖；它不能代替真实启动。GUI 场景步骤见 `docs/HYBRID_COLLABORATION.md`，内核边界见 `docs/KERNEL_ISOLATION.md`。格式沿用 `.clang-format` / `.editorconfig`；当前 pyproject 未配置 Ruff 依赖，不能把未安装的 lint 命令声称为既有 CI。
