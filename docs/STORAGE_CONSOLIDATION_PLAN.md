# 存储链路收敛与重构计划（架构取舍）

> 前置文档：[ENTITY_STORAGE_TEST_PLAN.md](ENTITY_STORAGE_TEST_PLAN.md)（逐实体单元测试与 Phase 1 实测基线）。
> 本文回答的问题：四条存储链路收敛成几条、废弃什么、保留什么、补什么。

## 1. 目标架构

```
┌─ 客户端（CAD 应用）────────────────────┐   ┌─ 中心服务器（纯数据面）──────────┐
│ ACIS 内核：建模 / 渲染 / 撤销重做        │   │ FastAPI：json-delta 同步        │
│ 实体图序列化 codec（唯一）               │←→ │   版本管理 / 冲突检查 / 审计     │
│ SAT 导入导出（交换格式 + 校验基准）      │HTTP│ Neo4j：唯一结构化存储            │
└──────────────────────────────────────┘   └────────────────────────────────┘
```

三条原则：

1. **内核只在客户端**。建模、渲染、几何计算需要 ACIS，天然属于客户端。
2. **服务器只是数据库**。做数据同步、版本管理、冲突检查，不碰几何内核。
3. **一条结构化存储链路**。所有实体数据走同一种 codec、同一种库内表示。

## 2. 现状 → 目标：四栈收敛为一栈

| 链路 | 现状 | 处置 | 理由 |
|---|---|---|---|
| **B. JsonGraphStore**（Mode1 默认） | 实体级节点，但 labels/props 是 JSON 字符串 | **升级为目标链路** | 协议、增量语义、版本链、冲突检查都已建成且有测试；只需补 codec + 升级库内表示 |
| **D. C++ Bolt 直写栈** | 论文原版字段级结构化，覆盖最全；入口是 bridge / 客户端直连 | **codec 移植，传输废弃** | 存储设计是论文核心贡献，保留为知识源；但直连要求客户端持有 DB 凭据 + apoc，冲突检查/写锁/审计要在库里重做，服务器失去管控，与目标架构冲突 |
| **A. Neo4jStore**（Mode0 blob） | SAT/entity_graph 整包塞 content_json | **冻结为 SAT 归档** | 已完成、简单、有测试；保留做数据备份与 SAT 交换，不再加功能 |
| **C. Neo4jEntityStore** | 休眠 legacy，无客户端调用，全局 local_id 缺陷 | **废弃** | 已被 B 取代，留着只会维持混乱 |
| C++ Storage Bridge（:8100） | 服务端跑 ACIS 转存 | **废弃** | 服务器不再需要内核，bridge 失去存在理由 |
| 客户端直连 Neo4j（Bolt） | access.cpp 性能对比 + 直连 UI 入口 | **入口退役，性能脚本保留** | 同 D；论文性能数据复现脚本保留 |
| access_delta_push/pull.cpp | Bolt 版增量实验，无调用方 | **废弃** | 已被 HTTP JSON delta（ui/mainwindow_json.cpp）取代 |
| PostgreSQL 实验直存 | ui/ 实验功能 | **冻结** | 与实体存储主链路无关 |
| SAT 本地文件 | access.cpp / entity_tests | **保留** | 交换格式 + 测试正确性基准（L1 失败诊断靠 SAT diff） |

## 3. 粒度权衡：论文理想 vs 工程实现

图数据库结构化的三档粒度：

| 档位 | 库内表示 | 代表 | 可查询性 |
|---|---|---|---|
| 粗 | 整包 blob 属性 | A | 无 |
| 中 | 实体级节点，属性打包 JSON 字符串 | B 现状 | 只能按 id/哈希 |
| **细** | **实体级节点 + 原生标签 + 一级扁平字段属性** | D 的落法 | `MATCH (f:Face {sense:1})` 原生可查可索引 |

**推荐目标：B 的传输协议与版本语义 + D 的物理表示（"细"档）。**

