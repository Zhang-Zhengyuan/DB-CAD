// $Id: curve_approx.hxx,v 1.5 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CURVE_APPROX_HXX
#define CURVE_APPROX_HXX

#include "dcl_kern.h"
#include "option.hxx"
#include "bs3curve.hxx" // defines bs3_curve_form and bs3_curve // defines bs3_curve_form and bs3_curve
#include "sw_common.hxx" // defines knot_removal_strategy // defines knot_removal_strategy

class law;
class curve;
class sw_curve;
class SPAinterval;

//
// Given a black SPAbox function (law) over an SPAinterval range,
// Fits a bs3_curve to it, attempting (but not guaranteed)
// to get an accuracy of fit_tol.
// If fit_tol is non-positive, a default value is used.
// knot_strategy = knot_removal_smart (default), knot_removal_never, 
//    or knot_removal_always (not recommended)
//
// The caller owns the returned sw_curve, and should ACIS_DELETE it when finished.
//
DECL_KERN bs3_curve law_to_bs3_curve(
	law * inlaw, 
	const SPAinterval &domain, 
	double fit_tol,
	double &actual_tol,
	knot_removal_strategy knot_strategy=knot_removal_smart
	);

// Same as above, but starts with and acis curve instead of a law.
// (Internally, this creates a law, then calls the above.)
DECL_KERN bs3_curve curve_to_bs3_curve(
	const curve * in_curve, 
	const SPAinterval &domain, 			   
	double fit_tol, 
	double & actual_tol,
	knot_removal_strategy knot_strategy=knot_removal_smart
	);

DECL_KERN bs3_curve sw_curve_to_bs3_curve(
	sw_curve * sw
	);


#endif

