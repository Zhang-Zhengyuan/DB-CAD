/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                  Geometry utility procedures
/*******************************************************************/
#ifndef geom_utl_hxx
#define geom_utl_hxx
#include "dcl_kern.h"
#include <math.h>
#include "logical.h"
#include "interval.hxx"
#include "vector_utils.hxx"
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class pick_ray;
/*******************************************************************/
/**
 * @file geom_utl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
#ifndef M_PI
   #define M_PI 3.14159265358979323846
#endif


/**
* Gets the distance from a point to a line, and the parameter along the line of the closest approach.
* <br><br>
* <b>Role:</b> This function calculates a perpendicular distance to a line
* defined by a point and a vector. The vector need not be normalized.
* The function returns the distance from the point to the line, and the
* parameter along the line.
* <br><br>
* <b>Errors:</b> input <tt>line_vec</tt> is zero
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pt
* position to project.
* @param line_pt
* position on line.
* @param line_vec
* direction of line.
* @param dist
* distance returned.
* @param line_prm
* line parameter returned.
**/
DECL_KERN logical dist_pt_to_line(
		SPAposition const   &pt,
		SPAposition const   &line_pt,
		SPAvector const     &line_vec,
		double              &dist,
		double              &line_prm
	);

DECL_KERN logical gme_dist_pt_to_line(SPAposition const& pt, SPAposition const& line_pt, SPAvector const& line_vec, double& dist, double& line_prm);

/**
* Projects a <tt>SPAposition</tt> onto a <tt>line</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pt
* position to project.
* @param line_pt
* position on line.
* @param line_dir
* direction of line.
**/
DECL_KERN SPAposition proj_pt_to_line(
    const SPAposition    &pt,
    const SPAposition    &line_pt,
    const SPAunit_vector &line_dir
    );

DECL_KERN SPAposition gme_proj_pt_to_line(
    const SPAposition    &pt,
    const SPAposition    &line_pt,
    const SPAunit_vector &line_dir
    );

/**
* Projects a <tt>SPAposition</tt> onto a <tt>plane</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pt
* position to project.
* @param c
* position on plane.
* @param n
* plane normal.
**/
DECL_KERN SPAposition proj_pt_to_plane(
    const SPAposition    &pt,
    const SPAposition    &c,
    const SPAunit_vector &n
    );

DECL_KERN SPAposition gme_proj_pt_to_plane(
    const SPAposition    &pt,
    const SPAposition    &c,
    const SPAunit_vector &n
    );
/**
* Calculates the angle between two unit vectors.
* <br><br>
* <b>Role:</b> Gets the angle (in radians) between two vectors or two unit vectors in the
* range 0 &lt;= angle &lt; 2 <tt>pi</tt>. This function is overloaded. This version accepts two
* unit vectors as arguments.
* <br><br>
* The third vector is the plane in which the angle is measured, and is required.
* It also defines a direction, from the first to the second unit vector. To simply get
* the angle in 3-space, pass the normalized cross product of the two input unit vectors
* as the third vector.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param z
* normal to plane.
**/
DECL_KERN double angle_between(
    const SPAunit_vector& v1,
    const SPAunit_vector& v2,
    const SPAunit_vector& z = *(SPAunit_vector *)NULL_REF
    );

DECL_KERN double gme_angle_between(const SPAunit_vector& v1, const SPAunit_vector& v2, const SPAunit_vector& z = *(SPAunit_vector *)NULL_REF);

/**
* Gets the angle between two vectors.<br><br>
* <b>Role:</b> Gets the angle (in radians) between two vectors or two unit vectors in the
* range 0 &lt;= angle &lt; 2 <tt>pi</tt>.
* This function is overloaded. Thi version accepts two vectors as arguments.
* <br><br>
* The third vector is the plane in which the angle is measured, and is required.
* It also defines a direction, from the first to the second vector. To simply get
* the angle in 3-space, pass the normalized cross product of the two input vectors
* as the third vector.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param z
* normal to plane.
**/
DECL_KERN double angle_between(
    const SPAvector& v1,
    const SPAvector& v2,
    const SPAunit_vector& z = *(SPAunit_vector *)NULL_REF
    );

DECL_KERN double gme_angle_between(const SPAvector& v1, const SPAvector& v2, const SPAunit_vector& z = *(SPAunit_vector *)NULL_REF);

/**
* Gets the intersection of a line with a plane.
* <br><br>
* <b>Role:</b> This function returns :<br>
*  1&nbsp;&nbsp;-&nbsp;&nbsp;   There is a normal intersection.<br>
* -1&nbsp;-&nbsp;&nbsp;   The line is parallel to the plane but is on the plane.<br>
*  0&nbsp;&nbsp;-&nbsp;&nbsp;   The line does not intersect the plane.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param line_point
* position on line.
* @param line_vector
* direction of line.
* @param plane_point
* position on plane.
* @param plane_normal
* plane normal.
* @param line_param
* returns line parameter.
* @param intersection_position
* returns intersection.
**/
DECL_KERN int intersect_line_plane(
    const SPAposition     &line_point,
    const SPAvector       &line_vector,
    const SPAposition     &plane_point,
    const SPAunit_vector  &plane_normal,
    double                &line_param,
    SPAposition           &intersection_position);

