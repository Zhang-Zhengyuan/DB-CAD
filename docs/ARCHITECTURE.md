# DBCAD 架构定稿（v2，2026-10-18）

> 本文是当前有效的架构基准。旧的"代码组织与维护边界"版本见 git 历史（`legacy/pre-architecture-v2` 及更早）。
> 分支：`refactor/architecture-v2`。本分支完成清理后，上层按此文件重建。

## 1. 总图

```mermaid
flowchart LR
    subgraph CLIENT["客户端（虚线框内为客户端的全部）"]
        subgraph CORE["核心进程（C++，与界面技术无关）"]
            direction TB
            K["几何内核<br/>建模 · 三角化 · 序列化"]
            T["上层<br/>版本管理 · 同步协同 · 场景组织"]
            K -->|"三角面片"| T
        end
        UI["UI 层<br/>Qt 或 Three.js，可整体替换"]
        SAT[("本地 SAT 文件")]
    end

    SRV["服务端（已定）<br/>FastAPI + 数据库"]

    K -- "① SAT 全量存取" --> SAT
    T -- "② 序列化推送 / 增量拉取（唯一的网络口）" --> SRV
    K -. "将来可选：SAT 直传" .-> SRV
    T -- "渲染场景（本地接口）" --> UI
    UI -. "操作命令" .-> T
```

一句话：**客户端就两个部件——核心进程 + UI**。核心进程对内包着几何内核，对外只有两个口子（本地文件口、网络口）；UI 只跟核心打交道，可整体替换。

## 2. 部件职责

| 部件 | 职责 | 边界 |
|---|---|---|
| 几何内核 | 建模、三角化（供渲染的网格）、序列化（内存模型 ↔ 传输 JSON 的唯一翻译） | 全程序唯一触碰 ACIS 的地方；输出全是中立数据（句柄、坐标、JSON），不含 SDK 类型 |
| 上层 | 版本管理、推送/拉取同步、协同冲突处置、把三角面片组织成可渲染场景 | 核心进程的一部分，无界面依赖；网络只在此处出现 |
| UI 层 | 显示与输入，别的什么都不干 | 只消费纯渲染数据（坐标/索引/颜色）和发操作命令；现在是 Qt，将来可换 Three.js 等，换 UI 不动核心 |
| 服务端 | FastAPI（版本管理、冲突检查、协同广播）+ 数据库（现为 Neo4j 图库，可换关系库） | 不加载几何内核，不解析 SAT；对客户端只有一个线协议 |

## 3. 设计不变量（三条，定下来不再讨论）

1. **内核是唯一懂几何的地方**——建模、三角化、序列化都从这里出，别处不碰 ACIS。
2. **存储是可换的后端**——就两条链路：SAT 全量（本地文件，内核原生读写）；序列化 JSON（经上层同步到服务端入库）。将来 SAT 直传云端、图库换关系库，都是加一条后端实现，不动内核和上层。
3. **UI 与 AI 都是核心的"客户端"**——核心不关心对面是人、是界面还是 AI，只认"要场景数据"和"发操作"两种交互。AI 将来作为另一个操作者直接在上层命令入口发命令，与 UI 平级，不需要外挂协议。

## 4. 存储与线协议

- **两条存储链路**：SAT 全量本地存取（`access.cpp`，交换格式 + 测试正确性基准）；序列化 JSON 云端协作（客户端编码组件 → FastAPI 合并 → Neo4j 不可变组件 + 版本清单链）。SAT 片段作为组件属性随 JSON 走，服务端不解析。
- **线协议冻结**：`POST/GET /projects/{id}/json-delta`（base_version 乐观锁、组件级整替合并）与 WS 通知协议保持不变，两侧测试（后端 44 项无 DB 回归 + L3 真 Neo4j 往返）是保护网。换数据库、升级库内物理表示，都不动协议。
- **库内物理表示（Phase B 已实施，2026-10-18）**：`JsonGraphEntity` 双表示——权威载荷 labels_json/props_json（graph() 读投影，迁移前后逐字节等价）+ 原生 Neo4j 标签与一级扁平字段（可查询投影，如 `MATCH (f:Face) WHERE f.geometry_type='spline'`；未识别类型显式落 `:Unknown`，绝不静默）。存量迁移脚本幂等（`backend/scripts/migrate_json_graph_native_v2.py`）。
- **协同模型（M4.1–4.3 已实施）**：混合粒度不变；自动微版本同步 + 显式定版标签（`/json-delta/tags`）、变更通知到达即自动拉取（无冲突时；同组件并发仍诚实拒绝）、组件级编辑意图广播与软锁提示（edit_intent）。分支 DAG 与语义 diff 仍为路线图（REFACTOR_PLAN M4.4/4.5）。拓扑级自动合并不做（拓扑命名问题无解，业界同样不做）。

## 5. 本轮清理记录（2026-10-18，refactor/architecture-v2）

删除（留档 tag：`legacy/pre-architecture-v2`）：

- **客户端 Bolt 直连 Neo4j 栈**：`storage/` 全部 12 文件、`neo4j.cpp/hxx`（mgclient 封装）、`kernel/legacy_storage.h`、`access.hxx`/`access.cpp` 中的 Neo4j 部分、`DbcadAPI` 兼容门面、内核里的 5000 圆柱打孔基准与直连诊断入口、对应菜单/模式/对话框/配置读取。
- **PostgreSQL 实验**：`pg_service`、`pg_store`、`ui/mainwindow_postgres.cpp`、部署脚本（with_pg/pgadmin/build_pg_demo）及构建依赖（libpq/openssl/glaze/mgclient 路径）。
- **死代码**：`entity_merge_engine`（零调用的三方合并引擎）；后端 `graph_diff`/`conflicting_ids`（未接线的旧冲突设计）与 `hasattr(store,"graph")` 不可达分支。
- **过时文档**：`技术路线指导.md`（bridge 时代架构，与现状冲突）。

保留的存取模式只剩两个：**SAT 文件（本地）** 与 **FastAPI（云端协作）**。

验证：客户端 Debug 编译通过（`x64/Debug/CAD_DB.exe`）；后端 `pytest -m 'not integration'` 43 项全绿。

## 6. 重建路线（2026-10-18 全部主里程碑完成）

> 执行计划、任务分解与各里程碑验收标准见 [REFACTOR_PLAN.md](REFACTOR_PLAN.md)。

1. **上层重建（M1/M2 已完成）**：协作编排已下沉 `CAD_DB/core/`（DocumentSession/CollabState/CollabController/CollabBackend/StorageBackend/CommandBus，仅 QtCore）；MainWindow/Qt 只剩壳；全部网络异步（无嵌套事件循环）。
2. **存储物理表示升级（M3/Phase B 已完成）**：原生标签 + 扁平字段 + 幂等迁移 + L3 结构断言；协议与客户端零改动。
3. **协同工作流（M4.1–4.3 已完成）**：自动拉取、在场感知（edit_intent 软锁提示）、定版标签；分支 DAG（M4.4）与语义 diff（M4.5）为后续路线图。
4. **AI native（M6 第一步已具备）**：CommandBus 提供名→schema→执行器的程序化命令入口（AI 与 UI 平级调用同一注册表）；审批流为后续工作。
5. **实体级增量（M5/Phase D）**：后续路线图，见 STORAGE_CONSOLIDATION_PLAN。

执行细节（命令记账、撤销栈、防回环、断线重连等）都属于上层内部实现，设计层面不在此展开。
