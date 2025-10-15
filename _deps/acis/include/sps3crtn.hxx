/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Routines for processing 3D B-spline curves, used to fit object-space
// curves to intersections of surfaces.
#if !defined( SG_BS3_CURVE_ROUTINES )
#define SG_BS3_CURVE_ROUTINES
#include "dcl_spl.h"
#include "logical.h"
#include "bs3curve.hxx"
#include "box.hxx"
#include "bs2curve.hxx"
#include "bs3surf.hxx"
#include "debugmsc.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "position.hxx"
#include "sp3crtn.hxx"
/**
 * @file sps3crtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3CURVEAPI
 *
 * @{
 */
class curve_curve_int;
class curve_surf_int;
class curve_extremum;
class SPAtransf;
class SPAparameter;
class SPApar_pos;
class SPApar_dir;
class curve;
class straight;
class ellipse;
class surface;
class plane;
class cone;
class sphere;
class spline;
class intcurve_data;
class intcurve;
class SPAposition;
class SPAunit_vector;

class curve_irregularities;

// STI dgh
class bs3_offset_seg;

// bs3_curve_conic_type returns this type:

/**
* Classifies a B-spline %curve as a type of conic section.
* @param bs_not_conic
* The B-spline %curve is not a conic
* @param bs_circle
* The B-spline %curve is a circle
* @param bs_ellipse
* The B-spline %curve is an ellipse
* @param bs_parabola
* The B-spline %curve is a parabola
* @param bs_hyperbola
* The B-spline %curve is a hyperbola
* @param bs_line
* The B-spline %curve is a line
* @param bs_polyline
* The B-spline %curve is a polyline
**/

enum bs_conic_type {
	bs_not_conic,
	bs_circle,
	bs_ellipse,
	bs_parabola,
	bs_hyperbola,
	bs_line,
	bs_polyline
};

// STI let (r6926): Extracted a new function from bs3_curve_from_ctrlpts().
// This should not be documented for now...


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void bs3_curve_determine_form( bs3_curve bs );


// Create a 3D B-spline curve specified as a sequence of control
// points, weights  and associated knot SPAvector. The control point
// tolerance and knot tolerance are used to determine when points
// or knots are the same.

/**
* Creates a 3D B-spline %curve specified as a sequence of control points,weights,and an associated knot vector.
* <br><br>
* <b>Role:</b> The control point tolerance and knot tolerance are used to
* determine when control points or knots are the same. If an error occurs, a <tt>NULL</tt>
* %curve is returned.
* <br><br>
* Allowed values for control point dimension are 1, 2 or 3. Default value is 3
* corresponding to x, y and z.
* <br><br>
* <i><b>Note:</b> Control dimension does NOT depend on rational or non-rational.</i>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param degree
* degree.
* @param rational
* rational.
* @param closed
* closed.
* @param periodic
* periodic.
* @param num_ctrlpts
* number of control points.
* @param positionctrlpts
* control points array.
* @param weights
* weights array.
* @param ctrlpt_tol
* control point tolerance.
* @param num_knots
* number of knots.
* @param knots
* knots array.
* @param knot_tol
* knot tolerance.
* @param dimension
* control point dimension specified in ctrlpts array.
**/

DECL_SPLINE bs3_curve
bs3_curve_from_ctrlpts(
	int degree,					// degree
	logical rational,			// rational
	logical closed,				// closed
	logical periodic,			// periodic
	int num_ctrlpts,				// number of control points
	const SPAposition ctrlpts[],	// control points
	const double weights[],			// weights
	double ctrlpt_tol,				// control point tolerance
	int num_knots,					// number of knots
	const double knots[],			// knots
	double knot_tol,			 		// knot tolerance
	const int &dimension = *(int *)NULL_REF	// Control point dimension specified in ctrlpts array.
										// Allowed values are 1, 2, or 3.
										// Default corresponds to 3 ==> x, y, and z.
										// Note: dimension does NOT depend on rational or non-rational
	);

/**  THIS FUNCTION (bs3_curve_from_periodic_ctrlpts) NEEDS TO BE
 *  REMOVED FROM THE PUBLIC INTERFACE AS IT IS NOT QUITE CONSISTENT
 *  WITH THE SURVIVING PERIODIC IMPLEMENTATION AND IT DOES NOT HAVE
 *  THE CAPABILITY OF TRACKING THE SEAM MULTIPLICITY.
 */
DECL_SPLINE bs3_curve
bs3_curve_from_periodic_ctrlpts(
	int degree,						// degree
	logical rational,				// rational
	int num_ctrlpts,				// number of control points
	const SPAposition ctrlpts[],	// control points
	const double weights[],			// weights
	int num_knots,					// number of knots
	const double knots[],			// knots
	double knot_tol,			 		// knot tolerance
	const int &dimension = *(int *)NULL_REF // Control point dimension specified in ctrlpts array.
										// Allowed values are 1, 2, or 3.
										// Default corresponds to 3 ==> x, y, and z.
										// Note: dimension does NOT depend on rational or non-rational
	);

// Return the dimension, degree, control points, weights, and knots
// for a 3D B-spline curve. Also indicate if the curve is rational or
// not.

