/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Description :
// Routines for processing 3D B-spline curves, used to fit object-space
// curves to intersections of surfaces.
/*******************************************************************/
#if !defined( BS3_CURVE_ROUTINES )
#define BS3_CURVE_ROUTINES
#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "bs3surf.hxx"
#include "dcl_spl.h"
#include "debugmsc.hxx"
#include "edge.hxx"
#include "logical.h"
#include "unitvec.hxx"
#include "api.hxx"
#include "cur_sur.hxx" //for enums
/**
* @file sp3crtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3CURVEAPI
 *
 * @{
 */
class cone;
class curve;
class curve_curve_int;
class curve_extremum;
class curve_surf_int;
class ellipse;
class helix;
class intcurve;
class plane;
class rat_poly_vec;
class SPAbox;
class SPAinterval;
class SPApar_dir;
class SPApar_pos;
class SPAparameter;
class SPAposition;
class SPAtransf;
class SPAunit_vector;
class SPAvector;
class sphere;
class spline;
class straight;
class surface;
class torus;
class law;

/*
// tbrv
*/
/**
 * @nodoc
 */

struct ag_spline;
// STIPORT TAC
#if defined( osf1 ) || defined( vms )
#include "position.hxx"
#endif
/*******************************************************************/
// Suppress decoration of function names, to ease interfacing to
// C or other languages.

// STI dgh {


// **** Initialisation and Shutdown ****

// Initialise the 3D spline curve system.
/**
 * Initializes the <tt>bs3_curve</tt> interface and the underlying %curve package.
 * <br><br>
 * <b>Role:</b> ACIS calls this routine once; it should not be called again.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_SPLINE void
bs3_curve_init();


// Shut things down (e.g. release caches of free store).
/**
 * Flushes any retained storage in the underlying %curve package.
 * <br><br>
 * <b>Role:</b> ACIS calls this routine; it should not be called externally.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_SPLINE void
bs3_curve_term();

// Synchronize tolerance in ag with ACIS SPAresabs, SPAresnor, SPAresmch, SPAresfit
/**
 * Synchronizes ag tolerance with ACIS tolerance.
 * <br><br>
 * <b>Role:</b> This routine can be called at any time.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_SPLINE void
bs3_synchronize_tolerance();

// **** Construction ****

// Construct a spline curve from a portion of a general ACIS curve.
/**
 * Creates a B-spline %curve from a portion of a general ACIS curve.
 * <br><br>
 * <b>Role:</b> The parameter range of the new %curve will be from the given
 * start parameter to the given end parameter. It may not match the original
 * parameterization in between.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cur
 * given curve.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * required fit tolerance.
 * @param actual_tol
 * actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_cur(
			curve const &cur,			// given curve
			double start_param,					// start SPAparameter
			double end_param,					// end SPAparameter
			double requested_tol = 0,				// required fit tolerance
			double &actual_tol = *(double *)NULL_REF
									// set to actual fit tolerance
		);


// Construct a spline curve from a straight line segment.
/**
 * Creates a parametric %curve that is geometrically the same as the portion of the given straight line between the given parameter values.
 * <br><br>
 * <b>Role:</b> The parameter range of the new %curve must be from the given
 * start parameter to the given end parameter. It does not have to be linear in
 * object space, but it would be most surprising if it were not.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param str
 * given straight line.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * requested tolerance returned.
 * @param actual_tol
 * actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_str(
			straight const &str,		// given straight line
			double start_param,					// start SPAparameter
			double end_param,					// end SPAparameter
			double requested_tol = 0,				// required fit tolerance
			double &actual_tol = *(double *)NULL_REF
									// set to actual fit tolerance
		);
/**
 * Creates a NUB (i.e., Non-Uniform B-spline) %curve from an elliptical arc.
 * <br><br>
 * <b>Role:</b> The parameter range of the new %curve will be from the given
 * start parameter to the given end parameter.  This function will return <tt>NULL</tt> 
 * if it fails; therefore, users must check the return value.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param ell
 * given ellipse.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * requested tolerance, should be much greater than <tt>SPAresabs</tt>, otherwise lots of control points.
 * @param actual_tol
 * actual tolerance achieved.
 **/
DECL_SPLINE bs3_curve bs3_curve_make_ell_nubs(
			ellipse const &ell,
			double start_param,
			double end_param,
			double req_tol = SPAresfit,				// requested fit tolerance
			double &actual_tol = *(double *)NULL_REF			// actual fit tolerance achieved
			);

// Construct a spline curve from an elliptical arc.
/**
 * Creates a B-spline %curve from an elliptical arc.
 * <br><br>
 * <b>Role:</b> The parameter range of the new %curve will be from the given
 * start parameter to the given end parameter. The parameterization of the %curve is
 * not defined, but will be as uniform as is practicable.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param ell
 * given ellipse.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * requested tolerance returned.
 * @param actual_tol
 * actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_ell(
			ellipse const &ell,		// given ellipse
			double start_param,					// start SPAparameter
			double end_param,					// end SPAparameter
			double requested_tol = 0,				// required fit tolerance
			double &actual_tol = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// Construct a spline curve from a helical arc.
/**
 * Creates a B-spline %curve from a helical arc.
 * <br><br>
 * <b>Role:</b> The parameter range of the new %curve will be from the given
 * start parameter to the given end parameter. The parameterization of the %curve is
 * not defined, but will be as uniform as is practicable.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param hel
 * given helix.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * requested tolerance returned.
 * @param actual_tol
 * actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_hel(
			helix const &hel,		// given helix
			double start_param,					// start SPAparameter
			double end_param,					// end SPAparameter
			double requested_tol = 0,				// required fit tolerance
			double &actual_tol = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// Construct a spline curve from an intcurve. Not normally very
// useful, but called by bs3_curve_make_cur().
/**
 * Creates a B-spline %curve from an intcurve.
 * <br><br>
 * <b>Role:</b> This routine is not normally very useful, but called by
 * <tt>bs3_curve_make_cur</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cur
 * given %curve.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param requested_tol
 * requested fit tolerance.
 * @param actual_tol
 * returned actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_int(
			intcurve const &cur,		// given curve
			double start_param,					// start SPAparameter
			double end_param,					// end SPAparameter
			double requested_tol = 0,				// required fit tolerance
			double &actual_tol = *(double *)NULL_REF
									// set to actual fit tolerance
		);


// Construct a conic curve.  Given are the start and end points of the
// segment to be represented, the intersection point of the tangents
// at the start and end, and the "rho" value.  This last determines the
// SPAposition of the mid-SPAparameter point along the line joining the
// mid-point of the chord and the intersection of the tangents, and is
// simply the ratio of its distance from the chord mid-point to the
// total distance.  A value of 0.5 gives a parabola, more gives a
// hyperbola, less an ellipse.
/**
 * Creates a general conic %curve segment given three distinct points and a rho value.
 * <br><br>
 * <b>Role:</b> This segment runs from the start point to the end point, and is
 * tangent to the line from the start to the intersection point at the start, and
 * to the line from the intersection point to the end.
 * <br><br>
 * The "fullness" of the %curve is controlled by the <tt>rho</tt> value. The line segment is
 * constructed from the mid point of the chord between start and end points to the
 * tangent intersection point. The "shoulder point" is the point that divides that
 * line segment in the ratio <tt>rho:(1 - rho)</tt>. The required conic is the one obeying
 * the end conditions that passes through the shoulder point.
 * <br><br>
 * <tt>rho</tt> <i>must be such that 0 &lt; </i><tt>rho</tt><i> &lt; 1. If </i><tt>rho</tt><i> is 0.5 (default), the conic is
 * parabolic. If </i><tt>rho</tt><i> is less than 0.5, the conic is a segment of an ellipse,
 * becoming more and more flattened as </i><tt>rho</tt><i> tends towards 0.0. If </i><tt>rho</tt><i> is greater
 * than 0.5, the conic is a segment of one branch of a hyperbola, with an
 * increasingly sharp bend near the tangent intersection point as </i><tt>rho</tt><i> nears 1.0.</i>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param start
 * start point.
 * @param tan_int
 * point where tangents intersect.
 * @param end
 * end point.
 * @param rho
 * controls fullness.
 * @param actual_tol
 * requested tolerance.
 * @param actual_fittol
 * returned actual tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_make_rho_conic(
			SPAposition const &start,		// start point
			SPAposition const &tan_int,		// point where tangents intersect
			SPAposition const &end,		// end point
			double rho = 0.5,			// "rho" value
			double actual_tol = 0,				// required fit tolerance
			double &actual_fittol = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE bs3_curve
bs3_curve_make_parabola(
			SPAposition const &start,
			SPAposition const &end,
			SPAposition const &apex,
			SPAposition const &focus
		);


// Construct a cubic curve which interpolates or fits to an array of
// points, with given start and end directions.
/**
 * Creates a B-spline %curve that interpolates or fits to the given tolerance the given points, 
 * with the given tangent directions at the two end points.
 * <br><br>
 * <b>Role:</b> If an end direction is a <tt>NULL</tt> reference or has zero length, then
 * a natural boundary condition is used, i.e., the second derivative at that end is
 * set to zero.
 * <br><br>
 * If any of the start or end conditions are set and are non-zero, or the last
 * point to interpolate is more than <tt>SPAresabs</tt> from the first point, the periodic
 * flag is ignored.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param npts
 * number of points to interpolate.
 * @param pts
 * points to interpolate or fit.
 * @param start_dir
 * start direction vector.
 * @param end_dir
 * end direction vector.
 * @param fitol
 * fit tolerance. Use 0 for interpolation, >0 for approximation; approximation will smooth the curve.
 * @param actual_tol
 * returned actual tolerance used.
 * @param periodic
 * make periodic if no end conditions and matching end points.
 **/
