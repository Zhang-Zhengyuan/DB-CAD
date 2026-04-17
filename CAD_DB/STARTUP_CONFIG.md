# DBCAD 配置与启动指南（develop417）

本说明用于开发机和交付环境统一启动 DBCAD 全栈：

- C++ Storage Bridge（CAD_DB.exe）
- FastAPI 后端（uv + uvicorn）
- Neo4j（可用 Docker）

---

## 1. 目录与角色

1. 源码部署脚本目录：`CAD_DB/deploy/server`
2. 交付包服务端目录：`dist/DBCAD-fullstack-package/server`
3. 交付包客户端目录：`dist/DBCAD-fullstack-package/client`

建议优先使用交付包目录启动服务，便于验收和迁移。

---

## 2. 前置条件

1. Windows PowerShell 5.1+。
2. Neo4j 可访问（本机或远程），默认 Bolt 端口 7687。
3. 网络放通 8000（FastAPI）与 8100（Bridge，可按需调整）。
4. 首次启动若未安装 uv，脚本会自动安装。

---

## 3. 环境配置（.env）

服务端目录中的 `.env` 是核心配置文件（示例值如下）：

```env
CAD_DB_STORAGE_BACKEND=neo4j
CAD_DB_STORAGE_BRIDGE_URL=http://127.0.0.1:8100
CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS=15
CAD_DB_NEO4J_URI=bolt://127.0.0.1:7687
CAD_DB_NEO4J_USER=neo4j
CAD_DB_NEO4J_PASSWORD=your_password
CAD_DB_NEO4J_DATABASE=neo4j
CAD_DB_API_PASSWORD=
```

字段说明：

1. `CAD_DB_STORAGE_BACKEND`：当前仅支持 `neo4j`。
2. `CAD_DB_STORAGE_BRIDGE_URL`：FastAPI 调用 Bridge 的地址。
3. `CAD_DB_NEO4J_URI`：Neo4j Bolt 地址。
4. `CAD_DB_NEO4J_PASSWORD`：最常见故障点，必须与 Neo4j 实际密码一致。
5. `CAD_DB_API_PASSWORD`：可选 API 访问口令，留空表示不校验。

快速改密码（只改 `.env` 文件）：

```powershell
.\set_neo4j_password.ps1 -Neo4jPassword your_password
```

---

## 4. 一键启动（推荐）

在 `dist/DBCAD-fullstack-package/server` 执行：

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\start_fullstack_oneclick.ps1 -FastApiHost 0.0.0.0 -FastApiPort 8000
```

若你希望显式传入 Neo4j 密码（覆盖 .env）：

```powershell
.\start_fullstack_oneclick.ps1 -Neo4jPassword your_password -FastApiHost 0.0.0.0 -FastApiPort 8000
```

启动成功后将输出：

1. Bridge 健康地址。
2. FastAPI 健康地址。
3. Swagger 文档地址。
4. Bridge/FastAPI 进程 PID。

---

## 5. 可选参数（已优化）

`start_fullstack_oneclick.ps1` 新增并支持透传到后端启动脚本：

1. `-SkipDependencySync`：跳过 `uv sync`，适合依赖已就绪时快速重启。
2. `-SkipBridgeHealthCheck`：跳过后端启动前的 Bridge 检测（仅建议排障时使用）。

示例：

```powershell
.\start_fullstack_oneclick.ps1 -SkipDependencySync
```

---

## 6. 健康检查与验收

官方检查：

```powershell
.\check_backend.ps1 -BaseUrl http://127.0.0.1:8000 -BridgeUrl http://127.0.0.1:8100
```

可手动访问：

1. `http://127.0.0.1:8100/health`
2. `http://127.0.0.1:8000/health`
3. `http://127.0.0.1:8000/docs`

---

## 7. 客户端配置

客户端目录：`dist/DBCAD-fullstack-package/client`

1. 修改 `fastapi_connect_info.conf` 第一行为服务端地址，例如：`http://192.168.1.100:8000`
2. 启动 `CAD_DB.exe`

---

## 8. 常见问题

1. 报错 `Neo4j数据库连接失败 / unauthorized`：`.env` 中 `CAD_DB_NEO4J_PASSWORD` 不正确。
2. 报错找不到脚本：请使用脚本绝对路径启动，或先 `Set-Location` 到脚本所在目录。
3. Bridge 配置是 `0.0.0.0` 时健康检查失败：新版脚本已自动将本地探测地址回落为 `127.0.0.1`。
4. FastAPI 端口已被占用：结束旧进程后重启，或改用 `-FastApiPort`。

---

## 9. 停止服务

如果前台窗口关闭即退出可忽略本节；若需主动结束：

```powershell
Get-Process CAD_DB,python -ErrorAction SilentlyContinue | Stop-Process -Force
```

如果需要查看错误日志：

1. 源码模式：`CAD_DB/backend/logs/backend-error.log`
2. 交付包模式：`dist/DBCAD-fullstack-package/server/logs/backend-error.log`
