# 客户端依赖契约

客户端需要授权 ACIS SDK。仓库不提供商业 SDK 的下载或授权；维护者向有权使用的成员提供版本一致的依赖包。不要将依赖二进制加入 Git。

## ACIS 依赖（2026.1.0.1 官方版）

获取授权 SDK、编译契约与 2026 行为差异的完整说明见 [ACIS 2026 迁移指南](ACIS_2026_MIGRATION.md)。

自 2025-10 重构起，客户端使用**未经修改的官方 3D ACIS Modeler 2026.1.0.1**（`SPAAcisDs` 组件化发行），
不再使用任何改头文件（`gme_` 扩展）的教学定制 SDK。所有几何读写均通过公有 API 完成。
旧 2021 定制 SDK 已于迁移验收后移除，如需回溯请从迁移前的 git 提交（`4044a69` 之前）获取。

默认布局：

```text
_deps/
  acis/include/                 # 官方 2026.1.0.1 头文件（与安装包 ACIS_2026.1.0.1/include 一致）
  acis/lib/SPAAcisDs.lib        # Release 导入库（安装包 win_b64/code/lib）
  acis/lib/SPAAcisDsd.lib       # Debug 导入库（安装包 win_b64_debug/code/lib）
  acis/bin/SPAAcisDs.dll        # Release 运行库（直接依赖仅系统 DLL 与 VC 运行时）
  acis/bin/SPAAcisDsd.dll       # Debug 运行库
  glaze-5.5.2/include/glaze/
  glfw/lib/glfw3_64.lib
  mgclient-1.4.2/mgclient.h
  openssl/lib/libssl64MD.lib
  openssl/lib/libcrypto64MD.lib
  openssl/lib/libssl64MDd.lib
  openssl/lib/libcrypto64MDd.lib
  openssl/bin/                  # 配套 DLL
  libpqxx-7.10.1/lib/libpq.lib  # 保留原依赖包布局；源码实际使用 libpq
  postgresql/include/           # libpq-fe.h 及其配套头文件
  postgresql/bin/               # libpq.dll 与完整传递依赖
```

### 官方 2026 SDK 的编译约定

- 链接 `SPAAcisDs.lib`（Release）/ `SPAAcisDsd.lib`（Debug），已写入 `CAD_DB.vcxproj`。
- 必须定义宏 `SPA_NO_AUTO_LINK`：官方 `dcl_*.h` 头内的 `#pragma comment(lib, "SpaACIS.lib")`
  自动链接指向旧单体库名，与 `SPAAcisDs` 组件库不匹配，会引发 LNK1104。
- 许可密钥来自 `CAD_DB/spatial_license.h`（配套 2026 版密钥，由维护者提供，不入库）；
  `acis.cpp` 的 `unlock_license()` 在 `api_start_modeller` 之后调用 `spa_unlock_products`。
- 运行时 CRT 必须为 `/MD`（MultiThreadedDLL）：向 ACIS API 传 `FILE*` 时，
  调用方与 DLL 必须共享同一动态 CRT，混用静态 CRT 会在 `ftelli64` 处触发
  invalid parameter 崩溃（0xC0000409）。
- SAT 文本保存前必须调用 `api_set_file_info` 设置有效 `units`，否则 2026 返回错误 68017。

PostgreSQL 头文件也可通过 `DBCAD_POSTGRES_INCLUDE` 指向 vcpkg 的 include；运行时通过 `DBCAD_RUNTIME_DIR` 指向配套的 bin。不能混用不同架构或不兼容运行时。Debug 和 Release 要分别验证。

建议维护者发布一个校验过的内部依赖包，并保留版本、来源、授权范围、平台、哈希与安装步骤。ACIS 版本以 `_deps/acis/include` 内官方 `packageid`（2026.1.0.1，buildID 3343121）与 `SPAAcisDs.dll` 文件版本（6.428.0.26083）为准。接收包后先运行 `scripts/check-environment.ps1`。

Qt 固定以 6.8.3 MSVC2022 x64 验证（本机 6.9.0 亦可构建），C++ 使用 v143 与 C++23。Qt 路径可配置；运行时由对应 kit 的 `windeployqt` 收集。给其他机器分发使用 Release，并安装 Microsoft Visual C++ 2015–2022 x64 Redistributable。

Python 使用 `CAD_DB/backend/pyproject.toml` 与 `uv.lock`。`requirements.txt` 仅保留直接依赖兼容入口，不承诺传递依赖版本一致；可复现环境统一使用 `uv sync --frozen`。
