# 本次规范建设的引入例外

日期：2026-10-18。用户明确要求：本次不创建 Issue，保留 `docs/local-ai-governance`，使用命令行推送并创建 PR。此授权仅针对首次引入规范，不是后续任务的常规流程。

## 本次范围

根/模块 AGENTS、项目地图、Git 工作流、验证矩阵、Claude/Copilot 入口、Issue/PR 模板、PR 命名和关联检查、管理员配置指南，以及检查脚本的正反例测试。不改变业务代码、SDK 或数据库。

已有 `docs/GETTING_STARTED.md` 修改与未跟踪 `docs/TEAM_DEPLOYMENT.md` 不属于本次提交；外层工作区入口也不纳入项目 Git。

## 首次上线

- 本次 PR 不填写虚构的 `Closes #编号`，也不创建占位 Issue；后续任务继续执行真实 Issue → 类型/编号分支 → 验证 → PR → 人工审核。
- `pull_request_target` 使用默认分支上的可信 workflow。该文件首次合入前尚未激活，维护者需人工审查本次 PR，不设置永久分支白名单或关闭检查。
- 合入后，由管理员按 MAINTAINER_SETUP.md 启用分支保护，并用新的真实 Issue/PR 验证命名、关联、CI 和人工审核门槛。
- agent 只创建 PR，不批准、不合并、不启用自动合并。

## 验证与回滚

验证记录见 BOOTSTRAP_VALIDATION.md。修改规范或配置可通过新的 PR 回退，不影响运行时数据。服务端分支保护独立配置，不能通过文件存在推断已经启用。
