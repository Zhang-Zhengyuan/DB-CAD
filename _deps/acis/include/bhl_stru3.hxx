/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (STITCH_STRUCTS)
#define STITCH_STRUCTS

#include "logical.h"

#include "dcl_stitch.h"

// Structure for input body analysis

struct DECL_STITCH bhl_anal_body_stats_results {
	int bhl_anal_body_type_no_solids;		// Number of solids in input
	int bhl_anal_body_type_no_opens;		// Number of open bodies in input
	int bhl_anal_body_type_no_faces;		// Number of free faces in input
	int bhl_anal_body_type_no_wires;		// Number of wires in the input
	int bhl_anal_body_type_no_vertices;		// Number of vertices in the input
	int bhl_anal_body_type_no_misc;			// Number of misc. entities

	// Constructor function
	bhl_anal_body_stats_results ()
	{
		bhl_anal_body_type_no_solids = 0;
		bhl_anal_body_type_no_opens = 0;
		bhl_anal_body_type_no_faces = 0;
		bhl_anal_body_type_no_wires = 0;
		bhl_anal_body_type_no_vertices = 0;
		bhl_anal_body_type_no_misc = 0;
	}

};

// Structure for stitch related statistics

struct DECL_STITCH bhl_stitch_status {
	int num_solids;
	int num_sheets;
	int num_free_faces;

	// Constructor
	bhl_stitch_status ()
	{
		num_solids = 0;
		num_sheets = 0;
		num_free_faces = 0;
	}
};

// Structure for stitch progress

struct DECL_STITCH bhl_stitch_progress {
	double bhl_stitch_tol;					// Current Stitch Tolerance
	int bhl_stitch_no_shared_edges;			// Number of edges stitched
	int bhl_stitch_no_unshared_edges;		// No of edges unstitched
	int bhl_stitch_no_total_edges;			// No of total edges
	logical bhl_stitch_state;		// 0 - Stepped Stitching, 1 - Incremental Stitching
	//MG 13Apr99 stitch options
//	logical bhl_curr_stitch_state ;
//	ENTITY_LIST bhl_paired_edge_list;
//	ENTITY_LIST bhl_stitch_edge_list;
//	double bhl_curr_stitch_tol ;
//	int bhl_stitch_curr_tol_order ;
//	logical bhl_ignore_top ;
//	double bhl_stitch_tol ;
//	double bhl_stitch_min_tol ;
//	int bhl_stitch_steps ;
  //  logical bhl_stitch_check_normals ;
//    logical bhl_stitch_face_normals ;
 //   logical bhl_make_one_solid;
  //  logical bhl_stitch_repeat ;*/
//	/*int bhl_stitch_curr_shared_edges ;
//	int bhl_stitch_curr_total_edges ;
//	int bhl_stitch_no_total_edges ;
//	int bhl_stitch_tot_unshared_edges ;
//	int bhl_stitch_curr_tol_count ;
//	int bhl_stitch_curr_tol_order ;
//	int bhl_tot_no_coalasced_edges ;
//	int bhl_tot_no_merged_edges ;
//	int bhl_tot_no_split_edges  ;*/

	//org stitch result
//	/*int bhl_stitch_no_solids;			// Number of solid bodies made
//	int bhl_stitch_no_open;				// Number of open bodies made
//	int bhl_stitch_no_unshared_edges;	// Number of final unshared edges
//	int bhl_no_unshared_loops;				// Number of unshared loops
//	int bhl_no_valid_unshared_edges;		// Number of valid unshared edges
//	int bhl_no_invalid_unshared_edges;		// Number of invalid unshared edges*/

	// Constructor function
	bhl_stitch_progress ()
	{
		bhl_stitch_tol = 0.0;
		bhl_stitch_no_shared_edges = 0;
		bhl_stitch_no_unshared_edges = 0;
		bhl_stitch_no_total_edges = 0;
		bhl_stitch_state = 0;
//		bhl_curr_stitch_state = 0;
//		bhl_curr_stitch_tol = 0;
//		bhl_stitch_curr_tol_order = 0;
//		bhl_ignore_top = FALSE;
//		bhl_stitch_tol = 0.01;
//		bhl_stitch_min_tol = 0.00001;
//		bhl_stitch_steps = 4;
//		bhl_stitch_check_normals = FALSE;
//		bhl_stitch_face_normals = FALSE;
//		bhl_make_one_solid = FALSE;
//		bhl_stitch_repeat = FALSE;*/
//		bhl_stitch_curr_shared_edges = 0;
//		bhl_stitch_curr_total_edges = 0;
//		bhl_stitch_no_total_edges = 0;
//		bhl_stitch_tot_unshared_edges = 0;
//		bhl_stitch_curr_tol_count = 0;
//		bhl_stitch_curr_tol_order = 0;
//		bhl_tot_no_coalasced_edges = 0;
//		bhl_tot_no_merged_edges = 0;
//		bhl_tot_no_split_edges = 0;*/
		//orig stitch res
//		/*bhl_stitch_no_solids = 0;
//		bhl_stitch_no_open = 0;
//		bhl_stitch_no_unshared_edges = 0;
//		bhl_no_unshared_loops = 0;
//		bhl_no_valid_unshared_edges = 0;
//		bhl_no_invalid_unshared_edges = 0;*/
	}

};


#endif// STITCH_STRUCTS

