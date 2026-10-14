# ACIS 2021 定制版 → ACIS 2026.1.0.1 官方版迁移指南

本文档记录 DBCAD 客户端从"2021 教学定制 SDK（含 `gme_` 头文件扩展）"迁移到
"官方未修改 3D ACIS Modeler 2026.1.0.1（`SPAAcisDs` 组件化发行）"的完整过程、
编译契约与行为差异。适用于新成员搭建环境、以及后续排查 ACIS 相关问题。

## 1. 迁移背景

- **旧依赖**：基于 ACIS 2021 SP1 的教学定制包，头文件内嵌 86 个文件的 `gme_`
  扩展方法（如 `intcurve::gme_get_fit`、`spl_sur::gme_get_u_singularity`），
  链接单体 `SpaACIS.lib`（60MB 全量 DLL）。
- **新依赖**：Spatial 官方 2026.1.0.1 完整安装包（`D:\ACISCAD\ACIS_2026.1.0.1`），
  头文件未经任何修改；链接组件化 `SPAAcisDs.lib`（其直接运行时依赖仅系统 DLL
  与 VC 运行时，不需要 MKL/boost/CAT 全家桶）。
- 旧 SDK 备份于 `_deps/acis_legacy/`，仅供回溯对照，不参与编译。

## 2. 获取官方 SDK 与授权（重要）

**ACIS SDK 与 DLL 无法从公共渠道下载，必须通过 Spatial 官网授权获取：**

