// 复杂工图存储保真度探针（一次性诊断，不进正式回归套件）。
// 回答两个问题：
//   Q1 细粒度（Mode1 JSON 实体图）：一个"随机复杂工图"里的全部实体类型，
//      哪些能完整往返（体积/边长/拓扑/包围盒四重比对），哪些被静默丢弃。
//   Q2 粗粒度（SAT 全量文本，A 栈 / 官方文件路径）：同一模型 SAT 存→取→存
//      是否字节级一致（内核自身格式保真度基线）。
// 场景 = 3 个部件：A 机加工座（布尔挖孔 + 圆角 + 抽壳）、B 弹簧（圆截面沿
// 螺旋路径扫掠 → 样条扫掠面 + intcurve + pcurve）、C 扭转放样（6 样条面）。
#include <kernapi.hxx>
#include <license.hxx>
#include <spa_unlock_result.hxx>
#include <cstrapi.hxx>
#include <boolapi.hxx>
#include <intrapi.hxx>
#include <fileinfo.hxx>
#include <alltop.hxx>
#include <allcurve.hxx>
#include <allsurf.hxx>
#include <sweepapi.hxx>
#include <swp_opts.hxx>
#include <deprecated_masspr.hxx>
#include <tensor.hxx>
#include <heldef.hxx>
#include <blendapi.hxx>
#include <transfrm.hxx>
#include <skinapi.hxx>
#include <shl_api.hxx>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include "entity_graph_serializer.h"

#include "spatial_license.h"

static int g_failed = 0;
static void check(bool ok, const char *what) {
    printf("  %-70s %s\n", what, ok ? "PASS" : "FAIL");
    if (!ok) g_failed++;
}

static bool body_volume(BODY *b, double &vol) {
    SPAposition root(0, 0, 0), cofg(0, 0, 0);
    SPAunit_vector normal(0, 0, 1);
    tensor inertia;
    double pm[3];
    SPAunit_vector pa[3];
    double achieved = 0;
    return api_body_mass_pr(b, root, normal, 2, 1e-6, vol, cofg, inertia, pm, pa, achieved).ok();
}

// mass_pr 失败时取错误码（诊断恢复体为何积不出体积）
static int body_volume_err(BODY *b, double &vol) {
    SPAposition root(0, 0, 0), cofg(0, 0, 0);
    SPAunit_vector normal(0, 0, 1);
    tensor inertia;
    double pm[3];
    SPAunit_vector pa[3];
    double achieved = 0;
    outcome r = api_body_mass_pr(b, root, normal, 2, 1e-6, vol, cofg, inertia, pm, pa, achieved);
    return r.ok() ? 0 : r.error_number();
}

// 该内核布尔结果体 = 原点居中几何 + TRANSFORM；顶点原始坐标需加偏移才与世界系一致。
// 偏移 = 世界包围盒中心 - 原始顶点包围盒中心。
static SPAvector body_coord_offset(BODY *b); // 在 body_box 之后定义

static bool body_box(BODY *b, double mn[3], double mx[3]) {
    SPAposition lo, hi;
    if (!api_get_entity_box(b, lo, hi).ok())
        return false;
    mn[0] = lo.x(); mn[1] = lo.y(); mn[2] = lo.z();
    mx[0] = hi.x(); mx[1] = hi.y(); mx[2] = hi.z();
    return true;
}

static SPAvector body_coord_offset(BODY *b) {
    double wb[3], WB[3];
    body_box(b, wb, WB);
    double rmin[3] = {1e30, 1e30, 1e30}, rmax[3] = {-1e30, -1e30, -1e30};
    ENTITY_LIST vl;
    api_get_vertices(b, vl);
    for (ENTITY *v : vl) {
        if (!is_VERTEX(v) || !((VERTEX *)v)->geometry()) continue;
        SPAposition p = ((VERTEX *)v)->geometry()->coords();
        double c[3] = {p.x(), p.y(), p.z()};
        for (int i = 0; i < 3; ++i) {
            rmin[i] = (c[i] < rmin[i]) ? c[i] : rmin[i];
            rmax[i] = (c[i] > rmax[i]) ? c[i] : rmax[i];
        }
    }
    return SPAvector((wb[0] + WB[0]) / 2 - (rmin[0] + rmax[0]) / 2,
                     (wb[1] + WB[1]) / 2 - (rmin[1] + rmax[1]) / 2,
                     (wb[2] + WB[2]) / 2 - (rmin[2] + rmax[2]) / 2);
}

