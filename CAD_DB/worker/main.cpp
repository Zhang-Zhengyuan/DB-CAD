// v3 几何 worker（V3_PLAN.md ADR-1/ADR-2 · P1/P2 交付）：无 Qt 控制台进程。
//
// P1 形态：批处理 CLI（import/merge/export/selftest），网关按次调用。
// P2 形态：serve 常驻模式——stdin/stdout JSON 行协议，网关每项目一会话：
//   → {"id":1,"op":"ping"}
//   → {"id":2,"op":"open","spool":"<dir>","components":[{"sid":"c0","file":"c0.sat"}]}
//   → {"id":3,"op":"exec","command":{"action":"component.create","params":{...}}}
//   ← {"id":3,"ok":true,"result":{"added":[...],"modified":[...],"removed":[...]}}
//   → {"id":4,"op":"close"}（进程随之退出）
//  exec 语义见 worker/session.h；命令失败回 ok:false+error，会话由网关重建。
//
// 批处理命令：
//   dbcad_worker import --sat <path> --outdir <dir>
//   dbcad_worker merge --sat-list <a,b> --out <p>
//   dbcad_worker export --sat-list <a,b> --glb-out <p>
//   dbcad_worker selftest
// 退出码：0 成功 / 1 失败（stderr 带原因）。错误显式，不静默兜底（v3 原则）。
#include "kernel/mesh_extractor.h"
#include "kernel/glb_writer.h"
#include "entity_graph_codec.h"
#include "session.h"

// STEP 导入（P9）：Spatial InterOp（授权包 _deps/interop）。头存在才编入；
// 缺包时 import 对 .step/.stp 显式报错（错误显式，不静默）。
#if __has_include("SPAIDocument.h")
#define DBCAD_HAS_INTEROP 1
#include "SPAIAcisDocument.h"
#include "SPAIConverter.h"
#include "SPAIFile.h"
#include "SPAIOptions.h"
#include "SPAIResult.h"
#include "SPAISystem.h"
#include "SPAIOptionName.h"
#include "SPAIValue.h"
#include "SPAIDocument.h"
#endif

#include "vendor/nlohmann/json.hpp"

#include "acis.hxx"

#include <iostream>
#include <memory>

#include "acis/include/acistype.hxx"
#include "acis/include/api.hxx"
#include "acis/include/body.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/fileinfo.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/vers.hxx"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

using json = nlohmann::json;

int fail(const std::string& message) {
    std::fprintf(stderr, "[worker] %s\n", message.c_str());
    return 1;
}

std::string quote(double value) {
    char text[64];
    std::snprintf(text, sizeof(text), "%.9g", value);
    return text;
}

// ---- ACIS SAT 读写（Qt-free 版，模式同 kernel/acis_sat_io.hxx）-----------
bool saveEntityList(const fs::path& path, const ENTITY_LIST& entities, std::string& why) {
    FILE* file = _wfopen(path.wstring().c_str(), L"w+b");
    if (!file) {
        why = "fopen failed: " + path.string();
        return false;
    }
    bool ok = api_save_version(get_major_version(), get_minor_version()).ok();
    if (!ok) why = "api_save_version failed";
    FileInfo info;
    info.set_units(1.0);
    info.set_product_id("DBCAD");
    if (ok && !api_set_file_info(FileIdent | FileUnits, info).ok()) {
        ok = false;
        why = "api_set_file_info failed";
    }
    if (ok && !api_set_int_option("sequence_save_files", 0).ok()) {
        ok = false;
        why = "api_set_int_option failed";
    }
    if (ok) {
        const outcome saved = api_save_entity_list(file, TRUE, entities);
        if (!saved.ok()) {
            ok = false;
            why = std::string("api_save_entity_list failed: ") + find_err_mess(saved.error_number());
        }
    }
    if (ok && std::fflush(file) != 0) {
        ok = false;
        why = "fflush failed";
    }
    std::fclose(file);
    return ok;
}

bool restoreEntityList(const fs::path& path, ENTITY_LIST& entities) {
    FILE* file = _wfopen(path.wstring().c_str(), L"rb");
    if (!file) return false;
    const bool ok = api_restore_entity_list(file, TRUE, entities).ok();
    std::fclose(file);
    return ok;
}

