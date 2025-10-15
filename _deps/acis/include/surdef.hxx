/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for (lower-case) surface.
// The generic surface is defined as a portmanteau for the specific
// surfaces. It contains nothing itself, except virtual functions for
// the derived types to elaborate. Some functions are pure, forcing
// derived classes to define their own versions, and preventing the
// construction of an object of the base class, and some have default
// definitions which may be useful in derived classes.
// The surface must always be passed by (constant) reference or
// pointer, so as not to lose the specific information.
//
/*******************************************************************/
#if !defined( surface_CLASS )
#define surface_CLASS

#include "dcl_kern.h"
#include "logical.h"
#include "debugmsc.hxx"
#include "unitvec.hxx"
#include "param.hxx"
/**
 * \defgroup ACISSURFACES surfaces
 * \ingroup ACISGEOMETRY
 */
 /**
  * @file surdef.hxx
  * @CAA2Level L1
  * @CAA2Usage U2
  * @addtogroup ACISSURFACES
  *
  * @{
  */
class curve;
class SPAposition;
class SPAvector;
class SPApar_pos;
class SPApar_vec;
class SPApar_dir;
class SPApar_box;
class SPAtransf;
class SPAinterval;
class SPAbox;
class surf_normcone;
class surf_princurv;
class discontinuity_info;
class check_status_list;
class check_fix;
class surface_evaldata;
class spline;
class surface;
class pointer_map;
class law;
class SizeAccumulator;
class minimize_helper;

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_istream;
class D3_ostream;
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>(D3_istream&, surface*&);
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<(D3_ostream&, surface const&);

#endif
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN surface* restore_surface();
/**
 * @nodoc
 */
DECL_KERN surface* dispatch_restore_su(int);
/**
 * @nodoc
 */
DECL_KERN surface* dispatch_restore_su(char*);
/*
// tbrv
*/
/**
 * Enumerated type used to determine which of four possible derivatives to evaluate in evaluate() when there is a discontinuity.
 * <br>
 * <b>Role:</b> The evaluation point on a surface can be illustrated in the following way as:<pre>
 *            |
 *         I  | II
 *       -----+-----
 *        III | IV
 *            |       </pre>
 * where the horizonal is the <i>u</i> direction and the vertical, <i>v</i>.
 * @param evaluate_surface_above_above
 * II of the quadrant.
 * @param evaluate_surface_below_above
 * I  of the quadrant.
 * @param evaluate_surface_below_below
 * III of the quadrant.
 * @param evaluate_surface_above_below
 * IV of the quadrant.
 * @param evaluate_surface_unknown
 * Not a specific quadrant.
 */
enum evaluate_surface_quadrant {
    evaluate_surface_above_above,
    evaluate_surface_below_above,
    evaluate_surface_below_below,
    evaluate_surface_above_below,
    evaluate_surface_unknown
};

// STI ROLL
// STI swa 27Jul98
/**
 * Enumerated type used to determine sweep path type if the surface can be thought of as a swept surface.
 * <br>
 * <b>Role:</b> For example, a torus and a <tt>rot_spl_sur</tt> both have a circular
 * sweep path type.
 * @param unknown_path_type
 * For surfaces that cannot be considered swept.
 * @param straight_path_type
 * The path is a straight line.
 * @param circular_path_type
 * The path is an ellipse.
 * @param defined_path_type
 * For surfaces with a given sweep path (e.g. @href sweep_spl_sur )
 */
enum sweep_path_type {
    unknown_path_type,
    straight_path_type,
    circular_path_type,
    defined_path_type
};
// STI swa END

// The surface class proper, defining a large range of virtual functions,
// mostly to be implemented by derived classes.
/**
 * Base class for all ACIS surface types, which defines the basic virtual functions that  are supplied for all specific surface classes.
 * <br>
 * <b>Role:</b> The surface class is the base class that all ACIS surface types
 * (plane, cone, sphere, torus, and spline) are derived. The surface class defines
 * the basic virtual functions that are supplied for all specific surface classes.
 * Some of these functions are pure; i.e., the derived classes must define their
 * own version; others have default definitions that can be used by the derived classes.
 * <br><br>
 * All ACIS surfaces have a parameterization scheme defined for them; however,
 * the analytic surfaces (plane, cone, sphere, and torus) are not considered
 * parametric surfaces. The only true parametric surface is the spline surface.
 * <br><br>
 * The parameterization of any ACIS surface maps a rectangle within a 2D vector
 * space (<i>u</i>,<i>v</i>-parameter space) into a 3D real vector space (<i>xyz</i> object space).
 * A surface is closed in <i>u</i> (or <i>v</i>) if the opposite sides of the rectangle map
 * into identical curves in object space. If the derivatives also match at these
 * boundaries, the surface is periodic in that parameter. If one side of this
 * rectangle maps into a single point in object space, this point is a parametric
 * singularity. If the surface normal is not continuous at this point, the point
 * is a surface singularity.
 * <br><br>
 * The parameterization can be either right-handed (i.e., the surface normal is
 * the cross product of <i>u</i> and <i>v</i>) or left-handed (i.e., the normal is the cross
 * product of <i>v</i> and <i>u</i>).
 * @see blend_support, int_cur, off_spl_sur, skin_spl_sur, surf_surf_int, surface_law_data, SPApar_box
 */
class DECL_KERN surface : public ACIS_OBJECT {

protected:

    // Any surface may be subsetted to a given SPAparameter range
/**
 * Range to which this <tt>surface</tt> is subsetted.
 */
    SPApar_box subset_range;
public:
    /**
     * Constructs a <tt>surface</tt> (default constructor).
     * <br><br>
     * <b>Role:</b> Requests memory for this object but does not populate it.
     */
    surface();  // Force creation of all surfaces to be by constructor
    /**
     * C++ destructor, deleting a <tt>surface</tt>.
     */
    virtual ~surface();	// Ensure any derived class destructor gets
    // a say when we destroy a surface.


// Make a copy of the given surface.  This function is not virtual
// so that we can handle a NULL "this".  It calls make_copy().
/**
 * Makes a copy of this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> This method calls <tt>make_copy</tt>.
 */
    surface* copy_surf() const;


    // Make a copy of the given surface. This is a pure virtual
    // function to ensure that each derived class defines its own.
/**
 * Makes a copy of this <tt>surface</tt> on the heap, and returns a pointer to it.
 */
    virtual surface* make_copy() const = 0;

    // The make copy method of surface sometimes shares data with
    // use counts.  The deep_copy method makes all shared data unshared.
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
    virtual surface* deep_copy(pointer_map* pm = NULL) const = 0;


    //
    // DEBUG FUNCTIONS:
    //

    // method to write scheme commands to file for visualization the surface
    // [in] pFp   - the file to write
    // [in] Color - color of the entity drawn
    //
/**
 * @nodoc
 */
    virtual void debug_scheme(FILE* pFp, int Color) const;

    // method to write scheme commands to file for visualization an U-isoline
    // [in] pFp    - the file to write
    // [in] Color  - color of the entity drawn
    // [in] U      - the value of U-isoline
    // [in] RangeV - the range of the V SPAparameter
    //
/**
 * @nodoc
 */
    void debug_scheme_isoline_u(FILE* pFp, int Color, double U, SPAinterval& RangeV) const;

    // method to write scheme commands to file for visualization an V-isoline
    // [in] pFp    - the file to write
    // [in] Color  - color of the entity drawn
    // [in] V      - the value of V-isoline
    // [in] RangeU - the range of the U SPAparameter
    //
/**
 * @nodoc
 */
    void debug_scheme_isoline_v(FILE* pFp, int Color, double V, SPAinterval& RangeU) const;



