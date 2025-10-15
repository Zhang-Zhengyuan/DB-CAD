/* ORIGINAL: 3dt2.1/geomhusk/bnd_crv.hxx */
// $Id: bnd_crv.hxx,v 1.10 2002/08/09 17:15:14 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------
// purpose---
//    define a bounded curve class
//
//----------------------------------------------------------------------

#ifndef bnd_crv_hxx
#define bnd_crv_hxx

//======================================================================

#include "dcl_kern.h"
#include "logical.h"
#include "position.hxx"
#include "unitvec.hxx"

#include "bs3curve.hxx"

/**
* @file bnd_crv.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISCURVES
 *
 * @{
 */

class curve;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAbox;
class EDGE;
class TCOEDGE;
class SPAtransf;
class plane;
class curve_extremum;
class curve_curve_int;

class bounded_line;
class bounded_arc;
class pick_ray;


//======================================================================
/**
 * Defines a bounded curve.
 * <br>
 * <b>Role:</b> This class defines bounded curves. A bounded curve is a curve with
 * start and end parameters that specify the bounds of the curve. This class makes
 * it easy to extract data from wireframe geometry. It supports most of the
 * functions, such as evaluation, curve length, etc., that are provided in the <tt>curve</tt> class.
 * @see curve
 */

class DECL_KERN bounded_curve : public ACIS_OBJECT
{
    friend class bounded_line;
    friend class bounded_arc;

  protected:
/**
 * The pointer to an ACIS curve.
 */
    curve* acis_curve;
/**
 * The start parameter of the ACIS curve.
 */
    double start_param;
/**
 * The end parameter of the ACIS curve.
 */
    double end_param;

  private:

    // Map a SPAparameter value into the principal range of the underlying
    // ACIS curve.
    double map_parameter_into_range(double) const;

  protected:

    // makes sure that we have a valid curve
/**
 * Ensures that the data in this <tt>bounded_curve</tt> is valid.
 * <br><br>
 * <b>Role:</b> This method helps to avoid checking for a valid curve pointer in
 * <tt>acis_curve</tt> or the zero parameter range. If logical argument is
 * <tt>TRUE</tt>, then this method causes an error to generate.
 * <br><br>
 * @param signal_error
 * flag to signal error or not.
 */
    void make_valid(logical signal_error = FALSE);

/**
 * Sets the ACIS curve for this <tt>bounded_curve</tt>.
 * <br><br>
 * @param crv
 * the new ACIS curve.
 */
    logical set_acis_curve(curve* crv);

  public:

    //----------------------------------------
/**
 * Returns the underlying curve for this <tt>bounded_curve</tt>.
 */
     curve* get_acis_curve() const {return acis_curve;}

    // get the type of the underlying acis curve
/**
 * Returns the type of underlying curve for this <tt>bounded_curve</tt>.
 */
     int acis_type() const;

    // get the name of the class
/**
 * Returns the string <tt>"bounded_curve"</tt>.
 */
     const char* type_name() const;

    // check for specific subclasses
/**
 * Checks for the <tt>bounded_point</tt> subclass.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if this <tt>bounded_curve</tt> is a <tt>bounded_point</tt>;
 * otherwise, it returns <tt>FALSE</tt>.
 */
     virtual logical is_point() const;

/**
 * Checks for the <tt>bounded_line</tt> subclass.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if this <tt>bounded_curve</tt> is a
 * <tt>bounded_line</tt>; otherwise, it returns <tt>FALSE</tt>.
 */
     virtual logical is_line() const;

 /**
  * Checks for the <tt>bounded_curve</tt> subclass.
  * <br><br>
  * <b>Role:</b> Returns <tt>TRUE</tt> if this <tt>bounded_curve</tt> is a
  * <tt>bounded_arc</tt>; otherwise, it returns <tt>FALSE</tt>.
  */
     virtual logical is_arc() const;

    // write debug output for a bounded_curve
/**
 * Writes the debug output for this <tt>bounded_curve</tt>.
 * <br><br>
 * @param lead
 * leader string.
 * @param fp
 * FILE pointer.
 */
   virtual void debug(
        const char* lead,
        FILE* fp = debug_file_ptr
        ) const;