DECL_SPLINE bs3_curve
bs3_curve_interp(
			int npts,					// number of points
			SPAposition const *pts,		// array of points to interpolate or fit
			SPAunit_vector const &start_dir,	// start direction
			SPAunit_vector const &end_dir,	// end direction
			double fitol,					// fit tolerance, 0 for interpolate
			double &actual_tol = *(double *)NULL_REF,
									// set to actual fit tolerance
		    logical periodic = FALSE
		                            // Periodic if closed and no end conds
		);


// Construct a curve along a u parameter line (i.e. one with constant
// v parameter) of a spline surface.
/**
 * Creates a B-spline %curve along the <i>u</i> parameter of a parametric %surface, with the <i>v</i> parameter held constant.
 * <br><br>
 * <b>Role:</b> This line is one whose <i>u</i> parameter of the %surface varies, while
 * the <i>v</i> parameter is fixed. The parameterization of the resulting %curve is
 * undefined, though it must have the same sense as the %surface <i>u</i> parameter, but
 * would normally be the same as the %surface <i>u</i> parameter. The %curve will be open,
 * closed or periodic according to whether the %surface was open, closed or periodic
 * in the <i>u</i> direction.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param surf
 * given %surface.
 * @param v
 * v parameter of %surface.
 **/
DECL_SPLINE bs3_curve
bs3_curve_u_param_line(
			bs3_surface surf,
			double v
		);


// Construct a curve along a v parameter line (i.e. one with constant
// u parameter) of a spline surface.
/**
 * Creates a B-spline %curve along the <i>v</i> parameter of a parametric %surface, with the <i>u</i> parameter held constant.
 * <br><br>
 * <b>Role:</b> Creates a %curve along a <i>v</i>-parameter line (i.e., one with
 * constant <i>u</i> parameter) of a B-spline %surface. This is one that the <i>v</i>-parameter of
 * the %surface varies, while the <i>u</i>-parameter is fixed. The parameterization of the
 * resulting %curve is undefined, though it must have the same sense as the %surface
 * <i>v</i>-parameter, but would normally be the same as the %surface <i>v</i>-parameter. The
 * %curve will be open, closed, or periodic according to whether the %surface was
 * open, closed, or periodic in the <i>v</i>-direction.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param surf
 * given %surface.
 * @param u
 * u parameter of %surface.
 **/
DECL_SPLINE bs3_curve
bs3_curve_v_param_line(
			bs3_surface surf,
			double u
		);


// Convert a 2D B-spline into 3D form, with zero Z value.
/**
 * Converts a two-dimensional, parameter-space %curve into a three-dimensional %curve.
 * <br><br>
 * <b>Role:</b> Converts a two-dimensional, parameter-space %curve into a
 * three-dimensional %curve by treating the <i>u</i> parameter as the <i>x</i>-coordinate, the <i>v</i>
 * parameter as the <i>y</i>-coordinate, and adding a zero as the <i>z</i>-coordinate. If an
 * error occurs, this routine returns a <tt>NULL</tt> %curve.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param curin
 * given parameter-space %curve.
 **/
DECL_SPLINE bs3_curve
bs3_curve_from_bs2(
			bs2_curve curin			// given curve
		);


// Construct a curve which is a subset of a given one, being the
// overlap in SPAparameter space of the given curve and a given SPAinterval.
// A curve periodic in one or both SPAparameter directions is rolled
// around if need be to cover the required range. It is very unlikely
// that the tolerance arguments will be needed, but they are included
// for completeness.

/**
 * Creates a B-spline %curve that is a subset of a given one.
 * <br><br>
 * <b>Role:</b> Creates a %curve that is a subset of a given one, being the
 * overlap in parameter space of the given %curve and a given interval. A %curve
 * periodic in one or both parameter directions is rolled around if need be to
 * cover the required range. It is very unlikely that the tolerance arguments will
 * be needed, but they are included for completeness. Note that NULL can be returned in the following
 * the following cases: if the requested parameter bounds have
 * no overlap with the input bs3_curve parameter bounds, or if the requested parameter bounds
 * is too small to create a legal bs3_curve.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param old_bs
 * given %curve.
 * @param new_range
 * required bounds.
 * @param requested_tol
 * requested fit tolerance.
 * @param actual_fit
 * returned actual fit tolerance used.
 **/
DECL_SPLINE bs3_curve
bs3_curve_subset(
			bs3_curve old_bs,				// given curve
			SPAinterval const &new_range,		// required bounds
			double requested_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF, // returns actual fit tolerance
			logical shift_seam = FALSE
		);


// **** Modification ****