/**
* Gets the dimension, degree, rationality, control points, weights, and knots for a 3D B-spline %curve.
* <br><br>
* <b>Role:</b> This function creates arrays of control points, weights, and
* knot points.
* <br><br>
* If the %curve is rational (i.e., <tt>rat</tt> = <tt>TRUE</tt>), an array of weights is returned.
* Otherwise, the weight array is <tt>NULL</tt>. Uses only the <i>x</i>- and <i>y</i>-components of the
* positions returned as the control points.
* <br><br>
* It is up to the application to delete the returned arrays. Here is an example
* of how to cleanup after calling this function:
* <pre>
* int dim, deg, num_ctrlpts, num_knots;
* logical rat;
* SPAposition *ctrlpts = NULL;
* double weights = NULL, knots = NULL;
*
* bs3_curve_to_array(bs, dim, deg, rat, num_ctrlpts, ctrlpts, weights, num_knots, knots);
*
* // cleanup
* ACIS_DELETE [] ctrlpts;
* ACIS_DELETE [] STD_CAST weights;
* ACIS_DELETE [] STD_CAST knots; </pre>
* @param bs
* given %curve.
* @param dim
* returned dimension.
* @param deg
* returned degree.
* @param rat
* returned rational.
* @param num_ctrlpts
* returned number of control points.
* @param ctrlpts
* returned control points.
* @param weights
* returned weights.
* @param num_knots
* returned number of knots.
* @param knots
* knots.
* @param use_initial_seam_multiplicity
* for internal use only.
* <br><br>
* @see ACIS_DELETE, STD_CAST
**/

DECL_SPLINE void
bs3_curve_to_array(
			bs3_curve bs		,// curve
			int& dim			,// dimension
			int& deg			,// degree
			logical& rat		,// rational
			int& num_ctrlpts			,// number of control points
			SPAposition*& ctrlpts		,// control points
			double*& weights		,// weights
			int& num_knots			,// number of knots
			double*& knots		,// knots
			const int use_initial_seam_multiplicity = FALSE // in: used for periodic geometry when initial knot multiplicty != order
		);

// Return the number of knots, and the knot SPAvector
// for a 3D B-spline curve.

/**
* Gets the number of knots and knot values for a 3D B-spline %curve.
* <br><br>
* <b>Role:</b> This function creates arrays of knot points. It is up to the
* application to delete these arrays.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
* @param num_knots
* number of knots.
* @param knots
* knots.
**/

DECL_SPLINE void
bs3_curve_knots(
			bs3_curve bs		,// curve
			int& num_knots			,// number of knots
			double*& knots		,// knots
			const int use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
		);

// Return the number of control points and an array of control points
// for a 3D B-spline curve.

/**
* Gets the number of control points and an array of control points for a 3D B-spline %curve.
* <br><br>
* <b>Role:</b> This function creates an array of control points. It is up to
* the application to delete this array.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %curve.
* @param num_pts
* number of control points output.
* @param ctrlpts
* output control point array.
**/

DECL_SPLINE void
bs3_curve_control_points(
			bs3_curve bs		,// curve
			int& num_pts			,// number of control points
			SPAposition*& ctrlpts 		,// control point array
			const int   use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
		);

// Gets the weight value corresponding to a specific control point
// for a rational, 3D B-spline curve.


/**
* Gets the weight value corresponding to a specific control point for a rational, 3D B-spline %curve.
* <br><br>
* <b>Role:</b> This function returns the weight associated with a control point
* of a rational 3D %curve.
* <br><br>
* <b>Errors:</b> The index value must be valid and the %curve must be rational.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs
* input %curve.
* @param index
* returned number of weights.
* @param weight
* weight array.
**/

DECL_SPLINE logical
bs3_curve_weight(
			bs3_curve bs		,// curve
			int index				,// index number of weight (or control point)
			double &weight			,// returned weight value
			const int   use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
		);

// Returns the number of weights and the weights for a rational, 3D
// B-spline curve.

/**
* Gets the number of weights and the values of the weights for a rational, 3D B-spline %curve.
* <br><br>
* <b>Role:</b> This function creates an array of weights for a %curve. It the
* application's responsibility to delete this array.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs
* input %curve.
* @param num_pts
* returned number of weights.
* @param weights
* returned weight array.
**/

DECL_SPLINE void
bs3_curve_weights(
			bs3_curve bs		,// curve
			int &num_pts			,// number of weights
			double *&weights		,// weight array
			const int   use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
		);

// Return an array of points and SPAparameter values which form the
// linear approximation to a curve within a specified tolerance.

/**
* Gets an array of points and parameter values that form the linear approximation to a %curve within a specified tolerance.
* <br><br>
* <b>Role:</b> Two arrays are input to the routine, one for %curve positions and
* one for the parameter values at these positions. The dimension of these arrays
* is specified by the maximum number of points to return.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
* @param s
* starting parameter.
* @param e
* ending parameter.
* @param tol
* tolerance.
* @param nmax
* max number of points.
* @param npts
* returned number of points generated, set to nmax+1 if nmax exceeded.
* @param pts
* returned array of points on the %curve.
* @param t
* returned parameter values at points.
**/