static double total_edge_len(BODY *b) {
    double sum = 0;
    ENTITY_LIST el;
    api_get_edges(b, el);
    for (ENTITY *e : el)
        if (is_EDGE(e))
            sum += ((EDGE *)e)->length();
    return sum;
}

// 排序后的逐边长度多重集（定位往返中哪条边参数化偏了）
static std::vector<double> sorted_edge_lens(BODY *b) {
    std::vector<double> v;
    ENTITY_LIST el;
    api_get_edges(b, el);
    for (ENTITY *e : el)
        if (is_EDGE(e))
            v.push_back(((EDGE *)e)->length());
    std::sort(v.begin(), v.end());
    return v;
}

// ---- 普查：api_get_entities 全量 + JSON 节点 label ----

static void census_model(BODY *b, const char *tag, std::map<std::string, int> &all) {
    ENTITY_LIST el;
    api_get_entities(b, el, -1, -1);
    for (ENTITY *e : el)
        all[e->type_name()]++;
    printf("  [%s] kernel entities = %d\n", tag, el.count());
}

static std::map<std::string, int> census_json(const QJsonObject &g) {
    std::map<std::string, int> m;
    for (const QJsonValue &v : g.value("nodes").toArray()) {
        QJsonArray labels = v.toObject().value("labels").toArray();
        if (!labels.isEmpty())
            m[labels[0].toString().toStdString()]++;
    }
    return m;
}

