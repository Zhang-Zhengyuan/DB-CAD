/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( pcurve_CLASS )
#define pcurve_CLASS
/**
 * \defgroup ACISPCURVES pcurves
 * \ingroup ACISGEOMETRY
 *
 */
 /**
 * @file pcudef.hxx
  * @CAA2Level L1
  * @CAA2Usage U1
  * \addtogroup ACISPCURVES
  *
  * @{
  */
#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "bs2curve.hxx"
#include "debugmsc.hxx"
#include "subtrans.hxx"
#include "param.hxx"
class curve;
class intcurve;
class surface;
class par_cur;
class SPAtransf;
class SPAinterval;
class BOUNDED_SURFACE;
class D3_ostream;
class D3_istream;
class law;
class pointer_map;
class minimize_helper;
class SizeAccumulator;
class pcurve;
/**
 * Transforms a pcurve in object space.
 *<br><br>
 * @param cur
 * pcurve.
 * @param trans
 * transformation.
 */
DECL_KERN pcurve operator*(pcurve const& cur, SPAtransf const& trans);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN pcurve* restore_pcurve();
/**
 * Adds a <tt>SPApar_vec</tt> to a pcurve's offset.
 *<br><br>
 * @param pv
 * parameter vec.
 * @param pc
 * pcurve.
 */
DECL_KERN pcurve operator+(SPApar_vec const& pv, pcurve const& pc);

/**
 * Defines a <tt>2D</tt> curve defined in the parameter space of a parametric surface.
 * <br>
 * <b>Role:</b> The <tt>pcurve</tt> class represents parameter-space curves that map an interval of the
 * real line into a <tt>2D</tt> real vector space (parameter space). This mapping is continuous, and
 * one-to-one except possibly at the ends of the interval whose images may coincide. It is
 * differentiable twice, and the direction of the first derivative with respect to the parameter
 * is continuous. This direction is the positive sense of the curve.
 *<br><br>
 * A parameter-space curve is always associated with a surface, that maps the parameter-space
 * image into <tt>3D</tt> real space (object space); therefore, the two mappings together can be considered
 * to be a single mapping from a real interval into object space. Most of the properties of a
 * parameter-space curve relate in fact to this combined mapping.
 *<br><br>
 * If the two ends of the curve are different in object space, the curve is open. If they are the
 * same, it is closed. If the curve joins itself smoothly, the curve is periodic, with its period
 * being the length of the interval that it is primarily defined. A periodic curve is defined for
 * all parameter values, by adding a multiple of the period to the parameter value so the result
 * is within the definition interval, and evaluating the curve at that resultant parameter.
 * The point at the ends of the primary interval is known as the seam. If the surface is
 * periodic, a closed or periodic parameter-space curve cannot in fact be closed in parameter
 * space, but its end values can differ by the surface parameter period in one or both directions.
 *<br><br>
 * Also, a parameter-space curve is always associated with an object-space curve lying in (or
 * fitted to) the surface. This curve is used to assist in the determining of the surface
 * parameter values corresponding to object-space points on the <tt>3D</tt> curve, by using the parameter
 * value on the <tt>3D</tt> curve to evaluate the <tt>2D</tt> curve for an approximation to the surface parameter
 * values for iterative refinement. For this reason, a parameter-space curve must always have the
 * same parameter range as its associated object-space curve, and its internal parameterization
 * must be similar, though not necessarily identical, to that of the object-space curve. A
 * parameter-space curve can have the same sense as its associated object-space curve, or be
 * opposite. In the latter case, the parameterization is negated one to the other.
 *<br><br>
 * In general, it is not necessary to have u or v continuity between two pcurves on a periodic
 * face or on a singular face, because evaluation of a parameter value outside the parameter
 * range is mapped back into the evaluation range by adding or subtracting the period from the
 * parameter value to be evaluated (periodic face), or by adding or subtracting the parameter
 * value changes while the actual 3-space location is coincident (singular face).
 *<br><br>
 * In the case of a spline converted sphere,face 0 ranges from u = -pi/2 to pi/2 and v = 0 to pi;
 * face 1 ranges from u = -pi/2 to pi/2 and v = pi to 2*pi.
 * Coedges 0 and 2 belong to face 0, so their param ranges should match the corresponding face param ranges.
 * Coedge 0 has param values: u = -pi/2 to pi/2 and v = 0.
 * Coedge 2 has param values: u = pi/2 to -pi/2 and v = pi.
 * <br><br>
 * The param values are correct, because Coedge 0 travels from top vertex to bottom vertex, and
 * Coedge 2 travels from bottom vertex to top vertex on the sphere; as a result, u values are
 * continuous (-pi/2 to pi/2 and pi/2 to -pi/2) moving along Coedge 0, onto Coedge 2, and along Coedge 2.
 *<br> For the V direction (since face 0 is singular in v), v ranges from 0 to pi along
 * both singular poles of the hemispherical face, so v value may be any value between 0 and pi
 * for the given u param values, and should still evaluate to the same 3-space point; at
 * u = -pi/2 or u = pi/2, all v values map to (0, 0, 20) and (0, 0, -20) respectively. Since the
 * pcurves map to surface param values, they could use any value at endpoints. However, since pcurve 0 must travel along
 * entire edge of face 0, the v value must be pi along the entire length of pcurve 0 (only valid value at u values not
 * at singularity points in v). Similarly, pcurve 2 must have v value of 0 along entire length of pcurve 2.
 * Transition from pcurve 0 to pcurve 2 is, then, continuous in u, and discontinuous in v, but discontinuity in v matches
 * singularity in v at poles of hemispherical face 0; in addition, evaluation of any value of v (0 to pi) at poles gives
 * correct 3-space position. Similar discussion results if data for Coedge 1 and 3 is examined.
 *<br><br>
 * A pcurve consists of pointer to a <tt>par_cur</tt> that holds the data defining the <tt>2D</tt> parameter space
 * curve and a logical flag indicating reversal of the pcurve from the underlying spline curve.
 * In addition, a parameter space vector is stored that represents the displacement of this
 * pcurve in the parameter space of the surface the pcurve lies in. By having a nonzero vector
 * for a periodic surface, a continuous sequence of object space curves <tt>(3D curves)</tt> can have a
 * continuous sequence of parameter space curves <tt>(2D curves)</tt>.
 *<br><br>
 * The <tt>par_cur</tt> in turn consists of a pointer to a <tt>bs2_curve (a 2D spline curve)</tt>, a fitting
 * tolerance, and a pointer to the surface where the parameter space curve lies.
 *<br><br>
 * @see PCURVE, pcurve_law_data, skin_spl_sur, surf_surf_int, SPApar_vec
 */
class DECL_KERN pcurve : public ACIS_OBJECT {

    par_cur* fit;		// spline which fits curve in a parametric
    // surface

    logical rev;		// true if this curve opposes the direction
    // of the underlying spline.

