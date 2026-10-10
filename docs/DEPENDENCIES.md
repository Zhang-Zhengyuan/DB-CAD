# 客户端依赖契约

客户端需要授权 ACIS SDK。仓库不提供商业 SDK 的下载或授权；维护者向有权使用的成员提供版本一致的依赖包。不要将依赖二进制加入 Git。

默认布局：

```text
_deps/
  acis/include/                 # 当前代码使用的 SDK 头文件
  acis/lib/SpaACIS.lib
  acis/lib/SpaACISd.lib
  acis/bin/SpaACIS.dll
  acis/bin/SpaACISd.dll
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

PostgreSQL 头文件也可通过 `DBCAD_POSTGRES_INCLUDE` 指向 vcpkg 的 include；运行时通过 `DBCAD_RUNTIME_DIR` 指向配套的 bin。不能混用不同架构或不兼容运行时。Debug 和 Release 要分别验证。

建议维护者发布一个校验过的内部依赖包，并保留版本、来源、授权范围、平台、哈希与安装步骤。当前无法仅凭目录名确定 ACIS/定制 SDK 的准确发行号，不应编造版本。接收包后先运行 `scripts/check-environment.ps1`。

Qt 固定以 6.8.3 MSVC2022 x64 验证，C++ 使用 v143 与 C++23。Qt 路径可配置；运行时由对应 kit 的 `windeployqt` 收集。给其他机器分发使用 Release，并安装 Microsoft Visual C++ 2015–2022 x64 Redistributable。

Python 使用 `CAD_DB/backend/pyproject.toml` 与 `uv.lock`。`requirements.txt` 仅保留直接依赖兼容入口，不承诺传递依赖版本一致；可复现环境统一使用 `uv sync --frozen`。
