// $Id: sw_curve.hxx,v 1.12 2002/08/09 17:13:30 jeff Exp $
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
//	File:		sw_curve.hxx
//															
/*-----------------------------------------------------------*/

#ifndef sw_curve_hxx
#define sw_curve_hxx


// The sw_curve class represents a generic NURB for use in Space Warping, or other application.
// Users of the Space Warping APIs must convert their NURB to this format.
// The result after space warping is a second NURB of this format,
// which the user must then convert back to their own format.
//
// sw_curve is patterned after the IGES/STEP Rational B-Spline Curve Entity (Type 126).
// There are two constructors, one which uses IGES data, and one with the parts broken up.
//
// An sw_curve must be of degree 1 or greater.


// include files---
#include "dcl_law.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end.

class SPAposition;
class SPAvector;
class bezier_segment;

// STI ROLL
class SizeAccumulator;
// STI ROLL

// **************
// class sw_curve 
// **************
class DECL_LAW sw_curve : public ACIS_OBJECT
{
public:
	sw_curve( // IGES interface
				int K_temp,  // Upper index of sum (num_control_points - 1)
				int degree_temp,  // Degree of basis functions.
				int planar_temp, 
				int closed_temp, 
				int polynomial_temp,
				int periodic_temp,
				double const *IGES_DATA_temp // knot_vector + weights + control_points + min_t + max_t + unit_normal
				);

	sw_curve( // non IGES interface
				int num_control_points_temp,
				int degree_temp, 
				int planar_temp, 
				int closed_temp, 
				int polynomial_temp,
				int periodic_temp,
				double *knot_vector_temp, // num_control_points+degree+1 of these
				double *weights_temp,
				double *control_points_temp, // x,y,z interlaced.
				double min_t_temp, // starting SPAparameter value ( >=T0 )
				double max_t_temp, // ending SPAparameter value ( <=TN ),
				double *unit_normal_temp=NULL // if curve is planar
				);
	~sw_curve();

	// Throws an error is there's a problem.
	// The caller must pre-allocate memory for point and/or derivatives.
	// A NULL value means that the information is not needed.
	void eval(double t_global, 
			SPAposition *point, SPAvector *derivative1=NULL, SPAvector *derivative2=NULL);

	int get_data_size();
	int get_num_control_points() const { return num_control_points; }
	int get_K() const { return num_control_points-1; }
	int get_degree() const { return degree; }
	int get_num_spans() const { return num_spans; }
	int get_num_knots() const { return num_knots; }

	int get_planar() const { return planar; }
	int get_closed() const { return closed; }
	int get_polynomial() const { return polynomial; }
	int get_periodic() const { return periodic; }

	const double * get_IGES_DATA() const { return IGES_DATA; }
	const double * get_knot_vector() const { return knot_ptr; }
	const double * get_weights() const { return weight_ptr; }
	const double * get_control_points() const { return control_point_ptr; }
	double get_min_t() const { return min_t; }
	double get_max_t() const { return max_t; }
	const double * get_unit_normal() const { return unit_normal_ptr; }
	void set_control_point(int i, double * p );
	void get_control_point(int i, double * p );

	sw_curve * copy_curve() const;
	void debug() const; 

	// These last routines are not implemented,
	// but they are required stubs for sw_curve_law.
	void save();
	static sw_curve * restore();
	void eval_curvature(double in_param, double * result);
	double length_param(double base,double length);
	double length(double start,double end);
	double point_perp(const SPAposition &in_point, SPAposition &foot, double  *guess_t=NULL);
	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

protected:
	int num_control_points;
	int degree; // Degree of basis functions.
	int num_spans; // Number of spans = num_control_points-degree
	int num_knots; // Number of knots = num_control_points+degree+1

	int planar; // 1 if the curve is contained within a plane, else 0. (PROP1 in IGES)
	int closed; // 1 if the curve is closed, else 0. (PROP2 in IGES)
	int polynomial; // 1 if the weights are not used, else 0. (PROP3 in IGES)
	int periodic; // 1 if the curve is periodic, else 0. (PROP4 in IGES)


	double * IGES_DATA;
	double * knot_ptr;
	double * weight_ptr;
	double * control_point_ptr;
	double min_t;
	double max_t;
	double * unit_normal_ptr;

	int last_index;

	// cache system.
	void init_caches();
	void check_caches();
	void set_caches();
	void delete_caches();

	// bezier segments cache.
	int b_cache_ready; // 1 if ready, else 0.
	bezier_segment * b_cache; // for point evaluation. there is one of these for each span
	bezier_segment * d1_cache; // for first derivative evaluation. there is one of these for each span
	bezier_segment * d2_cache; // for second derivative evaluation. there is one of these for each span

};


#endif /* sw_curve_hxx */
