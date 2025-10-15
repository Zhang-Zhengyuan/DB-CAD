/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                    straight curve class
// A straight line is represented by a point on it and a tangent
// direction. It also has a scale factor for the parametrisation, so
// that the SPAparameter values can be made invariant under
// transformation.
// The parametric equation of the line is :
//		        point = root_point + t * param_scale * direction
// where t is the SPAparameter.
/*******************************************************************/
#if !defined( straight_CLASS )
#define straight_CLASS
#include "dcl_kern.h"
#include "curdef.hxx"
#include "debugmsc.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
class SPAbox;
class SPAtransf;
class SPAparameter;
class law;
class SizeAccumulator;
class straight;
/*******************************************************************/
/**
* @file strdef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */

 /**
  * @nodoc
  * Define an identifying type for this (lower-case) curve.
  */
#define straight_type 1

  /**
   * Applies transformation to straight curve.
   * <br><br>
   * @param input
   * input straight curve.
   * @param trans
   * transformation to use.
   */
DECL_KERN straight operator*(straight const& input, SPAtransf const& trans);
/**
 * @nodoc
 * Restoring straight from file. Internal use.
 */
DECL_KERN curve* restore_straight();
/**
 * Defines an infinite straight line represented by a point and a unit vector specifying the direction.
 * <br>
 * <b>Role:</b> This class defines an infinite straight line represented by a point
 * and a unit vector specifying the direction. A straight also has a scale factor
 * for the parameterization, so the parameter values can be made invariant under
 * transformation.
 * <br><br>
 * A straight line is an open curve that is not periodic. It is parameterized as:
 * <pre>
 * point = root_point + t* param_scale* direction </pre>
 * where <i>t</i> is the parameter.
 * @see STRAIGHT, SPAposition, SPAunit_vector
 */
class DECL_KERN straight : public curve
{
public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    straight();
    straight(const char* gme);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Creates a <tt>straight</tt> using a point, a direction, and a parameter scaling.
     * <br><br>
     * @param pos
     * point.
     * @param dir
     * direction.
     * @param scale
     * parameter scaling.
     */
    straight(
        SPAposition const& pos,
        SPAunit_vector const& dir,
        double               scale = 1.0
    );
    straight(
        const char* gme,
        SPAposition const& pos,
        SPAunit_vector const& dir,
        double               scale = 1.0
    );
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param line
     * straight curve.
     */
    straight(
        straight const& line
    );
    straight(
        const char* gme,
        straight const& line
    );
    /**
     * C++ destructor, deleting a straight.
     */
    ~straight();
    /**
     * The tangent direction along the line.
     * <br><br>
     * <b>Role:</b> Defines the direction of the curve. A <tt>NULL</tt> unit vector is in an uninitialized straight line.
     */
    SPAunit_vector direction;
    /**
     * The scaling factor for parameterization that allows fixed parameters despite transformation.
     */
    double param_scale;
    /**
     * Base position through which the straight line passes.
     */
    SPAposition root_point;
#define ALL_CURVE_DERIVATIVES 9999
    /**
     * Returns the number of derivatives that <tt>evaluate</tt> can find accurately.
     * <br><br>
     * <b>Role:</b> For a <tt>straight</tt>, any number of derivatives can be calculated
     * (which are all zero after the first) and so the value <tt>ALL_CURVE_DERIVATIVES</tt> is returned.
     * <br><br>
     * @param cur
     * portion of the curve.
     */
    virtual int accurate_derivs(
        SPAinterval const& cur = *(SPAinterval*)NULL_REF
    ) const;

