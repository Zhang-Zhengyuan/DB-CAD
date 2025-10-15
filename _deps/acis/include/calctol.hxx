/* $Id: calctol.hxx,v 1.15 2002/08/09 17:15:19 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for calcualting the tolerance of
// a tolerant edge or vertex.

#if !defined( CALCULATE_TOLERANCE )
#define CALCULATE_TOLERANCE 

#include "dcl_kern.h"
#include "logical.h"

class ENTITY_LIST;
class ENTITY;
class BODY;
class WIRE;
class LUMP;
class SHELL;
class SUBSHELL;
class FACE;
class LOOP;
class COEDGE;
class EDGE;
class VERTEX;
class TEDGE;
class TVERTEX;

class curve;
class SPAinterval;

DECL_KERN void
calculate_edge_tolerance ( EDGE * const edge, double &tol );

DECL_KERN void
calculate_vertex_tolerance ( VERTEX * const vertex, double &tol );

// This function (calculate_curve_tolerance) performs the same
// calculation as calculate_edge_tolerance (indeed that function calls
// this one to do the work), but this one is provided in its own right
// so that algorithms can test whether something is really going to have
// to be tolerant before actually making it so. To avoid repeating the
// calculation, replace_edge_with_tedge now accepts an optional final
// argument, being the known tolerance. DAP.

DECL_KERN double
calculate_curve_tolerance(
				  SPAinterval const &test_range,	// range to test over
				  curve const *cur1,
				  SPAinterval const &c1_range,		// range of cur1
				  curve const *cur2,
				  SPAinterval const &c2_range,		// range of cur2
                  logical curves_correspond = FALSE // TRUE when the curves should
                                                    // roughly match in 3 space - 
                                                    // means function will try to 
                                                    // give a guess for the perp
				  );

DECL_KERN logical
find_max_tolerance ( ENTITY *this_entity, double &max_error );

DECL_KERN logical
walk_connected_find_max_tolerance ( EDGE *this_edge, double &max_error );

DECL_KERN logical update_tolerance( ENTITY *, logical mark = FALSE);
DECL_KERN logical update_body_tolerance( BODY *, logical mark = FALSE );
DECL_KERN logical update_wire_tolerance( WIRE *, logical mark = FALSE);
DECL_KERN logical update_lump_tolerance( LUMP *, logical mark = FALSE);
DECL_KERN logical update_shell_tolerance( SHELL *, logical mark = FALSE);
DECL_KERN logical update_subshell_tolerance( SUBSHELL *, logical mark = FALSE);
DECL_KERN logical update_face_tolerance( FACE *, logical mark = FALSE);
DECL_KERN logical update_loop_tolerance( LOOP *, logical mark = FALSE);
DECL_KERN logical update_coedge_tolerance( COEDGE *, logical mark = FALSE);
DECL_KERN logical update_edge_tolerance( EDGE *, logical mark = FALSE);
DECL_KERN logical update_vertex_tolerance( VERTEX *, logical mark = FALSE);
DECL_KERN logical update_edge_tolerance( TEDGE *tedge, logical mark = FALSE); 
DECL_KERN logical update_vertex_tolerance( TVERTEX *tvertex, logical mark = FALSE);

// Compute an upper bound for the tolerance between two curves
// (notionally an edge curve and a coedge curve), using the "point_perp
// from the coedge to the edge" rule. The curves must have the same
// sense here, and the ranges should be the approximate region of
// overlap. This guarantees to find a reliable upper bound reasonably
// quickly, aiming to get within about 5% or better of the true value.
// Note that we do not guarantee to find a bound reliably when the
// tolerance is very small, typically less than 1e-8 (on the grounds
// that no one can possibly be interested, the calling code should
// probably resort to SPAresabs instead).

// Passing a value for "tol" may improve performance if you already know
// you are not interested in tolerances less than this number (e.g. you
// have already processed one coedge on your edge, and this one is now
// the next.)

// Finally, the rough_bound argument can be passed TRUE to get a quicker
// but less accurate calculation. In this case, the answer may
// overestimate the real maximum tolerance often by as much as a
// factor of 2.

// Note that if the 2 curve geometries turn out to be incompatible (e.g.
// travelling in opposite directions in places) then the rough bound
// calculation will be performed even in cases where the tighter bound
// was requested.

DECL_KERN double calculate_curve_tolerance_bound(
							 curve const &e_crv,
							 SPAinterval const &e_rng,
							 curve const &c_crv,
							 SPAinterval const &c_rng,
							 double tol = 0.0,
							 logical rough_bound = FALSE
							 );

// Given two curves (notionally an edge and coedge curve), compute the
// precise region of overlap, according to the same "point_perp" rule as
// above.

// Return values:
// 0 - everything fine (the output ranges have been set)
// 1 - one curve was periodic, the other not
// 2 - failed to compute compatible start parameters
// 3 - failed to compute compatible end parameters

DECL_KERN int calculate_tolerant_curve_ranges(
					  curve const &edge_curve,
					  SPAinterval const &edge_range,
					  curve const &coedge_curve,
					  SPAinterval const &coedge_range,
					  SPAinterval &common_edge_range,	// output
					  SPAinterval &common_coedge_range	// output
					  );


// Internal use only.
// Functions to be used only to pre and post-process tolerant-friendly functions.
// Relies on current bulletin board.
DECL_KERN void update_current_bb_modified_entities_tolerances( logical update_boxes = FALSE );
DECL_KERN void update_current_bb_modified_entities_tolerances( ENTITY_LIST &, 
															   ENTITY_LIST &, 
															   logical update_boxes = FALSE );

// Internal use only.
// Determine the maximum distance between two nearly coincident edges' curves.
DECL_KERN double calculate_max_edge_edge_distance( EDGE* edge1, EDGE* edge2 );

#endif