    // copy the bounded curve
/**
 * Copies this <tt>bounded_curve</tt> and applies a transform, if given, to the copy.
 * <br><br>
 * @param transf
 * transformation to apply to the copy.
 */
   virtual bounded_curve* copy( const SPAtransf* transf = NULL) const;

//----------------------------------------------------------------------
// constructors

// create an empty curve object (for use in restore)
/**
 * Constructs a <tt>bounded_curve</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it.
 */
     bounded_curve();

// copy constructor
/**
 * Constructs a <tt>bounded_curve</tt> from another one (copy constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data from the
 * object supplied as an argument.
 * <br><br>
 * @param b_cur
 * bounded curve that is to be copied.
 */
     bounded_curve(const bounded_curve& b_cur);

// create a bounded_curve from an EDGE
/**
 * Constructs a <tt>bounded_curve</tt> from an <tt>EDGE</tt> and a transform.
 * <br><br>
 * @param edge
 * the EDGE used to construct the bounded curve.
 * @param transf
 * transformation to be applied to the EDGE's CURVE.
 */
    bounded_curve(EDGE* edge, const SPAtransf* transf);

// Create a bounded_curve given an ACIS curve and start and end parameters.
/**
 * Constructs a <tt>bounded_curve</tt> from an ACIS curve and start and end parameters.
 * <br><br>
 * @param cur
 * the curve used to construct the bounded curve.
 * @param start_param
 * start parameter of the constructed bounded curve.
 * @param end_param
 * end parameter of the constructed bounded curve.
 */
    bounded_curve(const curve* cur, double start_param, double end_param);

// Create a bounded_curve given an ACIS curve and start and end positions.
/**
 * Constructs a <tt>bounded_curve</tt> from an ACIS curve and start and end positions.
 * <br><br>
 * <b>Role:</b> The bounded curve created by this constructor does not own the curve,
 * and it must be deleted explicitly, if needed.
 * <br><br>
 * @param cur
 * the curve used to construct the bounded curve.
 * @param start_pos
 * start position of the constructed bounded curve.
 * @param end_pos
 * end position of the constructed bounded curve.
 */
     bounded_curve(const curve* cur, const SPAposition& start_pos, const SPAposition& end_pos);

//----------------------------------------------------------------------
// destructor
/**
 * C++ destructor, deleting a bounded_curve.
 */
     virtual ~bounded_curve();

//----------------------------------------------------------------------
// The following methods are taken from the ACIS curve class.

// NOTE: many of these methods were virtual for ACIS curves, but do not
// really need to be virtual here.  For now, I have left them virtual in
// case we want to someday derive different kinds of bounded_curve (like
// offset_curve).

// In ACIS, some of these methods would want a NULL reference which is
// not really legal C++.  I have changed those to accept pointers instead.

    // For each of the six enquiry routines below, there are two
    // optional logical arguments. The first, if TRUE, is a guarantee
    // from the calling code that the most recent call to any curve
    // or surface member function was in fact to one of these six
    // routines for the same curve as the current call. It allows
    // an implementation to cache useful intermediate results to speed
    // up repeated evaluations, but must be used with extreme care.

    // The second logical argument may be set TRUE if an approximate
    // return value is acceptable. Here "approximate" is not very
    // well-defined, but may be assumed to be sufficient for visual
    // inspection of the curve, for example. The function approx_error
    // below may be used to determine the maximum discrepancy in
    // object space between an approximate value and the true curve.
    // Again this is intended as a speed optimisation in non-critical
    // circumstances.

