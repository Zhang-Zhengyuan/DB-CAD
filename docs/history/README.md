# 历史文档（架构 v2 及更早）

这些规划/实现/验收记录对应已被 v3 取代或完成的阶段，按时间序归档。
现行架构以 [../ARCHITECTURE.md](../ARCHITECTURE.md) 与 [../V3_PLAN.md](../V3_PLAN.md) 为准；
引用它们的代码注释与文档链接仍指向本目录。

| 文档 | 内容 | 状态 |
|---|---|---|
| [KERNEL_ISOLATION_DESIGN_20261016.md](KERNEL_ISOLATION_DESIGN_20261016.md) | 几何内核隔离方案与测试计划 | 方案已并入 [../KERNEL_ISOLATION.md](../KERNEL_ISOLATION.md)；v3 下内核随 worker 进程隔离 |
| [KERNEL_ISOLATION_IMPLEMENTATION_20261016.md](KERNEL_ISOLATION_IMPLEMENTATION_20261016.md) | 内核隔离实现记录 | 完成 |
| [KERNEL_ISOLATION_FIX_20261016.md](KERNEL_ISOLATION_FIX_20261016.md) | Push 故障修复与隔离复查 | 完成 |
| [HYBRID_COLLABORATION.md](HYBRID_COLLABORATION.md) | Mode1 JSON+SAT 统一协作链路（v2 客户端权威） | v3 改为服务端权威版本链；协议设计仍有参考价值 |
| [STORAGE_CONSOLIDATION_PLAN.md](STORAGE_CONSOLIDATION_PLAN.md) | 存储物理表示统一（Mode0/Bridge 删除，细档原生标签+扁平字段的出处） | 完成；细档物理表示被 v3 实体投影复用（`json_graph_native.py`） |
| [ENTITY_STORAGE_TEST_PLAN.md](ENTITY_STORAGE_TEST_PLAN.md) | 实体级存储单测计划（v2 数据库储存） | 完成 |
| [FIDELITY_REFINEMENT_ROADMAP.md](FIDELITY_REFINEMENT_ROADMAP.md) | 100% 储存保真路线图（KNOWN-DIFF 注册表） | v3 以 SAT 为几何真相（ADR-3），纯图复原路径退役，KNOWN-DIFF 注册表随之退役 |
| [REFACTOR_PLAN.md](REFACTOR_PLAN.md) | 架构 v2 重构计划（Qt 壳 + core/ 协作库） | 完成；v3 已移除 Qt 客户端 |
| [UI_REFACTOR_PLAN.md](UI_REFACTOR_PLAN.md) | Qt → Web 前端计划（已并入 V3_PLAN） | 完成 |
| [REFACTOR_VALIDATION.md](REFACTOR_VALIDATION.md) | v2 重构本地验证记录（core_unit_tests 116 项） | 完成；core/ 随 Qt 客户端一并移除 |
| [LOCAL_SETUP.md](LOCAL_SETUP.md) | v2 时期本机 Qt/依赖迁移配置记录 | 归档；现行上手见 [../GETTING_STARTED.md](../GETTING_STARTED.md) |
