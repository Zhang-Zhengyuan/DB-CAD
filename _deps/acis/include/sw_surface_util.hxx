// $Id: sw_surface_util.hxx,v 1.13 2002/08/09 17:13:30 jeff Exp $
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
//	File:		sw_surface_util.hxx
//															
/*-----------------------------------------------------------*/
// History
// 01-Nov-02 rocon : Added discontinuous_at_u,v methods to surface_evaluator

#ifndef sw_surface_util_hxx
#define sw_surface_util_hxx

#include "dcl_law.h"
#include "position.hxx"
#include "vector.hxx"
#include "sw_common.hxx"
class law;
class SPAinterval;
class sw_surface;


DECL_LAW sw_surface * law_to_sw_surface(
			law * in_law, 
			const SPAinterval &range_s, 
			const SPAinterval & range_t,
			double fit_tol, 
			double & actual_tol,
			int initial_num_knots_u=0,
			int initial_num_knots_v=0,
			const double * initial_knots_u=NULL,
			const double * initial_knots_v=NULL
			);


// Make a polynomial NUB surface of degree1 by degree2 that evaluates to the identity.
// There are num_control_points1 by num_control_points2 control points.
// num_spans_in_s = num_control_points1-degree1.
// num_spans_in_t = num_control_points2-degree2.
// Evaluation ranges are 0<=s<=num_spans_in_s and 0<=t<=num_spans_in_t.
// This function is useful for testing purposes.
DECL_LAW sw_surface * make_identity_sw_surface(
		int num_control_points1, // set this to anything > degree1
		int num_control_points2, // set this to anything > degree2
		int degree1=3,
		int degree2=3
		);

// Make an sw_surface of degree1 by degree2 that evaluates to something nonuniform.
// There are num_control_points1 by num_control_points2 control points.
// num_spans_in_s = num_control_points1-degree1.
// num_spans_in_t = num_control_points2-degree2.
// This function is useful for testing purposes.
DECL_LAW sw_surface * make_random_sw_surface(
		int num_control_points1, // set this to anything > degree1
		int num_control_points2, // set this to anything > degree2
		int degree1,
		int degree2,
		int polynomial,
		double min1,
		double max1,
		double min2,
		double max2
		);

DECL_LAW double sw_surface_accuracy(law * in_law, 
						   sw_surface * sws);

// *****************
// surface_evaluator
// *****************
// derive your own class with your data, and implement eval().
// the surface must be G1 continuous.
class DECL_LAW surface_evaluator : public ACIS_OBJECT
{
public:
	surface_evaluator() {};
	virtual ~surface_evaluator() {};

	virtual SPAposition eval(double u, double v) const =0;
	virtual void eval_derivs(double u, double v, SPAposition &p, SPAvector &du, SPAvector &dv, SPAvector &duv,
		logical last_u, logical last_v) const;
	virtual int discontinuous_at_u(double u) const = 0;
	virtual int discontinuous_at_v(double v) const = 0;
};

// *********************
// surface_evaluator_law
// *********************
class DECL_LAW surface_evaluator_law : public surface_evaluator
{
public:
	surface_evaluator_law(law * in_law_temp);
	virtual ~surface_evaluator_law();

	virtual SPAposition eval(double u, double v) const;
	virtual void eval_derivs(double u, double v, SPAposition &p, SPAvector &du, SPAvector &dv, SPAvector &duv,
		logical last_u, logical last_v) const;

	virtual int discontinuous_at_u(double ) const {return 0;};
	virtual int discontinuous_at_v(double ) const {return 0;};

protected:
	law * in_law;
	law * in_law_du;
	law * in_law_dv;
	law * in_law_duv;
};

// ****************
// patch_breakpoint
// ****************
class DECL_LAW patch_breakpoint : public ACIS_OBJECT
{
public:
	patch_breakpoint(double u_temp, double v_temp,
		const SPAposition &p_temp, 
		const SPAvector & du_temp, 
		const SPAvector & dv_temp, 
		const SPAvector & duv_temp	
		)
	{
		p = p_temp;
		deriv[0] = du_temp;
		deriv[1] = dv_temp;
		deriv[2] = duv_temp;
		knot[0]=u_temp;
		knot[1]=v_temp;
		next[0]=NULL;
		next[1]=NULL;
		ok[0]=FALSE;
		ok[1]=FALSE;
	}

