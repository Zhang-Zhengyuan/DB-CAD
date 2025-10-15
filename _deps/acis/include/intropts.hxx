/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//$Id: //
// Header file for the intr options.

#if !defined(INTROPTS_H)
#define INTROPTS_H

#include "dcl_intr.h"
class option_header;

// intr\intersct\kernint\d3_cci\cci_intr.cxx:
// SESSION_GLOBAL_VAR option_header	cci_plot( "cci_plot", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header cci_plot;
#endif

// intr\intersct\kernint\d3_cci\cci_intr.cxx:
// SESSION_GLOBAL_VAR option_header cci_save( "cci_save", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header cci_save;
#endif

// intr\intersct\kernint\d3_cci\cci_intr.cxx:
// SESSION_GLOBAL_VAR option_header	cci_small_angle( "cci_small_angle", 0.3 );
#if defined D3_DEBUG
extern DECL_INTR option_header cci_small_angle;
#endif

// intr\intersct\kernint\d3_cci\cci_intr.cxx:
// SESSION_GLOBAL_VAR option_header	cci_subdivision_limit( "cci_subdivision_limit", 2000 );
#if defined D3_DEBUG
extern DECL_INTR option_header cci_subdivision_limit;
#endif

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header store_ccs_userdata( "store_ccs_us#erdata", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header store_ccs_userdata;
#endif

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header use_ccs_str_str( "use_ccs_str_str", TRUE );
#if defined D3_DEBUG
extern DECL_INTR option_header use_ccs_str_str;
#endif

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header use_ccs_special( "use_ccs_spec#ial", TRUE );
#if defined D3_DEBUG
extern DECL_INTR option_header use_ccs_special;
#endif

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header use_ccs_general( "use_ccs_gen#eral", TRUE );
#if defined D3_DEBUG
extern DECL_INTR option_header use_ccs_general;
#endif

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header use_ccs( "use_ccs", TRUE );
extern DECL_INTR option_header use_ccs;

// intr\intersct\kernint\d3_ccs\ccs_inf.cxx:
// SESSION_GLOBAL_VAR option_header pcurve_checks( "pcurve_checks", TRUE );
extern DECL_INTR option_header pcurve_checks;

// intr\intersct\kernint\d3_chk\chk_srf.cxx:
// SESSION_GLOBAL_VAR option_header	old_self_int( "old_self_int", FALSE );
extern DECL_INTR option_header old_self_int;

// intr\intersct\kernint\d3_chk\chk_srf.cxx:
//SESSION_GLOBAL_VAR option_header 	check_surface_irregular_and_selfint( "check_surface_irregular_and_selfint", TRUE );
extern DECL_INTR option_header 	check_surface_irregular_and_selfint;

// intr\intersct\kernapi\api\extrema.cxx:
//SESSION_GLOBAL_VAR option_header 	use_extrema_vertex_edge_tols( "use_extrema_vertex_edge_tols", TRUE );
extern DECL_INTR option_header 	use_extrema_vertex_edge_tols;

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header csf_plot( "csf_plot", -1 );
#if defined D3_DEBUG
extern DECL_INTR option_header csf_plot;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header csf_t0( "csf_t0", 0.0 );
#if defined D3_DEBUG
extern DECL_INTR option_header csf_t0;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header csf_t1( "csf_t1", 0.0 );
#if defined D3_DEBUG
extern DECL_INTR option_header csf_t1;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header	csi_plot( "csi_plot", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_plot;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header csi_save( "csi_save", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_save;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header	csi_all_plot( "csi_all_plot", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_all_plot;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header	csi_small_angle( "csi_small_angle", 0.6 );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_small_angle;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header	csi_boundary_angle( "csi_boundary_angle", 1.2 );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_boundary_angle;
#endif

// intr\intersct\kernint\d3_csi\csi_intr.cxx:
// SESSION_GLOBAL_VAR option_header	csi_subdivision_limit( "csi_subdivision_limit",
#if defined D3_DEBUG
extern DECL_INTR option_header csi_subdivision_limit;
#endif

// intr\intersct\kernint\d3_csi\csi_sing.cxx:
// SESSION_GLOBAL_VAR option_header csi_sing( "csi_sing", TRUE );
#if defined D3_DEBUG
extern DECL_INTR option_header csi_sing;
#endif

// intr\intersct\kernint\d3_csi\csi_sing.cxx:
// SESSION_GLOBAL_VAR option_header max_trim_distance( "max_trim_distance", -1.0 );
#if defined D3_DEBUG
extern DECL_INTR option_header max_trim_distance;
#endif

// intr\intersct\kernint\d3_csi\csi_sing.cxx:
// SESSION_GLOBAL_VAR option_header min_trim_distance( "min_trim_distance", -1.0 );
#if defined D3_DEBUG
extern DECL_INTR option_header min_trim_distance;
#endif

// intr\intersct\kernint\d3_ssi\sfn_sing.cxx:
// SESSION_GLOBAL_VAR option_header deg_uv_distance( "deg_uv_distance", SPAresnor );
#if defined D3_DEBUG
extern DECL_INTR option_header deg_uv_distance;
#endif

// intr\intersct\kernint\d3_ssi\sfn_step.cxx:
// SESSION_GLOBAL_VAR option_header sfn_check_step_hull( "sfn_check_step_hull", TRUE );
extern DECL_INTR option_header sfn_check_step_hull;

// intr\intersct\kernint\d3_ssi\sfn_step.cxx:
// SESSION_GLOBAL_VAR option_header return_para_line( "return_para_line", TRUE );
extern DECL_INTR option_header return_para_line;

// intr\intersct\kernint\d3_ssi\sfn_subd.cxx:
// SESSION_GLOBAL_VAR option_header sfn_cone_angle( "sfn_cone_angle", BISPAN_CONE_ANGLE );
#ifdef D3_DEBUG
extern DECL_INTR option_header sfn_cone_angle;
#endif

// intr\intersct\kernint\d3_ssi\sfn_subd.cxx:
// SESSION_GLOBAL_VAR option_header sfn_subdivision_limit( "subdivision_limit", SUBDIVISION_LIMIT );
#ifdef D3_DEBUG
extern DECL_INTR option_header sfn_subdivision_limit;
#endif

// intr\intersct\kernint\d3_ssi\sil.cxx:
// SESSION_GLOBAL_VAR option_header silhouette_curves( "silhouette_curves", TRUE );
extern DECL_INTR option_header silhouette_curves;

// intr\intersct\kernint\d3_ssi\sil_inf.cxx:
// SESSION_GLOBAL_VAR option_header d3sil_approx_sf( "d3sil_approx_sf", FALSE );
extern DECL_INTR option_header d3sil_approx_sf;

// intr\intersct\kernint\d3_ssi\sil_inf.cxx:
// SESSION_GLOBAL_VAR option_header abort_on_illegal_surface( "abort_on_illegal_surface", TRUE );
extern DECL_INTR option_header abort_on_illegal_surface;

// intr\intersct\kernint\d3_ssi\sil_inf.cxx:
// SESSION_GLOBAL_VAR option_header vbd_trim( "vbd_trim", 1.0 );
#ifdef D3_DEBUG
extern DECL_INTR option_header vbd_trim;
#endif

// intr\intersct\kernint\d3_ssi\sil_inf.cxx:
// SESSION_GLOBAL_VAR option_header sil_plot( "sil_plot", FALSE );
#ifdef D3_DEBUG
#endif
extern DECL_INTR option_header sil_plot;

// intr\intersct\kernint\d3_ssi\sil_inf.cxx:
// SESSION_GLOBAL_VAR option_header sil_save( "sil_save", FALSE );
#ifdef D3_DEBUG
extern DECL_INTR option_header sil_save;
#endif

// intr\intersct\kernint\d3_ssi\ssi.cxx:
// SESSION_GLOBAL_VAR option_header ssi_make_sf0_base( "ssi_make_sf0_base", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header ssi_make_sf0_base;
#endif

// intr\intersct\kernint\d3_ssi\ssi_bdry.cxx:
// SESSION_GLOBAL_VAR option_header	USE_CSI_FROM_SSI( "use_csi_from_ssi", 5 );
#ifdef D3_DEBUG
extern DECL_INTR option_header USE_CSI_FROM_SSI;
#endif

// intr\intersct\kernint\d3_ssi\ssi_intr.cxx:
// SESSION_GLOBAL_VAR option_header sur_test( "sur_test", "" );
#if defined D3_DEBUG
extern DECL_INTR option_header sur_test;
#endif

// intr\intersct\kernint\d3_ssi\ssi_intr.cxx:
// SESSION_GLOBAL_VAR option_header ssi_plot( "ssi_plot", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header ssi_plot;
#endif

// intr\intersct\kernint\d3_ssi\ssi_intr.cxx:
// SESSION_GLOBAL_VAR option_header ssi_save( "ssi_save", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header ssi_save;
#endif

// intr\intersct\kernint\d3_ssi\ssi_intr.cxx:
// SESSION_GLOBAL_VAR option_header ssi_write_failures( "ssi_write_failures", "off" );
#if defined D3_DEBUG
extern DECL_INTR option_header ssi_write_failures;
#endif

// intr\intersct\kernint\d3_ssi\ssi_sing.cxx:
// SESSION_GLOBAL_VAR option_header coising_distance( "coising_distance", -1.0 );
#if defined D3_DEBUG
extern DECL_INTR option_header coising_distance;
#endif

// intr\intersct\kernint\d3_ssi\ssi_sing.cxx:
// SESSION_GLOBAL_VAR option_header coising_plot( "coising_plot", FALSE );
#if defined D3_DEBUG
extern DECL_INTR option_header coising_plot;
#endif

// intr\intersct\kernint\d3_ssi\ssi_subd.cxx:
// SESSION_GLOBAL_VAR option_header	ssi_small_angle( "ssi_small_angle", 0.7 );
#ifdef D3_DEBUG
extern DECL_INTR option_header ssi_small_angle;
#endif

// intr\intersct\kernint\d3_ssi\ssi_subd.cxx:
// SESSION_GLOBAL_VAR option_header	ssi_boundary_angle( "ssi_boundary_angle", 1.2 );
#ifdef D3_DEBUG
extern DECL_INTR option_header ssi_boundary_angle;
#endif

// intr\intersct\kernint\d3_ssi\ssi_subd.cxx:
// SESSION_GLOBAL_VAR option_header	ssi_subdivision_limit( "ssi_subdivision_limit", 
#ifdef D3_DEBUG
extern DECL_INTR option_header ssi_subdivision_limit;
#endif

// intr\intersct\kernint\intcusf\intcusf.cxx:
// SESSION_GLOBAL_VAR option_header d3_intcusf( "d3_intcusf", FALSE );
#ifdef D3_DEBUG
extern DECL_INTR option_header d3_intcusf;
#endif

// intr\intersct\kernint\intcusf\utilcusf.cxx:
// GLOBAL_PCONST option_header fuzzy_angle( "fuzz#y_angle", 0.5 );
extern DECL_INTR option_header fuzzy_angle;

// intr\intersct\kernint\intsfsf\intplsp.cxx:
// GLOBAL_PCONST option_header helix_plane("helix_plane",FALSE);
extern DECL_INTR option_header helix_plane;

// intr\intersct\kernint\intsfsf\intsfsf.cxx:
// GLOBAL_PCONST option_header d3_intsfsf( "d3_intsfsf", FALSE );
#ifdef D3_DEBUG
extern DECL_INTR option_header d3_intsfsf;
#endif

// intr\intersct\kernutil\d3_fit\fit.cxx:
// SESSION_GLOBAL_VAR option_header fit_safety_factor( "fit_safety_factor", 1.0e4 );
#ifdef D3_DEBUG
extern DECL_INTR option_header fit_safety_factor;
#endif

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header d3_checks( "d3_checks", FALSE );
extern DECL_INTR option_header d3_checks;

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header d3_pcu_checks( "d3_pcu_checks", FALSE );
#ifdef D3_DEBUG
extern DECL_INTR option_header d3_pcu_checks;
#endif

// intr\intersct\sg_husk\sanity\sanity.cxx:
// #ifdef _DEBUG
//  GLOBAL_PCONST option_header show_warning_msg( "show_warning#_msg", TRUE );
// #else
//  GLOBAL_PCONST option_header show_warning_msg( "show_warning#_msg", FALSE );
// #endif
extern DECL_INTR option_header show_warning_msg;

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header check_and_fix( "check_fix", FALSE );
extern DECL_INTR option_header check_and_fix;

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header check_level( "check_level", c_level2);
extern DECL_INTR option_header check_level;

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header check_abort( "check_abort", FALSE );
extern DECL_INTR option_header check_abort;

// intr\intersct\sg_husk\sanity\sanity.cxx:
// GLOBAL_PCONST option_header get_aux_msg("get_aux#_msg", TRUE);
extern DECL_INTR option_header get_aux_msg;

extern DECL_INTR option_header intr_nominal_switch;

// Option for controlling whether parameter lines are drawn using the face
// parameter box (FALSE, the default) or the surface parameter box. The
// documentation states that this should be done using the face par-box, but
// historically it has been done with the surface par-box. This has been
// rectified, but this option has been added to allow the old
// behaviour to be restored, as it is possible that some customers have
// workarounds for this issue and don't want to have to do the work of
// removing them. Note that versioning of the change means that this option
// has no effect if the algorithmic version is less than 11
extern DECL_INTR option_header param_lines_on_surf;

// Option to control whether the discontinuity data for
// curves and surfaces is checked for correctness. By default this
// option is OFF.
extern DECL_INTR option_header check_discont;

extern DECL_INTR option_header quick_check;

// STL ajr 23Jan08. New option to control point_in_body, point_in_lump, point_in_shell 
// functionality. 
extern DECL_INTR option_header pt_in_functionality;

// kev Nov 2007. Moved some intersector options to the kernel because
// 1) they were being queried by the kernel, 2) the hooks 
// implemented for allowing kernel to "see" intersector options involved
// the use of statics, which are not thread-safe, and 3) it doesn't
// matter where the storage for a given option exists, just that it
// exists.  The following header pulls in the aforementioned options
// from the kernel.

extern DECL_INTR option_header fs_mp;

#include "chk_option.hxx"


#endif