// 统一模型装载：SAT 直接恢复；STEP/STP 经 InterOp 转换（BRep+Assembly）。
bool loadModel(const fs::path& path, ENTITY_LIST& entities, std::string& why) {
    std::string ext = path.extension().string();
    for (char& c : ext) c = (char)std::tolower((unsigned char)c);
    if (ext != ".step" && ext != ".stp") {
        if (restoreEntityList(path, entities)) return true;
        why = "SAT restore failed: " + path.string();
        return false;
    }
#if DBCAD_HAS_INTEROP
    ENTITY_LIST* converted = nullptr;
    {
        SPAIAcisDocument dst;
        SPAIDocument src(path.string().c_str());
        SPAIFile console(1);
        SPAIOptions options;
        SPAIValue representation("BRep+Assembly");
        options.Add(SPAIOptionName::Representation, representation);
        SPAIConverter converter;
        SPAIResult result = converter.StartLog(console);
        result &= converter.SetOptions(options);
        result &= converter.Convert(src, dst);
        result &= converter.StopLog(console);
        result &= dst.GetEntities(converted);
        if (!result.IsSuccess()) {
            why = "STEP conversion failed: " + path.string();
            return false;
        }
    }
    if (!converted || converted->count() == 0) {
        why = "STEP contained no entities: " + path.string();
        return false;
    }
    for (ENTITY* ent = converted->first(); ent; ent = converted->next()) entities.add(ent);
    delete converted;  // 实体归 ACIS 管理，仅归还列表容器
    return true;
#else
    (void)path;
    (void)entities;
    why = "STEP import not compiled (Spatial InterOp missing from _deps/interop)";
    return false;
#endif
}

bool fileBytes(const fs::path& path, std::string& out) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) return false;
    out.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return !out.empty();
}

// ---- 导入流水线：SAT → 组件（SAT+GLB）+ manifest ------------------------
struct ImportStats {
    int components = 0;
    long triangles = 0;
};

