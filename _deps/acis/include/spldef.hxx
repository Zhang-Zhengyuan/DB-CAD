/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for spline.

// This class records a B-spline surface.

// A spline holds a pointer to a spl_sur, an internal class defined
// defined so that we can use use-counts to avoid too much copying,
// and a logical denoting reversal of the sense of the stored surface.
// The spl_sur, which will always in fact be an object of a class
// derived from spl_sur, contains the detailed definition of the
// surface, normally with a spline approximation as well, to assist
// in global analysis. All access to the spl_sur is done via virtual
// functions, which may therefore be adapted to different surface
// types. Some are pure, but many are given default action, either
// using other virtual functions or using the spline approximation.
/*******************************************************************/

#if !defined( spline_CLASS )
#define spline_CLASS	
#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "cur_sur.hxx"
#include "surdef.hxx"
#include "extend.hxx"
#include "bs3surf.hxx"
#include "debugmsc.hxx"
#include "subtrans.hxx"
#include "transf.hxx"
#include "box.hxx"

/**
* @file spldef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_istream;

class D3_ostream;

#endif

class spl_sur;
class splsur_cache;

class curve;

class discontinuity_info;
class SPAposition;
class SPAvector;
class SPAunit_vector;

class SPAinterval;
class SPAbox;
class SPApar_box;

class GSM_progen_spl_sur;

class eval_sscache_entry;
class surface_evaluator_splsur;
class minimize_helper;
class geometry_definition_processor;

/**
* Controls the preservation of knot values when one B-spline surface is derived from another.
* <br>
* <b>Role:</b> This enumeration is used to signal that you desire that a derived B-spline surface
* have the same knot values as the surface on which it is based (its progenitor).  This is true
* of surfaces that form the primary definition of a face's geometry as well as of surfaces
* used as approximations to such surfaces.
* <br><br>
* This flag is currently recognized only in the context of spring back deformations.
* @param NO_COPY_KNOTS
* Do not copy knot values.
* @param COPY_KNOTS_INITIALLY
* Preserve knot values through the given operation.  Knots values may subsequently change.
**/
enum copy_knots_mode
{
    NO_COPY_KNOTS,
    COPY_KNOTS_INITIALLY
};


// Define an identifying type for this (lower-case) surface.

#define spline_type 10

class spline;
/**
 * Transforms a spline surface.
 *<br><br>
 * @param name
 * spline name.
 * @param trans
 * transformation.
 */
DECL_KERN spline operator*(spline const& name, SPAtransf const& trans);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN surface* restore_spline();
/*
// tbrv
*/
/**
 * @nodoc
 */
void calc_patches(spl_sur& sur, double& fit, double*& knots1, int& nknots1,
    double*& knots2, int& nknots2, logical uv_type,
    SPAinterval& orig_range1, SPAinterval& orig_range2,
    SPAinterval& range1, SPAinterval& range2);
/*
// tbrv
*/
/**
 * @nodoc
 */
logical calc_patch(spl_sur& sur, double& fit, double*& knots, int& nknots,
    logical uv_type, logical dir_type, logical calc_knots,
    SPAinterval& erange1, SPAinterval& erange2,
    SPAinterval& srange);

# if ( defined D3_DEBUG || defined D3_STANDALONE )
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<(D3_ostream& os, const spl_sur& ss);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>(D3_istream& is, spl_sur*& ss);
#endif

/**
 * Represents a parametric surface.
 * <br>
 * <b>Role:</b> The <tt>spline</tt> class represents a parametric surface that maps a
 * rectangle within a <tt>2D</tt> real vector space (parameter space) into a <tt>3D</tt> real vector
 * space (object space). This mapping must be continuous, and one-to-one except
 * possibly at the boundary of the rectangle in parameter space. It is differentiable
 * twice, and the normal direction is continuous, though the derivatives need not be.
 * The positive direction of the normal is in the sense of the cross product of the
 * partial derivatives with respect to <i>u</i> and <i>v</i> in that order. The portion of the
 * neighborhood of any point on the surface that the normal points to is outside
 * the surface, and the other part is inside.
 *<br><br>
 * Opposite sides of the rectangle can map into identical lines in object space,
 * in which case the surface is <i>closed</i> in the parameter direction normal to those
 * boundaries. If the parameterization and derivatives also match at these boundaries,
 * the surface is <i>periodic</i> in this parameter direction. The line in object space
 * corresponding to the coincident boundaries is known as the <i>seam</i> of a periodic surface.
 *<br><br>
 * If a surface is periodic in one parameter direction, it is defined for all values
 * of that parameter. A parameter value outside the domain rectangle is brought
 * within the rectangle by adding a multiple of the rectangle's width in that
 * parameter direction, and the surface evaluated at that value. If the surface is
 * periodic in both parameters, it is defined for all parameter pairs <i>(u,v)</i>, with
 * reduction to standard range happening with both parameters.
 *<br><br>
 * One side of the rectangle can map into a single point in object space. This point
 * is a <i>parametric singularity</i> of the surface. If the surface normal is not
 * continuous at this point, it is a <i>geometric singularity</i>.
 *<br><br>
 * The spline contains a pointer to another structure, a <tt>spl_sur</tt> or something derived from it,
 * which contains the bulk of the information about the surface, and a <tt>"reversed"</tt> bit,
 * which specifies whether the spline is reversed with respect to the <tt>spl_sur</tt>.
 * Providing this indirection serves two purposes. First, when a spline is duplicated,
 * the copy simply points to the same <tt>spl_sur</tt> and does not copy the bulk of the data.
 * The system maintains a <i>use count</i> in each <tt>spl_sur</tt>. This allows automatic duplication
 * if a shared <tt>spl_sur</tt> is to be modified, and deletes any <tt>spl_sur</tt> no longer needed.
 * Second, the <tt>spl_sur</tt> contains virtual functions to perform all spline operations
 * that depend on the type of the surface. Therefore, new surface types can be defined by declaring
 * and implementing derived classes and the spline class (and everything using it) require no changes
 * to make use of the new surface type.
 *<br><br>
 * @see discontinuity_info, spl_sur, SPLINE, sub_spl_sur
 */
class DECL_KERN spline : public surface {
    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef spline_FRIENDS
    spline_FRIENDS
#endif

private:
    spl_sur* spl;		// Spline surface

    logical rev;		// True if this surface opposes the direction
    // of the underlying one.

    discontinuity_info	u_disc_info;
    discontinuity_info	v_disc_info;
    // Discontinuity information. If the supporting geometry of the surface
    // has discontinuities, or if the surface has a default (tangent)
    // extension, then it will have discontinuities. These are stored here.
    // Note that this is a copy of the data stored in the corresponding
    // spl_sur, but with values outside the subset range removed.  It is
    // necessary to keep a separate copy to provide a read-only array to the
    // data because the surface may be periodic, and the subset may span the
    // periodic joins, resulting in discontinuities which may be outside the
    // periodic range.

    void		update_u_disc_info();
    void		update_v_disc_info();
    // Update the discontinuity information if a change_event occurs.

// STI dgh  make_single_ref public for use in DS husk
public:

    // Ensure that the reference supplied points to a singly-used
    // record.  Take no action if it is already single, otherwise
    // copy everything.
/**
 * Makes a single reference to this spline.
 */
    void make_single_ref();

    // STI dgh public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    spline() {
        spl = NULL;
    }


    // Construct a spline from a spline.  Ups the use-count of the
    // underlying spl_sur and copies the sense logical, but defers
    // further copying until needed.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 *<br><br>
 * @param spl
 * given spline
 */
    spline(
        spline const& spl
    );


    // Construct a spline from a pointer to an underlying spl_sur
    // (usually in fact a class derived from it). This counts as
    // adding one new reference to the spl_sur, for the purposes
    // of eventual deletion.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * <b>Role:</b> Constructs a spline from a pointer to an underlying <tt>spl_sur</tt>
 * (usually in fact a class derived from it). This adds one new reference to the
 * <tt>spl_sur</tt>, for the purposes of eventual deletion.
 *<br><br>
 * @param surf
 * spline surface.
 */
    spline(
        spl_sur* surf
    );


    // Construct a spline from a bs3_surface, which is the type
    // which represents the fundamental parametric surface. The
    // resulting spline surface is taken to be exactly the
    // bs3_surface supplied. After construction, the bs3_surface
    // is owned by the spline object, so should not be reused or
    // deleted by the caller.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * <b>Role:</b> Constructs a spline from a <tt>bs3_surface</tt>, which is the type that
 * represents the fundamental B-spline surface. The resulting spline surface
 * is taken to be exactly the <tt>bs3_surface</tt> supplied. After construction, the
 * <tt>bs3_surface</tt> is owned by the spline object, so should not be reused or deleted
 * by the caller.
 *<br><br>
 * @param surf
 * the bs3_surface.
 */
    spline(
        bs3_surface surf
    );


    // Make a copy of this spline on the heap, and return a
    // pointer to it (again minimal copying employed).
/**
 * Makes a copy of this spline on the heap, and returns a pointer to it.
 */
    virtual surface* make_copy() const;

    // Make a copy without any sharing of subdata.
/**
 * Creates a copy of an item that does not share any data with the original.
 *<br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 *<br><br>
 * @param pm
 * List of items within the entity that are already deep copied.
 */
    virtual surface* deep_copy(pointer_map* pm = NULL) const;

    // Remove the use of the underlying surface (decrement use-count).
/**
 * C++ destructor, deleting a spline.
 */
    virtual ~spline();


    // Read information from the surface definition
/**
 * Returns <tt>TRUE</tt> if the <tt>spline</tt> is reversed with respect to the underlying <tt>spl_sur</tt>.
 */
    logical reversed() const
    {
        return rev;
    }

    // Returns the underlying spline surface, evaluating it if it is not present
    // by calling make_approx.  Note - it may be impossible to calculate the
    // approximating surface, in which case this function will return NULL.

    // The default value of -1.0 for the fit tolerance instructs the make_approx
    // function to use whatever value it thinks is appropriate.
/**
 * Returns (a pointer to) the underlying surface, or <tt>NULL</tt> if none.
 *<br><br>
 * @param tol
 * tolerance.
 */
    bs3_surface sur(double tol = -1.0) const;

    // Report whether the approximating surface is present.
/**
 * Returns <tt>TRUE</tt> if there is underlying surface data.
 */
    logical	sur_present() const;


    // STI dgh: replace underlying bs3_surface approximation.  If the supplied
    // fitol is negative, then it will be left unchanged (PC).
/**
 * Sets the surface information.
 *<br><br>
 * @param surf
 * surface data.
 * @param fitol
 * fit tolerance.
 */
    void set_sur(bs3_surface surf, double fitol = -1.0);

    /**
     * Returns the fit tolerance of the <tt>bs3_curve</tt> to the true spline surface.
     */
    double fitol() const;			// fit tolerance
    /**
     * Returns defining spline surface and should only be used when absolutely necessary.
     */
    spl_sur const& get_spl_sur() const
    {
        return *spl;
    }			// shouldn't be used unless
                         // absolutely necessary.

/**
 * Replaces the defining spline surface and should only be used with extreme caution.
 */
    void replace_spl_sur(spl_sur& new_spl);

    // Function to determine if this spline depends on a pipe surface at all.
