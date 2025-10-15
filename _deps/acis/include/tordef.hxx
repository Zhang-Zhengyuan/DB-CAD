/* ORIGINAL: acis2.1/kerngeom/surface/tordef.hxx */
/* $Id: tordef.hxx,v 1.19 2002/08/09 17:15:27 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for torus. This is defined by its centre, the normal to
// the plane of its spine, the radius of the spine, and the radius of
// the circular cross-section. Special degenerate cases occur if the
// former radius is smaller in magnitude than the latter.


#if !defined( torus_CLASS )
#define torus_CLASS

#include <math.h>
#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "surdef.hxx"

#include "debugmsc.hxx"

#include "unitvec.hxx"
#include "position.hxx"

/**
* @file tordef.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

class curve;

// STI ROLL
class SizeAccumulator;
// STI ROLL

class SPAbox;
class SPAtransf;

// STI swa
class law;
// STI END

// Define an identifying type for this (lower-case) surface.

/**
 * @nodoc
 */
#define torus_type 4

class torus;

/**
 * Returns a transformed copy of a given torus.
 * <br><br>
 * @param tor
 * torus to be copied and transformed.
 * @param transf
 * transformation applied to the copy.
 */
DECL_KERN torus operator*(const torus& tor, const SPAtransf& transf);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN surface* restore_cone();

// Define torus proper.

class torus;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN torus operator*(const torus&, const SPAtransf&);

/**
 * Represents tori.
 * <br>
 * <b>Role:</b> A torus is defined by a circular spine and a circular cross-section
 * at each point on the spine. The spine of a torus is defined by a center point,
 * normal, and major radius. The circular cross-section is defined by a minor radius.
 * <br><br>
 * A normal torus (donut) is defined when the major radius is larger than the minor
 * radius. Special degenerate cases (lemon, vortex, and apple) occur if the major
 * radius is smaller than or equal to the minor radius. Two data members define the
 * parameterization of the torus:
 *<ul>
 *<li>
 * the member <tt>uv_oridir</tt> is the direction from the center of the torus to the origin
 * of parameter space. It is a unit vector.
 * </li>
 * <li> the member <tt>reverse_v</tt> is a flag indicating the direction of the constant
 * <i>u</i>-parameter lines, which are circles around the torus axis. The direction is
 * normally clockwise, but reversed if this flag is TRUE.
 *</li>
 *</ul>
 * The <i>u</i>-parameter is the latitude, with zero on the circle of greatest radius
 * about the torus axis, and the positive direction in the direction of the torus axis.
 * The <i>u</i>-parameter range depends upon the relative values of the major and minor
 * radii. For a doughnut, where the major radius is greater than the magnitude of the
 * minor, it runs from <tt>-pi</tt> to <tt>pi</tt>, and is periodic. For degenerate tori,
 * where the magnitude of the major axis is less than that of the minor, it runs from
 * <tt>-U</tt> to <tt>U</tt>, where:
 * <pre> U = arccos(-maj / |min|) </pre>
 * and the surface is singular at each end of the range. The <i>v</i>-parameter is the
 * longitude, running from <tt>-pi</tt> to <tt>pi</tt>, with 0 on the meridian
 * containing <tt>uv_oridir</tt>, and increasing in a clockwise direction around the
 * torus axis, unless <tt>reverse_v</tt> is <tt>TRUE</tt>, when it increases in a
 * counterclockwise direction.
 * <br><br>
 * Let <i>N</i> be the normal and <i>Q</i> be <tt>uv_oridir</tt>, and let <i>R</i> be
 * <i>N</i> x <i>Q</i>, negated if <tt>reverse_v</tt> is TRUE. Let <i>r</i> be the
 * absolute value of the minor radius. Then:
 * <pre> pos = center + r* sinu* N +
 *       (major_radius + (r* cosu)) * (cosv* Q + sinv R)</pre>
 * This parameterization is left-handed for a convex torus and right-handed for
 * a hollow one, if <tt>reverse_v</tt> is <tt>FALSE</tt>, and reversed if it is <tt>TRUE</tt>. When the
 * torus is transformed, the sense of <tt>reverse_v</tt> is inverted if the transform
 * includes a reflection. No special action is required for a negation.
 * <br><br>
 * In summary:
 *<ul>
 *<li>Tori are not true parametric surfaces.</li>
 * <li>Tori are closed in <i>v</i> but can or can not be closed in <i>u</i>.</li>
 *<li>Degenerate tori are not periodic in <i>u</i>; nondegenerate tori are periodic in
 * <i>u</i> (<tt>-pi</tt> to <tt>pi</tt> with period <tt>2pi</tt>).</li>
 * <li>All tori are periodic in <i>v</i> (with range <tt>-pi</tt> to <tt>pi</tt>, and period <tt>2pi</tt>).</li>
 * <li>Degenerate tori are singular in <i>u</i> at the poles (apices); all other values
 * of <i>u</i> and <i>v</i> are non-singular.</li>
 *</ul>
 * For images illustrating this class, look under Technical Articles of the Kernel Component at "Classes with Images"
 * <br><br>
 * @see torus, SPAposition, SPAunit_vector
 */