DECL_SPLINE void
bs3_curve_facet(
			bs3_curve bs		,// curve to approximate
			double s			,// starting SPAparameter on curve
			double e			,// ending SPAparameter on curve
			double tol			,// tolerance
			int nmax				,// maximum number of points
			int &npts			,// current number of points
			SPAposition pts[]		,// points on curve
			double t[]		 // SPAparameter value at point
		);

// STI INGR mnl: begin
// Mark a bs3_curve as being closed


/**
* Sets the form of a <tt>bs3_curve</tt> to be closed.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crv
* given %curve.
**/

DECL_SPLINE void
bs3_curve_set_closed(
		bs3_curve crv
	);

// Mark a bs3_curve as being open

/**
* Sets the form of a <tt>bs3_curve</tt> to be open.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param crv
* given %curve.
**/

DECL_SPLINE void
bs3_curve_set_open(
		bs3_curve crv
	);
// STI INGR mnl: end

// Mark a bs3_curve as being periodic


/**
* Sets the form of a <tt>bs3_curve</tt> to be periodic.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param crv
* given %curve.
**/

DECL_SPLINE void
bs3_curve_set_periodic(
		bs3_curve crv
	);

// bmt, 2-Oct-08: changed to no-doc, was doc'd. 
/**
 * @nodoc
 */

DECL_SPLINE double
bs3_curve_intcurve_invert(		// return SPAparameter value
		const SPAposition &pos,	// point to invert
		const intcurve &this_int,	// associated intcurve
		const SPAparameter &param_guess	// guess SPAparameter
	);

// Returns TRUE if the bs3_curve is a planar curve in which case the
// normal to the plane is also returned.


/**
* Determines whether the %curve is planar, within the specified tolerance.
* <br><br>
* <b>Role:</b> The routine returns one of the following values:
* <br><br>
* -2 = Curve is a point or a degenerate B-spline.
* <br>
* -1 = Curve is a straight line, normal not set.
* <br>
* &nbsp;0 = Curve is nonplanar.
* <br>
* &nbsp;1 = Curve is planar, and normal is set.
* <br><br>
* If the given %curve is planar, it sets the argument normal to be the normal of
* the %plane in which the given B-spline lies.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bc
* given %curve
* @param normal
* normal to %plane
* @param tolerance
* within this tolerance
**/

DECL_SPLINE int								// returns TRUE if the curve is planar
bs3_curve_planar(
		const bs3_curve &bc,		// bs3_curve
		SPAunit_vector &normal, 			// normal to the plane
		double tolerance = SPAresabs
	);

// Offset a planar bs3_curve by a SPAvector. The SPAvector does not have to
// lie in the plane of the curve.

/**
* Offsets a planar <tt>bs3_curve</tt> by a vector.
* <br><br>
* <b>Role:</b> The vector does not have to lie in the %plane of the %curve. If an
* error occurs, a <tt>NULL</tt> %curve is returned.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %curve.
* @param offset
* vector to offset by.
**/

DECL_SPLINE bs3_curve
bs3_curve_offset_by_vec(
		const bs3_curve bs,		// bs3_curve to offset
		const SPAvector& offset	// SPAvector to offset by
	);

// Offset a planar bs3_curve by a specified distance. The curve is offset
// in the plane in which it lies, and either to the right (or left) as
// viewed along the curve.

/**
* Offsets a planar <tt>bs3_curve</tt> by a specified distance.
* <br><br>
* <b>Role:</b> The %curve is offset in the %plane in which it lies, and either to
* the right (or left) as viewed along the %curve. If an error occurs, a <tt>NULL</tt> %curve
* is returned.
* <br><br>
* The <tt>plane_nor</tt> argument supplies the %plane normal, and is used only for linear
* b-spline cases. In all other cases <tt>plane_nor</tt> is ignored.
* <br><br>
* <b>Errors:</b> If the B-spline is linear and <tt>plane_nor</tt> is not specified, the function
* returns a <tt>NULL</tt> b-spline (unsuccessful).
* <br><br>
* If the given B-spline is a degenerate B-spline the function returns a <tt>NULL</tt>
* B-spline (unsuccessful).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %curve.
* @param dist
* distance to offset.
* @param to_right
* direction to offset (left or right).
* @param plane_nor
* direction of %plane normal.
**/

DECL_SPLINE bs3_curve
bs3_curve_offset_by_dist(
		const bs3_curve bs,		// bs3_curve to offset
		double dist,			// distance to offset
		logical to_right,		// direction (left or right) to offset
		const SPAunit_vector& plane_nor = *(SPAunit_vector *)NULL_REF	// Curve normal
	);

// STI dgh begin
// TEMPORARY interface for STRATA.  Do not include in official documentation
// It will be removed in future releases.


