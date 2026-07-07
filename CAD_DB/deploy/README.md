# DBCAD Deploy

主入口（Neo4j + FastAPI 协作模式）：

```powershell
.\deploy\start_dbcad_fullstack.cmd
```

主入口 + **PostgreSQL 旁路 demo**（同时拉 Postgres、pgAdmin、并启动 DBCAD GUI，新增"PostgreSQL"顶级菜单）：

```powershell
.\deploy\start_dbcad_with_pg.ps1
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

---

## PostgreSQL 直存 SAT 文本 mini-demo（实验性）

这是一个**完全独立**于主 Neo4j 一键部署的可选小 demo，用于向老师演示"直接用关系数据库存 SAT 文本、绕开 Neo4j"的可行性。**它不会启动主流程、不影响现有 Neo4j 一键部署**。

文件清单：

| 文件 | 作用 |
|---|---|
| `CAD_DB/pg_demo.cpp` | demo 命令行入口 |
| `CAD_DB/pg_store.hxx` / `pg_store.cpp` | **libpq**（不是 libpqxx）直连的小封装 |
| `CAD_DB/deploy/build_pg_demo.ps1` | 单独编译 `pg_demo.exe`（vcpkg `x64-windows` 动态 triplet） |
| `CAD_DB/deploy/_build_pg_demo.cmd` | 同上，.cmd 版（绕开 PS 转义坑） |
| `CAD_DB/deploy/start_pg_demo.ps1` | 仅 Postgres 容器（命令行 demo 用） |
| `CAD_DB/deploy/start_pg_with_pgadmin.ps1` | Postgres **+ pgAdmin** 一起拉（GUI 入口用） |
| `CAD_DB/deploy/start_dbcad_with_pg.ps1` | **一键：启 PG + pgAdmin + 写 conf + init schema + 启动 DBCAD GUI** |
| `CAD_DB/pg_connect_info.conf` | 客户端/GUI 读的连接配置（host/port/user/password/dbname 5 行） |
| `CAD_DB/mainwindow.cpp` / `.h` | 新增顶级菜单 **"PostgreSQL(P)"**，4 项：保存当前模型 / 从 PG 加载 / 列出已存零件 / 设置连接信息 |

> **注**：最初规划用 `libpqxx`，但 vcpkg 给出的 libpqxx 8.0.1（动态 + Meson 编译）在本机链接时会产生 `STATUS_STACK_BUFFER_OVERRUN` 之类 CRT/堆栈保护相关的崩溃，定位耗时；所以最终 demo 直接调 `libpq`（`libpq-fe.h`）。**功能层面等价，依赖更小**——`libpq.dll` + `libssl-3-x64.dll` + `libcrypto-3-x64.dll` + `z.dll` + `lz4.dll` 共 5 个 DLL 即可。

一键跑通：

```powershell
cd CAD_DB

# 1. 启动 demo 专用 Postgres 容器（不影响 Neo4j）
.\deploy\start_pg_demo.ps1

# 2. 编译 pg_demo.exe（仅这一次，后续修改 .cpp 需重跑）
cmd /c "deploy\_try_compile.cmd"

# 3. 初始化 schema（只需跑一次；表名 dbcad_pg_demo）
$env:DBCAD_PG_PASSWORD='<start_pg_demo.ps1 启动时打印的密码>'
.\x64\Demo\pg_demo.exe init

# 4. 保存一个 SAT 文件到 DB
#    （CLI 是 位置参数：save <name> <sat-file>）
#    用任意 SAT 或纯文本都行——这个 demo 只演示"按字节存/取"。
.\x64\Demo\pg_demo.exe save demo_part_1 path\to\some_model.sat

# 5. 取出来到另一个文件
.\x64\Demo\pg_demo.exe load demo_part_1 _roundtrip.sat

# 6. 比对 hash，确认 DB 真的完整存了（必须一致）
Get-FileHash path\to\some_model.sat
Get-FileHash _roundtrip.sat

# 7. 看库里几行
.\x64\Demo\pg_demo.exe count

# 想清理：
.\x64\Demo\pg_demo.exe delete demo_part_1