class DECL_KERN torus : public surface {

public:
    /**
     * The center of the circular spine.
     */
    SPAposition centre;		// The centre of the circular spine
    /**
     * The normal to the plane of the spine.
     */
    SPAunit_vector normal;		// Normal to the plane of the spine
    /**
     * The radius of the spine curve.
     * <br><br>
     * <b>Role:</b> This is normally positive, but it can be negative
     * (and smaller in magnitude than the minor radius) for the inner portion
     * (i.e., a lemon as opposed to an apple).
     */
    double major_radius;	// Radius of the spine, normally positive,
    // but negative (and smaller in magnitude
    // than the minor radius) for the inner
    // portion (i.e. a "lemon", as opposed to
    // an "apple");
/**
 * The absolute value of this value specifies the radius of a cross-sectional circle of
 * the ring.
 * <br><br>
 * <b>Role:</b> If this value is negative, then the torus is a void. If it is <tt>0</tt>,
 * then the torus is undefined.
 */
    double minor_radius;	// Radius of cross-sectional circle of
    // the ring, negated if torus is a void.
    // Exactly zero to indicate "undefined".
/**
 * The direction from the center toward the origin of parameter space.
 * <br><br>
 * <b>Role:</b> The <i>v</i>-parameter measures the azimuth angle about the normal and
 * so defines a cross-sectional circle. The <i>u</i>-parameter measures the evaluation
 * angle around that circle.
 */
    SPAunit_vector uv_oridir;	// Direction from the centre towards the
    // origin of SPAparameter space. The v
    // SPAparameter measures the azimuth angle
    // about "normal", and so defined a
    // cross-sectional circle. The u parameter
    // measures the angle around that circle -
    // zero means the outermost point, and the
    // positive direction at that point is
    // the same as the torus normal direction.
/**
 * Flag to measure the  <i>v</i>-parameter counterclockwise about the torus normal, rather
 * than clockwise.
 * <br><br>
 * <b>Role:</b> The <i>v</i>-parameter is normally measured clockwise about the torus normal.
 * If this logical is <tt>TRUE</tt>, it is to be measured counterclockwise.
 */
    logical reverse_v;		// The v parameter is normally measured
    // clockwise about the torus normal. If
    // this is TRUE, it is to be measured
    // anticlockwise.

// Permit uninitialised torus.
/**
 * Constructs a torus (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it.
 */
    torus();
    torus(const char* gme);


    // Construct a torus as a copy of another one
/**
 * Constructs a torus as a copy of an existing one (copy constructor).
 * <br><br>
 * @param tor
 * existing torus to copy.
 */
    torus(const torus& tor);
    torus(const char* gme, const torus& tor);


    // Construct a torus with given centre, normal and radii.
/**
 * Constructs a torus from the given center, normal and radii.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data supplied
 * as arguments.
 * <br><br>
 * @param center
 * center of the constructed torus.
 * @param normal
 * normal of the constructed torus.
 * @param major_radius
 * major radius of the constructed torus.
 * @param minor_radius
 * minor radius of the constructed torus.
 */
    torus(
        const SPAposition& center,
        const SPAunit_vector& normal,
        double major_radius,
        double minor_radius
    );
    torus(
        const char* gme,
        const SPAposition& center,
        const SPAunit_vector& normal,
        double major_radius,
        double minor_radius
    );

    // Destructor. This is virtual, so must be out-of-line.
/**
 * C++ destructor, deleting a torus.
 */
    ~torus();

