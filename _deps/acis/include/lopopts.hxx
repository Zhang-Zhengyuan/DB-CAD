/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Local Operations Options
/*******************************************************************/
#if !defined(LOPOPTS_H)
#define LOPOPTS_H

#include "dcl_lop.h"
#include "option.hxx"
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_use_cci_for_vertex;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_solution_choice;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_solution_chosen;
/**
 *   char type option
 */
extern DECL_LOP option_header lop_draw_loop_sols;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_merge_vertex;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_final_solution;
/**
 *   integer type option
 */
extern DECL_LOP option_header lop_stack_limit;
/**
 *   integer type option
 */
extern DECL_LOP option_header lop_solution_loop;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_solution_all;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_solution_zero;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_sort_on_convexity;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_breadth_pruning;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_fail_on_no_part_inv_sf;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_comp_faces;
/**
 *   logical type option
 */
#ifdef D3_DEBUG
extern DECL_LOP option_header lop_no_offset;
#endif
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_rem_faces_only;
/**
 *   logical type option
 */
extern DECL_LOP option_header old_offset_tangent_edges;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_tolerant_hot;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_split_vertices;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_debug_split;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_lateral;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_prefer_same_convexity_sol;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_edge_intersect;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_ff_int;
/**
 *   logical type option
 */
//extern DECL_LOP option_header lop_repair_self_int;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_prepare_repair;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_convert_pipe;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_convert_pipe_spine;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_check_invert;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_debug_surfs;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_debug_curves;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_debug_points;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_debug_model;
/**
 *   logical type option
 */
extern DECL_LOP option_header extend_mergeable_edges;
/**
 *   logical type option
 */
#ifdef D3_DEBUG
	extern DECL_LOP option_header extend_mergeable_edges;
#endif
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_prefer_nearest_sol;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_use_euclidean_dist_score;
/**
 *   float type option
 */
extern DECL_LOP option_header lop_geometry_extension;
#if defined D3_DEBUG
	/**
	*   char type option
	*/
	extern DECL_LOP option_header cur_test;
#endif
/**
 *   float type option
 */
extern DECL_LOP option_header lop_plane_angle_tol;
/**
 *   logical type option
 */
extern DECL_LOP option_header lop_split_sing_faces_along_sharp_edge;

#endif
