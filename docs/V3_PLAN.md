# DB-CAD v3 总计划：全新 API + 工业级 Web 客户端

> 拟定：2026-10-18。取代 [UI_REFACTOR_PLAN.md](UI_REFACTOR_PLAN.md)（该计划升级为本计划）。
> 一句话定位：**本项目的差异化不在 UI，而在自研的图数据库存储 + 版本管理 + 协同编辑（FastAPI 后端）**。v3 把这套核心价值用一套全新、完整、高性能的 API 暴露出来，上面跑一个工业级的 Web 客户端；旧 API 全部废弃，Qt 客户端先冻结后删除。

## 0. 三条原则（定下来不再讨论）

1. **旧 API 全部废弃**：json-delta 推拉协议、旧 WS 通知、旧 WebUI 端点标记 deprecated，v3 上线验收后物理删除。不搞兼容层、不做双写过渡。
2. **服务端权威（server-authoritative）**：建模命令在服务侧执行（worker 里的 ACIS），客户端只提交命令、渲染网格。这是 Onshape 验证过的云端 CAD 架构——几何计算集中、每个协作者看到同一状态、天然配合数据库式版本管理。旧的"客户端本地建模 + 推 json-delta"模式随之退役。
3. **几何内核边界保留，实现刷新**：`GeometryKernel` 的中立接口设计（PR #5 成果，输出全是句柄/坐标/JSON，无 SDK 类型泄漏）是对的，保留并扩展（参数化 create、新 MeshExtractor）；`gme_mesh` 旧离散化实现废弃重写。**导入导出面（2026-10-18 勘察 D:\ACISCAD 全套授权包后修正）**：授权包含完整 Interop（STEP/IGES/CATIA/SolidWorks/Inventor 转换器，导入库齐全），只是 `_deps` 尚未引入——SAT 原生 + GLB/STL 先行，STEP 纳入 P5（引入 `_deps` 即可用，无需新授权）；compound 系面片 API 在独立 DLL 且无导入库，不使用（见 P0_MESH_BENCH.md §6）。

## 1. 目标架构

### ADR-1 几何计算的位置：服务端权威（2026-10-18 决定）

**问题**：ACIS 是 C++ SDK 进不了浏览器，跑内核的进程放云上还是用户本机？

- A. 服务端权威 worker（选定）：云上执行几何命令，浏览器薄客户端。赢在：开链接即用；唯一真相（命令回放/分支/冲突全在服务端，正中本项目版本管理+协同的差异化）；网格缓存全员复用；粗操作定位下交互延迟（~100-300ms，拖拽用本地矩阵预览兜底）可接受。代价：worker 进程管理（先单实例+每项目排队）、ACIS 服务进程授权需确认。
- B. 每用户本地伴随进程（浏览器连 localhost）：交互零延迟、授权简单，但网页 CAD 退化成"先装本地服务"，N 份几何状态各自合并，冲突协议复杂化。
- C. 原生客户端自算自推（现状 json-delta）：无网页化。

**可逆性**：worker 是独立进程、说同一种 API；将来若有离线/超大本地文件需求，同一 worker 二进制移到用户本机运行、指向同一云端 API 即演化为 B，协议不变。业界印证：Onshape（A）、Fusion 360（B，桌面形态所以本地内核合理）——**内核位置跟着 UI 形态走，浏览器形态必然服务端**。

### ADR-2 内核并发模型：进程隔离 + 每项目串行命令队列（2026-10-18 决定）

**问题**：ACIS 不支持单进程多线程并发建模（实证：`acis_kernel.cpp:44` 每进程一次 `initialize_acis()`，`:49` 注释确认 modeller 状态为 thread-local；`mutex.hxx` 线程原语挂 `THREAD_SAFE_ACIS` 编译条件；`gme_mesh.cpp:367` 注释表明本项目一直按单线程持有内核）。三人同时编辑如何成立？

**答案：视图是客户端状态，内核状态只有一个/项目。** 三种场景全覆盖：

1. **3 人编辑 3 个不同项目** → 每项目一个 worker 进程（按需拉起、LRU 回收），OS 级真并行，各进程独立 ACIS 会话；
2. **3 人编辑同一项目** → 权威模型只有一个，命令进每项目串行队列逐个执行，每条命令=一个版本——**命令全序恰好就是版本链**，这不是妥协而是版本管理本来就要的语义（Onshape 同此）。三人各自的相机/选中/高亮/未提交预览全是浏览器本地状态，内核不感知；
3. **同项目分叉编辑** → 分支（M4.4）：worker 可从任意版本物化一个会话（SAT 组件重建），每分支独立会话。

