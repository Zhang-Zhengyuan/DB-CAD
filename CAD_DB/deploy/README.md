# DBCAD Deploy

主入口、密码管理、Docker Neo4j 部署的**完整说明**见：

- [`../DEPLOYMENT.md`](../DEPLOYMENT.md)

> 2026-10-18（架构 v2 清理）：实验性 PostgreSQL 直存 demo（`pg_demo`、`pg_store`、`build_pg_demo.ps1`、`start_pg_with_pgadmin.ps1`、`start_dbcad_with_pg.*` 及 GUI 的 PostgreSQL 菜单）已整体移除，留档见 git tag `legacy/pre-architecture-v2`。当前部署只有一条链路：Neo4j 容器 + FastAPI + 客户端。将来若做关系库后端，按 `docs/ARCHITECTURE.md` §4 的存储抽象实现，不再走客户端直连。
