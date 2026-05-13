# DBCAD 启动参数参考

推荐入口：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.ps1
```

## 参数

`-FastApiHost`

FastAPI 监听地址，默认 `0.0.0.0`。

`-FastApiPort`

FastAPI 端口，默认 `8000`。

`-BridgeHost`

C++ Storage Bridge 监听地址，默认 `127.0.0.1`。

`-BridgePort`

C++ Storage Bridge 端口，默认 `8100`。

`-Neo4jHost`

Neo4j Bolt 主机，默认从配置文件或 `127.0.0.1` 推断。

`-Neo4jPort`

Neo4j Bolt 端口，默认 `7687`。

`-Neo4jUser`

Neo4j 用户名，默认 `neo4j`。

`-Neo4jPassword`

Neo4j 密码。脚本会优先读取最新构建目录旁边的 `neo4j_connect_info.conf` 或 `.config`，也可以通过此参数覆盖。

`-ApiPassword`

FastAPI 可选访问密码。为空表示不校验。

`-ClientExePath`

指定要部署的 `CAD_DB.exe`。不传时，脚本优先使用真实构建输出目录中的最新 exe。

`-WinDeployQtPath`

指定 `windeployqt.exe`。不传时会自动查找 `D:\Qt`、`C:\Qt`、PATH 和 qmake 所在目录。

`-PackageRoot`

指定部署包输出目录，默认 `dist\DBCAD-fullstack-package`。

`-PrepareOnly`

只同步部署包和写配置，不启动任何服务。

`-SkipClient`

只启动 Bridge 和 FastAPI，不打开桌面客户端。

`-SkipDependencySync`

跳过 `uv sync`，适合依赖已经安装好的快速重启。

`-SkipNeo4jPortCheck`

跳过 Neo4j 端口预检查。仅在特殊网络代理或远程部署时使用。

`-KeepExistingServices`

如果 8000/8100 已有健康服务，复用现有服务。默认会要求先停止旧服务，避免误用旧版本。

## 推荐流程

1. 构建 `CAD_DB.exe`。
2. 启动 Neo4j。
3. 执行：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -Neo4jPassword 12345678
```

4. 两台客户端打开同一个 FastAPI 项目，验证实时协作。
