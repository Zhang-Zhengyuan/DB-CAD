/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( compcurv_CLASS )
#define compcurv_CLASS

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "curdef.hxx"
#include "scandef.hxx"
#include "debugmsc.hxx"
#include "transf.hxx"
class surface;
class pcurve;
class SPAbox;
class mesh_tree;
class par1_tree;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAparameter;
class SPApar_pos;
class SPApar_dir;
class ENTITY;
class ENTITY_LIST;
class P1NODE;
class ELEM1D;
class SizeAccumulator;
class compcurv;
DECL_KERN compcurv operator*(const compcurv &, SPAtransf const & );
DECL_KERN curve *restore_compcurv();

/**
 * @nodoc
 * Define an identifying type for compcurv curve.
 */
#define compcurv_type 40

class com_cur;

/**
 * @nodoc
 * Curve that is the general representation of a
 * patchwork curve used to represent boundaries of a meshsurf, which
 * is a patchwork surface.  Separate edges and vertices could have
 * been used to represent this data, but the compcurv class was
 * implemented to 1) save memory 2) increase execution speed
 * 3) represent parameter lines and 4) store attributes that refer
 * to corresponding nodes and elements on the meshsurfs that they
 * lie in.
*/
class DECL_KERN compcurv: public curve {

	com_cur *fit;		// Underlying data.
	logical rev;		// True if this curve opposes the direction
						// of the underlying mesh representation.

	// Ensure that the reference supplied points to a singly-used 
	// record.  Take no action if it is already single, otherwise 
	// copy everything.

	void make_single_ref();

	// Do an in place transform without taking care of com_cur
	// references.  Only done from the entity level.

