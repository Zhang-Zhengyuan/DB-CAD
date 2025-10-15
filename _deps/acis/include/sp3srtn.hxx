/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Routines for processing 3D B-spline surfaces.
/*******************************************************************/
#if !defined( BS3_SURFACE_ROUTINES )
#define BS3_SURFACE_ROUTINES
#include "dcl_spl.h"
#include "logical.h"
#include "bs3surf.hxx"
#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "debugmsc.hxx"
#include "unitvec.hxx"
/**
 * @file sp3srtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3SURFACEAPI
 *
 * @{
 */
class curve;
class straight;
class surface;
class plane;
class cone;
class sphere;
class torus;
class spline;
class SPAinterval;
class SPAbox;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class SPApar_pos;
class SPApar_vec;
class SPApar_dir;
class SPApar_box;
class SPApar_transf;
class rat_bipoly_vec;
class surf_surf_int;
class view_spec;
class law;
#if defined( osf1 )
#include "position.hxx"
#endif
/**
* Initializes the B-spline %surface system.
* <br><br>
* <b>Effect:</b> System routine
**/
DECL_SPLINE void bs3_surface_init();
/**
 * @nodoc
 * Shut things down (e.g. release caches of free store).
 */
DECL_SPLINE void bs3_surface_term();
// **** Construction ****
/**
* Converts a portion of a general ACIS %surface into a B-spline %surface.
* <br><br>
* <b>Role:</b> Converts a general %surface into a B-spline. There is no guarantee
* that the parameterization of the B-spline will have any obvious relationship to
* that of the original %surface.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original %surface.
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sur
* given %surface.
* @param region_of_interest
* region of interest.
* @param requested_fit
* requested fit tolerance.
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_sur(
			surface const &sur,		// given surface
			SPAbox const &region_of_interest,			// region of interest
			double requested_fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &pt = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


// Convert a plane into a B-spline surface. Tolerance values are
// supplied for consistency, but it is very unlikely that they will
// ever be needed.
/**
* Creates a B-spline %surface from the given plane.
* <br><br>
* <b>Role:</b> Creates a parametric %surface coincident with the given plane
* over at least the portion inside the box, within the specified positional
* tolerance, and has its normal in the same sense.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original plane.
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param pl
* given plane.
* @param region_of_interest
* region of interest.
* @param positional_fit
* required positional fit.
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_pla(
			plane const &pl,			// given plane
			SPAbox const &region_of_interest,			// region of interest
			double positional_fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &pt = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


// Convert a cone into a B-spline surface. This will match the given
// (possibly elliptical) cone everywhere within the given region of
// interest, within the specified positional precision. If the actual
// precision achieved is better than system positional accuracy (for
// example if the surface package supports rational quadratic
// polynomial surfaces) then the "actual tolerance" will be returned
// as exactly zero.
/**
* Creates a parametric %surface coincident with the given cone.
* <br><br>
* <b>Role:</b> Creates a parametric %surface coincident with the given (possibly
* elliptical) cone over at least the portion inside the box, within the specified
* positional tolerance, and has its normal in the same sense.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original cone.
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param c
* given cone.
* @param region_of_interest
* region of interest.
* @param positional_fit
* required positional fit.
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_con(
			cone const &c,			// given cone
			SPAbox const &region_of_interest,			// region of interest
			double positional_fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &pt = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


// Convert a sphere into a B-spline surface. This will match the given
// sphere everywhere within the given region of interest, within the
// specified positional precision. If the actual precision achieved
// is better than system positional accuracy (for example if the
// surface package supports rational quadratic polynomial surfaces)
// then the "actual tolerance" will be returned as exactly zero.

/**
* Creates a B-spline %surface from the given sphere.
* <br><br>
* <b>Role:</b> Creates a parametric %surface coincident with the given sphere
* over at least the portion inside the box, within the specified positional
* tolerance, and has its normal in the same sense.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original sphere.
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sph
* given sphere.
* @param region_of_interest
* region of interest.
* @param fit_tol
* requested fit tolerance.
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_sph(
			sphere const &sph,			// given sphere
			SPAbox const &region_of_interest,			// region of interest
			double fit_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &pt = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


// Convert a torus into a B-spline surface. This will match the given
// torus everywhere within the given region of interest, within the
// specified positional precision. If the actual precision achieved
// is better than system positional accuracy (for example if the
// surface package supports rational quadratic polynomial surfaces)
// then the "actual tolerance" will be returned as exactly zero.
/**
* Creates a B-spline %surface from the given torus.
* <br><br>
* <b>Role:</b> Creates a parametric %surface coincident with the given torus
* over at least the portion inside the box, within the specified positional
* tolerance, and has its normal in the same sense.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original torus.
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param tor
* given torus.
* @param region_of_interest
* region of interest.
* @param fit_tol
* requested fit tolerance.
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_tor(
			torus const &tor,			// given torus
			SPAbox const &region_of_interest,			// region of interest
			double fit_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &pt = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


/**
* Creates a B-spline %surface approximating the given spline %surface.
* <br><br>
* <b>Role:</b> Creates a B-spline %surface approximating the given spline
* over at least the portion inside the box, within the returned actual
* tolerance, and which has its normal in the same sense.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exactly zero if this is better than system positional tolerance (<tt>SPAresabs</tt>). 
* Note that this may be a geometric tolerance and not a parametric tolerance.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param spl
* given B-spline.
* @param reg_int
* region of interest.
* @param fit_tol
* NOT USED.
* @param actual_fit
* returns the actual tolerance achieved by the approximation. (NOTE: may not be parametric.)
* @param space_mapping
* NOT USED.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_spl(
			spline const &spl,			// given spline
			SPAbox const &reg_int,			// region of interest
			double fit_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			SPApar_transf &space_mapping = *(SPApar_transf*)NULL_REF
									// returns SPAparameter space mapping
		);


// Construct a B-spline surface by summing two 3D B-spline curves. The
// start points of the two curves need not match, in which case the
// first curve is used to define the low v SPAparameter line of the
// surface. All other SPAparameter lines are translations of the given
// curves. It is unlikely that the precision arguments will ever be
// neeed, but they are included here for consistency.

/**
* Creates a B-spline %surface by summing two 3D B-spline curves.
* <br><br>
* <b>Role:</b> Constructs a B-spline %surface from two B-spline curves, as the
* simple sum. The second %curve is translated so that its start point sweeps along
* the first %curve, and the %surface is what gets swept out. The resulting <i>u</i>
* parameter curves are all translations of the first %curve, <i>v</i>-parameter curves are
* translations of the second.
* <br><br>
* The start points of the two curves need not match, in which case the first %curve
* is used to define the low <i>v</i> parameter line of the %surface. All other parameter
* lines are translations of the given curves. It is unlikely that the precision
* arguments will ever be need, but they are included here for consistency.
* <br><br>
* If the <i>u</i>-direction %curve or the <i>v</i>-direction %curve are closed or periodic, the
* resulting %surface is closed or periodic in that parameter. Both curves must <i>not</i>
* be closed or periodic, because the resulting %surface would be self-intersecting.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param u_bs
* %curve in u direction.
* @param v_bs
* %curve in v direction.
* @param fit_tol
* requested fit tolerance.
* @param actual_fit
* returned actual fit tolerance used.
**/
DECL_SPLINE bs3_surface
bs3_surface_sum_curves(
			bs3_curve u_bs,				// curve in u direction
			bs3_curve v_bs,				// curve in v direction
			double fit_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// returns actual fit tolerance
		);


// Construct a B-spline surface by revolving a 3D B-spline curve about
// an axis. The curve need not be planar. The generating curve defines
// the zero angular SPAposition, from which the "start" and "stop" angles
// are calculated, clockwise around the given axis. If the angles are
// equal, the curve is swept around a full circle. The result will
// match the true surface of revolution within the specified
// positional precision. If the actual precision achieved is better
// than system positional accuracy (for example if the surface package
// supports rational quadratic polynomial surfaces) then the "actual
// tolerance" will be returned as exactly zero.

/**
* Creates a B-spline %surface by revolving a three-dimensional B-spline %curve about an axis.
* <br><br>
* <b>Role:</b> The %curve need not be planar. The generating %curve defines the
* zero angular position, from that the start and stop angles are calculated,
* clockwise around the given axis. If the angles are equal, the %curve is swept
* around a full circle. The result will match the true %surface of revolution
* within the specified positional precision. If the actual precision achieved is
* better than system positional accuracy (for example if the %surface package
* supports rational quadratic polynomial surfaces) then the actual tolerance will
* be returned as exactly zero.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param gen
* generator %curve.
* @param axis
* axis.
* @param start_ang
* start angle.
* @param end_ang
* stop angle.
* @param fit_tol
* requested fit tolerance.
* @param actual_fit
* returns actual fit tolerance used.
**/
DECL_SPLINE bs3_surface
bs3_surface_revolve_curve(
			bs3_curve gen,				// generator curve
			straight const&axis,		// axis
			double start_ang = 0.0,			// start angle
			double  end_ang = 0.0,			// stop angle
			double fit_tol = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// returns actual fit tolerance
		);


