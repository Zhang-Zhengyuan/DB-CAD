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

### 1.2 实体覆盖矩阵（现状）

对照存储模型设计规范（参考文献·张家荣论文第 3 章：10 类拓扑实体 + 5 类几何实体 + 3 类样条数据节点 + part/generation 版次链），Mode1 JSON 栈现状：

| 实体 | 规范要点 | 序列化 | 反序列化 | 状态 |
|---|---|---|---|---|
| body/lump/shell/wire/face/loop/coedge/edge/vertex | 40 类边 + 6 属性 | ✅ | ✅ | 完整 |
| transform | affine/translation/scaling 等 | ✅ | ✅ | 完整 |
| plane / sphere / torus / cone 面 | 定长属性，内嵌 face.props.geometry | ✅ | ✅ | 完整 |
| straight / ellipse 曲线 | 定长属性，内嵌 edge.props.geometry | ✅ | ✅ | 完整 |
| point (APOINT) | vertex 内嵌 position | —（嵌入） | ✅ 兼容 | 可接受 |
| **spline 面 + spl_sur 数据** | 15 字段 NURBS | ⚠️ 仅类型名（serializer.cpp:167） | ❌ parseSurface 拒绝（deserializer.cpp:156） | **缺口 1** |
| **intcurve + int_cur 数据** | 30+ 字段交线 | ⚠️ 仅类型名（:147） | ❌（:190） | **缺口 2** |
| **helix 曲线** | 8 字段 | ⚠️ 仅类型名（:145） | ❌（:190） | **缺口 3** |
| **subshell** | 5 边三向自引用树 | ❌ 完全跳过（:485 默认分支） | ❌ 未知 label 跳过 | **缺口 4** |
| **pcurve + par_cur 数据** | 4 属性 + 2 出边 | ❌ 完全跳过 | ❌ | **缺口 5** |

**后果**：任何含圆角、扫掠、自由曲面或布尔交线的完整工程图，几何数据在客户端序列化阶段就丢失；反序列化对 spline/helix/intcurve 直接拒绝。"打开不了"的首要原因在这里，而不是 Neo4j。

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

| 组 | 模型 | 覆盖实体/几何 | 实测（Phase A 第一轮修复后） |
|---|---|---|---|
| G1 | 方块 | 全拓扑 + plane + straight | **PASS**（区间编码修复） |
| G2 | 球 | sphere + 圆边 + 极点退化 | PASS |
| G3 | 圆柱 | cone(柱) + 圆边 | PASS |
| G4 | 圆锥 | cone + ellipse/straight | **PASS**（base_major_axis 字段名不匹配 + u_param_scale 未传修复） |
| G5 | 圆环 | torus | PASS |
| G6 | 六棱柱扫掠 | spline 面 + intcurve 边 | **PASS**（体积修复；侧面实为 plane，spline 断言待 G 场景） |
| G7 | 布尔并/差/交 | intcurve + pcurve + 混合几何 | **全 PASS**（含原负体积 subtract） |
| G8 | 螺旋线线框 | **helix** | 红：缺口 3（仅类型名，结构恢复正常） |
| G9 | 变换体 | transform 节点 | 红：变换叠加语义错误（几何与 transform 节点双重应用） |
| G10 | 圆角 | blend 产生 cone/sphere 面 + **tangent 边** | 红：**缺口 2 实锤**——圆角 48 条边为 `@7 tangent`（intcurve），面载荷已全对 |
| G11 | subshell 探测 | **subshell** | SKIP：测试模型未触发 ACIS 分层 |
| G12 | 直线线框 | wire 拓扑 + straight 边 | PASS |

### 3.1 Phase 1 发现与第一轮修复（已完成 ✅）

**已修复**（serializer/deserializer，均已由 L1 测试验证）：

0. ✅ **区间编码**：`intervalToJson` 恒定 4 元素；`parseInterval` 无限区间跳过 limit；边的 `range` 改为序列化 EDGE 自身参数区间（`edge->param_range()`）。修复 G1/G6/G7（含负体积）。
1. ✅ **cone 双缺陷**：`base_major_axis` 字段名不匹配（发 `base_major_axis` 读 `major_axis`）+ `u_param_scale` 未传（构造器默认 0 导致锥面损坏）+ uv_range 双格式混用。修复 G4。
2. ✅ `parseVector/parsePosition` 空数组默认值未置零（SAT 出现 1e-310 垃圾值）。
3. ✅ 后端 `topology_types` 补齐 subshell/pcurve 边。

**待修**（按优先级）：

1. **intcurve + int_cur 载荷**（缺口 2）——G10 圆角被此阻塞：48 条 `@7 tangent` 边无几何。
2. **transform 叠加语义**（G9）：几何与 transform 节点双重应用。
3. **helix 载荷**（缺口 3，G8）、spline/spl_sur（缺口 1）、subshell（缺口 4）、pcurve/par_cur（缺口 5）。
4. 观察项：SAT 文本含 `-0` vs `0` 的负零伪差异（数值等价，SHA 字本比对需归一化）；mass_pr 对混合面时向 stdout 打印分面积分（ACIS 行为）。

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
