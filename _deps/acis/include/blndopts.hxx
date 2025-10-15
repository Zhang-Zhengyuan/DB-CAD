/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//$Id:
// Header file for the blend options.

#if !defined(BLNDOPTS_H)

#define BLNDOPTS_H

void blend_options_FORCE_LINK();

#include "dcl_blnd.h"
#include "base.hxx"

class option_header;


extern DECL_BLND option_header blend_concurrent;

// blnd\blend\kernapi\api\fixblnds.cxx:
// SESSION_GLOBAL_VAR option_header blend_mix_convexity("blend_mix_convexity", TRUE);
extern DECL_BLND option_header blend_mix_convexity;

// blnd\blend\kernbool\blend1\bl_scor.cxx:
// SESSION_GLOBAL_VAR option_header force_capping( "force_cap#ping", FALSE );
extern DECL_BLND option_header force_capping;

// blnd\blend\kernbool\blend1\domitre.cxx:
// SESSION_GLOBAL_VAR option_header new_mitre_cross( "new_mitre_cross", TRUE );
extern DECL_BLND option_header new_mitre_cross;

// blnd\blend\kernbool\blend1\ff_to_ee.cxx:
// SESSION_GLOBAL_VAR option_header new_ee_ents( "new_ee_ents", TRUE );
extern DECL_BLND option_header new_ee_ents;

// blnd\blend\kernbool\blend1\veshfa.cxx:
// GLOBAL_PCONST option_header new_mitre( "new_mitre", TRUE );
extern DECL_BLND option_header new_mitre;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header DECL_BLND bl_new_technology( "bl_new_t#echnology", 3 );
extern DECL_BLND option_header bl_new_technology;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR DECL_BLND option_header blend_make_simple( "blend_make_s#imple", TRUE );
extern DECL_BLND option_header blend_make_simple;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header add_bl_atts_opt("add_bl_atts", FALSE);
extern DECL_BLND option_header add_bl_atts_opt;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_smooth_check( "bl_smooth_check", TRUE );
extern DECL_BLND option_header bl_smooth_check;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header new_blngraph( "new_blng#raph", TRUE );
extern DECL_BLND option_header new_blngraph;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_spine_check( "bl_spine_check", FALSE );
extern DECL_BLND option_header bl_spine_check;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header rb_replace_pipe( "rb_repl#ace_pipe", TRUE );
extern DECL_BLND option_header rb_replace_pipe;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_analytic_spine( "bl_ana#lytic_spine", TRUE );
extern DECL_BLND option_header bl_analytic_spine;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header use_extensions( "use_ext#ensions", TRUE );
extern DECL_BLND option_header use_extensions;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header output_abh_vr_blend_geom( "output_abh#_vr_blend_geom", FALSE);
extern DECL_BLND option_header output_abh_vr_blend_geom;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_preview_tightness( "bl_pre#view_tightness", 8.0 );
extern DECL_BLND option_header bl_preview_tightness;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_preview_approx_sf( "bl_preview_app#rox_sf", TRUE );
extern DECL_BLND option_header bl_preview_approx_sf;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_new_capping( "bl_new_c#apping", TRUE );
extern DECL_BLND option_header bl_new_capping;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_cap_box_factor( "bl_cap_box#_factor", 2.0 );
extern DECL_BLND option_header bl_cap_box_factor;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header cap_spcl_case( "cap_spcl#_case", 2);
extern DECL_BLND option_header cap_spcl_case;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header cap_preference( "cap_pref#erence", "old" );
extern DECL_BLND option_header cap_preference;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_off_xcurves( "abl_off_x#curves", FALSE );
extern DECL_BLND option_header abl_off_xcurves;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_caps( "abl_c#aps", TRUE );
extern DECL_BLND option_header abl_caps;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_require_on_support( "abl_require_on#_support", TRUE );
extern DECL_BLND option_header abl_require_on_support;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_plot( "abl_plot", FALSE );
extern DECL_BLND option_header abl_plot;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_no_sys_errors( "abl_no_sys#_errors", FALSE );
extern DECL_BLND option_header abl_no_sys_errors;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header v_blend_opt("v_blend_nsurf", TRUE);
extern DECL_BLND option_header v_blend_opt;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header v_blend_opt_rb("v_blend_rb", TRUE);
extern DECL_BLND option_header v_blend_opt_rb;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header vbl_check( "vbl_check", FALSE );
extern DECL_BLND option_header vbl_check;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header vbl_quick_check( "vbl_q#uick_check" , FALSE );
extern DECL_BLND option_header vbl_quick_check;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header vtx_blnd_simple( "vtx_blnd_simple", TRUE);
extern DECL_BLND option_header vtx_blnd_simple;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header vtx_blnd_simple_1( "vtx_blnd_simple_1", TRUE);
extern DECL_BLND option_header vtx_blnd_simple_1;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_new_def( "bl_new_def#_curve", 2 );	// 2: spine defining curve.
extern DECL_BLND option_header bl_new_def;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header drive_curve_c2( "drive#_curve_c2", 0 );
extern DECL_BLND option_header drive_curve_c2;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_drive_close_c2( "bl_drive_c#lose_c2", 0 );
extern DECL_BLND option_header bl_drive_close_c2;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_drive_npts( "bl_drive_n#pts", 5 );
extern DECL_BLND option_header bl_drive_npts;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_drive_spread( "bl_drive_s#pread", 0.01 );
extern DECL_BLND option_header bl_drive_spread;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header remote_ints( "bl_rem#ote_ints", FALSE );
extern DECL_BLND option_header remote_ints;

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header abl_remote_ints( "abl_rem#ote_ints", FALSE );
extern DECL_BLND option_header abl_remote_ints;

// This option determines whether blending is allowed to make tolerant
// edges in response to parts that are not themselves already tolerant
// (so far example, blending over a precise but near-tangent mitre). If
// turned OFF, blending reverts to its historical behaviour in which no
// tolerant edges are ever created (unless there was a tolerant edge
// there initially). If turned ON, blending may introduce tolerant edges
// of its own accord if it deems it to give the best chance of success.

// blnd\blend\kernbool\blending\blndopts.cxx:
// SESSION_GLOBAL_VAR option_header bl_tol_hot( "bl_tolerant_h#ot", -1 );
extern DECL_BLND option_header bl_tol_hot;

// Option to enable consistent transition (cross curve generation) logic.
// SPAblnd\blend_kernbool_blending.m\src\blndopts.cpp
// SESSION_GLOBAL_VAR option_header bl_consistent_cap( "bl_consistent_cap", FALSE );
// to be removed - KPI 17-Aug-2005.
extern DECL_BLND option_header bl_consistent_cap;

// This option will decide how to handle features that lie in the region removed by blending. 
// If the option is set to FALSE and when the blend completely engulfs a feature, we may 
// loose that entire feature. By setting this option to TRUE, blend attempts to retain all features 
// that are engulfed.
// SESSION_GLOBAL_VAR option_header bl_retain_features( "bl_retain#_features", TRUE );
extern DECL_BLND option_header bl_retain_features;

#endif
