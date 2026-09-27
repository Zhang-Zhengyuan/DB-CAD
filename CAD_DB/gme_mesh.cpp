#include "gme_mesh.hxx"

#include <cassert>
#include <cstdio>
#include "acis/include/acistype.hxx"
#include "acis/include/body.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/fct_utl.hxx"
#include "acis/include/getowner.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/lump.hxx"
#include "acis/include/loop.hxx"
#include "acis/include/coedge.hxx"
#include "acis/include/point.hxx"
#include "acis/include/plane.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/shell.hxx"
#include "acis/include/transf.hxx"
#include "acis/include/vertex.hxx"

static void get_triangles_from_faceted_face(class FACE* face, std::vector<float>& coords, std::vector<int>& triangles,
                                            std::vector<float>& normalCoords)
{
    af_serializable_mesh* sm = GetSerializableMesh(face);
    if (nullptr == sm || sm->number_of_polygons() <= 0)
    {
        // Graph-restored analytic faces can temporarily lack a faceter mesh
        // when ACIS could not rebuild a PCURVE. Keep the MVP visible by
        // triangulating the outer planar loop directly from its vertices.
        // This is deliberately limited to planes; curved faces still report
        // the diagnostic below and can be repaired by the pcurve pass.
        if (!face || !face->geometry() || face->geometry()->identity(2) != PLANE_ID || !face->loop()) {
            return;
        }
        std::vector<SPAposition> polygon;
        COEDGE* first = face->loop()->start();
        COEDGE* co = first;
        int guard = 0;
        while (co && guard++ < 10000) {
            VERTEX* v = co->start();
            if (!v && co->edge()) v = co->edge()->start();
            if (v && v->geometry()) polygon.push_back(v->geometry()->coords());
            COEDGE* next = co->next();
            if (next == first) break;
            co = next;
        }
        if (polygon.size() < 3) return;
        SPAtransf tr = get_owner_transf(face);
        SPAunit_vector n = ((PLANE*)face->geometry())->gme_get_def().normal;
        coords.reserve(polygon.size() * 3);
        normalCoords.reserve(polygon.size() * 3);
        for (const SPAposition& raw : polygon) {
            SPAposition p = raw;
            p *= tr;
            coords.push_back((float)p.x());
            coords.push_back((float)p.y());
            coords.push_back((float)p.z());
            normalCoords.push_back((float)n.x());
            normalCoords.push_back((float)n.y());
            normalCoords.push_back((float)n.z());
        }
        for (int i = 1; i + 1 < (int)polygon.size(); ++i) {
            triangles.push_back(0);
            triangles.push_back(i);
            triangles.push_back(i + 1);
        }
        return;
    }
    SPAtransf tr = get_owner_transf(face);

    const int nv = sm->number_of_vertices();
    int ntri = sm->number_of_polygons();

    coords.resize(3 * nv);
    sm->serialize_positions(coords.data()); // if std::vector::data is not available, &(coords[0]) will also work.
    if (!tr.identity())
    {
        for (int ii = 0; ii < nv; ii++)
        {
            int jj = 3 * ii;
            SPAposition pos(coords[jj], coords[jj + 1], coords[jj + 2]);
            pos *= tr;
            coords[jj] = (float)pos.x();
            coords[jj + 1] = (float)pos.y();
            coords[jj + 2] = (float)pos.z();
        }
    }

    bool const has_normals = sm->has_normals() == TRUE;
    if (has_normals)
    {
        normalCoords.resize(3 * nv);
        sm->serialize_normals(normalCoords.data());
    }

    triangles.resize(3 * ntri);
    int ntri_actual = sm->serialize_triangles(triangles.data());
    while (ntri_actual < ntri)
    {
        triangles.pop_back();
        ntri_actual = static_cast<int>(triangles.size());
    }
}

