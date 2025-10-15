/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Routines for processing 2D B-spline curves, used to fit true curves
// in the parameter space of a surface.
#if !defined( SG_BS2_CURVE_ROUTINES )
#define SG_BS2_CURVE_ROUTINES
#include "dcl_spl.h"
#include "logical.h"
#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "bs3surf.hxx"
#include "debugmsc.hxx"
#include "sp2crtn.hxx"
/**
* @file sps2crtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS2CURVEAPI
 *
 * @{
 */
class SPAposition;
class intcurve_data;
class SPApar_pos;
#if defined (ultrix) || defined( osf1 ) || defined( vms )
#include "position.hxx"
#endif
/**
 * Fits a <tt>bs2_curve</tt> to a collection of point and direction data.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param int_pts
 * defining data.
 * @param pea_bs
 * returned %curve that fits data.
 **/
DECL_SPLINE void bs2_curve_fit(
	intcurve_data const &int_pts,	// defining data
	bs2_curve &pea_bs				// curve fit to data
	);
/**
 * Creates a <tt>bs2_curve</tt> that approximates a segment of a <tt>bs3_curve</tt> lying on a %surface.
 * <br><br>
 * <b>Role:</b> Given a <tt>bs3_surface</tt> and a <tt>bs3_curve</tt> known to lie on the %surface
 * (within some tolerance), calculates a <tt>bs2_curve</tt> that will approximate the %curve
 * between a start parameter and an end parameter to within a fit tolerance.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param ebs
 * object space %curve.
 * @param sparam
 * start parameter on %curve.
 * @param eparam
 * end parameter on %curve.
 * @param srf
 * %surface %curve lies on.
 * @param fit
 * fit tolerance.
 **/
DECL_SPLINE bs2_curve bs2_curve_for_curve_on_surf(
	bs3_curve ebs   ,			// object space curve
	double sparam      ,			// start parameter on curve
	double eparam      ,			// end parameter on curve
	bs3_surface srf ,			// surface curve lies on
	double fit         			// fit tolerance
	);
/**
 * Marks a <tt>bs2_curve</tt> as being periodic.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * given %curve.
 **/
DECL_SPLINE void bs2_curve_set_periodic(
		bs2_curve crv
	);

// Mark a bs2_curve as being closed

/**
 * Marks a <tt>bs2_curve</tt> as being closed.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * given %curve.
 **/
DECL_SPLINE void bs2_curve_set_closed(
		bs2_curve crv
	);

// Mark a bs2_curve as being open

/**
 * Sets a %curve's form to be open.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param crv
 * given %curve.
 **/
DECL_SPLINE void bs2_curve_set_open(
		bs2_curve crv
	);

// New routine added for jyoti's skin code.
// Strictly a hack around, recommended that this routine may not be used
// anywhere.

/**
 * Resolves the AG references in the skin code.
 * <br><br>
 * <b>Role:</b> Not recommended for use. This routine simply transfers the
 * B-spline data from the <tt>bs3_curve</tt> structure to the <tt>bs2_structure</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param in_cur
 * given %curve.
 **/
DECL_SPLINE bs2_curve bs3_curve_to_bs2_curve(
		bs3_curve in_cur		// Input curve
	);

// New routine added for jyoti's skin code.
// Strictly a hack around, recommended that this routine may not be used
// anywhere.

/**
 * Resolves the AG references in the skin code.
 * <br><br>
 * <b>Role:</b> Not recommended for general use. This routine transfers the
 * <tt>ag_spline</tt> data from the <tt>bs2_curve</tt> structure to the <tt>bs3_structure</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param in_cur
 * given %curve.
 **/
DECL_SPLINE bs3_curve bs2_curve_to_bs3_curve(
		bs2_curve in_cur		// Input curve
	);


// Return the number of control points in a bs2_curve.


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int bs2_curve_num_ctlpts(
                     bs2_curve	bs,
                     const int use_initial_seam_multiplicity = FALSE// in: used for periodic geometry when initial knot multiplicty != order
		 );

// Create a 2D B-spline curve specified as a sequence of control
// points, weights  and associated knot SPAvector. The control point
// tolerance and knot tolerance are used to determine when points
// or knots are the same.
// As the dimension is 2, only the x and y components of the input
// positions are considered.

/**
 * Creates a two-dimensional B-spline %curve specified as a sequence of control points, weights, and an associated knot vector.
 * <br><br>
 * <b>Role:</b> The control point tolerance and knot tolerance are used to
 * determine when points or knots are the same. Only the <i>x</i> and <i>y</i>-components of the
 * input positions are considered.
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
 * @param ctrlpts
 * control points, x and y components only are used.
 * @param weights
 * weights.
 * @param cpt_tol
 * control point tolerance.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 * @param knot_tol
 * knot tolerance.
 **/
