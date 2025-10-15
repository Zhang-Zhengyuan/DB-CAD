/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (HH_STRUCTS)
#define HH_STRUCTS

#include "logical.h"

#include "dcl_heal.h"

//----------------------------------------------------------------------------
// Structure for Geometry Analysis

struct DECL_HEAL bhl_anal_geometry_results {
	int num_total_edges;		// Total number of edges
	int num_bad_edges;			// Number of bad edges
	int num_total_coedges;		// Total number of coedges
	int num_bad_coedges;		// Number of bad coedges
	int num_total_vertices;	// Total number of vertices
	int num_bad_vertices;		// Number of bad vertices

	int num_analytic_transverse_edges; 	// Number of bad transverse analytic junctions
	int num_spline_transverse_edges; 	// Number of bad transverse spline junctions

	int num_tangent_edges; 	// Number of bad tangent junctions
	int num_tangent_edges_analytic; // Number of bad analytic tangents
	int num_G1_analytic_tangent_edges; //Number of G1 bad analytic tangent edges
	int num_tangent_edges_uv_uv; // Number of bad tangent uvsplines
	int num_tangent_edges_uv_nonuv;   // No. of bad tangent uv-nouv junctions
	int num_tangent_edges_nonuv_nonuv;   // No. of bad tangent nonuv-nouv junctions
	int num_tangent_edges_3_4_sided; // No. of bad tangent splines with 3 or 4 sided faces on either sides

	int num_tangent_uv_uv_boun_boun;	// Number of bad tangent uv boundary- uv boundary junctions
	int num_tangent_complete_uv_uv; // Number of bad tangent complete uv boundary junctions
	int num_tangent_uv_uv_boun_internal; // Number of bad tangent uv boundary - uv internal junctions
	int num_tangent_uv_uv_internal_internal; // Number of bad tangent uv internal - uv internal junctions

	int num_bad_poles;			// No. of poles
	int num_total_surfaces;	// No. of bad surfaces
	int num_bad_surfaces;		// No. of discontinuous surfaces
	logical all_edge_vert_good;	// Are all the edges/vertices good?
	logical all_geom_good;			// Are all geometries good
	int healed_percentage;		// Percentage of geometries good
	logical within_scope_of_healer;		// Whether the all bad geometry is within the scope of body-healer

	//MG 11 jun 99 multithreading
	int bhl_anal_geom_checked_surfaces; //no of surfaces checked
	int bhl_anal_geom_checked_edges;	//no of edges checked
	int bhl_anal_geom_checked_coedges;	//no of coedges checked
	int bhl_anal_geom_checked_vertices;	//no of vertices checked

	// Constructor function
	bhl_anal_geometry_results ()
	{
		num_total_edges = 0;
		num_bad_edges = 0;
		num_total_coedges = 0;
		num_bad_coedges = 0;
		num_total_vertices = 0;
		num_bad_vertices = 0;

		num_analytic_transverse_edges = 0;
		num_spline_transverse_edges = 0;

		num_tangent_edges = 0;
		num_tangent_edges_analytic = 0;
		num_G1_analytic_tangent_edges = 0;
		num_tangent_edges_uv_uv = 0;
		num_tangent_edges_uv_nonuv = 0;
		num_tangent_edges_nonuv_nonuv = 0;
		num_tangent_edges_3_4_sided = 0;

		num_tangent_uv_uv_boun_boun = 0;
		num_tangent_complete_uv_uv = 0;
		num_tangent_uv_uv_boun_internal = 0;
		num_tangent_uv_uv_internal_internal = 0;

		num_bad_poles = 0;
		num_total_surfaces = 0;
		num_bad_surfaces = 0;
		healed_percentage = 0;
		all_edge_vert_good = TRUE;
		all_geom_good = TRUE;
		within_scope_of_healer = TRUE;

		bhl_anal_geom_checked_surfaces= 0;
		bhl_anal_geom_checked_edges = 0;
		bhl_anal_geom_checked_coedges = 0;
		bhl_anal_geom_checked_vertices= 0 ;
	}
};

// Structure for Analytic Solver Results