	void hard_transform( const SPAtransf & );

public:
   /**
	* Make a null compcurv.
    */
	compcurv() { fit = NULL; rev = FALSE; }
    /**
     * Copy constructor does a deferred copy.
     */
	compcurv( compcurv const & );
    /**
     * The deep_copy method makes a copy without shared data
     */
	virtual curve *deep_copy(pointer_map * pm = NULL) const;
    /**
     *@nodoc
     */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * Construct an compcurv from the underlying com_cur.
     */
    compcurv( com_cur * );
    /**
     * Get its defining com_cur
     */
	com_cur & get_com_cur() { return *fit; }
    /**
     * Get its defining com_cur
     */
	const com_cur & get_com_cur() const { return *fit; }
    /**
	 * Virtual copy.  Calls the copy constructor, and is therefore
	 */
	virtual curve *make_copy() const;
    /**
     * Remove the use of the underlying curve (decrements use-count).
     */
	~compcurv();
    /**
     *lose function.
     */
	virtual void lose();
    /**
     * Scan the node and element data and fill up the entity lists
	 * in the attribute.
     */
	void scan();
    /**
     * Pointer fixing.
     */
	void fix_pointers( ENTITY ** );
    /**
     * Return the sense.
     */
	logical reversed() const { return rev; }
    /**
     * Return whether the underlying curve exists.
     */
	virtual logical undef() const;
    /**
     * Return a negated copy.
     */
	compcurv operator-() const;
    /**
    * Assignment is basically the same as the copy constructor.
    */
	compcurv &operator=( compcurv const & );
    /**
     * Zero tolerance equality test.
     */
	virtual logical operator==( curve const & ) const;
    /**
     * Returned a transformed copy.  Copies, then calls *=.
     */
	friend DECL_KERN compcurv operator*(const compcurv &, SPAtransf const & );
    /**
     * Transform the curve.
     */
	virtual curve &operator*=( SPAtransf const & );
    /**
     * Negate the curve in place.
     */
	virtual curve &negate();
    /**
     * Construct a new curve which is a copy of the part of the
     * given one within given parameter bounds.
     */
	curve *subset( SPAinterval const & ) const;
    /**
	 * Divide an compcurv into two pieces at a parameter value.
	 * The supplied curve is modified to be the latter section,
	 * and the initial section is returned as value.
     */
    virtual curve *split( double, SPAposition const & = *(SPAposition *)NULL_REF );
    /**
     * Return a box around the curve. This may not be the minimum box
     * containing the curve, but a compromise between speed of
     * evaluation and tightness.
     */
	virtual SPAbox bound(
				SPAposition const &,
				SPAposition const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;
    /**
     * Return a box around the curve. This may not be the minimum box
     * containing the curve, but a compromise between speed of
     * evaluation and tightness.
     */
    virtual SPAbox bound(
				SPAinterval const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;

    /**
     * Return a box surrounding that portion of the curve within the
	 * given box
     */
	virtual SPAbox bound( SPAbox const &, SPAtransf const & = *(SPAtransf *)NULL_REF ) const;
    /**
     * The following is retained for historical reasons, but will be withdrawn.
     */
	SPAbox bound(
				double start,
				double end,
				SPAtransf const &t = *(SPAtransf *)NULL_REF
			) const
	{
		return bound( SPAinterval( start, end ), t );
	}
    /**
     * Return a cone bounding the tangent direction of the curve. The
     * logical argument indicates that an approximation is ok.
     */
	curve_tancone tangent_cone(
				SPAinterval const &range,
				logical approx_OK,
				SPAtransf const &t = *(SPAtransf *)NULL_REF
			) const;
    /**
     * Function to set the evaluation type for node seams for
     * the next evaluator or geometric call.
     */
	void set_neighborhood(evaluate_curve_side);
    /**
     * Find tangent direction to compcurv at given point.
     */
	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;
 	/**
     * Find curvature at point on compcurv.
     */
	virtual SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;
	/**
     * Find the foot of the perpendicular from the given point to
	 * the curve, and tangent to and curvature of the curve at that
	 * point, and its parameter value. If an input parameter value
	 * is supplied (as argument 5), the perpendicular found is the
	 * one nearest to the supplied parameter's position, otherwise it
	 * is the one at which the curve is nearest to the given point.
	 * Any of the return value arguments may be a NULL reference, in
	 * which case it is simply ignored.
     */
	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				SPAvector &,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF,
				logical f_weak=FALSE
			) const;
	/**
     * Find the foot of the perpendicular from the given point to
	 * the curve, and tangent to and curvature of the curve at that
	 * point, and its parameter value. If an input parameter value
	 * is supplied (as argument 5), the perpendicular found is the
	 * one nearest to the supplied parameter's position, otherwise it
	 * is the one at which the curve is nearest to the given point.
	 * Any of the return value arguments may be a NULL reference, in
	 * which case it is simply ignored.
     */
    void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPAunit_vector &foot_dt,
				SPAparameter const &guess = *(SPAparameter *)NULL_REF,
				SPAparameter &actual = *(SPAparameter *)NULL_REF,
				logical f_weak=FALSE
			) const
	{
		point_perp(
					pos,
					foot,
					foot_dt,
					*(SPAvector *)NULL_REF,
					guess,
					actual,
					f_weak
				);
	}
	/**
     * Find the foot of the perpendicular from the given point to
	 * the curve, and tangent to and curvature of the curve at that
	 * point, and its parameter value. If an input parameter value
	 * is supplied (as argument 5), the perpendicular found is the
	 * one nearest to the supplied parameter's position, otherwise it
	 * is the one at which the curve is nearest to the given point.
	 * Any of the return value arguments may be a NULL reference, in
	 * which case it is simply ignored.
     */
    void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPAparameter const &guess = *(SPAparameter *)NULL_REF,
				SPAparameter &actual = *(SPAparameter *)NULL_REF,
				logical f_weak=FALSE
			) const
	{
		point_perp(
					pos,
					foot,
					*(SPAunit_vector *)NULL_REF,
					*(SPAvector *)NULL_REF,
					guess,
					actual,
					f_weak
				);
	}
    /**
     * Find parameter value for given point on curve.
     */
	virtual double param(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;

	// In each of these five enquiry routines, there are two logical
	// arguments. The first is FALSE unless the most recent call to
	// this routine was for the same curve (to allow reuse of global
	// information about the curve which may be expensive to compute).
	// The second, if TRUE, means that an approximate result is
	// acceptable if this is quicker.
    /**
	* Evaluate the curve, giving position and first and second
	* derivatives.
    */
	virtual void eval(
				double,
				SPAposition &,
				SPAvector & = *(SPAvector *)NULL_REF,	// first derivative
				SPAvector & = *(SPAvector *)NULL_REF,	// second derivative
				logical = FALSE,
				logical = FALSE
			) const;
    /**
     *Find position on curve at given parameter value.
     */
	virtual SPAposition eval_position(
				double, 
				logical = FALSE,
				logical = FALSE
			) const;
    /**
     * Find parametric derivative, magnitude and direction, at point on curve
     */
	virtual SPAvector eval_deriv( double, logical = FALSE, logical = FALSE ) const;
    /**
     * Find curvature at point on curve.
     */
	virtual SPAvector eval_curvature(
				double,
				logical = FALSE,
				logical = FALSE
			) const;
    /**
     * Return a distance value, which represents the greatest
     * discrepancy between positions calculated by calls to eval() or
     * eval_position() with the "approx_OK" logical set by turns to
     * TRUE and FALSE.
     */
	virtual double approx_error() const;
    /**
	 * The evaluate() function calculates derivatives, of any order
	 * up to the number requested, and stores them in vectors provided
	 * by the user. It returns the number it was able to calculate;
	 * this will be equal to the number requested in all but the most
	 * exceptional circumstances. A certain number will be evaluated
	 * directly and (more or less) accurately; higher derivatives will
	 * be automatically calculated by finite differencing; the accuracy
	 * of these decreases with the order of the derivative, as the
	 * cost increases.
     */
	virtual int evaluate(
                double,				// Parameter
                SPAposition &,			// Point on curve at given SPAparameter
                SPAvector ** = NULL, 	// Array of pointers to vectors, of
									// size nd. Any of the pointers may
									// be null, in which case the
									// corresponding derivative will not
									// be returned. 
                int = 0,       		// Number of derivatives required (nd) 
				evaluate_curve_side = evaluate_curve_unknown
									// the evaluation location - above,
									// below or don't care.
            ) const;
    /**
	 * Return the number of derivatives which evaluate() can find
	 * "accurately" (and fairly directly), rather than by finite
	 * differencing, over the given portion of the curve. If there
	 * is no limit to the number of accurate derivatives, returns
	 * the value ALL_CURVE_DERIVATIVES, which is large enough to be
	 * more than anyone could reasonably want.
     */
	virtual int accurate_derivs( 
				SPAinterval const & = *(SPAinterval*)NULL_REF
								 	// Defaults to the whole curve
			) const;
    /**
     * Indicate whether the curve is periodic.  Meshcurves never are.
     */
	virtual logical periodic() const;
    /**
	 * Indicate whether a curve is closed, that is joins itself
	 * (smoothly or not) at the ends of its principal parameter
	 * range.
     */
	virtual logical closed() const;
    /**
     * Period range of compcurvs is exact 0.
     */
	virtual double param_period() const;
    /**
     * Find the parameter range of the compcurv as an interval.
     */
	virtual SPAinterval param_range(
				SPAbox const & = *(SPAbox *)NULL_REF
			) const;
    /**
	 * Arc-length. Return the algebraic distance along the curve
	 * between the given parameters, the sign being positive if the
	 * parameter values are given in increasing order, and negative
	 * if they are in decreasing order. The result is undefined if
	 * either parameter value is outside the parameter range of a
	 * bounded curve. For a periodic curve the parameters are not
	 * reduced to the principal range, and so the portion of the
	 * curve evaluated may include several complete circuits. This
	 * function is therefore always a monotonically increasing
	 * function of its second argument if the first is held constant,
	 * and a decreasing function of its first argument if the second
	 * is held constant. If approx_ok is true, and the curve has a B-spline approximation, 
	 * then this approximation is used in the length calculation.
     */
	virtual double length( double, double, logical approx_ok=TRUE) const;
    /**
 	 * The inverse of the length function. Return the parameter value
	 * of the point on the curve at the given algebraic arc length
	 * from that defined by the datum parameter. The result is not
	 * defined for a bounded non-periodic curve if the datum parameter
	 * is outside the parameter range, or if the length is outside
	 * the range bounded by the values for the ends of the parameter
	 * range. If approx_ok is true, and the curve has a B-spline approximation, 
	 * then this approximation is used in the length calculation.
     */
	virtual double length_param(
				double,			// datum parameter
				double,			// arc length
				logical approx_ok=TRUE
			) const;
    /**
     * Find the extrema of a curve in a given direction,ignoring its ends unless it is closed.
     */
	virtual curve_extremum *find_extrema( SPAunit_vector const & ) const;
    /**
     * Test point-on-curve, to given precision.
     */
	virtual logical test_point_tol(
				SPAposition const &,
				double = 0,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF
			) const;
    /**
     * Return a cylinder which encloses the portion of the curve bounded by the interval
     */
	//
	virtual BOUNDING_CYLINDER enclosing_cylinder( const SPAinterval& = 
												    *(SPAinterval*)NULL_REF ) const;
    /**
     * Return an identifier uniquely specifying the curve type
     */
	virtual int type() const;
    /**
     * Return a string identifying the curve type
     */
	virtual char const *type_name() const;
    /**
	 * Save and restore.  For save, we bypass the usual member
	 * function and define a new one that takes the entity lists
	 * required to convert pointers into integers for the file.
	 * The standard restore mechanism is used.
     */
	virtual void save() const;
    /**
     *
     */
	virtual void save(ENTITY_LIST &  ) const;
    /**
     *
     */
	friend DECL_KERN curve *restore_compcurv();
    /**
     *
     */
    void restore_data();
    /**
     * Output details of the intcurve for inspection.
     */
	virtual void debug(
				char const *,
				FILE * = debug_file_ptr
			) const;
    /**
     * Allow the curve entity private access.
     */
	friend class COMPCURV;
};