    SPApar_vec off;		// displacement in SPAparameter space between
    // the "fit" definition and this pcurve. This
    // allows the pcurve to be positioned in the
    // infinite SPAparameter space of a periodic
    // surface, so that continuous curve sequences
    // in object space are continuous in SPAparameter
    // space. The components of this SPAvector should
    // always be integer multiples of the
    // corresponding surface SPAparameter period, zero
    // if it is not periodic in that direction.

// Ensure that the reference supplied points to a singly-used
// record.  Takes no action if it is already single, otherwise
// it copies everything.

    void make_single_ref();

public:

    // Constructors.

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
    pcurve();

    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     *<br><br>
     * @param cur
     * parameter curve.
     */
    pcurve(pcurve const& cur);

    /**
     * Creates a copy of an item that does not share any data with the original.
     *<br><br>
     * <b>Role:</b>  Allocates new storage for all member data and any pointers. Returns a pointer to
     * the copied item.
     *<br><br>
     * In a deep copy, all the information about the copied item is self-contained in a new memory
     * block. By comparison, a shallow copy stores only the first instance of the item in memory, and
     * increments the reference count for each copy.
     *<br><br>
     * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that have already been
     * deep copied. For example, a <tt>deep_copy</tt> of a complex model results in self contained data, but
     * identical sub-parts within the model are allowed to share a single set of data.
     *<br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual pcurve* deep_copy(pointer_map* pm = NULL) const;

    // Make an explicit pcurve from its components.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * <b>Role:</b>  Creates a pcurve as a <tt>bs2_curve</tt> in the parameter-space of a surface and having a
 * specified fit tolerance.
 *<br><br>
 * @param curve
 * underlying bs2 curve.
 * @param para
 * parameter.
 * @param sur
 * surface.
 * @param knot
 * knots on curve default is unknown.
 * @param hull_cur
 * bs2_curve hull contains true curve.
 * @param hull_angle
 * hull angles ok default is unknown.
 * @param hull_selfintr
 * hull self intersects default is unknown.
 * @param owns_bs2 default is TRIE
 * owns bs2.
 */
    pcurve(
        bs2_curve curve,
        double para,
        surface const& sur,
        int knot = -1,
        int hull_cur = -1,
        int hull_angle = -1,
        int hull_selfintr = -1,
        logical owns_bs2 = TRUE,
        double = -1.0
    );

    // Make an implicit pcurve referring to the given intcurve.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param cur
 * intcurve name.
 * @param integer
 * integer.
 */
    pcurve(
        intcurve const& cur,
        int integer
    );

    // Fit a pcurve to the given object-space curve lying on the
    // given surface.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * <b>Role:</b> Creates a pcurve fitting it to the given object-space curve lying on the given surface.
 *<br><br>
 * @param cur
 * curve name.
 * @param sur
 * surface name.
 * @param para
 * parameter.
 */
    pcurve(
        curve const& cur,
        surface const& sur,
        double para
    );

    // Make an pcurve referring to the given law.

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 *<br><br>
 * @param law_type
 * law.
 * @param start_param
 * start parameter.
 * @param end_param
 * end parameter.
 * @param sur
 * surface.
 * @param tol
 * fit tolerance.
 * @param cur
 * underlying curve.
 */
    pcurve(
        law* law_type,
        double start_param,
        double end_param,
        surface const& sur,
        double tol = SPAresabs,
        bs2_curve cur = NULL
    );

    // Make an iso pcurve - uses the curve if supplied

/**
 * C++ initialize constructor requests memory for this object and makes an iso pcurve (uses the curve if supplied).
 *<br><br>
 * @param udir
 * u direction flag.
 * @param param
 * parameter.
 * @param sur
 * surface.
 * @param cur
 * curve.
 */
    pcurve(
        logical udir,
        double param,
        surface const& sur,
        curve const& cur = *(curve*)NULL_REF
    );

    // used to set the surface of a pcurve after a space warp

/**
 * Used to set the surface of a pcurve after a space warp.
 *<br><br>
 * @param sur
 * surface to set.
 */
    void set_surface(surface const& sur);

    // Just promote a par_cur to a full pcurve.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * <b>Role:</b> Creates a pcurve by promoting a <tt>par_cur</tt> to a full pcurve surface.
 *<br><br>
 * @param cur
 * parameter curve.
 */
    pcurve(par_cur* cur);


    /**
     * C++ destructor, deleting a <tt>PCURVE</tt>.
     */
    virtual ~pcurve();  // Ensure any derived class destructor gets
    // a say when we destroy a surface.


/**
 * Inquires whether the parameter space curve is in the same or opposite direction of the underlying <tt>2D NURBS</tt> curve.
 */
    logical reversed() const
    {
        return rev;
    }

    /**
     * Returns the offset.
     */
    SPApar_vec offset() const
    {
        return off;
    }

    // STI ROLL begin - PCURVE needs par_cur* to do compares in
    //                  bulletin_no_change_vf
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    par_cur const& get_par_cur() const
    {
        return *fit;
    }			// shouldn't be used unless
                         // absolutely necessary.
// STI ROLL end

/**
 * Replaces the defining spline curve and should only be used with extreme caution.
 */
    void replace_par_cur(par_cur& new_fit);

    /**
     * Returns the range over which checking occurs.
     */
    SPAinterval checked_range() const;
    /**
     * Returns <tt>1</tt> if the underlying <tt>bs2_curve</tt> is known to be Bezier form, and <tt>0</tt> if it is not in this form, and <tt>-1</tt> if this property has not yet been checked.
     */
    int bezier_form();
    /**
     * Returns <tt>1</tt> if all of the <tt>bs2_curve</tt> knots that are within the <tt>checked_range</tt> lie on the associated true curve (to within <tt>SPAresabs</tt>), or to <tt>0</tt> if this is not the case.
     *<br><br>
     * <b>Role:</b> Returns <tt>-1</tt> if this property has not yet been checked.
     */
    int knots_on_true_curve() const;
    /**
     * Returns <tt>1</tt> if the <tt>bs2_curve</tt> tangents are within <tt>SPAresnor</tt> of the true curve tangent vectors at all of the knots that lie within the <tt>checked_range</tt>, or <tt>0</tt> if this is not satisfied.
     *<br><br>
     * <b>Role:</b>  Returns <tt>-1</tt> if this property has not yet been checked.
     */
    int accurate_knot_tangents() const;
    /**
     * Returns the value for hull turning angle checking.
     *<br><br>
     * <b>Role:</b> Set to <tt>1</tt> if the <tt>bs2_curve</tt> hull does not turn too sharply at any point within the
     * <tt>checked_range</tt>, or set to <tt>0</tt> if the hull does turn too sharply at a point. Set to <tt>-1</tt> if this
     * property has not yet been checked.
     */
    int hull_turning_angles_ok() const;
    /**
     * Returns <tt>1</tt> if the <tt>bs2_curve</tt> hull self-intersects, within the checked_range, or to zero if it does not self-intersect within this range.
     *<br><br>
     * <b>Role:</b> Returns <tt>-1</tt> if this property has not yet been checked.
     */
    int hull_self_intersects() const;
    /**
     * Returns the distance from the true curve to the underlying <tt>bs2_curve</tt> convex hull, if the true curve comes outside of the hull.
     *<br><br>
     * <b>Role:</b> Set to <tt>0</tt> if the true curve is known to lie completely within the hull or <tt>-1</tt> if
     * this property has not been checked yet.
     */
    double hull_distance() const;

