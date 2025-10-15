/* $Id: d3cu_fix.hxx,v 1.5 2002/08/20 17:09:06 btomas Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CFIXH
#define CFIXH

#include "bs3.hxx"
#include "chk_stat.hxx"


// Refine the approximate curve (bs3) based on geometric considerations.
// Calls bs3_curve_make_approx. Ensures that for each span, control polygon vectors are within
// xx degrees of the SPAvector from first to last control point.

DECL_KERN logical grefine_approx_curve( intcurve* in_int, 
										double tol, 
										double &actual_tol );


DECL_KERN logical bs2_curve_fix_range( bs2_curve &bs2,
							   const SPAinterval &range,
							   const check_fix &fix,
							   check_fix &fixed );

// This function makes a curve which is almost closed into a truly 
// closed one. It modifies the B-spline vertices at either end of the
// curve provided they need moving no further than the distance boundary
// vertices are allowed to be moved by. This information is stored in
// 'fix'. If the curve was fixed, TRUE is returned, otherwise FALSE.

DECL_KERN logical bs2_curve_fix_closure( bs2_curve bs2, 
								 const check_fix &fix,
								 check_fix &fixed );

// Removes redundant knots if multiplicity exceeds degree and the knots
// are close enough together.
DECL_KERN logical bs2_curve_fix_extra_knots( bs2_curve bs2, 
									 const check_fix &fix,
									 check_fix &fixed );

// This function removes non-G1 discontinuities by moving the
// controlpoints before and after the discontinuity. For the time being
// this only deals with cubic curves.
// The check_fix stores the maximum angle between tangents on either
// side of the discontinuity that will be fixed and the maximum distance
// interior points may be moved by.

DECL_KERN logical bs2_curve_fix_nonG1_disc( bs2_curve bs2,
									double disc,
									const check_fix &fix,
									check_fix &fixed );

// Same as the bs2_curve fixes above
DECL_KERN logical bs3_curve_fix_closure( bs3_curve bs3, 
								 const check_fix &fix,
								 check_fix &fixed );

DECL_KERN logical bs3_curve_fix_extra_knots( bs3_curve bs3, 
									 const check_fix &fix,
									 check_fix &fixed );


// This function removes coincident vertices from the given curve and
// knots with a multiplicity of more than degree
DECL_KERN logical bs3_curve_fix_coi_verts( bs3_curve &bs3, 
								   const check_fix &fix,
								   check_fix &fixed );


// This function removes non-G1 discontinuities by moving the
// controlpoints before and after the discontinuity. For the time being
// this only deals with cubic curves.
// The check_fix stores the maximum angle between tangents on either
// side of the discontinuity that will be fixed and the maximum distance
// interior points may be moved by.

DECL_KERN logical bs3_curve_fix_nonG1_disc( bs3_curve bs3,
									double disc,
									const check_fix &fix,
									check_fix &fixed );

#endif