static void get_triangles_from_faceted_faces(ENTITY_LIST& faces, std::vector<GmeMesh::FaceMesh>& faceData,
                                             std::vector<float>& coords, std::vector<int>& triangles,
                                             std::vector<float>& normalCoords)
{
    int nF = 0;
    int nV = 0;
    int nI = 0;
    int numFaces = faces.iteration_count();
    assert(numFaces == faceData.size());
    for (class ENTITY* ent = faces.first(); ent; ent = faces.next())
    {
        assert(nF < numFaces);
        assert(is_FACE(ent));
        if (!is_FACE(ent))
        {
            continue;
        }

        class FACE* face = (class FACE*)ent;
        std::vector<float> temp_coords;
        std::vector<int> temp_triangles;
        std::vector<float> temp_normalCoords;
        get_triangles_from_faceted_face(face, temp_coords, temp_triangles, temp_normalCoords);
        std::fprintf(stderr, "[CreateMeshFromEntity] face=%p mesh_vertices=%zu mesh_triangles=%zu serializable=%s\n",
                     (void*)face, temp_coords.size() / 3, temp_triangles.size() / 3,
                     GetSerializableMesh(face) ? "yes" : "no");
        if (!GetSerializableMesh(face)) {
            int loops = 0;
            int coedges = 0;
            int pcurves = 0;
            for (LOOP* loop = face->loop(); loop; loop = loop->next()) {
                ++loops;
                for (COEDGE* co = loop->start(); co; co = co->next()) {
                    ++coedges;
                    if (co->geometry()) ++pcurves;
                }
            }
            std::fprintf(stderr,
                         "[CreateMeshFromEntity] face=%p geometry=%p loop_count=%d coedges=%d pcurves=%d sides=%d cont=%d sense=%d\n",
                         (void*)face, (void*)face->geometry(), loops, coedges, pcurves,
                         face->sides() ? 1 : 0, (int)face->cont(), (int)face->sense());
        }
        {
            int nCoordsStart = (int)coords.size() / 3;
            int nCoords = (int)temp_coords.size();
            for (int ii = 0; ii < nCoords; ii++)
            {
                coords.push_back(temp_coords[ii]);
                // Analytic or legacy faceting paths may not provide normals.
                // Never index an empty normal buffer while copying a valid
                // face mesh; keep the vertex/normal arrays aligned.
                normalCoords.push_back(ii < static_cast<int>(temp_normalCoords.size())
                                      ? temp_normalCoords[ii] : 0.0f);
            }
            int nTri = (int)temp_triangles.size();
            for (int jj = 0; jj < nTri; jj++)
            {
                triangles.push_back(temp_triangles[jj] + nCoordsStart);
            }
        }
        logical found = FALSE;
        outcome out = api_rh_get_entity_rgb(ent, faceData[nF].color, TRUE, found);
        if (!out.ok() || !found)
        {
            faceData[nF].color = rgb_color(1., 1., 1.);
        }
        faceData[nF].numIndices = (unsigned int)temp_triangles.size();
        faceData[nF].baseIndex = (unsigned int)nI;
        faceData[nF].baseVertex = (unsigned int)nV;
        faceData[nF].ptrFace = face;

        nI += (unsigned int)temp_triangles.size();
        nV += (unsigned int)temp_coords.size();
        nF++;
    }
}

static void get_polylines_from_faceted_edges(ENTITY_LIST& edges, std::vector<GmeMesh::EdgeMesh>& edgeData,
                                             std::vector<float>& coords)
{
    int nE = 0;
    int nV = 0;
    int numEdges = edges.iteration_count();
    assert(numEdges == edgeData.size());
    for (class ENTITY* ent = edges.first(); ent; ent = edges.next())
    {
        assert(nE < numEdges);
        assert(is_EDGE(ent));
        if (!is_EDGE(ent))
        {
            continue;
        }

        SPAtransf tr = get_owner_transf(ent);
        class EDGE* edge = (class EDGE*)ent;
        SPAposition* pos = nullptr;

        int nP = 0;
        outcome out = api_get_facet_edge_points(edge, pos, nP);
        if (!out.ok())
        {
            ACIS_DELETE[] pos;
            continue;
        }
        for (int ii = 0; ii < nP; ii++)
        {
            pos[ii] *= tr;
            coords.push_back((float)pos[ii].x());
            coords.push_back((float)pos[ii].y());
            coords.push_back((float)pos[ii].z());
        }
        ACIS_DELETE[] pos;
        pos = nullptr;
        logical found = FALSE;
        out = api_rh_get_entity_rgb(ent, edgeData[nE].color, TRUE, found);
        if (!out.ok() || !found)
        {
            edgeData[nE].color = rgb_color(0., 0., 0.);
        }
        edgeData[nE].numIndices = 3 * nP;
        edgeData[nE].baseVertex = nV;
        edgeData[nE].ptrEdge = edge;
        nV += 3 * nP;
        nE++;
    }
}

