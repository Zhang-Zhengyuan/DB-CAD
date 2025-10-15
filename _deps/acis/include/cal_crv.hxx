// $Id: cal_crv.hxx,v 1.7 2002/02/06 15:13:02 tgreen Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* ORIGINAL: acis2.1/sg_husk/vrbln/cal_crv.hxx */
//
// Function smooth_edges_to_curve(), used by api_smooth_edges_to_curve().
// Generates a calibration curve fit to a list of edges.
//

#ifndef CAL_CRV_H
#define CAL_CRV_H

//
// Modification---
//      02-feb-02 tgg   Added the "must_be_smooth" SPAparameter to support variable radius miters.
//	04-Nov-98 bd	Add comments about use-count of the result.
//	20-Nov-95 bd	New file, moved from sg_husk/api/smooth.cxx.
//

#include "dcl_blnd.h"
#include "lists.hxx"
#include "edge.hxx"
#include "curve.hxx"

// ********************************************************************
//
//    Make a single curve that approximates the given list
//    of smoothly-connected edges.  The list need not be sorted upon
//    input.  The curve and pointers to the edges corresponding to the
//    start and end of the curve are returned.  The curve is returned
//    as a new CURVE with a use count of 1, and must be disposed of
//    properly.  first_edge and last_edge are pointers directly into
//    the body.  This routine is used in blending, for example to find a
//    defining curve for a variable-radius blend sequence, which
//    parameterizes the blend surface and calibrates the radius function.
//
// ********************************************************************

DECL_BLND void smooth_edges_to_curve(
	ENTITY_LIST const &ents,        // input: (unsorted) list of edges
	CURVE* & calibration_curve,		// outputs
	EDGE*  & first_edge,
	EDGE*  & last_edge,
    VERTEX **vertex_of_param = NULL,
    double  *param_at_vertex = NULL,
    EDGE   **edge_array      = NULL,
    logical *edge_reversed   = NULL,
	logical must_be_smooth   = TRUE,
	logical &all_edges_sorted= *(logical *)NULL_REF
);

#endif