1. 访问 [https://www.spatial.com](https://www.spatial.com)，用客户账号登录
   **Spatial Download Site**（3D Systems / Spatial Corp. 的产品下载门户）。
   我们的许可主体为 Haihe Laboratory（客户 ID 18881），授权范围见
   `spatial_license.h` 文件头（3D ACIS Modeler / Defeaturing / Polyhedra、
   3D InterOp 等，Product Version 2026，License R36，Production 永不过期）。
2. 在下载页选择 **3D ACIS Modeler 2026.1.0.1**（Windows x64）完整安装包，
   运行安装器时可勾选组件（Modeler、Healing、Polyhedral、HLR、Defeaturing 等）。
3. 从安装目录提取本项目需要的三个部分放入 `_deps/acis/`：
   - `include/` ← 安装包 `ACIS_2026.1.0.1/include/`（1795 个头文件，原样复制）
   - `lib/SPAAcisDs.lib` ← `win_b64/code/lib/SPAAcisDs.lib`（Release 导入库）
   - `lib/SPAAcisDsd.lib` ← `win_b64_debug/code/lib/SPAAcisDs.lib`（Debug 导入库）
   - `bin/SPAAcisDs.dll` ← `win_b64/code/bin/SPAAcisDs.dll`（Release 运行库）
   - `bin/SPAAcisDsd.dll` ← `win_b64_debug/code/bin/SPAAcisDs.dll`（Debug 运行库）
4. **许可密钥**：向维护者索取配套 2026 的 `spatial_license.h`（官方
   customer portal 生成，含 `SPATIAL_LICENSE` 宏），放到 `CAD_DB/` 下。
   该文件已加入 `.gitignore`，严禁入库。
5. 接收依赖后先运行 `scripts/check-environment.ps1` 校验布局。

版本核对：`SPAAcisDs.dll` 文件版本应为 `6.428.0.26083`；SAT 输出头应显示
`ACIS 36.0.1 NT 24`（2026 的内部版本号 36）。

## 3. 官方编译契约（必须遵守）

对照官方《Compiler Flags for Windows 64-bit》说明与官方 2026 示例工程
（`samples/ACIS_Functional_Examples_app`、`scm/`）核对后的要求，
已全部写入 `CAD_DB/CAD_DB.vcxproj`：

| 项目 | 要求 | 已配置 |
|---|---|---|
| 运行时库 | **Debug 也必须 `/MD`**（`MultiThreadedDLL`） | ✅ 两配置均 `/MD` |
| 异常模型 | 链接 ACIS 的代码必须 `/EHa`（异步异常） | ✅ `ExceptionHandling=Async` |
| 栈大小 | `/STACK:4194304`（4MB，深度几何递归） | ✅ `StackReserveSize=4194304` |
| 禁用自动链接 | 定义 `SPA_NO_AUTO_LINK` | ✅ 已定义 |
| 组件库 | `SPAAcisDs.lib`（Release）/ `SPAAcisDsd.lib`（Debug） | ✅ 已链接 |

### 为什么 Debug 必须用 /MD（官方红线，已实证）

`SPAAcisDsd.dll`（Debug 库）自身链接的是 **Release 动态 CRT**
（`MSVCP140.dll`/`VCRUNTIME140.dll`，无 `d` 后缀）。若调用方使用 `/MDd`，
跨边界传递 `FILE*`（本项目 SAT 保存恢复全靠它）会在 DLL 内部
`ftelli64` 处触发 invalid parameter 崩溃（`0xC0000409`）。
我们用最小程序复现并确认：`/MDd` + `SPAAcisDsd` 必崩，`/MD` + 两者均正常。
官方文档原话：若坚持 `/MDd`，则"不得与 ACIS 交换文件指针或内存"——本项目的
架构做不到，因此选择官方第一方案：Debug 也 `/MD`。

`SPA_NO_AUTO_LINK` 同理：官方 `dcl_*.h` 头内 `#pragma comment(lib, "SpaACIS.lib")`
指向旧单体库名，与组件化 `SPAAcisDs` 不匹配，不禁用会 LNK1104。

## 4. API 迁移映射（gme_ → 官方公有 API）

全部 66 处侵入式调用已消除，核心映射：

| 侵入式（已删除） | 官方公有 API |
|---|---|
| `ENTITY->gme_get_def()` | `entity->equation()` + `static_cast` 到具体方程类 |
| `gme_get_subset_range()`（曲面） | `surface::subset_box()` |
| `gme_get_subset_range()`（曲线） | `curve::param_range()` |
| `gme_set_subset_range()` | `curve::limit()` / `surface::limit()`（规范化子集化） |
| `gme_set_param_scale()` | `straight(pos, dir, scale)` 三参构造器 |
| `gme_set_rev()` / 直写 `reverse_v` | 方程类公有 `operator-()` 取反 |
| `INTCURVE::gme_get_fit()` | `INTCURVE::equation()` → `intcurve::get_int_cur()` |
| `int_cur` 内部数据（bs3/fitol/surf/pcur/safe_range） | `intcurve::cur()/fitol()/pcur(1/2)->surf()/safe_range()` |
| `int_int_cur` 定制构造器 | `exact_int_cur` 公有构造（int_int_cur 类不公开，官方注释指定等价路径） |
| `exp_par_cur` 定制构造器 | `pcurve(bs2_curve, fitol, surface)` 公有构造 |
| `gme_set_spl()/gme_set_fit()`（装配） | `spline::replace_spl_sur()` + `ENTITY::set_def()` |
| spl_sur 闭合/奇异标志 | `bs3_surface_to_array` 出参 `form_u/v`、`pole_u/v` |

### 结构性重设计：样条承载面装配

`int_cur` 构造后无法从外部替换内部曲面（旧代码靠 `gme_set_surf1_spl` 绕过）。
新方案为**恢复预扫描**（`access_graph_restore.cpp`）：节点循环前先扫边表收集
`int_cur → spl_sur` 对应关系并预构造全部 `spl_sur`，主循环构造 `int_cur` 时
经 `spline::replace_spl_sur` 完成装配；原三条装配边变为 no-op。
`history_restore` 侧确认保存端在源头过滤样条实体，相关分支仅保留枚举完整性。

## 5. 2026 行为差异（排查手册）

1. **SAT 保存前必须设置 FileInfo**：`api_set_file_info(FileIdent|FileUnits, fi)`
   且 `units` 有效（错误 68017）、`product_id` 非空（错误 68016）。本项目
   `access.cpp` 的 `acis_save_entity_list` 已按官方顺序设置。
2. **布尔 API 参数序**：`api_unite/subtract/intersect(tool, blank)` ——
   **结果保留在第二个参数 blank 中，tool 被消耗**。误存/误删 tool 会得到
   错误 3015（Access violation，保存已消耗的 body）或 double-free 崩溃。
   主项目现有调用（`mainwindow_documents.cpp:402` 的
   `api_subtract(hole, block)`）语义正确：hole 是 tool、block 持结果。
3. **圆柱面 type_name 报 "cone"**：`api_solid_cylinder_cone` 产生的等半径
   柱面，其 `equation().type_name()` 返回 `"cone"`（半角为 0 的退化锥），
   往返保存不改变该表示。判定曲面类型时勿硬编码 `"cylinder"`。
4. **实体与几何分离**：`api_get_faces` 返回的是 `FACE`（ENTITY），几何需经
   `face->geometry()` 取 `SURFACE*` 后再 `equation()`。直接把 ENTITY 强转
   SURFACE 会崩。
5. **`int_int.hxx`/`int_int_cur` 不再公开**：旧定制头已删除；交线数据走
   `intdef.hxx` 的 `intcurve` 公有接口。
6. **`api_copy_entity_contents` 已移除**、`api_make_face_spline` 为新增；
   逐字节 SAT 比对策略在类型降级（surfintcur→exactcur）处会 differ，
   需按语义比对。

## 6. 验证记录（2026-10-14）

- **编译**：Release + Debug 双配置通过（含 Clean 重建），栈保留 4MB 生效
  （dumpbin headers 确认 `size of stack reserve 400000`）。
- **冒烟程序**（`D:\DBCAD\smoke`）：`initialize_base` → `api_start_modeller`
  → `spa_unlock_products`（返回 `SPA_UNLOCK_PASS`）→ `api_solid_block`
  → SAT 文本保存 → 恢复计数，全链路通过。
- **实体测试套件**（`entity_tests.cpp`，9 组 36 项检查全部 PASS）：
  方块、球、圆柱、圆锥、圆环、六棱柱（扫掠型面）、布尔并/差/交；
  每组做建模 → SAT 往返 → 体积（`api_body_mass_pr`）/包围盒（`api_get_entity_box`）
  /拓扑计数（`api_get_faces/edges/vertices`）三重校验。
- **主程序**：Release 与 Debug `CAD_DB.exe` 均正常启动（ACIS 初始化成功后
  才创建主窗口）。

## 7. 常见故障速查

| 症状 | 原因 | 处置 |
|---|---|---|
| LNK1104 无法打开 SpaACIS.lib | 官方头 auto-link pragma | 定义 `SPA_NO_AUTO_LINK` |
| 保存/恢复时 0xC0000409 崩溃（栈在 `ftelli64`） | 调用方用了 `/MDd` 或静态 CRT | 改 `/MD` |
| 保存报 68017/68016 | FileInfo 未设 units/product_id | 保存前 `api_set_file_info` |
| 布尔后保存报 3015 | 保存了被消耗的 tool body | 保存/持有第二个参数 blank |
| 布尔后进程崩（堆损坏） | 对 tool/blank 双重删除 | 布尔后只删结果 blank 一次 |
