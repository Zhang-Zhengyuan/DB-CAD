# 团队与 agent 的 Git 工作流

## 1. 登记与规划

先确认真实仓库根目录和 remote，再搜索相同任务的开放 Issue。所有实现（含 docs/ci）关联真实 Issue；只读答疑和审查无需创建分支。Issue 写清问题/复现、预期结果、范围与非目标、验收清单、验证计划和兼容风险。大的协议/存储变更先由维护者确认方案；简单需求无需反复请求授权。

## 2. 建分支

正式名称正则：`^(feat|fix|docs|refactor|test|chore|perf|build|ci|revert)/[1-9][0-9]*-[a-z0-9]+(?:-[a-z0-9]+)*$`。

| 前缀 | 用途 |
|---|---|
| feat / fix | 新功能 / 缺陷修复 |
| docs / test | 文档 / 测试 |
| refactor / perf | 行为不变重构 / 性能 |
| build / ci / chore | 构建 / 持续集成 / 维护 |
| revert | 有原因和影响分析的回退 |

从远端默认分支出发，不猜一定叫 main。本项目当前远端默认为 master；运行 `git remote show origin` 或查看 GitHub 确认。下面示例只有在工作区干净、Issue #123 真实存在时执行，任务类型按实际修改替换：

```powershell
git fetch origin
git switch -c fix/123-pull-version-conflict origin/master
```

不要为了同步而强行 `pull` 或重置已有改动。若现有工作属于别的任务，使用独立工作树并确认基线；未提交内容不会自动带入新工作树。若当前分支已含本 Issue 的工作，继续原分支，不重复建分支。

离线准备使用 `<type>/local-<summary>`，仅本地可逆工作；先保存可发布 Issue 草稿。恢复后创建真实 Issue、确认基线，再 `git branch -m docs/123-ai-governance`（编号只是示例）。有混入其他任务提交时逐条迁移到正确基线，不能以改名掩盖依赖。离线分支不能推送，CI 也不接受。

## 3. 实现与验证

每个 PR 聚焦一个验收目标；跨多个子系统但共同实现同一目标可以一个 PR，拆分互不依赖的改动。新增行为、失败处理与协议必须有针对性验证；文档修改检查路径/命令，避免为文字变更跑整个 SDK 全栈。按 VALIDATION.md 选择检查。

实现过程中记录关键取舍和限制到 Issue/PR；不把大量会话流水塞进根 AGENTS.md。避免多人同时修改共享核心文件；在 Issue 说明预计范围。不得自动撤销别人未提交内容或忽略新出现的差异。

## 4. 提交与 PR

```powershell
git diff --check
git status --short
# 明确列举本任务文件，以下路径仅为示例
git add docs/ai/WORKFLOW.md
git diff --cached
git commit -m "docs(workflow): clarify issue-first development"
git push -u origin docs/123-ai-governance
```

提交和 PR 标题统一使用 `<type>(<scope>): <description>`（scope 可选），PR type 与分支 type 一致。不要复制示例编号冒充已创建 Issue。

PR 正文使用模板并单独写 `Closes #123`。列出具体验证命令、结果、未运行项目及原因，涉及 UI 附必要截图但清理凭据；写清兼容/数据影响及回滚方法。大范围变更可先 Draft PR，验收未完成保持 Draft。

CI `PR policy / policy` 检查分支、标题、Closes 行和真实 Issue（不能是另一个 PR）；`Backend checks / backend` 检查无外部服务的后端测试。规则工作流使用 `pull_request_target`，只运行固定内联脚本和只读 GitHub API，**禁止 checkout/执行 PR 分支代码**。首次纳入默认分支前不会生效。

## 5. 人工审核与合并

至少一位具有写权限的团队维护者批准；审查需求完成度、架构、协议/数据兼容、测试证据和敏感文件。新提交触发重新检查，重要变更重新审核。所有讨论解决后由维护者合并；agent 结束于“等待人工审核”。失败验证、冲突或缺失关键 GUI 验收不得冒充 ready。

管理员必须按 [管理员配置](MAINTAINER_SETUP.md) 启用分支保护。文件里的禁止规则和 CI 都不能代替服务端权限控制；本地 hook 可绕过，不作为强制审核依据。

## 参考做法（2026-10-18 查阅）

- [AGENTS.md 官方约定](https://agents.md/)：统一入口与目录级补充规则，避免每轮重新探索。
- [GitHub coding agent 最佳实践](https://docs.github.com/en/copilot/using-github-copilot/using-copilot-coding-agent-to-work-on-tasks/best-practices-for-using-copilot-to-work-on-tasks)：提供项目、构建、测试和编码规范。
- [GitHub 保护分支](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)：强制 PR、审核和状态检查，并考虑管理员绕过权限。
