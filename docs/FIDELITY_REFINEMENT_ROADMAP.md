# 100% 储存保真：里程碑与细化路线图

> 分支 `storage/entity-unit-tests`。
> **v3 处置（2026-10-19，P9）**：本文 KNOWN-DIFF 登记（pcurve exppc 参数域
> 控制点 1 参数单位级偏差 ×3 文件）属 **v2 往返器**（serializer→deserializer）
> 的已知问题。v3（P7 起）反序列化器随 Qt 客户端退役，几何恢复统一走 SAT
> 资产（ACIS 权威格式，零偏差）；投影（entity_graph_codec）为单向派生，
> 不承担重建职责——**登记在 v3 语境退场（无消费方）**，往返验收由
> SAT 覆盖度（fallback 标记）+ L3 投影等价断言取代。abc 200 模型压测
> （docs/BENCHMARK_v3.md）零存储错误。压测另暴露并修复：STEP 产物中
> 无面体/不可离散面曾致整模型导入失败——现降级为 mesh_error 标记
> （SAT/graph 照常），见 worker main.cpp importDirectory。
> 策略（既定）：**能细粒度就细粒度，实在不行回退到粗粒度，以 100% 一致为目标；
> 达成后把粗粒度部分逐步细粒度化，触发集单调收缩，每步都有测试守门。**

## M0 里程碑：任意复杂工图 100% 可储存（2026-10-15，commit 4def18c）

机制：组件级 SAT 片段回退（实现细节见 `ENTITY_STORAGE_TEST_PLAN.md` §3.3）。

- 序列化端按 body 连通闭包做风险检测，命中即把组件完整实体表存为 SAT 文本
  挂 body 节点 `props.sat_fragment`（内核权威格式，含 ATTRIB/过程曲面定义）。
- 恢复端带片段的组件整体 `api_restore_entity_list` 读回，跳过图重建；
  片段损坏回退普通图重建。
- 实体节点照常发射，查询/合并粒度不变；协议向后兼容（旧客户端忽略新字段）。

达成证据（同日全量重跑）：

| 判据 | 结果 |
|---|---|
| 复杂工图探针（布尔+圆角+抽壳 / 螺旋扫掠 / 扭转放样） | `PROBE ALL CHECKS PASSED`（A/C 纯细粒度四重精确；B 片段回退 mass_pr 修复） |
| L1 C++ 往返套件 | 14 组 132 检查全过（G1–G14） |
| L2/L3 后端 + 真 Neo4j 集成 | 62 passed, 1 skipped（skip 与存储无关） |
| SAT 粗粒度基线 | 几何字节级一致（仅内核 history 指针字段规范化） |

顺带修复（细粒度级别，非回退）：straight 恢复改无界（SAT `I I` 惯例），
区间由顶点推导——修复抽壳连接边 4.3e-3 长度偏差（根因：`get_param_range()`
归一化丢方向，真实区间 [-0.8,0] 被序列化成 [0,0.8]）。

## 细化路线：逐类清空触发集

当前触发集（`entity_graph_serializer.cpp` 风险检测）与对应细化项：

