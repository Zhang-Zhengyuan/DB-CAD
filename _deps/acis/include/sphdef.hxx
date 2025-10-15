/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                     sphere surface
/*******************************************************************/
#if !defined( sphere_CLASS )
#define sphere_CLASS
#include "dcl_kern.h"
#include "surdef.hxx"
#include "debugmsc.hxx"
#include "unitvec.hxx"
#include "position.hxx"
class SizeAccumulator;
class curve;
class SPAbox;
class SPAtransf;
class sphere;
/*******************************************************************/
/**
* @file sphdef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
 /*
  * @nodoc
  * Define an identifying type for this (lower-case) surface.
  */
#define sphere_type 3
  /**
   * Transforms a sphere surface.
   * <br><br>
   * @param name
   * sphere name.
   * @param trans
   * transform to use.
   */
DECL_KERN sphere operator*(sphere const& name, SPAtransf const& trans);
DECL_KERN sphere gme_operator_multiply(sphere const& name, SPAtransf const& trans);
/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN surface* restore_cone();
/**
 * Spherical surface definition.
 * <br>
 * <b>Role:</b> A sphere is defined by a center point and radius.
 * A positive radius indicates an outward pointing surface normal.
 * A negative radius indicates an inward pointing surface normal.
 * <br><br>
 * Five data members define the parameterization of the sphere and
 * they are described in "Data".
 * <br><br>
 * The <i>u-parameter</i> is the latitude metric, running from <tt>-pi/2</tt> at
 * the south pole through 0 at the equator to <tt>pi/2</tt> at the north
 * pole.
 * <br><br>
 * The <i>v-parameter</i> is the longitude metric, running from <tt>-pi</tt> to <tt>pi</tt>,
 * with 0 on the meridian containing <tt>ori_dir</tt>, and increasing in a
 * clockwise direction around <tt>pole_dir</tt>, unless <tt>reverse_v</tt> is <tt>TRUE</tt>.
 * <br><br>
 * Let <i>P</i> be <tt>pole_dir</tt> and <i>Q</i> <tt>ori_dir</tt>, and let <i>R</i>
 * be <tt>P x Q</tt> , negated if <tt>reverse_v</tt> is <tt>TRUE</tt>.
 * Let <i>r</i> be the absolute value of the sphere radius. Then:<pre>
  * pos = center + r* sin(u)* P + r* cos(u) * (cos(v)* Q + sin(v) R)</pre>
 * This parameterization is left-handed for a convex sphere and
 * right-handed for a hollow one, if <tt>reverse_v</tt> is <tt>FALSE</tt>, and
 * reversed if it is <tt>TRUE</tt>. When the sphere is transformed, the
 * sense of <tt>reverse_v</tt> is inverted if the transform includes a
 * reflection. No special action is required for a negation.
 * <br><br>
 * The parametrisation implemented uses conventional latitude/
 * longitude angles. The direction <tt>pole_dir</tt> specifies the <tt>north pole</tt>,
 * and <tt>uv_oridir</tt> gives the zero meridian and equator.
 * The <i></i> <tt>SPAparameter</tt> is the angle between the line from center to
 * test point and the equatorial plane, negative in the southern
 * hemisphere, positive in the northern. It thus runs from <tt>-pi/2</tt>
 * to <tt>pi/2</tt>. The <i>v</i> <tt>SPAparameter</tt> is the azimuth angle, between
 * <tt>-pi</tt> and <tt>pi</tt>. The positive <i>v</i> direction is clockwise around
 * <tt>pole_dir</tt> unless <tt>reverse_v</tt> is <tt>TRUE</tt>, in which case it is anticlockwise.
 * <br><br>
 * In summary, spheres are:
 * <br><br>
 * -	Not true parametric surfaces.<br>
 * -	Periodic in <i>v</i> (<tt>-pi</tt> to <tt>pi</tt> with period <tt>2 * pi</tt>) but not in <tt>u</tt>.<br>
 * -	Closed in <i>v</i> but not in <i>u</i>.<br>
 * -	Singular in <i>u</i> at the poles; nonsingular everywhere else.
 * @see SPHERE, SPAposition, SPAunit_vector
 */
