# DBCAD 交付包启动配置说明

本目录用于直接部署服务端（Bridge + FastAPI）。

## 1. 配置文件

编辑同目录 `.env`：

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

重点：`CAD_DB_NEO4J_PASSWORD` 必须与 Neo4j 实际密码一致。

快速更新密码：

```powershell
.\set_neo4j_password.ps1 -Neo4jPassword your_password
```

## 2. 启动命令

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\start_fullstack_oneclick.ps1 -FastApiHost 0.0.0.0 -FastApiPort 8000
```

可选加速参数：

```powershell
.\start_fullstack_oneclick.ps1 -SkipDependencySync
```

## 3. 健康检查

```powershell
.\check_backend.ps1 -BaseUrl http://127.0.0.1:8000 -BridgeUrl http://127.0.0.1:8100
```

成功后可访问：

1. http://127.0.0.1:8100/health
2. http://127.0.0.1:8000/health
3. http://127.0.0.1:8000/docs

## 4. 常见问题

1. unauthorized：Neo4j 密码错误，更新 `.env` 后重启。
2. path not found：请在本目录执行，或用脚本绝对路径。
3. 端口冲突：更换 `-FastApiPort` 或结束旧进程。