struct DECL_HEAL bhl_analytic_solver_results
{
	int bhl_tangents_resolved;	// No. of Analytic tangent junctions resolved
	int bhl_tangents_unresolved;  // No. of Analytic tangent junctions unresolved
	int bhl_intersections_resolved; // No. of Analytic intersecions resolved
	int bhl_intersections_unresolved;  // No. of Analytic intersecions unresolved
	int bhl_vertices_resolved;	// No. of vertices resolved
	int bhl_vertices_unresolved;	// No. of vertices unresolved

	// Breakup
	int edges_intersected; // No. of edges calculated by analytic intersections
	int edges_exact_projected; // No. of edges calculated by exact projections
	int edges_approx_projected; // No. of edges calculated by approx projections
	int coincident_snaps; // No. of coincident snappings resolved
	int vertices_intersected; // No. of vertices calculated by intersections
	int vertices_projected; // No. of vertices calculated by projections
	int unstable_vertices_corrected; // No. of unstable vertices corrected

	int degree_of_graph; // Degree of the snapper graph
	int bodies_reversed; // No. of bodies reversed.

	// Constructor
	bhl_analytic_solver_results()
	{
		bhl_tangents_resolved = 0;
		bhl_tangents_unresolved = 0;
		bhl_intersections_resolved = 0;
		bhl_intersections_unresolved = 0;
		bhl_vertices_resolved = 0;
		bhl_vertices_unresolved = 0;
		edges_intersected = 0;
		edges_exact_projected = 0;
		edges_approx_projected = 0;
		coincident_snaps = 0;
		vertices_intersected = 0;
		vertices_projected = 0;
		unstable_vertices_corrected = 0;
		degree_of_graph = 0;
		bodies_reversed = 0;
	}
};

// Structure for IsoSpline Solver Results

struct DECL_HEAL bhl_spline_solver_results
{
	int n_isospline_edges_present; // No. if isospline tangent junctions present in the body

	int edges_resolved;	// No. of isospline tangent junctions resolved
	int edges_unresolved; // No. of isospline tangent junctions unresolved

	int n_complete_range_present; // No. of complete range isospline junctions present in the body
	int n_subset_present;		// No. of subset range isospline junctions present in the body
	int n_overlap_present;		// No. of overlap range isospline junctions present in the body
	int n_spline_plane_present;	// No. of spline-plane junctions present in the body
	int n_spline_analytic_present; // No. of spline-analytic junctions present in the body

	int n_complete_range_resolved; // No. of complete range isospline junctions resolved
	int n_subset_resolved;		 // No. of subset range isospline junctions resolved
	int n_overlap_resolved;		 // No. of overlap range isospline junctions resolved
	int n_spline_plane_resolved;	 // No. of spline-plane junctions resolved
	int n_spline_analytic_resolved;	 // No. of spline-analytic junctions resolved

	// MA (30 Apr 99) : Added fields for g1 results

	int n_edges_made_g1; // No. of edges made g1 successfully
	int n_edges_failed_g1; // No. of edges failed to make g1
	int n_edges_unsolvable_g1; // No. of edges not within current scope for g1

	int n_iso_splines_bent;	// No. of splines bent at vertices
	int n_iso_spline_bending_failed; // No. of failures in spline bending

//  n  int bhl_spline_sol_faces_split ;
//  n  int bhl_spline_sol_vert ;
//    int n_spline_analytic_resolved;
//  n  int bhl_spline_sol_uv_fixed ;
//    int edges_resolved;
//  n  int bhl_spline_sol_uv ;

	// Constructor
	bhl_spline_solver_results() {init();}

	void init()
	{
		n_isospline_edges_present = 0;

		edges_resolved = 0;
		edges_unresolved = 0;

		n_complete_range_present = 0;
		n_subset_present = 0;
		n_overlap_present = 0;
		n_spline_plane_present = 0;
		n_spline_analytic_present = 0;

		n_complete_range_resolved = 0;
		n_subset_resolved = 0;
		n_overlap_resolved = 0;
		n_spline_plane_resolved = 0;
		n_spline_analytic_resolved = 0;

		n_edges_made_g1 = 0;
		n_edges_failed_g1 = 0;
		n_edges_unsolvable_g1 = 0;

		n_iso_splines_bent = 0;
		n_iso_spline_bending_failed = 0;

//        bhl_spline_sol_faces_split = 0 ;
//        bhl_spline_sol_vert = 0 ;
//          n_spline_analytic_resolved = 0 ;
//        bhl_spline_sol_uv_fixed = 0 ;
//          edges_resolved = 0 ;
//        bhl_spline_sol_uv = 0 ;

	}
};

