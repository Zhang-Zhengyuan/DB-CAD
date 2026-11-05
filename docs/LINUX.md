# Linux worker 构建与交付

目标为 Ubuntu x86_64（amd64，64 位 Intel/AMD）。Linux worker 是独立进程，供网关通过 stdin/stdout JSON 行协议调用，不是可被 Python `import` 或 `dlopen` 的库。

## 当前状态与平台边界

已使用授权 Linux ACIS / InterOp 2026.1.0.1 完成 Ubuntu 22.04 / GCC 11.4 的 Release 构建。两份独立运行包在不挂载项目/SDK 的 Ubuntu 22.04 与 24.04 amd64 容器中均通过自检、175 项网格检查、常驻协议与真实 STEP 导入；AI-DBCAD 包另通过原生特征/查询和 STEP 往返。教师实际电脑尚未验收。

Ubuntu 24.04 / GCC 13 的 Debug/Release 均已编译、自检并通过各 175 项网格检查；它的成品要求更高的系统运行库，交付优先采用 22.04 构建。两个项目各 6 项真实 worker API 集成在 Ubuntu 22.04 与 24.04 通过（内存存储，含建模、撤销重做、SAT/STEP）；Ubuntu 24.04 后端非集成分别为 AI-DBCAD 135 passed、DB-CAD 45 passed。Linux 工具回归在 22.04 与 24.04 各 7 passed。Windows/macOS 原生重编译、Neo4j 持久化和浏览器全栈不属于本次已验证结论。

AI-DBCAD 的 Windows/macOS 构建入口已存在；DB-CAD 默认分支原先只有 Windows 构建入口。两者来自同一 v3 架构，但 worker 源码并不相同：AI-DBCAD 有额外的 AI 几何命令、查询与 STEP 导出，不能默认交换二进制。分别在对应仓库执行相同的 Linux 构建入口。

只运行几何 worker 不需要 Python 网关、前端或 Neo4j。若要在 Linux 使用整个网页系统，还需要 FastAPI、Neo4j、资产库与前端构建；AI 功能另需相应 provider、上游循环和渲染环境。Windows `.dll` 与 macOS `.dylib` 均不能用于 Linux。

## 输入依赖

- 与机器架构一致、有授权的 **Linux ACIS 2026.1.0.1**、配套 **Linux InterOp 2026** 和对应许可头。不能仅下载 Mac/Windows SDK；不能把商业 SDK、许可或成品提交到 GitHub。
- 构建工具需要 Python 3.10+；后端需要 Python 3.11+（推荐 3.12）。另需支持 C++20 且满足该 SDK 官方 ABI 要求的编译器、`ldd`。Ubuntu 可安装 `build-essential`，实际 SDK 所需编译器版本以其发行说明为准。现有锁文件中的 Pydantic 不支持 Python 3.14，后端请显式用 3.12。
- `_deps/vendor/nlohmann/json.hpp`，沿用项目已有依赖包，不重复引入新版本。
- 默认许可头为 `CAD_DB/spatial_license.h`，也可用 `--license-header` 指定私有文件；若源码目录也有许可，两者必须一致。

`--acis-root` / `--interop-root` 指含 `include/` 的 SDK 根；`--acis-bin` / `--interop-bin` 显式指向各自 **Linux** `code/bin/`。不猜厂商安装目录名称。脚本识别 `libSpaACIS.so` 或 `libSPAAcisDs.so`，以及 `libSPAIInterop.so`、`libSPAIAcis.so`、`libSPAIopUtilities.so`；实际发行包名称不同需按 SDK 声明调整，不能拿其他平台文件改名替代。

## 构建、验证与打包

在对应仓库根目录、Linux x86_64 上执行（路径为占位值）。不会下载 SDK、修改 SDK、启动服务或重置数据库：

```bash
python3 scripts/linux-worker.py build --configuration Release \
  --acis-root /opt/spatial/acis \
  --acis-bin /opt/spatial/acis/LINUX_PLATFORM/code/bin \
  --interop-root /opt/spatial/interop \
  --interop-bin /opt/spatial/interop/LINUX_PLATFORM/code/bin

python3 scripts/linux-worker.py check build/linux/Release --step-fixture /absolute/path/sample.step
python3 scripts/linux-worker.py package build/linux/Release \
  --step-fixture /absolute/path/sample.step \
  --output output/worker-linux-x86_64-release.tar.gz
```

执行时先把 `LINUX_PLATFORM` 替换成真实目录名。Debug 同样执行 `build --configuration Debug` 与 `check build/linux/Debug`；该模式对本项目源码使用 `-O0 -g`，默认链接相同 SDK Release 运行库，不假定厂商提供 Debug 内核。可用 `--cxx` 选择 SDK 兼容编译器。厂商 Linux 库无 SONAME，链接必须使用 `-l:库名.so` 与 `$ORIGIN`，不能把库的绝对路径写入 ELF；脚本与回归会检查这一点。输出目录必须尚不存在；重编译用 `--output build/linux/Release-<run-id>` 并同步后续命令路径，不自动删除旧产物。

构建暂存 SDK 运行库实际内容；打包仅选择 worker 和 SAT/STEP BRep 适配器的动态依赖闭包，避免携带无关 GUI / 格式插件。保留 InterOp 的 `runtime/code/bin/`、`runtime/resources/`、`runtime/reffiles/` 相对结构。拒绝 ACIS/InterOp 同名不同内容文件、损坏链接和错误 ELF 架构。启动包装设置 `LD_LIBRARY_PATH`、转换器 `PATH` 与 ACIS 所需最小栈；网关应指向包装入口，不要直接调用 `.bin`。

