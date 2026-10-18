// P0 面片引擎基准（V3_PLAN.md §2 第一步 / 里程碑 P0）
//
// 四路径离散化对比，量化指标：三角数/节点数/离散耗时/读取耗时/法线可用性/球面 sag 估计。
//   A 现状默认档   api_facet_entity(body)                       —— 旧 gme_mesh 等价路径（基线）
//   B 可视化档位   facet_options_visualization(coarse/medium_fine/fine)
//   C 精确容差     facet_options_precise(surface_tol=0.1/0.01/0.001)
//   D 缓存复用     同参数二次离散（不删 facet）——为 worker 网格缓存设计提供依据
//
// 读取通道统一用 af_serializable_mesh（GetSerializableMesh/FACE*）。调研发现：
// facet_api.hxx 的 api_get_facet_face_tri_points / api_get_sequential_mesh 属
// compound 组件（DECL_COMPOUND），符号不在授权库 SPAAcisDsd.lib 中，此 SDK 不可用。
//
// 构建：MSBuild CAD_DB.vcxproj /p:DBCADMeshBench=true /p:Configuration=Debug /p:Platform=x64
// 输出：CSV 到 stdout（model,path,quality,faces,tris,nodes,facet_ms,read_ms,normals,sag,sag_max）
// 说明：sag 仅对球模型按三角形质心径向偏差估计（解析曲面可算）；normals 列回答
// "带选项离散时内核是否直接产法线"（旧代码的平面法线手工回退是否仍有必要）。

#include "acis.hxx"

#include "acis/include/acistype.hxx"
#include "acis/include/body.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/getowner.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/lump.hxx"
#include "acis/include/loop.hxx"
#include "acis/include/coedge.hxx"
#include "acis/include/point.hxx"
#include "acis/include/shell.hxx"
#include "acis/include/transf.hxx"
#include "acis/include/vertex.hxx"

#include "acis/include/cstrapi.hxx"               // api_make_sphere/cuboid/torus, api_solid_cylinder_cone
#include "acis/include/boolapi.hxx"               // api_subtract
#include "acis/include/af_api.hxx"                // api_facet_entity / api_get_facet_edge_points
#include "acis/include/facet_options.hxx"         // visualization/precise 档位
#include "acis/include/af_serializable_mesh.hxx"  // GetSerializableMesh

#include <chrono>
#include <cmath>
#include <cstdio>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

double msBetween(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

struct BenchRow {
    const char* model;
    const char* path;
    const char* quality;
    int faces = 0;
    long tris = 0;
    long nodes = 0;
    double facetMs = 0.0;
    double readMs = 0.0;
    bool normals = false;
    bool sagValid = false;
    double sagMax = 0.0;
};

bool makeSphere(double r, BODY*& body) {
    return api_make_sphere(r, body).ok();
}

bool makeCylinder(double r, double h, BODY*& body) {
    SPAposition bottom(0, 0, 0), top(0, 0, h);
    return api_solid_cylinder_cone(bottom, top, r, r, r, NULL, body).ok();
}

// 板上打 3 个孔：cuboid 30x30x8 减去 3 个竖直圆柱，模拟典型工程件（多面+布尔缝合）。
bool makeHoledPlate(BODY*& body) {
    if (!api_make_cuboid(30, 30, 8, body).ok()) return false;
    const double pts[3][2] = {{-7, -5}, {7, -5}, {0, 7}};
    for (const auto& pt : pts) {
        BODY* tool = NULL;
        SPAposition bottom(pt[0], pt[1], -1), top(pt[0], pt[1], 9);
        if (!api_solid_cylinder_cone(bottom, top, 3, 3, 3, NULL, tool).ok()) return false;
        if (!api_subtract(tool, body).ok()) return false;
    }
    return true;
}

// A/B/C 共用：离散（可用选项）→ 逐面 af_serializable_mesh 读取；sphereR>0 时顺带估 sag。
// 读缓存控制：clearBefore=true 时先 api_delete_entity_facets，保证离散计时不含缓存。
BenchRow facetAndRead(BODY* body, facet_options* opts, const char* model,
                      const char* path, const char* quality, double sphereR, bool clearBefore) {
    BenchRow row{model, path, quality};
    if (clearBefore) api_delete_entity_facets(body, TRUE);

    Clock::time_point t0 = Clock::now();
    outcome out = opts ? api_facet_entity(body, opts) : api_facet_entity(body);
    Clock::time_point t1 = Clock::now();
    if (!out.ok()) {
        std::fprintf(stderr, "[ERR] facet %s/%s/%s failed\n", model, path, quality);
        return row;
    }
    row.facetMs = msBetween(t0, t1);

    ENTITY_LIST faces;
    api_get_faces(body, faces);
    row.faces = faces.count();

    Clock::time_point t2 = Clock::now();
    long tris = 0, nodes = 0;
    bool normalsOk = true;
    std::vector<float> coords, normalCoords;
    std::vector<int> trisIdx;
    for (ENTITY* fe = faces.first(); fe; fe = faces.next()) {
        FACE* face = (FACE*)fe;
        af_serializable_mesh* sm = GetSerializableMesh(face);
        if (!sm || sm->number_of_polygons() <= 0) {
            normalsOk = false;  // 无 facet 网格的面（旧代码遇到的 PCURVE 问题场景）
            continue;
        }
        const int nv = sm->number_of_vertices();
        const int np = sm->number_of_polygons();
        coords.resize(3 * (size_t)nv);
        sm->serialize_positions(coords.data());
        trisIdx.resize(3 * (size_t)np);
        int actual = sm->serialize_triangles(trisIdx.data());
        tris += actual;
        nodes += nv;
        if (sm->has_normals() == TRUE) {
            normalCoords.resize(3 * (size_t)nv);
            sm->serialize_normals(normalCoords.data());
        } else {
            normalsOk = false;
        }
        if (sphereR > 0) {
            for (int i = 0; i + 2 < actual * 3; i += 3) {
                const int ia = 3 * trisIdx[i], ib = 3 * trisIdx[i + 1], ic = 3 * trisIdx[i + 2];
                double cx = (coords[ia] + coords[ib] + coords[ic]) / 3.0;
                double cy = (coords[ia + 1] + coords[ib + 1] + coords[ic + 1]) / 3.0;
                double cz = (coords[ia + 2] + coords[ib + 2] + coords[ic + 2]) / 3.0;
                double dev = std::fabs(std::sqrt(cx * cx + cy * cy + cz * cz) - sphereR);
                if (!row.sagValid || dev > row.sagMax) {
                    row.sagValid = true;
                    row.sagMax = dev;
                }
            }
        }
    }
    Clock::time_point t3 = Clock::now();

    row.tris = tris;
    row.nodes = nodes;
    row.readMs = msBetween(t2, t3);
    row.normals = normalsOk && tris > 0;
    return row;
}

void printRow(const BenchRow& r) {
    std::printf("%s,%s,%s,%d,%ld,%ld,%.3f,%.3f,%s,%s,%.6f\n", r.model, r.path, r.quality,
                r.faces, r.tris, r.nodes, r.facetMs, r.readMs, r.normals ? "yes" : "no",
                r.sagValid ? "est" : "-", r.sagValid ? r.sagMax : 0.0);
}

}  // namespace

