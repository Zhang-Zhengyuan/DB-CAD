/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPLINE_BS3H
#define SPLINE_BS3H
#include "dcl_spl.h"
#include "sp3crtn.hxx" 
#include "bs3curve.hxx"
#include "bs2curve.hxx"
#include "sp2crtn.hxx"
#include "bs3surf.hxx"
/**
 * @file spd3rtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3SURFACEAPI
 *
 * @{
 */
class check_status_list;
class check_fix;
class SPAposition;
class VOID_LIST;
class SPApar_box;
class SPA_C2_cubic_curve_fit_options;
class SPA_knot_rem_options;
/**
* Creates a cubic curve which is supplied as B-spline vertexes and knot values.
* <br><br>
* <b>Role:</b> The number of knots is given, and the multiplicities of internal
* knots (the end knots have multiplicity 3).
* <br><br>
* <tt>vertices</tt> is an array containing the three or two space vertices. It should
* contain <tt>mult*(nkts-2)+4</tt> values.
* <br><br>
* <tt>knots</tt> is an array of nkts distinct knot values.
* <br><br>
* <tt>mult</tt> is the multiplicity of the internal knots. The end knots always get a
* multiplicity of three.
* <br><br>
* The form of the curve must be set after construction. The form of the underlying
* spline curve is set during the construction.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param nkts
* number of knots.
* @param vertices
* vertices.
* @param knots
* list of knots.
* @param mult
* multiplicity.
**/

DECL_SPLINE bs3_curve bs3_curve_construct( int nkts, const SPAposition* vertices,
					double* knots, int mult );

/**
* Creates a curve which is supplied as B-spline vertexes and knot values.
* <br><br>
* <b>Role:</b> The number of knots is given, and the multiplicities of internal
* knots (the end knots have multiplicity 3). Same as <tt>bs3_curve_construct</tt>, but in
* two dimensions.
* <br><br>
* <tt>pverts</tt> is an array containing the three or two space vertices. It should contain
* <tt>mult*(nkts-2)+4</tt> values.
* <br><br>
* <tt>knots</tt> is an array of nkts distinct knot values.
* <br><br>
* <tt>mult</tt> is the multiplicity of the internal knots. The end knots always get
* multiplicity three.
* <br><br>
* The form of the <tt>bs2_curve</tt> must be set after construction. The form of the
* underlying spline curve is set during the construction.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param nkts
* number of knots.
* @param pverts
* parameter position.
* @param knots
* knots.
* @param mult
* multiple.
**/
DECL_SPLINE bs2_curve bs2_curve_construct( int nkts, const SPApar_pos* pverts,
				      double* knots, int mult );


// Construct a single-patch Bezier surface by Hermite interpolation to data
// at the corners. The arrays each have length 4, and contain data at (0,0),
// (1,0), (0,1) and (1,1) in that order. The derivatives are with respect to
// a unit parametrisation.

/**
* Creates a single patch Bezier surface from Hermite data at the patch corners.
* <br><br>
* <b>Role:</b> The only point to bear in mind is that data is passed in ascending
* u order first, but <tt>ag_srf_data</tt> thinks it's in ascending v order first.
* Therefore care must be taken when copying control points into the big array.
* <br><br>
* The arrays each have length 4, and contain data at (0,0), (1,0), (0,1) and (1,1)
* in that order. The derivatives are with respect to a unit parameterization.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param corners
* corners.
* @param uderivs
* u derivatives.
* @param vderivs
* v derivatives.
* @param twists
* twists.
**/

DECL_SPLINE bs3_surface	bs3_surface_hermite( const SPAposition* corners,
					     const SPAvector* uderivs,
					     const SPAvector* vderivs,
					     const SPAvector* twists );


// Evaluate the maximum turning angle over a curve (from the B-spline hull).


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE double	bs3_curve_angle( bs3_curve );

// Evaluate the maximum turning angle over the surface boundaries.

/**
* Gets the boundary angle.
* <br><br>
* <b>Role:</b> Returns the maximum turning angle over the surface boundaries.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param sur
* given surface.
* @param u_angle
* u angle.
* @param v_angle
* v angle.
**/

DECL_SPLINE void	bs3_surface_boundary_angle( bs3_surface sur, double& u_angle,
							 double& v_angle );

// Return an upper limit to the maximum distance from a point to that part of
// the bs3 surface whose vertices have SPAparameter values between the given edge
// and the given value. If v_param_line is true, then the u values of the
// vertices' effective knot values are checked to make sure that they lie
// between the low/hi u edge (depending on whether from_hi is TRUE/FALSE) and
// the given (u) param value.

/**
* Estimates the parameter of a near point on a surface, given the parameter of a control point of the surface.
* <br><br>
* <b>Role:</b> Returns an upper limit to the maximum distance from a point to
* that part of the <tt>bs3_surface</tt> whose vertices have parameter values between the
* given edge and the given value. If <tt>v_param_line</tt> is true, then the <i>u</i> values of
* the vertices' effective knot values are checked to make sure that they lie
* between the low/hi <i>u</i> edge (depending on whether <tt>from_hi</tt> is <tt>TRUE</tt>/<tt>FALSE</tt>)
* and the given (<i>u</i>) param value.
* <br><br>
* <b>Limitations:</b> This function will give poor estimates if the knot vector or
* control point distribution is skewed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3_surf
* given surface.
* @param P
* position.
* @param param_val
* parameter value.
* @param from_hi_edge
* edge.
* @param v_param_line
* v parameter line.
**/

DECL_SPLINE double bs3_surface_max_size_to_param_line(
					  bs3_surface bs3_surf,
					  const SPAposition& P,
					  double param_val,
					  logical from_hi_edge,
					  logical v_param_line
					  );

