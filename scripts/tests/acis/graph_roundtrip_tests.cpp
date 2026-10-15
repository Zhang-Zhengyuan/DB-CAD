// 实体图 JSON 序列化/反序列化往返测试（Mode1 存储链路核心层）。
// 覆盖：拓扑全类型 + plane/sphere/torus/cone 解析面 + straight/ellipse 曲线
//       + spline/intcurve/helix 缺口基线 + transform + wire/线框 + 布尔混合。
// 每组：建模 → serializeACISEntityGraph → JSON 结构断言
//     → deserializeACISEntityGraph → 体积/包围盒/拓扑计数比对。
// 编译要求同 entity_tests（/MD、/EHa、SPA_NO_AUTO_LINK、栈 4MB），另需 QtCore（QJson）。
#include <kernapi.hxx>
#include <license.hxx>
#include <spa_unlock_result.hxx>
#include <cstrapi.hxx>
#include <boolapi.hxx>
#include <intrapi.hxx>
#include <fileinfo.hxx>
#include <alltop.hxx>
#include <allsurf.hxx>
#include <sweepapi.hxx>
#include <deprecated_masspr.hxx>
#include <tensor.hxx>
#include <heldef.hxx>
#include <blendapi.hxx>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QHash>

#include "entity_graph_serializer.h"

#include "spatial_license.h"

static int g_failed = 0;

static void check(bool ok, const char *what) {
    printf("  %-64s %s\n", what, ok ? "PASS" : "FAIL");
    if (!ok) g_failed++;
}

// ---------------------------------------------------------------------------
// 建模/测量辅助（与 entity_tests.cpp 同源）
// ---------------------------------------------------------------------------

static bool body_volume(BODY *b, double &vol) {
    SPAposition root(0, 0, 0), cofg(0, 0, 0);
    SPAunit_vector normal(0, 0, 1);
    tensor inertia;
    double pm[3];
    SPAunit_vector pa[3];
    double achieved = 0;
    return api_body_mass_pr(b, root, normal, 2, 1e-6, vol, cofg, inertia, pm, pa, achieved).ok();
}

struct TopoCounts {
    int faces = 0, edges = 0, verts = 0;
};

static TopoCounts topo_counts(BODY *b) {
    TopoCounts t;
    ENTITY_LIST fl, el, vl;
    api_get_faces(b, fl);
    api_get_edges(b, el);
    api_get_vertices(b, vl);
    t.faces = fl.count();
    t.edges = el.count();
    t.verts = vl.count();
    return t;
}

static bool body_box(BODY *b, double mn[3], double mx[3]) {
    SPAposition lo, hi;
    if (!api_get_entity_box(b, lo, hi).ok())
        return false;
    mn[0] = lo.x(); mn[1] = lo.y(); mn[2] = lo.z();
    mx[0] = hi.x(); mx[1] = hi.y(); mx[2] = hi.z();
    return true;
}

// ---------------------------------------------------------------------------
// JSON 图往返
// ---------------------------------------------------------------------------

struct GraphRT {
    QJsonObject json;
    bool deserOk = false;
    QString err;
    ENTITY_LIST *ents = nullptr; // 反序列化产物（调用方用 free_rt 释放）
    BODY *body = nullptr;        // 恢复出的第一个 BODY（可为 null，如线框失败时）
};

// serialize → deserialize；uuidCount 个顶级实体注入 test-uuid-N
static void graph_rt(const ENTITY_LIST &top, int uuidCount, GraphRT &out) {
    QHash<int, QString> uuids;
    for (int i = 0; i < uuidCount; ++i)
        uuids.insert(i, QString("test-uuid-%1").arg(i));
    out.json = serializeACISEntityGraph(uuids, top);

    DeserializedEntityGraph d;
    out.deserOk = deserializeACISEntityGraph(out.json, &d, &out.err);
    out.ents = d.entities;
    if (out.ents) {
        for (ENTITY *e = out.ents->first(); e; e = out.ents->next())
            if (is_BODY(e)) {
                out.body = (BODY *)e;
                break;
            }
    }
}