    // Functions to assign properties to the pcurve.

/**
 * Sets the checked range of the pcurve.
 *<br><br>
 * @param new_range
 * interval.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 */
    void set_checked_range(const SPAinterval& new_range, int num_knots = 0,
        double* knots = NULL);
    /**
     * Checks if the underlying <tt>bs2_curve</tt> is in Bezier form.
     */
    logical check_bezier_form();
    /**
     * Makes the underlying <tt>bs2_curve</tt> have Bezier form by adding knots as necessary.
     */
    logical make_bezier_form();
    /**
     * Set to <tt>1</tt> if all of the <tt>bs2_curve</tt> knots that are within the <tt>checked_range</tt> lie on the associated true curve (to within SPAresabs), or to 0 if this is not the case.
     *<br><br>
     * <b>Role:</b> Set to <tt>-1</tt> if this property has not yet been checked.
     *<br><br>
     * @param knots_on_cu
     * value.
     */
    void set_knots_on_true_curve(int knots_on_cu);
    /**
     * Set to <tt>1</tt> if the <tt>bs2_curve</tt> tangents are within <tt>SPAresnor</tt> of the true curve tangent vectors at all of the knots that lie within the checked_range, or to 0 if this is not satisfied.
     *<br><br>
     * <b>Role:</b> Set to <tt>-1</tt> if this property has not yet been checked.
     *<br><br>
     * @param acc_tangents
     * value.
     */
    void set_accurate_knot_tangents(int acc_tangents);
    /**
     * Sets the value for hull turning angle checking.
     *<br><br>
     * <b>Role:</b> Set to <tt>1</tt> if the <tt>bs2_curve</tt> hull does not turn too sharply at any point within the
     * <tt>checked_range</tt>, or set to <tt>0</tt> if the hull does turn too sharply at a point. Set to <tt>-1</tt> if this
     * property has not yet been checked.
     *<br><br>
     * @param angles_ok
     * value.
     */
    void set_hull_turning_angles_ok(int angles_ok);
    /**
     * Set to <tt>1</tt> if the <tt>bs2_curve</tt> hull self-intersects, within the <tt>checked_range</tt>, or to zero if it does not self-intersect within this range.
     *<br><br>
     * <b>Role:</b> Set to <tt>-1</tt> if this property has not yet been checked.
     * @param self_ints
     * value
     */
    void set_hull_self_intersects(int self_ints);
    /**
     * Sets the distance from the true curve to the underlying bs2_curve convex hull, if the true curve comes outside of the hull.
     *<br><br>
     *<b>Role:</b> Set to <tt>0</tt> if the true curve is known to lie completely within the hull or <tt>-1</tt> if
     * this property has not been checked yet.
     *<br><br>
     * @param dist
     * value.
     */
    void set_hull_distance(double dist);

    // Add a knot to the bs2_curve with the supplied multiplicity.
    // If the new SPApar_pos and derivative SPAvector are supplied, set
    // the new control points so that the bs2_curve has the supplied
    // SPAposition and derivative at the new knot. Return the
    // multiplicity of the new knot. It is assumed that the caller
    // will take care of any changes in the underlying par_cur
    // properties, (eg self intersection), that the addition of knots
    // causes.

/**
 * Adds a knot to the <tt>bs2_curve</tt> with the supplied multiplicity.
 *<br><br>
 * <b>Role:</b> If the new <tt>SPApar_pos</tt> and derivative vectors are supplied, set the new control
 * points so that the <tt>bs2_curve</tt> has the supplied position and derivatives at the new knot. Returns
 * the multiplicity of the new knot. It is assumed that the caller will take care of any changes
 * in the underlying <tt>par_cur</tt> properties, (self intersection, for example), that the addition of
 * knots generates.
 *<br><br>
 * @param new_knot_param
 * knot to add.
 * @param mult
 * multiplicity.
 * @param new_knot_uv
 * knot uv.
 * @param new_knot_deriv_below
 * derivative.
 * @param new_knot_deriv_above
 * derivative.
 */
    int add_bs2_knot(double new_knot_param, int mult,
        SPApar_pos& new_knot_uv = *(SPApar_pos*)NULL_REF,
        SPApar_vec& new_knot_deriv_below = *(SPApar_vec*)NULL_REF,
        SPApar_vec& new_knot_deriv_above = *(SPApar_vec*)NULL_REF);

    // Functions to return underlying information as though it was part
    // of this record.

/**
 * Returns the underlying <tt>2D NURBS</tt> defining the parameter curve.
 */
    bs2_curve cur() const;
    /**
     * Returns the fit tolerance of the parameter curve.
     */
    double fitol() const;
    /**
     * Returns the par tolerance of the parameter curve.
     */
    double partol() const;
    /**
     * Returns the surface that the parameter space curve is defined.
     */
    surface const& surf() const;


    // Make a negated curve

/**
 * Makes a negated curve.
 */
    pcurve operator-() const;


    // Displace the curve in surface SPAparameter space.

/**
 * Displaces the curve in surface parameter space.
 *<br><br>
 * @param param_vec
 * parameter vector.
 */
    pcurve operator+(SPApar_vec const& param_vec) const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend DECL_KERN pcurve operator+(SPApar_vec const& pv, pcurve const& pc)
    {
        return pc + pv;
    }
    /**
     * Makes a negated curve given a <tt>SPApar_vec</tt>.
     *<br><br>
     * @param param_vec
     * parameter vector.
     */
    pcurve operator-(SPApar_vec const& param_vec) const;
    /**
     * Adds a <tt>SPApar_vec</tt> to a pcurve's offset.
     *<br><br>
     * @param pv
     * parameter vector.
     */
    pcurve const& operator+=(SPApar_vec const& pv)
    {
        off += pv;
        return *this;
    }
    /**
     * Subtracts a <tt>SPApar_vec</tt> to a pcurve's offset.
     *<br><br>
     * @param pv
     * parameter vector.
     */
    pcurve const& operator-=(SPApar_vec const& pv)
    {
        off -= pv;
        return *this;
    }


    // Perform a linear transformation on the curve parametrisation,
    // so that it starts and ends at the given values (which must be
    // in increasing order).

/**
 * Performs a linear transformation on the curve parameterization, so that it starts and ends at the given values (that must be in increasing order).
 *<br><br>
 * @param firstp
 * first parameter.
 * @param secondp
 * second parameter.
 */
    void reparam(
        double firstp,
        double secondp
    );


    // Assignment. Only copy the pcurve record, and adjust the use
    // counts of the underlying information to suit.

/**
 * An assignment operator that copies the pcurve record, and adjusts the use counts of the underlying information.
 *<br><br>
 * @param cur
 * pcurve to be assigned.
 */
    pcurve& operator=(pcurve const& cur);


    // Transform a pcurve in object space.

/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN pcurve operator*(pcurve const&, SPAtransf const&);
    /**
     * Transforms a pcurve in object space.
     *<br><br>
     * @param trans
     * transformation.
     */
    pcurve& operator*=(SPAtransf const& trans);