// Reparametrise a 3D B-spline curve by a linear transformation, to
// achieve given start and end values.
/**
 * Reparameterizes the given B-spline %curve in place.
 * <br><br>
 * <b>Role:</b> Reparameterizes the given %curve in place by a linear
 * transformation so that its primary interval of definition is from the start to
 * the end parameters given (which must be in increasing order). Each new parameter
 * value will be the appropriate linear function of the old parameter value.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param start
 * start parameter desired.
 * @param end
 * end parameter desired.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE void
bs3_curve_reparam(
			double start,				// start SPAparameter desired
			double end,				// end SPAparameter desired
			bs3_curve cur			// given curve
		);


// Shift the parameter values of a 3D B-spline curve by a given amount.
/**
 * Reparameterizes the given B-spline %curve in place by adding the given shift value to its parameter values.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param delta
 * parameter shift desired.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE void
bs3_curve_shift(
			double delta,				// SPAparameter shift desired
			bs3_curve cur			// given curve
		);


// Reverse the direction of a 3D B-spline curve, negating its
// parametrisation.
/**
 * Reverses the direction of the given B-spline %curve, and negates the parameterization.
 * <br><br>
 * <b>Role:</b> Reverses the direction of the given %curve, and negate the
 * parameterization (so that the new primary definition interval is [<i>-b, -a</i>] if the
 * original was [<i>a, b</i>]).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE void
bs3_curve_reverse(
			bs3_curve cur			// given curve
		);


// Split a B-spline curve into two sections at a given parameter value.

// This routine divides a given 3D B-spline curve at a given
// parameter value.  If an object-space point is given (which need
// not be exactly at the point defined by the curve and parameter,
// but is assumed to be close), the appropriate end point of each
// resulting spline is shifted to lie exactly on the given position.

// If a tangent direction is given as a fourth argument, an attempt
// is made to match tangent directions at the cut.

// If the B-spline is open, this routine creates a new spline for the
// initial portion of the curve, and returns this as its value.
// If the B-spline is closed, it takes the portion before the given
// parameter value and tacks it on the end, but marks the spline as
// open henceforth.  In this case, the routine returns NULL.

// The second unit_vector gives the direction which the second (higher
// in parameter) curve is to have at the split. It only needs to be
// passed for curves that are not G1 at the split point (which is and
// should be very unusual). If omitted it is taken to be the same as the
// the first given direction. DAP.
/**
 * Splits a given 3D B-spline %curve at a given parameter value.
 * <br><br>
 * <b>Role:</b> This function divides a given 3D B-spline %curve at a given
 * parameter value. If an object space point (which need not be exactly at the
 * point defined by the %curve and parameter, but is assumed to be close) is given,
 * the appropriate end point of each resulting B-spline is shifted to lie exactly on
 * the given position.
 * <br><br>
 * If a tangent direction is given as a fourth argument, an attempt is made to
 * match tangent directions at the cut. If a fifth argument is also given, an
 * attempt is made to match the fourth argument direction on the low parameter side
 * of the split, and the fifth argument direction on the high parameter side of the
 * split. This gives the function the ability to split non-G1 B-splines nicely.
 * <br><br>
 * This function creates a new B-spline for the initial
 * portion of the %curve, and returns this as its value. If the B-spline is closed,
 * and ths split corresponds to the %curve seam, the function returns <tt>NULL</tt>.
 * <br><br>
 * The second <tt>SPAunit_vector</tt> gives the direction which the second (higher in
 * parameter) %curve is to have at the split. It only needs to be passed for curves
 * that are not G1 at the split point (which is, and should be, very unusual). If
 * omitted it is taken to be the same as the the first given direction.
 * <br><br>
 * The return value from this function is a %curve representing the portion of the
 * original %curve before the split point, or <tt>NULL</tt> if the split point was at the
 * start of the original %curve. The given %curve is modified to become the part of
 * the %curve subsequent to the split point, or is set to <tt>NULL</tt> if the split point
 * was at the end.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 * @param param
 * given parameter value.
 * @param split_pt
 * given position.
 * @param vec1
 * given direction of split.
 * @param vec2
 * given direction on "high" side.
 **/
DECL_SPLINE bs3_curve
bs3_curve_split(
			bs3_curve &cur,			// given curve
			double param,					// given SPAparameter value
			SPAposition const &split_pt = *(SPAposition *)NULL_REF, // given SPAposition
			SPAunit_vector const &vec1 = *(SPAunit_vector *)NULL_REF, // given direction
		    SPAunit_vector const &vec2 = *(SPAunit_vector *)NULL_REF  // ditto, on "high" side
		);


// Join two compatible 3D B-splines together.
// This routine does not check for compatibility.  It is commonly used
// after bs3_curve_split(), where the curves were originally the same curve.
// Compatible means that the splines have the same degree and rationality,
// and the end of the first is coincident with the start of the second.
// Note that this is not what is provided by bs3_curve_compat().
// That routine results in two curves that are compatible side-by-side,
// e.g. that could be used as two curves of the same surface, in the
// same direction.
// See also bs3_curve_connect().
/**
 * Creates a new B-spline %curve by appending the second B-spline %curve to the end of the first.
 * <br><br>
 * <b>Role:</b> Appends the second %curve to the end of the first, constructing a
 * new combined %curve (though destroying the originals). If two curves are input,
 * the first %curve is modified to be the combined %curve and returned; the second
 * %curve is deleted. If one %curve is <tt>NULL</tt>, the other %curve is returned as the
 * combined %curve.
 * <br><br>
 * <b>Limitations:</b> This routine does not check for compatibility. See the function <tt>bs3_curve_connect</tt> to
 * both make the curves compatible and join them. 
 * It is commonly
 * used after <tt>bs3_curve_split()</tt>, where the curves were originally the same curve.
 * Compatible means that the B-splines have the same degree and rationality, and the
 * end of the first is coincident with the start of the second. Note that this is
 * not what is provided by <tt>bs3_curve_compat()</tt>. That routine results in two curves
 * that are compatible side-by-side, e.g. that could be used as two curves of the
 * same %surface, in the same direction.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param first_part
 * first %curve.
 * @param last_part
 * second %curve.
 **/
DECL_SPLINE bs3_curve
bs3_curve_join(
			bs3_curve first_part,			// given curve
			bs3_curve last_part			// given curve
		);

//
// Connect two splines end to end in a C1 join, returning the joined spline.
//
// This routine takes care of compatibility of the curves,
// and of cleaning up: first_part and last_part are gone after
// the call, and the resulting spline is the return value.
// However, it's dangerous to say
//		s1 = bs3_curve_connect( s1, s2 );
// because if it fails, it returns a null pointer and leaves s1 and s2
// alone.  Thus s1 still exists but the caller has just zeroed its pointer.
// So always say
//		s3 = bs3_curve_connect( s1, s2 );
// then either s3 is valid and s1 and s2 are gone, or vice versa.
//
// The resulting spline has the same direction and parameterization
// as the first spline, s1.  The second one might be reversed,
// and will likely be reparameterized.  It gets attached to either
// the start or end of the first spline, wherever it is coincident.
// Its parameterization is
// adjusted so that its tangent magnitude matches that of the first
// spline, where they meet.  This gives a C1 join, if the input
// splines were G1.
//
// Both splines must have the normal knot multiplicity of knots at the ends.
//
// If there's a problem, the return value is NULL and the inputs are unchanged.
// The only problems might be if the splines have no common end points,
// or if either spline is null or corrupted somehow.
//
// This is not generalized to do both bs2_ and bs3_curves: dim is 3.
//

/**
 * Joins two B-splines end to end in a (possibly) C1 join.
 * <br><br>
 * <b>Role:</b> This routine takes care of compatibility of the curves: degree, rationality, and parameterization. 
 * If it suceeds, it will also delete the two input curves.
 * However, it's dangerous to say
 * <br><br>
 * <tt>s1 = bs3_curve_connect ( s1, s2 );</tt>
 * <br><br>
 * because if it fails, it returns a <tt>NULL</tt> pointer and leaves <tt>s1</tt> and <tt>s2</tt> alone. Thus
 * <tt>s1</tt> still exists but the caller has just zeroed its pointer. So always say
 * <br><br>
 * <tt>s3 = bs3_curve_connect ( s1, s2 );</tt>
 * <br><br>
 * then either <tt>s3</tt> is valid and <tt>s1</tt> and <tt>s2</tt> are gone, or vice versa.
 * <br><br>
 * The resulting B-spline has the same direction and parameterization as the first
 * B-spline, s1. The second one might be reversed, and if instructed to join C1 will likely be
 * reparameterized. It gets attached to either the start or end of the first B-spline, wherever it is
 * coincident. If instructed to join C1 its parameterization is adjusted so that its tangent magnitude
 * matches that of the first B-spline, where they meet. This gives a C1 join, if the
 * input B-splines were G1.
 * <br><br>
 * <b>Errors:</b> If there's a problem, the return value is <tt>NULL</tt> and the inputs are
 * unchanged. The only problems might be if the B-splines have no common end points,
 * or if either B-spline is null or corrupted somehow.
 * <br><br>
 * <b>Limitations:</b> Both B-splines must have the normal knot multiplicity of knots at
 * the ends. This is not generalized to do both <tt>bs2_curve</tt>s and <tt>bs3_curve</tt>s; the
 * dimension is 3.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param bs1
 * first given %curve.
 * @param bs2
 * second given %curve.
 * @param joinC1
 * logical (default TRUE) that instructs the function to join the B-spline curves in a C1 manner.
 **/
