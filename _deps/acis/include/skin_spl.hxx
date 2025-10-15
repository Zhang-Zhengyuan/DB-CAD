/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// This surface class defines a skin surface between a list of curves.
//
//	The surface parametrization is: u-direction corresponds to the curves to
//	be skinned and v-direction corresponds to the cubic bezier between the
//	skin-curves.
//
//	The input to this surface class is :
//
//	a. The curves to be skinned.  All the curves are reparametrized to lie
//		in ( 0.0 - 1.0 ) range.
//	b. Optional tangents( Magnitudes the curves have to match on the ends )
//		in u-direction.
//	c. Optional surfaces on which the curves lie.  This will determine the
//		tangent directions in v.
//
//	The evaluation process:
//
//	Step 1:
//		If any matching tangent magnitudes are given then the
//		section curves( curves to be skinned ) are reparametrized as
//		below:
//
//		SPAparameter t be the SPAparameter on the original curve.
//		Than SPAparameter u on the skin surface is determined such that
//		The u-partial at the ends on the skin surface is equal to the
//		matching tangent magnitude.
//
//		t = f(u), where
//		f(u) = ts * H03(u) + m0 * H13(u) + m1 * H23(u) + te * H33(u)
//
//		The H's are the hermite polynomials and the ts and te are the
//		start and end SPAparameter values of the original curves to be
//		skinned, which here are ( 0.0 - 1.0 ) respectively.
//
//		So, ds/du on the ends will be = dc/du
//		dc/du = dc/dt * dt/du
//
//		dt/du on the ends will be m0 and m1.  So by choosing the values
//		m0 and m1 such that the dc/du on left surface is equal to dc/du on
//		the right surface( provided that the curves are G1 ), we get a
//		C1 continuous surface even when skinning G1 continuous curves.
//
//	STEP 2:
//		Determining the tangent directions for the v.
//		These are determined on the fly by fitting a circle through the
//		points corresponding to the same u-value on the adjacent section
//		curves to the left and right.  The scheme followed is similiar to
//		the way bessels tangents are computed.
//		In the case, of two section curves, the circle radius is chosen as
//		Infinity.
//		Also, if the surfaces are given for any section.  The tangent
//		direction in v when on that curve is given as a cross product of
//		surface normal and the section curve tangent at that point.  The
//		direction also has an optional scalar value, which can be applied to
//		it.  The surface is called as a loft surface when such surfaces are
///		provided.
//
//	STEP 3:
//		Now the skin/loft surface is defined as hermite interpolants between
//		sections, which join each other C1 continuosly.  So, is one wants to
//		evaluate the surface S(u,v) at a particular "v" SPAparameter, the first
//		step is to find the segment to which this SPAparameter corresponds too.
//		Than a local paramter vi is computed which ranges from (0.0-1.0)
//		and the section curves ci and ci+1, and the v_tangents ti and ti+1
//		are also obtained, then the surface	is defined as:
//
//			S(u,v) = ci(u) * H03(vi) + ti * H13(vi) + ti+1 * H23(vi) +
//							ci+1 * H33( vi )
//		The parameteric derivates of this surface are obtained by differen
//			tiating the above equation algebriacly.
//
#if !defined( SKIN_SPL_SUR_CLASS )
#define SKIN_SPL_SUR_CLASS			// STIPORT pes _MSC_VER does not like trailing ;
#include "dcl_kern.h"
#include "logical.h"
#include "debugmsc.hxx"
/**
* @file skin_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

class curve;
class intcurve;
class SPApar_pos;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class law;
class pcurve;
class SPAbox;
class SPApar_box;
class skin_spl_sur;

// STI ROLL
class SizeAccumulator;
// STI ROLL

#include "spldef.hxx"
#include "lists.hxx"
#include "vlists.hxx"

/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN bs3_surface make_skin_approx( skin_spl_sur const &,
//	SPAinterval const&,  SPAinterval const&,  double const&,  double &,
//	logical  = FALSE, 
//	VOID_LIST& = *(VOID_LIST*)NULL_REF, 
//	SPApar_box& = *(SPApar_box*) NULL_REF, 
//	SPApar_box& = *(SPApar_box*) NULL_REF); 
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN double surface_error( skin_spl_sur const& origin_surf,
	bs3_surface& approx_surf, int nu = 0, double *u_knots = NULL, int nv = 0,
	double *v_knots = NULL);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical regen_approx_surf(skin_spl_sur &, double requested_tol,
	SPApar_box* = NULL );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN bs3_surface make_skin_approx_old( skin_spl_sur const &,
	SPAinterval const&,  SPAinterval const&,  double const&,  double &  );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void get_preferred_knots( skin_spl_sur const &this_surf,
	SPAinterval const &range_u, SPAinterval const &range_v, int& number_of_u,
	double*& u_cuts, int& number_of_v, double*& v_cuts );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN double surface_tolerance( skin_spl_sur const& origin_surf,
	bs3_surface &approx_surf  );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_skin_spl_sur();
#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif

// STI let (010813):  Implementation of the bernstein_coefficients structure.
// It is more efficient to pass arguments as a single struct than 44 doubles.
/*
// tbrv
*/
/**
 * @nodoc
 */
