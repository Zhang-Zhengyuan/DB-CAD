#include <acis/include/kernapi.hxx>
#include <acis/include/license.hxx>
#include <acis/include/spa_unlock_result.hxx>

#include <acis/include/alltop.hxx>
#include <acis/include/allcurve.hxx>
#include <acis/include/allcudef.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/allsfdef.hxx>
#include <acis/include/pcudef.hxx>
#include <acis/include/exp_par.hxx>
#include <acis/include/int_int.hxx>
#include <acis/include/pcurve.hxx>

#include "acis.hxx"

// -------------------------------------------------------------------------------------------------------
// #include "spatial_license.h"
// Customer Name:               Tsinghua university
// Application Name:            GME
// Licensed Spatial Products:
//    3D ACIS Modeler
// Spatial Product Version:     2022
// Spatial License Version:     R32
// License Type:                Production
// Expiration Date:             N/A
// Application Registration:    Not required
// License Key Index:           13585
// Generation Date:             2022/03/31


#ifndef SPATIAL_LICENSE_DEFINED
#define SPATIAL_LICENSE_DEFINED
#define SPATIAL_LICENSE "3VKKZ85TA8QRNSJT3HK3EC5NE82CNQT8785DJEMRR87UV8JARRHD2NP8PA7CMDM5PR7KNNJLK8HDEAME9ACUDE58PRNUVSJUJR7RNCHFEA2C2GP8VCNUJGTRV8M7PHHTPADKPJHXRRD3GHMWNR2UJJJE7CBDDAT7QC3UPHTHRRPCKUHWECLCJAJFJAXCPQTVR3K3AZH8VRRZAZM7J8SRVJN7QH2CSS2JKERDFE2B3HJKZ85NQR3FWN2H7V8FFQDFA3D3A8JUVGJLDAHNN8Q7VCMUQXCNEQDQ3GNMMZ22PEQ7EQD5PRTDZZ5NJR3MUY877CTMW8BJ9VG5A857A3J3AQT79VHMMZ22PEECU8XU3HD3AZ5TH8B3AUJXKATZMCHR9L3UNDJR9LVUMAJXR3N3AHPD9E8ZFG8NSACQEQDZ36NMKS2M3H8DZE5NJAS5WAJNKGT5AH5SQ8N7LQMSWTNREJQ7QHNC9H82TV5FDC2W3HKREH5NE82ZPEMUERMKPHJ57AJRPATWACSUJ85APREDKWH33X27JEJZ7RTCK8JC3LSKNCMQTLGCMCM7JC3DVSHXVCEUNZJRP8ZD2N5ZR8CDN2HTPCJ7KCPZPCEDNAHH9CTDK2JATAXCMCH59CHZG8PQVRB3VE5HKC8CGATV9AS7KEHV3HMKP8MATCUDZZ5NHAEF7EPDRGMLUY8B3HNKZZ5NHAEUDEPDRGMLUY8B3HNKZ"
#endif
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// ACIS环境初始化和结束函数

void unlock_license() {
    spa_unlock_result out = spa_unlock_products(SPATIAL_LICENSE);
    if (out.get_state() == SPA_UNLOCK_PASS_WARN) { // insert your warning notification code here
        ;
    } else if (out.get_state() != SPA_UNLOCK_PASS) { // insert your error handling code here
        ;
    }
}

// 返回初始化级别：1-基础，2-建模引擎，0-失败。
int initialize_acis() {
    outcome result;

    int level = 0;
    // 基础初始化将创建线程本地存储和线程ID。

    if (initialize_base()) {  // 在初始化ACIS时传递该默认配置
        ++level;
        outcome result = api_start_modeller(0);
        if (result.ok()) {
            ++level;
            // 在此添加通用ACIS状态。
        }
    }
    if (level < 3) unlock_license();
    return level;
}

void terminate_acis(int level) {
    // 终止ACIS
    if (level > 1) {
        // 终止之前必须先销毁该outcome对象，因为访问了线程本地存储。
        outcome result = api_stop_modeller();
        if (result.ok() != FALSE) {
            --level;
        }
    }

    if (level > 0) {
        if (terminate_base()) {
            --level;
        }
    }
}

std::string process(outcome& result) {
    if (!result.ok()) {
        std::string err(find_err_mess(result.error_number()));
        std::string errW(err.begin(), err.end());
        return errW;
    }
    err_mess_type* warnings = nullptr;
    int nwarn = get_warnings(warnings);
    for (int i = 0; i < nwarn; ++i) {
        printf("Warning %d : %s\n", warnings[i], find_err_mess(warnings[i]));
    }
    return "";
}

// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// ACIS类新增的方法

