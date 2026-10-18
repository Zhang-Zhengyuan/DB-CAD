# UI 重构计划：Qt → Web 前端（UI-v2）

> **[已被取代]** 本计划已升级合并为 [V3_PLAN.md](V3_PLAN.md)（全新 API + 工业级 Web 客户端总计划，旧 API 全废弃、服务端权威架构）。本文保留作过程记录，不再更新。

> 开始日期：2026-10-18（计划拟定）。配套文档：[ARCHITECTURE.md](ARCHITECTURE.md)（§1 早已预留"UI 层：Qt 或 Three.js，可整体替换"）、[REFACTOR_PLAN.md](REFACTOR_PLAN.md)（架构 v2，已合并）。
> 目标一句话：**浏览器渲染 + 粗粒度操作替代 Qt 界面，最终把 Qt 依赖整个丢掉。** 工业级精细操作明确不做——打开工程图、漂亮地渲染出来、能大致操作，就够。

## 0. 现状盘点（为什么这次比想象中快）

架构 v2 恰好把最难的三件事提前做完了：

| 现有资产 | 位置 | 对本计划的意义 |
|---|---|---|
| **网格提取通路存在，但属旧实现，须重构** | `kernel/geometry_kernel.h:25` `mesh(ShapeHandle, GmeMesh::DisplayData&)`；`gme_mesh.cpp` | 证明"ACIS 离散化在本项目可走通"（`api_facet_entity` + `api_get_facet_edge_points` + `api_rh_get_entity_rgb`），但该实现年代早、做法简陋：**调用 `api_facet_entity(ent)` 时不传任何质量参数（默认档）**、手工遍历 FACE 拼三角形、法线靠手工回退、还带着针对"图恢复面缺 facet"的防御性补丁。SDK 实际提供 `facet_options_visualization`（渲染质量档位）/`facet_options_precise`（sag/法向容差/最大边长）、`api_get_sequential_mesh`（结构化索引网格）、`api_get_facet_face_tri_points`、批量与缓存管理 API。**结论：不沿用，按 §2 第一步重写** |
| **UI 层契约已隔离** | ARCHITECTURE.md §1-2：UI "只消费纯渲染数据（坐标/索引/颜色）和发操作命令" | 换 UI 不动核心，这是 v2 的设计承诺，本次兑现 |
| **CommandBus 骨架** | `core/command_bus.*`（M1.6：名→schema→执行器，菜单即注册表生成） | 浏览器发命令 = 又一个平级操作者（与 M6 AI 同构），不需要发明新协议 |
| **组件不可变 + 版本清单** | Neo4j 侧（M3）：组件内容寻址、版本=组件清单链 | **网格缓存天然成立**：同内容组件永远复用同一份网格，版本切换=换清单，只补拉新增组件 |
| **全异步 core** | M2：无嵌套事件循环；M2.2 `InteractionSurface` 可注入 | 无头进程直接复用 CollabController/DocumentSession，零改造 |
| **后端已有静态托管** | `app/main.py` 挂 `/webui-static`（现行 Neo4j WebUI） | 前端同源部署，无 CORS，模式照抄 |

## 1. 目标架构（定下来不再讨论）

```text
浏览器（React + three.js）⇄ FastAPI（唯一入口）⇄ Neo4j（SAT 片段随组件入库，服务端不解析）
                                ⇅ 内部 HTTP
                    headless 几何 worker（C++ 控制台进程）
                    = core/ + kernel/ 去掉 Qt 壳的那个"核心进程"
                    · 订阅/拉取项目（复用 CollabController，json-delta 协议不动）
                    · kernel().mesh() → 每组件一份网格
                    · 组件内容哈希 → GLB 网格缓存（不可变，一次计算永久复用）
```

要点：

1. **ACIS 只能住在 C++ 进程里**（Python 服务端不可能解析 SAT，浏览器更不可能），所以必须有一个 headless worker——它就是 ARCHITECTURE.md §1 里画的"核心进程"，只是 UI 客户端从 Qt 换成了浏览器。旧"bridge"概念的正当重生，但这次站在干净的 core/ 上。
2. **worker 不是渲染器。** 渲染（画三角形到屏幕）发生在浏览器的 GPU 上；worker 只干三件事：持有 ACIS 内核、把 B-rep 几何**离散成三角网格**（计算，不是画）、执行建模命令（浏览器拖 gizmo → 命令 → worker 在真实几何上执行 → 返回新网格）。它是"几何服务"，与画面无关，所以才叫 headless（无窗口）。
3. **FastAPI 是浏览器唯一入口**（鉴权、端口、WS 都在一处）；worker 只对 FastAPI 内网可见。json-delta 线协议冻结不动，网格通道全部是**新增**端点。
4. **本地 SAT 文件模式**：浏览器上传/选择 SAT → worker 离散化 → 渲染，不经过项目流程。

## 2. 三步走（对应用户三问，逐步验收）

### 第一步：面片网格怎么获得 —— 按未完成对待，先调研后重写