| # | 触发条件 | 根因 | 细化方案 | 验收 | 状态 |
|---|---|---|---|---|---|
| R1 | `nobs3`：intcurve 无 bs3 载荷；恢复体 `null_surface` 支撑面 | 交线定义（surf1/surf2 两张曲面 + 参数域）未入载荷 | 定义载荷：嵌入 surf1/surf2 曲面载荷 + safe_range，恢复端带定义构造 | 两圆柱相交等真交线模型：恢复体 intcurve 定义面非空、SAT 无 `null_surface`、边长/体积精确 | **已完成（G15）** |
| R2 | `spl_subtype ∉ {37,60,63,44}`（67 sweepsur / 其余过程子类） | 过程曲面 NURBS 仅拟合 | **44 rot_spl_sur 已真重建（G16，采样偏差 0）**；67 sweepsur 实测 NURBS 偏差 3.1e-4 且 rail/draft/scale 三律非空、law 家族无公共构造 API → **片段回退为 67 的终态**（G14 负结果断言守门，防止无真重建而白名单化） | G16：44 往返采样零偏差、子类保持、无片段；G14：67 片段路径四重精确 | **已完成（分两半：44 细化 + 67 终态存证）** |
| R3 | ATTRIB 属性链存在 | 拓扑 BFS 结构性看不到属性实体 | **普查结论（G17）：现有建模链路中属性触发与 67 触发完全重叠**（布尔/圆角/抽壳/拉伸/放样零属性；唯一带属性的曲线路径扫掠必然同时带 sweepsur）→ 触发保留为 fail-safe（检测已加固到几何方程实体）；属性载荷推迟到出现真实独立人群或内核提供属性重建 API（内部类如 make_tangent 无公开构造器；公开类如 ATTRIB_RGB 待 DCAD 实际使用再做） | G17：拉伸体零属性、无片段、四重精确（防误报守门） | **已完成（普查定策：无独立人群，检测加固）** |
| R4 | `unknown_surface` / `unknown_curve` | 新实体类型 | 按需扩展类型分支 | 类型不触发回退 | 待 |

### R1 实现记录（2026-10-15）

- 序列化端（`entity_graph_serializer.cpp`）：intcurve 载荷新增 `def_surf1`/`def_surf2`（方程级曲面载荷，schema 与 face.geometry 一致）+ `safe_range`。定义源两级：① `pcur(index, TRUE)` 物化的参数曲线宿主面（parcur 型，如扫掠接缝 → sweepsur-spline）；② `int_int_cur`（surf-surf 型）无公共曲面访问器，用**边的宿主面曲面**（几何等价：交线落在两张宿主面上，经 coedge→loop→face 指针链）。
- 恢复端（`entity_graph_deserializer.cpp`）：`parseSurfaceEquation`（plane/sphere/torus/cone/exact_spl_sur→spline 包装）+ intcurve 构造器带定义曲面（构造器复制曲面，临时方程对象构造后删除）。
- G15 实证（两平行偏置圆柱相交 → 四次交线）：定义恢复后**体积/边长/包围盒全部精确**（修复前体积 rel 2.7e-5）；恢复体 SAT 与原件逐记录一致（surfintcur + 双 cone 定义 + nullbs——参数 2D 曲线原件本就为空，属忠实复刻）。
- 附带收益：`splSurToJson` 共享助手消除了 face.geometry 与定义曲面的 NURBS 提取重复。

### R2 实现记录（2026-10-15）

**实验定策（G14 [R2] 块，负结果存证）**：剥离片段后纯图路径重建螺旋扫掠管——
- 曲面采样（10×10 参数网格）最大偏差 **3.089e-04**（管半径 0.4，即 0.08% 量级）：sweepsur 的 bs3 只是粗逼近，**NURBS 不是权威表示**；
- 原件 `get_rail()/get_draft()/get_scale()` **三律全部非空**（默认扫掠也带律对象），而 law 家族（SIMPLE_RAIL/DCUR 等）无公共构造/解析入口；
- 结论：**片段回退是 67（sweepsur）的终态**，G14 固化区间断言（dev ∈ (1e-6, 1e-3)）防止未来无真重建而白名单化。

**rot_spl_sur（44）真重建（G16）**：与 sweepsur 不同，`rot_spl_sur` 无 law 且数据成员公开（`axis_root`/`axis_dir`/`cur` 母线指针），公开构造器 `(curve, axis_pt, axis_dir, uparam, vparam)` 完全可序列化：
- 序列化：spline 载荷新增 `rot_profile`（母线方程载荷：straight/ellipse/intcurve-bs3，经 `curveEquationToJson`）+ `rot_axis_root/rot_axis_dir` + `rot_uv`；
- 恢复：`parseSurface` spline 分支 rot 字段优先（母线经 parseCurve 实体→`equation()` 方程），失败落回 NURBS 路径；
- G16 实证（straight/ellipse 双母线合成面）：恢复子类保持 44（非 exactsur 退化）、**采样偏差 0.000e+00**、轴往返精确、无片段触发；
- 触发集白名单 {37,60,63} → **{37,60,63,44}**（净收缩）。

