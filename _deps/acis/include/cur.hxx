/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// 07-Jan-08 jb	: added CUR_is_off_intcurve for off_int_cur
// 07-Spe-07 wsd: added CUR_is_ssi_intcurve
// 08-Mar-06 al : Added is_helix.
//

#ifndef CURH
#define CURH

#include "dcl_kern.h"
#include "logical.h"
#include "curdef.hxx"

class intcurve;
class SPAposition;


// Test for various types of curve. 

DECL_KERN logical	CUR_is_straight( const curve& );
DECL_KERN logical	CUR_is_ellipse( const curve& );
DECL_KERN logical	CUR_is_circle( const curve& );
DECL_KERN logical	CUR_is_intcurve( const curve& );
DECL_KERN logical   CUR_is_helix( const curve& );
DECL_KERN logical   CUR_is_surf_intcurve( const curve& );
DECL_KERN logical   CUR_is_par_intcurve( const curve& );
DECL_KERN logical   CUR_is_off_intcurve( const curve& );
DECL_KERN logical   CUR_is_proj_intcurve( const curve& );
DECL_KERN logical   CUR_is_offset_intcurve( const curve& );
DECL_KERN logical   CUR_is_blend_intcurve( const curve& cu );
DECL_KERN logical   CUR_is_ssi_intcurve( const curve& cu );
DECL_KERN logical   CUR_is_degenerate(const curve &cu);

// Test if the curves are the same to a given tolerance

DECL_KERN logical	CUR_same_curves( const curve&, const curve&,
									 double );

// This function returns true if the curve is a spline, and its fitol is zero
// indicating that evaluator can simply evaluate the `approximating' spline

DECL_KERN logical CUR_is_exact_intcurve( const curve& cu ); 


// Return the knot value closest to the mid point of the given SPAinterval, if 
// there is one; otherwise return the mid point itself. 

DECL_KERN  double  CUR_knot_in_interval( const curve&, const SPAinterval& );

// Reset t1 to first knot in the SPAinterval t0, t1, if there is one. Return a 
// flag showing whether t1 is at a knot. If t1 is at a knot on entry, to within
// resolution, then it is unset but the function returns TRUE. 

DECL_KERN logical	CUR_pull_back_to_knot( const intcurve&, double t0, double& t1 );


// Split a curve at a particular SPAparameter value / point. The first part of the 
// curve is returned as the function value, and the second overwrites the input 
// argument. If the split value is at or beyond either end then the 
// corresponding part is returned as zero. The 'active' curve ends are supplied 
// - these need not correspond to the actual curves ends. If the curve is 
// periodic then it is converted to a closed curve, and NULL is returned. 

DECL_KERN curve*	CUR_split( curve*& cu, double t_start, double t_end, 
				double t_split, const SPAposition& split_P );


				  
#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;
class int_cur;

extern	DECL_KERN D3_ostream &operator<<( D3_ostream &os, const curve& cu );
extern 	DECL_KERN D3_ostream &operator<<( D3_ostream &os, const int_cur& cu );
extern 	DECL_KERN D3_ostream &operator<<( D3_ostream &os, const evaluate_curve_side side );
extern	DECL_KERN D3_istream &operator>>( D3_istream &is, curve*& cu );

#endif
#endif
