// GeometrySession 实现：命令执行器（图元创建/变换/布尔/求交/去重/删除）。
// 图元表复用 kernel/acis_primitives.inc（纯 ACIS switch，无 Qt 依赖）。
#include "session.h"

#include "kernel/glb_writer.h"
#include "kernel/mesh_extractor.h"
#include "kernel/model_types.h"

#include "vendor/nlohmann/json.hpp"

#include "acis.hxx"
#include "acis/include/acistype.hxx"
#include "acis/include/alltop.hxx"
#include "acis/include/api.hxx"
#include "acis/include/body.hxx"
#include "acis/include/boolapi.hxx"
#include "acis/include/ckoutcom.hxx"
#include "acis/include/cstrapi.hxx"
#include "acis/include/edge.hxx"
#include "acis/include/face.hxx"
#include "acis/include/fileinfo.hxx"
#include "acis/include/getowner.hxx"
#include "acis/include/intrapi.hxx"
#include "acis/include/intcucu.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/rnd_api.hxx"
#include "acis/include/splgrid.hxx"
#include "acis/include/transf.hxx"
#include "acis/include/transfrm.hxx"
#include "acis/include/vers.hxx"
#include "kernel/acis_primitives.inc"

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace dbcad::worker {
namespace {

struct KindInfo {
    const char* name;
    int id;
    char category;  // 'b'ody / 'f'ace / 'e'dge
};

// 33 图元（kernel/model_types.h BASIC_ENTITIES 的 API 名）。
const KindInfo kKinds[] = {
    {"cube", E_CUBE, 'b'},        {"sphere", E_SPHERE, 'b'},     {"cylinder", E_CYLINDER, 'b'},
    {"torus", E_TORUS, 'b'},      {"cone", E_CONE, 'b'},         {"prism", E_PRISM, 'b'},
    {"pyramid", E_PYRAMID, 'b'},  {"wiggle", E_WIGGLE, 'b'},
    {"plane", E_PLANE, 'f'},      {"pplane", E_PPLANE, 'f'},     {"cplane", E_CPLANE, 'f'},
    {"spheres", E_SPHERES, 'f'},  {"pspheres", E_PSPHERES, 'f'}, {"conic_side", E_CONIC_SIDE, 'f'},
    {"cylinder_side", E_CYLINDER_SIDE, 'f'}, {"pconic", E_PCONIC, 'f'},
    {"toruss", E_TORUSS, 'f'},    {"ptoruss", E_PTORUSS, 'f'},   {"laws", E_LAWS, 'f'},
    {"line", E_LINE, 'e'},        {"arc", E_ARC, 'e'},           {"ellipse", E_ELLIPSE, 'e'},
    {"conic", E_CONIC, 'e'},      {"helix", E_HELIX, 'e'},       {"spiral", E_SPIRAL, 'e'},
    {"spring", E_SPRING, 'e'},    {"lawc", E_LAWC, 'e'},
    {"bspline_ctrlpts", E_BSPLINE_CTRLPTS, 'e'}, {"bspline_fit", E_BSPLINE_FIT, 'e'},
    {"bspline_intrp", E_BSPLINE_INTRP, 'e'},     {"bezier", E_BEZIER, 'e'},
    {"bspline", E_BSPLINE, 'e'},
};

const KindInfo* findKind(const std::string& name) {
    for (const KindInfo& kind : kKinds)
        if (name == kind.name) return &kind;
    return nullptr;
}

std::string quote(double value) {
    char text[64];
    std::snprintf(text, sizeof(text), "%.9g", value);
    return text;
}

bool saveEntityList(const fs::path& path, const ENTITY_LIST& entities) {
    FILE* file = _wfopen(path.wstring().c_str(), L"w+b");
    if (!file) return false;
    bool ok = api_save_version(get_major_version(), get_minor_version()).ok();
    FileInfo info;
    info.set_units(1.0);
    info.set_product_id("DBCAD");
    ok = ok && api_set_file_info(FileIdent | FileUnits, info).ok();
    ok = ok && api_set_int_option("sequence_save_files", 0).ok();
    ok = ok && api_save_entity_list(file, TRUE, entities).ok();
    if (std::fflush(file) != 0) ok = false;
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

SPAposition vec3(const json& value, const char* what) {
    if (!value.is_array() || value.size() != 3)
        throw CommandError(std::string(what) + " must be [x,y,z]");
    return SPAposition(value[0].get<double>(), value[1].get<double>(), value[2].get<double>());
}

double scalar(const json& params, const char* key, double fallback) {
    if (!params.contains(key) || params[key].is_null()) return fallback;
    const json& value = params.at(key);
    if (!value.is_number()) throw CommandError(std::string(key) + " must be a number");
    return value.get<double>();
}

// 带参数的体图元（参数化插入，V3_PLAN §3.2 P0）；未覆盖的 kind 走单位图元。
BODY* buildBody(const std::string& kind, const json& params) {
    BODY* body = nullptr;
    if (kind == "cube") {
        const double w = scalar(params, "width", 1.0), h = scalar(params, "height", 1.0),
                     d = scalar(params, "depth", 1.0);
        if (!api_solid_block(SPAposition(-w / 2, -h / 2, -d / 2), SPAposition(w / 2, h / 2, d / 2), body).ok())
            throw CommandError("api_solid_block failed");
        return body;
    }
    if (kind == "sphere") {
        if (!api_make_sphere(scalar(params, "radius", 1.0), body).ok())
            throw CommandError("api_make_sphere failed");
        return body;
    }
    if (kind == "cylinder" || kind == "cone") {
        const double radius = scalar(params, "radius", 0.5);
        const double height = scalar(params, "height", 1.0);
        const double top = kind == "cone" ? 0.0 : radius;
        SPAposition bottom(0, 0, -height / 2), topPos(0, 0, height / 2);
        if (!api_solid_cylinder_cone(bottom, topPos, radius, radius, top, nullptr, body).ok())
            throw CommandError("api_solid_cylinder_cone failed");
        return body;
    }
    if (kind == "torus") {
        if (!api_solid_torus(SPAposition(0, 0, 0), scalar(params, "major_radius", 1.0),
                             scalar(params, "minor_radius", 0.5), body).ok())
            throw CommandError("api_solid_torus failed");
        return body;
    }
    if (kind == "prism" || kind == "pyramid") {
        const double w = scalar(params, "width", 1.0), h = scalar(params, "height", 1.0),
                     d = scalar(params, "depth", 1.0);
        const int sides = (int)scalar(params, "sides", kind == "prism" ? 6 : 4);
        outcome status = kind == "prism"
            ? api_make_prism(w, h, d, sides, body)
            : api_make_pyramid(w, h, d, 0.0, sides, body);
        if (!status.ok()) throw CommandError("api_make_prism/pyramid failed");
        return body;
    }
    // 单位图元（wiggle 及全部面/线 kind 经此路径，参数不适用）。
    const KindInfo* info = findKind(kind);
    if (!info || info->category != 'b') throw CommandError("unknown body kind: " + kind);
    ENTITY* entity = nullptr;
    std::vector<std::vector<SPAposition>> handles;
    outcome status;
    createPrimitive(info->id, entity, handles, status);
    if (!status.ok() || !entity) throw CommandError("createPrimitive failed for " + kind);
    return (BODY*)entity;
}

void bakeTransform(ENTITY* entity, const SPAtransf& transform) {
    if (!api_transform_entity(entity, transform).ok())
        throw CommandError("api_transform_entity failed");
    if (is_BODY(entity) && !api_change_body_trans((BODY*)entity, nullptr).ok())
        throw CommandError("api_change_body_trans (bake) failed");
}

// Qt 客户端同款非 BODY→BODY 提升（布尔运算需要实体）。
BODY* toBody(ENTITY* entity, ENTITY_LIST& temporary) {
    if (is_BODY(entity)) return (BODY*)entity;
    BODY* body = nullptr;
    if (is_FACE(entity)) {
        FACE* faces[] = {(FACE*)entity};
        if (!api_sheet_from_ff(1, faces, body).ok())
            throw CommandError("cannot lift face into a body for boolean");
    } else if (is_EDGE(entity)) {
        ENTITY* copy = nullptr;
        if (!api_deep_down_copy_entity(entity, copy).ok())
            throw CommandError("copy edge failed");
        EDGE* edges[] = {(EDGE*)copy};
        outcome status = api_make_ewire(1, edges, body);
        if (!status.ok()) {
            api_del_entity(copy);
            throw CommandError("api_make_ewire failed");
        }
    } else {
        throw CommandError("component cannot join a boolean");
    }
    temporary.add(body);
    return body;
}

void delEntity(ENTITY* entity) {
    if (entity) api_del_entity(entity);
}

void delEntityList(ENTITY_LIST& list) {
    for (ENTITY* ent = list.first(); ent; ent = list.next()) delEntity(ent);
}

}  // namespace

GeometrySession::GeometrySession(std::string spoolDir) : spoolDir_(std::move(spoolDir)) {}

GeometrySession::~GeometrySession() {
    for (ComponentRef& component : components_) delEntity(component.entity);
}

ComponentRef* GeometrySession::find(const std::string& sid) {
    for (ComponentRef& component : components_)
        if (component.sid == sid) return &component;
    return nullptr;
}

std::string GeometrySession::open(const std::vector<std::pair<std::string, std::string>>& components) {
    for (ComponentRef& component : components_) delEntity(component.entity);
    components_.clear();
    nextNewSid_ = 0;
    for (const auto& [sid, file] : components) {
        ENTITY_LIST restored;
        if (!restoreEntityList(fs::path(spoolDir_) / file, restored) || restored.count() == 0)
            return "SAT restore failed for " + sid;
        for (ENTITY* ent = restored.first(); ent; ent = restored.next())
            components_.push_back({sid, ent, ""});
    }
    opened_ = true;
    return "";
}

std::string GeometrySession::emitComponent(ComponentRef& component, bool added) {
    const fs::path satPath = fs::path(spoolDir_) / (component.sid + ".sat");
    ENTITY_LIST single;
    single.add(component.entity);
    if (!saveEntityList(satPath, single))
        throw CommandError("SAT emit failed for " + component.sid);

    MeshResult mesh = MeshExtractor::extractEntity(*component.entity, MeshQuality::MediumFine);
    if (!mesh.ok) throw CommandError("mesh emit failed for " + component.sid + ": " + mesh.error);
    std::string glbError;
    const std::vector<unsigned char> glb = GlbWriter::write(mesh.mesh, &glbError);
    if (glb.empty()) throw CommandError("glb emit failed for " + component.sid + ": " + glbError);
    {
        std::ofstream stream(fs::path(spoolDir_) / (component.sid + ".glb"), std::ios::binary);
        stream.write(reinterpret_cast<const char*>(glb.data()), (std::streamsize)glb.size());
        if (!stream) throw CommandError("glb write failed for " + component.sid);
    }

    SPAposition boxMin, boxMax;
    if (!api_get_entity_box(component.entity, boxMin, boxMax).ok())
        throw CommandError("bbox failed for " + component.sid);
    double color[3] = {0.8, 0.8, 0.8};
    rgb_color native;
    logical found = FALSE;
    if (api_rh_get_entity_rgb(component.entity, native, TRUE, found).ok() && found == TRUE) {
        color[0] = native.red();
        color[1] = native.green();
        color[2] = native.blue();
    }
    (void)added;
    json emit;
    emit["sid"] = component.sid;
    emit["kind"] = component.kind;
    emit["sat"] = component.sid + ".sat";
    emit["glb"] = component.sid + ".glb";
    emit["triangles"] = mesh.mesh.triangleCount();
    emit["vertices"] = mesh.mesh.vertexCount();
    emit["bbox"] = {{"min", json::array({boxMin.x(), boxMin.y(), boxMin.z()})},
                    {"max", json::array({boxMax.x(), boxMax.y(), boxMax.z()})}};
    emit["color"] = json::array({color[0], color[1], color[2]});
    return emit.dump();
}

std::string GeometrySession::exec(const std::string& commandJson) {
    json command = json::parse(commandJson, nullptr, false);
    if (command.is_discarded() || !command.contains("action"))
        throw CommandError("command must be JSON with an action");
    const std::string action = command.at("action").get<std::string>();
    const json& params = command.contains("params") ? command.at("params") : json::object();

    json result = {{"added", json::array()}, {"modified", json::array()}, {"removed", json::array()}};

    auto removeComponent = [&](ComponentRef* component) {
        delEntity(component->entity);
        result["removed"].push_back(component->sid);
        components_.erase(components_.begin() + (component - components_.data()));
    };

    if (action == "component.create") {
        const std::string kind = params.value("kind", "");
        const KindInfo* info = findKind(kind);
        if (!info) throw CommandError("unknown kind: " + kind);
        ENTITY* entity = nullptr;
        if (info->category == 'b') {
            entity = buildBody(kind, params);
        } else {
            std::vector<std::vector<SPAposition>> handles;
            outcome status;
            createPrimitive(info->id, entity, handles, status);
            if (!status.ok() || !entity) throw CommandError("createPrimitive failed for " + kind);
        }
        ComponentRef component{"n" + std::to_string(nextNewSid_++), entity, kind};
        if (params.contains("position")) {
            const SPAposition position = vec3(params.at("position"), "position");
            bakeTransform(entity, translate_transf(SPAvector(position.x(), position.y(), position.z())));
        }
        components_.push_back(component);
        result["added"].push_back(json::parse(emitComponent(components_.back(), true)));
        return result.dump();
    }

    if (action == "component.translate" || action == "component.rotate" || action == "component.scale") {
        ComponentRef* component = find(params.value("component", ""));
        if (!component) throw CommandError("unknown component sid");
        SPAtransf transform;
        if (action == "component.translate") {
            const SPAposition offset = vec3(params.at("offset"), "offset");
            transform = translate_transf(SPAvector(offset.x(), offset.y(), offset.z()));
        } else if (action == "component.rotate") {
            const double degrees = scalar(params, "angle_degrees", 0.0);
            const SPAposition axisPoint = vec3(params.at("axis"), "axis");
            transform = rotate_transf(degrees * (3.14159265358979323846 / 180.0),
                                      SPAvector(axisPoint.x(), axisPoint.y(), axisPoint.z()));
        } else {
            const double factor = scalar(params, "factor", 1.0);
            transform = scale_transf(factor);
        }
        bakeTransform(component->entity, transform);
        result["modified"].push_back(json::parse(emitComponent(*component, false)));
        return result.dump();
    }

    if (action == "component.delete") {
        ComponentRef* component = find(params.value("component", ""));
        if (!component) throw CommandError("unknown component sid");
        removeComponent(component);
        return result.dump();
    }

    if (action == "boolean.unite" || action == "boolean.subtract" || action == "boolean.intersect") {
        // P2 语义：恰好一目标 + 一工具（Qt 基线同款二元布尔）。多目标 unite
        // 留给后续（需要链式布尔与输出体合并语义）。
        if (!params.contains("targets") || !params.at("targets").is_array() ||
            params.at("targets").size() != 1)
            throw CommandError("targets must be an array with exactly one component");
        ComponentRef* tool = find(params.value("tool", ""));
        if (!tool) throw CommandError("unknown tool component sid");
        if (tool->sid == params.at("targets")[0].get<std::string>())
            throw CommandError("boolean needs two distinct components");
        ComponentRef* target = find(params.at("targets")[0].get<std::string>());
        if (!target) throw CommandError("unknown target component sid");
        ENTITY_LIST temporary;
        BODY* output = nullptr;
        BOOL_TYPE operation = action == "boolean.unite" ? UNION
                            : action == "boolean.subtract" ? SUBTRACTION
                                                           : INTERSECTION;
        if (!api_boolean((BODY*)tool->entity, toBody(target->entity, temporary), operation,
                         NDBOOL_KEEP_BOTH, output).ok())
            throw CommandError("api_boolean failed");
        if (!output || !output->lump()) throw CommandError("boolean produced empty result");
        // 输出挂目标槽位（继承 sid/名称/颜色语义），工具组件删除。
        // tool/target 指向 components_ 槽位：任何容器改动前先拷出 sid。
        const std::string survivorSid = target->sid;
        const std::string survivorKind = target->kind;
        const std::string toolSid = tool->sid;
        delEntity(target->entity);
        delEntity(tool->entity);
        delEntityList(temporary);
        for (auto& component : components_) {
            if (component.sid == survivorSid) component = {survivorSid, output, survivorKind};
        }
        for (int i = (int)components_.size() - 1; i >= 0; --i)
            if (components_[(size_t)i].sid == toolSid)
                components_.erase(components_.begin() + i);
        result["removed"].push_back(toolSid);
        result["modified"].push_back(json::parse(emitComponent(*find(survivorSid), false)));
        return result.dump();
    }

    if (action == "defeature.unrepeat") {
        // 成对减法空体检测去重（Qt 客户端同款算法）。
        if (!params.contains("targets") || !params.at("targets").is_array() || params.at("targets").size() < 2)
            throw CommandError("unrepeat needs >= 2 targets");
        ENTITY_LIST temporary;
        std::vector<BODY*> bodies;
        std::vector<ComponentRef*> refs;
        for (const json& sid : params.at("targets")) {
            ComponentRef* component = find(sid.get<std::string>());
            if (!component) throw CommandError("unknown target component sid");
            refs.push_back(component);
            bodies.push_back(toBody(component->entity, temporary));
        }
        std::vector<bool> removed(bodies.size(), false);
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = 0; j < bodies.size(); ++j) {
                if (i == j || removed[i] || removed[j]) continue;
                BODY* a = nullptr;
                BODY* b = nullptr;
                api_boolean(bodies[j], bodies[i], SUBTRACTION, NDBOOL_KEEP_BOTH, a);
                api_boolean(bodies[i], bodies[j], SUBTRACTION, NDBOOL_KEEP_BOTH, b);
                if (a && !a->lump()) removed[i] = true;
                if (b && !b->lump()) removed[j] = true;
                delEntity(a);
                delEntity(b);
            }
        }
        for (size_t i = 0; i < refs.size(); ++i) {
            if (removed[i]) {
                result["removed"].push_back(refs[i]->sid);
            } else {
                result["modified"].push_back(json::parse(emitComponent(*refs[i], false)));
            }
        }
        delEntityList(temporary);
        // 删除被去重的组件实体（refs 倒序擦除避免下标漂移）。
        for (int i = (int)refs.size() - 1; i >= 0; --i) {
            if (removed[(size_t)i]) {
                delEntity(refs[(size_t)i]->entity);
                components_.erase(components_.begin() + (refs[(size_t)i] - components_.data()));
            }
        }
        return result.dump();
    }

    if (action == "intersector.intersect") {
        ComponentRef* a = find(params.value("a", ""));
        ComponentRef* b = find(params.value("b", ""));
        if (!a || !b) throw CommandError("intersector needs components a and b");
        ENTITY_LIST outputs;
        ENTITY* left = a->entity;
        ENTITY* right = b->entity;
        if (is_FACE(left) && is_FACE(right)) {
            BODY* output = nullptr;
            if (!api_fafa_int((FACE*)left, (FACE*)right, output).ok())
                throw CommandError("api_fafa_int failed");
            if (output) outputs.add(output);
        } else if (is_EDGE(left) && is_FACE(right) || is_FACE(left) && is_EDGE(right)) {
            EDGE* edge = is_EDGE(left) ? (EDGE*)left : (EDGE*)right;
            FACE* face = is_FACE(left) ? (FACE*)left : (FACE*)right;
            ENTITY_LIST* list = nullptr;
            outcome status = api_edfa_int(edge, face, list);
            if (list) {
                outputs.add(*list);
                delete list;
            }
            if (!status.ok()) throw CommandError("api_edfa_int failed");
        } else if (is_EDGE(left) && is_EDGE(right)) {
            curve_curve_int* intersections = nullptr;
            const outcome status = api_inter_ed_ed((EDGE*)left, (EDGE*)right, intersections);
            if (!status.ok()) {
                delete_curve_curve_ints(intersections);
                throw CommandError("api_inter_ed_ed failed");
            }
            if (intersections)
                if (!api_ed_inters_to_ents((EDGE*)left, intersections, outputs).ok())
                    throw CommandError("api_ed_inters_to_ents failed");
            delete_curve_curve_ints(intersections);
        } else {
            throw CommandError("unsupported shape combination for intersector");
        }
        for (ENTITY* output = outputs.first(); output; output = outputs.next()) {
            ComponentRef component{"n" + std::to_string(nextNewSid_++), output, ""};
            components_.push_back(component);
            result["added"].push_back(json::parse(emitComponent(components_.back(), true)));
        }
        return result.dump();
    }

    throw CommandError("unknown action: " + action);
}

}  // namespace dbcad::worker