// Construct a B-spline surface which is an approximation to a constant-
// radius circular pipe centred on the given spine curve. The second
// curve gives the direction at each point of the zero cross-section
// SPAparameter (the u direction), and the SPAinterval gives the SPAparameter
// (angle) range required in the cross direction. The u SPAparameter
// increases clockwise around the spine direction for a positive radius
// (convex surface) and anticlockwise for a negative radius (concave
// surface.
/**
* Creates a B-spline %surface that is an approximation to a constant-radius circular pipe centered on the given spine %curve.
* <br><br>
* <b>Role:</b> The second %curve gives the direction at each point of the zero
* cross-section parameter (the <i>u</i>-direction), and the interval gives the parameter
* (angle) range required in the cross direction. The <i>u</i>-parameter increases
* clockwise around the spine direction for a positive radius (convex %surface) and
* counterclockwise for a negative radius (concave %surface).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param radius
* signed radius.
* @param spine
* spine %curve.
* @param zero
* %curve giving zero u direction.
* @param u_range
* angle range for cross-sections.
* @param requested_fit
* required fit tolerance.
* @param actual_fit
* returns the actual fit tolerance used.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_pipe(
			double radius,					// signed radius
			curve const &spine,			// spine curve
			curve const &zero,			// curve giving zero u direction
			SPAinterval const &u_range,		// angle range for cross-sections
			double requested_fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// returns actual fit tolerance
		);


// Construct a pipe surface given the spine, radius and two boundary
// curves, which bracket the zero u-SPAparameter direction. Optionally
// constructs SPAparameter-space curves corresponding to the given
// boundary curves, as well.
/**
* Creates a pipe %surface given the spine, radius, and two boundary curves that bracket the zero <i>u</i>-parameter direction.
* <br><br>
* <b>Role:</b> Optionally, this routine creates parameter-space curves
* corresponding to the given boundary curves as well.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param radius
* signed radius.
* @param spine
* spine %curve.
* @param lowu
* boundary %curve on the low u side.
* @param highu
* boundary %curve on the high u side.
* @param requested_fit
* requested fit tolerance.
* @param actual_fit
* returns the actual fit tolerance used.
* @param lowp
* parameter space %curve for low u boundary.
* @param highp
* parameter space %curve for high u boundary.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_pipe_boundary(
			double radius,					// signed radius
			curve const &spine,			// spine curve
			curve const &lowu,			// boundary curve on the low u
									// side
			curve const &highu,			// boundary curve on the high u
									// side
			double requested_fit,					// requested fit tolerance
			double &actual_fit = *(double *)NULL_REF,
									// returns actual fit tolerance
			bs2_curve &lowp = *(bs2_curve *)NULL_REF,
									// returns SPAparameter-space curve
									// for low-u boundary
			bs2_curve &highp = *(bs2_curve *)NULL_REF
									// returns SPAparameter-space curve
									// for high-u boundary
		);


// Construct a surface which is a subset of a given one, being the
// overlap in SPAparameter space of the given surface and a given SPAbox.
// A surface periodic in one or both SPAparameter directions is rolled
// around if need be to cover the required range. It is very unlikely
// that the tolerance arguments will be needed, but they are included
// for completeness.
/**
* Creates a %surface identical to the given %surface.
* <br><br>
* <b>Role:</b> Creates a %surface identical to the given %surface (including
* parameterization) within the overlap between the given parameter range and the
* range of the %surface (taking into account periodicity), but not necessarily
* defined outside.
* <br><br>
* If the subset cannot be taken, or is deemed not to be worthwhile, the routine
* may return <tt>NULL</tt>. The routine may return a %surface that is only a fit to the true
* subset %surface, to the specified precision. Because most systems will represent
* such a %surface exactly, it is unlikely that this option will be exercised. In
* this case, the actual fit value will be returned as exact 0.
* <br><br>
* <b>Errors:</b> Returns <tt>NULL</tt> if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param old_bs
* given %surface.
* @param new_range
* required boundary range.
* @param position_fit
* required positional fit.
* @param actual_fit
* actual fit used.
**/
DECL_SPLINE bs3_surface
bs3_surface_subset(
			bs3_surface old_bs,			// given surface
			SPApar_box const &new_range,		// required bounds
			double position_fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// returns actual fit tolerance
		);


// **** Modification ***

// Reparametrise a B-spline surface by a linear transformation in the
// u direction, to achieve given start and end values.
/**
* Reparameterizes the %surface in <i>u</i>.
* <br><br>
* <b>Role:</b> Reparameterizes the given %surface in place using a linear
* transformation in the <i>u</i> direction so that its primary interval of definition in
* the <i>u</i> direction is from the start to the end parameters given (which must be in
* increasing order).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param start
* start u parameter desired.
* @param end
* end u parameter desired.
* @param bs
* given %surface.
**/
DECL_SPLINE void
bs3_surface_reparam_u(
			double start,				// start u SPAparameter desired
			double end,				// end u SPAparameter desired
			bs3_surface bs			// given surface
		);


// Reparametrise a B-spline surface by a linear transformation in the
// v direction, to achieve given start and end values.
/**
* Reparameterizes the %surface in <i>v</i>.
* <br><br>
* <b>Role:</b> Reparameterizes a B-spline %surface using linear transformation
* in the <i>v</i>-direction, to achieve given start and end values. The new parameter
* value is the appropriate linear function of the old.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param start
* start v parameter desired.
* @param end
* end v parameter desired.
* @param bs
* given %surface.
**/
DECL_SPLINE void
bs3_surface_reparam_v(
			double start,				// start v SPAparameter desired
			double end,				// end v SPAparameter desired
			bs3_surface bs			// given surface
		);


// Shift the u SPAparameter values of a B-spline curve by a given amount.
/**
* Reparameterizes the given %surface in <i>u</i>.
* <br><br>
* <b>Role:</b> Reparameterizes the given %surface in place by adding the shift
* value to its <i>u</i> parameter values.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param delta
* parameter shift desired.
* @param bs
* given %surface.
**/
DECL_SPLINE void
bs3_surface_shift_u(
			double delta,				// SPAparameter shift desired
			bs3_surface bs			// given surface
		);


// Shift the v SPAparameter values of a B-spline curve by a given amount.
/**
* Reparameterizes the given %surface in <i>v</i>.
* <br><br>
* <b>Role:</b> Reparameterizes the given %surface in place by adding the shift
* value to its <i>v</i> parameter values.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param delta
* parameter shift desired.
* @param bs
* given %surface.
**/
DECL_SPLINE void
bs3_surface_shift_v(
			double delta,				// SPAparameter shift desired
			bs3_surface bs			// given surface
		);


// Split a B-spline surface into two sections at a given SPAparameter
// value. The part with SPAparameter less than that given is returned
// as the function value, and the supplied surface is modified to
// represent the remainder. If the given value is at one end of the
// SPAparameter range, or outside the range for a non-periodic surface,
// the original surface is returned as the appropriate piece, and the
// other is set to NULL.
/**
* Splits a B-spline %surface into two sections at a given <i>u</i> parameter value.
* <br><br>
* <b>Role:</b> The part with <i>u</i> parameter less than that given is returned as
* the function value, and the supplied %surface is modified to represent the
* remainder.
* <br><br>
* If the given parameter is at the beginning of the range, or before the beginning
* for a non-periodic %surface, the function returns <tt>NULL</tt>, and leaves the original
* %surface unchanged (except to mark a periodic %surface as "closed" instead).
* <br><br>
* Similarly, if the parameter is at the end of the range (or beyond for a
* non-periodic %surface), the function returns the original %surface, again
* unchanged except for "closed" instead of "periodic".
* <br><br>
* For a periodic %surface, the parameter range is first adjusted by a whole number
* of periods to bracket the given parameter.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sur
* given %surface.
* @param uparam
* given u parameter value.
**/
DECL_SPLINE bs3_surface
bs3_surface_split_u(
			bs3_surface &sur,			// given surface
			double uparam					// given u SPAparameter value
		);
