/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Routines for processing 2D B-spline curves, used to fit true curves
// in the SPAparameter space of a surface.
#if !defined( BS2_CURVE_ROUTINES )
#define BS2_CURVE_ROUTINES
#include "dcl_spl.h"
#include "logical.h"
#include "bs2curve.hxx"
#include "bs3surf.hxx"
#include "debugmsc.hxx"
#include "param.hxx"
/**
 * @file sp2crtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS2CURVEAPI
 *
 * @{
 */
class SPAinterval;
class SPApar_pos;
class SPApar_vec;
class SPApar_dir;
class SPApar_box;
class SPApar_transf;
class SPAtransf;
class surface;
/**
 * @nodoc
 */

struct ag_spline;

// STIPORT TAC
#if defined( osf1 ) || defined( vms )
#include "param.hxx"
#include "position.hxx"
#endif

// Make all routine names external in their unadorned form, to
// give C programmers a chance.

// STI dgh {

// **** Initialisation ****

// Initialise the 2D spline curve system.
/**
* Initializes the <tt>bs2_curve</tt> interface and the underlying %curve package.
* <br><br>
* <b>Role:</b> ACIS calls this function once internally; it should <i>not</i> be
* called more than once.
* <br><br>
* <b>Effect:</b> System routine
**/
DECL_SPLINE void
bs2_curve_init();


// Shut things down (e.g. release caches of free store).


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void
bs2_curve_term();


// **** Construction ****

// Construct a straight line between two points. The resulting
// parametrisation is undefined.