    // Evaluate a curve at a given SPAparameter value, giving SPAposition,
    // and first and second derivatives (all optionally).
    // The old eval method returned only a SPAposition.  It should be
    // replaced by the new method eval_position (see below).
/**
 * Evaluates this <tt>bounded_curve</tt> at a given parameter value, returning the position and the first and second derivatives.
 * <br><br>
 * <b>Role:</b> For this and the other five evaluation methods, there are two optional logical
 * arguments. The first, if <tt>TRUE</tt>, is a guarantee from the calling code that the most
 * recent call to any curve or surface member method was in fact to one of these six
 * methods for the same curve as the current call. This allows an implementation to
 * cache useful intermediate results to speed up repeated evaluations, but is an option
 * that should be used with extreme care. The second logical argument may be set
 * <tt>TRUE</tt> if an approximate return value is acceptable. Here, the approximation
 * may be assumed to be sufficient for visual inspection of the curve.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param pos
 * position on curve at the specified parameter.
 * @param first_deriv
 * first derivative at the specified parameter.
 * @param second_deriv
 * second derivative at the specified parameter.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
    virtual void eval(
                      double param,           // SPAparameter value
                      SPAposition* pos,        // point on curve
                      SPAvector* first_deriv = NULL,   // first derivative
                      SPAvector* second_deriv = NULL,   // second derivative
                      logical eval_repeated = FALSE,  // repeated evaluation of same curve
                      logical approx_ok = FALSE   // approximate results OK
                      ) const;

    // Find the closest point on a curve to a given pick location and
    // return the curve SPAparameter value.
/**
 * Finds the closest point on this <tt>bounded_curve</tt> to a given pick location and returns the curve parameter value.
 * <br><br>
 * @param ray
 * the pick location.
 */
    virtual double eval_t( const pick_ray& ray )const;


    // Find the point on a curve corresponding to a given SPAparameter
    // value.
/**
 * Finds the point on this <tt>bounded_curve</tt> corresponding to a given parameter value.
 * <br><br>
 * <b>Role:</b> See the description of the <tt>eval</tt> method regarding the
 * two logical arguments.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
    virtual SPAposition eval_position(
                                   double param,           // SPAparameter value
                                   logical eval_repeated = FALSE,  // repeated evaluation
                                   logical approx_ok = FALSE   // approximate results OK
                                   ) const;


    // Find the derivative (direction and magnitude) at the given
    // SPAparameter value on the curve. Defaults to using eval().
/**
 * Finds the derivative (direction and magnitude) at the given parameter value on this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> See the description of the <tt>eval</tt> method regarding the
 * two logical arguments.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
     virtual SPAvector eval_deriv(
                              double param,           // SPAparameter value
                              logical eval_repeated = FALSE,  // repeated evaluation
                              logical approx_ok = FALSE   // approximate results OK
                              ) const;


    // Find the tangent direction at the given SPAparameter value on
    // the curve.
/**
 * Finds the tangent direction at the given parameter value on this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> See the description of the <tt>eval</tt> method regarding the
 * two logical arguments.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
    SPAunit_vector eval_direction(
                               double param,           // SPAparameter value
                               logical eval_repeated = FALSE,  // repeated evaluation
                               logical approx_ok = FALSE   // approximate results OK
                               ) const;


    // Find the magnitude of the derivative at the given SPAparameter
    // value on the curve.
/**
 * Finds the magnitude of the derivative at the given parameter value on this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> See the description of the <tt>eval</tt> method regarding the
 * two logical arguments.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
    virtual double eval_deriv_len(
                                  double param,           // SPAparameter value
                                  logical eval_repeated = FALSE,  // repeated evaluation
                                  logical approx_ok = FALSE   // approximate results OK
                                  ) const;


    // Find the curvature at the given SPAparameter value on the curve.
/**
 * Finds the curvature at the given parameter value on this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> See the description of the <tt>eval</tt> method regarding the
 * two logical arguments.
 * <br><br>
 * @param param
 * parameter value at which to perform the evaluation.
 * @param eval_repeated
 * flag to signal a repeated evaluation.
 * @param approx_ok
 * flag to signal that approximate results are OK.
 */
    virtual SPAvector eval_curvature(
                                  double param,           // SPAparameter value
                                  logical eval_repeated = FALSE,  // repeated evaluation
                                  logical approx_ok = FALSE   // approximate results OK
                                  ) const;


    // Return a distance value, which represents the greatest
    // discrepancy between positions calculated by calls to eval() or
    // eval_position() with the "approx_OK" logical set by turns to
    // TRUE and FALSE. Defaults to returning 0, for curves which
    // do not distinguish between these cases.
/**
 * Returns the distance error associated with approximate evaluations.
 * <br><br>
 * <b>Role:</b> Returns a distance value that represents the greatest discrepancy between
 * positions calculated by calls to <tt>eval</tt> or <tt>eval_position</tt> with the
 * <tt>approx_OK</tt> logical set by turns to <tt>TRUE</tt> and <tt>FALSE</tt>.
 * Returns 0 as the default for curves that do not distinguish between these cases.
 */
    virtual double approx_error() const;

