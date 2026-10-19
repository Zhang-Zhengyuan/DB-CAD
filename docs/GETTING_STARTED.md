# 新成员复现与部署

v3 架构：浏览器是唯一客户端（React + three.js），几何计算在服务端 ACIS worker，存储为 Neo4j + 内容寻址资产库。Windows x64 开发环境；运行链路对平台无要求（容器 + HTTP）。

## 1. 获取源码与工具

```powershell
git clone https://github.com/Zhang-Zhengyuan/DB-CAD.git
cd DB-CAD
```

安装 Visual Studio 2022 的 C++ 桌面开发工作负载与 Windows SDK、Python 3.11+、uv、Node.js 20+、Docker Desktop（Linux 容器引擎）。无 Qt 依赖（P7 已移除 Qt 客户端）。

## 2. 准备依赖包

向维护者取得有权使用的 ACIS SDK 和项目依赖包，按 [依赖说明](DEPENDENCIES.md) 放到 `_deps/`。这些文件不进入 Git。缺少授权 SDK 时只能运行后端测试，不能构建 worker（STEP 导入还需要 Spatial InterOp 组件）。

```powershell
.\scripts\check-environment.ps1
```

也可复制 `build.local.props.example` 为 `build.local.props` 设置依赖根路径；该本机文件已忽略。运行时 DLL 由依赖包中的 `bin/` 提供（构建后自动暂存到 `x64/<Configuration>/`）。

## 3. 先验证后端

```powershell
Push-Location CAD_DB\backend
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
Pop-Location
```

该命令不要求 Neo4j、ACIS 或已启动的服务。`uv.lock` 是依赖版本的事实来源，部署也使用 `--frozen`。更新依赖时同时更新锁文件并重新验证。

## 4. 构建 worker 与 Web 客户端

```powershell
.\scripts\build.ps1 -Configuration Release   # CAD_DB.sln → dbcad_worker.exe（含 ACIS 运行时暂存）
cd web ; npm install ; npm run build          # 可选：本地开发用 npm run dev
```

脚本自动定位 Visual Studio 的 MSBuild，输出到 `x64/Release/`。支持 `-DepsRoot`、`-RuntimeDir` 参数。Debug 必须配套 Debug SDK/运行时。生产部署直接使用 FastAPI 托管的 `/web`，本地前端开发才需要 Vite。

## 5. 启动全栈

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

启动链路：Neo4j 容器（5.15 + APOC）→ FastAPI（端口 8000）→ 浏览器打开 `http://127.0.0.1:8000/web/`。worker 由 FastAPI 按需拉起，无需手工启动。首次运行自动生成被 Git 忽略的 `CAD_DB/deploy/dbcad.local.env` 与密码；不要上传生成配置或数据库数据。

服务检查：`/health`、`/docs`、`/web`（CAD 客户端）、`/webui`（Neo4j 管理台，要求 API 密码）。

**重置会删除数据。** 正常启动不需要 `reset_dbcad_deployment.cmd`。备份后才能执行重置。Neo4j 初始化后的密码变更需按[部署说明](../CAD_DB/DEPLOYMENT.md)处理。

## 6. 功能验收

浏览器开两个标签页登录同名项目；A 插入几何体并等待版本提交，B 刷新后看到相同模型；B 执行修改命令，A 侧收到版本事件提示。再检查：布尔/删除/撤销重做、STEP 导入（装配体组件树）、实体查询 `/api/v1/projects/{id}/entities`、过期 base_version 的 409 冲突提示、SAT/GLB/STL 导出。维护者记录实际结果，不能以编译通过替代完整验收。

跨机器访问：服务器侧放行 8000 端口，客户端浏览器直接访问；正式部署按组织网络环境设置防火墙和访问控制。

## 常见失败

| 症状 | 处理 |
|---|---|
| exe 缺 DLL | 提供完整依赖运行时并重新构建 |
| Docker engine 未启动 | 启动 Docker Desktop 并确认 `docker info` |
| Neo4j/APOC 检查失败 | 检查镜像下载、容器日志和端口；不要盲目重置 |
| worker 拉起失败 | 看 FastAPI 日志中 worker 路径与 ACIS DLL 是否就位 |
| 401 Invalid API password | 使用 `dbcad.local.env` 生成的密码；测试环境置空 `CAD_DB_API_PASSWORD` |

详细说明见 [依赖](DEPENDENCIES.md)、[代码架构](ARCHITECTURE.md)、[部署参数](../CAD_DB/DEPLOYMENT.md)。