现有 `gme_mesh` 通路只当"可行性证明"，不当资产。SDK 头文件（`_deps/acis/include/`）里可供调研的候选路径：

| 候选 | 接口 | 特点 |
|---|---|---|
| A. 现状 | `api_facet_entity(ent)`（NULL 选项）+ 手工 FACE 遍历 | 默认质量、手工拼装、法线回退——作为基线 |
| B. 可视化档位 | `facet_options_visualization`（face_quality/edge_quality 档位） | ACIS 为**渲染工作流**专门设计的档位，正对本计划场景 |
| C. 精确容差 | `facet_options_precise`（surface_tolerance/sag、normal_tolerance、max_edge_length、grid_aspect_ratio、min_grid_lines） | 分析级控制，可量化误差 |
| D. 结构化读取 | `api_get_sequential_mesh`（SEQUENTIAL_MESH 索引网格）/ `api_get_facet_face_tri_points` / `api_create_global_mesh` | 免手工遍历，直接拿带法线的索引网格；另有 `api_facet_entities` 批量与 `api_delete_entity_facets` 缓存管理 |

调研动作（W0）：abc_cache 抽样模型（简单/中等/复杂各若干），四路对比**三角数、sag 误差、法线质量、耗时、边线与面网格一致性**，输出选型结论与推荐质量档位。然后重写为独立的 `MeshExtractor`：质量档位可调（低/中/高/精确容差）、法线一律来自内核、批量离散、边折线输出、旧防御性补丁按新路径重新验证后清除，配单元测试（往返一致性、法线连续性、未识别几何兜底）。

**产出序列化**：`DisplayData → GLB`（glTF 2.0 二进制）。glTF 是行业标准，three.js 原生 `GLTFLoader`，任何查看器可调试；三角形 primitive（position+normal+index，组件色材质）+ 线段 primitive（B-rep 边）。分组/高亮元数据走**旁挂 manifest JSON**（组件 uuid → 面分组表），不硬塞进 glTF 语义。压缩：后期加 `EXT_meshopt_compression`（GPU 端解码、无损，2026 年主流管线已优于 Draco），MVP 先裸 GLB。

### 第二步：渲染怎么做 —— 常规 three.js 能力，无科研含量

- **CAD 外观三件套**：着色面（MeshStandardMaterial + HDR Environment）+ 深色边线（LineSegments + polygonOffset 防深度冲突——边折线数据内核直接给，无需算轮廓线）+ 地面网格/坐标轴。drei 的 `Grid`/`Environment`/`OrbitControls`/`TransformControls` 全现成。
- **相机**：OrbitControls + 按包围盒自动 fit（`Bounds3` 网格数据里就有）。
- **拾取/高亮**：Raycaster（大模型加 three-mesh-bvh）；命中面 → manifest 反查组件 → 整组件 emissive 高亮（与现有"组件级"协作粒度一致）。
- **性能**：每组件一个 GLB、按需懒加载 + 不可变缓存；abc_cache 里 10000 个 STEP 模型就是现成的大模型压测语料（W4 用）。

### 第三步：前端技术栈 —— 调研结论（2026-10）

| 层 | 选型 | 理由 |
|---|---|---|
| 渲染 | **three.js**（新 `WebGPURenderer`，运行时自动回退 WebGL2） | 2026 年 WebGPU 已为默认且生产可用；只用标准材质/几何体即可保住回退兼容。CAD Web 查看器（CADProps、@inweb/viewer-three 等）已验证此路 |
| 框架 | **React + React Three Fiber + drei** | 命令面板/组件树/版本历史/在场感知都是面板 UI，R3F 是当前最成熟的 3D-React 绑定 |
| 状态 | zustand | R3F 生态标配 |
| 工程 | TypeScript + Vite | 标配；构建产物丢进 FastAPI static（同源） |
| 数据 | GLB（网格）+ JSON manifest（分组/树）+ 既有 WS 通知 | 见第一步 |
| 测试 | vitest + Playwright（对 FastAPI+worker 做端到端） | 延续本仓库"每步门禁"文化 |

