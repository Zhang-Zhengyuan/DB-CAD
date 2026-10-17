#pragma once
#include "model_types.h"
#include "../gme_mesh.hxx"

// Business identity, dependencies and presentation settings never contain SDK objects.
struct ENTITY_TREE_ITEM {
    std::string uuid;
    std::string name = "Unknown";
    int index = 0;
    dbcad::ShapeHandle shape;
    OPERATOR_TYPES operatorType = OPERATOR_CONSTRUCTOR;
    dbcad::Translation trans;
    int subOperatorType = 0;
    dbcad::HandleGrid handles;
    std::vector<int> index_base;
    // All components of one operation are rebuilt together, once.
    int rebuildGroup = -1;
    bool isAutoUpdate = true;
    std::vector<int> index_support;
    GmeMesh::DisplayData* ptrDisplayData = nullptr;
    bool visible = false;
    DISPLAY_TYPES displayType = DISPLAY_ALL;
    bool link_handles = true;
};
