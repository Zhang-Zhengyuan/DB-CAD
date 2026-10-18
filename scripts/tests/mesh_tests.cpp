// v3 面片引擎验证工具（V3_PLAN.md P0 收尾：MeshExtractor + GlbWriter）。
//
// 真实 ACIS 内核上的不变量与回归锁定（core_unit_tests 用 FakeKernel，覆盖不到这层）：
//  1. 五个代表模型 × 三档质量：提取成功、法线恒在、索引合法、面数与内核一致、
//     边折线完整（无跳边）、三角形数随档位单调不降。
//  2. 包围盒：提取顶点落在 api_get_entity_box 之内（容差 eps）。
//  3. 变换语义：非烘焙平移（api_transform_entity，BODY 存变换属性）后面片与
//     边折线必须同处平移后位置——面片乘 get_owner_transf、边线信任 API 已变换，
//     锁死旧 gme_mesh 的边线双重变换 bug 不再复发。
//  4. GLB 结构：glTF 2.0 容器头/块结构、固定 BIN 布局与 MeshData 逐字节一致、
//     faceRanges 计数闭合、空网格显式报错。
//
// 构建：MSBuild CAD_DB.vcxproj /p:DBCADMeshTests=true /p:Configuration=Debug /p:Platform=x64
// 输出：PASS 行到 stdout，失败抛错退出码 1。
#include "kernel/mesh_extractor.h"
#include "kernel/glb_writer.h"

#include "acis.hxx"

#include "acis/include/acistype.hxx"
#include "acis/include/body.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/transf.hxx"
#include "acis/include/transfrm.hxx"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dbcad;

