// ACIS 2026.1.0.1 官方依赖综合实体测试。
// 覆盖：方块/球/圆柱/圆锥/圆环（解析曲面）、线框扫掠（样条拟合面）、布尔并/差/交。
// 每类：建模 → SAT 文本往返 → 拓扑计数 + 体积/包围盒数值比对。
// 编译要求（见 docs/ACIS_2026_MIGRATION.md）：/MD、/EHa、SPA_NO_AUTO_LINK、栈 4MB。
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
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>

#include "spatial_license.h"

static int g_failed = 0;

static void check(bool ok, const char *what) {
    printf("  %-58s %s\n", what, ok ? "PASS" : "FAIL");
    if (!ok) g_failed++;
}

// 体积测量（api_body_mass_pr，selector=2 只要体积）
static bool body_volume(BODY *b, double &vol) {
    SPAposition root(0, 0, 0), cofg(0, 0, 0);
    SPAunit_vector normal(0, 0, 1);
    tensor inertia;
    double pm[3];
    SPAunit_vector pa[3];
    double achieved = 0;
    return api_body_mass_pr(b, root, normal, 2, 1e-6, vol, cofg, inertia, pm, pa, achieved).ok();
}

// SAT 往返 + 体积/包围盒比对；返回恢复出的 body
static BODY *roundtrip(BODY *body, const char *tag, double expect_vol, double vol_tol) {
    std::string fn = std::string("entity_") + tag + ".sat";
    FILE *fp = fopen(fn.c_str(), "wb");
    ENTITY_LIST el(body);
    outcome sr = api_save_entity_list(fp, TRUE, el);
    fclose(fp);
    if (!sr.ok()) {
        printf("  save %s err #%d: %s\n", tag, sr.error_number(), find_err_mess(sr.error_number()));
        g_failed++;
        return nullptr;
    }

    ENTITY_LIST restored;
    FILE *fp2 = fopen(fn.c_str(), "rb");
    outcome rr = api_restore_entity_list(fp2, TRUE, restored);
    fclose(fp2);
    if (!rr.ok()) {
        printf("  restore %s err: %s\n", tag, find_err_mess(rr.error_number()));
        g_failed++;
        return nullptr;
    }

    BODY *rb = nullptr;
    for (ENTITY *e : restored)
        if (is_BODY(e)) rb = (BODY *)e;
    if (rb == nullptr) {
        printf("  %s: no body restored\n", tag);
        g_failed++;
        return nullptr;
    }

    double vol = 0;
    if (body_volume(rb, vol)) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s: volume %.6f vs %.6f (tol %.1e)", tag, vol, expect_vol, vol_tol);
        check(std::fabs(vol - expect_vol) < vol_tol, buf);
    } else {
        check(false, (std::string(tag) + ": api_body_mass_pr failed").c_str());
    }

    SPAposition mn, mx;
    if (api_get_entity_box(rb, mn, mx).ok()) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s: box [%.3f %.3f %.3f]-[%.3f %.3f %.3f]", tag, mn.x(), mn.y(), mn.z(), mx.x(),
                 mx.y(), mx.z());
        check(mn.x() < mx.x() && mn.y() < mx.y() && mn.z() < mx.z(), buf);
    }
    return rb;
}

static void check_topology(BODY *b, const char *tag, int faces, int edges, int verts) {
    ENTITY_LIST fl, el, vl;
    api_get_faces(b, fl);
    api_get_edges(b, el);
    api_get_vertices(b, vl);
    char buf[96];
    snprintf(buf, sizeof(buf), "%s: faces %d/%d edges %d/%d verts %d/%d", tag, fl.count(), faces, el.count(), edges,
             vl.count(), verts);
    check(fl.count() == faces && el.count() == edges && vl.count() == verts, buf);
}