// bs3_curve_max_dist_to_point_bound returns an upper limit to the maximum
// distance from a point in space to the given curve by finding the maximum
// distance from the point to each of the vertices the curve.

/**
* Gets an upper limit to the maximum distance from a point in space to the given curve.
* <br><br>
* <b>Role:</b> This routine finds the maximum distance from the point to each
* of the knots on the curve.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3
* given curve.
* @param P
* position.
**/

DECL_SPLINE double bs3_curve_max_dist_to_point_bound(
						bs3_curve bs3,
						const SPAposition& P
						);


// Estimate parameters for the nearest point to a given point.

/**
* Estimates the parameter of the nearest point or a curve to the given point.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param P
* given position on curve.
* @param cu
* given curve.
**/

DECL_SPLINE double	bs3_curve_estimate_param( const SPAposition& P, bs3_curve cu );

// The same again, but for a 2d SPAparameter point and curve.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE double bs2_curve_estimate_param( SPApar_pos const& uv, bs2_curve cu );

// Construct a bounding cylinder for a curve.

/**
* Gets a cylinder enclosing a <tt>bs3_curve</tt>.
* <br><br>
* <b>Role:</b> Computes a bounding cylinder of the given curve. 
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param cu
* given curve.
* @param root
* returned cylinder root point.
* @param axis
* returned cylinder axis.
* @param rad
* returned cylinder radius.
**/

DECL_SPLINE void bs3_curve_cylinder(
			       bs3_curve cu,         // Input curve
			       SPAposition& root,    // returned cyl root point
			       SPAunit_vector& axis, // returned cyl axis
			       double& rad        // returned cyl radius
			       );


// Check whether the control points of a bs3 surface are ok. Checks for
// coincident adjacent control points, surface which are or aren't closed which
// should or shouldn't be.

/**
* Checks for errors in the approximating surface.
* <br><br>
* <b>Role:</b> If supplied with a spline, this extension uses that for
* evaluation in the continuity check.
* <br><br>
* In addition, it checks whether the control points of a bs3 surface are valid,
* whether for coincident adjacent control points, and surfaces which are closed
* but shouldn't be, or surfaces which are not closed but should be.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3
* given surface.
* @param spl
* given spline.
* @param fix
* available fixes.
* @param fixed
* fixes made.
* @param list
* list of things to be checked.
**/

DECL_SPLINE check_status_list* bs3_surface_check( bs3_surface bs3,
				     const spline& spl = *(spline*)NULL_REF,
				     const check_fix& fix = *(check_fix*)NULL_REF,
				     check_fix& fixed = *(check_fix*)NULL_REF,
				     const check_status_list* list = NULL );

// As above, but for bs3 curves.

/**
* Checks the curve for closure, continuity etc and applies fixes if requested, and possible.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs3
* given curve.
* @param ic
* corresponding intcurve.
* @param check
* list of things to be checked.
**/

DECL_SPLINE check_status_list* bs3_curve_check( bs3_curve bs3,
				     const intcurve& ic = *(intcurve*)NULL_REF,
				     const check_status_list* check = NULL );


// Check whether the control points of a bs2 curve are okay. In
// particular, deal with coincident control points.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE check_status_list* bs2_curve_check( bs2_curve,
				     const intcurve& = *(intcurve*)NULL_REF,
				     const surface& = *(surface*)NULL_REF,
				     const check_fix& fix = *(check_fix*)NULL_REF,
				     check_fix& fixed = *(check_fix*)NULL_REF,
				     const check_status_list* = NULL );

// STI dgh begin - prototypes for advanced blending

/**
* Lofts a series of similar <tt>bs3_curves</tt> into a <tt>bs3_surface</tt>.
* <br><br>
* <b>Role:</b> Loft a series of similar <tt>bs3_curves</tt> into a <tt>bs3_surface</tt> by
* splining across the control points of the curves with a cubic interpolation, and
* using the given knot vector which will become the v knot vector of the surface.
* The knot vector of the curves becomes the u knot vector of the surface. Closure
* forms both left as open.
* <br><br>
* This function is essentially the same as <tt>bs3_surface_loft_curves</tt>. However it
* does it with <i>u</i> and <i>v</i> transposed. So the supplied curves become the <i>u</i> parameter
* curves, and the longitudinal direction becomes the <i>v</i> direction. <tt>n_crvs</tt> is the
* number of curves not including the two derivative curves, which are the final
* two of the array. knots must be addressable 2 above and 2 below its start.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param n_crvs
* number of curves.
* @param crvs
* array of curves.
* @param knots
* array of knots.
**/

DECL_SPLINE bs3_surface
bs3_surface_loft_u_curves( int n_crvs, bs3_curve crvs[],
			   double knots[] );
// STI dgh end


// STI dgh begin - these prototypes came from kerngeom/d3_vbl/vbl_bs3.hxx

// Construct a cubic curve which interpolates or fits to an array of
// points, with given start and end directions, and knot vector.

