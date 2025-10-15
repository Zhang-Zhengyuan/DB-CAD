/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                        intcurve
/*******************************************************************/
#if !defined( intcurve_CLASS )
#define intcurve_CLASS
#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "curdef.hxx"
#include "cur_sur.hxx"
#include "extend.hxx"
#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "debugmsc.hxx"
#include "subtrans.hxx"
#include "transf.hxx"
#include "spldef.hxx"
class surface;
class pcurve;
class SPAbox;
class law;
class curve_interp;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAparameter;
class SPApar_pos;
class SPApar_dir;
class intcur_cache;
class SizeAccumulator;
class int_cur;
class intcurve;
class minimize_helper;
class geometry_definition_processor;

/**
* @file intdef.hxx
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
#define intcurve_type 11
  /**
   * Returns a copy of the transformed curve.
   * <br><br>
   * @param intcur
   * given intcurve.
   * @param trans
   * given transformation.
   */
DECL_KERN intcurve operator*(intcurve const& intcur, SPAtransf const& trans);
/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN subtype_object* restore_int_int_cur();
/**
 * @nodoc
 * Internal use. Restore mechanism
 */
DECL_KERN curve* restore_intcurve();

/**
 * A parametric curve class.
 * <br>
 * <b>Role:</b> An <tt>intcurve</tt> is the general representation of any curve that is
 * not defined by an explicit equation. It may represent a <i>procedural curve</i> that
 * references other geometric entities (for instance, a procedural curve may be defined by
 * the intersection of two surfaces or the projection of a curve onto a surface), or it
 * may represent a NURBS curve.
 * <br><br>
 * The <tt>intcurve</tt> class represents a parametric, object-space curve that maps an
 * interval of the real line (parameter-space) into a 3D real vector space (object-space).
 * This mapping is continuous and one-to-one except possibly at the ends of the
 * interval whose images may coincide. It is differentiable twice and the direction
 * of the first derivative with respect to the parameter must be continuous. This
 * direction is the positive sense of the curve.
 * <br><br>
 * If the two ends of the curve are different in object space, the curve is <i>open</i>.
 * If they are the same, it is <i>closed</i>. If the curve joins itself smoothly, the
 * curve is <i>periodic</i>, and its period is the length of the parameter-space interval
 * that it is primarily defined. A periodic curve is defined for all parameter values
 * by adding a multiple of the period to the parameter value so that the result is
 * within the definition interval, and evaluating the curve at that resultant
 * parameter. The point at the ends of the primary interval is known as the <i>seam</i>.
 * <br><br>
 * The <tt>intcurve</tt> contains a "reversed" bit together with a pointer to another
 * structure, an <tt>int_cur</tt> or something derived from it, that contains the bulk of
 * the information about the <tt>intcurve</tt>.
 * <br><br>
 * Providing this indirection serves two purposes. First, when an <tt>intcurve</tt> is
 * duplicated, the copy simply points to the same <tt>int_cur</tt>, avoiding copying the
 * bulk of the data. The system maintains a <i>use count</i> in each <tt>int_cur</tt>, that allows
 * automatic duplication if a shared <tt>int_cur</tt> is to be modified, and deletes any
 * <tt>int_cur</tt> no longer needed.
 * Second, the <tt>int_cur</tt> contains virtual functions. These virtual functions perform
 * all the operations defined for <tt>intcurve</tt>s that depend on the method of definition
 * of the true curve, so new <tt>intcurve</tt> types can be defined by declaring and
 * implementing derived classes. The <tt>intcurve</tt> and everything using it require no
 * changes to make use of the new definition.
 * <br><br>
 * The base class @href int_cur contains the following information for defining the curve:
 * <ul>
 * <li>A use count indicating the number of times the <tt>int_cur</tt> is referenced.</li>
 * <li>A pointer to a 3D B-spline curve, that typically is an approximation to the
 *  true curve.</li>
 * <li>A fitting tolerance representing the precision of the 3D B-spline curve
 * 	to the true curve. This is 0.0 for an exact fit and greater than 0.0 for
 * 	an approximation.</li>
 * <li>Pointers to two surfaces containing the true curve. Classes derived from
 * 	<tt>int_cur</tt> can use them in different ways but the true curve must lie on
 * 	each of the surfaces. Either or both surface can be <tt>NULL</tt>.</li>
 * <li>A pointer to two 2D parameter-space curves, one on each of the non-<tt>NULL</tt>,
 *  surfaces defined above.</li>
 * </ul>
 * Classes derived from <tt>int_cur</tt> can contain additional information and record the
 * creation method of the true curve.
 * <br><br>
 * Note: Generally applications do not construct an <tt>intcurve</tt> using one of the
 * <tt>intcurve</tt> constructors.  Rather, they are usually constructed as the result
 * of a higher level modeling operation using API functions.
 * @see discontinuity_info, int_cur, INTCURVE
 */