struct bernstein_coefficients
{
	double H[4];
	double Hdu[4];
	double Hduu[4];
	double Hduuu[4];
	double Hdv[4];
	double Hdvv[4];
	double Hdvvv[4];
	double Hduv[4];
	double Hduuv[4];
	double Hduvv[4];
	double Hduuuv[4];
};
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef struct bernstein_coefficients bernstein_coefficients;
// STI let: end

// Implementation of the skin_data class. This class has been introduced for conveniently storing
// and manipulating surface information inorder to estimate the minimum radius of curvature of the
// skin surface quickly and for estimating the best tangent factor scale quickly. The skin surface
// now has a pointer to this class as a protected data member. Since the data need not be stored to
// disk this addition of a data member will not cause any change to the save/restore mechanism of
// the skin surface.
/*
// tbrv
*/
/**
 * @nodoc
 */
class skin_data : public ACIS_OBJECT {
	int usample;
	int vsample;
	int no_crv;
	logical cache;

	SPAvector **pos_array;
	SPAvector **tan_array;
	double **v_knots_array;
	double *base_tanfacs;
	logical *tanfac_influence;

	SPAvector **du1_array;
	SPAvector **du2_array;
	SPAvector **du3_array;

	SPAvector **dv1_array;
	SPAvector **dv2_array;
	SPAvector **dv3_array;

	SPAvector **duu1_array;
	SPAvector **duu2_array;
	SPAvector **duu3_array;

	SPAvector **duv1_array;
	SPAvector **duv2_array;
	SPAvector **duv3_array;

	SPAvector **dvv1_array;
	SPAvector **dvv2_array;
	SPAvector **dvv3_array;

public:
	skin_data(VOID_LIST &curves, int no_curves);
	~skin_data();

	int size();

	double *uparam;

	int usamp(){ return usample; }
	int vsamp(){ return vsample; }
	void set_deriv_data(int i, int j, int iter,
						SPAvector &du1, SPAvector &du2, SPAvector &du3,
						SPAvector &dv1, SPAvector &dv2, SPAvector &dv3,
						SPAvector &duu1, SPAvector &duu2, SPAvector &duu3,
						SPAvector &duv1, SPAvector &duv2, SPAvector &duv3,
						SPAvector &dvv1, SPAvector &dvv2, SPAvector &dvv3 );
	void get_deriv_data(int i, int j, int iter,
						SPAvector &du1, SPAvector &du2, SPAvector &du3,
						SPAvector &dv1, SPAvector &dv2, SPAvector &dv3,
						SPAvector &duu1, SPAvector &duu2, SPAvector &duu3,
						SPAvector &duv1, SPAvector &duv2, SPAvector &duv3,
						SPAvector &dvv1, SPAvector &dvv2, SPAvector &dvv3 );
	void set_knot_data(int i, double v[]){ for(int j = 0; j < no_crv; j++)  v_knots_array[i][j] = v[j]; }
	double get_knot_data(int i, int j){ return v_knots_array[i][j]; }
	void set_tanfacs(double fac[]);
	double get_tanfacs(int j){ return base_tanfacs[j]; }
	void set_pos_data(int i, int j, SPAvector &c0, SPAvector &n0, SPAvector &c1, SPAvector &n1);
	void get_pos_data(int i, int j, SPAvector &c0, SPAvector &n0, SPAvector &c1, SPAvector &n1);
	void set_cached(logical isCached) { cache = isCached; }
	logical cached(){ return cache; }
	void set_tanfac_influence(int j, logical influence){ tanfac_influence[j] = influence; }
	int get_tanfac_influence(int j){ return  tanfac_influence[j]; }
};

DECL_KERN bs3_surface make_skin_approx(
	skin_spl_sur const &,
	SPAinterval const&,			// u SPAparameter range
	SPAinterval const&,			// v SPAparameter range
	double const&,			// tol requested
	double &, 				// tol returned.
	logical self_int_test = FALSE,							// logical
	VOID_LIST& bad_uvs = *(VOID_LIST*)NULL_REF,				// bad_uvs
	SPApar_box& exclude_region = *(SPApar_box*)NULL_REF,	// exclude region
	SPApar_box& extension_box = *(SPApar_box*)NULL_REF);	// extension_box

