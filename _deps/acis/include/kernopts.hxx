/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//               Header file for the kern options.
/*******************************************************************/
#if !defined(KERNOPTS_H)
#define KERNOPTS_H

    #include "dcl_kern.h"
    class option_header;
 
    extern DECL_KERN option_header careful_option;
    extern DECL_KERN option_header api_checking_option;
    extern DECL_KERN option_header unknown_attribs_pat_compatible;
    extern DECL_KERN option_header compress_bb;
    extern DECL_KERN option_header logging_option;
    extern DECL_KERN option_header history_checks;
    extern DECL_KERN option_header immediate_close;
    extern DECL_KERN option_header delete_forward;
    extern DECL_KERN option_header unhook_annotations;
    extern DECL_KERN option_header annotations;
    extern DECL_KERN option_header error_no_tag;
    extern DECL_KERN option_header address_opt;
    extern DECL_KERN option_header preserve_unknown;
    extern DECL_KERN option_header debug_rollback;
    extern DECL_KERN option_header tight_sphere_box_obsolete;
    extern DECL_KERN option_header tight_torus_box_obsolete;
    extern DECL_KERN option_header tight_spline_box; 
    extern DECL_KERN option_header dpcpy_check;
    extern DECL_KERN option_header seq_opt;
    extern DECL_KERN option_header restore_locale_opt;
    extern DECL_KERN option_header convert_on_restore;
    extern DECL_KERN option_header save_entity_count;
    extern DECL_KERN option_header save_old_sab;
    extern DECL_KERN option_header new_tol_calc;
    extern DECL_KERN option_header res_near_tangent;
    extern DECL_KERN option_header save_box;
    extern DECL_KERN option_header restore_box;
    extern DECL_KERN option_header use_new_trans;
    extern DECL_KERN option_header split_opt;
    extern DECL_KERN option_header ic_evll;
    extern DECL_KERN option_header ic_pall;
    extern DECL_KERN option_header ic_ppll;
    extern DECL_KERN option_header ic_cpll;
    extern DECL_KERN option_header ic_testevalcache;
    extern DECL_KERN option_header ic_testparamcache;
    extern DECL_KERN option_header ic_testperpcache;
    extern DECL_KERN option_header ic_testclosestcache;
    extern DECL_KERN option_header ic_shocache;
    extern DECL_KERN option_header curve_fitol;
    extern DECL_KERN option_header intcurve_save_approx_level;
    extern DECL_KERN option_header brief_curve_debug;
    extern DECL_KERN option_header use_perpdata;
    #if defined D3_DEBUG
        extern DECL_KERN option_header no_cur_data;
        extern DECL_KERN option_header no_sur_data;
    #endif
    #if defined D3_DEBUG 
        extern DECL_KERN option_header n_curve_pts;
        extern DECL_KERN option_header cfn_function_plots;
        extern DECL_KERN option_header first_lower_limit;
        extern DECL_KERN option_header second_upper_limit;
        extern DECL_KERN option_header second_lower_limit;
        extern DECL_KERN option_header third_lower_limit;
        extern DECL_KERN option_header sing_svec_significant;
    #endif
    #if defined D3_DEBUG
        extern DECL_KERN option_header full_bdy_debug;
    #endif
    #if defined D3_DEBUG 
        extern DECL_KERN option_header n_u_lines;
        extern DECL_KERN option_header n_v_lines;
        extern DECL_KERN option_header n_u_line_points;
        extern DECL_KERN option_header n_v_line_points;
        extern DECL_KERN option_header def_plot;
        extern DECL_KERN option_header fd_shift;
        extern DECL_KERN option_header min_normal_fn;
    #endif
    #if defined D3_DEBUG
        extern DECL_KERN option_header vbl_warnings;
        extern DECL_KERN option_header vbl_trim;
    #endif
    #ifdef D3_DEBUG
        extern DECL_KERN option_header vbl_alt;
    #endif
    #if defined D3_DEBUG
        extern DECL_KERN option_header old_error_method;
        extern DECL_KERN option_header square_errors;
        extern DECL_KERN option_header function_plots;
    #endif
    extern DECL_KERN option_header use_evaldata;
    extern DECL_KERN option_header fix_pcurves;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header extra_hull_check;
    #endif
    extern DECL_KERN option_header brief_pcurve_debug;
    extern DECL_KERN option_header new_pcurve_fit;
    extern DECL_KERN option_header fast_pcurves;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header blend_fit_check;
    #endif
    extern DECL_KERN option_header cone_param_range_v;
    extern DECL_KERN option_header surface_fitol;
    extern DECL_KERN option_header spline_save_approx_level;
    extern DECL_KERN option_header new_perp_guess_opt;
    extern DECL_KERN option_header fpp_spl;
    extern DECL_KERN option_header new_perp;
    extern DECL_KERN option_header brief_debug;
    extern DECL_KERN option_header ss_evll;
    extern DECL_KERN option_header ss_pall;
    extern DECL_KERN option_header ss_ppll;
    extern DECL_KERN option_header ss_testevalcache;
    extern DECL_KERN option_header ss_testparamcache;
    extern DECL_KERN option_header ss_testperpcache;
    extern DECL_KERN option_header ss_shocache;
    extern DECL_KERN option_header torus_param_range;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header exp_angle;
        extern DECL_KERN option_header exp_step_limit;
    #endif
    extern DECL_KERN option_header relax_failure_reduction;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header res_inc;
        extern DECL_KERN option_header res_target;
        extern DECL_KERN option_header newbh;
        extern DECL_KERN option_header small_second_order_terms;
    #endif
    extern DECL_KERN option_header angular_control;
    #if defined D3_DEBUG
        extern DECL_KERN option_header d3_suppress_output;
    #endif
    #ifdef D3_DEBUG
        extern DECL_KERN option_header d3_verifiers_option;
        extern DECL_KERN option_header ps_fun;
        extern DECL_KERN option_header ps_ang;
        extern DECL_KERN option_header ps_fit;
        extern DECL_KERN option_header ps_sin;
        extern DECL_KERN option_header ps_con;
        extern DECL_KERN option_header ps_dsc;
        extern DECL_KERN option_header ps_tpt;
        extern DECL_KERN option_header ps_exp;
        extern DECL_KERN option_header ps_stp;
        extern DECL_KERN option_header ps_csi;
        extern DECL_KERN option_header ps_sub;
        extern DECL_KERN option_header ps_ccs;
        extern DECL_KERN option_header ps_pcu;
    #endif
    extern DECL_KERN option_header d3_times;
    extern DECL_KERN option_header check_strings;
    extern DECL_KERN option_header binary_format;
    extern DECL_KERN option_header binary_read_format;
    extern DECL_KERN option_header fdif_step;
    extern DECL_KERN option_header fdif_test;
    extern DECL_KERN option_header fdif_sample;
    extern DECL_KERN option_header fdif_accurate;
    extern DECL_KERN option_header d3_data_dir;
    extern DECL_KERN option_header fast_sg_curve;
    extern DECL_KERN option_header check_bs3_approx;
    extern DECL_KERN option_header use_error_abort;
    extern DECL_KERN option_header test_share;
    extern DECL_KERN option_header save_as_approx;
    extern DECL_KERN option_header brief_comp_debug;
    extern DECL_KERN option_header auto_remesh;
    extern DECL_KERN option_header box_backup;
    extern DECL_KERN option_header mesh_brief_debug;
    extern DECL_KERN option_header fpp_law;
    extern DECL_KERN option_header fast_net_approx;
    extern DECL_KERN option_header net_eval_debug;
    extern DECL_KERN option_header loop_type_caching_test;
    extern DECL_KERN option_header face_uv_box_caching_test;
    extern DECL_KERN option_header net_eval_debug_radius;
    extern DECL_KERN option_header no_net_catch_all;
    extern DECL_KERN option_header d3_make_approx;
    extern DECL_KERN option_header offset_true_surface;
    extern DECL_KERN option_header pcurve_fitol;
    extern DECL_KERN option_header fan_draft_angle;
    extern DECL_KERN option_header use_draft_approx;
    extern DECL_KERN option_header draft_law_debug;
    extern DECL_KERN option_header regen_skin_approx;
    extern DECL_KERN option_header skin_eval_debug;
    extern DECL_KERN option_header skin_approx_new;
    extern DECL_KERN option_header do_self_int_check;
    extern DECL_KERN option_header check_param;
    extern DECL_KERN option_header fpp_sweep;
    extern DECL_KERN option_header fast_sweep_approx;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header default_make_approx;
    #endif
    extern DECL_KERN option_header new_iter;
    extern DECL_KERN option_header env_srf;
    extern DECL_KERN option_header old_debug_format;
    extern DECL_KERN option_header fitol_curve_interp;
    extern DECL_KERN option_header fudge_factor;
    extern DECL_KERN option_header pipe_tolerance_factor;
    #ifdef D3_DEBUG
        extern DECL_KERN option_header hermite_plots;
        extern DECL_KERN option_header hermite_output;
        extern DECL_KERN option_header chord_triangle_plot;
        extern DECL_KERN option_header use_chord_triangle;
    #endif
    extern DECL_KERN option_header check_output;
    extern DECL_KERN option_header save_summary_mode;
    extern DECL_KERN option_header post_restore_remove_zle;

    #ifdef D3_DEBUG
        extern DECL_KERN option_header check_tolerance;
    #endif
    extern DECL_KERN option_header tight_box_entity;
    extern DECL_KERN option_header bl_optional_approx_changes;
    extern DECL_KERN option_header periodic_no_seam;
    extern DECL_KERN option_header split_periodic_splines;
    extern DECL_KERN option_header complete_bs_curve_debug;
    extern DECL_KERN option_header complete_bs_surface_debug;
    extern DECL_KERN option_header attribute_debug;
    extern DECL_KERN option_header keep_geometry_sharing_info;
	extern DECL_KERN option_header pif_r17;
	extern DECL_KERN option_header auto_merge_periodic_vertices;
	extern DECL_KERN option_header restore_tags;
	extern DECL_KERN option_header v_scheme_lazy_data_backup;


#endif