int importDirectory(const fs::path& satPath, const fs::path& outDir) {
    std::error_code ec;
    fs::create_directories(outDir, ec);
    if (ec) return fail("cannot create outdir: " + ec.message());

    ENTITY_LIST entities;
    std::string loadError;
    if (!loadModel(satPath, entities, loadError))
        return fail(loadError);
    if (entities.count() == 0) return fail("model contains no entities");

    // v3 组件三类：体/面/线同级（P6 重建存量投影依赖面/线组件可导入）。
    std::vector<ENTITY*> roots;
    for (ENTITY* ent = entities.first(); ent; ent = entities.next())
        if (is_BODY(ent) || is_FACE(ent) || is_EDGE(ent)) roots.push_back(ent);
    if (roots.empty()) return fail("SAT contains no top-level BODY/FACE/EDGE");

    ImportStats stats;
    std::string manifest = "{\"schema\":\"dbcad.worker.import/1\",\"components\":[";
    for (size_t i = 0; i < roots.size(); ++i) {
        ENTITY* entity = roots[i];
        const std::string stem = "comp-" + std::to_string(i);

        // 网格化容错（P9 压测实证：STEP 转换产物含无面体/不可离散面——
        // 线框体是合法几何，渲染缺网格只降级不失败）：GLB 跳过 + manifest
        // 标记 mesh_error，SAT/graph（几何真相与投影）照常产出。
        dbcad::MeshResult mesh = dbcad::MeshExtractor::extractEntity(*entity, dbcad::MeshQuality::MediumFine);
        std::string meshError;
        std::vector<unsigned char> glb;
        if (mesh.ok) {
            glb = dbcad::GlbWriter::write(mesh.mesh, &meshError);
            if (glb.empty()) meshError = "glb write: " + meshError;
        } else {
            meshError = mesh.error;
        }

        const fs::path satOut = outDir / (stem + ".sat");
        ENTITY_LIST single;
        single.add(entity);
        std::string why;
        if (!saveEntityList(satOut, single, why)) return fail(stem + ": SAT save failed: " + why);

        // 实体图投影（P6/ADR-3）：结构 + fallback 覆盖标记；root id 为临时
        // stem，网关以 SAT 内容哈希重编（同 P2 sid 重编语义）。
        std::string graphText;
        int graphNodes = 0;
        const char* graphFallback = "fine_grained";
        try {
            nlohmann::json graph = dbcad::worker::serializeEntityGraph(stem, *entity);
            for (const auto& node : graph.value("nodes", nlohmann::json::array())) {
                const nlohmann::json& props = node.value("props", nlohmann::json::object());
                if (props.value("fallback", "") == "sat_asset") graphFallback = "sat_asset";
            }
            graphNodes = (int)graph.value("nodes", nlohmann::json::array()).size();
            graphText = graph.dump();
        } catch (const std::exception& error) {
            return fail(stem + ": graph emit failed: " + error.what());
        }
        {
            std::ofstream stream(outDir / (stem + ".graph.json"), std::ios::binary);
            stream.write(graphText.data(), (std::streamsize)graphText.size());
            if (!stream) return fail(stem + ": graph write failed");
        }

        if (!glb.empty()) {
            const fs::path glbOut = outDir / (stem + ".glb");
            std::ofstream stream(glbOut, std::ios::binary);
            stream.write(reinterpret_cast<const char*>(glb.data()), (std::streamsize)glb.size());
            if (!stream) return fail(stem + ": GLB write failed");
        }

        SPAposition boxMin, boxMax;
        if (!api_get_entity_box(entity, boxMin, boxMax).ok())
            return fail(stem + ": bounding box failed");

        double color[3] = {0.8, 0.8, 0.8};
        rgb_color native;
        logical found = FALSE;
        if (api_rh_get_entity_rgb(entity, native, TRUE, found).ok() && found == TRUE) {
            color[0] = native.red();
            color[1] = native.green();
            color[2] = native.blue();
        }

        if (i) manifest += ",";
        // mesh_error 文本转义（保守：反斜杠/引号），保 JSON 合法
        std::string meshErrorJson;
        for (char ch : meshError) {
            if (ch == '\\' || ch == '"') meshErrorJson += '\\';
            meshErrorJson += ch;
        }
        manifest += "{\"id\":\"" + stem + "\",\"sat\":\"" + stem + ".sat\"," +
                    (glb.empty() ? std::string("\"glb\":\"\"") : "\"glb\":\"" + stem + ".glb\"") +
                    ",\"graph\":\"" + stem + ".graph.json\"" +
                    ",\"graph_nodes\":" + std::to_string(graphNodes) +
                    ",\"graph_fallback\":\"" + graphFallback + "\"" +
                    ",\"triangles\":" + std::to_string(mesh.mesh.triangleCount()) +
                    ",\"vertices\":" + std::to_string(mesh.mesh.vertexCount()) +
                    ",\"faces_skipped\":" + std::to_string(mesh.facesSkipped) +
                    (meshErrorJson.empty() ? "" : ",\"mesh_error\":\"" + meshErrorJson + "\"") +
                    ",\"bbox\":{\"min\":[" + quote(boxMin.x()) + "," + quote(boxMin.y()) + "," +
                    quote(boxMin.z()) + "],\"max\":[" + quote(boxMax.x()) + "," + quote(boxMax.y()) +
                    "," + quote(boxMax.z()) + "]}," +
                    "\"color\":[" + quote(color[0]) + "," + quote(color[1]) + "," + quote(color[2]) + "]}";
        stats.components++;
        stats.triangles += mesh.mesh.triangleCount();
    }
    manifest += "]}";

    const fs::path manifestPath = outDir / "manifest.json";
    {
        std::ofstream stream(manifestPath, std::ios::binary);
        stream.write(manifest.data(), (std::streamsize)manifest.size());
        if (!stream) return fail("manifest write failed");
    }

    api_del_entity_list(entities);
    std::printf("[worker] import ok: %d components, %ld triangles\n", stats.components, stats.triangles);
    return 0;
}

// ---- 自检：建模→存→读→导入→合并→导出 全链路 ---------------------------
int mergeSat(const std::vector<fs::path>& inputs, const fs::path& output);
int exportGlb(const std::vector<fs::path>& inputs, const fs::path& output);