/**
* Creates a cubic curve which interpolates or fits to an array of points,with given start and end directions.
* <br><br>
* <b>Role:</b> If a direction is a <tt>NULL</tt> reference or zero length, then a
* natural boundary condition is used, that is zero second derivative at the
* appropriate end.
* <br><br>
* <b>Limitations:</b> The number of knots in the knot vector must be <tt>(npt + degree +
* 1)</tt>. Since this is a cubic (degree is 3), the number of knots must be <tt>npt + 4</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param npt
* number of points to interpolate.
* @param pos
* points to interpolate.
* @param knots
* knot vector array.
* @param sdir
* start derivative.
* @param edir
* end derivative.
*
* NOTE: The "knots" argument is actually a pointer to the last clamping knot 
* at the start of the knot vector; for cubics this is the third knot.  So to
* call this routine, the user must allocate and initialize a knot array and
* then pass in the pointer to the knot array _INCREMENTED BY 2_.  This is illustrated
* in the following psuedo-code:
* 
* SPAposition pos[3] = {p0, p1, p2};//{ 3 positions here };
* double full_knots[7] = { A, A, A, B, C, C, C};  // 3 positions + 4 = 7 knots, with first and
*                                                 //  last 3 identical (clamped representation)
* bs3_curve new_curve = bs3_curve_interp_knots(3, pos, full_knots+2, sdir, edir);
* 
* where the "+2" ensures that the "knots" argument is pointing to the last "A" knot in
* the knot sequence.
* 
* Spatial Corp. recommends that users replace their calls to this routine with 
* the API function api_curve_spline2, which performs the above bookkeeping
* internally.
* 
**/

DECL_SPLINE bs3_curve bs3_curve_interp_knots( int npt,			// number of points
				 SPAposition const *pos,	// points to interpolate
				 double knots[],		// knot vector
				 const SPAvector &sdir,		// start direction
				 const SPAvector &edir		// end direction
				 );

// Loft a list of v-direction curves into a surface, assumes all curves have
// same knot vector etc.

/**
* Lofts a surface from an array of n <tt>bs3_curves</tt>.
* <br><br>
* <b>Role:</b> The array contains n+2 curves, the nth and n+1th being
* derivative curves to give the correct start and end derivatives while splining
* across the control points of the given curves. The curves proper are stored in
* elements 0 to n-1 inclusive. This does some casting of positions to vectors
* which isn't quite proper. It doesn't worry about potential periodicity or any
* other such details. The given knot vector becomes the u knot vector of the
* surface.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param curves
* curves.
* @param knots
* knots.
* @param fitol
* fit tolerance.
* @param n
* position.
* @param actual_tol
* returns the actual tolerance used.
**/

DECL_SPLINE bs3_surface bs3_surface_loft_curves( bs3_curve *curves,	// array of bs3 curves
				    double knots[],		// u knot vector
				    double fitol,		// requested fit tol
				    int n,				// no. of curves
				    double &actual_tol		// achieved fit tol
				    );

// Put a surface through a set of points. Look at the top of the function
// definition for a full description of the parameters.

/**
* Creates a bicubic surface that interpolates or fits a set of points,with specified boundary derivatives and twist vectors.
* <br><br>
* <b>Role:</b> This function interpolates or fits an array of positions, using
* knot vectors in the <i>u</i> and <i>v</i> directions, boundary derivatives (i.e., tangent
* vectors) and twist vectors.
* <br><br>
* <tt>nu</tt> is the number of positions in the <i>u</i> direction, <tt>nv</tt> the number
* of positions in the <i>v</i> direction. <tt>points</tt> is a two dimensional array of positions
* <tt>[nu*nv]</tt>. <tt>knotsu</tt> is the <i>u</i> knot vector, <tt>knotsv</tt> the
* <i>v</i> knot vector. Initially there is a one-to-one correspondence between knots and
* positions; however, it is required that the <i>-1, -2, n</i> and <i>n+1</i> element of the knot
* vectors be addressable, though they need not be set. Therefore the <tt>knotsu</tt>
* and <tt>knotsv</tt> should both be of size <tt>[nu+4]</tt>. The point
* for <i>u</i> knot <i>i</i> and <i>v</i> knot <i>j</i> is points <tt>[j*nu + i]</tt>. The increasing <i>u</i>
* values are stored contiguously.
* <br><br>
* <tt>deru</tt> contains the start and end derivatives for the <i>u</i> direction,
* elements 0 to <tt>nv-1</tt> for the bottom <i>u</i> knot end, and elements <tt>nv</tt> to <tt>2*nv-1</tt> for the
* top end. Specifying a zero length vector means that natural end conditions will
* be used for that place on the surface. <tt>derv</tt> and <tt>deru</tt> work the
* same way, replacing <tt>nv</tt> by <tt>nu</tt>.
* <br><br>
* Finally, <tt>deruv</tt> is an array of four vectors giving the twist vectors, i.e., the
* <i>uv</i> cross derivative at the corners of the domain. They are stored in the
* following order:
* <pre>
* (lo_u,lo_v), (hi_u,lo_v), (lo_u,hi_v), (hi_u,hi_v)</pre>
* As with first derivatives along the boundaries, a zero length vector is an
* indicator to use natural end conditions.
* <br><br>
* The interpolation algorithm assumes that all the knot values are distinct, the
* surface is open in both directions, and that there is generally nothing unusual
* going on.
* <br><br>
* As a side effect of the interpolation algorithm, this routine will set any
* vectors in <tt>derv</tt> which you have passed in as zero vectors. <tt>deru</tt> is left alone.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param nu
* number points in u.
* @param nv
* number points in v.
* @param points
* position array.
* @param knotsu
* u knot array.
* @param knotsv
* v knot array.
* @param deru
* u derivative array of end conditions.
* @param derv
* v derivative array of end conditions.
* @param deruv
* uv derivative array of corner twist vectors.
**/

DECL_SPLINE bs3_surface bs3_surface_interp_knots( int nu, 	// number of u knots
				     int nv,	// number of v knots
				     SPAposition *points,	// array of nu * nv points
				     double knotsu[],	// u knot values
				     double knotsv[],	// v knot values
				     SPAvector *deru,	// u derivative end conditions
				     SPAvector *derv,	// v derivative end conditions
				     SPAvector deruv[]	// corner twist vectors
				     );