    /**
     * Returns a box enclosing the two given points on the straight line.
     * <br><br>
     * @param pos1
     * first position.
     * @param pos2
     * second position.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAposition const& pos1,
        SPAposition const& pos2,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box enclosing the two given points on the straight line.
     * <br><br>
     * @param inter
     * two given points represented as an interval.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAinterval const& inter,
        SPAtransf   const& trans = *(SPAtransf*)NULL_REF
    ) const;

    /**
     * Returns a box surrounding that portion of the curve within the given box.
     * <br><br>
     * @param box
     * box.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAbox    const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box enclosing the two given points on the straight line.
     * <br><br>
     * <b>Role:</b> This function is retained for historical reasons.
     * <br><br>
     * @param start
     * start point on the line.
     * @param end
     * end point on the line.
     * @param t
     * transformation.
     */
    SPAbox bound(
        double          start,
        double          end,
        SPAtransf const& t = *(SPAtransf*)NULL_REF
    ) const
    {
        return bound(SPAinterval(start, end), t);
    }
    /**
     * Indicates if a curve is closed.
     * <br><br>
     * <b>Role:</b> This function joins itself (smoothly or not) at the ends of its
     * principal parameter range. This function should always return <tt>TRUE</tt> if periodic does.
     */
    virtual logical closed() const;
    /**
     * Outputs a title line and details about the straight line to the debug file or to the specified file.
     * <br><br>
     * @param title
     * title line.
     * @param fp
     * file name.
     */
    virtual void debug(
        char const* title,
        FILE* fp = debug_file_ptr
    ) const;
    /**
     * Creates a copy of an item that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers.
     * Returns a pointer to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual curve* deep_copy(pointer_map* pm = NULL) const;
    /**
     * Returns a cylinder that encloses the portion of the curve bounded by the interval.
     * <br><br>
     * @param inter
     * interval.
     */
    virtual BOUNDING_CYLINDER enclosing_cylinder(const SPAinterval& inter =
        *(SPAinterval*)NULL_REF) const;
    /**
     * Evaluates the curve at a given parameter value, giving the position, and first and second derivatives, all of which are optional.
     * <br><br>
     * @param val
     * parameter value.
     * @param pos
     * point on curve at given parameter.
     * @param der1
     * first derivative.
     * @param der2
     * second derivative.
     * @param flag
     * take advantage of report orders, so values don't have to be recomputed.
     * @param approx
     * approx ok?
     */
    virtual void eval(
        double      val,
        SPAposition& pos,
        SPAvector& der1 = *(SPAvector*)NULL_REF,
        SPAvector& der2 = *(SPAvector*)NULL_REF,
        logical     flag = FALSE,
        logical     approx = FALSE
    ) const;
    /**
     * Returns the curvature at a given parameter value.
     * <br><br>
     * <b>Role:</b> This is always zero, so all the arguments are ignored.
     * <br><br>
     * @param param
     * parameter.
     * @param repeat
     * repeat order?
     * @param approx
     * approx ok?
     */
    virtual SPAvector eval_curvature(
        double param,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Finds the magnitude of the derivative at the given parameter value on the curve.
     * <br><br>
     * @param pt
     * point.
     * @param repeat
     * repeat order?
     * @param approx
     * approx ok?
     */
    virtual double eval_deriv_len(
        double pt,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
     * <br><br>
     * <b>Role:</b> This function returns the number it was able to calculate;
     * this will be equal to the number requested in all but the most exceptional
     * circumstances. A certain number will be evaluated directly and (more or less)
     * accurately; higher derivatives will be automatically calculated by finite
     * differencing; the accuracy of these decreases with the order of the derivative,
     * as the cost increases.
     * <br><br>
     * @param parameter
     * parameter.
     * @param pt
     * Point on curve at given parameter.
     * @param vec
     * Array of pointers to vectors, of size nd. Any of the pointers may be null, in
     * which case the corresponding derivative will not be returned.
     * @param deriv
     * Number of derivatives required (nd).
     * @param location
     * the evaluation location - above, below or don't care.
     */
    virtual int evaluate(
        double      parameter,
        SPAposition& pt,
        SPAvector** vec = NULL,
        int         deriv = 0,
        evaluate_curve_side location = evaluate_curve_unknown
    ) const;
    /**
     * Finds the extrema of the curve in the given direction.
     * <br><br>
     * <b>Role:</b> A straight line has no extrema, so this method returns <tt>NULL</tt>.
     * <br><br>
     * @param dir
     * direction.
     */
    virtual curve_extremum* find_extrema(
        SPAunit_vector const& dir
    ) const;
    /**
     * @nodoc
     * Internal use.
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * Finds regions of high curvature of the curve.
     * <br><br>
     * <b>Role:</b> This method stores an array of intervals in spans argument over
     * which the curvature exceeds <i>k</i>. It returns the number of intervals stored.
     * @param k
     * maximum curvature
     * @param spans
     * interval list
     */
    virtual	int	high_curvature(double k, SPAinterval*& spans) const;
    /**
     * Returns the law form of the straight entity.
     */
    law* law_form();
    /**
     * Returns the algebraic distance along the curve between the given parameters.
     * <br><br>
     * <b>Role:</b> The sign is positive if the parameter values are given in increasing
     * order, and negative if they are in decreasing order. The result is undefined if
     * either parameter value is outside the parameter range of a bounded curve. For a
     * periodic curve the parameters are not reduced to the principal range, and so
     * the portion of the curve evaluated may include several complete circuits.
     * This function is therefore always a monotonically increasing function of its
     * second argument if the first is held constant, and a decreasing function of its
     * first argument if the second is held constant.
     * <br><br>
     * @param start
     * start parameter
     * @param end
     * end parameter
     * @param approx_ok
     * not used for straights
     */
    virtual double length(
        double start,
        double end,
        logical approx_ok = TRUE
    ) const;
    /**
     * Returns the parameter value of the point on the curve at the given algebraic arc length from that defined by the datum parameter.
     * <br><br>
     * <b>Role:</b> This method is the inverse of the <tt>length</tt> method. The result is not
     * defined for a bounded nonperiodic curve if the datum parameter is outside the
     * parameter range, or if the length is outside the range bounded by the values
     * for the ends of the parameter range.
     * <br><br>
     * @param datum
     * datum parameter.
     * @param arc
     * arc length.
     * @param approx_ok
     * not used for straights
     */
    virtual double length_param(
        double datum,
        double arc,
        logical approx_ok = TRUE
    ) const;
    /**
     * Makes a copy of this straight on the heap, and returns a pointer to it.
     */
    virtual curve* make_copy() const;
    /**
     * Negates this straight line by negating the direction.
     */
    virtual curve& negate();
    /**
     * Returns a straight line with the opposite sense from this line.
     */
    straight operator-() const;
    /**
     * Tests two straight curves for equality.
     * <br><br>
     * <b>Role:</b> Analyzes two straight curves for equality.
     * Use this result for optimization.
     * <br><br>
     * @param cur
     * curve.
     */
    virtual logical operator==(curve const& cur) const;

    /**
     * @nodoc
     */
    virtual logical operator>>(curve const&) const;

    /**
     * @nodoc
     * Return "this" straight line transformed by the given SPAtransf.
     */
    friend DECL_KERN straight operator*(
        straight const&,
        SPAtransf const&
        );
    /**
     * Transforms this straight line by the given transformation.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual curve& operator*=(
        SPAtransf const& trans
        );
    /**
     * Finds the parameter value at the given point on the curve.
     * <br><br>
     * <b>Role:</b> If the point is not on the curve, a plane is constructed perpendicular
     * to the line, and the parameter value for its intersection with the line returns.
     * <br><br>
     * @param pos
     * position.
     * @param guess
     * param guess.
     */
    virtual double param(
        SPAposition const& pos,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the parameter period, 0 in this case because a straight line is not periodic.
     */
    virtual double param_period() const;
    /**
     * Returns the principal parameter range, or what is inside the given box, if one is indeed given.
     * <br><br>
     * <b>Role:</b> If there is no box, the range is unbounded, and we return the empty interval.
     * <br><br>
     * @param box
     * box.
     */
    virtual SPAinterval param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Indicates if the curve is periodic and joins itself smoothly at the ends of its principal parameter range, so that edges may span the seam.
     */
    virtual logical periodic() const;
    /**
     * Returns the curvature, which is 0 for a straight line.
     * <br><br>
     * <b>Role:</b> It is immaterial whether the point is on or off the curve.
     * The <tt>SPAparameter</tt> argument is ignored.
     * <br><br>
     * @param pos
     * point.
     * @param guess
     * param guess.
     */
    virtual SPAvector point_curvature(
        SPAposition const& pos,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;

    /**
     * Returns the direction of the curve at a point on it.
     * <br><br>
     * <b>Role:</b> This is a constant for a straight line, so it is immaterial whether
     * the given point is on or off the curve.
     * <br><br>
     * @param pos
     * position.
     * @param guess
     * param guess.
     */
    virtual SPAunit_vector point_direction(
        SPAposition const& pos,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;

    /**
     * Finds the foot of the perpendicular from the given point to the curve, and tangent to the curve at that point, and its parameter value.
     * <br><br>
     * <b>Role:</b> If an input parameter value is supplied (as argument 5), the
     * perpendicular found is the one nearest to the supplied parameter position,
     * otherwise it is the one at which the curve is nearest to the given point.
     * Any of the return value arguments may be a <tt>NULL</tt> reference, in which case it
     * is simply ignored.
     * <br><br>
     * @param pos
     * position.
     * @param foot
     * foot.
     * @param dir
     * curve direction at foot.
     * @param cur
     * curvature at foot.
     * @param guess
     * param guess.
     * @param actual
     * actual param.
     * @param f_weak
     * weak flag.
     */
    virtual void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& dir,
        SPAvector& cur,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const;

    /**
     * Drops a perpendicular from the given point to the line, returning the foot of the perpendicular, and the curve direction there.
     * <br><br>
     * @param pos
     * position.
     * @param foot
     * foot.
     * @param foot_dt
     * tangent to curve at foot.
     * @param guess
     * param guess.
     * @param actual
     * actual param.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& foot_dt,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            foot_dt,
            *(SPAvector*)NULL_REF,
            guess,
            actual, f_weak
        );
    }
    /**
     * Finds the foot of the perpendicular from the given point to the curve, and tangent to the curve at that point, and its parameter value.
     * <br><br>
     * @param pos
     * position.
     * @param foot
     * foot.
     * @param guess
     * param guess.
     * @param actual
     * actual param.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            *(SPAunit_vector*)NULL_REF,
            *(SPAvector*)NULL_REF,
            guess,
            actual, f_weak
        );
    }
    /**
     * Restores the data for a straight from a save file.
     * <br><br>
     * <b>Role:</b>
     * <pre>
     * read_position			Root point
     * read_vector			The direction of straight
     * curve::restore_data		Restore the rest of the curve data.</pre>
     */
    void restore_data();
    /**
     * @nodoc
     * Internal use. Restore mechanism.
     */
    friend DECL_KERN curve* restore_straight();
    /**
     * Saves the <tt>type</tt> or <tt>id</tt>, then calls <tt>save_data</tt>.
     */
    virtual void save() const;
    /**
     * Saves the information for the straight to a save file.
     */
    void save_data() const;
    /**
     * Returns a cone bounding the tangent direction of a curve.
     * <br><br>
     * <b>Role:</b> The cone has its apex at the origin, and it has a given axis direction
     * and positive half-angle. If logical is <tt>TRUE</tt>, then approximate results are found.
     * The approximate result may lie wholly within or wholly outside the guaranteed
     * bound (obtained with the <tt>FALSE</tt> argument), but it may not cross from side to outside.
     * Flags in the returned object indicate whether the cone is the best available,
     * and if the result is inside or outside the best cone.
     * <br><br>
     * @param inter
     * line interval.
     * @param result
     * approx results OK?
     * @param trans
     * transformation.
     */
    virtual curve_tancone tangent_cone(
        SPAinterval const& inter,
        logical           result,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Tests a point on the curve, returning the parameter value if it is required.
     * <br><br>
     * @param pos
     * point.
     * @param tol
     * tolerance.
     * @param guess
     * param guess.
     * @param actual
     * actual param.
     */
    virtual logical test_point_tol(
        SPAposition const& pos,
        double             tol = 0,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the type of <tt>straight</tt>.
     */
    virtual int type() const;
    /**
     * Returns a string <tt>"straight"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Determines whether this curve is undefined.
     */
    logical undef() const;

#if defined D3_STANDALONE || defined D3_DEBUG
    /**
     * @nodoc
     * Internal use.
     */
    virtual void print(
        D3_ostream&
    ) const;
    /**
     * @nodoc
     * Internal use.
     */
    virtual void input(
        D3_istream&
    );

#endif

public:
    void gme_set_param_scale(double val);
};

/** @} */
#endif
