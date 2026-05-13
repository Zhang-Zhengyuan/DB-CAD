# DBCAD Deploy

唯一启动入口：

```powershell
.\deploy\start_dbcad_fullstack.cmd
```

双客户端入口：

```powershell
.\deploy\start_dbcad_two_clients.cmd
```

停止入口：

```powershell
.\deploy\stop_dbcad_fullstack.cmd
```

清理旧部署、旧 Neo4j 数据和旧密码文件：

```powershell
.\deploy\reset_dbcad_deployment.cmd
```

唯一可编辑密码源是：

```text
.\deploy\dbcad.local.env
```

详细使用说明见仓库根文档：

```text
CAD_DB\DEPLOYMENT.md
CAD_DB\STARTUP_CONFIG.md
```
