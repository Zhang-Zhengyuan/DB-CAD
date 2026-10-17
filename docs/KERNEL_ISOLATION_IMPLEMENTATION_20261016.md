# 内核隔离实现记录

> 本文记录首次隔离提交，当时按要求仅做编译检查。后续故障修复与回归见 [修复记录](KERNEL_ISOLATION_FIX_20261016.md)；当前实现、替换步骤和 issue #4 验收状态见 [内核隔离与替换指南](KERNEL_ISOLATION.md)。

本轮按已确认方案实现业务与 ACIS 隔离。代码提交：`42b5b7c`（隔离几何内核与业务调用）。未新增或运行测试；进行了 Release / Debug 编译检查。

## 已建立的边界

```text
CAD_DB（Qt 界面、文档、依赖关系、协作会话、HTTP、PG）
    ↓ GeometryKernel / ShapeHandle / Point3 / DisplayData
DbcadAcis.lib（当前适配器）
    ├─ ACIS 初始化、原生对象注册表、建模、变换、删除、离散化
    ├─ 原有混合 JSON / SAT 编解码
    └─ 旧版 Neo4j 图存储与历史增量兼容入口
```

- `kernel/model_types.h`：业务操作枚举、形状句柄、点、位移、颜色和包围盒。不包含 SDK 类型。
- `kernel/document_model.h`：业务 UUID、逻辑索引、依赖、手柄、显示设置。`shape` 是会话句柄。
- `kernel/geometry_kernel.h`：内核契约；创建、运算、变换、颜色、释放、网格、原生字节流、组件图和诊断。
- `kernel/acis_kernel.cpp` / `acis_primitives.inc`：唯一的当前适配器入口，保留原有基本体、曲面和曲线构造算法。
- `kernel/legacy_storage.h`：旧 Neo4j 存取和既有诊断菜单的兼容端口，不向 UI 暴露驱动或 SDK 对象。
- `DbcadAcis.vcxproj`：独立静态库；既有 `access*`、实体图编解码和离散化源文件编入这里。保留其原路径，方便核对与现有诊断脚本兼容。
- `CAD_DB.vcxproj`：界面及业务逻辑引用适配器项目。最终链接器通过 `kernel/acis_link.props` 解析 ACIS 静态库依赖；保留 /MD、/EHa 和 4 MB 栈契约。

## 所有权和行为

1. UUID 是持久业务身份，ShapeHandle 是进程内单调编号，二者不互相替代。没有公开原生指针提取接口；释放后再次使用句柄会报错。关闭内核时回收仍注册的顶级几何。
2. UI 的基本体、布尔、求交、去重、手柄重建和平移通过接口调用。布尔保留输入直到业务层决定删除；边转线框前复制，避免输入边被临时拓扑接管。原来尚未实现的简化操作仍返回不支持。
3. DisplayData 只含坐标、三角形、法线、颜色、范围、包围盒和所属形状句柄。OpenGL 高亮和拾取不再查询 FACE/EDGE/VERTEX 或 ACIS 包围盒。当前交互选择粒度仍是顶级形状。
4. 协作 Push 经 encodeGraph；Pull 经 decodeGraph，全部组件重建成功后准备新的业务树，释放旧几何成功后提交树和 UUID 映射。保留现有组件 SAT 兜底、v1/v2 格式及后端服务。
5. SAT 文件及 PG 的原生内容通过字节流接口读写，ACIS 的文件版本、FileInfo 和临时文件属于适配器。SAT 与旧版 Neo4j 全量读取先解码，再替换文档。
6. 旧版 Neo4j 图和历史增量继续使用原实现。历史恢复会清空 ACIS 默认流，因此兼容端口要求先释放当前文档句柄，防止原生对象被无声删除后留下悬空句柄。
7. 文档撤销/重做改为业务元数据与混合组件图快照，按 UUID 恢复，避免让 UI 持有 DELTA_STATE 或在重建历史时调用破坏历史的 clear。草稿单独记录；保留最近 50 个状态。恢复会进入本地协作变更账本。远端合并后建立新的撤销基线，避免回滚同事的修改。旧 GME_DELTA_STATE_user_data 已移除。
8. 原有存取诊断菜单保留在兼容实现，本轮没有执行。

## 替换内核时的修改点

实现 GeometryKernel，替换 createGeometryKernel 工厂实现和项目引用/链接属性。Qt 业务/UI、协作会话与后端 API 可继续使用现有契约。旧 ACIS 图/SAT 数据的读取需要新内核支持导入或保留 ACIS 转换器；接口隔离本身不承诺跨内核无损迁移。旧版 Neo4j 历史公告语义也是 ACIS 专属能力，应保留兼容转换器或显式拒绝，不能由新内核伪装支持。

当前变换契约覆盖现有交互的平移，完整仿射变换应在增加旋转、缩放功能时扩展中立值类型。当前内核仍运行在 GUI 线程；尚未引入插件加载、多内核并行或 RPC。

## 验证与后续关注

- Release / Debug 均成功编译、链接客户端与 DbcadAcis 静态库。
- `git diff --check` 通过。
- 源码检查：UI、Window、GLWidget、MainWindow 和公开 DbcadAPI 无 ENTITY*、ENTITY_LIST、SPA 数学类型、ACIS API 宏或 ACIS 头文件。
- 未执行功能、几何等价、协作集成或性能测试，编译通过不代表这些已验证。
- 快照包含组件 SAT 备份，可能增加内存与编辑延迟，已限制状态数量；大型模型的快照粒度/压缩可在后续性能工作中改进。
- 原有曲面网格缺失时仍可能不显示该几何；现接口对此报告网格失败，不把空显示误报为成功。
- 下轮建议验证构造/编辑、布尔输入生命周期、撤销后 Push、远端合并基线、文件/PG/旧 Neo4j 往返与复杂曲面显示。测试实现仍按本轮要求延期。