// 统计 face/edge 节点里 geometry.type 为 unknown_*（静默丢失）与缺 geometry 的情况
static void scan_unknown_geometry(const QJsonObject &g, int &unkSurf, int &unkCurve, int &noGeom,
                                  std::map<std::string, int> &geomTypes) {
    for (const QJsonValue &v : g.value("nodes").toArray()) {
        QJsonObject n = v.toObject();
        QString label = n.value("labels").toArray()[0].toString();
        if (label != "face" && label != "edge" && label != "coedge")
            continue;
        QJsonObject props = n.value("props").toObject();
        if (!props.contains("geometry")) {
            noGeom++; // coedge 无 pcurve 属正常；face/edge 无 geometry 会由 unkSurf/unkCurve 之外的计数暴露
            continue;
        }
        QString t = props.value("geometry").toObject().value("type").toString();
        geomTypes[t.toStdString()]++;
        if (t == "unknown_surface")
            unkSurf++;
        if (t == "unknown_curve")
            unkCurve++;
    }
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    if (!initialize_base()) { printf("FAIL base\n"); return 1; }
    outcome r = api_start_modeller(0);
    if (!r.ok()) { printf("FAIL modeller\n"); return 1; }
    spa_unlock_products(SPATIAL_LICENSE);
    FileInfo fi;
    fi.set_units(1.0);
    fi.set_product_id("probe_complex");
    api_set_file_info(3, fi);
    api_save_version(0, 0);

    // ===================== 部件 A：机加工座 =====================
    // 20x14x8 方块 → 挖竖孔（布尔减）→ 4 条竖棱圆角 → 顶面开口抽壳
    BODY *partA = nullptr;
    check(api_solid_block(SPAposition(0, 0, 0), SPAposition(20, 14, 8), partA).ok(), "A api_solid_block");
    {
        double b0[3], B0[3];
        body_box(partA, b0, B0);
        printf("  [dbg] box after block: (%.1f,%.1f,%.1f)-(%.1f,%.1f,%.1f)\n", b0[0], b0[1], b0[2], B0[0], B0[1], B0[2]);
        BODY *cyl = nullptr;
        api_solid_cylinder_cone(SPAposition(10, 7, -2), SPAposition(10, 7, 10), 3.0, 3.0, 3.0, nullptr, cyl);
        check(api_subtract(cyl, partA).ok(), "A api_subtract (hole)");
        body_box(partA, b0, B0);
        printf("  [dbg] box after subtract: (%.1f,%.1f,%.1f)-(%.1f,%.1f,%.1f)\n", b0[0], b0[1], b0[2], B0[0], B0[1], B0[2]);
        // 找 4 条竖棱：两端（世界系）x/y 均为包围盒角。原始顶点坐标 + body_coord_offset = 世界系。
        SPAvector off = body_coord_offset(partA);
        ENTITY_LIST el;
        api_get_edges(partA, el);
        std::vector<ENTITY *> vert;
        for (ENTITY *e : el) {
            if (!is_EDGE(e) || !((EDGE *)e)->start() || !((EDGE *)e)->end() || !((EDGE *)e)->start()->geometry() ||
                !((EDGE *)e)->end()->geometry())
                continue;
            EDGE *ed = (EDGE *)e;
            SPAposition p1 = ed->start()->geometry()->coords(), p2 = ed->end()->geometry()->coords();
            double w1[3] = {p1.x() + off.x(), p1.y() + off.y(), p1.z() + off.z()};
            double w2[3] = {p2.x() + off.x(), p2.y() + off.y(), p2.z() + off.z()};
            auto atCorner = [&](double x, double y) {
                return (fabs(x) < 1e-6 || fabs(x - 20) < 1e-6) && (fabs(y) < 1e-6 || fabs(y - 14) < 1e-6);
            };
            if (atCorner(w1[0], w1[1]) && atCorner(w2[0], w2[1]))
                vert.push_back(e);
        }
        printf("  [A] vertical edges for blend = %d\n", (int)vert.size());
        ENTITY_LIST blendList;
        for (ENTITY *e : vert) blendList.add(e);
        check(api_blend_edges(blendList, 1.5).ok(), "A api_blend_edges (r=1.5)");
        // 抽壳：顶面开口，壁厚 0.8。顶面 = 平面且（世界系）root.z≈盒顶 且法向 ±z
        ENTITY_LIST fl;
        api_get_faces(partA, fl);
        FACE *topFace = nullptr;
        double wb[3], WB[3];
        body_box(partA, wb, WB);
        for (ENTITY *f : fl) {
            if (!is_FACE(f)) continue;
            SURFACE *s = ((FACE *)f)->geometry();
            if (!s || s->identity(2) != PLANE_ID) continue;
            const plane &pl = static_cast<const plane &>(((PLANE *)s)->equation());
            if (fabs(pl.root_point.z() + off.z() - WB[2]) < 1e-6 && fabs(pl.normal.z()) > 0.9) { topFace = (FACE *)f; break; }
        }
        // 诊断：抽壳连接边（长≈壁厚 0.8）的参数状态
        {
            ENTITY_LIST el2;
            api_get_edges(partA, el2);
            for (ENTITY *e : el2) {
                if (!is_EDGE(e)) continue;
                EDGE *ed = (EDGE *)e;
                if (ed->length() < 0.7 || ed->length() > 0.9) continue;
                CURVE *g = ed->geometry();
                SPAinterval eb = ed->get_param_range();
                printf("    [dbg] connector edge len=%.3f geom=%s bound=[%g,%g]", ed->length(),
                       g ? g->type_name() : "(null)", eb.start_pt(), eb.end_pt());
                if (g) {
                    const curve &eq = g->equation();
                    SPAinterval cr = eq.param_range();
                    printf(" curve-range=[%g,%g]", cr.start_pt(), cr.end_pt());
                }
                printf("\n");
            }
        }
        printf("  [A] hollow open face found = %d\n", topFace ? 1 : 0);
        outcome hr = api_hollow_body(partA, topFace ? 1 : 0, &topFace, 0.8);
        printf("  [A] api_hollow_body(thickness=0.8) %s\n", hr.ok() ? "OK" : "FAILED (continuing without shell)");
    }

    // ===================== 部件 B：弹簧（样条扫掠） =====================
    // 半径 0.4 圆截面沿半径 3 / 螺距 1.5 / 3 圈螺旋路径扫掠
    BODY *partB = nullptr;
    {
        helix *hg = new helix(SPAposition(40, 0, 0), SPAunit_vector(0, 0, 1), SPAvector(3, 0, 0), 1.5, TRUE,
                              SPAinterval(0.0, 6 * 3.14159265358979));
        EDGE *pathEd = nullptr;
        outcome pr = api_make_edge_from_curve(hg, pathEd);
        delete hg;
        check(pr.ok(), "B helix path edge");
        ellipse *circ = new ellipse(SPAposition(40, 0, 0), SPAunit_vector(1, 0, 0), SPAvector(0, 0.4, 0), 1.0);
        EDGE *profEd = nullptr;
        outcome cr = api_make_edge_from_curve(circ, profEd);
        delete circ;
        check(cr.ok(), "B circle profile edge");
        sweep_options so;
        outcome sr = api_sweep_with_options(profEd, pathEd, &so, partB);
        printf("  [B] api_sweep(circle→helix) %s\n", sr.ok() ? "OK" : "FAILED (continuing without spring)");
        if (!sr.ok())
            check(false, "B sweep succeeded");
        // 注意：不删除 pathEd/profEd——扫掠体可能与输入边共享子实体，级联删除会摧毁新体
        // B 诊断：扫掠面 spl_sur 子类普查（equation().type() 数值，Q3 内部子类证据）
        if (sr.ok()) {
            ENTITY_LIST fl;
            api_get_faces(partB, fl);
            std::map<int, int> sub;
            int nonSpline = 0;
            for (ENTITY *f : fl) {
                if (!is_FACE(f)) continue;
                SURFACE *s = ((FACE *)f)->geometry();
                if (!s) continue;
                if (s->identity(2) == SPLINE_ID) {
                    const spline &gem = static_cast<const spline &>(((SPLINE *)s)->equation());
                    sub[(int)gem.get_spl_sur().type()]++;
                } else
                    nonSpline++;
            }
            for (auto &kv : sub)
                printf("  [B] spline face subtype(spl_sur::type())=%d x%d\n", kv.first, kv.second);
            if (nonSpline) printf("  [B] non-spline faces = %d\n", nonSpline);
        }
    }

    // ===================== 部件 C：扭转六边形放样（G13 同源） =====================
    BODY *partC = nullptr;
    {
        SPAposition hex1[7], hex2[7];
        for (int k = 0; k < 6; ++k) {
            double a1 = k * 3.14159265358979 / 3.0, a2 = a1 + 3.14159265358979 / 6.0;
            hex1[k] = SPAposition(60 + 4 * cos(a1), 4 * sin(a1), 0);
            hex2[k] = SPAposition(60 + 4 * cos(a2) * 1.08, 4 * sin(a2) * 1.08, 10);
        }
        hex1[6] = hex1[0];
        hex2[6] = hex2[0];
        BODY *w1 = nullptr, *w2 = nullptr;
        api_make_wire((BODY *)nullptr, 7, hex1, w1);
        api_make_wire((BODY *)nullptr, 7, hex2, w2);
        BODY *wires[2] = {w1, w2};
        outcome sk = api_skin_wires(2, wires, partC);
        check(sk.ok(), "C api_skin_wires");
        api_del_entity(w1);
        api_del_entity(w2);
    }

    // ===================== 普查：内核实体 vs JSON 节点 =====================
    std::map<std::string, int> kernel;
    BODY *parts[3] = {partA, partB, partC};
    const char *tags[3] = {"A", "B", "C"};
    for (int i = 0; i < 3; ++i)
        if (parts[i]) census_model(parts[i], tags[i], kernel);
    // 样条面子类普查（spl_sur::type()，决定回退白名单）
    {
        std::map<int, int> subAll;
        for (int i = 0; i < 3; ++i) {
            if (!parts[i]) continue;
            ENTITY_LIST fl;
            api_get_faces(parts[i], fl);
            for (ENTITY *f : fl) {
                if (!is_FACE(f)) continue;
                SURFACE *s = ((FACE *)f)->geometry();
                if (s && s->identity(2) == SPLINE_ID)
                    subAll[(int)static_cast<const spline &>(((SPLINE *)s)->equation()).get_spl_sur().type()]++;
            }
        }
        for (auto &kv : subAll)
            printf("  [ALL] spline face subtype(type()=%d) x%d\n", kv.first, kv.second);
    }
    printf("  == kernel entity census (type_name -> count) ==\n");
    for (auto &kv : kernel)
        printf("     %-24s %d\n", kv.first.c_str(), kv.second);

    ENTITY_LIST top;
    for (int i = 0; i < 3; ++i)
        if (parts[i]) top.add(parts[i]);
    // 诊断：A 与 C 的 straight 边方程原始数据（方向模长 = param_scale）
    for (int pi : {0, 2}) {
        if (!parts[pi]) continue;
        ENTITY_LIST elA;
        api_get_edges(parts[pi], elA);
        int shown = 0;
        for (ENTITY *e : elA) {
            if (!is_EDGE(e) || shown >= 3) continue;
            EDGE *ed = (EDGE *)e;
            CURVE *g = ed->geometry();
            if (!g || g->identity(2) != STRAIGHT_ID) continue;
            const straight &se = static_cast<const straight &>(((STRAIGHT *)g)->equation());
            SPAinterval cr = se.param_range(), eb = ed->get_param_range();
            printf("    [dbg] %s straight: len=%.4f dir-mag=%.6f crange=[%g,%g](%s) ebound=[%g,%g]\n", tags[pi],
                   ed->length(), se.direction.len(), cr.start_pt(), cr.end_pt(),
                   cr.type() == interval_type::interval_finite ? "finite" : "inf", eb.start_pt(), eb.end_pt());
            shown++;
        }
    }
    // 诊断：A 的短边（<1.0）在序列化时刻的真实参数状态
    if (partA) {
        ENTITY_LIST elA;
        api_get_edges(partA, elA);
        for (ENTITY *e : elA) {
            if (!is_EDGE(e)) continue;
            EDGE *ed = (EDGE *)e;
            double L = ed->length();
            if (L > 1.0) continue;
            CURVE *g = ed->geometry();
            SPAinterval eb = ed->get_param_range();
            printf("    [dbg] short edge len=%.6f geom=%s bound=[%g,%g]", L, g ? g->type_name() : "(null)",
                   eb.start_pt(), eb.end_pt());
                if (g) {
                    const straight *st = nullptr;
                    if (g->identity(2) == STRAIGHT_ID)
                        st = (const straight *)&static_cast<const straight &>(((STRAIGHT *)g)->equation());
                    if (st) {
                        SPAposition s0 = ed->start() && ed->start()->geometry() ? ed->start()->geometry()->coords()
                                                                               : SPAposition(9e9, 9e9, 9e9);
                        SPAposition e0 = ed->end() && ed->end()->geometry() ? ed->end()->geometry()->coords()
                                                                           : SPAposition(9e9, 9e9, 9e9);
                        printf(" dir=(%g,%g,%g) root=(%g,%g,%g) start=(%.3f,%.3f,%.3f) end=(%.3f,%.3f,%.3f)",
                               st->direction.x(), st->direction.y(), st->direction.z(), st->root_point.x(),
                               st->root_point.y(), st->root_point.z(), s0.x(), s0.y(), s0.z(), e0.x(), e0.y(),
                               e0.z());
                    }
                }
            printf("\n");
        }
    }
    QHash<int, QString> uuids;
    for (int i = 0; i < 3; ++i) uuids.insert(i, QString("probe-uuid-%1").arg(i));
    QJsonObject json = serializeACISEntityGraph(uuids, top);
    // E2E 夹具导出：真实 C++ 序列化输出（含 sat_fragment 回退字段），供后端
    // 集成测试直连 Neo4j 往返（test_collab_e2e_cpp_graph.py）
    if (getenv("PROBE_DUMP_JSON")) {
        QFile jf("probe_graph.json");
        if (jf.open(QIODevice::WriteOnly)) {
            jf.write(QJsonDocument(json).toJson(QJsonDocument::Compact));
            jf.close();
            printf("  [dump] probe_graph.json written (%lld bytes)\n", (long long)jf.size());
        }
    }
    // 诊断：JSON 里 A 连接边（straight）的 range 字段
    for (const QJsonValue &v : json.value("nodes").toArray()) {
        QJsonObject n = v.toObject();
        if (n.value("labels").toArray()[0].toString() != "edge")
            continue;
        QJsonObject g = n.value("props").toObject().value("geometry").toObject();
        if (g.value("type").toString() != "straight")
            continue;
        QJsonArray r = g.value("range").toArray();
        QJsonArray d = g.value("direction").toArray();
        if (d.size() == 3 && d[2].toDouble() < -0.5) // 竖直向下 = 抽壳连接边
            printf("    [dbg] json connector range=[%s] dir=[%s]\n",
                   QString::fromUtf8(QJsonDocument(r).toJson(QJsonDocument::Compact)).toUtf8().constData(),
                   QString::fromUtf8(QJsonDocument(d).toJson(QJsonDocument::Compact)).toUtf8().constData());
    }
    std::map<std::string, int> jc = census_json(json);
    printf("  == JSON node label census ==\n");
    for (auto &kv : jc)
        printf("     %-24s %d\n", kv.first.c_str(), kv.second);

    int unkSurf = 0, unkCurve = 0, noGeom = 0;
    std::map<std::string, int> geomTypes;
    scan_unknown_geometry(json, unkSurf, unkCurve, noGeom, geomTypes);
    printf("  == JSON geometry payload census ==\n");
    for (auto &kv : geomTypes)
        printf("     %-24s %d\n", kv.first.c_str(), kv.second);
    check(unkSurf == 0, "Q1 no unknown_surface in JSON (silent surface loss)");
    check(unkCurve == 0, "Q1 no unknown_curve in JSON (silent curve loss)");

    // ===================== Q1：JSON 细粒度往返保真 =====================
    // 回退机制生效后：风险组件（A 属性 / B sweepsur）带 sat_fragment 走内核
    // 读回，干净组件（C rulesur）保持纯实体级图重建。
    int fragBodies = 0;
    for (const QJsonValue &v : json.value("nodes").toArray()) {
        QJsonObject n = v.toObject();
        if (n.value("labels").toArray()[0].toString() == "body" &&
            n.value("props").toObject().contains("sat_fragment"))
            fragBodies++;
    }
    printf("  [Q1] bodies with sat_fragment = %d\n", fragBodies);
    check(fragBodies == 1, "Q1 exactly B (sweepsur) got fallback; A/C stay fine-grained");
    DeserializedEntityGraph d;
    QString err;
    bool okD = deserializeACISEntityGraph(json, &d, &err);
    check(okD, "Q1 deserializeACISEntityGraph ok");
    if (okD && d.entities) {
        std::vector<BODY *> rest(3, nullptr);
        for (int i = 0; i < 3; ++i) {
            auto it = d.uuidToEntity.find(QString("probe-uuid-%1").arg(i));
            if (it != d.uuidToEntity.end())
                rest[i] = (BODY *)it.value();
        }
        printf("  [Q1] restored bodies by uuid = %d/3\n", (int)std::count_if(rest.begin(), rest.end(),
                                                                             [](BODY *b) { return b != nullptr; }));
        for (int i = 0; i < 3 && i < (int)rest.size(); ++i) {
            BODY *o = parts[i], *r = rest[i];
            if (!o) continue;
            char buf[160];
            double vo = 0, vr = 0;
            bool okO = body_volume(o, vo), okR = body_volume(r, vr);
            if (okO && okR) {
                double rel = fabs(vo - vr) / (fabs(vo) > 1 ? fabs(vo) : 1);
                snprintf(buf, sizeof(buf), "Q1 %s: volume %.6f vs %.6f (rel %.1e)", tags[i], vr, vo, rel);
                check(rel < 1e-6, buf);
            } else {
                snprintf(buf, sizeof(buf), "Q1 %s: mass_pr orig=%d rest=%d", tags[i], (int)okO, (int)okR);
                check(false, buf);
            }
            double lo = total_edge_len(o), lr = total_edge_len(r);
            double rel = fabs(lo - lr) / (fabs(lo) > 1 ? fabs(lo) : 1);
            snprintf(buf, sizeof(buf), "Q1 %s: edge length %.6f vs %.6f (rel %.1e)", tags[i], lr, lo, rel);
            check(rel < 1e-6, buf);
            if (!okR) {
                double vd;
                int en = body_volume_err(r, vd);
                printf("  [Q1-%s] restored mass_pr error #%d: %s\n", tags[i], en, find_err_mess(en));
            }
            // 逐边长度多重集：第一条不匹配的边就是参数化缺陷所在
            if (o && r) {
                std::vector<double> lvo = sorted_edge_lens(o), lvr = sorted_edge_lens(r);
                for (size_t k = 0; k < lvo.size() && k < lvr.size(); ++k)
                    if (fabs(lvo[k] - lvr[k]) > 1e-9) {
                        printf("  [Q1-%s] first edge-length mismatch #%zu: %.9f vs %.9f (diff %.6f)\n", tags[i], k,
                               lvr[k], lvo[k], lvr[k] - lvo[k]);
                        break;
                    }
                if (i == 0) {
                    // 恢复侧零长度 straight 边：顶点是否恢复正确（长度按顶点推导）
                    ENTITY_LIST elr;
                    api_get_edges(r, elr);
                    for (ENTITY *e : elr) {
                        if (!is_EDGE(e)) continue;
                        EDGE *ed = (EDGE *)e;
                        if (ed->length() > 1e-9) continue;
                        CURVE *g = ed->geometry();
                        SPAposition s = ed->start() && ed->start()->geometry() ? ed->start()->geometry()->coords()
                                                                              : SPAposition(9e9, 9e9, 9e9);
                        SPAposition en = ed->end() && ed->end()->geometry() ? ed->end()->geometry()->coords()
                                                                          : SPAposition(9e9, 9e9, 9e9);
                        printf("    [dbg] restored zero straight: start=(%.3f,%.3f,%.3f) end=(%.3f,%.3f,%.3f) "
                               "geom=%s\n",
                               s.x(), s.y(), s.z(), en.x(), en.y(), en.z(), g ? g->type_name() : "(null)");
                    }
                }
            }
            double mo[3], Mo[3], mr[3], Mr[3];
            body_box(o, mo, Mo); body_box(r, mr, Mr);
            double dmax = 0;
            for (int k = 0; k < 3; ++k)
                dmax = fmax(dmax, fmax(fabs(mo[k] - mr[k]), fabs(Mo[k] - Mr[k])));
            snprintf(buf, sizeof(buf), "Q1 %s: bbox delta %.3e", tags[i], dmax);
            check(dmax < 1e-6, buf);
        }
        // JSON 恢复体落盘，供与原件 SAT 做行级 diff（观察属性/子类差异）
        FILE *fj = fopen("probe_json_rest.sat", "wb");
        if (fj) { api_save_entity_list(fj, TRUE, *(d.entities)); fclose(fj); }
    }

    // ===================== Q2：SAT 粗粒度（内核官方格式）往返 =====================
    {
        FILE *f1 = fopen("probe_orig.sat", "wb");
        check(f1 != nullptr, "Q2 save probe_orig.sat");
        api_save_entity_list(f1, TRUE, top);
        fclose(f1);
        FILE *f2 = fopen("probe_orig.sat", "rb");
        ENTITY_LIST rlist;
        logical ok = TRUE;
        outcome rr = api_restore_entity_list(f2, TRUE, rlist);
        fclose(f2);
        check(rr.ok() && rlist.count() == 3, "Q2 restore 3 bodies from SAT");
        FILE *f3 = fopen("probe_sat_r2.sat", "wb");
        api_save_entity_list(f3, TRUE, rlist);
        fclose(f3);
        // SAT 往返后体积一致
        int bi = 0;
        for (ENTITY *e = rlist.first(); e; e = rlist.next(), ++bi)
            if (is_BODY(e) && bi < 3 && parts[bi]) {
                double vo = 0, vr = 0;
                bool okO = body_volume(parts[bi], vo), okR = body_volume((BODY *)e, vr);
                char buf[160];
                snprintf(buf, sizeof(buf), "Q2 %s: SAT roundtrip volume %.6f vs %.6f", tags[bi], vr, vo);
                check(okO && okR && fabs(vo - vr) / (fabs(vo) > 1 ? fabs(vo) : 1) < 1e-9, buf);
            }
        for (ENTITY *e : rlist) if (is_BODY(e)) api_del_entity(e);
    }

    printf(g_failed == 0 ? "PROBE ALL CHECKS PASSED (failures=0)\n"
                         : "PROBE FAILURES = %d\n", g_failed);
    api_stop_modeller();
    return g_failed;
}
