/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "math.h"
#include "acis.hxx"
#include "option.hxx"

#include "dcl_stitch.h"
#include "analyzer3.hxx"

#ifndef _STITCH_TOLS_HXX
#define _STITCH_TOLS_HXX

#define no_glob_chars 8000

#define	BHL_PI		3.14159265358979323846

// hash defines
#define bhl_box_tol_fact 20
#define bhl_stitch_edge_len_tol_fact 3.0
#define bhl_prop_error 1
#define bhl_final_topology_clean  TRUE
#define bhl_solid_oriention  TRUE
#define bhl_points_on_curve  10
#define bhl_mid_edge_tol_fact  3.0
#define bhl_stitch_split_tol_fact  3.0
#define bhl_stitch_split  TRUE
#define bhl_stitch_min_tol_plus  0.00001
#define bhl_stitch_max_tol_plus  0.1
#define bhl_use_approx_curves_in_stitch  FALSE
#define bhl_fix_vertex_group  TRUE
#define bhl_make_single_sided  TRUE
#define bhl_bs3_connect  FALSE
#define bhl_split_edge_tol_fact  20.0
#define bhl_geom_clean_tol_fact  3.0
#define bhl_merge_small_edges  TRUE
#define bhl_split_closed_edges  TRUE
#define bhl_geom_clean  TRUE

// Colors
#define bhl_vertex_color  6
#define bhl_edge_color  1
#define bhl_bad_vertex_color  5
#define bhl_bad_edge_color  4
#define bhl_uv_bent_color  6
#define bhl_uv_boun_color  2
#define bhl_uv_color  7
#define bhl_tang_color  3
#define bhl_split_color  4
#define bhl_simgeom_color  4
#define bhl_body_color  1
#define bhl_boundary_color  2
#define bhl_uv_anly_bent_color  6
#define bhl_uv_anly_color  5
#define bhl_uv_nonuv_color  6
#define bhl_analytic_color  7

#define bhl_mer_fac  10.0
#define bhl_stitch_degenerate_solids  0
#define bhl_remove_pcurves_from_stitch  FALSE

//#define hh_use_geombuild_attribs  1

#define VALIDATE_BODY_BEFORE_HEALING FALSE
#define MAX_KNOT_VALUE	1.0e6


#ifdef GSS_BHL_UIF
DECL_STITCH extern ENTITY_LIST bhl_paired_edge_list;
DECL_STITCH extern ENTITY_LIST bhl_stitch_edge_list;
#endif // GSS_BHL_UIF

DECL_STITCH extern logical  bhl_stitch_snap;

// RNC: 16 Jun 99 bhl_g_body if defined .
#ifdef GSS_BHL_UIF
DECL_STITCH extern BODY *bhl_g_body;
#endif //GSS_BHL_UIF

#ifdef THREAD_SAFE_ACIS
DECL_STITCH extern safe_pointer_type<char> bhl_glob_sum_msg_str;
#else
DECL_STITCH extern char bhl_glob_sum_msg_str[no_glob_chars];
#endif

// RNC: 16 Jun 99
#ifdef GSS_BHL_UIF
DECL_STITCH extern VIEW_TYPE bhl_view_type ;
#endif //GSS_BHL_UIF

#ifdef GSS_BHL_UIF
DECL_STITCH extern logical  bhl_update_screen;
#endif // GSS_BHL_UIF

#ifdef GSS_BHL_UIF
DECL_STITCH extern logical bhl_stitch_repeat;
#endif //GSS_BHL_UIF

// Log file for stitching
// DECL_STITCH 
#ifdef THREAD_SAFE_ACIS
extern safe_pointer_type<FILE> results_log_file ;
#else
extern FILE* results_log_file ;
#endif

#endif
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