int selftest(const fs::path& scratch) {
    std::error_code ec;
    fs::create_directories(scratch, ec);
    if (ec) return fail("selftest scratch dir: " + ec.message());
    BODY* sphere = NULL;
    BODY* cuboid = NULL;
    if (!api_make_sphere(5, sphere).ok() || !api_make_cuboid(6, 4, 3, cuboid).ok()) {
        api_del_entity(sphere);
        return fail("selftest model build failed");
    }
    ENTITY_LIST models;
    models.add(sphere);
    models.add(cuboid);
    { std::string why; if (!saveEntityList(scratch / "selftest.sat", models, why)) return fail("selftest SAT save failed: " + why); }
    api_del_entity_list(models);

    const fs::path out = scratch / "selftest-out";
    const int imported = importDirectory(scratch / "selftest.sat", out);
    if (imported != 0) return imported;

    // manifest 结构断言：两组件、文件齐全、GLB 头合法。
    std::string manifestText;
    if (!fileBytes(out / "manifest.json", manifestText))
        return fail("selftest manifest unreadable");
    if (manifestText.find("\"components\":[") == std::string::npos ||
        manifestText.find("comp-0") == std::string::npos ||
        manifestText.find("comp-1") == std::string::npos)
        return fail("selftest manifest missing components");
    std::string glb;
    if (!fileBytes(out / "comp-0.glb", glb)) return fail("selftest glb missing");
    if (glb.size() < 20 || glb.compare(0, 4, "glTF") != 0)
        return fail("selftest glb header invalid");

    // 实体图投影断言（P6）：文件存在、schema 正确、root 为 body 顶层节点
    // 且带合法 fallback 标记。ATTRIB 链在 ACIS 实体上是常态（v2 probe 基线
    // 同为 sat_fragment），故 fallback 两种取值都合法，只验值域与拓扑计数。
    std::string graphText;
    if (!fileBytes(out / "comp-0.graph.json", graphText)) return fail("selftest graph missing");
    {
        json graph = json::parse(graphText, nullptr, false);
        if (graph.is_discarded() || graph.value("schema", "") != "dbcad.entity_graph.v2")
            return fail("selftest graph schema invalid");
        const json& nodes = graph.value("nodes", json::array());
        if (nodes.empty()) return fail("selftest graph has no nodes");
        bool rootOk = false;
        for (const json& node : nodes) {
            const json& labels = node.at("labels");
            const json& props = node.at("props");
            if (props.value("top_level", false)) {
                const std::string fb = props.value("fallback", "");
                rootOk = labels.size() == 1 && labels[0] == "body" &&
                         (fb == "fine_grained" || fb == "sat_asset") &&
                         props.value("topology_node_count", 0) == (int)nodes.size();
            }
        }
        if (!rootOk) return fail("selftest graph root node invalid");
        if (graph.value("rels", json::array()).empty()) return fail("selftest graph has no rels");
    }

    // 导出链路自检：组件合并回整体 SAT，再合成整体 GLB。
    const fs::path merged = scratch / "merged.sat";
    int merged2 = mergeSat({out / "comp-0.sat", out / "comp-1.sat"}, merged);
    if (merged2 != 0) return merged2;
    std::string mergedBytes;
    if (!fileBytes(merged, mergedBytes)) return fail("selftest merged SAT unreadable");
    if (mergedBytes.size() < 100) return fail("selftest merged SAT suspiciously small");
    const fs::path wholeGlb = scratch / "whole.glb";
    const int exported = exportGlb({out / "comp-0.sat", out / "comp-1.sat"}, wholeGlb);
    if (exported != 0) return exported;
    std::string wholeBytes;
    if (!fileBytes(wholeGlb, wholeBytes) || wholeBytes.compare(0, 4, "glTF") != 0)
        return fail("selftest whole glb invalid");

    fs::remove_all(scratch);
    std::printf("[worker] selftest ok\n");
    return 0;
}

// ---- 导出：多组件 SAT 合并 / 整体 GLB -----------------------------------
// merge：把若干 SAT 组件恢复进同一实体表再整体保存（fmt=sat）。
int mergeSat(const std::vector<fs::path>& inputs, const fs::path& output) {
    ENTITY_LIST all;
    for (const fs::path& input : inputs) {
        ENTITY_LIST part;
        if (!restoreEntityList(input, part)) return fail("SAT restore failed: " + input.string());
        for (ENTITY* ent = part.first(); ent; ent = part.next()) all.add(ent);
    }
    if (all.count() == 0) return fail("nothing to merge");
    std::string why;
    if (!saveEntityList(output, all, why)) return fail("merge save failed: " + why);
    api_del_entity_list(all);
    std::printf("[worker] merge ok: %zu inputs\n", inputs.size());
    return 0;
}