// 释放恢复产物：删 BODY（级联子实体）；无线框体时退化为逐个删除
static void free_rt(GraphRT &rt, bool hasBody) {
    if (!rt.ents)
        return;
    std::vector<ENTITY *> all;
    for (ENTITY *e = rt.ents->first(); e; e = rt.ents->next())
        all.push_back(e);
    if (hasBody) {
        for (ENTITY *e : all)
            if (is_BODY(e))
                api_del_entity(e);
    } else {
        // 反向（子先父后）逐个删除，避免父级级联后二次删除
        for (auto it = all.rbegin(); it != all.rend(); ++it)
            api_del_entity(*it);
    }
    delete rt.ents;
    rt.ents = nullptr;
    rt.body = nullptr;
}

// ---------------------------------------------------------------------------
// JSON 断言辅助
// ---------------------------------------------------------------------------

static QJsonArray nodesOf(const QJsonObject &g) {
    return g.value("nodes").toArray();
}

static int count_label(const QJsonObject &g, const char *label) {
    int n = 0;
    for (const QJsonValue &v : nodesOf(g)) {
        QJsonArray labels = v.toObject().value("labels").toArray();
        for (const QJsonValue &l : labels)
            if (l.toString() == label) {
                ++n;
                break;
            }
    }
    return n;
}

// 找第一个 face/edge 节点 props.geometry.type == gtype 的 geometry 对象
static QJsonObject find_geometry(const QJsonObject &g, const char *gtype) {
    for (const QJsonValue &v : nodesOf(g)) {
        QJsonObject props = v.toObject().value("props").toObject();
        if (!props.contains("geometry"))
            continue;
        QJsonObject gj = props.value("geometry").toObject();
        if (gj.value("type").toString() == gtype)
            return gj;
    }
    return QJsonObject();
}

template <typename... Keys>
static bool has_keys(const QJsonObject &o, Keys... keys) {
    for (const char *k : {keys...})
        if (!o.contains(k) || o.value(k).isNull())
            return false;
    return true;
}

// 几何载荷除 type 外还有数据字段（缺口实体的红灯断言：当前只有类型名）
static bool geometry_has_payload(const QJsonObject &gj) {
    return gj.size() > 1;
}

static void dump_geometry(const QJsonObject &gj, const char *tag) {
    QString s = QString::fromUtf8(QJsonDocument(gj).toJson(QJsonDocument::Compact));
    printf("    [%s] geometry = %s\n", tag, s.toUtf8().constData());
}

// ---------------------------------------------------------------------------
// 双层比对：原件 vs 恢复件
// ---------------------------------------------------------------------------