// Whether the given bs2_curve straddles any of the knot lines of the given
// bs3_surface, meaning that the curve inherits the continuity degree of the
// B-spline surface.

/**
* Determines if the convex hull of the curve straddles any knots.
* <br><br>
* <b>Role:</b> The method used is equivalent to checking if the convex hull of
* the curve straddles any knots. It determines if the given <tt>bs2_curve</tt>
* straddles any of the knot lines of the given <tt>bs3_surface</tt>, meaning
* that the curve inherits the continuity degree of the B-spline surface
* (otherwise its infinitely continuous). It doesn't matter if occasionally it
* does straddle some knots when it doesn't, but the converse error is not acceptable.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs2_cur
* given bs2_curve.
* @param bs3_sur
* given bs3_surface.
* @param v_direction
* v direction.
**/

DECL_SPLINE logical bs2_curve_straddles_bs3_surface_knots(
					      bs2_curve bs2_cur,
					      bs3_surface bs3_sur,
					      logical v_direction		// v direction?
					      );


// Extension functions - extend a curve or surface tangentially. A new
// curve or surface is returned if necessary in the initial reference
// argument, and the original will have been deleted.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE SPAinterval bs2_curve_extend( bs2_curve&, const SPAinterval& reqd_range,
			   const SPAinterval& orig_range = *(const SPAinterval*)NULL_REF );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE SPAinterval bs3_curve_extend( bs3_curve&, const SPAinterval& reqd_range,
			   const SPAinterval& orig_range = *(const SPAinterval*)NULL_REF );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE SPApar_box	bs3_surface_extend( bs3_surface&, const SPApar_box& reqd_range,
			    const SPApar_box& orig_range = *(const SPApar_box*)NULL_REF );


// For internal use only
/**
 * @nodoc
 */
enum SPAnat_ext_checks 
{ ignore_checks,
	fail_on_bad_checks
};

/**
* Computes a natural spline extension. The returned SPApar_box is the extension achieved. In u, v, or both, the returned SPApar_box can 
* be the bs3_surface's original param_range, if the original bs3_surface is closed, or if the attempted extension was 
* found to be self intersecting.
* <br><br>
* <b>Role:</b> The polynomial of the appropriate end span is evaluated in the extension. Due to non-linearities,
* the extension may soon exhibit unexpected behavior. Users should only extend by a 
* fraction of the last knot span width. CAUTION: Users should only extend by a fraction of the last span width. 
* High degree and rational spline surfaces tend to be worse behaved than low degree non-rational spline surfaces.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3_surface
* given bs3_surface.
* @param SPApar_box
* parameter bound of requested extension
* @param check
* checking behavior: ignore checks or perform self-intersection test & fail on bad result
**/
DECL_SPLINE SPApar_box bs3_surface_natural_ext(bs3_surface, const SPApar_box&, SPAnat_ext_checks check=fail_on_bad_checks);

/**
* Computes a natural spline extension. The returned SPAinterval is the extension achieved. The returned SPAinterval can 
* be the bs3_curve's original param_range, if the original bs3_curve is closed, or if the attempted extension was 
* found to be self intersecting.
* <br><br>
* <b>Role:</b> The polynomial of the appropriate end span is evaluated in the extension. Due to non-linearities,
* the extension may soon exhibit unexpected behavior. CAUTION: Users should only extend by a 
* fraction of the last knot span width. 
* High degree and rational spline curves tend to be worse behaved than low degree non-rational spline curves.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3_curve
* given bs3_curve.
* @param SPAinterval
* parameter bound of requested extension
* @param check
* checking behavior: ignore checks or perform self-intersection test & fail on bad result
**/
DECL_SPLINE SPAinterval bs3_curve_natural_ext(bs3_curve, const SPAinterval&, SPAnat_ext_checks check=fail_on_bad_checks);

// The following function approximates a surface with a bicubic spline surface.
// The approximating surface is guaranteed to be within 'actual_tol' of the true
// surface.  actual_tol will be less than or equal to the requested tolerance,
// 'tol'. The error is measured in the direction of the approximating surface
// normal.  The parametrisation of the approximating surface will broadly follow
// that of the true surface, but the error between corresponding points with
// the same SPAparameter values will not be bounded.  If a negative value of tol is
// supplied, then it is assumed to mean that there is no need to try to fit any
// particular tolerance value.
//
// Usually, the function will be supplied with no u or v knots, but if knots are
// supplied then the function will guarantee to produce a surface with precisely
// those knots.  If u and v knots are both supplied then the function has no
// freedom to fit the tolerance, so this will be ignored.  However, the actual
// error will still be calculated and returned correctly.  If only u or v knots
// are supplied, then the function will try to attain the required tolerance but
// cannot guarantee success.

// STI rr (11/02/02) begin: Added new flag to tell this function whether or not to do a
// a hull check. Presently this flag can take three values, only two of which are active.
// self_int_check = 0 => Do only hull check and if clash found return no result.
// self_int_check = 1 => Not used, but could mean do hull check and if clash found trim
//			 to clash. Could be useful when we are calculating extensions.
//			 At this time we fail altogether. It will be better if we return
//			 as much as we can. Alternatively we could throw a svec error and
//			 let the caller deal with it.
// self_int_check = 2 => Do full self--intersection check seeded by data from the hull check
//			 Use same function as entity checker. Presently used by off_spl_sur
//			 make_approx function as there are several cases where surface is
//			 legal but the hull self--intersects.
// STI rr end.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE	bs3_surface
bs3_surface_make_approx( const surface&,
			 const SPApar_box&,
			 double tol,
			 double& actual_tol = *(double*) NULL_REF,
			 int nuknots = 0,
			 const double* uknots = 0,
			 int nvknots = 0,
			 const double* vknots = 0,
			 int self_int_check = 0,
			 logical keep_approx = FALSE);


