// $Id: sw_curve_util.hxx,v 1.14 2002/08/09 17:13:30 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*-----------------------------------------------------------*/
//
//	File:		sw_curve_util.hxx
//															
/*-----------------------------------------------------------*/

#ifndef sw_curve_util_hxx
#define sw_curve_util_hxx

#include <float.h>
#include "dcl_law.h"
#include "position.hxx"
#include "vector.hxx"
#include "sw_common.hxx"
class law;
class sw_curve;
class SPAinterval;
class bezier_breakpoint_list;

// Given a black SPAbox function (law) over an SPAinterval range,
// Fits an sw_curve (NUB) to it, attempting (but not guaranteed)
// to get an accuracy of fit_tol.
// If fit_tol is non-positive, a default value is used.
// knot_strategy = knot_removal_smart (default), knot_removal_never, 
//    or knot_removal_always (not recommended)
//
// The caller owns the returned sw_curve, and should ACIS_DELETE it when finished.
//
DECL_LAW sw_curve * law_to_sw_curve(law * in_law, 
									const SPAinterval &in_curve_range, 
									double fit_tol,
									knot_removal_strategy knot_strategy=knot_removal_smart
									);

// Determines how close sw really is to in_law.
// Note that the error returned is not guaranteed to be accurate.
DECL_LAW double sw_curve_accuracy(law * in_law, sw_curve * sw);

// If you are just interested in getting the knot sequence, and don't need an sw_curve,
// then create_bezier_segments is faster than law_to_sw_curve.
// Just allocated yourself a bezier_breakpoint_list and call this function.
// Then loop through that list until it's NULL while pulling out the knot values.
DECL_LAW int create_bezier_segments(law * in_law, double low, double high, 
									double fit_tol, bezier_breakpoint_list & bez_list);


// Make a polynomial NUB curve of degree <degree> that evaluates to the identity.
// There are <num_control_points> control points.
// num_spans = num_control_points-degree.
// Evaluation range is 0<=s<=num_spans.
// This function is useful for testing purposes.
DECL_LAW sw_curve * make_identity_sw_curve(
		int num_control_points, // set this to anything > degree
		int degree
		);

class DECL_LAW bezier_breakpoint : public ACIS_OBJECT
{
public:
	bezier_breakpoint(const SPAposition &p_temp, const SPAvector & v_temp, double d_temp, double d_o = -DBL_MAX)
		: p(p_temp), v(v_temp), d(d_temp), d_orig(d_o) {next=NULL;}
	SPAposition p;
	SPAvector v;
	double d;
	double d_orig;	// New parameter, needed when the approximation will not retain the same parameterization.
	SPAposition p1; // first inbetween bezier control point
	SPAposition p2; // second inbetween bezier control point
	bezier_breakpoint * next;
};

class DECL_LAW bezier_breakpoint_list : public ACIS_OBJECT
{
public:
	bezier_breakpoint_list();
	virtual ~bezier_breakpoint_list();
	void insert_after(bezier_breakpoint * bez_before,
											  bezier_breakpoint * bez_new);
	void insert_end(bezier_breakpoint * bez_new);
	void erase_after(bezier_breakpoint * bez_before);
   void erase_all ();

	bezier_breakpoint * start;
	int count_segments();
};

class DECL_LAW spline_breakpoint : public ACIS_OBJECT
{
public:
	spline_breakpoint(double d_temp, const SPAposition p_temp)
		: d(d_temp), p(p_temp)  {next=NULL;}
	spline_breakpoint(double d_temp)
		: d(d_temp) {next=NULL;}
	double d;
	SPAposition p;
	spline_breakpoint * next;
};

class DECL_LAW spline_breakpoint_list : public ACIS_OBJECT
{
public:
	spline_breakpoint_list();
	virtual ~spline_breakpoint_list();				// this class is NOT for general use.
	void insert_first(spline_breakpoint * s_new); // must call this first (once)
	void insert_next(spline_breakpoint * s_new); // must call this second (repeatedly)
	void erase_after(spline_breakpoint * s_before); // must call this last (repeatedly)

	spline_breakpoint * start;
	int count;
protected:
	spline_breakpoint * last_inserted;
};


#endif /* sw_curve_util_hxx */