**性能账**：每项目串行的吞吐上限 = 单命令耗时（粗操作 10-500ms）×队列——3 人粗操作绰绰有余；跨项目扩展 = 加进程。单进程内并发建模永远不做（内核性质 + 无收益）。

```text
浏览器（React + three.js，薄客户端：渲染 + 交互输入）
  │  REST /api/v1 + WS /api/v1/events（唯一协议，OpenAPI 契约先行）
  ▼
FastAPI 网关（鉴权/限流/静态托管）
  ├─ 项目/版本/标签/分支 · 组件(不可变,内容寻址) · 命令执行 · 在场/事件
  ├─ 网格分发 /api/v1/assets/{hash}.glb（不可变缓存,任意 CDN 语义）
  └─ 内部调度 ──▶ 几何 worker（C++ 控制台进程,可多实例）
                    ACIS 建模/离散化/序列化；执行命令→产出新组件
                    ▼
               Neo4j 图存储（自研存储层：不可变组件 + 版本链/DAG）
```

与 v2 的本质区别：**命令执行从客户端搬进服务侧**。浏览器拖 gizmo → `POST /commands`（带 base_version）→ worker 在权威几何上执行 → 提交新版本（变更组件按内容寻址入库）→ WS 广播 → 所有客户端增量拉取变更组件的网格。撤销/重做也是命令。冲突 = base_version 过期 → 服务端按命令重放语义自动前移（rebase），几何级冲突（同组件并发）仍诚实拒绝。

## 2. API v1 设计（契约先行，OpenAPI 起草→评审→冻结→实现）

| 端点 | 方法 | 说明 |
|---|---|---|
| `/api/v1/projects` | GET/POST | 项目列表/创建 |
| `/api/v1/projects/{id}` | GET/PATCH/DELETE | 项目详情/改名/删除 |
| `/api/v1/projects/{id}/versions` | GET/POST | 版本列表（微版本+定版标签）；POST = 定版 |
| `/api/v1/projects/{id}/versions/{v}` | GET | 版本详情（组件清单、父版本、作者、时间） |
| `/api/v1/projects/{id}/branches` | GET/POST | 分支（版本链升级 DAG，预留 M4.4） |
| `/api/v1/projects/{id}/scene?v=` | GET | 场景清单：组件引用列表（hash、变换、名称、颜色、包围盒）|
| `/api/v1/assets/{hash}` | GET | 不可变资产：`.sat` 几何源 / `.glb` 网格，强缓存 |
| `/api/v1/projects/{id}/commands` | POST | **核心**：执行建模命令（id+参数+base_version）→ 返回新版本 + 变更组件 hash 列表 |
| `/api/v1/projects/{id}/commands/dry-run` | POST | 预演（高亮影响范围/校验参数，不落库） |
| `/api/v1/projects/{id}/undo` `/redo` | POST | 撤销/重做（命令的逆，服务端栈） |
| `/api/v1/files/import` | POST | 上传 SAT → 建为项目 |
| `/api/v1/files/export?v=&fmt=` | GET | 导出 SAT/GLB/STL |
| `/api/v1/schema/commands` | GET | 全部命令的 id→参数 JSON Schema（前端表单/校验/AI 共用） |
| `/api/v1/projects/{id}/entities?v=&label=` | GET | 实体级投影查询（P6，ADR-3）：版本定位 + label 过滤（如 Face）；WebUI/AI 场景摘要/演示共用 |
| `/api/v1/events` | WS | 版本提交、分支、在场（presence）、edit_intent、冲突通知 |

性能基线：不可变资产内容寻址 + `Cache-Control: immutable`；场景清单 ≤ 数 KB；命令响应只带变更增量；WS 推送替代轮询；GLB 后期挂 `EXT_meshopt_compression`。

## 3. 功能矩阵

### 3.1 原有 Qt 客户端功能（v3 必须全覆盖——基线）

| 域 | 现状（mainwindow_menus.cpp / 回归测试） |
|---|---|
| 图元 | 体 8 种（立方/球/圆柱/圆锥/圆环/棱柱/金字塔/摆动面）；面 16 种（含 B 样条×3、公式面）；线 10 种（含螺旋/弹簧/方程/贝塞尔/B样条） |
| 操作 | 求交器、布尔并/交/差、去重 |
| 变换 | 平移（含 bake）；删除；撤销/重做 |
| 显示 | 面/边/全部三模式、逐图元颜色 |
| 文件 | SAT 打开/保存/另存 |
| 协作 | 项目打开/推送/拉取/另存、自动拉取、在场感知、定版标签、版本切换、base_version 冲突（409） |

