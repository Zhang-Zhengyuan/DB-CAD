# 本机 ACIS 2026 环境（2026-10-15）

当前分支 `fix/acis-kernel-isolation`；本轮只迁移依赖与修复构建环境，未开始内核隔离重构。

## 启动与调试

在仓库根目录双击 `start-test.cmd`，启动默认 Release 全栈测试。也可执行：

```powershell
.\scripts\start-test.ps1
.\scripts\start-test.ps1 -Configuration Debug
```

启动器使用本机 `build.local.props` 指定的 Qt kit 打包，复用已运行服务，不清空数据库。Docker Desktop 必须运行。默认协作为 JSON Mode1，客户端需打开项目后测试 Push/Pull。

服务地址：<http://127.0.0.1:8000/health>、<http://127.0.0.1:8000/docs>、<http://127.0.0.1:8000/webui>。管理密码沿用被忽略的 `CAD_DB/deploy/dbcad.local.env`，不要上传该文件。

Visual Studio 打开 `CAD_DB.sln`，选择 x64 / Debug 后 F5；已配置 QtMsBuild 与本机 Qt 路径，`x64/Debug` 已收集 Qt 和第三方 DLL。重新构建也可使用：

```powershell
.\scripts\build.ps1 -Configuration Debug
.\scripts\build.ps1 -Configuration Release
```

## 已配置的依赖

- SDK 来源 `E:/ACIS_2026.1.0.1`，packageid 2026.1.0.1 / build 3343121；Release DLL 6.428.0.26083。官方头文件原样复制，Release/Debug 组件库分别落到 `_deps/acis/lib/SPAAcisDs.lib` 和 `SPAAcisDsd.lib`。
- `E:/spatial_license.h` 已复制到 `CAD_DB/spatial_license.h` 和实体测试目录；两处均由 Git 忽略，许可内容未输出。
- Qt 6.8.3：`E:/Devtools/6.8.3/msvc2022_64`；QtMsBuild：`C:/Users/Lenovo/AppData/Local/QtMsBuild`。路径保存于被忽略的 `build.local.props`。
- 使用本机已安装 v143 / MSVC 14.43，取消不存在的 14.44 固定版本；语言开关 `/std:c++latest`，两配置 `/MD`、`/EHa`、4MB 栈及 `SPA_NO_AUTO_LINK`。
- Debug 客户端使用 Qt Release DLL（同为 `/MD`），仍保留 `/Od`、完整 PDB 和 Debug ACIS 库；避免 Qt Debug `/MDd` 与 ACIS `/MD` 混用导致启动堆损坏。打包模式按实际 Qt 导入库选择。
- 官方 Debug 导入库内部仍引用 `SPAAcisDs.dll`；`build.ps1` 将 Debug DLL 在 `x64/Debug` 中按原导入名暂存。仅将库重命名为 `SPAAcisDsd.lib` 不会改变其导入的 DLL 名称。
- PostgreSQL/libpq 18.6、OpenSSL 3.5.9 LTS、GLFW 3.5.1；头文件、导入库和运行时取自同一官方配套包。公开依赖安装脚本：`python scripts/prepare-public-deps.py`，来源与 SHA-256 固定在脚本内；`scripts/build.ps1` 检测到固定命名的链接库缺失时会自动调用该脚本（可用 `-SkipDependencyProvision` 禁止），各机器无需手工预装即可收敛到同一套件。
- Glaze 5.5.2 与项目定制 mgclient 保留；对 mgclient 的 `alignof` 宏污染在包含边界做隔离。
- 旧 ACIS 保存在 `artifacts/acis-migration-2026-10-15-*/acis`，其他原依赖保存在 `artifacts/dependency-audit/original-deps`；备份和第三方包均不入库。

## 本机验证结果

- Release / Debug 主程序均构建成功，非 Qt DLL 暂存完成；Debug 在统一 Qt CRT、清理重建并修复 ACIS 实际 DLL 名称后启动保持运行，进入 GLWidget 绘制。
- `windeployqt` 收集双配置 Qt DLL/插件；递归 PE 依赖检查通过，发布包检查覆盖 27 个二进制。
- 实体测试覆盖 9 组：方块、球、圆柱、圆锥、圆环、棱柱、布尔并/差/交；全部通过，包含 SAT 往返、体积、包围盒、拓扑检查（实际输出 35 条 PASS）。日志：`artifacts/entity-results.log`。
- 后端非集成测试：32 passed，1 deselected；部署入口选择 7 项通过，运行时检查脚本通过。
- Docker Neo4j/APOC 启动，FastAPI `/health` 返回 ok / direct_json，Release 主界面进入并插入立方体成功。
- 旧 Bridge 额外回归：项目/模型提交及读取成功；block、subtract 的 SAT 去文件头后逐字节比较失败。尚未确定是序列化表示变化还是几何差异，不应据此声称 Bridge 往返完整通过；日志 `artifacts/bridge-roundtrip.log`。本轮不扩大到持久化算法修复。

编译存在 SDK 头文件编码/弃用警告和项目 dump 函数 LNK4217 警告，未阻止双配置构建。双客户端完整协作、长期使用和所有编辑命令仍需后续功能验收。