    // For ACIS curves, these methods take an object of class SPAparameter
    // as input for a guess.  This is kind of inconsistant since the
    // evaluators take doubles, and the param mathod even returns a
    // double.  To make things more consistent (although less consistant
    // with ACIS curve), these routines all take doubles for curve parameters.

    // Find the SPAparameter value of a point on a curve, corresponding
    // to the given point. Only guaranteed to be valid for points on
    // the curve, though particular curve types may give useful
    // curve-dependent results for other points.
/**
 * Finds the parameter value of the point on this <tt>bounded_curve</tt>, corresponding to the given point.
 * <br><br>
 * <b>Role:</b> The results of this method are only guaranteed to be valid for points
 * on the curve, though particular curve types may give useful curve-dependent results
 * for other points.
 * <br><br>
 * @param pos
 * point on the curve.
 * @param guess_param
 * approximate parameter value.
 */
    virtual double param(
                const SPAposition& pos ,
                const double* guess_param = NULL
            ) const;


    // Find the foot of the perpendicular from the given point to
    // the curve, and tangent to the curve at that point, and its
    // SPAparameter value. If an input SPAparameter value is supplied (as
    // argument 4), the perpendicular found is the one nearest to
    // the supplied SPAparameter SPAposition, otherwise it is the one
    // at which the curve is nearest to the given point. Any of the
    // return value arguments may be a NULL reference, in which case
    // it is simply ignored.
/**
 * Finds the foot of the perpendicular from the given point to this <tt>bounded_curve</tt>, the tangent to the curve at that point, and the corresponding parameter value.
 * <br><br>
 * <b>Role:</b> If an input parameter value is supplied (as the <tt>param_guess</tt>
 * argument), the perpendicular found is the one nearest to the corresponding position;
 * otherwise, it is the one at which the curve is nearest to the given point. Any of the
 * return value arguments may be a <tt>NULL</tt> reference, in which case it is ignored.
 * <br><br>
 * @param point
 * the input position.
 * @param foot
 * the position on the curve.
 * @param normal
 * the returned normal.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * the actual parameter.
 * @param f_weak
 * weak flag.
 */
    virtual void point_perp(
                const SPAposition& point,
                SPAposition* foot,
                SPAunit_vector* normal,
                const double* param_guess = NULL,
                double* param_actual = NULL,
				logical f_weak = FALSE
            ) const;


    // Transform a curve.
    // (this could replace move( SPAtransf* ))
/**
 * Transforms this <tt>bounded_curve</tt>.
 * <br><br>
 * @param transf
 * the transformation to apply.
 */
    virtual bounded_curve& operator*=(const SPAtransf& transf );

    // reverse direction of the curve
/**
 * Reverses the direction of this <tt>bounded_curve</tt>.
 */
     virtual bounded_curve& negate();

    // Arc-length. Return the algebraic distance along the curve
    // between the given parameters, the sign being positive if the
    // SPAparameter values are given in increasing order, and negative
    // if they are in decreasing order. The result is undefined if
    // either SPAparameter value is outside the SPAparameter range of a
    // bounded curve. For a periodic curve the parameters are not
    // reduced to the principal range, and so the portion of the
    // curve evaluated may include several complete circuits. This
    // function is therefore always a monotonically increasing
    // function of its second argument if the first is held constant,
    // and a decreasing function of its first argument if the second
    // is held constant.
/**
 * Returns the algebraic distance along this <tt>bounded_curve</tt> between the given parameters.
 * <br><br>
 * <b>Role:</b> The sign is positive if the parameter values are given in increasing
 * order, and negative if they are in decreasing order. The result is undefined if
 * either parameter value is outside the parameter range of the bounded curve. For a
 * periodic curve, the parameters are not reduced to the principal range, so the
 * portion of the curve evaluated may include several complete circuits. This method
 * is always a monotonically increasing function of <tt>t1</tt> if <tt>t0</tt> is held
 * constant, and a decreasing function of <tt>t0</tt> if <tt>t1</tt> is held constant.
 * <br><br>
 * @param t0
 * first parameter value.
 * @param t1
 * second parameter value.
 */
     virtual double length(double t0, double t1) const;