`check` 检查动态依赖、worker selftest、mesh_tests（存在时）与常驻 serve 的 ping/open/非法命令/close。指定 `--step-fixture` 时还做真实 STEP 导入（含中文输出目录），校验 SAT/GLB/graph 产物；含 STEP 适配器的运行包必须提供这个参数。`package` 复制到全新临时目录并重新执行这些检查，通过后才生成 `.tar.gz`；缺依赖、错误架构、许可失败或自检失败均停止。包中包含 `build.json`、`validation.json` 和 `SHA256.json`，不包含源码、许可头、对象文件、数据库、凭据或 SDK include。仅在已有 Spatial 分发授权范围内私下提供运行包；许可可能编入二进制，不能将包作为公共 Release。

Linux 的 glibc/libstdc++ 与 SDK ABI 必须兼容目标 Ubuntu；在较新系统上构建，不保证能在较旧 Ubuntu 运行。最终需在目标 Ubuntu 或匹配的容器中编译/验收；当前容器内脚本回归不代表真内核或任意 Ubuntu 版本通过。`ldd` 可发现链接依赖，运行时按需加载的转换器依赖还须用实际 STEP 文件验证。

## 用 Ubuntu 22.04 工具链构建

Ubuntu 24.04/GCC 13 构建出的程序可能要求 GLIBC 2.38 / GLIBCXX 3.4.32，不能直接发给 Ubuntu 22.04。面向 22.04 与 24.04 的通用交付应在 22.04 工具链中构建，并分别验证。

可直接在 Ubuntu 22.04 安装构建工具，或使用仓库中的工具链镜像。下面使用空构建上下文，SDK、许可和源码都不会进入镜像层：

```bash
docker build --platform linux/amd64 -t dbcad-worker-builder - < scripts/linux-worker.Dockerfile
```

运行镜像时将源码挂到 `/src`、ACIS 与 InterOp SDK 分别只读挂到 `/sdk/acis`、`/sdk/interop`，许可头只读挂到 `/private/spatial_license.h`，然后使用与上面相同的 `build` 参数，另加 `--license-header /private/spatial_license.h`。JSON 头必须位于源码 `_deps/vendor/nlohmann/json.hpp` 或由 `--json-include` 指定。普通 clone 的 `.git` 在源码内即可；Git worktree 还需可访问其 git common directory。构建输出与 SDK/许可均保持 Git 忽略。

## 教师收到运行包后

```bash
mkdir -p worker-linux
tar -xzf worker-linux-x86_64-release.tar.gz -C worker-linux
./worker-linux/dbcad-worker-linux/dbcad_worker selftest
printf '%s\n' '{"id":1,"op":"ping"}' '{"id":2,"op":"close"}' \
  | ./worker-linux/dbcad-worker-linux/dbcad_worker serve
```

检查返回中对应 id 的 `ok` 为 true；自检使用系统临时目录，不依赖源码或数据库。`selftest` 本身不覆盖 STEP，需另用授权的 STEP 样本：

```bash
./worker-linux/dbcad-worker-linux/dbcad_worker import \
  --sat /absolute/path/sample.step --outdir /absolute/path/new-step-output
```

## 网关/整套系统接入

Linux 默认 worker 为 `build/linux/Release/dbcad_worker`。使用独立运行包或自定义构建时，在后端 `.env` 设置：

```dotenv
CAD_DB_WORKER_EXE=/absolute/path/worker-linux/dbcad-worker-linux/dbcad_worker
```

Windows 默认路径保持 `x64/Debug/dbcad_worker.exe`；AI-DBCAD macOS 默认保持 `build/macos/Release/dbcad_worker`。JSON/HTTP/SAT/存储契约均未改变，无数据库迁移；回滚本任务脚本/配置/文档即可恢复原构建行为，已有数据不变。

整套系统先按后端 `.env.example` 设置 Neo4j URI、用户、密码和 API 密码，并启动独立的 Neo4j 服务。随后在仓库根：

```bash
cd web
npm ci
npm run build
cd -
cd CAD_DB/backend
uv sync --python 3.12 --frozen --extra dev
uv run --frozen uvicorn app.main:app --host 127.0.0.1 --port 8000
```

访问 `http://127.0.0.1:8000/web/`；worker 由网关按项目启动。需要其他电脑访问时，按部署环境配置绑定地址、鉴权和访问入口。不要在 Linux 调用 Windows PowerShell 启动器或 Mac 专用启动器。

## 回归与真内核验收

无需 SDK 的脚本回归（其中 ELF 动态加载/重定位和 POSIX 中文路径测试需要 Linux C/C++ 编译器）：

```bash
python3 scripts/tests/test-linux-worker.py
cd CAD_DB/backend
uv sync --python 3.12 --frozen --extra dev
uv run --frozen pytest -q -m 'not integration'
```

使用匹配的 Linux SDK，先完成两种配置的 `build` / `check`，再运行真实 worker 接入验证：

```bash
# 工作目录：CAD_DB/backend
export CAD_DB_WORKER_EXE=/absolute/path/to/build/linux/Release/dbcad_worker
export CAD_DB_STEP_FIXTURE=/absolute/path/sample.step
uv run --frozen pytest -q tests/test_v1_real_commands_integration.py \
  tests/test_v1_real_worker_integration.py tests/test_v1_step_import_integration.py
```

最后对独立包运行相同检查，并验证 Neo4j 持久化、浏览器建模、失败命令、撤销/重做、SAT/STEP 导入；AI-DBCAD 另验证 AI 扩展命令与 STEP 导出。出现 skip 必须核对缺少的 fixture/SDK，不能据此声称通过。教师目标机、Neo4j 持久化和浏览器全栈仍属单独验收范围；不要把真 worker + 内存存储 API 测试称作生产全栈验收。
