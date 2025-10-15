/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                       cone surface
/*******************************************************************/
#if !defined( cone_CLASS )
#define cone_CLASS
#include "logical.h"
#include <math.h>
#include "acis.hxx"
#include "dcl_kern.h"
#include "surdef.hxx"
#include "elldef.hxx"
#include "debugmsc.hxx"
class SizeAccumulator;
class SPAtransf;
class cone;
/*******************************************************************/
/**
* @file condef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
 /**
  * Defines an identifying type for this (lower-case) surface.
  */
#define cone_type 2
  /**
   * Returns a cone being (a copy of) this cone transformed by the given <tt>SPAtransf</tt>.
   * <br><br>
   * @param item
   * item to copy.
   * @param transform
   * transform.
   */
DECL_KERN cone operator*(cone const& item, SPAtransf const& transform);
DECL_KERN cone gme_operator_mul(cone const& item, SPAtransf const& transform);
/**
 * @nodoc
 * Internal use. Restoring mechanism.
 */
DECL_KERN surface* restore_cone();
/**
 * Defines the elliptical single cone.
 * <br>
 * <b>Role:</b> The cone class defines an  elliptical single cone. It is defined
 * by a base ellipse and the sine and cosine of the major half-angle of the cone.
 * <br><br>
 * The normal direction of the base ellipse represents the <i>axis</i> of the cone.
 * <br><br>
 * As special cases, the cross-section may be circular, or the cone may be a cylinder.
 * <br><br>
 * The base ellipse has the same data structure as an ellipse curve; i.e., center, normal,
 * major axis, radius ratio.
 * <br><br>
 * The polarity (sign) of the trigonometric functions define the slant of the surface
 * of the cone and the sense of the surface.
 * <br><br>
 * If <i>sine_angle</i> has different polarity than <i>cosine_angle</i>, the cross-section
 * <b>decreases</b> in the direction of the <i>axis</i> of the cone (ellipse surface normal)
 * <br><br>
 * If <i>sine_angle</i> has the same polarity as <i>cosine_angle</i>, the cross-section increases
 * in the direction of the <i>axis</i> of the cone (ellipse surface normal).
 * <br><br>
 * If <i>cosine_angle</i> is positive (+), the sense of the surface is away from the axis of
 * the cone (surface is convex).
 * <br><br>
 * If <i>cosine_angle</i> is negative (-), the sense of the surface is toward the axis of the
 * cone (surface is concave).
 * <br><br>
 * If <i>sine_angle</i> is identically zero (<tt>sine_angle == 0</tt>), the cone is a cylinder.
 * <br><br>
 * If <i>cosine_angle</i> is identically zero (<tt>cosine_angle ==0</tt>), the cone is planar.
 * <br><br>
 * The surface stops at the apex, if any; i.e., this surface type does not represent
 * a double cone.
 * <br><br>
 * There is a set of parameter-based functions. ACIS only requires them to have defined
 * results if the surface is <tt>parametric</tt> (i.e., method parametric returns <tt>TRUE</tt>), but components
 * and applications may expect results for all surface types.
 * <br><br>
 * The <i>u</i>-parameter direction is along the generators of the cone, with zero representing
 * the intersection of the generator with the base ellipse, and parameter increasing in the
 * direction of the cone axis; i.e., the normal of the base ellipse, if <tt>reverse_u</tt>
 * is <tt>FALSE</tt>, and in the opposite direction if <tt>reverse_u</tt> is <tt>TRUE</tt>.
 * The <i>v</i>-parameter direction is along a cross-sectional ellipse clockwise around the
 * cone axis, parameterized as for the base ellipse. The <i>u</i>-parameter scaling factor
 * stores the factor that when multiplied by the <i>u</i>-parameter of a point gives the
 * 3D distance of that point along the cone surface from the base ellipse. The <i>u</i>-parameter
 * is always 0.0 on the cone base ellipse. This enables the parameterization to
 * be preserved if the cone is offset.
 * <br><br>
 * To evaluate the position corresponding to a given uv pair, first evaluate the base
 * ellipse at parameter <i>v</i>, and subtract the center point to give vector <i>V</i>.
 * Let <i>s</i> and <i>c</i> be <i>sine_angle</i> and <i>cosine_angle</i>; if <i>cosine_angle</i> is positive,
 * or -<i>sine_angle</i> and -<i>cosine_angle</i> if not. Let R be the length of the major
 * axis of the base ellipse, negated if <tt>reverse_u</tt> is <tt>TRUE</tt>. Then:
 * <pre>
 * pos = base.center + (1 + s*u)* V + c*u*R*base.normal </pre>
 * This parameterization is left-handed for a convex cone (<tt>cosine_angle > 0</tt>) with
 * <tt>reverse_u</tt> <tt>FALSE</tt> or for a concave cone with <tt>reverse_u</tt> <tt>TRUE</tt>,
 * and right-handed otherwise.
 * <br><br>
 * When the cone is transformed, the sense of <tt>reverse_u</tt> is inverted if the transform
 * includes a reflection. A negation requires no special action.
 * <br><br>
 * In summary, cones are:<br>
 * - Not <tt>TRUE</tt> parametric surfaces.<br>
 * - Are closed in <i>v</i> but not in <i>u</i>.<br>
 * - Periodic in <i>v</i> ( -pi to pi with period 2 pi) but not in <i>u</i>.<br>
 * - Singular in <i>u</i> at the apex; nonsingular for all other <i>u</i> and <i>v</i> values.<br>
 * - For images illustrating this class, look under Technical Articles of the Kernel Component at "Classes with Images"
 * @see ellipse,CONE
 */