/**
* Offsets a planar <tt>bs3_curve</tt> by a specified distance.
* <br><br>
* <b>Role:</b> The %curve is offset in the %plane in which it lies, and either to
* the right (or left) as viewed along the %curve. If an error occurs, a <tt>NULL</tt> %curve
* is returned.
* <br><br>
* The <tt>plane_nor</tt> argument supplies the %plane normal, and is used only for linear
* b-spline cases. In all other cases <tt>plane_nor</tt> is ignored.
* <br><br>
* <b>Errors:</b> If the B-spline is linear and <tt>plane_nor</tt> is not specified, the function
* returns a <tt>NULL</tt> B-spline (unsuccessful).
* <br><br>
* If the given B-spline is a degenerate B-spline, the function returns a <tt>NULL</tt>
* B-spline (unsuccessful).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs
* given %curve.
* @param dist
* distance to offset.
* @param to_right
* direction to offset (left or right).
* @param make_seg_list
* %curve normal.
* @param seg_list
* list of segments.
* @param plane_nor
* %plane normal.
**/

DECL_SPLINE bs3_curve
bs3_curve_offset_by_dist(
		const bs3_curve bs,		// bs3_curve to offset
		double dist,			// distance to offset
		logical to_right,		// direction (left or right) to offset
		logical make_seg_list,
		bs3_offset_seg *&seg_list,
		SPAunit_vector& plane_nor = *(SPAunit_vector *)NULL_REF	// Curve normal
	);

// END TEMPORARY INTERFACE
// STI dgh end

// Find the start SPAposition of a bs3_curve

/**
* Gets the start point of the given B-spline %curve.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAposition
bs3_curve_start(
	bs3_curve bs 			// Bs3_curve, which start is to be found
);

// Find the start tangent of a bs3_curve

/**
* Gets the normalized tangent to the given B-spline at the start.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAunit_vector
bs3_curve_start_tangent(
	bs3_curve bs			// bs3_curve, which start_tangent is to be found
);

// STI jmb begin: Add end and mid for completness
// Find the end SPAposition of a bs3_curve

/**
* Gets the end point of the given B-spline %curve.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAposition
bs3_curve_end(
	bs3_curve bs 			// Bs3_curve, which end is to be found
);

// Find the end tangent of a bs3_curve

/**
* Gets the normalized tangent to the given B-spline at the end.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAunit_vector
bs3_curve_end_tangent(
	bs3_curve bs			// bs3_curve, which end_tangent is to be found
);

// Find the mid SPAposition of a bs3_curve

/**
* Gets the mid-point of the given B-spline %curve.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAposition
bs3_curve_mid(
	bs3_curve bs 			// Bs3_curve, which mid is to be found
);

// Find the mid tangent of a bs3_curve

/**
* Gets the normalized mid-tangent to the given B-spline.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %curve.
**/

DECL_SPLINE SPAunit_vector
bs3_curve_mid_tangent(
	bs3_curve bs			// bs3_curve, which mid_tangent is to be found
);
// STI jmb end:

// Find the self intersections in a curve

/**
* Determines self-intersections in a %curve.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param curv
* given %curve.
**/

DECL_SPLINE curve_curve_int *
bs3_curve_self_int(
		bs3_curve &curv
	);

// Find the intersection of two curves
/**
* Determines the intersection of two curves.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param curv1
* first given %curve.
* @param curv2
* second given %curve.
**/
DECL_SPLINE curve_curve_int *
bs3_curve_int(
		bs3_curve &curv1,
		bs3_curve &curv2
	);

// Make two curves mergeable.
// Reparameterizes the curves if they are rational,
// and doesn't match tangent magnitudes for C1 continuity.
// See also bs3_curve_connect().

/**
* Sets two curves so they can be merged.
* <br><br>
* <b>Role:</b> This function reparameterizes the curves if they are rational,
* but doesn't match tangent magnitudes for C1 continuity. Refer to
* <tt>bs3_curve_connect</tt> for additional information.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param b1
* first given %curve.
* @param b2
* second given %curve.
**/

DECL_SPLINE void
bs3_curve_mergeable(
		bs3_curve b1,
		bs3_curve b2
   );

//
// Make two curves compatible. For internal use only.
//
// This is not for making curves compatible to be connected end-to-end,
// but to make them compatible side-by-side, so that, for example,
// they could be used in the same surface, in the same direction.
//
// The resulting curve have the same degree, rationality, and knot SPAvector.
//

/**
* Sets two curves to be side-by-side compatible.
* <br><br>
* <b>Role:</b> This is not for making curves compatible to be connected
* end-to-end, but to make them compatible side-by-side, so that, for example, they
* could be used in the same %surface, in the same direction. The resulting curves
* have the same degree, rationality, and knot vector.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param b1
* first given %curve.
* @param b2
* second given %curve.
**/

DECL_SPLINE void
bs3_curve_compat(
		bs3_curve b1,
		bs3_curve b2
   );


// For C2/G2/C1/G1 continuity checking.
// A linked list is returned indicating knot values where the curve is discontinuous
// in tangent direction or magnitude.

/**
* Gets knot values where the %curve is discontinuous in tangent direction or magnitude.
* <br><br>
* <b>Role:</b> Returns a linked list that indicates knot values where the %curve
* is discontinuous in tangent direction or magnitude. For C1/G1 continuity
* checking.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given B-spline %curve.
* @param cirr
* returned list of C1/G1 discontinuities.
* @param no_pts
* returned number of points in list.
* @param continuity
* returned continuity, 1 = tangent (default), 2 = acceleration.
**/