class DECL_KERN sphere : public surface
{
public:
    /**
     * The center of the sphere.
     */
    SPAposition centre;
    /**
     * The radius of a sphere.
     * <br><br>
     * <b>Role:</b> If negative, the surface normal points inward to the center of the sphere.
     */
    double radius;
    /**
     * Direction from the center of the sphere to the origin of parameter
     * space.
     */
    SPAunit_vector uv_oridir;
    /**
     * Direction normal to <tt>uv_oridir</tt> that points from the center to the <tt>north pole</tt> of the sphere - the maximum-<i>u</i> singularity.
     */
    SPAunit_vector pole_dir;
    /**
     * Constant <i>u</i>-parameter lines are circles around <tt>pole_dir</tt>.
     * <br><br>
     * <b>Role:</b> Normally clockwise, but counterclockwise if this is <tt>TRUE</tt>.
     */
    logical reverse_v;
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    sphere();
    sphere(const char* gme);
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param sph
     * sphere.
     */
    sphere(
        sphere const& sph
    );
    sphere(const char* gme, const sphere& sph);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * @param pos
     * position.
     * @param radius
     * radius.
     */
    sphere(
        SPAposition const& pos,
        double radius
    );
    sphere(const char* gme, const SPAposition& pos, double radius);
    /**
     * C++ destructor, deleting a sphere.
     */
    ~sphere();
    /**
     * Returns the number of derivatives that evaluate can find accurately (and directly).
     * <br><br>
     * <b>Role:</b> Returns the number of derivatives that evaluate can find accurately
     * (and directly), rather than by finite differencing, over the given portion of
     * the curve. If there is no limit to the number of accurate derivatives, returns
     * the value <tt>ALL_SURFACE_DERIVATIVES</tt>. This is the case with a sphere.
     * <br><br>
     * @param name
     * parameter box name.
     */
    virtual int accurate_derivs(
        SPApar_box const& name = *(SPApar_box*)NULL_REF
    ) const;
    /**
     * Returns a box that encloses the surface in parameter space.
     * <br><br>
     * @param space
     * parameter space box.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPApar_box const& space = *(SPApar_box*)NULL_REF,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;

    /**
     * Returns a box that encloses the surface in object space.
     * <br><br>
     * @param box
     * box.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAbox const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Reports whether the surface is closed, smoothly or not, in the <i>u-parameter</i> direction.
     * <br><br>
     * <b>Role:</b> A sphere is open in the <i>u</i>-direction.
     */
    virtual logical closed_u() const;
    /**
     * Reports whether the surface is closed, smoothly or not, in the <i>v-parameter</i> direction.
     * <br><br>
     * <b>Role:</b> A sphere is closed in the <i>v</i>-direction.
     */
    virtual logical closed_v() const;
    /**
     * Prints out details of sphere.
     * <br><br>
     * @param leader
     * leader string.
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
     * <b>Role:</b> Allocates new storage for all member data and any
     * pointers. Returns a pointer to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual surface* deep_copy(pointer_map* pm = NULL) const;
    /**
     * Evaluates to point from parameters.<br><br>
     * <b>Role:</b> Finds the point on a parametric surface with given
     * parameter values, and optionally the first and second derivatives
     * as well or instead.
     * <br><br>
     * @param parpos
     * parameter position.
     * @param pos
     * position.
     * @param deriv1
     * 1st derivatives - array length 2, in order xu, xv.
     * @param deriv2
     * 2nd derivatives - array of length 3, in order xuu, xuv, xvv.
     */
    virtual void eval(
        SPApar_pos const& parpos,
        SPAposition& pos,
        SPAvector* deriv1 = NULL,
        SPAvector* deriv2 = NULL
    ) const;
    /**
     * Finds the curvature of a cross-section curve of the parametric surface at the point with given parameter values.
     * <br><br>
     * <b>Role:</b> The cross-section curve is given by the intersection
     * of the surface with a plane passing through the point and with given normal.
     * <br><br>
     * @param parpos
     * parameter position.
     * @param direction
     * direction.
     */
    virtual double eval_cross(
        SPApar_pos const& parpos,
        SPAunit_vector const& direction
    ) const;