class DECL_KERN cone : public surface
{
public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
    cone();
    cone(const char* gme);
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param c
     * cone.
     */
    cone(cone const& c);
    cone(const char* gme, cone const& c);
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Constructs a cone with given ellipse as base, and given sine and cosine of
     * its half-angle.
     * <br><br>
     * @param base
     * base ellipse.
     * @param sin_ang
     * sine of half-angle.
     * @param co_ang
     * cosine of half-angle.
     * @param para
     * u parameter scale.
     */
    cone(
        ellipse const& base,
        double        sin_ang,
        double        co_ang,
        double        para = 0.0
    );

    cone(const char* gme,
        ellipse const& base,
        double        sin_ang,
        double        co_ang,
        double        para = 0.0
    );
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Constructs a cone with axis through given point and in given direction.
     * <br><br>
     * @param centre
     * center.
     * @param dir
     * axis direction.
     * @param radius
     * major radius.
     * @param ratio
     * radius ratio.
     * @param sin_ang
     * sine of half-angle.
     * @param co_ang
     * cosine of half-angle.
     * @param para
     * u parameter scale.
     */
    cone(
        SPAposition    const& centre,
        SPAunit_vector const& dir,
        SPAvector      const& radius,
        double ratio,
        double sin_ang,
        double co_ang,
        double para = 0.0
    );
    cone(
        const char* gme,
        SPAposition    const& centre,
        SPAunit_vector const& dir,
        SPAvector      const& radius,
        double ratio,
        double sin_ang,
        double co_ang,
        double para = 0.0
    );

