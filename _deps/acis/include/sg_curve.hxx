/* $Id: sg_curve.hxx,v 1.4 2000/12/26 18:43:12 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SG_CURVE_HXX )
#define SG_CURVE_HXX


class curve;

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"

// Create a arc length parameterized curve from a curve
// using laws.
DECL_KERN curve *
sg_curve_arclength_param (
	curve const *cur,
	double tol,
	logical approx_ok
);

// Calculates a metric for the arc length of a curve.
// zero indicates 100% arc length parametrized.
DECL_KERN double sg_curve_arclength_metric (
	curve const *cur
);


DECL_KERN curve* sg_curve_arclength_param(
	EDGE* ed,
	logical approx_ok,
	double tol);

#endif