DECL_SPLINE bs3_curve
bs3_curve_connect(
			bs3_curve bs1,
			bs3_curve bs2,
			logical joinC1 = TRUE,
			logical joinC0 = FALSE
		);

// Make a bs3_curve a rational bs3_curve
/**
 * Modifies the <tt>bs3_curve</tt> into a rational <tt>bs3_curve</tt> with weights set to 1.
 * <br><br>
 * <b>Role:</b> Increases the dimension of each coefficient by one
 * and sets the weight to one.
 * <br><br>
 * <b>Limitations:</b> The function does not check if the <tt>bs3_curve</tt> is already
 * rational.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param bs
 * bs3_curve to modify.
 **/
DECL_SPLINE logical
bs3_curve_make_rational(bs3_curve bs);

// Transform a curve. This involves transforming the control
// points, and scaling the knot values, in order to maintain the
// SPAparameter as a distance measure.
/**
 * Transforms the given B-spline %curve in place.
 * <br><br>
 * <b>Role:</b> Simply transforms the %curve's control points and scales the knot
 * values. The parameterization is not affected by the transformation.
 * <br><br>
 * A <tt>SPAtransf</tt> consists of a 3 x 3 matrix with unit determinant, giving an affine
 * transformation, an overall scaling factor and a translation vector. There are
 * also three logical flags, relating to the matrix:
 * <br><br>
 * <tt>rotate</tt> indicates whether the matrix is anything other than the identity.
 * <br><br>
 * <tt>reflect</tt> indicates whether the determinant is -1.
 * <br><br>
 * <tt>shear</tt> is set if the matrix is not orthogonal.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 * @param t
 * given transform.
 **/
DECL_SPLINE void
bs3_curve_trans(
			bs3_curve cur,			// given curve
			SPAtransf const &t		// given SPAtransf
		);


/**
* Warp a B-spline %curve.
* <br><br>
* <b>Role:</b> Modify a B-spline %curve by applying the given law to the control points of the given B-spline %curve. 
* If the given law is linear, the result will be the same as composing the law with the B-spline %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param cur
* given %curve.
* @param the_law
* law from R3 to R3.
**/
DECL_SPLINE void
bs3_curve_warp(
			bs3_curve cur,	// given curve
			law const* the_warp		// given law
		);

// **** Enquiry ****

// Return a logical indicating whether the curve form is open
// or not.
/**
 * Determines if a <tt>bs3_curve</tt> is open.
 * <br><br>
 * <b>Role:</b> A %curve is open if its end points do not meet at the same point
 * to form a closed loop.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE logical
bs3_curve_open(
			bs3_curve cur			// given curve
		);


// Return a logical indicating whether the curve is a closed loop
// or not.
/**
 * Determines if a %curve is closed.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the %curve is closed, i.e., the two end points
 * are within system tolerance of each other in object space.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE logical
bs3_curve_closed(
			bs3_curve cur			// given curve
		);


// Return a logical indicating whether the curve is periodic
// or not.
/**
 * Determines if the %curve is smoothly closed and is an endless loop.
 * <br><br>
 * <b>Role:</b> If the %curve is periodic, it is defined for all parameter values.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE logical
bs3_curve_periodic(
			bs3_curve cur			// given curve
		);



// Return the parameter range of a 3D B-spline curve.
/**
 * Gets the primary interval on which the %curve is defined, 
 * which in open and nonperiodic closed curves is the actual domain.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAinterval
bs3_curve_range(
			bs3_curve cur			// given curve
		);


// Return the SPAparameter period of a 3D B-spline curve.
// Returns zero if curve is not closed or periodic.

/**
 * Gets the length of the primary interval (the parametric period) 
 * that defines a periodic %curve.
 * <br><br>
 * <b>Role:</b> Returns exactly 0 for any nonperiodic %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE double
bs3_curve_period(
			bs3_curve cur			// given curve
		);


// On the assumption that a bs3_curve is made up of a sequence of
// simpler pieces (e.g. a B spline may be considered to be a sequence
// of polynomial spans), return the number of such spans in the given
// curve.
/**
 * On the assumption that the B-spline %curve consists of a sequence of simple spans,
 * gets the number of such spans.
 * <br><br>
 * <b>Role:</b> The precise definition of "simple" is moot. But, for a piecewise
 * polynomial %curve the simple spans should be the polynomial pieces.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE int
bs3_curve_nspans(
			bs3_curve cur
		);


// Return the parameter bounds of the nth simple span of the curve,
// where the argument n runs from 0 for the first span to one fewer
// than the number returned by bs3_curve_nspans.

/**
 * Gets the parameter bounds of the span specified.
 * <br><br>
 * <b>Role:</b> Returns the parameter bounds of the span specified, counting
 * from 0 to <tt>bs3_curve_nspans</tt> - 1, in increasing parameter order. If the integer
 * index is out of range, this function returns an empty interval.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param n
 * span number.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAinterval
bs3_curve_span_range(
			int n,				// span number
			bs3_curve cur
		);


// Construct a new curve which consists only of the nth simple span
// of the given curve.
/**
 * Creates a new B-spline %curve that is the nth span of the given B-spline %curve.
 * <br><br>
 * <b>Role:</b> Creates a new %curve that consists only of the nth simple span of
 * the given %curve. The knot vector of the new %curve will have start and end
 * multiplicities equal to the degree; therefore, the new %curve will represent a
 * single Bezier span. In the case of a rational %curve, the weights associated with
 * the start and end control points have not been normalized.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param n
 * given span index.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE bs3_curve
bs3_curve_span(
			int n,
			bs3_curve cur
		);


// On the assumption that a bs3_curve is a piecewise rational
// polynomial SPAvector function of its SPAparameter, convert the nth
// span into a rational polynomial SPAvector, with a normalised [ 0, 1 ]
// parametrisation.
/**
 * Gets the normalized rational polynomial for the indexed span of the given B-spline %curve.
 * <br><br>
 * <b>Role:</b> On the assumption that a <tt>bs3_curve</tt> is a piecewise rational
 * polynomial vector function of its parameter, convert the nth span into a
 * rational polynomial vector.
 * <br><br>
 * The parameter range of the polynomial is normalized to [0, 1], to minimize
 * numerical problems. The class <tt>rat_poly_vec</tt> is defined in <tt>kernutil/poly/poly.hxx</tt>,
 * but can be treated roughly as four arrays of doubles, each being the
 * coefficients of the polynomial in order of increasing degree.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param n
 * given span index.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE rat_poly_vec
bs3_curve_span_poly(
			int n,
			bs3_curve cur
		);


// **** Evaluation ****

// Return the number of derivatives that bs2_curve_evaluate can calculate.
/**
 * Gets the number of derivatives that <tt>bs3_curve_evaluate</tt> can calculate.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param giv_curve
 * given %curve.
 **/
DECL_SPLINE int
bs3_curve_accurate_derivs(
			bs3_curve giv_curve
		);

// Return the parametric criterion used to decide whether a given
// SPAparameter is a knot (for the purposes of choosing between
// discontinuous "sided" derivatives).