/**
 * Defines a skin surface between a list of curves.
 * <br>
 * <b>Role</b>: Surface Parameterization
 * <br>
 * The surface parameterization is the u-direction corresponds to the parameterization
 * of the curves to be skinned and the v-direction corresponds to the cubic Bezier
 * between the skin-curves.
 * <br>
 * The input to this surface class are the curves to be skinned (all the curves
 * are reparameterized to lie in [0.0 - 1.0] range), optional tangents (the magnitude
 * of the curves' tangents have to match on the ends) in u-direction, and the optional
 * surfaces on which the curves lie. If surfaces containing the curves are provided,
 * these determine the tangent directions in v.
 * <br>
 * <b>Evaluation Process</b><br>
 * The evaluation process is a three-step process, as described below.<br>
 * <i>Step 1</i><br>
 * If any matching tangent magnitudes are given, the section curves (curves to be
 * skinned) are reparameterized as follows: parameter t is the parameter on the
 * original curve. Parameter u on the skin surface is determined such that the
 * u-partial at each end of the skin surface is equal to the matching tangent magnitude.
 * <br>
 * t = f(u) =	ts*H0(u) + m0*H1(u) + m1*H2(u) + te*H3(u)
 * <br>
 * In the above expression, Hn are the cubic Hermite polynomials and ts and te are
 * the start and end parameter values of the original curves to be skinned, which
 * here are 0 and 1, respectively.
 * <br>
 * So, ds/du on the ends are:
 * <br>
 * dc/du = dc/dt * dt/du
 * <br>
 * where the dt/du values on the ends are m0 and m1.
 * So by choosing the values m0 and m1 such that the dc/du on the left surface is
 * equal to dc/du on the right surface (provided that the curves are G1), a C1
 * continuous surface is achieved even when skinning G1 continuous curves.
 * <br>
 * <i>Step 2</i><br>
 * The tangent directions for the v are determined by fitting a circle through the
 * points corresponding to the same u-value on the adjacent section curves to the
 * left and right. The scheme followed is similar to the way Bessel tangents are
 * computed. If there are only two section curves, the circle radius is chosen to be
 * infinity. If the surfaces are given for any section, the tangent direction in v
 * when on that curve is obtained by the cross product of surface normal and the
 * section curve tangent at that point. The direction also has an optional scalar
 * value that can be applied. The surface is called a loft surface when such a
 * surface is provided.
 * <i>Step 3</i><br>
 * Now the skin/loft surface is defined using cubic Hermite interpolants between
 * sections that join each other C1 continuously. To evaluate the surface s(u,v)
 * at a particular v-parameter, the first step is to find the segment to which
 * this parameter corresponds. Then a local parameter vi is computed, which ranges
 * from 0 to 1. The section curves ci and ci+1, and the v-tangents ti and ti+1 are
 * also obtained. The surface is defined as:
 * <br>
 * s(u,v) =	ci(u)*H0(vi) + ti*H1(vi) + ti+1*H2(vi) + ci+1*H3(vi)
 * </br>
 * The parametric derivatives of this surface are obtained by differentiating the
 * above equation algebraically.
 */

class DECL_KERN skin_spl_sur: public spl_sur {

// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef skin_spl_sur_FRIENDS
skin_spl_sur_FRIENDS
#endif

protected:

	// Declare the function make_skin_approx as a friend of the skin_spl_sur
	// class, as this will enable this function to use the members of this
	// class freely.

	friend DECL_KERN bs3_surface make_skin_approx(
							skin_spl_sur const &,
							SPAinterval const&,        // u SPAparameter range
							SPAinterval const&,        // v SPAparameter range
							double const&,			// tol requested
							double &, 				// tol returned.
							logical self_int_test,							// logical
							VOID_LIST& bad_uvs,				// bad_uvs  
							SPApar_box& exclude_region,	// exclude region
							SPApar_box& extension_box);	// extension_box
						

	friend DECL_KERN bs3_surface make_skin_approx_old(
							skin_spl_sur const &,
							SPAinterval const&,        // u SPAparameter range
							SPAinterval const&,        // v SPAparameter range
							double const&,			// tol requested
							double & 				// tol returned.
						);

    friend DECL_KERN void get_preferred_knots(
                             skin_spl_sur const	&this_surf,
                             SPAinterval const		&range_u,
                             SPAinterval const		&range_v,
                             int& number_of_u,
                             double*& u_cuts,
                             int& number_of_v,
                             double*& v_cuts
                             );

	friend DECL_KERN double surface_tolerance(
            skin_spl_sur  const& origin_surf,
            bs3_surface   &approx_surf
			);

	friend DECL_KERN double surface_error(
				skin_spl_sur const& origin_surf,
				bs3_surface& approx_surf,
				int nu ,
				double *u_knots ,
				int nv ,
				double *v_knots );

	// STI let (r4223): Add a new, global, friend function
	// Regenerate the approximating surface for a skin_spl_sur
	// This changes the sur_data and fitol_data fields of the skin_spl_sur.

	friend DECL_KERN logical regen_approx_surf( skin_spl_sur &,
												double requested_tol,
												SPApar_box *
											);
	// STI let: end

	// Make or remake the approximating surface.  This function, which is a
    // virtual function of spl_sur, is implemented using make_skin_approx.
	// The function stores the approximating surface and the actual fit error
    // that was achieved in the spl_sur, overriding the declared const-ness
    // of the function to do this.