    // The inverse of the length function. Return the SPAparameter value
    // of the point on the curve at the given algebraic arc length
    // from that defined by the datum SPAparameter. The result is not
    // defined for a bounded non-periodic curve if the datum SPAparameter
    // is outside the SPAparameter range, or if the length is outside
    // the range bounded by the values for the ends of the SPAparameter
    // range.
/**
 * Returns the parameter value of the point on this <tt>bounded_curve</tt> at the given algebraic arc length from that defined by the given parameter.
 * <br><br>
 * <b>Role:</b>  This method is the inverse of the <tt>length</tt> method. The result is not
 * defined for a bounded nonperiodic curve if the <tt>param</tt> argument is outside the
 * parameter range, or if the <tt>length</tt> argument is outside the range bounded by the
 * values for the ends of the parameter range.
 * <br><br>
 * @param param
 * datum parameter value.
 * @param length
 * arc length.
 */
     virtual double length_param(
        double param,         // datum SPAparameter
        double length         // arc length
        ) const;

    // Find the extrema of a curve in a given direction.
/**
 * Finds the extrema of this <tt>bounded_curve</tt> in a given direction.
 * <br><br>
 * <b>Role:</b> The <tt>curve_extremum</tt> class is defined in <tt>curdef.hxx</tt>.
 * <br><br>
 * @param dir
 * unit vector specifying the direction.
 */
    virtual curve_extremum* find_extrema(
        const SPAunit_vector& dir
        ) const;

    // Test point-on-curve, optionally returning the exact SPAparameter
    // value if the point is on the curve. Tests to standard system
    // precision.
/**
 * Tests point-on-curve, optionally returning the exact parameter value if the point is
 * on this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> This method tests to standard system precision.
 * <br><br>
 * @param pos
 * position of the point to test.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * actual parameter.
 */
    logical test_point(
        const SPAposition& pos,
        const double* param_guess = NULL,
        double* param_actual = NULL
        ) const;

    // Test point-on-curve, optionally returning the exact SPAparameter
    // value if the point is on the curve. Tests to given precision.
/**
 * Tests point-on-curve to a given precision, optionally returning the exact parameter value if the point is on this <tt>bounded_curve</tt>.
 * <br><br>
 * @param pos
 * position of the point to test.
 * @param tol
 * tolerance within which to test.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * actual parameter.
 */
    virtual logical test_point_tol(
        const SPAposition& pos,
        double tol = 0,
        const double* param_guess = NULL,
        double* param_actual = NULL
        ) const;


    // Indicate whether a curve is periodic, that is joins itself
    // smoothly at the ends of its principal SPAparameter range, so that
    // edges may span the "seam".
    // (this is not strictly needed, because you can check for zero
    // returned from param_period)
/**
 * Indicates if this <tt>bounded_curve</tt> is periodic.
 * <br><br>
 * <b>Role:</b> If so, the curve joins itself smoothly at the ends of its principal
 * parameter range, so that <tt>EDGEs</tt> may span the seam.
 */
    virtual logical periodic() const;


    // Indicate whether a curve is closed, that is joins itself
    // (smoothly or not) at the ends of its principal SPAparameter
    // range. Note that this function should always return TRUE
    // if periodic() does.
/**
 * Indicates if this <tt>bounded_curve</tt> is closed.
 * <br><br>
 * <b>Role:</b> This method indicates whether the curve joins itself (smoothly or not) at
 * the ends of its principal parameter range. If the <tt>periodic</tt> method returns
 * <tt>TRUE</tt>, this method also returns <tt>TRUE</tt>.
 */
     virtual logical closed() const;


    // Return the period of a periodic curve, zero if the curve
    // is not periodic.
/**
 * Returns the period of a periodic curve, or 0 if this <tt>bounded_curve</tt> is not periodic.
 */
     virtual double param_period() const;

    // If this curve is a subset a a curve as a result of setting
    // the SPAparameter range, then this method will return the full
    // curve.  If it is not a subset, a copy of this curve will be
    // returned.
/**
 * Returns a copy of this <tt>bounded_curve</tt>, unsubsetted.
 * <br><br>
 * <b>Role:</b> If the curve is a subset of a curve as a result of setting the parameter
 * range, this method returns the full curve.
 */
    bounded_curve* get_full_curve() const;

/**
 * Computes a bounding box around this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> There is no guarantee that the bound is minimal.
 */
    SPAbox bound() const;

