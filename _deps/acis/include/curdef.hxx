/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for (lower-case) curve.

// This is an abstract base class for classes representing specific
// curve shapes within Acis. It provides a large variety of virtual
// functions for generic interaction with curves.
#if !defined( curve_CLASS )
#define curve_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "interval.hxx"
#include "box.hxx"
#include "debugmsc.hxx"
#include "position.hxx"
#include "unitvec.hxx"
#include "param.hxx"
/**
 * \defgroup ACISCURVES curves
 * \ingroup ACISGEOMETRY
 */
 /**
  * @file curdef.hxx
  * @CAA2Level L1
  * @CAA2Usage U2
  * @addtogroup ACISCURVES
  * @{
  */
class pcurve;
#if defined D3_STANDALONE || defined D3_DEBUG

class D3_istream;
class D3_ostream;

#endif

class SPAbox;
class law;

class SPAposition;
class SPAvector;
class SPAtransf;

class curve_extremum;
class curve_boundcyl;
class curve_tancone;
class discontinuity_info;
class check_status_list;
class check_fix;
class curve_evaldata;
class pointer_map;
class SizeAccumulator;
class curve;
class minimize_helper;

/**
 * Restores a curve.
 * <br><br>
 * <b>Role:</b> Although this <i>internal function is intended strictly for</i> ACIS
 * usage, a minimal amount of information about this function is provided for the
 * sole purpose of being able to understand and trace restoration from a SAT file.
 * This function should never be called directly, because it makes assumptions
 * about the availability of a SAT file, the location of the input pointer into the
 * SAT file, and the validity of SAT data it expects to read in. It also may start
 * a lengthy process of nested function or class method calls, which have many of
 * the same assumptions.
 * <br><br>
 * The <tt>restore</tt> function does the actual work of restoring the curve. It calls
 * the base class, then reads the selector, if the save file is new enough. This reads the
 * curve type and then switches in the run-time table to the correct restore routine.
 * <pre>
 *    if (restore_version_number &lt; CURVE_VERSION)
 *       read_int            Integer for the type of curve.
 *       dispatch_restore_cu Supply the number for the type of curve.
 *    else
 *       read_id             Reads in the string associated with the curve identification.
 *       dispatch_restore_cu Supply the curve identification for the type of curve.</pre>
 **/
DECL_KERN curve* restore_curve();
/**
 * @}
 */

 /**
  * @nodoc
  */
DECL_KERN curve* dispatch_restore_cu(int);

/**
 * @nodoc
 */
DECL_KERN curve* dispatch_restore_cu(char*);
#if defined D3_STANDALONE || defined D3_DEBUG

/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>(D3_istream&, curve*&);

/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<(D3_ostream&, const curve&);
#endif


// This class is the return type for the enclosing_cylinder() function.
// This function returns a cylinder which completely encloses that part
// of the curve lying between the specified parameter limits; the
// enclosing cylinder is unbounded along its length - only the cylinder
// axis root point and direction, and the radius, are stored. It provides
// a much tighter bound than a conventional box, but may be more expensive
// to compute and test.
// This class should be read-only, but there is a function for modifying
// the radius where this has been set unreasonably small, as this is how
// we find it has been used. In time this should go.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN curve_boundcyl : public ACIS_OBJECT {
    SPAposition _root_point;			// Reference point on axis
    SPAunit_vector _axis;				// Axis direction
    double _radius;					// Cylinder radius, conventionally
    // negative to mean "uninitialised"

public:
    curve_boundcyl() { _radius = -1; }

    curve_boundcyl(
        SPAposition const& root,
        SPAunit_vector const& axis,
        double rad
    ) : _root_point(root), _axis(axis), _radius(rad) {}

    // Read the values.

    SPAposition const& root_point() const { return _root_point; }
    SPAunit_vector const& axis() const { return _axis; }
    double radius() const { return _radius; }

    void set_radius(double r) { _radius = r; }
};


// Define a class for the return value from tangent_cone(). This
// defines a bound on the curve direction, used in intersection code
// to eliminate the possibility of tangencies and common normals.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN curve_tancone : public ACIS_OBJECT {
public:
    SPAunit_vector axis;
    double angle;
    logical approx;	// TRUE if this is an approximation to the best
    // cone available
    logical oversize;// if approx is TRUE, TRUE if this cone is
    // entirely outside the best cone available,
    // FALSE if it is inside.

    curve_tancone(
        SPAunit_vector const& ax,
        double ang,
        logical app,
        logical over
    ) : axis(ax)
    {
        angle = ang;
        approx = app;
        oversize = over;
    }
};


// Enumerated type used to determine which of two possible derivatives to
// evaluate in evaluate() when there is a discontinuity.

/*
// tbrv
*/
/**
 * @nodoc
 */

enum evaluate_curve_side {
    evaluate_curve_above,
    evaluate_curve_below,
    evaluate_curve_unknown
};

/**
 * @addtogroup ACISCURVES
 * @{
 */

 // The curve class proper, defining a large range of virtual functions,
 // mostly to be implemented by derived classes.
 /**
  * Provides methods and data common to all curve subclasses.
  * <br>
  * <b>Role:</b> The <tt>curve</tt> class is a base class from which all specific curve geometry
  * classes (<tt>straight</tt>, <tt>ellipse</tt>, <tt>helix</tt>, and <tt>intcurve</tt>) are derived.
  * It defines a large variety of virtual functions for generic interaction with curves.
  * <br><br>
  * Consider each curve in ACIS as a parametric curve that maps an interval of the
  * real line into a 3D vector space (object space). The mapping is continuous and one-to-one,
  * except for closed curves.
  * <br><br>
  * Considered as a function of its parameter, a curve is assumed to have a continuous
  * first derivative whose length is bounded above and below by nonzero constants. There is
  * no hard and fast rule about the values of these bounds or about the rate of change of the
  * length of the derivative.
  * <br><br>
  * The <tt>curve</tt> class defines virtual functions having the following functionality:
  * <ul>
  * <li> Determine the parameter range of the curve.</li>
  * <li> Determine if the curve is closed or periodic.</li>
  * <li> Determine the position, tangent, and curvature at a point or parameter value on the curve.</li>
  * <li> Determine the closest point on the curve to a given position.</li>
  * <li> Determine the parameter value on the curve corresponding to a given point on the curve.</li>
  * <li> Split a curve at a parameter value.</li>
  * <li> Determine the type of the curve.</li>
  * <li> Print the curve data to a file.</li>
  * </ul>
  * @see BDY_GEOM_PLANE, blend_spl_sur, blend_support, bounded_curve, curve_law_data, rot_spl_sur, subset_int_cur, sum_spl_sur, surf_surf_int, sweep_spl_sur, wire_law_data, SPAinterval
  */