调研来源：[three.js WebGPURenderer 官方文档](https://threejs.org/docs/WebGPURenderer.html)、[three.js 2026 变更综述](https://www.utsubo.com/blog/threejs-2026-what-changed)、[WebGPU vs WebGL 实测](https://www.reddit.com/r/threejs/comments/1vbj3ca/we_tested_webgpu_vs_webgl_in_a_real_threejs_scene)、[Draco vs Meshopt 对比（2026-09）](https://compress-glb.com/blog/draco-vs-meshopt)、[R3F 模型加载](https://r3f.docs.pmnd.rs/tutorials/loading-models)、[glTF/meshopt 互操作指南](https://www.cadinterop.com/en/formats/mesh/gltf.html)。

## 3. 里程碑与时间（单人 + AI 辅助的日历估算；每步门禁绿才进下一步）

| 里程碑 | 内容 | 验收 | 估时 |
|---|---|---|---|
| **W0 面片调研与选型** | §2 第一步的四路径对比（现状默认/visualization 档位/precise 容差/sequential_mesh），abc_cache 抽样基准 | 选型结论 + 基准数据（三角数/sag 误差/法线/耗时对比表） | **2-3 天** |
| **W1 面片引擎重写 + 端到端尖刺** | 新 `MeshExtractor`（质量档位、内核法线、批量、边线）；GLB+manifest 序列化；一次性 HTML 渲染 | 浏览器看到带边线着色模型；质量档位切换肉眼可见差异；旧 `gme_mesh` 通路退役 | 2-3 天 |
| **W2 headless worker + 网格端点** | 控制台 main 复用 core（文件模式 + FastAPI 项目模式，CollabController 原样）；内部 HTTP；`GET /projects/{id}/scene`、`GET /components/{hash}/mesh`（内容寻址缓存）；`POST /files/sat` | 浏览器渲染云端项目与本地 SAT；重复请求命中缓存 | 3-4 天 |
| **W3 查看器 MVP** | R3F 工程脚手架；场景加载/fit/边线；组件树、点选高亮、版本列表 | 双击版本即切换渲染（只补拉增量组件） | 4-5 天 |
| **W4 粗操作 + 协同** | 命令通道：WS → worker CommandBus（insert/draft/translate/erase + undo/redo）；TransformControls 拖拽平移；推/拉按钮 | 两个浏览器标签页互推互拉、409 冲突提示——等价于现有双客户端回归场景 | 4-5 天 |
| **W5 加固与门禁** | abc_cache 抽样压测（10k 面片级模型）：meshopt 量化、懒加载调优；Playwright e2e；文档 | 大模型首屏可交互 < 数秒级；e2e 绿 | 3-4 天 |
| **W6 去 Qt（独立里程碑）** | core/kernel 去 QtCore：QJson→nlohmann/json、QString→std::string、QByteArray→std::vector<std::uint8_t>、QSignal→回调（core 已是回调风格）；删 glwidget/window/mainwindow 与 Qt 工程/部署件；build.ps1 瘦身 | 构建不再需要 Qt；全部测试门禁绿；打 tag `qt-free` | 1-2 周 |

**到 W5（约 4-5 周）Qt 已无用户；到 W6（再 1-2 周）Qt 从依赖里消失。** W0-W3 期间 Qt 客户端保持可用不动（web 前端在独立目录，新旧并行），W4 起以 web 为主战场，Qt 冻结只修致命 bug。面片引擎（W0-W1）先于一切前端工作——它是整条链路的地基，且与 UI 无关，即使后续计划有变也是净收益。

## 4. 冻结项与红线（延续 v2 规矩）

1. json-delta 线协议与 WS 通知协议**不动**——网格/命令通道全部是新增端点；
2. `GeometryKernel` 接口**不动**——`mesh()` 本就是成员，序列化器放内核外（消费 DisplayData 中立数据）；
3. 后端既有 44 项回归 + core 116 项单测 + 客户端回归**每步全绿**（Qt 删除前的过渡期两者都跑）；
4. 协议乐观锁语义不变：浏览器操作同样走 base_version，冲突诚实拒绝。

## 5. 风险与对策

| 风险 | 对策 |
|---|---|
| ACIS 授权在服务态/headless 的合规性 | W1 前核对 spatial 授权条款（同一 SDK 从 GUI 进程挪到控制台进程，通常无碍，但须书面确认） |
| 大模型网格体积/首屏 | 每组件独立 GLB + 内容寻址缓存 + meshopt；abc_cache 抽样实测前置到 W0 顺手测一个 |
| R3F/WebGPU 生态仍在演进 | 锁版本；坚持标准材质/几何体，随时可退 WebGLRenderer |
| glTF 表达不了 CAD 分组语义 | 旁挂 manifest JSON，不与格式缠斗 |
| worker 进程管理 | 单机先手工/脚本拉起（复用 deploy 脚本模式）；不引入 k8s 类设施 |

## 6. 明确不做（与"够用就行"对齐）

工程级精细标注/草图约束、离线模式、移动端触控、剖切/测量（后续可加）、多人同屏光标同步。旧 Qt 客户端的高级交互不追求逐一复刻。

## 7. 变更记录

- 2026-10-18 计划拟定（本文件）。现状盘点、技术调研、里程碑估算如上。
- 2026-10-18 修订（评审意见）：① 澄清 headless worker 角色——它是持有 ACIS 的"几何服务"（离散化+建模命令执行），渲染在浏览器 GPU 上发生，worker 不画任何东西；② 面片获取从"已有答案"改为"按未完成对待"——旧 `gme_mesh` 仅作可行性基线，新增 W0 四路径调研（含 SDK 的 `facet_options_visualization`/`facet_options_precise`/`api_get_sequential_mesh`）与 W1 重写里程碑，总时长 3.5 周 → 4-5 周（到 Qt 无用户）。