DECL_KERN int gme_intersect_line_plane(
    const SPAposition     &line_point,
    const SPAvector       &line_vector,
    const SPAposition     &plane_point,
    const SPAunit_vector  &plane_normal,
    double                &line_param,
    SPAposition           &intersection_position);

/**
* Gets the intersection of two planes.
* <br><br>
* <b>Role:</b> This function returns :<br>
*  1&nbsp;&nbsp;-&nbsp;&nbsp;    The planes intersect.<br>
*  0&nbsp;&nbsp;-&nbsp;&nbsp;    The planes are parallel.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param p1
* position on plane 1.
* @param n1
* normal to plane 1.
* @param p2
* position on plane 2.
* @param n2
* normal to plane 2.
* @param line_pt
* returns position on line.
* @param line_dir
* returns line direction.
**/
DECL_KERN int intersect_plane_plane(
    const SPAposition    &p1,
    const SPAunit_vector &n1,
    const SPAposition    &p2,
    const SPAunit_vector &n2,
    SPAposition          &line_pt,
    SPAunit_vector       &line_dir
    );

DECL_KERN int gme_intersect_plane_plane(
    const SPAposition    &p1,
    const SPAunit_vector &n1,
    const SPAposition    &p2,
    const SPAunit_vector &n2,
    SPAposition          &line_pt,
    SPAunit_vector       &line_dir
    );
/**
* Closest point between two straight lines.
* <br><br>
* <b>Role:</b> Determines where two lines, defined by positions and
* directions, are closest.
* <br><br>
* This function returns <tt>TRUE</tt> if the lines intersect, and it finds
* the intersection. For parallel lines, this function returns the root position on
* the first line and its projection on the second line.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param p1
* root position on first line.
* @param v1
* direction of first line.
* @param p2
* root position on second line.
* @param v2
* direction on second line.
* @param t1
* closest first line parameter returned.
* @param pt1
* position on first line returned.
* @param t2
* closest second line parameter returned.
* @param pt2
* position on second line returned.
**/

DECL_KERN logical closest_points(
    const SPAposition     &p1,
    const SPAunit_vector  &v1,
    const SPAposition     &p2,
    const SPAunit_vector  &v2,
    double                &t1,
    SPAposition           &pt1,
    double                &t2,
    SPAposition           &pt2
    );

DECL_KERN logical gme_closest_points(
    const SPAposition     &p1,
    const SPAunit_vector  &v1,
    const SPAposition     &p2,
    const SPAunit_vector  &v2,
    double                &t1,
    SPAposition           &pt1,
    double                &t2,
    SPAposition           &pt2
    );

/**
* Gets the intersections of a line with a circle.
* <br><br>
* <b>Role:</b> This function returns the number of intersections (0, 1, or 2).
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param line_pt
* position on line.
* @param line_dir
* direction of line.
* @param center
* center of circle.
* @param normal
* circle normal.
* @param radius
* circle radius.
* @param intpts
* returns intersections.
**/
DECL_KERN int intersect_line_circle(
    const SPAposition     &line_pt,
    const SPAunit_vector  &line_dir,
    const SPAposition     &center,
    const SPAunit_vector  &normal,
    double                radius,
    SPAposition*          intpts
    );
DECL_KERN int gme_intersect_line_circle(
    const SPAposition     &line_pt,
    const SPAunit_vector  &line_dir,
    const SPAposition     &center,
    const SPAunit_vector  &normal,
    double                radius,
    SPAposition*          intpts
    );

/**
* Determines two orthogonal <tt>vectors</tt> to define a coordinate system, given a <tt>SPAunit_vector</tt>.
* <br><br>
* <b>Role:</b> This function uses the active WCS to find the valid <i>x</i>-axis and
* <i>y</i>-axis of a coordinate system if the <i>z</i>-axis is the only supplied information.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param z_axis
* given vector.
* @param x_axis
* first orthogonal vector returned.
* @param y_axis
* second orthogonal vector returned.
**/
DECL_KERN void compute_axes_from_z(
    const SPAunit_vector &z_axis,
    SPAunit_vector       &x_axis,
    SPAunit_vector       &y_axis);
DECL_KERN void gme_compute_axes_from_z(
    const SPAunit_vector &z_axis,
    SPAunit_vector       &x_axis,
    SPAunit_vector       &y_axis);

/**
 * Converts an angle from degrees to radians
 */