    // Split curve at given SPAparameter value, if the curve is
    // splittable (in practice one defined or approximated by one
    // or more splines). Returns a new curve for the low-SPAparameter
    // part, and the old one as the high-SPAparameter part. For a
    // non-splittable curve, leaves the old one alone and returns
    // NULL. The default is to make the curve non-splittable, which
    // is always safe - this is only a speed optimisation.
/**
 * Splits this <tt>bounded_curve</tt> at a given parameter value.
 * <br><br>
 * <b>Role:</b> If the curve is splittable (not closed - in practice, one defined
 * or approximated by one or more splines), this method returns a new curve for
 * the low-parameter part, and the original one as the high-parameter part. For a
 * nonsplittable curve, it leaves the original one alone and returns <tt>NULL</tt>.
 * The default is to make the curve nonsplittable.
 * <br><br>
 * @param param
 * parameter value at which to split.
 * @param pos
 * position through which the curve should pass.
 */
    virtual bounded_curve* split(
        double param,          // SPAparameter value
        const SPAposition& pos  // SPAposition curve should pass through
    );



//----------------------------------------------------------------------
// The following methods are not defined for ACIS curve, but are
// probably usefull, so I define them for bounded_curves.

    // get SPAvector normal to curve (zero SPAvector if not planar or straight)
/**
 * Returns the vector normal to this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> Returns the zero vector if the curve is straight or is nonplanar.
 */
    virtual SPAunit_vector get_normal() const;

    // determine which side of the curve a given point is on relative
    // to a plane defined by a SPAunit_vector.
    // returns +1 for right and -1 for left
/**
 * Determines which side of this <tt>bounded_curve</tt> a given point is on, relative to a plane defined by a unit-vector normal.
 * <br><br>
 * <b>Role:</b> This method returns +1 for right and -1 for left.  It returns 0 if the
 * side cannot be determined.  (This may be because the point is on the curve, or because
 * the curve tangent is parallel to the plane normal.)
 * <br><br>
 * @param vec
 * unit vector normal describing the plane.
 * @param pos
 * point to test.
 */
    int get_side(const SPAunit_vector& normal , const SPAposition& pos );

    // Set start and end points of curve.  This method assumes that
    // the given SPAposition lies on the curve, and modifies the curve
    // so that it starts or ends at that SPAposition.  If the SPAposition
    // is not on the curve, the closest SPAposition on the curve is used.
/**
 * Sets the start points of this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> This method assumes that the given position lies on the curve, and it
 * modifies the curve so it starts at that position. If the position is not on the
 * curve, the closest position on the curve is used.
 * <br><br>
 * For curves, this method takes an object of class <tt>SPAparameter</tt> as input for
 * an approximation. For consistency, these methods all use doubles for curve parameters.
 * <br><br>
 * @param pos
 * position at which the curve is to start.
 * @param approx
 * approximate parameter position.
 */

    double set_start_t( const SPAposition& pos,
                        const double* approx = NULL);
/**
 * Sets the end points of this <tt>bounded_curve</tt>.
 * <br><br>
 * <b>Role:</b> This method assumes that the given position lies on the curve, and it
 * modifies the curve so it ends at that position. If the position is not on the curve,
 * the closest position on the curve is used.
 * <br><br>
 * For curves, this method takes an object of class <tt>SPAparameter</tt> as input for
 * an approximation. For consistency, these methods all use doubles for curve parameters.
 * <br><br>
 * @param pos
 * position at which the curve is to end.
 * @param approx
 * approximate parameter position.
 */

    double set_end_t( const SPAposition& pos,
                      const double* approx = NULL);

    // change_start_pt and change_end_pt will try to actually move the
    // start or end point of a curve to a new location.  For some types
    // of curves, this may not be possible in which case these methods
    // act the same as set_start_pt and set_end_pt
/**
 * Moves the start point of this <tt>bounded_curve</tt> to a new location.
 * <br><br>
 * <b>Role:</b> For some types of curves this may not be possible, in which case
 * this method acts the same as <tt>set_start_pt</tt>.
 * <br><br>
 * @param pos
 * the new start point.
 */
    virtual logical change_start_pt(const SPAposition& pos);

/**
 * Moves the end point of this <tt>bounded_curve</tt> to a new location.
 * <br><br>
 * <b>Role:</b> For some types of curves this may not be possible, in which case
 * this method acts the same as <tt>set_end_pt</tt>.
 * <br><br>
 * @param pos
 * the new end point.
 */

