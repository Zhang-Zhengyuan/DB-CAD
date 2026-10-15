# 实体级存储单元测试计划（第二部分：数据库储存）

> 分支 `storage/entity-unit-tests`，工作树 `D:\DBCAD\DB-CAD-storage`。
> 本期只解决**储存**问题；UI 前端渲染缺口放到下一阶段。

## 1. 调研结论

### 1.1 存储链路全景

Mode1（默认路径，本次重点）端到端链路：

```
ACIS ENTITY (C++)
  → serializeACISEntityGraph()          CAD_DB/entity_graph_serializer.cpp
  → JSON {nodes, rels}
  → POST /projects/{id}/json-delta       CAD_DB/backend/app/api/json_delta.py
  → validate_graph + merge_graph         CAD_DB/backend/app/json_collab.py
  → JsonGraphStore.create()              CAD_DB/backend/app/json_graph_store.py
  → Neo4j: JsonGraphVersion / JsonGraphComponent / JsonGraphEntity / JsonGraphRelation
  → GET json-delta → JsonGraphStore.graph() 重组
  → deserializeACISEntityGraph()         CAD_DB/entity_graph_deserializer.cpp
```

并行存在的存储栈（均为现有功能，不可视为死代码）：

| 栈 | 文件 | 粒度 | 用途 |
|---|---|---|---|
| A. Neo4jStore（Mode0） | `backend/app/neo4j_store.py` | 版本级 JSON blob | SAT 全量版本 |
| B. JsonGraphStore（Mode1） | `backend/app/json_graph_store.py` | **实体级节点**，内容寻址去重 | JSON delta（默认） |
| C. Neo4jEntityStore（legacy） | `backend/app/neo4j_entity_store.py` | 全局 local_id 实体节点 | 直接实体图接口 |
| D. C++ Bolt 直写栈 | `CAD_DB/storage/access_graph_save/restore.cpp` | 实体级节点（apoc） | Bridge / 直连 Neo4j |

### 1.2 实体覆盖矩阵（终态，2026-10-15 全绿）

对照存储模型设计规范（参考文献·张家荣论文第 3 章：10 类拓扑实体 + 5 类几何实体
+ 3 类样条数据节点 + part/generation 版次链）+ 本期全部验收测试映射：

| 实体/组件 | 论文要求 | 实现 | 验收测试 |
|---|---|---|---|
| body/lump/shell/wire/face/loop/coedge/edge/vertex | 10 类拓扑 | ✅ 独立节点 + 40 类边 + 6 属性 | L1 G1–G17 四重比对；L2 矩阵；L3 SHA；E2E |
| subshell | 三向树 | ✅ 5 边 + 面直挂 | G11 合成接线；L2 subshell_matrix |
| transform | — | ✅ 分量逐字往返 | G9 |
| APOINT | — | ✅ vertex 内嵌 position | 各组 bbox/topo |
| plane/sphere/torus/cone | 5 类曲面(4 解析) | ✅ 定长属性内嵌 | G1–G5/G10/G15；L2/L3 逐面 SHA |
| spline 面 + spl_sur | NURBS 数据节点 | ✅ 15 字段 + 子类码 | G13/G16；L1.5 控制点逐行比对 |
| ├ exactsur(37/60) | — | ✅ NURBS 精确 | G8b/G13 |
| ├ rulesur(63) | — | ✅ NURBS 精确（无公开构造器，DLL 内部） | G13 四重精确 + L1.5 折叠 |
| ├ rot_spl_sur(44) | — | ✅ **过程真重建**（母线+轴） | G16 采样零偏差 |
| └ sweepsur(67) 等 | — | ⚠️ NURBS 仅拟合 → 组件 SAT 片段兜底 | G14 负结果存证（3.1e-4） |
| straight/ellipse/helix | 曲线 | ✅ 定长/8 字段载荷 | G1/G3/G8/G12/G17；L1.5 区间语义 |
| intcurve + int_cur | 交线数据节点 | ✅ bs3 NURBS + **定义曲面 surf1/surf2** + safe_range | G8b/G15（SAT 逐记录一致） |
| pcurve + par_cur | 参数曲线 | ✅ bs2 载荷内嵌 coedge | G13（质量积分依赖）；L2 |
| edge convexity/区间 | — | ✅ | G10；L1.5 straight 区间根因修复 |
| loop classification | — | ✅ 往返保真 | L1.5 |
| ATTRIB 属性链 | — | ⚠️ 检测（含几何实体盲区）→ 片段兜底 | G17 普查（无独立人群） |
| 组件级 SAT 片段回退 | —（超越论文） | ✅ fail-safe | G14 正负例；E2E 字段无损 |
| 版次链（generation） | part/generation | ✅ JsonGraphVersion/PREVIOUS_JSON_VERSION | L3 版本链断言 |

