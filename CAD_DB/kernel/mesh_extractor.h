#pragma once
// v3 面片引擎（V3_PLAN.md §0.3 实现刷新 / P0_MESH_BENCH.md 选型 B 落地）。
//
// 取代旧 gme_mesh 离散化通道的设计要点（与旧实现的差异）：
//  - 质量档位枚举映射 facet_options_visualization::face_quality_level
//    （coarse=LOD / medium_fine=默认 / fine=高精）；每次离散前清旧 facet
//    （内核不跨档复用网格，见基准报告路径 D）。
//  - 法线一律取内核 facet 法线：P0 实测 visualization 档 32/32 恒有法线，
//    面缺网格或缺法线=显式错误；旧实现的"平面静默回退"已废除。
//  - 边折线走 api_get_facet_edge_points：实测返回局部坐标（af_api.hxx 声称
//    已应用宿主变换，但对本路径不成立），与面片一致乘恰好一次 get_owner_transf
//    （mesh_tests 的 transform 用例锁定该语义）。
//  - Qt-free：P1 headless worker 与 GLB 序列化直接复用，P6 去 Qt 不受影响。
// 线程模型：ACIS modeller 状态为进程级（ADR-2），调用方负责串行化。
#include <cstdint>
#include <string>
#include <vector>

class BODY;

namespace dbcad {

enum class MeshQuality : int { Coarse = 0, MediumFine = 1, Fine = 2 };
constexpr int kMeshQualityCount = 3;

// 单个 FACE 的三角形区间（MeshData::indices 中连续，firstIndex 按 indices
// 数组下标计，indexCount = 3 × 三角数）。
struct MeshFaceRange {
    std::uint32_t firstIndex = 0;
    std::uint32_t indexCount = 0;
};

// 单条边折线的顶点区间（MeshData::edgePositions 中连续的 line strip）。
struct MeshEdgeRun {
    std::uint32_t firstVertex = 0;
    std::uint32_t vertexCount = 0;
};

struct MeshData {
    std::vector<float> positions;             // xyz/顶点，展平
    std::vector<float> normals;               // 与 positions 等长，内核 facet 法线
    std::vector<std::uint32_t> indices;       // 三角形顶点索引（按面连续分块）
    std::vector<float> edgePositions;         // xyz/边折线顶点，展平
    std::vector<MeshEdgeRun> edges;
    std::vector<MeshFaceRange> faces;

    std::uint32_t vertexCount() const { return static_cast<std::uint32_t>(positions.size() / 3); }
    std::uint32_t triangleCount() const { return static_cast<std::uint32_t>(indices.size() / 3); }
    bool empty() const { return indices.empty(); }
};

struct MeshResult {
    bool ok = false;
    std::string error;                // !ok 时非空：面向 worker 的显式错误策略
    std::uint32_t edgesSkipped = 0;   // 无折线而被跳过的边（正常应为 0；非致命）
    MeshData mesh;
};

class MeshExtractor {
public:
    // 离散化并读取一个 BODY。失败路径全部显式（ok=false + error），不做静默
    // 几何兜底。进程内串行调用。
    static MeshResult extract(BODY& body, MeshQuality quality);
};

}  // namespace dbcad