// The following function checks whether a surface has a self-intersecting
// b-spline hull.  It returns TRUE if the hull does not self-intersect and FALSE
// if it does, and so can be used as a cheap check for surface legality.  Note
// that a surface may self-intersect even though its hull does not, in which
// case this function does not realise that the surface is illegal.  Also, if
// the hull self-intersects then it is possible that the surface does not.
// Furthermore, the function may err on the side of caution, returning FALSE
// when the hull is very close to self-intersection.

// To speed up the check, a value of istep and jstep greater than 1 may be
// given.  This causes the algorithm to step through the B-spline array with
// these steps (istep refers to the u direction, jstep to the v direction).
// For example, if it is known that the surface is in cubic Bezier
// form (i.e. all knots are triple knots) then it may be justified to supply
// istep = jstep = 3.

// The optional argument sf_clash_list can be used to return information about
// the spans that clash.  An entity in the clash list corresponds to a pair
// of points on the hull that clash; it does not imply that the surface itself
// self-intersects.  Each entity returns both an estimate of the SPAposition of the
// suspected self-intersection, and a pair of ranges that definitely contain it,
// if it exists.

#include "position.hxx"

// For internal use only
/**
 * @nodoc
 */
class DECL_SPLINE sf_clash_list : public ACIS_OBJECT {
public:
    SPApar_pos	uv1;		// Estimate of parameters at intersection
    SPApar_pos	uv2;		// Estimate of parameters at intersection
    SPApar_box	range1;		// First range containing the clash point
    SPApar_box	range2;		// Second range containing the clash point
    sf_clash_list* next;
	SPAposition pos;

    sf_clash_list( const SPApar_pos& p1, const SPApar_pos& p2,
		const SPApar_box& r1, const SPApar_box& r2, sf_clash_list* n )
	: uv1( p1 ), uv2( p2 ), range1( r1 ), range2( r2 ), next( n ) {}
    //~sf_clash_list()	{ delete next; }
    ~sf_clash_list();				// bmt 73739, stack overflow, removed recusrsion

    sf_clash_list( const SPApar_pos& p2, const SPApar_box& r2, SPAposition &p, sf_clash_list* n )
		: uv2( p2 ), range2( r2 ), pos( p ), next( n ) {}
 };
/**
 * @nodoc
 * For internal use only
 */
class DECL_KERN CLASH_ANALYSER
{
public:
/**
 * @nodoc
 * For internal use only
 */
 virtual logical analyse_clash(const sf_clash_list& clash) = 0;
};
/**
 * @nodoc
 * For internal use only
 */
DECL_SPLINE	logical bs3_surface_check_hull( bs3_surface, double tol = SPAresabs,
			int istep=1, int jstep=1,
			sf_clash_list*& clashes = *(sf_clash_list**)NULL_REF,
			CLASH_ANALYSER* analyser = 0);
/**
 *.
 */
DECL_SPLINE	logical bs3_surface_check_hit( bs3_surface, double tol = SPAresabs,
			                                int istep=1, int jstep=1,
			                                sf_clash_list*& clashes = *(sf_clash_list**)NULL_REF );

////////////////////////////////////////////////////////////////////////////////////////
//
// This function tests a grid of points for self-intersections by comparing segments
// from the grid with triangles in the grid, where the grid is made up of points on a
// surface.  The grid has dimension nu x nv with grid point (i, j) having u-v param
// values given by (u_knots[i],v_knots[j]).  If provided, bad uv values are returned
// in bad_uvs list - each element of the list must be deleted by the caller.  The exclude
// region is a uv SPAbox that is not tested for self-intersection, although it is tested
// against regions outside this SPAbox.  When an exclude region is provided, the extension_box
// is set to a region that contains the exclude_region and does not contain any
// self-intersections. This is usually enough information to determine appropriate legal
// surface intersections.
//
// Note the main difference between this function and bs3_surface_check_hull is that instead
// of using control points on a bs3 surface, points actually on the surface are tested.
// This allows us to determine bad uv values (which is difficult to get from bs3 control
// points) and also to easily exclude a uv SPAbox from consideration, which is useful when a
// region is known to be good, as in the case of surface extensions.
//
// The role of istep and jstep is as defined above.
//
/////////////////////////////////////////////////////////////////////////////////////////

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
check_point_grid( int       nu,
                  int       nv,
                  SPAposition  *pts,
                  double    *u_knots,
                  double    *v_knots,
                  SPApar_box   &exclude_region = *(SPApar_box*)NULL_REF,
                  SPApar_box   &extension_box = *(SPApar_box*)NULL_REF,
                  VOID_LIST &bad_uvs = *(VOID_LIST*)NULL_REF,
                  int istep=1, int jstep=1,
				  sf_clash_list*& clashes = *(sf_clash_list**)NULL_REF );

// The following function checks whether a curve has a self-intersecting
// b-spline hull.  It returns TRUE if the hull does not self-intersect and FALSE
// if it does, and so can be used as a cheap check for curve legality.  Note
// that a curve may self-intersect even though its hull does not, in which
// case this function does not realise that the curve is illegal.  Also, if
// the hull self-intersects then it is possible that the curve does not.
// The optional argument cu_clash_list can be used to return information about
// the spans that clash.  An entity in the clash list corresponds to a pair
// of points on the hull that clash; it does not imply that the curve itself
// self-intersects.  Each entity returns both an estimate of the SPAposition of the
// suspected self-intersection, and a pair of ranges that definitely contain it,
// if it exists.