**全链路闭环（超越论文）**：C++ 序列化（L1/L1.5 四重+SAT 判据）→ 后端协议
（L2）→ 真 Neo4j（L3 SHA）→ 读回 C++ 重建（L1）；E2E 测试用真实 C++ 输出
夹具（452KB 三部件复杂工图）直连 Neo4j 往返，投影 SHA 逐字节一致。
Bolt 栈（栈 D）按规范覆盖完整，但存在实测 bug：

1. `rot_spl_sur` 缺 `break` 穿透到 exactcur 默认分支 → 数据静默损坏（`access_save_surfaces.cpp:9-19`）
2. 未知曲线/曲面 → 写入**无 label 无属性**节点，参数丢失且无报错（`access_graph_save.cpp:282-284, 378-380`）
3. 恢复时未知类型 `Neo4jNode_str2enum.at()` 抛 `std::out_of_range`，**中止整个恢复**（`access_graph_restore.cpp:157, 1063`）
4. 后端 legacy 栈按**全局** local_id MERGE，跨项目同名实体会互相覆盖（`neo4j_entity_store.py:260-267`）

### 1.3 测试基建现状

- C++：`scripts/tests/acis/entity_tests.cpp` 自研 main 框架（9 组 36 检查），**只测 SAT 往返**，不触及 Neo4j/JSON 图；`build_entity_tests.bat` 依赖本机 ACIS SDK（`_deps/acis`）+ `spatial_license.h`（不入库）。
- 后端：pytest 全部用内存 fake，**无任何真实 Neo4j 测试**；`test_json_delta_protocol.py` 用手写假 Store。conftest 只设置空 API 密码。
- Bridge：`test-acis-bridge-roundtrip.ps1` 端到端仅 2 个夹具（block/subtract）。
- CI（`.github/workflows/backend.yml`）：仅后端单元测试（无 ACIS SDK，C++ 测试只能本地跑）。
- 本机：Qt 6.9.0 / MSVC 2022 / ACIS 2026.1.0.1 就位；序列化器/反序列化器无项目内部依赖，可独立编译；**Neo4j 与 Docker 当前未运行**。

## 2. 目标与交付物

按既定思路：**先每个实体逐一单元测试 → 暴露缺口 → 实现缺口让测试通过**；复杂工程图整体回归放下一期。

1. 三层测试基建（L1 C++ / L2 后端无 DB / L3 真 Neo4j 集成）
2. 实体覆盖实测矩阵（当前哪些过、哪些红）
3. 缺口实现：spline/helix/intcurve/subshell/pcurve 的序列化与反序列化
4. Bolt 栈 bug 修复与回归

## 3. 分层测试设计

### L1：C++ JSON 图往返（无 DB，核心层）

新增 `scripts/tests/acis/graph_roundtrip_tests.cpp`（框架仿 `entity_tests.cpp`：自研 check/main、argv 控制运行到第几组、退出码=失败数），新增 `build_graph_tests.bat`（在原 bat 基础上加 QtCore include/lib）。

