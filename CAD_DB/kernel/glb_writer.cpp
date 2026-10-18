// GlbWriter 实现：手写最小 glTF 2.0 JSON + BIN 拼装，无第三方 JSON 依赖
// （core 引入 nlohmann 是 P6 的事，见 V3_PLAN.md）。
//
// 布局规则（mesh_tests 结构校验依赖）：按 positions | normals | indices |
// edgePositions | edgeIndices 的固定顺序拼 BIN，存在的段才出现；
// bufferView/accessor 编号按同顺序递增。三角形网格必有 TRIANGLES 图元；
// 有边折线必有 LINES 图元（纯线图元：仅 LINES）。
#include "glb_writer.h"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace dbcad {
namespace {

void appendU32(std::vector<unsigned char>& buffer, std::uint32_t value) {
    unsigned char bytes[4];
    std::memcpy(bytes, &value, 4);
    buffer.insert(buffer.end(), bytes, bytes + 4);
}

void appendF32(std::vector<unsigned char>& buffer, float value) {
    unsigned char bytes[4];
    std::memcpy(bytes, &value, 4);
    buffer.insert(buffer.end(), bytes, bytes + 4);
}

std::string num(double value) {
    char text[64];
    std::snprintf(text, sizeof(text), "%.9g", value);
    return text;
}

struct MinMax {
    double mn[3] = {1e300, 1e300, 1e300};
    double mx[3] = {-1e300, -1e300, -1e300};
    void add(const float* xyz) {
        for (int i = 0; i < 3; ++i) {
            if (xyz[i] < mn[i]) mn[i] = xyz[i];
            if (xyz[i] > mx[i]) mx[i] = xyz[i];
        }
    }
};

std::string vec3(const double v[3]) {
    return "[" + num(v[0]) + "," + num(v[1]) + "," + num(v[2]) + "]";
}

struct Section {
    std::uint32_t offset = 0;
    std::uint32_t bytes = 0;
};

}  // namespace

