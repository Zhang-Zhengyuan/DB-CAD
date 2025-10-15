/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                      ellipse curve
/*******************************************************************/
#if !defined( ellipse_CLASS )
#define ellipse_CLASS
#include "acis.hxx"
#include "dcl_kern.h"
#include "curdef.hxx"
#include "debugmsc.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "position.hxx"

class SizeAccumulator;
class SPAbox;
class SPAtransf;
class law;
class SPAparameter;
class ellipse;
/*******************************************************************/
/**
* @file elldef.hxx
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
#define ellipse_type 2
  /**
   * Returns an ellipse transformed by the given transformation.
   * <br><br>
   * @param name
   * ellipse to transform.
   * @param transf
   * transformation.
   */
DECL_KERN ellipse operator*(ellipse const& name, SPAtransf const& transf);
DECL_KERN ellipse gme_operator_mul(ellipse const& name, SPAtransf const& transf);
/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN curve* restore_ellipse();
/**
 * Defines an ellipse curve.
 * <br><br>
 * <b>Role:</b> The ellipse class represents circles and ellipses on any plane.
 * An ellipse is defined by a center point, a unit vector normal to the plane of the
 * ellipse, a vector defining the major axis of the ellipse (including the magnitude
 * of the major axis), and the radius ratio of the minor axis length to the major axis
 * length. Currently, the length of the major axis should be greater than <tt>SPAresfit</tt>.
 * In a circle, the radius ratio is exactly 1.0. The direction of the ellipse is defined
 * by the right hand rule using the normal vector. Direction is important when defining
 * an edge supported by this geometry.
 * <br><br>
 * An ellipse is a closed curve that has a period <tt>2*pi</tt> and an interval range of
 * <tt>[-pi, pi]</tt>. It is parameterized by:
 * <pre>
 * point = center + M cos(t - offset) + N sin(t - offset)</pre>
 * where <i>M</i> and <i>N</i> are the major and minor axes respectively. The offset is stored explicitly,
 * and is present to allow future addition of an isometric scaling that does not transform
 * the axes into those of the transformed ellipse.
 * <br>br>
 * For images illustrating this class, look under Technical Articles of the Kernel Component at "Classes with Images".
 * @see ELLIPSE, cone, SPAparameter, SPAposition, SPAunit_vector, SPAvector
 */
class DECL_KERN ellipse : public curve
{
public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    ellipse();
    ellipse(const char* gme);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Constructs an ellipse from its center, normal, radius vector and radius ratio, with an optional parameter offset.
     * <br><br>
     * @param center
     * center.
     * @param normal
     * normal.
     * @param maj_axis
     * major axis.
     * @param ratio
     * radius ratio.
     * @param  param_off
     * param_off.
     */
    ellipse(
        SPAposition const& center,
        SPAunit_vector const& normal,
        SPAvector const& maj_axis,
        double                ratio,
        double                param_off = 0.0
    );
    ellipse(
        const char* gme,
        SPAposition const& center,
        SPAunit_vector const& normal,
        SPAvector const& maj_axis,
        double                ratio,
        double                param_off = 0.0
    );
    /**
     * C++ destructor, deleting an <tt>ellipse</tt>.
     */
    ~ellipse();
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param name
     * ellipse name.
     */
    ellipse(ellipse const& name);
    ellipse(const char* gme, ellipse const& name);
    /**
     * Center point of ellipse.
     */
    SPAposition centre;
    /**
     * Vector giving length and direction of major axis.
     */
    SPAvector major_axis;
    /**
     * Length of the major axis.
     */
    double major_axis_length;
    /**
     * Normal to the plane of ellipse.
     */
    SPAunit_vector normal;
    /**
     * The parameter offset is the parameter value of the point at the end of the major axis.
     * <br><br>
     * <b>Role:</b> It is stored to allow the parameterization to remain unchanged under
     * transformation.
     */
    SPAparameter param_off;
    /**
     * Ratio between length of minor axis and major axis.
     * <br><br>
     * <b>Role:</b> Must be positive and less than or equal to 1. Ratio of 0 is used to indicate
     * <tt>"uninitialized"</tt>. Ratio of 1 is a circle.
     */
    double radius_ratio;