DECL_SPLINE bs2_curve bs2_curve_from_ctrlpts(
	int degree,					// degree
	logical rational,				// rational
	logical closed,				// closed
	logical periodic,				// periodic
	int num_ctrlpts,					// number of control points
// STIPORT TAC macintosh changed from const SPAposition []
	const SPAposition *ctrlpts,		// control points (only x and y components used)
	const double weights[],			// weights
	double cpt_tol,					// control point tolerance
	int num_knots,					// number of knots
	const double knots[],			// knots
	double knot_tol			 		// knot tolerance
);

// Return the dimension, degree, control points, weights, and knots
// for a 2D B-spline curve. Also indicate if the curve is rational or
// not.
// Use only the x and y components of the positions returned as the control
// points

/**
 * Gets the dimension, degree, control points, weights, and knots for a 2D B-spline %curve, and determines if the %curve is rational.
 * <br><br>
 * <b>Role:</b> This function creates arrays of control points, weights, and
 * knot points. It is up to the application to delete these arrays.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param bs
 * given %curve.
 * @param dim
 * returned dimension.
 * @param deg
 * returned degree.
 * @param rat
 * returned rational value.
 * @param num_ctrlpts
 * returned number of control points.
 * @param ctrlpts
 * returned control points, uses x and y.
 * @param weights
 * returned weights.
 * @param num_knots
 * returned number of knots.
 * @param knots
 * returned knots.
 **/
DECL_SPLINE void bs2_curve_to_array(
	bs2_curve bs,				// curve						Input
	int &dim,					// dimension					Returned
	int &deg,					// degree						Returned
	logical &rat,				// rational						returned
	int &num_ctrlpts,					// number of control points		Returned
	SPAposition *&ctrlpts,				// control points,use x and y	Returned
	double *&weights,				// weights						Returned
	int &num_knots,					// number of knots				Returned
	double *&knots,				// knots						Returned
    const int use_initial_seam_multiplicity = FALSE // in: used for periodic geometry when initial knot multiplicty != order
);

// Return the number of knots and the knot SPAvector
// for a 2D B-spline curve.

/**
 * Gets the number of knots and the knot vector for a 2D B-spline %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param bs
 * input %curve.
 * @param num_knots
 * number of knots.
 * @param knots
 * knot vector.
 **/
DECL_SPLINE void bs2_curve_knots(
	bs2_curve bs,				// curve						Input
	int& num_knots,					// number of knots				Returned
	double*& knots,				// knot SPAvector					Returned
    const int   use_initial_seam_multiplicity = FALSE// input: used to recover initial input data
);

// Return the number of control points and an array of control points
// for a 2D B-spline curve.

/**
 * Gets the number of control points and an array of control points for a 2D B-spline %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param bs
 * input %curve.
 * @param num_ctrlpts
 * number of control points output.
 * @param ctrlpts
 * output control point array.
 **/
DECL_SPLINE void bs2_curve_control_points(
	bs2_curve bs,				// curve						Input
	int &num_ctrlpts,					// number of control points		Returned
	SPApar_pos *&ctrlpts,				// control point array			Returned
    const int   use_initial_seam_multiplicity = FALSE// input: used to recover initial input data
);

//    Return the number of weights and an array of weights for a
//    rational, 2D B-spline curve.

/**
 * Gets the number of weights and an array of weights values for a rational, 2D B-spline %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param bs
 * input %curve.
 * @param num_weights
 * number of weights.
 * @param weights
 * array of weights.
 **/
DECL_SPLINE void bs2_curve_weights(
	bs2_curve bs,				// curve                        Input
	int &num_weights,					// number of weights            Returned
	double *&weights,                // weights array                Returned
    const int   use_initial_seam_multiplicity = FALSE// input: used to recover initial input data
);

// Return the degree of a bs2_curve.


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int bs2_curve_degree(
		 bs2_curve	bs
		 );

// STIINGR dgh begin

// Read B-spline curve


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE bs2_curve bs2_curve_read(FILE *infile, logical read_form);

// Write B-spline curve


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void bs2_curve_write(FILE *outfile, const bs2_curve cv2, logical write_form,
                     logical nor_kts);

// Set the form

/**
 * Sets a <tt>bs2_curve</tt> form variable to the same value as a corresponding <tt>bs3_curve</tt> form variable.
 * <br><br>
 * <b>Role:</b> This function receives a form variable for the <tt>bs3_curve</tt> that
 * the <tt>bs2_curve</tt> corresponds to and sets the <tt>bs2_curve</tt> form variable to the
 * <tt>bs3_curve</tt> form variable.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param cv2_form
 * form variable for bs3_curve that corresponds to given bs2_curve.
 * @param cv2
 * bs2_curve for which the form variable is to be set.
 **/