inline DECL_KERN double degrees_to_radians(double ang)
{return (ang * M_PI / 180.0);}

/**
 * Converts an angle from radians to degrees
 */
inline DECL_KERN double radians_to_degrees(double ang)
{return (ang * 180.0 / M_PI);}

/**
 * Find the <tt>SPAposition</tt> in array of positions which is closest to a given <tt>SPAposition</tt>.
 * <br><br>
 * @param npts
 * Number of points in array.
 * @param pts
 * Array of SPApositions.
 * @param pt
 * Test SPAposition.
 */

DECL_KERN int find_best_pt(
    const int              npts,
    const SPAposition      *pts,
    const SPAposition&     pt
    );

DECL_KERN int gme_find_best_pt(
    const int              npts,
    const SPAposition      *pts,
    const SPAposition&     pt
    );
/**
 * Find the <tt>SPAposition</tt> in array of positions which is closest to a <tt>pick_ray</tt>.
 * <br><br>
 * @param npts
 * Number of positions in array.
 * @param pts
 * Array of SPApositions.
 * @param pray
 * Pick Ray.
 */

DECL_KERN int find_best_pt(
    const int           npts,
    const SPAposition   *pts,
    const pick_ray&      pray
    );
DECL_KERN int gme_find_best_pt(
    const int           npts,
    const SPAposition   *pts,
    const pick_ray&      pray
    );

/**
 * Determines whether points in a list are collinear, to a given tolerance.
 */

DECL_KERN logical collinear(
		int                npts,
		const SPAposition  pts[],
		double             tol
	);

DECL_KERN logical gme_collinear(
		int                npts,
		const SPAposition  pts[],
		double             tol
	);

/**
 * Finds the distance from a point to a plane that's given by a point and a <tt>SPAunit_vector</tt>.
 */

DECL_KERN double dist_pt_to_plane(
		SPAposition const    &xyz,
		SPAposition const    &plane_pt,
		SPAunit_vector const &norm
	);

DECL_KERN double gme_dist_pt_to_plane(
		SPAposition const    &xyz,
		SPAposition const    &plane_pt,
		SPAunit_vector const &norm
	);


/**
 * Finds which side of a plane a <tt>SPAposition</tt> lies by returning a positive or negative distance.
 * <br><br>
 * @param root
 * Plane origin.
 * @param dir
 * Plane normal.
 * @param point
 * Any SPAposition.
 */

DECL_KERN double side_of_plane(
    const SPAposition     &root,
    const SPAunit_vector  &dir,
    const SPAposition     &point
    );

DECL_KERN double gme_side_of_plane(
    const SPAposition     &root,
    const SPAunit_vector  &dir,
    const SPAposition     &point
    );

/**
 * Intersects three planes using only geometry: no ACIS <tt>ENTITYs</tt>.
 * <br><br>
 * <b>Role:</b> The planes are specified by point and normal.
 * The normal vectors need not be unit vectors.
 * Function returns whether a unique intersection exists.
 * <br><br>
 * @param p1
 * point on the first plane.
 * @param n1
 * normal vector for the first plane.
 * @param p2
 * point on the second plane.
 * @param n2
 * normal vector for the second plane.
 * @param p3
 * point on the third plane.
 * @param n3
 * normal vector for the third plane.
 * @param int_pt
 * intersection point.
 * @param tol
 * tolerance.
 */

DECL_KERN logical intersect_3_planes(
		const SPAposition   &p1,
		SPAvector const     &n1,
		const SPAposition   &p2,
		SPAvector const     &n2,
		const SPAposition   &p3,
		SPAvector const     &n3,
		SPAposition         &int_pt,
		double              tol = SPAresabs
	);
DECL_KERN logical gme_intersect_3_planes(
		const SPAposition   &p1,
		SPAvector const     &n1,
		const SPAposition   &p2,
		SPAvector const     &n2,
		const SPAposition   &p3,
		SPAvector const     &n3,
		SPAposition         &int_pt,
		double              tol = SPAresabs
	);

/**
 * Intersects three planes using only geometry: no ACIS <tt>ENTITYs</tt>.
 * <br><br>
 * <b>Role:</b> The planes are specified by point and normal.
 * The normal vectors must be <tt>unit_vectors</tt>.  This version of the
 * function has a more robust method for ruling out the case when
 * at least two of the normal vectors are parallel, but it
 * requires that the vectors be normalized.
 * Function returns whether a unique intersection exists.
 * <br><br>
 * @param p1
 * point on the first plane.
 * @param n1
 * normal unit vector for the first plane.
 * @param p2
 * point on the second plane.
 * @param n2
 * normal unit vector for the second plane.
 * @param p3
 * point on the third plane.
 * @param n3
 * normal unit vector for the third plane.
 * @param int_pt
 * intersection point.
 * @param tol
 * tolerance.
 */