/**
* Creates a %straight line B-spline between two points.
* <br><br>
* <b>Role:</b> The resulting parameterization is not defined.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param start
* start point.
* @param end
* end point.
* @param double1
* requested fit tolerance.
* @param actual_fit
* returned actual fit used.
**/
DECL_SPLINE bs2_curve
bs2_curve_make_line(
			SPApar_pos const &start,		// start SPAposition
			SPApar_pos const &end,		// end SPAposition
			double double1 = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// Construct a conic curve. Given are the start and end points of the
// segment to be represented, the intersection point of the tangents
// at the start and end, and the "rho" value. This last determines the
// SPAposition of the mid-SPAparameter point along the line joining the
// mid-point of the chord and the intersection of the tangents, and is
// simply the ratio of its distance from the chord mid-point to the
// total distance. A value of 0.5 gives a parabola, more gives a
// hyperbola, less an ellipse. We may assume that the ends are
// distinct. The resulting parametrisation is undefined.

/**
* Creates a conic %curve given three distinct parametric positions and a <i>rho</i> value.
* <br><br>
* <b>Role:</b> Given are the start and end points of the segment to be
* represented, the intersection point of the tangents at the start and end, and
* the <i>rho</i> value. This last determines the position of the mid-parameter point
* along the line joining the midpoint of the chord and the intersection of the
* tangents, and is simply the ratio of its distance from the chord midpoint to the
* total distance. A value of 0.5 gives a parabola, more gives a hyperbola, less an
* ellipse. The ends are assumed to be distinct.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param start
* start point.
* @param tan_int
* tangent intersection point.
* @param end
* end point.
* @param rho
* rho value.
* @param actual_fit
* requested fit tolerance.
* @param acfitol
* returned actual fit used.
**/
DECL_SPLINE bs2_curve
bs2_curve_make_rho_conic(
			SPApar_pos const &start,	// start
			SPApar_pos const &tan_int,	// tangent intersection point
			SPApar_pos const &end,	// end
			double rho = 0.5,		// rho
			double actual_fit = 0,				// required fit tolerance
			double &acfitol = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// Construct a cubic curve which interpolates or fits to an array of
// points, with given start and end directions. The resulting
// parametrisation is undefined.
/**
* Creates a cubic parameter-space %curve that interpolates or fits to an array of points, 
* with given start and end directions.
* <br><br>
* <b>Role:</b> The resulting parameterization is undefined. If a direction is a
* <tt>NULL</tt> reference or zero length, then a natural boundary condition is used that
* has a zero second derivative at the appropriate end.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param npts
* number of points.
* @param pts
* array of points.
* @param start_dir
* start derivative.
* @param end_dir
* end derivative.
* @param fitol
* fit tolerance.
* @param acfitol
* returned actual fit tolerance used.
**/
DECL_SPLINE bs2_curve
bs2_curve_interp(
			int npts,					// number of points
			SPApar_pos const *pts,		// array of points to interpolate or fit
			SPApar_vec const &start_dir,		// start derivative
			SPApar_vec const &end_dir,		// end derivative
			double fitol = 0,				// fit tolerance, 0 for interpolate
			double &acfitol = *(double *)NULL_REF
									// set to actual fit tolerance
		);

// Construct a parameter-space curve along a u parameter line (i.e.
// one with constant v parameter) of a spline surface.
/**
* Creates a parameter-space %curve along a <i>u</i> parameter line of a B-spline %surface.
* <br><br>
* <b>Role:</b> Creates a parameter-space %curve along a <i>u</i> parameter line (i.e.,
* one with constant <i>v</i>-parameter) of a B-spline %surface.
* <br><br>
* In this prototype, the %surface is used only to obtain the <i>u</i>-parameter range.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
* @param v
* v parameter of %surface.
**/
DECL_SPLINE bs2_curve
bs2_curve_u_param_line(
			bs3_surface surf,
			double v
		);


// Construct a parameter-space curve along a u parameter line (i.e.
// one with constant v parameter) of a spline surface.
/**
* Creates a parameter-space %curve along a <i>u</i> parameter line of a B-spline %surface.
* <br><br>
* <b>Role:</b> Creates a parameter-space %curve along a <i>u</i> parameter line (i.e.,
* one with constant <i>v</i>-parameter) of a B-spline %surface.
* <br><br>
* In this prototype, the range and form are explicitly specified.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param u_range
* parametric %curve range.
* @param form
* form of required %curve; should correspond to form of %surface (object-space meaning rather than parameter-space).
* @param v
* v parameter of %surface.
**/
DECL_SPLINE bs2_curve
bs2_curve_u_param_line(
			const SPAinterval& u_range,
			bs2_curve_form form,
			double v
		);


// Construct a parameter-space curve along a v parameter line (i.e.
// one with constant u parameter) of a spline surface.
/**
* Creates a parameter-space %curve along a <i>v</i> parameter line of a B-spline %surface.
* <br><br>
* <b>Role:</b> Creates a parameter-space %curve along a <i>v</i> parameter line (i.
* e., one with constant <i>u</i> parameter) of a B-spline %surface.
* <br><br>
* The <i>v</i>-parameter of the %surface varies for this function, while the <i>u</i>-parameter
* is fixed. The parameterization of the resulting %curve is undefined, though it
* has the same sense as the %surface <i>v</i>-parameter, but would normally be the same as
* the %surface <i>v</i>-parameter. The %curve will be open, closed, or periodic according
* to whether the %surface was open, closed, or periodic in the <i>v</i>-direction.
* <br><br>
* This function is the preferred way to create the %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
* @param u
* u parameter of %surface.
**/
DECL_SPLINE bs2_curve
bs2_curve_v_param_line(
			bs3_surface surf,
			double u
		);


// Construct a parameter-space curve along a v parameter line (i.e.
// one with constant u parameter) of a spline surface.
/**
* Creates a parameter-space %curve along a <i>v</i> parameter line of a B-spline %surface.
* <br><br>
* <b>Role:</b> Creates a parameter-space %curve along a <i>v</i> parameter line (i.
* e., one with constant <i>u</i> parameter) of a B-spline %surface.
* <br><br>
* The <i>v</i>-parameter of the %surface varies for this function, while the <i>u</i>-parameter
* is fixed. The parameterization of the resulting %curve is undefined, though it
* has the same sense as the %surface <i>v</i>-parameter, but would normally be the same as
* the %surface <i>v</i>-parameter. The %curve will be open, closed, or periodic according
* to whether the %surface was open, closed, or periodic in the <i>v</i>-direction.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param v_range
* parametric %curve range.
* @param form
* form of required %curve; should correspond to form of %surface (object-space meaning rather than parameter-space).
* @param u
* u parameter of %surface.
**/
DECL_SPLINE bs2_curve
bs2_curve_v_param_line(
			const SPAinterval& v_range,
			bs2_curve_form form,
			double u
		);


// Construct a curve which is a subset of a given one, being the
// overlap in parameter space of the given curve and a given SPAinterval.
// A curve periodic in one or both SPAparameter directions is rolled
// around if need be to cover the required range. It is very unlikely
// that the tolerance arguments will be needed, but they are included
// for completeness.
/**
* Creates a B-spline %curve that is a subset of a given B-spline %curve.
* <br><br>
* <b>Role:</b> Creates a new %curve that is a subset of the given %curve. The
* subset is the overlap in parameter space of the given %curve and a given interval.
* A %curve periodic in one or both parameter directions is rolled around if need
* be to cover the required range. It is very unlikely that the tolerance arguments
* will be needed, but they are included for completeness.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param old_bs
* given %curve.
* @param new_range
* required bounds.
* @param fit
* required fit tolerance.
* @param actual_fit
* returned actual fit tolerance used.
**/
DECL_SPLINE bs2_curve
bs2_curve_subset(
			bs2_curve old_bs,				// given curve
			SPAinterval const &new_range,		// required bounds
			double fit = 0,				// required fit tolerance
			double &actual_fit = *(double *)NULL_REF
									// returns actual fit tolerance
		);


// **** Modification ****

// Reparametrise a 2D B-spline curve by a linear transformation, to
// achieve given start and end values.
/**
* Reparameterizes the given B-spline %curve in place so that its primary interval of definition 
* is from the start to the given end parameters.
* <br><br>
* <b>Role:</b> The start to the given end parameters must be in increasing
* order.
* <br><br>
* Each new parameter value will be the appropriate linear function of the old
* parameter.
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
bs2_curve_reparam(
			double start,				// start parameter desired
			double end,				// end parameter desired
			bs2_curve cur			// given curve
		);


// Shift the parameter values of a 2D B-spline curve by a given amount.
/**
* Reparameterizes the given B-spline %curve in place by adding the given shift value to its parameter values.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param delta
* parameter shift desired.
* @param cur
* input/output %curve.
**/
DECL_SPLINE void
bs2_curve_shift(
			double delta,			// given change in SPAparameter values
			bs2_curve cur		// given curve
		);


// Reset the start and/or end parameters of a 2D B-spline curve.
/**
* Sets the values of the start and/or end parameters of a B-spline %curve.
* <br><br>
* <b>Role:</b> This function adjusts the start and/or end parameter values of a
* B-spline. The parameter values are passed in as pointers so that they may be set
* individually. A <tt>NULL</tt> pointer indicates no change at that end.
* <br><br>
* <b>Errors:</b> Knot sequences must be strictly increasing, so the new start value must
* be less than the second knot value, and the new end value must be greater than
* the penultimate knot value. If this condition is violated at either end, the
* function returns <tt>FALSE</tt> and the %curve is left unchanged.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param t0
* new start parameter.
* @param t1
* new end parameter.
* @param cur
* %curve to modify.
**/
DECL_SPLINE logical
bs2_curve_set_end_prms(
			double *t0,		// ptr to new start SPAparameter, NULL if no change
			double *t1,		// ptr to new end SPAparameter, NULL if no change
			bs2_curve cur		// given curve
		);


// Reverse the direction of a 2D B-spline curve, negating its
// parametrisation.
/**
* Reverses the direction of the given B-spline %curve, and negates the parameterization.
* <br><br>
* <b>Role:</b> Reverses the direction of the given %curve, and negates the
* parameterization (so that the new primary definition interval is [<i>-b</i>, <i>-a</i>] if the
* original was [<i>a</i>, <i>b</i>]).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE void
bs2_curve_reverse(
			bs2_curve cur			// given curve
		);


// Split a B-spline curve into two sections at a given SPAparameter value.

// This routine divides a given 2D B-spline curve at a given
// SPAparameter value.  If a surface SPAparameter-space point is given
// (which need not be exactly at the point defined by the curve and
// SPAparameter, but is assumed to be close), the appropriate end point
// of each resulting spline is shifted to lie exactly on the given
// point.

// If the B-spline is open, this routine creates a new spline for the
// initial portion of the curve, and returns this as its value.
// If the B-spline is closed, it takes the portion before the given
// SPAparameter value and tacks it on the end, but marks the spline as
// open henceforth.  In this case, the routine returns NULL.
/**
* Splits a given parametric %curve into two sections at a given parameter value.
* <br><br>
* <b>Role:</b> If a parameter-space point is given (which need not be exactly
* at the point defined by the %curve and parameter, but is assumed to be close),
* the appropriate end point of each resulting B-spline is shifted to lie exactly on
* the given position.
* <br><br>
* No attempt is made to match tangent directions at the cut.
* <br><br>
* If the B-spline is open, this routine creates a new B-spline for the initial
* portion of the %curve and returns it as its value. If the B-spline is closed, it
* takes the portion before the given parameter value and tacks on the end, but
* marks the B-spline as open. In this case, the routine returns <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param cur
* given %curve.
* @param param
* given parameter value.
* @param split_pt
* given position.
* @param split_dr
* given direction.
**/
DECL_SPLINE bs2_curve
bs2_curve_split(
			bs2_curve &cur,					// given curve
			double param,							// given SPAparameter value
			SPApar_pos const &split_pt = *(SPApar_pos *)NULL_REF,	// given SPAposition
			SPApar_dir const &split_dr = *(SPApar_dir *)NULL_REF	// UNUSED
		);


// Join two compatible 2D B-splines together (without checking for
// compatibility).
/**
* Creates a new B-spline %curve by appending the second B-spline %curve to the end of the first.
* <br><br>
* <b>Role:</b> Appends the second %curve to the end of the first, constructing a
* new combined %curve (though destroying the originals). It is assumed that the
* ends to be joined match in position and direction, and that the parameterization
* is continuous.
* <br><br>
* ACIS uses this routine only to rejoin parts of a periodic %curve that have been
* split by <tt>bs2_curve_split</tt>. They are rejoined in the opposite order so that the
* resultant %curve starts and ends at the split point. A %curve made this way is
* marked as closed by <tt>bs2_curve_join</tt>, but is not marked periodic.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param first_cur
* first %curve.
* @param last_cur
* second %curve.
**/
DECL_SPLINE bs2_curve
bs2_curve_join(
			bs2_curve first_cur,		// first given curve
			bs2_curve last_cur		// second given curve
		);

// Check Join compatible of two 2D B-splines.

// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical
bs2_curve_check_join(
			bs2_curve,		// first given curve
			bs2_curve		// second given curve
		);

//STIBUGFIX bd vrblenda, etc. begin:
//
// Connect two 2D B-splines end to end.
//
// The two input curves will be deleted if successful.
// As with bs3_curve_connect(),
// this routine takes care of compatibility of the curves,
// and of cleaning up: the two input splines are gone after
// the call, and the resulting spline is the return value.
// However, it's dangerous to say
//		s1 = bs2_curve_connect( s1, s2 );
// because if it fails, it returns a null pointer and leaves s1 and s2 alone.
// Thus s1 still exists but the caller has just zeroed its pointer.
// So always say
//		s3 = bs2_curve_connect( s1, s2 );
// then either s3 is valid and s1 and s2 are gone, or vice versa.
//
// This is similar to bs3_curve_connect(), but not identical because
// the curve values have different meanings.  In particular, it
// does not compare end points to decide which end to connect to which.
// Also, the second curve is not reparameterized to be true C1 with
// the first curve, because that's not appropriate in SPAparameter space.
// As with bs2_curve_join(), the second curve will be translated
// to match the first curve's end point.  This also applies to the
// parameterizations.
//
/**
* Joins two 2D B-spline curves end to end.
* <br><br>
* <b>Role:</b> The two input curves will be deleted if successful. As with
* <tt>bs3_curve_connect</tt>, this routine takes care of compatibility of the curves, and
* of cleaning up: the two input B-splines are gone after the call, and the resulting
* B-spline is the return value.
* <br><br>
* It's dangerous to code:
* <br><br>
* <tt>s1 = bs2_curve_connect(s1, s2)</tt>
* <br><br>
* because if it fails, it returns a <tt>NULL</tt> pointer and leaves <tt>s1</tt> and <tt>s2</tt> alone. Thus
* <tt>s1</tt> still exists but the caller has just zeroed its pointer. So always code:
* <br><br>
* <tt>s3 = bs2_curve_connect(s1, s2)</tt>
* <br><br>
* then either <tt>s3</tt> is valid and <tt>s1</tt> and <tt>s2</tt> are gone, or vice versa.
* <br><br>
* This is similar to <tt>bs3_curve_connect</tt>, but not identical because the %curve values
* have different meanings. In particular, it does not compare end points to decide
* which end to connect to which. Also, the second %curve is not reparameterized to
* be true C1 with the first %curve, because that's not appropriate in parameter
* space. As with <tt>bs2_curve_join</tt>, the second %curve will be translated to match the
* first %curve's end point. This also applies to the parameterizations.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crv1
* first %curve.
* @param crv2
* second %curve.
**/
DECL_SPLINE bs2_curve
bs2_curve_connect( bs2_curve crv1, bs2_curve crv2 );

//STIBUGFIX bd vrblenda, etc. end.

// Transform a 2D curve in object space.
// Because the curve is in the SPAparameter space of a surface, most
// of the transformation can be ignored.
/**
* Transforms the given B-spline %curve in place.
* <br><br>
* <b>Role:</b> A <tt>SPAtransf</tt> consists of a 3x3 matrix with unit determinant,
* giving an affine transformation, an overall scaling factor and a translation
* vector. The three logical flags relating to the matrix are:
* <br><br>
* <tt>rotate</tt> indicates whether the matrix is anything other than the identity.
* <br><br>
* <tt>reflect</tt> indicates whether the determinant is -1.
* <br><br>
* <tt>shear</tt> is set if the matrix is not orthogonal.
* <br><br>
* This function is only provided for completeness. Because both %curve and %surface
* parameterizations are transformation-independent, there is nothing for it to do.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crv1
* given %curve.
* @param trns
* transform.
**/
DECL_SPLINE void
bs2_curve_trans(
			bs2_curve crv1,		// given curve
			SPAtransf const &trns	// given transform
		);

// STI let (9/98) Added 3 new bs2_curve functions

// Transform a 2D curve in SPAparameter space.
/**
* Transforms the given <tt>bs2_curve</tt> in parameter space.
* <br><br>
* <b>Role:</b> A <tt>SPApar_transf</tt> consists of scaling and translation components.
* Thus, a <tt>bs2_curve</tt> can be scaled and translated in parameter space using this
* function.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %curve.
* @param t
* transform.
**/
DECL_SPLINE void
bs2_curve_par_trans(
			bs2_curve bs,			// given curve
			SPApar_transf const &t	// given transform
		);

// This function allows a single control point of a bs2_curve to be modified.

/**
* Sets the position of one control point.
* <br><br>
* <b>Role:</b> Checks that <tt>curv</tt> has a control point at the given index. If it
* does, it copies the <i>uv</i> values of <tt>pos</tt> into control point's data structure. When
* <tt>curv</tt> is rational, it also copies the <tt>weight</tt> value into the control point's data
* structure.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param curv
* bs2_curve to modify.
* @param index
* index of target control point.
* @param pos
* uv location copied into control point.
* @param weight
* weight to which control point is assigned, only used if %curve is rational.
**/
DECL_SPLINE void
bs2_curve_set_ctrlpt(    // eff: set the SPAposition of one cpt
  		bs2_curve curv,       // in : tgt bs2_curve to modify
   		int index,             // in : index of tgt control point
   		SPApar_pos const &pos,  // in : [uv] loc copied into cpt
    	double weight,          // in : weight to which cpt is assigned
                                //     only used if curv is rational
		const int use_initial_seam_multiplicity = FALSE
        );


// This function allow all of the control points of a bs2_curve to be modified.
/**
* Sets the positions, and optionally weights, of all control points on a %curve.
* <br><br>
* <b>Role:</b> Iterates through all control points on <tt>curv</tt>. Copies the values
* of <tt>pos</tt> into the corresponding control points. If <tt>curv</tt> is rational, it also
* copies the <tt>weight</tt> values.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param curv
* bs2_curve to modify.
* @param pos
* [uv0,uv1...] to copy sized:[cpt_count].
* @param weight
* weight to which control point is assigned, only used if %curve is rational.
**/
DECL_SPLINE void
bs2_curve_set_ctrlpts(         // eff: set the SPAposition of all cpts
  		bs2_curve curv,        // in : tgt bs2_curve to modify
   		const SPApar_pos pos[],   // in : [uv0,uv1..] to copy sized:[cpt_count]
   		const double weight[], // in : weights for each cpt. only used if
                               //      curv is rational sized:[cpt_count]
		const int use_initial_seam_multiplicity = FALSE
        );


// This function replaces a wiggling bs2_curve with a linear bs2_curve when
// the 3-space error between the curves is less than fit.  Also snaps the
// linear bs2_curve to the surface bounds when close.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs2_curve_simplify(
        bs2_curve curv,               // eff    : simplifies the bs2
        double &fit,                  // in/out : 3-space requested error and error achieved
        surface const &surf,          // in     : bs2 lies on this surface
        SPApar_pos *sug_start = NULL,    // in     : suggested start uv (optional)
        SPApar_pos *sug_end = NULL,      // in     : suggested end uv (optional)
        double  *sug_low = NULL,      // in     : suggested low param (optional)
        double  *sug_high = NULL      // in     : suggested high param (optional)
    );

// **** Enquiry ****

// Return a logical indicating whether the curve form is open or not
/**
* Determines if the B-spline %curve is open or not.
* <br><br>
* <b>Role:</b> The end points of an open %curve do not meet to form a closed
* loop.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE logical
bs2_curve_open(
			bs2_curve cur		// given curve
		);

// Return a logical indicating whether the curve is a closed loop
// or not.

/**
* Determines whether a B-spline is closed or open.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE logical
bs2_curve_closed(
			bs2_curve cur		// given curve
		);


// Return a logical indicating whether the curve is periodic
// or not.
/**
* Determines if the B-spline %curve is smoothly closed in object space 
* and may be considered an endless loop (periodic).
* <br><br>
* <b>Role:</b> If the %curve is periodic, the %curve is defined for all parameter
* values.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE logical
bs2_curve_periodic(
			bs2_curve cur			// given curve
		);

// Return the SPAparameter range of a 2D B-spline curve.
// Interval returned will be [0.0, 0.0] if given (pointer to) curve
// is NULL.
/**
* Gets the primary interval on which the B-spline %curve is defined.
* <br><br>
* <b>Role:</b> In the open and non-periodic closed curves, the interval is also
* the actual domain.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE SPAinterval
bs2_curve_range(
			bs2_curve cur		// given curve
		);


// Return the SPAparameter period of a 2D B-spline curve.
// Result returned will be zero if curve is open or non-periodic, or
// given (pointer to) curve is NULL.
/**
* Gets the primary interval (the parametric period) of definition of a periodic B-spline %curve.
* <br><br>
* <b>Role:</b> This function returns exactly zero for any non-periodic %curve.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE double
bs2_curve_period(
			bs2_curve cur		// given curve
		);


// **** Evaluation ****


// Return the number of derivatives that bs2_curve_evaluate can calculate.

/**
* Gets the number of derivatives that <tt>bs2_curve_evaluate</tt> can calculate.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param Giv_cur
* given %curve.
**/
DECL_SPLINE int
bs2_curve_accurate_derivs(
			bs2_curve Giv_cur
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
bs2_curve_knottol();

// General evaluator, giving an arbitrary number of derivatives (up
// to a maximum returned by "accurate_derivs"), and selection of the
// handedness of derivatives at discontinuities. Returns the number
// of derivatives actually evaluated - any requested but beyond the
// maximum are set to zero.
/**
* Evaluates an arbitrary number of derivatives and selects the handedness of derivatives at discontinuities.
* <br><br>
* <b>Role:</b> Gives an arbitrary number of derivatives (up to a maximum
* returned by <tt>accurate_derivs</tt>), and selects the handedness of derivatives at
* discontinuities. This function returns the number of derivatives actually
* evaluated. Any derivatives requested but beyond the maximum are set to 0.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param param
* given parameter.
* @param cur
* given %curve.
* @param pos
* returned parametric position.
* @param deriv
* array of at least nd pointers to locations into which derivatives are to be placed.
* @param nd
* number of derivatives to be evaluated.
* @param index
* negative to evaluate the left-hand derivative at a knot, positive to evaluate the right-hand derivative, 0 for don't care.
**/
DECL_SPLINE int
bs2_curve_evaluate(
			double param,					// given SPAparameter t
			bs2_curve cur,				// given curve
			SPApar_pos &pos,				// parametric SPAposition returned
			SPApar_vec **deriv = NULL,		// array of pointers to locations
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

// Evaluate position and derivatives of a given 2D B-spline curve at
// given parameter value. Any of the return arguments may be a NULL
// pointer, when the corresponding value is not returned.
/**
* Evaluates the B-spline %curve and its parametric derivative at the given parameter value.
* <br><br>
* <b>Role:</b> When that value is not returned and need not be evaluated,
* either of the two return values may be <tt>NULL</tt> references.
* <br><br>
* The number of derivatives evaluated depends upon the last two arguments. If the
* last argument is not <tt>NULL</tt>, two derivatives will be evaluated. If the last
* argument is <tt>NULL</tt> and the next-to-last argument is not <tt>NULL</tt>, one derivative will
* be calculated. If both are <tt>NULL</tt>, no derivatives will be calculated.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param param
* given parameter value.
* @param cur
* given %curve.
* @param x
* returned parametric position.
* @param xdot
* returned first derivative.
* @param xdotdot
* returned second derivative.
**/
DECL_SPLINE void
bs2_curve_eval(
			double param,					// given SPAparameter value
			bs2_curve cur,				// given curve
			SPApar_pos &x,				// parametric SPAposition returned
			SPApar_vec &xdot = *(SPApar_vec *)NULL_REF,
									// first derivative returned
			SPApar_vec &xdotdot = *(SPApar_vec *)NULL_REF
									// second derivative returned
		);


// Evaluate parametric position on a 2D B-spline curve at
// given SPAparameter value.

/**
* Evaluates the B-spline %curve at the given parameter value.
* <br><br>
* <b>Role:</b> Normally, this is implemented as a call to <tt>bs2_curve_eval</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param param
* given parameter value.
* @param cur
* given %curve.
**/
DECL_SPLINE SPApar_pos
bs2_curve_position(
			double param,			// given SPAparameter value
			bs2_curve cur		// given curve
		);


// Evaluate the first derivative of a 2D B-spline curve at given
// SPAparameter value.

/**
* Evaluates the first derivative of the B-spline %curve with respect to the parameter at the given parameter value.
* <br><br>
* <b>Role:</b> Normally, this is implemented as a call to <tt>bs2_curve_eval</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param param
* given parameter value.
* @param cur
* given %curve.
**/
DECL_SPLINE SPApar_vec
bs2_curve_deriv(
			double param,			// given SPAparameter value
			bs2_curve cur		// given curve
		);


// Evaluate the tangent to a 2D B-spline curve at given SPAparameter value.
/**
* Evaluates the B-spline %curve direction at the given parameter value.
* <br><br>
* <b>Role:</b> Normally, this is implemented as a call to <tt>bs2_curve_eval</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param param
* given parameter value.
* @param cur
* given %curve.
**/
DECL_SPLINE SPApar_dir
bs2_curve_tangent(
			double param,			// given SPAparameter value
			bs2_curve cur		// given curve
		);


// **** Geometric Properties ****

// Find the parameter of a point on a 2D B-spline curve.
// If a parameter value close to that desired is known, it may be given
// as the last argument (ignored at present).

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE double
bs2_curve_invert(
			SPApar_pos const &,	// given point
			bs2_curve,			// given curve
			SPAparameter const & = *( SPAparameter * ) NULL_REF  // SPAparameter guess
		);


// Drop a perpendicular from a point to a spline curve, returning
// the foot of the perpendicular, and the tangent direction and
// SPAparameter value at that point.

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void
bs2_curve_perp(
			SPApar_pos const &,	// given point
			bs2_curve,			// curve
			SPApar_pos &,			// (returned) foot of perpendicular
			SPApar_dir &,		// (returned) curve tangent
			SPAparameter const & = *( SPAparameter * ) NULL_REF,
								// supplied approximate SPAparameter
			SPAparameter & = *( SPAparameter * ) NULL_REF
								// (returned) actual SPAparameter
		);


// Return the (local space) bounding SPAbox for the given curve, expanded
// by a given tolerance (in case the curve is only a fit to a true
// curve).
/**
* Gets a box that encloses the B-spline %curve with additional tolerances.
* <br><br>
* <b>Role:</b> Returns a box (presently a rectangular parallelepiped parallel
* to parameter-space axes) that completely contains the %curve, with an additional
* allowance of the given tolerance all around. The box will not be the smallest
* possible, but will be a compromise between a tight fit and fast evaluation.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
* @param fitol
* given tolerance.
**/
DECL_SPLINE SPApar_box
bs2_curve_box(
			bs2_curve cur,		// given curve
			double fitol			// given tolerance
		);


// Test two curves to be the same. More accurately, this tells us
// if they are different - coincident curves with different
// representation will be flagged "different", so this is only useful
// as an optimising filter.
/**
* Determines whether two B-spline curves are the same.
* <br><br>
* <b>Role:</b> This routine checks that the two curves share the same form, and
* the same knot vectors and control points (within tolerance). The control points
* may (all) be shifted by any multiple of <tt>u_period</tt> or <tt>v_period</tt>. These values refer
* to the %surface in whose parameter space this <tt>bs2_curve</tt> is embedded, and they
* default to zero. This routine is not to be used for coincidence testing, but
* only as a simple filter to discard obvious cases.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs1
* first %curve.
* @param bs2
* second %curve.
* @param tol
* parameter space tolerance for equal control points.
**/
DECL_SPLINE logical
bs2_curve_same(
			bs2_curve bs1,
			bs2_curve bs2,
			double tol = 0.0	// SPAparameter-space tolerance for equal
							// control points.
		);


// **** Utility ****

// Duplicate (in free store) and return, a 2D B-spline curve.

/**
* Creates an exact copy of the B-spline %curve.
* <br><br>
* <b>Role:</b> Makes an exact copy of the %curve. ACIS calls this routine only
* when a change is to be made to one copy of the %curve duplication further.
* Ordinary duplication of ACIS parameter-space curves merely creates a new
* reference to the same underlying <tt>bs2_curve</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param cur
* given %curve.
**/

DECL_SPLINE bs2_curve
bs2_curve_copy(
			bs2_curve cur		// given curve
		);


// Remove a 2D B-spline curve from free store.

/**
* Deletes storage occupied by a B-spline %curve that is no longer required.
* <br><br>
* <b>Role:</b> If it does not prevent the operation of the standard C memory
* allocation mechanism, ACIS makes no assumptions about how the underlying %surface
* package manages its storage space.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE void
bs2_curve_delete(
			bs2_curve &cur
		);


// Save a 2D B-spline curve to disc in recoverable format.
/**
* Saves a B-spline %curve to a file.
* <br><br>
* <b>Role:</b> Writes a representation of the parametric %curve to some external
* medium, using routines <tt>write_int</tt>, <tt>write_long</tt>, <tt>write_real</tt>, and <tt>write_string</tt>,
* defined in ACIS file <tt>kernutil/fileio/fileio.hxx</tt>. It makes a format the allows
* reconstruction of the %curve from the data by a single sequential pass.
* <br><br>
* The overloaded &lt;&lt; operator acts like <tt>bs2_curve_save</tt>, but writes to a C++ style
* stream using stream operators. The output format need not necessarily be the
* same as for <tt>bs2_curve_save</tt>, but it is strongly recommended that it be so. For
* example:
* <br><br>
* <tt>bs2_curve cur</tt>;
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param cur
* given %curve.
**/
DECL_SPLINE void
bs2_curve_save(
			bs2_curve cur
		);


// Restore a 2D B-spline from disc.

/**
* Restores a B-spline %curve.
* <br><br>
* <b>Role:</b> Reads back a representation of a parametric %curve written by
* <tt>bs2_curve_save</tt> and constructs a duplicate of the original %curve. Reading uses
* routines <tt>read_int</tt>, <tt>read_long</tt>, <tt>read_real</tt>, and <tt>read_string</tt> defined in
* <tt>kernutil/fileio/fileio.hxx</tt>.
* <br><br>
* The overloaded &gt;&gt; operator acts like <tt>bs2_curve_restore</tt>, but reads from a C++
* style stream using stream operators and sets the result into the second argument.
* For example:
* <br><br>
* <tt>bs2_curve cur</tt>;
* <br>
* <tt>bs_2_3_spline_restore</tt> Information to restore from SAT
* <br><br>
* <b>Effect:</b> Changes model
**/
DECL_SPLINE bs2_curve
bs2_curve_restore();


// Produce a simple-minded printout of a 2D B-spline curve

/**
* Gets a readable representation of the B-spline %curve and writes it to a file.
* <br><br>
* <b>Role:</b> If there is more than one text line (as is almost certain), all
* lines but the first start with the leader string. Do not terminate the last line
* by a new line.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param cur
* given %curve.
* @param leader
* leader string.
* @param fp
* debug output file.
**/
DECL_SPLINE void
bs2_curve_debug(
			bs2_curve cur,
			char const *leader,
			FILE *fp = debug_file_ptr
		);



// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void
bs2_set_box_bs( ag_spline *bs );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE ag_spline*
bs2_bld_bs( ag_spline* next, ag_spline* prev,
	int ctype, int degree, int num, int rat, int form );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs2_rem_kn_red( ag_spline* bs, double tol );

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs2_rem_knot( ag_spline* bs, double tol, logical remove );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE double
bs2_len_bs( ag_spline* bs, double tol, int &err );
// STI dgh }	// end of 

/**
* Determines whether the B-spline %curve is linear, within the specified tolerance.
* <br><br>
* <b>Role:</b> The routine returns one of the following values:
* <br>
* &nbsp;-1 &nbsp;&nbsp;&nbsp;&nbsp;Curve is a point or a degenerate B-spline.
* <br>
* &nbsp;&nbsp;0 &nbsp;&nbsp;&nbsp;&nbsp;Curve is not a %straight line.
* <br>
* &nbsp;&nbsp;1 &nbsp;&nbsp;&nbsp;&nbsp;Curve is a %straight line, and direction is set.
* <br><br>
* If the given %curve is %straight, it sets the argument direction to be the direction of
* the %straight line in parameter space.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bc
* given %curve.
* @param direction
* direction of %straight line.
* @param tolerance
* within this tolerance.
**/
DECL_SPLINE int
bs2_curve_linear( const bs2_curve& bc, SPApar_dir &direction = *(SPApar_dir*)NULL_REF, double tolerance = SPAresabs );

/** @} */
#endif
