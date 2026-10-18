#pragma once
// v3 GLB 序列化（V3_PLAN.md P0 收尾 / P1 worker 交付）：MeshData → 二进制 glTF 2.0。
//
// 布局契约（mesh_tests 结构校验依赖此布局）：
//  - 单 mesh 双 primitive：[0] TRIANGLES（POSITION/NORMAL），[1] LINES（POSITION，
//    边折线按相邻顶点对展开为线段索引）。
//  - BIN 缓冲区段顺序固定：positions | normals | indices(uint32) | edgePositions
//    | edgeIndices(uint32)，全部 float32/uint32 天然 4 字节对齐。
//  - 逐面拾取数据放 mesh.extras：faceRanges（对应 primitive[0].indices 下标区间）
//    与 edgeRuns（对应 edgePositions 顶点区间），避免每面一个 primitive。
// Qt-free。
#include "mesh_extractor.h"
#include <string>
#include <vector>

namespace dbcad {

class GlbWriter {
public:
    // 失败（空网格/非有限值/索引越界）返回空 vector 并置 *error。
    static std::vector<unsigned char> write(const MeshData& mesh, std::string* error = nullptr);
};

}  // namespace dbcad
