/* ORIGINAL: acis2.1/spline/bs3_crv/sp3crtn.hxx */
// $Id: bs3cutil.hxx,v 1.5 2002/08/09 17:19:06 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Routines to fit object-space curves to intersections of surfaces.

#if !defined( BS3_CURVE_UTIL )
#define BS3_CURVE_UTIL

#include "dcl_intr.h"
#include "bs3curve.hxx"
#include "base.hxx"

/**
* @file bs3cutil.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class curve_surf_int;
class SPAbox;
class curve;
class surface;
class plane;
class cone;
class sphere;
class torus;
class spline;


// Intersect a spline with a plane

/** \addtogroup INTRDIRECTBS3CURVEINT
 *  \brief Declared at <bs3cutil.hxx>, SPAintr
 *  @{
 */

/**
* Intersects the given parametric curve with a plane.
* <br><br>
* <b>Role:</b> Intersects the given parametric curve with a plane and returns
* an ordered list of intersection points together with information about
* relationships between the curve and surface in the neighborhood of each
* intersection point.
* <br><br>
* Tangency and extended regions of coincidence are related to the given tolerance,
* and not standard system tolerance, except when the given tolerance is less than
* standard.
* <br><br>
* The information returned for each intersection point is described fully in the
* documentation of ACIS curve-surface intersections, but a brief description
* follows:
* <br><br>
* <tt>int_point</tt>, the intersection point, is an object-space position. At a tangency,
* where the "true" intersection point is not well-defined, this point will be the
* average of the points of minimum (or maximum if the curve penetrates the
* surface) distance. For a region of coincidence, the ends of that region are
* given, as two intersections.
* <br><br>
* <tt>param</tt>, the parameter value of <tt>int_point</tt> on the curve.
* <br><br>
* <tt>surf_param</tt>, if the surface is parametric, this is the parameter values on the
* surface of <tt>int_point</tt>. If not, it is undefined.
* <br><br>
* <tt>low_rel</tt>, <tt>high_rel</tt>, is the relationship between the curve and the surface in the
* neighborhood of the intersection, on the side with the lower curve parameter and
* the side with higher curve parameter. Classifications are in and out for normal
* crossings, in-tangent and out-tangent (for tangency) and coincident.
* <br><br>
* <tt>fuzzy</tt>, <tt>low_param</tt>, <tt>high_param</tt>, if the region of uncertainty in the intersection
* is significantly higher than tolerance (at a tangency or very low-angle
* crossing) the range of curve parameter values over which the curve is entirely
* within tolerance of the surface are given as (<tt>low_param</tt>, <tt>high_param)</tt>, and <tt>fuzzy</tt>
* is set TRUE. At well-defined intersections, <tt>fuzzy</tt> is FALSE and both <tt>low_param</tt>
* and <tt>high_param</tt> will be equal to param.
* <br><br>
* Intersection points outside the region of interest need not be evaluated, but do
* no harm to ACIS if they are.
* <br><br>
* A special case arises if the curve is everywhere coincident with the surface
* (within the region of interest). If so, a single intersection point will be
* returned at an arbitrary position on the curve, with <tt>low_rel</tt> and <tt>high_rel</tt> both
* set to coincident.
* <br><br>
* ACIS makes an assumption for tangent intersections that <tt>low_param</tt> and <tt>high_param</tt>
* are symmetric about param.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param bs
* given curve.
* @param pla
* given plane.
* @param fitol
* fit tolerance.
* @param region_of_interest
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_pla_int(
			bs3_curve bs,			// given curve
			plane const &pla,		// given plane
			double fitol,				// given fit tolerance
			SPAbox const &region_of_interest,			// region of interest
			double const epsilon = SPAresabs
		);


// Intersect a spline with a cone

/**
* Intersects the given parametric curve with a cone.
* <br><br>
* <b>Role:</b> In all other respects, the definition of this routine is the
* same as that of <tt>bs3_curve_pla_int</tt>.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param bs
* given curve.
* @param con
* given cone.
* @param fitol
* fit tolerance.
* @param region_of_interest
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_con_int(
			bs3_curve bs,			// given curve
			cone const & con,		// given cone
			double fitol,				// given fit tolerance
			SPAbox const &region_of_interest,			// region of interest
			double const epsilon = SPAresabs
		);


// Intersect a spline with a sphere

/**
* Intersects the given parametric curve with a sphere.
* <br><br>
* <b>Role:</b> In all other respects, the definition of this routine is the
* same as that of <tt>bs3_curve_pla_int</tt>.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param bs
* given curve.
* @param sph
* given sphere.
* @param fitol
* fit tolerance.
* @param region_of_interest
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_sph_int(
			bs3_curve bs,			// given curve
			sphere const & sph,		// given sphere
			double fitol,				// given fit tolerance
			SPAbox const & region_of_interest,			// region of interest
			double const epsilon = SPAresabs
		);


// Intersect a spline with a torus

/**
* Intersects the given parametric curve with a torus.
* <br><br>
* <b>Role:</b> In all other respects, the definition of this routine is the
* same as that of <tt>bs3_curve_pla_int</tt>.
* <br><br>
* The standard version of this function as supplied is not dependent on the
* underlying surface package, in that it uses only ACIS kernel facilities together
* with other <tt>bs3_curve</tt> functions. Re-implementation is optional for a different
* surface package.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param bs
* given curve.
* @param tor
* given torus.
* @param fitol
* fit tolerance.
* @param region_of_interest
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_tor_int(
			bs3_curve bs,			// given curve
			torus const & tor,		// given torus
			double fitol,				// given fit tolerance
			SPAbox const & region_of_interest,			// region of interest
			double const epsilon = SPAresabs
		);


// Intersect a spline with a general (analytic) surface.

/**
* Intersects a spline with a general (analytic) surface.
* <br><br>
* <b>Role</b>: This should not be used for a spline surface.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param cur
* bs3_curve.
* @param sur
* surface.
* @param fitol
* tolerance.
* @param region_of_interest
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_ana_int(
			bs3_curve cur,			// given curve
			surface const &sur,	// given surface
			double fitol,				// given fit tolerance
			SPAbox const &region_of_interest,			// region of interest
			double const epsilon = SPAresabs
		);

// Intersect a spline with a spline surface

/**
* Intersects the given parametric curve with a spline surface.
* <br><br>
* <b>Role</b>: In all other respects, the definition of this routine is the
* same as that of <tt>bs3_curve_pla_int</tt>.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param bs_cur
* given curve.
* @param spl
* given spline surface.
* @param fitol
* fit tolerance.
* @param b
* region of interest.
* @param epsilon
* intersection tolerance
**/
DECL_INTR curve_surf_int *
bs3_curve_spl_int(
            bs3_curve bs_cur,          // given curve
            spline const &spl,     // given spline surface
            double fitol,             // given fit tolerance (ignored)
            SPAbox const &b,         // region of interest
			double const epsilon = SPAresabs
        );

// Routine to project the given bs3_curve along the given path on to
// the given plane.  This routine is very simple minded, it takes the
// input curve and projects the control polygon on to the plane along the
// given curve and constructs the control polygon for the result.  The
// result is the curve construted from the control polygon so obtained.
/**
* Projects the given <tt>bs3_curve</tt> along the given path onto the given plane.
* <br><br>
* <b>Role</b>: This routine takes the input curve and projects the control
* polygon onto the plane along the given curve and creates the control polygon for
* the result.
* <br><br>
* <b>Effect:</b> Changes Model<br><br>
* @param in_cur
* given curve.
* @param path_c
* path curve along which in_cur is projected.
* @param to_surf
* plane onto which in_cur is projected.
**/
DECL_INTR bs3_curve
bs3_curve_project(
		bs3_curve in_cur,
		const curve *path_c,
		const plane &to_surf
	);

/** @} */

#endif
