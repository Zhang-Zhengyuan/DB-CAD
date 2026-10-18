# 管理员启用服务端约束

状态：本次只准备仓库内配置，未修改 GitHub 设置。需要仓库管理员在本 PR 合入默认分支后设置并验收；文件存在不代表保护已开启。

## 默认分支保护 / Ruleset

1. 在仓库 Settings → Branches 或 Rules → Rulesets 中，对真实默认分支（当前 master）启用 Active 规则；其他长期受保护分支也覆盖。
2. Require a pull request before merging：至少 1 个 approval；Dismiss stale approvals；Require approval of the most recent reviewable push；Require conversation resolution。
3. Require status checks：先让真实 PR 运行一次，再选 `policy` 与 `backend`（对应页面显示 `PR policy / policy` 和 `Backend checks / backend`），确认唯一 job 名并要求最新基线。不要在检查尚未注册时猜名称。
4. 禁止 force push 和删除；经典保护启用 Do not allow bypassing；Ruleset 不给 agent/机器人 bypass 权限。检查管理员例外名单，避免管理员账号上的 agent 直接推送。
5. agent 不持有管理员凭据，不启用自动合并；由团队维护者实际审核与合并。若采用 CODEOWNERS，填入真实有写权限的维护者/团队并启用 code-owner review；本次不猜测账号归属。
6. 限制 Actions 默认 token 为只读；新增流程不得授予写权限或执行来自 fork 的不可信代码。

GitHub 原生 approval 代表账号审核，无法单凭一份文件证明账号背后是人。团队需要约定维护者账号由人执行审核，不将机器人的 approval 算作人工批准。可用的保护功能受仓库可见性和套餐影响，参见 [官方说明](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)。

## 上线验收（用临时分支/PR，避免测试写主分支）

- 合法真实 Issue + `docs/编号-summary` + 正确标题/Closes → policy 通过。
- 无编号分支、local 分支、错误类型标题、无 Closes、无效 Issue、编号指向 PR → policy 失败。
- 未批准时合并按钮被阻止；维护者批准但 CI 失败仍不可合并。
- 审核后新提交应重新检查并使旧审核失效；直接 push 默认分支和 force push 被服务端拒绝。
- fork PR 可读本仓库 Issue，但流程不执行 fork 中的代码。修改流程/权限的 PR 重点人工审查。

## 日常维护

将 AGENTS、协议和构建命令变化纳入同一 PR；每次发现 agent 重复犯错，只补可验证且必要的规则。根入口保持短，模块细节下沉；不要堆砌多年历史。模板和 CI 变化要保留正反输入验证记录。
