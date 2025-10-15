/* ORIGINAL: acis2.1/kernbool/blend1/iblend1.hxx */
/* $Id: ed_cvxty.hxx,v 1.16 2000/12/26 18:42:03 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Declarations of edge convexity routines

#if !defined( ED_CVXTY_HXX )
#define ED_CVXTY_HXX

class EDGE;
class ENTITY_LIST;

#include "dcl_intr.h"
#include "logical.h"
#include "bl_enum.hxx"

#include "pt_cvty.hxx"
#include "ed_cvty.hxx"

// The functions that new code should use are the top ones:

// Compute and cache convexity information at start/middle/end of an
// edge.

DECL_INTR pt_cvty_info compute_cache_start_pt_cvty_info( EDGE * );
DECL_INTR pt_cvty_info compute_cache_mid_pt_cvty_info( EDGE * );
DECL_INTR pt_cvty_info compute_cache_end_pt_cvty_info( EDGE * );

// The related functions that do no caching, are definied in
// intersct/kernutil/cvty/compcvty.hxx, including the one for
// calculating convexity information at a given SPAparameter on an edge
// (compute_pt_cvty_info).

// Compute and cache convexity information for a whole edge. If
// approx_ok is passed TRUE, an approximation may be produced in which
// the angle range should still bound the true angle range. This may (or
// may not) be quicker for difficult edges.

DECL_INTR ed_cvty_info compute_cache_ed_cvty_info(
										  EDGE *edge,
										  logical approx_ok = FALSE
										  );

// Clear all the convexity attributes that are caching convexity
// information.

DECL_INTR void clear_cvty_attribs();

// Returns a list of convexity attributes from the global list
// that are on the current default history stream.
DECL_INTR void get_cvxty_attribs( ENTITY_LIST&);

// The functions BELOW HERE are retained temporarily for backwards
// compatibility and should no longer be invoked by new code:


// NOTE: the function bl_edge_mid_convex, bl_edge_mid_cvx,
// bl_edge_start_convex, bl_edge_end_convex CACHE the return values in
// attributes on the edges. api routines using these functions should
// take care to wipe all the attributes before they finish (using
// bl_edge_clear_cvxty_attribs) as we have no means, in general, of
// ensuring they remain up-to-date throughout changes to the model.

// Note also that we do not cache the more general calls to
// bl_edge_param_convex - there is normally less need to make them as
// edges should not be flipping convexity along their length, and it
// makes the caching mechanism more of an aggro to maintain.

// Return details, convex or concave, smooth, cusp or not.

DECL_INTR bl_ed_convexity bl_edge_mid_convex(EDGE*);
DECL_INTR bl_ed_convexity bl_edge_start_convex(EDGE*);
DECL_INTR bl_ed_convexity bl_edge_end_convex(EDGE*);

// Return just convex (true) or concave (false).

DECL_INTR logical bl_edge_mid_cvx(EDGE*);

// Return details, convex or concave, smooth, cusp or not at SPAparameter
// value on edge. This one does NOT cache information.

DECL_INTR bl_ed_convexity bl_edge_param_convex(EDGE*, double);



// Delete any attributes on edges made by the above functions which
// remember the computed convexity value.

DECL_INTR void bl_edge_clear_cvxty_attribs();

#endif