DECL_KERN logical intersect_3_planes(
		const SPAposition		&p1,
		SPAunit_vector const    &n1,
		const SPAposition		&p2,
		SPAunit_vector const    &n2,
		const SPAposition		&p3,
		SPAunit_vector const    &n3,
		SPAposition				&int_pt,
		double					tol = SPAresabs
	);

DECL_KERN logical gme_base_intersect_3_planes(
		const SPAposition		&p1,
		SPAunit_vector const    &n1,
		const SPAposition		&p2,
		SPAunit_vector const    &n2,
		const SPAposition		&p3,
		SPAunit_vector const    &n3,
		SPAposition				&int_pt,
		double					tol = SPAresabs
	);
/**
 * Intersects two circles using only geometry: no ACIS <tt>ENTITYs</tt>.
 * <br><br>
 * <b>Role:</b> Circles are specified by a point and radius,
 * and a common normal. They are assumed to be coplanar.
 * If the circles are coincident, no intersections are returned.
 * The normal <tt>SPAvector</tt> need not be a unit <tt>SPAvector</tt>.
 * Function returns the number of intersections: 0, 1 or 2.
 * <br><br>
 * @param C1
 * point on the first circle.
 * @param r1
 * radius for the first circle.
 * @param C2
 * point on the second circle.
 * @param r2
 * radius for the second circle.
 * @param norm
 * normal vector.
 * @param int1
 * intersection point.
 * @param int2
 * intersection point.
 */
DECL_KERN int intersect_2_circles(
		const SPAposition   &C1,
		double              r1,
		const SPAposition   &C2,
		double              r2,
		const SPAvector     &norm,
		SPAposition         &int1,
		SPAposition         &int2
	);

DECL_KERN int gme_intersect_2_circles(
		const SPAposition   &C1,
		double              r1,
		const SPAposition   &C2,
		double              r2,
		const SPAvector     &norm,
		SPAposition         &int1,
		SPAposition         &int2
	);
/**
 * Finds where two lines defined by positions and directions are closest.
 * <br><br>
 * <b>Role:</b> Identical to <tt>closest_points()</tt> except that it doesn't require
 * unit vectors, and it doesn't return the points.
 * If the positions and vectors are values and first derivatives
 * of curves, then the parameters returned would be the first-order
 * <tt>SPAparameter</tt> shifts to find the intersection of the curves.
 * <br><br>
 * @param pt1
 * First SPAposition.
 * @param pt2
 * First vector.
 * @param pt3
 * Second SPAposition.
 * @param pt3
 * Second vector.
 * @param pt3
 * first derivative.
 * @param pt3
 * Second derivative.
 */
DECL_KERN logical int_2_lines_3d(
		const SPAposition &p1,
		const SPAvector   &v1,
		const SPAposition &p2,
		const SPAvector   &v2,
		double            &dt1,
		double            &dt2
	);

DECL_KERN logical gme_int_2_lines_3d(
		const SPAposition &p1,
		const SPAvector   &v1,
		const SPAposition &p2,
		const SPAvector   &v2,
		double            &dt1,
		double            &dt2
	);
/**
 * Finds a circular arc defined by 3 positions.
 * <br><br>
 * <b>Role:</b> This function returns
 * <tt>TRUE</tt> if a solution was computed, or <tt>FALSE</tt> if the three positions
 * are colinear.
 * <br><br>
 * @param pt1
 * First SPAposition.
 * @param pt2
 * Second SPAposition.
 * @param pt3
 * Third SPAposition.
 * @param center
 * Center of the circle.
 * @param normal
 * SPAunit_vector normal to plane of circle.
 * @param radius
 * Radius of the circle.
 */
DECL_KERN logical circle_3_pos(
	const SPAposition &pt1,
	const SPAposition &pt2,
	const SPAposition &pt3,
	SPAposition       &center,
	SPAunit_vector    &normal,
	double            &radius
	);

DECL_KERN logical gme_circle_3_pos(const SPAposition& pt1, const SPAposition& pt2, const SPAposition& pt3, SPAposition& center, SPAunit_vector& normal, double& radius);

/**
 * Brings a value into, or near, a base <tt>SPAinterval</tt> in a periodic domain.
 * <br><br>
 * <b>Role:</b> The <tt>SPAinterval</tt> is not necessarily the entire period, and might cross
 * a "seam".  This routine does not deal with seams, it essentially
 * just brings the <tt>SPAparameter</tt> as close as possible to the midpoint of
 * the given range.
 * <br><br>
 * @param range
 * range.
 * @param period
 * period.
 * @param param
 * param.
 */

DECL_KERN double reduce_to_range(
		const SPAinterval &range,
		const double      period,
		double            param
	);

DECL_KERN double gme_reduce_to_range(
		const SPAinterval &range,
		const double      period,
		double            param
	);
/** @} */
#endif