	SPAposition p;
	SPAvector deriv[3]; // du, dv, duv

	double knot[2]; // knot SPAposition (u,v)

	patch_breakpoint * next[2]; // next_u, next_v

	logical ok[2]; // checked in u, checked in v
};

// *********************
// patch_breakpoint_list
// *********************
// patch_breakpoint_list is used to construct a b-spline patch approximation to a black SPAbox surface.
//
// To use this class, contruct it, optionally add your own knots (once),
// and get the results by calling convert_to_bspline.
// The resulting degree is always 3, and its nonrational.
//
// The caller owns the returned arrays (cp, knots_u, and knots_v),
// and should ACIS_DELETE them when finished with them.
class DECL_LAW patch_breakpoint_list : public ACIS_OBJECT
{
public:
	patch_breakpoint_list(const surface_evaluator * se_temp, 
				double low_u_temp, 
				double high_u_temp, 
				double low_v_temp, 
				double high_v_temp, 
				double surface_fit_tol_temp=SPAresfit,
				knot_removal_strategy knot_strategy_temp = knot_removal_smart
				);

	// Optionally call this ONCE, BEFORE calling convert_to_bspline().
	void add_starting_knots( 
				int initial_num_knots_u, // pass in your own knots, for example at degeneracies
				int initial_num_knots_v,
				const double * initial_knots_u,
				const double * initial_knots_v
				);

	// Refines the patch_breakpoint_list to the desired resolution.
	// Converts the patch_breakpoint_list into useful information for making a bspline.
	// The knots returned have multiplicity 4 at the endpoints.
	// (only 3 are really needed, but some nurb formats use 4.)
	// The caller owns the 3 returned arrays, and should ACIS_DELETE them.
	// Call this ONCE, AFTER optionally calling add_starting_knots().
	void convert_to_bspline( 
				int &num_cp_u,
				int &num_cp_v,
				SPAposition *&cp,
				int &num_knots_u,
				int &num_knots_v,
				double *&knots_u,
				double *&knots_v,
				logical row_major=TRUE,	// ordering of cp array 
										// (TRUE for ag-spline, FALSE for IGES and sw_surface)
				logical approx_ok=FALSE
				);

	virtual ~patch_breakpoint_list();

private:
	void iso_line_refinement();
	void inside_patch_refinement();
	void combine_knots();

	// Refines the Bezier patches to the desired resolution.
	void refine(); 

	logical iso_seg_needs_divide(
						SPAvector cp[4], 
						double low, 
						double high,
						int split_direction, // 0 for u direction, 1 for v direction
						double fixed_knot_value // the constant knot value, in the other direction.
						);
	void subdivide_patches(int move_direction, int split_direction, double mid_knot, patch_breakpoint* pb_starting = NULL);
	logical maybe_subdivide_at_value(patch_breakpoint * pb, 
												  int move_direction,
												  int split_direction);

	logical can_combine(patch_breakpoint * current, 
											 int split_direction,
											 int other_direction);
	void do_combine(patch_breakpoint * current, 
											 int split_direction,
											 int other_direction);

	logical can_remove_double_knot(patch_breakpoint * first_knot, 
													  SPAvector * first_cp, 
													  int cp_offset,
													  int direction);

	void adjust_for_periodic(int move_direction, int periodic_direction);

	const surface_evaluator * se;
	double surface_fit_tol;
	double surface_fit_tol_safe;
	double curve_fit_tol;
	double curve_fit_tol_squared;
	double knot_tolerance;
	double knot_tolerance_squared;

	logical knot_shifting;
	knot_removal_strategy knot_strategy;

	patch_breakpoint * start;
	int breakpoints_u;
	int breakpoints_v;
	double low_u;
	double high_u;
	double low_v;
	double high_v;
	logical bad_surface;
//GSSL VPL Incremental approx project
//flag to control whether to add knots in a direction. if the flag is set, knots are not added.
	logical _approx_u;
	logical _approx_v;
};


#endif /* sw_surface_util_hxx */