DECL_SPLINE void
bs3_curve_check_smoothness(
		bs3_curve bs, 					// Input bspline
		curve_irregularities *&cirr,		// List indicating C2/G2/C1/G1 discontinuities
		int& no_pts,						// number of entries in list
		int continuity = 1                     // Continuity 1 = tangent (default) 2 = acceleration
	);

// CUSTOMER MODIFICATION BEGIN: CHANGE 156

// DO NOT DOCUMENT this customer-specific function.


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs3_curve_find_bad_disc(
    bs3_curve crv,      // Input: the given curve
    double params[],    // Input: array of discontinuity SPAparameter values
    int num_values,     // Input: number of discontinuity SPAparameter values
    double knot_tol     // Input: knot tolerance
    );

// CUSTOMER MODIFICATION END

// For a cubic fit to a bspline

/**
* Creates a nonrational cubic B-spline fit to the given conic.
* <br><br>
* <b>Role:</b> The routine does not check to determine whether the given input
* B-spline is other than conic. If this routine is tried for any general rational,
* the routine works but the B-spline will be inaccurate; i.e., it is not guaranteed
* to be within the fit tolerance given as input. If an error occurs, this routine
* returns a <tt>NULL</tt> %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param input
* given %curve.
* @param fit_tol
* fit tolerance.
**/

DECL_SPLINE bs3_curve
bs3_curve_cub_fit_to_conic(
		bs3_curve input ,		// Input bspline
		double fit_tol 			// fit tolerance
	);

// Routine to construct a rail curve for sweeping

/**
* Gets a rail %curve to sweep for %surface construction.
* <br><br>
* <b>Role:</b> To establish the twist angle, this routine uses the angle of
* rotation (in radians). The routine can also be used to get the lateral curves
* for sweep.
* <br><br>
* As per the definition of the sweep, the <tt>up_dir</tt> is propagated along the center
* %curve. <tt>up_dir</tt> is the normal to the %plane formed by the center a nonplanar
* B-spline. Any normal is acceptable.
* <br><br>
* The rail is constructed so that it forms a Frenet frame as it goes along the
* path %curve to make up a sweep %surface.
* <br><br>
* For this routine, the center %curve must be a cubic B-spline.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param input
* given %curve.
* @param up_dir
* up direction, normal to the B-spline at start.
* @param twist_ang
* twist angle.
**/

DECL_SPLINE bs3_curve
bs3_curve_rail_from_cen(
		bs3_curve               input,		// Input cubic bspline
		const SPAunit_vector&   up_dir,	// up_direction ( normal to the bspline at start )
		double                  twist_ang			// Twist angle
	);

// Return the degree of a bs3_curve.

/**
* Gets the degree of a <tt>bs3_curve</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* bs3_curve.
**/

DECL_SPLINE int
bs3_curve_degree(
		bs3_curve bs
	);

// Return the number of control points in a bs3_curve.

/**
* Gets the number of control points in a <tt>bs3_curve</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* bs3_curve.
**/

DECL_SPLINE int
bs3_curve_num_ctlpts(
					 bs3_curve bs,
					 const int   use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
					 );

// Routine returns if the current curve is rational or not

/**
* Determines whether a B-spline %curve is rational.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if it is rational; otherwise returns
* <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param input
* given %curve.
**/

DECL_SPLINE logical
bs3_curve_rational(
		bs3_curve input		// Input curve
	);

// Determine whether a bs3_curve is a conic section, and what type.

/**
* Determines whether the given %curve is a conic section.
* <br><br>
* <b>Role:</b> This API returns a value of the enumerated type <tt>bs_conic_type</tt>,
* that indicates what type of conic the %curve is (ellipse, circle, parabola,
* hyperbola, or a line or polyline), or that it is not a conic section.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3c
* given %curve.
**/

DECL_SPLINE bs_conic_type
bs3_curve_conic_type(
		bs3_curve bs3c		// Input curve
	);

// Routine returns TRUE, if the current curve has multiple end knots

/**
* Determines if the current %curve has multiple end knots.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> is the %curve has fully multiple end
* knots at the beginning and end of the %curve; returns <tt>FALSE</tt> otherwise.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param input
* given %curve.
**/

DECL_SPLINE logical
bs3_curve_mult(
		bs3_curve input		// Input curve
	);


// This routine adds multiple end knots to a B-spline curve.  This routine
// is always called for periodic curves.  Such that, the bspline control
// polygon always interpolates the start and end of the curve.

/**
* Adds multiple end knots to a B-spline %curve.
* <br><br>
* <b>Role:</b> This routine is always called for periodic curves such that the
* B-spline control polygon always interpolates the start and end of the %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param input
* given %curve.
**/

DECL_SPLINE void
bs3_curve_add_mult_ekn(
		bs3_curve input
	);

// Add a knot to a bspline at a given SPAparameter value. The routine returns
// the number of knots added.  If the knot value to be added is an existing
// knot, the knot is inserted, provided the multiplicity of the current knot
// does not exceed the degree of the B-spline curve.  The equality of the knots
// are tested using the tolerance given as input.  The knot value has to be
// within the SPAparameter range of the input bspline

