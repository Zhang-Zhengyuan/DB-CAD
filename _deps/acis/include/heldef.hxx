/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                      helix curve
//
//  30-Nov-06 al : Update documentation
//  21-Sep-06 al : Update documentation
//  15-Aug-06 al : Removed obselete constructor.
//  04-Apr-06 al : Added reparam().
//  23-Mar-06 al : Added helix_range.
//  08-Mar-06 al : Added methods for backsaves.
//
/*******************************************************************/
#if !defined( helix_CLASS )
#define helix_CLASS
#include "acis.hxx"
#include "dcl_kern.h"
#include "curdef.hxx"
#include "debugmsc.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "position.hxx"
//
class SizeAccumulator;
class SPAbox;
class SPAtransf;
class law;
class SPAparameter;
class helix;
class intcurve;
/*******************************************************************/
/**
* @file heldef.hxx
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
#define helix_type 4
  /**
   * Returns a helix transformed by the given transformation.
   * <br><br>
   * @param name
   * helix to transform.
   * @param transf
   * transformation.
   */
DECL_KERN helix operator*(helix const& name, SPAtransf const& transf);
/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN curve* restore_helix();

class HELIX;

/**
 * Defines a (possibly tapered) helical curve.
 * <br><br>
 * <b>Role:</b> The curve is defined by a point on the axis (its root point),
 * axis direction, displacement vector from root point to a point on the curve (<tt>start_disp</tt>),
 * pitch, taper, handedness, parameter scaling and parameter range.
 * The general case is a tapered helix.
 * For an untapered helix the taper is exactly 0.0.
 *
 * The curve is parameterized by:
 * <pre>
 *      point = axis_root + axis_dir * pitch * t/(2*pi*t0) +
 *                  X_hat * r(t) * cos(t/t0) + sgn(handedness) * Y_hat * r(t) * sin(t/t0)
 * </pre>
 * where <tt>X_hat</tt> is defined by projecting <tt>start_disp</tt> onto the plane perpendicular to the axis direction
 * and then normalising:
 * <pre>
 *      start_disp_proj = start_disp - (start_disp % axis_dir) * axis_dir
 *      X_hat = start_disp_proj / start_disp_proj.len()
 *      r(t) = start_disp_proj.len() + taper * t / (2*pi*t0)
 *      (X_hat, Y_hat, axis_dir) is a right handed coordinate system
 *      sgn(handedness) = +1 if handedness == TRUE,
 *                      = -1 if handedness == FALSE.
 *      t0 = parameter_scaling > 0
 *      pitch > 0
 * </pre>
 * <br><br>
 */
class DECL_KERN helix : public curve
{
    class tapered_extrema_func;
    friend class HELIX;

public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    helix();
    helix(const char* gme);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Constructs a helix from its parameters.
     * <br><br>
     * @param axis_root
     * axis root position.
     * @param axis_dir
     * axis direction.
     * @param start_disp
     * displacement to start point.
     * @param pitch
     * pitch (distance between turns along axis direction).
     * @param handedness
     * handedness (TRUE is right, FALSE is left).
     * @param helix_range
     * maximum parameter range (must be bounded)
     * @param par_scaling
     * parameter scaling (one turn corresponds to parameter distance of <tt> 2*pi*par_scaling </tt>).
     * @param taper
     * taper (radial distance between turns).
     */
    helix(
        SPAposition const& axis_root,
        SPAunit_vector const& axis_dir,
        SPAvector const& start_disp,
        double                 pitch,
        logical                handedness,
        SPAinterval const& helix_range,
        double                 par_scaling = 1.0,
        double                 taper = 0.0
    );
    helix(const char* gme, const SPAposition& axis_root, const SPAunit_vector& axis_dir,
        const SPAvector& start_disp, double pitch, logical handedness, const SPAinterval& helix_range,
        double par_scaling = 1.0, double taper = 0.0);
    /**
     * C++ destructor, deleting a <tt>helix</tt>.
     */
    ~helix();
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param name
     * helix name.
     */
    helix(helix const& name);
    helix(const char* gme, const helix& name);

