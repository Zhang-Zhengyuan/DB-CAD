/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef HH_GEOM_STRUCTS
#define HH_GEOM_STRUCTS

#include "dcl_heal.h"
#include "hh_stru.hxx"
#include "aggrstch.hxx"
#include "bhl_stru2.hxx"
#include "bhl_stru3.hxx"

// Structure and enumeration for Simgeom analysis

enum ANAL_SIMGEOM {
	FULLY_SPLINE,			// Denotes that the body will be fully spline
	MOSTLY_SPLINE,			// Denotes that the body will be mostly spline
	FULLY_ANALYTIC,			// Denotes that the body will be fully analytic
	MOSTLY_ANALYTIC			// Denotes that the body will be mostly analytic
};

struct DECL_HEAL bhl_anal_simgeom_results {
	ANAL_SIMGEOM bhl_input_anal_simgeom;		// Result of simgeom analysis (M.A. 11 Oct 1997)
	ANAL_SIMGEOM bhl_output_anal_simgeom;		// Expected result of geom. simplification(M.A. 11 Oct 1997)
	int bhl_anal_simgeom_no_faces;		// Total number of faces
	int bhl_anal_simgeom_input_no_analytics;	// Total number of input analytics
	int bhl_anal_simgeom_input_no_splines;	// Total number of input splines
	int bhl_input_no_planes;		// Number of planes in input
	int bhl_input_no_cylinders;	// Number of cyliners in input
	int bhl_input_no_cones;		// Number of cones in input
	int bhl_input_no_tori;		// Number of tori in input
	int bhl_input_no_spheres;	// Number of spheres in input
	int bhl_anal_simgeom_no_converted;	// Number of splines which can be simplified
	int bhl_anal_simgeom_output_no_analytics;	// Total number of output analytics(M.A. 11 Oct 1997)
	int bhl_anal_simgeom_output_no_splines;	// Total number of output splines(M.A. 11 Oct 1997)
	int bhl_anal_simgeom_no_planes;		// Number of planes expected
	int bhl_anal_simgeom_no_cylinders;	// Number of cyliners expected
	int bhl_anal_simgeom_no_cones;		// Number of cones expected
	int bhl_anal_simgeom_no_tori;		// Number of tori expected
	int bhl_anal_simgeom_no_spheres;	// Number of spheres expected
	logical bhl_sim_geom_recommended;		// Whether Simplication is recommended
	logical bhl_sim_only_planes_recommended;  // Whether only planes should be converted
	double bhl_sim_geom_recommended_tol;	// Recommended tolerance value


	// Constructor function
	bhl_anal_simgeom_results ()
	{
		bhl_anal_simgeom_no_faces = 0;
		bhl_anal_simgeom_input_no_analytics = 0;
		bhl_anal_simgeom_input_no_splines = 0;
		bhl_input_no_planes = 0;
		bhl_input_no_cylinders = 0;
		bhl_input_no_cones = 0;
		bhl_input_no_tori = 0;
		bhl_input_no_spheres = 0;
		bhl_anal_simgeom_no_converted = 0;
		bhl_anal_simgeom_output_no_analytics = 0;
		bhl_anal_simgeom_output_no_splines = 0;
		bhl_anal_simgeom_no_planes = 0;
		bhl_anal_simgeom_no_cylinders = 0;
		bhl_anal_simgeom_no_cones = 0;
		bhl_anal_simgeom_no_tori = 0;
		bhl_anal_simgeom_no_spheres = 0;
		bhl_sim_geom_recommended_tol = 0.0001;

		bhl_sim_geom_recommended = FALSE;
		bhl_sim_only_planes_recommended = FALSE;
	}

};


// Structure for Simgeom results

