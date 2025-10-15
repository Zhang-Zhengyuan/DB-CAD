/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BHL_ALL_CHECK
#define BHL_ALL_CHECK

#include "bs3surf.hxx"
#include "bs3curve.hxx"
#include "chk_stat.hxx"
#include "allcurve.hxx"

#include "dcl_stitch.h"

// STI jhs 10mar01 begin
// forward declare to decouple savres.hxx
#include "logical.h"
// STI jhs 10mar01 end

// GSSL/RNC 7 Oct, 2003 PROJECT WORK STARTS
// Project: API_STITCH_IMPROVEMENTS
#include "stchapi.hxx"

class ENTITY;
class EDGE;
class BODY;
class ENTITY_LIST;
class SPAposition;
class curve;


// Entity checks

DECL_STITCH logical
bhl_check_wire_body(ENTITY* this_ent);

DECL_STITCH logical
bhl_check_lump_body(ENTITY* this_ent);

DECL_STITCH logical
bhl_check_manifold_solid( ENTITY* , logical* manifold = NULL);

DECL_STITCH logical
bhl_check_independent_face( ENTITY* );

DECL_STITCH logical
bhl_check_solid( ENTITY*,int& num_edges );

DECL_STITCH double
bhl_get_edge_length(EDGE*,logical = TRUE );

// GSSL/RNC 16 August 2002 CODE STARTS
// Bug no 69373
// Description : Modified this routine to handle different tolerances
// which gets passed to it as found suitable.
DECL_STITCH logical
bhl_check_zero_edge(EDGE* , double edge_len_tol, double edge_param_tol, 
					double vert_dist_tol);

DECL_STITCH void
stch_remove_zero_edges(const ENTITY_LIST &stitched_edge_list, int& no_zero_edges);
// GSSL/RNC 16 August 2002 CODE ENDS

DECL_STITCH logical
bhl_check_edge_small (EDGE* , double );

DECL_STITCH logical
bhl_check_edges_small (ENTITY_LIST& , double );

DECL_STITCH logical
bhl_check_body_edges_small(ENTITY* , double );

DECL_STITCH double
hh_get_edge_length(EDGE* );

DECL_STITCH logical
bhl_check_edge_closed (EDGE* ,double);

DECL_STITCH logical
bhl_check_solids_in_list( ENTITY_LIST& , ENTITY_LIST* = NULL );

DECL_STITCH void
hh_check_for_tedge(BODY*);

// GSSL/RS (Mar 05,2003) : Function to check if body has unshared tedges 
// unshared (or connected to those unshared) and untolerize them
//void
//stch_check_for_tedge(BODY* body);
void stch_check_for_tedge(const ENTITY_LIST& all_edges, ENTITY_LIST& new_edges, 
							ENTITY_LIST& connected_untolerized_edges,
							const tolerant_stitch_options* tol_sopts);

// Surface checks

DECL_STITCH logical
bhl_check_for_pole_surface(bs3_surface& , int& no_poles, int[4] , double[4] = NULL);

// Position checks

DECL_STITCH logical
bhl_check_position_on_edge(EDGE* edge, const SPAposition& foot);

DECL_STITCH logical
bhl_check_position_on_edge_with_curve(EDGE* edge, const SPAposition& foot, const curve& cur);

DECL_STITCH logical
hh_check_vertices_across_shells(ENTITY* ent);

// RNC 19th Feb 2002. Function added for specific checks on curve.
DECL_STITCH logical 
hh_specific_check_curve(bs3_curve, check_status, 
						const intcurve& = *(intcurve*)NULL_REF );

#endif