- 每个实体一个节点：原生标签（body/lump/shell/face/…，直接用序列化器现有 label）；
- 实体参数做**一级扁平化**：`props.sense → sense`；嵌套的 `geometry.*` 加前缀（`geometry_type`、`geometry_root_point`、`geometry_ctrlpts`…）；样条大数组（ctrlpts/knots）整体一个 list 属性——字段级拆分，不做元素级；
- 保留 B 的**不可变 + 内容寻址去重**：`(component_hash, entity_id)` 唯一约束不变，MERGE ON CREATE SET；
- **协议完全不变**：客户端发的还是现在这份 JSON，表示升级是服务端内部的事——旧客户端无需改动，这是本方案最大的工程优势。

**兜底方案**（若迁移代价超预期）：保持 labels_json/props_json 打包，只做 codec 补全。多人协同与版本管理不受影响，但论文"图数据库结构化"的贡献被削弱为"图容器"，WebUI 也只能展示 JSON。先按推荐方案做，兜底作为决策检查点。

**与论文设计的映射**（写论文/答辩时对应）：

| 论文概念 | 工程落点 |
|---|---|
| 字段级拆分存储（3.3.3） | 实体节点一级扁平字段属性 |
| 版次链 generation_prev_ptr（3.3.4） | JsonGraphVersion PREVIOUS_JSON_VERSION 链 |
| generation_new_ptr/old_ptr 不可变引用 | 不可变实体节点 + 内容寻址（同内容跨版本共享） |
| spl_sur/int_cur/par_cur 分离存储 | geometry_* 前缀属性（大数组独立属性，等效分离） |
| 实体级增量存储（第 5 章） | D 栈 `generation_new_ptr/old_ptr` 已实现（见第 4 节），Phase D 移植到 B 协议 |

**未知实体策略**（吸取 D 的教训——未知类型写成无标签裸节点、静默丢参数）：任何 codec 不认识的实体，显式写 `:Unknown` 节点 + 原始载荷属性 + 序列化统计上报，**绝不静默丢弃**。

## 4. 增量更新的现状与演进（论文第 5 章对应物）

历史遗留确认：D 栈的原始版本**已实现实体级增量更新**，且 UI 菜单至今在调用：

| 组件 | 位置 | 机制 | 现状 |
|---|---|---|---|
| 公告板扫描 | `storage/access_history_state.cpp:12`（`api_compute_delta_since`，干净版） | 遍历 [lastsave_ds, 当前] 的差状态/公告板/公告，`new_entity_ptr`→新增集、`old_entity_ptr`→删除集，对冲先建后删 | 干净版零调用；`api_advance_delta_since`（基线推进）仍在 4 处 UI 调用 |
| 实体级增量写入 | `storage/access_history_save.cpp`（`api_save_neo4j`，菜单"增量保存"调用） | 每次保存建 `(:generation)` 节点；修改=新节点+`generation_old_ptr`(旧)+`generation_new_ptr`(新)；删除=仅 `generation_old_ptr`；`generation_prev_ptr` 版次链；旧节点入边复制重定向到新节点 | 活代码，但走 Bolt 直连（Phase C 退役其传输层） |
| 增量恢复 | `storage/access_history_restore.cpp`（`api_restore_neo4j`） | 沿三种 ptr 子图遍历 → new 端点 − 仍被 old 引用者 → `apoc.algo.cover` 取边重建 | 同上 |

与 B 的关系：B 的增量是**组件级**（变更 body 整组件替换 + 内容寻址去重）；D 是**实体级**（只写公告板中真正变化的实体）——后者是论文第 5 章的核心贡献，B 未继承。D 增量版的三个固有缺陷（移植时必须修复）：

1. 类型白名单排除 spline/intcurve/pcurve（与全量路径同源缺口，Phase A 解决）；
2. `ptr2nodeid` 内存态、跨会话丢失；各客户端基线与服务端版本链无 base_version 对齐（B 已有，需并入）；
3. 入边复制重定向使边数随版本线性增长，无压实机制。

**演进路线（列为 Phase D，Phase B 之后做）**：协议不动（`changes` 已是 uuid 级）——