    /**
     * Finds the normal to a parametric surface at a point with given parameter values.
     * <br><br>
     * @param parpos
     * parameter position.
     */
    virtual SPAunit_vector eval_normal(
        SPApar_pos const& parpos
    ) const;
    /**
     * Finds the principal axes of curvature and the curvatures in those directions of the surface at a point with given parameter values.
     * <br><br>
     * <b>Role:</b> For a sphere, the curvature in every direction is a
     * constant, so the direction of the principal axes is arbitrary.
     * <br><br>
     * @param parameter
     * parameter.
     * @param axis1
     * first axis direction.
     * @param curv1
     * curvature in first direction.
     * @param axis2
     * second axis direction.
     * @param curv2
     * curvatues in second direction.
     */
    virtual void eval_prin_curv(
        SPApar_pos const& parameter,
        SPAunit_vector& axis1,
        double& curv1,
        SPAunit_vector& axis2,
        double& curv2
    ) const;
    /**
     * Finds the principal axes of curvature and the curvatures in those directions of the surface at a point with given parameter values.
     * <br><br>
     * <b>Role:</b> For a sphere, the curvature in every direction is a
     * constant, so the direction of the principal axes is arbitrary.
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
    /**
     * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
     * <br><br>
     * <b>Role:</b> The function returns the number of derivatives
     * calculated. Any of the pointers may be <tt>NULL</tt>, in which case the
     * corresponding derivatives will not be returned. Otherwise they
     * must point to arrays long enough for all the derivatives of that
     * order - i.e., 2 for the first derivatives, 3 for the second, etc.
     * <br><br>
     * @param parpos
     * param position.
     * @param pos
     * pt on surface at given param position.
     * @param vec
     * array of pointers to arrays of vectors.
     * @param deriv
     * number of derivatives required.
     * @param location
     * the evaluation location above, below for each parameter dir., or
     * do not care.
     */
    virtual int evaluate(
        SPApar_pos const& parpos,
        SPAposition& pos,
        SPAvector** vec = NULL,
        int              deriv = 0,
        evaluate_surface_quadrant location = evaluate_surface_unknown
    ) const;
    /**
     * @nodoc
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * Determines if a sphere is hollow, - negative radius.
     */
    logical hollow() const {
        return radius < 0;
    }
    /**
     * Indicates whether the parameter coordinate system of the surface is right-handed or left-handed.
     * <br><br>
     * <b>Role:</b> With a right-handed system, at any point the outward
     * normal is given by the cross product of the increasing <i>u</i>-direction
     * with the increasing <i>v</i>-direction, in that order. With a left-handed
     * system the outward normal is in the opposite direction from this
     * cross product.
     */
    virtual logical left_handed_uv() const;
    /**
     * Makes a copy of this sphere on the heap, and returns a pointer to it.
     */
    virtual surface* make_copy() const;
    /**
     * Negates this sphere.
     */
    virtual surface& negate();
    /**
     * Returns a cone bounding the normal direction of a curve.
     * <br><br>
     * <b>Role:</b> The cone is deemed to have its apex at the origin,
     * and has a given axis direction and (positive) half-angle. If the
     * logical argument is <tt>TRUE</tt>, then a quick approximation may be found.
     * The approximate result may lie completely inside or outside the
     * guaranteed bound (obtained with a <tt>FALSE</tt> argument), but may not
     * cross from inside to outside. Flags in the returned object
     * indicate whether the cone is in fact the best available, and if
     * not whether this result is inside or outside the best cone.
     * <br><br>
     * @param bound
     * parameter bounds.
     * @param result
     * approximate results OK?
     * @param trans
     * transformation.
     */
    virtual surf_normcone normal_cone(
        SPApar_box const& bound,
        logical          result = FALSE,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a copy of this sphere negated - with normal reversed.
     */
    sphere operator-() const;
    /**
     * Tests two surfaces for equality.
     * <br><br>
     * <b>Role:</b> This, like testing floating point numbers for
     * equality, is not guaranteed to say <i>equal</i> for effectively equal
     * surfaces, but is guaranteed to say <i>not equal</i> if they are indeed
     * not equal. The result can be used for optimization, but not where
     * it really matters.
     * <br><br>
     * @param surf
     * surface name.
     */
    virtual logical operator==(surface const& surf) const;
    /**
     * @nodoc
     * Internal use.
     */
    friend DECL_KERN sphere operator*(
        sphere const&,
        SPAtransf const&
        );
    /**
     * Transforms this sphere by the given transform.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual surface& operator*=(
        SPAtransf const& trans
        );
    /**
     * Finds the parameter values of a point on a surface, given an optional first guess.
     * <br><br>
     * @param pos
     * position.
     * @param parpos
     * parameter position.
     */
    virtual SPApar_pos param(
        SPAposition const& pos,
        SPApar_pos const& parpos = *(SPApar_pos*)NULL_REF
    ) const;

    /**
     * Returns the period of a periodic parametric surface, or 0 if the surface is not periodic in the <i>u</i>-parameter or not parametric.
     * <br><br>
     * <b>Role:</b> A sphere is not periodic in the <i>u</i>-direction.
     */
    virtual double param_period_u() const;
    /**
     * Returns the period of a periodic parametric surface, or 0 if the surface is not  periodic in the <i>v</i>-parameter or not parametric.
     * <br><br>
     * <b>Role:</b> A sphere has a period of <tt>2 * pi</tt> in the <i>v</i>-direction.
     */
    virtual double param_period_v() const;
    /**
     * Returns the parameter ranges of the portion of a surface lying within the given box.
     * <br><br>
     * @param name
     * box name.
     */
    virtual SPApar_box param_range(
        SPAbox const& name = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the parameter ranges of the portion of a surface that lies within the given box in a <i>u</i>-parameter direction.
     * <br><br>
     * @param name
     * box name.
     */
    virtual SPAinterval param_range_u(
        SPAbox const& name = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the parameter ranges of the portion of a surface that lies within the given box in a <i>v</i>-parameter direction.
     * <br><br>
     * @param name
     * box name.
     */
    virtual SPAinterval param_range_v(
        SPAbox const& name = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Finds rate of change at a parameter.<br><br>
     * <b>Role:</b>
     * Finds the rate of change in surface parameter corresponding to a
     * unit velocity in a given object-space direction at a given
     * position in parameter space.
     * <br><br>
     * @param direction
     * direction.
     * @param parpos
     * parameter position.
     */
    virtual SPApar_vec param_unitvec(
        SPAunit_vector const& direction,
        SPApar_pos const& parpos
    ) const;
    /**
     * Determines if a sphere is parametric.
     * <br><br>
     * <b>Role:</b> Returns if this surface is parametric. A sphere is not considered to be parametric.
     */
    virtual logical parametric() const;
    /**
     * Reports whether the surface is periodic in the <i>u-parameter</i> direction.
     * <br><br>
     * <b>Role:</b> Reports whether the surface is periodic in the
     * <i>u</i>-parameter direction; i.e., it is smoothly closed, so faces can
     * run over the seam. A sphere is not periodic in the <i>u</i>-direction.
     */
    virtual logical periodic_u() const;
    /**
     * Reports whether the surface is periodic in the <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> Reports whether the surface is periodic in the
     * <i>v</i>-parameter direction; i.e., it is smoothly closed, so faces can
     * run over the seam. A sphere is not periodic in the <i>v</i>-direction.
     */
    virtual logical periodic_v() const;
    /**
     * Returns the surface normal at a given point on the surface.
     * <br><br>
     * @param pos
     * position.
     * @param parpos
     * parameter position.
     */
    virtual SPAunit_vector point_normal(
        SPAposition const& pos,
        SPApar_pos const& parpos = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Returns the curvature of a curve in the surface through a given point normal to a given direction in the surface.
     * <br><br>
     * <b>Role:</b> The curvature of a sphere is <tt>1/radius</tt> in all directions, at all locations.
     * <br><br>
     * @param pos
     * position.
     * @param direction
     * direction.
     * @param parpos
     * parameter position.
     */
    virtual double point_cross(
        SPAposition const& pos,
        SPAunit_vector const& direction,
        SPApar_pos const& parpos = *(SPApar_pos*)NULL_REF
    ) const;
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
     * @param parpos
     * parameter position.
     */
    virtual void point_prin_curv(
        SPAposition const& pos,
        SPAunit_vector& axis1,
        double& cur1,
        SPAunit_vector& axis2,
        double& cur2,
        SPApar_pos const& parpos = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Finds the principal axes of curvature of the surface at a given point, and the curvatures in those directions.
     * <br><br>
     * @param pos
     * position.
     * @param param_guess
     * parameter position.
     */
    surf_princurv point_prin_curv(
        SPAposition const& pos,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF
    ) const
    {
        return surface::point_prin_curv(pos, param_guess);
    }

    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> Optionally, the function may determine the normal to
     * and principal curvatures of the surface at that point. If the
     * surface is parametric, also return the parameter values at the
     * found point.
     * <br><br>
     * @param pos1
     * first position.
     * @param pos2
     * second position.
     * @param direction
     * direction.
     * @param princurv
     * surf principal curve.
     * @param parpos1
     * parameter position.
     * @param parpos2
     * parameter position.
     * @param f_weak
     * weak flag.
     */
    virtual void point_perp(
        SPAposition const& pos1,
        SPAposition& pos2,
        SPAunit_vector& direction,
        surf_princurv& princurv,
        SPApar_pos const& parpos1 = *(SPApar_pos*)NULL_REF,
        SPApar_pos& parpos2 = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const;
    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> Optionally, the function may determine the normal to
     * the surface at that point. If the surface is parametric, also
     * return the parameter values at the found point.
     * <br><br>
     * @param pos
     * position.
     * @param foot
     * foot position.
     * @param norm
     * direction.
     * @param param_guess
     * possible param.
     * @param param_actual
     * actual param.
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
     * <b>Role:</b> If the surface is parametric, also return the
     * parameter values at the found point.
     * <br><br>
     * @param pos
     * position.
     * @param foot
     * foot position.
     * @param param_guess
     * possible param.
     * @param param_actual
     * actual param.
     * @param f_weak
     * weak flag.
     */
    void point_perp(
        SPAposition const& pos,
        SPAposition& foot,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical           f_weak = FALSE
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
    /**
     * @nodoc
     * Internal use. Restore mechanism.
     */
    friend DECL_KERN surface* restore_cone();
    /**
     * Restores the data for a <tt>sphere</tt> from a save file.
     * <br><br>
     * <b>Role:</b><pre>
     * read_position			Center of sphere
     * read_real				Radius of sphere
     * if (restore_version_number &lt; SURFACE_VERSION)
     * 	// Old style - that is all to read.
     * else
     * read_unit_vector			uv x-axis
     * read_unti_vector			pole direction (z-axis)
     * read_logical				Reverse v; either "forward_v" or "reversed_v"
     * surface::restore_data	Restore remainder of surface data </pre>
     */
    void restore_data();

    /**
     * Saves the <tt>type</tt> or <tt>id</tt>, then calls <tt>save_data</tt>.
     */
    virtual void save() const;
    /**
     * Saves the information for sphere in the save file.
     */
    void save_data() const;
    /**
     * Reports whether the surface parameterization is singular at the specified <i>u</i>-parameter value.
     * <br><br>
     * <b>Role:</b> A sphere is singular in <i>u</i> at both poles.
     * <br><br>
     * @param uparam
     * constant u-parameter.
     */
    virtual logical singular_u(
        double	uparam
    ) const;
    /**
     * Reports whether the surface parameterization is singular at the specified <i>v-parameter</i> value.
     * <br><br>
     * <b>Role:</b> A sphere is not singular in <i>v</i> anywhere.
     * <br><br>
     * @param vparam
     * constant <i>v-parameter</i>.
     */
    virtual logical singular_v(
        double vparam
    ) const;
    /**
     * Tests whether a point lies on the surface, to user-supplied precision.
     * <br><br>
     * @param pos
     * position.
     * @param param
     * parameter.
     * @param parpos
     * parameter position.
     * @param parameter
     * parameter.
     */
    virtual logical test_point_tol(
        SPAposition const& pos,
        double            param = 0,
        SPApar_pos const& parpos = *(SPApar_pos*)NULL_REF,
        SPApar_pos& parameter = *(SPApar_pos*)NULL_REF
    ) const;

    /**
     * Returns the type of sphere.
     */
    virtual int type() const;
    /**
     * Returns the string <tt>"sphere"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Verifies if the sphere is properly defined.
     */
    virtual logical undef() const;
    /**
     * Constructs a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>u</i>-parameter line runs in the direction of
     * increasing <i>u</i>-parameter, at constant <i>v</i>. The parameterization in
     * the nonconstant direction matches that of the surface, and has
     * the range obtained by use of <tt>param_range_u</tt>. The new curve is
     * constructed in free store, so it is the responsibility of the
     * caller to ensure that it is correctly deleted.
     * <br><br>
     * @param vparam
     * constant v-parameter.
     */
    virtual curve* u_param_line(
        double vparam
    ) const;
    /**
     * Constructs a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>v</i>-parameter line runs in the direction of
     * increasing <i>v</i>, at constant <i>u</i>. The parameterization in the
     * nonconstant direction matches that of the surface, and has the
     * range obtained by use of <tt>param_range_v</tt>. The new curve is
     * constructed in free store, so it is the responsibility of the
     * caller to ensure that it is correctly deleted.
     * <br><br>
     * @param uparam
     * constant u-parameter.
     */
    virtual curve* v_param_line(
        double	uparam
    ) const;

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

};

/** @} */
#endif
