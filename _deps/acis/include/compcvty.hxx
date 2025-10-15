/* $Id: compcvty.hxx,v 1.5 2002/08/09 17:18:58 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COMPCVTY_H
#define COMPCVTY_H

// Indent style: Acis

// History:
//
// 07-Jul-99 DAP	New code.

#include "logical.h"

#include "dcl_intr.h"

#include "pt_cvty.hxx"
#include "ed_cvty.hxx"

class COEDGE;
class EDGE;
class SPAinterval;

// Some convenient interface functions for calculating convexity
// information at points along an edge, or for the edge as a whole.
// These are provided as convenient interfaces to the cvty_calculator
// class (cvtycalc.hxx), but which may be used directly. Note that these
// functions do no caching. To find out exactly what the pt_cvty_info
// and ed_cvty_info classes represent, the reader is referred to the
// pt_cvty.hxx and ed_cvty.hxx files in this same directory.

// These functions are fine for "one-off" convexity enquiries, but if an
// application is going to make repeated calls to do a more detailed
// analysis on an edge then the cvty_calculator class should probably be
// used directly.


// Compute pt_cvty_info for a given SPAparameter. For a tolerant edge would
// use the nominal edge as the main curve, from which closest points on
// the coedge 3D curves are found.

DECL_INTR pt_cvty_info compute_pt_cvty_info(
						EDGE *edge,
						double param,
						logical use_curvatures = TRUE
						);

// As above, but for a tolerant edge would use the given coedge's 3D
// curve as the driving curve for the calculations. However, the
// parameters involved are always still taken to be *edge* parameters.

DECL_INTR pt_cvty_info compute_pt_cvty_info(
						COEDGE *coedge,
						double param,
						logical use_curvatures = TRUE
						);

// At the start/mid/end of an edge. These use the start/end of the
// *common_range* where needed. (The common_range may be different from
// the edge range for a tolerant edge - it is defined as the SPAinterval of
// the main curve for which all 3 curves overlap sensibly.)

DECL_INTR pt_cvty_info compute_start_pt_cvty_info(
							  EDGE *edge,
							  logical use_curvatures = TRUE
							  );
DECL_INTR pt_cvty_info compute_mid_pt_cvty_info(
							EDGE *edge,
							logical use_curvatures = TRUE
							);
DECL_INTR pt_cvty_info compute_end_pt_cvty_info(
							EDGE *edge,
							logical use_curvatures = TRUE
							);

// As above again, but using the given coedge's 3D curve to drive the
// angle definition.

DECL_INTR pt_cvty_info compute_start_pt_cvty_info(
							  COEDGE *coedge,
							  logical use_curvatures = TRUE
							  );
DECL_INTR pt_cvty_info compute_mid_pt_cvty_info(
							COEDGE *coedge,
							logical use_curvatures = TRUE
							);
DECL_INTR pt_cvty_info compute_end_pt_cvty_info(
							COEDGE *coedge,
							logical use_curvatures = TRUE
							);

// Compute edge convexity information for an entire edge. Uses the
// common_range where necessary. If range not passed the entire edge
// (common_)range is used. If approx_ok is FALSE, precise angle bounds
// will be computed for the edge. If TRUE, an approximate but generally
// slightly pessimistic bound is computed, which ought to be quicker for
// very difficult edges.

DECL_INTR ed_cvty_info compute_ed_cvty_info(
						  EDGE *edge,
						  logical use_curvatures = TRUE,
						  logical approx_ok = FALSE,
						  SPAinterval const &range = *(SPAinterval const *)NULL_REF
						  );

// As above, but taking the given coedge's 3D curve to drive the angle
// definition.

DECL_INTR ed_cvty_info compute_ed_cvty_info(
						  COEDGE *coedge,
						  logical use_curvatures = TRUE,
						  logical approx_ok = FALSE,
						  SPAinterval const &range = *(SPAinterval const *)NULL_REF
						  );

#endif