/**
 * @nodoc
 * Contains the actual node and element data.
 * It is kept seperate from the compcurv class
 * to allow deferred local copies.  However, unlike the spl_sur
 * class, com_cur is not designed to support multiple references
 * within a model.  This should not be a problem for now since
 * using a com_cur as a pipe spine or a sweep path is not likely
 * anytime soon.
 * Also unlike spl_sur, the com_cur is not a pure class.  It
 * can be used as is.  In fact, there is probably no reason for
 * deriving other classes from it, as the composite curve should be
 * completely general.
*/
class DECL_KERN com_cur : public ACIS_OBJECT {
private:

	// Use count data;

	int use_count;

	// Store current evaluation mode (local or averaged).

	evaluate_curve_side eval_mode;

	// Store whether this object is lost or not.

	logical lost;

	// Scan list data

	scan_list scan_list_data;

	// Param subdivision bounding boxes in a tree structure.

	par1_tree *boxes_ptr;

	// The actual head of the element list.

	ELEM1D *element_ptr;

	// Constructors.

	// Construct a generally null com_cur. This is to allow flexibility
	// for constructors for derived classes to set the common data
	// members in whatever way is most convenient.

	com_cur();

	// Destructor.

	~com_cur();

	// Lose function, since the nodes and elements are entities.

