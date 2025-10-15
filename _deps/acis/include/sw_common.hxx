// $Id: sw_common.hxx,v 1.10 2002/08/09 17:13:30 jeff Exp $
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
//	File:		sw_common.hxx
//															
// This file contains classes and methods common to sw_curve and sw_surface
//
/*-----------------------------------------------------------*/

#ifndef sw_common_hxx
#define sw_common_hxx


// include files---
#include "dcl_law.h"
#include "position.hxx"

class SPAvector;
class bezier_segment;

// Defines when to remove double knots from the interior of bspline approximations.
typedef enum 
{
	knot_removal_smart, // default - removes knots when it is safe to do so.
				// This resets fit_tol smaller to ensure that removing knots
				// does not push the resulting error beyond the original fit_tol.

	knot_removal_never,
	knot_removal_always // not recommended, since the result could be very inaccurate.
} knot_removal_strategy;


// measures the ratio of start_value from start_low to start_high.
// sets end_value to the same ratio between end_low and end_high.
// For example, passing in 1,3,4, 10,X,40 sets X to 30.
DECL_LAW void sw_ratio(	double start_low, double start_value, double start_high,
				double end_low, double & end_value, double end_high);
DECL_LAW void sw_ratio(	double start_low, double start_value, double start_high,
				const SPAvector &end_low, SPAvector & end_value, const SPAvector &end_high);


// Evaluates a degree <order-1> bezier segment at SPAparameter value t.
void bezier_segment_eval_v_v(double t, 
						 int order,
						 const SPAvector * in, // there are <order> of these
						 SPAvector * out,
						 double * weights = NULL, // NULL, or <order> of these
						 double * w=NULL // if not NULL, the weight gets returned here.
						 );

// This version just calls the SPAvector version.
void bezier_segment_eval_p_v(double t, 
						 int order,
						 const SPAposition * in, // there are <order> of these
						 SPAvector * out,
						 double * weights = NULL // NULL, or <order> of these
						 );

// This version just calls the SPAvector version.
void bezier_segment_eval_p_p(double t, 
						 int order,
						 const SPAposition * in, // there are <order> of these
						 SPAposition * out,
						 double * weights = NULL // NULL, or <order> of these
						 );

// This version just calls the SPAvector version.
void bezier_segment_eval_v_p(double t, 
						 int order,
						 const SPAvector * in, // there are <order> of these
						 SPAposition * out,
						 double * weights = NULL // NULL, or <order> of these
						 );

// Evaluates a degree <order-1> bezier segment at SPAparameter value t.
// The point dimension is 1, not 3.
// Answer goes inside w.
void bezier_segment_eval_d_d(double t, 
						 int order,
						 const double * weights, // there are <order> of these
						 double & w
						 );


// Evaluates the 1st derivative of a degree <order-1> rational bezier segment at SPAparameter value t.
SPAvector rational_bezier_segment_derivative1(double t, 
						 int order,
						 const SPAvector * cp, // there are <order> of these
						 double * weights  // there are <order> of these, may NOT be NULL.
						 );

// Evaluates the 2nd derivative of a degree <order-1> rational bezier segment at SPAparameter value t.
SPAvector rational_bezier_segment_derivative2(double t, 
						 int order,
						 const SPAvector * cp, // there are <order> of these
						 double * weights  // there are <order> of these, may NOT be NULL.
						 );


// Convert a rational spline curve segment to a bezier curve segment.
// step_size is useful for traversing two-dimensional arrays in the unnatural direction.
// You MUST supply all arguments.
// NOTE: we use the SPAvector class instead of positions because they support better math.
void convert_rational_bspline_to_bezier(int degree, 
			const SPAvector *spline_control_points, 
			const double *knots,
			SPAvector * bezier_control_points, 
			int step_size, 
			const double *spline_weights, 
			double *bezier_weights
			);

// Convert a polynomial spline curve segment to a bezier curve segment.
// step_size is useful for traversing two-dimensional arrays in the unnatural direction.
// NOTE: we use the SPAvector class instead of positions because they support better math.
void convert_polynomial_bspline_to_bezier(int degree, 
			const SPAvector *spline_control_points, 
			const double *knots,
			SPAvector * bezier_control_points, 
			int step_size
			);


// ********************
// class bezier_segment 
// ********************
// The bezier_segment class is for caching the bspline segments as bezier curves
// For internal use.
class bezier_segment : public ACIS_OBJECT
{
public:
	bezier_segment();
	~bezier_segment();
	void init(int order_temp, logical need_weights);
	void eval(double t, SPAvector & result);
	void eval(double t, SPAposition & result);

	// These derivative methods assume the patch is rational!
	// Don't call them if w==NULL.
	void eval_derivative1(double t, SPAvector & result);
	void eval_derivative2(double t, SPAvector & result);

	int order; // degree+1
	SPAvector * cp; // array of size [order] control points.

	double * w; // array of size [order] weights, or NULL.
protected:
};


// Stores the bspline patches as bezier patches
// NOTE: when indexing into the control point double arrays,
// the data associated with the first sum direction moves most rapidly.
// This seems non-standard, but its the way IGES does it.
class DECL_LAW bezier_patch : public ACIS_OBJECT
{
public:
	bezier_patch();
	~bezier_patch();
	void init(int order1_temp, int order2_temp, logical need_weights);
	SPAvector eval(double s, double t, double * w_final = NULL);

	// These derivative methods assume the patch is rational!
	// Don't call them if w==NULL.
	void eval_derivative_10(double s, double t, SPAvector *result);
	void eval_derivative_01(double s, double t, SPAvector *result);
	void eval_derivative_20(double s, double t, SPAvector *result);
	void eval_derivative_11(double s, double t, SPAvector *result);
	void eval_derivative_02(double s, double t, SPAvector *result);

	// Evaluates the points and weights as a 4 dimentional polynomial.
	// w must != NULL.
	SPAvector eval_dim4(double s, double t, double * w_final);

	// Evaluates just the weights as a 1 dimentional polynomial.
	// w must != NULL.
	void eval_dim1(double s, double t, double * w_final);

	int order1; // degree1+1
	int order2; // degree2+1

	SPAvector * cp; // order1*order2 array of control points.
	double * w; // order1*order2 array of weights, or NULL.
protected:
};

// Stores the 3x3 bspline patches and their derivatives as power basis patches
class DECL_LAW power_patch33 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[16];
	double y[16];
	double z[16];
};
class DECL_LAW power_patch32 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[12];
	double y[12];
	double z[12];
};
class DECL_LAW power_patch23 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[12];
	double y[12];
	double z[12];
};
class DECL_LAW power_patch13 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[8];
	double y[8];
	double z[8];
};
class DECL_LAW power_patch22 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[9];
	double y[9];
	double z[9];
};
class DECL_LAW power_patch31 : public ACIS_OBJECT
{
public:
	SPAvector eval(double s, double t);
	double x[8];
	double y[8];
	double z[8];
};

#endif /* sw_common_hxx */