class DECL_KERN curve : public ACIS_OBJECT {

protected:

    // Any curve may be subsetted to a given parameter range.
/**
 * Range to which this <tt>curve</tt> is subsetted.
 */
    SPAinterval subset_range;

public:

    // The default constructor uses the whole of the underlying curve.
/**
 * Constructs a curve (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it.
 */
    curve();
    /**
     * C++ destructor, deleting a curve.
     * <br><br>
     * <b>Role:</b> Ensures a derived class destructor is consulted when destroying a curve.
     */
    virtual ~curve();	// Ensure any derived class destructor gets
    // a say when we destroy a curve.


//
// DEBUG FUNCTIONS:
//

// method to write scheme commands to file for visualization
// [in] pFp   - the file to write
// [in] Color - color of the entity drawn
//
/**
 * @nodoc
 */
    virtual void debug_scheme(FILE* pFp, int Color) const;

    // method to write scheme commands to file for visualization
    // [in] pFp   - the file to write
    // [in] Color - color of the entity drawn
    // [in] range - the relevant range of the curve to be plotted
    //
/**
 * @nodoc
 */
    virtual void debug_scheme_range(FILE* pFp, int Color, SPAinterval range) const;


    // Make a copy of the given curve.  This function is not virtual
    // so as to handle a NULL "this".
/**
 * Makes a copy of this <tt>curve</tt>.
 */
    curve* copy_curve() const;


    // Make a copy of the given curve. This is a pure virtual
    // function to ensure that each derived class defines its own.
/**
 * Makes a copy of this <tt>curve</tt>.
 * <br><br>
 * <b>Role:</b> This is a pure virtual function to ensure that each derived class
 * defines its own.
 */
    virtual curve* make_copy() const = 0;

    // The deep_copy method makes a copy without shared data
/**
 * Creates a copy of this object that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a
 * pointer to the copied item.
 * <br><br>
 * In a <i>deep</i> copy, all the information about the copied item is self-contained in a
 * new memory block. By comparison, a <i>shallow</i> copy stores only the first instance
 * of the item in memory, and increments the reference count for each copy.
 * <br><br>
 * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that have already
 * been deep copied. For example, a deep copy of a complex model results in self contained
 * data, but identical sub-parts within the model are allowed to share a single set of data.
 * <br><br>
 * @param pm
 * list of items within the object that are already deep copied.
 */
    virtual curve* deep_copy(pointer_map* pm = NULL) const = 0;

    // STI ROLL
/**
 * @nodoc
 */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
/**
 * @nodoc
 */
    virtual void minimize(minimize_helper*) {};

    // Test two curves for equality. This, like testing floating-point
    // numbers for equality, is not guaranteed to say "equal" for
    // effectively-equal curves, but is guaranteed to say "not equal"
    // if they are indeed not equal. The result can be used for
    // optimisation, but not where it really matters. The default
    // always says "not equal", for safety.
/**
 * Tests this <tt>curve</tt> for equality to another curve.
 * <br><br>
 * <b>Role:</b> This method is not guaranteed to return <tt>TRUE</tt> for
 * effectively-equal curves, but is guaranteed to return <tt>FALSE</tt> if the
 * curves are not equal. The result can be used for optimization. The default is
 * <tt>FALSE</tt>.
 * <br><br>
 * @param cur
 * curve to compare for equality.
 */
    virtual logical operator==(const curve& cur) const;
    /**
     * Tests this <tt>curve</tt> for inequality to another curve.
     * <br><br>
     * @param cur
     * curve to compare for inequality.
     */
    logical operator!=(const curve& cur) const {
        return !(*this == cur);
    }

    // Determine whether a curve is entirely enclosed within
    // another. (used in merging)
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual logical operator>>(const curve&) const;

    // Transform a curve.
/**
 * Transforms this <tt>curve</tt>.
 * <br><br>
 * @param transf
 * transformation to apply.
 */
    virtual curve& operator*=(const SPAtransf& transf) = 0;


    // Return TRUE if the nth parameter-space curve is defined
    // (i.e. pcur() would return a non-NULL pcurve pointer),
    // FALSE otherwise.
/**
 * Determines if the <i>n</i>th parameter-space curve is defined for this <tt>curve</tt>.
 * <br><br>
 * @param n
 * integer denoting the parameter-space curve.
 */
    virtual logical pcur_present(int n) const;

    // Return nth parametric curve if this curve is defined with
    // respect to n or more surfaces and the nth is parametric, NULL
    // if not. If the argument is negative, then return the pcurve
    // corresponding to the absolute value of the argument, but
    // reversed in sense.
/**
 * Returns the <i>n</i>th parametric curve.
 * <br><br>
 * <b>Role:</b> If this <tt>curve</tt> is defined with respect to <i>n</i> or more surfaces
 * and the <i>n</i>th is parametric, this method returns the corresponding <tt>pcurve</tt>;
 * otherwise, <tt>NULL</tt> is returned. If the argument is negative, this method
 * returns the <tt>pcurve</tt> corresponding to the absolute value of the argument,
 * but reversed in sense.
 * <br><br>
 * @param n
 * integer denoting the parameter-space curve.
 * @param copy_curve
 * flag indicating whether the pcurve returned is temporary - if
 * so, the bs2_curve will not be copied, improving performance.
 */
    virtual pcurve* pcur(int n,
        logical copy_curve = FALSE
    ) const;

    // Reverse the sense of the curve.
/**
 * Reverses the sense of this <tt>curve</tt>.
 */
    virtual curve& negate() = 0;

    // Indicate whether the curve is properly-defined or not. A NULL
    // or generic curve is always undefined - other curves depend on
    // their contents.
/**
 * Indicates whether this <tt>curve</tt> is properly defined.
 * <br><br>
 * <b>Role:</b> A <tt>NULL</tt> or generic curve is always undefined - other curves depend on
 * their contents.
 */
    logical undefined() const;
    /**
     * Indicates whether this <tt>curve</tt> is properly defined.
     * <br><br>
     * <b>Role:</b> A <tt>NULL</tt> or generic curve is always undefined - other curves depend on
     * their contents.
     */
    virtual logical undef() const;