namespace {

int checks = 0;

void expect(bool value, const std::string& message) {
    if (!value) throw std::runtime_error("FAIL " + message);
    ++checks;
    std::printf("PASS %s\n", message.c_str());
    std::fflush(stdout);
}

struct BBox {
    double mn[3] = {1e300, 1e300, 1e300};
    double mx[3] = {-1e300, -1e300, -1e300};
    void add(const float* xyz) {
        for (int i = 0; i < 3; ++i) {
            if (xyz[i] < mn[i]) mn[i] = xyz[i];
            if (xyz[i] > mx[i]) mx[i] = xyz[i];
        }
    }
    double center(int axis) const { return (mn[axis] + mx[axis]) / 2; }
};

BBox bboxOf(const std::vector<float>& positions) {
    BBox box;
    for (size_t i = 0; i + 2 < positions.size(); i += 3) box.add(&positions[i]);
    return box;
}

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

std::uint32_t edgeVertices(const MeshData& mesh) {
    std::uint32_t total = 0;
    for (const MeshEdgeRun& run : mesh.edges) total += run.vertexCount;
    return total;
}

// ---- 用例 ------------------------------------------------------------

void testExtractLadder(const char* name, BODY& body) {
    const MeshQuality qualities[kMeshQualityCount] = {MeshQuality::Coarse, MeshQuality::MediumFine,
                                                      MeshQuality::Fine};
    std::uint32_t previousTris = 0;
    ENTITY_LIST faceList;
    api_get_faces(&body, faceList);
    const int kernelFaces = faceList.count();
    ENTITY_LIST edgeList;
    api_get_edges(&body, edgeList);
    const int kernelEdges = edgeList.count();  // 完全球/环等闭曲面体合法为 0
    for (int q = 0; q < kMeshQualityCount; ++q) {
        MeshResult result = MeshExtractor::extract(body, qualities[q]);
        const std::string tag = std::string(name) + " q" + std::to_string(q);
        expect(result.ok, tag + ": extract ok (" + result.error + ")");
        const MeshData& mesh = result.mesh;
        expect(!mesh.positions.empty() && !mesh.indices.empty(), tag + ": non-empty triangles");
        expect(mesh.normals.size() == mesh.positions.size(), tag + ": kernel normals present");
        expect(mesh.faces.size() == static_cast<size_t>(kernelFaces), tag + ": face count matches kernel");
        expect(result.edgesSkipped == 0, tag + ": no skipped edges");
        // 边折线与拓扑一致：有边的体必须有折线；无边拓扑（闭曲面）折线为空。
        expect(mesh.edges.size() == static_cast<size_t>(kernelEdges),
                tag + ": edge runs match kernel topology");
        if (kernelEdges > 0) expect(edgeVertices(mesh) > 0, tag + ": edge polylines present");
        bool indicesOk = true;
        const std::uint32_t vc = mesh.vertexCount();
        for (std::uint32_t index : mesh.indices)
            if (index >= vc) indicesOk = false;
        expect(indicesOk, tag + ": indices in range");
        expect(mesh.triangleCount() >= previousTris, tag + ": quality ladder monotonic");
        previousTris = mesh.triangleCount();
        // 精确回归：平面立方体 = 6 面 x 2 三角。曾因 serialize_triangles
        // 返回值语义误读（三角形数 vs 索引数）少算 2/3，mesh_tests 当时
        // 内部自洽未察觉——worker 集成才暴露，此处钉死。
        if (std::string(name) == "cuboid" && q == 1)
            expect(mesh.triangleCount() == 12, "cuboid exact triangle count (6 faces x 2)");
        // faceRanges 计数闭合
        std::uint64_t sum = 0;
        for (const MeshFaceRange& range : mesh.faces) sum += range.indexCount;
        expect(sum == mesh.indices.size(), tag + ": faceRanges cover all indices");
    }
}

void testBBox(const char* name, BODY& body) {
    MeshResult result = MeshExtractor::extract(body, MeshQuality::MediumFine);
    expect(result.ok, std::string(name) + ": bbox extract ok");
    SPAposition boxMin, boxMax;
    if (!api_get_entity_box(&body, boxMin, boxMax).ok())
        throw std::runtime_error("FAIL api_get_entity_box");
    const BBox mesh = bboxOf(result.mesh.positions);
    const double eps = 1e-3;
    const double boxMinAxis[3] = {boxMin.x(), boxMin.y(), boxMin.z()};
    const double boxMaxAxis[3] = {boxMax.x(), boxMax.y(), boxMax.z()};
    bool inside = true;
    for (int i = 0; i < 3; ++i) {
        if (mesh.mn[i] < boxMinAxis[i] - eps) inside = false;
        if (mesh.mx[i] > boxMaxAxis[i] + eps) inside = false;
    }
    expect(inside, std::string(name) + ": mesh vertices inside entity box");
}

void testTransformSemantics() {
    BODY* body = NULL;
    // 圆柱有真实边拓扑（两条圆边），同时覆盖面片与边折线两条变换路径。
    if (!api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 4), 2, 2, 2, NULL, body).ok())
        throw std::runtime_error("FAIL make cylinder");
    // 与内核 translate(bake=false) 同路径：BODY 存变换属性而非烘焙几何。
    if (!api_transform_entity(body, translate_transf(SPAvector(100, 50, 0))).ok())
        throw std::runtime_error("FAIL api_transform_entity");
    MeshResult result = MeshExtractor::extract(*body, MeshQuality::MediumFine);
    expect(result.ok, "transform: extract ok (" + result.error + ")");
    expect(result.mesh.edges.size() == 2, "transform: cylinder has two edge rings");

    const double eps = 0.05;
    const BBox fb = bboxOf(result.mesh.positions);
    expect(std::fabs(fb.center(0) - 100) < eps && std::fabs(fb.center(1) - 50) < eps &&
               std::fabs(fb.center(2) - 2) < eps,
           "transform: face vertices follow body transform");
    const BBox eb = bboxOf(result.mesh.edgePositions);
    expect(std::fabs(eb.center(0) - 100) < eps && std::fabs(eb.center(1) - 50) < eps &&
               std::fabs(eb.center(2) - 2) < eps,
           "transform: edge polyline single-transform (no double shift)");
    api_del_entity(body);
}

std::uint32_t readU32(const std::vector<unsigned char>& bytes, size_t offset) {
    std::uint32_t value = 0;
    std::memcpy(&value, bytes.data() + offset, 4);
    return value;
}

float readF32(const std::vector<unsigned char>& bytes, size_t offset) {
    float value = 0;
    std::memcpy(&value, bytes.data() + offset, 4);
    return value;
}