    // Test two surfaces for equality. This, like testing floating-
    // point numbers for equality, is not guaranteed to say "equal" for
    // effectively-equal surfaces, but is guaranteed to say "not equal"
    // if they are indeed not equal. The result can be used for
    // optimisation, but not where it really matters. The default
    // always says "not equal", for safety.
/**
 * Tests this <tt>surface</tt> for equality to another surface.
 * <br><br>
 * <b>Role:</b> This method does not guarantee equality for effectively-equal
 * surfaces, but it is guaranteed to determine inequality if the two surfaces
 * are not equal. Use this result for optimization.
 * <br><br>
 * @param surf
 * surface to compare for equality.
 */
    virtual logical operator==(const surface& surf) const;
    /**
     * Tests this <tt>surface</tt> for inequality to another surface.
     * <br><br>
     * <b>Role:</b> This method does not guarantee equality for effectively-equal
     * surfaces, but it is guaranteed to determine inequality if the two surfaces
     * are not equal. Use this result for optimization.
     * <br><br>
     * @param surf
     * surface to compare for inequality.
     */
    logical operator!=(const surface& surf) const {
        return !(*this == surf);
    }
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual logical operator>>(const surface&) const;

    // Transform a surface.
/**
 * Transforms this <tt>surface</tt> by the given transformation.
 * <br><br>
 * @param transf
 * transformation to apply.
 */
    virtual surface& operator*=(const SPAtransf& transf) = 0;

    // Reverse the sense of a surface.
/**
 * Reverses the sense of this <tt>surface</tt>.
 */
    virtual surface& negate() = 0;

    // Generic function to determine whether a surface is defined or
    // not. A NULL one or a generic surface are always "undefined",
    // others depend on their contents.
/**
 * Determines whether this <tt>surface</tt> is defined or undefined.
 * <br><br>
 * <b>Role:</b> A <tt>NULL</tt> or generic surface is always undefined; for other surfaces,
 * it depends on their contents.
 */
    logical undefined() const;
    /**
     * Determines whether this <tt>surface</tt> is defined or undefined.
     * <br><br>
     * <b>Role:</b> A <tt>NULL</tt> or generic surface is always undefined; for other surfaces,
     * it depend on their contents.
     */
    virtual logical undef() const;

    // Play around with subset surfaces. These will probably be virtual,
    // but not during testing.
    // Construct a subsetted copy.
/**
 * Constructs a subset copy, within the given parameter box, of this surface.
 * <br><br>
 * @param box
 * parameter box.
 */
    surface* subset(const SPApar_box& box) const;
    /**
     * Constructs a subset copy, within the given <i>u</i> interval, of this surface.
     * <br><br>
     * @param range
     * u interval.
     */
    surface* subset_u(const SPAinterval& range) const;
    /**
     * Constructs a subset copy within the given <i>v</i> interval.
     * <br><br>
     * @param range
     * v interval
     */
    surface* subset_v(const SPAinterval& range) const;

    // Subset a surface in place
/**
 * Subsets this <tt>surface</tt> in place, according to the given parameter box.
 * <br><br>
 * @param box
 * parameter box defining the subset.
 */
    void limit(const SPApar_box& box);
    /**
     * Subsets this <tt>surface</tt> in place, according to the given interval in <i>u</i>.
     * <br><br>
     * @param range
     * interval defining the subset.
     */
    void limit_u(const SPAinterval& range);
    /**
     * Subsets this <tt>surface</tt> in place, according to the given interval in <i>v</i>.
     * <br><br>
     * @param range
     * interval defining the subset.
     */
    void limit_v(const SPAinterval& range);

    // Construct a copy of the unbounded surface underlying this one.
/**
 * Constructs a copy of the unbounded surface underlying this one.
 */
    surface* unsubset() const;
    /**
     * Constructs a copy of the unbounded (in <i>u</i>) surface underlying this one.
     */
    surface* unsubset_u() const;
    /**
     * Constructs a copy of the unbounded (in <i>v</i>) surface underlying this one.
     */
    surface* unsubset_v() const;

    // Remove the subsetting from this surface.
/**
 * Removes the subsetting from this <tt>surface</tt>.
 */
    void unlimit();
    /**
     * Removes the subsetting from this <tt>surface</tt>, in the <i>u</i> direction.
     */
    void unlimit_u();
    /**
     * Removes the subsetting from this <tt>surface</tt>, in the <i>v</i> direction.
     */
    void unlimit_v();

    // Indicate whether the surface has a significant subset range.
/**
 * Determines whether this <tt>surface</tt> has a significant subset range.
 */
    logical subsetted() const;
    /**
     * Determines whether this <tt>surface</tt> has a significant subset range in the
     * <i>u</i> direction.
     */
    logical subsetted_u() const;
    /**
     * Determines whether this <tt>surface</tt> has a significant subset range in the <i>v</i> direction.
     */
    logical subsetted_v() const;

    // return subsets
/**
 * Returns the subset box of this <tt>surface</tt>.
 */
    SPApar_box subset_box() const;
    /**
     * Returns a subset interval of this <tt>surface</tt>, in <i>u</i>.
     */
    SPAinterval subset_u_interval() const;
    /**
     * Returns a subset interval of this <tt>surface</tt>, in <i>v</i>.
     */
    SPAinterval subset_v_interval() const;

    // Return a SPAbox around the surface. This need not be the smallest
    // SPAbox which contains the specified portion of the surface, but
    // needs to balance the tightness of the bound against the cost of
    // evaluation.

    // *** These will be pure, but to ease implementation will start
    // *** off with dummy defaults.