    virtual logical change_end_pt(const SPAposition& pos);

    // return start or end SPAparameter
/**
 * Returns the start parameter of this <tt>bounded_curve</tt>.
 */
    double get_start_param() const {return start_param;}

/**
 * Returns the end parameter of this <tt>bounded_curve</tt>.
 */
    double get_end_param() const {return end_param;}

    // convert between parameters in the range [0,1] to the real range
/**
 * Converts to parameters within the range from 0 to 1, from the double range.
 * <br><br>
 * @param t
 * parameter to convert.
 */
    double param_to_01(double t);

/**
 * Converts from parameters within the range from 0 to 1, to the double range.
 * <br><br>
 * @param t
 * parameter to convert.
 */
    double param_from_01(double t)
       { return (start_param + (t*(end_param-start_param))); }

    // return start or end point
/**
 * Returns the start position of this <tt>bounded_curve</tt>.
 */
    SPAposition get_start_pt() const {return eval_position(get_start_param());}
/**
 * Returns the end position of this <tt>bounded_curve</tt>.
 */
    SPAposition get_end_pt() const  {return eval_position(get_end_param());}

    // return direction at start or end
/**
 * Returns the start direction of this <tt>bounded_curve</tt>.
 */
    SPAunit_vector get_start_dir() const {return eval_direction(get_start_param());}

/**
 * Returns the end direction of this <tt>bounded_curve</tt>.
 */
    SPAunit_vector get_end_dir() const {return eval_direction(get_end_param());}

    // set start or end parameters
/**
 * Sets the start parameter of this <tt>bounded_curve</tt>.
 * <br><br>
 * @param param
 * the new start parameter.
 */
    double set_start_param(double param);

/**
 * Sets the end parameter of this <tt>bounded_curve</tt>.
 * <br><br>
 * @param param
 * the new end parameter.
 */
    double set_end_param(double param);

/**
 * Sets the parameter range of this <tt>bounded_curve</tt>.
 * <br><br>
 * @param start_param
 * the new start parameter.
 * @param end_param
 * the new end parameter.
 */
     void set_parameter_range(double start_param, double end_param);

    // get SPAparameter range
/**
 * Returns the parameter range of this <tt>bounded_curve</tt>.
 */
     double get_range() const { return (get_end_param() - get_start_param());}

    // Return a curve which is the projection of this curve onto a plane
/**
 * Returns a curve that is the projection of this <tt>bounded_curve</tt> onto a given plane.
 * <br><br>
 * <b>Role:</b> This method returns a new curve which is the projection of
 * this curve onto a given plane. The default behavior is to create a curve
 * that is an accurate projection of the <tt>bounded_curve</tt> to within
 * <tt>SPAresabs</tt>.
 * <br><br>
 * If called with the argument <tt>accurate</tt> set to <tt>FALSE</tt>, a less
 * accurate projection curve is returned. The benefit of using this is that
 * the projection can be constructed much more quickly, though applications
 * should use this only for, e.g., getting a visual representation of the curve,
 * as it is <i>not</i> guaranteed that these "quick and dirty" projection curves will
 * be coincident with the true projection to within <tt>SPAresabs</tt>.
 * <br><br>
 * Note that regardless of the flag setting, it is possible for the returned curve not
 * to be a legal ACIS curve e.g. it may self-intersect or have discontinuities.
 * <br><br>
 * @param plane1
 * plane on which to project.
 * @param accurate
 * flag indicating whether to do an accurate (default) or a quick inaccurate
 * projection.
 */
     bounded_curve* project_to_plane(const plane& plane1,
							         logical accurate = TRUE) const;


    // Make the line and arc classes friends so that they can access the
    // protected data and methods

  public:
    // create an EDGE from this curve
/**
 * Creates an <tt>EDGE</tt> from this <tt>bounded_curve</tt>.
 */
     virtual EDGE* make_edge() const;

    // create a bs3_curve from this bounded_curve
/**
 * Creates a <tt>bs3_curve</tt> from this <tt>bounded_curve</tt>.
 */
    bs3_curve make_bs3_curve() const;

