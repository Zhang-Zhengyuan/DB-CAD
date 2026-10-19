# C++ worker/内核补充规则

继承根 AGENTS.md；后端与部署另见其目录规则。先按根项目地图定位，不读整个 ACIS SDK。

- 默认构建目标是 `dbcad_worker.exe`（服务端权威的 ACIS worker）。改文件归属同时核查 `CAD_DB.vcxproj` 与 `.filters`，避免仅编辑源码却未参与构建；Qt 系头文件/库不得回流。
- ACIS 只在本目录内：许可初始化在 `acis.cpp`，命令执行在 `worker/`，网格与 GLB 在 `kernel/`。中立头（`kernel/model_types.h`）不引入 ACIS 类型或授权信息；Python 网关与前端不出现内核头。
- worker 内实体指针即会话内身份（ACIS backup 语义下跨命令稳定）；持久身份是组件 `hash_sat` 与实体 `entity_key`（内容寻址），不能混淆。命令失败不得产出半成品版本。
- ACIS API 的 outcome、历史配对、异常路径和资源清理由当前实现管理；查证当前 SDK 声明，不按参考 PDF 残缺代码猜测实现。
- 改命令要验证 selftest、mesh_tests（`/p:DBCADMeshTests=true`）与后端真栈集成；改导入导出验证 SAT/STEP 往返（entity_key 集合相等口径）。构建/回归命令见 `docs/ai/VALIDATION.md`（仓库根）。
