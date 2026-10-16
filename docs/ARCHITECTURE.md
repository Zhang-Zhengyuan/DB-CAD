# 代码组织与维护边界

## 协作链路

唯一可写版本链为 `JsonGraphVersion`。客户端通过 `POST /projects/{id}/json-delta` 提交，`GET` 增量 Pull；保存、另存、版本列表、历史打开均使用这条链路。WebSocket 只负责在线成员、版本通知和刷新。旧 Mode0、独立 EntityGraph 版本写入、Bridge 进程和路由已删除。

`entity_graph_serializer.cpp` 复用单机底层的 ACIS 拓扑/几何编码、风险识别和内核 SAT 片段保存。每个顶级组件携带 SAT：`on_failure` 优先细粒度重建，`sat_fragment` 用于已知有损组件。`entity_graph_deserializer.cpp` 按 UUID 分组件恢复，全部成功后才把结果交给 UI。

`ui/mainwindow_json.cpp` 负责变更提交、版本冲突与 Pull；`mainwindow_graph.cpp` 按 UUID 替换组件，保留无冲突的本地修改。重建失败不推进本地版本。`CollabSession` 管理提交与版本状态。

## 存储

`backend/app/json_collab.py` 处理验证、拓扑闭包和组件合并。`json_graph_store.py` 保存不可变组件及版本清单，未改变组件可复用。SAT 是同一组件的属性，没有第二次数据库提交。后端不解析 SAT，不依赖 ACIS。

`neo4j_store.py` 仅管理项目目录。`neo4j_webui/` 保留现有数据库的只读历史审计（包括旧模式记录），不构成协作写入链路。已有旧 Mode0 数据不会自动转换或删除。

`access.cpp` 和 `storage/` 中仍由单机 Neo4j 保存、恢复、历史菜单使用的实现保留；ACIS 文件存取和独立 PostgreSQL 菜单也保留。

## 部署

`deploy/start_dbcad_fullstack.ps1` 启动 Neo4j、FastAPI 和客户端。`deploy/lib/` 管理配置、运行时、Docker、进程和打包。打包同步后端静态资源；不复制或启动 Bridge。

验证边界与手工测试步骤见 [统一协作链路说明](HYBRID_COLLABORATION.md)。