每组流程：建模 → `serializeACISEntityGraph`（注入测试 uuid）→ **JSON 结构断言**（节点数、label 集合、关键 props 完整性）→ `deserializeACISEntityGraph` → **几何往返断言**（体积相对误差、包围盒、拓扑计数与原件相等）。

| 组 | 模型 | 覆盖实体/几何 | 最终状态 |
|---|---|---|---|
| G1-G7 | 方块/球/圆柱/圆锥/圆环/六棱柱/布尔 | 全拓扑 + 解析面/曲线 + 混合几何 | **全 PASS**（体积/边长/拓扑/包围盒四重断言） |
| G8/G8b | 螺旋线 + NURBS 边 | **helix / intcurve（bs3）** | **PASS**（含边长精确比对） |
| G9 | 变换体 | transform | **PASS**（分量逐字往返；教训：任何"补偿"都会破坏作用） |
| G10 | 圆角 | blend：cone/sphere 面 + tangent 边 + **凸性** | **PASS**（关键修复：边区间取曲线自身参数空间 + 周期整圆不设限 + convexity 序列化） |
| G11 | subshell | **subshell（三向树 + face/wire 直属）** | **PASS（合成接线回归）**：常规建模不触发 ACIS 分层，测试手工构造 subshell 树接入拓扑，验证节点/关系/恢复树链接/face 双向链接全往返；全级 api_get_entities 在 face↔subshell 互指结构上不终止（内核行为，测试用受控遍历规避） |
| G12 | 直线线框 | wire + straight（**param_scale 入方向模长**） | **PASS** |
| G13 | 扭转六边形放样 | **spline 面（spl_sur NURBS）+ pcurve** | **全 PASS（含体积 505.070416 精确一致）**。原 KNOWN-LIMIT 经隔离实验证伪：rulesur 子类不背锅，真因是 pcurve 装配把 SURFACE 实体当方程对象解引用（类型混淆 → mass_pr 求值 AV）；隔离实验（原体仅换 exactsur 面）mass_pr 通过 + 错误码 #110007 实锤 spline 面积分需要 pcurve |

### 3.1 Phase 1 发现与第一轮修复（已完成 ✅）

**已修复**（serializer/deserializer，均已由 L1 测试验证）：

0. ✅ **区间编码**：`intervalToJson` 恒定 4 元素；`parseInterval` 无限区间跳过 limit；边的 `range` 改为序列化 EDGE 自身参数区间（`edge->param_range()`）。修复 G1/G6/G7（含负体积）。
1. ✅ **cone 双缺陷**：`base_major_axis` 字段名不匹配（发 `base_major_axis` 读 `major_axis`）+ `u_param_scale` 未传（构造器默认 0 导致锥面损坏）+ uv_range 双格式混用。修复 G4。
2. ✅ `parseVector/parsePosition` 空数组默认值未置零（SAT 出现 1e-310 垃圾值）。
3. ✅ 后端 `topology_types` 补齐 subshell/pcurve 边。

**待修**（按优先级）：

1. ~~intcurve 载荷~~（缺口 2 已实现，G8b 验证）；剩余：G10 圆角弧边参数区间语义与 G9 变换按作用值序列化。
2. **transform 叠加语义**（G9）：几何与 transform 节点双重应用。
3. **helix 载荷**（缺口 3，G8）、spline/spl_sur（缺口 1）、subshell（缺口 4）、pcurve/par_cur（缺口 5）。
4. 观察项：SAT 文本含 `-0` vs `0` 的负零伪差异（数值等价，SHA 字本比对需归一化）；mass_pr 对混合面时向 stdout 打印分面积分（ACIS 行为）。

## 3.2 复杂工图探针（2026-10-15，Q1/Q2 实测取证；3.3 修复后全绿）