    // Surface bounded in SPAparameter space.
/**
 * Returns an object-space box bounding this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> This box need not be the smallest box that contains the specified
 * portion of the surface, but it must balance the tightness of the bound against
 * the cost of the evaluation.
 * <br><br>
 * @param parbox
 * the region of interest.
 * @param transf
 * optional transformation.
 */
    virtual SPAbox bound(
        const SPApar_box& parbox = *(SPApar_box*)NULL_REF,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const;

    // Surface bounded in object space.
/**
 * Returns an object-space box bounding this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> This box need not be the smallest box that contains the specified
 * portion of the surface, but it must balance the tightness of the bound against
 * the cost of the evaluation.
 * <br><br>
 * @param box
 * the region of interest.
 * @param transf
 * optional transformation.
 */
    virtual SPAbox bound(
        const SPAbox& box,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const;


    // Return a cone bounding the normal direction of a surface. The
    // cone is deemed to have its apex at the origin, and has a given
    // axis direction and (positive) half-angle. If the logical
    // argument is TRUE, then a quick approximation may be found. The
    // approximate result may lie wholly within or wholly outside the
    // guaranteed bound (obtained with a FALSE argument), but may not
    // cross from inside to outside. Flags in the returned object
    // indicate whether the cone is in fact the best available, and
    // if not whether this result is inside or outside the best cone.

    // *** This will probably become pure, but to ease implementation
    // *** it will start as a dummy in the surface object.
/**
 * Returns a cone bounding the normal direction of this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> The cone is deemed to have its apex at the origin and have a given
 * axis direction and (positive) half-angle. If the logical argument is <tt>TRUE</tt>, then
 * this method finds a quick approximation. The approximate result may lie wholly
 * within or wholly outside the guaranteed bound (obtained with a <tt>FALSE</tt> argument), but may
 * not cross from inside to outside. Flags in the turned object indicate whether the
 * cone is the best available and, if not, whether this result is inside or outside
 * the best cone.
 * <br><br>
 * @param parbox
 * the given region.
 * @param approx_OK
 * approximate result is ok.
 * @param transf
 * transformation to apply.
 */
    virtual surf_normcone normal_cone(
        const SPApar_box& parbox,	// SPAparameter bounds
        logical approx_OK = FALSE,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const;


    // Geometric enquiries

    // Return TRUE if the surface is parametric, FALSE if not. This
    // reflects the fundamental nature of the surface, and is used
    // by the Acis kernel to avoid the use of parameters where they
    // are unnecessary. For a surface, the Acis kernel does not
    // use any of the SPAparameter-based functions, though they are
    // in fact implemented for the sake of those husks and applications
    // which prefer SPAparameter-based representations of every surface.

    // Whether to declare a surface parametric or not depends largely
    // on the implementation of the point-based evaluations: if the
    // availability of a good approximation to the point's SPAparameter
    // values makes little or no improvement to the speed of these
    // functions then the surface should be declared non-parametric,
    // otherwise they should be made parametric. A plane is an obvious
    // non-parametric surface, a B-spline an obvious parametric one.
/**
 * Returns <tt>TRUE</tt> if this <tt>surface</tt> is a parametric surface - otherwise returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Role:</b> This result reflects the fundamental nature of the surface and is used
 * by the ACIS kernel to avoid using parameters when they are not necessary.
 * For a surface, the ACIS kernel does not use any of the parameter-based functions,
 * though they are implemented for the sake of those components and applications
 * that prefer parameter-based representations of every surface.
 * <br><br>
 * Whether to declare a surface parametric or not depends on the implementation
 * of the point-based evaluations. If the availability of a good approximation to
 * the point's parameter values makes no significant improvement to the speed of
 * these functions, then the surface is nonparametric; otherwise, it is parametric.
 * A plane is an obvious nonparametric surface; a B-spline is a parametric surface.
 */
    virtual logical parametric() const;


    // Divide a surface into separate pieces which are smooth (and therefore
    // suitable for offsetting or blending). The surface is split if the
    // curvature exceeds the minimum curvature argument. If it is closed after
    // this, it is then split into two. The functions return the number of pieces, 	  // and the pieces themselves are a return argument. Only implemented for
    // splines and elliptical cones.
/**
 * Divides this <tt>surface</tt> along <i>u</i> into separate pieces that are smooth (and therefore suitable for offsetting or blending).
 * <br><br>
 * <b>Role:</b> The surface is split if the curvature exceeds the minimum curvature
 * argument. If it is closed after this, it is then split into two. The split
 * pieces are stored in the pieces argument. The function returns the count of
 * split pieces. Only implemented for splines and elliptical cones.
 * <br><br>
 * @param pieces
 * the returned split pieces.
 * @param curvature
 * the minimum curvature.
 */
    virtual int split_at_kinks_u(spline**& pieces, double curvature = 0.0) const;
    /**
     * Divides this <tt>surface</tt> along <i>v</i> into separate pieces that are smooth (and therefore suitable for offsetting or blending).
     * <br><br>
     * <b>Role:</b> The surface is split if the curvature exceeds the minimum curvature
     * argument. If it is closed after this, it is then split into two. The split
     * pieces are stored in the pieces argument. The function returns the count of
     * split pieces. Only implemented for splines and elliptical cones.
     * <br><br>
     * @param pieces
     * the returned split pieces.
     * @param curvature
     * the minimum curvature.
     */
    virtual int split_at_kinks_v(spline**& pieces, double curvature = 0.0) const;


    // Position-based enquiries, implemented for all surfaces, but
    // with an extra argument to speed up processing for parametric
    // ones by allowing SPAparameter values of a point near the
    // desired point, to be given if known.


    // Find the normal to the surface at a point on the surface
    // nearest to the given point.
/**
 * Finds the normal to this <tt>surface</tt> at a point on the surface nearest to the given position.
 * <br><br>
 * @param pos
 * the given position.
 * @param param_guess
 * parameter guess.
 */
    virtual SPAunit_vector point_normal(
        const SPAposition& pos,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF
    ) const = 0;

    // Find an outward direction from the surface at a point on
    // the surface nearest to the given point. This will usually be
    // the normal, but if the nearest point is a singularity (like the
    // apex of a cone) this routine will still return an outward
    // direction. The base class definition just returns point_normal()
    // so may be used by default by simple surfaces.
/**
 * Finds the outward direction from this <tt>surface</tt> at a point nearest the given position.
 * <br><br>
 * <b>Role:</b> This method usually returns the normal, but if the nearest point is a
 * singularity (like the apex of a cone), this method still returns an outward
 * direction. The base class definition returns <tt>point_normal</tt>, which is used by
 * default on simple surfaces.
 * <br><br>
 * @param pos
 * given position.
 * @param guess
 * parameter guess.
 */
    virtual SPAunit_vector point_outdir(
        const SPAposition& pos,
        const SPApar_pos& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the principal axes of curvature of the surface at a
    // given point, and the curvatures in those directions.
/**
 * Finds the principal axes of curvature of this <tt>surface</tt> at a given point, and the curvatures along these axes.
 * <br><br>
 * @param pos
 * position at which to evaluate the curvatures and curvature axes.
 * @param axis1
 * first axis of curvature.
 * @param cur1
 * curvature along the first axis.
 * @param axis2
 * second axis of curvature.
 * @param cur2
 * curvature along the second axis.
 * @param param_guess
 * parameter guess.
 */
    virtual void point_prin_curv(
        const SPAposition& pos,
        SPAunit_vector& axis1,		// first axis direction
        double& cur1,			// curvature in first direction
        SPAunit_vector& axis2,		// second axis direction
        double& cur2,			// curvature in second direction
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF
    ) const = 0;


    // Do the same, but return the value in a struct defined for the
    // purpose.
/**
 * Finds the principal axes of curvature of this <tt>surface</tt> at a given point and the curvatures in those directions, in a structure defined for the purpose.
 * <br><br>
 * @param pos
 * position at which to evaluate the curvature.
 * @param guess
 * parameter guess
 */
    surf_princurv point_prin_curv(
        const SPAposition& pos,
        const SPApar_pos& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the curvature of a cross-section curve of the surface at
    // the given point.  The cross-section curve is given by the
    // intersection of the surface with a plane passing through the
    // given point and with given normal. This can always be derived
    // from the principal curvatures, but there may be a much quicker
    // way for some surfaces.
/**
 * Finds the curvature of a cross-sectional curve of this <tt>surface</tt> at the given point.
 * <br><br>
 * <b>Role:</b> The cross-sectional curve is given by the intersection of the surface
 * with a plane passing through the given point and with the given normal.
 * <br><br>
 * @param pos
 * position on the plane.
 * @param normal
 * normal to the plane.
 * @param param_guess
 * parameter guess.
 */
    virtual double point_cross(
        const SPAposition& pos,
        const SPAunit_vector& normal,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF
    ) const;

    /**
     * Finds the curvature of a cross-sectional curve of this <tt>surface</tt> at the given point.
     * <br><br>
     * <b>Role:</b> The cross-sectional curve is given by the intersection of the surface
     * with a plane passing through the given point and with the given normal.
     * <br><br>
     * @param pos
     * position on the plane.
     * @param normal
     * normal to the plane.
     * @param param_guess
     * parameter guess.
     * @param use_sided_eval
     * use sided evaluation for derivatives.
     * @param right_side
     * evaluate right side of direction.
     */
    virtual double point_cross(
        const SPAposition& pos,
        const SPAunit_vector& normal,
        const SPApar_pos& param_guess,
        logical use_sided_eval,
        logical right_side
    ) const;

    // Find the point on the surface nearest to the given point and
    // optionally the normal to and principal curvatures of the
    // surface at that point.  If the surface is parametric, also
    // return the SPAparameter values at the found point.
/**
 * Finds the point on this <tt>surface</tt> nearest to the given point and, optionally, the normal to and the principal curvatures of the surface at that point.
 * <br><br>
 * <b>Role:</b> If the surface is parametric, this method also returns the parameter
 * values at the found point.
 * <br><br>
 * @param pos
 * the input position.
 * @param foot
 * the position on the surface.
 * @param normal
 * the returned normal.
 * @param cur
 * the principal curvature.
 * @param param_guess
 * parameter guess.
 * @param param_actual
 * actual parameter position.
 * @param f_weak
 * weak flag - for internal use.
 */
    virtual void point_perp(
        const SPAposition& pos,
        SPAposition& foot,
        SPAunit_vector& normal,
        surf_princurv& cur,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const = 0;
    /**
     * Finds the point on this <tt>surface</tt> nearest to the given point and, optionally, the normal to the surface at that point.
     * <br><br>
     * <b>Role:</b> If the surface is parametric, this method also returns the parameter
     * values at the found point.
     * <br><br>
     * @param pos
     * the input position.
     * @param foot
     * the position on the surface.
     * @param normal
     * the returned normal.
     * @param param_guess
     * parameter guess.
     * @param param_actual
     * actual parameter position.
     * @param f_weak
     * weak flag - for internal use.
     */
    void point_perp(
        const SPAposition& pos,
        SPAposition& foot,
        SPAunit_vector& normal,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            normal,
            *(surf_princurv*)NULL_REF,
            param_guess,
            param_actual, f_weak
        );
    }
    /**
     * Finds the point on this <tt>surface</tt> nearest to the given point.
     * <br><br>
     * <b>Role:</b> If the surface is parametric, this method also returns the parameter
     * values at the found point.
     * <br><br>
     * @param pos
     * the input position.
     * @param foot
     * the position on the surface.
     * @param param_guess
     * parameter guess.
     * @param param_actual
     * actual parameter position.
     * @param f_weak
     * weak flag - for internal use.
     */
    void point_perp(
        const SPAposition& pos,
        SPAposition& foot,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            *(SPAunit_vector*)NULL_REF,
            *(surf_princurv*)NULL_REF,
            param_guess,
            param_actual, f_weak
        );
    }


    // Now there is a set of SPAparameter-based functions. The Acis
    // kernel only requires them to have defined results if the
    // surface is parametric (i.e. the member function parametric()
    // returns TRUE), but husks and applications may expect valid
    // results for all surface types.

    // Find the SPAparameter values of a point on a surface, given an
    // optional first guess.
/**
 * Finds the parameter position of a given point on this <tt>surface</tt>, given an optional first guess.
 * <br><br>
 * @param pos
 * the position for which the parameter position is to be found.
 * @param param_guess
 * parameter guess.
 */
    virtual SPApar_pos param(
        const SPAposition& pos,
        const SPApar_pos& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the direction in SPAparameter space of a given object
    // space direction on a surface at a given SPAparameter value.
/**
 * Finds the direction in parameter space of a given object-space direction on this <tt>surface</tt>, at a given parameter position.
 * <br><br>
 * @param dir
 * object-space direction for which the parameter-space direction is to be determined.
 * @param pos
 * parameter position at which the direction is to be determined.
 */
    SPApar_dir param_dir(
        const SPAunit_vector& dir,
        const SPApar_pos& pos
    ) const;


    // Find the rate of change in surface SPAparameter corresponding to
    // a unit velocity in a given object-space direction at a given
    // SPAposition in SPAparameter space..
/**
 * Finds the rate of change in the surface parameter corresponding to a unit velocity in a given object-space direction, at a given position in parameter space.
 * <br><br>
 * @param dir
 * object-space direction for which the result is determined.
 * @param pos
 * parameter position at which the result is determined.
 */
    virtual SPApar_vec param_unitvec(
        const SPAunit_vector& dir,
        const SPApar_pos& pos
    ) const;

    /**
     * Finds the rate of change in the surface parameter corresponding to a unit velocity in a given object-space direction, at a given position in parameter space.
     * <br><br>
     * @param dir
     * object-space direction for which the result is determined.
     * @param pos
     * parameter position at which the result is determined.
     * logical well_conditioned which would be equal to FALSE if the matrix is ill/bad conditioned.
     */
    virtual SPApar_vec param_unitvec(
        const SPAunit_vector& dir,
        const SPApar_pos& pos, logical& well_conditioned
    ) const;

    // Find the point on a parametric surface with given SPAparameter
    // values, and optionally the first and second derivatives as
    // well or instead.
/**
 * Finds the position on a parametric surface corresponding to the given parameter values, and optionally the first and second derivatives as well.
 * <br><br>
 * @param param
 * parameter position at which to perform the evaluation.
 * @param pos
 * position on surface at the specified parameter position.
 * @param first_derivs
 * first derivatives (array of length 2 in the order Xu, Xv).
 * @param second_derivs
 * second derivatives (array of length 3 in the order Xuu, Xuv, Xvv).
 */
    virtual void eval(
        const SPApar_pos& param,
        SPAposition& pos,
        SPAvector* first_derivs = NULL,	// first derivatives - array of
        // length 2, in order xu, xv
        SPAvector* second_derivs = NULL		// second derivatives - array of
        // length 3, in order xuu, xuv, xvv
    ) const;


    // Find the point on a parametric surface with given SPAparameter
    // values.
/**
 * Finds the point on a parametric surface with the given parameter position.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the surface position.
 */
    virtual SPAposition eval_position(const SPApar_pos& pos) const;

    // Find the normal to a parametric surface at a point with given
    // SPAparameter values.
/**
 * Finds the normal to a parametric surface at the point with the given parameter position.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the surface normal.
 */
    virtual SPAunit_vector eval_normal(const SPApar_pos& pos) const;

    // Find an outward direction from the surface at a point with
    // given SPAparameter values. This will usually be the normal, but
    // if the nearest point is a singularity (like the apex of a
    // cone) this routine will still return an outward direction.
/**
 * Finds the outward direction from this <tt>surface</tt> at the point with the given parameter position.
 * <br><br>
 * <b>Role:</b> This method usually returns the normal, but if the nearest point
 * is a singularity (like the apex of a cone), it still returns an outward direction.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the outward direction.
 */
    virtual SPAunit_vector eval_outdir(const SPApar_pos& pos) const;

    // Find the principal axes of curvature of the surface at a
    // point with given SPAparameter values, and the curvatures in those
    // directions.
/**
 * Finds the principal axes of curvature of this <tt>surface</tt> at the point with the given parameter position, and the curvatures in those directions.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the curvatures and curvature axes.
 * @param axis1
 * first axis of curvature.
 * @param cur1
 * curvature along the first axis.
 * @param axis2
 * second axis of curvature.
 * @param cur2
 * curvature along the second axis.
 */
    virtual void eval_prin_curv(
        const SPApar_pos& pos,
        SPAunit_vector& axis1,		// first axis direction
        double& cur1,			// curvature in first direction
        SPAunit_vector& axis2,		// second axis direction
        double& cur2			// curvature in second direction
    ) const;


    // Do the same, but return the value in a struct defined for the
    // purpose.
/**
 * Finds the principal axes of curvature of this <tt>surface</tt> at the point with the given parameter position and the curvatures in those directions, in a structure defined for the purpose.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the curvature.
 */
    surf_princurv eval_prin_curv(const SPApar_pos& pos) const;

    // Find the curvature of a cross-section curve of the parametric
    // surface at the point with given SPAparameter values.  The
    // cross-section curve is given by the intersection of the
    // surface with a plane passing through the point and with given
    // normal.
/**
 * Finds the curvature of a cross-sectional curve of the parametric surface at the point with the given parameter position.
 * <br><br>
 * <b>Role:</b> The cross-sectional curve is given by the intersection of the surface
 * with a plane passing through the given point and with the given normal.
 * <br><br>
 * @param pos
 * position on the interecting plane.
 * @param normal
 * normal to the interecting plane.
 */
    virtual double eval_cross(
        const SPApar_pos& pos,
        const SPAunit_vector& normal
    ) const;


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
 * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
 * <br><br>
 * <b>Role:</b> This method returns the number of derivatives it was able to
 * calculate; usually, this equals the requested number. A certain number are
 * evaluated directly and accurately; higher derivatives are automatically
 * calculated by finite differencing. The accuracy of these decreases with the
 * order of the derivative, as the cost increases. Any of the pointers may be <tt>NULL</tt>,
 * in which case the corresponding derivatives will not be returned. Otherwise
 * they must point to arrays long enough for all the derivatives of that order.
 * <br><br>
 * @param param
 * the parameter position at which the surface is to be evaluated.
 * @param pos
 * point on the surface at the given parameter position.
 * @param derivs
 * pointer arrays of derivatives.
 * @param nd
 * the number of derivatives required.
 * @param quad
 * the evaluation location - above, below, or "don't care" (for each parameter direction).
 */
    virtual int evaluate(
        const SPApar_pos& param,	// Parameter
        SPAposition& pos,			// Point on surface at given SPAparameter
        SPAvector** derivs = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int nd = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant quad = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each SPAparameter direction,
        // or don't care.
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
 * <tt>evaluate_iter</tt> is additionally supplied with a data object which contains results
 * from a previous close evaluation, for use as initial values for any iteration involved.
 * This object may furthermore be updated to reflect the results of this evaluation.
 * <br><br>
 * @param param
 * the parameter position at which the surface is to be evaluated.
 * @param data
 * data supplying initial values, and set to reflect the results of this evaluation.
 * @param pos
 * point on the curve at the given parameter.
 * @param derivs
 * pointer arrays of derivatives.
 * @param nd
 * the number of derivatives required.
 * @param quad
 * the evaluation location - above, below, or "don't care" (for each parameter direction).
 */
    virtual int evaluate_iter(
        const SPApar_pos& param,	// Parameter
        surface_evaldata* data,	// Data supplying initial values, and
        // set to reflect the results of this
        // evaluation.
        SPAposition& pos,			// Point on surface at given SPAparameter
        SPAvector** derivs = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int nd = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant quad = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each SPAparameter direction,
        // or don't care.
    ) const;

    // Construct a data object to retain evaluation information across
    // calls to evaluate_iter(). This is to allow subsidiary calls
    // within an iterative evaluator to start iteration much closer to the
    // required result than is possible just using the curve information
    // itself. The default returns NULL, indicating that no special
    // information is required or usable.
/**
 * Constructs a data object to retain evaluation information across calls to <tt>evaluate_iter</tt>.
 * <br><br>
 * <b>Role:</b> This is to allow subsidiary calls within an iterative evaluator
 * to start iteration much closer to the required result than is possible just
 * using the curve information itself.
 */
    virtual surface_evaldata* make_evaldata() const;

    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.

/**
 * @nodoc
 */
#define ALL_SURFACE_DERIVATIVES 9999

 /**
  * Returns the number of derivatives of this <tt>surface</tt> that can be found accurately and relatively directly.
  * <br><br>
  * <b>Role:</b> Returns the number of derivatives that can be found by <tt>evaluate</tt>
  * accurately and relatively directly, rather than by finite differencing over the given
  * portion of the surface. If there is no limit to the number of accurate derivatives,
  * returns the value <tt>ALL_SURFACE_DERIVATIVES</tt>.
  * <br><br>
  * @param box
  * portion of the surface to be considered (optional).
  */
    virtual int accurate_derivs(
        const SPApar_box& box = *(SPApar_box*)NULL_REF
        // Defaults to the whole surface
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
     * <b>Role:</b> This method is available to any derived class for use in its own
     * <tt>evaluate</tt> method. It calls back the <tt>evaluate</tt> function for adjacent
     * points to evaluate a number of derivatives, so <tt>evaluate</tt> must ensure that
     * this does not cause a further call to this method.  See <tt>evaluate</tt> for details
     * regarding the parameters.
     * <br><br>
     * @param param
     * the parameter position at which the surface is to be evaluated.
     * @param pos
     * point on the surface at the given parameter.
     * @param derivs
     * pointer arrays of derivatives.
     * @param nd
     * the number of derivatives required.
     * @param nfound
     * the number of derivatives already evaluated, and directly evaluable in the neighborhood of the param.
     * @param du
     * the finite differencing step to use in the u-direction.
     * @param dv
     * the finite differencing step to use in the v-direction.
     * @param quad
     * the evaluation location - above, below, or "don't care" (for each parameter direction).
     */
    int finite_difference_derivatives(
        const SPApar_pos& param,	// Parameter
        SPAposition& pos,			// Point on surface at given SPAparameter
        SPAvector** derivs, 			// Array of pointers to arrays of
        // vectors, of size nd. See evaluate()
        // for details.
        int nd,	       		// Number of derivatives required (nd)
        int nfound,				// Number of derivatives already
        // evaluated, and directly evaluable
        // in the neighbourhood of the param
        // (nfound)
        double du,				// finite differencing step to use
        // in the u SPAparameter
        double dv,				// finite differencing step to use
        // in the v SPAparameter
        evaluate_surface_quadrant quad
        // the evaluation quadrant
    ) const;

public:
    // STI aed: add planar query method
        // Report whether all points on a surface are within SPAresabs of a plane.
        // Returns the normal to the plane if the surface is planar.
        // Guaranteed not to report planar if the surface is non-planar, but not
        // guaranteed to report planar for a surface which may actually be planar.
        // The default reports non-planar for safety.
    /**
     * Reports whether this <tt>surface</tt> is planar.
     * <br><br>
     * @param pos
     * point on the surface.
     * @param normal
     * the planar normal.
     */
    virtual logical planar(
        SPAposition& pos,
        SPAunit_vector& normal
    ) const;
    // STI aed: end

        // Report whether a parametric surface is periodic in either
        // SPAparameter direction (i.e. it is smoothly closed, so faces can
        // run over the seam).
    /**
     * Determines if this <tt>surface</tt> is periodic in the <i>u</i>-parameter direction (i.e., if it is smoothly closed, so that faces can run over the seam).
     */
    virtual logical periodic_u() const;
    /**
     * Determines if this <tt>surface</tt> is periodic in the <i>v</i>-parameter direction (i.e., if it is smoothly closed, so that faces can run over the seam).
     */
    virtual logical periodic_v() const;


    // Report whether a parametric surface is closed, smoothly or
    // not, in either SPAparameter direction. Note that the "closed"
    // function will always return TRUE if the corresponding "periodic"
    // one does.
/**
 * Determines whether this <tt>surface</tt> is closed, smoothly or not, in the <i>u</i>-parameter direction.
 * <br><br>
 * <b>Role:</b> Always returns <tt>TRUE</tt> if the corresponding <tt>periodic</tt> method
 * returns <tt>TRUE</tt>.
 */
    virtual logical closed_u() const;
    /**
     * Determines whether this <tt>surface</tt> is closed, smoothly or not, in the <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> Always returns <tt>TRUE</tt> if the corresponding <tt>periodic</tt> method returns <tt>TRUE</tt>.
     */
    virtual logical closed_v() const;


    // Return the period of a periodic parametric surface, zero if
    // the surface is not periodic in the chosen SPAparameter or not
    // parametric.
/**
 * Returns the period, in <i>u</i>, of a periodic parametric surface.
 * <br><br>
 * <b>Role:</b> If the surface is not parametric, or is not periodic in the <i>u</i>-direction, this method returns 0.
 */
    virtual double param_period_u() const;
    /**
     * Returns the period, in <i>v</i>, of a periodic parametric surface.
     * <br><br>
     * <b>Role:</b> If the surface is not parametric or is not periodic in the
     * <i>v</i>-direction, this method returns 0.
     */
    virtual double param_period_v() const;


    // Return the principal SPAparameter range of a surface in
    // a chosen SPAparameter direction.

    // A periodic surface is defined for all SPAparameter values in the
    // periodic direction, by reducing the given SPAparameter modulo the
    // period into this principal range. For a surface open or non-
    // periodic in the chosen direction the surface evaluation
    // functions are defined only for SPAparameter values in the
    // returned range.

    // If a SPAbox is provided, the SPAparameter range returned may be
    // restricted to a portion of the surface which is guaranteed to
    // contain all portions of the surface which lie within the region
    // of interest. If none is provided, and the SPAparameter range in
    // some direction is unbounded, then conventionally an empty
    // SPApar_box is returned.
/**
 * Returns the principal parameter range of this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> A periodic surface is defined for all parameter values in the
 * periodic direction by reducing the given parameter modulo the period into this
 * principal range. For a surface that is open or nonperiodic in the chosen
 * direction, the surface evaluation functions are defined only for the parameter
 * values in the returned range.
 * <br><br>
 * If a box is provided, the parameter range returned may be restricted to a
 * portion of the surface that is guaranteed to contain all portions of the surface
 * that lie within the region of interest. If none is provided and the parameter
 * range in some direction is unbounded, then this method returns an empty interval.
 * <br><br>
 * @param box
 * box restricting the result.
 */
    virtual SPApar_box param_range(
        const SPAbox& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the <i>u</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction by reducing the given parameter modulo the period into this
     * principal range. For a surface that is open or nonperiodic in the chosen
     * direction, the surface evaluation functions are defined only for the parameter
     * values in the returned range.
     * <br><br>
     * If a box is provided, the parameter range returned may be restricted to a portion
     * of the surface that is guaranteed to contain all portions of the surface that
     * lie within the region of interest. If none is provided and the parameter range
     * in some direction is unbounded, then this method returns an empty interval.
     * <br><br>
     * @param box
     * box restricting the result.
     */
    virtual SPAinterval param_range_u(
        const SPAbox& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction by reducing the given parameter modulo the period into this
     * principal range. For a surface that is open or nonperiodic in the chosen
     * direction, the surface evaluation functions are defined only for the parameter
     * values in the returned range.
     * <br><br>
     * If a box is provided, the parameter range returned may be restricted to a portion
     * of the surface that is guaranteed to contain all portions of the surface that
     * lie within the region of interest. If none is provided and the parameter range
     * in some direction is unbounded, then this method returns an empty interval.
     * <br><br>
     * @param box
     * box restricting the result.
     */
    virtual SPAinterval param_range_v(
        const SPAbox& box = *(SPAbox*)NULL_REF
    ) const;


    // Report whether the surface parametrisation is singular at
    // the specified u or v SPAparameter value. The only singularity
    // recognised is where every value of the non-constant SPAparameter
    // generates the same object-space point, and these can only
    // occur at the ends of the SPAparameter range as returned by
    // the functions above.
/**
 * Determines whether the parameterization of this <tt>surface</tt> is singular at the specified <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> The only singularity recognized is where every value of the
 * nonconstant parameter generates the same object-space point, and these can
 * only occur at the ends of the parameter range.
 * <br><br>
 * @param uparam
 * u-parameter value to test.
 */
    virtual logical singular_u(
        double uparam	// constant u SPAparameter
    ) const;
    /**
     * Determines whether the parameterization of this <tt>surface</tt> is singular at the specified <i>v</i>-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the
     * nonconstant parameter generates the same object-space point, and these can
     * only occur at the ends of the parameter range.
     * <br><br>
     * @param vparam
     * v-parameter value to test.
     */
    virtual logical singular_v(
        double vparam	// constant v SPAparameter
    ) const;


    // Indicate whether the SPAparameter coordinate system of the surface
    // is right- or left-handed. With a right-handed system, at any
    // point the outward normal is given by the cross product of the
    // increasing u direction with the increasing v direction, in that
    // order. With a left-handed system the outward normal is in the
    // opposite direction from this cross product.
/**
 * Indicates whether the parameter coordinate system of this <tt>surface</tt> is right-handed or left-handed.
 * <br><br>
 * <b>Role:</b> With a right-handed system, the outward normal at any point is given
 * by the cross product of the increasing <i>u</i>-direction with the increasing
 * <i>v</i>-direction, in that order. With a left-handed system, the outward normal is
 * in the opposite direction from this cross product.
 */
    virtual logical left_handed_uv() const;

    // Construct a SPAparameter line on the surface. A u SPAparameter line
    // runs in the direction of increasing u SPAparameter, at constant v.
    // A v SPAparameter line runs in the direction of increasing v, at
    // constant u. The parametrisation in the non-constant direction
    // matches that of the surface, and has the range obtained by
    // use of param_range_u() or param_range_v() appropriately. If
    // the supplied constant SPAparameter value is outside the valid
    // range for the surface, or at a singularity, a NULL is returned.

    // The new curve is constructed in free store, so it is the
    // responsibility of the caller to ensure that it is correctly
    // deleted.
/**
 * Constructs a parameter line on this <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> A <i>u</i>-parameter line runs in the direction of increasing <i>u</i> parameter,
 * at constant <i>v</i>. The parameterization in the nonconstant direction matches that
 * of the surface, and it has the range obtained by the use of <tt>param_range_u</tt>.
 * If the supplied constant parameter value is outside the valid range for the
 * surface, or if it is at a singularity, this method returns <tt>NULL</tt>.
 * <br><br>
 * The new curve is constructed in free storage, so it is the responsibility of
 * the caller to ensure that it is correctly deleted.
 * <br><br>
 * @param vparam
 * <i>v</i> parameter defining the parameter line.
 */
    virtual curve* u_param_line(
        double vparam		// constant v SPAparameter
    ) const;
    /**
     * Constructs a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>v</i>-parameter line runs in the direction of increasing <i>v</i> parameter,
     * at constant <i>u</i>. The parameterization int he nonconstant direction matches that
     * of the surface, and it has the range obtained by the use of <tt>param_range_v</tt>.
     * If the supplied constant parameter value is outside the valid range for the
     * surface, or if it is at a singularity, this method returns <tt>NULL</tt>.
     * <br><br>
     * The new curve is constructed in free storage, so it is the responsibility of
     * the caller to ensure that it is correctly deleted.
     * <br><br>
     * @param uparam
     * <i>u</i> parameter defining the parameter line.
     */
    virtual curve* v_param_line(
        double uparam		// constant u SPAparameter
    ) const;


    // Test whether a point lies on the surface, to system precision
/**
 * Determines whether a point lies on this <tt>surface</tt> to the system precision.
 * <br><br>
 * @param pos
 * position to test.
 * @param uv_guess
 * parameter guess.
 * @param uv_actual
 * actual parameter.
 */
    logical test_point(
        const SPAposition& pos,
        const SPApar_pos& uv_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& uv_actual = *(SPApar_pos*)NULL_REF
    ) const
    {
        return test_point_tol(pos, 0, uv_guess, uv_actual);
    }


    // Test whether a point lies on the surface, to given precision
/**
 * Determines whether a point lies on this <tt>surface</tt> to the given tolerance.
 * <br><br>
 * @param pos
 * position to test.
 * @param tol
 * tolerance to use (defaults to SPAresabs).
 * @param uv_guess
 * parameter guess.
 * @param uv_actual
 * actual parameter.
 */
    virtual logical test_point_tol(
        const SPAposition& pos,
        double tol = 0,		// defaults to SPAresabs
        const SPApar_pos& uv_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& uv_actual = *(SPApar_pos*)NULL_REF
    ) const = 0;


    // Return (in a read-only array) the number and SPAparameter values of
    // discontinuities of the surface, of the given order (maximum three).
/**
 * Returns the number and parameter values of discontinuities in <i>u</i> of this <tt>surface</tt>, of the given order (maximum three), in a read-only array.
 * <br><br>
 * @param n_discont
 * number of discontinuities found.
 * @param order
 * order of the discontinuities.
 */
    virtual const double* discontinuities_u(int& n_discont, int order) const;
    /**
     * Returns the number and parameter values of discontinuities in <i>v</i> of this <tt>surface</tt>, of the given order (maximum three), in a read-only array.
     * <br><br>
     * @param n_discont
     * number of discontinuities found.
     * @param order
     * order of the discontinuities.
     */
    virtual	const double* discontinuities_v(int& n_discont, int order) const;


    // Return (in a read-only array) the number and SPAparameter values of
    // discontinuities of the surface, up to the given order (maximum three).
/**
 * Returns the number and parameter values of all discontinuities in <i>u</i> of this <tt>surface</tt>, up to the given order (maximum three).
 * <br><br>
 * <b>Role:</b> The returned array is read-only.
 * <br><br>
 * @param n_discont
 * number of discontinuities found.
 * @param order
 * maximum order of the discontinuities.
 */
    virtual const double* all_discontinuities_u(int& n_discont, int order);
    /**
     * Returns the number and parameter values of all discontinuities in <i>v</i> of this <tt>surface</tt>, up to the given order (maximum three).
     * <br><br>
     * <b>Role:</b> The returned array is read-only.
     * @param n_discont
     * number of discontinuities found.
     * @param order
     * maximum order of the discontinuities.
     */
    virtual	const double* all_discontinuities_v(int& n_discont, int order);


    // State whether a particular SPAparameter value is a discontinuity, and if so,
    // return the order of the discontinuity.
/**
 * Determines whether a particular value of <i>u</i> represents a discontinuity of this <tt>surface</tt>.
 * <br><br>
 * @param u
 * parameter value to test.
 */
    virtual	int	discontinuous_at_u(double u) const;
    /**
     * Determines whether a particular value of <i>v</i> represents a discontinuity of this <tt>surface</tt>.
     * <br><br>
     * @param v
     * parameter value to test.
     */
    virtual	int	discontinuous_at_v(double v) const;


    // Return read-only access to disc_info objects, if they exist.  The
    // default version of the functions return null.
/**
 * Returns read-only access to a <tt>discontinuity_info</tt> object, if there is one, detailing discontinuities in <i>u</i>.
 * <br><br>
 * <b>Role:</b> The default version of this method returns <tt>NULL</tt>.
 */
    virtual const discontinuity_info& get_disc_info_u() const;
    /**
     * Returns read-only access to a <tt>discontinuity_info</tt> object, if there is one, detailing discontinuities in <i>v</i>.
     * <br><br>
     * <b>Role:</b> The default version of this method returns <tt>NULL</tt>.
     */
    virtual const discontinuity_info& get_disc_info_v() const;

    // Return type of surface.
/**
 * Returns the type of this <tt>surface</tt>.
 */
    virtual int type() const = 0;

    // Return string identifier of surface.
/**
 * Returns the string <tt>"surface"</tt>.
 */
    virtual const char* type_name() const = 0;


    // Save and restore. Save is easy, as derived class switching goes
    // through the normal virtual function mechanism. Restore is more
    // complicated, because until it is invoked we do not have an
    // object of the right class. Instead we switch on a table defined
    // by static instances of the restore_cu_def class (see below), to
    // invoke a simple friend function which constructs an object of
    // the right (derived) type. Then it can call the appropriate
    // member function to do the actual work. The save_data and
    // restore_data function for each class can be called in
    // circumstances when we know what type of surface we are expecting
    // and have one in our hand to be filled in.

    // Function to be called to save a surface of unknown type, or
    // NULL. Just checks for null, then calls save().
/**
 * Saves this <tt>surface</tt> if the surface is of an unknown type or <tt>NULL</tt>.
 * <br><br>
 * <b>Role:</b> Checks for NULL, then calls the <tt>save</tt> method.
 */
    void save_surface() const;

    // Function to save a surface of unknown type, together with its
    // type code for later retrieval.
/**
 * Calls the virtual <tt>save</tt> method for the particular type of this <tt>surface</tt>.
 */
    virtual void save() const = 0;

    // Function to save a surface of a known type, where the context
    // determines the surface type, so no type code is necessary.
/**
 * Saves the information for this <tt>surface</tt> to a save file.
 */
    void save_data() const;

    /**
     * Queries whether the object needs to be saved to an old version as an approximation.
     * <br><br>
     * <b>Role:</b>
     *    Called by SAT save APIs with check_progenitors==TRUE to determine if an object
     *    needs to be tolerantly simplified before being saved to an old version.  The
     *    check_progenitors flag indicates that the object should call need_save_as_approx()
     *    on any curve or surface construction geometry upon which it depends, and return
     *    TRUE if any of the progenitors do.  Customers with their own derived curve or surface types
     *    should override this routine to return true whenever they want their geometry to be imported
     *    (as a B-spline approximation) into other applications or when saving to a
     *    version of their application before the derived geometry type in question existed.
     * @param save_to_version
     * version to which the object will be saved
     * @param check_progenitors
     * if TRUE, query construction geometry and base class as well
     */
    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

    // Function to be called to restore a surface of unknown type -
    // reads the type code, and switches to surface-specific restore
    // code.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN surface* restore_surface();

    // Function to restore the data for a surface of known type, either
    // where the context determines the type, or after the type code
    // has been read and processed.
/**
 * Restores the data for this <tt>surface</tt> from a save file.
 * <br><br>
 * <b>Role:</b><pre>
 *    if (restore_version_number &gt;= BNDSUR_VERSION)
 * 	     read_interval		subset <i>u</i> interval
 * 	     read_interval		subset <i>v</i> interval </pre>
 */
    void restore_data();


    // Notify the derived type that the surface has been changed (e.g. the
    // subset_range has changed) so that it can update itself.  The default
    // version of the function does nothing.
/**
 * Notifies the derived type that this <tt>surface</tt> has been changed.
 * <br><br>
 * <b>Role:</b> Used when the <tt>subset_range</tt> has changed, so that the surface
 * can update itself. The default version of this method does nothing.
 */
    virtual	void change_event();

    // Check for any data errors in the surface, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // surface on exit.  The default version of this function does nothing.
    // See chk_stat.hxx for information on the argument types used here.
/**
 * Checks for any data errors in this <tt>surface</tt>, and corrects the errors if possible.
 * <br><br>
 * <b>Role:</b> The various arguments provide control over which checks are made,
 * which fixes can be applied and which fixes were actually applied. The function
 * returns a list of errors that remain in the surface on exit.
 * <br><br>
 * The default for the set of flags which say which fixes are allowable is none
 * (nothing is fixed). If the list of checks to be made is <tt>NULL</tt>, then every
 * possible check will be made. Otherwise, the function will only check for things
 * in the list. Note that it is possible for errors not in the input list to be
 * returned, as certain problems preclude other checks being performed. In such
 * situations, the return list will contain errors which were not requested, but
 * which have prevented requested checks from being made.
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
    /**
     * @nodoc
     */
    friend DECL_KERN D3_istream& operator>>(
        D3_istream&,
        surface*&
        );
    /**
     * @nodoc
     */
    friend DECL_KERN D3_ostream& operator<<(
        D3_ostream&,
        surface const&
        );
    /**
     * @nodoc
     */
    virtual void print(
        D3_ostream&
    ) const;

    /**
     * @nodoc
     */
    virtual void input(
        D3_istream&
    );
#endif


    // Print out details of the surface.
/**
 * Prints out a title line and details about this <tt>surface</tt> to the debug file or any other specified file.
 * <br><br>
 * @param title
 * title string.
 * @param fp
 * FILE pointer.
 */
    virtual void debug(
        const char* title,
        FILE* fp = debug_file_ptr
    ) const = 0;

    // STI swa 27Jul98 -- functions used to determine the surface sweep information, when applicable
public:
    /**
     * Returns the sweep path type for this <tt>surface</tt>.
     */
    virtual sweep_path_type get_path_type() const { return unknown_path_type; }
    /**
     * Returns the sweep path curve for this <tt>surface</tt>. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const { return NULL; }
    /**
     * Returns the sweep profile on this <tt>surface</tt>. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     * <br><br>
     * @param param
     * parameter at which to get the profile.
     */
    virtual curve* get_profile(double param/*param*/) const
    {
        SPAUNUSED(param)
            return NULL;
    }
    /**
     * Returns the rail law for this swept surface.
     */
    virtual law* get_rail() const { return NULL; }
    // STI swa END

        // STI ROLL
    /**
     * @nodoc
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL

/**
 * @nodoc
 */
    virtual void minimize(minimize_helper*) {}

public:
    // set/unset the flag for the sanity check.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void set_checked() {}
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void unset_checked() {}

protected:

    // Functions to validate the uv guess so that it lies within param range of the surface

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_uv_guess(const SPApar_pos& uv_guess,
        SPApar_pos& valid_uv_guess
    ) const;
    /**
     * @nodoc
     */
    virtual double param_period_u_ignoring_limits() const;
    /**
     * @nodoc
     */
    virtual double param_period_v_ignoring_limits() const;

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_v_guess(const SPAparameter& v_guess,
        SPAparameter& valid_v_guess) const;

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_u_guess(const SPAparameter& u_guess,
        SPAparameter& valid_u_guess) const;

public:
    SPApar_box gme_get_subset_range();
    void gme_set_subset_range(const SPApar_box& box);
}; // End of surface class definition
/** @} */

// end validate functions

// Define a class for the return value from normal_cone(). This
// defines a bound on the surface normal, used in intersection code
// to eliminate the possibility of tangencies and common normals.
/*
// tbrv
*/
/**
 * @nodoc
 */
enum surface_vardir {
    surface_var_unknown,
    surface_var_ugreater,
    surface_var_vgreater
};

/**
 * @addtogroup ACISSURFACES
 * @{
 */

 /**
  * Provides a return value for <tt>normal_cone</tt>, which returns a cone bounding the surface normal.
  * <br>
  * <b>Role:</b> Defines a bound on the surface normal. It is used in intersection
  * code to eliminate the possibility of tangencies and common normals.
  * @see SPAunit_vector
  */
class DECL_KERN surf_normcone : public ACIS_OBJECT {
public:
    /**
     * Axis direction for the cone.
     * <br><br>
     * <b>Role:</b> The cone is deemed to have its apex at the origin.
     */
    SPAunit_vector axis;
    /**
     * Positive half angle defining the bounding cone.
     */
    double angle;
    /**
     * Classifies whether the normal direction varies more in the <i>u</i>-parameter direction, more in the <i>v</i>-parameter direction, or in neither.
     * <br><br>
     * <b>Role:</b> This item is to be used only as a hint.
     */
    surface_vardir vardir;	// classifies whether the normal
    // direction varies more in the u
    // SPAparameter direction, more in the
    // v direction, or in neither
    // particularly. To be used only as a
    // hint.
/**
 * Is <tt>TRUE</tt> if this is only an approximation to the best cone available.
 */
    logical approx;	// TRUE if this is only an approximation to the
    // best cone available
/**
 * If <tt>approx</tt> is <tt>TRUE</tt>, this flag is <tt>TRUE</tt> if this cone is entirely outside the best available cone and <tt>FALSE</tt> if it is inside.
 */
    logical oversize;// if approx is TRUE, TRUE if this cone is
    // entirely outside the best cone available,
    // FALSE if it is inside.
/**
 * Constructs a <tt>surf_normcone</tt> from the specified parameters.
 * <br><br>
 * @param ax
 * axis direction of the cone.
 * @param ang
 * positive half angle.
 * @param vdir
 * variation direction.
 * @param approx_OK
 * are approximate results OK?
 * @param over
 * inside or outside?
 */
    surf_normcone(
        const SPAunit_vector& ax,
        double ang,
        surface_vardir vdir,
        logical approx_OK,
        logical over
    );
};  // End of surf_normcone definition
/** @} */

/**
 * @nodoc
 */
inline surf_normcone::surf_normcone(
    const SPAunit_vector& ax,
    double ang,
    surface_vardir vdir,
    logical app,
    logical over
) : axis(ax)
{
    angle = ang;
    vardir = vdir;
    approx = app;
    oversize = over;
}

/**
 * @addtogroup ACISSURFACES
 * @{
 */

 // Return value for principal curvature functions, returning two
 // directions and two curvatures.
 /**
  * Provides the return value for the principal curvature functions, returning two directions and two curvatures for a surface.
  * @see SPAunit_vector
  */
class DECL_KERN surf_princurv : public ACIS_OBJECT {
public:
    /**
     * First direction vector.
     */
    SPAunit_vector dir1;
    /**
     * The first curvature.
     */
    double curv1;
    /**
     * The second direction vector.
     */
    SPAunit_vector dir2;
    /**
     * The second curvature.
     */
    double curv2;
    /**
     * Constructs a <tt>surf_princurv</tt> (default constructor).
     * <br><br>
     * <b>Role:</b> Requests memory for this object but does not populate it.
     */
    surf_princurv() {}
    /**
     * Constructs a <tt>surf_princurv</tt> from two curvatures and two directions.
     * <br><br>
     * @param d1
     * first direction vector.
     * @param c1
     * first curvature.
     * @param d2
     * second direction vector.
     * @param c2
     * second curvature.
     */
    surf_princurv(
        const SPAunit_vector& d1,
        double c1,
        const SPAunit_vector& d2,
        double c2
    );
}; // End of surf_princurv definition
/** @} */

/**
 * @nodoc
 */
inline surf_princurv::surf_princurv(
    const SPAunit_vector& d1,
    double c1,
    const SPAunit_vector& d2,
    double c2
) : dir1(d1), dir2(d2)
{
    curv1 = c1;
    curv2 = c2;
}

// Restore mechanism for surfaces. Static declarations of objects
// of this class form themselves into a table containing the external
// (integer) identifier of the particular surface subtype, together
// with a pointer to the correct restore routine. The surface
// restore routine reads the external identifier, and switches
// according to the table. Note that the save routine doesn't need
// any of this stuff, as the virtual function mechanism works fine
// for that.

// The table will be very short (half a dozen entries at most), and
// so can be simply a linear list, for ease of implementation.
/**
 * @nodoc
 */
typedef surface* (*rest_su_fn_type)();
/*
// tbrv
// knt - we reference "restore_su_def" in the cone and
// ellipse R10 docs.  Maybe we should doc it?  Or remove
// references to it?
*/
/**
 * @nodoc
 */
class DECL_KERN restore_su_def : public ACIS_OBJECT {
    restore_su_def* next;	// list pointer
    int type;				// surface subtype (old style)
    char const* name;		// surface subtype (new style)
    rest_su_fn_type rest_fn;// restore function
public:
    restore_su_def(
        int,			// internal type number (for old
        // save files)
        char const*,	// external type identifier (for
        // new save files)
        rest_su_fn_type
    );
    ~restore_su_def();

    // Note that old save files have an integer surface type, while
    // new ones have a character string. Implement the dispatch
    // for both.

    friend DECL_KERN surface* dispatch_restore_su(int);
    friend DECL_KERN surface* dispatch_restore_su(char*);
};

// The default implementation of eval_prin_curv uses the first and
// second parametric derivatives to find the axes and curvatures. This
// is also useful within the implementation of spline surfaces, so
// the difficult bit of the algorithm is pulled out and made global.
/**
 * @nodoc
 */
DECL_KERN void surf_deriv_to_curv(
    SPAvector const*,	// array of two first derivatives
    SPAvector const*,	// array of three second derivatives
    SPAunit_vector&,	// set to first principal axis
    double&,		// set to first principal curvature
    SPAunit_vector&,	// set to second principal axis
    double&		// set to second principal curvature
);

// Data carrier for spanning successive calls to the evaluator, as occur
// within evaluations of other curves and surfaces. Surface types capable of
// using such information declare a derived class containing the information
// useful to that specific surface evaluator, including (probably) pointers
// to curve_evaldata and surface_evaldata objects for support curves and
// surfaces.
/*
// tbrv
*/
/**
 * @nodoc
 */
class surface_evaldata : public ACIS_OBJECT {
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

    surface_evaldata();
    virtual ~surface_evaldata();
};

#endif