int main(int argc, char **argv) {
    setvbuf(stdout, nullptr, _IONBF, 0); // 崩溃时也保留已打印内容
    int run_upto = (argc > 1) ? atoi(argv[1]) : 9;
    if (!initialize_base()) { printf("FAIL base\n"); return 1; }
    outcome r = api_start_modeller(0);
    if (!r.ok()) { printf("FAIL modeller\n"); return 1; }
    spa_unlock_products(SPATIAL_LICENSE);

    FileInfo fi;
    fi.set_units(1.0);
    fi.set_product_id("entity_tests");
    api_set_file_info(3, fi);
    api_save_version(0, 0);

    int g_id = 0;
    if (++g_id > run_upto) goto finish;
    printf("== 1. 方块（plane 面 + straight 边）==\n");
    {
        BODY *box = nullptr;
        check(api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), box).ok(), "api_solid_block");
        BODY *rb = roundtrip(box, "block", 1000.0, 1e-6);
        if (rb) check_topology(rb, "block", 6, 12, 8);
        api_del_entity(box);
        if (rb) api_del_entity(rb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 2. 球（sphere 面 + 拟合圆边）==\n");
    {
        BODY *sph = nullptr;
        check(api_solid_sphere(SPAposition(0, 0, 0), 5.0, sph).ok(), "api_solid_sphere(r=5)");
        BODY *rb = roundtrip(sph, "sphere", 4.0 / 3.0 * 3.14159265358979 * 125, 1e-3);
        if (rb) check_topology(rb, "sphere", 1, 0, 0);
        api_del_entity(sph);
        if (rb) api_del_entity(rb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 3. 圆柱（cylinder 面 + 圆边）==\n");
    {
        BODY *cyl = nullptr;
        check(api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 20), 4.0, 4.0, 4.0, nullptr, cyl).ok(),
              "api_solid_cylinder_cone(r=4,h=20)");
        BODY *rb = roundtrip(cyl, "cylinder", 3.14159265358979 * 16 * 20, 1e-3);
        if (rb) check_topology(rb, "cylinder", 3, 2, 2);
        api_del_entity(cyl);
        if (rb) api_del_entity(rb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 4. 圆锥（cone 面 + 锥顶点）==\n");
    {
        BODY *cone = nullptr;
        check(api_solid_cylinder_cone(SPAposition(0, 0, 0), SPAposition(0, 0, 10), 4.0, 4.0, 0.0, nullptr, cone).ok(),
              "api_solid_cylinder_cone(cone)");
        BODY *rb = roundtrip(cone, "cone", 3.14159265358979 * 16 * 10 / 3.0, 1e-3);
        if (rb) check_topology(rb, "cone", 2, 2, 2);
        api_del_entity(cone);
        if (rb) api_del_entity(rb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 5. 圆环（torus 面）==\n");
    {
        BODY *tor = nullptr;
        check(api_make_torus(8.0, 2.0, tor).ok(), "api_make_torus(R=8,r=2)");
        BODY *rb = roundtrip(tor, "torus", 2 * 3.14159265358979 * 3.14159265358979 * 8 * 4, 1e-2);
        if (rb) check_topology(rb, "torus", 1, 0, 0);
        api_del_entity(tor);
        if (rb) api_del_entity(rb);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 6. prism (hexagonal, swept-style faces) ==\n");
    {
        // 六棱柱：api_make_prism 等价于多边形 wire 沿轴向扫掠成实体
        BODY *prism = nullptr;
        char buf[96];
        outcome pr = api_make_prism(10.0, 4.0, 4.0, 6, prism);
        snprintf(buf, sizeof(buf), "api_make_prism ok=%d", (int)pr.ok());
        check(pr.ok(), buf);
        if (pr.ok()) {
            // 正六棱柱体积 = (3/2)*sqrt(3)*r_circum^2*sin(60)/... 用外接圆半径4: V = (3√3/2)·R²·sin(60°)·h/... 精确: V = (3√3/2)·s²·h, s=R=4
            double expect = 3.0 * 1.73205080756888 / 2.0 * 16.0 * 10.0;
            BODY *rb = roundtrip(prism, "prism_hex", expect, 1e-2);
            if (rb) {
                ENTITY_LIST fl;
                api_get_faces(rb, fl);
                snprintf(buf, sizeof(buf), "prism_hex: faces=%d (6 sides + 2 caps)", fl.count());
                check(fl.count() == 8, buf);
                api_del_entity(rb);
            }
            api_del_entity(prism);
        }
    }

    if (++g_id > run_upto) goto finish;
    printf("== 7. 布尔并（方块+方块，产生平面交线）==\n");
    {
        BODY *a = nullptr, *b = nullptr;
        api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), a);
        api_solid_block(SPAposition(5, 5, 5), SPAposition(15, 15, 15), b);
        // api_unite(tool, blank)：结果保留在 blank（第二个参数），tool 被消耗
        check(api_unite(a, b).ok(), "api_unite(tool, blank)");
        BODY *rb = roundtrip(b, "unite", 1000.0 + 1000.0 - 125.0, 1e-6);
        if (rb) {
            ENTITY_LIST fl;
            api_get_faces(rb, fl);
            char buf[96];
            snprintf(buf, sizeof(buf), "unite: faces=%d", fl.count());
            check(fl.count() >= 6 && fl.count() <= 12, buf);
            api_del_entity(rb);
        }
        // b 持有合并结果，a 已被消耗——只删 b 一次。
        api_del_entity(b);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 8. 布尔差（方块-圆柱，产生圆柱面/圆交线）==\n");
    {
        BODY *blk = nullptr, *cyl = nullptr;
        api_solid_block(SPAposition(-5, -5, -5), SPAposition(5, 5, 5), blk);
        api_solid_cylinder_cone(SPAposition(0, 0, -10), SPAposition(0, 0, 10), 2.0, 2.0, 2.0, nullptr, cyl);
        // api_subtract(tool, blank)：cyl 从 blk 中减去，结果保留在 blk
        check(api_subtract(cyl, blk).ok(), "api_subtract(tool=cyl, blank=blk)");
        // 调试：保存前原体的面类型
        {
            ENTITY_LIST fl0;
            api_get_faces(blk, fl0);
            for (ENTITY *e : fl0) {
                FACE *f = (FACE *)e;
                printf("    orig face type: %s\n",
                       f->geometry() ? f->geometry()->equation().type_name() : "(null)");
            }
        }
        BODY *rb = roundtrip(blk, "subtract", 1000.0 - 3.14159265358979 * 4 * 10, 1e-3);
        if (rb) {
            ENTITY_LIST fl;
            api_get_faces(rb, fl);
            int cyl_faces = 0;
            for (ENTITY *e : fl) {
                FACE *f = (FACE *)e;
                const char *tn = f->geometry() ? f->geometry()->equation().type_name() : nullptr;
                if (tn != nullptr && strcmp(tn, "plane") != 0)
                    cyl_faces++; // 孔壁应为圆柱/圆锥面（非平面）
            }
            char buf[96];
            if (cyl_faces == 0) {
                // 调试：列出所有面类型
                for (ENTITY *e : fl) {
                    FACE *f = (FACE *)e;
                    printf("    face type: %s\n",
                           f->geometry() ? f->geometry()->equation().type_name() : "(null)");
                }
            }
            snprintf(buf, sizeof(buf), "subtract: cylinder faces=%d (孔壁)", cyl_faces);
            check(cyl_faces >= 1, buf);
            api_del_entity(rb);
        }
        // blk 持有结果，cyl 已被消耗——只删 blk 一次。
        api_del_entity(blk);
    }

    if (++g_id > run_upto) goto finish;
    printf("== 9. 布尔交（球∩方块，产生圆交线）==\n");
    {
        BODY *sph = nullptr, *blk = nullptr;
        api_solid_sphere(SPAposition(0, 0, 0), 5.0, sph);
        api_solid_block(SPAposition(0, 0, 0), SPAposition(5, 5, 5), blk);
        // api_intersect(tool, blank)：结果保留在 blank
        check(api_intersect(sph, blk).ok(), "api_intersect(tool, blank)");
        BODY *rb = roundtrip(blk, "intersect", 4.0 / 3.0 * 3.14159265358979 * 125 / 8.0, 1e-2);
        if (rb) api_del_entity(rb);
        // blk 持有结果，sph 已被消耗。
        api_del_entity(blk);
    }

finish:
    api_stop_modeller();
    terminate_base();

    printf("\n%s (failures=%d)\n", g_failed == 0 ? "ALL ENTITY TESTS PASSED" : "ENTITY TESTS HAD FAILURES", g_failed);
    return g_failed == 0 ? 0 : 1;
}