    // Evaluate a pcurve at a SPAparameter value, giving SPAposition and
    // optionally first derivative.

/**
 * Evaluates a pcurve at a parameter value, giving position and optionally first and second derivatives.
 * Note that pcurve::eval evaluates the approximate coedge geometry, not the procedural imprint of the edge geometry onto the face geometry.
 *<br><br>
 * @param par
 * parameter at which the pcurve is evaluated.
 * @param par_pos
 * param position.
 * @param first_der
 * 1st derivative.
 * @param second_der
 * 2nd derivative.
 */
    void eval(
        double par,
        SPApar_pos& par_pos,		// SPAposition in SPAparameter space
        SPApar_vec& first_der = *(SPApar_vec*)NULL_REF,
        // first derivative
        SPApar_vec& second_der = *(SPApar_vec*)NULL_REF
        // second derivative
    ) const;

    // Evaluate a pcurve at a SPAparameter value.

/**
 * Evaluates a pcurve at a parameter value.
 * Note that this signature of pcurve::eval_position evaluates the approximate coedge geometry, not the procedural imprint of the edge geometry onto the face geometry.
 *<br><br>
 * @param par
 * parameter at which the pcurve is evaluated.
 */
    SPApar_pos eval_position(double par) const;

    // Evaluate a pcurve at a SPAparameter value, to give the derivative
    // with respect to its SPAparameter.

/**
 * Evaluates a pcurve at a parameter value, to give the derivative with respect to its parameter.
 * Note that pcurve::eval_deriv evaluates the coedge approximate geometry, not the procedural imprint of the edge geometry onto the face geometry.
 *<br><br>
 * @param par
 * parameter at which the pcurve is evaluated.
 */
    SPApar_vec eval_deriv(double par) const;


    // Evaluate a pcurve exactly at an object-space point and SPAparameter
    // value, by inverting the point in the surface, using the
    // approximate pcurve SPAposition to give a start point.

/**
 * Evaluates a pcurve at a parameter value.
 * Note that this signature of pcurve::eval_position evaluates the procedural imprint of the input position onto the face geometry, not the approximate coedge geometry.
 *<br><br>
 * @param pos_par
 * position, from within the edge/coedge bundle.
 * @param par
 * parameter, at which an approximate coedge geometry evaluation (i.e., pcurve::eval) is close to the procedural imprint par_pos.
 * @param point_perp
 * use point_perp if true, param if false. The recommended value for this parameter is TRUE.
 */
    SPApar_pos eval_position(SPAposition const& pos_par, double par, logical point_perp = FALSE) const;

    // Return the SPAparameter of a pcurve, given a SPApar_pos.


// The closure of the pcurve in object space.
/**
 * @nodoc
 */
    logical periodic();

    logical closed();

    logical open();


    /**
     * Returns the parameter of the pcurve at the given <tt>SPApar_pos</tt>.
     *<br><br>
     * @param uv
     * parameter position.
     */
    double param(const SPApar_pos& uv);

    // Return the principal SPAparameter range of a pcurve. A periodic
    // curve is defined for all SPAparameter values, by reducing the
    // given SPAparameter modulo the period into this principal range.
    // For open or non-periodic closed curves the curve evaluation
    // functions are defined only for SPAparameter values in this range.

/**
 * Returns the principal parameter range of a pcurve.
 */
    SPAinterval param_range() const;


    // Return the SPAparameter period - the length of the SPAparameter range
    // if periodic, zero otherwise.

/**
 * Returns the parameter period - the length of the parameter range if periodic, <tt>0</tt> otherwise.
 */
    double param_period() const;


    // Negate a pcurve in place.

/**
 * Negates a pcurve in place.
 */
    pcurve& negate();


    // Divide a pcurve into two pieces at a SPAparameter value. Creates
    // a new pcurve on the heap, but either one of the pcurves may have
    // a NULL actual curve. The supplied curve is modified to be the
    // latter section, and the initial section is returned as value.

/**
 * Divides a pcurve into two pieces at a parameter value.
 *<br><br>
 * <b>Role:</b> This function creates a new pcurve on the heap, but either one of the pcurves may
 * have a <tt>NULL</tt> actual curve. The supplied curve is modified to be the latter section, and the
 * initial section is returned as value.
 *<br><br>
 * @param par
 * parameter.
 * @param par_pos
 * parameter position.
 * @param par_pos_vec
 * parameter position.
 */
    pcurve* split(double par, SPApar_pos const& par_pos = *(SPApar_pos*)NULL_REF,
        SPApar_vec const& par_pos_vec = *(SPApar_vec*)NULL_REF);

    // Function to trim the pcurve to the supplied range, having the
    // supplied end par_pos's, if these are supplied. The new pcurve
    // range is returned.

/**
 * Trims the pcurve to the supplied range, using the supplied end <tt>par_pos's</tt>, if any.
 *<br><br>
 *<b>Role:</b> The new pcurve range is returned.
 *<br><br>
 * @param new_range
 * trim to range.
 * @param start_uv
 * start param.
 * @param start_duv
 * slope.
 * @param end_uv
 * end param.
 * @param end_duv
 * slope.
 */
    SPAinterval trim(const SPAinterval& new_range,
        const SPApar_pos& start_uv = *(SPApar_pos*)NULL_REF,
        const SPApar_vec& start_duv = *(SPApar_vec*)NULL_REF,
        const SPApar_pos& end_uv = *(SPApar_pos*)NULL_REF,
        const SPApar_vec& end_duv = *(SPApar_vec*)NULL_REF);

    // Function to trim the pcurve to the range of the supplied curve.
    // The new pcurve range is returned.

/**
 * Trims the pcurve to the range of the supplied curve.
 *<br><br>
 * <b>Role:</b> The new pcurve range is returned.
 *<br><br>
 * @param cu
 * trimming curve.
 */
    SPAinterval trim_to_curve_range(const curve* cu);

    // Return a SPAbox around the curve.

/**
 * Returns a box around the curve.
 *<br><br>
 * @param int_name
 * interval name.
 * @param true_cu
 * curve.
 */
    SPApar_box bound(
        SPAinterval const& int_name,
        const curve* true_cu = NULL
    ) const;

    /**
     * Returns a box around the curve.
     *<br><br>
     * @param first_param
     * first parameter.
     * @param second_param
     * second parameter.
     * @param true_cu
     * curve.
     */
    SPApar_box bound(
        double first_param,
        double second_param,
        const curve* true_cu = NULL
    ) const;


    // Function to make up a bounded surface from the underlying surface.
    // This does not have to accurately bound the pcurve, but is just
    // used to make up SVECs for the other member functions.

/**
 * Makes up a bounded surface from the underlying surface.
 *<br><br>
 * <b>Role:</b> This does not have to accurately bound the pcurve, but is just used to make up
 * <tt>SVECs</tt> for the other member functions.
 */
    BOUNDED_SURFACE* make_bounded_surface() const;

