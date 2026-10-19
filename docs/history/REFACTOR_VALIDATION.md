# 本地重构与验证记录

验证日期：2026-10-09。工作分支：`zhangzhengyuan-dev`。本记录描述本地工作区；未执行 commit、push、PR 或分支历史修改，GitHub CI 也尚未运行。

## 改动与兼容边界

- MainWindow 按连接、协作、消息、编辑、文件等职责拆分到 `CAD_DB/ui/`；原公开成员接口保留。
- `access.cpp` 按图、历史、delta 和几何编解码拆分到 `CAD_DB/storage/`；共享状态保持一个实例。
- Storage Bridge 拆分 HTTP 编排、项目、模型、版本、delta 和查询/SAT 辅助模块；JSON 序列化与反序列化分开。
- FastAPI 按 API、业务服务、协作协议、生命周期拆分；Neo4j 管理 API 与 HTML/CSS/JS 分离，静态资源随部署包复制。
- 删除未被引用的 SQLAlchemy 原型、重复项目文件/pytest 配置、未使用几何 DTO 与已备份的运行日志；保留 PostgreSQL、直接 Neo4j、Mode0、Mode1 和教学入口。
- 统一构建入口，依赖路径支持配置；部署使用锁文件、检查打包工具退出码，复制独立运行需要的脚本和资源；补充上手、依赖、架构、贡献与后端 CI 文件。

真实数据库检查中修复了版本 DTO 不匹配、固定路由被版本参数路由遮蔽、实体存储初始化/闭包变量、Neo4j 结果读取和关系属性编码问题。这些修复与单纯文件搬移不同，PR 应说明其行为影响。

重构前已有未提交修改已保留。备份位于仓库外 `D:/THU_Things/DBCAD/current/refactor-backup-20261009/`，包括 `source-before.zip` 与 `pre-refactor.patch`。

## 已执行验证

| 检查 | 结果及范围 |
|---|---|
| 后端基线 | 29 项通过，1 项需真实服务的集成测试未运行 |
| 重构后测试 | 32 项通过，1 项 integration 排除；新增路由、静态资源、存储 DTO 回归检查 |
| Python 静态检查 | `ruff check CAD_DB/backend/app --select F,E9` 通过 |
| 锁文件 | `uv lock --check` 通过；部署/测试采用 `--frozen` |
| JS 与 PowerShell | 四份 JS 的 `node --check`，部署/构建脚本解析通过 |
| 教学补丁 | 三份当前补丁在 `CAD_DB` 目录运行 `git apply --check` 通过 |
| C++ 构建 | Windows x64 Debug、Release 编译通过；现有 ACIS 导入/链接警告仍存在 |
| 干净源码 | 排除 SDK、.venv、构建/部署输出，从源码导出目录构建 Release 并新建后端环境测试通过；使用外部授权 SDK 与明确的依赖路径 |
| 部署包 | `-PrepareOnly` 与 windeployqt 通过；包含客户端 DLL/Qt 插件、后端 app/static、锁文件、lib 辅助脚本 |
| Release 运行冒烟 | 客户端进程可启动；C++ Bridge `/health` 返回 `status=ok`。未操作 GUI 几何功能 |
| 真实 Neo4j | 在独立测试数据库检查项目创建、Mode0 保存/查询/版本列表、过期版本 409、JSON delta 新增/读取/删除、两种模式 WS presence/ping、WebUI 与静态资源；非空实体图含关系属性可保存并读回 |
| Git 差异 | `git diff --check` 通过；项目清单无缺失源码引用 |

第一轮真实 Neo4j 检查使用专用容器 `dbcad-refactor-audit`，Bolt/HTTP 端口 17687/17474，服务端口 18000；没有重置用户数据库。第一轮独立包重启曾受 Docker Desktop 的 `dockerInference` 文件访问错误影响。Docker 恢复后已完成源码和独立包的双客户端全栈启动，详见下方追加验证。

工具环境：Visual Studio 2022/v143、Qt 6.8.3 MSVC2022 x64、Python 3.11、uv 与本机授权 ACIS 依赖包。商业/定制 SDK 的准确发行号仍需维护者确认并登记，不能只靠目录名推断。

## 合并前仍需验收

- 默认参数在团队第二台机器运行 Release 独立部署包；本机独立端口的配置、密码、启动和停止流程已通过。
- 双客户端检查创建几何、修改、删除、Push/Pull、重连、未提交修改保护与过期版本冲突；检查 SAT Mode0、文件存取、撤销/重做与 PostgreSQL 实验入口。
- 旧 `/entities` 存储仍按 `local_id` 共享实体节点；历史版本不可变性需要专门验证。本轮保留旧数据模型，未宣称已解决这项语义风险。默认 JSON delta 使用独立存储实现。
- 图恢复和历史保存中的部分函数仍然较长，涉及 ACIS 阶段顺序和状态；后续应在几何回归样本齐备后继续提取子步骤，不能宣称已删除所有死代码或保证全部功能零回归。
- 向新成员提供有权使用、版本一致的依赖包并在第二台机器复现；当前“干净源码”验证仍在维护者机器完成。