### R3 实现记录（2026-10-15，普查定策）

**人群普查（G17）**：矩形线框沿直线路径拉伸（最常见 CAD 操作）——**零属性**、六面全 plane、无片段触发、四重精确。汇总现有证据：

| 操作 | 留属性？ | 过程曲面？ | 属性触发独立人群？ |
|---|---|---|---|
| 布尔/圆角/抽壳/放样/拉伸 | 否（探针 A/G10/G17 实测） | 视面型 | 无 |
| 曲线路径扫掠（管/簧） | 是（make_tangent，挂边） | 是（sweepsur 67） | 否（两触发同时命中，片段已全覆盖属性） |

**结论**：属性载荷在现有建模链路中不会缩小任何组件的粗粒度集合（零增量），且内部属性类（make_tangent 家族）无公开构造器、无法纯图重建。**触发保留为 fail-safe，检测加固到几何方程实体**（SURFACE/CURVE/PCURVE/APOINT 上的属性链——它们不是图节点，拓扑遍历结构性不可见）。属性载荷推迟到：(a) 出现带属性而无其他触发的真实人群，或 (b) DCAD 实际使用公开属性类（如 ATTRIB_RGB 颜色）时再做。




配套工程：

- **L1.5 语料库回归（2026-10-15 落地）**：`scripts/tests/acis/sat_corpus_regression.cpp`
  （`build_sat_corpus.bat`）——目录驱动：任意 `.sat` → 内核读入 → JSON 全链路往返 →
  ①逐 body 四重测量（体积/边长/拓扑/包围盒，强制）②SAT 归一化多重集 diff
  （$ 引用剥离 / 负零折叠 / 15 位有效数字折叠 / loop 分类与 spline 子类标记等
  内核记账字段折叠）。当前语料 15 文件：**12 全一致 + 3 已登记 KNOWN-DIFF**
  （pcurve exppc 参数域控制点 1 参数单位级偏差，3D 四重全过；退场条件 =
  pcurve 参数域精化落地后从登记表移除）。运行：`sat_corpus_regression.exe .`。
  探针（`probe_complex.cpp`）是其三部件实例，仍独立保留。
- **G 编号守门**：每类细化配一个正式组（R1→G15，R2→G16/G14，R3→G17），
  回退机制自身由 G14 锁定（正例触发 + 负例不触发）。

### L1.5 过程中的真实修复（负结果同样入库）

1. **straight 曲线区间语义**（真根因，一处修正连带 4 文件转绿）：序列化端曾用
   EDGE `get_param_range()`——归一化视图丢方向（真实 [-0.8,0] → [0,0.8]），
   恢复端 limit 后端顶点投影出界 → 抽壳连接边长度退化 0；此前无界恢复修复
   又丢圆角体直线的 F 区间。终态：区间 = **曲线方程自身 `param_range()`**
   （limit 状态权威视图，与 SAT 落盘一一对应；方块棱 I I / 圆角直线 F -7..7
   两种形态都真实存在），边区间由恢复端顶点推导。
2. **loop classification**（loop_type）：序列化/恢复带分类字段、内存往返保真；
   但内核建模管线写 periphery 时的落盘形态（带子循环引用尾巴）与公开
   `set_classification` 不同源 → 归一化折叠为记账字段（不比对）。
3. **rulesur(63) 定案**：类型名仅存在于 DLL 内部（无公开头文件/构造器），
   NURBS 表示几何精确（四重全过）但 SAT 记录形态不同 → 白名单保留，
   归一化折叠子类标记行，控制点行参与比对。

## 约束与边界（沿袭既定原则）

- 协议只加不改：新载荷字段向后兼容，旧客户端忽略；不动 HTTP/WS、Mode0/Mode1
  隔离、base_version 语义。
- 每轮细化的合并条件：探针全绿 + L1 全绿 + 后端全绿 + 触发集净减少。
- 100% 的最终判据永远是 SAT 等价（内核权威），不是"测量值差不多"。