//保存用
SPAinterval curve::gme_get_subset_range() { return subset_range; }
straight STRAIGHT::gme_get_def() { return def; }
ellipse ELLIPSE::gme_get_def() { return def; }
int_cur* intcurve::gme_get_fit() { return fit; }
bs3_curve int_cur::gme_get_cur_data() { return cur_data; }
double int_cur::gme_get_fitol_data() { return fitol_data; }
surface* int_cur::gme_get_surf1_data() { return surf1_data; }
surface* int_cur::gme_get_surf2_data() { return surf2_data; }
bs2_curve int_cur::gme_get_pcur1_data() { return pcur1_data; }
bs2_curve int_cur::gme_get_pcur2_data() { return pcur2_data; }
SPAinterval int_cur::gme_get_safe_range() { return safe_range; }
intcurve INTCURVE::gme_get_def() { return def; }
helix HELIX::gme_get_def() { return def; }
SPApar_box surface::gme_get_subset_range() { return subset_range; }
plane PLANE::gme_get_def() { return def; }
cone CONE::gme_get_def() { return def; }
sphere SPHERE::gme_get_def() { return def; }
torus TORUS::gme_get_def() { return def; }
spline SPLINE::gme_get_def() { return def; }
spl_sur* spline::gme_get_spl() { return spl; }
int spl_sur::gme_get_u_singularity() { return u_singularity; }
int spl_sur::gme_get_v_singularity() { return v_singularity; }
int spl_sur::gme_get_closed_in_u() { return closed_in_u; }
int spl_sur::gme_get_closed_in_v() { return closed_in_v; }
par_cur* pcurve::gme_get_fit() { return fit; }
bs2_curve exp_par_cur::gme_get_cur_data() { return cur_data; }
double exp_par_cur::gme_get_fitol_data() { return fitol_data; }
surface* exp_par_cur::gme_get_surf_data() { return surf_data; }
int PCURVE::gme_get_def_type() { return def_type; }
pcurve PCURVE::gme_get_def() { return def; }