### 3.2 工业级增量（参照市面 CAD 的基本功能要求）

| 优先级 | 功能 | 说明 |
|---|---|---|
| P0 | 标准视图（前/后/左/右/顶/底/等轴测）、适配视图 | 工业软件入门动作 |
| P0 | 显示模式：着色 / 线框 / 带边着色 / 仅边 | 现 DISPLAY_TYPES 扩展 |
| P0 | 参数化插入 | 图元带尺寸参数（长宽高/半径…），替代固定 draft |
| P0 | 变换扩展：平移/旋转/缩放（gizmo 三轴） | 现 translate 扩展 |
| P0 | 属性面板：类型/包围盒/体积/表面积/颜色 | 体积/面积由内核计算 |
| P0 | 版本时间线 UI（微版本流+定版标签+作者时间） | 协同核心价值可视化 |
| P1 | 测量（点到点距离、边长） | 前端 Raycast 即可 |
| P1 | 剖切面（平面剖切视图） | three.js clippingPlanes |
| P1 | 导入 SAT / 导出 SAT·GLB·STL | API 已含 |
| P1 | 在场与编辑意图可视化（谁在编辑哪个组件） | edit_intent 数据已有 |
| P1 | 冲突 UX：409 → 对比 + 重试/放弃 | 替代裸报错 |
| P2 | 分支创建/切换/合并（异组件自动+同组件三选一） | M4.4 转正，协议已预留 |
| P2 | 阵列/镜像、单位制、快照对比（同图元两版本叠加 diff 高亮） | 语义 diff（M4.5）的第一步 |

## 4. 里程碑（单人+AI 辅助估算；每步门禁绿才进下一步）

