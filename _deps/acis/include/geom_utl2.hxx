/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_EDGE)
#define BHL_EDGE

#include "dcl_stitch.h"
#include "sps3srtn.hxx"
#include "bs2curve.hxx"

class CURVE;
class curve;
class EDGE;
class VERTEX;
class COEDGE;
class FACE;
class ENTITY;
class pcurve;
class intcurve;
class tolerant_stitch_options;

// Enum which stores whether curves are to be kept
// snapped or joined.
enum merge_cur_treatment
{
	UNKNOWN_TRT,
	KEEP_CURVE, // Keep one of the curve
	SNAP_CURVE, // Snap the curve to a point.
	JOIN_CURVE	// Join the curves.
};

DECL_STITCH CURVE*
bhl_make_entity_curve(const curve& this_cur);

DECL_STITCH logical
bhl_split_edge_at_vertex(	EDGE* ,	VERTEX* ,	EDGE*& );

DECL_STITCH logical
bhl_split_edge_at_vertex_and_keep_pcurve(	EDGE* ,	VERTEX* ,	EDGE*&,
										 const tolerant_stitch_options* tol_sopts = NULL);

DECL_STITCH int
bhl_split_edge_at_discontinuity(EDGE*,logical& crash_status ,double =0.0 );

logical
bhl_split_edge_at_midpoint (EDGE* , EDGE*& , VERTEX *& = *(VERTEX **)NULL_REF);

DECL_STITCH logical
bhl_split_closed_edge (EDGE* , EDGE*&,double);


DECL_STITCH logical
bhl_merge_two_edges( EDGE* , EDGE* , VERTEX* , logical=FALSE);

void
bhl_coalasce_edges_between_two_vertices( VERTEX* , VERTEX* , ENTITY_LIST& );

logical
bhl_join_edge_at_vertex( EDGE* , EDGE* , VERTEX* , merge_cur_treatment, 
						logical = TRUE, logical = TRUE );

DECL_STITCH void
get_intcurve_from_analytic_curve( EDGE*, curve*& );

DECL_STITCH void
make_analytic_edge_to_intcurve( EDGE * );


void
bhl_set_partner_partner_null( EDGE*  );

DECL_STITCH void
repar_curve(bs3_curve ,bs3_curve&, double =0.01);


// unction added for surface reparam before smoothing
DECL_STITCH void reparam_surface(bs3_surface, bs3_surface&, double, int, int);

void reparam_surf_uv(int, int, double*&, SPAposition *, int, int, int,logical&, logical& , double = 0.01);

logical check_param_uv(SPAposition*&, int, int, int, double*&, double*&, int,logical&, double = 0.01);

double avg_ratio( double *, int);

double max_knot_ratio( double *, int);

DECL_STITCH void insert_knot_on_surface(bs3_surface&, int, int, int&, int&, double*&, double*&, double = 0.01, double = 0.1);

void insert_knot_surf_uv(bs3_surface&, int, int, double*&, SPAposition *, int, int, int, double*&, int&,
						 double = 0.01, double = 0.1);

logical check_deviation(SPAposition*& , int , int , int , double*& , double , logical& );

logical check_knot_uv(bs3_surface&, SPAposition*&, int, int, int, double*&, double*&, int, logical&, logical&, double = 0.01,
					  double = 0.1);


DECL_STITCH void
insert_knot_on_curve(bs3_curve , double =0.01,double = 0.01);

// Description : Added bs2_curve_form enum as argument.
DECL_STITCH logical
bhl_merge_two_pcurves( 	const pcurve& , const pcurve& , const SPAposition&, pcurve*&,
					  merge_cur_treatment, bs2_curve_form, logical);

DECL_STITCH logical
bhl_merge_two_intcurves( const intcurve& , const intcurve& , const SPAposition&, intcurve*& );

DECL_STITCH void bhl_split_closed_edges_crossing_seam( ENTITY* this_ent );

// GSSL\SXT 77453 changed signature
DECL_STITCH void fix_coedge_pcurve_parameterization( COEDGE *coedge, double& tol_sq, 
    SPAinterval* orig_coedge_range = NULL, logical new_pcu_preferred = FALSE );

logical
stch_split_closed_edge (EDGE* , EDGE*& ,double );

void 
stch_merge_split_unstitched_edges_in_body(ENTITY_LIST& );

void
stch_merge_split_unstitched_edges(ENTITY *& );

void
stch_update_unsplit_edge_info(EDGE* );

void
stch_update_split_edges_info(EDGE* , EDGE* , VERTEX* );

void
stch_find_mergeable_edge(EDGE* , EDGE* &, VERTEX* & , ENTITY_LIST );

// GSSL/RS (Jan 28,2003) : To fix the form of the pcurve based on the edge curve form
DECL_STITCH logical
stch_fix_coedge_pcurve_form(COEDGE *coedge);

// GSSL/RS (Jan 28,2003) : To fix the form of the pcurves of the coedges based on the edge curve form
DECL_STITCH logical
stch_fix_edge_pcurve_form(EDGE *edge);

// GSSL\SXT(Nov. 8, 2004) : Given an entity(face or edge), this function considers
// all features surrounding the entity and return largest tolerance value permissible
// for the entity
// 
DECL_STITCH
double get_sq_of_largest_gap_tolerance(ENTITY* ent);


#endif