/**
* Splits a B-spline %surface into two sections at a given <i>v</i> parameter value.
* <br><br>
* <b>Role:</b> The part with <i>v</i> parameter less than that given is returned as
* the function value, and the supplied %surface is modified to represent the
* remainder.
* <br><br>
* If the given parameter is at the beginning of the range, or before the beginning
* for a non-periodic %surface, the function returns <tt>NULL</tt>, and leaves the original
* %surface unchanged (except to mark a periodic %surface as "closed" instead).
* <br><br>
* Similarly, if the parameter is at the end of the range (or beyond for a
* non-periodic %surface), the function returns the original %surface, again
* unchanged except for "closed" instead of "periodic".
* <br><br>
* For a periodic %surface, the parameter range is first adjusted by a whole number
* of periods to bracket the given parameter.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sur
* given %surface.
* @param vparam
* given v parameter value.
**/
DECL_SPLINE bs3_surface
bs3_surface_split_v(
			bs3_surface &sur,			// given surface
			double vparam				// given v SPAparameter value
		);


// Join two compatible 3D B-spline surfaces together (without
// checking for compatibility). The surfaces are joined along the
// high-SPAparameter edge of the first, and the low-SPAparameter edge of
// the second.
/**
* Joins two compatible three-dimensional B-spline %surfaces together.
* <br><br>
* <b>Role:</b> Joins two compatible three-dimensional B-spline %surfaces
* together (without checking for compatibility). The %surfaces are joined along the
* high <i>u</i>-parameter edge of the first, and the low <i>u</i>-parameter edge of the second.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param first_part
* left-hand %surface.
* @param last_part
* right-hand %surface.
**/
DECL_SPLINE bs3_surface
bs3_surface_join_u(
			bs3_surface first_part,			// "left-hand" surface
			bs3_surface last_part				// "right-hand" surface
		);

/**
* Joins two compatible three-dimensional B-spline %surfaces together.
* <br><br>
* <b>Role:</b> Joins two compatible three-dimensional B-spline surfaces
* together (without checking for compatibility). The surfaces are joined along the
* high <i>v</i>-parameter edge of the first, and the low <i>v</i>-parameter edge of the second.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param first_part
* left-hand %surface.
* @param last_part
* right-hand %surface.
**/
DECL_SPLINE bs3_surface
bs3_surface_join_v(
			bs3_surface first_part,			// "lower" surface
			bs3_surface last_part				// "upper" surface
		);


// Transform a 3D B-spline surface
/**
* Transforms a %surface.
* <br><br>
* <b>Role:</b> A transform consists of a 3 x 3 matrix with unit determinant,
* giving an affine transformation, an overall scaling factor, and a translation
* vector. There are three logical flags, relating to the matrix.
* <br><br>
* Rotate indicates whether the matrix is anything other than the identity.
* <br><br>
* Reflect indicates whether the determinant is -1.
* <br><br>
* Shear is set if the matrix isn't orthogonal.
* <br><br>
* The parameterization of a %surface must be independent of transformation.
* Therefore, the result of evaluating a transformed %surface will be the same as
* evaluating the untransformed %surface at the same parameter value, and
* transforming the result.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sur
* given %surface.
* @param t
* transform.
**/
DECL_SPLINE void
bs3_surface_trans(
			bs3_surface sur,			// given surface
			SPAtransf const &t
		);

// Warp a 3D B-spline surface
/**
* Warp a B-spline %surface.
* <br><br>
* Modify a B-spline %surface by applying the given law to the control points of the given B-spline %surface. If the given law
* is linear, the result will be the same as composing the law with the B-spline %surface.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param sur
* given %surface.
* @param the_law
* law from R3 to R3.
**/
DECL_SPLINE void
bs3_surface_warp(
			bs3_surface sur,			// given surface
			law const* the_law
		);

// STI mnl: moved prototype for ACIS2.0
// Remove knots of multiplicity higher than the degree.
/**
* Deletes knots where multiplicities are greater than the degree in both <i>u</i> and <i>v</i>.
* <br><br>
* <b>Role:</b> This function searches for knots multiplicities greater than the
* degree in both the <i>u</i> and <i>v</i> directions. The appropriate knots and control points
* are removed from the data structure.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs
* given %surface.
* @param knot_tol
* knot tolerance.
**/
DECL_SPLINE void
bs3_surface_remove_extra_knots(
    		bs3_surface bs,  			// given surface
    		double knot_tol      			// knot tolerance
		);


// **** Enquiry ****

// Return a logical indicating whether the surface is open in the
// u SPAparameter or not.


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs3_surface_open_u(
			bs3_surface			// given surface
		);


// Return a logical indicating whether the surface is open in the
// v SPAparameter or not.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs3_surface_open_v(
			bs3_surface			// given surface
		);


// Return a logical indicating whether the surface is closed in the
// u SPAparameter or not.
/**
* Determines whether a given %surface is closed in the <i>u</i>-parameter.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the parameter line on the %surface
* corresponding to minimum <i>u</i>-parameter is geometrically identical to that for
* maximum <i>u</i>-parameter; otherwise it returns <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_closed_u(
			bs3_surface bs			// given surface
		);


// Return a logical indicating whether the surface is closed in the
// v SPAparameter or not.
/**
* Determines whether the given %surface is closed in the <i>v</i>-parameter.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the parameter line on the %surface
* corresponding to minimum <i>v</i>-parameter is geometrically identical to that for
* maximum <i>v</i>-parameter; otherwise it returns <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_closed_v(
			bs3_surface bs			// given surface
		);


// Return a logical indicating whether the surface is periodic in the
// u SPAparameter or not.
/**
* Determines whether the %surface is periodic in the <i>u</i>-parameter.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the parameter line on the %surface
* corresponding to minimum <i>u</i>-parameter is geometrically identical to that for
* maximum <i>u</i>-parameter, the parameterizations are the same, and the normals are
* continuous across the boundary.
* <br><br>
* If this routine returns <tt>TRUE</tt> for a %surface, any routine that expects a <i>uv</i>
* parameter value must be prepared to accept any <i>u</i> value, and to map it into the
* principal range of the periodic %surface (by adding or subtracting a multiple of
* the period) before evaluating the %surface. Any routine that returns an actual <i>uv</i>
* value "near" to a given one must return the actual <i>u</i> value close to the given
* one, even if it is outside the standard %surface parameter range.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_periodic_u(
			bs3_surface bs			// given surface
		);


// Return a logical indicating whether the surface is periodic in the
// v SPAparameter or not.
/**
* Determines whether the %surface is periodic in the <i>v</i>-parameter.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the parameter line on the %surface
* corresponding to minimum <i>v</i>-parameter is geometrically identical to that for
* maximum <i>v</i>-parameter, the parameterizations are the same, and the normals are
* continuous across the boundary.
* <br><br>
* If this routine returns <tt>TRUE</tt> for a %surface, any routine that expects a <i>uv</i>
* parameter value must be prepared to accept any <i>v</i> value, and to map it into the
* principal range of the periodic %surface (by adding or subtracting a multiple of
* the period) before evaluating the %surface. In addition, any routine that returns
* an actual <i>uv</i> value "near" to a given one must return the actual <i>v</i> value close to
* the given one, even if it is outside the standard %surface parameter range.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_periodic_v(
			bs3_surface bs
		);


// Return the SPAparameter range of a 3D B-spline surface
/**
* Gets the range of both parameter values that defines the %surface as the bounds of a rectangular box in parameter space.
* <br><br>
* <b>Role:</b> If the %surface is periodic in one or both directions, it is
* defined for all parameter values in the periodic direction. This function
* returns a standard range over which the %surface is traversed exactly once.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE SPApar_box
bs3_surface_range(
			bs3_surface bs
		);


// Return the u SPAparameter range of a 3D B-spline surface
/**
* Gets the <i>u</i> parameter range a three-dimensional B-spline %surface.
* <br><br>
* <b>Role:</b> If the %surface is periodic in the <i>u</i> parameter, it is deemed to
* be defined for all <i>u</i>, but this routine returns a standard range over which the
* %surface is described exactly once. The length of this interval is the value
* returned by <tt>bs3_surface_period_u</tt> for such a %surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE SPAinterval
bs3_surface_range_u(
			bs3_surface bs			// given surface
		);


// Return the v SPAparameter range of a 3D B-spline %surface
/**
* Gets the <i>v</i> parameter range of a three-dimensional B-spline %surface.
* <br><br>
* <b>Role:</b> If the %surface is periodic in the <i>v</i> parameter, it is deemed to
* be defined for all <i>v</i>, but this routine returns a standard range over which the
* %surface is described exactly once. The length of this interval is the value
* returned by <tt>bs3_surface_period_v</tt> for such a %surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE SPAinterval
bs3_surface_range_v(
			bs3_surface bs
		);


// Return the u SPAparameter period of a 3D B-spline surface
/**
* Gets the <i>u</i>-parameter period of a three-dimensional B-spline %surface.
* <br><br>
* <b>Role:</b> If the given %surface is not periodic, the routine returns 0.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE double
bs3_surface_period_u(
			bs3_surface bs			// given surface
		);


// Return the v SPAparameter period of a 3D B-spline surface

/**
* Gets the <i>v</i>-parameter period of a three-dimensional B-spline %surface.
* <br><br>
* <b>Role:</b> If the given %surface is not periodic, the routine returns 0.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE double
bs3_surface_period_v(
			bs3_surface bs			// given surface
		);


// Report on whether the parametrisation is singular at a given
// constant SPAparameter value. We only support singularities along one
// or more edges of the SPAparameter region, and only the sort of
// singularity which means that every point with the given SPAparameter
// value maps into a single object-space point.
/**
* Determines if the mapping from parameter space to object-space is singular along the given constant <i>u</i>-parameter line.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the parameter space to object space
* mapping is singular, otherwise it returns <tt>FALSE</tt>.
* <br><br>
* Normally, the only form of singularity allowed is where the whole parameter line
* maps to a single object-space point, and it may only occur at one end of the
* parameter range.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param u
* u parameter of interest.
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_singular_u(
			double u,				// u SPAparameter at which to test
								// for a singularity
			bs3_surface bs
		);

/**
* Determines if the mapping from parameter space to object space is singular along the given constant <i>v</i>-parameter line.
* <br><br>
* <b>Role:</b> If the mapping from parameter space to object space is singular
* along the given constant <i>v</i>-parameter line, return <tt>TRUE</tt>; otherwise, <tt>FALSE</tt>.
* <br><br>
* Normally, the only form of singularity allowed is where the whole parameter line
* maps to a single object space point, and it may only occur at one end of the
* parameter range.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* v parameter of interest.
* @param bs
* given %surface.
**/
DECL_SPLINE logical
bs3_surface_singular_v(
			double v,				// v SPAparameter at which to test
								// for a singularity
			bs3_surface bs
		);