    /**
     * Returns the number of derivatives that the evaluate function can find accurately.
     * <br><br>
     * As there is no limit to the number of accurate derivatives, this function returns the
     * value <tt>ALL_CURVE_DERIVATIVES</tt>.
     * <br><br>
     * @param cur
     * portion of a curve.
     */
    virtual int accurate_derivs(
        SPAinterval const& cur = *(SPAinterval*)NULL_REF
    ) const;
    /**
     * Finds box around a helix or portion thereof bounded by points on the curve.
     * <br><br>
     * <b>Role:</b> Increasing in parameter value. The points lie on the curve as supplied,
     * not as transformed. The resulting box is not necessarily the minimal one.
     * <br><br>
     * @param p1
     * first position.
     * @param p2
     * second position.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAposition const& p1,
        SPAposition const& p2,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Finds box around a helix or portion thereof bounded by parameter values (in increasing order).
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
     * Returns a bounding box around the portion of the helix inside a given box.
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
     * Indicates whether a curve is closed, that is joins itself (smoothly or not) at the ends of its principal parameter range.
     * <br><br>
     * <b>Role:</b> This function should always return <tt>TRUE</tt> if periodic does.
     */
    virtual logical closed() const;

    /**
     * Check for any data errors in the helix, and correct the errors if
     * possible.  The various arguments provide control over which checks
     * are made, which fixes can be applied and which fixes were actually
     * applied.  The function returns a list of errors that remain in the
     * curve on exit.  This version of the function fixes nothing. The only
     * check which is done is the spiral check.
     */
    check_status_list* check(
        const check_fix& fix,
        check_fix& fixed,
        const check_status_list* check_list
    );

    /**
     * Output details of the helix for inspection.
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
     * code that the most recent call to any curve or surface member function was in fact
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
     * Finds the extrema of a helix in a given direction.
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
     * Returns a pointer to the law form that is part of the helix definition.
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
     * not used for helices.
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
     * not used for helices.
     */
    virtual double length_param(
        double para,
        double	len,
        logical approx_ok = TRUE
    ) const;
    /**
     * Makes a copy of this helix on the heap, and return a pointer to it.
     */
    virtual curve* make_copy() const;
    /**
     * Negates this helix, i.e. make it the same curve but described in the opposite sense.
     */
    virtual curve& negate();
    /**
     * Returns a helix with the opposite sense from this one.
     */
    helix operator-() const;
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
     * Returns this <tt>helix</tt> transformed by the given <tt>SPAtransf</tt>.
     */
    friend DECL_KERN helix operator*(
        helix const&,
        SPAtransf const&
        );

    /**
     * Transforms this <tt>helix</tt> by the given <tt>SPAtransf</tt>, in place.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual curve& operator*=(SPAtransf const& trans);
    /**
     * Finds the parameter value at the given point on the helix.
     * <br><br>
     * <b>Role:</b> Follow the same procedure as for <tt>point_direction</tt> if the
     * point is not on the helix.
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
     * Returns the parameter range of the helix.
     * <br><br>
     * @param box
     * bounding box.
     */
    virtual SPAinterval param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;