	void lose();

	// Scan the node and element data and fill up the entity lists
	// in the attribute.

	void scan();

	// Pointer fixing.

	void fix_pointers( ENTITY ** );

private:

	// Use count manipulation, just passing through the underlying
	// ones.

	void operator++();
	void operator--();
	logical mult();

	// Build up the param and object-space bounding tree.

	void build_boxes();

public:
    /**
	 * Basic constructor, given curves.
     */
	com_cur(int, curve**, SPAposition *, logical);
    /**
	 * Constructor given a list.
     */
	com_cur(ELEM1D *);
    /**
	 * Read the element.  Allow a non-const read of
	 * the element of a const curve.  This is ok since we
	 * want to prevent changes to the cur, not its entities.
     */
	ELEM1D *element() const { return element_ptr; }
    /**
	 * Sset the element.  Allow a non-const read of
	 * the element of a const curve.  This is ok since we
	 * want to prevent changes to the cur, not its entities.
     */
	void set_element(ELEM1D *el) { element_ptr = el; }
    /**
	 * Set lose data.
     */
	void set_lost(logical l ) { lost = l; }
    /**
	 * Store the scan stuff.
     */
	scan_list &get_scan_list() { return scan_list_data; }
    /**
	 * Read the bounding boxes.
     */
	par1_tree *boxes() { return boxes_ptr; }
    /**
	 * Const version.
     */
	const par1_tree *boxes() const { return boxes_ptr; }
    /**
	 * Get an element (one of two in seam cases) that
	 * corresponds with the parameter value.
     */
	ELEM1D *get_element(double) const;
    /**
	 * The real copy.  Takes entity lists of the world data
	 * and the curve mesh data for converting pointers into integers.
     */
	com_cur *copy( const ENTITY_LIST & = *(ENTITY_LIST *)NULL_REF ) const;
    /**
	 * Just do a copy of the pointers and the boxes.  Used in
	 * back up.
     */
	int ref_count() {return use_count;};
    