## master 的历史问题

本次检查发现 `origin/master` 与 `zhangzhengyuan-dev` 没有共同祖先。普通 GitHub PR 不能直接按当前分支历史比较合并。推送前请先确认仓库历史处理方案：通常可从最新 master 创建集成分支，把已审查的源码改动迁移过去，再 PR 回 master；涉及历史合并或替换应由维护者明确决定。本轮没有自行处理历史。

## 双客户端启动路径修复

构建输出明确固定到仓库根目录 `x64/Debug`、`x64/Release`，中间文件位于 `x64/obj/CAD_DB/<Configuration>`；直接构建 vcxproj 和解决方案构建使用同一路径。源码启动器仅自动选择这两个输出目录，支持 `-Configuration Auto/Debug/Release` 和显式 `-ClientExePath`。独立部署包使用自己的 client exe，不再从附近源码或旧 dist 选择。

`start_dbcad_two_clients.cmd` 直接调用统一启动器并设置 `ClientCount=2`，保留参数与失败退出码；ps1 入口兼容当前 PowerShell 宿主。修复 Windows PowerShell 的源码时间筛选，脚本改动无需重编 C++。

本次通过真实 cmd 入口执行 `-PrepareOnly -Configuration Debug`，确认 Source executable 是仓库根目录的 Debug 构建；源 exe、包 client exe 和 bridge exe 哈希一致，39 个后端源码/静态资源与全部 6 个辅助模块哈希一致。独立包 cmd 入口、PowerShell 7 ps1 入口及失败退出码检查通过。7 项 exe 选择回归在 Windows PowerShell 5.1 和 PowerShell 7 下通过，可运行 `scripts/tests/test-deploy-resolution.ps1` 复查。Debug 和 Release 均重新构建通过。

Docker 恢复后，使用新测试容器 `dbcad-launch-audit`、Bolt/HTTP 17687/17474、FastAPI 18002，实际执行三个完整启动场景：源码 cmd 明确 Debug、源码 cmd 默认 Auto（选中最新 Release）、独立部署包 cmd（使用包内 exe）。三次 Neo4j/APOC 检查及 FastAPI 启动正常；每次两份客户端均出现 `DBCAD` 窗口，进程响应正常。Debug、Release 与独立包分别检查鉴权、项目与 Mode0 版本读写、过期版本 409、JSON Push/Pull/删除与冲突、两连接 WS 通知、Mode0/Mode1 ping，以及 WebUI/静态资源。

停止脚本正确结束此次启动的客户端与后端。测试数据库位于仓库外，未重置用户数据库。保留测试数据与日志，停止专用容器。上述 GUI 检查仅确认启动和窗口响应，没有替代人工几何建模、渲染和 GUI Push/Pull 验收。

## 默认部署 DLL 故障与修复

前述独立测试显式传入了 windeployqt，未覆盖双击默认入口的工具探测路径。用户随后报告 DLL 弹窗。默认包检查确认：选中 Release exe，而 client 目录仅有 Debug Qt DLL 和插件，缺少 exe 实际导入的 Qt6Core.dll 等 Release 文件。原因是默认工具探测未读取 E 盘构建 kit，缺工具时仅警告继续；包内又保留了旧 DLL。此前“启动正常”的结论仅适用于显式配置的测试包，对默认部署的验证不充分。

修复后从 Qt VS Tools 的构建元数据自动定位对应 kit；PE 导入直接决定 Debug/Release；缺少打包工具即失败；源码打包清理包内旧二进制和 Qt 插件，避免混用；增加 x64 PE 导入、DLL 传递依赖与插件检查，缺失时脚本报具体名称；客户端启动后检查提前退出，创建独立日志目录。准备好的独立包无需安装 Qt 开发工具即可运行。

在用户默认配置、默认 dist 目录实际运行无参数 `start_dbcad_two_clients.cmd`：自动选择 Release，调用 Qt 6.8.3 的 windeployqt，24 个运行时二进制依赖检查通过；两份 DBCAD 窗口响应正常，日志出现 OpenGL paintGL 绘制；8000 服务健康、鉴权概览、WebUI 和静态资源检查通过。Debug 包也未传 Qt 工具路径而自动打包通过。新增 `scripts/tests/test-client-runtime.ps1` 检查 PE 模式、残留 Debug DLL、kit 定位和缺 DLL 失败分支，在 Windows PowerShell 5.1 运行通过。

`neo4j-apoc` 为原配置的容器名称，镜像为 neo4j:5.15；它被保留并复用，未删除或重置用户数据库。名称不表示客户端版本；后端 health 验证 collaboration_mode=direct_json。默认测试只做数据库读取，没有写入验收数据。