1. 客户端：编辑记账复用 `pendingEntityChanges`；公告板扫描作为兜底/自动发布触发器保留；
2. 服务端：JsonGraphStore 写入从"变更组件整体替换"细化为"组件内实体级 diff"（内容寻址天然只写新实体节点）；
3. 版本回放：`JsonGraphVersion` 组件链语义升级为 generation 式实体级回放（等价于 `generation_new_ptr/old_ptr` 的不可变节点集合语义）；
4. 压实：定期后台合并连续微改版本，控制节点/边膨胀。

## 5. 分阶段计划

### Phase A：客户端 codec 补全（当前分支 storage/entity-unit-tests 已开始）

与库内表示无关、先做不返工——无论目标表示是"细"还是兜底，JSON 载荷都必须完整。

1. 区间编码修复（G1/G4/G6/G10 共同根因：无限区间 2 元素 vs parseInterval ≥4 元素回退 [0,1]；cone uv_range 双格式混用）
2. 序列化 EDGE 自身参数区间（当前只发底层曲线 param_range）
3. spline/spl_sur、intcurve/int_cur 载荷——从 `access_save_surfaces.cpp` / `access_save_curves.cpp` 移植提取逻辑
4. helix、subshell、pcurve/par_cur
5. transform 叠加语义、subtract 负体积（face sense/环方向）

**验收**：`graph_roundtrip_tests.exe` 12/12 组绿；`uv run pytest -q -m 'not integration'` 全绿；L2 夹具同步扩展。

### Phase B：服务端存储表示升级 v2（协议兼容，可灰度）

1. JsonGraphStore 写读双路径 v2：原生标签 + 扁平属性；常用标签建索引；约束不变
2. 存量数据一次性迁移（Cypher：labels_json/props_json → 原生），旧节点标记 legacy 保留
3. L3 真 Neo4j 集成测试：逐实体 push→库内结构断言（标签/字段/边）→pull 等价；同图二次 push 去重幂等
4. WebUI 适配原生标签浏览
5. 未知实体兜底 + 序列化覆盖率统计落地

**验收**：L3 结构断言全绿（如 `MATCH (f:Face) WHERE f.geometry_type='spline' RETURN count(f)` 可用）；迁移脚本对空库/存量库幂等。

### Phase C：退役清理（每项删除单独一个 PR，动 master 前公告并行协作方）

1. 删 Storage Bridge（service + storage_bridge_*.cpp + 后端 storage_bridge.py + URL 配置项）
2. 退役客户端直连 Neo4j 入口（UI 与凭据配置；`access_graph_save/restore.cpp` 保留为 codec 参考，标注 frozen）
3. 删 C 栈端点（neo4j_entity_store.py + /entities API）
4. 删 access_delta_push/pull.cpp；冻结 PostgreSQL 实验
5. Mode0 WS submit_entity_graph 数据流下线（A 保留为 SAT 归档 HTTP 端点）；WS 协同流收敛为单通道
6. ARCHITECTURE.md 存储章节重写；README 路线图更新

**验收**：全测试绿；启动脚本不再拉起 bridge；文档与实际一致。

### Phase D：实体级增量更新（论文第 5 章的完整恢复）

Phase B 稳定后启动，协议不变，详见第 4 节演进路线。**验收**：仅改一个 face 半径的二次 push，库内新增实体节点数 ≈ 变更实体数（而非整个组件）；版本回放与全量图等价（L3 断言）。

## 6. 风险与兼容

- **wire 兼容**：Phase B 不动协议字段，旧客户端照常工作（风险最低的收敛方式）。
- **数据迁移**：一次性脚本 + 旧节点保留，可回退；迁移前后 graph() 重组结果必须逐字节等价（用 L3 断言兜住）。
- **Neo4j 上限**：list 属性上限远大于样条控制点数组（数百 KB）；超阈值（如 1MB）告警并记录，不阻断。
- **去重膨胀**：字段级不可变节点下，频繁微改会产生新组件；Phase B 阶段靠内容寻址共享未变部分，Phase D 落地实体级增量 + 压实后彻底解决。
- **并行协作**：Phase C 删除前逐项公告；在此之前所有删除目标先标 `// DEPRECATED:` 冻结。
- **决策检查点**：Phase B 开工前用半天做一个写读 PoC（单 face 原生标签往返），确认兜底方案不需要启用。
