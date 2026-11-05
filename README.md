# DBCAD — 多人协作 CAD 数据库系统

服务端权威（server-authoritative）的多人协作 CAD：几何计算在服务端 C++ ACIS worker 执行，FastAPI 网关管理命令队列与版本链，Neo4j 存储 SAT/GLB 资产与实体级图投影，浏览器（React + three.js）是唯一客户端。参照 Onshape 的架构分层，文件被数据库替代。

## Linux：收到运行包后部署完整 CAD 系统

适用 **Ubuntu 22.04 / 24.04，x86_64**。维护者发送 **`DB-CAD-worker-ubuntu22.04-x86_64-20261105.tar.gz`**（约 70 MB）；这个包已包含 worker、ACIS/InterOp 动态库、STEP 转换器与资源。接收者无需安装 ACIS SDK 或编译 C++。它与本仓库配套，勿取另一项目的包。

首次安装需要联网。预先安装 Git、Docker Engine（含运行权限）、Node.js 22.12+（22.x）和 uv；Ubuntu 自带的 Python 3 可用于生成本机配置，后端由 uv 安装 Python 3.12。Docker 只用来运行 Neo4j；网关直接运行在 Linux 上，前端由网关托管。

**1. 拉默认分支，校验并安装运行包。** 新机器执行 clone；已有 clone 则先切到 `master` 再 `git pull --ff-only`。

```bash
git clone --branch master https://github.com/Zhang-Zhengyuan/DB-CAD.git
cd DB-CAD
# 后续步骤使用 Bash，工作目录均从仓库根开始。
set -euo pipefail
test "$(uname -m)" = x86_64
RUNTIME_ARCHIVE="$HOME/Downloads/DB-CAD-worker-ubuntu22.04-x86_64-20261105.tar.gz"
printf '%s  %s\n' '110d650813dccf5cf85f63e3b33ed3bd607b88eefc03f6b4a6986ce047c4632a' "$RUNTIME_ARCHIVE" | sha256sum --check
# 全新安装目录；已部署时保留原目录和配置，勿混解压两个版本。
mkdir -p _deps/runtime/linux
tar -xzf "$RUNTIME_ARCHIVE" -C _deps/runtime/linux
./_deps/runtime/linux/dbcad-worker-linux/dbcad_worker selftest
```

如运行包不在 `Downloads`，只需改 `RUNTIME_ARCHIVE`。不要把 `.so` 拆散复制；`_deps/` 整目录不进 Git。

**2. 生成本机配置。** 两个密码自行输入，不会显示或写入 Git。数据库密码使用至少 16 个字符；网页访问密码是打开 CAD 网页时输入的密码。

```bash
mkdir -p .local
python3 - <<'CONFIG'
from pathlib import Path
import getpass, os, shlex
root = Path.cwd().resolve()
p = root / '.local/linux.env'
if p.exists():
    raise SystemExit('已有 .local/linux.env，保留原配置；需要修改时用编辑器打开。')
neo = getpass.getpass('设置 Neo4j 密码（至少16字符）: ')
api = getpass.getpass('设置 CAD 网页访问密码: ')
if len(neo) < 16 or not api:
    raise SystemExit('密码不满足要求，请重新执行。')
values = {
    'CAD_DB_STORAGE_BACKEND': 'neo4j',
    'CAD_DB_NEO4J_URI': 'bolt://127.0.0.1:7687',
    'CAD_DB_NEO4J_USER': 'neo4j',
    'CAD_DB_NEO4J_PASSWORD': neo,
    'CAD_DB_NEO4J_DATABASE': 'neo4j',
    'CAD_DB_API_PASSWORD': api,
    'CAD_DB_WORKER_EXE': str(root / '_deps/runtime/linux/dbcad-worker-linux/dbcad_worker'),
    'CAD_DB_COMPONENT_ROOT': str(root / '.local/linux/components'),
}
with os.fdopen(os.open(p, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600), 'w') as f:
    f.write(''.join('export ' + k + '=' + shlex.quote(v) + '\n' for k, v in values.items()))
print('配置已保存到 .local/linux.env（仅本机）。')
CONFIG
source .local/linux.env
```

**3. 安装锁定依赖并构建网页。**

```bash
uv python install 3.12
uv sync --project CAD_DB/backend --python 3.12 --frozen
(cd web && npm ci && npm run build)
```

**4. 启动持久化数据库。** 以下容器名和数据卷归本次部署使用。首次运行执行 `docker run`；以后重启用 `docker start db-cad-neo4j`。两个项目同时部署时须另配 Bolt/HTTP 端口，不能占用同一组端口。

```bash
source .local/linux.env
export NEO4J_AUTH="neo4j/$CAD_DB_NEO4J_PASSWORD"
docker run -d --name db-cad-neo4j --restart unless-stopped \
  -p 127.0.0.1:7474:7474 -p 127.0.0.1:7687:7687 \
  -v db-cad-neo4j-data:/data -e NEO4J_AUTH neo4j:5.26.12-community
unset NEO4J_AUTH
```