// On the assumption that a bs3_surface is made up of a rectangular
// array of simpler pieces (e.g. a B-spline may be considered to be
// an array of bipolynomial patches), return the number of such spans
// in the given parameter direction.
/**
* Gets number of simple patches in the <i>u</i> parameter direction forming a <tt>bs3_surface</tt>.
* <br><br>
* <b>Role:</b> If a B-spline %surface consists of a rectangular array of simple
* patches, each rectangular in parameter space, returns the number of such patches
* in the <i>u</i> parameter direction.
* <br><br>
* What is considered a "simple" patch is open to interpretation, but with a
* piecewise bipolynomial %surface (such as a B-spline), each bipolynomial piece
* would be a reasonable choice.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param sur
* given %surface.
**/
DECL_SPLINE int
bs3_surface_nspans_u(
			bs3_surface sur
		);
/**
* Gets number of simple patches in the <i>v</i> parameter direction forming a <tt>bs3_surface</tt>.
* <br><br>
* <b>Role:</b> If a B-spline %surface consists of a rectangular array of simple
* patches, each rectangular in parameter space, returns the number of such patches
* in the <i>v</i> parameter direction.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param sur
* given %surface.
**/
DECL_SPLINE int
bs3_surface_nspans_v(
			bs3_surface sur
		);


// Return the SPAparameter bounds of the (i,j)th simple patch of the
// surface, where the argument i runs from 0 for the first span to
// one fewer than the number returned by bs3_surface_nspans_u, and
// j is similar in the v direction.
/**
* Gets the parameter bounds of a simple %surface patch.
* <br><br>
* <b>Role:</b> Returns the parameter bounds of the <i>(i,j)</i>th simple patch of the
* %surface, where the argument <i>i</i> runs from 0 for the first span to one fewer than
* the number returned by <tt>bs3_surface_nspans_u</tt>, and <i>j</i> is similar in the <i>v</i>-direction.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param i
* span number in u.
* @param j
* span number in v.
* @param sur
* given %surface.
**/
DECL_SPLINE SPApar_box
bs3_surface_bispan_range(
			int i,				// span number i in u direction
			int j,				// span number j in v direction
			bs3_surface sur
		);
/**
* Gets the <i>u</i> parameter bounds of a %surface's simple patches.
* <br><br>
* <b>Role:</b> If the %surface is an array of simple patches, numbered
* sequentially in the increasing <i>u</i> and <i>v</i> directions, starting at 0.0, return the <i>u</i>
* parameter bounds of the patches indexed in the <i>u</i> direction by the given integer.
* <br><br>
* This routine returns an empty interval if the index is out of the range implied
* by the value returned by <tt>bs3_surface_nspans_u</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param i
* span number n in u direction.
* @param sur
* given %surface.
**/
DECL_SPLINE SPAinterval
bs3_surface_span_range_u(
			int i,				// span number i in u direction
			bs3_surface sur
		);
/**
* Gets the <i>v</i> parameter bounds of a %surface's simple patches.
* <br><br>
* <b>Role:</b> If the %surface is an array of simple patches, numbered
* sequentially in the increasing <i>u</i> and <i>v</i> directions, starting at zero, this
* function returns the <i>v</i> parameter bounds of the patches indexed in the <i>v</i>
* direction by the given integer.
* <br><br>
* This routine returns an empty interval if the index is out of the range implied
* by the value returned by <tt>bs3_surface_nspans_v</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param j
* span number n in v direction.
* @param sur
* given %surface.
**/
DECL_SPLINE SPAinterval
bs3_surface_span_range_v(
			int j,				// span number j in v direction
			bs3_surface sur
		);


// Construct a new surface which consists only of the (i,j)th simple
// patch of the given surface.
/**
* Creates a %surface from a simple patch of a %surface.
* <br><br>
* <b>Role:</b> Creates a new %surface that consists only of the <i>(i,j)</i>th simple
* patch of the given %surface. The knot vectors of the new %surface will have start
* and end multiplicities equal to the degree; therefore, the new %surface will
* represent a single Bezier patch. In the case of a rational %surface, the weights
* associated with the boundary control points have not been normalized.
* <br><br>
* <b>Errors:</b> If an error occurs, this function returns a <tt>NULL</tt> %surface.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param i
* span number in u direction.
* @param j
* span number in v direction.
* @param sur
* given %surface.
**/
DECL_SPLINE bs3_surface
bs3_surface_bispan(
			int i,				// span number i in u direction
			int j,				// span number j in v direction
			bs3_surface sur
		);


// On the assumption that a bs3_surface is a piecewise rational
// bipolynomial SPAvector function of its parameters, convert the (i,j)th
// span into a rational bipolynomial SPAvector, with a normalised [ 0, 1 ]
// parametrisation in each direction.
/**
* Converts a span into a rational bipolynomial vector with normalized parameterization in each direction.
* <br><br>
* <b>Role:</b> Converts the <i>(i,j)</i>th span into a rational bipolynomial vector,
* with a normalized [0, 1] parameterization in each direction. Assumes that a
* <tt>bs3_surface</tt> is a piecewise rational bipolynomial vector function of its
* parameters. The return type is <tt>rat_bipoly_vec</tt>.
* <br><br>
* <b>Errors:</b> If an error occurs, this routine returns the zero vector.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param nuspan
* ith span in u direction.
* @param nvspan
* ith span in v direction.
* @param sur
* given %surface.
**/
DECL_SPLINE rat_bipoly_vec
bs3_surface_bispan_poly(
			int nuspan,
			int nvspan,
			bs3_surface sur
		);


// **** Evaluation ****

// Return the number of derivatives which bs3_surface_evaluate will
// evaluate "accurately".
/**
* Gets the number of derivatives that <tt>bs3_surface_evaluate</tt> evaluates accurately.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3_surface1
* given %surface.
**/
DECL_SPLINE int
bs3_surface_accurate_derivs(
			 bs3_surface bs3_surface1
        );