// Structure for Transversal Solver Results

struct DECL_HEAL bhl_transversal_solver_results
{
	int bhl_edges_resolved;		// No. of edges resolved
	int bhl_edges_unresolved;	// No. of edges unresolved
	int bhl_vertices_resolved;	// No. of vertices resolved
	int bhl_vertices_unresolved; // No. of vertices unresolved

	// Breakup
	int edges_intersected; // No. of edges resolved by intersections
	int edges_exact_projected; // No. of edges resolved by exact projections
	int edges_approx_projected; // No. of edges resolved by approx projections
	int vertices_intersected; // No. of vertices resolved by intersections
	int vertices_exact_projected; // No. of vertices resolved by exact projections
	int vertices_approx_projected; // No. of vertices resolved by approx projections

	// Constructor
	bhl_transversal_solver_results()
	{
		bhl_edges_resolved = 0;
		bhl_edges_unresolved = 0;
		bhl_vertices_resolved = 0;
		bhl_vertices_unresolved = 0;
		edges_intersected = 0;
		edges_exact_projected = 0;
		edges_approx_projected = 0;
		vertices_intersected = 0;
		vertices_exact_projected = 0;
		vertices_approx_projected = 0;
	}
};

// Structure for Advanced Spline Solver_results

struct DECL_HEAL bhl_advanced_spline_solver_results
{
	int bhl_no_nets;					//No of faces to be netted
	int bhl_tgt_spl_no_nets_4sided;		// No of 4-sided patches made
	int bhl_tgt_spl_no_nets_3sided;		// No of 3-sided patches made
	int bhl_tgt_spl_no_bad_nets;		// No of junction failures
	int bhl_tgt_spl_unsolvable;			// No of unsolvable junctions

	// Constructor function
	bhl_advanced_spline_solver_results()
	{
		bhl_no_nets = 0;
		bhl_tgt_spl_no_nets_4sided = 0;
		bhl_tgt_spl_no_nets_3sided = 0;
		bhl_tgt_spl_no_bad_nets = 0;
		bhl_tgt_spl_unsolvable = 0;
	}
};

// Struct for wrapup results

struct DECL_HEAL bhl_wrapup_results
{
	int pcurves_computed;		// No of pcurves computed
	int edges_trimmed;		// No of edges trimmed

	// Constructor function
	bhl_wrapup_results()
	{
		pcurves_computed = 0;
		edges_trimmed = 0;
	}
};

// Struct for results of reblending
// GSSL/RS - Reblend project
// Moved this to reblendhusk - Commented here
/*
struct DECL_HEAL bhl_reblend_results
{
	int num_blends_classified;
	int num_blends_remade;
	int num_blend_sequences;

	logical remake_ok;

	// Constructor
	bhl_reblend_results()
	{
		num_blends_classified = 0;
		num_blends_remade = 0;
		num_blend_sequences = 0;

		remake_ok = TRUE;
	}
};
*/

// Structure for Geometry results

struct DECL_HEAL bhl_geometry_results
{
	struct bhl_analytic_solver_results analytic_solver_results;	// Results of analytic solver
	struct bhl_spline_solver_results spline_solver_results;		// Results of spline solver
	struct bhl_transversal_solver_results transversal_solver_results;	// Results of transversal solver
	struct bhl_advanced_spline_solver_results advanced_spline_solver_results; // Results of advanced spline solver
	struct bhl_wrapup_results wrapup_results; // Results of wrapup
	// GSSL/RS - Reblend project
	// Reblend moved to SPAblnd
	///struct bhl_reblend_results reblend_results; // Results of reblending
};


#endif
