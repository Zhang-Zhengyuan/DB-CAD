/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include <math.h>
#include "acis.hxx"
#include "option.hxx"

#include "dcl_heal.h"
#include "analyzer.hxx"
#include "tols2.hxx"
#include "tols3.hxx"

class HH_SurfSnap_Node_Type ;

#ifndef BHL_GEOM_TOLS
#define BHL_GEOM_TOLS


// hash defines
#define bhl_curve_tang_tol 0.1
#define bhl_geombuild_face_box_fact 10

#define bhl_use_acis_extend  FALSE
#define bhl_grid_lines_u 4
#define bhl_grid_lines_v 4
#define bhl_samples_for_test  25
#define bhl_samples_per_grid  2
#define bhl_use_grid_heuristics TRUE
#define bhl_angle_per_grid M_PI/6
#define bhl_max_aspect_ratio 10.0
#define bhl_max_grid_lines 25
#define bhl_use_par_int_net TRUE
#define bhl_bend_spline_retry FALSE
#define bhl_reset_each_face FALSE
#define bhl_iso_data_reduction  TRUE
#define bhl_tight_split  FALSE

#define bhl_check_tolerance  SPAresabs
#define bhl_snap_cross_cyl  FALSE
#define bhl_uif_snap  FALSE
#define bhl_use_old_extend  TRUE
#define bhl_merge_pcurve_attrib  FALSE

#define bhl_fix_forms  TRUE
#define bhl_approx_pcurves  FALSE

#define bhl_open_geom_tol  0.01
#define bhl_geom_intersect  TRUE
#define hh_geometry_swap  TRUE
#define hh_autoheal_attrib  FALSE
#define bhl_check_pole  TRUE
#define bhl_check_tang  TRUE
#define bhl_check_coedge  TRUE
#define bhl_check_edge_vert  TRUE
#define bhl_check_surface  FALSE
#define bhl_points_for_uv  4

#define bhl_geom_check	TRUE
#define bhl_curv_sort TRUE
#define bhl_use_approx_curves_in_geombuild  TRUE

#define bhl_sim_max_radius  1000.0
#define bhl_sim_max_radius_R10_SP4_and_above  10000.0
//#define bhl_sim_geom_curve  FALSE
#define bhl_sim_geom_surf  TRUE
#define bhl_bend_c1_tol  0.01
#define bhl_bend_g1_tol  0.01
#define bhl_trim_surfaces  TRUE
#define bhl_trim_curves  TRUE
#define bhl_trim_geometry  TRUE
#define bhl_save_pass  1
#define bhl_straighten_out_grid  TRUE
#define hh_clean_distortions  FALSE
#define bhl_add_pcurves  TRUE
#define bhl_geom_bld_bad_vert  0
#define hh_use_sg_add_pcurve  TRUE
#define bhl_geom_bld_exact_proj  0
#define bhl_simgeom_box_fact  10.0
#define bhl_anal_geom_all_good  0
#define bhl_anal_geom_edge_vert_good  0
#define bhl_anal_geom_bad_poles  0
#define hh_bend_tol_fact  5.0
#define BHL_MIN_EDGE_LENGTH  1.0e-4
#define bhl_remove_pcurves_from_geombuild  TRUE
#define bhl_initialize_tolerances  TRUE
#define bhl_remove_attribs TRUE

#define bhl_sim_geom_tol_plus  0.00001
#define bhl_check_small_edge_tol 1
#define bhl_anal_graph  1
#define bhl_fix_anal_geometry 1

#define hh_simgeom_attrib 1

#define hh_log_details FALSE

struct DECL_HEAL bhl_geombld_options
{
	int bhl_geom_bld_approx_proj ;
	int bhl_geom_bld_proj_vert ;
	int bhl_geom_bld_good_vert ;
	int bhl_geom_bld_good_intersect ;
	int bhl_geom_bld_bad_intersect;

	bhl_geombld_options()
	{
		bhl_geom_bld_approx_proj = 0;
		bhl_geom_bld_proj_vert = 0;
		bhl_geom_bld_good_vert = 0;
		bhl_geom_bld_good_intersect = 0;
		bhl_geom_bld_bad_intersect = 0;
	}
};

DECL_HEAL void bhl_init_tols();

//DECL_HEAL extern option_header bhl_smooth_edges;
// RNC 31 Aug option for surface smoothing
//DECL_HEAL extern option_header bhl_smooth_surfaces;

#ifdef THREAD_SAFE_ACIS
DECL_HEAL extern safe_integral_type<logical> bhl_sim_geom_curve;
#else
DECL_HEAL extern logical bhl_sim_geom_curve;
#endif

//DECL_HEAL extern double bhl_geom_clean_tol_nor;


#ifdef GSS_BHL_UIF
DECL_HEAL extern bhl_anal_geometry_results intermediate_anal_geom_results;
#endif //GSS_BHL_UIF

#ifdef GSS_BHL_UIF
DECL_HEAL extern logical  bhl_uv_split;
#endif //GSS_BHL_UIF


DECL_HEAL extern int bhl_BodyHealerFailed ;


// -----------------------------------------------------
// The ACIS options for which the body is valid
// -----------------------------------------------------
DECL_HEAL extern double bhl_acisabs ;
DECL_HEAL extern double bhl_acisfit ;
DECL_HEAL extern double bhl_acisnor ;



#ifdef GSS_BHL_UIF
DECL_HEAL extern logical bhl_check_final_grid;
DECL_HEAL extern logical bhl_make_grid_wires;
#endif

// Healer log file
//extern FILE *bhl_log_file;

// RS - start (Dec 27, 2001) -- Option header for gen-spline solver
//DECL_HEAL extern option_header hh_skip_gen_spline_solver;
// RS - end

// RS - start (Jan 29, 2002) -- Export Option header for global graph-based new analytic solver
//DECL_HEAL extern option_header hh_use_new_analytic_solver;
// RS - end

#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