    /**
     * Finds the curvature on a helix at the given point on the curve.
     * <br><br>
     * <b>Role:</b> If the point is not on the helix, the same procedure as for <tt>point_direction</tt>
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
     * Finds the tangent to a helix at the given point on the curve.
     * <br><br>
     * <b>Role:</b> If the point is not on the helix, a useful value is returned. In effect,
     * the half-plane bounded by the axis of the helix that contains the given point is
     * constructed. The tangent of the curve where it intersects this half plane is returned (all
     * the points give the same result for a helix). If the point is on the axis, so a half plane
     * cannot be constructed, a zero tangent vector is returned.
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
     * Restores the data for a helix from a save file.
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
     * <dl><dt><tt>read_position</tt></dt>
     * <dd>Axis root position.</dd>
     * <dt><tt>read_unit_vector</tt></dt>
     * <dd>Axis direction.</dd>
     * <dt><tt>read_vector</tt></dt>
     * <dd>Displacement to start point.</dd>
     * <dt><tt>read_real</tt></dt>
     * <dd>Pitch.</dd>
     * <dt><tt>read_logical</tt></dt>
     * <dd>Handedness.</dd>
     * <dt><tt>read_real</tt></dt>
     * <dd>Parameter scaling.</dd>
     * <dt><tt>read_real</tt></dt>
     * <dd>Taper.</dd>
     * <dt><tt>read_interval</tt></dt>
     * <dd>Helix range.</dd>
     * <dt><tt>curve::restore_data</tt></dt>
     * <dd>Restore the underlying curve of the helix.</dd></dl>
     */
    void restore_data();
    /**
     * @nodoc
     * Restore mechanism.
     */
    friend DECL_KERN curve* restore_helix();
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
     * Returns a string <tt>"helix"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Indicates whether the helix is properly defined.
     */
    virtual logical undef() const;
    /**
     * Reparameterizes the helix to start and end at the given values, which are in increasing order.
     * <br><br>
     * @param start
     * start point.
     * @param end
     * end point.
     */
    void reparam(
        double start,
        double end
    );
    /**
      * For back saves
      */
    virtual SPAinterval helix_range() const { return _helix_range; }

    virtual void set_helix_range(SPAinterval const& helix_range)
    {
        _helix_range = helix_range;
    }

    logical need_save_as_approx(int save_to_version, logical) const;

    intcurve* convert_to_law_curve(const SPAinterval& domain = *(SPAinterval*)NULL_REF);
    /**
     * Saves the id, then calls <tt>save_data</tt>.
     */
    virtual void save() const;
    /**
     * Saves the data for a helix to a save file.
     */
    void save_data() const;

    /**
    * Returns the root of this <tt>helix</tt>.
    */
    const SPAposition& axis_root() const { return _axis_root; }
    /**
     * Returns the axis direction of this <tt>helix</tt>.
     */
    const SPAunit_vector& axis_dir() const { return _axis_dir; }
    /**
     * Returns the start displacement of this <tt>helix</tt>.
     */
    const SPAvector& start_disp() const { return _start_disp; }
    /**
     * Returns the pitch of this <tt>helix</tt>.
     */
    double pitch() const { return _pitch; }
    /**
     * Returns the handedness of this <tt>helix</tt>.
     */
    logical handedness() const { return _handedness; }

    /**
     * Returns the parameter scaling of this <tt>helix</tt>.
     */
    double par_scaling() const { return _par_scaling; }

    /**
     * Returns the taper of this <tt>helix</tt>.
     */
    double taper() const { return _taper; }
    /**
     * Returns the radius of this <tt>helix</tt>.
     */
    double radius() const;

    /**
     * Returns the major direction of this <tt>helix</tt>.
     */
    SPAunit_vector maj_dir() const;
protected:
    /**
     * Axis root position of helix.
     */
    SPAposition _axis_root;
    /**
     * Axis direction.
     */
    SPAunit_vector _axis_dir;
    /**
     * Vector defining the displacement from the axis root position to a start point on helix.
     */
    SPAvector _start_disp;
    /**
     * Thread distance (pitch) of the helix.
     */
    double _pitch;
    /**
     * Handedness of the helix.
     */
    logical _handedness;

    /**
     * Parameter scaling of the helix.
     */
    double _par_scaling;

    /**
     * Taper of the helix.
     */
    double _taper;

    /**
      * Maximum range of the helix.
      */
    SPAinterval _helix_range;

    // Cache the scaled axes of the helix to speed up repeat evaluations using the same helix.
    /**
     * @nodoc
     */
    mutable SPAvector _major_axis;
    mutable SPAvector _minor_axis;
    mutable SPAvector _scaled_axis;
    mutable logical _repeat;

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