| 里程碑 | 内容 | 验收 | 估时 |
|---|---|---|---|
| **P0 面片引擎** | 四路径离散化调研（默认档/visualization/precise/sequential_mesh，抽样基准）→ 重写 `MeshExtractor`（质量档位、内核法线、批量）→ GLB+manifest 序列化 | **已完成（2026-10-18）**：调研选型 B `facet_options_visualization`（[P0_MESH_BENCH.md](P0_MESH_BENCH.md)，compound API 不可用）；`kernel/mesh_extractor`（三档质量、内核法线、边折线、显式错误策略）+ `kernel/glb_writer`（glTF 2.0 二进制 + faceRanges extras）已交付，`mesh_tests` 171 项真实内核校验全绿（含变换语义实证：边折线须乘一次宿主变换，af_api.hxx 文档声明不实） | **1 周** |
| **P1 API 契约 + 服务端骨架** | OpenAPI v1 草案评审冻结；组件内容寻址入库（SAT 源+GLB 资产双写）；版本提交协议（含 branches 预留）；worker 进程管理与内部调度；WS events | **已完成（2026-10-18）**：契约冻结（`tests/test_api_v1_contract.py` 路径守门）；`app/component_store`（内容寻址，并发幂等——Windows 竞态已修）；`app/v1_store`（Memory/Neo4j 双实现，base_version 409 乐观锁，标签/分支预留）；`CAD_DB/worker` 批处理 CLI（import/merge/export/selftest，目标 DBCADWorker，Qt-free，ACIS 全链路自检绿）；`/api/v1` 全端点 + WS events；真栈集成测试绿（SAT→ACIS→GLB→scene/export 闭环）。常驻 worker+每项目串行队列随 P2 命令引擎升级（批处理即退化单实例） | 1.5-2 周 |
| **P2 命令执行引擎（服务端全功能）** | CommandBus 面向服务器化：全部 34 图元 + 布尔/求交/去重 + 变换 + undo/redo 栈；参数 schema 端点；dry-run；rebase 语义 | **已完成（2026-10-18）**：worker `serve` 常驻行协议（nlohmann 已 vendor，session = 进程内权威几何）；命令集 10 个 action——参数化创建（33 图元经 acis_primitives.inc 复用，体图元带尺寸参数）/平移/旋转/缩放/删除/布尔×3/求交器/去重；`GeometryPool` 每项目一进程 + 串行锁（Popen+to_thread，规避 asyncio subprocess 的 loop 亲和）；rebase 语义（base 清单与 head 的消失集合判定，sid=内容哈希）；树状 undo/redo（清单重放）；schema/dry-run 端点。真栈集成测试通过：纯 API 跑通 创建→变换→布尔→场景/资产→撤销→导出，面/线图元为同级组件。附带修复：serialize_triangles 返回三角形数非索引数（曾致全部网格少 2/3，mesh_tests 已钉死精确计数） | 1 周 |
| **P3 Web 查看器** | R3F 脚手架；scene/asset 增量加载；相机/标准视图/显示模式/边线；组件树、点选高亮、属性面板；版本时间线 | **已完成（2026-10-18）**：`web/`（React 19 + R3F 9 + drei + zustand + Vite 7 + TS strict，typecheck+build 绿）；后端挂载 `/web`（StaticFiles，vite base=/web/）。查看器：GLB 组件渲染（内核 facet 边线=GLB LINES 图元，非前端重算）、显示模式四档（着色/带边/线框/仅边）、标准视图八向+适配（z-up 匹配 ACIS）、点选高亮（emissive+边线变色）、组件树、属性面板（名称/包围盒/尺寸/三角数/哈希）、版本时间线（微版本+定版标签，按版本缓存清单——切换零请求）。实测（真栈 + 浏览器）：项目打开→6 组件（体×4/面/线）GLB 全 200 解析，v6→v3→v6 组件数精确切换，模式/视图交互无错误 | 1.5 周 |
| **P4 建模与协同交互** | 工具栏+参数化插入表单（schema 驱动生成）；gizmo 平移/旋转/缩放；布尔/删除/撤销；双端协同（WS 增量刷新、在场、edit_intent、409 冲突 UX） | **已完成（2026-10-18）**：插入表单由 /schema/commands 驱动生成（33 图元+参数网格）；TransformControls gizmo（平移/旋转/缩放，松手增量→命令，本地仅预览——服务端权威）；布尔并/差/交+删除+树状撤销/重做工具栏；WS 双向：presence/edit_intent 广播（后端 hub 名单/意图表）；版本提交→其他标签零刷新同步；409→冲突横幅+自动拉 head；ErrorBoundary 显式崩溃；/web 入口 no-cache（根治换构建白屏）。实测双标签（alice/bob）：A 插入 cone→B 秒同步 v7/7 组件；A 选中→B 显示 ✏alice 徽标；B 撤销→A 同步 v8。**附带修复 P0 遗留缺陷**：GlbWriter bufferView/accessor 共用计数器致 primitive 引用越界（GLTFLoader 实炸），分离编号+mesh_tests 交叉引用校验（175 项）+ web/scripts/validate-assets.mjs（GLTFLoader 全量解析组件库，6/6 绿） | 1.5-2 周 |
| **P5 工业级收口** | 测量、剖切、导入导出（含 STEP：从全套授权包引入 Interop 到 `_deps`）、性能压测（abc_cache 万面模型 + demo_models：meshopt/LOD/懒加载）、Playwright e2e 门禁 | **大部分完成（2026-10-18）**：测量（点两点→raycast 距离标签）、剖切（X/Y/Z 轴+包围盒范围滑杆，材质 clippingPlanes）、导入 SAT（UI 上传）/导出 SAT·GLB·STL（worker ASCII STL 新增）、Playwright e2e 门禁（真浏览器+真后端+真 ACIS：渲染/属性/资产强缓存头/测量/schema 表单建模/版本切换/显示模式，5s 绿）；顺带修复两个真实缺陷：初始相机不对中（几何出视锥→CameraRig 首帧 iso 对中）、`.viewport>div` CSS 误伤工具栏（按钮拉成整列高吞掉画布点击→canvas-host 专属类）。**遗留**：STEP（Interop 引入 _deps+worker step 命令，授权已含待接）、大规模性能压测（abc_cache 语料） | 1 周 |
| **P6 实体级图投影（ADR-3）** | `entity_graph_serializer` 去 Qt 化（QJson*→nlohmann，uvsafe-topology-v5-bounded codec 语义不动）搬入 worker，emit 管线产出 `{sid}.graph.json`；网关组件入库时写 Neo4j 实体投影（`(V1Component)-[:HAS_ENTITY]->实体节点`，"细"档物理表示复用 Phase B 字段映射与 `:Unknown` 兜底）；只读查询端点 `/entities`；既有 L1 往返 fixtures 复用为 codec 回归基准 | 改一个 face：未变更组件投影零写入、变更组件投影幂等重生成；`MATCH (f:Face) WHERE f.geometry_type='spline'` 原生可查；投影读回与 serializer 输出等价（L3 断言） | **已完成（2026-10-19）**：`worker/entity_graph_codec`（Qt-free 移植，几何载荷/区间/NURBS RLE/风险判定逐 case 保真；sat_fragment 内嵌裁剪为 fallback 标记 fine_grained/sat_asset——组件 SAT 已是独立内容寻址资产）；emit/import 两管线产出 graph.json（selftest 断言钉死 schema/root/拓扑计数）；组件库第三资产 kind=graph；v1_store 双实现投影接口（HAS_ENTITY + TOPOLOGY{rel_type} 统一边型 + 细档字段，幂等零写入）；commands/files commit 后写投影（root 临时 sid 重编为 SAT 哈希）；`/entities` 端点（label 大小写不敏感）；scene 组件带 graph 资产哈希。门禁全绿：后端 96 项（新增契约 7 + 真 ACIS 集成验收：cube 真拓扑 6 face/12 edge/8 vertex、label=face→plane+uv_range 细档、幂等不膨胀 + Neo4j L3 细档原生查询/Unknown 兜底/TOPOLOGY 边）；worker selftest、mesh_tests 175、Qt 客户端构建均绿。遗留：P6 前存量组件无投影（重导或重建脚本补齐，小任务） | 1-1.5 周 |
| **P7 旧物拆除（原 P6，范围收缩）** | json-delta/旧 WS/旧 WebUI 端点与 `json_graph_store/json_collab/collaboration` 删除；Qt 客户端与 QtCore 依赖删除（core 换 nlohmann/std）；`entity_graph_deserializer` 随 v2 删（v3 组件恢复走 SAT restore），serializer 已于 P6 搬家保留；`neo4j_webui` 改造为 V1 模型+投影浏览（不随旧协议删除）；构建与部署脚本瘦身 | 构建无 Qt；仓库无旧协议代码；全部门禁绿；tag `v3` | 1-1.5 周 |
| **P8 实体级增量（Phase D，论文第 5 章完整恢复）** | 组件内实体级 diff（内容寻址只写新实体节点）；generation 式版本回放（移植 D 栈 `generation_new_ptr/old_ptr` 不可变引用语义）；压实（后台合并连续微版本，控节点/边膨胀）；edit_intent 软锁与 409 几何冲突判定细化到实体级；为语义 diff（M4.5）与 AI 场景摘要供数据基础 | 改一个 face 半径的二次提交，新增实体节点数 ≈ 变更实体数；版本回放与全量图等价（L3 断言） | 2-3 周 |

