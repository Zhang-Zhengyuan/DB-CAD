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
#include <swp_opts.hxx>
#include <swp_spl.hxx>
#include <rot_spl.hxx>
#include <ellipse.hxx>
#include <deprecated_masspr.hxx>
#include <tensor.hxx>
#include <heldef.hxx>
#include <blendapi.hxx>
#include <transfrm.hxx>
#include <sps3crtn.hxx>
#include <curve.hxx>
#include <straight.hxx>
#include <intcurve.hxx>
#include <skinapi.hxx>
#include <sps3srtn.hxx>
#include <exct_spl.hxx>
#include "spldef.hxx"
#include <pcurve.hxx>
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
    } else if (okO || okR) {
        // 单侧失败才是错误；双侧失败 = 开放体（sheet/wire），无体积可比
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

    // 边总长度：弧段参数化/方向错乱会导致长度变化（比体积更敏感的曲线级指标）
    {
        auto totalEdgeLen = [](BODY *b) {
            double sum = 0;
            ENTITY_LIST el;
            api_get_edges(b, el);
            for (ENTITY *e : el)
                if (is_EDGE(e))
                    sum += ((EDGE *)e)->length();
            return sum;
        };
        double lo = totalEdgeLen(orig), lr = totalEdgeLen(rest);
        double rel = std::fabs(lo - lr) / (std::fabs(lo) > 1 ? std::fabs(lo) : 1);
        snprintf(buf, sizeof(buf), "%s: edge length %.6f vs %.6f (rel %.1e)", tag, lr, lo, rel);
        check(rel < 1e-6, buf);
    }

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
    int run_upto = (argc > 1) ? atoi(argv[1]) : 18;
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
            FILE *jf = fopen("artifacts/G4_graph.json", "wb");
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

        // G8b：intcurve 载荷冒烟——api_mk_ed_bs3_curve 直接产生 NURBS 支撑的 INTCURVE 边
        printf("  -- G8b intcurve（bs3 载荷往返）--\n");
        {
            SPAposition cps[4] = {SPAposition(0, 0, 0), SPAposition(3, 0, 2), SPAposition(6, 0, -2),
                                  SPAposition(9, 0, 0)};
            double knotArr[8] = {0, 0, 0, 0, 1, 1, 1, 1};
            EDGE *ned = nullptr;
            check(api_mk_ed_bs3_curve(bs3_curve_from_ctrlpts(3, 0, 0, 0, 4, cps, nullptr, SPAresabs.value(), 8,
                                                             knotArr, SPAresnor.value()),
                                      ned)
                      .ok(),
                  "G8b api_mk_ed_bs3_curve");
            COEDGE *nce = new COEDGE(ned, FALSE, nullptr, nullptr);
            ned->set_coedge(nce);
            WIRE *nw = new WIRE(nce, nullptr);
            BODY *nwb = new BODY(nw);

            ENTITY_LIST top2(nwb);
            GraphRT rt2;
            graph_rt(top2, 1, rt2);
            QJsonObject icg = find_geometry(rt2.json, "intcurve");
            snprintf(buf, sizeof(buf), "G8b json intcurve payload ctrlpts=%d knots=%d",
                     icg.value("ctrlpts").toArray().size(), icg.value("knots").toArray().size());
            check(geometry_has_payload(icg) && icg.value("ctrlpts").toArray().size() >= 12, buf);
            check(rt2.deserOk, "G8b deserialize ok");
            if (rt2.body) {
                ENTITY_LIST el2;
                api_get_edges(rt2.body, el2);
                snprintf(buf, sizeof(buf), "G8b restored edges %d/1", el2.count());
                check(el2.count() == 1, buf);
                if (el2.count() == 1) {
                    EDGE *re = (EDGE *)el2[0];
                    bool isInt = re->geometry() && re->geometry()->identity(2) == INTCURVE_ID;
                    check(isInt, "G8b restored edge geometry is INTCURVE");
                    double l1 = ned->length(), l2 = re->length();
                    snprintf(buf, sizeof(buf), "G8b edge length %.6f vs %.6f", l2, l1);
                    check(std::fabs(l1 - l2) < 1e-6, buf);
                }
            }
            free_rt(rt2, rt2.body != nullptr);
            api_del_entity(nwb);
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
        // 原始体基准：api_transform_entity 会重定位几何并把剩余变换存入
        // TRANSFORM 实体，期望值必须取实测（历史教训：曾按手算 [10,30] 断言误判）
        double oVol = 0, omn[3], omx[3];
        bool haveVol = body_volume(box, oVol);
        bool haveBox = body_box(box, omn, omx);

        ENTITY_LIST top(box);
        GraphRT rt;
        graph_rt(top, 1, rt);
        char buf[128];
        snprintf(buf, sizeof(buf), "G9 json transform node=%d", count_label(rt.json, "transform"));
        check(count_label(rt.json, "transform") >= 1, buf);
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
        check(rt.body != nullptr && rt.body->transform() != nullptr, "G9 restored body carries TRANSFORM");
        if (rt.body && haveVol) {
            double v = 0;
            if (body_volume(rt.body, v)) {
                snprintf(buf, sizeof(buf), "G9 volume %.3f vs %.3f", v, oVol);
                check(std::fabs(v - oVol) < 1e-2, buf);
            } else {
                check(false, "G9 restored mass_pr failed");
            }
        }
        if (rt.body && haveBox) {
            double mn[3], mx[3];
            if (body_box(rt.body, mn, mx)) {
                double dmax = 0;
                for (int i = 0; i < 3; ++i) {
                    dmax = std::max(dmax, std::fabs(mn[i] - omn[i]));
                    dmax = std::max(dmax, std::fabs(mx[i] - omx[i]));
                }
                snprintf(buf, sizeof(buf), "G9 bbox delta %.3e", dmax);
                check(dmax < 1e-6, buf);
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
    printf("== G11 subshell（自然探测 + 合成接线回归）==\n");
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
            // 合成接线：常规建模不触发 ACIS 分层，手工构造 subshell 树接入 shell，
            // 使序列化/反序列化的 subshell 编解码路径获得真实往返覆盖。
            SHELL *shell = blk->lump()->shell();
            ENTITY_LIST fl;
            api_get_faces(blk, fl);
            if (shell != nullptr && fl.count() >= 2) {
                API_BEGIN;
                SUBSHELL *ss1 = ACIS_NEW SUBSHELL();
                SUBSHELL *ss2 = ACIS_NEW SUBSHELL();
                shell->set_subshell(ss1);
                ss1->set_child(ss2);
                ss2->set_parent(ss1);
                ss1->set_face((FACE *)fl[0]);
                ((FACE *)fl[0])->set_subshell(ss1);
                ss2->set_face((FACE *)fl[1]);
                ((FACE *)fl[1])->set_subshell(ss2);
                API_END;
                nSub = 2;
                printf("  (ACIS 未自然分层，已合成接线 2 个 subshell 验证编解码)\n");
            }
        }

        char buf[160];
        snprintf(buf, sizeof(buf), "G11 model subshells=%d", nSub);
        check(nSub >= 1, buf);
        if (nSub >= 1) {
            ENTITY_LIST top(blk);
            GraphRT rt;
            graph_rt(top, 1, rt);
            snprintf(buf, sizeof(buf), "G11 json subshell node=%d/%d", count_label(rt.json, "subshell"), nSub);
            check(count_label(rt.json, "subshell") == nSub, buf);
            int nShellSub = 0, nParent = 0, nChild = 0, nSsFace = 0;
            for (const QJsonValue &rv : rt.json.value("rels").toArray()) {
                QJsonObject r = rv.toObject();
                QString t = r.value("type").toString();
                if (t == "shell_subshell")
                    nShellSub++;
                else if (t == "subshell_parent")
                    nParent++;
                else if (t == "subshell_child")
                    nChild++;
                else if (t == "subshell_face" || t == "face_subshell")
                    nSsFace++;
            }
            snprintf(buf, sizeof(buf), "G11 json rels shell_subshell=%d parent=%d child=%d face=%d", nShellSub,
                     nParent, nChild, nSsFace);
            check(nShellSub >= 1 && nParent >= 1 && nChild >= 1 && nSsFace >= 2, buf);
            check(rt.deserOk, "G11 deserialize ok");
            if (rt.body) {
                // 手动受控遍历统计 subshell：全级 api_get_entities 在
                // face↔subshell 互指的合成结构上不终止（实测），带步数上限替代。
                int nSubR = 0, guard = 0;
                for (LUMP *lp = rt.body->lump(); lp && guard++ < 100; lp = lp->next()) {
                    for (SHELL *sh = lp->shell(); sh && guard++ < 100; sh = sh->next()) {
                        for (SUBSHELL *ss = sh->subshell(); ss && guard++ < 100; ss = ss->sibling()) {
                            ++nSubR;
                            for (SUBSHELL *c = ss->child(); c && guard++ < 100; c = c->sibling())
                                ++nSubR;
                        }
                    }
                }
                snprintf(buf, sizeof(buf), "G11 restored subshells=%d/%d", nSubR, nSub);
                check(nSubR == nSub, buf);
                if (nSubR == 2) {
                    // 链路完整性：shell→subshell 父子树 + subshell→face 直属
                    SHELL *rshell = rt.body->lump()->shell();
                    SUBSHELL *rs1 = rshell ? rshell->subshell() : nullptr;
                    check(rs1 != nullptr && rs1->child() != nullptr && rs1->child()->parent() == rs1,
                          "G11 restored subshell tree links");
                    check(rs1 != nullptr && rs1->face() != nullptr && rs1->face()->subshell() == rs1,
                          "G11 restored subshell-face links");
                }
                // 不对合成体做 mass_pr/几何比对：手工接线的 subshell 双亲归属
                //（face 同时挂 shell 与 subshell）会使质量积分遍历不终止；
                // 几何一致性由 G1-G10/G13 覆盖，本组只验 subshell 编解码。
                (void)0;
            }
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

    if (++g_id > run_upto) goto finish;
    printf("== G13 样条曲面（扭转六边形放样 → spline 面，缺口1）==\n");
    {
        // 两个错角六边形线框，skin 放样出扭曲侧面（spline 面）
        SPAposition hex1[7], hex2[7];
        for (int k = 0; k < 6; ++k) {
            double a1 = k * 3.14159265358979 / 3.0;
            double a2 = a1 + 0.523598775598299; // 30 度扭转
            hex1[k] = SPAposition(4 * cos(a1), 4 * sin(a1), 0);
            hex2[k] = SPAposition(5 * cos(a2), 5 * sin(a2), 10);
        }
        hex1[6] = hex1[0];
        hex2[6] = hex2[0];
        BODY *w1 = nullptr, *w2 = nullptr;
        check(api_make_wire((BODY *)nullptr, 7, hex1, w1).ok(), "G13 wire1 (hexagon)");
        check(api_make_wire((BODY *)nullptr, 7, hex2, w2).ok(), "G13 wire2 (twisted hexagon)");
        BODY *wires[2] = {w1, w2};
        BODY *skinned = nullptr;
        outcome sk = api_skin_wires(2, wires, skinned);
        if (!sk.ok())
            printf("    [diag] skin err #%d: %s\n", sk.error_number(), find_err_mess(sk.error_number()));
        check(sk.ok(), "G13 api_skin_wires");
        api_del_entity(w1);
        api_del_entity(w2);
        if (skinned != nullptr) {
            ENTITY_LIST fl;
            api_get_faces(skinned, fl);
            int nSpl = 0;
            for (ENTITY *e : fl) {
                FACE *f = (FACE *)e;
                if (f->geometry() && f->geometry()->identity(2) == SPLINE_ID)
                    nSpl++;
            }
            char buf[160];
            snprintf(buf, sizeof(buf), "G13 model spline faces=%d/%d", nSpl, fl.count());
            check(nSpl >= 1, buf);
            ENTITY_LIST top(skinned);
            GraphRT rt;
            graph_rt(top, 1, rt);
            QJsonObject sp = find_geometry(rt.json, "spline");
            if (sp.isEmpty()) {
                check(false, "G13 json spline payload present");
            } else {
                snprintf(buf, sizeof(buf), "G13 spline payload ctrlpts=%d uknots=%d vknots=%d deg=%d/%d",
                         sp.value("ctrlpts").toArray().size(), sp.value("uknots").toArray().size(),
                         sp.value("vknots").toArray().size(), sp.value("degree_u").toInt(-1),
                         sp.value("degree_v").toInt(-1));
                check(sp.value("ctrlpts").toArray().size() >= 12, buf);
            }
            check(rt.deserOk, "G13 deserialize ok");
            // 体积已恢复为真实检查：原 KNOWN-LIMIT（怀疑 exactsur 子类被积分器
            // 拒绝）经隔离实验证伪——真实根因是 pcurve 装配把 SURFACE 实体当
            // 方程对象解引用（类型混淆 → mass_pr 求值 AV），修复后 exactsur 完全可用。
            if (rt.body) {
                compare_bodies(skinned, rt.body, "G13 skin-solid", 1e-5);
            }
            free_rt(rt, true);
            api_del_entity(skinned);
        }
    }

    if (++g_id > run_upto) goto finish;
    printf("== G14 保真回退（sweepsur 组件 → SAT 片段；干净体保持细粒度）==\n");
    {
        // 正例：圆截面沿螺旋扫掠 → sweepsur（spl_sur::type()=67，过程曲面），
        // NURBS 仅拟合（探针实证恢复体 mass_pr #111001）→ 必须触发组件级
        // SAT 片段回退，恢复端整组件走内核读回。
        helix *hg = new helix(SPAposition(0, 0, 0), SPAunit_vector(0, 0, 1), SPAvector(3, 0, 0), 1.5, TRUE,
                              SPAinterval(0.0, 6 * 3.14159265358979));
        EDGE *pathEd = nullptr;
        check(api_make_edge_from_curve(hg, pathEd).ok(), "G14 helix path edge");
        delete hg;
        ellipse *circ = new ellipse(SPAposition(0, 0, 0), SPAunit_vector(1, 0, 0), SPAvector(0, 0.4, 0), 1.0);
        EDGE *profEd = nullptr;
        check(api_make_edge_from_curve(circ, profEd).ok(), "G14 circle profile edge");
        delete circ;
        sweep_options so;
        BODY *tube = nullptr;
        check(api_sweep_with_options(profEd, pathEd, &so, tube).ok(), "G14 api_sweep(circle→helix)");
        ENTITY_LIST top(tube);
        GraphRT rt;
        graph_rt(top, 1, rt);
        bool hasFrag = false;
        for (const QJsonValue &v : nodesOf(rt.json)) {
            QJsonObject n = v.toObject();
            if (n.value("labels").toArray()[0].toString() == "body" &&
                n.value("props").toObject().value("fallback").toString() == "sat_fragment")
                hasFrag = true;
        }
        check(hasFrag, "G14 sweepsur body carries sat_fragment");
        check(rt.deserOk, "G14 deserialize ok (fragment path)");
        if (rt.body) {
            double vo = 0, vr = 0;
            bool okO = body_volume(tube, vo), okR = body_volume(rt.body, vr);
            check(okO && okR, "G14 tube mass_pr both succeed (fragment fixes #111001)");
            compare_bodies(tube, rt.body, "G14 tube(fallback)", 1e-9);
        }
        free_rt(rt, true);
        // [R2] 纯图路径保真实验：剥离 sat_fragment 后重建，隔离测 sweepsur 的
        // NURBS 残余偏差（该体当前因 ATTRIB(make_tangent) 触发片段）。
        {
            QJsonObject jf = rt.json;
            QJsonArray na = jf["nodes"].toArray();
            for (int k = 0; k < na.size(); ++k) {
                QJsonObject nn = na[k].toObject();
                if (nn["labels"].toArray()[0].toString() == "body") {
                    QJsonObject pp = nn["props"].toObject();
                    pp.remove("sat_fragment");
                    pp.remove("fallback");
                    nn["props"] = pp;
                    na.replace(k, nn);
                }
            }
            jf["nodes"] = na;
            DeserializedEntityGraph dg;
            QString eg;
            bool okG = deserializeACISEntityGraph(jf, &dg, &eg);
            check(okG, "G14 [R2] graph-path deserialize ok");
            BODY *tube2 = nullptr;
            if (okG && dg.entities)
                for (ENTITY *e = dg.entities->first(); e; e = dg.entities->next())
                    if (is_BODY(e)) {
                        tube2 = (BODY *)e;
                        break;
                    }
            if (tube2) {
                double vo2 = 0, vr2 = 0;
                bool o2 = body_volume(tube, vo2), r2v = body_volume(tube2, vr2);
                // 负结果存证（R2 实验 2026-10-15）：sweepsur 的 NURBS 仅为粗逼近
                // （采样偏差 3.1e-4），且 rail/draft/scale 三律非空、law 家族无公共
                // 构造 API → 片段回退是 67 的正确终态，不得无真重建而白名单化。
                printf("    [R2] graph-path mass_pr orig=%d rest=%d (negative result: NURBS not authoritative)\n",
                       (int)o2, (int)r2v);
                double lo2 = 0, lr2 = 0;
                {
                    ENTITY_LIST e1, e2;
                    api_get_edges(tube, e1);
                    api_get_edges(tube2, e2);
                    for (ENTITY *e : e1)
                        if (is_EDGE(e))
                            lo2 += ((EDGE *)e)->length();
                    for (ENTITY *e : e2)
                        if (is_EDGE(e))
                            lr2 += ((EDGE *)e)->length();
                }
                check(fabs(lo2 - lr2) / (fabs(lo2) > 1 ? fabs(lo2) : 1) < 1e-6, "G14 [R2] graph-path edge length");
                // 曲面采样偏差（10x10 参数网格）：过程面 vs NURBS 重建面
                auto sweepEq = [](BODY *b) -> const spl_sur * {
                    ENTITY_LIST fl;
                    api_get_faces(b, fl);
                    for (ENTITY *f : fl) {
                        if (!is_FACE(f))
                            continue;
                        SURFACE *g = ((FACE *)f)->geometry();
                        if (g && g->identity(2) == SPLINE_ID)
                            return &static_cast<const spline &>(((SPLINE *)g)->equation()).get_spl_sur();
                    }
                    return nullptr;
                };
                const spl_sur *so = sweepEq(tube), *sr2 = sweepEq(tube2);
                char buf2[128];
                if (so && sr2) {
                    // [R2 探测] 原件 sweepsur 的律状态（rail/draft/scale 是否为空）
                    if (so->type() == 67) {
                        const sweep_spl_sur *sw = (const sweep_spl_sur *)so;
                        law *rl = sw->get_rail(), *df = sw->get_draft(), *sc = sw->get_scale();
                        printf("    [R2-probe] orig sweepsur rail=%d draft=%d scale=%d path-type=%d planar=%d\n",
                               rl ? 1 : 0, df ? 1 : 0, sc ? 1 : 0, (int)sw->get_path_type(),
                               (int)sw->planar_profile());
                    }
                    SPApar_box pb = so->param_range();
                    SPAinterval ur = pb.u_range(), vrr = pb.v_range();
                    double maxDev = 0;
                    for (int i = 0; i <= 10; ++i)
                        for (int j = 0; j <= 10; ++j) {
                            double u = ur.start_pt() + (ur.end_pt() - ur.start_pt()) * i / 10;
                            double v = vrr.start_pt() + (vrr.end_pt() - vrr.start_pt()) * j / 10;
                            SPAposition p1 = so->eval_position(SPApar_pos(u, v));
                            SPAposition p2 = sr2->eval_position(SPApar_pos(u, v));
                            maxDev = fmax(maxDev, (p1 - p2).len());
                        }
                    snprintf(buf2, sizeof(buf2), "G14 [R2] surface sampling max dev=%.3e in (1e-6, 1e-3) "
                                                 "(NURBS is approximation; fragment is terminal for 67)",
                             maxDev);
                    check(maxDev > 1e-6 && maxDev < 1e-3, buf2);
                } else {
                    check(false, "G14 [R2] spline face equations found");
                }
                for (ENTITY *e = dg.entities->first(); e; e = dg.entities->next())
                    if (is_BODY(e))
                        api_del_entity((BODY *)e);
                delete dg.entities;
            }
        }
        // 负例：干净方块不触发回退（保持实体级细粒度——回退是逐组件的）
        BODY *blk = nullptr;
        check(api_solid_block(SPAposition(0, 0, 0), SPAposition(5, 5, 5), blk).ok(), "G14 api_solid_block");
        ENTITY_LIST top2(blk);
        GraphRT rt2;
        graph_rt(top2, 1, rt2);
        bool frag2 = false;
        for (const QJsonValue &v : nodesOf(rt2.json)) {
            QJsonObject n = v.toObject();
            if (n.value("labels").toArray()[0].toString() == "body" &&
                n.value("props").toObject().value("fallback").toString() == "sat_fragment")
                frag2 = true;
        }
        check(!frag2, "G14 clean block has NO forced SAT (stays fine-grained)");
        check(rt2.deserOk, "G14 block deserialize ok (graph path)");
        if (rt2.body)
            compare_bodies(blk, rt2.body, "G14 block(fine-grained)", 1e-9);
        free_rt(rt2, true);
        api_del_entity(blk);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G15 交线定义载荷（两圆柱相交 → intcurve surf1/surf2 真定义重建）==\n");
    {
        BODY *c1 = nullptr, *c2 = nullptr;
        check(api_solid_cylinder_cone(SPAposition(0, 0, -10), SPAposition(0, 0, 10), 4.0, 4.0, 4.0, nullptr, c1).ok(),
              "G15 cylinder1");
        check(api_solid_cylinder_cone(SPAposition(-10, 3, 0), SPAposition(10, 3, 0), 3.0, 3.0, 3.0, nullptr, c2).ok(),
              "G15 cylinder2");
        outcome ir = api_intersect(c1, c2);
        check(ir.ok(), "G15 api_intersect (两圆柱 → 四次交线)");
        BODY *res = c2; // 结果在 blank（第二参数）
        int nInt = 0;
        {
            ENTITY_LIST el;
            api_get_edges(res, el);
            for (ENTITY *e : el)
                if (is_EDGE(e) && ((EDGE *)e)->geometry() &&
                    ((EDGE *)e)->geometry()->identity(2) == INTCURVE_ID)
                    nInt++;
        }
        char buf[96];
        snprintf(buf, sizeof(buf), "G15 model intcurve edges=%d", nInt);
        check(nInt >= 1, buf);
        ENTITY_LIST top(res);
        GraphRT rt;
        graph_rt(top, 1, rt);
        QJsonObject ic = find_geometry(rt.json, "intcurve");
        // 交线常为 parcur 型（仅一张定义曲面，subtype 25）；断言取 def_surf1
        bool hasDef = !ic.isEmpty() && ic.contains("def_surf1") &&
                      ic.value("def_surf1").toObject().value("type").toString() == "cone";
        check(hasDef, "G15 json intcurve payload has def_surf1 (cone)");
        bool frag = false;
        for (const QJsonValue &v : nodesOf(rt.json)) {
            QJsonObject n = v.toObject();
            if (n.value("labels").toArray()[0].toString() == "body" &&
                n.value("props").toObject().value("fallback").toString() == "sat_fragment")
                frag = true;
        }
        check(!frag, "G15 no forced SAT (analytic definitions stay fine-grained)");
        check(rt.deserOk, "G15 deserialize ok");
        if (rt.body) {
            compare_bodies(res, rt.body, "G15 cyl-intersect", 1e-9);
            // R1 保真判据：恢复体 SAT 落盘无 null_surface（定义曲面真实存在；
            // 修复前恢复体带 null_surface/nullbs 记录，破坏下游 mass_pr 等算法）
            {
                FILE *tf = tmpfile();
                if (tf) {
                    ENTITY_LIST lr(rt.body);
                    api_save_entity_list(tf, TRUE, lr);
                    fseek(tf, 0, SEEK_END);
                    long sz = ftell(tf);
                    rewind(tf);
                    std::string sat((size_t)sz, '\0');
                    fread(&sat[0], 1, (size_t)sz, tf);
                    fclose(tf);
                    FILE *dbg = fopen("G15_rest.sat", "wb");
                    if (dbg) {
                        fwrite(sat.data(), 1, sat.size(), dbg);
                        fclose(dbg);
                    }
                    {
                        FILE *fo = fopen("G15_orig.sat", "wb");
                        if (fo) {
                            ENTITY_LIST lo(res);
                            api_save_entity_list(fo, TRUE, lo);
                            fclose(fo);
                        }
                    }
                    // 判据（与原件实测一致）：无 null_surface + 定义块存在（surfintcur）。
                    // 块内的 nullbs（参数 2D 曲线）原件本就为空，属忠实复刻。
                    check(sat.find("null_surface") == std::string::npos &&
                              sat.find("surfintcur") != std::string::npos,
                          "G15 restored SAT has no null_surface (real definition)");
                }
            }
        }
        free_rt(rt, true);
        api_del_entity(res);
    }

    if (++g_id > run_upto) goto finish;
    printf("== G16 旋转面过程载荷（rot_spl_sur 合成面：母线+轴精确重建，R2b）==\n");
    {
        // 两例母线：straight（载荷定长）/ ellipse（非平凡曲线载荷）。
        // 旋转 v 区间取 (0, π/2)，轴 z。rot_spl_sur 数据成员公开（axis_root/
        // axis_dir/cur），无 law → 过程重建在公开 API 下完全可达（与 sweepsur 对比）。
        const char *profTags[2] = {"straight", "ellipse"};
        for (int pi = 0; pi < 2; ++pi) {
            curve *prof = nullptr;
            if (pi == 0) {
                straight *st = ACIS_NEW straight(SPAposition(3, 0, 0), SPAunit_vector(0, 0, 1));
                st->limit(SPAinterval(0, 4));
                prof = st;
            } else {
                ellipse *el = ACIS_NEW ellipse(SPAposition(4, 0, 2), SPAunit_vector(0, 1, 0), SPAvector(0, 0, 1), 0.5);
                prof = el;
            }
            rot_spl_sur *rs = ACIS_NEW rot_spl_sur(*prof, SPAposition(0, 0, 0), SPAunit_vector(0, 0, 1),
                                              prof->param_range(), SPAinterval(0, 1.5707963267948966));
            ACIS_DELETE prof;
            // 参考副本（供采样比对）：SPLINE(*spEq) 深拷贝后 spEq/rs 生命周期独立
            rot_spl_sur *rsRef = ACIS_NEW rot_spl_sur(*rs);
            spline *spEq = ACIS_NEW spline(rs);
            SPLINE *spEnt = ACIS_NEW SPLINE(*spEq);
            ACIS_DELETE spEq;
            FACE *f = ACIS_NEW FACE(nullptr, nullptr, spEnt, 0);
            // 最小体链接线（反序列化以 body 计数判成功；loopless face 合法）
            BODY *fb = nullptr;
            api_body(fb);
            LUMP *lp = ACIS_NEW LUMP();
            SHELL *sh = ACIS_NEW SHELL();
            fb->set_lump(lp);
            lp->set_body(fb);
            lp->set_shell(sh);
            sh->set_lump(lp);
            sh->set_face(f);
            f->set_shell(sh);
            ENTITY_LIST top(fb);
            GraphRT rt;
            graph_rt(top, 1, rt);
            char buf[128];
            QJsonObject sj = find_geometry(rt.json, "spline");
            bool hasRot = !sj.isEmpty() && sj.contains("rot_profile") && sj.contains("rot_axis_root") &&
                          sj.contains("rot_uv") && sj.value("spl_subtype").toInt(-1) == 44;
            snprintf(buf, sizeof(buf), "G16(%s) json rot payload (subtype 44)", profTags[pi]);
            check(hasRot, buf);
            bool frag = false;
            for (const QJsonValue &v : nodesOf(rt.json)) {
                QJsonObject n = v.toObject();
                if (n.value("props").toObject().value("fallback").toString() == "sat_fragment")
                    frag = true;
            }
            snprintf(buf, sizeof(buf), "G16(%s) no forced SAT (44 fine-grained)", profTags[pi]);
            check(!frag, buf);
            check(rt.deserOk, "G16 deserialize ok");
            // 恢复面：子类保持 rot_spl_sur（44，非 exactsur NURBS）
            if (rt.body) {
                FACE *rf = nullptr;
                {
                    ENTITY_LIST flr;
                    api_get_faces(rt.body, flr);
                    if (flr.count() > 0)
                        rf = (FACE *)flr[0];
                }
                if (rf && rf->geometry() && rf->geometry()->identity(2) == SPLINE_ID) {
                    const spl_sur &ss =
                        static_cast<const spline &>(((SPLINE *)rf->geometry())->equation()).get_spl_sur();
                    snprintf(buf, sizeof(buf), "G16(%s) restored subtype=44 (got %d)", profTags[pi], ss.type());
                    check(ss.type() == 44, buf);
                    // 逐点采样（10x10 参数网格，域取自原件）：过程重建应零偏差
                    const rot_spl_sur *r1 = (const rot_spl_sur *)&ss;
                    SPApar_box prr = rsRef->param_range();
                    double u0 = prr.u_range().start_pt(), u1 = prr.u_range().end_pt();
                    double v0 = prr.v_range().start_pt(), v1 = prr.v_range().end_pt();
                    double maxDev = 0;
                    for (int i = 0; i <= 10; ++i)
                        for (int j = 0; j <= 10; ++j) {
                            double u = u0 + (u1 - u0) * i / 10;
                            double v = v0 + (v1 - v0) * j / 10;
                            SPAposition p1 = rsRef->eval_position(SPApar_pos(u, v));
                            SPAposition p2 = r1->eval_position(SPApar_pos(u, v));
                            maxDev = fmax(maxDev, (p1 - p2).len());
                        }
                    snprintf(buf, sizeof(buf), "G16(%s) sampling max dev=%.3e (exact rebuild)", profTags[pi],
                             maxDev);
                    check(maxDev < 1e-9, buf);
                    // 轴往返
                    snprintf(buf, sizeof(buf), "G16(%s) axis_root/axis_dir roundtrip", profTags[pi]);
                    check((r1->axis_root - rsRef->axis_root).len() < 1e-12 &&
                              (r1->axis_dir - rsRef->axis_dir).len() < 1e-12,
                          buf);
                    // rsRef 由 use-count 管理（析构私有），测试作用域内不释放
                } else {
                    check(false, "G16 restored face has spline surface");
                }
            }
            free_rt(rt, true);
            api_del_entity(fb);
        }
    }

    if (++g_id > run_upto) goto finish;
    printf("== G17 拉伸体属性普查（extrude：解析面 × ATTRIB 触发重叠调查，R3）==\n");
    {
        // 矩形线框沿 z 拉伸 8 —— 最常见 CAD 操作。若拉伸留属性而面全解析，
        // 则属性触发独立于 67 触发存在（R3 有独立增量）；否则完全重叠。
        SPAposition rect[5] = {SPAposition(2, 1, 0), SPAposition(6, 1, 0), SPAposition(6, 4, 0),
                               SPAposition(2, 4, 0), SPAposition(2, 1, 0)};
        BODY *rw = nullptr;
        check(api_make_wire((BODY *)nullptr, 5, rect, rw).ok(), "G17 api_make_wire(rect)");
        BODY *ext = nullptr;
        sweep_options so;
        EDGE *pathEd = nullptr;
        {
            straight *p = ACIS_NEW straight(SPAposition(4, 2.5, 0), SPAunit_vector(0, 0, 1));
            p->limit(SPAinterval(0, 8)); // 有界路径（无限直线路径扫掠会失败）
            api_make_edge_from_curve(p, pathEd);
            ACIS_DELETE p;
        }
        check(api_sweep_with_options(rw, pathEd, &so, ext).ok(), "G17 api_sweep(rect→straight)");
        if (!ext)
            ext = rw; // 线框轮廓时结果可能留在 profile body 里
        // 属性普查：遍历全实体 attrib() 链 + 面型普查
        {
            std::map<std::string, int> ac;
            int withAttrib = 0;
            std::map<int, int> fsub;
            ENTITY_LIST al;
            api_get_entities(ext, al, -1, -1);
            for (ENTITY *e : al) {
                if (is_FACE(e) && ((FACE *)e)->geometry())
                    fsub[(int)((FACE *)e)->geometry()->identity(2)]++;
                ATTRIB *a = e->attrib();
                if (a)
                    withAttrib++;
                for (; a; a = a->next())
                    ac[a->type_name()]++;
            }
            printf("    [G17] entities=%d with-attrib-entities=%d\n", al.count(), withAttrib);
            for (auto &kv : ac)
                printf("    [G17] attrib %-28s x%d\n", kv.first.c_str(), kv.second);
            for (auto &kv : fsub)
                printf("    [G17] face identity(2)=%d x%d\n", kv.first, kv.second);
        }
        ENTITY_LIST top(ext);
        GraphRT rt;
        graph_rt(top, 1, rt);
        bool frag = false;
        for (const QJsonValue &v : nodesOf(rt.json)) {
            QJsonObject n = v.toObject();
            if (n.value("labels").toArray()[0].toString() == "body" &&
                n.value("props").toObject().value("fallback").toString() == "sat_fragment")
                frag = true;
        }
        printf("    [G17] sat_fragment triggered = %d\n", (int)frag);
        check(rt.deserOk, "G17 deserialize ok");
        if (rt.body)
            compare_bodies(ext, rt.body, "G17 extrude", 1e-9);
        free_rt(rt, true);
        api_del_entity(ext);
    }


    if (++g_id > run_upto) goto finish;
    printf("== G18 hybrid fallback and stable standalone roots ==\n");
    {
        BODY *original = nullptr;
        check(api_make_cuboid(3, 4, 5, original).ok(), "G18 make cube");
        QHash<int, QString> ids{{0, "hybrid-cube"}};
        auto graph = serializeACISEntityGraph(ids, ENTITY_LIST(original));
        auto nodes = graph.value("nodes").toArray();
        bool backup = false;
        for (auto value : nodes) {
            auto node = value.toObject();
            if (node.value("id").toString() == "hybrid-cube") {
                auto props = node.value("props").toObject();
                backup = !props.value("sat_fragment").toString().isEmpty() &&
                         props.value("fallback").toString() == "on_failure";
            }
        }
        check(backup, "G18 supported cube has on_failure SAT backup");
        // Remove SAT from a supported component to prove the JSON path works.
        auto fine = graph;
        auto fineNodes = nodes;
        for (int i = 0; i < fineNodes.size(); ++i) {
            auto node = fineNodes[i].toObject();
            if (node.value("id").toString() != "hybrid-cube") continue;
            auto props = node.value("props").toObject();
            props.remove("sat_fragment"); props.remove("fallback");
            node.insert("props", props); fineNodes[i] = node;
        }
        fine.insert("nodes", fineNodes);
        DeserializedEntityGraph fineDecoded{nullptr, {}};
        QString fineError;
        bool fineOk = deserializeACISEntityGraph(fine, &fineDecoded, &fineError);
        check(fineOk, "G18 supported cube reconstructs without SAT");
        if (fineOk) { api_del_entity_list(*fineDecoded.entities); delete fineDecoded.entities; }


        auto truncated = graph;
        truncated.insert("rels", QJsonArray{});
        DeserializedEntityGraph truncDecoded{nullptr, {}};
        QString truncError;
        bool truncOk = deserializeACISEntityGraph(truncated, &truncDecoded, &truncError);
        check(truncOk, "G18 incomplete topology triggers SAT instead of empty Body");
        if (truncOk) {
            compare_bodies(original, (BODY *)truncDecoded.uuidToEntity.value("hybrid-cube"), "G18 truncated topology", 1e-9);
            api_del_entity_list(*truncDecoded.entities); delete truncDecoded.entities;
        }
        // Unsupported fine geometry must use the component's SAT backup.
        bool sabotaged = false;
        for (int i = 0; i < nodes.size(); ++i) {
            auto node = nodes[i].toObject();
            if (!sabotaged && node.value("labels").toArray().contains("face")) {
                auto props = node.value("props").toObject();
                props.insert("geometry", QJsonObject{{"type", "unknown_surface"}});
                node.insert("props", props); nodes[i] = node; sabotaged = true;
            }
        }
        graph.insert("nodes", nodes);
        DeserializedEntityGraph decoded{nullptr, {}};
        QString error;
        bool ok = deserializeACISEntityGraph(graph, &decoded, &error);
        check(ok && decoded.uuidToEntity.contains("hybrid-cube"), "G18 JSON failure restores same UUID from SAT");
        if (ok) {
            compare_bodies(original, (BODY *)decoded.uuidToEntity.value("hybrid-cube"), "G18 SAT retry", 1e-9);
            api_del_entity_list(*decoded.entities); delete decoded.entities;
        }
        for (int i = 0; i < nodes.size(); ++i) {
            auto node = nodes[i].toObject();
            if (node.value("id").toString() == "hybrid-cube") {
                auto props = node.value("props").toObject();
                props.remove("sat_fragment"); props.remove("fallback");
                node.insert("props", props); nodes[i] = node;
            }
        }
        graph.insert("nodes", nodes);
        ok = deserializeACISEntityGraph(graph, &decoded, &error);
        check(!ok && !decoded.entities && decoded.uuidToEntity.isEmpty(), "G18 no backup rejects incomplete JSON");
        // Known-risk SAT failure must not silently return a partial skeleton.
        for (int i = 0; i < nodes.size(); ++i) {
            auto node = nodes[i].toObject();
            if (node.value("id").toString() == "hybrid-cube") {
                auto props = node.value("props").toObject();
                props.insert("sat_fragment", "not a SAT file"); props.insert("fallback", "sat_fragment");
                node.insert("props", props); nodes[i] = node;
            }
        }
        graph.insert("nodes", nodes);
        ok = deserializeACISEntityGraph(graph, &decoded, &error);
        check(!ok && !decoded.entities, "G18 invalid mandatory SAT is rejected");
        BODY *second = nullptr;
        api_make_cuboid(1, 2, 3, second);
        ENTITY_LIST pair; pair.add(original); pair.add(second);
        auto pairGraph = serializeACISEntityGraph(QHash<int, QString>{{0, "good"}, {1, "bad"}}, pair);
        auto pairNodes = pairGraph.value("nodes").toArray();
        for (int i = 0; i < pairNodes.size(); ++i) {
            auto node = pairNodes[i].toObject();
            if (node.value("id").toString() != "bad") continue;
            auto props = node.value("props").toObject();
            props.insert("sat_fragment", "invalid SAT"); props.insert("fallback", "sat_fragment");
            node.insert("props", props); pairNodes[i] = node;
        }
        pairGraph.insert("nodes", pairNodes);
        ok = deserializeACISEntityGraph(pairGraph, &decoded, &error);
        check(!ok && !decoded.entities && decoded.uuidToEntity.isEmpty(), "G18 two roots fail atomically without partial output");
        api_del_entity(second);

        api_del_entity(original);

        EDGE *edge = nullptr;
        straight line(SPAposition(1, 2, 3), SPAunit_vector(0, 0, 1));
        line.limit(SPAinterval(0, 6));
        check(api_make_edge_from_curve(&line, edge).ok(), "G18 standalone edge");
        graph = serializeACISEntityGraph(QHash<int, QString>{{0, "standalone-edge"}}, ENTITY_LIST(edge));
        ok = deserializeACISEntityGraph(graph, &decoded, &error);
        check(ok && decoded.uuidToEntity.contains("standalone-edge") &&
              is_EDGE((ENTITY *)decoded.uuidToEntity.value("standalone-edge")), "G18 standalone edge SAT preserves type and UUID");
        if (ok) { api_del_entity_list(*decoded.entities); delete decoded.entities; }
        api_del_entity(edge);
        FACE *face = nullptr;
        check(api_make_planar_disk(SPAposition(2, 3, 4), SPAunit_vector(0, 0, 1), 5, face).ok(), "G18 standalone face");
        graph = serializeACISEntityGraph(QHash<int, QString>{{0, "standalone-face"}}, ENTITY_LIST(face));
        ok = deserializeACISEntityGraph(graph, &decoded, &error);
        check(ok && decoded.uuidToEntity.contains("standalone-face") &&
              is_FACE((ENTITY *)decoded.uuidToEntity.value("standalone-face")), "G18 standalone face SAT preserves type and UUID");
        if (ok) { api_del_entity_list(*decoded.entities); delete decoded.entities; }
        api_del_entity(face);

    }

finish:
    api_stop_modeller();
    terminate_base();

    printf("\n%s (failures=%d)\n",
           g_failed == 0 ? "ALL GRAPH ROUNDTRIP TESTS PASSED" : "GRAPH ROUNDTRIP TESTS HAD FAILURES", g_failed);
    return g_failed == 0 ? 0 : 1;
}
