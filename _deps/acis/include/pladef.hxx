/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for (lower-case) plane.
// The plane is instanced in the PLANE entity.
#if !defined( plane_CLASS )
#define plane_CLASS
/**
 * @file pladef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
#include "dcl_kern.h"
#include "surdef.hxx"
#include "debugmsc.hxx"
#include "unitvec.hxx"
#include "position.hxx"
class SizeAccumulator;
class curve;
class SPAbox;
class SPAtransf;
// Define an identifying type for this (lower-case) surface.

#define plane_type 1

class plane;
/**
 * Returns a plane that is (a copy of) the given plane transformed by the given transform.
 *<br><br>
 * @param pla
 * given plane.
 * @param trans
 * transform.
 */
DECL_KERN plane operator*(plane const& pla, SPAtransf const& trans);
DECL_KERN plane gme_operator_mul(plane const& pla, SPAtransf const& trans);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN surface* restore_cone();


// Define plane proper.
/**
 * Defines a planar surface.
 * <br>
 *<b>Role:</b>  A plane class defines a plane with a point and a unit vector normal to the plane.
 * Usually, the point chosen to define the plane is near the center of interest. The normal
 * represents the outside of the surface. This is important when a plane is used to define a <tt>FACE</tt>
 * of a shell or solid.
 *<br><br>
 * Four data members describe the parameterization of the plane. For more information about data
 * members, see <tt>"Data"</tt>.
 *<br><br>
 * Four data members describe the parameterization of the plane. For more information about data
 * members, see <tt>"Data"</tt>.
 *<br><br>
 * To find the object-space point corresponding to a given (u, <tt>v)</tt> pair, first find the cross
 * product of the plane normal with <tt>u_deriv</tt>, negate it if <tt>reverse_v</tt> is <tt>TRUE</tt>, and call it <tt>v_deriv</tt>.
 * Then the evaluated position is:
 * <tt>pos = root_point + u* u_deriv + v* v_deriv</tt>
 * When the plane is transformed, <tt>u_deriv</tt> is transformed in the usual way, along with the root
 * point and normal, and <tt>reverse_v</tt> is inverted if the transform includes a reflection. When the
 * plane is negated, the direction of the normal is reversed, and <tt>reverse_v</tt> is inverted.
 *<br><br>
 * When a plane is constructed, <tt>u_deriv</tt> is automatically generated to be a fairly arbitrary unit
 * vector perpendicular to the normal, and <tt>reverse_v</tt> is set <tt>FALSE</tt>. If the normal is of zero
 * length, or if the plane is constructed using the raw constructor with no normal, <tt>u_deriv</tt> is
 * set to be a zero vector, and the arbitrary direction is generated whenever a parameter-based
 * function is called. Whenever an application changes the normal directly, it should also ensure
 * that <tt>u_deriv</tt> is perpendicular to it.
 *<br><br>
 * In summary, planes are:
 * <pre>
 * -	Not true parametric surfaces.
 * -	Open in u and v.
 * -	Not periodic in either u or v.
 * -	Not singular at any u or v.
 * </pre>
 * @see PLANE, SPAposition, SPAunit_vector, SPAvector
 */
class DECL_KERN plane : public surface {

public:

    /**
     * A point though which the plane passes.
     */
    SPAposition root_point;	// A point through which the plane passes

    /**
     * The normal to the plane.
     * <br><br>
     * <b>Role:</b> Conventionally set to a NULL unit vector to indicate that the plane is undefined.
     */
    SPAunit_vector normal;		// The normal to the plane. Conventionally
    // set to a null unit SPAvector to indicate
    // that the plane is undefined.

/**
 * The direction in the plane of constant v-parameter lines, with a magnitude to convert dimensionless parameter values into distances.
 * <br><br>
 * <b>Role:</b> This vector gives the direction and scaling of u-parameter lines.
 */
    SPAvector u_deriv;			// A SPAvector in the plane which gives the
    // direction and scaling of u SPAparameter
    // lines.

/**
 * By default the v-direction is the cross product of normal with <tt>u_dir</tt>.
 * <br><br>
 * <b>Role:</b> If this is <tt>TRUE</tt>, the v-direction must be negated. This is set to <tt>TRUE</tt> if the
 * parameterization is left-handed with respect to the surface normal, or <tt>FALSE</tt> if it is
 * right-handed. A right-handed parameterization is such as make the surface normal the direction
 * of the cross product of the u and v-directions, respectively.
 */
    logical reverse_v;		// By default the v direction is the
    // cross product of normal with u_dir.
    // If this is TRUE, the v direction must
    // be negated.


// "Uninitialised" plane, though the members are in fact
// initialised to null values.

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
    plane();
    plane(const char* gme);