    // Function to add knots to the pcurve's bs2_curve, at any of the
    // corresponding curve discontinuities. The bs2_curve knots can be
    // supplied to this function in an array, but if they are not,
    // they will be extracted from the pcurve.

/**
 * Adds knots to the pcurve's <tt>bs2_curve</tt>, at any of the corresponding curve discontinuities.
 *<br><br>
 * <b>Role:</b> The pcurve knots can be supplied to this function in an array, but if they are
 * not, they will be extracted from the pcurve.
 *<br><br>
 * @param cu
 * curve.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 */
    void add_knots_at_discontinuities(const curve* cu,
        int num_knots = 0,
        double* knots = NULL);

    // Function to carry out all the validity checks for a pcurve,
    // over the supplied SPAinterval. If a logical is supplied as a
    // NULL reference, the corresponding test is not carried out.
    // This function extracts the knots and control points from the
    // underlying par_cur and supplies them to the validity checking
    // functions, so that they only have to be extracted once.

/**
 * Carries out all the validity checks for a pcurve, over the supplied interval.
 *<br><br>
 * <b>Role:</b> If a logical is supplied as a <tt>NULL</tt> reference, the corresponding test is not
 * carried out. This function extracts the knots and control points from the pcurve and supplies
 * them to the validity checking functions, so that they only have to be extracted once.
 *<br><br>
 * @param cu
 * curve.
 * @param chkd_range
 * interval.
 * @param knots_on_true_cur
 * knots of true curve.
 * @param knot_tangents_ok
 * knot tangents ok.
 * @param hull_turn_angles_ok
 * hull turning angles ok.
 * @param hull_intersects
 * hull self intersections.
 * @param hull_contains_true_cu
 * hull contains true curve.
 */
    void validity_checks(const curve* cu,
        const SPAinterval& chkd_range,
        logical& knots_on_true_cur,
        logical& knot_tangents_ok,
        logical& hull_turn_angles_ok,
        logical& hull_intersects,
        logical& hull_contains_true_cu);

    // Function to check that all the knots of the pcurve bs2_curve
    // lie on the supplied true curve. The bounded surface and the
    // knots can be supplied to this function, but if they are not,
    // they will be extracted from the underlying par_cur. If the
    // number of knots and the knot array is supplied, these are
    // supplied as references, as more knots may be added to correct
    // the hull. The function returns TRUE if all of the knot points
    // lie on the true curve, or FALSE otherwise.

/**
 * Checks that all the knots of the pcurve <tt>bs2_curve</tt> lie on the supplied true curve.
 *<br><br>
 * <b>Role:</b> The bounded surface and the knots can be supplied to this function, but if they
 * are not, they will be extracted from the underlying <tt>par_cur</tt>. If the number of knots and the
 * knot array is supplied, these are supplied as references, as more knots may be added to
 * correct the hull. The function returns <tt>TRUE</tt> if all of the knot points lie on the true curve,
 * or <tt>FALSE</tt> otherwise.
 *<br><br>
 * @param true_cu
 * curve.
 * @param bsf
 * bounded surface.
 * @param deg
 * degree.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 */
    logical check_knots_on_true_curve(const curve* true_cu,
        BOUNDED_SURFACE* bsf,
        int deg,
        int& num_knots,
        double*& knots);

    // Function to check that the pcurve bs2_curve tangent directions
    // agree with the true curve tangent directions at each of the
    // knots. The boundeded surface and the knots can be supplied to
    // this function, but if they are not, they will be extracted from
    // the underlying par_cur. The function returns TRUE if the tangent
    // directions agree at all of the knot points, or FALSE otherwise.

/**
 * Checks that the pcurve <tt>bs2_curve</tt> tangent directions agree with the true curve tangent directions at each of the knots.
 *<br><br>
 * <b>Role:</b> The bounded surface and the knots can be supplied to this function, but if they
 * are not, they will be extracted from the underlying <tt>par_cur</tt>. The function returns <tt>TRUE</tt> if the
 * tangent directions agree at all of the knot points, or <tt>FALSE</tt> otherwise.
 *<br><br>
 * @param true_cu
 * curve.
 * @param bsf
 * bounded surface.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 */
    logical check_knot_tangents(const curve* true_cu,
        BOUNDED_SURFACE* bsf = NULL,
        int num_knots = 0,
        double* knots = NULL);

    // Function to check that the pcurve bs2_curve hull does not turn
    // by more than the minimum turning angle. If it does, then the hull
    // points are projected onto a SPAparameter line and the order of the
    // points is checked, to test if the pcurve has kinked. The bounded
    // surface and the knots and control points can be supplied to this
    // function, or if they are NULL, they will be extracted from the
    // underlying par_cur. If the number of knots and control points,
    // and the knot and control point arrays are supplied, these are
    // supplied as references, as more knots and control points may be
    // added to correct the hull. If the true curve is supplied, this is
    // used to find the positions for any new knots that are added to
    // the bs2_curve. The function returns TRUE if the hull does not
    // turn by more than the defined minimum angle, or FALSE otherwise.

/**
 * Checks that the pcurve <tt>bs2_curve</tt> hull does not turn by more than the minimum turning angle.
 *<br><br>
 * <b>Role:</b> Checks that the pcurve <tt>bs2_curve</tt> hull does not turn by more than the minimum
 * turning angle. If it does, then the hull points are projected onto a parameter line and the
 * order of the points is checked, to test if the pcurve has kinked. The bounded surface and the
 * knots and control points can be supplied to this function, or if they are <tt>NULL</tt>, they will be
 * extracted from the underlying <tt>par_cur</tt>. If the number of knots and control points, and the knot
 * and control point arrays are supplied, these are supplied as references, as more knots and
 * control points may be added to correct the hull. If the true curve is supplied, this is used
 * to find the positions for any new knots that are added to the <tt>bs2_curve</tt>. The function returns
 * <tt>TRUE</tt> if the hull does not turn by more than the defined minimum angle, or <tt>FALSE</tt> otherwise.
 *<br><br>
 * @param true_cu
 * curve.
 * @param bsf
 * bounded surface.
 * @param deg
 * degree.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 * @param num_ctrlpts
 * number of control points.
 * @param ctrlpts
 * control points.
 */
    logical check_hull_turning_angle(const curve* true_cu,
        BOUNDED_SURFACE* bsf,
        int deg,
        int& num_knots,
        double*& knots,
        int& num_ctrlpts,
        SPApar_pos*& ctrlpts);

    // Function to test if the convex hull of a pcurve self-intersects.
    // The degree and control points can be supplied to this function
    // in an array, but if they are not, they will be extracted from
    // the underlying par_cur. The function returns TRUE if the hull
    // self-intersects, or FALSE otherwise.

/**
 * Checks if the convex hull of a pcurve self-intersects.
 *<br><br>
 * <b>Role:</b> The degree and control points can be supplied to this function in an array, but
 * if they are not, they will be extracted from the underlying <tt>par_cur</tt>. The function returns <tt>TRUE</tt>
 * if the hull self-intersects, or <tt>FALSE</tt> otherwise.
 *<br><br>
 * @param deg
 * degree.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 * @param num_ctrlpts
 * number of control points.
 * @param ctrlpts
 * control points.
 */
    logical check_hull_intersection(int deg = 0,
        int num_knots = 0,
        double* knots = NULL,
        int num_ctrlpts = 0,
        SPApar_pos* ctrlpts = NULL);