// Return the parametric criterion used to decide whether a given
// SPAparameter is a knot (for the purposes of choosing between
// discontinuous "sided" derivatives).
/**
* Gets the parametric criterion used to decide whether a given parameter is a knot.
* <br><br>
* <b>Role:</b> This routine is for the purpose of choosing between
* discontinuous "sided" derivatives.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_SPLINE double
bs3_surface_knottol();

// Evaluate the SPAposition and an arbitrary number of derivatives of the
// surface, with control over the handedness of the evaluation if the
// derivatives are discontinuous. There is a limit on the number of
// derivatives which will be evaluated, as returned by
// bs3_surface_accurate_derivs; any further derivatives requested are
// set to zero.
/**
* Evaluates the position and an arbitrary number of derivatives of the %surface.
* <br><br>
* <b>Role:</b> Evaluates the position and an arbitrary number of derivatives of
* the %surface, with control over the handedness of the evaluation if the
* derivatives are discontinuous.
* <br><br>
* <tt>deriv</tt> specifies an array of pointers to arrays, containing at least <tt>nderiv</tt>
* values, though any or all may be <tt>NULL</tt>. If not <tt>NULL</tt>, entry <tt>n</tt> (representing the
* <tt>(n+1)</tt>th derivative) must point to an array of at least <tt>n+1</tt> vectors.
* <br><br>
* <tt>uindex</tt> specifies the sense of evaluation at a <i>u</i> discontinuity: negative means
* evaluate to the left, positive means evaluate to the right, and 0 means "don't
* care."
* <br><br>
* <tt>vindex</tt> specifies the sense of evaluation at a <i>v</i> discontinuity: negative means
* evaluate below, positive means evaluate above, and 0 means "don't care."
* <br><br>
* <b>Errors:</b> Returns -1 if input %surface is null, or <tt>nderiv</tt> &lt; 0.
* <br><br>
* <b>Limitations:</b> There is a limit on the number of evaluated derivatives, as
* returned by <tt>bs3_surface_accurate_derivs</tt>; any further derivatives requested are
* set to 0.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter position.
* @param sur
* given %surface.
* @param pos
* returned position.
* @param deriv
* returned derivatives array of pointers to arrays of vectors,each such array containing one more vector than the order of the derivative.
* @param nderiv
* returned number of derivatives to be evaluated, equal to the length of the deriv array.
* @param uindex
* returned senses at u discontinuities.
* @param vindex
* returned senses at v discontinuities.
**/
DECL_SPLINE int
bs3_surface_evaluate(
			SPApar_pos const &uv ,
			bs3_surface sur,
			SPAposition &pos,
			SPAvector **deriv = NULL,		// Array of pointers to arrays,
									// containing at least nd values,
									// though any or all may be NULL.
									// If not NULL, entry n (representing
									// the (n+1)th derivative) must point
									// to an array of at least n+1 vectors.
			int nderiv = 0,				// number of derivatives (nd) to
									// be evaluated.
			int uindex = 0,				// sense of evaluation at a u
									// discontinuity: -ve means evaluate
									// to the left (u < param), +ve means
									// evaluate to the right (u > param),
									// and 0 means "don't care"
			int vindex = 0					// sense of evaluation at a v
									// discontinuity: -ve means evaluate
									// below (v < param), +ve means
									// evaluate above (v > param),
									// and 0 means "don't care"
		);


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE int
bs3_surface_evaluate(
			SPApar_pos const &uv,
			bs3_surface sur,
			SPAposition &pos,			// point returned
			SPAvector **deriv,			// array of pointers to arrays
									// of vectors, each such array
									// containing one more SPAvector than
									// the order of the derivative.
			int nuderiv,				// number of derivatives to be
									// evaluated, and hence the length
									// of the deriv array
			int nvderiv,				// number of derivatives to be
									// evaluated, and hence the length
									// of the deriv array
			int uindex,				// -ve to select the low-u version
									// of derivatives, +ve for the high-u
									// version, and 0 for "don't care"
			int vindex				// same for v direction
		);

// Evaluate the SPAposition, first derivative with respect to the two
// parameters u and v into d1[ 0 ] and d1[ 1 ] respectively, and the
// second derivatives into d2, in the order duu, duv, dvv.
/**
* Evaluates a <tt>bs3_surface</tt> for position, first, and second derivatives at the given parameter value.
* <br><br>
* <b>Role:</b> If <tt>pos</tt> is a non-null reference, it is set to the evaluated
* position of the %surface at the given parameter values. If <tt>d1uv</tt> is not <tt>NULL</tt>, it
* must point to an array of vectors of length 2, and these are set to the %surface
* derivatives with respect to the parameters <i>u</i> and <i>v</i> respectively. If <tt>d2uv</tt> is not
* <tt>NULL</tt>, it must point to an array of vectors of length 3, and these are set to the
* second derivatives of the %surface, with respect to <i>u</i> twice, <i>u</i> and <i>v</i>, and <i>v</i> twice.
* (For all ordinary surfaces we may assume that the derivative with respect to v
* and <i>u</i> will be the same as that with respect to <i>u</i> and <i>v</i>.)
* <br><br>
* <b>Limitations:</b> There is no provision to handle discontinuities of second
* derivative, so it is assumed that the second derivatives are continuous across
* the portion of the %surface that is of interest. Also, it is assumed that the
* first derivatives are continuous everywhere. The direction of the %surface normal
* is always required to be continuous throughout the interior of the portion of
* interest.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter point uv.
* @param bs
* given %surface.
* @param pos
* position returned.
* @param d1
* du and dv returned in an array of 2 vectors.
* @param d2
* duu, duv and dvv returned in array of 3 vectors.
**/
DECL_SPLINE void
bs3_surface_eval(
			SPApar_pos const &uv,		// given SPAparameter point (u,v)
			bs3_surface bs,			// given surface
			SPAposition &pos,				// SPAposition returned
			SPAvector *d1 = NULL,		// du and dv returned in SPAvector
									// array (d1) of length 2
			SPAvector *d2 = NULL			// duu, duv and dvv returned in
									// SPAvector array (d2) of length 3
		);


// Evaluate a SPAposition on a 3D B-spline surface at a given (u,v).
/**
* Evaluates and returns a position on a three-dimensional B-spline %surface at a given <i>uv</i>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter point.
* @param bs
* given %surface.
**/
DECL_SPLINE SPAposition
bs3_surface_position(
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			bs3_surface bs			// given surface
		);


// Evaluate the tangent to a 3D B-spline curve at a given (u,v).
/**
* Evaluates the normal to a three-dimensional B-spline %surface at a given (u,v).
* <br><br>
* <b>Role:</b> This routine is often implemented as a call to <tt>bs3_surface_eval</tt>
* to obtain the first derivatives, returning the cross product, suitably
* normalized, but special action may be required at parameter singularities. At a
* %surface singularity, like the apex of a cone, the routine returns a unit vector
* with all its components zero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter point.
* @param bs
* given %surface.
**/
DECL_SPLINE SPAunit_vector
bs3_surface_normal(
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			bs3_surface bs			// given surface
		);


// Evaluate the principal axes of curvature and corresponding
// curvatures of a 3D B-spline surface at a given (u,v).
/**
* Evaluates the principal axes of curvature and corresponding curvatures of a three-dimensional B-spline %surface at a given <i>uv</i>.
* <br><br>
* <b>Role:</b> The sign and order of the principal axes are not significant.
* The sign of the curvature value indicates whether the %surface is convex or
* concave with respect to the normal direction, which is considered to point outward
* from the region bounded by the %surface. A convex %surface (one that curves back
* from the outward normal direction) has positive curvature, a concave one has
* negative curvature.
* <br><br>
* <b>Errors:</b> Returns without setting anything if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter point.
* @param bs
* given %surface.
* @param u1
* returns first principal axis.
* @param c1
* returns first curvature.
* @param u2
* returns second principal axis.
* @param c2
* returns second curvature.
**/
DECL_SPLINE void
bs3_surface_prin_curv(
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			bs3_surface bs,		// given surface
			SPAunit_vector &u1,		// first principal axis
			double &c1,			// first curvature
			SPAunit_vector &u2,		// second principal axis
			double &c2			// second curvature
		);


// Evaluate the cross curvature of a 3D B-spline surface at a
// given (u,v).

/**
* Evaluates the cross curvature of a three-dimensional B-spline %surface at a given <i>uv</i>.
* <br><br>
* <b>Role:</b> This is equivalent to constructing a plane through the given point and
* perpendicular to the given direction, which must be tangent to the %surface at
* the given point, and returning the curvature of the intersection %curve between
* the plane and the %surface at that point.
* <br><br>
* If the intersection %curve is convex when viewed from the outside of the %surface
* (the side that the normal points toward), the sign of the result is negative. If
* the %curve is is concave, the sign is positive. If there is a discontinuity in
* curvature at the given point, the value returned is for the left-hand side of
* the intersection %curve as viewed in the given tangent direction with the %surface
* normal upwards.
* <br><br>
* <b>Errors:</b> Returns -1 if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param uv
* given parameter point.
* @param dir
* object space tangent direction.
* @param bs
* given %surface.
**/
DECL_SPLINE double
bs3_surface_cross(
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			SPAunit_vector const &dir,
			bs3_surface bs			// given surface
		);


