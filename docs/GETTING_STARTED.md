# 新成员复现与部署

当前支持 Windows x64 桌面客户端。后端为 FastAPI，数据库为 Neo4j。协作只使用 Mode1 JSON + SAT 备份；单机 Neo4j、ACIS 文件和 PostgreSQL 菜单保留。

## 1. 获取源码与工具

```powershell
git clone --branch zhangzhengyuan-dev https://github.com/Zhang-Zhengyuan/DB-CAD.git
cd DB-CAD
```

安装 Visual Studio 2022 的 C++ 桌面开发工作负载与 Windows SDK、Qt 6.8.3 MSVC2022 x64（含 Qt WebSockets）、Qt VS Tools、Python 3.11+、uv、Docker Desktop。启动 Docker Desktop 的 Linux 容器引擎。

Qt VS Tools 必须注册 Qt kit，或显式提供 Qt 安装路径与 QtMsBuild 路径。QtMsBuild 指包含 `qt.props` 和 `qt.targets` 的目录。

## 2. 准备依赖包

向维护者取得有权使用的 ACIS SDK 和项目依赖包，按 [依赖说明](DEPENDENCIES.md) 放到 `_deps/`。这些文件不进入 Git。缺少授权 SDK 时只能运行后端测试，不能完整构建 CAD 客户端。

```powershell
$env:DBCAD_QT_ROOT = 'C:\Qt\6.8.3\msvc2022_64' # 改为自己的 kit
$env:DBCAD_POSTGRES_INCLUDE = 'C:\vcpkg\installed\x64-windows\include'
$env:DBCAD_RUNTIME_DIR = 'C:\vcpkg\installed\x64-windows\bin'
.\scripts\check-environment.ps1
```

也可复制 `build.local.props.example` 为 `build.local.props`，设置 Qt、依赖和 PostgreSQL 头文件路径；该本机文件已忽略。运行时 DLL 通过 `DBCAD_RUNTIME_DIR` 或依赖包中的 `bin/` 提供。

## 3. 先验证后端

```powershell
Push-Location CAD_DB\backend
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
Pop-Location
```

该命令不要求 Neo4j、ACIS 或已启动的服务。`uv.lock` 是依赖版本的事实来源，部署也使用 `--frozen`。更新依赖时同时更新锁文件并重新验证。

## 4. 构建客户端

```powershell
.\scripts\build.ps1 -Configuration Release
```

脚本自动定位 Visual Studio 的 MSBuild，构建唯一的 `CAD_DB/CAD_DB.vcxproj`，输出到 `x64/Release/`，并暂存非 Qt 运行时 DLL。也支持 `-QtRoot`、`-QtMsBuildPath`、`-DepsRoot`、`-PostgresInclude`、`-RuntimeDir` 参数。Debug 必须配套 Debug SDK/运行时。构建后会从依赖包或 RuntimeDir 复制非 Qt DLL；部署使用对应 kit 的 windeployqt。

直接在 Visual Studio 构建项目也使用仓库根目录的 `x64/<Configuration>/`。双客户端启动器自动打包该目录中的最新构建，无需手动复制 exe；可传 `-Configuration Debug` 或 `-Configuration Release` 固定配置。

## 5. 生成并启动部署包

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -WinDeployQtPath "$env:DBCAD_QT_ROOT\bin\windeployqt.exe"
```

首次创建被 Git 忽略的 `CAD_DB/deploy/dbcad.local.env`，生成密码；创建 Neo4j 5.15 + APOC；将源码、客户端、配置和启动辅助模块放入 `dist/DBCAD-fullstack-package/`；启动后端和一个客户端。不要上传生成配置、部署包或数据库数据。

服务检查：`http://127.0.0.1:8000/health`、`http://127.0.0.1:8000/docs`、`http://127.0.0.1:8000/webui`。管理界面要求配置中的 API 密码。

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -PrepareOnly # 只打包
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -SkipClient  # 服务端
.\CAD_DB\deploy\start_dbcad_two_clients.cmd             # 两个客户端
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

Mode0 和 C++ Bridge 已移除；不再使用 `-LegacyBridgeMode`。

**重置会删除数据。** 正常启动不需要 `-ResetNeo4jDockerData`。备份后才能执行重置脚本。Neo4j 初始化后的密码变更需按部署文档处理。

## 6. 功能验收

两客户端打开同名项目；A 新建几何体并 Push，B Pull 后显示相同模型；B 修改并 Push，A Pull 验证。再检查删除、重连、本地未提交编辑、过期版本冲突、JSON 重建失败时的 SAT 回退、撤销/重做与文件存取。维护者记录实际结果，不能以编译通过替代完整 GUI 验收。

跨机器时在客户端连接设置中填服务器 IP 和 API 密码，确认 8000 端口可达。正式部署需要按组织网络环境设置防火墙和访问控制。

## 常见失败

| 症状 | 处理 |
|---|---|
| QtMsBuild 找不到 | 安装 Qt VS Tools 或设置 `QtMsBuild` |
| `libpq-fe.h` 找不到 | 设置 `DBCAD_POSTGRES_INCLUDE` |
| exe 缺 DLL | 提供完整依赖运行时并重新构建、打包 |
| Docker engine 未启动 | 启动 Docker Desktop 并确认 `docker info` |
| Neo4j/APOC 检查失败 | 检查镜像下载、容器日志和端口；不要盲目重置 |
| exe 比源码旧 | 重新构建；启动器会拒绝使用旧程序 |

详细说明见 [依赖](DEPENDENCIES.md)、[代码架构](ARCHITECTURE.md)、[部署参数](../CAD_DB/DEPLOYMENT.md)。