/**
 * Returns <tt>TRUE</tt> if this spline depends on a pipe surface.
 */
    logical contains_pipe() const;


    // Special operation for splines - perform a linear
    // transformation on the parametrisation, so that it starts
    // and ends at the given values (which must be in increasing
    // order).
/**
 * Reparameterizes the surface in <i>u</i>.
 *<br><br>
 * @param start
 * new start u parameter.
 * @param end
 * new end u parameter.
 */
    void reparam_u(
        double start,	// new start u parameter
        double end		// new end u parameter
    );
    /**
     * Reparameterizes the surface in <i>v</i>.
     *<br><br>
     * @param start
     * new start v parameter.
     * @param end
     * new end v parameter.
     */
    void reparam_v(
        double start,	// new start v parameter
        double end		// new end v parameter
    );
    /**
     * Reparameterizes the surface.
     *<br><br>
     * @param startu
     * new start u parameter.
     * @param endu
     * new end u parameter.
     * @param startv
     * new start v parameter.
     * @param endv
     * new end v parameter.
     */
    void reparam(
        double startu,		// new start u parameter
        double endu,		// new end u parameter
        double startv,		// new start v parameter
        double endv			// new end v parameter
    );


    // Returns a surface with a reversed sense
/**
 * Returns a surface with a reversed sense.
 */
    spline operator-() const;


    // Assignment.  Merely copy the spline record, and adjust the
    // use counts of the underlying information to suit.
/**
 * Copies the spline record, and adjust the use counts of the underlying information to suit.
 *<br><br>
 * @param name
 * spline name.
 */
    spline& operator=(
        spline const& name
        );


    // Test two surfaces for equality. This, like testing floating-
    // point numbers for equality, is not guaranteed to say "equal" for
    // effectively-equal surfaces, but is guaranteed to say "not equal"
    // if they are indeed not equal. The result can be used for
    // optimisation, but not where it really matters. The default
    // always says "not equal", for safety.
/**
 * The default always says not equal, for safety.
 *<br><br>
 * <b>Role:</b> This, like testing floating point numbers for equality, is not
 * guaranteed to say equal for effectively equal surfaces, but is guaranteed to
 * say not equal if they are indeed not equal. The result can be used for optimization,
 * but not where it really matters.
 *<br><br>
 * @param surf
 * surface to be compared.
 */
    virtual logical operator==(surface const& surf) const;

    // Determine whether a surface is entirely enclosed within
    // another.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual logical operator>>(surface const&) const;


    // Transform a spline surface.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN spline operator*(
        spline const&,
        SPAtransf const&
        );


    // Transform this spline by the given SPAtransf.
/**
 * Transforms this spline by the given transformation.
 *<br><br>
 * @param trans
 * transformation.
 */
    virtual surface& operator*=(
        SPAtransf const& trans
        );


    // Negate this spline.
/**
 * Negates this spline.
 */
    virtual surface& negate();


    // Classification of a spline. The only thing we can say about it
    // for now is whether it is properly defined or not.
/**
 * Indicates if the spline is improperly defined.
 */
    virtual logical undef() const;


    // Returns a box around the surface. This need not be the smallest
    // box which contains the specified portion of the surface, but
    // needs to balance the tightness of the bound against the cost of
    // evaluation.

    // Surface bounded in parameter space.