# 关掉 demo Postgres：
.\deploy\start_pg_demo.ps1 -Stop
```

数据库 schema（`pg_demo init` 会自动创建）：

```sql
CREATE TABLE IF NOT EXISTS dbcad_pg_demo (
  id           BIGSERIAL PRIMARY KEY,
  name         TEXT NOT NULL UNIQUE,
  sat_text     TEXT NOT NULL,
  byte_size    INTEGER NOT NULL,
  source_label TEXT NOT NULL DEFAULT 'unknown',
  created_at   TIMESTAMPTZ NOT NULL DEFAULT NOW(),
  updated_at   TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
```

### 主流程是否受影响？

**几乎是零侵入。** `pg_demo.cpp`、`pg_store.*`、`build_pg_demo.ps1`、`start_pg_demo.ps1`、`start_pg_with_pgadmin.ps1`、`start_dbcad_with_pg.cmd` 都没有被加入 `CAD_DB.vcxproj`；`start_dbcad_fullstack.cmd` 默认行为完全没改。`dbcad.local.env` 与 `start_dbcad_fullstack.ps1` 里仅以"白名单字段"形式 append 了 `DBCAD_POSTGRES_*`，Neo4j / FastAPI / Bridge 路径代码字节级未触动。

唯一被修改的主流程文件是 `mainwindow.cpp` / `mainwindow.h` —— 加了一个独立的顶级菜单 **"PostgreSQL(P)"** 与 4 个 slot，slot 实现**完全不碰** `saveFile()`、`loadFile()`、`storage_bridge_service`、`backend_api_client`、FastAPI 路由：导出当前 ACIS 实体到临时 SAT → 用 `QProcess` 调 `pg_demo.exe save <name> <sat>`；反过来从 DB 拿 SAT → 临时文件 → 用 ACIS 模式 loadFile 的代码段读入。等于用 `pg_demo.exe` 当一个 QProcess 驱动的 SAT 字节存/取后端。

### 接入 GUI（一键体验）

```powershell
# 1. 拉起 Postgres + pgAdmin + 编译 pg_demo.exe + 写 conf + init schema + 启动 DBCAD
.\deploy\start_dbcad_with_pg.ps1
```

按提示操作后会自动：
1. `docker run -d --name dbcad-postgres-demo postgres:16`（监听 127.0.0.1:5432）
2. `docker run -d --name dbcad-pgadmin dpage/pgadmin4`（监听 127.0.0.1:5050，登录 `admin@dbcad.local / admin`）
3. 编译 `x64\Demo\pg_demo.exe`（如已存在则跳过）
4. 写入 `CAD_DB\pg_connect_info.conf`
5. `pg_demo.exe init` 建表
6. `start x64\Release\CAD_DB.exe`（或 `x64\Debug\CAD_DB.exe`，任一存在的版本）

进 DBCAD 后菜单里会多一项 **PostgreSQL(P)**，四个子项：
- **保存当前模型到 PostgreSQL** — 弹框输入零件名，把当前 ACIS 实体导出为临时 SAT，调 `pg_demo.exe save`，弹窗显示 `[OK] saved id=N bytes=M`。
- **从 PostgreSQL 加载到当前窗口...** — 弹框输入零件名，调 `pg_demo.exe load` 出临时 SAT，按 ACIS 模式读入当前窗口。
- **列出已存零件** — 弹窗显示最近 200 行 `id | name | bytes | updated_at`。
- **设置 PostgreSQL 连接信息** — 弹窗编辑 5 行（host/port/user/password/dbname），即时写到 `pg_connect_info.conf` 并刷新当前进程环境变量。

**用浏览器看表数据**：http://localhost:5050 → 登录 → Add New Server：`Host=dbcad-postgres-demo, Port=5432, Maintenance db=postgres, User=postgres, Password=admin` → 在左侧找到 `dbcad_demo / Schemas / public / Tables / dbcad_pg_demo` → 右键 View/Edit Data → 看到刚才 GUI 保存进去的行。

### 验证记录

本地 (127.0.0.1:5432, db=dbcad_demo, user=postgres) 实测：

```
==== init ====        [OK] schema ready (table dbcad_pg_demo)
==== save ====        [OK] saved name=demo_part_1 id=3 bytes=41
==== count ====       [OK] rows=1
==== load ====        [OK] loaded name=demo_part_1 bytes=41 -> _roundtrip.sat
==== compare ====     SHA256(input)   == SHA256(roundtrip)  ✅
==== delete ====      [OK] deleted name=demo_part_1 id=3
==== count again ==== [OK] rows=0
```

### 后续

如果老师认可本 demo，下一步可以：

1. 把 `pg_store.*` 的接口挂到 `storage_bridge_service.cpp`，让 Bridge 在 `CAD_DB_STORAGE_BACKEND=postgres` 时切换；
2. 把 FastAPI 的 `CAD_DB_STORAGE_BACKEND` env 加上 `postgres` 分支；
3. 增加 `face_count`、`bbox` 等列，用 SQL 查询替代 Neo4j 的 Cypher。

这些**第一阶段先不碰**，等老师拍板再动。

