# 代码组织与维护边界

公共启动入口保留：`CAD_DB.sln`、`CAD_DB/backend/app/main.py`、`CAD_DB/deploy/start_dbcad_fullstack.cmd`。

## 桌面客户端

`CAD_DB/mainwindow.cpp` 负责窗口初始化和关闭；`ui/` 按职责提供 MainWindow 成员实现：连接配置、PostgreSQL、变更记录、快照恢复、增量应用、JSON Push/Pull、协作控制、在线成员、消息分发、文档、菜单与编辑操作。`mainwindow_internal.h` 是私有实现依赖，公共声明仍在 `mainwindow.h`。

WebSocket 消息先经过模式过滤，再分到 presence/ack、delta、entity graph、model 四个处理单元。`CollabSession` 继续拥有版本和提交状态；不要在拆分文件后建立第二份状态。模型恢复期间需抑制自动发布，并保持 UUID 对齐。

`CAD_DB/access.cpp` 保留 SAT 文件与诊断测试；`storage/` 区分图保存、图恢复、历史保存/恢复、零件访问、delta Push/Pull 与几何属性编码。内部头文件声明共享的数据类型和函数，函数实现放在 cpp，避免把整套逻辑塞进头文件。所有旧公开函数签名保留。

直接 Neo4j、PostgreSQL 与 Bridge 仍是现有功能，不能因为默认采用 JSON Mode1 就判断它们为死代码。

`storage_bridge_service.cpp` 负责 HTTP 解析、连接生命周期与路由分发；`storage/storage_bridge_{projects,models,versions,delta}.cpp` 分别实现项目、模型写入、版本读取和增量接口。`storage_bridge_helpers.cpp` 维护查询与 SAT 转换，私有声明集中在 `storage_bridge_internal.h`。

`entity_graph_serializer.cpp` 负责 ACIS → JSON，`entity_graph_deserializer.cpp` 负责 JSON → ACIS。恢复保持“骨架、几何、拓扑”三阶段顺序，公共接口继续使用 `entity_graph_serializer.h`。

## 后端

| 目录/文件 | 职责 |
|---|---|
| `app/main.py` | 装配路由、异常处理、健康检查 |
| `app/lifecycle.py` | 初始化/关闭存储和日志 |
| `app/dependencies.py` | API 鉴权依赖 |
| `app/api/` | 项目、模型、JSON delta、实体、WebSocket 入口 |
| `app/services/` | 版本写入及事件构造 |
| `app/collaboration/` | 快照、提交校验和消息分发 |
| `app/crud.py` | 业务规则及存储适配选择 |
| `app/*store.py`、`storage_bridge.py` | Neo4j 或 C++ Bridge 数据访问 |
| `app/neo4j_webui/` | 管理界面 API，按项目、版本、图与管理操作拆分 |
| `app/static/webui/` | 独立 HTML、CSS 与按功能拆分的 JS |

保留 HTTP 与 WS 路径、请求/响应字段、Mode0/Mode1 隔离、base_version 冲突与 UUID delta 语义。固定路径需先于 `/{version}` 注册，防止 `versions`、`diff` 被当成版本号。

## 部署

`deploy/start_dbcad_fullstack.ps1` 保留参数和启动编排。`deploy/lib/` 分别维护文件工具、配置、运行时探测、Docker、进程与打包。打包必须同时复制 `lib/` 与后端 `app/static/`；不能依赖源码目录中的资源。

## 开发约定

按职责拆分，不机械按行数拆分；新功能优先进入相应模块。注释解释约束、取舍和危险边界，避免逐句翻译代码。清理代码必须核对菜单、脚本、测试与历史兼容路径。遵循 `.editorconfig`、`.clang-format`；Python 使用 Ruff 格式化。

参考：[C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) 的 F.2/F.3 与 SF 规则；[FastAPI Bigger Applications](https://fastapi.tiangolo.com/tutorial/bigger-applications/) 的 APIRouter 组织；[MSBuild 项目结构](https://learn.microsoft.com/en-us/cpp/build/reference/vcxproj-file-structure?view=msvc-170)。