    // Function to check if the pcurve bs2_curve hull contains the
    // bounded true curve, and if not, to find the 2-space distance
    // that the true curve moves outside the hull by. The bounded
    // surface, degree, knots, and control points can be supplied to
    // this function in an array, but if they are not, they will be
    // extracted from the underlying par_cur. The function returns
    // TRUE if the hull contains the true curve, or FALSE otherwise.

/**
 * Checks if the pcurve <tt>bs2_curve</tt> hull contains the bounded true curve, and if not, to find the <tt>2-space</tt> distance that the true curve moves outside the hull by.
 *<br><br>
 * <b>Role:</b> The bounded surface, degree, knots, and control points can be supplied to this
 * function in an array, but if they are not, they will be extracted from the underlying <tt>par_cur</tt>.
 * The function returns <tt>TRUE</tt> if the hull contains the true curve, or <tt>FALSE</tt> otherwise.
 *<br><br>
 * @param true_cu
 * curve.
 * @param bsf
 * bounded surface.
 * @param deg
 * degree.
 * @param num_knots
 * number of knots.
 * @param knots
 * knots.
 * @param num_ctrlpts
 * number of control points.
 * @param ctrlpts
 * control points.
 */
    logical check_hull_curve_enclosure(const curve* true_cu,
        BOUNDED_SURFACE* bsf = NULL,
        int deg = 0,
        int num_knots = 0,
        double* knots = NULL,
        int num_ctrlpts = 0,
        SPApar_pos* ctrlpts = NULL);


    // Return a string identifying the pcurve type

/**
 * Returns a string identifying the pcurve type.
 *<br><br>
 * <b>Role:</b>  For <tt>exp_par_cur</tt> and its derived types, this method returns <tt>"exppc"</tt>.
 */
    char const* type_name() const;


    // Save and restore.

    // Function to be called to save a pcurve of unknown type, or
    // NULL. Just checks for null, then calls save(). This separation
    // is not really necessary for pcurves at present, as there are no
    // subtypes, but we retain it for consistency with curve and
    // surface.

/**
 * Function to be called to save a pcurve of unknown type, or <tt>NULL</tt>.
 *<br><br>
 * <b>Role:</b> Just checks for null, then calls save. This separation is not really necessary
 * for pcurves at present, as there are no subtypes, but we retain it for consistency with curve
 * and surface.
 */
    void save_pcurve() const;

    // Function to save a pcurve of unknown type, together with its
    // type code for later retrieval.

/**
 * Saves the id the calls <tt>save_data</tt>.
 */
    void save() const;

    // Function to save a pcurve of a known type, where the context
    // determines the pcurve type, so no type code is necessary.

/**
 * Saves the information for a pcurve to the save file.
 *<br><br>
 * <b>Role:</b> Function to save a pcurve of a known type, where the context determines the
 * pcurve type, so no type code is necessary.
 */
    void save_data() const;

    // Function to be called to restore a pcurve of unknown type -
    // reads the type code, and switches to pcurve-specific restore
    // code.

/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN pcurve* restore_pcurve();

    // Function to restore the data for a pcurve of known type, either
    // where the context determines the type, or after the type code
    // has been read and processed.

/**
 * Restores the data for a pcurve from a save file.
 *<br><br>
 * <b>Role:</b> Restores the data for a pcurve of known type, either
 * where the context determines the type, or after the type code
 * has been read and processed.
 * <br><br>
 * <pre>
 * read_logical				Sense of the pcurve: either "forward" or "reversed".
 * if (restore_version_number &lt; PCURVE_VERSION)
 * 	// Restore as an explicit pcurve.
 * 	par_cur* restore_exp_par_cur	Restore the appropriate pcurve
 * else
 * 	// Switch to the right restore routine, using the standard  system mechanism.
 * 	(par_cur *)dispatch_restore_subtype
 * 	read_real			du; u offset
 * 	read_real			dv; v offset
 * </pre>
 */
    void restore_data();

    // Output details of a pcurve.

/**
 * Outputs details of a pcurve.
 *<br><br>
 * @param title
 * title.
 * @param file_name
 * file name.
 */
    void debug(
        char const* title,
        FILE* file_name = debug_file_ptr
    ) const;

    // STI ROLL
    // Function to count the full size in bytes of this object and
    // everything he owns.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
/**
 * @nodoc
 */
    virtual void minimize(minimize_helper*);

#if defined D3_STANDALONE || defined D3_DEBUG

    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    void print(
        D3_ostream& os
    ) const;

    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    void input(
        D3_istream& is
    );

#endif
public:
    // for save
    par_cur* gme_get_fit();
    // for restore
    void gme_set_rev(int data);
    void gme_set_off(SPApar_vec data);
    par_cur** gme_get_fit_ptr();
};

// Class for the fitted spline itself, defined so that we shall be able
// to use use-counts to avoid too much copying, and so that different
// representations may be used. This is an abstract base class, from
// which the actual types are derived. It is derived from the standard
// transformable subtype object, which handles shared save and restore
// and transformations which do not destroy sharing.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN par_cur : public subtrans_object {
protected:
    SPAinterval check_range;	// The range over which the validity checks,
    // relating to the following arguments, have
    // been carried out. Initialised to the
    // infinite range.

    int _bezier_form;		// Set to 1 if the underlying bs2_curve is
    // known to be Bezier form, and 0 if it is
    // not in this form. Set to -1 if this
    // property has not yet been checked.

    int knots_on_true_cu;	// Set to 1 if all of the bs2_curve knots
    // that are within the checked_range lie on
    // the associated true curve (to within
    // SPAresabs), or to 0 if this is not the case.
    // Set to -1 if this property has not yet
    // been checked.

    int acc_knot_tangents;  // Set to 1 if the bs2_curve tangents are
    // within SPAresnor of the true curve tangent
    // vectors at all of the knots that lie
    // within the checked_range, or to 0 if
    // this is not satisfied. Set to -1 if this
    // property has not yet been checked.

    int hull_angles_ok;		// Set to 1 if the bs2_curve hull does not
    // turn too sharply at any point within the
    // checked_range, or set to 0 if the hull does
    // turn too sharply at a point. Set to -1 if
    // this property has not yet been checked.

    int hull_self_ints;		// Set to 1 if the bs2_curve hull
    // self-intersects, within the checked_range,
    // or to zero if it does not self-intersect
    // within this range. Set to -1 if this
    // property has not yet been checked.

    double hull_dist;	// The distance from the true curve to the underlying
    // bs2_curve convex hull, if the true curve comes
    // outside of the hull. Set to 0 if the true curve
    // is known to lie completely within the hull or
    // -1 if this property has not been checked yet.

private:

    // Use count manipulation, just passing through the underlying
    // ones.

    void operator++() { add_ref(); }
    void operator--() { remove_ref(); }
    logical mult() const { return mult_ref(); }


