// MeshExtractor 实现：档位离散 + af_serializable_mesh 读取 + 边折线。
// 头文件见 mesh_extractor.h（设计说明与旧实现差异清单）。
#include "mesh_extractor.h"

#include "acis.hxx"

#include "acis/include/acistype.hxx"
#include "acis/include/body.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/getowner.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/transf.hxx"

#include "acis/include/af_api.hxx"                // api_facet_entity / api_get_facet_edge_points
#include "acis/include/af_serializable_mesh.hxx"  // GetSerializableMesh
#include "acis/include/facet_options.hxx"         // facet_options_visualization

#include <vector>

namespace dbcad {
namespace {

face_quality_level toAcisQuality(MeshQuality quality) {
    switch (quality) {
    case MeshQuality::Coarse:
        return coarse;
    case MeshQuality::Fine:
        return fine;
    case MeshQuality::MediumFine:
    default:
        return medium_fine;
    }
}

MeshResult failure(std::string what) {
    MeshResult result;
    result.error = std::move(what);
    return result;
}

// facet_options_visualization 拷贝构造私有：调用点就地构造 + 本函数设档。
void setVisualizationQuality(facet_options_visualization& options, MeshQuality quality) {
    options.set_face_quality(toAcisQuality(quality));
}

// 读取一个已离散 FACE 的网格并追加到 data。失败返回 false 并置 error。
bool readFacetedFace(FACE& face, int faceNo, MeshData& data, std::string& error) {
    af_serializable_mesh* sm = GetSerializableMesh(&face);
    if (nullptr == sm || sm->number_of_polygons() <= 0) {
        error = "face " + std::to_string(faceNo) + " has no faceted mesh";
        return false;
    }
    if (sm->has_normals() != TRUE) {
        error = "face " + std::to_string(faceNo) + " faceted without kernel normals";
        return false;
    }

    const int nv = sm->number_of_vertices();
    const int np = sm->number_of_polygons();
    if (nv < 3 || np < 1) {
        error = "face " + std::to_string(faceNo) + " produced a degenerate mesh";
        return false;
    }

    const std::uint32_t baseVertex = data.vertexCount();
    std::vector<float> facePos(3 * static_cast<size_t>(nv), 0.0f);
    sm->serialize_positions(facePos.data());
    const SPAtransf tr = get_owner_transf(&face);
    if (!tr.identity()) {
        // serialize_positions 输出局部坐标；法线跟随变换的旋转部分，
        // 均匀缩放后重新归一。
        for (int i = 0; i < nv; ++i) {
            SPAposition p(facePos[3 * i], facePos[3 * i + 1], facePos[3 * i + 2]);
            p *= tr;
            facePos[3 * i] = static_cast<float>(p.x());
            facePos[3 * i + 1] = static_cast<float>(p.y());
            facePos[3 * i + 2] = static_cast<float>(p.z());
        }
    }
    std::vector<float> faceNrm(3 * static_cast<size_t>(nv), 0.0f);
    sm->serialize_normals(faceNrm.data());
    if (!tr.identity()) {
        for (int i = 0; i < nv; ++i) {
            SPAvector v(faceNrm[3 * i], faceNrm[3 * i + 1], faceNrm[3 * i + 2]);
            v *= tr;
            const double len = v.len();
            if (len > 1e-12) {
                faceNrm[3 * i] = static_cast<float>(v.x() / len);
                faceNrm[3 * i + 1] = static_cast<float>(v.y() / len);
                faceNrm[3 * i + 2] = static_cast<float>(v.z() / len);
            }
        }
    }

    // serialize_triangles 返回实际三角形数（可少于 np），写入 3×该值个索引。
    std::vector<int> triScratch(3 * static_cast<size_t>(np), 0);
    const int actualTris = sm->serialize_triangles(triScratch.data());
    if (actualTris < 1) {
        error = "face " + std::to_string(faceNo) + " serialized no triangles";
        return false;
    }

    MeshFaceRange range;
    range.firstIndex = static_cast<std::uint32_t>(data.indices.size());
    for (int t = 0; t < 3 * actualTris; ++t) {
        const int local = triScratch[t];
        if (local < 0 || local >= nv) {
            error = "face " + std::to_string(faceNo) + " index out of range";
            return false;
        }
        data.indices.push_back(baseVertex + static_cast<std::uint32_t>(local));
    }
    range.indexCount = static_cast<std::uint32_t>(3 * actualTris);
    data.faces.push_back(range);
    data.positions.insert(data.positions.end(), facePos.begin(), facePos.end());
    data.normals.insert(data.normals.end(), faceNrm.begin(), faceNrm.end());
    return true;
}

// 边折线：api_get_facet_edge_points 返回局部坐标——af_api.hxx 声称宿主变换
// 已应用，但对本路径实测不成立（mesh_tests 变换用例：平移体边线落在原点），
// 故与面片一样乘 get_owner_transf，恰好一次。
void readEdges(ENTITY_LIST& edgeList, MeshData& data, std::uint32_t& skipped) {
    for (ENTITY* ent = edgeList.first(); ent; ent = edgeList.next()) {
        if (!is_EDGE(ent))
            continue;
        EDGE* edge = static_cast<EDGE*>(ent);
        SPAposition* polyline = nullptr;
        int npts = 0;
        const outcome eout = api_get_facet_edge_points(edge, polyline, npts);
        if (!eout.ok() || nullptr == polyline || npts < 2) {
            ACIS_DELETE[] polyline;
            ++skipped;
            continue;
        }
        const SPAtransf tr = get_owner_transf(edge);
        MeshEdgeRun run;
        run.firstVertex = static_cast<std::uint32_t>(data.edgePositions.size() / 3);
        for (int i = 0; i < npts; ++i) {
            SPAposition p = polyline[i];
            p *= tr;
            data.edgePositions.push_back(static_cast<float>(p.x()));
            data.edgePositions.push_back(static_cast<float>(p.y()));
            data.edgePositions.push_back(static_cast<float>(p.z()));
        }
        run.vertexCount = static_cast<std::uint32_t>(npts);
        data.edges.push_back(run);
        ACIS_DELETE[] polyline;
    }
}

void finalizeOrValidate(MeshResult& result) {
    MeshData& data = result.mesh;
    const std::uint32_t vc = data.vertexCount();
    for (std::uint32_t index : data.indices)
        if (index >= vc) {
            result.ok = false;
            result.error = "assembled mesh has an out-of-range index";
            return;
        }
    if (data.normals.size() != data.positions.size()) {
        result.ok = false;
        result.error = "assembled mesh has misaligned normals";
        return;
    }
    result.ok = true;
}

}  // namespace

MeshResult MeshExtractor::extract(BODY& body, MeshQuality quality) {
    // 内核不跨质量档复用 facet（P0_MESH_BENCH.md 路径 D），先清旧网格。
    api_delete_entity_facets(&body, TRUE);
    facet_options_visualization options;
    setVisualizationQuality(options, quality);
    if (!api_facet_entity(&body, &options).ok())
        return failure("api_facet_entity failed (quality=" + std::to_string(static_cast<int>(quality)) + ")");

    MeshResult result;
    MeshData& data = result.mesh;

    ENTITY_LIST faceList;
    api_get_faces(&body, faceList);
    if (faceList.count() == 0)
        return failure("body has no faces");

    int faceNo = 0;
    for (ENTITY* ent = faceList.first(); ent; ent = faceList.next(), ++faceNo) {
        if (!is_FACE(ent))
            return failure("api_get_faces returned a non-FACE entity");
        if (!readFacetedFace(*static_cast<FACE*>(ent), faceNo, data, result.error))
            return failure(result.error);
    }

    // 完整球/环等闭曲面体可能无边拓扑（api_get_edges 为空），轮廓线由着色
    // 呈现，非缺陷。
    ENTITY_LIST edgeList;
    api_get_edges(&body, edgeList);
    readEdges(edgeList, data, result.edgesSkipped);

    finalizeOrValidate(result);
    return result;
}

MeshResult MeshExtractor::extractEntity(ENTITY& entity, MeshQuality quality) {
    if (is_BODY(&entity))
        return extract(*static_cast<BODY*>(&entity), quality);

    facet_options_visualization options;
    setVisualizationQuality(options, quality);
    if (is_FACE(&entity)) {
        FACE& face = *static_cast<FACE*>(&entity);
        api_delete_entity_facets(&face, TRUE);
        if (!api_facet_entity(&face, &options).ok())
            return failure("api_facet_entity(face) failed");
        MeshResult result;
        if (!readFacetedFace(face, 0, result.mesh, result.error))
            return failure(result.error);
        ENTITY_LIST edgeList;
        api_get_edges(&face, edgeList);
        readEdges(edgeList, result.mesh, result.edgesSkipped);
        finalizeOrValidate(result);
        return result;
    }
    if (is_EDGE(&entity)) {
        EDGE& edge = *static_cast<EDGE*>(&entity);
        api_delete_entity_facets(&edge, TRUE);
        if (!api_facet_entity(&edge, &options).ok())
            return failure("api_facet_entity(edge) failed");
        MeshResult result;
        ENTITY_LIST edgeList;
        edgeList.add(&edge);
        readEdges(edgeList, result.mesh, result.edgesSkipped);
        if (result.mesh.edges.empty())
            return failure("edge produced no polyline");
        result.ok = true;  // 纯线图元：无三角形/法线是正常形态
        return result;
    }
    return failure("unsupported entity type for mesh extraction");
}

}  // namespace dbcad