// collectMesh：恢复组件 SAT 并把全部 BODY 的网格合并进 merged。
int collectMesh(const std::vector<fs::path>& inputs, dbcad::MeshData& merged) {
    for (const fs::path& input : inputs) {
        ENTITY_LIST part;
        if (!restoreEntityList(input, part)) return fail("SAT restore failed: " + input.string());
        for (ENTITY* ent = part.first(); ent; ent = part.next()) {
            dbcad::MeshResult mesh = dbcad::MeshExtractor::extractEntity(*ent, dbcad::MeshQuality::MediumFine);
            if (!mesh.ok) return fail("export mesh failed: " + mesh.error);
            dbcad::appendMesh(merged, mesh.mesh);
        }
        api_del_entity_list(part);
    }
    return 0;
}

// exportGLB：合并网格为单一 GLB（fmt=glb）。
int exportGlb(const std::vector<fs::path>& inputs, const fs::path& output) {
    dbcad::MeshData merged;
    int code = collectMesh(inputs, merged);
    if (code != 0) return code;
    std::string error;
    const std::vector<unsigned char> glb = dbcad::GlbWriter::write(merged, &error);
    if (glb.empty()) return fail("export glb failed: " + error);
    std::ofstream stream(output, std::ios::binary);
    stream.write(reinterpret_cast<const char*>(glb.data()), (std::streamsize)glb.size());
    if (!stream) return fail("export glb write failed");
    std::printf("[worker] export glb ok: %u triangles\n", merged.triangleCount());
    return 0;
}

// exportStl：ASCII STL（P5；facet 顶点取自合并网格，法线按右手叉积）。
int exportStl(const std::vector<fs::path>& inputs, const fs::path& output) {
    dbcad::MeshData merged;
    int code = collectMesh(inputs, merged);
    if (code != 0) return code;
    std::ofstream stream(output, std::ios::binary);
    stream << "solid dbcad\n";
    char line[256];
    for (size_t t = 0; t + 2 < merged.indices.size(); t += 3) {
        const size_t ia = 3 * (size_t)merged.indices[t];
        const size_t ib = 3 * (size_t)merged.indices[t + 1];
        const size_t ic = 3 * (size_t)merged.indices[t + 2];
        const double ax = merged.positions[ia], ay = merged.positions[ia + 1], az = merged.positions[ia + 2];
        const double bx = merged.positions[ib], by = merged.positions[ib + 1], bz = merged.positions[ib + 2];
        const double cx = merged.positions[ic], cy = merged.positions[ic + 1], cz = merged.positions[ic + 2];
        const double ux = bx - ax, uy = by - ay, uz = bz - az;
        const double vx = cx - ax, vy = cy - ay, vz = cz - az;
        double nx = uy * vz - uz * vy, ny = uz * vx - ux * vz, nz = ux * vy - uy * vx;
        const double len = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (len > 1e-12) { nx /= len; ny /= len; nz /= len; } else { nx = ny = 0; nz = 1; }
        std::snprintf(line, sizeof(line), "  facet normal %.9g %.9g %.9g\n", nx, ny, nz);
        stream << line << "    outer loop\n";
        std::snprintf(line, sizeof(line), "      vertex %.9g %.9g %.9g\n", ax, ay, az); stream << line;
        std::snprintf(line, sizeof(line), "      vertex %.9g %.9g %.9g\n", bx, by, bz); stream << line;
        std::snprintf(line, sizeof(line), "      vertex %.9g %.9g %.9g\n", cx, cy, cz); stream << line;
        stream << "    endloop\n  endfacet\n";
    }
    stream << "endsolid dbcad\n";
    if (!stream) return fail("export stl write failed");
    std::printf("[worker] export stl ok: %u triangles\n", merged.triangleCount());
    return 0;
}