/**
 * Returns a box that encloses the portion of the surface within the given range after transformation.
 *<br><br>
 * @param box
 * parameter range.
 * @param trans
 * transformation.
 */
    virtual SPAbox bound(
        SPApar_box const& box = *(SPApar_box*)NULL_REF,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;

    // Surface bounded in object space.
/**
 * Returns a box that encloses the portion of the surface that lies within the given box after transformation.
 *<br><br>
 * @param box
 * object space box.
 * @param trans
 * transformation.
 */
    virtual SPAbox bound(
        SPAbox const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;


    // Returns a SPAbox around the spline. This is retained for historical
    // reasons - it exactly parallels the first surface virtual
    // function.
/**
 * Returns a box around the spline.
 *<br><br>
 * <b>Role:</b> This is retained for historical reasons-it exactly parallels the
 * previous virtual function.
 *<br><br>
 * @param trans
 * transform.
 * @param parbox
 * parameter range.
 */
    SPAbox bound(
        SPAtransf const& trans,
        SPApar_box const& parbox = *(SPApar_box*)NULL_REF
    ) const;


    // Returns a cone bounding the normal direction of the surface. The
    // cone is deemed to have its apex at the origin, and has a given
    // axis direction and (positive) half-angle. If the logical
    // argument is TRUE, then a quick approximation may be found. The
    // approximate result may lie wholly within or wholly outside the
    // guaranteed bound (obtained with a FALSE argument), but may not
    // cross from inside to outside. Flags in the returned object
    // indicate whether the cone is in fact the best available, and
    // if not whether this result is inside or outside the best cone.
/**
 * Returns a cone bounding the normal direction of the surface.
 *<br><br>
 * <b>Role:</b> The cone is deemed to have its apex at the origin, and has a given
 * axis direction and (positive) half-angle. If the logical argument is <tt>TRUE</tt>, then
 * a quick approximation may be found. The approximate result may lie wholly within
 * or wholly outside the guaranteed bound (obtained with a <tt>FALSE</tt> argument), but may
 * not cross from inside to outside. Flags in the returned object indicate whether
 * the cone is in fact the best available, and if not whether this result is inside
 * or outside the best cone.
 *<br><br>
 * @param parbox
 * parameter bounds.
 * @param approx
 * approx.
 * @param trans
 * transformation.
 */
    virtual surf_normcone normal_cone(
        SPApar_box const& parbox,	// parameter bounds
        logical approx = FALSE,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;


    // Construct a new spline which is a copy of the part of the
    // given one within given parameter bounds. This is syntactic
    // sugar cladding multiple calls to split.
/**
 * Constructs a new spline that is a copy of the part of the given one within given parameter bounds.
 *<br><br>
 * @param parbox
 * parameter range.
 */
    spline* subset(
        SPApar_box const& parbox
    ) const;


    // Divides a surface into two pieces at a given parameter value.
    // Returns a new surface for the low-parameter side, and change the
    // old one to represent the high-parameter side.
/**
 * Divides a surface into two pieces at a <i>u</i>-parameter value.
 *<br><br>
 * <b>Role:</b> Returns a new surface for the low-parameter side, and change the
 * old one to represent the high-parameter side.
 *<br><br>
 * @param parameter
 * given u parameter.
 */
    spline* split_u(double parameter);
    /**
     * Divides a surface into two pieces at a <i>v</i>-parameter value.
     *<br><br>
     * <b>Role:</b> Returns a new surface for the low-parameter side, and change the
     * old one to represent the high-parameter side.
     *<br><br>
     * @param parameter
     * given v parameter.
     */
    spline* split_v(double parameter);


    // Divides a surface into separate pieces which are smooth (and therefore
    // suitable for offsetting or blending). The surface is split at its
    // non-G1 discontinuities, and if it is closed after this, it is then
    // split into two. The functions return the number of pieces, and the
    // pieces themselves are a return argument.
/**
 * Divides a surface into separate pieces which are smooth (and therefore suitable for offsetting or blending).
 *<br><br>
 * <b>Role:</b> The surface is split at its non-<i>G1</i> discontinuities, and if it is
 * closed after this, it is then split into two. The split pieces are stored in
 * the the pieces argument. The function returns the count of split pieces.
 *<br><br>
 * @param pieces
 * pieces.
 * @param curvature
 * curvature.
 */
    int split_at_kinks_u(spline**& pieces, double curvature = 0.0) const;
    /**
     * Divides a surface into separate pieces which are smooth (and therefore suitable for offsetting or blending).
     * <br><br>
     * <b>Role:</b> The surface is split at its non<i>-G1</i> discontinuities, and if it is
     * closed after this, it is then split into two. The split pieces are stored in
     * the the pieces argument. The function returns the count of split pieces.
     * <br><br>
     * @param pieces
     * pieces.
     * @param curvature
     * curvature.
     */
    int split_at_kinks_v(spline**& pieces, double curvature = 0.0) const;

    /**
     * Indicates if the surface is parametric.
     * <br><br>
     * <b>Role:</b> Always <tt>TRUE</tt> for splines.
     */
    virtual logical parametric() const;	// returns true for spline


    // Find the normal to the surface at the given point. Returns an
    // exact zero if the point is a singularity of the surface where
    // there is no well-defined normal.
/**
 * Finds the normal to the surface at the given point.
 * <br><br>
 * @param pos
 * position.
 * @param guess
 * parameter guess.
 */
    virtual SPAunit_vector point_normal(
        SPAposition const& pos,
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find an outward direction from the surface at a point on
    // the surface nearest to the given point. Normally just the
    // normal, but non-zero at a singularity.
/**
 * Finds an outward direction from the surface at a point on the surface nearest to the given point.
 * <br><br>
 * <b>Role:</b> Normally just the normal, but nonzero at a singularity.
 * <br><br>
 * @param pos
 * position.
 * @param guess
 * parameter guess.
 */
    virtual SPAunit_vector point_outdir(
        SPAposition const& pos,
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the principal axes of curvature of the surface at a
    // given point, and the curvatures in those directions.
/**
 * Finds the principal axes of curvature of the surface at a given point, and the curvatures in those directions.
 * <br><br>
 * @param pos
 * position.
 * @param axis1
 * first axis direction.
 * @param cur1
 * curvature in first direction.
 * @param axis2
 * second axis direction.
 * @param cur2
 * curvature in second direction.
 * @param guess
 * parameter guess.
 */
    virtual void point_prin_curv(
        SPAposition const& pos,
        SPAunit_vector& axis1,		// first axis direction
        double& cur1,			// curvature in first direction
        SPAunit_vector& axis2,		// second axis direction
        double& cur2,			// curvature in second direction
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Finds the principal curvatures at a given point, returning the values in a struct.
     * <br><br>
     * <b>Role:</b> Just uses the other (virtual) principal curvature function.
     * <br><br>
     * @param pos
     * position.
     * <br><br>
     * @param param_guess
     * parameter position - possible parameter.
     */
    surf_princurv point_prin_curv(
        SPAposition const& pos,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF
    ) const
    {
        return surface::point_prin_curv(pos, param_guess);
    }


    // Find the curvature of a cross-section curve of the surface at
    // the point on the surface closest to the given point, iterating
    // from the given parameter values (if supplied).
    // The cross-section curve is determined by the intersection of
    // the surface with a plane passing through the point on the
    // surface and with given normal.
/**
 * Finds the curvature of a cross-section curve of the surface at the point on the surface closest to the given point, iterating from the given parameter values (if supplied).
 * <br><br>
 * <b>Role:</b> The cross-section curve is determined by the intersection of the
 * surface with a plane passing through the point on the surface and with given normal.
 * <br><br>
 * @param pos
 * position.
 * @param direction
 * direction.
 * @param guess
 * initial param guess.
 */
    virtual double point_cross(
        SPAposition const& pos,
        SPAunit_vector const& direction,
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF
    ) const;

    /**
     * Finds the curvature of a cross-section curve of the surface at the point on the surface closest to the given point, iterating from the given parameter values (if supplied).
     * <br><br>
     * <b>Role:</b> The cross-section curve is determined by the intersection of the
     * surface with a plane passing through the point on the surface and with given normal.
     * <br><br>
     * @param pos
     * position.
     * @param direction
     * direction.
     * @param guess
     * initial param guess.
     * @param use_sided_eval
     * use sided evaluation for derivatives.
     * @param right_side
     * evaluate right side of direction.
     */

    virtual double point_cross(
        SPAposition const& pos,
        SPAunit_vector const& direction,
        SPApar_pos const& guess,
        logical use_sided_eval,
        logical right_side
    ) const;

    // Find the point on the surface nearest to the given point and
    // optionally the normal to and principal curvatures of the
    // surface at that point.  If the surface is parametric, also
    // return the parameter values at the found point.
/**
 * Finds the point on the surface nearest to the given point and the normal to and principal curvatures of the surface at that point.
 * <br><br>
 * <b>Role:</b> If the surface is parametric, also return the parameter values at the found point.
 * <br><br>
 * @param pos
 * given position.
 * @param surf
 * position on a surface.
 * @param normal
 * surface normal.
 * @param curv
 * principal curvature.
 * @param guess
 * parameter guess.
 * @param actual
 * actual parameter.
 * @param f_weak
 * weak flag.
 */
    virtual void point_perp(
        SPAposition const& pos,
        SPAposition& surf,
        SPAunit_vector& normal,
        surf_princurv& curv,
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const;
    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> If the surface is parametric, also return the parameter values at
     * the found point.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * point on a surface.
     * @param norm
     * surface normal.
     * @param param_guess
     * parameter position - parameter guess.
     * @param param_actual
     * actual parameter.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPAunit_vector& norm,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            norm,
            *(surf_princurv*)NULL_REF,
            param_guess,
            param_actual, f_weak
        );
    }
    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> If the surface is parametric, also return the parameter values at
     * the found point.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * point on a surface.
     * @param param_guess
     * parameter position - parameter guess.
     * @param param_actual
     * actual parameter.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF,
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


    // Find the parameter values of a point on a 3D B-spline surface,
    // iterating from the given parameter values (if supplied).
/**
 * Finds the parameter values of a point on a <tt>3D</tt> <i>B</i>-spline surface, iterating from the given parameter values (if supplied).
 * <br><br>
 * @param pos
 * given position.
 * @param guess
 * initial guess.
 */
    virtual SPApar_pos param(
        SPAposition const& pos,
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the change in surface parameter corresponding to a unit
    // offset in a given direction at a given SPAposition.
/**
 * Finds the change in surface parameter corresponding to a unit offset in a given direction at a given position.
 * <br><br>
 * @param direction
 * direction.
 * @param parameter
 * parameter.
 */
    virtual SPApar_vec param_unitvec(
        SPAunit_vector const& direction,
        SPApar_pos const& parameter
    ) const;
    /**
     * Finds the change in surface parameter corresponding to a unit offset in a given direction at a given position.
     * <br><br>
     * @param direction
     * direction.
     * @param parameter
     * parameter.
     * logical well_conditioned which would be equal to FALSE if the matrix is ill/bad conditioned.
     */
    virtual SPApar_vec param_unitvec(
        const SPAunit_vector& dir,
        const SPApar_pos& pos, logical& well_conditioned
    ) const;
    // Find the SPAposition and first and second derivatives of the
    // surface at a given point. This will be a surface virtual
    // function (defined only for parametric surfaces), but isn't
    // declared as such yet here until it gets into surdef.
/**
 * Finds the position and first and second derivatives of the surface at given parameter values.
 * <br><br>
 * @param uv
 * given parameter-space location.
 * @param pos
 * position.
 * @param dpos
 * 1st derivatives array length 2 in order xu, xv.
 * @param ddpos
 * second derivatives - array of length 3 in order xuu, xuv, xvv.
 */
    virtual void eval(
        SPApar_pos const& uv,
        SPAposition& pos,
        SPAvector* dpos = NULL,// first derivatives - array of
        // length 2 in order xu, xv
        SPAvector* ddpos = NULL // second derivatives - array of
        // length 3 in order xuu, xuv, xvv
    ) const;


    // Find the point on the spline with given parameter values.
/**
 * Finds the point on the spline with given parameter values.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 */
    virtual SPAposition eval_position(
        SPApar_pos const& parameter
    ) const;


    // Find the normal to the spline at the point with given
    // parameter values.
/**
 * Finds the normal to the spline at the point with given parameter values.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 */
    virtual SPAunit_vector eval_normal(
        SPApar_pos const& parameter
    ) const;


    // Find an outward direction from the surface at a point with
    // given parameter values.
/**
 * Finds an outward direction from the surface at a point with given parameter values.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 */
    virtual SPAunit_vector eval_outdir(
        SPApar_pos const& parameter
    ) const;


    // Find the principal axes of curvature of the surface at a
    // point with given parameter values, and the curvatures in those
    // directions.
/**
 * Finds the principal axes of curvature of the surface at a point with given parameter values, and the curvatures in those directions.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 * @param axis1
 * first axis direction.
 * @param cur1
 * 1st direction - curvature direction.
 * @param axis2
 * second axis direction.
 * @param cur2
 * second direction curvature.
 */
    virtual void eval_prin_curv(
        SPApar_pos const& parameter,
        SPAunit_vector& axis1,		// first axis direction
        double& cur1,			// curvature in first direction
        SPAunit_vector& axis2,		// second axis direction
        double& cur2			// curvature in second direction
    ) const;
    /**
     * Finds the principal axes of curvature of the surface at a given parameter-space location.
     * <br><br>
     * @param param
     * given parameter-space location.
     */
    surf_princurv eval_prin_curv(
        SPApar_pos const& param
    ) const
    {
        return surface::eval_prin_curv(param);
    }


    // Find the curvature of a cross-section curve of the surface at
    // the point on the surface with given parameter values.
    // The cross-section curve is determined by the intersection of
    // the surface with a plane passing through the point on the
    // surface and with given normal.
/**
 * Finds the curvature of a cross-section curve of the surface at the point on the surface with given parameter values.
 * <br><br>
 * <b>Role:</b> The cross-section curve is determined by the intersection of the
 * surface with a plane passing through the point on the surface and with given normal.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 * @param normal
 * curve normal.
 */
    virtual double eval_cross(
        SPApar_pos const& parameter,
        SPAunit_vector const& normal
    ) const;


    // The evaluate() function calculates derivatives, of any order
    // up to the number requested, and stores them in vectors provided
    // by the user. It returns the number it was able to calculate;
    // this will be equal to the number requested in all but the most
    // exceptional circumstances. A certain number will be evaluated
    // directly and (more or less) accurately; higher derivatives will
    // be automatically calculated by finite differencing; the accuracy
    // of these decreases with the order of the derivative, as the
    // cost increases. The default simply calls evaluate_iter() with a
    // NULL surface_evaldata argument.
/**
 * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
 * <br><br>
 * <b>Role:</b> Any of the pointers may be <tt>NULL</tt>, in which case the corresponding
 * derivatives will not be returned. Otherwise they must point to arrays long
 * enough for all the derivatives of that order; i.e., <tt>2</tt> for the first derivatives,
 * <tt>3</tt> for the second, etc.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 * @param pos
 * pt on surface at given parameter.
 * @param vec
 * array of ptrs to arrays of vectors.
 * @param deriv
 * Number of derivatives required (nd).
 * @param eval
 * the evaluation loc. above, below for each parameter direction, or don't care.
 */
    virtual int evaluate(
        SPApar_pos const& parameter,	// Parameter
        SPAposition& pos,			// Point on surface at given parameter
        SPAvector** vec = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int deriv = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant eval = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each parameter direction,
        // or don't care.
    ) const;


    // The evaluate_iter() function is just like evaluate(), but is
    // supplied with a data object which contains results from a previous
    // close evaluation, for use as initial values for any iteration
    // involved.
/**
 * The <tt>evaluate_iter</tt> function is just like evaluate, but is supplied with a data object which contains results from a previous close evaluation.
 * <br><br>
 * <b>Role:</b> For use as initial values for any iteration involved.
 * <br><br>
 * @param parameter
 * given parameter-space location.
 * @param data
 * data supplying initial values and set to reflect the results of this evaluation.
 * @param pos
 * Point on surface at given parameter.
 * @param vec
 * Array of pointers to arrays of vectors, of size nd. Any of the pointers may be null, in which case the corresponding derivatives will not be returned.
 * @param deriv
 * Number of derivatives required (nd).
 * @param eval
 * the evaluation location - above, below or don't care.
 */
    virtual int evaluate_iter(
        SPApar_pos const& parameter,	// Parameter
        surface_evaldata* data,	// Data supplying initial values, and
        // set to reflect the results of this
        // evaluation.
        SPAposition& pos,			// Point on surface at given parameter
        SPAvector** vec = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int deriv = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant eval = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each parameter direction,
        // or don't care.
    ) const;

    // Construct a data object to retain evaluation information across
    // calls to evaluate_iter(). This is to allow subsidiary calls
    // within an iterative evaluator to start iteration much closer to the
    // required result than is possible just using the curve information
    // itself.
/**
 * Constructs a data object to retain evaluation information across calls to <tt>evaluate_iter</tt>.
 * <br><br>
 * <b>Role:</b> This is to allow subsidiary calls within an iterative evaluator to
 * start iteration much closer to the required result than is possible just using
 * the curve information itself.
 */
    virtual surface_evaldata* make_evaldata() const;

    // Search the underlying spl_sur cache for an entry at the given SPAposition.
    // Returns the matching eval entry if this is found, or NULL otherwise.
/**
 * Searches the underlying cache for an entry at the given position.
 * <br><br>
 * <b>Role:</b> Returns the matching eval entry if this is found, or <tt>NULL</tt> otherwise.
* <br><br>
 * @param pos
 * position to evaluate.
 */
    const eval_sscache_entry* search_eval_cache(const SPAposition& pos) const;

    // Returns the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.
/**
 * Returns the number of derivatives that evaluate can find accurately and fairly directly.
 * <br><br>
 * <b>Role:</b> Returns the number of derivatives that evaluate can find accurately
 * and fairly directly, rather than by finite differencing, over the given portion
 * of the surface. If there is no limit to the number of accurate derivatives,
 * returns the value <tt>ALL_SURFACE_DERIVATIVES</tt>.
 * <br><br>
 * @param def
 * default to the surface.
 */
    virtual int accurate_derivs(
        SPApar_box const& def = *(SPApar_box*)NULL_REF
        // Defaults to the whole surface
    ) const;


    // STI aed: add planar method
        // Report whether surface is planar
    /**
     * Determines whether spline is planar.
     * <br><br>
     * @param pos
     * location.
     * @param uv
     * unit vector.
     */
    virtual logical planar(
        SPAposition& pos,
        SPAunit_vector& uv
    ) const;
    // STI aed: end

        // Report whether a parametric surface is periodic in either
        // parameter direction (i.e. it is smoothly closed, so faces can
        // run over the seam).
    /**
     * Reports whether a parametric surface is periodic in the <i>u</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> a parametric surface is periodic if it is smoothly closed, so faces
     * can run over the seam.
     */
    virtual logical periodic_u() const;
    /**
     * Reports whether a parametric surface is periodic in the <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> a parametric surface is periodic if it is smoothly closed, so faces
     * can run over the seam.
     */
    virtual logical periodic_v() const;


    // Report whether the surface is closed, smoothly or not, in
    // either parameter direction.
/**
 * Reports whether the surface is closed, smoothly or not, in the <i>u</i>-parameter direction.
 */
    virtual logical closed_u() const;
    /**
     * Reports whether the surface is closed, smoothly or not, in the <i>v</i>-parameter direction.
     */
    virtual logical closed_v() const;


    // Returns the period of a periodic parametric surface, zero if
    // the surface is not periodic in the chosen parameter or not
    // parametric.
/**
 * Returns the period of a periodic parametric surface.
 * <br><br>
 * <b>Role:</b> <tt>0</tt> if the surface is not periodic in the <i>u</i>-parameter or not parametric.
 */
    virtual double param_period_u() const;
    /**
     * Returns the period of a periodic parametric surface.
     * <br><br>
     * <b>Role:</b> <tt>0</tt> if the surface is not periodic in the <i>v</i>-parameter or not parametric.
     */
    virtual double param_period_v() const;


    // Returns the principal parameter range of a parametric surface in
    // a chosen parameter direction, or in both. For a non-parametric
    // surface, the range is returned as the empty SPAinterval or SPAbox.

    // A periodic surface is defined for all parameter values in the
    // periodic direction, by reducing the given parameter modulo the
    // period into this principal range. For a surface open or non-
    // periodic in the chosen direction the surface evaluation
    // functions are defined only for parameter values in the
    // returned range.

    // If a SPAbox is provided, the parameter range returned may be
    // restricted to a portion of the surface which is guaranteed to
    // contain all portions of the surface which lie within the region
    // of interest. If none is provided, and the parameter range in
    // some direction is unbounded, then conventionally an empty
    // SPAinterval is returned.
/**
 * Returns the principal parameter range of a parametric surface in a chosen parameter direction.
 * <br><br>
 * <b>Role:</b> A periodic surface is defined for all parameter values in the
 * periodic direction, by reducing the given parameter modulo the period into this
 * principal range. For a surface open or nonperiodic in the chosen direction the
 * surface evaluation functions are defined only for parameter values in the
 * returned range. If a box is provided, the parameter range returned may be
 * restricted to a portion of the surface that is guaranteed to contain all portions
 * of the surface that lie within the region of interest. If none is provided, and
 * the parameter range in some direction is unbounded, then conventionally an empty
 * interval is returned.
 * <br><br>
 * @param region
 * region of interest.
 */
    virtual SPApar_box param_range(
        SPAbox const& region = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a parametric surface in a chosen parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction, by reducing the given parameter modulo the period into this
     * principal range. For a surface open or nonperiodic in the chosen direction the
     * surface evaluation functions are defined only for parameter values in the
     * returned range. If a box is provided, the parameter range returned may be
     * restricted to a portion of the surface that is guaranteed to contain all portions
     * of the surface that lie within the region of interest. If none is provided, and
     * the parameter range in some direction is unbounded, then conventionally an empty
     * interval is returned.
     * <br><br>
     * @param region
     * region of interest.
     */
    virtual SPAinterval param_range_u(
        SPAbox const& region = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a parametric surface in a chosen parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction, by reducing the given parameter modulo the period into this
     * principal range. For a surface open or nonperiodic in the chosen direction the
     * surface evaluation functions are defined only for parameter values in the
     * returned range. If a box is provided, the parameter range returned may be
     * restricted to a portion of the surface that is guaranteed to contain all portions
     * of the surface that lie within the region of interest. If none is provided, and
     * the parameter range in some direction is unbounded, then conventionally an empty
     * interval is returned.
     * <br><br>
     * @param region
     * region of interest.
     */
    virtual SPAinterval param_range_v(
        SPAbox const& region = *(SPAbox*)NULL_REF
    ) const;


    // Report whether the surface parametrisation is singular at
    // the specified u or v parameter value. The only singularity
    // recognised is where every value of the non-constant parameter
    // generates the same object-space point, and these can only
    // occur at the ends of the parameter range as returned by
    // the functions above. A plane is nonsingular in both directions.
/**
 * Reports whether the surface parameterization is singular at the specified <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> The only singularity recognized is where every value of the
 * nonconstant parameter generates the same object-space point, and these can
 * only occur at the ends of the parameter range as returned by <tt>param_range_u</tt>.
 * <br><br>
 * @param uparam
 * constant u parameter.
 */
    virtual logical singular_u(
        double	uparam	// constant u parameter
    ) const;
    /**
     * Reports whether the surface parameterization is singular at the specified <i>v</i>-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the nonconstant
     * parameter generates the same object-space point, and these can only occur at the
     * ends of the parameter range as returned by <tt>param_range_v</tt>.
     * <br><br>
     * @param vparam
     * constant v parameter.
     */
    virtual logical singular_v(
        double	vparam	// constant v parameter
    ) const;


    // Indicate whether the parameter coordinate system of the surface
    // is right- or left-handed. With a right-handed system, at any
    // point the outward normal is given by the cross product of the
    // increasing u direction with the increasing v direction, in that
    // order. With a left-handed system the outward normal is in the
    // opposite direction from this cross product.
/**
 * Indicates whether the parameter coordinate system of the surface is right or left-handed.
 * <br><br>
 * <b>Role:</b> With a right-handed system, at any point the outward normal is
 * given by the cross product of the increasing <i>u</i>-direction with the increasing
 * <i>v</i>-direction, in that order. With a left-handed system the outward normal is in
 * the opposite direction from this cross product.
 */
    virtual logical left_handed_uv() const;


    // Construct a parameter line on the surface. A u parameter line
    // runs in the direction of increasing u parameter, at constant v.
    // A v parameter line runs in the direction of increasing v, at
    // constant u. The parametrisation in the non-constant direction
    // matches that of the surface, and has the range obtained by
    // use of param_range_u() or param_range_v() appropriately.

    // The new curve is constructed in free store, so it is the
    // responsibility of the caller to ensure that it is correctly
    // deleted.
/**
 * Constructs a <i>u</i>-parameter line on the surface.
 * <br><br>
 * <b>Role:</b> A <i>u</i>-parameter line runs in the direction of increasing <i>u</i>-parameter,
 * at constant <i>v</i>. The parameterization in the nonconstant direction matches that
 * of the surface, and has the range obtained by use of <tt>param_range_u</tt>. The new curve
 * is constructed in free store, so it is the responsibility of the caller to ensure
 * that it is correctly deleted.
 * <br><br>
 * @param uparam
 * <i>u</i>-parameter.
 */
    virtual curve* u_param_line(
        double	uparam		// constant v parameter
    ) const;
    /**
     * Constructs a <i>v</i>-parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>v</i>-parameter line runs in the direction of increasing <i>v</i>, at
     * constant <i>u</i>. The parameterization in the nonconstant direction matches that of
     * the surface, and has the range obtained by use of <tt>param_range_v</tt>. The new curve
     * is constructed in free store, so it is the responsibility of the caller to
     * ensure that it is correctly deleted.
     * <br><br>
     * @param vparam
     * <i>v</i>-parameter.
     */
    virtual curve* v_param_line(
        double	vparam		// constant u parameter
    ) const;


    // Test whether a point lies on the surface, to user-specified
    // tolerance.
/**
 * Tests whether a point lies on the surface to a user-specified tolerance.
 * <br><br>
 * @param pos
 * position.
 * @param tol
 * specified tolerance.
 * @param guess
 * parameter guess.
 * @param actual
 * actual parameter.
 */
    virtual logical test_point_tol(
        SPAposition const& pos,
        double tol = 0,		// defaults to SPAresabs
        SPApar_pos const& guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& actual = *(SPApar_pos*)NULL_REF
    ) const;


    // Returns (in a read-only array) the number and parameter values of
    // discontinuities of the surface, of the given order (maximum three).
/**
 * Returns the number and parameter values of discontinuities of the surface of the given order (maximum three) in a read-only array.
 * <br><br>
 * @param n_discont
 * number of discontinuities in the returned array.
 * @param order
 * given order.
 */
    virtual const double* discontinuities_u(int& n_discont, int order) const;
    /**
     * Returns the number and parameter values of discontinuities of the surface of the given order (maximum three) in a read-only array.
     * <br><br>
     * @param n_discont
     * number of discontinuities in the returned array.
     * @param order
     * given order.
     */
    virtual	const double* discontinuities_v(int& n_discont, int order) const;


    // Returns (in a read-only array) the number and parameter values of
    // discontinuities of the surface, up to the given order (maximum three).
/**
 * Returns in a read-only array the number and parameter values of discontinuities of the surface, up to the given order (maximum three).
 * <br><br>
 * @param n_discont
 * number of discontinuities in the returned array.
 * @param order
 * given order.
 */
    virtual const double* all_discontinuities_u(int& n_discont, int order);
    /**
     * Returns in a read-only array the number and parameter values of discontinuities of the surface, up to the given order (maximum three).
     * <br><br>
     * @param n_discont
     * number of discontinuities in the returned array.
     * @param order
     * given order.
     */
    virtual	const double* all_discontinuities_v(int& n_discont, int order);


    // State whether a particular parameter value is a discontinuity, and if so,
    // return the order of the discontinuity.
/**
 * Returns whether a particular parameter value is a discontinuity.
 * <br><br>
 * @param u
 * u parameter value.
 */
    virtual		int		discontinuous_at_u(double u) const;
    /**
     * Returns whether a particular parameter value is a discontinuity.
     * <br><br>
     * @param v
     * v parameter value.
     */
    virtual		int		discontinuous_at_v(double v) const;


    // Returns read-only access to the disc_info objects.
/**
 * Returns read-only access to the <tt>disc_info</tt> objects.
 */
    virtual 	const	discontinuity_info& get_disc_info_u()	 	const;
    /**
     * Returns read-only access to the <tt>disc_info</tt> objects.
     */
    virtual 	const	discontinuity_info& get_disc_info_v()	 	const;


    // Returns type of (lower-case) surface.
/**
 * Returns the type of spline.
 */
    virtual int type() const;

    // Returns string identifier of surface.
/**
 * Returns string <tt>"spline_xxx"</tt> where <tt>xxx</tt> is replaced with <tt>type_names</tt> of the underlying <tt>spl_sur</tt>.
 */
    virtual char const* type_name() const;


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
/**
 * Saves the type or id, then calls <tt>save_data</tt>.
 */
    virtual void save() const;
    /**
     * Saves the information for the spline in the save file.
     */
    void save_data() const;
    /**
     * @nodoc
     */
    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend DECL_KERN surface* restore_spline();
    /**
     * Restore the data for a spline from a save file.
     * <br><br>
     * <b>Role:</b> <pre>
     * if (restore_version_number &lt; SPLINE_VERSION )		 		// Just restore as an exact spline.
     * 	(spl_sur *)dispatch_restore_subtype( "sur", "exactsur" )
     * else
     * 	read_logical	 						//Reverse flag; either "forward" or "reversed".
     * 									//Switch to the right restore routine, using the standard system mechanism.
     * 									//Note that the argument is to enable the reader to distinguish old-style
     * 									//types where "exact" was both an int_cur and a spl_sur.
     * 									//They are now "exactcur" and "exactsur".
     * 	(spl_sur *)dispatch_restore_subtype( "sur" )
     * surface::restore_data			//Fix the underlying surface
     * </pre>
     */
    void restore_data();

    // Notify the derived type that the surface has been changed (e.g. the
    // subset_range has changed) so that it can update itself.
/**
 * Notifies the derived type that the surface has been changed (like, the <tt>subset_range</tt> has changed), so that it can update itself.
 */
    virtual	void	change_event();


    // Check for any data errors in the surface, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // surface on exit.
    // See chk_stat.hxx for information on the argument types used here.
/**
 * Check for any data errors in the spline, and correct the errors if possible.
 * <br><br>
 * <b>Role:</b> The various arguments provide control over which checks are made,
 * which fixes can be applied and which fixes were actually applied. The function
 * returns a list of errors that remain in the curve on exit.
 * <br><br>
 * The default for the set of flags which say which fixes are allowable is none
 * (nothing is fixed). If the list of checks to be made is null, then every
 * possible check will be made. Otherwise, the function will only check for things
 * in the list. Note that it is possible for errors not in the input list to be
 * returned, as certain problems preclude other checks being performed. In such
 * situations, the return list will contain errors which were not requested, but
 * which have prevented requested checks from being made.
 * <br><br>
 * @param input
 * flags for allowed fixes.
 * @param result
 * fixes applied.
 * @param status_list
 * checks to be made, default is none.
 */
    virtual	check_status_list* check(
        const check_fix& input = *(const check_fix*)NULL_REF,
        // supplies a set of flags which say which fixes
        // are allowable (the default is to fix nothing)
        check_fix& result = *(check_fix*)NULL_REF,
        // returns a set of flags which say which fixes
        // were applied
        const check_status_list* status_list = (const check_status_list*)NULL_REF
        // list of checks that are to be made.  If the
        // list is null, then every possible check will
        // be made; otherwise, the function will only
        // check for things in the list.  The return
        // value for the function will then be a subset
        // of this list.
    );


    // Stream i/o.

#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void input(
        D3_istream&
    );
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void print(
        D3_ostream&
    ) const;

#endif

    // Print out details of spline.
/**
 * Prints out details of a spline.
 * <br><br>
 * @param title
 * title line.
 * @param fp
 * file.
 */
    virtual void debug(
        char const* title,
        FILE* fp = debug_file_ptr
    ) const;

    // STI swa 27Jul98 -- functions for getting sweep information on a spline surface
public:
    /**
     * Returns the sweep path type for this spline.
     */
    virtual sweep_path_type get_path_type() const;
    /**
     * Returns the sweep path curve for this spline. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const;
    /**
     * Returns the sweep profile curve for this spline.  The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     * <br><br>
     * @param param
     * parameter.
     */
    virtual curve* get_profile(double param/*param*/) const;
    /**
     * Returns the sweep rail law for this spline.
     */
    virtual law* get_rail() const;
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
    virtual void minimize(minimize_helper*);

public:
    // set/unset the flag for the sanity check.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void set_checked();
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void unset_checked();

protected:
    // GSSL/RS - start : Functions for param_period ignoring subset range
    virtual double param_period_u_ignoring_limits() const;
    virtual double param_period_v_ignoring_limits() const;
    // GSSL/RS - end
    // GSSL/RA - start : Function to validate the v guess so that it lies within
    // param range of the surface

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_u_guess(const SPAparameter& u_guess,
        SPAparameter& valid_u_guess) const;
    // GSSL/RA - end

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_v_guess(const SPAparameter& v_guess,
        SPAparameter& valid_v_guess) const;

public:
    // get and set functions for access.
    spl_sur* gme_get_spl();
    void gme_set_rev(int data);
    spl_sur** gme_get_spl_ptr();
};


// The class containing 'summary data' enabling the approximating surface to be
// stored concisely, and regenerated quickly.
/*
// tbrv
*/
/**
 * @nodoc
 */
class summary_bs3_surface : public ACIS_OBJECT {
    friend class spl_sur;
    friend class GSM_progen_spl_sur;
    friend DECL_KERN void GSM_progen_spl_sur_make_approx(GSM_progen_spl_sur*, double, copy_knots_mode, const spline&);

    int nuknots;
    double* uknots;
    int nvknots;
    double* vknots;

    summary_bs3_surface(int nu, double* uk, int nv, double* vk);
    summary_bs3_surface(const summary_bs3_surface&);
    ~summary_bs3_surface();

    void	save();
    void	restore();
};

// An enum recording whether a surface is singular at the low end of its range,
// at the high end, both or neither.
/*
// tbrv
*/
/**
 * @nodoc
 */
enum	singularity_type
{
    NON_SINGULAR, SINGULAR_LOW, SINGULAR_HIGH, SINGULAR_BOTH,
    SINGULARITY_UNKNOWN
};
/*
// tbrv
*/
/**
 * @nodoc
 */
extern 	const enum_table	singularity_type_map;
/*
// tbrv
*/
/**
 * @nodoc
 */
logical pt_fn(double u, double	v, void* data, SPAposition& pt);
/*
// tbrv
*/
/**
 * @nodoc
 */
logical du_fn(double u, double v, void* data, SPAvector& upar);
/*
// tbrv
*/
/**
 * @nodoc
 */
logical dv_fn(double u, double v, void* data, SPAvector& vpar);
/*
// tbrv
*/
/**
 * @nodoc
 */
logical duv_fn(double u, double v, void* data, SPAvector& uvpar);

// Class for the spline surface itself, defined so that we shall be
// able to use use-counts to avoid too much copying, and to allow
// derivation to construct surfaces only approximated by the spline.
// There are virtual functions to support all the functions defined
// for the class spline which depend on the true definition of the
// surface.

// The base class is pure: classes for real surface types must be
// derived from it.

// In order to make spl_surs sharable in save files, this class is
// derived from "subtrans_object", which is in turn a derived class
// of "subtype_object", which handles saving and restoring, and also
// the use count. The class subtrans_object handles transformations,
// ensuring that shared objects are still shared even after being
// transformed separately.
/**
 * Defines an abstract base class from which spline surface definitions are derived.
 * <br>
 * <b>Role:</b> In ACIS a sculptured surface is represented by the class spline,
 * which contains a pointer to an internal description called <tt>spl_sur</tt>. The <tt>spl_sur</tt>
 * further contains a <tt>bs3_surface</tt> that is a pointer to a rational or nonrational,
 * nonuniform <i>B</i>-spline surface in the underlying surface package.
 * <br><br>
 * To support various types of surface construction, ACIS uses classes derived from
 * the internal representation <tt>spl_sur</tt>. Also, surface classes can be derived from
 * the derived class to construct more complicated surfaces. This section covers
 * the base class <tt>spl_sur</tt> along with the methods used to create derived classes,
 * rewritten per their specifications.
 * <br><br>
 * This class contains the mathematical definition for a spline surface. It uses
 * use counts to limit copying, and it allows derivation to construct surfaces that
 * are only approximated by the <tt>bs3_surface</tt>. The base class <tt>spl_sur</tt> contains the
 * following information for defining the surface:
 * <br><br>
 * -	A <i>use count</i> indicating the number of times this spl_sur is used.<br>
 * -	A pointer to a <tt>bs3_surface</tt> that represents a B-spline surface.<br>
 * -	A fitting tolerance representing the precision of the B-spline approximation to the true surface.
 * <br><br>
 * Classes derived from <tt>spl_sur</tt> can contain additional information, and can record
 * the creation method of the true spline surface.
 * <br><br>
 * All functions defined for the spline class are supported by virtual functions
 * that depend on the true definition of the surface. The functionality is made
 * virtual to allow the derived surfaces to implement the functionality on their
 * own. For surfaces that have an exact <tt>bs3_surface</tt>, there is no need to implement
 * the functionality because the methods written for the base class are sufficient.
 * <br><br>
 * @see discontinuity_info, summary_bs3_surface, spline, SPAinterval
 */
class DECL_KERN spl_sur : public subtrans_object {
    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef spl_sur_FRIENDS
    spl_sur_FRIENDS
#endif

        friend class splsur_cache;
    friend class gcmgr;
    friend class ss_gcmgr;
    friend class surface_evaluator_splsur;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend logical pt_fn(double u, double	v, void* data, SPAposition& pt);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend logical du_fn(double u, double v, void* data, SPAvector& upar);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend logical dv_fn(double u, double v, void* data, SPAvector& vpar);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend logical duv_fn(double u, double v, void* data, SPAvector& uvpar);

protected:
    bs3_surface sur_data;
    // Object-space approximation to true surface.

    double fitol_data;
    // The precision with which the spline approximates the
    // true surface.

// Return whether this spl_sur is precisely defined by its bs3_surface
// i.e. there is never a difference between evaluating the spl_sur and
// evaluating the bs3_surface. The default implementation is to return false.
    virtual logical is_exactly_bs3() const;

    // Return whether the approximated bs3_surface of the spl_sur is rational or not.
    logical is_rational_surface() const;

    discontinuity_info 	u_disc_info;
    discontinuity_info 	v_disc_info;
    const discontinuity_info& get_disc_info_u() const;
    const discontinuity_info& get_disc_info_v() const;
    // Discontinuity information. If the supporting geometry of the surface
    // has discontinuities, or if the surface has a default (tangent)
    // extension, then it will have discontinuities. These are stored here.

    SPAinterval 	u_range;
    SPAinterval 	v_range;
    // The full ranges of the spl_sur, as returned by param_range_u/v.
    // If an approximating surface is present (in sur_data) then these
    // ranges should identical to those of the approximating surface.

    closed_forms	closed_in_u;
    closed_forms	closed_in_v;
    // Take the values OPEN, CLOSED or PERIODIC (or unset if the spl_sur is
    // undefined). If an approximating surface is present (in sur_data)
    // then the closure of the approximating surface will be consistent.

    singularity_type	u_singularity;
    singularity_type	v_singularity;
    // Record whether the surface is singular. If an approximating surface
    // is present (in sur_data) then the singularities of the approximating
    // surface will be consistent.

    summary_bs3_surface* summary_data;
    // bs3_surface data in summary form.  This field may be set on
    // restore, if the full surface is not available.  It may be used to
    // make the actual bs3_surface.

    int 			summary_nuknots()	const { return summary_data->nuknots; }
    int			 	summary_nvknots()	const { return summary_data->nvknots; }
    const double* summary_uknots()	const { return summary_data->uknots; }
    const double* summary_vknots()	const { return summary_data->vknots; }
    // Provide read-only access to the summary_data for derived classes
    // (so that they don't all have to be friends).

    void			delete_summary_data()
    {
        ACIS_DELETE summary_data; summary_data = 0;
    }
    // Allow derived classes to delete summary_data when it goes out of
    // date.

// Copy data members from the "other" surface
// Internal use only
    logical copy_data_from_other(const surface* other);

    double local_geometric_resabs;

#ifdef ENABLE_CACHE_SWITCHING
private:
    // Pointer to cache information. This is manipulated solely by
    // base class functions, so is private rather than protected.
    mutable splsur_cache* cache;
#endif
public:
    /**
     * Method to be called by any user who modifies the surface in an external process, to ensure that stale evaluation results are discarded.
     */
    void invalidate_cache();
    // Function to be called by anyone who modifies the surface
    // behind our backs, to ensure that stale evaluation results
    // are discarded.

protected:
    disc_info_calc_status disc_calc_status;
    virtual	void 	calculate_disc_info();
    // Calculate the discontinuity information if it was never stored.  This
    // function is intended to support restore of old versions of splines.

// STL apc 11Apr03 Extra methods for (generic) checking of discontinuity
// data for spl_sur types (bug 69775)
    int calc_continuity(const SPApar_pos& uv, logical in_u) const;
    disc_info_status validate_disc_info(const discontinuity_info& supplied,
        const discontinuity_info& calculated,
        logical in_u,
        unmarked_discon_type& discon_type,
        int to_order = 3) const;

    void	update_data(bs3_surface);
    // Update the range, closure and singularity information from a
    // bs3_surface.

// Constructors.

// Construct a generally null spl_sur. This is to allow flexibility
// for constructors for derived classes to set the common data
// members in whatever way is most convenient.

    spl_sur();

public:
    // A constructor given all the basic data required.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
* <br><br>
 * @param appr
 * approximation surface.
 * @param fit
 * fit tolerance.
 */
    spl_sur(
        bs3_surface appr,
        double fit = 0
    );


    // A form of the constructor which takes ranges and closure information
    // instead of an approximating surface.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as arguments.
 * <br><br>
 * @param u_range
 * u range.
 * @param v_range
 * v range.
 * @param closure_u
 * type of closure in u.
 * @param closure_v
 * type of closure in v.
 * @param type_u
 * singularity type for u.
 * @param type_v
 * singularity type for v.
 */
    spl_sur(
        const SPAinterval& u_range,
        const SPAinterval& v_range,
        closed_forms closure_u,
        closed_forms closure_v,
        singularity_type type_u,
        singularity_type type_v
    );

    // Copy constructor. Derived classes should also define copy constructors,
    // and use this one, to ensure that spl_sur data is initialised.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param spl
 * spline surface.
 */
    spl_sur(const spl_sur& spl);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    void deep_copy_elements(const spl_sur& spl);

    // Make a copy without sharing subdata.
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * In a <i>deep copy</i>, all the information about the copied item is self-contained
 * in a new memory block. By comparison, a <i>shallow copy</i> stores only the first
 * instance of the item in memory, and increments the reference count for each copy.
 * <br><br>
 * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that have
 * already been deep copied. For example, a deep copy of a complex model results
 * in self-contained data, but identical sub-parts within the model are allowed
 * to share a single set of data.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
    virtual spl_sur* deep_copy(pointer_map* pm = NULL) const = 0;

protected:
    // The destructor eliminates all the dependent spline curve and
    // surface data. Each derived class must have a destructor if it
    // adds further dependent data.

    virtual ~spl_sur();

private:
    // Use count manipulation

    void operator++() { add_ref(); }
    void operator--() { remove_ref(); }
    logical mult() { return mult_ref(); }

protected:

    // Function to determine if this spl_sur is a pipe or is a wrapper
    // around a pipe surface (eg offset or subset pipe).

    virtual logical contains_pipe() const
    {
        return FALSE;
    }

    // Search the underlying cache for an entry at the given SPAposition.
    // Returns the matching eval entry if this is found, or NULL otherwise.

    const eval_sscache_entry* search_eval_cache(const SPAposition&) const;

    /**
     * @nodoc
     * Trim the query cache to range
     */
    void trim_cache_to_range(SPApar_box const&) const;


    // Prevent recursive calls to make_approx

    logical	calling_make_approx;

    // Make or remake the approximating surface.  The force flag forces the
    // approximating surface to be made even if it is illegal.  This can be
    // used to restore old parts that were not checked properly before being
    // saved.  The spline argument 'spl' may be null but if it is supplied the
    // function may be a little faster.  The function stores the approximating
    // surface and the actual fit error that was achieved in the spl_sur,
    // overriding the declared const-ness of the function to do this.

    virtual void	make_approx(
        double fit,
        const spline& spl = *(spline*)NULL_REF,
        logical force = FALSE
    ) const;


    // incremental_make_approx
    // makes an approximating surface for an extended spl_sur incrementally
    // given an approximating surface for the original spl_sur.
    // The  extension is done  first for the u/v direction which
    // is a smaller percentage of the original range.
    // this function could be called for any spl_sur but  at the
    // moment (July 2000) it is only being used for off_spl_sur's.

//GSSL VPL : Incremental make approx project - changed return type..
    virtual logical incremental_make_approx(double fit);

    // calc_patches is called by incremental_make_approx

    friend void calc_patches(spl_sur& sur, double& fit,
        double*& knots1, int& nknots1,
        double*& knots2, int& nknots2,
        logical uv_type,
        SPAinterval& orig_range1, SPAinterval& orig_range2,
        SPAinterval& range1, SPAinterval& range2);

    // calc_patch is called by calc_patches

    friend logical calc_patch(spl_sur& sur, double& fit,
        double*& knots, int& nknots,
        logical uv_type, logical dir_type,
        logical calc_knots, SPAinterval& erange1,
        SPAinterval& erange2, SPAinterval& srange);

    // Simple object-space bounding SPAbox, for a subset of the surface.
    // The default version uses the object-space spline, expanding
    // its SPAbox by the fit tolerance. Note that this cannot be const,
    // as it uses subset(), which is itself not const.

    virtual SPAbox bound(
        SPApar_box const& = *(SPApar_box*)NULL_REF
    );


    // Returns a cone bounding the normal direction of the surface. The
    // cone is deemed to have its apex at the origin, and has a given
    // axis direction and (positive) half-angle. If the logical
    // argument is TRUE, then a quick approximation may be found. The
    // approximate result may lie wholly within or wholly outside the
    // guaranteed bound (obtained with a FALSE argument), but may not
    // cross from inside to outside. Flags in the returned object
    // indicate whether the cone is in fact the best available, and
    // if not whether this result is inside or outside the best cone.
    // Note that this cannot be const, as it uses subset(), which is
    // itself not const.

    virtual surf_normcone normal_cone(
        SPApar_box const& = *(SPApar_box*)NULL_REF,
        logical = FALSE
    );


    // Extract detailed information about the surface.
public:
    /**
     * Returns the <tt>bs3_surface</tt> approximation.
     */
    bs3_surface sur() const { return sur_data; }
    /**
     * Returns the fit tolerance for the approximating <tt>bs3_surface</tt>.
     */
    double fitol() const { return fitol_data; }
    /**
     * Constructs a duplicate <tt>spl_sur</tt> in free storage of this object, with a zero use count.
     */
    virtual subtrans_object* copy() const = 0;

protected:

    // STI dgh: replace underlying bs3_surface approximation.  If the supplied
    // tol is negative, then it will be left unchanged (PC).

    void set_sur(bs3_surface, double tol = -1.0);

    // Duplication. Can't be done by constructor, as we want it
    // to be virtual.

    // Test for equality. This is sufficient for many derived classes,
    // and can be used by most others to check the basic
    // representation.
    // It does not guarantee that all effectively equal surfaces are
    // determined to be equal, but does guarantee that different
    // surfaces are correctly identified as such.

    virtual logical operator==(subtype_object const&) const;

    // Determine whether a spl_sur is entirely enclosed within
    // another.

    virtual logical operator>>(subtype_object const&) const;

    // Transformation. The base class transforms the spline and
    // tolerance - this may often be enough.

    virtual void operator*=(SPAtransf const&);

    // Parameter shift: adjust the spline surface to have a parameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic surface by
    // integral multiples of the period.

    virtual void shift_u(double);
    virtual void shift_v(double);


    // Perform a linear transformation on the parametrisation, so that
    // it starts and ends at the given values (which must be in increasing
    // order).

    // While adding these functions, the base class version throws
    // an error - once the implementation has propagated through the system,
    // it may be convenient to do the reparametrisation of the approximating
    // surface here.

    virtual void reparam_u(
        double,		// new start u parameter
        double		// new end u parameter
    );
    virtual void reparam_v(
        double,		// new start v parameter
        double		// new end v parameter
    );
    virtual void reparam(
        double,		// new start u parameter
        double,		// new end u parameter
        double,		// new start v parameter
        double		// new end v parameter
    );


    // Construct a new spl_sur which is a copy of the part of the
    // given one within given parameter bounds, unless this is not
    // a proper subset, when return the "this" pointer, or there is
    // no overlap of the ranges, when return NULL. The ranges should
    // not overlap at a single point in either parameter direction -
    // if they do, the whole parameter range in that direction is
    // assumed. Note that this cannot be "const" because it sometimes
    // returns "this" as a non-const pointer.

    virtual spl_sur* subset(
        SPApar_box const&
    );


    // Divides a surface into two pieces at a given parameter value.
    // If the split is at the end of the parameter range, the spl_sur
    // is just returned as the appropriate half (in increasing
    // parameter order), and the other is NULL. Otherwise a new spl_sur
    // is used for one part, and the old one is modified for the other.

    virtual void split_u(
        double,
        spl_sur* [2]
    ) = 0;
    virtual void split_v(
        double,
        spl_sur* [2]
    ) = 0;

    // The same as for split_u/v, but given an empty spl_sur in case it
    // is needed. This way a derived type can provide a spl_sur of its
    // own type for us to work with. The functions return TRUE if this
    // spl_sur is used, FALSE if not. These functions are not called
    // from the outside, but are available for use by derived class
    // implementations of splt_u/v().
    // Typically, the derived class implementations of split_u/v call these
    // functions with a slightly different parameter to the one that they
    // were originaly called with (the new parameter is obtained by relaxing
    // from the split point to the approximating surface, and so can be
    // regarded as the parameter on the approximating surface).  This makes
    // the housekeeping difficult, so I've changed it to take BOTH versions
    // of the parameter.  PC Apr 00.

    logical split_spl_sur_u(
        double approx_par, double real_par,
        spl_sur*,
        spl_sur* [2]
    );
    logical split_spl_sur_v(
        double approx_par, double real_par,
        spl_sur*,
        spl_sur* [2]
    );


    // Divides a surface into separate pieces which are smooth (and therefore
    // suitable for offsetting or blending). The surface is split at its non-G1
    // discontinuities, and if it is closed after this, it is then split into
    // two. The functions return the number of pieces, and the pieces themselves
    // are a return argument.

    virtual int split_at_kinks(spl_sur**& pieces, logical udir,
        double curvature = 0.0) const;


    // Concatenate the contents of two surfaces into one. The surfaces
    // are guaranteed to be the same base or derived type, and to have
    // contiguous parameter ranges ("this" is the beginning part of
    // the combined surface (i.e. lower parameter values), the
    // argument gives the end part).

    virtual void append_u(spl_sur&);
    virtual void append_v(spl_sur&);


    // Geometric evaluation.

    // Find the normal to the surface at the given point. Returns an
    // exact zero if the point is a singularity of the surface where
    // there is no well-defined normal.

    virtual SPAunit_vector point_normal(
        SPAposition const&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF
    ) const;


    // Returns a direction which points outward from the surface. This
    // should be the outward normal if the point is not singular,
    // otherwise a fairly arbitrary outward direction.

    virtual SPAunit_vector point_outdir(
        SPAposition const&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the principal axes of curvature of the surface at a
    // given point, and the curvatures in those directions.

    virtual void point_prin_curv(
        SPAposition const&,
        SPAunit_vector&,		// first axis direction
        double&,			// curvature in first direction
        SPAunit_vector&,		// second axis direction
        double&,			// curvature in second direction
        SPApar_pos const& = *(SPApar_pos*)NULL_REF,
        evaluate_surface_quadrant = evaluate_surface_unknown
    ) const;


    // Find the curvature of a cross-section curve of the surface at
    // the point on the surface closest to the given point, iterating
    // from the given parameter values (if supplied).
    // The cross-section curve is determined by the intersection of
    // the surface with a plane passing through the point on the
    // surface and with given normal.

    virtual double point_cross(
        SPAposition const&,
        SPAunit_vector const&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF
    ) const;

    // This version is for sided evaluation of curvature. The evaluation
    // can be specified on the right or left side of the given tangent
    // direction.

    virtual double point_cross(
        SPAposition const&,
        SPAunit_vector const&,
        SPApar_pos const& uv_guess,
        logical use_sided_eval,
        logical right_side
    ) const;

    // This non-virtual function looks in the cache for a given position
    // and parameter guess if any.  If found it returns the foot, normal,
    // curvature and parameter.  Otherwise it uses point_perp to find the
    // result, places them in the cache, and returns them.
    // Point_perp_with_cache, rather than point_perp, should be
    // called by classes derived from int_cur, so as to get
    // the benefit of caching.

    void point_perp_with_cache(
        SPAposition const&,
        SPAposition&,
        SPAunit_vector&,
        surf_princurv&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF,
        SPApar_pos & = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE,
        SPApar_box const& = *(SPApar_box*)NULL_REF
    ) const;

    // non-virtual method encapsulating the guts of the base class point perp algorithm
    logical point_perp_engine(
        SPAposition const& point,
        SPAposition& foot,
        SPAunit_vector& norm,
        surf_princurv& curv,
        SPApar_pos const& given_uv_guess,
        SPApar_pos& uv_actual,
        SPApar_pos& uv_fallback,
        logical f_weak,
        SPApar_box const& subset_range
    ) const;
    // Find the point on the surface nearest to the given point,
    // iterating from the given parameter values (if supplied).
    // Returns the found point, the normal to the surface at that
    // point and the parameter values at the found point.

    virtual void point_perp(
        SPAposition const&,
        SPAposition&,
        SPAunit_vector&,
        surf_princurv&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF,
        SPApar_pos & = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE,
        SPApar_box const& = *(SPApar_box*)NULL_REF
    ) const;


    // Support function for point_perp (and bs3_surface_perp). This
    // works quite hard at finding a true perpendicular given an
    // initial SPAparameter guess, and avoiding oscillations. It may be
    // set to iterate to the nearest perpendicular of any sort (minimum
    // or maximum distance, or inflexion), or to find only minima
    // (which is sometimes more reliable when there are inflexions),
    // and returns a success or failure indication.

public:
    /**
     * Support function for <tt>point_perp</tt> (and <tt>bs3_surface_perp</tt>).
     * <br><br>
     * <b>Role:</b> This method finds a true perpendicular given an initial parameter
     * guess, and avoiding oscillations. It may be set to iterate to the nearest
     * perpendicular of any sort (minimum or maximum distance, or inflexion), or to
     * find only minima (which is sometimes more reliable when there are inflexions),
     * and it returns a success or failure indication.
     * <br><br>
     * @param pos
     * given position.
     * @param surf
     * position on surface.
     * @param normal
     * normal to surface.
     * @param curv
     * principal curvature.
     * @param guess
     * guess parameter.
     * @param actual
     * actual parameter.
     * @param pt
     * <tt>TRUE</tt> to iterate to a (local) near-point rather than any perpendicular..
     */
    logical iterate_perp(
        SPAposition const& pos,
        SPAposition& surf,
        SPAunit_vector& normal,
        surf_princurv& curv,
        SPApar_pos const& guess,
        SPApar_pos& actual,
        logical	pt		// TRUE to iterate to a (local) near-
        // point rather than any perpendicular.
    ) const;
    // Find the point on the surface with given parameter values.

/**
 * Finds the point on the spline for a given parameter-space point.
 * <br><br>
 * @param parameter
 * given parameter-space point.
 */
    virtual SPAposition eval_position(
        SPApar_pos const& parameter
    ) const;


    // Find the position and first and second derivatives of the
    // surface at a given point.
/**
 * Finds the position and the first and second derivatives of the surface at a specified parameter-space point.
 * <br><br>
 * @param uv
 * given parameter-space point.
 * @param pos
 * returned point.
 * @param dpos
 * first derivative.
 * @param ddpos
 * second derivative.
 */
    virtual void eval(
        SPApar_pos const& uv,
        SPAposition& pos,
        SPAvector* dpos,	// first derivatives - array of
        // length 2 in order xu, xv
        SPAvector* ddpos	// second derivatives - array of
        // length 3 in order xuu, xuv, xvv
    ) const;


protected:

    // Similar function with an "evaldata" argument for improved efficiency.

    logical iterate_perp(
        SPAposition const&,
        surface_evaldata*,
        SPAposition&,
        SPAunit_vector&,
        surf_princurv&,
        SPApar_pos const&,
        SPApar_pos&,
        logical			// TRUE to iterate to a (local) near-
                        // point rather than any perpendicular.
    ) const;


    // This non-virtual function looks in the cache for a given position.
    // If found it returns the parameter, otherwise it finds the parameter
    // using param, places it in the cache, and returns it.
    // Param_with_cache, rather than param, should be called by classes
    // derived from int_cur, so as to get the benefit of caching.

    SPApar_pos param_with_cache(
        SPAposition const&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF
    );


    // Find the parameter values of a point on the surface.

    virtual SPApar_pos param(
        SPAposition const&,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF
    ) const = 0;


    // Find the change in surface parameter corresponding to a unit
    // offset in a given direction at a given position, the position
    // and direction both lying in the surface.

    virtual SPApar_vec param_unitvec(
        SPAunit_vector const&,
        SPApar_pos const&
    ) const;

    virtual SPApar_vec param_unitvec(
        const SPAunit_vector& dir,
        const SPApar_pos& pos, logical& well_conditioned
    ) const;

    // This non-virtual function looks in the cache for position and first
    // and second derivatives at the given parameter value.  If found it
    // returns them, otherwise it computes them, puts them in the cache,
    // and returns them.
    // Eval_with_cache, rather than eval, should be called by classes
    // derived from int_cur, so as to get the benefit of caching.

    void eval_with_cache(
        SPApar_pos const&,	// given parameter
        SPAposition&,			// point found
        SPAvector*,			// first derivative array
        SPAvector*			// second derivative array
    ) const;



    // Find the normal to the surface at the point with given
    // parameter values.

    virtual SPAunit_vector eval_normal(
        SPApar_pos const&
    ) const;


    // Returns a direction which points outward from the surface. This
    // should be the outward normal if the point is not singular,
    // otherwise a fairly arbitrary outward direction.

    virtual SPAunit_vector eval_outdir(
        SPApar_pos const&
    ) const;


    // Find the principal axes of curvature of the surface at a
    // point with given parameter values, and the curvatures in those
    // directions.

    virtual void eval_prin_curv(
        SPApar_pos const&,
        SPAunit_vector&,		// first axis direction
        double&,			// curvature in first direction
        SPAunit_vector&,		// second axis direction
        double&,			// curvature in second direction
        evaluate_surface_quadrant = evaluate_surface_unknown
    ) const;


    // Find the curvature of a cross-section curve of the surface at
    // the point on the surface with given parameter values.
    // The cross-section curve is defined as the intersection of
    // the surface with a plane passing through the point on the
    // surface and normal to the given direction, which must lie in
    // the surface.

    virtual double eval_cross(
        SPApar_pos const&,
        SPAunit_vector const&
    ) const;


    // This non-virtual function looks in the cache for SPAposition and nd
    // derivatives at the given parameter value.  If found it returns them,
    // otherwise it computes them, puts them in the cache, and returns them.
    // Evaluate_with_cache, rather than evaluate, should be called by classes
    // derived from int_cur, so as to get the benefit of caching.

    int evaluate_with_cache(
        SPApar_pos const&,	// Parameter
        SPAposition&,			// Point on surface at given parameter
        SPAvector** = NULL, 	// Array of pointers to arrays of vectors,
        // of size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivatives will not
        // be returned.
        int = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each parameter direction,
        // or don't care.
    ) const;


    // The evaluate() function calculates derivatives, of any order
    // up to the number requested, and stores them in vectors provided
    // by the user. It returns the number it was able to calculate;
    // any more will have to be evaluated by our caller using finite
    // differences. The number returned may be negated - the absolute
    // value is still the number of derivatives calculated, but the minus
    // sign indicates that no attempt should be made to evaluate further
    // derivatives using finite differences.

    virtual int evaluate(
        SPApar_pos const&,	// Parameter
        SPAposition&,			// Point on surface at given parameter
        SPAvector** = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each parameter direction,
        // or don't care.
    ) const;

    // This non-virtual function looks in the cache for SPAposition and nd
    // derivatives at the given parameter value.  If found it returns them,
    // otherwise it computes them, puts them in the cache, and returns them.
    // Evaluate_with_cache, rather than evaluate, should be called by classes
    // derived from int_cur, so as to get the benefit of caching.

    int evaluate_iter_with_cache(
        SPApar_pos const&,	// Parameter
        surface_evaldata*,	// Data supplying initial values, and
        // set to reflect the results of this
        // evaluation.
        SPAposition&,			// Point on curve at given parameter
        SPAvector** = NULL, 	// Array of pointers to arrays of vectors,
        // of size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivatives will not
        // be returned.
        int = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant = evaluate_surface_unknown
        // the evaluation location - above,
        // below or don't care.
    ) const;

    // The evaluate_iter() function is just like evaluate(), but is
    // supplied with a data object which contains results from a previous
    // close evaluation, for use as initial values for any iteration
    // involved.

    virtual int evaluate_iter(
        SPApar_pos const&,	// Parameter
        surface_evaldata*,	// Data supplying initial values, and
        // set to reflect the results of this
        // evaluation.
        SPAposition&,			// Point on surface at given parameter
        SPAvector** = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each parameter direction,
        // or don't care.
    ) const;

    // Construct a data object to retain evaluation information across
    // calls to evaluate_iter(). This is to allow subsidiary calls
    // within an iterative evaluator to start iteration much closer to the
    // required result than is possible just using the curve information
    // itself.

    virtual surface_evaldata* make_evaldata() const;

    // Returns the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.

    virtual int accurate_derivs(
        SPApar_box const& = *(SPApar_box*)NULL_REF
        // Defaults to the whole surface
    ) const;


    // STI aed: add planar method
        // Report whether surface is planar

    virtual logical planar(
        SPAposition&,
        SPAunit_vector&
    ) const;
    // STI aed: end

        // Enquiry functions - make these public, so that code building the derived
        // types can use them.

public:

    // Report whether a parametric surface is periodic in either
    // parameter direction (i.e. it is smoothly closed, so faces can
    // run over the seam).
/**
 * Determines if a parametric surface is periodic in the <i>u</i> direction.
 * <br><br>
 * <b>Role:</b> Determines if a parametric surface is periodic in the <i>u</i> direction.
 * (i.e. it is smoothly closed, so faces can run over the seam).
 */
    logical periodic_u() const { return closed_in_u == PERIODIC; }
    /**
     * Determines if a parametric surface is periodic in the v direction.
     * <br><br>
     * <b>Role:</b> Determines if a parametric surface is periodic in the <i>v</i> direction.
     * (i.e. it is smoothly closed, so faces can run over the seam).
     */
    logical periodic_v() const { return closed_in_v == PERIODIC; }


    // Report whether the surface is closed, smoothly or not, in
    // either parameter direction.
/**
 * Determines if the surface is closed, smoothly or not, in the <i>u</i>-parameter direction.
 */
    logical closed_u() const {
        return closed_in_u == CLOSED ||
            closed_in_u == PERIODIC;
    }
    /**
     * Determines if the surface is closed, smoothly or not, in the <i>v</i>-parameter direction.
     */
    logical closed_v() const {
        return closed_in_v == CLOSED ||
            closed_in_v == PERIODIC;
    }


    // Returns the period of a periodic parametric surface, zero if
    // the surface is not periodic in the chosen parameter or not
    // parametric.
/**
 * Returns the <i>u</i> period of a periodic parametric surface, zero if the surface is not periodic in the <i>u</i> direction.
 */
    double param_period_u() const
    {
        return closed_in_u == PERIODIC ? u_range.length() : 0.0;
    }
    /**
     * Returns the <i>v</i> period of a periodic parametric surface, zero if the surface is not periodic in the <i>v</i> direction.
     */
    double param_period_v() const
    {
        return closed_in_v == PERIODIC ? v_range.length() : 0.0;
    }


    // Returns the principal parameter range of a parametric surface in
    // a chosen parameter direction, or in both. For a non-parametric
    // surface, the range is returned as the empty interval or box.

    // A periodic surface is defined for all parameter values in the
    // periodic direction, by reducing the given parameter modulo the
    // period into this principal range. For a surface open or non-
    // periodic in the chosen direction the surface evaluation
    // functions are defined only for parameter values in the
    // returned range.

    // If a box is provided, the parameter range returned may be
    // restricted to a portion of the surface which is guaranteed to
    // contain all portions of the surface which lie within the region
    // of interest. If none is provided, and the parameter range in
    // some direction is unbounded, then conventionally an empty
    // interval is returned.

    // The default versions of these functions use the corresponding function
    // for the approximating surface if the surface is not subsetted, or the
    // subsetted range if it is.
/**
 * Returns the principal parameter range of a parametric surface in both <i>u</i> and <i>v</i>-parameter directions.
 * <br><br>
 * <b>Role:</b>
 * A periodic surface is defined for all parameter values in the periodic direction,
 * by reducing the given parameter modulo the period into this principal range.
 * For a surface open or nonperiodic in the chosen direction the surface evaluation
 * functions are defined only for parameter values in the returned range.
 * <br><br>
 * If a box is specified, the parameter range returned may be restricted to a portion
 * of the surface which is guaranteed to contain all portions of the surface that
 * lie within the region of interest. If none is specified and the parameter range
 * in some direction is unbounded, then an interval unbounded in that direction is returned.
 * <br><br>
 * @param box
 * object space box.
 */
    virtual SPApar_box param_range(SPAbox const& box = *(SPAbox*)NULL_REF) const
    {
        SPAUNUSED(box)
            return SPApar_box(u_range, v_range);
    }
    /**
     * Returns the principal parameter range of a parametric surface in the <i>u</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction, by reducing the given parameter modulo the period into this
     * principal range. For a surface open or nonperiodic in the chosen direction the
     * surface evaluation functions are defined only for parameter values in the
     * returned range.
     * <br><br>
     * @param box
     * object space box.
     */
    virtual SPAinterval param_range_u(SPAbox const& box = *(SPAbox*)NULL_REF) const
    {
        SPAUNUSED(box)
            return u_range;
    }
    /**
     * Returns the principal parameter range of a parametric surface in the <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> A periodic surface is defined for all parameter values in the
     * periodic direction, by reducing the given parameter modulo the period into this
     * principal range. For a surface open or nonperiodic in the chosen direction the
     * surface evaluation functions are defined only for parameter values in the returned
     * range.
     * <br><br>
     * @param box
     * object space box.
     */
    virtual SPAinterval param_range_v(SPAbox const& box = *(SPAbox*)NULL_REF) const
    {
        SPAUNUSED(box)
            return v_range;
    }

    // Report whether the surface parametrisation is singular at
    // the specified u or v parameter value. The only singularity
    // recognised is where every value of the non-constant parameter
    // generates the same object-space point, and these can only
    // occur at the ends of the parameter range as returned by
    // the functions above. A plane is nonsingular in both directions.
/**
 * Reports whether the surface parameterization is singular at the specified <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> The only singularity recognized is where every value of the
 * <i>v</i> parameter generates the same object-space point, and these can only
 * occur at the ends of the parameter range as returned by <tt>param_range_u</tt>.
 * <br><br>
 * @param uparam
 * specified u parameter value.
 */
    logical singular_u(
        double	uparam	// constant u parameter
    ) const;
    /**
     * Reports whether the surface parameterization is singular at the specified <i>v</i>-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the
     * <i>u</i> parameter generates the same object-space point, and these can only
     * occur at the ends of the parameter range as returned by <tt>param_range_v</tt>.
     * <br><br>
     * @param vparam
     * specified v parameter value.
     */
    logical singular_v(
        double	vparam	// constant v parameter
    ) const;

protected:

    // Indicate whether the parameter coordinate system of the surface
    // is right- or left-handed. With a right-handed system, at any
    // point the outward normal is given by the cross product of the
    // increasing u direction with the increasing v direction, in that
    // order. With a left-handed system the outward normal is in the
    // opposite direction from this cross product.

    virtual logical left_handed_uv() const;


    // Construct a parameter line on the surface. A u parameter line
    // runs in the direction of increasing u parameter, at constant v.
    // A v parameter line runs in the direction of increasing v, at
    // constant u. The parametrisation in the non-constant direction
    // matches that of the surface, and has the range obtained by
    // use of param_range_u() or param_range_v() appropriately.

    // The new curve is constructed in free store, so it is the
    // responsibility of the caller to ensure that it is correctly
    // deleted.

    virtual curve* u_param_line(
        double,			// constant v parameter
        spline const&	// owning surface
    ) const;
    virtual curve* v_param_line(
        double,			// constant u parameter
        spline const&	// owning surface
    ) const;


    // Test whether a point lies on the surface, within a user-defined
    // tolerance.

    virtual logical test_point_tol(
        SPAposition const&,
        double,
        SPApar_pos const& = *(SPApar_pos*)NULL_REF,
        SPApar_pos & = *(SPApar_pos*)NULL_REF
    ) const;


    // Save and restore. Save is easy, as derived class switching goes
    // through the normal virtual function mechanism. Restore is more
    // complicated, because until it is invoked we do not have an
    // object of the right class. Instead we switch on a table defined
    // by static instances of the restore_ss_def class (see below), to
    // invoke a simple friend function which constructs an object of
    // the right (derived) type. Then it can call the appropriate
    // member function to do the actual work.

    virtual char const* type_name() const = 0;

    virtual void save_data() const;

    virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

    // Save data common to all spl_surs.

    void	save_common_data(save_approx_level)	const;


    // Ask the spl_sur the default level at which the approximating
    // surface should be be stored.

    save_approx_level enquire_save_approx_level()	const;


    // Note that restore_data is not virtual (so no base class version is
    // necessary).
    // restore_common_data restores the data saved by save_common_data.

    void restore_common_data();

#if defined D3_STANDALONE || defined D3_DEBUG

    friend DECL_KERN D3_ostream& operator<<(
        D3_ostream&,
        spl_sur const&
        );

    virtual void input(
        D3_istream&
    );

    friend DECL_KERN D3_istream& operator>>(
        D3_istream&,
        spl_sur*&
        );

    virtual void print(
        D3_ostream&
    ) const;

#endif

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
    void debug_data(
        char const*,
        logical,
        FILE*
    ) const;

    // After all this private stuff, make spline a friend so that it
    // can use it.

    // STIBUGFIX erich approx_save begin
    void save_as_approx() const;
    // STIBUGFIX erich approx_save end

    friend class spline;


    // Check for any data errors in the surface, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // surface on exit.
    // See chk_stat.hxx for information on the argument types used here.

    virtual	check_status_list* check(
        const check_fix& input = *(const check_fix*)NULL_REF,
        // supplies a set of flags which say which fixes
        // are allowable (the default is to fix nothing)
        check_fix& result = *(check_fix*)NULL_REF,
        // returns a set of flags which say which fixes
        // were applied
        const check_status_list* = (const check_status_list*)NULL_REF
        // list of checks that are to be made.  If the
        // list is null, then every possible check will
        // be made; otherwise, the function will only
        // check for things in the list.  The return
        // value for the function will then be a subset
        // of this list.
    );


# if ( defined D3_DEBUG || defined D3_STANDALONE )
    friend DECL_KERN D3_ostream& operator<<(D3_ostream& os, const spl_sur& ss);
    friend DECL_KERN D3_istream& operator>>(D3_istream& is, spl_sur*& ss);
#endif

    // STI swa 27Jul98 -- return sweep information
public:
    /**
     * Returns the sweep path type for this <tt>spl_sur</tt>.
     */
    virtual sweep_path_type get_path_type() const { return unknown_path_type; }
    /**
     * Returns the sweep path curve for this <tt>spl_sur</tt>. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const { return NULL; }
    /**
     * Returns the sweep profile curve for this <tt>spl_sur</tt>.  The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     * @param param
     * parameter
     */
    virtual curve* get_profile(double param/*param*/) const
    {
        SPAUNUSED(param)
            return NULL;
    }
    /**
     * Returns the sweep rail law for this <tt>spl_sur</tt>.
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
    virtual void minimize(minimize_helper*);

    /**
    * @nodoc
    **/
    virtual void process(geometry_definition_processor& p) const
#ifdef INTERNAL_DEBUG_CHECKS
        = 0;
#else
        ;
#endif

public:
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    logical checked;
protected:

    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_u_guess(const SPAparameter& u_guess,
        SPAparameter& valid_u_guess) const;
    // For internal use only.
/**
 * @nodoc
 */
    virtual logical validate_v_guess(const SPAparameter& v_guess,
        SPAparameter& valid_v_guess) const;
    /**
     * @nodoc
     */
    virtual logical unbounded_u() const;

    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual logical unbounded_v() const;

public:
    // get and set functions for access.
    int gme_get_u_singularity();
    int gme_get_v_singularity();
    int gme_get_closed_in_u();
    int gme_get_closed_in_v();
    void gme_set_fitol_data(double data);
};



/** @} */
#endif

