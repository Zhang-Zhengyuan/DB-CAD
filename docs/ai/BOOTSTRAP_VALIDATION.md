# 首次引入验证记录

日期：2026-10-18；本次仅修改规范、模板、流程配置与对应检查测试，不修改应用业务代码。

| 检查 | 本次结果 |
|---|---|
| `python scripts/tests/test-pr-policy.py`（仓库根） | 5 组测试通过，合计 38 个正反输入；运行 workflow 中实际内联 JavaScript，mock 只读 GitHub API |
| `git diff --check` | 通过；已有 copilot 文件的 LF/CRLF 提示不属于失败 |
| 新增文件空白检查 | 文本以换行结束、无行尾空白，通过；未检查/修改他人已有未跟踪文档 |
| YAML 解析 | pr-policy workflow、Issue 模板、Issue 配置通过；核对 target 事件、只读权限及无 checkout |
| Markdown 本地链接 | 13 份 Markdown 的 22 条本地链接存在，无缺失 |
| 项目命令 | 对照现有构建脚本、pyproject、backend workflow 与模块路径核对 |
| 应用构建、后端/数据库/GUI | 未运行；本次无应用行为变更，既有历史验证不计作本次证据 |
| GitHub PR 事件、服务端保护 | 尚未完成服务端验收/配置；本次按用户授权通过命令行创建 PR，合入后按 MAINTAINER_SETUP 验收 |

policy 覆盖：10 种合法类型、中文说明、CRLF 正文；不合法/离线/无编号分支，类型不一致和格式错误标题，缺失/重复/不匹配关联，编号指向 PR、已关闭 Issue，以及 403/404/500 API 错误。API 错误会阻止检查通过，不会绕过。

保护用户已有内容：`docs/GETTING_STARTED.md` 和 `docs/TEAM_DEPLOYMENT.md` 本次未编辑。本次按用户明确授权使用 `docs/local-ai-governance` 提交并推送、创建 PR；不创建 Issue、不合并。外层 `../AGENTS.md` 是本机定位入口，不在项目 Git 中；团队获取仓库时根 AGENTS 已可直接使用。
