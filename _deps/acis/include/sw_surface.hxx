// $Id: sw_surface.hxx,v 1.9 2002/08/09 17:13:30 jeff Exp $
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
//	File:		sw_surface.hxx
//															
/*-----------------------------------------------------------*/

#ifndef sw_surface_hxx
#define sw_surface_hxx

// The sw_surface class represents a generic NURB surface for use in Space Warping.
// Users of the Space Warping APIs must convert their NURB to this format.
// The result after space warping is a second NURB of this format,
// which the user must then convert back to their own format.
//
// sw_surface is patterned after the IGES/STEP Rational B-Spline Surface Entity (Type 128).
// There are two constructors, one which uses IGES data, and one with the parts broken up.
//
// sw_surface depends on the base component for SPAvector types, memory management,
// and error handling.
//

// include files---
#include "dcl_law.h"
#include "position.hxx"
#include "sw_common.hxx"


class DECL_LAW sw_surface : public ACIS_OBJECT
{
public:
	// NOTE: when indexing into the control point or weights double arrays,
	// the data associated with the first sum direction moves most rapidly.
	// This seems non-standard, but its the way IGES does it.
	sw_surface( // IGES interface
				int K1_temp,  // Upper index of first sum (num_control_points1 - 1)
				int K2_temp,  // Upper index of second sum (num_control_points2 - 1)
				int degree1_temp,  // Degree of first set of basis functions.
				int degree2_temp,  // Degree of second set of basis functions.
				int closed1_temp, 
				int closed2_temp, 
				int polynomial_temp,
				int periodic1_temp,
				int periodic2_temp,
				double const *IGES_DATA_temp // knot_vector1 + knot_vector2 + weights + 
										// control_points + min_s + max_s + min_t + max_t
				);

	sw_surface( // non IGES interface
				int num_control_points1_temp,
				int num_control_points2_temp,
				int degree1_temp,  // Degree of first set of basis functions.
				int degree2_temp,  // Degree of second set of basis functions.
				int closed1_temp, 
				int closed2_temp, 
				int polynomial_temp,
				int periodic1_temp,
				int periodic2_temp,
				double *knot_vector1_temp, // num_control_points1+degree1+1 of these
				double *knot_vector2_temp, // num_control_points2+degree2+1 of these
				double *weights_temp,
				SPAposition *control_points_temp, // x,y,z interlaced.
				double min_s_temp, // starting SPAparameter value ( >=S0 )
				double max_s_temp, // ending SPAparameter value ( <=SN ),
				double min_t_temp, // starting SPAparameter value ( >=T0 )
				double max_t_temp  // ending SPAparameter value ( <=TN ),
				);
	~sw_surface();

	// The caller must pre-allocate memory for the point.
	// Throws an error is there's a problem, such as evaluation out of range.
	void eval(double s_global, double t_global, SPAposition *point);

	// The caller must pre-allocate memory for point and/or derivatives.
	// A NULL value means that the information is not needed.
	// Throws an error is there's a problem, such as evaluation out of range.
	void eval_derivatives(
			double s_global, double t_global, 
			SPAposition *point, 
			SPAvector *derivative1_s=NULL, SPAvector *derivative1_t=NULL, 
			SPAvector *derivative2_ss=NULL, SPAvector *derivative2_st=NULL, SPAvector *derivative2_tt=NULL);

	int get_data_size();
	int get_num_control_points1() const { return num_control_points1; }
	int get_num_control_points2() const { return num_control_points2; }
	int get_K1() const { return num_control_points1-1; }
	int get_K2() const { return num_control_points2-1; }
	int get_degree1() const { return degree1; }
	int get_degree2() const { return degree2; }
	int get_num_spans1() const { return num_spans1; }
	int get_num_spans2() const { return num_spans2; }
	int get_num_knots1() const { return num_knots1; }
	int get_num_knots2() const { return num_knots2; }