    virtual void make_approx(
							 double fit,
							 const spline& spl = *(spline*) NULL_REF,
						     logical force = FALSE
							 ) const;

	void compute_bernstein_polynomials (
				logical first_required,
				logical second_required,
				logical third_required,
				int index,
				SPApar_pos const 	&uv,
				double v_knot_deriv_0[4],
				double v_knot_deriv_1[4],
				logical arc_length_activated,
				bernstein_coefficients*
				) const;

	void calculate_iso_data(
			evaluate_surface_quadrant quadrant,
			SPApar_pos const 	&uv,
			logical first_required,
			logical second_required,
			logical third_required,
			SPAvector &c0, SPAvector &c0d, SPAvector &c0dd, SPAvector &c0ddd,
			SPAvector &c1, SPAvector &c1d, SPAvector &c1dd, SPAvector &c1ddd,
			SPAvector &n0, SPAvector &n0d, SPAvector &n0dd, SPAvector &n0ddd,
			SPAvector &n1, SPAvector &n1d, SPAvector &n1dd, SPAvector &n1ddd,
			int &index
		) const;

	void calculate_arcwise_data(
			evaluate_surface_quadrant quadrant,
			SPApar_pos const 	&uv,
			logical first_required,
			logical second_required,
			logical third_required,
			double v_knot_deriv_0[4], double v_knot_deriv_1[4],
			SPAvector &c0, SPAvector &c0d, SPAvector &c0dd, SPAvector &c0ddd,
			SPAvector &c1, SPAvector &c1d, SPAvector &c1dd, SPAvector &c1ddd,
			SPAvector &n0, SPAvector &n0d, SPAvector &n0dd, SPAvector &n0ddd,
			SPAvector &n1, SPAvector &n1d, SPAvector &n1dd, SPAvector &n1ddd,
			int &index
		) const;

	void arclength_index_start(
			evaluate_surface_quadrant quadrant,
			SPApar_pos const 	&uv,
			logical first_required,
			logical second_required,
			logical third_required,
			double v_knot_deriv_0[4], double v_knot_deriv_1[4],
			double v_knot_deriv_2[4], double v_knot_deriv_3[4],
			SPAvector normal_derivs0[4], SPAvector normal_derivs1[4],
			surface *surf0, surface *surf1,
			law *law0, law *law1,
			double fac0, double fac1,
			SPAvector &c0, SPAvector &c0d, SPAvector &c0dd, SPAvector &c0ddd,
			SPAvector &c1, SPAvector &c1d, SPAvector &c1dd, SPAvector &c1ddd,
			SPAvector &n0, SPAvector &n0d, SPAvector &n0dd, SPAvector &n0ddd,
			SPAvector &n1, SPAvector &n1d, SPAvector &n1dd, SPAvector &n1ddd,
			int &index
		) const;

	void arclength_index_end(
			evaluate_surface_quadrant quadrant,
			SPApar_pos const 	&uv,
			logical first_required,
			logical second_required,
			logical third_required,
			double v_knot_deriv_0[4], double v_knot_deriv_1[4],
			double v_knot_deriv_2[4], double v_knot_deriv_3[4],
			SPAvector normal_derivs0[4], SPAvector normal_derivs1[4],
			surface *surf0, surface *surf1,
			law *law0, law *law1,
			double fac0, double fac1,
			SPAvector &c0, SPAvector &c0d, SPAvector &c0dd, SPAvector &c0ddd,
			SPAvector &c1, SPAvector &c1d, SPAvector &c1dd, SPAvector &c1ddd,
			SPAvector &n0, SPAvector &n0d, SPAvector &n0dd, SPAvector &n0ddd,
			SPAvector &n1, SPAvector &n1d, SPAvector &n1dd, SPAvector &n1ddd,
			int &index
		) const;

	void arclength_index_general(
			evaluate_surface_quadrant quadrant,
			SPApar_pos const 	&uv,
			logical first_required,
			logical second_required,
			logical third_required,
			double v_knot_deriv_0[4], double v_knot_deriv_1[4],
			double v_knot_deriv_2[4], double v_knot_deriv_3[4],
			SPAvector normal_derivs0[4], SPAvector normal_derivs1[4],
			surface *surf0, surface *surf1,
			law *law0, law *law1,
			double fac0, double fac1,
			SPAvector &c0, SPAvector &c0d, SPAvector &c0dd, SPAvector &c0ddd,
			SPAvector &c1, SPAvector &c1d, SPAvector &c1dd, SPAvector &c1ddd,
			SPAvector &n0, SPAvector &n0d, SPAvector &n0dd, SPAvector &n0ddd,
			SPAvector &n1, SPAvector &n1d, SPAvector &n1dd, SPAvector &n1ddd,
			int &index
		) const;


