/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// original acisr19
/********************************************************************/
// The functions in this file are low level and not intended for general use.

#ifndef __BS3_CURVE_ALIGN
#define __BS3_CURVE_ALIGN
#include "dcl_spl.h"
#include "bs3curve.hxx"  // bs3_curve - cannot fwd declare a type_def  

// remove knot at specified location
// do not use without consulting spatial kernel developers.
/**
* @nodoc
*/
DECL_SPLINE int bs3_curve_rem_knot_at(bs3_curve bs3, double t, double tol, int ntimes);

// avoids sliver spans by moving knots slightly if knots on different curves are within effective tolerance
// effective tolerance is tol/crvn ( The within tolerance relationship is non transitive.  This 
// function aims to produce curves where knots are at least tol away from each other.)
//
// this function does not add any knots to the input bs3 curves.
//
// this function does not check that the parameter ranges for the input curves are the same, or adjust param ranges in any way.
//
// do not use without consulting spatial kernel developers.
/**
* @nodoc
*/
DECL_SPLINE void bs3_align_close_knots(int crvn, bs3_curve* crvs, double tol);

#endif