	int get_closed1() const { return closed1; }
	int get_closed2() const { return closed2; }
	int get_polynomial() const { return polynomial; }
	int get_periodic1() const { return periodic1; }
	int get_periodic2() const { return periodic2; }

	const double * get_IGES_DATA() const { return IGES_DATA; }
	const double * get_knot_vector1() const { return knot_ptr1; }
	const double * get_knot_vector2() const { return knot_ptr2; }
	const double * get_weights() const { return weight_ptr; }
	const SPAposition * get_control_points() const { return control_point_ptr; }
	double get_min_s() const { return min_s; }
	double get_max_s() const { return max_s; }
	double get_min_t() const { return min_t; }
	double get_max_t() const { return max_t; }
	void set_control_point(int index1, int index2, SPAposition & p );
	void get_control_point(int index1, int index2, SPAposition & p );

	sw_surface * copy_surface() const;
	void save();
	static sw_surface * restore();
	void debug() const;

protected:
	int num_control_points1;
	int num_control_points2;
	int degree1; // Degree of basis functions.
	int degree2; // Degree of basis functions.
	int order1; // Order of basis functions (degree+1).
	int order2; // Order of basis functions (degree+1).
	int num_spans1; // Number of spans1 = num_control_points1-degree1
	int num_spans2; // Number of spans2 = num_control_points2-degree2
	int num_knots1; // Number of knots1 = num_control_points1+degree1+1
	int num_knots2; // Number of knots2 = num_control_points2+degree2+1

	int closed1; // 1 if the surface is closed in s direction, else 0.
	int closed2; // 1 if the surface is closed in t direction, else 0.
	int polynomial; // 1 if the weights are not used, else 0.
	int periodic1; // 1 if the surface is periodic in s direction, else 0.
	int periodic2; // 1 if the surface is periodic in t direction, else 0.


	double * IGES_DATA;
	double * knot_ptr1;
	double * knot_ptr2;
	double * weight_ptr;
	SPAposition * control_point_ptr;
	double min_s;
	double max_s;
	double min_t;
	double max_t;

	SPAvector * temp_control_points; // temporary memory the size of order1*order2.
	double * temp_weights; // temporary memory the size of order1*order2.
	int last_s_index;
	int last_t_index;
	logical power_on; // in the degree 3x3 case, we use power basis caches, which are faster.

	void finish_init();

	// sets the bezier cache for the specified patch.
	void bspline_to_bezier_patch(int i_span, int j_span);
	void bezier_patch_to_power_patch(int i_span, int j_span); 

	// sets the bezier cache derivatives for the specified patch.
	void set_derivative_b_caches(int i_span, int j_span); 
	void set_derivative_p_caches(int i_span, int j_span); 

	// cache system.
	void init_caches();
	void check_caches(int level);
	void set_caches(int level);
	void delete_caches();

	// bezier patch cache.
	int cache_level; // -1 if not ready, 0 for points, 1 for 1st derivs, 2 for second.
	bezier_patch * b_cache; // for point evaluation. there is one of these for each span
	bezier_patch * d_s_cache; // for first derivative evaluation. (one for each span)
	bezier_patch * d_t_cache; // for first derivative evaluation. (one for each span)
	bezier_patch * d_ss_cache; // for second derivative evaluation. (one for each span)
	bezier_patch * d_st_cache; // for second derivative evaluation. (one for each span)
	bezier_patch * d_tt_cache; // for second derivative evaluation. (one for each span)

	// power patch cache, only for 4X4 patches.
	power_patch33 * p_cache; // for point evaluation. (one for each span)
	power_patch23 * p_s_cache; // for first derivative evaluation. (one for each span)
	power_patch32 * p_t_cache; // for first derivative evaluation. (one for each span)
	power_patch13 * p_ss_cache; // for second derivative evaluation. (one for each span)
	power_patch22 * p_st_cache; // for second derivative evaluation. (one for each span)
	power_patch31 * p_tt_cache; // for second derivative evaluation. (one for each span)
};


#endif /* sw_surface_hxx */