**P0–P5 已于 2026-10-18 完成；剩余 P6–P8 约 4.5–6 周。**实体级投影（P6）先于拆除（P7）是有意排序：serializer 搬家先行，P7 拆除范围随之收缩（见变更记录 2026-10-19）。P4 验收后旧客户端已事实退役，P7 物理清除。

## 5. 风险与对策

| 风险 | 对策 |
|---|---|
| 服务端权威重构动到协同语义（回放/rebase 正确性） | P1 起命令执行带全量契约测试；沿用"组件不可变+版本清单"存储模型，存储层不动，只换上层协议 |
| worker 多实例状态/竞态 | P1 先单实例+每项目串行队列（同既有 busy 闸门思想），扩容留到需要时 |
| 34 种图元逐一服务化的工作量 | CommandBus 已有 id→schema→执行器结构，映射机械；按"体→布尔→面→线"分批验收 |
| 大模型性能 | 内容寻址缓存 + 增量清单已把传输最小化；abc_cache 万模型语料是现成压测床 |
| ACIS 授权（服务进程多实例） | P1 前书面确认；单实例起步可规避 |
| 前端 3D 生态演进 | 锁版本；WebGPU 默认带 WebGL2 回退，只用标准能力 |
| 实体身份/拓扑命名（投影跨版本追踪） | 第一版投影挂不可变组件之下、随组件幂等生成，零 uuid 依赖；跨版本"同一实体"追踪推迟到 P8（内容寻址 + generation 链），语义同 D 栈已验证实现 |
| serializer 去 Qt 化引入回归 | codec 语义（uvsafe-topology-v5-bounded）不动，仅容器类型替换；用既有 L1 序列化往返基准（probe_complex_graph.json 等 fixtures）钉死 |

## 6. 明确不做（本期）

草图约束/工程制图/图纸输出、装配体结构、离线模式、移动端。STEP 已升级为 P5 范围（授权已含 Interop，见 §0.3）。这些是 v3 之后的路线。