/**
* Adds a knot to a B-spline at a given parameter value.
* <br><br>
* <b>Role:</b> The routine returns the number of knots added. If the knot value
* to be added is an existing knot, the knot is inserted, provided the multiplicity
* of the current knot does not exceed the degree of the B-spline %curve. The equality
* of the knots are tested using the tolerance given as input. The knot value must
* be within the parameter range of the input B-spline.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param input
* given %curve, modified in place.
* @param new_par
* returned new knot value.
* @param mult_req
* returned new knot's multiplicity.
* @param knot_tol
* returned knot tolerance.
**/

DECL_SPLINE int
bs3_curve_add_knot(
		bs3_curve input,	// Input spline, modified inplace
		double new_par,		// new knot value returned
		int mult_req,		// multiplicity of the new knot added
		double knot_tol		// knot tolerance
	);

// Determine the multiplicity of a knot of a bspline curve.

/**
* Determines the multiplicity of a B-spline %curve knot.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param input
* given %curve.
* @param knot_value
* knot at which multiplicity is to be determined.
* @param knot_tol
* knot tolerance.
**/

DECL_SPLINE int
bs3_curve_knot_mult(
		bs3_curve input,	// Input B-spline
		double knot_value,		// knot at which the multiplicity is to be determined
		double knot_tol,		// knot tolerance
		const int   use_initial_seam_multiplicity = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
	);

// Remove redundant knots from a bs3_curve.
// This only removes knots that are truly redundant, i.e. their removal
// will not affect the shape or parameterization of the curve at all.

/**
* Removes redundant knots from a <tt>bs3_curve</tt>.
* <br><br>
* <b>Role:</b> This only removes knots that are truly redundant, i.e. when
* their removal will not affect the shape or parameterization of the %curve by more
* than the given tolerance.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs3
* given %curve.
* @param tol
* tolerance.
**/

DECL_SPLINE void
bs3_curve_rem_extra_knots(
		bs3_curve bs3,
		double tol		// tolerance: rough measure of how much
	);				//  the curve might change.

// Raise the degree of a bspline curve in place by one.

/**
* Raises the degree of a B-spline %curve in place by one.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param input
* given %curve, modified in place.
**/

DECL_SPLINE void
bs3_curve_degree_elevate(
		bs3_curve &input	// Input B-spline modified inplace
	);

// STI jmb begin: Add Spline Interface functions to support AAA

// Determine radius and center of an arc tangent to three curves.
// Return an error code which is 0 if no error.

/**
* Gets the radius and center of an arc tangent to three curves.
* <br><br>
* <b>Role:</b> All three B-splines must lie in the same %plane. There are
* potentially many circles lying tangent to the three curves. <tt>t1</tt>,
* <tt>t2</tt>, and <tt>t3</tt> provide starting guesses for the parameter
* value at the point of tangency. Upon successful completion, each contains the
* actual parameter value at the point of tangency. This function returns an
* error code, which is 0 if there is no error.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param crv1
* first %curve.
* @param t1
* parameter of tangency on first %curve.
* @param crv2
* second %curve.
* @param t2
* parameter of tangency on second %curve.
* @param crv3
* third %curve.
* @param t3
* parameter of tangency on third %curve.
* @param normal
* normal to %plane of all curves.
* @param radius
* returned arc radius.
* @param center
* returned arc center.
**/

DECL_SPLINE int
bs3_curve_arc_3curve(
	bs3_curve crv1,              // first curve
	double&   t1,                // Output SPAparameter of tangency or first curve
	bs3_curve crv2,              // second curve
	double&   t2,                // Output SPAparameter of tangency or second curve
	bs3_curve crv3,              // third curve
	double&   t3,                // Output SPAparameter of tangency or third curve
	const SPAunit_vector& normal,   // Normal to plane of all curves
	double&   radius,            // Output radius
	SPAposition& center             // Output center
);

// CUSTOMER MODIFICATION BEGIN: CHANGE 156

// Determine radius and center of an arc tangent to three curves.
// A curve may degenerate to a single point.  If a degenerate curve is desired,
// the curve is NULL and the corresponding SPAposition is non-NULL.
// The array of tangency parameters provides a starting points for iterations
// as well as a means of returning the actual parameters of the tangency points
// The function returns an error code which is 0 if no error.

/**
* Gets the radius and center of an arc tangent to three curves.
* <br><br>
* <b>Role:</b> Gets the radius and center of an arc tangent to three curves.
* One or two of the curves may degenerate to a point. If a %curve degenerates to a
* point, this is signified by a <tt>NULL</tt> %curve pointer and a non-<tt>NULL</tt>
* position entry. It returns an error code which is 0 if no error.
* <br><br>
* All three B-splines must lie in the same %plane. There are potentially many
* circles lying tangent to the three curves. <tt>t1</tt>, <tt>t2</tt>, and
* <tt>t3</tt> provide starting guesses for the parameter value at the point
* of tangency. Upon successful completion, each contains the actual parameter
* value at the point of tangency. This function returns an error code, which
* is 0 if there is no error.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crvs
* array of bs3 curves.
* @param pos
* array of positions.
* @param ts
* array of tangency parameters.
* @param normal
* normal to %plane of all curves.
* @param radius
* radius.
* @param center
* center.
**/