// **** Geometric Properties ****

// Find the SPAparameter of a point on a 3D B-spline surface
/**
* Gets the parameter of a point on a 3D B-spline %surface.
* <br><br>
* <b>Role:</b> If initial parameter values are given, they may be assumed to be
* close to the desired point compared with any other point of (local) minimum
* distance, so there is no requirement to check that the value obtained is indeed
* the nearest point.
* <br><br>
* <b>Errors:</b> Returns an empty <tt>SPApar_pos</tt> if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pos
* given point.
* @param bs
* given %surface.
* @param uv
* uv guess.
**/
DECL_SPLINE SPApar_pos
bs3_surface_invert(
			SPAposition const &pos,		// given point
			bs3_surface bs,			// given surface
			SPApar_pos const &uv = *(SPApar_pos *)NULL_REF
		);


// Find the direction in SPAparameter space SPAparameter of a direction
// on a 3D B-spline surface at a given SPAposition.
/**
* Gets the direction in the parameter space of a %surface at a given position that corresponds to a given object-space tangent direction.
* <br><br>
* <b>Role:</b> Usually, this routine will normalize the result of calling
* <tt>bs3_surface_unitvec</tt>.
* <br><br>
* <b>Errors:</b> Returns an empty <tt>SPApar_dir</tt> if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Limitations:</b> Results are not defined for points that do not lie on the
* %surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param dir
* given direction.
* @param uv
* given parameter point.
* @param bs
* given %surface.
**/
DECL_SPLINE SPApar_dir
bs3_surface_invdir(
			SPAunit_vector const &dir,	// given direction
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			bs3_surface bs			// given surface
		);


// Find the offset in SPAparameter space corresponding to a unit move in
// a direction on a 3D B-spline surface at a given SPAposition.
/**
* Determines the offset in parameter space corresponding to a unit move in a direction on a 3D B-spline %surface at a given position.
* <br><br>
* <b>Errors:</b> Returns an empty <tt>SPApar_vec</tt> if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param dir
* given direction.
* @param uv
* given parameter point.
* @param bs
* given %surface.
**/
DECL_SPLINE SPApar_vec
bs3_surface_unitvec(
			SPAunit_vector const &dir,	// given direction
			SPApar_pos const &uv,	// given SPAparameter point (u,v)
			bs3_surface bs			// given surface
		);


// Test whether a point lies within tolerance of a 3D B-spline surface
/**
* Determines whether an object-space point lies on a given %surface within the given positional precision.
* <br><br>
* <b>Role:</b> This function takes advantage of an estimate of the %surface
* parameter values to speed up processing, but works correctly without them.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pos
* given point.
* @param tol
* permitted tolerance.
* @param bs
* given %surface.
* @param uv_guess
* approximation to the parameter value of the foot of the perpendicular from the point to the %surface.
* @param uv_actual
* returned actual parameter value used.
**/
DECL_SPLINE logical
bs3_surface_testpt(
			SPAposition const &pos,		// given point
			double tol,					// permitted tolerance
			bs3_surface bs,			// given surface
			SPApar_pos const &uv_guess = *(SPApar_pos *)NULL_REF,
									// (u, v) guess
			SPApar_pos &uv_actual = *(SPApar_pos *)NULL_REF
									// (u, v) actual
		);


// Find the foot of a perpendicular from a point to a 3D B-spline
// surface.  We assume that the point is close, and that we are given
// a reasonable approximation to the correct SPAparameter values, or that
// if not the surface is not too badly-behaved.

/**
* Gets the intersection with the %surface and the normal to the %surface of a perpendicular dropped from a point to the %surface.
* <br><br>
* <b>Role:</b> If an initial guess of the parameter values for the foot of the
* perpendicular is given, it is assumed to be close to the desired position,
* allowing faster processing.
* <br><br>
* For an open %surface, there may be no perpendicular at all if the given point is
* beyond the boundary. In this case, a perpendicular will be dropped to a boundary
* edge, even though this is not perpendicular to the %surface itself. If there is
* no perpendicular to the edges, then one of the corners will be returned as the
* <tt>foot</tt>. The normal direction will always be the normal to the %surface at the foot.
* <br><br>
* <b>Errors:</b> Returns without setting any arguments if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param point
* given point.
* @param bs
* given %surface.
* @param foot
* returned foot of perpendicular.
* @param norm
* returned normal.
* @param uv_guess
* returned guess uv if known.
* @param uv_actual
* returned actual uv used.
* @param var
* var.
**/
DECL_SPLINE void
bs3_surface_perp(
			SPAposition const &point,		// given point
			bs3_surface bs,			// given surface
			SPAposition &foot,				// foot of perp. returned
			SPAunit_vector &norm,			// normal returned
			SPApar_pos const &uv_guess = *(SPApar_pos *)NULL_REF,	// (u,v) guess if known
			SPApar_pos &uv_actual = *(SPApar_pos *)NULL_REF,	// (u,v) returned if wanted
			SPApar_box const &var = *(SPApar_box *)NULL_REF
		);


// Estimate the SPAparameter values of the foot of a perpendicular from
// a given point to the surface. It is expected that this function
// followed by a call to bs3_surface_perp using the estimated SPAparameter
// value will be substantially faster than a call to bs3_surface_perp
// with no estimated SPAparameter value, but that the result may not
// give the nearest perpendicular, even if the given point is very near
// to the surface. It may only be used by algorithms which are
// resistant to such unexpected results.
/**
* Estimates the parameter values of the foot of a perpendicular from a given point to the %surface.
* <br><br>
* <b>Role:</b> It is expected that this function, followed by a call to
* <tt>bs3_surface_perp</tt> using the estimated parameter value, will be substantially
* faster than a call to <tt>bs3_surface_perp</tt> with no estimated parameter value;
* however, the result may not give the nearest perpendicular, even if the given
* point is very near to the %surface. Use <tt>bs3_surface_estimate_param</tt> only with
* algorithms that are resistant to such unexpected results.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pos
* given point.
* @param sur
* given %surface.
* @param recurse
* use recursion.
**/
DECL_SPLINE SPApar_pos
bs3_surface_estimate_param(
			SPAposition const &pos,
			bs3_surface sur,
			logical recurse = FALSE 	// anti-recursion flag, set TRUE to
				                        // avoid recursive calls
		);


// Return a SPAbox enclosing a portion of a 3D B-spline surface
/**
* Gets a box that encloses a portion of a three-dimensional B-spline %surface.
* <br><br>
* <b>Role:</b> The box will not be the smallest possible, but will be a
* compromise between a tight fit and fast evaluation.
* <br><br>
* If the parameter box is <tt>NULL</tt>, the box will contain the whole %surface. ACIS
* ensures that any parameter box given is entirely within the parameter range for
* the %surface; however, if the %surface is periodic, it may be partially or wholly
* outside the basic range of the periodic parameter.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
* @param test
* parameter range of interest.
**/
DECL_SPLINE SPAbox
bs3_surface_box(
			bs3_surface bs,		// given surface
			SPApar_box const &test = *(SPApar_box *)NULL_REF
								// SPAparameter range of interest (full
								// surface if not given).
		);
// STI dgh }	// Temporary end of "extern C"

// Return a cone bounding the normal direction of a surface. The
// cone is deemed to have its apex at the origin, and has a given
// axis direction and (positive) half-angle. The function also
// evaluates a hint to indicate which SPAparameter direction shows the
// greater average variation in normal direction. If the logical
// argument is TRUE, then a quick approximation is found. The
// resulting cone is guaranteed to be wholly within the one found
// with a FALSE argument, but is not guaranteed to be a true bound -
// the surface normal could lie outside it at some parts of the
// surface.


// For internal use only
/**
 * @nodoc
 */

enum bs3_surface_vardir {
	bs3_surface_var_unknown,
	bs3_surface_var_ugreater,
	bs3_surface_var_vgreater
};


// For internal use only
/**
 * @nodoc
 */
class DECL_SPLINE bs3_surface_normcone : public ACIS_OBJECT {
public:
	SPAunit_vector axis;
	double angle;
	bs3_surface_vardir vardir;

	bs3_surface_normcone();
	bs3_surface_normcone(
				SPAunit_vector const &ax,
				double ang,
				bs3_surface_vardir v = bs3_surface_var_unknown
			);
};

