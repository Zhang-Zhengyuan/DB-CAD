#include "gme_mesh.hxx"

#include <cassert>
#include "acis/include/acistype.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/fct_utl.hxx"
#include "acis/include/getowner.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/point.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/transf.hxx"
#include "acis/include/vertex.hxx"

static void get_triangles_from_faceted_face(class FACE* face, std::vector<float>& coords, std::vector<int>& triangles,
                                            std::vector<float>& normalCoords)
{
    af_serializable_mesh* sm = GetSerializableMesh(face);
    if (nullptr == sm)
    {
        // Application decision: do we throw for unfaceted faces?
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
        {
            int nCoordsStart = (int)coords.size() / 3;
            int nCoords = (int)temp_coords.size();
            for (int ii = 0; ii < nCoords; ii++)
            {
                coords.push_back(temp_coords[ii]);
                normalCoords.push_back(temp_normalCoords[ii]);
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

bool CreateMeshFromEntityList(ENTITY_LIST& el, GmeMesh::DisplayData& dd)
{
    API_BEGIN;
        int numEnt = el.iteration_count();
        if (0 == numEnt)
        {
            return false;
        }
        {
            for (class ENTITY* ent = el.first(); ent; ent = el.next())
            {
                outcome out = api_facet_entity(ent);
                // outcome out = gme_api_facet_entity(ent);
                if (!out.ok())
                {
                    return false;
                }
            }
        }
        ENTITY_LIST faces;
        {
            for (class ENTITY* ent = el.first(); ent; ent = el.next())
            {
                if (is_EDGE(ent))
                {
                    continue;
                }
                outcome out = api_get_faces(ent, faces);
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
            for (class ENTITY* ent = el.first(); ent; ent = el.next())
            {
                outcome out = api_get_edges(ent, edges);
                if (!out.ok())
                {
                    return false;
                }
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
    API_END;
}

bool CreateMeshFromEntity(class ENTITY* e, GmeMesh::DisplayData& dd)
{
    API_BEGIN;
        if (nullptr == e)
        {
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
    API_END;
}

GmeMesh::GmeMesh(DisplayData* dd)
{
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