// For internal use only
/**
 * @nodoc
 */
class DECL_SPLINE cu_clash_list : public ACIS_OBJECT {
public:
    double	param1;		// Estimate of parameter at intersection
    double	param2;		// Estimate of parameter at intersection
    SPAinterval	range1;		// First range containing the clash point
    SPAinterval	range2;		// Second range containing the clash point

    cu_clash_list* next;
    cu_clash_list( double p1, double p2,
		const SPAinterval& i1, const SPAinterval& i2, cu_clash_list* n )
	: param1( p1 ), param2( p2 ), range1( i1 ), range2( i2 ), next( n ) {}
    ~cu_clash_list();// bmt, 73739 made iterative, moved to d3cu_ext.cpp
	// Static function to delete a (list of) cu_clash_list objects
	// without using the recursive delete in the destructor. This avoids
	// stack overflow if the list is long (e.g. bug 70464)
/*
	static void non_recursive_delete(cu_clash_list* head);
*/
    };


// For internal use only
/**
 * @nodoc
 */
class DECL_SPLINE CU_CLASH_ANALYSER
{
public:
	/**
	  * @nodoc
	  */
	virtual logical analyse_clash(const cu_clash_list& clash) = 0;
};

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE	logical
bs3_curve_check_hull( bs3_curve,
		      double tol = SPAresabs,
		      cu_clash_list*& clashes = *(cu_clash_list**)NULL_REF,
			  CU_CLASH_ANALYSER* analyser = 0,
			  double approx_tol = -1.0,
			  intcurve const* intc = 0);


// A chord is defined by two B-spline vertices of a surface. The vertices are
// usually adjacent ones, but may be the ends of a section which is known to be
// straight. The chord contains a copy of a pointer to the B-spline vertex
// positions, and the endpoints of the chord in the array. 

#include "sp3srtn.hxx"
#include "math.hxx"
#include "pos.hxx"
#include "box.hxx"

// STI rr 11/12/02 begin: New enum that states what checking we must do in the clash test function subGrid::intersects.
/**
 * @nodoc
 */
enum clash_check_type
{
	hull_check = 0,		// Look for a clash using the B-spline hull and return as soon as one is found.
	hull_clash = 1,		// Find all clashes of the B-spline hull and return the data in the clash list.
	on_surface_check = 2,	// Look for a clash using points on the surface (check_point_grid) and return as soon as one is found.
	bad_uv_check = 3,	// Find all clashes using points on the surface (check_point_grid) and return them in bad uv list
	on_surface_clash = 4	// Find all clashes using points on the surface (check_point_grid) and return them in clash list.
};
// STI rr end

/**
 * @nodoc
 */
class CHORDS : public ACIS_OBJECT
{
	SPAposition*	_P0;		// Start of chord
	SPAposition*	_P1;		// End of chord
	int 	_i0, _i1, _j0, _j1; // Indices of the chord within the array
	logical _null;		// Is the chord null (are its endpoints equal)?
	
public:
	CHORDS( SPAposition* pts, int dim, int i0, int j0, int i1, int j1 )
		:	_i0( D3_min( i0, i1 ) ), 
			_i1( D3_max( i0, i1 ) ), 
			_j0( D3_min( j0, j1 ) ), 
			_j1( D3_max( j0, j1 ) )
	{
		_P0 = pts + i0*dim + j0;
		_P1 = pts + i1*dim + j1; 
		_null = VEC_equal( *_P0, *_P1 );
	}
	
	CHORDS( SPAposition &pts0, SPAposition &pts1)
		:	_i0( -1 ), 
			_i1( -1 ), 
			_j0( -1 ), 
			_j1( -1 )
	{
		_P0 = &pts0;
		_P1 = &pts1; 
		_null = VEC_equal( *_P0, *_P1 );
	}

	SPAposition&   P0() { return *_P0; }
	SPAposition&   P1() { return *_P1; }
	
	int 		i0()	{ return _i0; }
	int 		i1()	{ return _i1; }
	int 		j0()	{ return _j0; }
	int 		j1()	{ return _j1; }
	
	logical 	null()	{ return _null; }
};


// A GRID is a sub-patch of the B-spline surface. It contains a copy of a
// pointer to the B-spline vertex positions, and the indices of the bounding
// rows and columns. The array is actually one-dimensional, but the GRID member
// functions enable it to be treated as two-dimensional. 
//
// The grid may represent a region of the extended surface, in which case the
// vertices of each row or column are colinear. In this case, the rows or
// columns can be processed by processing their endpoints. The grid enables this
// by storing the 'step' between vertices in each direction. 
//
// The grid can also process periodic surfaces, by treating vertices as equal if
// their indices are equal modulo the period. 
// STI rr (11/02/02) begin: Tree class that will store the box information for the B-spline hull.
// This base class simply stores the basic tree information.
/**
 * @nodoc
 */
class quadTree : public ACIS_OBJECT
{
	int _no_of_progeny;
	quadTree *_progeny[4];
	
protected:
	
	struct coords {
		int x, y;
		coords( int xin, int yin ) : x(xin), y(yin) {}
		~coords() {}
	};
	
	virtual ~quadTree(){}
	void set_progeny(quadTree *child, int index = -1)
	{
		if( _no_of_progeny == 4 ) return;
		if( index < 0 || index > 3 || _progeny[index] )
			_progeny[_no_of_progeny++] = child;
		else
		{
			_progeny[index] = child;
			_no_of_progeny++;
		}
		return;
	}
public:
	quadTree(): _no_of_progeny(0) { for( int i = 0; i < 4; i++ ) _progeny[i] = 0; }
	int no_of_progeny() const { return _no_of_progeny; }
	quadTree *progeny(int i) const { return _progeny[i]; }
	void remove();
};
// STI rr end.
class GRID;
// STI rr (11/02/02) begin: Class derived from tree that will store the actual box information for the B-spline.
// Sub grid is a portion of the entire grid that represents the full B-spline.
/**
 * @nodoc
 */
