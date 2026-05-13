# DBCAD 启动参数参考

推荐入口：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.ps1
```

双客户端入口：

```powershell
.\CAD_DB\deploy\start_dbcad_two_clients.ps1
```

## 核心参数

`-ConfigPath`

指定统一配置文件。默认是 `CAD_DB\deploy\dbcad.local.env`。

`-ResetNeo4jDockerData`

删除并重建本地 Neo4j Docker 容器和 `neo4j-runtime` 数据。修改 `DBCAD_PASSWORD` 后需要使用。

`-SkipNeo4jDocker`

不创建、不启动 Docker 容器。适合使用外部 Neo4j。

`-FastApiHost` / `-FastApiPort`

FastAPI 监听地址和端口，默认来自 `dbcad.local.env`。

`-BridgeHost` / `-BridgePort`

C++ Storage Bridge 监听地址和端口，默认来自 `dbcad.local.env`。

`-Neo4jHost` / `-Neo4jPort`

Neo4j Bolt 地址，默认来自 `dbcad.local.env`。

`-Neo4jUser` / `-Neo4jPassword`

Neo4j 认证信息。默认使用 `DBCAD_NEO4J_USER` 和统一的 `DBCAD_PASSWORD`。命令行参数优先级高于配置文件，通常不需要传。

`-ApiPassword`

FastAPI 访问密码。默认使用统一的 `DBCAD_PASSWORD`。不建议传空值；空 FastAPI 密码会关闭鉴权，导致客户端连接测试无论填什么都通过。

`-ClientExePath`

指定要部署的 `CAD_DB.exe`。不传时，脚本优先使用真实构建输出目录中的最新 exe。

`-WinDeployQtPath`

指定 `windeployqt.exe`。不传时自动查找 `D:\Qt`、`C:\Qt`、PATH 和 qmake 所在目录。

`-PackageRoot`

指定部署包输出目录，默认 `dist\DBCAD-fullstack-package`。

`-PrepareOnly`

只同步部署包和写配置，不启动 Docker、Bridge、FastAPI 或客户端。

`-SkipClient`

只启动 Docker、Bridge 和 FastAPI，不打开桌面客户端。

`-ClientCount`

启动客户端实例数量，默认 `1`。传 `-ClientCount 2` 等价于使用 `start_dbcad_two_clients.ps1`。

`-SkipDependencySync`

跳过 `uv sync`，适合依赖已经安装好的快速重启。

`-KeepExistingServices`

如果 8000/8100 已有健康服务，复用现有服务。默认要求先停止旧服务，避免误用旧版本。

## 推荐流程

1. 构建 `CAD_DB.exe`。
2. 执行一次启动器，让它创建或迁移 `CAD_DB\deploy\dbcad.local.env`。
3. 确认配置里只有一个 `DBCAD_PASSWORD`，不要再维护 `DBCAD_NEO4J_PASSWORD` 或 `DBCAD_API_PASSWORD`。
4. 重建本地 Neo4j 数据并启动：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -ResetNeo4jDockerData
```

5. 两台客户端打开同一个 FastAPI 项目，验证实时协作。