protected:
    // Straightforward constructor and (virtual) destructor.

    par_cur();
    virtual ~par_cur();

    // Extract the defining data, which is assumed to take the same
    // form for each subtype, though represented differently.

    virtual bs2_curve cur() const = 0;
    virtual double fitol() const = 0;
    virtual double partol() const = 0;
    virtual surface const* surf() const = 0;

    // The closure of the pcurve in object space.
/**
 * @nodoc
 */
    virtual logical periodic();
    virtual logical closed();
    virtual logical open();

    SPAinterval checked_range() const
    {
        return check_range;
    }
    int bezier_form()
    {
        if (_bezier_form == -1) check_bezier_form();
        return _bezier_form;
    }
    int knots_on_true_curve() const
    {
        return knots_on_true_cu;
    }
    int accurate_knot_tangents() const
    {
        return acc_knot_tangents;
    }
    int hull_turning_angles_ok() const
    {
        return hull_angles_ok;
    }
    int hull_self_intersects() const
    {
        return hull_self_ints;
    }
    double hull_distance() const
    {
        return hull_dist;
    }

    // Functions to assign properties to the par_cur.

    void set_checked_range(const SPAinterval& new_range, int num_knots = 0,
        double* knots = NULL);
    logical check_bezier_form();
    logical make_bezier_form();
    void set_knots_on_true_curve(int knots_on_cu)
    {
        knots_on_true_cu = knots_on_cu;
    }
    void set_accurate_knot_tangents(int acc_tangents)
    {
        acc_knot_tangents = acc_tangents;
    }
    void set_hull_turning_angles_ok(int angles_ok)
    {
        hull_angles_ok = angles_ok;
    }
    void set_hull_self_intersects(int self_ints)
    {
        hull_self_ints = self_ints;
    }
    void set_hull_distance(double dist);

    // Function to set the underlying bs2_curve to be the supplied
    // bs2_curve UNLESS this is an imp_par_cur, which would be messy
    // as it would involve changing the intcurve underlying bs2_curves.
    // If a bs2_curve already exists, this will be deleted.

    virtual void set_bs2_curve(bs2_curve new_bs2) = 0;

    // Add a knot to the bs2_curve with the supplied multiplicity.
    // If the new SPApar_pos and derivative SPAvector are supplied, set
    // the new control points so that the bs2_curve has the supplied
    // SPAposition and derivative at the new knot. Return the
    // multiplicity of the new knot. It is assumed that the caller
    // will take care of any changes in the par_cur properties,
    // (eg self intersection), that the addition of knots causes.
    // GSSL/RS (Dec 24,2002) : Making this function virtual

    virtual int add_bs2_knot(double new_knot_param, int mult,
        SPApar_pos& new_knot_uv = *(SPApar_pos*)NULL_REF,
        SPApar_vec& new_knot_deriv_below = *(SPApar_vec*)NULL_REF,
        SPApar_vec& new_knot_deriv_above = *(SPApar_vec*)NULL_REF);

    // Used to set the surface after a space warp

    virtual void set_surface(surface const&);

    // Duplication. Can't be done by constructor, as we want it
    // to be virtual.

    virtual subtrans_object* copy() const = 0;


    // Test for equality.
    // It does not guarantee that all effectively equal curves are
    // determined to be equal, but does guarantee that different
    // curves are correctly identified as such. Each subtype must
    // provide its own test.

    virtual logical operator==(subtype_object const&) const = 0;


    // Transformation

    virtual void operator*=(SPAtransf const&) = 0;


    // Simple SPAparameter-space bounding SPAbox. The pcurve function handles
    // any limiting to a SPAparameter range, so this just bound the whole
    // curve.

    virtual SPApar_box bound() const;


    // Find the defined SPAparameter range, or the canonical range for
    // a periodic curve.

    virtual SPAinterval param_range() const;


    // Return the SPAparameter period - the length of the SPAparameter range
    // if periodic, zero otherwise.

    virtual double param_period() const;


    // Parameter shift: adjust the spline curve to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic curve by
    // integral multiples of the period.

    virtual void shift(double) = 0;


    // Perform a linear transformation on the parametrisation, so
    // that it starts and ends at the given values (which must be
    // in increasing order).

    virtual void reparam(
        double,
        double
    ) = 0;


    // Divide a curve into two pieces at a given SPAparameter value,
    // possibly adjusting the spline approximations to an exact value
    // at the split point. If the SPAparameter value is at the beginning,
    // sets the first piece to NULL, and places the original curve
    // in the second slot; if the SPAparameter value is at the end,
    // places the original curve in the first slot, and sets the
    // second to NULL.

    virtual void split(
        double,
        SPApar_pos const&,
        par_cur* [2],
        SPApar_vec const& = *(SPApar_vec*)NULL_REF
    ) = 0;


    // Concatenate the contents of two curves into one. The curves are
    // guaranteed to be the same base or derived type, and to have
    // contiguous SPAparameter ranges ("this" is the beginning part of
    // the combined curve, the argument gives the end part).

    virtual void append(par_cur&) = 0;


    // Find SPAposition, first and second derivative on curve at given
    // SPAparameter value.

    virtual void eval(
        double,
        SPApar_pos&,		// SPAposition in SPAparameter space
        SPApar_vec & = *(SPApar_vec*)NULL_REF,
        // first derivative
        SPApar_vec & = *(SPApar_vec*)NULL_REF
        // second derivative
    ) const = 0;

    // Evaluate a par_cur at a SPAparameter value.

    SPApar_pos eval_position(double) const;

    // Return the SPAparameter of a par_cur, given a SPApar_pos.

    virtual double param(const SPApar_pos& uv);

    // Evaluate a par_cur at a SPAparameter value, to give the derivative
    // with respect to its SPAparameter.

    SPApar_vec eval_deriv(double) const;

    // Function to make up a bounded surface from the underlying surface.
    // This does not have to accurately bound the par_cur, but is just
    // used to make up SVECs for the other member functions.

    BOUNDED_SURFACE* make_bounded_surface(SPApar_vec offset) const;

    // Function to check that all the knots of the par_cur bs2_curve
    // lie on the supplied true curve. The supplied reverse flag and
    // SPApar_vec denote whether the par_cur should be reversed or shifted
    // in SPAparameter space before comparing with the true curve. The
    // bounded surface and the knots can be supplied to this function,
    // but if they are not, they will be extracted from the par_cur. If
    // the number of knots and the knot array is supplied, these are
    // supplied as references, as more knots may be added to correct the
    // hull. The function returns TRUE if all of the knot points lie on
    // the true curve, or FALSE otherwise.

    logical check_knots_on_true_curve(const curve* true_cu,
        logical rev, SPApar_vec offset,
        BOUNDED_SURFACE* bsf,
        int deg,
        int& num_knots,
        double*& knots);

    // Function to check that the pcurve bs2_curve tangent directions
    // agree with the true curve tangent directions at each of the
    // knots. The supplied reverse flag denote whether the par_cur should
    // be reversed before comparing with the true curve.  The bounded
    // surface and the knots can be supplied to this function, but if they
    // are not, they will be extracted from the pcurve. The function
    // returns TRUE if the tangent directions agree at all of the knot
    // points, or FALSE otherwise.

    logical check_knot_tangents(const curve* true_cu,
        logical rev,
        BOUNDED_SURFACE* bsf = NULL,
        int num_knots = 0,
        double* knots = NULL);

    // Function to check that the pcurve bs2_curve hull does not turn
    // by more than the minimum turning angle. If it does, then the hull
    // points are projected onto a SPAparameter line and the order of the
    // points is checked, to test if the pcurve has kinked. The bounded
    // surface and the knots and control points can be supplied to this
    // function, or if they are NULL, they will be extracted from the
    // pcurve. If the number of knots and control points, and the knot
    // and control point arrays are supplied, these are supplied as
    // references, as more knots and control points may be added to
    // correct the hull. If the true curve is supplied, this is used to
    // find the positions for any new knots that are added to the bs2_curve.
    // The supplied reverse flag and SPApar_vec denote whether the par_cur
    // should be reversed or shifted in SPAparameter space before comparing
    // with the true curve. The function returns TRUE if the hull does not
    // turn by more than the defined minimum angle, or FALSE otherwise.

    logical check_hull_turning_angle(const curve* true_cu,
        logical rev, SPApar_vec offset,
        BOUNDED_SURFACE* bsf,
        int deg,
        int& num_knots,
        double*& knots,
        int& num_ctrlpts,
        SPApar_pos*& ctrlpts);

    // Function to add extra knots to a section of par_cur that has been
    // found to turn too sharply, and re-test the turning angles. The
    // true curve, list of control points, list of knots, and the index
    // of the control that turns sharply, are passed into the function.
    // The true curve pointer, the integer references num_knots and
    // num_ctrlpts, and the pointers to the knots and control points are
    // assumed to never be NULL. The BOUNDED_SURFACE pointer reference
    // may be NULL, but if so, and if this function makes up a
    // BOUNDED_SURFACE, it is assumed that the calling function will
    // take responsibility for deleting this. The function returns TRUE
    // if it manages to obtain acceptable turning angles for the control
    // points, or FALSE otherwise.

    logical correct_hull_turning_angle(const curve* true_cu,
        SPApar_vec offset,
        BOUNDED_SURFACE*& bsf,
        int deg, int& num_knots,
        double*& knots, int& num_ctrlpts,
        SPApar_pos*& ctrlpts, int& knot_index,
        int& check_n_knots, int& ctrlpt_index,
        double knottol);

    // Function to check the turning angles of the control points between
    // the knots beginning at knot_index, and the knot numbered
    // knot_index + check_n_knots. The integer references num_knots and
    // num_ctrlpts, and the pointers to the knots and control points are
    // assumed to never be NULL. The BOUNDED_SURFACE pointer reference may
    // be NULL, but if so, and if this function makes up a BOUNDED_SURFACE,
    // it is assumed that the calling function will take responsibility for
    // deleting this. The function returns TRUE if the hull does not turn
    // by more than the defined minimum angle, or FALSE otherwise.

    logical iter_check_hull_turning_angle(const curve* true_cu,
        SPApar_vec offset,
        BOUNDED_SURFACE*& bsf,
        int deg, int& num_knots,
        double*& knots, int& num_ctrlpts,
        SPApar_pos*& ctrlpts,
        int& knot_index,
        int& check_n_knots,
        int& ctrlpt_index,
        SPApar_dir& prev_dir,
        SPApar_dir& curr_dir,
        double knottol,
        logical& start_over);

    // Function to test if the convex hull of a par_cur self-intersects.
    // The degree and control points can be supplied to this function
    // in an array, but if they are not, they will be extracted from
    // the par_cur. The function returns TRUE if the hull self-intersects,
    // or FALSE otherwise.

    logical check_hull_intersection(int deg = 0,
        int num_knots = 0,
        double* knots = NULL,
        int num_ctrlpts = 0,
        SPApar_pos* ctrlpts = NULL);

    // Function to check if the par_cur bs2_curve hull contains the
    // bounded true curve, and if not, to find the 2-space distance
    // that the true curve moves outside the hull by. The supplied
    // reverse flag and SPApar_vec denote whether the par_cur should be
    // reversed or shifted in SPAparameter space before comparing with the
    // true curve. The bounded surface, degree, knots, and control points
    // can be supplied to this function in an array, but if they are not,
    // they will be extracted from the par_cur. The function returns TRUE
    // if the hull contains the true curve, or FALSE otherwise.

    logical check_hull_curve_enclosure(const curve* true_cu,
        logical rev, SPApar_vec offset,
        BOUNDED_SURFACE* bsf = NULL,
        int deg = 0,
        int num_knots = 0,
        double* knots = NULL,
        int num_ctrlpts = 0,
        SPApar_pos* ctrlpts = NULL);


    // Save and restore. Save is easy, as derived class switching goes
    // through the normal virtual function mechanism. Restore is more
    // complicated, because until it is invoked we do not have an
    // object of the right class. Instead our parent subtype_object
    // class searches a table for the right restore function, and
    // then calls it.

    virtual char const* type_name() const = 0;

    virtual void save_data() const = 0;

    //	friend subtype_object *restore_par_cur();
    //	void restore_data();

        // Debug printout. As for save and restore we split the operation
        // into two parts: the virtual function "debug" prints a class-
        // specific identifying line, then calls the ordinary function
        // "debug_data" to put out the details. It is done this way so that
        // a derived class' debug_data can call its parent's version first,
        // to put out the common data. Indeed, if the derived class has no
        // additional data it need not define its own version of debug_data
        // and use its parent's instead. A string argument provides the
        // introduction to each displayed line after the first, and a
        // logical sets "brief" output (normally removing detailed
        // subsidiary curve and surface definitions).

    virtual void debug(
        char const*,
        logical,
        FILE*
    ) const = 0;
    //	void debug_data(
    //				char const *,
    //				logical,
    //				FILE *
    //			) const;