std::vector<unsigned char> GlbWriter::write(const MeshData& mesh, std::string* error) {
    const auto fail = [&](const char* message) {
        if (error) *error = message;
        return std::vector<unsigned char>();
    };
    const bool hasTriangles = !mesh.indices.empty();
    const bool hasEdges = !mesh.edgePositions.empty();
    if (!hasTriangles && !hasEdges)
        return fail("mesh has no triangles or edges");
    if (hasTriangles && mesh.normals.size() != mesh.positions.size())
        return fail("normals/positions size mismatch");
    if (hasTriangles && mesh.positions.empty())
        return fail("triangles without positions");
    const std::uint32_t vertexCount = mesh.vertexCount();
    for (float value : mesh.positions)
        if (!std::isfinite(value)) return fail("non-finite position");
    for (float value : mesh.normals)
        if (!std::isfinite(value)) return fail("non-finite normal");
    for (float value : mesh.edgePositions)
        if (!std::isfinite(value)) return fail("non-finite edge position");
    for (std::uint32_t index : mesh.indices)
        if (index >= vertexCount) return fail("index out of range");

    // ---- BIN：固定段顺序，存在的段才写入 ------------------------------
    std::vector<unsigned char> bin;
    Section positionSection, normalSection, indexSection, edgePositionSection, edgeIndexSection;
    MinMax posRange, edgeRange;
    std::uint32_t indexMax = 0;
    if (hasTriangles) {
        positionSection.offset = 0;
        positionSection.bytes = static_cast<std::uint32_t>(mesh.positions.size() * 4);
        for (size_t i = 0; i + 2 < mesh.positions.size(); i += 3) {
            posRange.add(&mesh.positions[i]);
            appendF32(bin, mesh.positions[i]);
            appendF32(bin, mesh.positions[i + 1]);
            appendF32(bin, mesh.positions[i + 2]);
        }
        normalSection.offset = positionSection.bytes;
        normalSection.bytes = static_cast<std::uint32_t>(mesh.normals.size() * 4);
        for (float value : mesh.normals) appendF32(bin, value);
        indexSection.offset = normalSection.offset + normalSection.bytes;
        indexSection.bytes = static_cast<std::uint32_t>(mesh.indices.size() * 4);
        for (std::uint32_t index : mesh.indices) {
            if (index > indexMax) indexMax = index;
            appendU32(bin, index);
        }
    }
    std::uint32_t edgeIndexCount = 0;
    std::uint32_t edgeIndexMax = 0;
    std::vector<std::uint32_t> edgeIndices;
    for (const MeshEdgeRun& run : mesh.edges) {
        for (std::uint32_t i = 1; i < run.vertexCount; ++i) {
            const std::uint32_t a = run.firstVertex + i - 1;
            const std::uint32_t b = run.firstVertex + i;
            edgeIndices.push_back(a);
            edgeIndices.push_back(b);
            if (b > edgeIndexMax) edgeIndexMax = b;
        }
    }
    if (hasEdges) {
        edgePositionSection.offset = static_cast<std::uint32_t>(bin.size());
        edgePositionSection.bytes = static_cast<std::uint32_t>(mesh.edgePositions.size() * 4);
        for (size_t i = 0; i + 2 < mesh.edgePositions.size(); i += 3)
            edgeRange.add(&mesh.edgePositions[i]);
        for (float value : mesh.edgePositions) appendF32(bin, value);
        edgeIndexSection.offset = edgePositionSection.offset + edgePositionSection.bytes;
        edgeIndexSection.bytes = static_cast<std::uint32_t>(edgeIndices.size() * 4);
        edgeIndexCount = static_cast<std::uint32_t>(edgeIndices.size());
        for (std::uint32_t index : edgeIndices) appendU32(bin, index);
    }

    // ---- JSON：bufferView 与 accessor 是两个独立数组，编号各自从 0 起 ----
    // （曾共用车一个全局计数器：primitive 引用越界 accessor → GLTFLoader
    // "reading 'bufferView'"，mesh_tests 已加交叉引用校验钉死。）
    std::string json;
    json += "{\"asset\":{\"version\":\"2.0\",\"generator\":\"DB-CAD v3 MeshExtractor\"},";
    json += "\"scene\":0,\"scenes\":[{\"nodes\":[0]}],\"nodes\":[{\"mesh\":0,\"name\":\"component\"}],";

    int viewPosition = -1, viewNormal = -1, viewIndex = -1, viewEdgePosition = -1, viewEdgeIndex = -1;
    int accPosition = -1, accNormal = -1, accIndex = -1, accEdgePosition = -1, accEdgeIndex = -1;
    std::string viewsJson;
    int nextView = 0;
    auto addView = [&](Section section) {
        int id = nextView++;
        if (!viewsJson.empty()) viewsJson += ",";
        viewsJson += "{\"buffer\":0,\"byteOffset\":" + num(section.offset) +
                     ",\"byteLength\":" + num(section.bytes) + "}";
        return id;
    };
    if (hasTriangles) {
        viewPosition = addView(positionSection);
        viewNormal = addView(normalSection);
        viewIndex = addView(indexSection);
    }
    if (hasEdges) {
        viewEdgePosition = addView(edgePositionSection);
        viewEdgeIndex = addView(edgeIndexSection);
    }
    json += "\"bufferViews\":[" + viewsJson + "],";

    std::string accessorsJson;
    int nextAccessor = 0;
    auto addAccessor = [&](int viewIdx, const char* type, int componentType, std::uint32_t count,
                           const char* extra) {
        int id = nextAccessor++;
        if (!accessorsJson.empty()) accessorsJson += ",";
        accessorsJson += "{\"bufferView\":" + num(viewIdx) + ",\"componentType\":" + num(componentType) +
                         ",\"count\":" + num(count) + ",\"type\":\"" + type + "\"" + extra + "}";
        return id;
    };
    if (hasTriangles) {
        accPosition = addAccessor(viewPosition, "VEC3", 5126, vertexCount,
                                  (",\"min\":" + vec3(posRange.mn) + ",\"max\":" + vec3(posRange.mx)).c_str());
        accNormal = addAccessor(viewNormal, "VEC3", 5126, vertexCount, "");
        accIndex = addAccessor(viewIndex, "SCALAR", 5125,
                               static_cast<std::uint32_t>(mesh.indices.size()),
                               (",\"max\":" + num(indexMax)).c_str());
    }
    if (hasEdges) {
        accEdgePosition = addAccessor(viewEdgePosition, "VEC3", 5126,
                                      static_cast<std::uint32_t>(mesh.edgePositions.size() / 3),
                                      (",\"min\":" + vec3(edgeRange.mn) + ",\"max\":" + vec3(edgeRange.mx)).c_str());
        accEdgeIndex = addAccessor(viewEdgeIndex, "SCALAR", 5125, edgeIndexCount,
                                   (",\"max\":" + num(edgeIndexMax)).c_str());
    }
    json += "\"accessors\":[" + accessorsJson + "],";

    std::string primitivesJson;
    if (hasTriangles)
        primitivesJson += "{\"attributes\":{\"POSITION\":" + num(accPosition) +
                          ",\"NORMAL\":" + num(accNormal) + "},\"indices\":" + num(accIndex) + ",\"mode\":4}";
    if (hasEdges) {
        if (!primitivesJson.empty()) primitivesJson += ",";
        primitivesJson += "{\"attributes\":{\"POSITION\":" + num(accEdgePosition) +
                          "},\"indices\":" + num(accEdgeIndex) + ",\"mode\":1}";
    }
    json += "\"meshes\":[{\"primitives\":[" + primitivesJson + "],\"extras\":{";
    json += "\"faceRanges\":[";
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        if (i) json += ",";
        json += "{\"firstIndex\":" + num(mesh.faces[i].firstIndex) +
                ",\"indexCount\":" + num(mesh.faces[i].indexCount) + "}";
    }
    json += "],\"edgeRuns\":[";
    for (size_t i = 0; i < mesh.edges.size(); ++i) {
        if (i) json += ",";
        json += "{\"firstVertex\":" + num(mesh.edges[i].firstVertex) +
                ",\"vertexCount\":" + num(mesh.edges[i].vertexCount) + "}";
    }
    json += "]}}],\"buffers\":[{\"byteLength\":" + num(bin.size()) + "}]}";
    while (json.size() % 4) json += ' ';
    while (bin.size() % 4) bin.push_back(0);

    // ---- GLB 容器 --------------------------------------------------------
    std::vector<unsigned char> glb;
    const std::uint32_t jsonLength = static_cast<std::uint32_t>(json.size());
    const std::uint32_t binLength = static_cast<std::uint32_t>(bin.size());
    const std::uint32_t total = 12 + 8 + jsonLength + 8 + binLength;
    appendU32(glb, 0x46546C67);  // 'glTF'
    appendU32(glb, 2);
    appendU32(glb, total);
    appendU32(glb, jsonLength);
    appendU32(glb, 0x4E4F534A);  // 'JSON'
    glb.insert(glb.end(), json.begin(), json.end());
    appendU32(glb, binLength);
    appendU32(glb, 0x004E4942);  // 'BIN'
    glb.insert(glb.end(), bin.begin(), bin.end());
    return glb;
}

}  // namespace dbcad
