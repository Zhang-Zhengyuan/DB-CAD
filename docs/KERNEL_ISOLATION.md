# 内核隔离与替换指南

本文解释当前实现，供维护、评审和后续内核适配使用。核对日期：2026-10-17；代码基线：`fix/acis-kernel-isolation` / `ad93c4c`，当天通过 `git ls-remote` 确认与远端同名分支一致。后续代码变更应同步更新本文。

关联任务：[issue #4：隔离项目逻辑与 ACIS 内核，为后续 CAD 内核替换建立适配边界](https://github.com/Zhang-Zhengyuan/DB-CAD/issues/4)。

当前已完成现有业务调用与 ACIS 类型/API 的主要隔离，并建立独立适配库；尚未满足 issue 的全部验收条件。尤其缺少自动依赖防护、无 SDK 业务测试构建和非 ACIS 第二实现，不能据此宣布任意内核可以直接替换。

## 1. 演进与阅读基线

| 提交 | 作用 |
|---|---|
| `4def18c` 及后续 codec 提交 | 建立组件 SAT 回退，补强交线、过程曲面和属性风险检测 |
| `2f2c6f6` | 记录真实 C++ 输出与 Neo4j 往返的既有测试工作；发生在本轮隔离前 |
| `402136c` | 统一协作写入/历史链路，移除 Mode0、Bridge 和重复写入入口 |
| `42b5b7c` | 建立中立类型与接口，迁移 UI/文档调用，拆出 ACIS 静态库 |
| `628b0a6` | 保存第一轮实现记录 |
| `ad93c4c` | 修复冷启动 SAT FileInfo、稳定选择身份、撤销提交及依赖重建等问题，并加入客户端回归 |

核对隔离本身时用 `git diff 402136c..ad93c4c`；核对协作收敛与隔离的整体变化时用 `git diff 2f2c6f6..ad93c4c`。不要用外层旧副本代表本分支。

[设计方案](KERNEL_ISOLATION_DESIGN_20261016.md)、[首次实现记录](KERNEL_ISOLATION_IMPLEMENTATION_20261016.md)和[后续修复记录](KERNEL_ISOLATION_FIX_20261016.md)保留阶段背景；本文描述当前边界与未完成项。

## 2. 依赖方向与文件职责

```text
Qt 界面、Window 文档编辑、CollabSession、HTTP/PG
    │
    │ GeometryKernel / ShapeHandle / Point3 / Translation / DisplayData
    ▼
DbcadAcis 静态库
    ├─ AcisKernel：许可、初始化、对象注册、建模、变换、释放
    ├─ gme_mesh.cpp：ACIS 离散化 → 普通网格数据
    ├─ entity_graph_serializer/deserializer：ACIS JSON + SAT 编解码
    ├─ acis_sat_io.hxx：SAT 参数、临时文件与原生读写
    └─ access.cpp / storage/ / neo4j.cpp：旧图与历史兼容

HTTP → FastAPI → JsonGraphVersion / 不可变组件 → Neo4j
              后端不加载 ACIS；仍理解当前 ACIS 风格图协议
```

| 文件/模块 | 当前职责 |
|---|---|
| `CAD_DB/kernel/model_types.h` | DBCAD 自有句柄、操作枚举、点、平移、颜色、包围盒 |
| `CAD_DB/kernel/document_model.h` | UUID、逻辑索引、依赖、构造参数、显示设置及几何句柄 |
| `CAD_DB/kernel/geometry_kernel.h` | 建模、形状类别、平移、网格、原生存取、组件图与诊断接口 |
| `CAD_DB/kernel/acis_kernel.cpp`、`acis_primitives.inc` | 当前接口实现与工厂；原生对象只在适配器内部解析 |
| `CAD_DB/gme_mesh.hxx` | 中立显示 DTO，不含 FACE/EDGE/VERTEX 指针 |
| `CAD_DB/kernel/legacy_storage.h` | 旧 Neo4j 存储与诊断兼容端口，不是通用内核仓储接口 |
| `CAD_DB/DbcadAPI.h/.cpp` | 使用中立类型的兼容外观；业务主要建模调用直接经过 GeometryKernel |
| `CAD_DB/DbcadAcis.vcxproj` | 将 ACIS、codec、离散化和旧存储源文件编成静态库 |
| `CAD_DB/CAD_DB.vcxproj` | 业务/UI 目标，引用适配库；最终链接解析 ACIS 依赖 |

文档模型已经不含 SDK 对象，但仍包含显示缓存指针；文档、命令和协作协调仍有代码位于 Window/MainWindow。内核隔离不代表 UI 与业务核心也已完全拆开。

## 3. 身份、所有权、线程与错误

- **UUID**：持久业务身份，保存、协作和历史按 UUID 对齐。
- **ShapeHandle**：进程内单调生成的编号，不是原生指针，也不应持久化。适配器用注册表管理 `ENTITY*`；删除后继续使用旧句柄会报错。几何恢复后获得新句柄，UUID 可以保持不变。
- **逻辑索引**：实体树的本地稳定索引，清空后不复用；UI 不保存 vector 元素地址作为长期身份。
- **子拓扑 ID**：现有 codec 仍按地址生成，不能作为跨客户端稳定的面/边身份。协作冲突与替换单位是顶级 UUID 组件。
- **所有权**：适配器拥有原生几何；文档决定何时调用 erase。布尔使用保留输入的内核选项，业务成功后决定是否删除输入；边转线框时先复制输入。关闭内核前应先释放文档几何。
- **线程**：当前调用在 GUI 线程顺序执行，适配器是进程级单例，没有承诺线程安全、多个会话或多个内核并行。不要直接移到工作线程。
- **错误**：多数接口抛出 `std::runtime_error`；decodeGraph 返回 bool 与错误文本，mesh 返回 bool。尚无统一结构化错误码或能力查询接口。
- **单位/容差**：SAT 保存显式设置单位为 `1.0` 并使用当前运行内核版本；这避免首次保存依赖其他调用的全局 FileInfo。公共接口尚未提供文档单位、模型容差及跨内核误差策略，不能把这一设置解释成完整单位契约。

接口当前没有统一保证所有修改型操作失败后原对象完全不变。Pull 的暂存解码与 UI 提交顺序已改进，但 erase、变换和依赖批量重建仍需明确失败保证，并增加故障注入验证；不能把暂存恢复等同于任意内核操作的完整事务。

## 4. 建模、显示和历史如何经过边界

### 建模与显示

Window 收集业务操作、控制点和输入句柄，调用 create/operate/translate；AcisKernel 转换为 SDK 类型并执行原有算法。生成几何后，业务更新文档依赖、UUID 记账和显示缓存。mesh 输出普通顶点、三角形、法线、颜色和包围盒；GLWidget 绘制及拾取不再查询 ACIS 对象。

当前中立变换只覆盖平移；未实现的简化操作明确报不支持；交互拾取仍是顶级形状粒度。完整仿射变换、精确几何查询和能力差异协商尚未形成通用接口。

### 撤销/重做

`Window::captureHistory` 保存文档元数据与混合组件图，并将快照根的策略设为 SAT 恢复。快照中的 shape 和显示缓存指针被清空，不复制原生实体或 ACIS 历史对象。草稿单独记录，最多保留 50 个状态。

`restoreHistory` 先恢复几何，再释放当前几何，按 UUID 重建文档并重新记录协作变更。撤销后可以 Push。当前 captureHistory 仍调用接口 noteState，由 ACIS 实现调用原生历史 API；其他内核需要定义这一钩子的行为，项目快照不应要求其复制 ACIS bulletin/history。

远端合并后 resetDocumentHistory 建立新基线，防止本地撤销覆盖远端修改；代价是此前本地撤销历史也被清除。快照数量上限不是字节上限，大型模型的内存与延迟尚需测量。

## 5. 混合存储与协作

Push 只编码变更的顶级组件，encodeGraph 在适配器内部生成当前 v2 JSON 图和每根 SAT 备份；HTTP 携带 base_version、changes、removed_ids。后端校验并合并，保存同一条 JsonGraphVersion 链，未变化组件可复用。WebSocket 仅通知版本及在线状态。

恢复按组件 UUID 进行：普通 Body 的 `fallback=on_failure` 先尝试细图；已知有损组件的 `fallback=sat_fragment` 直接读 SAT；JSON 失败或组件不完整时再尝试该组件备份。整批成功后才交给调用方，失败时清理解码结果。UI 准备新文档树、释放被替换几何，再提交树和 UUID 映射；失败不推进已应用版本。

风险识别覆盖未知几何、属性链、部分过程曲面和缺失必要曲线载荷等。当前并未对每次成功的 JSON 重建全面验证几何等价，因此“携带 SAT”不等于任意静默失真都会自动被识别。保真声明应限于已有测试证据；后续应增加细粒度可信范围、合法性检查和适用的几何/属性校验。

远端几何作为导入对象替换旧构造配方；目前不传输完整参数化编辑历史，也不支持同一 Body 不同面的自动无冲突合并。详见 [统一协作链路](HYBRID_COLLABORATION.md)。

## 6. 允许保留的 ACIS 专用模块与数据兼容

以下是有意保留的实现边界，业务代码不应直接包含或调用它们：

- `acis.hxx/.cpp`、`kernel/acis_*`：SDK 初始化、许可、原生算法、SAT I/O。
- `entity_graph_serializer.h/.cpp`、`entity_graph_deserializer.cpp`：原生图 codec。其内部头仍暴露 ENTITY_LIST/void*，仅属于适配器内部接口，不是对业务开放的 GeometryKernel 契约。
- `access.hxx/.cpp`、`storage/`、`neo4j.hxx/.cpp`、`common.*`、`gme_dump_object.*`、`gme_mesh.cpp`：旧库、诊断、原生编码及网格实现，编入适配库。
- ACIS 专项测试可直接使用 SDK，以验证原生保真；不能因此允许 SDK 类型重新进入业务头。

文件仍在原路径不表示业务目标仍编译它们，应以 vcxproj 中的 ClCompile 和实际 include 为准。当前业务编译的 include 搜索路径仍可访问 SDK；缺少自动防护，尚不是强制禁止越界的构建体系。

数据兼容规则：

| 数据 | 当前行为及更换内核的要求 |
|---|---|
| ACIS SAT / 组件 SAT | 当前适配器读取；新内核必须支持对应格式或保留 ACIS 转换器，不能假定无损通用 |
| v1/v2 ACIS JSON 图 | 保留既有读取；v2 要求顶级 SAT。协议仍包含 ACIS 风格拓扑/几何语义，尚无通用表示描述 |
| 无 SAT 的旧 v1 组件 | 只能依赖已有细图，服务端不能补造原生信息；后续新提交该组件时补备份 |
| 旧 Mode0 / 独立 EG 历史 | 只读审计，不自动迁移或删除；需要旧客户端导出，再导入新项目 |
| 旧 Neo4j 原生增量历史 | 兼容端口仍依赖 ACIS history，内部有 AcisKernel 显式转换；新内核需保留转换工具或明确拒绝 |

schema 与 serializer_build 已有版本/构建标识，但尚未提供完整的内核来源、原生表示版本、单位和能力描述。不能仅修改工厂就承诺所有旧文件及数据库继续可读。

## 7. 编译时替换步骤

当前 `createGeometryKernel()` 固定返回 AcisKernel，`kernel()` 持有进程级单例。issue #4 不要求动态插件，当前采用编译时选择即可。

1. 实现 GeometryKernel，定义自身原生对象注册与释放规则；无覆盖能力显式报不支持，不能返回假成功。
2. 替换工厂实现和项目引用/链接配置。当前工厂与旧兼容函数同在 acis_kernel.cpp；接入第二实现时需分离装配代码，并明确保留或禁用旧菜单。
3. 提供 mesh 的中立 DTO，以及文档快照需要的编码/恢复能力；原生存取用自身格式。当前 JSON 后端有 ACIS 协议假设，需要兼容编码或有版本的协议演进。
4. 对旧 SAT、旧 JSON 和 Neo4j 历史给出导入、转换或显式拒绝策略，并告知调用方。
5. 用真实第二实现完成代表性建模→平移→网格/显示→保存恢复，确认主体业务调用方不修改；再扩大能力范围。

Qt 类型是当前接口的依赖，接口不是无 Qt 的通用 SDK。现有最终客户端仍链接 ACIS，且保留 /MD、/EHa、4 MB 栈和 SPA_NO_AUTO_LINK；独立静态库并不消除最终程序的运行时与许可要求。

## 8. issue #4 验收对照

以下是代码/记录核对，不是维护者评审签字，也不是本次重新执行测试的结果。

| issue 验收项 | 当前状态 | 证据与剩余工作 |
|---|---|---|
| 边界、契约、泄漏清单、兼容策略、迁移计划，经评审 | 部分满足 | 已有设计文档与本文；仍需维护者评审，补齐单位/容差、失败保证和能力差异契约 |
| 业务/协作/公共 UI 不暴露 SDK；例外明确 | 主要满足 | 公共调用改为句柄和值类型，专用 codec/旧库留适配器；允许例外见第 6 节，仍需自动覆盖全部依赖图 |
| 自动依赖检查；无 SDK 测试替身编译业务 | 未满足 | 本次静态扫描无类型/API 命中，但不是自动门禁；当前 regression 依赖真实 ACIS，没有替身构建目标 |
| 独立适配器、编译契约、许可与运行时，双配置通过 | 已有通过记录 | vcxproj、acis_link.props 与修复记录；最终程序承担栈/链接约束。本次未重跑构建 |
| 至少 9 类/36 项语义检查及所有权/失败回滚 | 部分满足 | 已有 76 项内核检查、18 组图往返记录；计数不等于逐条覆盖 issue 的 36 项，仍需映射及故障回滚检查 |
| 存取、历史、编辑、显示、双客户端协作全面回归 | 部分满足，部分条目已退役 | 有客户端/HTTP/WS 回归；旧 Mode0/Bridge 已删除，应将其验收改为退役/兼容验证。隔离后的真实 Neo4j、旧菜单和完整双客户端/重连证据仍需补齐 |
| 非 ACIS 第二适配器/可执行原型 | 未满足 | 当前只有 AcisKernel；测试替身不能替代真实第二实现 |

结论：可以作为“主要调用隔离与适配库拆分已完成”的阶段成果；按 issue 原文完整关闭还不充分。运行时插件不属于当前缺口，无需为满足该 issue 先增加动态加载框架。

## 9. 验证入口与后续顺序

已有修复记录报告 Release/Debug 各通过 76 项内核检查和 25 项客户端/HTTP/WS/窗口检查，另有 18 组图往返与 44 项后端测试通过。客户端联调用真实 Qt/ACIS/FastAPI 与测试 MemoryStore，不代表隔离后已验证用户 Neo4j 部署。隔离前的真实数据库测试也不能直接替代隔离后的验收。

配置依赖和许可后复跑：

```powershell
.\scripts\tests\run-client-regression.ps1 -Configuration Release
.\scripts\tests\run-client-regression.ps1 -Configuration Debug
```

脚本会构建并短暂显示测试窗口、启动本机临时测试服务，日志输出至 artifacts。后端与几何专项验证范围见 [修复记录](KERNEL_ISOLATION_FIX_20261016.md) 和 [统一协作链路说明](HYBRID_COLLABORATION.md)。

建议分阶段补齐：先确认本文与 issue 的范围，建立业务依赖门禁及无 SDK 测试目标；再明确失败/单位/能力契约并映射语义测试；随后完成第二适配器最小闭环和旧数据兼容方案；最后补真实数据库与双客户端回归。性能优化依据包大小、快照内存及数据库增长实测推进。