// STI dgh {


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE bs3_surface_normcone
bs3_surface_normal_cone(
			bs3_surface,
			logical = FALSE,	// default to give proper bound
			double angle_tolerance = SPAresnor
				 // the minimum angle difference before a judgement will be
				 // made as to whether the surface turns more in the u- or
				 // v- direction
		);


// Construct a bounding "hull" around a surface. This is a set
// of planes which (in some sense) closely bound the entire surface.
// These could in principle just be the standard SPAbox planes, but it
// is expected that this function should generate a significantly
// tighter bound, preferably transformation-independent, at the
// expense of moderate extra effort. By choosing two of the planes to
// be roughly parallel to the surface, the distance between them will
// fall as the square of the linear dimension of a smooth surface, as
// that dimension gets small, so the volume of the bound can be
// made to fall more rapidly than the cube of this length. This
// function is only called after a SPAbox test using the standard boxes.
// The function receives arrays with enough space for six planes, represented as
// points and normals. It returns the number of planes constructed, which must
// not exceed six.
/**
* Creates a bounding hull around a %surface.
* <br><br>
* <b>Role:</b> The hull is a set of six planes that (in some sense) closely
* bounds the entire %surface. In principle, these could be the standard box planes,
* but this function will probably generate a significantly tighter bound,
* preferably transformation-independent, at the expense of moderate extra effort.
* By choosing two planes to be roughly parallel to the %surface, the distance
* between them will fall as the square of the linear dimension of a smooth %surface.
* As that dimension gets small, the volume of the bound can be made to fall more
* rapidly than the cube of the length.
* <br><br>
* <b>Limitations:</b> Call this function only after a box test using the standard
* boxes. It returns the number of planes constructed and allocates an array of
* planes in free space, which it is the caller's responsibility to delete.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surface
* given %surface.
* @param points
* six points on planes.
* @param normals
* six plane normals.
**/
DECL_SPLINE int
bs3_surface_hull_planes(
			bs3_surface surface,
			SPAposition* points, 		// space for six points (positions on planes)
			SPAunit_vector* normals	// space for six plane normals
		);


// Test whether two B-spline surfaces are (apparently) the same. This is
// not a comprehensive test - surfaces which are coincident but have
// differing extents, knot vectors or other internal details will not
// be spotted. The routine should be used only as a (fairly) cheap
// filter.
/**
* Determines whether two B-spline surfaces are the same.
* <br><br>
* <b>Role:</b> Determines whether two B-spline surfaces are (apparently) the same.
* This is not a comprehensive test - surfaces that are coincident but have
* differing extents, knot vectors or other internal details will not be spotted.
* Use this routine as a filter.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs1
* first %surface.
* @param bs2
* second %surface.
* @param tol
* tolerance of control point positions.
**/
DECL_SPLINE logical
bs3_surface_same(
			bs3_surface bs1,		// first surface for comparison
			bs3_surface bs2,		// second surface
			double tol = 0.0		// tolerance on control point positions
		);


// Construct the silhouette lines of a surface viewed in a given
// way (point or parallel projection.

/**
* Creates the silhouettes of the %surface.
* <br><br>
* <b>Role:</b> Creates the silhouettes of the %surface when viewed from the
* point or direction specified by the <tt>view_spec</tt> (defined in <tt>makesil/makesil.hxx</tt>).
* <br><br>
* The <tt>view_spec</tt> class contains the following information:
* <br><br>
* <tt>to_point</tt> is a position in object space, normally in the general neighborhood of
* the %surface.
* <br><br>
* <tt>dir</tt> is the direction in object space from <tt>to_point</tt> to the view position, i.e.,
* opposite to the view direction.
* <br><br>
* <tt>inv_dist</tt> is the reciprocal of the distance from <tt>to_point</tt> to the view position,
* along <tt>dir</tt>. This is zero for a parallel projection.
* <br><br>
* The results of this function are returned using the <tt>surf_surf_int</tt> class, but
* only some of the members are significant:
* <br><br>
* <tt>cur</tt> is a pointer to an ACIS curve that contains the object-space description of
* the intersection %curve. In most cases this routine will construct an intcurve,
* but in special cases of parametric surfaces it may return straight lines or
* ellipses.
* <br><br>
* <tt>pcur1</tt> is a pointer to parameter-space curves, specifying the %curve in the
* parameter space of the %surface.
* <br><br>
* <tt>start_term</tt>, <tt>start_param</tt>; If the %curve starts at a branch-point (i.e., several
* silhouettes join there), <tt>start_term</tt> points to a <tt>surf_surf_term</tt> representing this
* phenomenon, and <tt>start_param</tt> gives its parameter value on this intersection %curve.
* If not, <tt>start_term</tt> is <tt>NULL</tt>, and <tt>start_param</tt> undefined. Note that for a given
* point, all %curve segments starting or ending there must point to the same
* <tt>surf_surf_term</tt> object, not to separate objects with the same position.
* <br><br>
* <tt>end_term</tt>, <tt>end_param</tt>; as for <tt>start_term</tt>, etc. but for the end of this %curve.
* <br><br>
* <tt>nsplit</tt>, <tt>split_param</tt>; If this %curve has more than one disjoint portion lying
* within the region of interest, split points must be provided which lie outside
* the region of interest and divide the %curve into portions, each of which has at
* most one segment within the region of interest. These points are defined by an
* ordered sequence of parameter values.
* <br><br>
* <tt>left_surf_rel[0]</tt>, <tt>right_surf_rel[0]</tt>; The visibility of the %surface on either
* side of the %curve. If the %surface normal on the appropriate side of the %curve
* points away from the view point (i.e., has a positive dot product with the view
* direction), the relationship is outside, otherwise inside. In this context, left
* and right are as viewed in the direction of the %curve, with the %surface normal
* upwards.
* <br><br>
* <tt>pcur2</tt>, <tt>left_surf_rel[1]</tt>, <tt>right_surf_rel[1]</tt>, <tt>int_type</tt>, <tt>aux_surf</tt>, and
* <tt>aux_left_rel[ ]</tt> are not used.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %surface.
* @param logical_surf
* TRUE if %surface is negated.
* @param view_specs
* view specification.
* @param reg_int
* region of interest.
**/

DECL_SPLINE surf_surf_int *
bs3_surface_sil(
			bs3_surface bs,			// given surface
			logical logical_surf,				// True if surface negated
			view_spec const &view_specs,		// view specification
			SPAbox const &reg_int				// region of interest
		);


// **** Utility ****

// Duplicate a 3D B-spline surface (in free store)
/**
* Creates an exact copy of the %surface in free store.
* <br><br>
* <b>Role:</b> ACIS calls this routine only when a change is to be made to one
* copy of the %surface, so there is no advantage to be gained by deferring the
* duplication further. Ordinary duplication of ACIS B-spline %surfaces merely creates
* a new reference to the same underlying <tt>bs3_surface</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %surface.
**/
DECL_SPLINE bs3_surface
bs3_surface_copy(
			bs3_surface bs			// given surface
		);


// Remove a 3D B-spline surface from free store.
/**
* Deletes storage occupied by the given %surface.
* <br><br>
* <b>Role:</b> No assumptions are made by ACIS about how the underlying %surface
* package manages its storage space, provided that it does not prevent the
* standard C memory allocation mechanism from working.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs
* given %surface.
**/

DECL_SPLINE void
bs3_surface_delete(
			bs3_surface &bs			// given surface
		);


// Save a 3D B-spline surface to disc in recoverable format.
/**
* Saves a %surface.
* <br><br>
* <b>Role:</b> Writes a representation of the parametric %surface to some
* external medium, using routines <tt>write_int</tt>, <tt>write_long</tt>, <tt>write_real</tt>, and
* <tt>write_string</tt>, defined in ACIS file <tt>kernutil/fileio/fileio.hxx</tt>.
* <br><br>
* For the previous four routines, no single external format will be suitable for
* all possible parametric %surface representations. But, where appropriate, use the
* definition for rational and nonrational B-splines for compatibility.
* <br><br>
* The overloaded &lt;&lt; operator behaves like <tt>bs3_surface_save</tt>, except that it writes
* to a C++ style stream using stream operators. <tt>bs3_surface_save</tt> does not require
* the output format to be the same, but it is strongly recommended that it is. For
* example: <tt>bs3_surface surf</tt>.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param sur
* given %surface.
**/

DECL_SPLINE void
bs3_surface_save(
			bs3_surface sur			// given surface
		);