class subGrid : public quadTree
{
	coords low, high; // row,column coordinates of the lower left and the upper right corners that define the sub grid.
protected:
	~subGrid(){}
	
	SPAbox subGrid_box; // The box of this subgrid.
	GRID *grid; // reference to the grid of which this is a part.
public:
	subGrid() : quadTree(), low(0,0), high(0,0), grid(0) {}
	subGrid( int x1, int x2, int y1, int y2, GRID *g )
		: quadTree(), low(x1,y1), high(x2,y2), grid(g) {}
	void subDivide();
	// STI rr (11/12/02) begin: Look for the enum definition above.
	logical intersects( CHORDS &ch, SPAbox const &ch_box, double const &tol, clash_check_type const &check_type,logical& finish );
	logical intersects1(CHORDS &ch, SPAbox const &chord_box, double const &tol, clash_check_type const &check_type );
};

// STI rr begin: Declare this function as it is needed to store the clash information during tree traversal.
/**
 * @nodoc
 */
void add_to_list( sf_clash_list*& clashes, 
							GRID& , CHORDS& chord, 
							int ig0, int jg0, int ig1, int jg1, 
							int deg_u, int deg_v, 
							double* uknots, double* vknots );
/**
 * @nodoc
 */
void add_to_list1( sf_clash_list*& clashes, 
							GRID& , CHORDS& chord, 
							int ig0, int jg0, int ig1, int jg1, 
							int deg_u, int deg_v, 
							double* uknots, double* vknots, SPAposition &pos );

/**
 * @nodoc
 */
class GRID : public ACIS_OBJECT
{
	SPAposition*	_pts;		// Array of B-spline vertices
	int 	_dim;		// Dimension of array in v-direction
	int 	_istep, _jstep; // Index steps between	'adjacent' vertices 
	int 	_imin, _imax, _jmin, _jmax; // Grid bounds
	int 	_imod, _jmod;	// periodic ranges (0 for non-periodic surface)
	double	_tol;		// Tolerance used for clash testing
	double	_tol_sq;	// Square of _tol
	
	subGrid		*_subGrid; // Pointer to the tree of subgrids constructed from this grid by subdivision.
	int			degree_u, degree_v; // Bspline data that is needed to store clash info.
	double		*uknots, *vknots;
	sf_clash_list *clash_list; // Linked list of clashes.
	SPApar_box	*_exclude_region, *_extension_box; // Data for Check point grid function.
	VOID_LIST	*_bad_uvs; // List of Bad uv's at which clashes found for check_point_grid function.
	
	SPAbox *_boxes;    // For quicker boxing when looking for self-intersections
	
	CLASH_ANALYSER* _analyser;

	logical ihit( CHORDS &ch, int i )
	{ 
		return i <= ch.i1() && i+_istep >= ch.i0() || 
			i <= ch.i0() && i+_istep >= ch.i1();
	}
	
	logical jhit( CHORDS &ch, int j )
	{ 
		return j <= ch.j1() && j+_jstep >= ch.j0() || 
			j <= ch.j0() && j+_jstep >= ch.j1();
	}

public:
	GRID( SPAposition* pts, int dim, int imod, int jmod, 
		int imin, int jmin, int imax, int jmax, 
		int istep, int jstep, double tol, CLASH_ANALYSER* analyser = 0)
		:	_pts( pts ), _dim( dim ), _imod( imod ), _jmod( jmod ), 
			_imin( imin ), _imax( imax ), _jmin( jmin ), _jmax( jmax ), 
		_istep( istep ), _jstep( jstep ), _boxes(NULL), _subGrid(NULL),
		_exclude_region(0), _extension_box(0), _bad_uvs(0),
		clash_list(NULL),_tol( tol ), _tol_sq( tol*tol ),
	    _analyser(analyser) {}
	
	~GRID() { if (_boxes) ACIS_DELETE [] _boxes; _subGrid->remove(); }
	
	int 		imin()	{ return _imin; }
	int 		imax()	{ return _imax; }
	int 		jmin()	{ return _jmin; }
	int 		jmax()	{ return _jmax; }
	int 		istep() { return _istep; }
	int 		jstep() { return _jstep; }
	double	tol()	{ return _tol; }
	double	tol_sq(){ return _tol_sq; }
	
	// STI rr (11/02/02) begin: Functions that access info about the subgrid and clashes 
	// stored in the grid.
	subGrid * sub_grid() { return _subGrid; }
	sf_clash_list *get_clash_list() { return clash_list; }
	void clear_clash_list() { ACIS_DELETE clash_list; clash_list = NULL; }
	void clear_bs3_data() { 	ACIS_DELETE [] _pts; 
	                            ACIS_DELETE [] STD_CAST uknots; 
								ACIS_DELETE [] STD_CAST vknots; }
	void set_bs3_data(int deg_u, int deg_v, double *uk, double *vk,
		SPApar_box *exclude_region = NULL, 
		SPApar_box *extension_box = NULL,
		VOID_LIST *bad_uvs = NULL)
	{
		degree_u = deg_u; 
		degree_v = deg_v; 
		uknots = uk; 
		vknots = vk; 
		_exclude_region = exclude_region;
		_extension_box = extension_box; 
		_bad_uvs = bad_uvs;
	}
	double *u_knots() const { return uknots; }
	double *v_knots() const { return vknots; }
	SPApar_box &exclude_region() const { return _exclude_region ? *_exclude_region : *(SPApar_box*)NULL_REF; }
	SPApar_box &extension_box() { return _extension_box ? *_extension_box : *(SPApar_box*)NULL_REF; }
	VOID_LIST &bad_uvs() { return _bad_uvs ? *_bad_uvs : *(VOID_LIST *)NULL_REF; }
	