/**
 * Gets the parametric criterion used to determine whether a given parameter is a knot.
 * <br><br>
 * <b>Role:</b> For the purposes of choosing between discontinuous "sided"
 * derivatives.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/
DECL_SPLINE double
bs3_curve_knottol();

// General evaluator, giving an arbitrary number of derivatives (up
// to a maximum returned by "accurate_derivs"), and selection of the
// handedness of derivatives at discontinuities. Returns the number
// of derivatives actually evaluated - any requested but beyond the
// maximum are set to zero.
/**
 * Calculates an arbitrary number of derivatives of a B-spline %curve.
 * <br><br>
 * <b>Role:</b> General evaluator, giving an arbitrary number of derivatives (up
 * to a maximum returned by <tt>accurate_derivs</tt>), and selection of the handedness of
 * derivatives at discontinuities. This routine returns the number of derivatives
 * actually evaluated. Any derivatives requested but beyond the maximum are set to
 * 0.0.
 * <br><br>
 * <tt>deriv</tt> must point to an array of locations into which the calculated derivatives
 * are placed. It must contain at least <tt>nd</tt> pointers, but any pointer may be <tt>NULL</tt> to
 * indicate that that derivative is not required.
 * <br><br>
 * <tt>nd</tt> specifies the number of derivatives that should be calculated.
 * <br><br>
 * <tt>index</tt> is negative to evaluate the left-hand derivative at a knot and positive to
 * evaluate the right-hand derivative at a knot, or 0 for don't care about
 * discontinuities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * given parameter.
 * @param cur
 * given %curve.
 * @param pos
 * returned position.
 * @param vec
 * returned pointer to derivatives.
 * @param nd
 * number of derivatives to be evaluated.
 * @param index
 * -ve to evaluate the left-hand derivative at a knot, +ve to evaluate the right-hand derivative, 0 for "don't care"..
 **/
DECL_SPLINE int
bs3_curve_evaluate(
			double param,					// given SPAparameter t
			bs3_curve cur,				// given curve
			SPAposition &pos,				// SPAposition returned
			SPAvector * const *vec = NULL,
									// array of pointers to locations
									// in which derivatives are to be
									// placed. Must contain at least
									// nd pointers, but any may be NULL
									// to indicate that that derivative
									// is not required.
			int nd = 0,				// number of derivatives (nd) to be
									// evaluated.
			int index = 0					// -ve to evaluate the left-hand
									// derivative at a knot, +ve to
									// evaluate the right-hand derivative,
									// and 0 for "don't care".
		);

// Evaluate position and parametric derivatives of a given 3D B-spline
// curve at given parameter value. Only evaluates derivatives if
// requested.
/**
 * Evaluates the B-spline %curve and its parametric derivatives at the given parameter value.
 * <br><br>
 * <b>Role:</b> The number of derivatives evaluated depends upon the last two
 * arguments. If the last argument is not <tt>NULL</tt>, two derivatives will be evaluated.
 * If the last argument is <tt>NULL</tt> and the next-to-last argument is not <tt>NULL</tt>, one
 * derivative will be calculated. If both are <tt>NULL</tt>, no derivatives will be
 * calculated.
 * <br><br>
 * <b>Limitations:</b> This routine ignores the possibility of discontinuities. At a
 * discontinuity, values will be returned for either the left or the right side,
 * with no current guarantee of which.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * given parameter.
 * @param cur
 * given %curve.
 * @param x
 * returned position.
 * @param xdot
 * returned first derivative.
 * @param xdotdot
 * returned second derivative.
 **/
DECL_SPLINE void
bs3_curve_eval(
			double param,						// given SPAparameter value
			bs3_curve cur,					// given curve
			SPAposition &x,					// SPAposition returned
			SPAvector &xdot = *(SPAvector *)NULL_REF,	// first derivative returned
			SPAvector &xdotdot = *(SPAvector *)NULL_REF	// second derivative returned
		);


// Evaluate a SPAposition on a given 3D B-spline curve at given SPAparameter
// value.

/**
 * Evaluates the B-spline %curve at the given parameter value.
 * <br><br>
 * <b>Role:</b> Normally, this is implemented as a call to <tt>bs3_curve_eval</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * given parameter value.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAposition
bs3_curve_position(
			double param,				// given SPAparameter value
			bs3_curve cur			// given curve
		);


// Evaluate the parametric derivative (direction and magnitude) of
// a given 3D B-spline curve at a given parameter value.
/**
 * Evaluates the parametric derivative (direction and magnitude) of a given 3D B-spline %curve at a given parameter value.
 * <br><br>
 * <b>Role:</b> Normally, this is implemented as a call to <tt>bs3_curve_eval</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * given parameter value.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAvector
bs3_curve_deriv(
			double param,				// given SPAparameter value
			bs3_curve cur			// given curve
		);


// Evaluate the tangent direction to a given 3D B-spline curve at a
// given SPAparameter value.
/**
 * Determines the B-spline %curve direction at the given parameter value.
 * <br><br>
 * <b>Role:</b> Normally, this normalizes the result of <tt>bs3_curve_deriv</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param param
 * given parameter value.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAunit_vector
bs3_curve_tangent(
			double param,				// given SPAparameter value
			bs3_curve cur			// given curve
		);


// Evaluate the curvature of a 3D B-spline curve at a
// given SPAparameter value.
/**
 * Evaluates the curvature of the B-spline %curve at the given parameter value.
 * <br><br>
 * <b>Role:</b> The direction of the vector is from the point on the %curve
 * towards the center of curvature, and the magnitude is the curvature. This is not
 * a true Euclidean vector, because it does not transform correctly under scaling.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * given parameter value.
 * @param cur
 * given %curve.
 **/
DECL_SPLINE SPAvector
bs3_curve_curvature(
			double param,				// given SPAparameter value
			bs3_curve cur			// given curve
		);

// STI mnl begin: moved prototype for ACIS2.0
// New Code for more accurate second partials on derived surface types.
// Evaluate the n'th derivative of a given bs3_curve at a given
// SPAparameter value.  The derivative is returned.  This routine is
// mostly used for computing the 3rd derivative of a bs3_curve or up.
/**
 * Evaluates the <i>n</i>th derivative of a given <tt>bs3_curve</tt> at a given parameter value.
 * <br><br>
 * <b>Role:</b> The derivative is returned. This routine is mostly used for
 * computing the third derivative or higher of a <tt>bs3_curve</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * parameter at which derivative is desired.
 * @param cur
 * given %curve.
 * @param nder
 * derivative number.
 **/
DECL_SPLINE SPAvector
bs3_curve_nderiv(
			double param,          // Parameter at which the deriv is desired
			bs3_curve cur,        // given curve
            int nder              // derivative number
        );

// **** Geometric Properties ****

// Find the parameter of a point on a 3D B-spline curve.
// Returns an error if point is not within given tolerance of curve.
// If a parameter value close to that deired is known, it may be given
// as the last argument (ignored at present).
/**
 * Determines the parameter value of the closest point on the bs3_curve to the given point.
 * <br><br>
 * <b>Role:</b> If a guess for the parameter value is supplied, it is assumed to
 * be a good one, so that a close local minimum of distance may be determined. If
 * there is no guess, it is assumed that there is at most one point of minimum
 * distance within the specified tolerance. If the given point is near one end of the
 * %curve, then the end point itself may be the near point.
 *<p>
 * Note: if the given point happens to lie on the bs3_curve, this function is the inverse of position
 * evaluation (within <tt>SPAresabs</tt>), hence the name bs3_curve_invert.
 *</p>
 * <br><br>
 * <b>Errors</b>: 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param pos
 * given point.
 * @param given_tol
 * unused.
 * @param cur
 * given %curve.
 * @param param_guess
 * guess parameter guess parameter.
 **/
DECL_SPLINE double
bs3_curve_invert(
			SPAposition const &pos,	// given point
			double given_tol,				// given tolerance
			bs3_curve cur,			// given curve
			SPAparameter const &param_guess = *(SPAparameter *)NULL_REF	// SPAparameter guess
		);