    // Tests if a curve lies in a plane which is perpendicular to
    // the given SPAunit_vector.
/**
 * Returns <tt>TRUE</tt> if this <tt>bounded_curve</tt> lies in a plane that is perpendicular to the given unit-vector normal; otherwise, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param normal
 * the plane normal.
 */
     virtual logical is_in_parallel_plane(const SPAunit_vector& normal) const;

    // Test to see if a curve lies in a given plane
/**
 * Returns <tt>TRUE</tt> if this <tt>bounded_curve</tt> lies in a given plane; otherwise, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param pos
 * position on the plane.
 * @param normal
 * normal to plane.
 */
    virtual logical is_in_plane(const SPAposition& pos, const SPAunit_vector& normal) const;

    // get a tolerance to use for comparing if two SPAparameter values will
    // evaluate to the same point
/**
 * Returns a tolerance to use when determining whether two parameter values evaluate to the same position.
 * <br><br>
 * @param t
 * parameter at which go get the tolerance.
 * @param tol
 * tolerance to use on the points.
 */
    double get_parameter_tolerance(
        double t,            // parameter at which to get tolerance
        double tol           // tolerance that you want to use on the points
        ) const;

    // Adjust a SPAparameter value so that it is within the principal range
    // of a periodic curve.  If the curve is not periodic, then the
    // input SPAparameter is returned.  For a periodic curve, a SPAparameter value
    // between start_param and end_param is returned
/**
 * Adjusts a parameter value so that it is within the principal range of a periodic curve.
 * <br><br>
 * <b>Role:</b> If this curve is not periodic, this method returns the input parameter.
 * For a periodic curve, this method returns a parameter value between the start
 * parameter and end parameter.
 * <br><br>
 * @param t
 * parameter value to be adjusted.
 */
    double adjust_parameter(double t) const;

};

//======================================================================
//----------------------------------------------------------------------
// purpose---
//    create a new elliptical curve
//----------------------------------------------------------------------

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN bounded_curve* new_ellipse(
    const SPAposition&,     // center
    const SPAunit_vector&,  // normal SPAvector
    const SPAvector&,       // major axis
    double,                 // ratio of major axis to minor axis
    double,                 // start angle
    double                  // end angle
    );

//======================================================================
// Get a bounded_curve from an EDGE.  The bounded curve will contain a
// copy of the curve pointed to by the EDGE.  If the EDGE belongs to
// a BODY, then the BODY transform will be applied, so that the bounded_curve
// is always defined in the model coordinate system rather than in the
// coordinate system of the body.

// This is different than the constructor for bounded_curve because it
// checks the type of the ACIS curve of the EDGE, and creates the appropriate
// type of bounded_curve.  For example, if the EDGE points to a straight,
// then this procedure will create a bounded_line and return that.  We need
// this procedure since we cannot define virtual constructors.

// STI let (3/97): Put in a flag so that the body transform
// can *optionally* be applied.  This defaults to the previous
// behavior of applying the transform.
//DECL_KERN bounded_curve* get_bounded_curve(EDGE* crv);

/**
 * Gets a <tt>bounded_curve</tt> from an <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>bounded_curve</tt> contains a copy of the curve
 * pointed to by the <tt>EDGE</tt>. If the <tt>EDGE</tt> belongs to a
 * <tt>BODY</tt>, then by default the <tt>BODY</tt> transformation is applied,
 * so the bounded curve is always defined in the model coordinate system rather
 * than in the coordinate system of the <tt>BODY</tt>.
 * <br><br>
 * This method differs from the constructor for bounded curve in that it checks the
 * type of the curve of the <tt>EDGE</tt>, and creates the appropriate type of bounded curve.
 * For example, if the <tt>EDGE</tt> points to a <tt>STRAIGHT</tt>, this method creates a bounded
 * line and returns it.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param edge
 * EDGE from which to get the bounded curve.
 * @param apply_transf
 * flag to apply the BODY transformation.
 **/
DECL_KERN bounded_curve* get_bounded_curve(	EDGE* edge,
											logical apply_transf = TRUE);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN bounded_curve* get_bounded_curve(	TCOEDGE* crv,
											logical apply_transf = TRUE);
// STI let: end

/** @} */
#endif
