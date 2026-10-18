# 开发与维护

先阅读 [AGENTS.md](AGENTS.md)、[团队 Git 工作流](docs/ai/WORKFLOW.md)，再按任务阅读 docs/GETTING_STARTED.md 与 docs/ARCHITECTURE.md。

所有实现先关联真实 Issue，再创建 `type/issue-number-summary` 分支（例如 `fix/123-pull-conflict`），验证后提 PR。PR 正文单独写 `Closes #123`，至少一位人类维护者批准后由维护者合并；禁止直接推送主分支和 agent 自行合并。离线仅允许本地草稿准备，不能伪造 Issue 编号。

Agent 上下文：[项目地图](docs/ai/PROJECT_CONTEXT.md) · [验证矩阵](docs/ai/VALIDATION.md) · [管理员启用保护](docs/ai/MAINTAINER_SETUP.md)。规则修改也走同一流程；历史 PR_CHECKLIST 是旧重构记录，不替代当前通用 PR 模板。

- 保持客户端、协议与存储模式兼容；业务变化和代码搬移分别说明。
- 按职责组织文件，复杂函数提取命名清晰的子步骤，共享状态保持一个所有者。
- 注释解释 UUID 对齐、版本冲突、恢复和生命周期等约束。
- 删除代码前检查菜单、槽、脚本、测试、旧模式和教学入口。
- Python 依赖使用 pyproject/uv.lock，格式使用 Ruff；C++ 使用 .clang-format。
- 不提交 SDK、密码、本机配置、数据库、日志、构建与部署输出。

提交前运行后端测试。客户端改动构建 Debug/Release 并检查相关 GUI 场景。打包改动检查 PrepareOnly 和部署包直接启动。CI 当前覆盖后端，授权 SDK 客户端需要本机验证。