// Drop a perpendicular from a point to a spline curve, returning
// the foot of the perpendicular, and the tangent direction and
// parameter value at that point.
/**
 * Gets the position, tangent direction, and parameter value at the foot of a 
 * perpendicular dropped from a point to the B-spline %curve.
 * <br><br>
 * <b>Role:</b> All return parameters are optional (i.e., they may be <tt>NULL</tt>
 * references, meaning that the value is not required). If an approximate parameter
 * is supplied, the returned point will be the one near (in some sense) to the
 * specified parameter. If not, it will be the point nearest in object space to the
 * given one.
 * <br><br>
 * For an open %curve, there may be no perpendicular at all. In this case, one end
 * of the %curve is returned and the tangent and parameter at that point are
 * returned as usual.
 * <br><br>
 * If a non-zero <tt>quick_exit_dist_tol</tt> is supplied, this value is used to stop the
 * point perp iteration as soon as the near pt and <tt>pos</tt> are within
 * <tt>quick_exit_dist_tol</tt> of each other.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param pos
 * given point.
 * @param cur
 * given %curve.
 * @param foot
 * returned foot of perpendicular.
 * @param tan
 * returned %curve tangent.
 * @param param_guess
 * guess value for parameter.
 * @param param_actual
 * returned actual parameter used.
 * @param quick_exit_dist_tol
 * if non-zero, used to stop iteration when foot is closer than tol to pos.
 **/
DECL_SPLINE void
bs3_curve_perp(
			SPAposition const &pos,	// given point
			bs3_curve cur,			// curve
			SPAposition &foot,			// (returned) foot of perpendicular
			SPAunit_vector &tan,		// (returned) curve tangent
			SPAparameter const &param_guess = *(SPAparameter *)NULL_REF,
								// supplied approximate SPAparameter
			SPAparameter &param_actual = *(SPAparameter *)NULL_REF,
								// (returned) actual SPAparameter
            double quick_exit_dist_tol = 0.0        // distance to curve stopping tolerance
		);

// Find the closest point on a spline curve to a given point, returning
// the foot (i.e. closest point), and the SPAparameter value at that point.

// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void
bs3_curve_closest_point(
			SPAposition const &,	// given point
			bs3_curve,			// curve
			SPAposition &,			// (returned) foot of perpendicular
			SPAparameter const & = *(SPAparameter *)NULL_REF,
								// supplied approximate SPAparameter
			SPAparameter & = *(SPAparameter *)NULL_REF,
								// (returned) actual SPAparameter
            double = 0.0        // distance to curve stopping tolerance
		);


// Test whether a point lies within tolerance of a 3D B-spline curve.
/**
 * Determines whether a point lies within tolerance of a three-dimensional B-spline %curve.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param pos
 * given point.
 * @param tol
 * given tolerance.
 * @param cur
 * given %curve.
 * @param param_guess
 * guess value for parameter returned.
 * @param param_exact
 * set to exact parameter value.
 **/
DECL_SPLINE logical
bs3_curve_testpt(
			SPAposition const &pos,	// given point
			double tol,				// given tolerance
			bs3_curve cur,			// given curve
			SPAparameter const &param_guess = *(SPAparameter *)NULL_REF,
								// approximation to SPAparameter value
			SPAparameter &param_exact = *(SPAparameter *)NULL_REF
								// set to exact SPAparameter value
		);


// Find the length of a 3D B-spline curve between given SPAparameter
// bounds.
/**
 * Determines the arc length of a three-dimensional B-spline %curve between given parameter bounds.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 * @param cur_range
 * optional parametric bounds.
 * @param appr_len
 * TRUE to get approximate length quickly (length of control polygon).
 **/
DECL_SPLINE double
bs3_curve_length(
			bs3_curve cur,			// Given curve
			SPAinterval const &cur_range = *(SPAinterval *)NULL_REF,
								// Optional parametric bounds
			logical appr_len = FALSE		// TRUE to get approximate length
								// quickly (e.g. length of control
								// polygon)
		);


// Find the signed arc length of the curve between two SPAparameter
// values. The length will be positive if the second SPAparameter
// is greater than the first, and negative if it is less, provided
// both parameters are valid for the curve.

/**
 * Gets the signed arc length of the %curve between two parameter values.
 * <br><br>
 * <b>Role:</b> The length will be positive if the second parameter is greater
 * than the first, and negative if it is less, provided both parameters are valid
 * for the %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 * @param start
 * parameter of start point.
 * @param end
 * parameter of end point.
 **/
DECL_SPLINE double
bs3_curve_param_length(
			bs3_curve cur,			// Given curve
			double start,				// SPAparameter of start point
			double end				// SPAparameter of end point
		);


// Find the SPAparameter value of the point at a given arc length from
// the given SPAparameter value. Acts as an inverse to
// bs3_curve_param_length.
/**
 * Determines the parameter value of the point at a given arc length from the given parameter value.
 * <br><br>
 * <b>Role:</b> This routine acts as an inverse to <tt>bs3_curve_param_length</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 * @param start
 * parameter to datum point.
 * @param length
 * arc length, possibly the negative of the desired point.
 **/
DECL_SPLINE double
bs3_curve_length_param(
			bs3_curve cur,			// Given curve
			double start,				// SPAparameter of datum point
			double length 				// arc length (possibly negative) of
								// desired point
		);


// Find the extrema of a 3D B-spline curve in a given direction.
/**
 * Determines the extreme points (maxima and minima) of a parametric %curve with respect to a given direction in object space.
 * <br><br>
 * <b>Role:</b> The return value is a linked list of <tt>curve_extremum</tt> objects,
 * each of which contains the parameter value of an extremum, together with a
 * classification of whether it is a maximum or a minimum. The list is returned in
 * increasing order of parameter.
 * Note that the caller takes ownership of the returned <tt>curve_extremum</tt>, and must delete the memory as follows:
 *
 *	curve_extremum *extrema = bs3_curve_extrema( my_bs3, SPAunit_vector(1,0,0) );
 *	// Code for processing the extrema goes here
 *	// ...
 *	// Now delete the curve_extremum.
 *	while ( extrema != NULL )
 *	{
 *		curve_extremum *next = extrema->next;
 *		ACIS_DELETE extrema;
 *		extrema = next;
 *	}
 *
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param cur
 * given %curve.
 * @param dir
 * given direction.
 **/
DECL_SPLINE curve_extremum *
bs3_curve_extrema(
			bs3_curve cur,			// given curve
			SPAunit_vector const &dir	// given direction
		);


// Return a cone bounding the tangent direction of a curve. The
// cone is deemed to have its apex at the origin, and has a given
// axis direction and (positive) half-angle. If the logical
// argument is TRUE, then a quick approximation is found. The
// resulting cone is guaranteed to be wholly within the one found
// with a FALSE argument, but is not guaranteed to be a true bound -
// the curve tangent could lie outside it at some parts of the
// curve.

// For internal use only
/**
 * @nodoc
 */
class DECL_SPLINE bs3_curve_tancone : public ACIS_OBJECT {
public:
	SPAunit_vector axis;
	double angle;

	bs3_curve_tancone( SPAunit_vector const &ax, double ang ) {
		axis = ax;
		angle = ang;
	}
};

/**
 * Creates a cone bounding the tangent directions of a %curve.
 * <br><br>
 * <b>Role:</b> A tangent cone is a cone that contains all the tangent vectors
 * of a %curve. If the %curve is a %straight line, the cone would be directed along
 * the line and having a very small angle would contain all the tangent vectors of
 * the line; in fact, the angle could be 0. All the tangent go in the same
 * direction. If the %curve was almost a %straight line, the cone axis could
 * approximate the direction of the %curve somehow, and the cone's angle would need
 * to be a little bigger. The more the %curve curves, the bigger the cone angle. If
 * the %curve were a complete circle, the %cone would need to encompass every
 * direction (even though it lies in a %plane), so the cone's half angle would be <i>pi</i>.
 * If the %curve were a %helix, the cone's half angle would be slightly less than
 * <i>pi</i>/2, depending on the pitch and radius of the %helix.
 * <br><br>
 * If the logical <tt>approx</tt> argument is <tt>TRUE</tt>, a quick approximation is found. The
 * resulting cone is guaranteed to be wholly within the one found with a <tt>FALSE</tt>
 * argument, but is not guaranteed to be a true bound - the %curve tangent could lie
 * outside it at some parts of the %curve.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 * @param approx_OK
 * make a quick approximation.
 **/
