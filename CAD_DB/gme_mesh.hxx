#pragma once

#include <vector>

#include "acis/include/rgbcolor.hxx"

class ENTITY_LIST;
class FACE;
class EDGE;
class VERTEX;

// 显示类型
enum DISPLAY_TYPES { DISPLAY_FACE = 0, DISPLAY_EDGE, DISPLAY_ALL };

class GmeMesh {
public:
    struct FaceMesh {
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        FACE* ptrFace;
        rgb_color color;
        FaceMesh() {
            numIndices = 0;
            baseVertex = 0;
            baseIndex = 0;
            color = 0;
            ptrFace = nullptr;
        }
    };
    struct EdgeMesh {
        unsigned int numIndices;
        unsigned int baseVertex;
        rgb_color color;
        EDGE* ptrEdge;
        EdgeMesh() {
            numIndices = 0;
            baseVertex = 0;
            color = 0;
            ptrEdge = nullptr;
        }
    };
    struct VertexMesh {
        unsigned int numIndices;
        rgb_color color;
        VERTEX* ptrVertex;
        VertexMesh() {
            numIndices = 0;
            color = 0;
            ptrVertex = nullptr;
        }
    };

    struct DisplayData {
        std::vector<float> faceCoords;
        std::vector<int> triangles;
        std::vector<float> normalCoords;
        std::vector<float> edgeCoords;
        std::vector<float> vertexCoords;
        std::vector<GmeMesh::FaceMesh> faceMesh;
        std::vector<GmeMesh::EdgeMesh> edgeMesh;
        std::vector<GmeMesh::VertexMesh> vertexMesh;
        DISPLAY_TYPES displayType = DISPLAY_TYPES::DISPLAY_ALL;
    };
    // 获得DisplayData
    GmeMesh(DisplayData* dd);
    ~GmeMesh();

private:
    int m_level;
    DisplayData* m_data;
};