DECL_SPLINE int
bs3_curve_arc_3curve_modified(
    bs3_curve crvs[3],           // Input:  Array of bs3_curves
    SPAposition  pos[3],            // Input:  Array of positions
    double    ts[3],             // Input/Output: Array of tangency parameters
    const SPAunit_vector& normal,   // Input:  Normal to plane of all curves
    double&   radius,            // Output: Radius
    SPAposition& center             // Output: Center
);

// CUSTOMER MODIFICATION END

// Construct a fillet curve between two curves.


/**
* Creates a fillet %curve between two curves.
* <br><br>
* <b>Role:</b> If an error occurs, this routine returns a <tt>NULL</tt> %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crv1
* first given %curve.
* @param radius1
* first given radius.
* @param t1
* guess for start parameter on first %curve.
* @param crv2
* second given %curve.
* @param radius2
* second given radius.
* @param t2
* guess for start parameter on second %curve.
* @param normal
* normal to %plane of both curves.
**/

DECL_SPLINE bs3_curve
bs3_curve_fillet_curves(
	bs3_curve   crv1,            // first curve
	double      radius1,         // first radius
	double&     t1,              // starting guess for SPAparameter on first curve
	bs3_curve   crv2,            // second curve
	double      radius2,         // second radius
	double&     t2,              // starting guess for SPAparameter on second curve
	const SPAunit_vector& normal    // Normal to plane of both curves
);

// Compute the positions for creating a line tangent to two bs3_curves
// Returns TRUE if the points were computed, or FALSE if no solution was found

/**
* Determines the positions for creating a line tangent to two <tt>bs3_curves</tt>.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the points were computed, or <tt>FALSE</tt> if no
* solution was found.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param crv1
* first given %curve.
* @param t1
* first %curve parameter guess.
* @param crv2
* second given %curve.
* @param t2
* second %curve parameter guess.
* @param normal
* returned normal to %plane of %curve.
* @param pt1
* returned first end of tangent line.
* @param pt2
* returned second end of tangent line.
**/

DECL_SPLINE logical
bs3_curve_line_tan_2crv(
    bs3_curve crv1,            // first curve
    double t1,                 // starting guess for SPAparameter on first curve
    bs3_curve crv2,            // second curve
    double t2,                 // starting guess for SPAparameter on second curve
    const SPAunit_vector& normal, // normal to plane of curve
    SPAposition& pt1,             // first end of tangent line
    SPAposition& pt2              // second end of tangent line
    );

// Compute all positions on a bs3_curve where a line from a given SPAposition
// is tangent to the curve
// Returns TRUE if the points were computed, or FALSE if no solution was found

/**
* Determines all positions on a <tt>bs3_curve</tt> where a line from a given position is tangent to the %curve.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the points were computed; <tt>FALSE</tt> if no solution
* was found.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param point
* start point.
* @param crv
* given %curve.
* @param normal
* %curve normal %curve normal.
* @param nmax
* maximum number of points to find.
* @param num_pts
* returned number of tangency points found.
* @param tanpts
* returned tangency points.
**/

DECL_SPLINE logical
bs3_curve_line_tan_pt_crv(
    const SPAposition& point,     // the point to start from
    bs3_curve crv,             // the curve
    const SPAunit_vector& normal, // curve normal
    int nmax,                  // maximum number of points to find
    int& num_pts,              // the number of tangency points found
    SPAposition tanpts[]          // tangency points
    );

// STI jmb end: Add Spline Interface functions to support AAA

// CUSTOMER MODIFICATION BEGIN: CHANGE 156

// Compute a SPAposition on a bs3_curve where a line from a given SPAposition
// is tangent to the curve.
// Returns TRUE if the point was computed, or FALSE if no solution was found.

/**
* Determines a position on a <tt>bs3_curve</tt> where a line from a given position is tangent to the %curve.
* <br><br>
* <b>Role:</b> Determines a position on a <tt>bs3_curve</tt> where a line from a given
* position is tangent to the %curve. Uses a guess parameter value to aid the
* solution (especially where there are multiple solutions).
* <br><br>
* Returns <tt>TRUE</tt> if the point was computed; <tt>FALSE</tt> if no solution
* was found. <tt>FALSE</tt> is preferred as the solution method (do not use the
* old method). If <tt>old_method</tt> is <tt>TRUE</tt>, then the x- and y-axis values are needed.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param point
* input start point.
* @param crv
* input %curve.
* @param normal
* %curve normal.
* @param t
* %curve parameter.
* @param old_method
* solution method.
* @param xa
* x-axis.
* @param ya
* y-axis.
* @param guess_pt
* input guess point.
**/

