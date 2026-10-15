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

| 组 | 模型 | 覆盖实体/几何 | Phase 1 实测 |
|---|---|---|---|
| G1 | 方块 | 全拓扑 + plane + straight | **体积 100/1000**（区间编码缺陷，见下） |
| G2 | 球 | sphere + 圆边 + 极点退化 | PASS |
| G3 | 圆柱 | cone(柱) + 圆边 | PASS |
| G4 | 圆锥 | cone + ellipse/straight | **mass_pr 失败 + bbox 偏 4**（cone uv_range 解析缺陷） |
| G5 | 圆环 | torus | PASS |
| G6 | 六棱柱扫掠 | spline 面 + intcurve 边 | 侧面实为 plane（未产生 spline）；**体积恰 1/4**（同区间缺陷） |
| G7 | 布尔并/差/交 | intcurve + pcurve + 混合几何 | intersect PASS；**unite 体积 147.5/1875、subtract 体积为负** |
| G8 | 螺旋线线框 | **helix** | 缺口 3 红灯确认：`{"type":"helix"}` 仅类型名；结构恢复正常 |
| G9 | 变换体 | transform 节点 | **体积 800/8000、bbox 双倍平移**（变换叠加语义错误，叠加 G1 缺陷） |
| G10 | 圆角 | blend 产生 cone/sphere 面 | 载荷完整；**体积偏差 31%**（同区间缺陷） |
| G11 | subshell 探测 | **subshell** | SKIP：测试模型未触发 ACIS 分层 |
| G12 | 直线线框 | wire 拓扑 + straight 边 | PASS |

### 3.1 Phase 1 实测新发现（按修复优先级）

测试暴露的问题比"缺实体类型"更根本——**即使已支持的实体类型，恢复路径也存在几何损坏**：

0. **区间编码缺陷（最高优先级，G1/G6/G7/G10 共同根因）**：`intervalToJson`（serializer.cpp:67）把无限/半无限区间编码为 2–3 元素数组，而 `parseInterval`（deserializer.cpp:87）要求 ≥4 元素、否则回退为固定 `[0,1]`。方块边的底层直线是无限区间（SAT diff 实锤：原始边区间 `-5..5` → 恢复后 `0..1`），导致所有平面体的边在参数空间塌缩、体积错误（10×10×10 方块恢复成 100）。修复：编码恒定 4 元素 + 解码兼容 2/3/4 元素；同时应序列化 EDGE 自身的参数区间（当前只发了底层曲线的 param_range）。
1. **cone 面 uv_range 双格式混用**：序列化端发裸值 `[u0,u1,v0,v1]`，cone 分支（deserializer.cpp:145）却按 typed interval `[type,val,...]` 解析 → 底圆 ellipse 被 limit 成 `[0,1]` → G4 全坏。
2. **transform 恢复语义错误**（G9）：几何与 transform 节点的叠加关系错位（疑似几何已带变换、恢复时又附加 transform 节点），待 G0 修复后复查。
3. **subtract 负体积**（G7）：面 sense/环方向在布尔差结果上恢复错误，待查。
4. **后端 topology_types 缺 subshell/pcurve 边**（已在本分支修复 ✅）：`json_collab._component_ids` 的拓扑边集合缺 `shell_subshell`、`subshell_parent/sibling/child/face/wire`、`face_subshell`、`wire_subshell`、`coedge_geometry`，导致这些节点会被拆进 `__orphan__` 组件。
5. 观察项：含球+锥面的体在 `api_body_mass_pr` 时 ACIS 向 stdout 打印分面积分数值（非本项目代码），不影响判定。

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