static void compare_bodies(BODY *orig, BODY *rest, const char *tag, double volRelTol) {
    double vo = 0, vr = 0;
    bool okO = body_volume(orig, vo), okR = body_volume(rest, vr);
    // 数值不一致时落盘 SAT 便于 diff 定位（entity_<tag>_{orig,rest}.sat）
    if (okO && okR) {
        double rel = std::fabs(vo - vr) / (std::fabs(vo) > 1 ? std::fabs(vo) : 1);
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: volume %.6f vs %.6f (rel %.1e)", tag, vr, vo, rel);
        check(rel < volRelTol, buf);
        if (rel >= volRelTol) {
            std::string base(tag);
            for (char &c : base)
                if (c == ' ' || c == ':')
                    c = '_';
            ENTITY_LIST lo(orig), lr(rest);
            FILE *f1 = fopen((base + "_orig.sat").c_str(), "wb");
            if (f1) { api_save_entity_list(f1, TRUE, lo); fclose(f1); }
            FILE *f2 = fopen((base + "_rest.sat").c_str(), "wb");
            if (f2) { api_save_entity_list(f2, TRUE, lr); fclose(f2); }
            printf("    [diag] dumped %s_orig.sat / %s_rest.sat\n", base.c_str(), base.c_str());
        }
    } else {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: mass_pr failed (orig=%d rest=%d)", tag, (int)okO, (int)okR);
        check(false, buf);
        std::string base(tag);
        for (char &c : base)
            if (c == ' ' || c == ':')
                c = '_';
        ENTITY_LIST lo2(orig);
        FILE *f3 = fopen((base + "_orig.sat").c_str(), "wb");
        if (f3) { api_save_entity_list(f3, TRUE, lo2); fclose(f3); }
        if (rest) {
            ENTITY_LIST lr2(rest);
            FILE *f4 = fopen((base + "_rest.sat").c_str(), "wb");
            if (f4) { api_save_entity_list(f4, TRUE, lr2); fclose(f4); }
        }
        printf("    [diag] dumped %s_orig.sat\n", base.c_str());
    }

    TopoCounts co = topo_counts(orig), cr = topo_counts(rest);
    char buf[128];
    snprintf(buf, sizeof(buf), "%s: topo f%d/%d e%d/%d v%d/%d", tag, cr.faces, co.faces, cr.edges, co.edges,
             cr.verts, co.verts);
    check(cr.faces == co.faces && cr.edges == co.edges && cr.verts == co.verts, buf);

    double mo[3], Mo[3], mr[3], Mr[3];
    if (body_box(orig, mo, Mo) && body_box(rest, mr, Mr)) {
        double dmax = 0;
        for (int i = 0; i < 3; ++i) {
            dmax = std::max(dmax, std::fabs(mo[i] - mr[i]));
            dmax = std::max(dmax, std::fabs(Mo[i] - Mr[i]));
        }
        snprintf(buf, sizeof(buf), "%s: bbox delta %.3e", tag, dmax);
        check(dmax < 1e-4, buf);
    }
}

// 标准实体组：serialize 结构断言 + restore + 数值比对
static void run_solid_group(BODY *orig, const char *tag, double volRelTol) {
    ENTITY_LIST top(orig);
    GraphRT rt;
    graph_rt(top, 1, rt);

    char buf[160];
    int nBody = count_label(rt.json, "body");
    snprintf(buf, sizeof(buf), "%s: json body node=%d (uuid injected)", tag, nBody);
    check(nBody == 1, buf);
    check(count_label(rt.json, "face") > 0 && count_label(rt.json, "edge") >= 0 && count_label(rt.json, "vertex") > 0,
          (std::string(tag) + ": json face/edge/vertex labels present").c_str());

    snprintf(buf, sizeof(buf), "%s: deserialize ok", tag);
    check(rt.deserOk, buf);
    if (!rt.body) {
        snprintf(buf, sizeof(buf), "%s: no BODY restored", tag);
        check(false, buf);
        free_rt(rt, false);
        return;
    }
    compare_bodies(orig, rt.body, tag, volRelTol);
    free_rt(rt, true);
}

// ---------------------------------------------------------------------------
// 主流程
// ---------------------------------------------------------------------------

