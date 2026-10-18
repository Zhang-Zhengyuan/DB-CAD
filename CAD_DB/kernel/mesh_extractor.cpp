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

}  // namespace

MeshResult MeshExtractor::extract(BODY& body, MeshQuality quality) {
    // 内核不跨质量档复用 facet（P0_MESH_BENCH.md 路径 D），先清旧网格。
    api_delete_entity_facets(&body, TRUE);

    facet_options_visualization options;
    options.set_face_quality(toAcisQuality(quality));
    if (!api_facet_entity(&body, &options).ok())
        return failure("api_facet_entity failed (quality=" + std::to_string(static_cast<int>(quality)) + ")");

    MeshResult result;
    MeshData& data = result.mesh;

    ENTITY_LIST faceList;
    api_get_faces(&body, faceList);
    if (faceList.count() == 0)
        return failure("body has no faces");

    std::vector<float> facePos, faceNrm;
    std::vector<int> triScratch;
    int faceNo = 0;
    for (ENTITY* ent = faceList.first(); ent; ent = faceList.next(), ++faceNo) {
        if (!is_FACE(ent))
            return failure("api_get_faces returned a non-FACE entity");
        FACE* face = static_cast<FACE*>(ent);

        af_serializable_mesh* sm = GetSerializableMesh(face);
        if (nullptr == sm || sm->number_of_polygons() <= 0)
            return failure("face " + std::to_string(faceNo) + " has no faceted mesh");
        if (sm->has_normals() != TRUE)
            return failure("face " + std::to_string(faceNo) + " faceted without kernel normals");

        const int nv = sm->number_of_vertices();
        const int np = sm->number_of_polygons();
        if (nv < 3 || np < 1)
            return failure("face " + std::to_string(faceNo) + " produced a degenerate mesh");

        const std::uint32_t baseVertex = data.vertexCount();
        facePos.assign(3 * static_cast<size_t>(nv), 0.0f);
        sm->serialize_positions(facePos.data());
        const SPAtransf tr = get_owner_transf(face);
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
        faceNrm.assign(3 * static_cast<size_t>(nv), 0.0f);
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

        // serialize_triangles 可能实际写入少于 np 个三角形（旧实现同样修剪）。
        triScratch.assign(3 * static_cast<size_t>(np), 0);
        const int actual = sm->serialize_triangles(triScratch.data());
        if (actual < 1)
            return failure("face " + std::to_string(faceNo) + " serialized no triangles");

        MeshFaceRange range;
        range.firstIndex = static_cast<std::uint32_t>(data.indices.size());
        for (int t = 0; t < actual; ++t) {
            const int local = triScratch[t];
            if (local < 0 || local >= nv)
                return failure("face " + std::to_string(faceNo) + " index out of range");
            data.indices.push_back(baseVertex + static_cast<std::uint32_t>(local));
        }
        range.indexCount = static_cast<std::uint32_t>(actual);
        data.faces.push_back(range);
        data.positions.insert(data.positions.end(), facePos.begin(), facePos.end());
        data.normals.insert(data.normals.end(), faceNrm.begin(), faceNrm.end());
    }

    // 边折线：api_get_facet_edge_points 返回局部坐标——af_api.hxx 声称宿主变换
    // 已应用，但对本路径实测不成立（mesh_tests 变换用例：平移体边线落在原点），
    // 故与面片一样乘 get_owner_transf，恰好一次。注意：完整球/环等闭曲面体
    // 可能无边拓扑（api_get_edges 为空），轮廓线由着色呈现，非缺陷。
    ENTITY_LIST edgeList;
    api_get_edges(&body, edgeList);
    for (ENTITY* ent = edgeList.first(); ent; ent = edgeList.next()) {
        if (!is_EDGE(ent))
            continue;
        EDGE* edge = static_cast<EDGE*>(ent);
        SPAposition* polyline = nullptr;
        int npts = 0;
        const outcome eout = api_get_facet_edge_points(edge, polyline, npts);
        if (!eout.ok() || nullptr == polyline || npts < 2) {
            ACIS_DELETE[] polyline;
            ++result.edgesSkipped;
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

    // 收尾自检：越界索引/尺寸失衡在此拦截，绝不产出消费端才炸的网格。
    const std::uint32_t vc = data.vertexCount();
    for (std::uint32_t index : data.indices)
        if (index >= vc)
            return failure("assembled mesh has an out-of-range index");
    if (data.normals.size() != data.positions.size())
        return failure("assembled mesh has misaligned normals");
    result.ok = true;
    return result;
}

}  // namespace dbcad