	// For internal use only.
/**
 * @nodoc
 */
		virtual logical validate_u_guess( const SPAparameter &u_guess, 
									SPAparameter &valid_u_guess ) const;

private:

	void constrain_surface_data2(
			surface *surf,
			SPApar_pos const &uv,
			logical first_required,
			logical second_required,
			logical third_required,
			SPAposition c,
			curve *cur,
			SPAvector &cd,
			SPAvector &n,
			SPAvector &nd,
			SPAvector &ndd,
			SPAvector &nddd,
			double fac
		) const;


	// Added a new private method for the hp700 compiler.
	// This should not be called by anyone.
	// It is simply the second half of the above function.

	void constrain_surface_data(
        	surface *surf0,
        	surface *surf1,
        	law *law0,
        	law  *law1,
        	SPApar_pos const &uv,
        	logical first_required,
        	logical second_required,
        	logical third_required,
        	SPAposition cx,
        	SPAposition cy,
        	curve * cur0,
        	curve * cur1,
        	SPAvector &c0d,
        	SPAvector &n0,
        	SPAvector &n0d,
        	SPAvector &n0dd,
        	SPAvector &n0ddd,
        	double fac0,
        	double fac1,
        	SPAvector &c1d,
        	SPAvector &n1,
        	SPAvector &n1d,
        	SPAvector &n1dd,
        	SPAvector &n1ddd
		) const;

	// This function is used to cache surface data for a base
	// set of tangent factors ( in the range [-1,1] ). Then,
	// we repeatedly use this information to compute the optimal scaling
	// factor for the tangent factors. The reason this is a method on
	// the skin_spl_sur class is because it needs access to methods of
	// this class. It is protected because nobody should need to call this
	// function directly.

	void cache_data();

	// This is a special function that computes the radius of curvature of skin surfaces.
	// It should not be used in general and is a wrapper around the global function used
	// to boost performance of the tan_fac estimation algorithms.

	void eval_skin_curv(
			int i,
			int j,
			int iter,
			double ratio,
			logical influence1,
			logical influence2,
			SPAunit_vector &dir1,
			double &curv1,
			SPAunit_vector &dir2,
			double &curv2 );

protected:

	void sg_recalculate_vknots_and_dervs(
			double u_par,
			evaluate_surface_quadrant surface_quadrant,
			logical first_required,
			logical second_required,
			logical third_required,
			double *v_par_arr1,
			double *v_par_arr2,
			double *v_par_arr3
		) const;

	// Use finite differnce to calculate the derivative of the surface normal
	// with respect to the coedge curve.
	void sg_calculate_surface_normal_dervs(
 			SPApar_pos const		&uv,
			logical				first_required,
			logical				second_required,
			logical				third_required,
			SPAposition			center_point,
			curve				*cur,
			surface				*surf,
			SPAvector				*normal_derivs

		) const;

	// Find the SPAposition and first and second  derivatives of the
	// 2.1 skin surface at a given point.

	void eval_2p1_skin(
			SPApar_pos const &uv,
			SPAposition &pos,
			SPAvector *dpos,	// first derivatives - array of
							// length 2 in order xu, xv
			SPAvector *ddpos	// second derivatives - array of
							// length 3 in order xuu, xuv, xvv
		) const;

	void eval_skin(
			SPApar_pos const &uv,
			SPAposition &pos,
			SPAvector *dpos,	// first derivatives - array of
							// length 2 in order xu, xv
			SPAvector *ddpos,	// second derivatives - array of
							// length 3 in order xuu, xuv, xvv
			SPAvector *dddpos,	// second derivatives - array of
							// length 4 in order xuuu, xuuv, xuvv, xvvv
			evaluate_surface_quadrant quadrant
		) const;

	// Protected member functions of this class , as this is called
	// only by the class members.
	void remap_and_eval(
			evaluate_surface_quadrant quadrant,
			int,			// section number.
			curve *, 		// Curve to evaluated.
			double,			// u SPAparameter
			SPAposition &, 	// SPAposition returned.
			SPAvector &, 		// First derivative returned
			SPAvector &, 		// Second derivative returned
			SPAvector & = *( SPAvector *)NULL_REF, // 3'rd derivative , Used for
										// computing second partials on a
			SPAvector & = *( SPAvector *)NULL_REF	// 4'rd derivative , Used for
										// computing second partials on a
										// loft surface.
										// loft surface.
		) const;

	// Calculate and add the dicontinuities from the generating curves
	// to the skin surface
	void curve_add_discontinuities();

	// Number of curves to be skinned and some mapping information.
	int no_crv;
	double *deriv_cache;
	double *matching_derivs;

	// Arc length parameterization option flag 1=arc length parameterization
	logical arc_length_param;

	// Loft direction option flag 1=perpendicular direction.
	logical perpendicular_option;

	// This is a flag to indicate we have a old skin surface.
	logical skin_2p1_surface;

	// Array of curves to be skinned saved in an entity_list
	VOID_LIST curves;