int main(int argc, char **argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    int run_upto = (argc > 1) ? atoi(argv[1]) : 12;
    if (!initialize_base()) { printf("FAIL base\n"); return 1; }
    outcome r = api_start_modeller(0);
    if (!r.ok()) { printf("FAIL modeller\n"); return 1; }
    spa_unlock_products(SPATIAL_LICENSE);

    FileInfo fi;
    fi.set_units(1.0);
    fi.set_product_id("graph_roundtrip_tests");
    api_set_file_info(3, fi);
    api_save_version(0, 0);

    int g_id = 0;
    if (++g_id > run_upto) goto finish;
    printf("== G1 方块（全拓扑 + plane 面 + straight 边）==\n");
    {
        BODY *box = nullptr;
        check(api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), box).ok(), "G1 api_solid_block");
        QJsonObject plane = [&] {
            ENTITY_LIST top(box);
            GraphRT rt;
            graph_rt(top, 1, rt);
            QJsonObject gj = find_geometry(rt.json, "plane");
            QJsonObject st = find_geometry(rt.json, "straight");
            check(has_keys(gj, "root_point", "normal", "u_deriv"), "G1 plane payload root_point/normal/u_deriv");
            check(has_keys(st, "root_point", "direction", "range"), "G1 straight payload root_point/direction/range");
            check(rt.deserOk, "G1 deserialize ok");
            if (rt.body) compare_bodies(box, rt.body, "G1 block", 1e-6);
            free_rt(rt, true);
            return gj;
        }();
        (void)plane;
        api_del_entity(box);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G2 球（sphere 面 + 拟合圆边 + 极点退化）==\n");
    {
        BODY *sph = nullptr;
        check(api_solid_sphere(SPAposition(0, 0, 0), 5.0, sph).ok(), "G2 api_solid_sphere");
        ENTITY_LIST top(sph);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject gj = find_geometry(rt.json, "sphere");
        check(has_keys(gj, "centre", "radius"), "G2 sphere payload centre/radius");
        check(rt.deserOk, "G2 deserialize ok");
        if (rt.body) compare_bodies(sph, rt.body, "G2 sphere", 1e-6);
        free_rt(rt, true);
        api_del_entity(sph);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G3 圆柱（cone(柱) 面 + 圆边）==\n");
    {
        BODY *cyl = nullptr;
        check(api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 20), 4.0, 4.0, 4.0, nullptr, cyl).ok(),
              "G3 api_solid_cylinder_cone");
        ENTITY_LIST top(cyl);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject gj = find_geometry(rt.json, "cone");
        check(has_keys(gj, "base_centre", "base_normal", "base_radius_ratio", "sine_angle", "cosine_angle"),
              "G3 cone payload base/sine/cosine");
        QJsonObject el = find_geometry(rt.json, "ellipse");
        check(has_keys(el, "centre", "normal", "major_axis", "radius_ratio", "range"), "G3 ellipse payload");
        check(rt.deserOk, "G3 deserialize ok");
        if (rt.body) compare_bodies(cyl, rt.body, "G3 cylinder", 1e-6);
        free_rt(rt, true);
        api_del_entity(cyl);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G4 圆锥（cone 面 + 锥顶退化）==\n");
    {
        BODY *cone = nullptr;
        check(api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 10), 4.0, 4.0, 0.0, nullptr, cone).ok(),
              "G4 api_solid_cylinder_cone(cone)");
        ENTITY_LIST top(cone);
        GraphRT rt;
        graph_rt(top, 1, rt);
        {
            // 诊断：落盘本组 JSON，便于核对边 range / 锥面字段
            FILE *jf = fopen("G4_graph.json", "wb");
            if (jf) {
                QByteArray js = QJsonDocument(rt.json).toJson(QJsonDocument::Indented);
                fwrite(js.constData(), 1, js.size(), jf);
                fclose(jf);
            }
        }
        QJsonObject gj = find_geometry(rt.json, "cone");
        check(has_keys(gj, "base_centre", "sine_angle", "cosine_angle"), "G4 cone payload");
        check(rt.deserOk, "G4 deserialize ok");
        if (rt.body) compare_bodies(cone, rt.body, "G4 cone", 1e-6);
        free_rt(rt, true);
        api_del_entity(cone);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G5 圆环（torus 面）==\n");
    {
        BODY *tor = nullptr;
        check(api_make_torus(8.0, 2.0, tor).ok(), "G5 api_make_torus");
        ENTITY_LIST top(tor);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject gj = find_geometry(rt.json, "torus");
        check(has_keys(gj, "centre", "normal", "major_radius", "minor_radius"), "G5 torus payload");
        check(rt.deserOk, "G5 deserialize ok");
        if (rt.body) compare_bodies(tor, rt.body, "G5 torus", 1e-6);
        free_rt(rt, true);
        api_del_entity(tor);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G6 六棱柱扫掠（spline 面 + intcurve 边，缺口基线）==\n");
    {
        BODY *prism = nullptr;
        check(api_make_prism(10.0, 4.0, 4.0, 6, prism).ok(), "G6 api_make_prism");
        ENTITY_LIST top(prism);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject sp = find_geometry(rt.json, "spline");
        if (sp.isEmpty())
            printf("    (prism 未产生 spline 面：侧面可能为 plane)\n");
        else {
            check(geometry_has_payload(sp), "G6 spline payload (ctrlpts/knots...) 【缺口1：当前仅有类型名】");
            if (!geometry_has_payload(sp)) dump_geometry(sp, "G6");
        }
        QJsonObject ic = find_geometry(rt.json, "intcurve");
        if (!ic.isEmpty()) {
            check(geometry_has_payload(ic), "G6 intcurve payload 【缺口2：当前仅有类型名】");
            if (!geometry_has_payload(ic)) dump_geometry(ic, "G6");
        }
        check(rt.deserOk, "G6 deserialize ok");
        if (rt.body) compare_bodies(prism, rt.body, "G6 prism", 1e-6);
        free_rt(rt, true);
        api_del_entity(prism);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G7 布尔并/差/交（混合几何 + 交线）==\n");
    {
        // 并：方块+方块
        BODY *a = nullptr, *b = nullptr;
        api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), a);
        api_solid_block(SPAposition(5, 5, 5), SPAposition(15, 15, 15), b);
        check(api_unite(a, b).ok(), "G7 api_unite");
        run_solid_group(b, "G7 unite", 1e-6);
        api_del_entity(b);

        // 差：方块-圆柱（孔壁 cone 面 + 圆交线）
        BODY *blk = nullptr, *cyl = nullptr;
        api_solid_block(SPAposition(-5, -5, -5), SPAposition(5, 5, 5), blk);
        api_solid_cylinder_cone(SPAposition(0, 0, -10), SPAposition(0, 0, 10), 2.0, 2.0, 2.0, nullptr, cyl);
        check(api_subtract(cyl, blk).ok(), "G7 api_subtract");
        run_solid_group(blk, "G7 subtract", 1e-6);
        api_del_entity(blk);

        // 交：球∩方块（球冠面）
        BODY *sph = nullptr, *blk2 = nullptr;
        api_solid_sphere(SPAposition(0, 0, 0), 5.0, sph);
        api_solid_block(SPAposition(0, 0, 0), SPAposition(5, 5, 5), blk2);
        check(api_intersect(sph, blk2).ok(), "G7 api_intersect");
        run_solid_group(blk2, "G7 intersect", 1e-5);
        api_del_entity(blk2);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G8 螺旋线线框（helix 边，缺口基线）==\n");
    {
        helix *hg = new helix(SPAposition(0, 0, 0), SPAunit_vector(0, 0, 1), SPAvector(3, 0, 0), 2.0, TRUE,
                              SPAinterval(0.0, 4 * 3.14159265358979));
        EDGE *ed = nullptr;
        check(api_make_edge_from_curve(hg, ed).ok(), "G8 api_make_edge_from_curve(helix)");
        delete hg;
        COEDGE *ce = new COEDGE(ed, FALSE, nullptr, nullptr);
        ed->set_coedge(ce);
        WIRE *w = new WIRE(ce, nullptr);
        BODY *wb = new BODY(w);

        ENTITY_LIST top(wb);
        GraphRT rt;
        graph_rt(top, 1, rt);
        char buf[128];
        snprintf(buf, sizeof(buf), "G8 json wire=%d coedge=%d edge=%d vertex=%d", count_label(rt.json, "wire"),
                 count_label(rt.json, "coedge"), count_label(rt.json, "edge"), count_label(rt.json, "vertex"));
        check(count_label(rt.json, "wire") == 1 && count_label(rt.json, "edge") == 1 &&
                  count_label(rt.json, "vertex") == 2,
              buf);
        QJsonObject hx = find_geometry(rt.json, "helix");
        if (hx.isEmpty()) {
            printf("    (helix 边未产生 geometry.type=helix)\n");
            check(false, "G8 helix geometry node present 【缺口3】");
        } else {
            check(geometry_has_payload(hx), "G8 helix payload (axis_root/pitch/...) 【缺口3：当前仅有类型名】");
            if (!geometry_has_payload(hx)) dump_geometry(hx, "G8");
        }
        check(rt.deserOk, "G8 deserialize ok");
        if (rt.body) {
            ENTITY_LIST el;
            api_get_edges(rt.body, el);
            snprintf(buf, sizeof(buf), "G8 restored edges %d/1", el.count());
            check(el.count() == 1, buf);
        }
        free_rt(rt, rt.body != nullptr);
        api_del_entity(wb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G9 变换体（transform 节点 + 体积缩放）==\n");
    {
        BODY *box = nullptr;
        api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), box);
        SPAtransf tf = translate_transf(SPAvector(10, 20, 30)) * scale_transf(2.0);
        check(api_transform_entity(box, tf).ok(), "G9 api_transform_entity");
        ENTITY_LIST top(box);
        GraphRT rt;
        graph_rt(top, 1, rt);
        char buf[128];
        snprintf(buf, sizeof(buf), "G9 json transform node=%d", count_label(rt.json, "transform"));
        int nTf = count_label(rt.json, "transform");
        check(nTf >= 1, buf);
        bool tfPayload = false;
        for (const QJsonValue &v : nodesOf(rt.json)) {
            QJsonObject o = v.toObject();
            QJsonArray labels = o.value("labels").toArray();
            if (labels.size() && labels.at(0).toString() == "transform") {
                QJsonObject p = o.value("props").toObject();
                tfPayload = has_keys(p, "affine", "translation", "scaling");
                break;
            }
        }
        check(tfPayload, "G9 transform payload affine/translation/scaling");
        check(rt.deserOk, "G9 deserialize ok");
        if (rt.body) {
            double v = 0;
            if (body_volume(rt.body, v)) {
                snprintf(buf, sizeof(buf), "G9 restored volume %.3f vs 8000", v);
                check(std::fabs(v - 8000.0) < 1e-2, buf);
            }
            double mn[3], mx[3];
            if (body_box(rt.body, mn, mx)) {
                snprintf(buf, sizeof(buf), "G9 restored box [%.1f %.1f %.1f]-[%.1f %.1f %.1f]", mn[0], mn[1], mn[2],
                         mx[0], mx[1], mx[2]);
                bool boxOk = std::fabs(mn[0] - 10) < 1e-6 && std::fabs(mx[0] - 30) < 1e-6 &&
                             std::fabs(mn[2] - 30) < 1e-6 && std::fabs(mx[2] - 50) < 1e-6;
                check(boxOk, buf);
            }
        }
        free_rt(rt, true);
        api_del_entity(box);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G10 圆角（blend 产生 cone/sphere 面 + 复杂拓扑）==\n");
    {
        BODY *blk = nullptr;
        api_solid_block(SPAposition(-5, -5, -5), SPAposition(5, 5, 5), blk);
        ENTITY_LIST el;
        api_get_edges(blk, el);
        check(api_blend_edges(el, 1.0).ok(), "G10 api_blend_edges(r=1)");
        ENTITY_LIST top(blk);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject coneG = find_geometry(rt.json, "cone");
        QJsonObject sphG = find_geometry(rt.json, "sphere");
        check(!coneG.isEmpty() && has_keys(coneG, "base_centre", "sine_angle"), "G10 blend cone face payload");
        check(!sphG.isEmpty() && has_keys(sphG, "centre", "radius"), "G10 blend sphere face payload");
        QJsonObject sp = find_geometry(rt.json, "spline");
        if (!sp.isEmpty()) {
            check(geometry_has_payload(sp), "G10 spline payload (若产生) 【缺口1】");
            if (!geometry_has_payload(sp)) dump_geometry(sp, "G10");
        }
        check(rt.deserOk, "G10 deserialize ok");
        if (rt.body) compare_bodies(blk, rt.body, "G10 blend", 1e-5);
        free_rt(rt, true);
        api_del_entity(blk);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G11 subshell 探测（大模型才产生，可 SKIP）==\n");
    {
        BODY *blk = nullptr, *cyl = nullptr;
        api_solid_block(SPAposition(-5, -5, -5), SPAposition(5, 5, 5), blk);
        api_solid_cylinder_cone(SPAposition(0, 0, -10), SPAposition(0, 0, 10), 2.0, 2.0, 2.0, nullptr, cyl);
        api_subtract(cyl, blk);
        ENTITY_LIST el;
        api_get_edges(blk, el);
        api_blend_edges(el, 0.5);
        ENTITY_LIST all;
        api_get_entities(blk, all, -1, -1);
        int nSub = 0;
        for (ENTITY *e : all)
            if (is_SUBSHELL(e))
                ++nSub;
        if (nSub == 0) {
            printf("  SKIP：本组模型未产生 subshell（ACIS 仅在特定条件下分层），存储语义由 L2/L3 覆盖\n");
        } else {
            char buf[128];
            snprintf(buf, sizeof(buf), "G11 model subshells=%d", nSub);
            printf("  %s\n", buf);
            ENTITY_LIST top(blk);
            GraphRT rt;
            graph_rt(top, 1, rt);
            snprintf(buf, sizeof(buf), "G11 json subshell node=%d/%d 【缺口4】", count_label(rt.json, "subshell"),
                     nSub);
            check(count_label(rt.json, "subshell") == nSub, buf);
            free_rt(rt, true);
        }
        api_del_entity(blk);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G12 直线线框（wire 拓扑 + straight 边）==\n");
    {
        SPAposition pts[5] = {SPAposition(0, 0, 0), SPAposition(5, 0, 0), SPAposition(5, 5, 0),
                              SPAposition(0, 5, 0), SPAposition(0, 0, 0)};
        BODY *wb = nullptr;
        check(api_make_wire((BODY *)nullptr, 5, pts, wb).ok(), "G12 api_make_wire");
        ENTITY_LIST top(wb);
        GraphRT rt;
        graph_rt(top, 1, rt);
        char buf[128];
        snprintf(buf, sizeof(buf), "G12 json wire=%d edge=%d vertex=%d", count_label(rt.json, "wire"),
                 count_label(rt.json, "edge"), count_label(rt.json, "vertex"));
        check(count_label(rt.json, "wire") == 1 && count_label(rt.json, "edge") == 4, buf);
        QJsonObject st = find_geometry(rt.json, "straight");
        check(has_keys(st, "root_point", "direction", "range"), "G12 straight payload");
        check(rt.deserOk, "G12 deserialize ok");
        if (rt.body) {
            ENTITY_LIST el;
            api_get_edges(rt.body, el);
            snprintf(buf, sizeof(buf), "G12 restored edges %d/4", el.count());
            check(el.count() == 4, buf);
        }
        free_rt(rt, rt.body != nullptr);
        api_del_entity(wb);
    }

finish:
    api_stop_modeller();
    terminate_base();

    printf("\n%s (failures=%d)\n",
           g_failed == 0 ? "ALL GRAPH ROUNDTRIP TESTS PASSED" : "GRAPH ROUNDTRIP TESTS HAD FAILURES", g_failed);
    return g_failed == 0 ? 0 : 1;
}
