# 本轮重构的合并准备

候选标题：`refactor: modularize CAD client and backend; document reproducible setup`

## 可用于 PR 的说明

当前客户端 UI、几何存储和后端路由集中在大文件中，部署依赖本机布局。本次按职责拆分模块，保留现有协议和功能入口，增加可配置构建、锁定后端依赖、完整部署资源与团队上手文档；同时修复真实数据库检查中发现的路由、DTO 和实体关系存取问题。

本地后端 32 项测试通过，Debug/Release 与干净源码 Release 构建通过。双客户端源码 Debug、默认 Release 和独立部署包全栈启动通过；两份窗口响应正常，协作接口与 WebUI 冒烟检查通过。真实数据库检查和仍需人工执行的 GUI 场景详见 `docs/history/REFACTOR_VALIDATION.md`。SDK 仍需维护者按授权提供。

## 提交与验收

- [ ] 审查本地差异，区分已有工作、代码搬移和行为修复。
- [ ] 确认新文件（ui/storage/api/static、脚本、文档、CI）全部进入提交；确认无 SDK、密码、日志、数据库与输出目录。
- [ ] 完成验证记录中的独立包与双客户端人工验收。
- [ ] 在另一位成员机器按 GETTING_STARTED 复现并记录 SDK 版本。
- [ ] 确认 master 与开发分支无共同祖先的处理方案，再选择可以正常 PR 的分支。
- [ ] 手动 commit/push 并创建 PR；至少一位团队成员审查后合并。

本轮没有自动提交或推送。`git diff --stat` 不包含未跟踪文件，审查时同时检查 `git status --short`。