// ---- P2 serve 模式：常驻行协议 ------------------------------------------
int serveLoop() {
    using dbcad::worker::GeometrySession;
    std::unique_ptr<GeometrySession> session;
    std::string line;
    while (std::getline(std::cin, line)) {
        json request = json::parse(line, nullptr, false);
        json response = {{"ok", false}, {"error", "bad request: expected JSON with id"}};
        int id = -1;
        bool hasId = false;
        if (!request.is_discarded() && request.contains("id")) {
            id = request.at("id").get<int>();
            hasId = true;
        }
        if (!hasId) {
            std::cout << response.dump() << "\n" << std::flush;
            continue;
        }
        response = {{"id", id}, {"ok", false}, {"error", "unknown op"}};
        const std::string op = request.value("op", "");
        try {
            if (op == "ping") {
                response = {{"id", id}, {"ok", true}};
            } else if (op == "open") {
                session = std::make_unique<GeometrySession>(request.value("spool", ""));
                std::vector<std::pair<std::string, std::string>> components;
                if (request.contains("components"))
                    for (const json& entry : request.at("components"))
                        components.push_back({entry.value("sid", ""), entry.value("file", "")});
                const std::string error = session->open(components);
                if (error.empty()) response = {{"id", id}, {"ok", true}};
                else response = {{"id", id}, {"ok", false}, {"error", error}};
            } else if (op == "exec") {
                if (!session || !session->opened())
                    throw std::runtime_error("no open session");
                const std::string result = session->exec(request.at("command").dump());
                response = {{"id", id}, {"ok", true}, {"result", json::parse(result)}};
            } else if (op == "close") {
                session.reset();
                response = {{"id", id}, {"ok", true}};
            }
        } catch (const std::exception& exc) {
            response = {{"id", id}, {"ok", false}, {"error", exc.what()}};
        }
        std::cout << response.dump() << "\n" << std::flush;
        if (op == "close") return 0;
    }
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    const int level = initialize_acis();
    if (level < 2) {
        terminate_acis(level);
        return fail("ACIS init failed");
    }
#if DBCAD_HAS_INTEROP
    SPAISystem::Initialize();
#endif

    int code = 1;
    const std::string command = argc >= 2 ? argv[1] : "";
    if (argc == 2 && command == "serve") {
        code = serveLoop();
    } else if (command == "selftest") {
        code = selftest(fs::temp_directory_path() / "dbcad_worker_selftest");
    } else if (argc == 6 && command == "merge" && std::string(argv[2]) == "--sat-list" &&
               std::string(argv[4]) == "--out") {
        std::vector<fs::path> inputs;
        std::string list = argv[3];
        size_t start = 0;
        while (start <= list.size()) {
            const size_t comma = list.find(',', start);
            const std::string one = list.substr(start, comma == std::string::npos ? comma : comma - start);
            if (!one.empty()) inputs.push_back(one);
            if (comma == std::string::npos) break;
            start = comma + 1;
        }
        code = inputs.empty() ? fail("empty --sat-list") : mergeSat(inputs, argv[5]);
    } else if (argc == 6 && command == "export" && std::string(argv[2]) == "--sat-list" &&
               std::string(argv[4]) == "--stl-out") {
        std::vector<fs::path> inputs;
        std::string list = argv[3];
        size_t start = 0;
        while (start <= list.size()) {
            const size_t comma = list.find(',', start);
            const std::string one = list.substr(start, comma == std::string::npos ? comma : comma - start);
            if (!one.empty()) inputs.push_back(one);
            if (comma == std::string::npos) break;
            start = comma + 1;
        }
        code = inputs.empty() ? fail("empty --sat-list") : exportStl(inputs, argv[5]);
    } else if (argc == 6 && command == "export" && std::string(argv[2]) == "--sat-list" &&
               std::string(argv[4]) == "--glb-out") {
        std::vector<fs::path> inputs;
        std::string list = argv[3];
        size_t start = 0;
        while (start <= list.size()) {
            const size_t comma = list.find(',', start);
            const std::string one = list.substr(start, comma == std::string::npos ? comma : comma - start);
            if (!one.empty()) inputs.push_back(one);
            if (comma == std::string::npos) break;
            start = comma + 1;
        }
        code = inputs.empty() ? fail("empty --sat-list") : exportGlb(inputs, argv[5]);
    } else if (argc == 6 && command == "import" && std::string(argv[2]) == "--sat" &&
               std::string(argv[4]) == "--outdir") {
        code = importDirectory(fs::path(argv[3]), fs::path(argv[5]));
    } else {
        std::fprintf(stderr,
                     "usage: dbcad_worker import --sat <p.sat|p.step|p.stp> --outdir <dir>\n"
                     "       dbcad_worker merge --sat-list <a,b> --out <p>\n"
                     "       dbcad_worker export --sat-list <a,b> --glb-out <p>\n"
                     "       dbcad_worker selftest\n");
    }

#if DBCAD_HAS_INTEROP
    SPAISystem::Shutdown();
#endif
    terminate_acis(level);
    return code;
}
