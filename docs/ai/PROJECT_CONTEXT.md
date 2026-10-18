# 项目上下文与按需导航

核对日期：2026-10-18。本文是定位索引，实际源码、锁文件、构建文件与当前 CI 是事实来源；历史设计稿描述的目标不等于已实现。模块路径或命令变化时同 PR 更新本索引。

## 运行链路

`Qt UI → GeometryKernel/文档状态 → 组件 JSON + SAT → FastAPI → Neo4j JsonGraphVersion`。
HTTP 负责写入/Pull；WS 通知远端版本与在线成员。后端存储 SAT 片段但不解释几何，不需要 ACIS。每个组件按 UUID 恢复，重建失败不能提交半成品给 UI。

客户端有 `CAD_DB.vcxproj` 和 `DbcadAcis.vcxproj` 两个目标。当前为编译时 ACIS 适配，尚无第二几何内核、运行时插件和完整无 SDK 客户端构建。Window 仍协调编辑与历史。

## 从需求到源码

| 需求 | 先定位 | 再读 / 验证 |
|---|---|---|
| 菜单、文档、编辑、保存/Pull | `CAD_DB/ui/`、`window.cpp`、`mainwindow.h` | `docs/ARCHITECTURE.md`、`docs/KERNEL_ISOLATION.md`；客户端回归 |
| 网络、会话、冲突 | `backend_api_client.*`、`collab_session.*`、`ui/mainwindow_json.cpp` | `docs/HYBRID_COLLABORATION.md`；前后端同步验证 |
| 几何接口、生命周期、渲染 | `kernel/geometry_kernel.h`、`model_types.h`、`document_model.h`、`acis_kernel.cpp`、`glwidget.cpp` | `docs/KERNEL_ISOLATION.md`、`docs/ACIS_2026_MIGRATION.md` |
| 组件 JSON/SAT 编解码 | `entity_graph_serializer.cpp`、`entity_graph_deserializer.cpp`、`storage/` | `docs/HYBRID_COLLABORATION.md`、`docs/ENTITY_STORAGE_TEST_PLAN.md`；往返和失败恢复 |
| HTTP/WS 协议 | `backend/app/api/`、`schemas.py`、`json_collab.py`、`collaboration/` | `CAD_DB/backend/README.md`、`backend/tests/test_json*` |
| Neo4j 版本与组件复用 | `backend/app/json_graph_store.py`、`neo4j_store.py` | 组件闭包/冲突/真实数据库集成检查 |
| WebUI | `backend/app/neo4j_webui/`、`static/webui/` | `backend/tests/test_webui_audit.py`；鉴权与页面人工验证 |
| 本机环境、构建依赖 | `scripts/check-environment.ps1`、`build.ps1`、`Directory.Build.props` | `docs/GETTING_STARTED.md`、`DEPENDENCIES.md`、`LOCAL_SETUP.md` |
| 部署和打包 | `CAD_DB/deploy/`、`deploy/lib/` | `CAD_DB/DEPLOYMENT.md`、`scripts/tests/test-deploy-*.ps1` |
| 旧库兼容 / PostgreSQL 实验 | `storage/`、`neo4j.cpp`、`pg_*`、`ui/mainwindow_postgres.cpp` | `docs/ARCHITECTURE.md`、`CAD_DB/deploy/README.md`；先核实真实入口 |

表内省略 `CAD_DB/` 的客户端路径均相对此目录；backend 路径相对 `CAD_DB/`。不确定文件名先限定目录用 `rg --files` 查找。

## 不要默认读取的内容

`_deps/`、`.vs/`、`x64/`、`dist/`、`artifacts/`、`neo4j-runtime/`、虚拟环境和参考文献不在日常源码搜索范围。需要 SDK 声明时只定位相关头文件；不把授权 SDK 复制进仓库。外层同名目录和 clean 副本不是当前代码事实来源。

## 技术参考包

提供的《重要文档（给AI用的）》含 00 索引及 01 保存恢复、02 历史、03 错误、04 Bulletin、05 属性、06 应用接口、07 API 参考，整理自 18 份 ACIS PDF。它是几何内核参考集，不是 Git 协作规范。

使用原则：保存恢复要验证拓扑/属性/版本兼容；历史状态配对与回滚要覆盖失败路径；检查 ACIS outcome 及恢复后对象有效性；Bulletin/内核历史不能直接等同于多人协作提交；UUID 不能依赖进程地址或一次会话的句柄。具体内存所有权、宏和 API 签名必须查本项目当前适配实现及授权 SDK 文档，PDF 提取示例可能缺行。

本次刷新远端后确认，维护者已将这组整理稿收录到 `参考文献/00_索引.md` 至 `07_API参考.md`。涉及相应 ACIS 主题时按需读取，不默认加载全文；不要重复导入附件，也不要进一步复制 SDK、许可或完整厂商文档。新增资料公开前由维护者确认授权范围。