	com_cur *copy_pointers() const;
    /**
	 * The deep_copy method makes a copy without shared data
     */
	virtual com_cur *deep_copy(pointer_map * pm = NULL) const;
    /**
	 * Zero tolerance equality check.  Good for copies.
     */
	logical operator==( com_cur const & ) const;
    /**
	 * Find a bounding box for the subset of the curve
	 * within the given parameter bounds.
     */
	SPAbox bound( SPAinterval const & = *(SPAinterval *)NULL_REF ) const;
    /**
	 * Return a cone bounding the tangent direction of the curve. The
	 * logical argument indicates that an approximation is ok.
     */
	curve_tancone tangent_cone( SPAinterval const &, logical ) const;
    /**
	 * Divide a curve into two pieces at a given parameter value,
	 * possibly adjusting the spline approximations to an exact value
	 * at the split point. If the parameter value is at the beginning,
	 * sets the first piece to NULL, and places the original curve
	 * in the second slot; if the parameter value is at the end,
	 * places the original curve in the first slot, and sets the
	 * second to NULL.
     */
	void split( double, SPAposition const &, com_cur *[ 2 ] );
    /**
	 * Subset the com_cur.  Calls split and copy.
     */
	com_cur *subset( SPAinterval const & ) const;
    /**
	 * Transform the surface in place.
     */
	void operator*=( SPAtransf const & );
    /**
	 * Geometric evaluation
     * Function to set the evaluation type for node seams for
	 * the next evaluator or geometric call.
     */
	void set_neighborhood(evaluate_curve_side);
    /**
	 * Tangent direction to mesh curve at given point.  Seam
	 * points (nodes) are averaged by default.
     */
	SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;
     /**
	 * Curvature at point on mesh curve.
     */
	SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;
    /**	
	 * Find the closest point given a start radius.
	 * Called from point_perp.
     */
	SPAposition closest_point(
					mesh_tree *,
					const SPAposition&,
					const SPAposition&,
					double &,
					ELEM1D *&,
					double &
				) const;
    /**
	 * Find the foot of the perpendicular from the given point to
	 * the curve, and tangent to and curvature of the curve at that
	 * point, and its parameter value. If an input parameter value
	 * is supplied (as argument 5), the perpendicular found is the
	 * one nearest to the supplied parameter's position, otherwise it
	 * is the one at which the curve is nearest to the given point.
     */
	void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				SPAvector &,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF,
				logical f_weak=FALSE
			) const;
    /**
	 * Parameter value for given point on curve. The base class
     */
	double param(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;
    /**
	 * Find position, first and second derivative on curve at given
	 * parameter value.
     */
	void eval(
				double,
				SPAposition &,
				SPAvector & = *(SPAvector *)NULL_REF,
				SPAvector & = *(SPAvector *)NULL_REF,
				logical = FALSE
			) const;
    /**
	 * Find position on curve at given parameter value.
     */
	SPAposition eval_position( double, logical = FALSE ) const;
    /**
	 * Find parametric derivative, magnitude and direction.
     */
	SPAvector eval_deriv( double, logical = FALSE ) const;
    /**
	 * Find curvature.
     */
	SPAvector eval_curvature( double, logical = FALSE ) const;
    /**
	 * Return a distance value, which represents the greatest
	 * discrepancy between positions calculated by calls to eval() or
	 * eval_position() with the "approx_OK" logical set by turns to
	 * TRUE and FALSE.  Simply return the largest fitol value in
	 * the mesh.
     */
	double approx_error() const;
    /**
	 * The evaluate() function calculates derivatives, of any order
	 * up to the number requested, and stores them in vectors provided
	 * by the user. It returns the number it was able to calculate;
	 * this will be equal to the number requested in all but the most
	 * exceptional circumstances. A certain number will be evaluated
	 * directly and (more or less) accurately; higher derivatives will
	 * be automatically calculated by finite differencing; the accuracy
	 * of these decreases with the order of the derivative, as the
	 * cost increases.
     */
	int evaluate(
                double,				// Parameter
                SPAposition &,			// Point on curve at given parameter
                SPAvector ** = NULL, 	// Array of pointers to vectors, of
									// size nd. Any of the pointers may
									// be null, in which case the
									// corresponding derivative will not
									// be returned. 
                int = 0,       		// Number of derivatives required (nd) 
				evaluate_curve_side = evaluate_curve_unknown
									// the evaluation location - above,
									// below or don't care.
            ) const;

    /**
	 * Return the number of derivatives which evaluate() can find
	 * "accurately" (and fairly directly), rather than by finite
	 * differencing, over the given portion of the curve. If there
	 * is no limit to the number of accurate derivatives, returns
	 * the value ALL_CURVE_DERIVATIVES, which is large enough to be
	 * more than anyone could reasonably want.
     */
	int accurate_derivs( 
				SPAinterval const & = *(SPAinterval*)NULL_REF
								 	// Defaults to the whole curve
			) const;
    /**
	 * Mesh curves are not periodic.
     */
	logical periodic() const;
    /**
	 * Indicate whether a curve is closed, that is joins itself
	 * (smoothly or not) at the ends of its principal parameter
	 * range.
     */
	logical closed() const;
    /**
	 * Returns exact 0.
     */
	double param_period() const;
    /**
	 * Find the parameter range of the interpolated curve as an 
	 * interval.
     */
	SPAinterval param_range( SPAbox const & = *(SPAbox *)NULL_REF ) const;
    /**
	 * Arc-length. Return the algebraic distance along the curve
	 * between the given parameters, the sign being positive if the
	 * parameter values are given in increasing order, and negative
	 * if they are in decreasing order. The result is undefined if
	 * either parameter value is outside the parameter range of a
	 * bounded curve. For a periodic curve the parameters are not
	 * reduced to the principal range, and so the portion of the
	 * curve evaluated may include several complete circuits. This
	 * function is therefore always a monotonically increasing
	 * function of its second argument if the first is held constant,
	 * and a decreasing function of its first argument if the second
	 * is held constant. If approx_ok is true, and the curve has a B-spline approximation, 
	 * then this approximation is used in the length calculation.
     */
	double length( double, double, logical approx_ok=TRUE ) const;
    /**
	 * The inverse of the length function. Return the parameter value
	 * of the point on the curve at the given algebraic arc length
	 * from that defined by the datum parameter. The result is not
	 * defined for a bounded non-periodic curve if the datum parameter
	 * is outside the parameter range, or if the length is outside
	 * the range bounded by the values for the ends of the parameter
	 * range. If approx_ok is true, and the curve has a B-spline approximation, 
	 * then this approximation is used in the length calculation.
     */

	double length_param(
				double,			// datum parameter
				double,			// arc length
				logical approx_ok=TRUE
			) const;
    /**
	 * Find the extrema of an intersection curve in a given direction,
	 * ignoring its ends unless it is closed.
     */
	curve_extremum *find_extrema( SPAunit_vector const & ) const;
    /**
     * Test point-on-curve, to given precision.
     */
	logical test_point_tol(
				SPAposition const &,
				double = 0,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF
			) const;
    /**
     * Find a cylinder that bounds the mesh.
     */
	virtual BOUNDING_CYLINDER enclosing_cylinder( const SPAinterval& = 
												    *(SPAinterval*)NULL_REF ) const;
    /** 
     * Returns type name
     */
	char const *type_name() const;
    /**
     *
     */
	void save(ENTITY_LIST & = *(ENTITY_LIST *)NULL_REF) const;
    /**
     *
     */
	friend DECL_KERN com_cur *restore_com_cur();
    /**
     *
     */
	void restore_data();

    /**
     * Debug printout.
     */
	void debug(
				char const *,
				logical,
				FILE *
			) const;
    /**
     *
     */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;

	// Allow the curve class to get at any private data.

	friend class compcurv;
	friend class pointer_map;
};

#endif
