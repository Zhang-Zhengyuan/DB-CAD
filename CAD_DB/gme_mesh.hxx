#pragma once
#include <vector>
#include "kernel/model_types.h"
enum DISPLAY_TYPES { DISPLAY_FACE=0, DISPLAY_EDGE, DISPLAY_ALL };
class GmeMesh {
public:
    struct FaceMesh {
        unsigned int numIndices=0, baseVertex=0, baseIndex=0;
        dbcad::Color color;
        dbcad::Bounds3 bounds;
    };
    struct EdgeMesh {
        unsigned int numIndices=0, baseVertex=0;
        dbcad::Color color;
        dbcad::Bounds3 bounds;
    };
    struct VertexMesh {
        unsigned int numIndices=0;
        dbcad::Color color;
        dbcad::Bounds3 bounds;
    };
    struct DisplayData {
        dbcad::ShapeHandle owner;
        std::vector<float> faceCoords, normalCoords, edgeCoords, vertexCoords;
        std::vector<int> triangles;
        std::vector<FaceMesh> faceMesh;
        std::vector<EdgeMesh> edgeMesh;
        std::vector<VertexMesh> vertexMesh;
        DISPLAY_TYPES displayType=DISPLAY_ALL;
    };
    GmeMesh(DisplayData* dd);
    ~GmeMesh();
private:
    DisplayData* m_data;
};