	logical add_clashes(CHORDS &ch, int ig0, int jg0, int ig1, int jg1);

	void add_clashes(CHORDS &ch, int ig0, int jg0, int ig1, int jg1, SPAposition &pos)
	{
		add_to_list1( clash_list, *this, ch, ig0, jg0, ig1, jg1, degree_u, degree_v, uknots, vknots, pos );
	}

	logical add_clashes( SPApar_pos &bad_pos, SPApar_pos &grid_pos, SPApar_box &chord_box, SPApar_box &grid_box );

	void	reset( int imin, int jmin, int imax, int jmax )
	{ _imin=imin; _jmin=jmin; _imax=imax; _jmax=jmax; }
	
	// make boxes for each row to speed intersection tests, or in the tree structure by default.
	void make_boxes(logical old_method = FALSE);
	
	// test row SPAbox chord overlap
	logical row_box_hit_chord( CHORDS &ch, int row );
	
	// test if row intersects the row of a chord by comparing boxes
	logical row_intersects_chord_row( int row, int chord_row );
	
	// P is the point in the i,jth SPAposition. U is the next point in the u
	// direction, ignoring intermediate colinear points. Similarly, V is the
	// next point in the v direction, and W is the next in both u and v (i.e.
	// the next diagonally from P). 
	
	void square(  int i, int j, SPAposition*& P00, SPAposition*& P10, 
		SPAposition*& P01, SPAposition*& P11 )
	{ P00 = _pts + i*_dim + j;
	P10 = P00 + _istep *_dim;
	P01 = P00 + _jstep;
	P11 = P10 + _jstep; }
	
	SPAposition& P( int i, int j )	{ return _pts[ i*_dim + j ]; }
	SPApar_pos grid_par_pos(int row, int col, SPAposition const &pos, SPApar_box &grid_box);
	
	logical col_hit_chord( CHORDS &ch, int i )
	{ 
		return ihit( ch, i ) || 
			_imod != 0 && ( ihit( ch, i-_imod ) || ihit( ch, i+_imod ) );
	}
	
	logical row_hit_chord( CHORDS &ch, int j )
	{ 
		return jhit( ch, j ) || 
			_jmod != 0 && ( jhit( ch, j-_jmod ) || jhit( ch, j+_jmod ) );
	}
};

// The following function approximates a curve with a cubic spline curve.
// The approximating curve is guaranteed to be within 'actual_tol' of the true
// curve.  actual_tol will be less than or equal to the requested tolerance,
// 'tol'. The error is measured between corresponding parameter values.
// If a negative value of tol is supplied, then it is assumed to mean that there
// is no need to try to fit any particular tolerance value.
//

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE	bs3_curve
bs3_curve_make_approx( const curve&,
		       const SPAinterval&,
		       double tol,
		       double& actual_tol = *(double*) NULL_REF,
		       int nknots = 0,
		       const double* knots = 0,
		       logical geometric_refine = FALSE,
		       int self_int_test=0, 
			   bool disable_deriv_checks = false );

/**
 * Make a C2 cubic spline approximation to a curve, attempting to achieve user-specified tolerance, tol.
 * @param max_it
 * Limits the number of allowable iterations, limits the time taken for noisy curves; suggested value 40
 * @param range
 * Parameter range of the approximant, a subset of the curve's range
 * @param tol
 * A positive value, the desired accuracy; smaller means longer run-time, more control points in the approximation
 * @actual_tol
 * The tolerance achieved; users should always check that <tt>actual_tol <= tol</tt> before accepting the resulting bs3_curve
 * @param cu
 * An ACIS curve
 * @param opts
 * Non-experts should leave <tt>NULL</tt> for defaults; see the class SPA_curve_fit_options
 * @param approximate_arc_length
 * parameterization to be arc length
*/
DECL_SPLINE bs3_curve bs3_curve_cubic_approx(int max_it, 
                       SPAinterval const& range, 
                       double tol, 
                       double& actual_tol, 
                       curve const* cu, 
                       int nSeeds=0, 
                       double* seeds=NULL, 
                       SPA_C2_cubic_curve_fit_options* opts=NULL,
                       logical approximate_arc_length = FALSE);



/**
 * @nodoc
 * Make an approximately arclength parameterized curve that is geometrically identical to the input curve over the input
 * subset range.
 */
DECL_SPLINE curve* approx_arclength_curve(const curve& base_curve, double start, double end);


// The following functions state whether a bs2_curve is a parameter line of a
// surface. The final argument reports whether the curve parametrisation is the
// same as the surface. So, in the constant_u (v-parameter line) case, the
// same_para flag is set if the curve parametrisation matches v.


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical	bs2_curve_constant_u( bs2_curve, double& u = *(double*)NULL_REF,
			      logical& same_para = *(logical*)NULL_REF );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical	bs2_curve_constant_v( bs2_curve, double& v = *(double*)NULL_REF,
			      logical& same_para = *(logical*)NULL_REF );

// STI dgh end - prototypes from kerngeom/d3_vbl/vbl_bs3.hxx


// STI dgh } // End 


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE bs3_curve ag_bs_data_curve(int dim , int ctype , int m , int n , int mult , int rat , double  point_array[], double  knots[]);

/** @} */
#endif