class DECL_KERN intcurve : public curve
{
    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef intcurve_FRIENDS
    intcurve_FRIENDS
#endif

private:
    /**
     * Underlying definition of this curve.
     */
    int_cur* fit;
    /**
     * This curve opposes the direction of the underlying int_cur.
     */
    logical rev;
    /**
     * Discontinuity information.
     * <br><br>
     * <b>Role :</b> If the supporting surfaces of the curve
     * has discontinuities, or if the curve has a default (tangent)
     * extension, then it will have discontinuities. These are stored here.
     * Note that this is a copy of the data stored in the corresponding
     * <tt>int_cur</tt>, but with values outside the subset range removed.  It is
     * necessary to keep a separate copy to provide a read-only array to the
     * data because the curve may be periodic, and the subset may span the
     * periodic joins, resulting in discontinuities which may be outside the
     * periodic range.
     * @see discontinuity_info
    */
    discontinuity_info disc_info;
    /**
     * Update the discontinuity information if a change_event occurs.
     */
    void update_disc_info();
public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    intcurve();
    intcurve(const char* gme);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> This constructor is often used to construct an "exact" B-spline curve from a <tt>bs3_curve</tt>, or an object-space
     * curve that is based upon a object-space surface and a parameter-space curve. In the latter case, in which the parameter-
     * space curve serves at the primary representation of curve, <tt>parametric_is_primary</tt> should be set to <tt>TRUE</tt>;
     * for all other cases it is <tt>FALSE</tt>.
     * <br><br>
     * @param cur
     * bs3_curve; intcurve takes ownership.
     * @param tol
     * tolerance.
     * @param surf1
     * first surface; intcurve copies, caller keeps ownership.
     * @param surf2
     * second surface; intcurve copies, caller keeps ownership.
     * @param cur1
     * first bs2_curve; intcurve takes ownership.
     * @param cur2
     * second bs2_curve; intcurve takes ownership.
     * @param inter
     * interval.
     * @param parametric_is_primary
     * parameter-space representation is primary.
     */
    intcurve(
        bs3_curve cur,
        double tol,
        surface const& surf1,
        surface const& surf2,
        bs2_curve cur1 = NULL,
        bs2_curve cur2 = NULL,
        const SPAinterval& inter = *(const class SPAinterval*)NULL_REF,
        logical parametric_is_primary = FALSE,
        logical make_tight_approx = FALSE
    );
    /**
     * C++ interpolation constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> The <tt>curve_interp</tt> object contains all the data and methods needed
     * to do the interpolation. In addition, a bounding box is supplied within which the fit
     * tolerance must be met.
     * <br><br>
     * @param cur
     * curve interpolation object.
     * @param box
     * region of interest.
     */
    intcurve(
        curve_interp& cur,
        SPAbox const& box = *(const class SPAbox*)NULL_REF,
        logical make_tight_approx = FALSE
    );
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> This creates an intcurve object when the underlying <tt>int_cur</tt> has
     * been constructed. The <tt>int_cur</tt> is a derived type which does not need a curve
     * fit or uses a non-standard one.
     * <br><br>
     * @param intcur
     * defining int_cur.
     */
    intcurve(
        int_cur* intcur,
        logical make_tight_approx = FALSE
    );
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param intcur
     * given intcurve.
     */
    intcurve(
        intcurve const& intcur
    );
    intcurve(const char* gme, intcurve const& intcur);
    /**
     * C++ destructor, deleting an intcurve.
     * <br><br>
     * <b>Role:</b> Deletes an intcurve by manipulating the use count of the
     * underlying int_cur structure.
     */
    virtual ~intcurve(); // Ensure any derived class destructor gets
    // a say when we destroy a surface.
/**
 * Evaluates accurate derivatives.
 * <br><br>
 * <b>Role:</b> Returns the number of derivatives that <tt>evaluate</tt> finds accurately and directly,
 * rather than by finite differencing, over the given portion of the curve. If there is no limit to
 * the number of accurate derivatives, this method returns the value <tt>ALL_CURVE_DERIVATIVES</tt>.
 * <br><br>
 * @param inter
 * region of interest. Defaults to the whole curve is not specified.
 */
    virtual int accurate_derivs(
        SPAinterval const& inter = *(SPAinterval*)NULL_REF
    ) const;
    /**
     * Returns, in a read-only array, the number and parameter values of discontinuities of the curve up to the given order (maximum three).
     * <br><br>
     * @param n_discont
     * number of discontinuities.
     * @param order
     * maximum order of discontinuity.
     */
    virtual	const double* all_discontinuities(int& n_discont, int order);
    /**
     * Returns the maximum error between the approximate evaluation of this curve and the true evaluation of it.
     * <br><br>
     * <b>Role:</b> Returns a distance value, which represents the greatest discrepancy
     * between positions calculated by calls to <tt>eval</tt> or <tt>eval_position</tt> with the
     * approximate results obtained by setting the <tt>approx</tt> argument to <tt>TRUE</tt> or <tt>FALSE</tt>.
     */
    virtual double approx_error() const;
    /**
     * Returns a box enclosing the portion of the curve between the two given points on the curve.
     * <br><br>
     * @param pos1
     * first position.
     * @param pos2
     * second position.
     * @param trans
     * optional transformation.
     */
    virtual SPAbox bound(
        SPAposition const& pos1,
        SPAposition const& pos2,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box enclosing the specified interval the curve.
     * <br><br>
     * @param inter
     * specified interval.
     * @param trans
     * optional transformation.
     */
    virtual SPAbox bound(
        SPAinterval const& inter,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box surrounding that portion of the curve within the given box.
     * <br><br>
     * @param box
     * region of interest.
     * @param trans
     * optional transformation.
     */
    virtual SPAbox bound(
        SPAbox const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box enclosing the portion of the curve defined between the two given parameter values.
     * <br><br>
     * @param start
     * first parameter value.
     * @param end
     * second parameter value.
     * @param t
     * optional transformation.
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
     * Checks if it is known that there is a good <tt>bs1_curve</tt> hull.
     * <br><br>
     * <b>Role :</b> Returns 1 if the <tt>bs1_curve</tt> hull turning angles are known to be
     * acceptable, 0 if they are not acceptable, and -1 if unknown.
     */
    int bs1_hull_angles_ok() const;
    /**
     * Checks if it is known that the <tt>bs1_curve</tt> self-intersects.
     * <br><br>
     * <b>Role :</b>
     * Returns 1 if the <tt>bs1_curve</tt> hull is known to self-intersect, 0 if it does not, and -1 if unknown.
     */
    int bs1_hull_self_intersects() const;
    /**
     * Checks if it is known that the knots of the <tt>bs1_curve</tt> lie on the curve.
     * <br><br>
     * <b>Role :</b> Returns 1 if the <tt>bs1_curve</tt> knots are known to lie on the curve,
     * 0 if they do not, and -1 if unknown.
     */
    int bs1_knots_on_curve() const;
    /**
     * Checks if it is known that there is a good <tt>bs2_curve</tt> hull.
     * <br><br>
     * <b>Role :</b> Returns 1 if the <tt>bs2_curve</tt> hull turning angles are known to be acceptable,
     * 0 if they are not acceptable, and -1 if unknown.
     */
    int bs2_hull_angles_ok() const;
    /**
     * Checks if the <tt>bs2_curve</tt> self-intersects.
     * <br><br>
     * <b>Role :</b>
     * Returns 1 if the <tt>bs2_curve</tt> hull is known to self-intersect, 0 if it does not, and -1 if unknown.
     */
    int bs2_hull_self_intersects() const;
    /**
     * Checks if it is known that the knots of the <tt>bs2_curve</tt> lie on the curve.
     * <br><br>
     * <b>Role :</b>
     * Returns 1 if the <tt>bs2_curve</tt> knots are known to lie on the curve,
     * 0 if they do not, and -1 if unknown.
     */
    int bs2_knots_on_curve() const;
    /**
     * Notifies the derived type that the curve has been changed so that it can update itself.
     */
    virtual	void	change_event();
    /**
     * Checks for any data errors in the curve, and corrects the errors if possible.
     * <br><br>
     * <b>Role:</b> The various arguments provide control over which checks are made,
     * which fixes can be applied and which fixes were actually applied. The function
     * returns a list of errors that remain in the curve on exit.
     * <br><br>
     * The default for the set of flags which say which fixes are allowable is none
     * (nothing is fixed). If the list of checks to be made is null, then every possible
     * check will be made. Otherwise, the function will only check for things in the
     * list. The return value for the function will then be a subset of this list.
     * <br><br>
     * @param input
     * flags for allowed fixes.
     * @param result
     * fixes applied.
     * @param list
     * checks to be made, default is none.
     */
    virtual	check_status_list* check(
        const                    check_fix& input = *(const check_fix*)NULL_REF,
        check_fix& result = *(check_fix*)NULL_REF,
        const check_status_list* list = (const check_status_list*)NULL_REF
    );
    /**
     * Indicates if an intcurve is closed.
     * <br><br>
     * <b>Role:</b> A closed curve joins itself (smoothly or not) at the ends of its
     * principal parameter range. This method always returns <tt>TRUE</tt> if
     * <tt>periodic</tt> returns <tt>TRUE</tt>.
     */
    virtual logical closed() const;
    /**
     * Finds the closest point on the curve (the foot) to the given point, and optionally its parameter value.
     * <br><br>
     * <b>Role:</b> If an input parameter value is supplied (as the first parameter
     * argument), the foot found is only a local solution nearest to the supplied
     * parameter position. Any of the return value arguments may be a <tt>NULL</tt> reference,
     * in which case it is simply ignored.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * foot position.
     * @param param_guess
     * input guess value of param.
     * @param param_actual
     * actual value of param.
     */
    virtual void closest_point(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the underlying <tt>bs3_curve</tt>.
     * <br><br>
     * <b>Role:</b> It returns <tt>NULL</tt> if there is no <tt>int_cur</tt>.
     * The default value of -1.0 for the fit tolerance instructs the
     * <tt>make_approx</tt> function to use whatever value it thinks is appropriate.
     * <br><br>
     * @param tol
     * fit tolerance.
     * @param force
     * if <tt>TRUE</tt>, method will return a <tt>bs3_curve</tt> even if it self-intersects
     */
    bs3_curve cur(double tol = -1.0, logical force = FALSE) const;


    /**
    * @nodoc
    * Returns the tight bs3 approximation
    */
    bs3_curve tight_bs3() const;

    /**
     * @nodoc
     * Returns the tight bs3 approximation tolerance
     */
    double tight_tol() const;
    /**
     * @nodoc
     * Extends the tight bs3 approximation, if it exists
     */
    logical extend_tight_approx();
    /**
     * @nodoc
     * Extends the tight bs3 approximation, if it exists
     */
    logical extend_tight_approx_incremental(const SPAinterval& new_range);
    /**
     * @nodoc
     * make_tight_approx
     *
     */
    logical make_tight_approx();
    /**
     * @nodoc
     * make_tight_approx
     *
     */
    logical delete_tight_approx();
    /**
     * @nodoc
     * activate_tight_approx
     *
     */
    logical activate_tight_approx();
    /**
     * @nodoc
     * deactivate_tight_approx
     *
     */
    logical deactivate_tight_approx();
    /**
     * Returns <tt>TRUE</tt> if the underlying <tt>int_cur</tt> contains a <tt>bs3_curve</tt>.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if the defining <tt>int_cur</tt> has a non-<tt>NULL</tt> <tt>bs3_curve</tt>, <tt>FALSE</tt> otherwise.
     */
    logical	cur_present() const;
    /**
     * Outputs a title line and the details of the <tt>intcurve</tt> for inspection to standard output or to the specified file.
     * <br><br>
     * @param title
     * title line.
     * @param fp
     * file pointer.
     */
    virtual void debug(
        char const* title,
        FILE* fp = debug_file_ptr
    ) const;
    /**
     * @nodoc
     * Force recalculate discontinuity info.
     * <br><br>
     * <b>Role :</b>
     * Do a recalculation for the <tt>int_cur</tt> and for any linked geometries
     * upon which the recalculation depends, then update the current
     * object with the result.  Return <tt>FALSE</tt> if no calculations were done
     * by the <tt>int_cur</tt> for any reason.
     */
    virtual logical deep_calc_disc_info();
    /**
     * Creates a copy of an <tt>intcurve</tt> that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers.
     * Returns a pointer to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual curve* deep_copy(pointer_map* pm = NULL) const;
    /**
     * Returns the number and parameter values of discontinuities of the curve in a read-only array of the given order (maximum three).
     * <br><br>
     * @param n_discont
     * number of discontinuities.
     * @param order
     * order of the discontinuities.
     */
    virtual	const double* discontinuities(int& n_discont, int order) const;
    /**
     * Determines whether a particular parameter value is a discontinuity.
     * <br><br>
     * <b>Role:</b> If there is a discontinuity at the given parameter value,
     * returns the order of the discontinuity. Otherwise, returns 0.
     * <br><br>
     * @param t
     * given parameter value.
     */
    virtual	int	discontinuous_at(double t) const;
    /**
     * Returns a cylinder that encloses the portion of the curve bounded by the interval.
     * <br><br>
     * @param inter
     * region of interest.
     */
    virtual BOUNDING_CYLINDER enclosing_cylinder(const SPAinterval& inter =
        *(SPAinterval*)NULL_REF) const;
    /**
     * Evaluates the curve, giving the position and the first and second derivatives.
     * <br><br>
     * @param value
     * given parameter value.
     * @param pos
     * position.
     * @param der1
     * first derivative.
     * @param der2
     * second derivative.
     * @param repeat
     * perform a repeat evaluation on a curve whose underlying geometry data has not changed?
     * @param approx
     * approx results OK?
     */
    virtual void eval(
        double value,
        SPAposition& pos,
        SPAvector& der1 = *(SPAvector*)NULL_REF,
        SPAvector& der2 = *(SPAvector*)NULL_REF,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Finds the curvature at given parameter value.
     * <br><br>
     * @param value
     * given parameter value.
     * @param repeat
     * perform a repeat evaluation on a curve whose underlying geometry data has not changed?
     * @param approx
     * approx results OK?
     */
    virtual SPAvector eval_curvature(
        double value,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Finds the parametric derivative, magnitude, and direction, at given parameter value.
     * <br><br>
     * @param value
     * given parameter value.
     * @param repeat
     * perform a repeat evaluation on a curve whose underlying geometry data has not changed?
     * @param approx
     * approx results OK?
     */
    virtual SPAvector eval_deriv(
        double value,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Finds the tangent direction at the given parameter value on the curve.
     * <br><br>
     * @param value
     * given parameter value.
     * @param tangency
     * determine tangency.
     * @param approx
     * approx results OK?
     */
    virtual SPAunit_vector eval_direction(
        double value,
        logical tangency = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Finds the position on a curve at the given parameter value.
     * <br><br>
     * @param value
     * given parameter value.
     * @param repeat
     * perform a repeat evaluation on a curve whose underlying geometry data has not changed?
     * @param approx
     * approx results OK?
     */
    virtual SPAposition eval_position(
        double value,
        logical repeat = FALSE,
        logical approx = FALSE
    ) const;
    /**
     * Evaluates the position and the first and second derivatives at given parameter value.
     * <br><br>
     * @param param
     * given parameter value.
     * @param pt
     * point.
     * @param vec
     * array of pointers to vectors.
     * @param der
     * number of derivatives required.
     * @param location
     * the evaluation location - above, below or don't care.
     */
    virtual int evaluate(
        double              param,
        SPAposition& pt,
        SPAvector** vec = NULL,
        int                 der = 0,
        evaluate_curve_side location = evaluate_curve_unknown
    ) const;
    /**
     * Evaluates the position and the first and second derivatives at given parameter value.
     * <br><br>
     * <b>Role:</b> The <tt>evaluate_iter</tt> function is just like <tt>evaluate</tt>, but is
     * supplied with a data object which contains results from a previous close evaluation, for
     * use as initial values for any iteration involved.
     * <br><br>
     * @param param
     * given parameter value.
     * @param data
     * Data supplying initial values, and set to reflect the results of this evaluation.
     * @param pt
     * Point on curve at given parameter.
     * @param vec
     * array of pointers to vectors.
     * @param deriv
     * number of derivatives required.
     * @param location
     * the evaluation location - above, below or don't care.
     */
    virtual int evaluate_iter(
        double              param,
        curve_evaldata* data,
        SPAposition& pt,
        SPAvector** vec = NULL,
        int                 deriv = 0,
        evaluate_curve_side location = evaluate_curve_unknown
    ) const;
    /**
     * Finds the extrema of an intcurve in a given direction.
     * <br><br>
     * <b>Role:</b> This method ignores its ends unless it is closed. The default
     * version uses the corresponding function for the approximating B-spline.
     * <br><br>
     * @param dir
     * given direction.
     */
    virtual curve_extremum* find_extrema(
        SPAunit_vector const& dir
    ) const;
    /**
     * Returns the fit tolerance.
     * <br><br>
     * <b>Role:</b> It returns 0 if there is no precise <tt>int_cur</tt>.
     */
    double fitol() const;
    /**
     * @nodoc
     * Internal use. Roll mechanism.
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;

    /**
     * @nodoc
     */
    virtual void minimize(minimize_helper*);

    /**
     * Returns read-only access to a <tt>discontinuity_info</tt> object, if there is one.
     * <br><br>
     * <b>Role:</b> The default version of the function returns <tt>NULL</tt>.
     */
    virtual const discontinuity_info& get_disc_info() const;

    /**
     * @nodoc
     */
    int_cur const& get_int_cur() const
    {
        return *fit;
    }

    /**
     * Replaces the defining spline curve and should only be used with extreme caution.
     */
    void replace_int_cur(int_cur& new_fit);

    /**
     * Checks if it is known that the <tt>bs1_curve</tt> hull encloses the curve.
     * <br><br>
     * <b>Role :</b>
     * Returns 1 if the <tt>bs1_curve</tt> hull is known to enclose the curve, 0 if it does not, and -1 if unknown.
     */
    int hull1_enclosure() const;
    /**
     * Check if it is known that the <tt>bs2_curve</tt> hull encloses the curve.
     * <br><br>
     * <b>Role :</b>
     * Returns 1 if the <tt>bs2_curve</tt> hull is known to enclose the curve, 0 if it does not, and -1 if unknown.
     */
    int hull2_enclosure() const;
    /**
     * Joins two pieces of <tt>intcurve</tt> together, adding a discontinuity of the given order at the join (if order is not supplied, or is nonpositive, then it is calculated).
     * <br><br>
     * <b>Role:</b> The curves must be suitable for joining. The following conditions must be satisfied:
     * <br><br>
     * <ol>
     * <li>The start of the second curve must match the end of `this'.</li>
     * <li>The underlying <tt>int_cur</tt>s must have the same type.</li>
     * <li>The first curve must be unlimited above, and the second curve unlimited below.</li>
     * </ol>
     * <br><br>
     * If any of these conditions are violated, the function returns <tt>FALSE</tt> and the
     * curves are unchanged.
     * <br><br>
     * The other ends may also match, in which case they will also be joined and the
     * final curve will be periodic. Join will be C1.
     * <br><br>
     * If the second curve is <tt>NULL</tt>, then the curve is made periodic if it was
     * closed, providing the join ic C1.
     * <br><br>
     * @param second
     * intcurve.
     * @param order
     * discontinuity order.
     */
    logical join(intcurve& second, int order = -1);
    /**
     * Returns the law form for an <tt>intcurve</tt>.
     */
    law* law_form();
    /**
     * Returns the algebraic distance along the curve between the given parameters.
     * <br><br>
     * <b>Role:</b> The value is positive if the parameter values are given in increasing
     * order and negative if they are in decreasing order. The result is undefined if
     * either parameter value is outside the parameter range of a bounded curve. For a
     * periodic curve, the parameters are not reduced to the principal range, and so
     * the portion of the curve evaluated may include several complete circuits.
     * This method is always a monotonically increasing function of its second argument
     * if the first is held constant, and a decreasing function of its first argument
     * if the second is held constant.
     * <br><br>
     * @param param1
     * first parameter.
     * @param param2
     * second parameter.
     * @param approx_ok
     * If approx_ok is <tt>TRUE</tt>, the B-spline approximation is used in the length calculation.
     */
    virtual double length(
        double param1,
        double param2,
        logical approx_ok = TRUE
    ) const;
    /**
     * Returns the parameter value of the point on the curve at the given algebraic arc length from that defined by the datum parameter.
     * <br><br>
     * <b>Role:</b> The result is not defined for a bounded nonperiodic curve if the
     * datum parameter is outside the parameter range, or if the length is outside
     * the range bounded by the values for the ends of the parameter range.
     * <br><br>
     * @param datum
     * datum parameter.
     * @param arc
     * arc length.
     * @param approx_ok
     * If approx_ok is <tt>TRUE</tt>, the B-spline approximation is used in the length calculation.
     */
    virtual double length_param(
        double datum,
        double arc,
        logical approx_ok = TRUE
    ) const;
    /**
     * Virtual function to copy a curve without knowing what its type is.
     */
    virtual curve* make_copy() const;
    /**
     * Constructs a data object to retain evaluation information across calls to <tt>evaluate_iter</tt>.
     * <br><br>
     * <b>Role:</b> This is to allow subsidiary calls within an iterative evaluator
     * to start iteration much closer to the required result than is possible just
     * using the curve information itself.
     */
    virtual curve_evaldata* make_evaldata() const;
    /**
     * Ensures that the reference supplied points to a singly-used record.
     * <br><br>
     * <b>Role:</b> Takes no action if it is already single, otherwise copies everything.
     */
    void make_single_ref();
    /**
     * Negates an <tt>intcurve</tt> in place.
     * <br><br>
     * <b>Role:</b> This is a curve virtual function, which is why it returns a <tt>curve&</tt>
     * instead of an <tt>intcurve&</tt>.
     */
    virtual curve& negate();
    /**
     * Negates the curve.
     */
    intcurve operator-() const;
    /**
     * Assignment operator, which copies only the intcurve record and adjusts the use counts of the underlying information.
     * <br><br>
     * @param intcur
     * intcurve.
     */
    intcurve& operator=(
        intcurve const& intcur
        );
    /**
     * Tests two curves for equality.
     * <br><br>
     * <b>Role:</b> This method does not guarantee to say "equal" for
     * effectively-equal curves, but it is guaranteed to say "not equal" if they are
     * indeed not equal. Use the result for optimization, but not where it really
     * matters. The default always says "not equal."
     * <br><br>
     * @param intcur
     * intcurve.
     */
    virtual logical operator==(curve const& intcur) const;
    /**
     * Determines whether a <tt>intcurve</tt> is entirely enclosed within another.
     */
    virtual logical operator>>(curve const& intcur) const;
    /**
     * @nodoc
     * Returns a copy of this <tt>intcurve</tt> transformed by the given <tt>SPAtransf</tt>.
     */
    friend DECL_KERN intcurve operator*(
        intcurve const&,
        SPAtransf const&
        );
    /**
     * Transforms this <tt>intcurve</tt> by the given <tt>SPAtransf</tt>, in place.
     * <br><br>
     * <b>Role:</b> This is complicated by the effort to maintain sharing when several
     * <tt>intcurve</tt>s sharing the same <tt>int_cur</tt> are transformed successively with the same
     * transformation. A list of transformed versions of each <tt>int_cur</tt> is maintained,
     * and this method searches for a match before making a new one.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual curve& operator*=(
        SPAtransf const& trans
        );
    /**
     * Returns the parameter value for a given point.
     * <br><br>
     * @param pos
     * given position.
     * @param param
     * guess parameter value.
     */
    virtual double param(
        SPAposition const& pos,
        SPAparameter const& param = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the period of a periodic curve.
     * <br><br>
     * <b>Role:</b> It returns 0 if the curve is not periodic.
     */
    virtual double param_period() const;
    /**
     * Returns the parameter range of the intcurve.
     * <br><br>
     * @param box
     * region of interest.
     */
    virtual SPAinterval param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the parameter space tolerance of the first parameter space curve.
     */
    double partol1() const;
    /**
     * Returns the parameter space tolerance of the second parameter space curve.
     */
    double partol2() const;
    /**
     * Indicates if the curve is periodic.
     * <br><br>
     * <b>Role:</b> A periodic curve joins itself smoothly with matching derivatives
     * at the ends of its principal parameter range so that edges may span the seam.
     */
    virtual logical periodic() const;
    /**
     * Returns the parameter space curves with respect to the surfaces defining this <tt>intcurve</tt>.
     * <br><br>
     * <b>Role:</b> <tt>index</tt> may be 1 or 2, representing the two surfaces in order, or -1
     * or -2 meaning the negation of those <tt>pcurve</tt>s.
     * <br><br>
     * This method constructs a new <tt>pcurve</tt> on the heap and it is the responsibility of the caller
     * to provide for the destruction of this <tt>pcurve</tt>.
     * <br><br>
     * @param index
     * surface index.
     * @param temporary
     * flag indicating if the pcurve returned is temporary.
     */
    virtual pcurve* pcur(
        int index,
        logical temporary = FALSE
    ) const;
    /**
     * Returns <tt>TRUE</tt> if the n<sup>th</sup> parameter-space curve is defined.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if the n<sup>th</sup> parameter-space curve is defined
     * (i.e., <tt>pcur</tt> returns a non-<tt>NULL</tt> pcurve pointer); otherwise, it returns <tt>FALSE</tt>.
     * <br><br>
     * @param nparam
     * n<sup>th</sup> parameter-space curve.
     */
    virtual logical pcur_present(
        int nparam
    ) const;

    /**
     * Returns a curve in parameter space of the surface returned by <tt>surf1</tt>, if the surface is parametric.
     * <br><br>
     * <b>Role:</b> Returns <tt>NULL</tt> if the surface (as returned by <tt>surf1</tt>)
     * is <tt>NULL</tt> or not parametric.
     * <br><br>
     * @param force
     * force surface return.
     */
    bs2_curve pcur1(logical force = FALSE) const;
    /**
     * Returns a curve in parameter space of the surface returned by <tt>surf2</tt>, if the surface is parametric.
     * <br><br>
     * <b>Role:</b> Returns <tt>NULL</tt> if the surface (as returned by <tt>surf2</tt>)
     * is <tt>NULL</tt> or not parametric.
     * <br><br>
     * @param force
     * force surface return.
     */
    bs2_curve pcur2(logical force = FALSE) const;
    /**
     * Finds the curvature at a point on the <tt>intcurve</tt>.
     * <br><br>
     * @param pt
     * given point.
     * @param guess
     * param guess.
     */
    virtual SPAvector point_curvature(
        SPAposition const& pt,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Finds the tangent direction of the <tt>intcurve</tt> at a point on the <tt>intcurve</tt>.
     * <br><br>
     * @param pt
     * given point.
     * @param guess
     * param guess.
     */
    virtual SPAunit_vector point_direction(
        SPAposition const& pt,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to the curve and the curve tangent direction and curvature at that point and its parameter value.
     * <br><br>
     * <b>Role:</b> If an input parameter value is supplied as the guess value, the
     * perpendicular found is the one nearest to the supplied parameter position;
     * otherwise, it is the one at which the curve is nearest to the given point.
     * Any of the return value arguments may be <tt>NULL</tt> reference, in which case it is
     * simply ignored.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * returned foot.
     * @param tangent
     * returned tangent.
     * @param cur
     * returned curvature.
     * @param guess
     * guess parameter value.
     * @param actual
     * returned actual parameter value.
     * @param f_weak
     * weak flag.
     */
    virtual void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& tangent,
        SPAvector& cur,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to the curve and the tangent direction to the curve at that point and its parameter value.
     * <br><br>
     * <b>Role:</b> If an input parameter value is supplied as the guess value,
     * the perpendicular found is the one nearest to the supplied parameter position;
     * otherwise, it is the one at which the curve is nearest to the given point.
     * Any of the return value arguments may be <tt>NULL</tt> reference, in which case it is
     * simply ignored.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * returned foot.
     * @param tangent
     * the returned tangent.
     * @param guess
     * guess parameter value.
     * @param actual
     * returned actual parameter value.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& tangent,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to the curve and its parameter value.
     * <br><br>
     * <b>Role:</b> If an input parameter value is supplied as the guess value, the
     * perpendicular found is the one nearest to the supplied parameter position;
     * otherwise, it is the one at which the curve is nearest to the given point.
     * Any of the return value arguments may be a <tt>NULL</tt> reference, in which case it
     * is simply ignored.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * returned foot.
     * @param guess
     * guess parameter value.
     * @param actual
     * returned actual parameter value.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical            f_weak = FALSE
    ) const;
    /**
     * Reparameterizes the curve to start and end at the given parameter values, which are in increasing order.
     * <br><br>
     * @param start
     * start parameter value.
     * @param end
     * end parameter value.
     */
    void reparam(
        double start,
        double end
    );
    /**
     * @nodoc
     * Internal use. Restore mechanism.
     */
    friend DECL_KERN curve* restore_intcurve();
    /**
     * Restores the data from a save file.
     * <br><br>
     * <b>Role:</b> The restore operation switches on a table defined by static instances
     * of the <tt>restore_cu_def</tt> class. This invokes a simple friend function which
     * constructs an object of the right derived type. Then it calls the appropriate
     * base class member function to do the actual work. The <tt>restore_data</tt> function
     * for each class can be called in circumstances when it is known what type of
     * surface is to be expected and a surface of that type is on hand to be filled in.
     * <br><pre>
     * read_logical			Curve direction either "forward" or "reversed".
     * if (restore_version_number &lt; INTCURVE_VERSION)
     * 	// Restore as a surface-surface intersection object. The
     * 	// restore function for int_int_cur handles the possibility
     * 	// that it is in fact an exact_int_cur or a surf_int_cur.
     * 	subtype_object * dispatch_restore_subtype
     *
     * //Called with "cur" and "surfintcur". Restore just the data associated with
     * //                                    that type of curve. In earlier versions,
     * //                                    there was only one type of int_cur,
     * //                                    which covered what is now "exact",
     * //                                    "surf", and "int". There was no ID.
     * else
     * 	// Switch to the right restore routine, using the standard
     * 	// system mechanism. Note that the argument is to enable
     * 	// the reader to distinguish old-style types where "exact"
     * 	// was both an int_cur and a spl_sur. They are now "exactcur"
     * 	// and "exactsur".
     * 	subtype_object * dispatch_restore_subtype
     * //                                    Called with "cur"
     * //                                    Restore the underlying curve. Generic curve data.
     * </pre>
     */
    void restore_data();
    /**
     * Returns <tt>TRUE</tt> if the <tt>intcurve</tt> is reversed with respect to the underlying <tt>int_cur</tt>.
     */
    logical reversed() const
    {
        return rev;
    }
    /**
     * Returns the safe range or an empty interval if there is no <tt>int_cur</tt>.
     */
    SPAinterval safe_range() const;
    /**
     * Saves the curve type or id, then calls <tt>save_data</tt>.
     */
    virtual void save() const;
    /**
     * @nodoc
     */
    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    /**
     * Saves the <tt>intcurve</tt> data to a save file.
     */
    void save_data() const;
    /**
     * Sets the property in the underlying <tt>int_cur</tt>, of the <tt>bs2_curve</tt> hull not turning too sharply.
     * <br><br>
     * <b>Role:</b> The first argument should be 1 or 2 to indicate the <tt>bs2_curve</tt>
     * which the enclosure is being set for, and the second integer argument should
     * be -1, 0 or 1, to indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     * <br><br>
     * @param pcu_no
     * enclosure.
     * @param hull_angles_ok
     * unknown, false or true.
     */
    void set_bs_hull_angles_ok(int pcu_no, int hull_angles_ok);
    /**
     * Sets the property in the underlying <tt>int_cur</tt>, of the <tt>bs2_curve</tt> hull self-intersecting (or not).
     * <br><br>
     * <b>Role:</b> The first argument should be 1 or 2, to indicate the <tt>bs2_curve</tt>
     * which the enclosure is being set for, and the second integer argument should
     * be -1, 0 or 1, to indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     * <br><br>
     * @param pcu_no
     * enclosure.
     * @param hull_self_ints
     * unknown, false or true.
     */
    void set_bs_hull_self_intersects(int pcu_no, int hull_self_ints);

    /**
     * Sets the property in the underlying <tt>int_cur</tt>, of the <tt>bs2_curve</tt> of whether the knots lie on the intcurve.
     * <br><br>
     * <b>Role:</b> The first argument should be 1 or 2, to indicate the <tt>bs2_curve</tt>
     * which the enclosure is being set for, and the second integer argument should
     * be -1, 0 or 1, to indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     * <br><br>
     * @param pcu_no
     * enclosure.
     * @param knots_on_cu
     * unknown, false or true.
     */
    void set_bs_knots_on_curve(int pcu_no, int knots_on_cu);
    /**
     * Replaces the underlying <tt>bs3_curve</tt> approximation.
     * <br><br>
     * <b>Role:</b> Replaces the internal approximation with the one provided.
     * If the supplied tolerance is negative, then it will be left unchanged.
     * <br><br>
     * @param cur
     * given bs3_curve.
     * @param tol
     * tolerance.
     * @param delete_old_bs3
     * Whether to delete the old <tt>bs3_curve</tt> or not.
     * @param allow_zero_fitol
     * Whether to allow setting the fitol to zero.
     */
    void set_cur(bs3_curve cur, double tol = -1.0,
        logical delete_old_bs3 = TRUE, logical allow_zero_fitol = FALSE);
    /**
     * Sets the curve enclosure in the underlying <tt>int_cur</tt>.
     * <br><br>
     * <b>Role:</b> The first argument should be 1 or 2, to indicate the <tt>bs2_curve</tt>
     * which the enclosure is being set for, and the second integer argument should
     * be -1, 0 or 1, to give the value for the hull enclosure.
     * <br><br>
     * @param pcu_no
     * enclosure.
     * @param encl
     * value.
     */
    void set_hull_enclosure(int pcu_no, int encl);
    /**
     * Marks an <tt>intcurve</tt> as periodic.
     * <br><br>
     * <b>Role:</b> This is used after splitting a periodic intcurve to restore the
     * periodic status that the split changed to closed.
     */
    void set_periodic();
    /**
     * Divides an <tt>intcurve</tt> into two pieces at a parameter value.
     * <br><br>
     * <b>Role:</b> This method creates a new <tt>intcurve</tt> on the heap, but either one
     * of the <tt>intcurve</tt>s may have a <tt>NULL</tt> actual curve. After calling <tt>split</tt>,
     * clients must call the <tt>undef</tt> method on the input curve and on the returned curve;
     * if <tt>undef</tt> returns <tt>TRUE</tt>, the corresponding piece is undefined and must
     * be deleted. The supplied curve is modified
     * to be the latter section and the initial section is returned as a value.
     * <br><br>
     * @param param
     * param value.
     * @param pos
     * exact position.
     */
    virtual curve* split(
        double param,
        SPAposition const& pos = *(SPAposition*)NULL_REF
    );
    /**
     * Constructs a new curve, which is a copy of the portion of the given one within the specified parameter bounds.
     * <br><br>
     * This method constructs a new <tt>intcurve</tt> on the heap and it is the responsibility of the caller
     * to provide for the destruction of this <tt>intcurve</tt>.
     * <br><br>
     * @param inter
     * specified interval.
     */
    curve* subset(
        SPAinterval const& inter
    ) const;
    /**
     * Returns the first surface supporting the curve.
     * <br><br>
     * <b>Role:</b> By default, surfaces are only returned if the true curve lies on
     * the surface. Surfaces defining the curve but distant from it are not returned.
     * To force the surface to be returned regardless, the logical flag should be
     * set to <tt>TRUE</tt>.
     * <br><br>
     * @param force
     * force surface to be returned.
     */
    surface const& surf1(logical force = FALSE) const;
    /**
     * Returns the second surface supporting the curve.
     * <br><br>
     * <b>Role:</b> By default, surfaces are only returned if the true curve lies on
     * the surface. Surfaces defining the curve but distant from it are not returned.
     * To force the surface to be returned regardless, the logical flag should be
     * set to <tt>TRUE</tt>.
     * <br><br>
     * @param force
     * force surface to be returned.
     */
    surface const& surf2(logical force = FALSE) const;
    /**
     * Returns a cone bounding the tangent direction of a curve.
     * <br><br>
     * <b>Role:</b> The cone has its apex at the origin and a given axis direction
     * and (positive) half-angle. If <tt>approx_OK</tt> is <tt>TRUE</tt>, then a quick approximation
     * may be found. The approximate result may lie wholly within or wholly outside
     * the guaranteed bound (obtained with a <tt>FALSE</tt> argument), but it may not cross
     * from inside to outside. Flags in the returned object indicate whether the cone
     * is in fact the best available, and if this result is inside or outside the
     * best cone.
     * <br><br>
     * @param range
     * interval.
     * @param appox_OK
     * approx results OK?
     * @param t
     * transformation.
     */
    curve_tancone tangent_cone(
        SPAinterval const& range,
        logical           approx_OK,
        SPAtransf const& t = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Tests if a given point lies on the curve to a given precision.
     * <br><br>
     * <b>Role:</b> The default version uses the corresponding function for the
     * approximating B-spline, to a tolerance expanded using the fit tolerance, and
     * then tests the perpendicular to the true curve. It is suitable for most
     * derived classes.
     * <br><br>
     * @param pt
     * given point.
     * @param tol
     * given tolerance.
     * @param guess
     * parameter guess.
     * @param actual
     * actual parameter.
     */
    virtual logical test_point_tol(
        SPAposition const& pt,
        double             tol = 0,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Returns the type of <tt>intcurve</tt>.
     */
    virtual int type() const;
    /**
     * Returns the string <tt>"intcurve"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Indicates if the curve is defined or undefined.
     */
    virtual logical undef() const;


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
public:
    // get and set functions for access.
    int_cur* gme_get_fit();
    void gme_set_rev(int data);
    int_cur** gme_get_fit_ptr();
};
/**
 * @nodoc
 * This class contains 'summary data', enabling the approximating curve to be
 * stored concisely, and regenerated quickly.
 */
class DECL_KERN summary_bs3_curve : public ACIS_OBJECT {
    friend class int_cur;
    friend class blend_spl_sur;

    int          nknots;
    double* _knots;
    int deg;

    summary_bs3_curve(int nk, double* kn, const int dg = 3);

    summary_bs3_curve(const bs3_curve&);

    summary_bs3_curve(const summary_bs3_curve&);

    virtual ~summary_bs3_curve(); // Ensure any derived class destructor gets
    // a say when we destroy a surface.

    void	save();
    void	restore();

    // Make a linear change of parameter to all the knot values
    // (new value = a * old_value + b). 
    void reparam(double aa, double bb);

public:
    /**
     * @nodoc
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * @nodoc
     */
    int num_knots() const;
    /**
     * @nodoc
     */
    const double* knots() const;
    /**
     * @nodoc
     */
    int degree() const;
};

/**
 * Defines an abstract base class from which <tt>intcurve</tt> definitions are derived.
 * <br>
 * <b>Role:</b> In ACIS an interpolated curve is represented by the class <tt>intcurve</tt>,
 * which contains a pointer to an internal description called <tt>int_cur</tt>.
 * To support various types of curve construction, ACIS uses classes derived from
 * <tt>int_cur</tt>. Also, curve classes can be derived from the classes derived from <tt>int_cur</tt>
 * to construct more specialized curve types.
 * <br><br>
 * This class contains the mathematical definition for an <tt>intcurve</tt>. It uses use counts
 * to limit copying, and it allows derivation to construct various types of procedural curves,
 * as well as one type that represents a NURBS curve. The base class <tt>int_cur</tt> contains
 * the following information:
 * <ul>
 * <li>A <i>use count</i> indicating the number of times this <tt>int_cur</tt> is used.</li>
 * <li>A pointer to a B-spline curve.</li>
 * <li>A fitting tolerance representing the precision of the B-spline approximation to the true curve.</li>
 * <li>Two pointers to surfaces (which may be <tt>NULL</tt>).</li>
 * <li>Two pointers to parameter space curves (which may be <tt>NULL</tt>). Each parameter space curve
 *		(if defined) reside in the parameter space of the surface with the same index.</li>
 * </ul>
 * Classes derived from <tt>int_cur</tt> can contain additional information that records
 * the creation method of the true curve.
 * <br><br>
 * This class is supported by virtual functions that depend on the definition
 * of the true curve. The virtual functions allow the derived curves to implement the
 * functionality on their own. For curves with an exact <tt>bs3_curve</tt>, there is no
 * need to implement the functionality because the methods written for the base
 * class are sufficient.
 * <br><br>
 * Note: <tt>int_cur</tt> methods are very rarely called by applications. They are generally called
 * by <tt>intcurve</tt> methods.
 * @see discontinuity_info, summary_bs3_curve, surface, intcurve, SPAinterval
 */
class DECL_KERN int_cur : public subtrans_object
{
private:

    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef int_cur_FRIENDS
    int_cur_FRIENDS
#endif
        /**
         *  Make intcurv_cache a friend so that it can use it.
         */
        friend class intcur_cache;
    friend class gcmgr;
    friend class ic_gcmgr;
    /**
     * Increase the use count.
     */
    void operator++() { add_ref(); }
    /**
     * Decrease the use count.
     */
    void operator--() { remove_ref(); }
    /**
     * Returns <tt>TRUE</tt> if there is more than one reference to this object; otherwise, it returns <tt>FALSE</tt>.
     */
    logical mult() { return mult_ref(); }

#ifdef ENABLE_CACHE_SWITCHING
    /**
     * Cached data
     * <br><br>
     * <b>Role :</b>
     * Provides a pointer to cached data, which is used to speed up enquiries such
     * as <tt>param</tt>, <tt>point_perp</tt> and <tt>evaluate</tt>.  The calls to the
     * caching code are all made in member functions of the base class so derived classes
     * do not need to concern themselves with caching and the pointer can be private.
    */
    mutable intcur_cache* cache;
#endif

    /**
     *  Make intcurve a friend so that it can use it.
     */
    friend class intcurve;
    // STL apc 11Apr03 Extra methods for (generic) checking of discontinuity
    // data for int_cur types (bug 69775)
    int calc_continuity(double t) const;
    disc_info_status validate_disc_info(const discontinuity_info& supplied,
        const discontinuity_info& calculated,
        unmarked_discon_type& discon_type,
        int to_order = 3) const;
protected:

    /**
    * @nodoc.
    **/
    logical corrupt_progenitors() const;
    /**
     * @nodoc
     * Trim the query cache to range
     */
    void trim_cache_to_range(SPAinterval const&) const;

    disc_info_calc_status disc_calc_status;
    /**
     * @nodoc
     * Internal convenience function -
     * is there a valid tight approximation?
     */
    logical has_valid_tight_approx() const;
    /**
     * bs3_curve for tight fit
     */
    bs3_curve tight_bs3_curve;
    /**
     * Tight bs3_curve tolerance
     */
    double tight_bs3_tolerance;
    /**
     * Method for making tight bs3_curve fit
     */
    logical make_tight_bs3_curve(double tol);
    /**
     * Remove the tight bs3_curve approximation
     */
    void remove_tight_approx();
    /**
     * Recalculate the tight approximation after restoring data from a .sat file
     */
    void recreate_tight_approx() const;
    /**
     * Object-space approximation to the true curve.
     */
    bs3_curve cur_data;
    /**
     * @nodoc
     * activate_tight_approx
     * Note - none persistant data
     */
    logical activate_tight_approx();
    /**
     * @nodoc
     * deactivate_tight_approx
     * Note - none persistant data
     */
    logical deactivate_tight_approx();
    /**
     * @nodoc
     * extend_tight_approx_incremental
     *
     */
    logical extend_tight_approx_incremental(double tol, const SPAinterval& /*new_range*/);
    /**
     *
     */
    logical use_tight_approx;
    /**
     * Precision to which the B-spline approximates to the true
     * object-space curve.
     */
    double fitol_data;
    /**
     * First defining surface.
     * <br><br>
     * <b>Role :</b> Up to two surfaces defining the true curve. Derived classes
     * may use them in different ways, but in the base class the
     * true curve is known to lie on any surface given here.
     */
    surface* surf1_data;
    /**
     * Second defining surface.
     * <br><br>
     * <b>Role :</b> Up to two surfaces defining the true curve. Derived classes
     * may use them in different ways, but in the base class the
     * true curve is known to lie on any surface given here.
     */
    surface* surf2_data;
    /**
     * Parameter-space curve with respect to the first defining surface.
     * <br><br>
     * <b>Role :</b> Parameter-space curves with respect to the given
     * surfaces. Only non-<tt>NULL</tt> if the corresponding surface
     * exists and is parametric.
     */
    bs2_curve pcur1_data;
    /**
     * Parameter-space curve with respect to the second defining surface.
     * <br><br>
     * <b>Role :</b> Parameter-space curves with respect to the given
     * surfaces. Only non-<tt>NULL</tt> if the corresponding surface
     * exists and is parametric.
     */
    bs2_curve pcur2_data;
    /**
     * Pcurve1 precision.
     * <br><br>
     * <b>Role :</b>
     * Precision in parameter-space to which the parameter-space curves to the true
     * object-space curve.
     */
    double partol1_data;
    /**
     * Pcurve2 precision.
     * <br><br>
     * <b>Role :</b>
     * Precision in parameter-space to which the parameter-space curves to the true
     * object-space curve.
     */
    double partol2_data;
    /**
     * bs1_curve properties
     * <br><br>
     * <b>Role :</b> Defines the following properties of the <tt>bs1_curves</tt>:
     * <pre>
     *   a. Whether or not all of the <tt>bs2_curve</tt> knots lie on the <tt>int_cur</tt>.
     *   b. Whether all of the <tt>bs2_curve</tt> hull turning angles are not too sharp.
     *   c. Whether the <tt>bs2_curve</tt> hull is known to self-intersect or not.
     *   d. Whether the <tt>bs2_curve</tt> hull fully encloses the <tt>int_cur</tt>.
     * </pre>
     * Has a value consisting of four digits.<br>
     * The first digit has the following values :
     * <pre>
     *	0 if Knots on curve property is unknown
     *	1 if Knots on curve property is <tt>FALSE</tt>
     *	2 if Knots on curve property is <tt>TRUE</tt>
     *</pre>
     * The second digit has the following values :<br>
     * <pre>
     *	0 if Turning angles ok property is unknown
     *	1 if Turning angles ok property is <tt>FALSE</tt>
     *	2 if Turning angles ok property is <tt>TRUE</tt>
     * </pre>
     * The third digit has the following values :<br>
     * <pre>
     *	0 if Hull self-intersection property is unknown
     *	1 if Hull self-intersection property is <tt>FALSE</tt>
     *	2 if Hull self-intersection property is <tt>TRUE</tt>
     * </pre>
     * The fourth digit has the following values :<br>
     *	0 if Hull enclosure property is unknown
     *	1 if Hull enclosure property is <tt>FALSE</tt>
     *	2 if Hull enclosure property is <tt>TRUE</tt>
     *</pre>
     */
    int bs1_properties;
    /**
     * bs2_curve properties
     * <br><br>
     * <b>Role :</b> Defines the following properties of the bs2_curves:
     * <pre>
     *   a. Whether or not all of the <tt>bs2_curve</tt> knots lie on the <tt>int_cur</tt>.
     *   b. Whether all of the <tt>bs2_curve</tt> hull turning angles are not too sharp.
     *   c. Whether the <tt>bs2_curve</tt> hull is known to self-intersect or not.
     *   d. Whether the <tt>bs2_curve</tt> hull fully encloses the <tt>int_cur</tt>.
     * </pre>
     * Has a value consisting of four digits.<br>
     * The first digit has the following values :
     * <pre>
     *	0 if Knots on curve property is unknown
     *	1 if Knots on curve property is <tt>FALSE</tt>
     *	2 if Knots on curve property is <tt>TRUE</tt>
     *</pre>
     * The second digit has the following values :<br>
     * <pre>
     *	0 if Turning angles ok property is unknown
     *	1 if Turning angles ok property is <tt>FALSE</tt>
     *	2 if Turning angles ok property is <tt>TRUE</tt>
     * </pre>
     * The third digit has the following values :<br>
     * <pre>
     *	0 if Hull self-intersection property is unknown
     *	1 if Hull self-intersection property is <tt>FALSE</tt>
     *	2 if Hull self-intersection property is <tt>TRUE</tt>
     * </pre>
     * The fourth digit has the following values :<br>
     *	0 if Hull enclosure property is unknown
     *	1 if Hull enclosure property is <tt>FALSE</tt>
     *	2 if Hull enclosure property is <tt>TRUE</tt>
     *</pre>
     */
    int bs2_properties;
    /**
     * Discontinuity information.
     * <br><br><b>Role :</b>
     * If the supporting surfaces of the curve
     * have discontinuities, or if the curve has a default (tangent)
     * extension, then it will have discontinuities. These are stored
     * here.
     */
    discontinuity_info	disc_info;
    const discontinuity_info& get_disc_info() const;
    /**
     * The full range of the <tt>int_cur</tt>, as returned by <tt>param_range</tt>.
     * <br><br>
     * <b>Role :</b> If an approximating curve is present (in <tt>cur_data</tt>)
     * then <tt>range</tt> and <tt>bs3_curve_range( cur_data )</tt> should be identical.
     */
    SPAinterval range;
    /**
     * Closure type.
     * <br><br>
     * <b>Role :</b> Takes the value <tt>OPEN</tt>, <tt>CLOSED</tt> or <tt>PERIODIC</tt> (or unset if the
     * <tt>int_cur</tt> is undefined). If an approximating curve is present
     * (in <tt>cur_data</tt>) then the closure of the approximating curve
     * will be consistent.
    */
    closed_forms closure;
    /**
     * Update the range and closure information from a bs3_curve.
     */
    void update_data(bs3_curve);
    /**
     * Calculate the closure of the curve from geometric tests.
     */
    closed_forms	calculate_closure();
    /**
     * Curve safe range.
     * <br><br>
     * <b>Role :</b>
     * A sub-range of the curve which avoids any terminators at the ends
     * of the curve, and consequently is safe for relaxation. Outside this
     * range, but inside the full curve range, the approximating curve is
     * taken to define the curve. Typically (i.e. if no terminators or
     * surface singularities are present), the <tt>safe_range</tt> is the infinite
     * <tt>SPAinterval</tt>; if a terminator is present the <tt>safe_range</tt> will stop just
     * short of it.
     * <br>
     * Note that the base class administrates the <tt>safe_range</tt> (for example,
     * it updates it following reparametrization) but it is the
     * responsibility of the derived class to set it initially.
     */
    SPAinterval safe_range;
    /**
     * Set the safe range.
     * <br><br>
     * <b>Role :</b> Sets the <tt>safe_range</tt>. The base class uses it when it is
     * uncertain how to process the base range. The default version just
     * sets it to a <tt>NULL</tt> <tt>SPAinterval</tt>. Note that other implementations of this
     * function are not available to the base class constructor, and
     * cannot be used safely when the curve is input from a data stream,
     * so the derived classes are responsible for setting the <tt>safe_range</tt>.
     */
    virtual	void set_safe_range();
    /**
     * Returns if the <tt>bs1_curve</tt> knots are known to lie on the curve.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs1_knots_on_curve() const;
    /**
     * Returns if the <tt>bs2_curve</tt> knots are known to lie on the curve.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs2_knots_on_curve() const;
    /**
     * Return if the <tt>bs1_curve</tt> hull angles are acceptable in size.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs1_hull_angles_ok() const;
    /**
     * Return if the <tt>bs2_curve</tt> hull angles are acceptable in size.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs2_hull_angles_ok() const;
    /**
     * Return if the <tt>bs1_curve</tt> hull self-intersects.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs1_hull_self_intersects() const;
    /**
     * Return if the <tt>bs2_curve</tt> hull self-intersects.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int bs2_hull_self_intersects() const;
    /**
     * Return if the <tt>bs1_curve</tt> hull is known to enclose the curve.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int hull1_enclosure() const;
    /**
     * Return if the <tt>bs2_curve</tt> hull is known to enclose the curve.
     * Returns -1 if unknown, 0 if <tt>FALSE</tt>, or 1 if <tt>TRUE</tt>.
     */
    int hull2_enclosure() const;
    /**
     * Set <tt>bs2_curve</tt> knots lie on curve
     * <br><br>
     * <b>Role :</b> Function to set the property of whether the <tt>bs2_curve</tt> knots all
     * lie on the <tt>int_cur</tt>. The first argument should be 1 or 2, to
     * indicate the <tt>bs2_curve</tt> which the property is being set for,
     * and the second integer argument should be -1, 0 or 1, to
     * indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     */
    void set_bs_knots_on_curve(int pcu_no, int knots_on_cu);
    /**
     * Set <tt>bs2_curve</tt> hull ok property.
     * <br><br>
     * <b>Role :</b> Function to set the property of whether the <tt>bs2_curve</tt> hull
     * does not turn too sharply. The first argument should be 1 or 2,
     * to indicate the <tt>bs2_curve</tt> which the property is being set for,
     * and the second integer argument should be -1, 0 or 1, to
     * indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     */
    void set_bs_hull_angles_ok(int pcu_no, int hull_angles_ok);
    /**
     * Set <tt>bs2_curve</tt> self-intersection property.
     * <br><br>
     * <b>Role :</b> Function to set the property of whether the <tt>bs2_curve</tt> hull
     * self-intersects. The first argument should be 1 or 2,
     * to indicate the <tt>bs2_curve</tt> which the property is being set for,
     * and the second integer argument should be -1, 0 or 1, to
     * indicate that the property is unknown, <tt>FALSE</tt> or <tt>TRUE</tt>.
     */
    void set_bs_hull_self_intersects(int pcu_no, int hull_self_ints);
    /**
     * Set hull enclose type.
     * <br><br>
     * <b>Role :</b> Function to set the curve enclosure. The first argument should
     * be 1 or 2, to indicate the <tt>bs2_curve</tt> which the enclosure is being
     * set for, and the second integer argument should be -1, 0 or 1,
     * to give the value for the hull enclosure.
     */
    void set_hull_enclosure(int pcu_no, int encl);
    /**
     * bs3_curve data in summary form.
     * <br><br>
     * <b>Role :</b> This field may be set on
     * restore, if the full curve is not available.  It may be used to
     * make the actual bs3_curve.
     */
    summary_bs3_curve* summary_data;
    /**
     * Returns the number of knots in summary data.
     */
    int summary_nknots() const
    {
        return summary_data->nknots;
    }
    /**
     * Returns the knots in summary data.
     */
    const double* summary_knots() const
    {
        return summary_data->_knots;
    }
    /**
     * Returns the degree of summary_data.
     */
    int summary_degree() const
    {
        return summary_data->deg;
    }
    /**
     * Deletes the summary_data.
     */
    void delete_summary_data()
    {
        ACIS_DELETE summary_data; summary_data = 0;
    }
    /**
     * Tight approx data in summary form.
     * <br><br>
     * <b>Role :</b> This field may be set on
     * restore, if the full curve is not available.  It may be used to
     * make the actual bs3_curve.
     */
    summary_bs3_curve* tight_summary_data;
    /**
     * Deletes the tight_summary_data.
     */
    void delete_tight_summary_data()
    {
        ACIS_DELETE tight_summary_data; tight_summary_data = NULL;
    }

    // Copy data members from the "other" curve
    // Internal use only
    logical copy_data_from_other(const curve* other);

    /**
     * C++ Default constructor.
     * <br><br>
     * <b>Role :</b> Construct a generally null int_cur. This is to allow flexibility
     * for constructors for derived classes to set the common data
     * members in whatever way is most convenient.
     */
    int_cur();

    // C++ initialize constructor requests memory for this object and populates it
    // with the data supplied as arguments.
    // This constructor cannot be called directly to make an int_int_cur;
    // however, the following procedure can be used to make an object of the int_int_cur class type.
    //   Make an object of type int_int_interp (refer to the definition of int_int_interp in the curve_interp class description).
    //   Call the int_int_interp method, make_int_cur. This method returns an int_cur class object.
    /**
     * C++ initialize constructor requests memory for this object and populates it
     * with the data supplied as arguments.
     * <br><br>
     * @param bs3
     * Given bs3_curve.
     * @param bs3tol
     * Fit tolerance.
     * @param surf1
     * First surface.
     * @param surf2
     * Second surface.
     * @param pcurve1
     * First surface pcurve.
     * @param pcurve2
     * Second surface pcurve.
     * @param safe_range
     * Safe range.
     * @param disc_info
     * Discontinuities.
     */
    int_cur(
        bs3_curve            bs3,
        double               bs3tol,
        surface const& surf1,
        surface const& surf2,
        bs2_curve            pcurve1,
        bs2_curve            pcurve2,
        const SPAinterval& safe_range = *(const class SPAinterval*)NULL_REF,
        const discontinuity_info& disc_info = *(discontinuity_info*)NULL_REF
    );
    /**
     * A version of the constructor which takes the range and closure instead
     * of an approximating curve.
     * <br><br>
     * <b>Role :</b> Available for derived class constructors.
     * @param curv_int
     * Curve range.
     * @param cform
     * Curve form (closure).
     * @param surf1
     * First surface.
     * @param surf2
     * Second surface.
     * @param pcurve1
     * First surface pcurve.
     * @param pcurve2
     * Second surface pcurve.
     * @param safe_range
     * Safe range.
     * @param disc_info
     * Discontinuities.
     */
    int_cur(
        const SPAinterval& curv_int,
        closed_forms        cform,
        surface const& surf1,
        surface const& surf2,
        bs2_curve           pcurve1,
        bs2_curve           pcurve2,
        const SPAinterval& safe_range = *(const class SPAinterval*)NULL_REF,
        const discontinuity_info& disc_info = *(discontinuity_info*)NULL_REF
    );
    /**
     * Copy constructor. Derived classes should also define copy constructors,
     * and use this one, to ensure that <tt>int_cur</tt> data is initialized.
     * @param intc
     * Curve to be copied.
     */
    int_cur(const int_cur& intc);
    /**
     * C++ Destructor.
     * <br><br>
     * <b>Role :</b>
     * The destructor eliminates all the dependent B-spline curve and
     * surface data. Each derived class must have a destructor if it
     * adds further dependent data.
     */
    virtual ~int_cur();
    /**
     * Approximation is being made.
     * <br><br>
     * <b>Role :</b> Prevent recursive calls to make_approx.
     */
    logical	calling_make_approx;
    /**
     * Generate an approximating curve.
     * <br><br>
     * <b>Role :</b>
     * Make or remake the approximating curve.  The <tt>intcurve</tt> argument <tt>ic</tt>
     * may be <tt>NULL</tt> but if it is supplied the function may be a little faster.
     * The function stores the approximating curve and the actual fit error
     * that was achieved in the <tt>int_cur</tt>, overriding the declared const-ness
     * of the method to do this.
     * @param fit
     * Desired fit tolerance.
     * @param ic
     * Helper <tt>intcurve</tt>.
     * @param force
     * if <tt>TRUE</tt>, will make an approx even if it is self-intersecting.
    */
    virtual void make_approx(
        double fit,
        const intcurve& ic = *(intcurve*)NULL_REF,
        logical force = FALSE
    ) const;
    /**
     * Returns the underlying B-spline curve, which is generally an approximation to the true curve.
     */
    bs3_curve cur() const { return cur_data; }
    /**
     * Returns the B-spline curve fit tolerance.
     */
    double fitol() const { return fitol_data; }
    /**
     * Replace the underlying <tt>bs3_curve</tt> approximation. If the
     * supplied tolerance is negative, then fitol will be left unchanged.
     * @param bs3
     * New <tt>bs3_curve</tt> approximation.
     * @param tol
     * Tolerance. If negative, fitol will be left unchanged.
     * @param delete_old_bs3
     * Whether to delete the old <tt>bs3_curve</tt> or not.
     * @param allow_zero_fitol
     * Whether to allow setting the fitol to zero.
    */
    void set_cur(bs3_curve bs3,
        double tol = -1.0,
        logical delete_old_bs3 = TRUE,
        logical allow_zero_fitol = FALSE);
    /**
    * Return the first surface.
    * <br><br>
    * <b>Role :</b>By default, surfaces are only returned if the true
    *  curve lies on the surface surfaces defining the curve but
    * distant from it are not returned. To force the surface to be
    * returned regardless, the logical flag should be set to <tt>TRUE</tt>.
    * @param force
    * Return the surface regardless.
    */
    virtual surface const* surf1(logical force = FALSE) const;
    /**
    * Return the second surface.
    * <br><br>
    * <b>Role :</b>By default, surfaces are only returned if the true
    * curve lies on the surface surfaces defining the curve but
    * distant from it are not returned. To force the surface to be
    * returned regardless, the logical flag should be set to <tt>TRUE</tt>.
    * @param force
    * Return the surface regardless.
    */
    virtual surface const* surf2(logical force = FALSE) const;
    /**
     * Return curve in parameter-space of surface returned by <tt>surf1</tt>,
     * if the surface is parametric. Return <tt>NULL</tt> if the surface
     * (as returned by the methods above) is <tt>NULL</tt> or not parametric.
    */
    virtual bs2_curve pcur1(logical force = FALSE) const;
    /**
     * Return curve in parameter-space of surface returned by <tt>surf2</tt>,
     * if the surface is parametric. Return <tt>NULL</tt> if the surface
     * (as returned by the methods above) is <tt>NULL</tt> or not parametric.
    */
    virtual bs2_curve pcur2(logical force = FALSE) const;
    /**
     * Return the parameter-space tolerance of <tt>pcur1</tt>.
    */
    double partol1() const { return partol1_data; }
    /**
     * Return the parameter-space tolerance of <tt>pcur2</tt>.
    */
    double partol2() const { return partol2_data; }
    /**
    * Set the parameter-space tolerance of <tt>pcur1</tt>.
    */
    void set_partol1(double partol) { partol1_data = partol; }
    /**
    * Set the parameter-space tolerance of <tt>pcur2</tt>.
    */
    void set_partol2(double partol) { partol2_data = partol; }
    /**
    * Duplication. Can't be done by constructor, as we want it
    * to be virtual, and pure to force each derived type to have its
    * own.
    */
    virtual subtrans_object* copy() const = 0;
    /**
    * Test for equality. This is sufficient for many derived classes,
    * and can be used by most others to check the basic
    * representation. It does not guarantee that all effectively
    * equal surfaces are determined to be equal, but does guarantee
    * that different surfaces are correctly identified as such. The
    * default version checks the B-splines and surfaces, and checks that
    * the derived types are the same. This may be sufficient for
    * simple derived types - others may find it useful to call this
    * as part of the operation.
    */
    virtual logical operator==(subtype_object const& rhs) const;
    /**
    * Determine whether a <tt>int_cur</tt> is entirely enclosed within
    * another. (used in merging)
    */
    virtual logical operator>>(subtype_object const& rhs) const;
    /**
    * Return parametric curves with respect to the surfaces
    * defining this intcurve.  The argument may be 1 or 2,
    * representing the two surfaces in order. The default uses
    * <tt>pcur1</tt> or <tt>pcur2</tt>, <tt>surf1</tt> or <tt>surf2</tt>,
    * and <tt>fitol</tt>, and so will be suitable for most derived classes.
    * The second argument is flag indicating whether or not the
    * pcurve returned is temporary; if so, the <tt>bs2_curve</tt> will not
    * be copied, improving performance.
    */
    virtual pcurve* pcur(
        int     index,
        logical temporary = FALSE
    ) const;
    /**
    * Find an object-space bounding box, for the subset of the curve
    * within the given parameter bounds. The default finds the bound
    * on the B-spline approximation of the appropriate subset of the
    * curve, expanded by the fit tolerance, so will be suitable
    * for most derived classes.
    */
    virtual SPAbox bound(
        SPAinterval const& range = *(SPAinterval*)NULL_REF
    ) const;
    /**
    * Return a cone bounding the tangent direction of the curve. The
    * cone is deemed to have its apex at the origin, and has a given
    * axis direction and (positive) half-angle. If the logical
    * argument is <tt>TRUE</tt>, then a quick approximation may be found. The
    * approximate result may lie wholly within or wholly outside the
    * guaranteed bound (obtained with a <tt>FALSE</tt> argument), but may not
    * cross from inside to outside. Flags in the returned object
    * indicate whether the cone is in fact the best available, and
    * if not whether this result is inside or outside the best cone.
    * The default finds the cone for the B-spline approximation to the
    * curve, so will be suitable for most derived classes.
    */
    virtual curve_tancone tangent_cone(
        SPAinterval const& subset_range,
        logical             approx_ok
    ) const;
    /**
    * Parameter shift: adjust the spline curves to have a parameter
    * range increased by the argument value (which may be negative).
    * This is only used by <tt>intcurve::split</tt> to move portions of a
    * periodic curve by integral multiples of the period. The default
    * just shifts the parametrization of the approximating B-splines.
    */
    virtual void shift(double incr);
    /**
    * Perform a linear transformation on the parametrization, so
    * that it starts and ends at the given values (which must be
    * in increasing order). The default just reparametrizes the
    * approximating B-splines.
    */
    virtual void reparam(
        double start,
        double end
    );
    /**
    * Construct a new curve which is a copy of the part of the
    * given one within the given parameter bounds, unless this means
    * the whole curve, in which case return <tt>NULL</tt>.
    */
    virtual int_cur* subset(
        SPAinterval const& new_range
    ) const;
    /**
    * Divide a curve into two pieces at a given parameter value,
    * possibly adjusting the B-spline approximations to an exact value
    * at the split point. If the parameter value is at the beginning,
    * sets the first piece to <tt>NULL</tt>, and places the original curve
    * in the second slot; if the parameter value is at the end,
    * places the original curve in the first slot, and sets the
    * second to <tt>NULL</tt>. This is a pure virtual function to force
    * derived classes to have their own (though many will be able to use
    * <tt>split_int_cur</tt> below to do a lot of the hard work).
    */
    virtual void split(
        double              param,
        SPAposition const& exact,
        int_cur* piece[2]
    ) = 0;
    /**
    * The same specification as for <tt>split</tt>, except that a newly-
    * created, but empty, <tt>int_cur</tt> (normally in fact a derived object)
    * is supplied to be the second part of the split, if necessary.
    * The method returns <tt>TRUE</tt> if the second <tt>int_cur</tt> was used,
    * <tt>FALSE</tt> if not. This method assumes that the pcurve on any
    * surface is the locus of the foot of the perpendicular from
    * the curve to the surface. It is not called by any <tt>intcurve</tt>
    * member function, but is available for use by a derived class
    * <tt>split</tt> method to split the spline curves.
    */
    logical split_int_cur(
        double              param,
        SPAposition const& exact,
        int_cur* second_cur,
        int_cur* piece[2]
    );
    /**
    * Concatenate the contents of two curves into one. The curves are
    * guaranteed to be the same base or derived type, and to have
    * contiguous parameter ranges ("this" is the beginning part of
    * the combined curve, the argument gives the end part). The
    * default concatenates the approximating B-splines, assuming that
    * any other information is the same for both curves and the
    * result.
    */
    virtual void append(int_cur& back);
    /**
     * Transform the curve. The default transforms the B-spline
     * approximations and the surfaces, and scales the fit tolerance.
     */
    virtual void operator*=(SPAtransf const& trans);
    /**
     * Calculate the tangent direction at given point on the curve. The base class
     * version uses <tt>param</tt> and <tt>eval</tt>, and so is always valid, but
     * many derived types will have a better way.
    */
    virtual SPAunit_vector point_direction(
        SPAposition const& pos,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;
    /**
     * Calculate the curvature at the given point on the curve. The base class version uses
     * <tt>param</tt> and <tt>eval</tt>, and so is always valid, but many derived
     * types will have a better way.
    */
    virtual SPAvector point_curvature(
        SPAposition const& pt,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF
    ) const;
    /**
    * This non-virtual method looks in the cache for a given position
    * and parameter guess, if any. If found it returns the foot, direction,
    * curvature and parameter value. Otherwise it uses <tt>point_perp</tt> to find the
    * results, places them in the cache, and returns them.<br><br>
    * <tt>Point_perp_with_cache</tt>, rather than <tt>point_perp</tt>, should be
    * called by classes derived from <tt>int_cur</tt>, so as to get
    * the benefit of caching.
    */
    void point_perp_with_cache(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& tangent,
        SPAvector& curvature,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical              f_weak = FALSE
    ) const;
    /**
    * Find the foot of the perpendicular from the given point to
    * the curve, and tangent to and curvature of the curve at that
    * point, and its parameter value. If an input parameter value
    * is supplied (as argument 5), the perpendicular found is the
    * one nearest to the supplied parameter's position, otherwise it
    * is the one at which the curve is nearest to the given position.
    * Any of the return value arguments may be a NULL reference, in
    * which case it is simply ignored. The default version uses the
    * approximating B-spline to get a first approximation (if the given
    * guess is missing or needs improvement), and then iterates to the
    * exact value using <tt>eval</tt>. It will therefore be suitable for
    * most derived classes.
    */
    virtual void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& tangent,
        SPAvector& curvature,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF,
        logical              f_weak = FALSE
    ) const;
    /**
    * This non-virtual method looks in the cache for a given position and
    * parameter guess if any.  If found it returns the foot and
    * parameter value. Otherwise it uses <tt>closest_point</tt> to find the
    * results, places them in the cache, and returns them. <br><br>
    * <tt>Closest_point_with_cache</tt>, rather than <tt>closest_point</tt>, should be called
    * by classes derived from <tt>int_cur</tt>, so as to get the benefit of caching.
    */
    void closest_point_with_cache(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual = *(SPAparameter*)NULL_REF
    ) const;
    /**
    * Find the closest point on the curve, the foot, to the given point,
    * and optionally its parameter value. If an input parameter value is
    * supplied (as the first <tt>SPAparameter</tt> argument), the foot found is only
    * a local solution nearest to the supplied <tt>SPAparameter</tt>'s position. Any
    * of the return value arguments may be a NULL reference, in which
    * case it is simply ignored.
    */
    virtual void closest_point(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const
    {
        point_perp(
            pos,
            foot,
            *(SPAunit_vector*)NULL_REF,
            *(SPAvector*)NULL_REF,
            param_guess,
            param_actual, FALSE
        );
    }
    /**
    * This non-virtual function looks in the cache for a given position.
    * If found it returns the parameter value, otherwise it finds the parameter
    * value using <tt>param</tt>, places it in the cache, and returns it. <br><br>
    * <tt>Param_with_cache</tt>, rather than <tt>param</tt>, should be called by classes
    * derived from <tt>int_cur</tt>, so as to get the benefit of caching.
    */
    double param_with_cache(
        SPAposition const& pos,
        SPAparameter const& param = *(SPAparameter*)NULL_REF
    );
    /**
    * Determine the parameter value for the given point on the curve. The base
    * class version finds the foot of the perpendicular to the approximating
    * B-spline, and returns the parameter value of that. This will be
    * correct for many derived types, but not necessarily all.
     */
    virtual double param(
        SPAposition const& pos,
        SPAparameter const& param = *(SPAparameter*)NULL_REF
    ) const;
    /**
    * Find the position, first and second derivatives on the curve at the given
    * parameter value.
    * <br><br>
    * <b>Role:</b>
    * This non-virtual function looks in the cache for position and first
    * and second derivatives at the given parameter value.  If found it
    * returns them, otherwise it computes them, puts them in the cache,
    * and returns them. <br><br>
    * <tt>Eval_with_cache</tt>, rather than <tt>eval</tt>, should be called by classes
    * derived from <tt>int_cur</tt>, so as to get the benefit of caching.
    * @param par
    * Given parameter value.
    * @param pos
    * Found position.
    * @param fd
    * First derivative
    * @param ffd
    * Second derivative.
    * @param approx_ok
    * Approximation is ok to be used.
    */
    void eval_with_cache(
        double        par,
        SPAposition& pos,
        SPAvector& fd = *(SPAvector*)NULL_REF,
        SPAvector& ffd = *(SPAvector*)NULL_REF,
        logical       approx_ok = FALSE
    ) const;
    /**
     * Find the position, first and second derivatives on curve at given
     * parameter value.
    */
    virtual void eval(
        double        par,
        SPAposition& pos,
        SPAvector& fd = *(SPAvector*)NULL_REF,
        SPAvector& ffd = *(SPAvector*)NULL_REF,
        logical       approx_ok = FALSE
    ) const;
    /**
     * Find the position on curve at given parameter value. The default
     * uses <tt>eval</tt>.
     */
    virtual SPAposition eval_position(
        double        par,
        logical       approx_ok = FALSE
    ) const;
    /**
     * Find parametric derivative, magnitude and direction, at the given parameter value. The
     * default uses <tt>eval</tt>.
    */
    virtual SPAvector eval_deriv(
        double        par,
        logical       approx_ok = FALSE
    ) const;
    /**
     * Find the tangent direction at the given parameter value on
     * the curve. Default uses <tt>eval_deriv</tt>.
    */
    virtual SPAunit_vector eval_direction(
        double        par,
        logical       approx_ok = FALSE
    ) const;
    /**
     * Find the curvature at the given parameter value on
     * the curve. The default uses <tt>eval</tt>.
     */
    virtual SPAvector eval_curvature(
        double        par,
        logical       approx_ok = FALSE
    ) const;
    /**
    * This non-virtual function looks in the cache for position and <tt>nd</tt>
    * derivatives at the given parameter value.  If found it returns them,
    * otherwise it computes them, puts them in the cache, and returns them.<br><br>
    * <tt>Evaluate_with_cache</tt>, rather than <tt>evaluate</tt>, should be called by classes
    * derived from <tt>int_cur</tt>, so as to get the benefit of caching.
    * @param par
    * Given parameter value.
    * @param pos
    * Point on curve at given parameter value.
    * @param der
    * Array of pointers to vectors, of size <tt>nd</tt>. Any of the pointers may
    * be <tt>NULL</tt>, in which case the corresponding derivative will not be returned.
    * @param nd
    * Number of derivatives required.
    * @param eval_at
    * Evaluation location (above, below, or unknown)
    * @param ok
    * Approximation is good and should be used.
    */
    int evaluate_with_cache(
        double         par,
        SPAposition& pos,
        SPAvector** der = NULL,
        int            nd = 0,
        evaluate_curve_side eval_at = evaluate_curve_unknown,
        logical        ok = FALSE
    ) const;
    /**
    * The <tt>evaluate</tt> function calculates derivatives, of any order up
    * to the number requested, and stores them in vectors provided by
    * the user. It returns the number it was able to calculate.
    * This will be equal to the number requested in all but the most
    * exceptional circumstances. A certain number will be evaluated
    * directly and (more or less) accurately; higher derivatives will
    * be automatically calculated by finite differencing; the accuracy
    * of these decreases with the order of the derivative, as the
    * cost increases.  Derived classes of <tt>int_cur</tt> must implement this
    * function.
    * @param par
    * Given parameter value.
    * @param pos
    * Point on curve at given parameter value.
    * @param der
    * Array of pointers to vectors, of size <tt>nd</tt>. Any of the pointers may
    * be <tt>NULL</tt>, in which case the corresponding derivative will not be returned.
    * @param nd
    * Number of derivatives required.
    * @param eval_at
    * Evaluation location (above, bellow or unknown)
    */
    virtual int evaluate(
        double        par,
        SPAposition& pos,
        SPAvector** der = NULL,
        int           nd = 0,
        evaluate_curve_side eval_at = evaluate_curve_unknown
    ) const;
    /**
    * This non-virtual function looks in the cache for position and <tt>nd</tt>
    * derivatives at the given parameter value.  If found it returns them,
    * otherwise it computes them, puts them in the cache, and returns them.<br><br>
    * <tt>Evaluate_with_cache</tt>, rather than <tt>evaluate</tt>, should be called by classes
    * derived from <tt>int_cur</tt>, so as to get the benefit of caching.
    * @param par
    * Given parameter value.
    * @param evdata
    * Data supplying initial values, and set to reflect the results of this evaluation.
    * @param pos
    * Point on curve at given parameter.
    * @param der
    * Array of pointers to vectors, of size <tt>nd</tt>. Any of the pointers may
    * be <tt>NULL<tt>, in which case the corresponding derivative will not be returned.
    * @param nd
    * Number of derivatives required.
    * @param eval_at
    * Evaluation location (above, bellow or unknown)
    * @param ok
    * Approximation is good and should be used.
    */
    int evaluate_iter_with_cache(
        double              par,
        curve_evaldata* evdata,
        SPAposition& pos,
        SPAvector** der = NULL,
        int                 nd = 0,
        evaluate_curve_side eval_at = evaluate_curve_unknown,
        logical             ok = FALSE
    ) const;
    /**
    * The <tt>evaluate_iter</tt> function is just like <tt>evaluate</tt>, but is
    * supplied with a data object which contains results from a previous
    * nearby evaluation, for use as initial values for any iteration
    * involved.
    * @param par
    * Given parameter value.
    * @param evdata
    * Data supplying initial values, and set to reflect the results of this evaluation.
    * @param pos
    * Point on curve at given parameter.
    * @param der
    * Array of pointers to vectors, of size nd. Any of the pointers may
    * be null, in which case the corresponding derivative will not be returned.
    * @param nd
    * Number of derivatives required.
    * @param eval_at
    * Evaluation location (above, bellow or unknown)
    */
    virtual int evaluate_iter(
        double              par,
        curve_evaldata* evdata,
        SPAposition& pos,
        SPAvector** der = NULL,
        int                 nd = 0,
        evaluate_curve_side eval_at = evaluate_curve_unknown
    ) const;
    /**
    * Construct a data object to retain evaluation information across
    * calls to <tt>evaluate_iter</tt>. This is to allow subsidiary calls
    * within an iterative evaluator to start iteration much closer to the
    * required result than is possible just using the curve information
    * itself.
    */
    virtual curve_evaldata* make_evaldata() const;
    /**
    * This function sets discontinuity information in the curve
    * corresponding to discontinuities in the underlying surface(s).
    * An evaluator that takes surface arguments in addition to curve
    * arguments.  As well as returning curve positions and derivatives,
    * it returns the derivatives of the surface wrt <tt>t</tt> (these will often
    * but not always be equal to the curve derivs) and also the derivatives
    * of the surface parameters wrt <tt>t</tt>.  The array of vectors to return
    * the curve derivatives must be of length at least <tt>nd_cu</tt>, and the
    * various arrays of vectors to return the surface data can either
    * be <tt>NULL</tt>, indicating that this particular derivative is not required,
    * or be of length at least <tt>nd_sf</tt>.
    *
    * Unlike the other evaluators, this function OVERWRITES the integer
    * arguments specifying the numners of derivatives required, with the
    * number actually obtained.  The function itself returns information
    * about the surface data that was calculated:
    * <pre>
    *    0 => no surface data (e.g. exact_int_cur)
    *    1 => data for first surface only
    *    2 => data for second surface only
    *    3 => data for both surfaces
    * </pre>
    */
    void	disc_from_surfs();
    /**
    * Return the number of derivatives which <tt>evaluate</tt> can find
    * "accurately" (and fairly directly), rather than by finite
    * differencing, over the given portion of the curve. If there
    * is no limit to the number of accurate derivatives, returns
    * the value <tt>ALL_CURVE_DERIVATIVES</tt>, which is large enough to be
    * more than anyone could reasonably want.
    */
    virtual int accurate_derivs(
        SPAinterval const& roi = *(SPAinterval*)NULL_REF
        // Defaults to the whole curve
    ) const;
    /**
     * Shift the given parameter value to be within the principle period
     * of a periodic curve. Used to ensure the pcurves are evaluated
     * within their defined ranges.
     * @param param
     * Given parameter value.
     */
    SPAparameter limit_param(
        SPAparameter const& param
    ) const;
    /**
    * Computes the arc-length of the specified portion of the curve.
    * Returns the algebraic distance along the curve
    * between the given parameter values, the sign being positive if the
    * parameter values are given in increasing order, and negative
    * if they are in decreasing order. The result is undefined if
    * either parameter value is outside the parameter range of a
    * bounded curve. For a periodic curve the parameters are not
    * reduced to the principal range, and so the portion of the
    * curve evaluated may include several complete circuits. This
    * function is therefore always a monotonically increasing
    * function of its second argument if the first is held constant,
    * and a decreasing function of its first argument if the second
    * is held constant. The default version uses the corresponding
    * function for the approximating B-spline. A sys_warning will be raised if
    * unable to compute precise arc-length.
    * @param first_p
    * First parameter value.
    * @param second_p
    * Second parameter value.
    * @param approx_ok
    * If <tt>approx_ok</tt> is <tt>TRUE</tt>, the B-spline approximation is used in the length calculation.
    */
    virtual double length(
        double first_p,
        double second_p,
        logical approx_ok = TRUE
    ) const;
    /**
     * Returns the parameter value of the point on the curve at the given
     * algebraic arc length from that defined by the datum parameter.
     * The result is not defined for a bounded, nonperiodic curve if the datum
     * parameter is outside the parameter range, or if the length is outside
     * the range bounded by the values for the ends of the parameter range
     * the approximating B-spline. A sys_warning will be raised if
     * unable to compute precise length parameter.
     * @param datum_p
     * Datum parameter value.
     * @param al
     * Arc length.
     * @param approx_ok
     * If <tt>approx_ok</tt> is <tt>TRUE</tt>, the B-spline approximation is used in the length calculation.
     */
    virtual double length_param(
        double datum_p,
        double al,
        logical approx_ok = TRUE
    ) const;
    /**
     * Find the extrema of a parametric curve in a given direction,
     * ignoring its ends unless it is closed. The default version uses
     * the corresponding function for the approximating B-spline.
    */
    virtual curve_extremum* find_extrema(
        SPAunit_vector const& dir
    ) const;
    /**
     * Test point-on-curve to the given precision.
     * <br><br>
     * <b>Role :</b> The default version
     * uses the corresponding function for the approximating B-spline,
     * to a tolerance expanded using the fit tolerance, and then
     * tests the perpendicular to the true curve. Thus, it is suitable
     * for most derived classes.
    */
    virtual logical test_point_tol(
        SPAposition const& point,
        double               tol = 0,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const;

    /**
     * Return a cylinder which encloses the portion of the curve bounded
     * by the <tt>SPAinterval</tt>.
     * @param interv
     * region of interest.
    */
    virtual BOUNDING_CYLINDER enclosing_cylinder(const SPAinterval& interv =
        *(SPAinterval*)NULL_REF) const;
    /**
     * Returns the string "intcurve"
     */
    virtual char const* type_name() const = 0;
    /**
     * Save the int_cur to the save file.
     */
    virtual void save_data() const;

    /**
     * @nodoc
     */
    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    /**
     * @nodoc
     */
    virtual logical need_save_approx_as_full() const;
    /**
     * Save data common to all int_curs.
     */
    void save_common_data(save_approx_level)	const;
    /**
     * Ask the <tt>int_cur</tt> the default level at which the approximating
     * surface should be be stored.
     */
    save_approx_level enquire_save_approx_level()	const;
    /**
     * Restores the data saved by <tt>save_common_data</tt>.
     * Note that <tt>restore_data</tt> is not virtual (so no base class version is
     * necessary).
     */
    void restore_common_data();
    /**
     * The restore function for <tt>int_int_cur</tt> is special, as it has to
     * handle old-style save files, where that form was used for
     * <tt>exact_int_cur</tt>s and <tt>surf_int_cur</tt>s as well.
     * As a result, it has to get at the surface and pcurve pointers.
     */
    friend DECL_KERN subtype_object* restore_int_int_cur();

    /**
     * Check for any data errors in the curve, and correct the errors if
     * possible. The various arguments provide control over which checks
     * are made, which fixes can be applied and which fixes were actually
     * applied.  The function returns a list of errors that remain in the
     * curve upon exit.
     * @param input
     * Supplies a set of flags which say which fixes are allowable
     * (the default is to fix nothing).
     * @param result
     * Returns a set of flags which say which fixes were applied.
     * @param csl
     * List of checks that are to be made.  If the
     * list is <tt>NULL</tt>, then every possible check will
     * be made; otherwise, the function will only
     * check for things in the list.  The return
     * value for the function will then be a subset
     * of this list.
    */
    virtual	check_status_list* check(
        const check_fix& input = *(const check_fix*)NULL_REF,
        check_fix& result = *(check_fix*)NULL_REF,
        const check_status_list* csl = (const check_status_list*)NULL_REF
    );
#if defined D3_STANDALONE || defined D3_DEBUG
    /**
     * @nodoc
     */
    friend DECL_KERN D3_ostream& operator<<(
        D3_ostream&,
        int_cur const&
        );
    /**
     * @nodoc
     */
    friend DECL_KERN D3_istream& operator>>(
        D3_istream&,
        int_cur*&
        );
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

    /**
     * Outputs a title line and the details of the int_cur to the specified file.
     */
    virtual void debug(
        char const* title,
        logical brief,
        FILE* fp
    ) const = 0;
    /**
    * Debug utility function.
    * <br>
    * Debug mechanism. As with the save and restore mechanism we split the operation
    * into two parts: the virtual function <tt>debug</tt> prints a class-
    * specific identifying line, then calls the ordinary function
    * <tt>debug_data</tt> to put out the details. It is done this way so that
    * a derived class' <tt>debug_data</tt> can call its parent's version first,
    * to output the common data. Indeed, if the derived class has no
    * additional data it need not define its own version of <tt>debug_data</tt>
    * and use its parent's instead. A string argument provides the
    * introduction to each displayed line after the first, and a
    * logical sets "brief" output (normally removing detailed
    * subsidiary curve and surface definitions).
    */
    void debug_data(
        char const* title,
        logical brief,
        FILE* fp
    ) const;
    /**
     * Saves the incurve as a bs3_curve approximation.
     */
    void save_as_approx() const;
public:
    /**
     * Calculates the discontinuity information for the <tt>int_cur</tt> if none had been stored in <tt>disc_info</tt>.
     * <br>
     * <b>Role:</b> This function is intended to support restore of old versions of
     * <tt>int_cur</tt>.
     */
    virtual	void calculate_disc_info();
    /**
     * Indicates whether a curve is closed.
     * <br><br>
     * <b>Role:</b> Indicates whether a curve is closed, i.e., joins itself (smoothly
     * or not) at the ends of its principal parameter range. This method always
     * returns <tt>TRUE</tt> if periodic returns <tt>TRUE</tt>. The default version uses the corresponding
     * function for the approximating B-spline.
     */
    logical closed() const
    {
        return closure == CLOSED || closure == PERIODIC;
    }
    /**
     * @nodoc
     * Force calculate discontinuity info.
     * <br><br>
     * Default behavior is to simply call calculate_disc_info and return
     * TRUE.  Override in derived classes that refer to parent geometries
     * whose disc_info also needs recalcuating and updating.  Should return
     * FALSE in case of failure.
     */
    virtual	logical deep_calc_disc_info();
    /**
     * @nodoc
     * Deep copy elements without shared data.
     */
    void deep_copy_elements(const int_cur&, pointer_map* pm);
    /**
     * Creates a copy of the <tt>int_cur</tt> that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers.
     * Returns a pointer to the copied item.
     * <br><br>
     * In a <i>deep copy</i> all the information about the copied item is self-contained
     * in a new memory block. By comparison a <i>shallow copy</i> stores only the first
     * instance of the item in memory, and increments the <i>use count</i> for each copy.
     * <br><br>
     * The <tt>pointer_map</tt> <tt>pm</tt> keeps a list of all pointers in the original object that have
     * already been deep copied. For example, a <i>deep copy</i> of a complex model results
     * in self-contained data, but identical sub-parts within the model are allowed
     * to share a single set of data.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual int_cur* deep_copy(pointer_map* pm = NULL) const = 0;

    /**
    * @nodoc
    **/
    virtual void process(geometry_definition_processor& p) const
#ifdef INTERNAL_DEBUG_CHECKS
        = 0;
#else
        ;
#endif
    /**
     * An evaluator that takes surface arguments in addition to curve arguments.
     * <br><br>
     * <b>Role:</b> As well as returning curve position and derivatives, it returns
     * the derivatives of the surface wrt t (these will often but not always be equal
     * to the curve derivs) and also the derivatives of the surface parameters with
     * respect to t. The array of vectors to return the curve derivatives must be of
     * length at least <tt>nd_cu</tt>, and the various arrays of vectors to return the surface
     * data can either be <tt>NULL</tt>, indicating that this particular derivative is not
     * required, or be of length at least <tt>nd_sf</tt>.
     * <br><br>
     * The caller must supply an array of length <tt>nd_cu</tt> or <tt>NULL</tt> to indicate that
     * derivatives are not required for vector.
     * <br><br>
     * Unlike the other evaluators, this function OVERWRITES the integer arguments
     * specifying the numbers of derivatives required, with the number actually obtained.
     * The function itself returns information about the surface data that was calculated:
     * <br><br>
     * 0 => no surface data (e.g. <tt>exact_int_cur</tt>)<br>
     * 1 => data for first surface only<br>
     * 2 => data for second surface only<br>
     * 3 => data for both surfaces
     * <br><br>
     * This is the default implementation of the function, and is inefficient. It should
     * be implemented for each <tt>int_cur</tt> type.
     * <br><br>
     * @param param
     * Given parameter value.
     * @param pt
     * Point on curve at given parameter value.
     * @param der
     * Derivatives.
     * @param nd_cu
     * Number of curve derivs required or calculated.
     * @param nd_sf
     * Number of surface derivs required or calculated.
     * @param location
     * eval location.
     * @param pt1
     * Point on support surface 1.
     * @param der1
     * Derivs of first support surface.
     * @param pt2
     * Point on support surface 2.
     * @param der2
     * Derivs of second support surface.
     * @param param1
     * Params on surface 1.
     * @param derparam1
     * Derivs of params on surface 1.
     * @param param2
     * Params on surface 2.
     * @param derparam2
     * Derivs of params on surface 2.
     * @param guess1
     * optional guess value for 1st par_pos.
     * @param guess2
     * optional guess value for 2nd par_pos.
     */
    virtual int evaluate_surfs(
        double              param,
        SPAposition& pt,
        SPAvector* der,
        int& nd_cu,
        int& nd_sf,
        evaluate_curve_side location = evaluate_curve_unknown,
        SPAposition& pt1 = *(SPAposition*)NULL_REF,
        SPAvector* der1 = NULL,
        SPAposition& pt2 = *(SPAposition*)NULL_REF,
        SPAvector* der2 = NULL,
        SPApar_pos& param1 = *(SPApar_pos*)NULL_REF,
        SPApar_vec* derparam1 = NULL,
        SPApar_pos& param2 = *(SPApar_pos*)NULL_REF,
        SPApar_vec* derparam2 = NULL,
        SPApar_pos const& guess1 = *(SPApar_pos*)NULL_REF,
        SPApar_pos const& guess2 = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * @nodoc
     * Internal use. Roll mechanism.
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * @nodoc
     */
    virtual void minimize(minimize_helper*);
    /**
     * Returns the law form of an <tt>int_cur</tt>.
     */
    virtual law* law_form() { return NULL; }
    /**
     * Assignment operator. Copies all the underlying information.
     * <br><br>
     * @param intcur
     * given int_cur.
     */
    virtual int_cur& operator=(
        int_cur const& intcur
        );
    /**
     * Finds the parametric period of the interpolated curve, returning exactly 0.0 if the curve is not periodic.
     * <br><br>
     * <b>Role:</b> The default version uses the corresponding function for the
     * approximating B-spline.
     */
    double param_period() const
    {
        return closure == PERIODIC ? range.length() : 0.0;
    }
    /**
     * Finds the parameter range of the interpolated curve.
     * <br><br>
     * <b>Role:</b> The default version uses the corresponding function for the
     * approximating B-spline.
     * <br><br>
     * @param box
     * region of interest.
     */
    SPAinterval param_range(SPAbox const& box = *(SPAbox*)NULL_REF) const
    {
        SPAUNUSED(box)
            return range;
    }
    /**
     * Indicates whether the curve is periodic.
     * <br><br>
     * <b>Role:</b> Indicates whether the curve is periodic, i.e., joins itself smoothly
     * at the ends of its principal parameter range, so that edges may span the seam.
     * The default version uses the corresponding function for the approximating B-spline.
     */
    logical periodic() const
    {
        return closure == PERIODIC;
    }
    /**
     * Returns <tt>TRUE</tt> if the n<sup>th</sup> parameter-space curve is defined.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if the n<sup>th</sup> parameter-space curve is defined
     * (i.e., <tt>pcur</tt> returns a non-<tt>NULL</tt> <tt>pcurve</tt> pointer); otherwise, it
     * returns <tt>FALSE</tt>. The default tests the result of <tt>pcur1</tt> or <tt>pcur2</tt>
     * as appropriate, and so it suffices for most derived classes. The <tt>index</tt> argument
     * may be 1 or 2 indicating either the first or second parameter-space curve.
     * <br><br>
     * @param index
     * parameter-space curve index.
     */
    virtual logical pcur_present(
        int index
    ) const;

public:
    // for save
    bs3_curve gme_get_cur_data();
    double gme_get_fitol_data();
    surface* gme_get_surf1_data();
    surface* gme_get_surf2_data();
    bs2_curve gme_get_pcur1_data();
    bs2_curve gme_get_pcur2_data();
    SPAinterval gme_get_safe_range();
    // for restore
    void gme_set_fitol_data(double data);
    void gme_set_safe_range(SPAinterval data);
    void** gme_get_surf1_spl_ptr();
    void** gme_get_surf2_spl_ptr();
    void set_surf1_spl(spl_sur* spl);
    void set_surf2_spl(spl_sur* spl);
};
#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<(D3_ostream&, int_cur const&);
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>(D3_istream&, int_cur*&);
#endif

/** @} */
#endif
