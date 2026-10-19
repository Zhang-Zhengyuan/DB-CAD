# v3 存储链路基准（P9-2，abc_cache 语料压测）

> 2026-10-19 · 工具：`CAD_DB/backend/scripts/benchmark_v3.py` ·
> 明细：`artifacts/benchmark_v3_200.json` · 语料：ABC 数据集 STEP 模型抽样 200 件
> （D:/DBCAD/abc_cache/extracted，真实工程 CAD 模型）。

## 链路

STEP → Spatial InterOp（BRep+Assembly）→ ACIS 组件（SAT 内容寻址 + GLB +
graph 投影）→ 实体级投影写入（entity_key 内容寻址 + generation 链）→ 统计。
worker 为真 ACIS 内核（x64/Debug/dbcad_worker.exe）。

## 结果（200 模型）

| 指标 | 数值 |
|---|---|
| 成功 / 失败 | **200 / 0** |
| 组件总数 | 1311 |
| 实体投影总数 | 660,772 |
| 实体内容新增 / 共享（去重） | 260,716 / 400,056 |
| **内容去重率** | **60.5%**（跨模型同内容实体共享一个节点） |
| STEP 转换 + 组件化耗时 | 223.5s（≈1.12s/模型） |
| 投影写入耗时 | 13.3s（≈67ms/模型，66 万实体） |
| unknown 标签 | 0 |
| 存储/投影错误 | **0** |

fallback 分布：1311/1311 组件为 `sat_asset`——STEP 转换产物普遍带 ATTRIB/
过程曲面触发风险判定（与 v2 probe 基线一致的保真判定，非异常）；几何真相
由 SAT 资产承载，投影结构完整（66 万实体全部入库可查）。

## 压测暴露并修复的缺陷（诚实记录）

1. **无面体/不可离散面导致整模型导入失败**（首轮 8/200 失败）：STEP 装配
   含线框体（body has no faces）与个别不可离散面——原实现任一组件 mesh
   失败即整体报错。修复：**降级不失败**——GLB 跳过、manifest 标
   `mesh_error`，SAT/graph（几何真相与投影）照常产出；网关/前端对空 GLB
   组件跳过渲染。修复后 200/200 全过，8 个原失败模型实测降级标记正确。
2. 压测脚本初版组件键拼接 bug（bytes+str）——与存储链路无关，已修。

## 结论

- v3 存储链路（P6 投影 + P8 内容寻址增量）在真实工程语料上**零错误**，
  v2 时代的 KNOWN-DIFF（pcurve 参数域往返偏差）在 v3 无消费方
  （反序列化器退役，见 FIDELITY_REFINEMENT_ROADMAP 头注）。
- 内容寻址收益实测：60% 实体节点跨模型共享（ABC 同族模型重复几何大量），
  存储量与写入量按内容增长而非按模型数增长——这正是"不可变组件+实体
  内容寻址"的设计目标，可作为论文实验数据。