    // Construct a plane as a copy of another one

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param plane_name
 * plane name.
 */
    plane(
        plane const& plane_name
    );
    plane(const char* gme, plane const& plane_name);

    // Construct a plane with given SPAposition and normal.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pos
 * plane root position.
 * @param vec
 * plane unit normal vector.
 */
    plane(
        SPAposition const& pos,
        SPAunit_vector const& vec
    );
    plane(const char* gme,
        SPAposition const& pos,
        SPAunit_vector const& vec
    );
    /**
    * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
    * <br><br>
    * @param pos
    * plane root position.
    * @param vec
    * plane unit normal vector.
    * @param udir
    * In plane u-direction vector.
    */
    plane(
        SPAposition const& pos,
        SPAunit_vector const& vec,
        SPAvector const& udir
    );
    plane(
        const char* gme,
        SPAposition const& pos,
        SPAunit_vector const& vec,
        SPAvector const& udir
    );

    // Virtual destructor - can't be inlined, so we define it to
    // avoid getting multiple copies.

/**
 * C++ destructor, deleting a plane.
 */
    ~plane();


    // Make a copy of this plane on the heap, and return a pointer to it.

/**
 * Returns a copy of the plane.
 */
    virtual surface* make_copy() const;
    // Make a copy without any sharing of subdata.

/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a pointer to the copied item.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
    virtual surface* deep_copy(pointer_map* pm = NULL) const;
    // Return a plane being (a copy of) this plane negated (i.e. with
    // opposite normal).

/**
 * Returns a plane being (a copy of) this plane negated; that is, with opposite normal.
 */
    plane operator-() const;

    // Test two surfaces for equality. This, like testing floating-
    // point numbers for equality, is not guaranteed to say "equal" for
    // effectively-equal surfaces, but is guaranteed to say "not equal"
    // if they are indeed not equal. The result can be used for
    // optimisation, but not where it really matters.

/**
 * Tests two surfaces for equality.
 * <br><br>
 * <b>Role:</b> This is not guaranteed to state equal for effectively equal surfaces, but is
 * guaranteed to state not equal if the surfaces are not equal. The result can be used for
 * optimization.
 * <br><br>
 * @param sur
 * surface.
 */
    virtual logical operator==(surface const& sur) const;

    // Return a plane being (a copy of) this plane transformed by
    // the given SPAtransf.

/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN plane operator*(
        plane const&,
        SPAtransf const&
        );

    // Transform this plane by the given SPAtransf.

/**
 * Transforms this plane by the given transform.
 * <br><br>
 * @param trans
 * transformation.
 */
    virtual surface& operator*=(
        SPAtransf const& trans
        );
    // Negate this plane.

/**
 * Negates this plane; that is reverses the surface normal.
 */
    virtual surface& negate();

    // Classification of a plane. The only thing we can say about it
    // is whether it is properly defined or not.

/**
 * Classification of a plane.
 */
    virtual logical undef() const;

    // Return a SPAbox around the surface. This need not be the smallest
    // SPAbox which contains the specified portion of the surface, but
    // needs to balance the tightness of the bound against the cost of
    // evaluation.