    /**
     * C++ destructor, deleting a cone.
     * <br><br>
     * <b>Role:</b> The destructor is explicitly defined to avoid multiple copies.
     */
    ~cone();
    /**
     * Cross-section at right angles to axis.
     */
    ellipse base;
    /**
     * Cosine of the angle between major generator and axis.
     */
    double cosine_angle;
    /**
     * This cone is a cylinder.
     */
    logical _IsCylinder;
    /**
     * Required to support transformation independent parameterization.
     * <br><br>
     * <b>Role:</b> The <i>u</i> parameter direction (along generators) is normally in the same general
     * direction as the cone axis (normal to the base ellipse), but is reversed if
     * the value is <tt>TRUE</tt>.
     */
    logical reverse_u;
    /**
     * Sine of the angle between major generator and axis.
     * <br><br>
     * <b>Role:</b> By convention, both sine and cosine are made exactly zero to indicate that
     * the curve is undefined.
     */
    double sine_angle;
    /**
     * Scaling of the <i>u</i> parameter lines.
     */
    double u_param_scale;
    /**
     * Returns the number of derivatives.
     * <br><br>
     * <b>Role:</b> Number of derivatives that <tt>evaluate</tt> can find accurately and directly,
     * rather than by finite differencing, over the given portion of the surface. If there
     * is no limit to the number of accurate derivatives, returns the value
     * <tt>ALL_SURFACE_DERIVATIVES</tt>.
     * <br><br>
     * @param box
     * parameter box.
     */
    virtual int accurate_derivs(
        SPApar_box const& box = *(SPApar_box*)NULL_REF
    ) const;
    /**
     * Returns a box around the portion of a surface bounded in parameter space.
     * <br><br>
     * @param box
     * given box in param space.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPApar_box const& box = *(SPApar_box*)NULL_REF,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Returns a box around the portion of a surface bounded by a box in object space.
     * <br><br>
     * @param box
     * box in object space.
     * @param trans
     * transformation.
     */
    virtual SPAbox bound(
        SPAbox const& box,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Classification routine that sets the base ratio to 1; therefore, the base is circular.
     *<br><br>
     * <b>Role:</b> Most circular cones are exactly circular, and there is
     * little harm barring speed loss in saying that a circular
     * cone is elliptical, so we just look for exact equality.
     */
    logical circular() const { return base.radius_ratio == 1; }
    /**
     * Reports whether the surface is closed, smoothly or not, in the <i>u</i>-parameter direction.
     */
    virtual logical closed_u() const;
    /**
     * Reports whether the surface is closed, smoothly or not, in the <i>v</i>-parameter direction.
     */
    virtual logical closed_v() const;
    /**
     * Classification routine that returns <tt>TRUE</tt> if the sine angle and cosine angle are of opposite signs and <tt>FALSE</tt>, otherwise.
     * <br><br>
     * <b>Role:</b> This is only meaningful if <tt>cylinder()</tt> returns <tt>FALSE</tt>.
     */
    logical contracting() const { logical retval = (sine_angle >= 0) != (cosine_angle >= 0);	return retval; }
    /**
     * Classification routine that returns <tt>TRUE</tt> if the sine angle is essentially zero (within <tt>SPAresnor</tt>).
     */
    logical cylinder() const { return _IsCylinder; }
    /**
     * Prints out the details of cone to a file.
     * <br><br>
     * @param lead_str
     * leader string.
     * @param fname
     * file name.
     */
    virtual void debug(
        char const* lead_str,
        FILE* fname = debug_file_ptr
    ) const;
    /**
     * Creates a copy of an item that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a
     * pointer to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
    virtual surface* deep_copy(pointer_map* pm = NULL) const;
    /**
     * Returns <tt>TRUE</tt> if the cosine angle and the sine angle are of the square sign.
     * <br><br>
     * <b>Role:</b> This is only really meaningful if <tt>cylinder()</tt> returns <tt>FALSE</tt>,
     * but at least we make expanding/contracting, hollow and positive consistent in all cases.
     */
    logical expanding() const {
        logical retval = (sine_angle >= 0) == (cosine_angle >= 0);
        return retval;
    }
    /**
     * Finds the point on a parametric surface with given parameter values, and optionally the first and second derivatives as well or instead.
     * <br><br>
     * @param para
     * parameter.
     * @param posi
     * position.
     * @param first
     * first derivative.
     * @param second
     * second derivatives.
     */
    virtual void eval(
        SPApar_pos const& para,
        SPAposition& posi,
        SPAvector* first = NULL,
        SPAvector* second = NULL
    ) const;
    /**
     * Finds the outward direction from the surface at a point with given parameter values.
     * <br><br>
     * @param para
     * parameter.
     */
    virtual SPAunit_vector eval_outdir(
        SPApar_pos const& para
    ) const;
    /**
     * Find the principal axes of curvature of the surface at a point with given parameter values, and the curvatures in those directions.
     * <br><br>
     * @param para
     * parameter.
     * @param f_axis
     * first axis direction.
     * @param f_curv
     * curvature in first direction.
     * @param s_axis
     * second axis direction.
     * @param s_curv
     * curvature in second direction.
     */
    virtual void eval_prin_curv(
        SPApar_pos const& para,
        SPAunit_vector& f_axis,
        double& f_curv,
        SPAunit_vector& s_axis,
        double& s_curv
    ) const;
    /**
     * Finds the principal axes of curvature of the surface at a point with given parameter values.
     * <br><br>
     * @param param
     * parameter.
     */
    surf_princurv eval_prin_curv(
        SPApar_pos const& param
    ) const
    {
        return surface::eval_prin_curv(param);
    }
    /**
     * Calculates derivatives, of any order up to the number requested, and store them in vectors provided by the user.
     * <br><br>
     * <b>Role:</b> This function returns the number it was able to calculate; this is equal
     * to the number requested in all but the most exceptional circumstances. A certain number
     * are evaluated directly and (more or less) accurately; higher derivatives are automatically
     * calculated by finite differencing. The accuracy of the finite difference derivatives
     * decreases with the order of the derivative as the cost increases. Any of the pointers
     * may be <tt>NULL</tt>, in which case the corresponding derivatives will not be returned. Otherwise
     * they must point to arrays long enough for all the derivatives of that order; i.e., 2 for
     * the first derivatives, 3 for the second, etc.
     * <br><br>
     * @param para
     * parameter.
     * @param pt
     * pt on surface at parameter.
     * @param vec_array
     * ptr array to vector array.
     * @param number
     * Number of derivatives required.
     * @param loc
     * the evaluation location.
     */
    virtual int evaluate(
        SPApar_pos const& para,
        SPAposition& pt,
        SPAvector** vec_array = NULL,
        int                       number = 0,
        evaluate_surface_quadrant loc = evaluate_surface_unknown
    ) const;
    /**
     * Classification routine that checks whether the cosine angle is essentially zero (within <tt>SPAresnor</tt>).
     * <br><br>
     * <b>Role:</b> This is a criterion for treating a cone as completely planar, although this
     * should never occur.
     */
    logical flat() const { return fabs(cosine_angle) < SPAresnor; }

    /**
     * @nodoc
     */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    /**
     * Determines the apex of the cone if it is not a cylinder.
     */
    virtual SPAposition get_apex() const;
    /**
     * Returns the sweep path for a cone. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const;
    /**
     * Returns the <i>v</i> parameter line sweep information for the cone.  The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     * <br><br>
     * @param param
     * parameter.
     */
    virtual curve* get_profile(double param) const { return v_param_line(param); }
    /**
     * Returns the rail law for the sweep path for a cone.
     */
    virtual law* get_rail() const;
    /**
     * Returns <tt>TRUE</tt> if the cosine angle is negative.
     */
    logical hollow() const { return cosine_angle < 0; }
    /**
     * Indicates whether the parameter coordinate system of the surface is right or left-handed.
     * <br><br>
     * <b>Role:</b> A convex cone has a left-handed coordinate system if <i>reverse-u</i> is <tt>FALSE</tt>,
     * right-handed if it is <tt>TRUE</tt>. The converse is <tt>TRUE</tt> for a hollow curve.
     */
    virtual logical left_handed_uv() const;
    /**
     * Makes a copy of this cone on the heap, and returns a pointer to it.
     */
    virtual surface* make_copy() const;
    /**
     * Returns a cone bounding the normal direction of a curve.
     * <br><br>
     * <b>Role:</b> The cone is deemed to have its apex at the origin, and has a given
     * axis direction and (positive) half-angle. If the logical argument is <tt>TRUE</tt>,
     * then a quick approximation may be found. The approximate result may lie completely
     * inside or outside the guaranteed bound (obtained with a <tt>FALSE</tt> argument),
     * but may not cross from inside to outside. Flags in the returned object indicate
     * whether the cone is in fact the best available, and if not whether this result
     * is inside or outside the best cone.
     * <br><br>
     * @param para
     * parameter bounds.
     * @param approx
     * approximation ok?
     * @param trans
     * transformation.
     */
    virtual surf_normcone normal_cone(
        SPApar_box const& para,
        logical          approx = FALSE,
        SPAtransf const& trans = *(SPAtransf*)NULL_REF
    ) const;
    /**
     * Finds the parameter values of a point on a surface, given an optional first guess.
     * <br><br>
     * @param name
     * position name.
     * @param posi
     * parameter position.
     */
    virtual SPApar_pos param(
        SPAposition const& name,
        SPApar_pos  const& posi = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Returns the period of a periodic parametric surface.
     * <br><br>
     * <b>Role:</b> Returns 0 if the surface is not periodic in the <i>u</i>-parameter or not parametric.
     * For a cone the <i>u</i>-parameter is nonperiodic.
     */
    virtual double param_period_u() const;
    /**
     * Returns the period of a periodic parametric surface.
     * <br><br>
     * <b>Role:</b> Returns 0 if the surface is not periodic in the <i>v</i>-parameter or not parametric.
     * For a cone the <i>v</i>-parameter always has period <tt> 2 * p2 </tt>
     */
    virtual double param_period_v() const;
    /**
     * Returns the principal parameter range of a surface.
     * <br><br>
     * <b>Role:</b> If a box is provided the parameter range may be restricted to a portion
     * of the surface that is guaranteed to contain all portions of the surface that lie within
     * the region of interest. If none is provided and the parameter range in some direction is
     * unbounded then an empty interval is returned.
     * <br><br>
     * @param box
     * bounding box.
     */
    virtual SPApar_box param_range(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the <i>u</i>-parameter direction.
     * <br><br>
     * @param box
     * bounding box.
     */
    virtual SPAinterval param_range_u(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Returns the principal parameter range of a surface in the <i>v</i>-parameter direction.
     * <br><br>
     * @param box
     * bounding box.
     */
    virtual SPAinterval param_range_v(
        SPAbox const& box = *(SPAbox*)NULL_REF
    ) const;
    /**
     * Finds the rate of change in surface parameter corresponding to a unit velocity in a given object-space direction at a given position in parameter space.
     * <br><br>
     * @param dirc
     * direction.
     * @param posi
     * parameter position.
     */
    virtual SPApar_vec param_unitvec(
        SPAunit_vector const& dirc,
        SPApar_pos     const& posi
    ) const;
    /**
     * Determines if a cone is parametric and returns <tt>FALSE</tt>.
     * <br><br>
     * <b>Role:</b> A cone is not considered to be parameterized, as surface properties are
     * easy to find in object space. Wherever a point evaluator has a <tt>SPApar_pos</tt> argument,
     * this is ignored, so would normally be <tt>NULL</tt> or defaulted.
     */
    virtual logical parametric() const;
    /**
     * Reports whether the surface is periodic in the <i>u</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> It is smoothly closed, so faces can run over the seam.
     */
    virtual logical periodic_u() const;
    /**
     * Reports whether the surface is periodic in the <i>u</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> It is smoothly closed, so faces can run over the seam. A cone is periodic
     * in the <i>v</i>-direction, not in the <i>u</i>.
     */
    virtual logical periodic_v() const;
    /**
     * Returns normal at point on cone.
     * <br><br>
     * @param posi
     * position.
     * @param para
     * parameter.
     */
    virtual SPAunit_vector point_normal(
        SPAposition const& posi,
        SPApar_pos const& para = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Finds an outward direction from the surface at a point on the surface.
     * <br><br>
     * <b>Role:</b> This will usually be the normal, but if the point is the apex of the cone,
     * this routine still returns an outward direction, being the (positive or negative) axis direction.
     * <br><br>
     * @param posi
     * position.
     * @param para
     * parameter.
     */
    virtual SPAunit_vector point_outdir(
        SPAposition const& posi,
        SPApar_pos const& para = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Finds the foot of the perpendicular.
     * <br><br>
     * <b>Role:</b> Finds the point on the surface nearest to the given point.
     * Optionally, finds the normal to and principal curvatures of the surface at that point.
     * If the surface is parametric, returns the parameter values at the found point.
     * <br><br>
     * @param pt
     * point.
     * @param foot
     * foot.
     * @param vect
     * vector.
     * @param curv
     * curvature.
     * @param param_guess
     * param guess.
     * @param para
     * actual param.
     * @param f_weak
     * weak flag.
     */
    virtual void point_perp(
        SPAposition const& pt,
        SPAposition& foot,
        SPAunit_vector& vect,
        surf_princurv& curv,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& para = *(SPApar_pos*)NULL_REF,
        logical           f_weak = FALSE
    ) const;
    /**
     * Finds the point on the surface nearest to the given point.
     * <br><br>
     * <b>Role:</b> Optionally, finds the normal to and principal curvatures of the surface at
     * that point. If the surface is parametric, returns the parameter values at the found point.
     * <br><br>
     * @param pos
     * point.
     * @param foot
     * foot.
     * @param norm
     * direction.
     * @param param_guess
     * param guess.
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
        logical           f_weak = FALSE
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
     * Find the point on the surface nearest to the given point and optionally the normal to and principal curvatures of the surface at that point.
     * <br><br>
     * @param pos
     * point.
     * @param foot
     * foot.
     * @param param_guess
     * param guess.
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
     * Find the principal axes of curvature of the surface at a given point, and the curvatures in those directions.
     * <br><br>
     * @param point
     * point.
     * @param dir
     * first axis dir.
     * @param curv
     * curvature in first dir.
     * @param dir1
     * second axis dir.
     * @param curv1
     * curvature in second direction.
     * @param para
     * parameter guess.
     */
    virtual void point_prin_curv(
        SPAposition const& point,
        SPAunit_vector& dir,
        double& curv,
        SPAunit_vector& dir1,
        double& curv1,
        SPApar_pos const& para = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Finds the principal axes of the curvature of the surface at a given point.
     * <br><br>
     * @param pos
     * position.
     * @param param_guess
     * parameter guess.
     */
    surf_princurv point_prin_curv(
        SPAposition const& pos,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF
    ) const
    {
        return surface::point_prin_curv(pos, param_guess);
    }

    /**
     * Returns the inverse of the cone, with opposite normal.
     */
    cone operator-() const;
    /**
     * Tests two surfaces for equality.
     * <br><br>
     * <b>Role:</b> It is not guaranteed to say <i>equal</i> for effectively-equal surfaces,
     * but it is guaranteed to say <i>not equal</i> if they are not equal.
     * <br><br>
     * @param s_name
     * surface name.
     */
    virtual logical operator==(surface const& s_name) const;
    /**
     * Returns <tt>TRUE</tt> if the sine angle is negative.
     * <br><br>
     * <b>Role:</b> This function is often used in determining senses of intersections. Returns
     * <tt>TRUE</tt> if the surface normal is in the same general direction as the cone axis; i.e., their
     * dot product is positive, and <tt>FALSE</tt> if the normal and axis are in opposite directions.
     * Only really meaningful if <tt>cylinder</tt> returns <tt>FALSE</tt>, but consistent with the other functions
     * even in the cylinder case.
     */
    logical positive() const { return sine_angle < 0; }
    /**
     * Negates the cone.
     */
    virtual surface& negate();
    /**
     * @nodoc
     * Internal use
     */
    friend DECL_KERN cone operator*(
        cone const&,
        SPAtransf const&
        );
    /**
     * @nodoc
     * Internal use. Restore mechanism.
     */
    friend DECL_KERN surface* restore_cone();
    /**
     * Restores the data from a save file.
     * <br><br>
     * <b>Role:</b> The restore operation switches on a table defined by static instances
     * of the <tt>restore_su_def</tt> class. This invokes a simple friend function which constructs
     * an object of the right derived type. Then it calls the appropriate base class member
     * function to do the actual work.
     * <PRE>
     * cone::restore_data		Restore the information for the base cone
     * read_real				Sine of cone angle
     * read_real				Cosine of cone angle
     * if (restore_version_number &lt; CONE_SCALING_VERSION)
     * // the u parameter scale is obtained from the ellipse major axis
     * else
     * read_real			u parameter scale
     * if (restore_version_number &lt; SURFACE_VERSION)
     * // the reverse u flag is set to FALSE
     * else
     * read_logical			u parameter reversed, either "forward" or "reversed"
     * surface::restore_data		Generic surface data </PRE>
     */
    void restore_data();
    /**
     * Transforms this cone by the given transformation.
     * <br><br>
     * @param trans
     * transformation.
     */
    virtual surface& operator*=(SPAtransf const& trans);
    /**
     * Returns <tt>TRUE</tt> if the cosine angle is small (less than 0.1 in absolute value).
     * <br><br>
     * <b>Role:</b> This can be used as a warning of possible algorithmic problems because of a
     * large half-angle.
     */
    logical shallow() const { return fabs(cosine_angle) < 0.1; }
    /**
     * Saves the <tt>cone</tt>'s type and <tt>ellipse</tt> type, or <tt>cone_id</tt>, then calls <tt>cone::save_data</tt>.
     */
    virtual void save() const;
    /**
     * Save occurs as derived class switching goes through the normal virtual function mechanism.
     * <br><br>
     * <b>Role:</b> The <tt>save_data</tt> and <tt>restore_data</tt> function for each class can be called in
     * circumstances when we know what type of surface we are expecting and have one in our
     * hand to be filled in.
     */
    void save_data() const;
    /**
     * Reports whether the surface parameterization is singular at the <i>u</i>-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the nonconstant
     * parameter generates the same object-space point, and these can only occur at the ends
     * of the parameter range as returned by the functions above. A cone is singular for the
     * <i>u</i>-parameter corresponding to the apex, nonsingular for every other <i>u</i> value.
     * <br><br>
     * @param u_para
     * constant u-parameter.
     */
    virtual logical singular_u(
        double u_para
    ) const;
    /**
     * Reports whether the surface parameterization is singular at the <i>v</i>-parameter value.
     * <br><br>
     * <b>Role:</b> The only singularity recognized is where every value of the nonconstant
     * parameter generates the same object-space point, and these can only occur at the ends
     * of the parameter range as returned by the functions above. A cone is singular for the
     * <i>u</i>-parameter corresponding to the apex, nonsingular for every other <i>v</i> value.
     * <br><br>
     * @param v_para
     * constant v-parameter.
     */
    virtual logical singular_v(
        double	v_para
    ) const;
    /**
     * Divides a surface into separate pieces which are smooth (and therefore suitable for offsetting or blending).
     * <br><br>
     * <b>Role:</b> The surface is split if the curvature exceeds the minimum curvature argument.
     * If it is closed after this, it is then split into two. The functions return the number of
     * pieces. The split pieces are stored in pieces argument.
     * <br><br>
     * @param pieces
     * output pieces.
     * @param curvature
     * minimum curvature.
     */
    virtual int split_at_kinks_v(spline**& pieces, double curvature = 0.0) const;
    /**
     * Returns the sweep path type for a cone.
     */
    virtual sweep_path_type get_path_type() const { return straight_path_type; }
    /**
     * Tests if a point lies on the surface to user-supplied precision.
     * <br><br>
     * <b>Role:</b> Defaults to system precision (<tt>SPAresabs</tt>).
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
        double            tol = 0,
        SPApar_pos const& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& act_param = *(SPApar_pos*)NULL_REF
    ) const;
    /**
     * Returns type code for surface,that is <tt>cone_type</tt>.
     */
    virtual int type() const;
    /**
     * Returns the string <tt>"cone"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Tests for uninitialized cone.
     */
    virtual logical undef() const;
    /**
     * Construct a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>u</i>-parameter line runs in the direction of increasing <i>u</i>-parameter,
     * at constant <i>v</i>. The parameterization in the nonconstant direction matches that of
     * the surface, and has the range obtained by use of <tt>param_range_u</tt> or <tt>param_range_v</tt>
     * appropriately. The new curve is constructed in free store, so it is the responsibility
     * of the caller to ensure that it is correctly deleted.
     * <br><br>
     * @param u_para
     * constant u-parameter.
     */
    virtual curve* u_param_line(
        double	u_para
    ) const;
    /**
     * Construct a parameter line on the surface.
     * <br><br>
     * <b>Role:</b> A <i>v</i>-parameter line runs in the direction of increasing <i>v</i>, at constant <i>u</i>.
     * The parameterization in the nonconstant direction matches that of the surface, and
     * has the range obtained by use of <tt>param_range_u</tt> or <tt>param_range_v</tt>
     * appropriately. The new curve is constructed in free store, so it is the responsibility
     * of the caller to ensure that it is correctly deleted.
     * <br><br>
     * @param v_para
     * constant v-parameter.
     */
    virtual curve* v_param_line(
        double	v_para
    ) const;
#if defined D3_STANDALONE || defined D3_DEBUG
    /**
     * @nodoc
     * Internal use.
     */
    virtual void input(
        D3_istream&
    );
    /**
     * @nodoc
     * Internal use.
     */
    virtual void print(
        D3_ostream&
    ) const;

#endif
};
/** @} */
#endif