    // Make a copy of this torus on the heap, and return a pointer
    // to it.
/**
 * @nodoc
 */
    virtual surface* make_copy() const; // documented in base class

    // Make a copy without any sharing of subdata.
/**
 * @nodoc
 */
    virtual surface* deep_copy(pointer_map* pm = NULL) const; // documented in base class

    // Return a torus being (a copy of) this torus negated (i.e. with
    // opposite normal).
/**
 * Returns a torus that is a negated copy of <tt>this</tt> torus.
 * <br><br>
 * <b>Role:</b> By negated, we mean that the new torus has the opposite normal.
 */
    torus operator-() const;


    // Test two surfaces for equality. This, like testing floating-
    // point numbers for equality, is not guaranteed to say "equal" for
    // effectively-equal surfaces, but is guaranteed to say "not equal"
    // if they are indeed not equal. The result can be used for
    // optimisation, but not where it really matters.
/**
 * Tests <tt>this</tt> surface for equality to another surface.
 * <br><br>
 * <b>Role:</b> This method does not guarantee equality for effectively-equal
 * surfaces, but it is guaranteed to determine inequality if the two surfaces
 * are not equal. Use this result for optimization.
 * <br><br>
 * @param surf
 * surface to compare for equality.
 */
    virtual logical operator==(const surface& surf) const;


    // Return a torus being (a copy of) this torus transformed by
    // the given SPAtransf.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend DECL_KERN torus operator*(
        const torus&,
        const SPAtransf&
        );


    // Transform this torus by the given SPAtransf.
/**
 * Transforms <tt>this</tt> torus by the given transformation.
 * <br><br>
 * @param transf
 * transformation to apply.
 */
    virtual surface& operator*=(
        const SPAtransf& transf
        );

    // Negate this torus.
/**
 * Negates <tt>this</tt> torus; ie, reverses the direction of the surface normal.
 */
    virtual surface& negate();

    // Indicate whether the torus is properly defined or not.
/**
 * Determines whether <tt>this</tt> torus is properly defined.
 */
    virtual logical undef() const;

    // Return a SPAbox around the surface. This need not be the smallest
    // SPAbox which contains the specified portion of the surface, but
    // needs to balance the tightness of the bound against the cost of
    // evaluation.

    // Surface bounded in SPAparameter space.
