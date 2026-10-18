# 客户端补充规则

继承根 AGENTS.md；后端与部署另见其目录规则。先按根项目地图定位，不读整个 ACIS SDK。

- 两个目标：CAD_DB 是业务/UI，DbcadAcis 是内核适配及专用存储兼容。改文件归属同时核查 `.vcxproj` 与 `.filters`，避免仅编辑源码却未参与构建。
- 新业务走 `kernel/geometry_kernel.h`；中立头不引入 ACIS 类型/头或授权信息。跨 DLL/静态库边界注意现有构建配置与所有权。
- ShapeHandle 是会话内句柄，UUID 是持久身份，不能互换。恢复/撤销/Pull 后重新获取有效句柄，不能沿用失效原生指针。
- ACIS API 的 outcome、历史配对、异常路径和资源清理由当前适配层管理；查证当前 SDK 声明，不按参考 PDF 残缺代码猜测实现。
- 更改绘制/拾取时保持 DisplayData 普通数据边界；耗时网络/几何调用遵循既有 Qt 线程/生命周期规则，避免 UI 阻塞和对象悬空。
- 改建模/存取要验证保存重开、撤销重做、相关 JSON/SAT 往返和失败恢复；改协作检查本地修改保护和 base_version。构建/回归命令见 `docs/ai/VALIDATION.md`（仓库根）。
