// v3 几何 worker（V3_PLAN.md ADR-1/ADR-2 · P1 交付）：无 Qt 控制台进程。
//
// P1 形态：批处理 CLI，由 FastAPI 网关按次调用（导入是低频操作）。
// 常驻进程 + 命令队列在 P2 命令引擎时升级，届时本 main 换成行协议循环，
// SAT→组件→GLB 的流水线函数（importDirectory）原样复用。
//
// 命令：
//   dbcad_worker import --sat <path> --outdir <dir>
//     SAT → 逐顶层 BODY：独立 SAT 组件 + medium_fine GLB + 包围盒/颜色元数据
//     → <dir>/manifest.json（schema=dbcad.worker.import/1）
//   dbcad_worker selftest
//     内建模型 存→读→导入 全链路自检（网关集成测试前的快速验证）
// 退出码：0 成功 / 1 失败（stderr 带原因）。错误显式，不静默兜底（v3 原则）。
#include "kernel/mesh_extractor.h"
#include "kernel/glb_writer.h"

#include "acis.hxx"

#include "acis/include/acistype.hxx"
#include "acis/include/api.hxx"
#include "acis/include/body.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/fileinfo.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/vers.hxx"

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

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
    if (!restoreEntityList(satPath, entities))
        return fail("SAT restore failed: " + satPath.string());
    if (entities.count() == 0) return fail("SAT contains no entities");

    std::vector<BODY*> bodies;
    for (ENTITY* ent = entities.first(); ent; ent = entities.next())
        if (is_BODY(ent)) bodies.push_back((BODY*)ent);
    if (bodies.empty()) return fail("SAT contains no top-level BODY");

    ImportStats stats;
    std::string manifest = "{\"schema\":\"dbcad.worker.import/1\",\"components\":[";
    for (size_t i = 0; i < bodies.size(); ++i) {
        BODY* body = bodies[i];
        const std::string stem = "comp-" + std::to_string(i);

        dbcad::MeshResult mesh = dbcad::MeshExtractor::extract(*body, dbcad::MeshQuality::MediumFine);
        if (!mesh.ok) return fail(stem + ": mesh extraction failed: " + mesh.error);
        std::string glbError;
        const std::vector<unsigned char> glb = dbcad::GlbWriter::write(mesh.mesh, &glbError);
        if (glb.empty()) return fail(stem + ": glb write failed: " + glbError);

        const fs::path satOut = outDir / (stem + ".sat");
        ENTITY_LIST single;
        single.add(body);
        std::string why;
        if (!saveEntityList(satOut, single, why)) return fail(stem + ": SAT save failed: " + why);

        const fs::path glbOut = outDir / (stem + ".glb");
        {
            std::ofstream stream(glbOut, std::ios::binary);
            stream.write(reinterpret_cast<const char*>(glb.data()), (std::streamsize)glb.size());
            if (!stream) return fail(stem + ": GLB write failed");
        }

        SPAposition boxMin, boxMax;
        if (!api_get_entity_box(body, boxMin, boxMax).ok())
            return fail(stem + ": bounding box failed");

        double color[3] = {0.8, 0.8, 0.8};
        rgb_color native;
        logical found = FALSE;
        if (api_rh_get_entity_rgb(body, native, TRUE, found).ok() && found == TRUE) {
            color[0] = native.red();
            color[1] = native.green();
            color[2] = native.blue();
        }

        if (i) manifest += ",";
        manifest += "{\"id\":\"" + stem + "\",\"sat\":\"" + stem + ".sat\",\"glb\":\"" + stem +
                    ".glb\",\"triangles\":" + std::to_string(mesh.mesh.triangleCount()) +
                    ",\"vertices\":" + std::to_string(mesh.mesh.vertexCount()) +
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

// exportGLB：合并网格为单一 GLB（fmt=glb）。STL 留给 P5。
int exportGlb(const std::vector<fs::path>& inputs, const fs::path& output) {
    dbcad::MeshData merged;
    for (const fs::path& input : inputs) {
        ENTITY_LIST part;
        if (!restoreEntityList(input, part)) return fail("SAT restore failed: " + input.string());
        for (ENTITY* ent = part.first(); ent; ent = part.next()) {
            if (!is_BODY(ent)) continue;
            dbcad::MeshResult mesh = dbcad::MeshExtractor::extract(*(BODY*)ent,
                                                                   dbcad::MeshQuality::MediumFine);
            if (!mesh.ok) return fail("export mesh failed: " + mesh.error);
            dbcad::appendMesh(merged, mesh.mesh);
        }
        api_del_entity_list(part);
    }
    std::string error;
    const std::vector<unsigned char> glb = dbcad::GlbWriter::write(merged, &error);
    if (glb.empty()) return fail("export glb failed: " + error);
    std::ofstream stream(output, std::ios::binary);
    stream.write(reinterpret_cast<const char*>(glb.data()), (std::streamsize)glb.size());
    if (!stream) return fail("export glb write failed");
    std::printf("[worker] export glb ok: %u triangles\n", merged.triangleCount());
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    const int level = initialize_acis();
    if (level < 2) {
        terminate_acis(level);
        return fail("ACIS init failed");
    }

    int code = 1;
    const std::string command = argc >= 2 ? argv[1] : "";
    if (command == "selftest") {
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
                     "usage: dbcad_worker import --sat <p> --outdir <dir>\n"
                     "       dbcad_worker merge --sat-list <a,b> --out <p>\n"
                     "       dbcad_worker export --sat-list <a,b> --glb-out <p>\n"
                     "       dbcad_worker selftest\n");
    }

    terminate_acis(level);
    return code;
}