struct DECL_HEAL bhl_simgeom_results {
	int bhl_simgeom_no_faces;			// Total number of faces
	int bhl_simgeom_initial_splines;	// Number of initial splines
	int bhl_simgeom_final_splines;		// Number of final splines
	int bhl_simgeom_no_planes;			// Number of planes made
	int bhl_simgeom_no_cylinders;		// Number of cylinders made
	int bhl_simgeom_no_cones;			// Number of cones made
	int bhl_simgeom_no_spheres;			// Number of spheres made
	int bhl_simgeom_no_tori;			// Number of tori made
	ANAL_SIMGEOM bhl_output_simgeom;	// Result of geom. simplification

	// Constructor function
	bhl_simgeom_results ()
	{
		bhl_simgeom_no_faces = 0;
		bhl_simgeom_initial_splines = 0;
		bhl_simgeom_final_splines = 0;
		bhl_simgeom_no_planes = 0;
		bhl_simgeom_no_cylinders = 0;
		bhl_simgeom_no_cones = 0;
		bhl_simgeom_no_spheres = 0;
		bhl_simgeom_no_tori = 0;
	}

};
// Function to set the member variables of struct bhl_simgeaom_results based
// on the member variables of the struct bhl_anal_simgeom_results passed
// as the second argument.

void set_struct_simgeom_results ( struct bhl_simgeom_results& simgeom_results ,
								 struct bhl_anal_simgeom_results anal_simgeom_results);



//  PVA Added : 7/2/98 end
struct DECL_HEAL bhl_autoheal_results {
	AUTOHEAL_RESULT bhl_autoheal_result;	//	Result of autoheal
	struct bhl_anal_body_stats_results anal_body_stats_results; // Results of body type analysis
	struct bhl_anal_simgeom_results anal_simgeom_results; //Results of simgeom analysis
	struct bhl_simgeom_results simgeom_results; //Results of geometry simplification
	bhl_anal_stitch_results anal_stitch_results;		// Stitch analysis summary
	bhl_stitch_results stitch_results;		// Results of stitch analysis
	struct bhl_anal_geometry_results input_anal_geometry_results; // Result geometry analysis of input model
	struct bhl_anal_geometry_results sharp_edge_results;//REsults of sharp edge
	struct bhl_geometry_results geometry_results; // Results of Geometry building
	struct bhl_anal_geometry_results output_anal_geometry_results; // Result geometry analysis of output model

	//  PVA Added : 7/2/98
	struct bhl_file_details file_details; //Details of the file sizes.

	HealStates fail_state; // Module in which the api failed, if at all it failed.

	// Constructor function
	bhl_autoheal_results ()
	{
		fail_state = INVALID_STATE;
	}
};



// Structure to store tolerance range (MA - 16 Feb 1998)
struct DECL_HEAL bhl_tol_range
{
	double min_tol;
	double max_tol;

	// Constructor
	bhl_tol_range()
	{
		min_tol = 0.0;
		max_tol = 0.0;
	}
};

// Structure for tolerance analysis results (MA - 16 Feb 1998)
struct DECL_HEAL bhl_tolerance_analysis_results
{
	logical bad_in_given_tol_range;
	double valid_tol;

	// Constructor
	bhl_tolerance_analysis_results ()
	{
		bad_in_given_tol_range = TRUE;
		valid_tol = -1.0;
	}
};


// Structure for simgeom progress

struct DECL_HEAL bhl_simplify_progress {
	double bhl_simgeom_tol;						// Current Simgeom Tolerance
	int bhl_simgeom_no_total_faces;				// Number of total faces
	int bhl_simgeom_no_faces_processed;			// Number of faces processed
	int bhl_simgeom_no_curr_splines;			// Number of current splines
	int bhl_simgeom_no_curr_analytics;			// Number of current analytics
	int bhl_simgeom_no_curr_converted;			// Number of faces simplified

	// Constructor function
	bhl_simplify_progress ()
	{
		bhl_simgeom_tol = 0.0;
		bhl_simgeom_no_total_faces = 0;
		bhl_simgeom_no_faces_processed = 0;
		bhl_simgeom_no_curr_splines = 0;
		bhl_simgeom_no_curr_analytics = 0;
		bhl_simgeom_no_curr_converted = 0;
	}

};