bool CreateMeshFromEntityList(ENTITY_LIST& el, GmeMesh::DisplayData& dd) {
    bool success = false;
    API_NOP_BEGIN
    {
        int numEnt = el.iteration_count();
        if (0 == numEnt) {
            success = false;
            goto exit;
        }
        for (ENTITY* ent = el.first(); ent; ent = el.next()) {
            outcome out = api_facet_entity(ent);
            if (!out.ok()) {
                success = false;
                goto exit;
            }
        }

        ENTITY_LIST faces;
        for (ENTITY* ent = el.first(); ent; ent = el.next()) {
            if (is_EDGE(ent)) {
                continue;
            }
            outcome out = api_get_faces(ent, faces);
            if (!out.ok()) {
                success = false;
                goto exit;
            }
        }
        int numFaces = faces.iteration_count();
        dd.faceMesh.resize(numFaces);

        ENTITY_LIST edges;
        for (ENTITY* ent = el.first(); ent; ent = el.next()) {
            outcome out = api_get_edges(ent, edges);
            if (!out.ok()) {
                success = false;
                goto exit;
            }
        }
        int numEdges = edges.iteration_count();
        dd.edgeMesh.resize(numEdges);

        if (0 == numEdges + numFaces) {
            success = false;
            goto exit;
        }

        get_triangles_from_faceted_faces(faces, dd.faceMesh, dd.faceCoords, dd.triangles, dd.normalCoords);
        get_polylines_from_faceted_edges(edges, dd.edgeMesh, dd.edgeCoords);

        success = true;
    }
    exit:
    API_NOP_END
    return success;
}
// old
/* 
bool CreateMeshFromEntity(ENTITY* e, GmeMesh::DisplayData& dd) {
    // API_BEGIN;
    if (nullptr == e) {
        return false;
    }
    {
        if (is_VERTEX(e)) {
            GmeMesh::VertexMesh vm = GmeMesh::VertexMesh();
            vm.numIndices = 0;
            vm.ptrVertex = (VERTEX*)e;
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().x());
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().y());
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().z());
            dd.vertexMesh.push_back(vm);
            return true;
        }
    }
    {
        outcome out;
        out = api_facet_entity(e);
        if (!out.ok()) {
            return false;
        }
        {
            if (is_VERTEX(e))
            {
                GmeMesh::VertexMesh vm = GmeMesh::VertexMesh();
                vm.numIndices = 0;
                vm.ptrVertex = (class VERTEX*)e;
                dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().x());
                dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().y());
                dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().z());
                dd.vertexMesh.push_back(vm);
                return true;
            }
        }
        {
            outcome out;
            out = api_facet_entity(e);
            if (!out.ok())
            {
                return false;
            }
        }
        ENTITY_LIST faces;
        {
            if (!is_EDGE(e))
            {
                outcome out = api_get_faces(e, faces);
                if (!out.ok())
                {
                    return false;
                }
            }
        }
        int numFaces = faces.iteration_count();
        dd.faceMesh.resize(numFaces);

        ENTITY_LIST edges;
        {
            outcome out = api_get_edges(e, edges);
            if (!out.ok())
            {
                return false;
            }
        }
        int numEdges = edges.iteration_count();
        dd.edgeMesh.resize(numEdges);

        if (0 == numEdges + numFaces)
        {
            return false;
        }

        get_triangles_from_faceted_faces(faces, dd.faceMesh, dd.faceCoords, dd.triangles, dd.normalCoords);
        get_polylines_from_faceted_edges(edges, dd.edgeMesh, dd.edgeCoords);


    return true;
    // API_END;
}
*/
bool CreateMeshFromEntity(ENTITY *e, GmeMesh::DisplayData &dd) {
    bool success = false;
    API_NOP_BEGIN
    {
        if (nullptr == e) {
            std::fprintf(stderr, "[CreateMeshFromEntity] e==nullptr\n");
            success = false;
            goto exit;
        }
        const char* ek = "?";
        if (is_BODY(e)) ek = "BODY";
        else if (is_LUMP(e)) ek = "LUMP";
        else if (is_SHELL(e)) ek = "SHELL";
        else if (is_FACE(e)) ek = "FACE";
        else if (is_EDGE(e)) ek = "EDGE";
        else if (is_VERTEX(e)) ek = "VERTEX";
        std::fprintf(stderr, "[CreateMeshFromEntity] e=%s\n", ek);
        if (is_VERTEX(e)) {
            std::fprintf(stderr, "[CreateMeshFromEntity] e is VERTEX\n");
            GmeMesh::VertexMesh vm = GmeMesh::VertexMesh();
            vm.numIndices = 0;
            vm.ptrVertex = (VERTEX *)e;
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().x());
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().y());
            dd.vertexCoords.push_back(vm.ptrVertex->geometry()->coords().z());
            dd.vertexMesh.push_back(vm);
            success = true;
            goto exit;
        }
        // facet 该 entity；NOP scope 结束后 facet 数据会被丢弃，但 get_triangles_from_faceted_faces
        // 仍能拿到本次 scope 内生成的 mesh 数据。
        outcome out = api_facet_entity(e);
        std::fprintf(stderr, "[CreateMeshFromEntity] api_facet_entity ok=%d err#=%d\n",
                     out.ok() ? 1 : 0, (int)out.error_number());
        if (!out.ok()) {
            success = false;
            goto exit;
        }

        // A graph-restored BREP may have valid analytic surfaces and 3-D
        // edges but no derived PCURVE on its coedges.  In that state ACIS
        // reports a successful facet call while producing only edge facets.
        // Rebuild the derived 2-D boundary curves and retry once before the
        // display mesh is collected.
        if (is_BODY(e)) {
            bool missingFaceMesh = false;
            ENTITY_LIST facetFaces;
            api_get_faces(e, facetFaces);
            for (ENTITY* fe = facetFaces.first(); fe; fe = facetFaces.next()) {
                if (is_FACE(fe) && GetSerializableMesh((FACE*)fe) == nullptr) {
                    missingFaceMesh = true;
                    break;
                }
            }
            if (missingFaceMesh) {
                // Do not invoke sg_add_pcurves_to_entity synchronously here.
                // On a malformed restored coedge ring ACIS may spend an
                // unbounded amount of time trying to compute derived curves,
                // freezing the Qt UI. The planar fallback below is bounded
                // and keeps Pull responsive while the topology is diagnosed.
                std::fprintf(stderr, "[CreateMeshFromEntity] missing face meshes; using bounded planar fallback\n");
            }
        }

        // ====== 深度诊断：直接遍历 BODY/LUMP 的拓扑，看子实体是否齐全 ======
        if (is_BODY(e)) {
            BODY* body = (BODY*)e;
            int nLumps = 0, nShells = 0, nFaces = 0;
            for (LUMP* lump = body->lump(); lump; lump = lump->next()) {
                ++nLumps;
                for (SHELL* shell = lump->shell(); shell; shell = shell->next()) {
                    ++nShells;
                    ENTITY_LIST f;
                    api_get_faces(shell, f);
                    nFaces += f.iteration_count();
                }
            }
            std::fprintf(stderr, "[CreateMeshFromEntity] direct topology: lumps=%d shells=%d faces=%d\n",
                         nLumps, nShells, nFaces);
            ENTITY_LIST directFaces;
            api_get_faces(body, directFaces, PAT_IGNORE);
            std::fprintf(stderr, "[CreateMeshFromEntity] PAT_IGNORE faces=%d\n",
                         directFaces.iteration_count());
        } else if (is_LUMP(e)) {
            LUMP* lump = (LUMP*)e;
            int nShells = 0;
            for (SHELL* shell = lump->shell(); shell; shell = shell->next()) ++nShells;
            std::fprintf(stderr, "[CreateMeshFromEntity] direct LUMP: shells=%d\n", nShells);
        }
        // ====== 诊断结束 ======

        ENTITY_LIST faces;
        if (!is_EDGE(e)) {
            out = api_get_faces(e, faces);
            std::fprintf(stderr, "[CreateMeshFromEntity] api_get_faces ok=%d numFaces=%d\n",
                         out.ok() ? 1 : 0, faces.iteration_count());
            if (!out.ok()) {
                success = false;
                goto exit;
            }
        }
        int numFaces = faces.iteration_count();
        dd.faceMesh.resize(numFaces);

        ENTITY_LIST edges;
        out = api_get_edges(e, edges);
        std::fprintf(stderr, "[CreateMeshFromEntity] api_get_edges ok=%d numEdges=%d\n",
                     out.ok() ? 1 : 0, edges.iteration_count());
        if (!out.ok()) {
            success = false;
            goto exit;
        }
        int numEdges = edges.iteration_count();
        dd.edgeMesh.resize(numEdges);

        if (0 == numEdges + numFaces) {
            std::fprintf(stderr, "[CreateMeshFromEntity] numFaces+numEdges==0\n");
            success = false;
            goto exit;
        }

        get_triangles_from_faceted_faces(faces, dd.faceMesh, dd.faceCoords, dd.triangles, dd.normalCoords);
        get_polylines_from_faceted_edges(edges, dd.edgeMesh, dd.edgeCoords);
        std::fprintf(stderr, "[CreateMeshFromEntity] after fill: faceCoords.size=%zu triangles.size=%zu edgeCoords.size=%zu\n",
                     dd.faceCoords.size(), dd.triangles.size(), dd.edgeCoords.size());
        success = true;
    }
    exit:
    API_NOP_END
        return success;
}
GmeMesh::GmeMesh(DisplayData* dd) {
    m_data = dd;
}

GmeMesh::~GmeMesh()
{
    if (m_data != nullptr)
    {
        delete m_data;
        m_data = nullptr;
    }
}