**5. 检查数据库连接，再启动网关。** 保持这个终端运行；不要只根据 `/health` 成功判断持久化已接通，当前程序在数据库故障时可能降级到内存。

```bash
source .local/linux.env
cd CAD_DB/backend
uv run --frozen python - <<'CHECK'
from app.config import settings
from neo4j import GraphDatabase
import time
for attempt in range(30):
    try:
        with GraphDatabase.driver(settings.neo4j_uri, auth=(settings.neo4j_user, settings.neo4j_password)) as d:
            d.verify_connectivity()
            with d.session(database=settings.neo4j_database) as s:
                s.run('RETURN 1').single(strict=True)
        print('Neo4j 连接与认证通过')
        break
    except Exception:
        if attempt == 29:
            raise SystemExit('Neo4j 未就绪：检查 docker logs、端口、密码与原数据卷的密码；不要清库重试。')
        time.sleep(2)
CHECK
uv run --frozen uvicorn app.master:app --host 0.0.0.0 --port 8000
```

**6. 打开网页并验收。** 本机访问 `http://127.0.0.1:8000/web/`；其他电脑访问 `http://服务器IP:8000/web/`（需服务器允许访问 8000 端口）。输入步骤 2 设置的 CAD 网页访问密码，创建项目并插入立方体；停止网关后重新启动，确认项目、版本和模型仍存在。`/webui` 是数据库管理页面，使用同一访问密码。

日常启动：从仓库根 `source .local/linux.env`，`docker start db-cad-neo4j`，然后进入 `CAD_DB/backend` 执行最后一条 uvicorn 命令。停止网关按 Ctrl+C；停止数据库用 `docker stop db-cad-neo4j`，保留数据卷。更新代码后重新执行步骤 3；只有维护者通知 worker 协议/功能更新时才替换运行包。


## 新成员上手

使用 coding agent 前先读 [AGENTS.md](AGENTS.md)；团队按 [Issue → 分支 → 验证 → PR → 人工审核](docs/ai/WORKFLOW.md) 协作。项目按需导航见 [AI 项目地图](docs/ai/PROJECT_CONTEXT.md)。

完整步骤见 [复现与部署指南](docs/GETTING_STARTED.md)。worker 构建需要授权 ACIS SDK 与配套依赖；源码不包含 SDK。依赖布局见 [依赖说明](docs/DEPENDENCIES.md)；SDK 已迁移到官方 ACIS 2026.1.0.1（纯公有 API，无定制头文件），编译契约与行为差异见 [ACIS 2026 迁移指南](docs/ACIS_2026_MIGRATION.md)。

准备依赖和本机配置后，在仓库根目录执行：

```powershell
.\scripts\check-environment.ps1
.\scripts\build.ps1 -Configuration Release
Push-Location web
npm ci
npm run build
Pop-Location
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
```

只验证后端，不需要 CAD SDK 或数据库：

```powershell
cd CAD_DB/backend
uv sync --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

## 平台支持与部署运行

| 平台 | worker（几何内核）来源 | 网关 / 前端 / Neo4j | 指南 |
| --- | --- | --- | --- |
| Windows 10/11 x64 | 本机源码构建（VS 2022 + 授权 ACIS Windows SDK） | 支持 | [复现与部署指南](docs/GETTING_STARTED.md) |
| Linux x86_64（Ubuntu 22.04+，glibc ≥ 2.35） | 预编译 worker 运行包（无需 SDK），或授权 SDK 自建 | 支持 | [Linux 指南](docs/LINUX.md) |
| macOS | 无官方构建入口（macOS 支持在 AI-DBCAD 仓库提供） | — | [AI-DBCAD](https://github.com/Zhang-Zhengyuan/AI-DBCAD) |

各平台的网关、前端与 Neo4j 部署方式一致（Python 3.11+ / uv、Node.js 20.19+ 或 22.12+、Neo4j），差别只在 worker 二进制如何获得。Linux 运行包仅限 x86_64；ARM 主机与 glibc < 2.35 的旧发行版（CentOS 7、Ubuntu 20.04 等）不能运行。DB-CAD 与 AI-DBCAD 的 worker 同源但命令集不同（AI-DBCAD 含 AI 建模/查询与 STEP 导出扩展），二进制不可互换。

### 新机器部署流程

1. **取代码**：`git pull` 本仓库。商业 SDK 与许可不随源码分发，仓库不包含任何授权组件。
2. **放运行库**：向维护者索取对应平台的依赖/运行包，统一放在仓库根 `_deps/`（整个目录不进 Git，布局见 [依赖说明](docs/DEPENDENCIES.md)）：
   - Windows 构建者：ACIS Windows TeamKit 解压为 `_deps/acis/` 等目录（含 InterOp 依赖）；
   - Linux 使用者：直接执行本文开头的 Linux 六步流程，运行包固定安装到 `_deps/runtime/linux/`，配置保存到 `.local/linux.env`。
3. **首次安装与构建**：Windows 见上文[新成员上手](#新成员上手)的命令序列；Linux 按 [Linux 指南](docs/LINUX.md) 安装依赖并构建前端。
4. **启动**：Windows 执行 `.\CAD_DB\deploy\start_dbcad_fullstack.cmd`；Linux 在 `CAD_DB/backend` 运行 `uv run --frozen uvicorn app.main:app --host 127.0.0.1 --port 8000`，浏览器访问 `http://127.0.0.1:8000/web/`。Neo4j 需独立可用（连接配置见各指南与 `CAD_DB/backend/.env.example`）。