// Restore a 3D B-spline surface from disc.
/**
* Restores a saved %surface.
* <br><br>
* <b>Role:</b> Reads back a representation of a parametric %surface as written
* by <tt>bs3_surface_save</tt>, and creates a duplicate of the original %surface.
* <br><br>
* The overloaded &gt;&gt; operator behaves like <tt>bs3_surface_restore</tt>, except that it
* reads from a C++ style stream using stream operators, and sets the result into
* the second argument.
* <br><br>
* For example: <tt>bs3_surface surf</tt>;
* <br><br>
* Reading uses routines <tt>read_int</tt>, <tt>read_long</tt>, <tt>read_real</tt>, and <tt>read_string</tt> that are
* defined in <tt>kernutil/fileio/fileio.hxx</tt>.
* <pre>
* if (restore_version_number &lt; SPLINE_VERSION)
* if (read_int() == -1)
*  // First check that there is a %surface to read.
*  read_int         stype
*  read_int         save_dim
*  read_int         u degree
*  read_int         v degree
*  read_int         save nu span
*  read_int         save nv span
*  read_int         rat u
*  read_int         rat v
*  read_int         form u
*  read_int         form v
*  read_int         pole u
*  read_int         pole v
* else
* // New style header. There are keywords instead of numbers
* // where appropriate, and redundant values are missing.
*  read_id          id string
* if (strcmp( id_string, type_nullbs ) == 0)
*       // return NULL;
* else if (strcmp( id_string, type_nubs ) == 0 )
*       // rational = FALSE;
* else if (strcmp( id_string, type_nurbs ) == 0 )
*       // rational = TRUE;
* else
*       // sys_error( UNKNOWN_BS_SURFACE );
* read_int          u degree
* read_int          v degree
* if (rational)
*  read_id          id string for rational_u or rational_v
* if (restore_version_number &lt; CONSISTENT_VERSION)
*  read_id          id string for formu
*  read_id          id string for formv
*  read_id          id string for poleu
*  read_id          id string for polev
* else
*  read_enum        Read enumeration bs3_surf_form for form_map for form u
*  read_enum        Read enumeration bs3_surf_form for form_map for form v
*  read_enum        Read enumeration sing_map for pole u
*  read_enum        Read enumeration sing_map for pole v
* // Read the knots and multiplicities, allocating space for
* // the knot values as we go, and accumulating the total of
* // knots and multiplicities.
* read_int          Number of knots in u
* if (restore_version_number &gt;= SPLINE_VERSION)
*  read_int          Number of knots in v
* for (int i = 0; i &lt; n_uknots; i++)
*  read_real         u knot
*  read_int          u multiplicity
* if (restore_version_number &lt; SPLINE_VERSION)
*  read_int         Number of knots in v
* for (i = 0; i &lt; n_vknots; i++)
*  read_real        v knot
*  read_int         v multiplicity
* // Finally read the control point values.
* for (row_start = bs-&gt;node0; row_start != NULL; row_start = row_start-&gt;vnext)
*  for (ag_snode *this_node = row_start; this_node != NULL; this_node = this_node-&gt;unext)
*   for (i = 0; i &lt; dimh; i++)
*    read_real      node Pw weight</pre>
* <b>Effect:</b> System routine
**/

DECL_SPLINE bs3_surface
bs3_surface_restore();


// Produce a simple-minded printout of a 3D B-spline surface
/**
* Gets a readable representation of the %curve and writes it to a file.
* <br><br>
* <b>Role:</b> Produces a readable representation of the %surface on the given
* open file, in any convenient format.
* <br><br>
* If this extends to more than one text line (as is almost certain), start all
* lines but the first with the leader string. Do not terminate the last line by a
* newline character.
* <br><br>
* If the intersection %curve is convex when viewed from the outside of the %surface
* (the side that the normal points toward), the sign of the result is negative. If
* the %curve is is concave, the sign is positive. If there is a discontinuity in
* curvature at the given point, the value returned is for the left-hand side of
* the intersection %curve as viewed in the given tangent direction with the %surface
* normal upwards.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param sur
* given %surface.
* @param leader
* string to precede second and subsequent lines.
* @param fp
* output file.
**/

DECL_SPLINE void
bs3_surface_debug(
			bs3_surface sur,			// given surface
			char const *leader,
			FILE *fp = debug_file_ptr
		);


// STI dgh }		// End 

// STI mnl begin: Compute an approximation to the maximum  side length
// of a bs3_surface by computing the length of the control polygon.
/**
* Determines the fit tolerance of a %surface.
* <br><br>
* <b>Role:</b> The computed fit tolerance is not less than <tt>10*SPAresabs</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param sur
* given %surface.
**/

DECL_SPLINE double
bs3_surface_fitol(
			bs3_surface sur    // given bs3_surface
		);
// STI mnl end


// Needed for deformable surface library
/**
* Sets the position of one control point.
* <br><br>
* <b>Role:</b> Checks that surf has an (<i>i</i>,<i>j</i>) control point. If it does it
* copies the <i>xyz</i> values of <tt>pos</tt> into control point's data structure. When <tt>surf</tt> is
* rational it also copies the weight value into the control point's data structure.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* target bs3_surface to modify.
* @param uindex
* u index of target control point.
* @param vindex
* v index of target control point.
* @param pos
* xyz location copied into control point,size:[3].
* @param weight
* weight to which control point is assigned only used if surf is rational.
**/

DECL_SPLINE void
bs3_surface_set_ctrlpt(  		// eff: set the SPAposition of one cpt
  			bs3_surface surf,   		// in : tgt bs3_surface to modify
   			int uindex,         		// in : u index of tgt control point
   			int vindex,         		// in : v index of tgt control point
   			double *pos,           // in : [xyz] loc copied into cpt,size:[3]
   			double weight,      		// in : weight to which cpt is assigned
   			                    //      only used if surf is rational
			const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
			);

// Needed for deformable surface library
/**
* Sets the position of all control points.
* <br><br>
* <b>Role:</b> Copies the new <i>xyz</i> values of the <tt>pos_vec</tt> array into %surface's
* control point data structure. If <tt>surf</tt> is rational, the weight values are also
* copied.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* target bs3_surface to modify.
* @param u_cpt_count
* number of control points in u.
* @param v_cpt_count
* number of control points in v.
* @param pos_vec
* control point locations [c00, c01, ... , c10, c11, ...] where cij = xyz for control point i,j. size = u_cpt_count * v_cpt_count * 3.
* @param weight
* weight to which control points are assigned only used if surf is rational size = u_cpt_count * v_cpt_count.
**/

DECL_SPLINE void
bs3_surface_set_ctrlpts( 		// eff: set the SPAposition of one cpt
  			bs3_surface surf,   		// in : tgt bs3_surface to modify
   			int u_cpt_count,    			// in : number of control points in u
   			int v_cpt_count,    			// in : number of control points in v
   			double *pos_vec,    		// in : array of cpt locations
                           		//      [c00, c01, ... , c10, c11, ...]
                           		//      where cij = [x y z] for cpt i,j
                           		//      sized:[u_cpt_count*v_cpt_count*3]
   			double *weight,    		// in : weight to which cpts are assigned
   			                    //      only used if surf is rational
                           		//      sized:[u_cpt_count*v_cpt_count]
			const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
			);
/**
* Creates a NUBS spline %surface from the given torus.
* <br><br>
* <b>Role:</b> Creates a parametric %surface coincident with the given torus
* over at least the portion inside the box, within the specified positional
* tolerance, and has its normal in the same sense. A NUBS approximation will be C2;
* a NURBS representation will be G1, but may not be C1.
* <br><br>
* If the <tt>actual_fit</tt> argument is supplied, it returns the actual tolerance achieved,
* or exact zero if this is better than system positional tolerance. The
* parameterization of the resulting %surface need not match in any way that of the
* original torus.
* <br><br>
* <b>Limitations:</b> The torus must NOT be degenerate (must have major radius > minor radius).
* <br><br>
* Optionally, the mapping from the old parameter bounds to the new parameter
* bounds can be returned by supplying the <tt>pt</tt> argument.
* <br><br>
* @param tor
* given torus.
* @param region_of_interest
* region of interest.
* @param fit_tol
* requested fit tolerance, >=0; if > 0, %surface will be a NUBS (C2); if =0, %surface will be a NURBS (G1, but may not be C1).
* @param actual_fit
* returns actual fit used.
* @param pt
* returns parameter space mapping.
**/
DECL_SPLINE bs3_surface
bs3_surface_make_tor_nubs(
			torus const &tor,
			SPAbox const &region_of_interest,
			double requested_fit,
			double &actual_fit=*(double*)NULL_REF,
			SPApar_transf &pt=*(SPApar_transf*)NULL_REF
			);



/** @} */
#endif