	// Array of crossboundary tangents, these may be NULL, then
	// the tangents at each point are calculated on the fly.
	SPAvector *tangents;

	// Array of pointers to surface.  The curves which are skinned lie
	// on these surfaces respectively and these are used to obtain cross
	// boundary tangents. This constitutes the lofting.  If this pointer
	// is NULL, then the tangents are calculated on the fly.
	// Also, a array of doubles is given which is a constant to be multiplied
	// to the crossboundary tangents.
	surface **surfaces;
	double *tan_factors;
	law **laws;
	pcurve **pcurves;

	// Need to cache data for computing the optimal tangent factors for the skin surface.
	skin_data *surface_data;

	int no_path_crv;

	// Array of curves to that defines the path saved in an entity_list
	VOID_LIST path_curves;

	// Array of doubles indicating the knot values at each section that is
	// being interpolated.
	double *v_knots;

	// Default constructor, used in to create a empty class, so that
	// the copy function can use it.
	skin_spl_sur();

	// Initialize the member data
	void initialize();

	// Calculate the discontinuity information from the defining curves.
    virtual	void calculate_disc_info();

private:

	// Construct skin surface from the given section curves, and optional
	// Maching tangents. (Example Calling sequence in sg_skin/).
	skin_spl_sur(
		    logical,        // arc length option
			int,			// Number of curves in the array
			curve **,		// Array[no] of curves
			double *,		// Array[no] of knots values ( increasing order ).
			double *, 		// Array[no] of Matching Tangents on left.
			double *, 		// Array[no] of Matching Tangents on Right.
			SPAvector * = (SPAvector *)NULL,
							// Array of tangent directions for each curve,
							// may be NULL.
			closed_forms = OPEN,
							// Flag for periodicity of surface in u
			closed_forms = OPEN,
							// Flag for periodicity of surface in v
            logical      = TRUE
                            // Flag allowing tolerance to be less than requested
		);

	// Construct a loft surface from the given section curves and the
	// corresponding surfaces on which the sections lie and optional
	// tangent factors that should be applied to cross boundary tangents.
	// Note that the surfaces passed in are owned by the skin_surface, so
	// user should pass in a copy and not worry about deleting it.
	skin_spl_sur(
		    logical,        // arc length option
			logical,		// Perpendicular diection option
			int,			// Number of section curves to skin.
			curve **,		// Array[no] of curves.
			double *,		// Array[no] of knots values.
			double *,		// Array[no] of matching tangents on left.
			double *,		// Array[no] of matching tangents on right.
			surface **,		// Array[no] of surfaces, indivdual entries may be NULL.
			double *,		// Array of crossboundary tangent factors.
			law **,			// Array[no] of laws, indivdual entries may be NULL.
			closed_forms = OPEN,
							// Flag for periodicity of surface in u
			closed_forms = OPEN,
							// Flag for periodicity of surface in v
            logical      = TRUE
                            // Flag allowing tolerance to be less than requested
		);

public:
/**
 * Constructs a skin surface from the given section curves and the optional matching tangents.
 * @param length
 * arc length option
 * @param cur
 * number of curves
 * @param arrcur
 * array of curves
 * @param arrknot
 * aray of knot values
 * @param left
 * array of left tangents
 * @param right
 * array of right tangents
 * @param dir
 * array of tangent dirs
 * @param flagu
 * flag for periodicity of surface in u
 * @param flagv
 * flag for periodicity of surface in v
 */
    static 	spl_sur *make_skin_spl_sur(
		    logical length,        // arc length option
			int cur,			// Number of curves in the array
			curve ** arrcur,		// Array[no] of curves
			double * arrknot,		// Array[no] of knots values ( increasing order ).
			double * left, 		// Array[no] of Matching Tangents on left.
			double * right, 		// Array[no] of Matching Tangents on Right.
			SPAvector * dir= (SPAvector *)NULL,
							// Array of tangent directions for each curve,
							// may be NULL.
			closed_forms flagu = OPEN,
							// Flag for periodicity of surface in u
			closed_forms flagv = OPEN
							// Flag for periodicity of surface in v
		);

/**
 * Constructs a loft surface from the given section curves and the corresponding surfaces on which the sections lie and the optional tangent factors that should be applied to cross-boundary tangents.
 * <br>
 * <b>Role</b>: The surfaces are passed in are owned by the skin_surface, so the
 * user should pass in a copy. All arrays are the size of int, the number of sections.
 * @param arc
 * arc-length flag
 * @param perp
 * perpendicular flag
 * @param section
 * number of section curves
 * @param cur
 * array of curves
 * @param arrknot
 * array of knot values
 * @param left
 * array of left tangents
 * @param right
 * array of right tangents
 * @param surf
 * array of surfaces
 * @param fac
 * crossboundary tangents array
 * @param arrlaw
 * array of laws
 * @param flagu
 * flag for periodicity of surface in u
 * @param flagv
 * flag for periodicity of surface in v
 */
    static 	spl_sur *make_skin_spl_sur(
		    logical arc,        // arc length option
			logical perp,		// Perpendicular diection option
			int section,			// Number of section curves to skin.
			curve ** cur,		// Array[no] of curves.
			double * arrknot,		// Array[no] of knots values.
			double * left,		// Array[no] of matching tangents on left.
			double * right,		// Array[no] of matching tangents on right.
			surface ** surf,		// Array[no] of surfaces, indivdual entries may be NULL.
			double * fac,		// Array of crossboundary tangent factors.
			law ** arrlaw,			// Array[no] of laws, indivdual entries may be NULL.
			closed_forms flagu = OPEN,
							// Flag for periodicity of surface in u
			closed_forms flagv = OPEN
							// Flag for periodicity of surface in v
		);


