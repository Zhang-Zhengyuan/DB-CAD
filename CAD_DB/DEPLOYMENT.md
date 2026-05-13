# DBCAD 一键部署和启动

当前只保留一套部署入口：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
```

这个脚本会自动完成：

1. 查找最新的 `CAD_DB.exe`。
2. 调用 `windeployqt` 补齐 Qt 运行时 DLL。
3. 同步完整包到 `dist\DBCAD-fullstack-package`。
4. 写入客户端和后端配置。
5. 启动 C++ Storage Bridge、FastAPI 和桌面客户端。

## 前置条件

1. Neo4j 已启动，默认 Bolt 地址为 `127.0.0.1:7687`。
2. Qt 安装在 `D:\Qt\6.9.0\msvc2022_64`，或通过参数传入 `windeployqt.exe`。
3. Python 后端使用 `uv` 管理依赖；脚本找不到 `uv` 时会自动安装。

## 常用命令

启动完整前后端和客户端：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
```

只准备部署包，不启动进程：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -PrepareOnly
```

指定 Neo4j 密码：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -Neo4jPassword 12345678
```

强制使用某个 exe：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -ClientExePath ..\x64\Release\CAD_DB.exe
```

指定 `windeployqt`：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -WinDeployQtPath D:\Qt\6.9.0\msvc2022_64\bin\windeployqt.exe
```

只启动 Bridge 和 FastAPI，不打开客户端：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -SkipClient
```

停止由启动器拉起的进程：

```powershell
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

## 生成目录

统一包目录：

```text
dist\DBCAD-fullstack-package
```

关键文件：

```text
dist\DBCAD-fullstack-package\start_dbcad_fullstack.cmd
dist\DBCAD-fullstack-package\stop_dbcad_fullstack.cmd
dist\DBCAD-fullstack-package\client\CAD_DB.exe
dist\DBCAD-fullstack-package\server\app\main.py
dist\DBCAD-fullstack-package\server\bridge-bin\CAD_DB.exe
```

## 配置文件

客户端连接 FastAPI：

```text
dist\DBCAD-fullstack-package\client\fastapi_connect_info.conf
```

后端环境变量：

```text
dist\DBCAD-fullstack-package\server\.env
```

脚本会自动写入这些文件。需要改服务地址、端口或密码时，优先通过启动参数传入。

## 健康检查

启动后检查：

```powershell
Invoke-RestMethod http://127.0.0.1:8100/health
Invoke-RestMethod http://127.0.0.1:8000/health
```

FastAPI 文档：

```text
http://127.0.0.1:8000/docs
```

## 常见问题

如果提示缺少 Qt DLL，重新执行：

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd -PrepareOnly -WinDeployQtPath D:\Qt\6.9.0\msvc2022_64\bin\windeployqt.exe
```

如果提示 Neo4j 不可达，先确认：

```powershell
Test-NetConnection 127.0.0.1 -Port 7687
```

如果 8000 或 8100 被占用，先执行：

```powershell
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```