void testGlbStructure(BODY& body) {
    MeshResult result = MeshExtractor::extract(body, MeshQuality::MediumFine);
    expect(result.ok, "glb: extract ok");
    const MeshData& mesh = result.mesh;

    std::string error;
    const std::vector<unsigned char> glb = GlbWriter::write(mesh, &error);
    expect(!glb.empty(), "glb: write ok (" + error + ")");
    expect(glb.size() >= 20 && readU32(glb, 0) == 0x46546C67 && readU32(glb, 4) == 2,
           "glb: header magic/version");
    expect(readU32(glb, 8) == glb.size(), "glb: total length matches file size");

    const std::uint32_t jsonLen = readU32(glb, 12);
    expect(readU32(glb, 16) == 0x4E4F534A, "glb: JSON chunk tag");
    const std::string json(reinterpret_cast<const char*>(glb.data()) + 20, jsonLen);
    const size_t binHeader = 20 + jsonLen;
    const std::uint32_t binLen = readU32(glb, binHeader);
    expect(readU32(glb, binHeader + 4) == 0x004E4942, "glb: BIN chunk tag");
    const unsigned char* bin = glb.data() + binHeader + 8;
    expect(binHeader + 8 + binLen == glb.size(), "glb: BIN length closes the file");

    expect(json.find("\"generator\":\"DB-CAD v3 MeshExtractor\"") != std::string::npos &&
               json.find("\"mode\":4") != std::string::npos && json.find("\"mode\":1") != std::string::npos,
           "glb: JSON declares generator + tri/lines primitives");
    expect(json.find("\"faceRanges\":[") != std::string::npos &&
               json.find("\"edgeRuns\":[") != std::string::npos,
           "glb: extras carry faceRanges/edgeRuns");

    // 固定 BIN 布局逐字节校验（布局是 GlbWriter 的公开契约，见 glb_writer.h）。
    const size_t positionBytes = mesh.positions.size() * 4;
    const size_t normalBytes = mesh.normals.size() * 4;
    const size_t indexBytes = mesh.indices.size() * 4;
    bool positionsMatch = true, normalsMatch = true, indicesMatch = true;
    for (size_t i = 0; i < mesh.positions.size(); ++i)
        if (readF32(glb, binHeader + 8 + 4 * i) != mesh.positions[i]) positionsMatch = false;
    for (size_t i = 0; i < mesh.normals.size(); ++i)
        if (readF32(glb, binHeader + 8 + positionBytes + 4 * i) != mesh.normals[i]) normalsMatch = false;
    for (size_t i = 0; i < mesh.indices.size(); ++i)
        if (readU32(glb, binHeader + 8 + positionBytes + normalBytes + 4 * i) != mesh.indices[i])
            indicesMatch = false;
    expect(positionsMatch && normalsMatch && indicesMatch, "glb: BIN layout matches MeshData");
    expect(binLen >= positionBytes + normalBytes + indexBytes, "glb: BIN length covers buffers");

    // 空网格显式报错，不产出静默空文件。
    const MeshData empty;
    std::string emptyError = "untouched";
    expect(GlbWriter::write(empty, &emptyError).empty() && emptyError != "untouched",
           "glb: empty mesh rejected with explicit error");
}

}  // namespace

int main() {
    const int level = initialize_acis();
    if (level < 2) {
        terminate_acis(level);
        std::fprintf(stderr, "ACIS init failed (level=%d)\n", level);
        return 1;
    }

    struct Model {
        const char* name;
        BODY* body;
    };
    std::vector<Model> models;
    BODY* b = NULL;
    if (api_make_cuboid(10, 12, 8, b).ok()) models.push_back({"cuboid", b});
    b = NULL;
    if (api_make_sphere(10, b).ok()) models.push_back({"sphere_r10", b});
    b = NULL;
    if (api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 20), 5, 5, 5, NULL, b).ok())
        models.push_back({"cyl_r5h20", b});
    b = NULL;
    if (api_make_torus(8, 2, b).ok()) models.push_back({"torus_R8r2", b});
    b = NULL;
    if (makeHoledPlate(b)) models.push_back({"plate_3holes", b});
    expect(models.size() == 5, "all five bench models built");

    int failures = 0;
    try {
        for (const Model& model : models) {
            testExtractLadder(model.name, *model.body);
            testBBox(model.name, *model.body);
        }
        testTransformSemantics();
        testGlbStructure(*models[4].body);  // plate_3holes：有边拓扑，覆盖 LINES 图元
    } catch (const std::exception& e) {
        std::printf("%s\n", e.what());
        std::fflush(stdout);
        failures = 1;
    }

    ENTITY_LIST cleanup;
    for (const Model& model : models) cleanup.add(model.body);
    api_del_entity_list(cleanup);
    terminate_acis(level);
    if (failures) return 1;
    std::printf("mesh_tests: %d checks passed\n", checks);
    return 0;
}