int main() {
    int level = initialize_acis();
    if (level < 2) {
        terminate_acis(level);
        std::fprintf(stderr, "ACIS init failed (level=%d)\n", level);
        return 1;
    }

    struct ModelSpec {
        const char* name;
        double sphereR;  // >0 时启用 sag 估计
        BODY* body;
    };
    std::vector<ModelSpec> models;
    auto addModel = [&](const char* name, double sphereR, BODY* b) {
        if (b) models.push_back({name, sphereR, b});
        else std::fprintf(stderr, "[ERR] build model %s failed\n", name);
    };

    BODY* b = NULL;
    if (makeSphere(10, b)) addModel("sphere_r10", 10, b);
    b = NULL;
    if (makeCylinder(5, 20, b)) addModel("cyl_r5h20", 0, b);
    b = NULL;
    if (api_make_torus(8, 2, b).ok()) addModel("torus_R8r2", 0, b);
    b = NULL;
    if (makeHoledPlate(b)) addModel("plate_3holes", 0, b);

    std::printf("model,path,quality,faces,tris,nodes,facet_ms,read_ms,normals,sag,sag_max\n");

    for (const auto& m : models) {
        // A 现状默认档
        printRow(facetAndRead(m.body, NULL, m.name, "A", "default", m.sphereR, true));
        // B 可视化档位（coarse=0 / medium_fine=2 / fine=3）
        for (int q = 0; q < 3; ++q) {
            const char* names[3] = {"coarse", "medium_fine", "fine"};
            const face_quality_level levels[3] = {coarse, medium_fine, fine};
            facet_options_visualization vo;
            vo.set_face_quality(levels[q]);
            printRow(facetAndRead(m.body, &vo, m.name, "B", names[q], m.sphereR, true));
        }
        // C 精确容差（sag 控制）
        for (int c = 0; c < 3; ++c) {
            const double sags[3] = {0.1, 0.01, 0.001};
            char label[32];
            std::snprintf(label, sizeof(label), "sag=%.3f", sags[c]);
            facet_options_precise po;
            po.set_surface_tolerance(sags[c]);
            po.set_normal_tolerance(0.4);
            printRow(facetAndRead(m.body, &po, m.name, "C", label, m.sphereR, true));
        }
        // D 缓存复用：同参数（默认档）二次离散，不删 facet——facet 缓存是否免重算
        printRow(facetAndRead(m.body, NULL, m.name, "D", "cache-hit", m.sphereR, false));
        std::fflush(stdout);
    }

    ENTITY_LIST cleanup;
    for (const auto& m : models) cleanup.add(m.body);
    api_del_entity_list(cleanup);
    terminate_acis(level);
    return 0;
}