## 7. 变更记录

- 2026-10-18 拟定。取代 UI_REFACTOR_PLAN.md（UI 视角）为本计划（项目总视角）：旧 API 全废弃、服务端权威命令执行、工业级功能矩阵、8-10 周里程碑。
- 2026-10-18 P0 完成：MeshExtractor + GlbWriter 交付（`kernel/mesh_extractor`、`kernel/glb_writer`，mesh_tests 171 项全绿）。实证勘误：完整球/环体可无边拓扑（轮廓由着色呈现）；`api_get_facet_edge_points` 实返局部坐标（SDK 头文档声明不实，须乘一次宿主变换）。
- 2026-10-18 P1 完成：`/api/v1` 全契约落地（projects/versions/branches/scene/assets/commands 占位/WS events）；组件内容寻址存储（SAT+GLB 双写，并发幂等）；Qt-free worker CLI（import/merge/export/selftest）；真栈集成测试通过。命令引擎（501 占位）与常驻 worker 随 P2 交付。
- 2026-10-18 P2 完成：常驻 worker serve 协议 + 命令引擎（10 action）+ GeometryPool（每项目串行）+ rebase（消失集合判定）+ 树状 undo/redo + schema/dry-run；真栈集成测试通过（纯 API 建模全流程）。MeshExtractor 支持 FACE/EDGE 顶层图元（面/线与体同级组件）；GlbWriter 支持纯线网格；vendor nlohmann/json（P6 去 Qt 的既定依赖前置落地）。修复：serialize_triangles 语义误读（三角形数≠索引数，网格曾少 2/3）、边折线宿主变换、布尔分支悬垂指针。
- 2026-10-18 P3 完成：web/ 查看器（R3F + zustand，四显示模式/标准视图/点选高亮/组件树/属性/版本时间线），后端挂载 /web；真栈浏览器实测通过（GLB 全链路、版本增量切换）。
- 2026-10-18 P4 完成：schema 驱动插入表单、gizmo（增量命令模式）、布尔/删除/撤销工具栏、WS presence/edit_intent 双向、跨标签零刷新同步、409 冲突 UX。修复 GlbWriter 编号缺陷（view/accessor 计数器混用→primitive 越界引用，浏览器端 GLTFLoader 实炸暴露）：分离编号；mesh_tests 增 JSON 交叉引用校验；新增 validate-assets.mjs 全量资产校验器。
- 2026-10-18 P5 大部分完成：测量/剖切/导入导出（含 STL）/Playwright e2e 门禁（真栈 5s 绿）；修复初始相机不对中与工具栏 CSS 吞点击两缺陷。遗留：STEP（Interop 已授权未接入）、大规模压测。
- 2026-10-19 计划调整（ADR-3，实体级图存储保留）：**实体级图存储以投影层形态保留并提级**——新增 P6 实体级图投影（serializer 去 Qt 化搬入 worker，网关写 Neo4j `HAS_ENTITY` 投影，复用 Phase B"细"档物理表示）；原 P6 拆除顺延为 **P7** 且范围收缩（serializer 先搬家不删、deserializer 随 v2 删、`neo4j_webui` 改造为 V1 浏览）；新增 **P8** 实体级增量（接管 REFACTOR_PLAN M5 / Phase D，论文第 5 章完整恢复）。动因：实体级结构化存储是论文核心贡献（STORAGE_CONSOLIDATION_PLAN §2/§3"细"档目标）与协同细粒度（实体级 edit_intent/冲突判定/语义 diff）的数据基础，v3 组件级模型下应以投影形态延续而非丢弃。
- 2026-10-19 P6 完成：Qt-free codec（`worker/entity_graph_codec`，uvsafe-topology-v5-bounded 逐 case 保真，sat_fragment 内嵌裁剪为 fallback 标记）；emit/import 管线 graph.json；组件库 graph 资产 kind；v1_store Memory/Neo4j 投影双实现（细档复用 json_graph_native，`HAS_ENTITY`+`TOPOLOGY{rel_type}`，幂等零写入，root 重编为组件哈希）；commands/files 触发投影写入；`GET /projects/{id}/entities`；scene 带 graph 哈希。门禁：后端 96 项全绿（含真 ACIS 集成验收与 Neo4j L3 细档查询）、worker selftest、mesh_tests 175、Qt 构建。经验证：ATTRIB 链在 ACIS 实体上是常态（v2 probe 基线同为 sat_fragment），fallback=sat_asset 是保真判定而非异常。