DECL_SPLINE void bs2_curve_set_form( bs3_curve_form cv2_form, bs2_curve cv2 );

// Add a knot to a B-spline at a given SPAparameter value. The routine returns
// the number of knots added. If the knot value to be added is an existing
// knot, the knot is inserted, provided the multiplicity of the current knot
// does not exceed the degree of the spline curve. The equality of the knots
// are tested using the tolerance given as input. The knot value has to be
// within the SPAparameter range of the input B-spline.If the SPApar_pos for the
// new knot is supplied, the new pcurve is made to have its Bezier form and
// then the new knot is made to lie at this point. If the 2d curve derivative
// is supplied, the pcurve is forced to agree with this at the new knot.

/**
 * Adds a knot to a B-spline at a given parameter value.
 * <br><br>
 * <b>Role:</b> The routine returns the number of knots added. If the knot value to
 * be added is an existing knot, the knot is inserted, provided the multiplicity of
 * the current knot does not exceed the degree of the B-spline %curve. The equality of
 * the knots are tested using the tolerance given as input. The knot value must be
 * within the parameter range of the input B-spline. If the <tt>SPApar_pos</tt> for the
 * new knot is supplied, the new pcurve is made to have its Bezier form and then the new
 * knot is made to lie at this point. If the 2D %curve derivative is supplied, the pcurve
 * is forced to agree with this at the new knot. At present, this is only implemented
 * for curves of degree 3 (which is most curves).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param bs2
 * given %curve.
 * @param new_knot_param
 * returned new knot parameter value.
 * @param mult_req
 * returned new knot's multiplicity.
 * @param knot_tol
 * returned knot tolerance.
 * @param new_knot_uv
 * new knot %surface par_pos.
 * @param new_knot_deriv_below
 * 2-space %curve deriv below new knot.
 * @param new_knot_deriv_above
 * deriv above  new knot.
 **/
DECL_SPLINE int bs2_curve_add_knot(
						   bs2_curve bs2,		  // Input bs2_curve
						   double new_knot_param, // New knot SPAparameter value
						   int mult_req,		  // Multiplicity of new knot
						   double knot_tol,		  // Knot tolerance
						   const SPApar_pos &new_knot_uv =
						                          *( SPApar_pos * ) NULL_REF,
						                          // New knot surface SPApar_pos
						   const SPApar_vec &new_knot_deriv_below =
						                          *( SPApar_vec * ) NULL_REF,
						                          // Derivative below new knot
						   const SPApar_vec &new_knot_deriv_above =
						                          *( SPApar_vec * ) NULL_REF
						                          // Derivative above new knot
						   );


// Function to determine if a bs2_curve is in Bezier form or not.


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical bs2_curve_bezier(
							 bs2_curve bs2,		// Input bs2_curve
							 double knot_tol	// Knot tolerance
							 );

// Function to add knots to the bs2_curve so that it is in its
// Bezier form. Returns TRUE if it successfully converts the curve
// to its Bezier form, or FALSE otherwise.


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical bs2_curve_make_bezier(
								  bs2_curve bs2,	// Input bs2_curve
								  double knot_tol	// Knot tolerance
								  );

// Function to test if the supplied SPAparameter value is at one of the
// bs2_curve knots.


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE logical bs2_curve_at_knot(
						   bs2_curve bs2,		  // Input bs2_curve
						   double param,		  // SPAparameter value
						   double knot_tol		  // Knot tolerance
						   );

//
// Make two curves compatible. For internal use only.
//
// This is not for making curves compatible to be connected end-to-end,
// but to make them compatible side-by-side, so that, for example,
// they could be used in the same surface, in the same direction.
//
// The resulting curve has the same degree, rationality, and knot SPAvector.
//


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void bs2_curve_compat(
   							bs2_curve 	b1,		// Curve1
   							bs2_curve 	b2 		// Curve2
							);



// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE int bs2_curve_size(
							bs2_curve_def* crv
							);


// STIINGR dgh end


// For internal use only
/**
 * @nodoc
 */
DECL_SPLINE void bs2_curve_degree_elevate(
							bs2_curve	&input
						    );


/**
* Determines whether a bs2_curve is rational.
* <br><br>
* <b>Role:</b> This routine returns <tt>TRUE</tt> if the B-spline %curve is rational; otherwise returns
* <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param input
* given %curve.
**/

DECL_SPLINE logical bs2_curve_rational(
		bs2_curve input		// Input curve
	);

/** @} */
#endif