//恢复用
void** BODY::gme_get_lump_ptr() { return (void**)(&lump_ptr); }
void** BODY::gme_get_wire_ptr() { return (void**)(&wire_ptr); }
void** BODY::gme_get_transform_ptr() { return (void**)(&transform_ptr); }
void** LUMP::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** LUMP::gme_get_shell_ptr() { return (void**)(&shell_ptr); }
void** LUMP::gme_get_body_ptr() { return (void**)(&body_ptr); }
void** SHELL::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** SHELL::gme_get_subshell_ptr() { return (void**)(&subshell_ptr); }
void** SHELL::gme_get_face_ptr() { return (void**)(&face_ptr); }
void** SHELL::gme_get_wire_ptr() { return (void**)(&wire_ptr); }
void** SHELL::gme_get_lump_ptr() { return (void**)(&lump_ptr); }
void** SUBSHELL::gme_get_parent_ptr() { return (void**)(&parent_ptr); }
void** SUBSHELL::gme_get_sibling_ptr() { return (void**)(&sibling_ptr); }
void** SUBSHELL::gme_get_child_ptr() { return (void**)(&child_ptr); }
void** SUBSHELL::gme_get_face_ptr() { return (void**)(&face_ptr); }
void** SUBSHELL::gme_get_wire_ptr() { return (void**)(&wire_ptr); }
void** FACE::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** FACE::gme_get_loop_ptr() { return (void**)(&loop_ptr); }
void** FACE::gme_get_shell_ptr() { return (void**)(&shell_ptr); }
void** FACE::gme_get_subshell_ptr() { return (void**)(&subshell_ptr); }
void** FACE::gme_get_geometry_ptr() { return (void**)(&geometry_ptr); }
void** LOOP::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** LOOP::gme_get_start_ptr() { return (void**)(&start_ptr); }
void** LOOP::gme_get_face_ptr() { return (void**)(&face_ptr); }
void** WIRE::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** WIRE::gme_get_coedge_ptr() { return (void**)(&coedge_ptr); }
void** WIRE::gme_get_owner_ptr() { return (void**)(&owner_ptr); }
void** WIRE::gme_get_subshell_ptr() { return (void**)(&subshell_ptr); }
void** COEDGE::gme_get_next_ptr() { return (void**)(&next_ptr); }
void** COEDGE::gme_get_previous_ptr() { return (void**)(&previous_ptr); }
void** COEDGE::gme_get_partner_ptr() { return (void**)(&partner_ptr); }
void** COEDGE::gme_get_edge_ptr() { return (void**)(&edge_ptr); }
void** COEDGE::gme_get_owner_ptr() { return (void**)(&owner_ptr); }
void** COEDGE::gme_get_geometry_ptr() { return (void**)(&geometry_ptr); }
void** EDGE::gme_get_start_ptr() { return (void**)(&start_ptr); }
void** EDGE::gme_get_end_ptr() { return (void**)(&end_ptr); }
void** EDGE::gme_get_coedge_ptr() { return (void**)(&coedge_ptr); }
void** EDGE::gme_get_geometry_ptr() { return (void**)(&geometry_ptr); }
void** VERTEX::gme_get_edge_ptr() { return (void**)(&edge_ptr); }
void** VERTEX::gme_get_geometry_ptr() { return (void**)(&geometry_ptr); }
pcurve* PCURVE::gme_get_def_ptr() { return &def; }
void** PCURVE::gme_get_ref_curve_ptr() { return (void**)(&cur); }
void PCURVE::set_fit(par_cur* fit) { def.replace_par_cur(*fit); }
void** PCURVE::gme_get_fit_ptr() { return (void**)(def.gme_get_fit_ptr()); }
void pcurve::gme_set_rev(int data) { rev = data; }
void pcurve::gme_set_off(SPApar_vec data) { off = data; }
par_cur** pcurve::gme_get_fit_ptr() { return &fit; }
void exp_par_cur::gme_set_surf_spl(spl_sur* spl) { ((spline*)surf_data)->replace_spl_sur(*spl); }
void** exp_par_cur::gme_get_surf_spl_ptr() { return (void**)(((spline*)surf_data)->gme_get_spl_ptr()); }
exp_par_cur* exp_par_cur::gme_exp_par_cur_public_constructor(bs2_curve cur, double fit_tol, double par_tol, surface const& surf, logical owns_bs2) {
    exp_par_cur* ret = ACIS_NEW exp_par_cur(cur, fit_tol, par_tol, surf, owns_bs2);
    return ret;
}
void straight::gme_set_param_scale(double val) { param_scale = val; }
void INTCURVE::gme_set_fit(int_cur* fit) { def.replace_int_cur(*fit); }
void** INTCURVE::gme_get_fit_ptr() { return (void**)(def.gme_get_fit_ptr()); }
void intcurve::gme_set_rev(int data) { rev = data; }
int_cur** intcurve::gme_get_fit_ptr() { return &fit; }
void int_cur::gme_set_fitol_data(double data) { fitol_data = data; }
void int_cur::gme_set_safe_range(SPAinterval data) { safe_range = data; }
void** int_cur::gme_get_surf1_spl_ptr() { return (void**)(((spline*)surf1_data)->gme_get_spl_ptr()); }
void** int_cur::gme_get_surf2_spl_ptr() { return (void**)(((spline*)surf2_data)->gme_get_spl_ptr()); }
void int_cur::set_surf1_spl(spl_sur* spl) { ((spline*)surf1_data)->replace_spl_sur(*spl); }
void int_cur::set_surf2_spl(spl_sur* spl) { ((spline*)surf2_data)->replace_spl_sur(*spl); }
void SPLINE::gme_set_spl(spl_sur* spl) { def.replace_spl_sur(*spl); }
void** SPLINE::gme_get_spl_ptr() { return (void**)(def.gme_get_spl_ptr()); }
void spline::gme_set_rev(int data) { rev = data; }
spl_sur** spline::gme_get_spl_ptr() { return &spl; }
void spl_sur::gme_set_fitol_data(double data) { fitol_data = data; }

int_int_cur* int_int_cur::gme_int_int_cur_public_constructor(
    bs3_curve surf,			// spline curve
    double tol,
    surface const& f_cur,
    // first surface on which curve lies
    surface const& s_cur,
    // second surface on which curve lies
    bs2_curve f_surf,	// curve in SPAparameter space of the
    // first surface
    bs2_curve s_surf,	// curve in SPAparameter space of the
    // second surface
    const SPAinterval& safe_range
) {
    int_int_cur* ret = ACIS_NEW int_int_cur(surf, tol, f_cur, s_cur, f_surf, s_surf, safe_range);
    return ret;
}

int_int_cur::int_int_cur(const char* gme, bs3_curve surf, double tol, surface const& f_sur, surface const& s_sur, bs2_curve f_cur, bs2_curve s_cur, const SPAinterval& safe_range) : int_cur(surf, tol, f_sur, s_sur, f_cur, s_cur, safe_range) {}

void curve::gme_set_subset_range(const SPAinterval& range) { subset_range = range; }
void surface::gme_set_subset_range(const SPApar_box& box) { subset_range = box; }

// -------------------------------------------------------------------------------------------------------