   // Copy constructor.
/*
// tbrv
*/
/**
 * @nodoc
 */
	void deep_copy_elements_skin( const skin_spl_sur& old , pointer_map * pm = NULL);

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * @param surf
 * surface to copy
 */
	skin_spl_sur( const skin_spl_sur& surf);
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br>
 * <b>Role</b>: Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br>
 * In a deep copy, all the information about the copied item is self-contained
 * in a new memory block. By comparison, a shallow copy stores only the first
 * instance of the item in memory, and increments the reference count for each copy.
 * <br>
 * The pointer_map keeps a list of all pointers in the original object that have
 * already been deep copied. For example, a deep_copy of a complex model results
 * in self contained data, but identical sub-parts within the model are allowed
 * to share a single set of data.
 * @param mp
 * list of items within the entity that are already deep copied
 */
   virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

/**
 * Calculates the derivatives within the given parameter box.
 * @param area
 * area for deriv
 */
   int accurate_derivs(
               SPApar_box const &area = *(SPApar_box *)NULL_REF
                                   // Defaults to the whole surface
           ) const;

   // Member function to add the Path data to the skin_spl_sur object.
/**
 * Adds the path data to the skin_spl_sur object.
 * @param no_curves
 * number of curves in path
 * @param curves
 * curve paths to add
 */
   void add_path_data( int no_curves, curve ** curves );

/**
 * @nodoc
 */
   logical is_arc_length_param() const {return arc_length_param;};

/**
 * @nodoc
 */
   logical is_perpendicular() const {return perpendicular_option;}; 


protected:
	// Destructor needs to delete the spine curve.

	virtual ~skin_spl_sur();

	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const; 


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;


	// Transform this skin by the given SPAtransf.

	virtual void operator*=( SPAtransf const &	);


    // Parameter shift: adjust the spline surface to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic surface by
    // integral multiples of the period.

    virtual void shift_u( double );
    virtual void shift_v( double );