    // Split curve at given parameter value, if the curve is
    // splittable (in practice one defined or approximated by one
    // or more splines). Returns a new curve for the low-parameter
    // part, and the old one as the high-parameter part. For a
    // non-splittable curve, leaves the old one alone and returns
    // NULL. The default is to make the curve non-splittable, which
    // is always safe - this is only a speed optimisation.
/**
 * Splits this <tt>curve</tt> at the given parameter value, if possible.
 * <br><br>
 * <b>Role:</b> If the curve is splittable, constructs a new curve coincident with <i>and</i>
 * with the same parameterization as the given curve and modifies the given curve to
 * represent only the remainder of the curve. If the curve cannot be split, returns <tt>NULL</tt>.
 * The default behavior is for the curve to be nonsplittable.
 * <br><br>
 * @param param
 * parameter value at which to split.
 * @param pos
 * position on the curve.
 */
    virtual curve* split(
        double param,
        const SPAposition& pos = *(SPAposition*)NULL_REF
    );


    // Construct a subsetted copy.

    // STI let: temporarily change back to a non-virtual function.
    // This prevents intcurves referenced as curves from "accidentally"
    // getting the intcurve::subset() method.

//	virtual curve *subset(
/**
 * Constructs a subsetted copy of this <tt>curve</tt>.
 * This method constructs a new curve on the heap and it is the responsibility
 * of the caller to provide for the destruction of this curve.
 * <br><br>
 * @param range
 * the subset range.
 */
    curve* subset(const SPAinterval& range) const;

    // Subset a curve in place
/**
 * Subsets this <tt>curve</tt> in place, ensuring canonical results if the underlying curve is bounded or periodic.
 * <br><br>
 * @param range
 * subset range.
 */
    void limit(const SPAinterval& range);

    // Construct a copy of the unbounded curve underlying this one.
/**
 * Constructs a copy of the unbounded curve underlying this one.
 */
    curve* unsubset() const;

    // Remove the subsetting from this curve.
/**
 * Removes the parameter limits from this <tt>curve</tt>.
 */
    void unlimit();

    // Indicate whether the curve has a significant subset range.
/**
 * Indicates whether this <tt>curve</tt> has a significant subset range.
 */
    logical subsetted() const;