DECL_SPLINE logical
bs3_curve_tan_pt_crv(
    const SPAposition& point,                   // Input: the given SPAposition
    bs3_curve crv,                           // Input: the curve
    const SPAunit_vector& normal,               // Input: the curve normal
    double *t,                               // Input/Output: curve SPAparameter
	logical old_method = FALSE,               // Input: solution method
								              //     (FALSE is preferred)
	const SPAvector& xa = (*(SPAvector*)NULL_REF),  // Input: xaxis
											  // (only needed with old method)
	const SPAvector& ya = (*(SPAvector*)NULL_REF),  // Input: yaxis
											  // (only needed with old method)
	const SPAposition& guess_pt = (*(SPAposition*)NULL_REF)  // Input: guess point
							                  // (only needed with old method)
    );

// CUSTOMER MODIFICATION END

// Routine which takes points and tangents and interpolates a cubic
// bspline curve( Hermite interpolation ).
/**
* Interpolates a cubic B-spline %curve (Hermite interpolation) from points and tangents.
* <br><br>
* <b>Role:</b> Each array is of size <tt>no_pts</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param no_pts
* number of points to interpolate.
* @param pts
* array of points.
* @param tans
* array of tangents.
* @param knots
* array of knots.
**/
DECL_SPLINE bs3_curve
bs3_curve_hermite_interp(
		int no_pts,			// Number of points to be interpolated
		SPAposition *pts,		// points (SPAposition[no_pts] )
		SPAvector *tans,		// tangents (SPAvector[no_pts] )
		double *knots		// knots (double[no_pts] )
	);

// Routine which modifies the curve, such that the curve
// passes through the given point at given parmeter values and
// has given tangent directions.  The given curve is modified in
// place.  Return TRUE if the operation is successfull.

/**
* Modifies the %curve, such that the %curve passes through the given point at the 
* given parameter value and has the given tangent direction.
* <br><br>
* <b>Role:</b> The given %curve is modified in place. Returns <tt>TRUE</tt> if the
* operation is successful.
* <br><br>
* It may assume that the ends to be joined match in position and direction, and
* that the parameterization is continuous. ACIS uses this routine only to rejoin
* parts of a periodic %curve that have been split by <tt>bs3_curve_split</tt>, in the
* opposite order, so that the resultant %curve starts and ends at the split point.
* A %curve made in this way may be marked as closed by <tt>bs3_curve_join</tt>, but should
* not be marked periodic.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs_cur
* given %curve.
* @param pos
* position to be interpolated.
* @param dir
* direction to be interpolated.
* @param t
* parameter value to be interpolated.
**/

DECL_SPLINE logical
bs3_curve_modify(
		bs3_curve bs_cur,			// Given curve to be modified
		SPAposition const &pos,	// Position to be interpolated
		SPAunit_vector const &dir,// Direction to be interpolated
		double t				// SPAparameter value to be interpolated.
	);

// Set the form

/**
* Sets the form of a <tt>bs3_curve</tt>.
* <br><br>
* <b>Role:</b> The form of the %curve will be open (<tt>bs3_curve_open_ends</tt>), closed
* (<tt>bs3_curve_closed_ends</tt>), periodic (<tt>bs3_curve_periodic_ends</tt>), or unknown
* (<tt>bs3_curve_unknown_ends</tt>).
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs3
* given %curve.
**/

DECL_SPLINE void
bs3_curve_set_form( bs3_curve bs3 );

/**
* Determines if the %curve has zero first derivative anywhere.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the %curve has coincident control points which 
* might give rise to zero first derivative; <tt>FALSE</tt> otherwise.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs3
* input %curve.
* @param indx
* output coincident control point index.
* @param approx_param
* output approximate zero derivative parameter.
**/
DECL_SPLINE logical
bs3_curve_coincident_verts(
	bs3_curve bs3,
	int* indx = NULL,
	double* approx_param = NULL
);

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE int
bs3_curve_size(bs3_curve_def* crv);

// Added to facilitate integration of Intergraph code in stop_modeller()
// Cleans up memory before exiting modeller


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void
final_cleanup();

// CUSTOMER MODIFICATION BEGIN: CHANGE 156
// DO NOT DOCUMENT these customer-specific functions.


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs3_curve_tan_from_4pt(
	const SPAposition& P0,		// Input: The first point
	const SPAposition& P1,		// Input: The second point
	const SPAposition& P2,		// Input: The third point
	const SPAposition& P3,		// Input: The fourth point
	logical which_end,		// Input: FALSE == start, TRUE == end
	SPAvector& dir				// Output: Tangent at given end point
							// assuming cubic interpolation of 4 pts
	);


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE logical
bs3_curve_tan_from_3pt(
	const SPAposition& P0,		// Input: The first point
	const SPAposition& P1,		// Input: The second point
	const SPAposition& P2,		// Input: The third point
	int which_pt,			// Input: Point to use
	SPAvector& dir				// Output: Tangent at given point
							// assuming circular interpolation of 3 pts
	);

//================================================================================
//  recover seam multiplicity within a tolerance and then store
//  it off in the  bs3_curve structure (useful if the curve was
//  originally created without seam multiplicity information)
//================================================================================
// temporary fix to allow recovery of seam multiplicity for curves not
// using the accepted periodic curve format.
/**
 * @nodoc
 */
DECL_SPLINE int
bs3_curve_determine_and_save_initial_seam_multiplicity(bs3_curve bs, 
                                                       double msTol);


// CUSTOMER MODIFICATION END

/** @} */
#endif