	// Divide a surface into two pieces at a given SPAparameter value.
	// If the split is at the end of the SPAparameter range, the spl_sur
	// is just returned as the appropriate half (in increasing
	// SPAparameter order), and the other is NULL. Otherwise a new spl_sur
	// is used for one part, and the old one is modified for the other.

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);


	// Concatenate the contents of two surfaces into one. The surfaces
	// are guaranteed to be the same base or derived type, and to have
	// contiguous SPAparameter ranges ("this" is the beginning part of
	// the combined surface (i.e. lower SPAparameter values), the
	// argument gives the end part).

	// virtual void append_u( spl_sur & );
	// virtual void append_v( spl_sur & );


	// Geometric evaluation.

	// Find the SPAparameter values of a point on the surface

	virtual SPApar_pos param(
				 SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point.

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;


   // The evaluate() function calculates derivatives, of any order
   // up to the number requested, and stores them in vectors provided
   // by the user. It returns the number it was able to calculate;
   // this will be equal to the number requested in all but the most
   // exceptional circumstances. A certain number will be evaluated
   // directly and (more or less) accurately; higher derivatives will
   // be automatically calculated by finite differencing; the accuracy
   // of these decreases with the order of the derivative, as the
   // cost increases.

   virtual int evaluate(
               SPApar_pos const &,    // Parameter
               SPAposition &,         // Point on surface at given SPAparameter
               SPAvector ** = NULL,   // Array of pointers to arrays of
                                   // vectors, of size nd. Any of the
                                   // pointers may be null, in which
                                   // case the corresponding derivatives
                                   // will not be returned. Otherwise
                                   // they must point to arrays long
                                   // enough for all the derivatives of
                                   // that order - i.e. 2 for the first
                                   // derivatives, 3 for the second, etc.
               int = 0,            // Number of derivatives required (nd)
               evaluate_surface_quadrant = evaluate_surface_unknown
                                   // the evaluation location - above,
                                   // below for each SPAparameter direction,
                                   // or don't care.
           ) const;

// STI let (12/98) (slic025): Removed u_ and v_param_line methods
// The methods in the base class are preferrable.

	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:
/**
 * Returns the ID for the skin_spl_sur list.
 */
	static int id();
/**
 * Returns the type of skin_spl_sur.
 */
	virtual int type() const;
/**
 * Returns the string of the given spline surface type, which is "skinsur" for a skin_spl_surf.
 */
	virtual char const *type_name() const;
/**
 * Saves the skin_spl_sur as an approximation if there is a need to approximate.
 */
	virtual void save() const;
/**
 * Saves the information for skin_spl_sur to the save file.
 */
	virtual void save_data() const;

	/**
	*  @nodoc
	**/
	virtual void process(geometry_definition_processor& p ) const;
/**
 * @nodoc
 */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

	// Get the surface curves.
/**
 * Returns the surface curves.
 * @param no_crv
 * number of curves
 * @param curves
 * output array pointer
 */
	void get_curves(
			int &no_crv,
			curve ***curves
		) const;

	// Get the laws. The use count of the laws are bumped up by one.
	// The array of laws need to be deleted by the calling routine.
/**
 * Returns a list of laws used by the skin_spl_sur.
 * <br>
 * <b>Role</b>: The use count of the laws is incremented by one.
 * @param no_laws
 * number of laws
 * @param laws
 * list of laws pointer
 */
	void get_laws(
			int &no_laws,
			law ** &laws
		) const;

	// Get the surfaces. The array of surfaces need to be deleted by
	// the calling routine.
/**
 * Returns the surfaces.
 * <br>
 * <b>Role</b>: The array of surfaces need to be deleted by the calling routine.
 * @param no_surfaces
 * number of surfaces
 * @param surf_arr
 * surface array pointer
 */
	void get_surfaces(
			int &no_surfaces,
			surface ** &surf_arr
		) const;

/**
 * Returns the v_knot sequence for a given parameter value.
 * @param u
 * u parameter
 * @param out_no_knots
 * number of knots
 * @param out_vknots
 * output array pointer
 */
	void get_v_knots(
			double u,
			int &out_no_knots,
			double **out_vknots
		) const;

	// Function that estimates the scaling factor range by which the tangent
	// factors controlling the skin surface should be scaled so as to get the
	// surface with as large a radius of curvature as possible.
/**
 * Estimates the scaling factor range.
 * <br>
 * <b>Role</b>: Estimates the scaling factor range, by which the tangent factors
 * controlling the skin surface should be scaled so as to get the surface with
 * as large a radius of curvature as possible.
 * @param tan_range
 * range to use
 */
	void estimate_tanfacs_scale( SPAinterval &tan_range);

	// Function that estimates the minimum radius of curvature of the
	// skin surface for a given set of tangent factor values.  In some
	// sense this function can be thought of as the inverse of the
	// function above.
/**
 * This function estimates the minimum radius of curvature of the skin surface for a given set of tangent factor values.
 */
	double estimate_min_rad_curv();

	// We need to be able to set the tangent factors and get them for purposes
	// of determining optimal values for them.
/**
 * Set the tangent factors and get them for purposes of determining optimal values for them.
 * @param tangents
 * tangent factors
 * @param remake_approx
 * remake enabled or not
 */
	void set_tanfacs(double *tangents, logical remake_approx = TRUE);
/**
 * Get the tangent factors to determine optimal values for them.
 * @param tangents
 * tangent factors,
 * returns FALSE if there are none
 */
	logical get_tanfacs(double *tangents) const;

protected:
	friend DECL_KERN subtype_object *restore_skin_spl_sur();
	void restore_data();


#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

#endif

	// Debug printout. As for save and restore we split the operation
	// into two parts: the virtual function "debug" prints a class-
	// specific identifying line, then calls the ordinary function
	// "debug_data" to put out the details. It is done this way so that
	// a derived class' debug_data can call its parent's version first,
	// to put out the common data. Indeed, if the derived class has no
	// additional data it need not define its own version of debug_data
	// and use its parent's instead. A string argument provides the
	// introduction to each displayed line after the first, and a
	// logical sets "brief" output (normally removing detailed
	// subsidiary curve and surface definitions).

	virtual void debug(
				char const *,
				logical,
				FILE *
			) const;
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;
};
/**
 * @nodoc
 * Routine, which get the v-knots derivatives , for a given index.
 */
DECL_KERN void sg_get_vknots_dervs
(
	int    index,
	int	   no_curs,
	double *v_par0,
	double *v_par1,
	double *v_par2,
	double *v_par3,
	double *v_par_arr_1,
	double *v_par_arr_2,
	double *v_par_arr_3,
	double *v_par_arr_4,
	logical periodic = FALSE,
	double period_v = 0.0
);
/**
 * @nodoc
 * Routine which computes the knots for the sections by calculating
 * the average distance between them.
 */
DECL_KERN void sg_compute_avg_knots( 
		int		no_knots,
		curve   **curve_arr,
		double	*knots
);

/** @} */
#endif