`scripts/tests/acis/probe_complex.cpp`（`build_probe_complex.bat` 构建）：布尔挖孔 + 12 边圆角 + 顶开抽壳（A）、圆截面沿螺旋扫掠薄壁管（B）、扭转六边形放样（C）三部件场景，全实体普查 + JSON 往返四重比对 + SAT 双向比对。**Q2（SAT 粗粒度）基线**：三部件体积全部精确一致，save→restore→save 仅 32 个实体的内核 history 指针字段规范化为 -1，几何字节相同——任意复杂图经 SAT 可完整正确储存。**Q1（JSON 细粒度）修复后探针全绿**（`PROBE ALL CHECKS PASSED`）：A/C 纯细粒度四重精确，B 片段回退后 mass_pr/包围盒精确。探针曾暴露、随后被修复的缺口：

| 缺口 | 根因（实证） | 修复 |
|---|---|---|
| B 管恢复体 mass_pr #111001 | sweepsur（`spl_sur::type()=67`）为过程曲面，NURBS 仅拟合；接缝 intcurve 空支撑面重建（SAT `nullbs`/`null_surface` 记录为证） | 组件级 SAT 片段回退（§3.3），G14 锁定 |
| A 边总长 rel 4.3e-3（体积精确） | 抽壳连接边真实参数区间 **[-0.8,0]**（端顶点在参数 -0.8 处），`get_param_range()` 归一化成 [0,0.8] 丢方向；恢复端按区间 limit 后端顶点投影不进曲线 → 长度退化 0 | straight 恢复为**无界**（SAT 惯例 `I I`），区间由顶点推导 |
| ATTRIB 属性实体丢失 | 拓扑 BFS 结构性看不到属性链（`integer_attrib-name_attrib-gen-attrib … make_tangent`，扫掠体产物） | 片段回退触发条件之一；纯图路径暂不支持属性载荷 |

另证：该内核布尔/构造结果体为"原点居中几何 + TRANSFORM"结构（原始顶点坐标与世界系差一个平移），探针用 `body_coord_offset` 校正后选边/选面。

## 3.3 组件级 SAT 片段回退（fail-safe，2026-10-15 实现）

策略（用户确认）：**能细粒度就细粒度，实在不行回退到粗粒度，以 100% 一致为目标；达成后再把粗粒度逐步细粒度化**。落地机制：

- **序列化端**（`entity_graph_serializer.cpp`）：组件（body 连通闭包，与后端 `split_graph_components` 语义一致）风险检测，任一命中即把该组件完整实体表存为 SAT 文本挂到 body 节点 `props.sat_fragment`（内核权威格式，含 ATTRIB/过程曲面定义）：
  1. face 出现 `unknown_surface` 或缺 geometry（edge 无 geometry 是合法退化结构如锥顶边，不触发）
  2. 已访问实体挂有 ATTRIB 属性链（不能静默丢）
  3. 样条面子类 ∉ {37,60 exactsur, 63 rulesur}（63 经 G13/探针实证 NURBS 精确；67 sweepsur/44 rot 为过程曲面）
  4. pcurve/intcurve 无 bs2/bs3（nobs2/nobs3 标记）
- **恢复端**（`entity_graph_deserializer.cpp`）：body 节点带 sat_fragment 的组件整体 `api_restore_entity_list` 读回（100% 一致），该组件节点跳过 pass1/2/3（避免二次 set_* 损坏）；片段恢复失败回退普通图重建（不劣于现状）。
- **协议兼容**：实体节点照常发射（查询/合并粒度不变）；后端 props 透明传输（L2 `test_sat_fragment_fallback_field` 锁定：validate/组件归属/内容寻址敏感）；旧客户端忽略新字段。
- **细化路线**：每攻掉一类触发条件（intcurve 定义载荷、sweepsur 轨道、ATTRIB 载荷）即从触发集中移除，粗粒度集合单调收缩；G14 锁定机制（正例 sweepsur 触发 + 负例干净方块不触发）。


### L2：后端契约与图逻辑（无 DB，pytest，进 CI）