    // Return a box around the curve.
/**
 * Finds a box around this <tt>curve</tt>, or portion thereof, bounded by points on the curve increasing in parameter value.
 * <br><br>
 * <b>Role:</b> The points lie on the curve as supplied, not as transmitted. The
 * resulting box is not necessarily the minimal one.
 * <br><br>
 * @param pos1
 * first bounding position.
 * @param pos2
 * second bounding position.
 * @param transf
 * optional transformation.
 */
    virtual SPAbox bound(
        const SPAposition& pos1,
        const SPAposition& pos2,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const = 0;

    /**
     * Returns a box surrounding the portion of this <tt>curve</tt> between two parameter values.
     * <br><br>
     * <b>Role:</b> The resulting box is not necessarily the minimal one.
     * <br><br>
     * @param range
     * the range of interest.
     * @param transf
     * optional transformation.
     */
    virtual SPAbox bound(
        const SPAinterval& range,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const = 0;

    /**
     * Returns an object space bounding box surrounding the portion of this <tt>curve</tt> within the given box.
     * <br><br>
     * <b>Role:</b> There is no guarantee that the box will be minimal.
     * <br><br>
     * @param region
     * the region of interest.
     * @param transf
     * optional transformation.
     */
    virtual SPAbox bound(
        const SPAbox& region,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const = 0;

    // The following is retained for historical reasons, but will be
    // withdrawn.
/**
 * Retained temporarily for historical reasons.
 * <br><br>
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param transf
 * transformation.
 */
    SPAbox bound(
        double start,
        double end,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const
    {
        return bound(SPAinterval(start, end), transf);
    }


    // Return a cone bounding the tangent direction of a curve. The
    // cone is deemed to have its apex at the origin, and has a given
    // axis direction and (positive) half-angle. If the logical
    // argument is TRUE, then a quick approximation may be found. The
    // approximate result may lie wholly within or wholly outside the
    // guaranteed bound (obtained with a FALSE argument), but may not
    // cross from inside to outside. Flags in the returned object
    // indicate whether the cone is in fact the best available, and
    // if not whether this result is inside or outside the best cone.
/**
 * Returns a cone bounding the tangent direction of this <tt>curve</tt>.
 * <br><br>
 * <b>Role:</b> The cone is deemed to have its apex at the origin and have a given
 * axis direction and (positive) half angle. If the logical argument is <tt>TRUE</tt>,
 * then a quick approximation may be found. The approximate result may lie wholly within
 * or wholly outside the guaranteed bound (obtained with a <tt>FALSE</tt> argument),
 * but may not cross from inside to outside. Flags in the returned object indicate whether
 * the cone is in fact the best available and, if not, whether this result is inside
 * or outside the best cone.
 * <br><br>
 * @param range
 * the given range.
 * @param approx_OK
 * approximate result is ok.
 * @param transf
 * transformation to apply.
 */
    virtual curve_tancone tangent_cone(
        const SPAinterval& range,
        logical approx_OK = FALSE,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const = 0;


    // Position-based enquiries, implemented for all curves, but
    // with an extra argument to speed up processing for parametric
    // ones by allowing the parameter value of a point near the
    // desired point to be given, if known. Each specific curve type
    // must implement these functions, so they are declared to be
    // pure virtual functions.


    // Find tangent direction of curve at given point.
/**
 * Finds tangent direction of this <tt>curve</tt> at the given point.
 * <br><br>
 * @param pos
 * position of the point at which the tangent direction is to be found.
 * @param param_guess
 * parameter guess.
 */
    virtual SPAunit_vector point_direction(
        const SPAposition& pos,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF
    ) const = 0;


    // Find the curvature of the curve at given point.
/**
 * Finds the curvature of this <tt>curve</tt> at the given point.
 * <br><br>
 * @param pos
 * position of the point at which the curvature is to be found.
 * @param param_guess
 * parameter guess.
 */
    virtual SPAvector point_curvature(
        const SPAposition& pos,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF
    ) const = 0;


    // Find the foot of the perpendicular from the given point to
    // the curve, and optionally the tangent to and curvature of the
    // curve at that point, and its parameter value. If an input
    // parameter value is supplied (as the first parameter argument),
    // the perpendicular found is the one nearest to the supplied
    // parameter's position, otherwise it is the one at which the curve
    // is nearest to the given point. Any of the return value
    // arguments may be a NULL reference, in which case it is simply
    // ignored.
/**
 * Finds the foot of the perpendicular from the given point to this <tt>curve</tt>, the tangent and curvature of the curve at that point, and the corresponding parameter value.
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
 * @param tangent
 * the returned tangent.
 * @param curvature
 * the curvature at the curve position.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * the actual parameter.
 * @param f_weak
 * weak flag.
 */
    virtual void point_perp(
        const SPAposition& point,
        SPAposition& foot,
        SPAunit_vector& tangent,
        SPAvector& curvature,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& actual_param = *(SPAparameter*)NULL_REF,
        logical f_weak = FALSE
    ) const = 0;
    /**
     * Finds the foot of the perpendicular from the given point to this <tt>curve</tt>, the tangent to the curve at that point, and the corresponding parameter value.
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
     * @param tangent
     * the returned tangent.
     * @param param_guess
     * parameter guess.
     * @param param_actual
     * the actual parameter.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        const SPAposition& point,
        SPAposition& foot,
        SPAunit_vector& tangent,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF,
        logical f_weak = FALSE
    ) const;
    /**
     * Finds the foot of the perpendicular from the given point to this <tt>curve</tt> and the corresponding parameter value.
     * <br><br>
     * <b>Role:</b> And its parameter value. If an input parameter value is supplied
     * (as the first parameter argument), the perpendicular found is the one nearest
     * to the supplied parameter position; otherwise, it is the one that the curve is
     * nearest to the given point. Any of the return value arguments may be a <tt>NULL</tt> reference,
     * in which case it is simply ignored.
     * <br><br>
     * @param point
     * the input position.
     * @param foot
     * the position on the curve.
     * @param param_guess
     * parameter guess.
     * @param param_actual
     * the actual parameter.
     * @param f_weak
     * weak flag.
     */

    void point_perp(
        const SPAposition& point,
        SPAposition& foot,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF,
        logical f_weak = FALSE
    ) const;


    // Find the closest point on the curve, the foot, to the given point,
    // and optionally its parameter value. If an input parameter value is
    // supplied (as the first parameter argument), the foot found is only
    // a local solution nearest to the supplied parameter's position. Any
    // of the return value arguments may be a NULL reference, in which
    // case it is simply ignored.
/**
 * Finds the closest point on this <tt>curve</tt> (the foot) to the given point.
 * <br><br>
 * <b>Role:</b> Optionally finds its parameter value as well. If an input parameter
 * is supplied (as the first <tt>SPAparameter</tt> argument), the foot found is only the local
 * solution nearest to the supplied parameter's position. Any of the return value
 * arguments may be a <tt>NULL</tt> reference, in which case it is simply ignored.
 * <br><br>
 * @param pos
 * position whose closest curve point is desired.
 * @param foot
 * closest position on the curve.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * actual parameter.
 */
    virtual void closest_point(
        const SPAposition& pos,
        SPAposition& foot,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
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

    // Parameter-based enquiries. Unlike surfaces, curves are always
    // parametrised, even if they have an implicit form. The parameter
    // range should be a continuous subset of the real line, possibly
    // infinite or semi-infinite. Considered as a function of its
    // parameter, the curve is assumed to have a continuous first
    // derivative, whose length is bounded above and below by non-zero
    // constants. There is no hard and fast rule about the values of
    // these bounds, or about the rate of change of the length of the
    // derivative, but there is an informal inverse relationship
    // between the reliability of the modeller and the magnitude of
    // the ratio between the high and low bounds and of the rate of
    // change.

    // The fundamental parameter-based functions are pure virtual,
    // to ensure that each derived class defines its own versions.
    // Others are defined in the base class in terms of the fundamental
    // ones - a derived class may use the default, but it will normally
    // be advantageous in terms of speed and possibly precision for
    // it to define its own version.

    // Find the parameter value of a point on a curve, corresponding
    // to the given point. Only guaranteed to be valid for points on
    // the curve, though particular curve types may give useful
    // curve-dependent results for other points.
/**
 * Finds the parameter value of a given position on the curve.
 * <br><br>
 * @param pos
 * the position for which the parameter value is to be found.
 * @param param_guess
 * parameter guess.
 */
    virtual double param(
        const SPAposition& pos,
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF
    ) const = 0;


    // For each of the six enquiry routines below, there are two
    // optional logical arguments. The first, if TRUE, is a guarantee
    // from the calling code that the most recent call to any curve
    // or surface member function was in fact to one of these four
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

    // Evaluate a curve at a given parameter value, giving SPAposition,
    // and first and second derivatives (all optionally).
/**
 * Evaluates this <tt>curve</tt> at a given parameter value, returning the position and the first and second derivatives (all optionally).
 * <br><br>
 * <b>Role:</b> The first logical argument, if <tt>TRUE</tt>, is a guarantee from the calling code
 * that the most recent call to any curve or surface member function was in fact to the
 * routine for the same curve as the current call. It allows an implementation to cache
 * useful intermediate results to speed up repeated calculations, but must be used with
 * extreme care.
 * <br><br>
 * The second logical argument may be set to <tt>TRUE</tt> if an approximate return value
 * is acceptable. Here "approximate" is not well-defined, but may be assumed to be
 * sufficient for visual inspection of the curve.
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
        double param,
        SPAposition& pos,
        SPAvector& first_deriv = *(SPAvector*)NULL_REF,	// first derivative
        SPAvector& second_deriv = *(SPAvector*)NULL_REF,	// second derivative
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;


    // Find the point on a curve corresponding to a given parameter
    // value. Defaults to using eval().
/**
 * Finds the point on this <tt>curve</tt> corresponding to a given parameter value.
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
        double param,
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;

    // Find the derivative (direction and magnitude) at the given
    // parameter value on the curve. Defaults to using eval().
/**
 * Finds the derivative (direction and magnitude) at the given parameter value on this <tt>curve</tt>.
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
        double param,
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;


    // Find the tangent direction at the given parameter value on
    // the curve. Default uses eval_deriv().
/**
 * Finds the tangent direction at the given parameter value on this <tt>curve</tt>.
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
    virtual SPAunit_vector eval_direction(
        double param,
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;


    // Find the magnitude of the derivative at the given parameter
    // value on the curve. Defaults to taking the length of the
    // derivative explicitly, but some curve types may allow quicker
    // evaluation.
/**
 * Finds the magnitude of the derivative at the given parameter value on this <tt>curve</tt>.
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
        double param,
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;

    // Find the curvature at the given parameter value on the curve.
    // Defaults to using eval().
/**
 * Finds the curvature at the given parameter value on this <tt>curve</tt>.
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
        double param,
        logical eval_repeated = FALSE,
        logical approx_ok = FALSE
    ) const;


    // Find regions of high curvature of the curve.  Given a curvature value k,
    // an array of intervals is returned over which the curvature exceeds k.
    // The function value returns the number of intervals.
/**
 * Finds regions of high curvature of this <tt>curve</tt>.
 * <br><br>
 * <b>Role:</b> This method creates an array of intervals in the <tt>spans</tt> argument,
 * to signify ranges over which the curvature exceeds the given value, <tt>k</tt>. It returns the number
 * of intervals in the array. The array of intervals is created on the heap and it is the responsibility
 * of the caller to provide for the destruction of this array.
 * <br><br>
 * @param k
 * given curvature value.
 * @param spans
 * the array of intervals.
 */
    virtual	int	high_curvature(double k, SPAinterval*& spans) const;


    // Return a distance value, which represents the greatest
    // discrepancy between positions calculated by calls to eval() or
    // eval_position() with the "approx_OK" logical set by turns to
    // TRUE and FALSE. Defaults to returning 0, for curves which
    // do not distinguish between these cases.
/**
 * Returns the maximum error between the approximate evaluation of this <tt>curve</tt> and the true evaluation of it.
 */
    virtual double approx_error() const;


    // The evaluate() function calculates derivatives, of any order
    // up to the number requested, and stores them in vectors provided
    // by the user. It returns the number it was able to calculate;
    // this will be equal to the number requested in all but the most
    // exceptional circumstances. A certain number will be evaluated
    // directly and (more or less) accurately; higher derivatives will
    // be automatically calculated by finite differencing; the accuracy
    // of these decreases with the order of the derivative, as the
    // cost increases.
/**
 * Calculates derivatives, of any order up to the number requested, and store them in vectors provided by the user.
 * <br><br>
 * <b>Role:</b> This function returns the number it was able to calculate;
 * this is equal to the number requested in all but the most exceptional
 * circumstances. A certain number are evaluated directly and (more or less)
 * accurately; higher derivatives are automatically calculated by finite
 * differencing. The accuracy of these decreases with the order of the derivative,
 * as the cost increases.
 * <br><br>
 * Any of the <tt>derivs</tt> pointers may be <tt>NULL</tt>, in which case the corresponding
 * derivative will not be returned.
 * <br><br>
 * @param param
 * the parameter at which the curve is to be evaluated.
 * @param pos
 * point on the curve at the given parameter.
 * @param derivs
 * pointer arrays of derivatives.
 * @param num
 * the number of derivatives required.
 * @param side
 * the evaluation location - above, below, or "don't care".
 */
    virtual int evaluate(
        double param,				// Parameter
        SPAposition& pos,			// Point on curve at given SPAparameter
        SPAvector** derivs = NULL, 	// Array of pointers to vectors, of
        // size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivative will not
        // be returned.
        int num = 0,       		// Number of derivatives required (nd)
        evaluate_curve_side side = evaluate_curve_unknown
        // the evaluation location - above,
        // below or don't care.
    ) const;

    // The evaluate_iter() function is just like evaluate(), but is
    // supplied with a data object which contains results from a previous
    // close evaluation, for use as initial values for any iteration
    // involved. The default implementation simply ignores this value
    // and calls evaluate().
/**
 * Calculates derivatives, of any order up to the number requested, in an iterative fashion.
 * <br><br>
 * <b>Role:</b> The purpose of this method is the same as that of <tt>evaluate</tt>, but
 * <tt>evaluate_iter</tt> is additionally supplied with a data object that contains results
 * from a previous close evaluation, for use as initial values for any iteration
 * involved. This object may furthermore be updated to reflect the results of this
 * evaluation. The default implementation simply ignores this object and calls
 * <tt>evaluate</tt>.
 * <br><br>
 * @param param
 * the parameter at which the curve is to be evaluated.
 * @param ini_data
 * data supplying initial values.
 * @param pos
 * point on the curve at the given parameter.
 * @param derivs
 * pointer arrays of derivatives.
 * @param num
 * the number of derivatives required.
 * @param side
 * the evaluation location - above, below, or "don't care".
 */
    virtual int evaluate_iter(
        double param,				// Parameter
        curve_evaldata* ini_data,	// Data supplying initial values, and
        // set to reflect the results of this
        // evaluation.
        SPAposition& pos,			// Point on curve at given SPAparameter
        SPAvector** derivs = NULL, 	// Array of pointers to vectors, of
        // size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivative will not
        // be returned.
        int num = 0,       		// Number of derivatives required (nd)
        evaluate_curve_side side = evaluate_curve_unknown
        // the evaluation location - above,
        // below or don't care.
    ) const;

    // Construct a data object to retain evaluation information across
    // calls to evaluate_iter(). This is to allow subsidiary calls
    // within an iterative evaluator to start iteration much closer to the
    // required result than is possible just using the curve information
    // itself. The default returns NULL, indicating that no special
    // information is required or usable.
/**
 * Constructs a data object to retain evaluation information across calls to the <tt>evaluate_iter</tt> method.
 * <br><br>
 * <b>Role:</b> This is to allow subsidiary calls within an iterative evaluator to start
 * iteration much closer to the required result than is possible just using the curve
 * information itself. The default returns <tt>NULL</tt>, indicating that no special information
 * is required or usable.
 */
    virtual curve_evaldata* make_evaldata() const;

    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the curve. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_CURVE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.

/**
 * @nodoc
 */
#define ALL_CURVE_DERIVATIVES 9999

 /**
  * Returns the number of derivatives of this <tt>curve</tt> that can be found accurately and relatively directly.
  * <br><br>
  * <b>Role:</b> Returns the number of derivatives that can be found by <tt>evaluate</tt>
  * accurately and relatively directly, rather than by finite differencing over the given
  * portion of the curve. If there is no limit to the number of accurate derivatives,
  * returns the value <tt>ALL_CURVE_DERIVATIVES</tt>.
  * <br><br>
  * @param range
  * portion of the curve to be considered (optional).
  */
    virtual int accurate_derivs(
        const SPAinterval& range = *(SPAinterval*)NULL_REF
        // Defaults to the whole curve
    ) const;


    // Evaluate higher derivatives than are available accurately in
    // evaluate(), by finite differencing. This function is available
    // to any derived class for use in its own evaluate(). It calls
    // back the evaluate() function for adjacent points (separated by
    // the value dparam) to evaluate nfound derivatives, so evaluate()
    // must ensure that this will not cause a further call to
    // this function.

protected:
    /**
     * Evaluate higher derivatives than are available accurately in <tt>evaluate</tt> by finite differencing.
     * <br><br>
     * <b>Role:</b> Any of the <tt>derivs</tt> pointers may be <tt>NULL</tt>, in which case the
     * corresponding derivative will not be returned.
     * <br><br>
     * @param param
     * the parameter at which the curve is to be evaluated.
     * @param pos
     * point on the curve at the given parameter.
     * @param derivs
     * pointer arrays of derivatives.
     * @param nd
     * the number of derivatives required.
     * @param nfound
     * the number of derivatives already evaluated, and directly evaluable in the neighborhood of the param.
     * @param dt
     * the finite differencing step to use.
     * @param side
     * the evaluation location - above, below, or "don't care".
     */
    virtual int finite_difference_derivatives(
        double param,		        // Parameter
        SPAposition& pos,	        // Point on curve at given SPAparameter
        SPAvector** derivs,         // Array of pointers to vectors, of
        // size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivative will not
        // be returned.
        int nd,	       		        // Number of derivatives required (nd)
        int nfound,				    // Number of derivatives already
        // evaluated, and directly evaluable
        // in the neighbourhood of the param
        // (nfound)
        double dt,				    // finite differencing step to use
        evaluate_curve_side side	// the evaluation location - above,
        // below or don't care.
    ) const;

    /**
     * @nodoc
     */
    virtual void step_from_guess(
        const SPAposition& pos,
        double& param_wanted
    ) const;

    /**
     * @nodoc
     */
    virtual void point_perp_internal(
        const SPAposition& point,
        double param_wanted,
        double& param_best,
        SPAposition& foot_best,
        SPAunit_vector& tan_best,
        SPAvector& foot_dt_best,
        SPAvector& foot_ddt_best,
        logical linear_search,
        double tol,
        logical& point_found
    ) const;

public:
    // Indicate whether a curve is periodic, that is joins itself
    // smoothly at the ends of its principal parameter range, so that
    // edges may span the "seam".
/**
 * Indicates if this <tt>curve</tt> is periodic.
 * <br><br>
 * <b>Role:</b> If so, it joins itself smoothly at the ends of its principal parameter range,
 * so that <tt>EDGEs</tt> span the seam.
 */
    virtual logical periodic() const = 0;

    // Indicate whether a curve is closed, that is joins itself
    // (smoothly or not) at the ends of its principal parameter
    // range. Note that this function should always return TRUE
    // if periodic() does.
/**
 * Indicates whether this <tt>curve</tt> is closed.
 * <br><br>
 * <b>Role:</b> A closed curve joins itself (smoothly or not) at the ends of its
 * principal parameter range. This function should always return <tt>TRUE</tt> if <tt>periodic</tt>
 * returns <tt>TRUE</tt>.
 */
    virtual logical closed() const = 0;

    // Return the period of a periodic curve, zero if the curve
    // is not periodic.
/**
 * Returns the period of a periodic curve, or 0 if this <tt>curve</tt> is not periodic.
 */
    virtual double param_period() const = 0;


    // Return the principal parameter range of a curve. A periodic
    // curve is defined for all parameter values, by reducing the
    // given parameter modulo the period into this principal range.
    // For an open unbounded curve, the principal range is
    // conventionally the empty interval. For bounded open or non-
    // periodic curves the curve evaluation functions are defined
    // only for parameter values in this range.
    // If a region of interest is provided, a valid range is always
    // returned, even for an unbounded curve, representing a portion
    // of the curve which is guaranteed to include all segments which
    // lie within the region of interest.
/**
 * Returns the principal parameter range of this <tt>curve</tt>.
 * <br><br>
 * <b>Role:</b> The definition of a periodic curve is extended to all parameter values
 * by reducing the given parameter modulo the period into this principal range.
 * For an open unbounded curve, the principal range is conventionally the empty
 * interval. For bounded open or nonperiodic curves the curve evaluation functions
 * are defined only for parameter values in this range. If a region of interest is
 * provided, a valid range is always returned, even for an unbounded curve,
 * representing a portion of the curve that is guaranteed to include all segments
 * that lie within the region of interest.
 * <br><br>
 * @param region
 * region of interest.
 */
    virtual SPAinterval param_range(
        const SPAbox& region = *(SPAbox*)NULL_REF
    ) const = 0;


    // Arc-length. Return the algebraic distance along the curve
    // between the given parameters, the sign being positive if the
    // parameter values are given in increasing order, and negative
    // if they are in decreasing order. The result is undefined if
    // either parameter value is outside the SPAparameter range of a
    // bounded curve. For a periodic curve the parameters are not
    // reduced to the principal range, and so the portion of the
    // curve evaluated may include several complete circuits. This
    // function is therefore always a monotonically increasing
    // function of its second argument if the first is held constant,
    // and a decreasing function of its first argument if the second
    // is held constant.
/**
 * Returns the algebraic distance (arc length) along this <tt>curve</tt> between the two given parameters.
 * <br><br>
 * <b>Role:</b> The sign is positive if the parameter values are given in increasing
 * order and negative if they are in decreasing order.
 * <br><br>
 * @param param1
 * first parameter bounding the arc.
 * @param param2
 * second parameter bounding the arc.
 * @param approx_ok
 * If approx_ok is true, and the curve has a B-spline approximation, then this approximation is used in the length calculation.
 */
    virtual double length(
        double param1,
        double param2,
        logical approx_ok = TRUE
    ) const = 0;


    // The inverse of the length function. Return the parameter value
    // of the point on the curve at the given algebraic arc length
    // from that defined by the datum parameter. The result is not
    // defined for a bounded non-periodic curve if the datum parameter
    // is outside the parameter range, or if the length is outside
    // the range bounded by the values for the ends of the parameter
    // range.
/**
 * Returns the parameter value of the point on this <tt>curve</tt> at the given algebraic arc length from that defined by the given parameter.
 * <br><br>
 * <b>Role:</b> This method is the inverse of the <tt>length</tt> method. The result is not
 * defined for a bounded nonperiodic curve if the <tt>param</tt> argument is outside the
 * parameter range, or if the <tt>length</tt> argument is outside the range bounded by the
 * values for the ends of the parameter range.
 * <br><br>
 * @param param
 * datum parameter value.
 * @param length
 * arc length.
 * @param approx_ok
 * If approx_ok is true, and the curve has a B-spline approximation, then this approximation is used in the length calculation.
 */
    virtual double length_param(
        double param,			// datum parameter
        double length,		// arc length
        logical approx_ok = TRUE
    ) const = 0;


    // Find the extrema of a curve in a given direction.
/**
 * Finds the extrema of this <tt>curve</tt> in a given direction.
 * <br><br>
 * @param dir
 * direction in which to find the extrema.
 */
    virtual curve_extremum* find_extrema(const SPAunit_vector& dir) const = 0;

    // Test point-on-curve, optionally returning the exact parameter
    // value if the point is on the curve. Tests to standard system
    // precision.
/**
 * Tests point-on-curve, optionally returning the exact parameter value if the point is on this <tt>curve</tt>.
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
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const
    {
        return test_point_tol(pos, 0, param_guess, param_actual);
    }

    // Test point-on-curve, optionally returning the exact parameter
    // value if the point is on the curve. Tests to given precision.
/**
 * Tests point-on-curve to a given precision, optionally returning the exact parameter value if the point is on this <tt>curve</tt>.
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
        const SPAparameter& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const = 0;

    // Return a cylinder which encloses the portion of the curve
    // bounded by the given parameter range.
/**
 * Returns a cylinder that encloses the portion of this <tt>curve</tt> bounded by the given parameter interval.
 * <br><br>
 * @param range
 * region of interest.
 */
    virtual curve_boundcyl enclosing_cylinder(
        const SPAinterval& range = *(SPAinterval*)NULL_REF
    ) const = 0;


    // Return (in a read-only array) the number and parameter values of
    // discontinuities of the curve, of the given order (maximum three).
/**
 * Returns the number and parameter values of discontinuities of this <tt>curve</tt>, of the given order (maximum three), in a read-only array.
 * <br><br>
 * @param n_discont
 * number of discontinuities found.
 * @param order
 * order of the discontinuities.
 */
    virtual	const double* discontinuities(int& n_discont, int order) const;


    // Return (in a read-only array) the number and parameter values of
    // discontinuities of the curve, up to the given order (maximum three).
/**
 * Returns the number and parameter values of all discontinuities of this <tt>curve</tt>, up to the given order (maximum three).
 * <br><br>
 * <b>Role:</b> The returned array is read-only.
 * @param n_discont
 * number of discontinuities found.
 * @param order
 * maximum order of the discontinuities.
 */
    virtual	const double* all_discontinuities(int& n_discont, int order);


    // State whether a particular parameter value is a discontinuity, and if so,
    // return the order of the discontinuity.
/**
 * Determines whether a particular parameter value represents a discontinuity of this <tt>curve</tt>.
 * <br><br>
 * <b>Role:</b> If there is a discontinuity at the given parameter value,
 * returns the order of the discontinuity. Otherwise, returns 0.
 * <br><br>
 * @param t
 * parameter value to test.
 */
    virtual int discontinuous_at(double t) const;

    // Return read-only access to a disc_info object, if there is one.  The
    // default version of the function returns null.
/**
 * Returns read-only access to a <tt>discontinuity_info</tt> object, if there is one.
 * <br><br>
 * <b>Role:</b> The default version of this method returns <tt>NULL</tt>.
 */
    virtual const discontinuity_info& get_disc_info() const;

    // Returns FALSE.  Derived classes with disc_info objects should return
    // TRUE if a calculation was successfully performed.  Their calculations
    // should be "deep" in the sense that they should recalculate the disc_info's
    // of any geometries upon which the curve in question depends.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual logical deep_calc_disc_info();


    // Return a law pointer that is the same a the curve or
    // else a NULL pointer.
/**
 * Returns a pointer to the law form of this <tt>curve</tt>, or else <tt>NULL</tt>.
 */
    virtual law* law_form() { return NULL; }

    // Return an identifier uniquely specifying the curve type
/**
 * Returns an identifier that specifies the curve type.
 */
    virtual int type() const = 0;


    // Return a string identifying the curve type
/**
 * Returns the string <tt>"curve"</tt>.
 */
    virtual char const* type_name() const = 0;


    // Save and restore. Save is easy, as derived class switching goes
    // through the normal virtual function mechanism. Restore is more
    // complicated, because until it is invoked we do not have an
    // object of the right class. Instead we switch on a table defined
    // by static instances of the restore_cu_def class (see below), to
    // invoke a simple friend function which constructs an object of
    // the right (derived) type. Then it can call the appropriate
    // member function to do the actual work. The save_data and
    // restore_data function for each class can be called in
    // circumstances when we know what type of curve we are expecting
    // and have one in our hand to be filled in.

    // Function to be called to save a curve of unknown type, or
    // NULL. Just checks for null, then calls save().
/**
 * Saves a curve of unknown type, or <tt>NULL</tt>.
 * <br><br>
 * <b>Role:</b> Simply checks for <tt>NULL</tt>, then calls <tt>save</tt>.
 */
    void save_curve() const;

    // Function to save a curve of unknown type, together with its type
    // code for later retrieval.
/**
 * Saves a curve of unknown type, together with its type code for later retrieval.
 */
    virtual void save() const = 0;

    // Function to save a curve of a known type, where the context
    // determines the curve type, so no type code is necessary. The
    // base class version just saves the subset_range member - for
    // convenience it can be called by derived class versions.
/**
 * Saves a curve of a known type.
 * <br><br>
 * <b>Role:</b> The context determines the curve type, so that no type code is necessary.
 * The base class version simply saves the <tt>subset_range</tt> member - for convenience,
 * it can be called by derived class versions.
 */
    void save_data() const;

    /**
     * Queries whether the object needs to be saved to an old version as an approximation.
     * <br><br>
     * <b>Role:</b>
     *    Called by SAT save APIs with <tt>check_progenitors == TRUE</tt> to determine if an object
     *    needs to be tolerantly simplified before being saved to an old version.  The
     *    <tt>check_progenitors</tt> flag indicates that the object should call <tt>need_save_as_approx</tt>
     *    on any curve or surface construction geometry upon which it depends, and return
     *    <tt>TRUE</tt> if any of the progenitors do.  Customers with their own derived curve or surface types
     *    should override this routine to return <tt>TRUE</tt> whenever they want their geometry to be imported
     *    (as a B-spline approximation) into other applications or when saving to a
     *    version of their application before the derived geometry type in question existed.
     * @param save_to_version
     * ACIS version to which the object will be saved
     * @param check_progenitors
     * if <tt>TRUE</tt>, query construction geometry and base class as well
     */
    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

    // Function to be called to restore a curve of unknown type -
    // reads the type code, and switches to curve-specific restore
    // code.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN curve* restore_curve();

    // Function to restore the data for a curve of known type, either
    // where the context determines the type, or after the type code
    // has been read and processed. The base class version just restores
    // the subset_range member - for convenience it can be called by
    // derived class versions.
/**
 * Restores the data for this <tt>curve</tt>, according to its type.
 * <br><br>
 * <b>Role:</b> The base class version of this method only restores the
 * <tt>subset_range</tt> member. For convenience, it can be called by derived class
 * versions.
 * <br><br>
 * The restore operation switches on a table defined by static instances. This
 * invokes a simple friend function which constructs an object of the right derived
 * type. Then it calls the appropriate base class member function to do the actual
 * work. The <tt>restore_data</tt> function for each class can be called in circumstances
 * when it is known what type of curve is to be expected and a curve of that type is
 * on hand to be filled in.
 * <pre>
 *    if (restore_version_number &gt;= BNDCUR_VERSION)
 *       read_interval		Interval for the subset range. </pre>
 */
    void restore_data();


    // Notify the derived type that the curve has been changed (e.g. the
    // subset_range has changed) so that it can update itself.  The default
    // version of the function does nothing.
/**
 * Notifies the derived type that this <tt>curve</tt> has been changed.
 * <br><br>
 * <b>Role:</b> Used when the <tt>subset_range</tt> has changed, so that the curve
 * can update itself. The default version of this method does nothing.
 */
    virtual	void change_event();

    // Check for any data errors in the curve, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // curve on exit.  The default version of this function does nothing.
    // See chk_stat.hxx for information on the argument types used here.
/**
 * Checks for any data errors in this <tt>curve</tt>, and corrects the errors if possible.
 * <br><br>
 * <b>Role:</b> The various arguments provide control over which checks are made,
 * which fixes can be applied and which fixes were actually applied. The function
 * returns a list of errors that remain in the curve on exit.
 * <br><br>
 * The default for allowed fixes is none (nothing fixed). If the list of checks
 * to be made is null, then every possible check will be made. Otherwise, the function
 * will only check for things in the list. The return value for the function will then
 * be a subset of this list.
 * <br><br>
 * @param input
 * set of flags for fixes allowed.
 * @param result
 * fixes applied.
 * @param chks
 * checks to be made (default is none).
 */
    virtual	check_status_list* check(
        const check_fix& input = *(const check_fix*)NULL_REF,
        // supplies a set of flags which say which fixes
        // are allowable (the default is to fix nothing)
        check_fix& result = *(check_fix*)NULL_REF,
        // returns a set of flags which say which fixes
        // were applied
        const check_status_list* chks = (const check_status_list*)NULL_REF
        // list of checks that are to be made.  If the
        // list is null, then every possible check will
        // be made; otherwise, the function will only
        // check for things in the list.  The return
        // value for the function will then be a subset
        // of this list.
    );


#if defined D3_STANDALONE || defined D3_DEBUG
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void input(D3_istream&);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void print(D3_ostream&) const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend DECL_KERN D3_istream& operator>>(
        D3_istream&,
        curve*&
        );
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend DECL_KERN D3_ostream& operator<<(
        D3_ostream&,
        const curve&
        );

#endif

    // Display a description of the curve.
/**
 * Writes a description of this <tt>curve</tt> to a file.
 * <br><br>
 * @param lead
 * leader string.
 * @param fp
 * FILE pointer.
 */
    virtual void debug(
        const char* lead,
        FILE* fp = debug_file_ptr
    ) const = 0;

public:
    SPAinterval gme_get_subset_range();
    void gme_set_subset_range(const SPAinterval& range);
};  // End of curve class definition
/** @} */

// Put this outside the class definition so as not to confuse mkman
/**
 * @nodoc
 */
inline curve::curve() : subset_range(interval_infinite) {}

// Subsidiary class to contain the return values from find_extrema.

/**
 * @nodoc
 */
enum extremum_type {
    extremum_unknown,
    extremum_minimum,
    extremum_maximum
};

/**
 * @nodoc
 */
class DECL_KERN curve_extremum : public ACIS_OBJECT {
public:
    curve_extremum* next;
    SPAparameter param;
    extremum_type type;

    curve_extremum(curve_extremum*, double, extremum_type);
};

// Restore mechanism for curves. Static declarations of objects
// of this class form themselves into a table containing the external
// (integer) identifier of the particular curve subtype, together
// with a pointer to the correct restore routine. The curve
// restore routine reads the external identifier, and switches
// according to the table. Note that the save routine doesn't need
// any of this stuff, as the virtual function mechanism works fine
// for that.

// The table will be very short (half a dozen entries at most), and
// so can be simply a linear list, for ease of implementation.
/**
 * @nodoc
 */
typedef curve* (*rest_cu_fn_type)();
/**
 * @nodoc
 */
class DECL_KERN restore_cu_def : public ACIS_OBJECT {
    restore_cu_def* next;	// list pointer
    int type;				// curve subtype (old style)
    char const* name;		// curve subtype (new style)
    rest_cu_fn_type rest_fn;// restore function
public:
    restore_cu_def(
        int,			// internal type number (for old
        // save files)
        char const*,	// external type identifier (for
        // new save files)
        rest_cu_fn_type
    );
    ~restore_cu_def();

    // Note that old save files have an integer curve type, while
    // new ones have a character string. Implement the dispatch
    // for both.

    friend DECL_KERN curve* dispatch_restore_cu(int);
    friend DECL_KERN curve* dispatch_restore_cu(char*);
};



// Define a different name for the bounding cylinder, which did not
// match our usual standards, but has been used variously.
/**
 * @nodoc
 */
#define BOUNDING_CYLINDER curve_boundcyl

 // Data carrier for spanning successive calls to the evaluator, as occur
 // within evaluations of other curves and surfaces. Curve types capable of
 // using such information declare a derived class containing the information
 // useful to that specific curve evaluator, including (probably) pointers
 // to curve_evaldata and surface_evaldata objects for support curves and
 // surfaces.
 /**
  * @nodoc
  */
class curve_evaldata : public ACIS_OBJECT {
public:
    double tol;			// set on entry to evaluate_iter() to the tolerance
    // required for this evaluation. Zero or negative to
    // mean "best tolerance available".

    double residue;		// set by evaluate_iter to a measure of the tolerance
    // actually achieved. This is taken to be the square
    // of a distance - an evaluator with subsidiary curves
    // and surfaces should sum all their residues together
    // with the squares of its own error distances to
    // produce its own residue. An evaluator which
    // calculates to numerical accuracy may return zero.

    logical init;		// FALSE initially, set to TRUE by the evaluator when
    // results from the evaluation have been set into
    // data members of the derived class. Determines
    // whether evaluate_iter() can use these data members
    // or must resort to intrinsic methods to start
    // iterations.

    curve_evaldata();
    virtual ~curve_evaldata();
};

#endif