DECL_SPLINE bs3_curve_tancone
bs3_curve_tangent_cone(
			bs3_curve cur,
			logical approx_OK = FALSE
		);


// Return the (local space) bounding SPAbox for the given 3D spline
// curve, expanded by a given tolerance (in case the curve is only
// a fit to a true curve).
/**
 * Determines a box that encloses the B-spline %curve with additional tolerances.
 * <br><br>
 * <b>Role:</b> Returns a <tt>box</tt> (presently a rectangular parallelepiped parallel
 * to object-space axes) that completely contains the %curve, with an additional
 * allowance of the given tolerance all round. The box will not be the smallest
 * possible, but will be a compromise between a tight fit and fast evaluation.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cur
 * given %curve.
 * @param fitol
 * given fit tolerance.
 **/
DECL_SPLINE SPAbox
bs3_curve_box(
			bs3_curve cur,			// given curve
			double fitol				// given tolerance
		);


// Test whether two spline curves are (apparently) the same. This is
// not a comprehensive test - curves which are coincident but have
// differing extents, knot vectors or other internal details will not
// be spotted. The routine should be used only as a (fairly) cheap
// filter.
/**
 * Determines whether two B-spline curves are the same.
 * <br><br>
 * <b>Role:</b> This routine checks that the two curves share the same form, the
 * same knot vectors, and the same control points (within tolerance). This routine
 * is not to be used for coincidence testing, but only as a simple filter to
 * discard obvious cases.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param bs1
 * first %curve.
 * @param bs2
 * second %curve.
 * @param tol
 * parameter space tolerance for equal control points.
 **/
DECL_SPLINE logical
bs3_curve_same(
			bs3_curve bs1,			// first curve for comparison
			bs3_curve bs2,			// second curve
			double tol = 0.0		// tolerance on control point positions
		);


// **** Intersections ****

// OBSOLETE (9/97):  This function will be deleted in a future version of the
// software.  It is recommended that users of this function intersect intcurves
// instead of the underlying bs3_curves.

// Intersect two spline curves
/**
 * Intersects two %curves.
 * <br><br>
 * <b>Role:</b> If the curves pass within the tolerance of each other, an
 * intersection is produced. Intersections are returned in an undefined order.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cur1
 * first given %curve.
 * @param cur2
 * second given %curve.
 * @param tol
 * given fit tolerance.
 **/
[[deprecated("Will be deprecated soon : Use api_intersect_curves instead")]]
DECL_SPLINE curve_curve_int *
bs3_curve_bs3_curve_int(
			bs3_curve cur1,			// first given curve
			bs3_curve cur2,			// second given curve
			double tol 				// given fit tolerance
		);


// **** Utility ****

// Duplicate a 3D B-spline curve (in free store).
/**
 * Creates an exact copy of the B-spline %curve in free store.
 * <br><br>
 * <b>Role:</b> ACIS calls this routine only when a change is to be made to one
 * copy of the %curve, so there is no advantage to be gained by deferring the
 * duplication further. Ordinary duplication of ACIS intersection curves merely
 * creates a new reference to the same underlying <tt>bs3_curve</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE bs3_curve
bs3_curve_copy(
			bs3_curve cur			// given curve
		);


// Remove a 3D B-spline curve from free store.
/**
 * Deletes storage occupied by the given B-spline %curve that is no longer required.
 * <br><br>
 * <b>Role:</b> ACIS makes no assumptions about how the underlying %surface
 * package manages its storage space if it doesn't incapacitate the standard C
 * memory allocation mechanism.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE void
bs3_curve_delete(
			bs3_curve &cur		// given curve
		);


// Save a 3D B-spline curve to disc in recoverable format.
/**
 * Saves a B-spline %curve to a file.
 * <br><br>
 * <b>Role:</b> Writes a representation of the parametric %curve to some external
 * medium, using routines <tt>write_int</tt>, <tt>write_long</tt>, <tt>write_real</tt>,
 * and <tt>write_string</tt>, defined in ACIS file <tt>kernutil/fileio/fileio.hxx</tt>.
 * <br><br>
 * The overloaded <tt>&lt;&lt;</tt> operator acts like <tt>bs3_curve_save</tt>, but writes to a C++ style
 * stream using stream operators <tt>streams/streams.hxx</tt>. The output format need not
 * necessarily be the same as for <tt>bs3_curve_save</tt>, but it is strongly recommended
 * that they be so.
 * <br><br>
 * bs3_curve cur;
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 **/
DECL_SPLINE void
bs3_curve_save(
			   bs3_curve cur			// given curve
			   );


// Restore a 3D B-spline curve from disc.
/**
 * Restores a B-spline %curve from a file.
 * <br><br>
 * <b>Role:</b> Reads back a representation of a parametric %curve written by
 * <tt>bs3_curve_save</tt> and constructs a duplicate of the original %curve. Reading uses
 * routines <tt>read_int</tt>, <tt>read_long</tt>, <tt>read_real</tt>, and <tt>read_string</tt> defined in
 * <tt>kernutil/fileio/fileio.hxx</tt>.
 * <br><br>
 * The overloaded <tt>&gt;&gt;</tt> operator acts like <tt>bs3_curve_restore</tt>, but reads from a C++
 * style stream using stream operators, and sets the result into the second argument.
 * <br><br>
 * <tt>bs_2_3_spline_restore</tt>&nbsp;&nbsp;&nbsp;&nbsp;Restore spline
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_SPLINE bs3_curve
bs3_curve_restore();


// Produce a simple-minded printout of a 3D B-spline curve
/**
 * Gets a readable representation of the B-spline %curve and writes it to a file.
 * <br><br>
 * <b>Role:</b> If there is more than one text line (as is almost certain), all
 * lines but the first will start with the leader string. Do not use a <tt>newline</tt> to
 * terminate the last line.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param cur
 * given %curve.
 * @param leader
 * leader string.
 * @param fp
 * output file.
 **/
DECL_SPLINE void
bs3_curve_debug(
			bs3_curve cur,			// given curve
			char const *leader,
			FILE *fp = debug_file_ptr
		);


// STI dgh }		// End 

/**
 * Sets the fit tolerance of a %curve.
 * <br><br>
 * <b>Role:</b> Estimates a fit tolerance for interpolating the given set of
 * initial points. It takes an initial value from the <tt>fitol_curve_interp</tt> option if
 * this is positive, otherwise it starts with <tt>SPAresfit</tt>. Then it estimates the
 * maximum curvature by fitting a circle to each consecutive set of three points,
 * and, if necessary reduces the fit tolerance to an order of magnitude smaller
 * than the corresponding minimum radius of curvature. As a sanity check, it never
 * returns less than <tt>SPAresabs</tt>.
 * <br><br>
 * If <tt>fit_tol</tt> been set in the <tt>fitol_curve_interp</tt> option, that is used as a starting
 * value, otherwise <tt>opt_tol</tt> is used if supplied. Otherwise the starting value is
 * <tt>SPAresfit</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param npoints
 * number of points.
 * @param pos
 * array of object space positions.
 * @param opt_tol
 * starting value.
 **/
DECL_SPLINE double
bs3_curve_set_fitol(
            int npoints,            	// number of points
            SPAposition const *pos,   // array of object space points
            double opt_tol = -1.0		// optional starting value
        );
// STI mnl end

// Needed for deformable surface code

/**
 * Sets the position of one control point.
 * <br><br>
 * <b>Role:</b> Checks if <tt>curv</tt> has an <tt>index</tt> control point. If it does it
 * copies the <i>xyz</i> values of <tt>pos</tt> into control point's data structure. When <tt>curv</tt> is
 * rational it also copies the <tt>weight</tt> value into the control point's data structure.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param curv
 * bs3_curve to modify.
 * @param index
 * index of control point.
 * @param pos
 * xyz location copied into control point, size [3].
 * @param weight
 * weight which control point is assigned, only used if curv is rational.
 **/