// Structure for Geometry Anal progress

struct DECL_HEAL bhl_geombuild_progress {
	int bhl_geom_check_total_coedges;		// Total no of coedges
	int bhl_geom_check_checked_coedges ;	// No of coedges checked
	int bhl_geom_check_total_edges ;		//Total no of edges
	int bhl_geom_check_checked_edges;		// No of edges checked
	int bhl_geom_check_total_vertices;		// Total no of vertices
	int bhl_geom_check_checked_vertices;	// No of vertices checked
	int bhl_geom_check_total_surfaces;		// Total no of surfaces
	int bhl_geom_check_checked_surfaces;	// No of surfaces checked

	// Constructor function
	bhl_geombuild_progress ()
	{
		bhl_geom_check_total_coedges = 0;
		bhl_geom_check_checked_coedges  = 0;
		bhl_geom_check_total_edges  = 0;
		bhl_geom_check_checked_edges = 0;
		bhl_geom_check_total_vertices = 0;
		bhl_geom_check_checked_vertices = 0;
		bhl_geom_check_total_surfaces = 0;
		bhl_geom_check_checked_surfaces = 0;
	}

};

// Structure for Anal Solver progress

struct DECL_HEAL bhl_analytic_progress {
	int bhl_anal_geom_stage;				// 1 means tangent
											// 2 means unstable vertices
											// 3 means intersections
	int num_stack_edges;		// No of snaps to be done
	int num_stack_count;		// No of edges snapped
	int num_intersect_edge;	// No of good edge intersections
	int num_intersect_vert;	// Good vertex intersections
	int num_intersect_fail;	// No of bad intersections
	int num_project_vert;		// No of vertex projections
	int num_project_edge;		// No of edge projections

	// Constructor function
	bhl_analytic_progress ()
	{
		bhl_anal_geom_stage = 0;
		num_stack_edges = 0;
		num_stack_count = 0;
		num_intersect_edge = 0;
		num_intersect_vert = 0;
		num_intersect_fail = 0;
		num_project_vert = 0;
		num_project_edge = 0;
	}

};

//Structure for isospline
struct DECL_HEAL bhl_isospline_progress{
	int faces_analyzed;
	int edges_processed;

	bhl_isospline_progress()
	{
		faces_analyzed = 0;
		edges_processed = 0;
	}
};

// Structure for spline geom progress

struct DECL_HEAL bhl_sharpedge_progress {
	int bhl_spline_geom_no_intersect_edge;	// No of good edge intersections
	int bhl_spline_geom_no_intersect_vert;	// Good vertex intersections
	int bhl_spline_geom_no_intersect_fail;	// No of bad intersections
	int bhl_spline_geom_no_project_vert;	// No of vertex projections
	int bhl_spline_geom_no_project_edge;	// No of edge projections

	// Constructor function
	bhl_sharpedge_progress ()
	{
		bhl_spline_geom_no_intersect_edge = 0;
		bhl_spline_geom_no_intersect_vert = 0;
		bhl_spline_geom_no_intersect_fail = 0;
		bhl_spline_geom_no_project_vert = 0;
		bhl_spline_geom_no_project_edge = 0;
	}

};

//Structure for gen spline
struct DECL_HEAL bhl_genspline_progress{
	int faces_analyzed;
	int faces_processed;

	bhl_genspline_progress()
	{
		faces_analyzed = 0;
		faces_processed = 0;
	}
};

// Structure for pcurve progress

struct DECL_HEAL bhl_wrapup_progress {
	int bhl_param_curve_no_pcurves;			// No of pcurves added

	// Constructor function
	bhl_wrapup_progress ()
	{
		bhl_param_curve_no_pcurves = 0;
	}

};

#endif