    // The inverse function requires the length of the quarter-ellipse
    // before it starts its real work, so we remember the most recent one
    // evaluated, in case several inversions are done on the same ellipse.
    // For slightly increased generality, we save a normalised length,
    // which depends only on the radius ratio.
    // These used to be LOCAL_VAR's - assumedly because they are modified
    // in const functions.  Mutable solves that problem - but we have to use
    // the homebrew version of mutable to ensure full platform support.
/**
 * @nodoc
 */
    mutable SPAvector minor_axis;
    /**
     * @nodoc
     */
    mutable double Prev_Half_Length;
    /**
     * @nodoc
     */
    mutable double Prev_Radius_Ratio;
    /**
     * @nodoc
     */
    mutable logical Force_Half_Length;
    /**
     * Returns the number of derivatives that the evaluate function can find accurately.
     * <br><br>
     * <b>Role:</b> And fairly directly, rather than by finite differencing, over the given portion
     * of the curve. If there is no limit to the number of accurate derivatives, returns the
     * value <tt>ALL_CURVE_DERIVATIVES</tt>.
     * <br><br>
     * @param cur
     * portion of a curve.
     */
    virtual int accurate_derivs(
        SPAinterval const& cur = *(SPAinterval*)NULL_REF
    ) const;
    /**
     * Finds box around an ellipse or portion thereof bounded by points on the curve.
     * <br><br>
     * <b>Role:</b> Increasing in parameter value. The points lie on the curve as supplied,
     * not as transformed. The resulting box is not necessarily be the minimal one.
     * <br><br>
     * @param range
     * given range.
     * @param posi
     * second position.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAposition const& range,
        SPAposition const& posi,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Finds box around an ellipse or portion thereof bounded by parameter values (in increasing order).
     * <br><br>
     * <b>Role:</b> The resulting box is not necessarily the minimal one.
     * <br><br>
     * @param range
     * given range.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAinterval const& range,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a bounding box around the portion of the ellipse inside a given box.
     * <br><br>
     * <b>Role:</b> The resulting bound is not necessarily the minimal one.
     * <br><br>
     * @param region
     * region of interest.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAbox const& region,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Retained temporarily for historical reasons.
     * <br><br>
     * @param start
     * start param value.
     * @param end
     * end param value.
     * @param t
     * Transformation.
     */
    SPAbox bound(
        double start,
        double end,
        SPAtransf const& t = *(SPAtransf*)NULL_REF
    ) const
    {
        return bound(SPAinterval(start, end), t);
    }
    /**
     * Indicates whether a curve is closed, that is joins itself (smoothly or not) at the ends of its principal parameter range.
     * <br><br>
     * <b>Role:</b> This function should always return <tt>TRUE</tt> if periodic does.
     */
    virtual logical closed() const;
    /**
     * Finds closest point.
     * <br><br>
     * <b>Role:</b> Finds the closest point on the curve, the foot
     * to the given point, and optionally its <tt>SPAparameter</tt> value. If an
     * input <tt>SPAparameter</tt> value is supplied (as the first <tt>SPAparameter </tt>
     * argument), the foot found is only a local solution nearest to
     * the supplied <tt>SPAparameter</tt>, <tt>SPAposition</tt>. Any of the return value
     * arguments may be a <tt>NULL</tt> reference, in which case it is simply
     * ignored.
     * <br><br>
     * @param pos
     * point.
     * @param foot
     * foot.
     * @param param_guess
     * param guess.
     * @param_actual
     * actual parameter.
     */
    virtual void closest_point(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Output details of the ellipse for inspection.
     * <br><br>
     * @param leader
     * leader.
     * @param fp
     * file pointer.
     */
    virtual void debug(
        char const* leader,
        FILE* fp = debug_file_ptr
    ) const;
    /**
     * Creates a copy of an item that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a pointer
     * to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual curve* deep_copy(pointer_map* pm = NULL) const;
    /**
     * Returns a cylinder that encloses the portion of the curve bounded by the interval.
     * <br><br>
     * @param interval
     * interval.
     */
    virtual BOUNDING_CYLINDER enclosing_cylinder(const SPAinterval& interval =
        *(SPAinterval*)NULL_REF) const;
    /**
     * Evaluates a curve at a given parameter value, giving position, and first and second derivatives (all optionally).
     * <br><br>
     * <b>Role:</b> The first logical argument, if <tt>TRUE</tt>, is a guarantee from the calling
     * code that  the most recent call to any curve or surface member function was in fact
     * to this routine for the same curve as the current call. It allows an implementation to
     * cache useful intermediate results to speed up repeated evaluations, but must be used with
     * extreme care.
     * <br><br>
     * The second logical argument may be set <tt>TRUE</tt> if an approximate return value is
     * acceptable. Here approximate is not very well-defined, but may be assumed to be sufficient
     * for visual inspection of the curve.
     * <br><br>
     * @param val
     * parameter value.
     * @param posi
     * position.
     * @param first
     * first derivative.
     * @param second
     * second derivative.
     * @param repeat
     * repeat.
     * @param logi
     * logical.
     */
    virtual void eval(
        double val,
        SPAposition& posi,
        SPAvector& first = *(SPAvector*)NULL_REF,
        SPAvector& second = *(SPAvector*)NULL_REF,
        logical repeat = FALSE,
        logical logi = FALSE
    ) const;
    /**
     * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
     * <br><br>
     * <b>Role:</b> This function returns the number it was able to calculate;
     * this is equal to the number requested in all but the most exceptional circumstances.
     * A certain number are evaluated directly and (more or less) accurately; higher derivatives
     * will be automatically calculated by finite differencing; the accuracy of these decreases
     * with the order of the derivative, as the cost increases.
     * <br><br>
     * @param val
     * parameter value.
     * @param pt
     * point on curve at given parameter.
     * @param ptr_arr
     * ptr array to derivative vectors.
     * @param num
     * number of derivatives.
     * @param loc
     * the evaluation location.
     */
    virtual int evaluate(
        double       val,
        SPAposition& pt,
        SPAvector** ptr_arr = NULL,
        int          num = 0,
        evaluate_curve_side loc = evaluate_curve_unknown
    ) const;
    /**
     * Finds the extrema of an ellipse in a given direction.
     * <br><br>
     * @param dirc
     * direction.
     */
    virtual curve_extremum* find_extrema(SPAunit_vector const& dirc) const;
    /**
     * @nodoc
     * Internal use. Roll mechanism
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * This method returns the length of major axis.
     */
    double GetMajorAxisLength() const;
    /**
     * Finds regions of high curvature of the curve.
     * <br><br>
     * <b>Role:</b> This method stores an array of intervals in spans argument over which
     * the curvature exceeds <i>k</i>. It returns the number of intervals stored.
     * <br><br>
     * @param k
     * maximum curvature.
     * @param spans
     * interval list.
     */
    int	 high_curvature(double k, SPAinterval*& spans) const;
    /**
     * Returns a pointer to the law form that is part of the ellipse definition.
     */
    law* law_form();
    /**
     * Arc length.
     * <br><br>
     * <b>Role:</b> Returns the algebraic distance along the curve between the given
     * parameters, the sign being positive if the parameter values are given in increasing
     * order, and negative if they are in decreasing order.
     * <br><br>
     * The result is undefined if either parameter value is outside the parameter
     * range of a bounded curve. For a periodic curve the parameters are not reduced to
     * the principal range, and so the portion of the curve evaluated may include several
     * complete circuits. This function is therefore always a monotonically increasing
     * function of its second argument if the first is held constant, and a decreasing
     * function of its first argument if the second is held constant.
     * <br><br>
     * @param first
     * first parameter.
     * @param second
     * second parameter.
     * @param approx_ok
     * not used for ellipses
     */
    virtual double length(double first, double second, logical approx_ok = TRUE) const;
    /**
     * The inverse of the length function returns the parameter value of the point on the curve at the given algebraic arc length from that defined by the datum parameter.
     * <br><br>
     * <b>Role:</b> The result is not defined for a bounded nonperiodic curve if the
     * datum parameter is outside the parameter range, or if the length is outside the
     * range bounded by the values for the ends of the parameter range.
     * <br><br>
     * @param para
     * datum parameter.
     * @param len
     * arc length.
     * @param approx_ok
     * not used for ellipses
     */
    virtual double length_param(
        double para,
        double	len,
        logical approx_ok = TRUE
    ) const;
    /**
     * Makes a copy of this ellipse on the heap, and return a pointer to it.
     */
    virtual curve* make_copy() const;
    /**
     * Negates this ellipse, negate the normal.
     */
    virtual curve& negate();
    /**
     * Returns an ellipse with the opposite sense from this one.
     */
    ellipse operator-() const;
    /**
     * Tests two curves for equality.
     * <br><br>
     * <b>Role:</b> This test is not guaranteed to say equal for effectively equal curves,
     * but is guaranteed to say not equal if the curves are, in fact, not equal.
     * <br><br>
     * @param name
     * curve name.
     */
    virtual logical operator==(curve const& name) const;
    /**
     * @nodoc
     * Returns this <tt>ellipse</tt> transformed by the given <tt>SPAtransf</tt>.
     */
    friend DECL_KERN ellipse operator*(
        ellipse const&,
        SPAtransf const&
        );
    /**
     * Transforms this <tt>ellipse</tt> by the given <tt>SPAtransf</tt>, in place.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual curve& operator*=(SPAtransf const& trans);
    /**
     * Finds the parameter value at the given point on the ellipse.
     * <br><br>
     * <b>Role:</b> Follow the same procedure as for <tt>point_direction</tt> if the
     * point is not on the ellipse.
     * <br><br>
     * @param name
     * position name.
     * @param cur
     * param curve.
     */
    virtual double param(
        SPAposition const& name,
        SPAparameter const& cur = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the period of the curve parameter.
     */
    virtual double param_period() const;
    /**
     * Returns the parameter range of the ellipse.
     * <br><br>
     * @param box
     * bounding box.
     */
    virtual SPAinterval param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;


    /**
     * Finds the curvature on an ellipse at the given point on the curve.
     * <br><br>
     * <b>Role:</b> If the point is not on the ellipse, the same procedure as for <tt>point_direction</tt>
     * is followed.
     * <br><br>
     * @param posi
     * position.
     * @param para
     * parameter.
     */
    virtual SPAvector point_curvature(
        SPAposition const& posi,
        SPAparameter const& para = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Finds the tangent to an ellipse at the given point on the curve.
     * <br><br>
     * <b>Role:</b> If the point is not on the ellipse, a useful value is returned. In effect,
     * the half-plane bounded by the axis of the ellipse that contains the given point is
     * constructed. The the curvature of the curve where it intersects this half plane is returned.
     * If the point is on the axis, so a half plane cannot be constructed, a zero curvature vector
     * is returned.
     * <br><br>
     * @param posi
     * position.
     * @param para
     * parameter.
     */
    virtual SPAunit_vector point_direction(
        SPAposition const& posi,
        SPAparameter const& para = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to the curve.
     * <br><br>
     * <b>Role:</b> Finds the foot of the perpendicular from the given point to the curve,
     * and tangent to the curve at that point, and its parameter value. If an input
     * parameter value is supplied (as argument 5), the perpendicular found is the one nearest
     * to the supplied parameter position, otherwise it is the one that the curve is nearest to
     * the given point. Any of the return value arguments may be a <tt>NULL</tt> reference, in which case
     * it is ignored.
     * <br><br>
     * @param pt
     * point.
     * @param ft
     * foot.
     * @param vec
     * direction vector.
     * @param cur
     * curvature.
     * @param param_guess
     * param guess.
     * @param act_guess
     * actual guess.
     * @param f_weak
     * weak flag.
     */
    virtual void point_perp(
        SPAposition const& pt,
        SPAposition& ft,
        SPAunit_vector& vec,
        SPAvector& cur,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& act_guess = *(SPAparameter*)NULL_REF,
        logical             f_weak = FALSE
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to the curve.
     * <br><br>
     * <b>Role:</b> Finds the foot of the perpendicular from the given point to the curve,
     * and tangent to the curve at that point, and its parameter value. If an input
     * parameter value is supplied (as argument 5), the perpendicular found is the one nearest
     * to the supplied parameter position, otherwise it is the one that the curve is nearest to
     * the given point. Any of the return value arguments may be a <tt>NULL</tt> reference,
     * in which case it is ignored.
     * <br><br>
     * @param pos
     * point.
     * @param foot
     * foot.
     * @param foot_dt
     * direction.
     * @param guess
     * param guess.
     * @param actual
     * actual guess.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& foot_dt,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical             f_weak = FALSE
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
     * Finds the foot of the perpendicular from the given point to the curve.
     * <br><br>
     * <b>Role:</b> Finds the foot of the perpendicular from the given point to the curve,
     * as well as the curve direction and parameter. If an approximate parameter is
     * given, the perpendicular is the one nearest to that parameter value, otherwise it is
     * nearest to the given point.
     * <br><br>
     * @param pos
     * point.
     * @param foot
     * foot.
     * @param guess
     * param guess.
     * @param actual
     * actual guess.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical             f_weak = FALSE
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
     * Indicates whether a curve is periodic.
     * <br><br>
     * <b>Role:</b> Joins itself smoothly at the ends of its principal parameter range,
     * so that edges may span the seam.
     */
    virtual logical periodic() const;
    /**
     * Restores the data for a ellipse from a save file.
     * <br><br>
     * <b>Role:</b> Switching, for the derived class save, goes through the normal virtual function
     * mechanism. Until restore is invoked there is not an object of the right class. Instead,
     * we switch on a table defined by static instances of the <tt>restore_su_def</tt> class,
     * to invoke a simple friend function that constructs an object of the right
     * (derived) type.
     * <br><br>
     * Then it can call the appropriate member function to do the actual work. The <tt>save_data</tt>
     * and <tt>restore_data</tt> function for each class is called in circumstances
     * when the type of surface is known and there is one available to be filled in.
     * <br><br>
     * <dl>
     * <dt><tt>read_position</tt></dt>
     * <dd>Position of the center of the ellipse.</dd>
     * <dt><tt>read_unit_vector</tt></dt>
     * <dd>Unit vector that is normal to plane of the ellipse.</dd>
     * <dt><tt>read_vector</tt></dt>
     * <dd>major axis of the ellipse.</dd>
     * <dt><tt>read_real</tt></dt>
     * <dd>Ratio of the radii.</dd>
     * <dt><tt>curve::restore_data</tt></dt>
     * <dd>Restore the underlying curve of the ellipse.</dd></dl>
     */
    void restore_data();
    /**
     * @nodoc
     * Restore mechanism.
     */
    friend DECL_KERN curve* restore_ellipse();
    /**
     * Returns a cone bounding the tangent direction of the curve.
     * <br><br>
     * <b>Role:</b> The cone is deemed to have its apex at the origin, and have a given axis
     * direction and (positive) half-angle. If the logical argument is <tt>TRUE</tt>,
     * then a quick approximation may be found. The approximate result may lie wholly
     * within or wholly outside the guaranteed bound (obtained with a <tt>FALSE</tt>
     * argument), but may not cross from inside to outside. Flags in the returned
     * object indicate whether the cone is in fact the best available, and if not whether
     * this result is inside or outside the best cone.
     * <br><br>
     * @param range
     * given range.
     * @param approx
     * approximation ok.
     * @param trans
     * transformation.
     */
    virtual curve_tancone tangent_cone(
        SPAinterval const& range,
        logical           approx = FALSE,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Tests point-on-curve to given precision, returning its parameter value as well if requested.
     * <br><br>
     * @param pt
     * point.
     * @param tol
     * tolerance.
     * @param param_guess
     * param guess.
     * @param act_param
     * actual param.
     */
    virtual logical test_point_tol(
        SPAposition const& pt,
        double              tol = 0,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& act_param = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns an identifier uniquely specifying the curve type.
     */
    virtual int type() const;
    /**
     * Returns a string <tt>"ellipse"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Indicates whether the ellipse is properly defined.
     */
    virtual logical undef() const;
    /**
     * Saves the curve type or id, then calls <tt>save_data</tt>.
     */
    virtual void save() const;
    /**
     * Saves the data for a ellipse to a save file.
     */
    void save_data() const;

#if defined D3_STANDALONE || defined D3_DEBUG
    /**
     * @nodoc
     */
    virtual void input(
        D3_istream&
    );
    /**
     * @nodoc
     */
    virtual void print(
        D3_ostream&
    ) const;

#endif

};

/** @} */
#endif