## 功能与模式（v3，2026-10-19 定稿）

- **服务端权威命令**：建模（曲线/曲面/布尔/抽壳/变换）、撤销/重做全部在 ACIS worker 执行，浏览器只提交命令与消费网格（ADR-2：每项目一 worker 进程，命令串行队列即版本链）。
- **版本链与协作**：commit 带 base_version 乐观锁（过期 409 并返回实体级冲突清单）；组件不可变、按 `hash_sat` 全局去重；WebSocket 推送版本事件与在场感知（edit_intent）。
- **三级存储（ADR-3）**：SAT 资产 = 几何真相（内容寻址，还原字节级无损）；GLB = 渲染网格；实体图投影 = 可查询细档索引——原生标签 + 扁平字段，`DERIVED_FROM` 血缘链、`/entities` 与 `/diff` 实体级查询，内容寻址实体跨组件去重（ABC 压测实测 60.5%）。
- **导入导出**：SAT 与 STEP（Spatial InterOp）导入，SAT/GLB/STL 导出；STEP 装配体按组件树拆分入库。
- **渲染全覆盖**：可视化网格 + 边折线；无面体输出纯线框 GLB，退化面计入 `faces_skipped` 诚实上报（ABC 200/200 零失败；CNC 车床装配 248 组件 / 48.8 万三角形实测流畅渲染）。
- Neo4j 管理 WebUI（项目/版本/图/原始提交审计）。
- Qt 桌面客户端、C++ Bridge、客户端 Bolt 直连与 PostgreSQL 实验均已移除（历史见 `docs/history/` 与 tag `legacy/pre-architecture-v2`）。

## 几何内核隔离

ACIS 只存在于 worker 进程：许可初始化与 SDK 调用集中在 `CAD_DB/acis.cpp` 与 `worker/`、`kernel/` 的适配实现；`kernel/model_types.h` 定义中立值类型，`glb_writer`/`mesh_extractor` 输出与内核无关的网格资产。网关与浏览器不接触任何内核类型。

当前为编译时适配：接入其他内核需要实现 worker 命令层与网格提取，并处理已有 SAT 数据的兼容读取。尚未接入第二内核，不承诺无损读取 SAT，也没有运行时插件加载。设计取舍见 [内核隔离与替换指南](docs/KERNEL_ISOLATION.md)。

## 目录

```text
CAD_DB.sln                唯一 Visual Studio 解决方案（默认目标 dbcad_worker）
CAD_DB/
  acis.cpp/common.cpp     ACIS 许可初始化与错误处理
  worker/                 ACIS worker：命令执行、会话、实体图序列化
  kernel/                 网格提取、GLB 写入、中立类型
  backend/app/
    api/v1/               HTTP/WS 入口（项目/版本/命令/资产/实体/事件）
    v1_store.py           版本链 + 实体投影存储（Memory/Neo4j 双实现）
    neo4j_webui/          管理 API；static/webui/ 管理台前端
  backend/tests/          回归与集成检查（API 契约 / 真内核 / STEP / Neo4j 投影）
  backend/scripts/        abc 压测、投影重建工具
  deploy/                 一键启停、Docker、打包
web/                      React + three.js 客户端（Vite）
scripts/                  环境检查与便携构建
docs/                     架构、计划、基准、上手指南；docs/history/ 为 v2 归档
```

## 启停

```powershell
.\CAD_DB\deploy\start_dbcad_fullstack.cmd
.\CAD_DB\deploy\stop_dbcad_fullstack.cmd
```

启动链路：Neo4j → FastAPI → 浏览器 `/web/`，worker 由后端按需启动。默认端口 8000，以启动器输出地址为准；登录使用 `CAD_DB/deploy/dbcad.local.env` 中的 `DBCAD_PASSWORD`。重建与同机双客户端测试见[上手指南](docs/GETTING_STARTED.md#修改代码后重新运行)，参数见[部署说明](CAD_DB/DEPLOYMENT.md)。

[架构](docs/ARCHITECTURE.md) · [v3 计划](docs/V3_PLAN.md) · [压测基准](docs/BENCHMARK_v3.md) · [贡献说明](CONTRIBUTING.md) · [后端说明](CAD_DB/backend/README.md)

Linux worker 构建、独立运行包与网关接入见 [Linux 指南](docs/LINUX.md)（平台和验收范围见指南）。