新增 `backend/tests/test_json_graph_entity_matrix.py`：按 `entity_graph_serializer.h` 文档契约手工构造与 C++ 输出一致的每实体 JSON 夹具，逐实体验证：

- `validate_graph` 通过；每种 label 至少一个节点、每种 topology_types 边至少一条
- `split_graph_components`：单 body 单组件；跨组件边正确分离
- `graph_hash` 稳定性；`merge_graph` 组件替换语义
- 夹具形状随缺口实现同步扩展（spline/intcurve/helix/subshell/pcurve 节点）

### L3：集成（真 Neo4j，标记 `integration`，连不上自动跳过）

- 新增 `backend/tests/test_json_graph_neo4j_roundtrip.py`：bolt 直连（backend/.env），逐实体 `JsonGraphStore.create()` → **Neo4j 实际结构断言**（JsonGraphEntity 数量 = 节点数、labels_json/props_json 内容、JSON_TO 边、版本链）→ `graph()` 重组与原图等价；同图二次 push 验证组件内容寻址去重（幂等）。
- 扩展 `test-acis-bridge-roundtrip.ps1` 夹具（Bolt 栈）：G1/G6/G7。
- 运行方式：`CAD_DB/deploy/lib/docker.ps1` 拉起 neo4j-apoc 后 `uv run pytest -q -m integration`。

## 4. 阶段排期

- **Phase 0（已完成）**：工作树 + 分支 + 本地依赖布置（`_deps` junction、spatial_license.h、backend/.env）。
- **Phase 1**：L1 测试基建 + 12 组矩阵跑通，产出实测覆盖矩阵（测试先行，红的组就是待办清单）。
- **Phase 2**：缺口实现，按工程图影响排序，每项 TDD（先红后绿）：
  1. spline 面 + spl_sur 数据（圆角/扫掠/布尔全都会产生，影响最大）
  2. intcurve + int_cur 数据
  3. helix
  4. subshell
  5. pcurve + par_cur
  实现方式：参数提取从 Bolt 栈移植（`access_save_surfaces.cpp` / `access_save_curves.cpp` 已有全部提取逻辑，JSON 栈改为内嵌数组）；反序列化扩展 `parseSurface`/`parseCurve`，沿用三阶段（骨架/几何/拓扑）流程。
- **Phase 3**：L2 + L3 后端测试落地。
- **Phase 4**：Bolt 栈 bug 修复（rot_spl_sur 补 break、未知类型写属性标记而非裸节点、恢复 `.at()` 改防御性查找）+ bridge 回归。

## 5. 风险与边界

- **subshell 建模触发困难**：仅在大型/复杂壳结构中出现；若无法构造实例，G11 记为已知限制，以 L2/L3 mock 图覆盖存储语义。
- **样条数据量**：控制点数组可达数百 KB；JSON 栈选择内嵌数组（与 Bolt 栈字段级拆分不同），Neo4j 单属性上限约 16MB，安全；但 delta 推送体积会增大，需在 L3 观察写入耗时。
- **CI 跑不了 C++**：无 ACIS SDK；C++ 测试本地执行，结果记录进 docs/；L2 进 CI。
- **不改协议**：不动 HTTP/WS 路径、Mode0/Mode1 隔离、base_version 语义；JSON 只加节点类型与字段，向后兼容（旧图无新字段时走现有回退）。
- **渲染缺口不在本期**（deserializer 已有平面回退显示，见 `entity_graph_deserializer.cpp:631` 注释）。

## 6. 验收标准

1. `graph_roundtrip_tests.exe`：Phase 1 后 G1–G5、G9、G12 绿并给出 G6–G8、G10–G11 的红色基线；Phase 2 后全部 12 组绿。
2. `uv run --frozen pytest -q -m 'not integration'` 全绿（CI 同口径）。
3. L3（Neo4j 可用时）：逐实体 push→pull 图等价 + Neo4j 结构断言全绿。
4. 本文档随进度更新实测矩阵。