DECL_SPLINE void
bs3_curve_set_ctrlpt(    // eff: set the SPAposition of one cpt
  		bs3_curve curv,       // in : tgt bs3_curve to modify
   		int index,             // in : index of tgt control point
   		double *pos,        // in : [xyz] loc copied into cpt,size:[3]
    	double weight,          // in : weight to which cpt is assigned
                         //      only used if curv is rational
		const logical use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
		);

/**
 * Sets the position of all control points.
 * <br><br>
 * <b>Role:</b> Iterates through all control points on <tt>curv</tt>. Copies the values
 * of <tt>pos</tt> into the control points and if <tt>curv</tt> is rational also copies weight values.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param curv
 * tgt bs3_curve to modify.
 * @param cpt_count
 * number of control points.
 * @param pos
 * [xyz0,xyz1..] to copy sized:[3*cpt_count].
 * @param weight
 * weights for each cpt. only used if curv is rational sized:[cpt_count].
 **/
DECL_SPLINE void
bs3_curve_set_ctrlpts(   // eff: set the SPAposition of all cpts
  		bs3_curve curv,  // in : tgt bs3_curve to modify
   		int cpt_count,   // in : number of control points
   		double *pos,     // in : [xyz0,xyz1..] to copy sized:[3*cpt_count]
   		double *weight,  // in : weights for each cpt. only used if
                         //      curv is rational sized:[cpt_count]
		const logical use_bs3_seam_data = FALSE // in: used for periodic geometry when initial knot multiplicty != order
		);

/**
* Snaps the input bs3_curve to given set of positions with minimal shape deformation.
* <br><br>
* <b>Role:</b> Modifies the input bs3_curve so that it passes through the given list of positions ( <tt>target_points</tt> ) 
* keeping the original shape as much as possible. Returns FALSE on failure.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param crv
* input bs3_curve to snap.
* @param params
* curve parameters corresponding to input list of positions.
* @param target_points
* Target positions for the curve to snap to.
* @param npts
* number of input positions.
**/
DECL_SPLINE logical 
bs3_curve_snap_to_pts( 
		bs3_curve crv,
		double const * params,
		SPAposition const * target_points,
		int npts 
        );

// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void
bs3_set_box_bs( ag_spline* bs );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE ag_spline*
bs3_bld_bs( ag_spline* next, ag_spline* prev,
	int ctype, int degree, int num, int rat, int form );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical
bs3_rem_kn_red( ag_spline* bs, double tol );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical
bs3_rem_knot( ag_spline* bs, double tol, logical remove );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE double
bs3_len_bs( ag_spline* bs, double tol, int &err );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE double*
bs3_al_dbl( int n );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void
bs3_V_copy( double* P1, double* P2, int dim );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int
bs3_dal_mem( void** SPAptr, unsigned int len );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int
bs3_Bez_ret( ag_spline** bs );


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int
bs3_db_bs( ag_spline** bs );


class chain_curve_options_impl;
/**
* @file sp3crtn.hxx
* @CAA2Level L1
* @CAA2Usage U1
*/


/** \addtogroup BS3CURVEAPI
*  \brief Declared at <sp3crtn.hxx>, SPAkern
*  @{
*/

/**
* Represents common data and functionality for curve chain options.
* <br>
* <b>Role:</b> <tt>chain_curve_options</tt> is the generic class to be used
* in <tt>api_bs3_curve_from_chain</tt>.
* <br><br>
* @see api_bs3_curve_from_chain
*/
class DECL_SPLINE chain_curve_options : public ACIS_OBJECT 
{
public:
	/**
	* @nodoc
	*/
	enum chain_ext_type
	{
		CHAIN_NATURAL_EXT,
		CHAIN_TANGENT_EXT
	};
	/**
	* Constructor.
	* <br><br>
	* <b>Role:</b> Creates a chain_curve_options object.
	* <br><br>
	*/
	chain_curve_options();
	/**
	* Destructor.
	* <br><br>
	* <b>Role:</b> Deletes chain_curve_options object.
	* <br><br>
	*/
	~chain_curve_options();
	/**
	* Sets the closed form.
	*/
	void set_closure(closed_forms cl);
	/**
	* @nodoc
	*/
	void set_start_ext(chain_ext_type s_ext);
	/**
	* @nodoc
	*/
	void set_end_ext(chain_ext_type e_ext);
	/**
	* Sets the point tolerance.
	*/
	void set_pt_coin_tol(double tol);
	/**
	* Sets the fit tolerance.
	*/
	void set_fit_tol(double tol);
	/**
	* Sets the arclen re-parameterization mode.
	*/
	void set_arclen_reparam(bool arclen);
	/**
	* Returns the closed form.
	*/
	closed_forms closure() const;
	/**
	* @nodoc
	*/
	chain_ext_type start_ext() const;
	/**
	* @nodoc
	*/
	chain_ext_type end_ext()const;
	/**
	* Returns the point tolerance
	*/
	double pt_coin_tol()const;
	/**
	* Returns the fit tolerance
	*/
	double fit_tol()const;
	/**
	* Returns the arclen re-param mode.
	*/
	bool arclen_reparam()const;
	/**
	* @nodoc
	*/
	void get_info(char const *& closure_info, char const *& start_ext_info, char const*& end_ext_info)const;

private:
	chain_curve_options_impl *_cco;
};

/** \addtogroup BS3CURVEAPI
*  \brief Declared at <sp3crtn.hxx>, SPAkern
*  @{
*/

/**
* Takes a list of curves that form a chain and returns one C2-smooth curve that is approximately arclength parameterized by default.
* <b>Role:</b> Takes a list of curves that form a chain and links them together to create one B-Spline curve.
* The input list of curves must be ordered in the sense that the first edge links to the second, and the second
* to the third, etc.
* The process generally works by fitting a bs3_curve to the each curve in the list and arclength parameterizing the approximation.  These
* bs3_curves are joined to form one curve and all non-C1 and non-C2 discontinuities are removed with a smoothing algorithm.
* <br><br>
* The <tt>SPAinterval</tt> argument is used to define the interval for the input curves.It must be ensured that there is one-to-one correspondence between the curves and their intervals.
* <br><br>
* The <tt>chain_curve_options</tt> argument is used to define the options for the operations.
* <br>
* <ul>
* <li>The <tt>arclen</tt> option specifies whether to perform arclength re-parameterization of bs3_curve approximations. Default value is TRUE.</li>
* <li>The <tt>closure</tt> option determines how to deal with a chain that forms a closed loop.</li>
* <ul>
* <li>If set to <tt>periodic</tt>, closed loops are smoothened at the seam and the resulting curve is periodic.</li>
* <li>If set to <tt>closed</tt>, the seam is not smoothened.</li>
* </ul>
* <li>If the loop is not closed, the <tt>closure</tt> option must be set to <tt>open</tt> (default).</li>
* <li>If the closure is set to </tt>unset</tt>, then the algorithm will compute the appropriate closure property.</li>
* <li>The <tt>pt_tol</tt> option sets the tolerance within which the end points of curves are considered to be coincident. Default value is SPAresfit.</li>
* <li>The <tt>fit_tol</tt> option specifies the tolerance to be used when creating the bs3_curve approximations to the original curves. Default value is 100*SPAresabs.</li>
* </ul>
* <br>
* <b>Topology Changes:</b>
* None.
* <br><br> 
* <b>Geometry Changes:</b>
* Creates a new bs3 curve in the model.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param num_curves
* number of curves.
* @param chain
* list of curves.
* @param intervals
* limits of the curve.
* @param opts
* options object which controls closure,point coincidence tolerance and fit tolerance. NULL argument will set the default values.
* @param ao
* ACIS options
**/
DECL_SPLINE outcome
api_bs3_curve_from_chain(int const num_curves, curve const* const* chain, 
					     bs3_curve & bs3_from_chain,
						 SPAinterval const *intervals=NULL,chain_curve_options const* opts=NULL, 
	                     AcisOptions* ao = NULL);

/** @} */

#endif
