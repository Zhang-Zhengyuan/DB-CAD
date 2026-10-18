// GlbWriter 实现：手写最小 glTF 2.0 JSON + BIN 拼装，无第三方 JSON 依赖
// （core 引入 nlohmann 是 P6 的事，见 V3_PLAN.md）。
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

}  // namespace

std::vector<unsigned char> GlbWriter::write(const MeshData& mesh, std::string* error) {
    const auto fail = [&](const char* message) {
        if (error) *error = message;
        return std::vector<unsigned char>();
    };
    if (mesh.positions.empty() || mesh.indices.empty())
        return fail("mesh has no triangles");
    if (mesh.normals.size() != mesh.positions.size())
        return fail("normals/positions size mismatch");
    const std::uint32_t vertexCount = mesh.vertexCount();
    for (float value : mesh.positions)
        if (!std::isfinite(value)) return fail("non-finite position");
    for (float value : mesh.normals)
        if (!std::isfinite(value)) return fail("non-finite normal");
    for (std::uint32_t index : mesh.indices)
        if (index >= vertexCount) return fail("index out of range");
    const bool hasEdges = !mesh.edgePositions.empty();

    // ---- BIN：按头文件声明的固定段顺序拼装 ------------------------------
    std::vector<unsigned char> bin;
    MinMax posRange;
    for (size_t i = 0; i + 2 < mesh.positions.size(); i += 3) {
        posRange.add(&mesh.positions[i]);
        appendF32(bin, mesh.positions[i]);
        appendF32(bin, mesh.positions[i + 1]);
        appendF32(bin, mesh.positions[i + 2]);
    }
    for (float value : mesh.normals) appendF32(bin, value);
    std::uint32_t indexMax = 0;
    for (std::uint32_t index : mesh.indices) {
        if (index > indexMax) indexMax = index;
        appendU32(bin, index);
    }
    MinMax edgeRange;
    std::vector<std::uint32_t> edgeIndices;
    std::uint32_t edgeIndexMax = 0;
    if (hasEdges) {
        for (size_t i = 0; i + 2 < mesh.edgePositions.size(); i += 3)
            edgeRange.add(&mesh.edgePositions[i]);
        for (float value : mesh.edgePositions) appendF32(bin, value);
        for (const MeshEdgeRun& run : mesh.edges) {
            for (std::uint32_t i = 1; i < run.vertexCount; ++i) {
                const std::uint32_t a = run.firstVertex + i - 1;
                const std::uint32_t b = run.firstVertex + i;
                edgeIndices.push_back(a);
                edgeIndices.push_back(b);
                if (b > edgeIndexMax) edgeIndexMax = b;
            }
        }
        for (std::uint32_t index : edgeIndices) appendU32(bin, index);
    }
    const std::uint32_t positionBytes = static_cast<std::uint32_t>(mesh.positions.size() * 4);
    const std::uint32_t normalBytes = static_cast<std::uint32_t>(mesh.normals.size() * 4);
    const std::uint32_t indexBytes = static_cast<std::uint32_t>(mesh.indices.size() * 4);
    const std::uint32_t edgePositionBytes =
        hasEdges ? static_cast<std::uint32_t>(mesh.edgePositions.size() * 4) : 0;
    // 块大小均为 4 的倍数，无需补齐即可满足 glTF 对齐要求。

    // ---- JSON -----------------------------------------------------------
    const std::uint32_t offsetNormals = positionBytes;
    const std::uint32_t offsetIndices = offsetNormals + normalBytes;
    const std::uint32_t offsetEdgePositions = offsetIndices + indexBytes;
    const std::uint32_t offsetEdgeIndices = offsetEdgePositions + edgePositionBytes;

    std::string json;
    json += "{\"asset\":{\"version\":\"2.0\",\"generator\":\"DB-CAD v3 MeshExtractor\"},";
    json += "\"scene\":0,\"scenes\":[{\"nodes\":[0]}],\"nodes\":[{\"mesh\":0,\"name\":\"component\"}],";

    json += "\"bufferViews\":[";
    auto view = [&](std::uint32_t idx, std::uint32_t offset, std::uint32_t bytes) {
        if (idx) json += ",";
        json += "{\"buffer\":0,\"byteOffset\":" + num(offset) + ",\"byteLength\":" + num(bytes) + "}";
    };
    view(0, 0, positionBytes);
    view(1, offsetNormals, normalBytes);
    view(2, offsetIndices, indexBytes);
    if (hasEdges) {
        view(3, offsetEdgePositions, edgePositionBytes);
        view(4, offsetEdgeIndices, static_cast<std::uint32_t>(edgeIndices.size() * 4));
    }
    json += "],";

    json += "\"accessors\":[";
    auto accessorHead = [&](std::uint32_t idx, std::uint32_t viewIdx, const char* type,
                            int componentType, std::uint32_t count) {
        if (idx) json += ",";
        json += "{\"bufferView\":" + num(viewIdx) + ",\"componentType\":" + num(componentType) +
                ",\"count\":" + num(count) + ",\"type\":\"" + type + "\"";
    };
    accessorHead(0, 0, "VEC3", 5126, vertexCount);
    json += ",\"min\":" + vec3(posRange.mn) + ",\"max\":" + vec3(posRange.mx) + "}";
    accessorHead(1, 1, "VEC3", 5126, vertexCount);
    json += "}";
    accessorHead(2, 2, "SCALAR", 5125, static_cast<std::uint32_t>(mesh.indices.size()));
    json += ",\"max\":" + num(indexMax) + "}";
    if (hasEdges) {
        const std::uint32_t edgeVertexCount =
            static_cast<std::uint32_t>(mesh.edgePositions.size() / 3);
        accessorHead(3, 3, "VEC3", 5126, edgeVertexCount);
        json += ",\"min\":" + vec3(edgeRange.mn) + ",\"max\":" + vec3(edgeRange.mx) + "}";
        accessorHead(4, 4, "SCALAR", 5125, static_cast<std::uint32_t>(edgeIndices.size()));
        json += ",\"max\":" + num(edgeIndexMax) + "}";
    }
    json += "],";

    json += "\"meshes\":[{\"primitives\":[";
    json += "{\"attributes\":{\"POSITION\":0,\"NORMAL\":1},\"indices\":2,\"mode\":4}";
    if (hasEdges)
        json += ",{\"attributes\":{\"POSITION\":3},\"indices\":4,\"mode\":1}";
    json += "],\"extras\":{";
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