#if defined D3_STANDALONE || defined D3_DEBUG

    virtual void print(
        D3_ostream& os
    ) const = 0;

    virtual void input(
        D3_istream& is
    ) = 0;

#endif

    // All this is used by pcurve, so make it a friend.

    friend class pcurve;

public:
    virtual par_cur* deep_copy(pointer_map* pm = NULL) const = 0;

    // STI ROLL
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL

/**
 * @nodoc
 */
    virtual void minimize(minimize_helper*) {}

    virtual bool is_dependent_on(const surface&) const { return false; }
};

// STI let (8/97): Merged local functions from oper3.0 and kern3.0
// into a single, global function in kern3.0.  This function is
// only called by two functions, one of which is in the oper3.0 RIP
// directory.  When that function is laid to rest, this can be
// made static.  It should not be documented.

// STI let (3/98): Made this function a static function.
// It should no longer be documented!
//DECL_KERN SPApar_pos
//sg_srf_param_on_crv(
//	surface const& srf,			// surface the curve lies on
//	double param,				// SPAparameter value of point on curve
//	curve const& crv,			// curve lying on surface
//	logical rev,				// direction of evaluation
//	int param_on_curve			// 0 => start, 1 => end, -1 => interior
//);
// STI let: end


/** @} */
#endif
