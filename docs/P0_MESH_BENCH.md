# P0 面片引擎调研报告：四路径离散化基准（2026-10-18）

> V3_PLAN.md 里程碑 P0 验收产物。工具：`scripts/tests/mesh_bench.cpp`（`/p:DBCADMeshBench=true` 构建 → `x64/Debug/mesh_bench.exe`，CSV 输出）。
> 模型：球 R10、圆柱 r5h20、圆环 R8r2、三孔板（30×30×8 减 3 圆柱）。指标：三角数/耗时/法线可用性/球面 sag（质心径向偏差估计）。

## 选型结论

**采用 B：`facet_options_visualization` 质量档位**（coarse / medium_fine / fine），默认档 medium_fine。

1. **档位行为完全符合文档**（球面：728 → 2808 → 5624 三角，sag 0.116 → 0.030 → 0.015，耗时线性放大）——正好映射 MeshExtractor 的质量档位：coarse=预览/LOD、medium_fine=默认、fine=高精度。容差按包围盒对角线比例自适应，无需按模型尺寸调参。
2. **C（facet_options_precise）排除**：默认网格参数压倒 sag 设置——三个 sag 档（0.1/0.01/0.001）产出完全相同的爆量网格（球 162 万三角、圆环 324 万三角，25-50 秒）。它是分析级网格模式，直接用于可视化会灾难；将来做分析导出再回头细调其全套网格参数。
3. **A（默认档，现状）**：介于 B 的 coarse 与 medium_fine 之间，无参数可控——被 B 严格取代。
4. **法线结论**：所有路径（含默认档）`has_normals()` 均为真，`serialize_normals` 直接可用——**旧代码的平面法线手工回退不必要**；旧回退真正兜的是"图恢复面缺 facet 网格"场景（PCURVE 重建失败），新 MeshExtractor 改为显式报错+日志，不再静默三角化外环。
5. **缓存结论**：同参数二次离散**没有任何缓存收益**（球 49ms vs 首次 18ms，反而更慢——疑似在旧 facet 上重算）——内核层无免费缓存，**worker 侧内容寻址网格缓存（V3_PLAN API 设计 `/assets/{hash}`）是必需设计而非优化项**。
6. **SDK 能力边界（2026-10-18 复核修正）**：`api_get_facet_face_tri_points` / `api_get_sequential_mesh` 等 compound API **在授权包内，但位于独立 DLL `SpaAcisCompound.dll`**（bin 下 release/debug 各一，dumpbin 证实导出这批符号含 `INT_ARRAY` 系列），而发行包**未附带该 DLL 的导入库**（全包 .lib 检索无果，官方 samples 也不链它），项目 `_deps` 更未引入——故链接失败。如确需启用：`dumpbin /exports SpaAcisCompound.dll` → 生成 .def → `lib /def` 造导入库并随部署拷 DLL。**本选型不需要**：`GetSerializableMesh` + serialize_* 用现有已链接库即可拿到相同数据（位置/法线/三角形索引），少部署一个 DLL。可用/不可用清单：`api_facet_entity`、`api_delete_entity_facets`、`GetSerializableMesh`、`api_get_facet_edge_points` ✓；compound 系（tri_points/sequential_mesh/SEQUENTIAL_MESH）✗（除非自制导入库）。
7. **授权包家底补充（D:\ACISCAD 全套包勘察）**：① Interop_2026.1.0.1 完整在包（SPAIInterop/SPAIop 等导入库齐全）——STEP/IGES/CATIA/SolidWorks/Inventor 转换器授权可用，v3 导入导出面比预想宽；② `2026.1.0.1_demo_models/` 含 ACIS/CATIAV5/IGES/SolidWorks 示例模型——MeshExtractor 重写后的天然验证语料。两者均尚未引入 `_deps`。

## 基准数据（节选）

| 模型 | 路径/档位 | 三角数 | 离散 ms | 读取 ms | 法线 | sag |
|---|---|---|---|---|---|---|
| sphere_r10 | A default | 1368 | 18.1 | 0.26 | yes | 0.061 |
| sphere_r10 | B coarse | 728 | 9.4 | 0.10 | yes | 0.116 |
| sphere_r10 | B medium_fine | 2808 | 35.1 | 0.40 | yes | 0.030 |
| sphere_r10 | B fine | 5624 | 67.3 | 0.74 | yes | 0.015 |
| sphere_r10 | C sag=0.1~0.001 | 1620528（三档同） | 23716~28495 | ~250 | yes | 0.00006 |
| cyl_r5h20 | A / B fine | 124 / 284 | 3.8 / 5.1 | 0.03 | yes | - |
| torus_R8r2 | B coarse~fine | 790→8574 | 10→126 | 0.08→0.97 | yes | - |
| torus_R8r2 | C（任一 sag） | 3241078 | ~50000 | ~300 | yes | - |
| plate_3holes | B medium_fine | 444 | 6.7 | 0.05 | yes | - |
| plate_3holes | D 缓存复测 | 300 | 5.9 | 0.05 | yes | - |

完整 CSV 见 [p0_mesh_bench.csv](p0_mesh_bench.csv)（随本报告提交；复现：构建并运行 `mesh_bench.exe`，约 5 分钟，耗时主要在 C 路径的爆量离散）。

## 对 W1（MeshExtractor 重写）的直接输入

- 接口形态：`extract(BODY*, quality_level) → IndexedMesh{positions, normals, indices, faces[]}`；质量档位枚举直接映射 `facet_options_visualization::face_quality_level`。
- 每次离散前 `api_delete_entity_facets(body, TRUE)` 清理（本基准证实残留 facet 会拖慢重算）。
- 边折线沿用 `api_get_facet_edge_points`（EDGE* 旧式签名，可用）。
- 不可用 API 黑名单（省得再踩）：`api_get_facet_face_tri_points`、`api_get_sequential_mesh`、`SEQUENTIAL_MESH`。