    // Surface bounded in SPAparameter space.

/**
 * Returns a box around the surface.
 * <br><br>
 * @param box
 * parameter space box.
 * @param trans
 * transformation.
 */
    virtual SPAbox bound(
        SPApar_box const& box = *(SPApar_box*)NULL_REF,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;

    // Surface bounded in object space.
/**
 * Returns a box around the surface.
 * <br><br>
 * @param box
 * box name.
 * @param trans
 * transformation.
 */
    virtual SPAbox bound(
        SPAbox const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;


    // Return a cone bounding the normal direction of the surface. The
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
 * <br><br>
 * <b>Role:</b> The cone is deemed to have its apex at the origin, and has a given axis direction
 * and (positive) half-angle. If the logical argument is <tt>TRUE</tt>, then a quick approximation may be
 * found. The approximate result may lie completely inside or outside the guaranteed bound
 * (obtained with a <tt>FALSE</tt> argument), but may not cross from inside to outside. Flags in the
 * returned object indicate whether the cone is in fact the best available, and if not whether
 * this result is inside or outside the best cone.
 * <br><br>
 * @param para_bound
 * parameter bounds.
 * @param result
 * approximate results OK.
 * @param trans
 * plane transform.
 */
    virtual surf_normcone normal_cone(
        SPApar_box const& para_bound,	// SPAparameter bounds
        logical result = FALSE,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;


    // Get the u SPAparameter direction. If there is one stored, and it
    // is suitable (i.e. it is not parallel to the plane normal),
    // return its projection into the plane, otherwise construct an
    // arbitrary direction in the plane.

/**
 * Gets the u-parameter direction.
 */
    SPAvector u_axis() const;

    // Geometric enquiry routines.

    // A plane is not considered to be parametrised, as surface
    // properties are easy to find in object space. Wherever a point
    // evaluator has a SPApar_pos argument, this is ignored, so
    // would normally be NULL or defaulted.

/**
 * Determines if a plane is parametric.
 * <br><br>
 * <b>Role:</b> A plane is not a parametric surface, as surface characteristics are easy to
 * calculate without and independent of parameter values.
 */
    virtual logical parametric() const;

    // Return the surface normal at a given point on the surface.

/**
 * Returns the surface normal at a given point on the surface.
 * <br><br>
 * @param pos
 * position.
 * @param param_pos
 * parameter position.
 */
    virtual SPAunit_vector point_normal(
        SPAposition const& pos,
        SPApar_pos const& param_pos = *(SPApar_pos*)NULL_REF
    ) const;

    // Return the principal directions and magnitudes of curvature
    // at a given point on the surface. The curvature is zero
    // everywhere on a plane, so the principal directions are rather
    // arbitrary in this case.

/**
 * Returns the principal directions and magnitudes of curvature at a given point on the surface.
 * <br><br>
 * <b>Role:</b> The curvature is zero everywhere on a plane, so the principal directions are
 * rather arbitrary in this case.
 * <br><br>
 * @param pos
 * position.
 * @param vec1
 * direction.
 * @param para1
 * parameter.
 * @param vec2
 * direction.
 * @param para2
 * parameter.
 * @param param_pos
 * parameter position.
 */
    virtual void point_prin_curv(
        SPAposition const& pos,
        SPAunit_vector& vec1,		// first axis direction
        double& para1,			// curvature in first direction
        SPAunit_vector& vec2,		// second axis direction
        double& para2,			// curvature in second direction
        SPApar_pos const& param_pos = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Returns the principal directions and magnitudes of curvature at a given point on the surface.
     * <br><br>
     * <b>Role:</b> The curvature is zero everywhere on a plane, so the principal directions are
     * rather arbitrary in this case.
     * <br><br>
     * @param pos
     * position.
     * @param param_guess
     * parameter position possible parameter.
     */
    surf_princurv point_prin_curv(
        SPAposition const& pos,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF
    ) const
    {
        return surface::point_prin_curv(pos, param_guess);
    }

    // Return the curvature of a curve in the surface through a
    // given point normal to a given direction in the surface.
    // The curvature of a plane is always zero.

/**
 * Returns the curvature of a curve in the surface through a given point normal to a given direction in the surface.
 * <br><br>
 * <b>Role:</b> The curvature of any curve on a plane is always zero.
 * <br><br>
 * @param pos
 * position name.
 * @param vec
 * direction.
 * @param para
 * parameter.
 */
    virtual double point_cross(
        SPAposition const& pos,
        SPAunit_vector const& vec,
        SPApar_pos const& para = *(SPApar_pos*)NULL_REF
    ) const;

    // Find the point on the surface nearest to the given point and
    // optionally the normal to and principal curvatures of the
    // surface at that point.  If the surface is parametric, also
    // return the SPAparameter values at the found point.

/**
 * Finds the point on the surface nearest to the given point.
 * <br><br>
 * <b>Role:</b> It may optionally return the normal to and principal curvatures of the surface at
 * that point. Also returns the parameter values at the found point, if desired.
 * <br><br>
 * @param pos
 * given position.
 * @param pos_plane
 * position on plane.
 * @param pt_plane
 * normal direction at point on plane.
 * @param cur
 * surface principle curve.
 * @param param_pos
 * param position.
 * @param act_pos
 * actual position.
 * @param f_weak
 * weak flag.
 */
    virtual void point_perp(
        SPAposition const& pos,
        SPAposition& pos_plane,
        SPAunit_vector& pt_plane,
        surf_princurv& cur,
        SPApar_pos const& param_pos = *(SPApar_pos*)NULL_REF,
        SPApar_pos& act_pos = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const;
    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> It may optionally return the parameter value at that point.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * position on plane.
     * @param norm
     * normal direction at point on plane.
     * @param param_guess
     * param position.
     * @param param_actual
     * possible position.
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
     * <b>Role:</b> It may optionally return the parameter value at that point.
     * <br><br>
     * @param pos
     * given position.
     * @param foot
     * position on plane.
     * @param param_guess
     * param position.
     * @param param_actual
     * possible position.
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

    // Now there is a set of SPAparameter-based functions. The Acis
    // kernel only requires them to have defined results if the
    // surface is parametric (i.e. the member function parametric()
    // returns TRUE), but husks and applications may expect valid
    // results for all surface types.

    // The parametrisation implemented uses a SPAvector (u_dir) in the
    // plane to show the u SPAparameter direction, and the scaling
    // factor for both parameters (to ensure that they are
    // dimensionless). The v direction is orthogonal to the plane
    // normal and u direction, and (n, u, v) form a right-handed or
    // left-handed set according to the value of "reverse_v".

    // Find the SPAparameter values of a point on a surface, given an
    // optional first guess.


/**
 * Finds the parameter values corresponding to a point on a surface.
 * <br><br>
 * @param pos
 * position.
 * @param param_pos
 * parameter position.
 */
    virtual SPApar_pos param(
        SPAposition const& pos,
        SPApar_pos const& param_pos = *(SPApar_pos*)NULL_REF
    ) const;

    // Find the rate of change in surface SPAparameter corresponding to
    // a unit velocity in a given object-space direction at a given
    // SPAposition in SPAparameter space..

/**
 * Finds the rate of change in surface parameter corresponding to a unit velocity in a given object-space direction at a given position in parameter space.
 * <br><br>
 * @param vec
 * direction.
 * @param param_pos
 * parameter position.
 */
    virtual SPApar_vec param_unitvec(
        SPAunit_vector const& vec,
        SPApar_pos const& param_pos
    ) const;
    // Find the point on a parametric surface with given SPAparameter
    // values, and optionally the first and second derivatives as
    // well or instead.

/**
 * Finds the point on the plane corresponding to the given parameter values.
 * <br><br>
 * <b>Role:</b> It may also return the first and second derivatives at this point.
 * <br><br>
 * @param param_pos
 * parameter position.
 * @param pos
 * position.
 * @param first_der
 * first derivatives - array of length 2, in order xu, xv.
 * @param second_der
 * second derivatives - array of length 3, in order xuu, xuv, xvv.
 */
    virtual void eval(
        SPApar_pos const& param_pos,
        SPAposition& pos,
        SPAvector* first_der = NULL,	// first derivatives - array of
        // length 2, in order xu, xv
        SPAvector* second_der = NULL		// second derivatives - array of
        // length 3, in order xuu, xuv, xvv
    ) const;


    // Find the point on a parametric surface with given SPAparameter
    // values. The default using eval() is fine.

//	virtual SPAposition eval_position(
//				SPApar_pos const &
//			) const;


    // Find the normal to a parametric surface at a point with given
    // SPAparameter values.

/**
 * Returns the surface normal at a given point on the surface.
 * <br><br>
 * @param param_pos
 * parameter position.
 */
    virtual SPAunit_vector eval_normal(
        SPApar_pos const& param_pos
    ) const;

    // Find the principal axes of curvature of the surface at a
    // point with given SPAparameter values, and the curvatures in those
    // directions.

/**
 * Finds the principal axes of curvature of the surface at a point with given parameter values.
 * <br><br>
 * <b>Role:</b> The function also determines curvatures in those directions.
 * <br><br>
 * @param param_pos
 * parameter position.
 * @param vec1
 * first axis direction.
 * @param cur1
 * curvature in first direction.
 * @param vec2
 * second axis direction.
 * @param cur2
 * curvature in second direction.
 */
    virtual void eval_prin_curv(
        SPApar_pos const& param_pos,
        SPAunit_vector& vec1,		// first axis direction
        double& cur1,			// curvature in first direction
        SPAunit_vector& vec2,		// second axis direction
        double& cur2			// curvature in second direction
    ) const;
    /**
     * Finds the principal axes of curvature of the surface at a point with given parameter values.
     * <br><br>
     * <b>Role:</b> The function also determines curvatures in those directions.
     * <br><br>
     * @param param
     * parameter position.
     */
    surf_princurv eval_prin_curv(
        SPApar_pos const& param
    ) const
    {
        return surface::eval_prin_curv(param);
    }

    // Find the curvature of a cross-section curve of the parametric
    // surface at the point with given SPAparameter values.  The
    // cross-section curve is given by the intersection of the
    // surface with a plane passing through the point and with given
    // normal.

/**
 * Finds the curvature of a cross-section curve of the plane at the point with given parameter values.
 * <br><br>
 * <b>Role:</b> The cross-section curve is given by the intersection of the surface with a plane
 * passing through the point and with given normal direction.
 * <br><br>
 * @param pos
 * parameter position.
 * @param vec
 * direction.
 */
    virtual double eval_cross(
        SPApar_pos const& pos,
        SPAunit_vector const& vec
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
 * Finds the principal axes of curvature of the surface at a point with given parameter values.
 * <br><br>
 * <b>Role:</b> The function also determines curvatures in those directions. Any of the pointers
 * may be <tt>NULL</tt>, in which case the corresponding derivatives will not be returned. Otherwise they
 * must point to arrays long enough for all the derivatives of that order - i.e. <tt>2</tt> for the first
 * derivatives, <tt>3</tt> for the second, etc.
 * <br><br>
 * @param param_pos
 * parameter position.
 * @param pos
 * position.
 * @param ptrs
 * array of ptrs.
 * @param nd
 * number of derivatives required (nd).
 * @param loc
 * the evaluation location, which is not used.
 */
    virtual int evaluate(
        SPApar_pos const& param_pos,	// Parameter
        SPAposition& pos,			// Point on surface at given SPAparameter
        SPAvector** ptrs = NULL, 	// Array of pointers to arrays of
        // vectors, of size nd. Any of the
        // pointers may be null, in which
        // case the corresponding derivatives
        // will not be returned. Otherwise
        // they must point to arrays long
        // enough for all the derivatives of
        // that order - i.e. 2 for the first
        // derivatives, 3 for the second, etc.
        int nd = 0,       		// Number of derivatives required (nd)
        evaluate_surface_quadrant loc = evaluate_surface_unknown
        // the evaluation location - above,
        // below for each SPAparameter direction,
        // or don't care.
    ) const;


    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.

/**
 * Returns the number of derivatives that evaluate can find accurately (and directly), rather than by finite differencing, over the given portion of the surface.
 * <br><br>
 * <b>Role:</b> For a plane, all surface derivatives can be obtained accurately.
 * <br><br>
 * @param para
 * parameter.
 */
    virtual int accurate_derivs(
        SPApar_box const& para = *(SPApar_box*)NULL_REF
        // Defaults to the whole surface
    ) const;

    // STI aed: add planar method
        // Report whether surface is planar

    /**
     * Reports on whether the plane is planar.
     * <br><br>
     * @param pt
     * Point through which the plane passes.
     * @param vec
     * axis direction.
     */
    virtual logical planar(
        SPAposition& pt,
        SPAunit_vector& vec
    ) const;
    // STI aed: end

        // Report whether the surface is periodic in either SPAparameter
        // direction (i.e. it is smoothly closed, so faces can run over
        // the seam). A plane is not periodic in either direction.

    /**
     * Reports whether the surface is periodic in the u-parameter direction; that is it is smoothly closed, so faces can run over the seam.
     * <br><br>
     * <b>Role:</b> A plane is not periodic in the u-direction.
     */
    virtual logical periodic_u() const;
    /**
     * Reports whether the surface is periodic in the v-parameter direction; that is it is smoothly closed, so faces can run over the seam.
     * <br><br>
     * <b>Role:</b> A plane is not periodic in the v-direction.
     */
    virtual logical periodic_v() const;


    // Report whether the surface is closed, smoothly or not, in
    // either SPAparameter direction. A plane is open in both directions.

/**
 * Reports if the surface is closed, smoothly or not, in the u-parameter direction.
 * <br><br>
 * <b>Role:</b> A plane is open in both directions.
 */
    virtual logical closed_u() const;
    /**
     * Reports if the surface is closed, smoothly or not, in the v-parameter direction.
     * <br><br>
     * <b>Role:</b> A plane is open in both directions.
     */
    virtual logical closed_v() const;


    // Return the period of a periodic parametric surface, zero if
    // the surface is not periodic in the chosen SPAparameter or not
    // parametric. A plane is non-periodic in both directions.

/**
 * Returns the period of a periodic parametric surface, or <tt>0</tt> if the surface is not periodic in the u-parameter or not parametric.
 * <br><br>
 * <b>Role:</b> A plane is not periodic in both directions.
 */
    virtual double param_period_u() const;
    /**
     * Returns the period of a periodic parametric surface, or <tt>0</tt> if the surface is not periodic in the v-parameter or not parametric.
     * <br><br>
     * <b>Role:</b> A plane is not periodic in both directions.
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
    // SPAinterval is returned.

/**
 * Returns the parameter range of a portion of the surface in the bounding box.
 * <br><br>
 * <b>Role:</b> If a box is provided, the parameter range returned is restricted to a portion of
 * the surface that is guaranteed to contain all portions of the surface that lie within the
 * region of interest. If none is provided, and the parameter range in some direction is
 * unbounded, then conventionally an empty interval is returned.
 * <br><br>
 * @param box
 * box name.
 */
    virtual SPApar_box param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the u-parameter direction.
     * <br><br>
     * <b>Role:</b> If a box is provided, the parameter range returned may be restricted to a portion
     * of the surface that is guaranteed to contain all portions of the surface that lie within the
     * region of interest. If none is provided, and the parameter range in some direction is
     * unbounded, then conventionally an empty interval is returned.
     * <br><br>
     * @param box
     * box name.
     */
    virtual SPAinterval param_range_u(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the v-parameter direction.
     * <br><br>
     * <b>Role:</b> If a box is provided, the parameter range returned may be restricted to a portion
     * of the surface that is guaranteed to contain all portions of the surface that lie within the
     * region of interest. If none is provided, and the parameter range in some direction is
     * unbounded, then conventionally an empty interval is returned.
     * <br><br>
     * @param box
     * box name.
     */
    virtual SPAinterval param_range_v(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;

    // Report whether the surface parametrisation is singular at
    // the specified u or v SPAparameter value. The only singularity
    // recognised is where every value of the non-constant SPAparameter
    // generates the same object-space point, and these can only
    // occur at the ends of the SPAparameter range as returned by
    // the functions above. A plane is nonsingular in both directions.

/**
 * Reports whether the surface parameterization is singular at the specified u-parameter value.
 * <br><br>
 * <b>Role:</b> The only singularity recognized is where every value of the non-constant
 * parameter generates the same object-space point, and these can only occur at the ends of the
 * parameter range as returned by the functions above. A plane is non-singular in both
 * directions.
 * <br><br>
 * @param uparam
 * constant u parameter.
 */
    virtual logical singular_u(
        double uparam		// constant u SPAparameter
    ) const;
    /**
     * Reports whether the surface parameterization is singular at the specified v-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the non-constant
     * parameter generates the same object-space point, and these can only occur at the ends of the
     * parameter range as returned by the functions above. A plane is non-singular in both
     * directions.
     * <br><br>
     * @param vparam
     * constant v parameter.
     */
    virtual logical singular_v(
        double vparam		// constant v SPAparameter
    ) const;

    // Indicate whether the SPAparameter coordinate system of the surface
    // is right- or left-handed. With a right-handed system, at any
    // point the outward normal is given by the cross product of the
    // increasing u direction with the increasing v direction, in that
    // order. With a left-handed system the outward normal is in the
    // opposite direction from this cross product.

/**
 * Indicates whether the parameter coordinate system of the surface is right or left-handed.
 * <br><br>
 * <b>Role:</b> With a right-handed system, at any point the outward normal is given by the cross
 * product of the increasing u-direction with the increasing v-direction, in that order. With a
 * left-handed system the outward normal is in the opposite direction from this cross product.
 */
    virtual logical left_handed_uv() const;

    // Construct a SPAparameter line on the surface. A u SPAparameter line
    // runs in the direction of increasing u SPAparameter, at constant v.
    // A v SPAparameter line runs in the direction of increasing v, at
    // constant u. The parametrisation in the non-constant direction
    // matches that of the surface, and has the range obtained by
    // use of param_range_u() or param_range_v() appropriately.

    // The new curve is constructed in free store, so it is the
    // responsibility of the caller to ensure that it is correctly
    // deleted.

/**
 * Constructs a parameter line on the surface.
 * <br><br>
 * <b>Role:</b> A u-parameter line runs in the direction of increasing u-parameter, at
 * constant v. The parameterization in the non-constant direction matches that of the surface,
 * and has the range obtained by use of param_range_u or param_range_v appropriately. If the
 * supplied constant parameter value is outside the valid range for the surface, or singularity,
 * a <tt>NULL</tt> is returned.
 * <br>
 * Because the new curve is constructed in free store, it is the responsibility of the caller to
 * ensure that it is correctly deleted.
 * <br><br>
 * @param val
 * parameter value.
 */
    virtual curve* u_param_line(
        double val			// constant v SPAparameter
    ) const;
    /**
     * Constructs a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A v-parameter line runs in the direction of increasing <tt>v</tt>, at constant <tt>u</tt>. The
     * parameterization in the non-constant direction matches that of the surface, and has the range
     * obtained by use of <tt>param_range_u</tt> or <tt>param_range_v</tt> appropriately. If the supplied constant
     * parameter value is outside the valid range for the surface, or singularity, a <tt>NULL</tt> is
     * returned.
     * <br>
     * Because the new curve is constructed in free store, it is the responsibility of the caller to
     * ensure that it is correctly deleted.
     * <br><br>
     * @param val
     * parameter value.
     */
    virtual curve* v_param_line(
        double val			// constant u SPAparameter
    ) const;


    // Test whether a point lies on the surface, to user-supplied
    // precision.

/**
 * Tests whether a point lies on the surface, to user-supplied precision.
 * <br><br>
 * <b>Role:</b> The function may optionally return the parametric position of the nearest point.
 * <br><br>
 * @param pos
 * position.
 * @param tol
 * tolerance value.
 * @param param_pos1
 * param position.
 * @param param_pos2
 * param position.
 */
    virtual logical test_point_tol(
        SPAposition const& pos,
        double tol = 0,
        SPApar_pos const& param_pos1 = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_pos2 = *(SPApar_pos*)NULL_REF
    ) const;

    // Return type of (lower-case) surface.

/**
 * Returns the type of plane.
 */
    virtual int type() const;
    // Return string identifier of surface.

/**
 * Returns string <tt>"plane"</tt>.
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
 * Saves the id then calls <tt>save_data</tt>.
 */
    virtual void save() const;
    /**
     * Saves the information for a plane to the save file.
     */
    void save_data() const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    friend DECL_KERN surface* restore_cone();
    /**
     * Restore the data for a plane from a save file.
     * <br><br>
     * <b>Role:</b> The restore_data function for each class is called in circumstances when the type
     * of surface is known and there is one available to be filled in.
     *<pre>
     * read_position			    					//Root point
     * read_unit_vector			    					//Normal vector to plane
     * if (restore_version_number &lt; SURFACE_VERSION) 	// Old style
     * else
     * read_vector			        					//u derivative vector
     * read_logical			       						// reverse v measured with respect to right hand rule: either "forward_v" or "reversed_v"
     * surface::restore_data							//Generic surface data
     *</pre>
     */
    void restore_data();


#if defined D3_STANDALONE || defined D3_DEBUG
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void print(
        D3_ostream&
    ) const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void input(
        D3_istream&
    );

#endif

    // Print out details of plane.

/**
 * Prints out details of plane.
 * <br><br>
 * @param debug_str
 * debug leader string.
 * @param ptr
 * file pointer.
 */
    virtual void debug(
        char const* debug_str,
        FILE* ptr = debug_file_ptr
    ) const;

    // STI swa 27Jul98 -- functions to get sweep information on a plane
public:
    /**
     * Gets the type of sweeping path used for sweeping a plane.
     */
    virtual sweep_path_type get_path_type() const { return straight_path_type; }
    /**
     * Gets the curve used as a sweeping path.
     * <br><br>
     * <b>Role:</b> This is <tt>NULL</tt> for a plane, but the method is included for compatibility with other
     * geometry classes. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const;
    /**
     * Returns the sweeping rail for the plane. This is normal to the plane.
     */
    virtual law* get_rail() const;
    // STI swa END

        // STI ROLL
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
};

/** @} */
#endif