/**
 * @nodoc
 */
    virtual SPAbox bound(
        const SPApar_box& box = *(SPApar_box*)NULL_REF,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const; // documented in base class

    // Surface bounded in object space.
/**
 * @nodoc
 */
    virtual SPAbox bound(
        const SPAbox& box,
        const SPAtransf& transf = *(SPAtransf*)NULL_REF
    ) const; // documented in base class


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
 * Returns a cone bounding the normal direction of <tt>this</tt> surface.
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


    // Get the origin of coordinates, constructing one if necessary.
/**
 * Returns the origin of the coordinates for <tt>this</tt> torus, constructing one if necessary.
 */
    SPAunit_vector origin_dir() const;

    // Geometric enquiry routines - torus-specific.

    // Classify the shape according to the relationship between
    // the major and minor radii. Exactly one of the first four
    // functions should return TRUE for any given torus, though it
    // is just conceivable that numerical errors might mean that
    // apple() and vortex(), vortex() and doughnut(), or none of them,
    // would return TRUE when the major and minor radii differ by
    // exactly SPAresabs.
/**
 * Returns <tt>TRUE</tt> if <tt>this</tt> torus has a lemon shape.
 * <br><br>
 * <b>Role:</b> This is true if the major radius of the torus is less than or equal to <tt>0</tt>.
 */
    logical lemon() const {
        return major_radius <= 0;
    }
    /**
     * Returns <tt>TRUE</tt> if <tt>this</tt> torus has an apple shape.
     * <br><br>
     * <b>Role:</b> This is true if the major radius of the torus is greater than zero and if
     * the major radius is less than the absolute value of the minor radius minus SPAresabs:
     * <pre>  0 &lt; major_radius &lt;= (|minor_radius| - SPAresabs). </pre>
     */
    logical apple() const {
        return major_radius > 0 &&
            major_radius <= fabs(minor_radius) - SPAresabs;
    }
    /**
     * Returns <tt>TRUE</tt> if <tt>this</tt> torus has a vortex shape.
     * <br><br>
     * <b>Role:</b> This is true if the absolute value of the difference between the major radius
     * of the torus and the absolute value of the minor radius is less than <tt>SPAresabs</tt>:
     * <pre>  |(major_radius - |minor_radius|)| &lt; SPAresabs.</pre>
     */
    logical vortex() const {
        double min_rad = fabs(minor_radius);
        return fabs(major_radius - min_rad) < SPAresabs;
    }
    /**
     * Returns <tt>TRUE</tt> if <tt>this</tt> torus has a doughnut shape.
     * <br><br>
     * <b>Role:</b> This is true if the major radius of the torus is greater than or equal to
     * the absolute value of the minor radius plus <tt>SPAresabs</tt>:
     * <pre>  major_radius &gt;= (|minor_radius| + SPAresabs).</pre>
     */
    logical doughnut() const {
        return major_radius >= fabs(minor_radius) + SPAresabs;
    }
    /**
     * Returns <tt>TRUE</tt> if <tt>this</tt> torus is degenerate (it has apices).
     */
    logical degenerate() const {
        return !doughnut();
    }
    /**
     * Returns <tt>TRUE</tt> if the minor radius of <tt>this</tt> torus is less than zero.
     */
    logical hollow() const {
        return minor_radius < 0;
    }

    // Return the (positive) distance along the torus axis from the
    // centre to the apices, or 0 if the torus is not degenerate (i.e.
    // has no apices).
/**
 * Returns the positive distance along the axis of <tt>this</tt> torus, from the center
 * to the apices.
 * <br><br>
 * <b>Role:</b> This method returns <tt>0</tt> if the torus is nondegenerate (i.e., it has no
 * apices).
 */
    double apex_dist() const;

    // Geometric enquiry routines. Generic for all surfaces.

    // A torus is not considered to be parametrised, as surface
    // properties are easy to find in object space. Wherever a point
    // evaluator has a SPApar_pos argument, this is ignored, so
    // would normally be NULL or defaulted.
/**
 * Returns <tt>TRUE</tt> if <tt>this</tt> surface is a parametric surface - otherwise returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Role:</b> Tori are not considered to be parametric surfaces because
 * their surface properties are easy to find in object space.
 */
    virtual logical parametric() const;

    // Position-based enquiries, implemented for all surfaces, but
    // with an extra argument to speed up processing for parametric
    // ones by allowing SPAparameter values of a point near the
    // desired point, to be given if known.

    // Find the normal to the surface at a point on the surface
    // nearest to the given point.
/**
 * Finds the normal to <tt>this</tt> surface at a point on the surface nearest to the
 * given position.
 * <br><br>
 * <b>Role:</b> The second argument is not used in the torus implementation of this function.
 * <br><br>
 * @param pos
 * the given position.
 * @param unused
 * parameter guess (unused).
 */
    virtual SPAunit_vector point_normal(
        const SPAposition& pos,
        const SPApar_pos& unused = *(SPApar_pos*)NULL_REF
    ) const;


    // Find an outward direction from the surface at a point with
    // given SPAparameter values. This will usually be the normal, but
    // if the nearest point is an apex of a degenerate torus, this
    // routine will still return an outward direction.
/**
 * Finds the outward direction from <tt>this</tt> surface at a point nearest the given
 * position.
 * <br><br>
 * <b>Role:</b> This method usually returns the normal, but if the nearest point is an
 * apex of a degenerate torus, this method still returns an outward direction. The second
 * argument is not used in the torus implementation of this function.
 * <br><br>
 * @param pos
 * given position.
 * @param unused
 * parameter guess (unused).
 */
    virtual SPAunit_vector point_outdir(
        const SPAposition& pos,
        const SPApar_pos& unused = *(SPApar_pos*)NULL_REF
    ) const;


    // Find the principal axes of curvature of the surface at a
    // given point, and the curvatures in those directions.
/**
 * Finds the principal axes of curvature of <tt>this</tt> surface at a given point,
 * and the curvatures along these axes.
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
    ) const;
    /**
     * Finds the principal axes of curvature of <tt>this</tt> surface at a given point and
     * the curvatures in those directions, in a structure defined for the purpose.
     * <br><br>
     * @param pos
     * position at which to evaluate the curvature.
     * @param param_guess
     * parameter guess
     */
    surf_princurv point_prin_curv(
        const SPAposition& pos,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF
    ) const
    {
        return surface::point_prin_curv(pos, param_guess);
    }


    // Find the curvature of a cross-section of the cone.
    // The section is given by the intersection of the cone with a
    // plane passing through the given point and with given normal,
    // which must be tangential to the cone at that point.

/* The generic surface function, which uses point_prin_curv(), is
    quite adequate for us, as there is no easy solution for toruses.

    virtual double point_cross(
                SPAposition const &,			// point
                SPAunit_vector const &,		// normal
                SPApar_pos const & = *(SPApar_pos *)NULL_REF
            ) const;
*/


// Find the point on the surface nearest to the given point and
// optionally the normal to and principal curvatures of the
// surface at that point.  If the surface is parametric, also
// return the SPAparameter values at the found point.
/**
 * Finds the point on <tt>this</tt> surface nearest to the given point and, optionally,
 * the normal to and the principal curvatures of the surface at that point.
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
        SPAunit_vector& ppnormal,
        surf_princurv& cur,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const;
    /**
     * Finds the point on <tt>this</tt> surface nearest to the given point and, optionally,
     * the normal to the surface at that point.
     * <br><br>
     * <b>Role:</b> If the surface is parametric, this method also returns the parameter
     * values at the found point.
     * <br><br>
     * @param pos
     * the input position.
     * @param foot
     * the position on the surface.
     * @param ppnormal
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
        SPAunit_vector& ppnormal,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& param_actual = *(SPApar_pos*)NULL_REF,
        logical f_weak = FALSE
    ) const
    {
        point_perp(
            pos,
            foot,
            ppnormal,
            *(surf_princurv*)NULL_REF,
            param_guess,
            param_actual, f_weak
        );
    }
    /**
     * Finds the point on <tt>this</tt> surface nearest to the given point.
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
            param_actual,
            f_weak
        );
    }


    // Now there is a set of SPAparameter-based functions. The Acis
    // kernel only requires them to have defined results if the
    // surface is parametric (i.e. the member function parametric()
    // returns TRUE), but husks and applications may expect valid
    // results for all surface types.

    // The parametrisation implemented is similar to the conventional
    // latitude/longitude angles on a sphere. The direction
    // "uv_oridir" gives the zero meridian, and the zero latitude is
    // the outermost circle. The u parameter is the angle between the
    // line from the torus spine to the test point and the equatorial
    // plane, negative in the southern hemisphere, positive in the
    // northern (where the torus normal points "north"). It thus runs
    // from -pi to pi for a non-degenerate torus, and for a smaller
    // though still symmetric range for a degenerate one. The v
    // SPAparameter is the azimuth angle, between -pi and pi. The
    // positive v direction is clockwise around pole_dir unless
    // reverse_v is TRUE, in which case it is anticlockwise.

    // Find the SPAparameter values of a point on a surface, given an
    // optional first guess.
/**
 * Finds the parameter position of a given point on <tt>this</tt> surface, given an
 * optional first guess.
 * <br><br>
 * @param pos
 * the position for which the parameter position is to be found.
 * @param param_guess
 * parameter guess.
 */
    virtual SPApar_pos param(
        const SPAposition& pos,
        const SPApar_pos& param_guess = *(SPApar_pos*)NULL_REF
    ) const;

    // Find the rate of change in surface SPAparameter corresponding to
    // a unit velocity in a given object-space direction at a given
    // SPAposition in SPAparameter space.
/**
 * Finds the rate of change in the surface parameter corresponding to a unit velocity
 * in a given object-space direction, at a given position in parameter space.
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


    // Find the point on a parametric surface with given SPAparameter
    // values, and optionally the first and second derivatives as
    // well or instead.
/**
 * Finds the position on a parametric surface corresponding to the given parameter
 * values, and optionally the first and second derivatives as well.
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
    // values. The default, using eval(), is fine here.

//	virtual SPAposition eval_position(
//				SPApar_pos const &
//			) const;


    // Find the normal to a parametric surface at a point with given
    // SPAparameter values.
/**
 * Finds the normal to a parametric surface at the point with the given parameter position.
 * <br><br>
 * @param pos
 * parameter position at which to evaluate the surface normal.
 */
    virtual SPAunit_vector eval_normal(const SPApar_pos& par_pos) const;


    // Find the principal axes of curvature of the surface at a
    // point with given SPAparameter values, and the curvatures in those
    // directions.
/**
 * Finds the principal axes of curvature of <tt>this</tt> surface at the point with the
 * given parameter position, and the curvatures in those directions.
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
        double& cur2		// curvature in second direction
    ) const;
    /**
     * Finds the principal axes of curvature of <tt>this</tt> surface at the point with the
     * given parameter position and the curvatures in those directions, in a structure
     * defined for the purpose.
     * <br><br>
     * @param pos
     * parameter position at which to evaluate the curvature.
     */
    surf_princurv eval_prin_curv(const SPApar_pos& pos) const
    {
        return surface::eval_prin_curv(pos);
    }


    // Find the curvature of a cross-section curve of the parametric
    // surface at the point with given SPAparameter values.  The
    // cross-section curve is given by the intersection of the
    // surface with a plane passing through the point and with given
    // normal.

/* The generic surface function, which uses eval_prin_curv(), is
    quite adequate for us, as there is no easy solution for toruses.

    virtual double eval_cross(
                SPApar_pos const &,
                SPAunit_vector const &
            ) const;
*/


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
 * Calculates derivatives, of any order up to the number requested, and stores them in
 * vectors provided by the user.
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


    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.
/**
 * Returns the number of derivatives of <tt>this</tt> surface that can be found accurately
 * and relatively directly.
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


    // Report whether the surface is periodic in either SPAparameter
    // direction (i.e. it is smoothly closed, so faces can run over
    // the seam). A torus is always periodic in the v direction, and
    // is also periodic in the u direction if it is non-degenerate.
/**
 * Determines if <tt>this</tt> surface is periodic in the <i>u</i>-parameter direction
 * (i.e., if it is smoothly closed, so that faces can run over the seam).
 * <br><br>
 * <b>Role:</b> A torus is periodic in the <i>u</i>-direction if it is nondegenerate.
 */
    virtual logical periodic_u() const;
    /**
     * Determines if <tt>this</tt> surface is periodic in the <i>v</i>-parameter direction
     * (i.e., if it is smoothly closed, so that faces can run over the seam).
     * <br><br>
     * <b>Role:</b> A torus is always periodic in the <i>v</i>-direction.
     */
    virtual logical periodic_v() const;


    // Report whether the surface is closed, smoothly or not, in
    // either SPAparameter direction. A torus is closed in the v
    // direction, and may or may not be closed in the u direction.
/**
 * Determines whether <tt>this</tt> surface is closed, smoothly or not, in the
 * <i>u</i>-parameter direction.
 * <br><br>
 * <b>Role:</b> A torus may or may not be closed in the <i>u</i>-direction.
 */
    virtual logical closed_u() const;
    /**
     * Determines whether <tt>this</tt> surface is closed, smoothly or not, in the
     * <i>v</i>-parameter direction.
     * <br><br>
     * <b>Role:</b> A torus is closed in the <i>v</i>-direction.
     */
    virtual logical closed_v() const;


    // Return the period of a periodic parametric surface, zero if
    // the surface is not periodic in the chosen SPAparameter or not
    // parametric. If the torus is periodic in the u direction its
    // period is 2 * pi, and it always has a period of 2 * pi in the
    // v direction.
/**
 * Returns the period, in <i>u</i>, of a periodic parametric surface.
 * <br><br>
 * <b>Role:</b> If the surface is not parametric, or is not periodic in the
 * <i>u</i>-direction, this method returns 0. If the torus is periodic in the
 * <i>u</i>-direction, (i.e., it is a doughnut) its period is <tt>2pi</tt>.
 */
    virtual double param_period_u() const;
    /**
     * Returns the period, in <i>v</i>, of a periodic parametric surface.
     * <br><br>
     * <b>Role:</b> If the surface is not parametric, or is not periodic in the
     * <i>v</i>-direction, this method returns 0. A torus always has a period of <tt>2pi</tt>
     * in the <i>v</i>-direction.
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
 * Returns the principal parameter range of <tt>this</tt> surface.
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
    // the specified u or v parameter value. The only singularity
    // recognised is where every value of the non-constant SPAparameter
    // generates the same object-space point, and these can only
    // occur at the ends of the SPAparameter range as returned by
    // the functions above. A degenerate torus is singular in the u
    // direction at the poles.
/**
 * Determines whether the parameterization of <tt>this</tt> surface is singular at
 * the specified <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> The only singularity recognized is where every value of the
 * nonconstant parameter generates the same object-space point, and these can
 * only occur at the ends of the parameter range.  A degenerate torus is singular
 * in the <i>u</i>-direction at the poles.
 * <br><br>
 * @param uparam
 * u-parameter value to test.
 */
    virtual logical singular_u(
        double	uparam	// constant u parameter
    ) const;
    /**
     * Determines whether the parameterization of <tt>this</tt> surface is singular at
     * the specified <i>v</i>-parameter value.
     * <br><br>
     * <b>Role:</b> A degenerate torus is not singular in the <i>v</i>-direction.
     * <br><br>
     * @param vparam
     * v-parameter value to test.
     */
    virtual logical singular_v(
        double	vparam	// constant v parameter
    ) const;


    // Indicate whether the SPAparameter coordinate system of the surface
    // is right- or left-handed. With a right-handed system, at any
    // point the outward normal is given by the cross product of the
    // increasing u direction with the increasing v direction, in that
    // order. With a left-handed system the outward normal is in the
    // opposite direction from this cross product.
/**
 * Indicates whether the parameter coordinate system of <tt>this</tt> surface is
 * right-handed or left-handed.
 * <br><br>
 * <b>Role:</b> With a right-handed system, the outward normal at any point is given
 * by the cross product of the increasing <i>u</i>-direction with the increasing
 * <i>v</i>-direction, in that order. With a left-handed system, the outward normal is
 * in the opposite direction from this cross product.
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
 * Constructs a parameter line on <tt>this</tt> surface.
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
        double	vparam		// constant v parameter
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
        double		uparam	// constant u parameter
    ) const;


    // Test whether a point lies on the surface to given
    // tolerance.
/**
 * Determines whether a point lies on <tt>this</tt> surface to the given tolerance.
 * <br><br>
 * <b>Role:</b> Determines whether a point lies on the surface to the given tolerance.
 * The last two arguments are not used in the torus implementation of this function.
 * <br><br>
 * @param pos
 * position to test.
 * @param tol
 * tolerance to use (defaults to SPAresabs).
 * @param uv_guess
 * parameter guess (unused).
 * @param uv_actual
 * actual parameter (unused).
 */
    virtual logical test_point_tol(
        const SPAposition& pos,
        double tol = 0,
        const SPApar_pos& uv_guess = *(SPApar_pos*)NULL_REF,
        SPApar_pos& uv_actual = *(SPApar_pos*)NULL_REF
    ) const;


    // Return type of (lower-case) surface.
/**
 * Returns the type of torus.
 */
    virtual int type() const;

    // Return string identifier of surface.
/**
 * Returns the string "torus".
 */
    virtual const char* type_name() const;


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
 * Saves the type of id, then calls <tt>save_data</tt>.
 */
    virtual void save() const;
    /**
     * Save the information for <tt>this</tt> torus to a save file.
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
     * Restores the data for <tt>this</tt> torus from a save file.
     * <br><br>
     * <b>Role:</b>
     * <pre>
     *    read_position				Center of torus
     *    read_unit_vector				Normal vector
     *    read_real					major radius
     *    read_real					minor radius
     *    if (restore_version_number &lt; SURFACE_VERSION)  // Old style - that is all to read.
     *    else
     *       read_unit_vector				uv origin direction
     *       read_logical				Either "forward_v" or "reversed_v"
     *    surface::restore_data			Remaining surface data </pre>
     */
    void restore_data();

#if defined D3_STANDALONE || defined D3_DEBUG
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

    // Print out details of torus.
/**
 * @nodoc
 */
    virtual void debug(
        const char* str,
        FILE* ptr = debug_file_ptr
    ) const; // documented in base class

    // STI swa 27Jul98 -- functions to determine torus sweep information
public:
    /**
     * Returns the sweep path type for <tt>this</tt> surface.
     */
    virtual sweep_path_type get_path_type() const { return circular_path_type; }
    /**
     * Returns the sweep path for <tt>this</tt> surface. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve* get_path() const;
    /**
     * Returns the sweep information for <tt>this</tt> surface. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     * <br><br>
     * @param param
     * parameter at which to get the profile.
     */
    virtual curve* get_profile(double param) const;
    /**
     * Returns the rail law for <tt>this</tt> swept surface.
     */
    virtual law* get_rail() const;
    // STI swa END

        // STI ROLL
    /**
    * @nodoc
    */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
};

/** @} */
#endif
