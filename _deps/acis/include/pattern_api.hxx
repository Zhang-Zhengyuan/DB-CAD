/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef PATTERN_TYPES_HXX
#define PATTERN_TYPES_HXX
#include <math.h>
#include "dcl_kern.h"
#include "acis3dt.h"
#include "api.hxx"
#include "interval.hxx"
#include "vector.hxx"
/**
 * \defgroup ACISPATTERNS Patterns
 * \ingroup KERNAPI
 *
 */
/**
 * @file pattern_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPATTERNS
 *
 * @{
 */
class pattern;
class law;
class SPApar_box;
class pcurve;
class SPAvector;
class SPAunit_vector;
class SPAposition;
class SPAtransf;
class curve;
class surface;
class FACE;
class EDGE;
class COEDGE;

// Pattern-creation functions

// A circular pattern is defined by an axis of rotation which is
// defined by a center point and an axis direction or normal.
// The number argument tells how many entities are in the pattern.
/**
 * Creates a pattern parallel to a curve.
 * <br><br>
 * <b>Role:</b> Creates a one-dimensional pattern of <tt>num_elements</tt> elements, equally
 * spaced in parameter space, along the curve specified by the <tt>in_curve</tt> argument,
 * over the interval given by <tt>param_range</tt>. The argument <tt>root</tt> specifies the position
 * (which can be on or off the pattern seed entity, as desired) to be mapped to the
 * pattern sites. The pattern can be extended to the endpoints of the edge by
 * setting <tt>on_endpoints</tt> to TRUE. By default, pattern members are oriented
 * identically to one another. They will follow a rail law if <tt>rail_law</tt> is provided.
 * In that case, the vectors <tt>rail_dir</tt> and <tt>tangent_dir</tt> specify the directions,
 * relative to the seed entity, that are mapped to the rail law and tangent
 * directions of the edge.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a spiral curve
 * EDGE* edge = NULL;
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * SPAposition start_position(3, 0, 0);
 * double width = 3.0;
 * double angle = 6.0 * M_PI;
 * check_outcome(result = api_edge_spiral(center, normal,
 *     start_position, width, angle, edge));
 * const curve& crv = edge->geometry()->equation();
 * SPAinterval param_range = edge->param_range();
 * 
 * if (edge->sense() == REVERSED)
 *     param_range.negate();
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int number = 36;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_create_on_curve(pat, crv, 
 *     param_range, number, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(edge));</pre>
 * <b>Errors:</b> The number of elements is less than one, or the rail direction was
 * specified without specifying a tangent direction.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern returned.
 * @param in_curve
 * guiding curve.
 * @param param_range
 * range.
 * @param num_elements
 * number of elements in the pattern.
 * @param root
 * position mapped to the pattern sites.
 * @param on_endpoints
 * flag for beginning and ending on endpoints.
 * @param rail_law
 * rail law to follow.
 * @param rail_dir
 * direction mapped to rail direction.
 * @param tangent_dir
 * direction mapped to tangent direction.
 * @param in_trans
 * check for transform.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_curve
(
    pattern*&           pat,
    const curve&        in_curve,
    const SPAinterval&  param_range,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    law*                rail_law        = NULL,
    const SPAvector&    rail_dir        = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
    const SPAtransf&    in_trans        = *(SPAtransf*)NULL_REF,
	AcisOptions*        ao              = NULL);
/**
 * Creates a pattern with cylindrical symmetry.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional pattern with cylindrical symmetry, with a radius and axis
 * defined by the cylindrical face <tt>in_face</tt>. The numbers of angular and axial elements in
 * the pattern are set by <tt>num_angular</tt> and <tt>num_axial</tt>, respectively, and the distance
 * between circular pattern layers by the <tt>ring_spacing</tt> argument. If <tt>alternating</tt> is TRUE,
 * adjacent layers are staggered in angle. The pattern coordinates are specified in the order (angular, axial).
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a cylindrical face
 * SPAposition center(5, 0, 0);
 * SPAvector normal(0, 1, 0);
 * double radius = 2.0;
 * double start = 0.0;
 * double end = M_PI;
 * double ratio = 1.0;
 * SPAposition pt(1, 0, 0);
 * FACE* face = NULL;
 * check_outcome(result = api_face_cylinder_cone(center, normal,
 *     radius, radius, start, end, ratio, &pt, face));
 *
 * // Create a pattern	
 * pattern* pat = NULL;
 * int num_angular = 8;
 * int num_axial = 5;
 * double spacing = 1.5;
 * check_outcome(result = api_pattern_create_cylindrical(pat, face,
 *     num_angular, num_axial, spacing));
 *
 * // Create a cylinder
 * SPAposition bottom(0, 0, 0);
 * SPAposition top(0.5, 0, 0);
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * BODY* cylinder = NULL;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, cylinder));
 *
 * // Apply the pattern to the cylinder
 * check_outcome(result = api_pattern_apply_to_entity(cylinder, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Errors:</b> The number of angular or axial elements is less than one, or the face
 * that is specified is not cylindrical.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_face
 * face defining pattern axis and radius.
 * @param num_angular
 * number of pattern elements about cylinder axis.
 * @param num_axial
 * number of pattern elements along cylinder axis.
 * @param ring_spacing
 * distance between circular pattern layers.
 * @param alternating
 * flag to stagger angle between layers.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_cylindrical
(
    pattern*&           pat,
    const FACE*         in_face,
    int                 num_angular,
    int                 num_axial       = 1,
    double              ring_spacing    = 0.0,
    logical             alternating     = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern with cylindrical symmetry.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional pattern with cylindrical symmetry, with a radius and axis
 * defined by the center position and normal vector. The numbers of angular and axial elements in
 * the pattern are set by <tt>num_angular</tt> and <tt>num_axial</tt>, respectively, and the distance
 * between circular pattern layers by the <tt>ring_spacing</tt> argument. If <tt>alternating</tt> is TRUE,
 * adjacent layers are staggered in angle. The pattern coordinates are specified in the order (angular, axial).
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(5, 0, 0);
 * SPAvector normal(0, 1, 0);
 * int num_angular = 8;
 * int num_axial = 5;
 * double spacing = 5.0;
 * check_outcome(result = api_pattern_create_cylindrical(pat, center, 
 *     normal, num_angular, num_axial, spacing));
 *
 * // Create a cylinder
 * SPAposition bottom(0, 0, 0);
 * SPAposition top(0.5, 0, 0);
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * BODY* cylinder = NULL;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, cylinder));
 *
 * // Apply the pattern to the cylinder
 * check_outcome(result = api_pattern_apply_to_entity(cylinder, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The number of angular or axial elements is less than one, or the face
 * that is specified is not cylindrical.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * starting position for cylinder axis.
 * @param normal
 * direction of the cylinder axis.
 * @param num_angular
 * number of pattern elements about cylinder axis.
 * @param num_axial
 * number of pattern elements along cylinder axis.
 * @param ring_spacing
 * distance between circular pattern layers.
 * @param alternating
 * flag to stagger angle between layers.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_cylindrical
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_angular,
    int                 num_axial       = 1,
    double              ring_spacing    = 0.0,
    logical             alternating     = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern parallel to an edge.
 * <br><br>
 * <b>Role:</b> Creates a one-dimensional pattern of <tt>num_elements</tt> elements, equally
 * spaced in parameter space, parallel to the edge specified by the
 * <tt>in_edge</tt> argument. The argument <tt>root</tt> specifies the position (which
 * can be on or off the pattern seed entity, as desired) to be mapped to the
 * pattern sites. The pattern can be extended to the endpoints of the edge by
 * setting <tt>on_endpoints</tt> to TRUE. By default, the orientations of pattern members
 * are identical. They will instead follow the edge's
 * rail law if <tt>rail_dir</tt> and <tt>tangent_dir</tt> are specified.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a spline edge
 * SPAposition pts[7];
 * pts[0] = SPAposition(0, 0, 0);
 * pts[1] = SPAposition(10, 5, 0);
 * pts[2] = SPAposition(20, 2, 0);
 * pts[3] = SPAposition(30, 8, 0);
 * pts[4] = SPAposition(40, 2, 0);
 * pts[5] = SPAposition(50, 5, 0);
 * pts[6] = SPAposition(60, 0, 0);
 * SPAunit_vector dir_start(0, 1, 0);
 * SPAunit_vector dir_end(0, -1, 0);
 * EDGE* edge = NULL;
 * check_outcome(result = api_curve_spline(7, pts,
 *     &dir_start, &dir_end, edge));
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int number = 20;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_create_on_edge(pat, edge, 
 *     number, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(edge));</pre>
 * <b>Errors:</b> The number of elements is less than one, or the rail
 * direction was specified without specifying a tangent direction, or a NULL entity
 * was specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_edge
 * edge.
 * @param num_elements
 * number of elements.
 * @param root
 * start position.
 * @param on_endpoints
 * extend to endpoints or not.
 * @param rail_dir
 * use rail law for pattern orientation.
 * @param tangent_dir
 * use tangent to edge's face for pattern orientation.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_edge
(
    pattern*&           pat,
    EDGE*               in_edge,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    rail_dir        = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern parallel to an edge.
 * <br><br>
 * <b>Role:</b> Creates a one-dimensional pattern of <tt>num_elements</tt> elements, equally
 * spaced in parameter space, parallel to the edge specified by the
 * <tt>in_edge</tt> argument. The argument <tt>root</tt> specifies the position (which
 * can be on or off the pattern seed entity, as desired) to be mapped to the
 * pattern sites. The pattern can be extended to the endpoints of the edge by
 * setting <tt>on_endpoints</tt> to TRUE. By default, the orientations of pattern members
 * are identical. They will instead follow the normal to the edge's
 * face <tt>in_face</tt> if <tt>normal_dir</tt> and <tt>tangent_dir</tt> are given.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a law face and get one of its edges
 * FACE* face = NULL;
 * law* face_law = NULL;
 * check_outcome(result = api_str_to_law("vec(x, 5 * sin(x/5), y)", &face_law));
 * check_outcome(result = api_face_law(face_law, -50, 50, -50, 50, face));
 * EDGE* edge = face->loop()->start()->edge();
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int number = 50;
 * SPAposition root(0, 0, 0);
 * logical on_endpoints = FALSE;
 * SPAvector normal_dir(1, 0, 0);
 * SPAvector tangent_dir(0, 1, 0);
 * check_outcome(result = api_pattern_create_on_edge(pat, edge, face,
 *     number, root, on_endpoints, normal_dir, tangent_dir));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * face_law->remove();
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Errors:</b> The number of elements is less than one, or the normal
 * direction was specified without specifying a tangent direction, or a NULL entity
 * was specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_edge
 * edge.
 * @param in_face
 * face.
 * @param num_elements
 * number of elements.
 * @param root
 * start position.
 * @param on_endpoints
 * extend to endpoints or not.
 * @param normal_dir
 * use normal to face for pattern orientation.
 * @param tangent_dir
 * use tangent to face for pattern orientation.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_edge
(
    pattern*&           pat,
    EDGE*               in_edge,
    FACE*               in_face,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    normal_dir      = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern parallel to an edge.
 * <br><br>
 * <b>Role:</b> Creates a one-dimensional pattern of <tt>num_elements</tt> elements, equally
 * spaced in parameter space, parallel to the coedge specified by the
 * <tt>in_coedge</tt> argument. The argument <tt>root</tt> specifies the position (which
 * can be on or off the pattern seed entity, as desired) to be mapped to the
 * pattern sites. The pattern can be extended to the endpoints of the coedge by
 * setting <tt>on_endpoints</tt> to TRUE. By default, the orientations of pattern members
 * are identical. They will instead follow the normal to the coedge's
 * face if <tt>normal_dir</tt> and <tt>tangent_dir</tt> are given.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a spline face and get one of its coedges
 * FACE* face = NULL;
 * SPAposition* pts = ACIS_NEW SPAposition[9];
 * pts[0] = SPAposition(0, 0, 0);
 * pts[1] = SPAposition(10, 0, 50);
 * pts[2] = SPAposition(20, 0, 60);
 * pts[3] = SPAposition(0, 10, 20);
 * pts[4] = SPAposition(10, 10, 30);
 * pts[5] = SPAposition(20, 10, 40);
 * pts[6] = SPAposition(0, 20, -20);
 * pts[7] = SPAposition(10, 20, 0);
 * pts[8] = SPAposition(20, 20, 10);
 * splgrid grid;
 * grid.set_gridpts_array(pts, 3, 3);
 * check_outcome(result = api_face_spl_apprx(&grid, face));
 * COEDGE* coedge = face->loop()->start();
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int number = 20;
 * SPAposition root(0, 0, 0);
 * logical on_endpoints = FALSE;
 * SPAvector normal_dir(-1, 0, 0);
 * SPAvector tangent_dir(0, 0, 1);
 * check_outcome(result = api_pattern_create_on_edge(pat, coedge, 
 *     number, root, on_endpoints, normal_dir, tangent_dir));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Errors:</b> The number of elements is less than one, or the normal
 * direction was specified without specifying a tangent direction, or a <tt>NULL</tt> entity
 * was specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_coedge
 * coedge.
 * @param num_elements
 * number of elements.
 * @param root
 * start position.
 * @param on_endpoints
 * extend to endpoints or not.
 * @param normal_dir
 * use normal to coedge's face for pattern orientation.
 * @param tangent_dir
 * use tangent to coedge's face for pattern orientation.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_edge
(
    pattern*&           pat,
    COEDGE*             in_coedge,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    normal_dir      = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates an elliptical pattern.
 * <br><br>
 * <b>Role:</b> Creates a one-dimensional elliptical pattern defined by an axis
 * of rotation. The <tt>center</tt> and <tt>normal</tt> arguments indicate the (global) position and
 * orientation of the axis. The <tt>num_elements</tt> argument defines the number of entities in
 * the pattern. These elements are kept in a fixed relative orientation if
 * <tt>not_rotate</tt> is TRUE, in which case <tt>root</tt>, the position that is mapped to the
 * pattern sites, must be specified. The <tt>angle</tt> argument fixes the angular extent of
 * the pattern, with positive or negative values indicating a pattern proceeding
 * clockwise or counter-clockwise about the <tt>normal</tt> vector. The <tt>ratio</tt> argument sets
 * the ratio of minor/major radii of the pattern. If <tt>major_axis</tt> is given, it
 * specifies the major-axis direction of the pattern; otherwise, this axis is directed from
 * <tt>center</tt> to <tt>root</tt>.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(10, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int number = 12;
 * check_outcome(result = api_pattern_create_elliptical(pat, center, 
 *     normal, number));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The number of elements is less than one, or the user has failed to supply a
 * root position with the value of <tt>not_rotate</tt> set to TRUE.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * center of pattern.
 * @param normal
 * normal to pattern plane.
 * @param num_elements
 * number of pattern elements.
 * @param not_rotate
 * TRUE eliminates rotation of elements.
 * @param root
 * position mapped to pattern sites.
 * @param angle
 * angular extent of pattern.
 * @param ratio
 * ratio of major/minor radii.
 * @param major_axis
 * orientation of major axis.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_elliptical
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_elements,
    logical             not_rotate      = FALSE,
    const SPAposition&  root            = *(SPAposition*)NULL_REF,
    double              angle           = 2.0 * M_PI,
    double              ratio           = 1.0,
    const SPAvector&    major_axis      = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates a hexagonal pattern with cylindrical symmetry.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional hexagonal pattern with cylindrical
 * symmetry, with a radius and axis defined by the cylindrical face <tt>in_face</tt>.
 * The number of angular and axial elements in the pattern are set by <tt>num_angular</tt> and <tt>num_axial</tt>, respectively,
 * and the distance between pattern elements by <tt>spacing</tt>. The pattern coordinates
 * are specified in the order (angular, axial).
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a cylindrical face
 * SPAposition center(5, 0, 0);
 * SPAvector normal(0, 1, 0);
 * double radius = 2.0;
 * double start = 0.0;
 * double end = M_PI;
 * double ratio = 1.0;
 * SPAposition pt(1, 0, 0);
 * FACE* face = NULL;
 * check_outcome(result = api_face_cylinder_cone(center, normal,
 *     radius, radius, start, end, ratio, &pt, face));
 *
 * // Create a pattern	
 * pattern* pat = NULL;
 * int num_angular = 8;
 * int num_axial = 6;
 * double spacing = 3.0;
 * check_outcome(result = api_pattern_create_hex_cylindrical(pat, 
 *     face, num_angular, num_axial, spacing));
 *
 * // Create a cylinder
 * SPAposition bottom(0, 0, 0);
 * SPAposition top(0.5, 0, 0);
 * double maj_rad = 1.0;
 * double min_rad = 1.0;
 * BODY* cylinder = NULL;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, cylinder));
 *
 * // Apply the pattern to the cylinder
 * check_outcome(result = api_pattern_apply_to_entity(cylinder, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_face
 * face defining the pattern.
 * @param num_angular
 * number of pattern elements about cylinder axis.
 * @param num_axial
 * number of pattern elements along cylinder axis.
 * @param spacing
 * spacing of pattern elements.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_hex_cylindrical
(
    pattern*&           pat,
    const FACE*         in_face,
    int                 num_angular,
    int                 num_axial       = 1,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

/**
 * Creates a hexagonal pattern with cylindrical symmetry.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional hexagonal pattern with cylindrical
 * symmetry, with a radius and axis defined by the center position and
 * normal vector. The number of angular and
 * axial elements in the pattern are set by <tt>num_angular</tt> and <tt>num_axial</tt>, respectively,
 * and the distance between pattern elements by <tt>spacing</tt>. The pattern coordinates
 * are specified in the order (angular, axial).
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(5, 0, 0);
 * SPAvector normal(0, 1, 0);
 * int num_angular = 8;
 * int num_axial = 6;
 * double spacing = 3.0;
 * check_outcome(result = api_pattern_create_hex_cylindrical(pat, 
 *     center, normal, num_angular, num_axial, spacing));
 *
 * // Create a cylinder
 * SPAposition bottom(0, 0, 0);
 * SPAposition top(0.5, 0, 0);
 * double maj_rad = 1.0;
 * double min_rad = 1.0;
 * BODY* cylinder = NULL;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, cylinder));
 *
 * // Apply the pattern to the cylinder
 * check_outcome(result = api_pattern_apply_to_entity(cylinder, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * start position.
 * @param normal
 * direction of cylinder axis.
 * @param num_angular
 * number of pattern elements about cylinder axis.
 * @param num_axial
 * number of pattern elements along cylinder axis.
 * @param spacing
 * spacing of pattern elements.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_hex_cylindrical
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_angular,
    int                 num_axial       = 1,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

// A hex pattern is the hexagon packing pattern on the plane with
// the given normal.  The grid goes in the xvec direction with the center
// to center distance between hexagons equaling the xvec's length.  The
// num_x, and num_y arguments tell how many positions are created in the
// xvec direction and in the y direction which is the cross product of the
// normal and the x directions.
/**
 * Creates a hexagonal pattern in two or three dimensions.
 * <br><br>
 * <b>Role:</b> Creates a hexagonal pattern in two or three dimensions. For
 * two-dimensional patterns, the <tt>normal</tt> parameter specifies the direction normal to
 * the pattern plane; for three-dimensional patterns, it sets the z-direction. The
 * <tt>x_vec</tt> argument defines the pattern's starting axis and displacement; <tt>num_x</tt>,
 * <tt>num_y</tt>, and <tt>num_z</tt> set the number of repetitions in each dimension over which the
 * pattern extends.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector normal(0, 0, 1);
 * SPAvector x_vec(2, 0, 0);
 * int num_x = 4;
 * int num_y = 4;
 * int num_z = 4;
 * check_outcome(result = api_pattern_create_hex(pat, normal, x_vec, 
 *     num_x, num_y, num_z));
 *
 * // Create a sphere
 * SPAposition center(1, 1, 0);
 * double radius = 1.0;
 * BODY* sph = NULL;
 * check_outcome(result = api_solid_sphere(center, radius, sph));
 *
 * // Apply the pattern to the sphere
 * check_outcome(result = api_pattern_apply_to_entity(sph, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param normal
 * normal to pattern.
 * @param x_vec
 * starting axis.
 * @param num_x
 * repeat in x.
 * @param num_y
 * repeat in y.
 * @param num_z
 * number of times to repeat in z.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_hex
(
    pattern*&           pat,
    const SPAvector&    normal,
    const SPAvector&    x_vec,
    int                 num_x,
    int                 num_y,
    int                 num_z           = 1,
	AcisOptions*        ao              = NULL);

// A linear pattern is defined by an x SPAvector a y SPAvector, and a z SPAvector
// and the number of elements in each directions.  The y and z part are
// optional.  Note that the vectors do not have to be in the coordinate
// directions or orthogonal.

/**
 * Creates a linear pattern.
 * <br><br>
 * <b>Role:</b> Creates a linear pattern in one, two, or three dimensions,
 * depending upon the number of input arguments. The pattern orientation is
 * specified by <tt>x_vec</tt>, <tt>y_vec</tt>, and <tt>z_vec</tt>, which are neither required to be in the
 * coordinate directions nor to be orthogonal. The number of repetitions along each
 * axis is defined by <tt>num_x</tt>, <tt>num_y</tt>, and <tt>num_z</tt>. If <tt>y_staggered</tt>
 * or <tt>z_staggered</tt> is TRUE, the pattern is staggered along the associated directions.
 * If <tt>fit_distance</tt> is TRUE, the vectors <tt>x_vec</tt>, <tt>y_vec</tt>, and <tt>z_vec</tt> represent
 * displacements over the entire pattern rather than displacements between adjacent pattern elements.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(2, 0, 0);
 * int num_x = 4;
 * SPAvector y_vec(0, 2, 0);
 * int num_y = 3;
 * SPAvector z_vec(1, 1, 2);
 * int num_z = 3;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, num_x, 
 *     y_vec, num_y, z_vec, num_z));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param x_vec
 * displacement vector in the x-direction.
 * @param num_x
 * number of elements in the x-direction.
 * @param y_vec
 * displacement vector in the y-direction.
 * @param num_y
 * number of elements in the y-direction.
 * @param z_vec
 * displacement vector in the z-direction.
 * @param num_z
 * number of elements in the z-direction.
 * @param y_staggered
 * flag to stagger the pattern y-components.
 * @param z_staggered
 * flag to stagger the pattern z-components.
 * @param fit_distance
 * displacement flag acis options.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_linear
(
    pattern*&           pat,
    const SPAvector&    x_vec,
    int                 num_x,
    const SPAvector&    y_vec           = *(SPAvector*)NULL_REF,
    int                 num_y           = 1,
    const SPAvector&    z_vec           = *(SPAvector*)NULL_REF,
    int                 num_z           = 1,
    logical             y_staggered     = FALSE,
    logical             z_staggered     = FALSE,
    logical             fit_distance    = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a polar-grid pattern.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional polar-grid pattern defined by a root
 * position <tt>center</tt> (which may or may not lie upon the seed entity) and the vector
 * <tt>normal</tt>, which sets the orientation of the pattern. The number of rings in the
 * grid (including the center) is specified by <tt>num_rings</tt>, and the distance between
 * rings by <tt>spacing</tt>. The optional <tt>start</tt> argument specifies the direction of the
 * first spoke of the pattern. The elements of the pattern are kept in a fixed
 * orientation if <tt>not_rotate</tt> is TRUE; setting <tt>hex_symmetry</tt> to TRUE ensures that
 * hexagonal symmetry is maintained for patterns extending either 360 or 180
 * degrees. The <tt>start_angle</tt> and <tt>end_angle</tt> arguments fix the angular extent of the
 * pattern, in radians, and the <tt>ratio</tt> argument sets the ratio of minor/major radii
 * of the pattern perimeter. The pattern coordinates are specified in the order(radial, angular).
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_rings = 5;
 * double spacing = 4.0;
 * check_outcome(result = api_pattern_create_polar_grid(pat, center, normal, 
 *                                                num_rings, spacing));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * center (root) position.
 * @param normal
 * normal to pattern plane.
 * @param num_rings
 * number of rings in pattern.
 * @param distance
 * distance between pattern rings.
 * @param start
 * pattern start direction.
 * @param not_rotate
 * rotation flag.
 * @param hex_symmetry
 * force hex symmetry flag.
 * @param start_angle
 * start angle.
 * @param end_angle
 * end angle.
 * @param ratio
 * ratio of minor/major radii.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_polar_grid
(
    pattern*&           pat,
    const SPAposition&  center,
	const SPAvector&    normal,
	int					num_rings,
	double				distance,
	const SPAvector&    start           = *(SPAvector*)NULL_REF,
	logical				not_rotate      = FALSE,
	logical				hex_symmetry    = FALSE,
	double				start_angle     = 0.0,
	double				end_angle       = 2.0 * M_PI,
	double				ratio           = 1.0,
	AcisOptions*        ao              = NULL);

/**
 * Creates a radial pattern.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional radial pattern defined by a root
 * position <tt>center</tt> (which may or may not lie upon the seed entity) and the vector
 * <tt>normal</tt>, which sets the orientation of the pattern. The number of elements in the
 * radial and angular directions are specified by <tt>num_radial</tt> and <tt>num_angular</tt>,
 * respectively, and the distance between successive rings of the pattern by the
 * <tt>spacing</tt> argument. The optional <tt>start</tt> argument specifies the direction of the
 * first spoke of the pattern. The elements of the pattern are kept in a fixed
 * relative orientation if <tt>not_rotate</tt> is TRUE. The <tt>start_angle</tt>
 * and <tt>end_angle</tt> arguments fix the angular extent of the pattern,
 * while the <tt>ratio</tt> argument sets
 * the ratio of minor/major radii of the pattern perimeter. The pattern coordinates
 * are specified in the order (radial, angular).
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_radial = 4;
 * int num_angular = 5;
 * double spacing = 3.0;
 * check_outcome(result = api_pattern_create_radial(pat, center, normal, 
 *     num_radial, num_angular, spacing));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * center (root) position.
 * @param normal
 * normal to pattern plane.
 * @param num_radial
 * number of radial pattern rings.
 * @param num_angular
 * number of polar pattern radii.
 * @param spacing
 * distance between pattern rings.
 * @param start
 * start direction.
 * @param not_rotate
 * rotation flag.
 * @param start_angle
 * start angle.
 * @param end_angle
 * end angle.
 * @param ratio
 * ratio of minor/major radii.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_radial
(
    pattern*&           pat,
    const SPAposition&  center,
	const SPAvector&    normal,
	int					num_radial,
	int					num_angular,
	double				spacing,
	const SPAvector&    start           = *(SPAvector*)NULL_REF,
	logical				not_rotate      = FALSE,
	double				start_angle     = 0.0,
	double				end_angle       = 2.0 * M_PI,
	double				ratio           = 1.0,
	AcisOptions*        ao              = NULL);
/**
 * Creates a random pattern within the indicated region.
 * <br><br>
 * <b>Role:</b> Creates a random pattern of <tt>num_elements</tt> elements, centered at the
 * location of the pattern seed entity and extending distances given by the
 * components of <tt>extents</tt> in <tt>dimension</tt> dimensions. The arguments
 * <tt>x_vec</tt> and <tt>y_vec</tt>
 * specify the orientation of the pattern, and are the directions associated with
 * the first two components of <tt>extents</tt>. (The third component is associated with the
 * cross product of these arguments.) When an ellipsoidal pattern is selected
 * (<tt>ellipsoidal = TRUE</tt>), the number of pattern elements actually generated may differ
 * somewhat from <tt>num_elements</tt>.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector extents(50, 25, 10);
 * int number = 100;
 * int dimensions = 3;
 * check_outcome(result = api_pattern_create_random(pat, extents, number, dimensions));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The number of elements specified is less than one, or the dimensionality
 * is greater than three or less than one.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern created.
 * @param extents
 * vector components give pattern extents in each direction.
 * @param num_elements
 * number of pattern elements.
 * @param dimension
 * pattern dimensionality.
 * @param ellipsoidal
 * ellipsoidal flag.
 * @param x_vec
 * direction for first extent component.
 * @param y_vec
 * direction for second extent component.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_random
(
    pattern*&           pat,
    const SPAvector&    extents,
    int                 num_elements,
    int                 dimension       = 3,
    logical             ellipsoidal     = FALSE,
    const SPAvector&    x_vec           = SPAvector(1, 0, 0),
    const SPAvector&    y_vec           = SPAvector(0, 1, 0),
	AcisOptions*        ao              = NULL);

/**
 * Creates a spherical pattern.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional spherical pattern about the <tt>center</tt>
 * position, with the pattern seed entity at one pole of the associated sphere. The
 * pattern elements are approximately equally spaced, with the parameter
 * <tt>num_latitudes</tt> specifying by default the number of latitudinal rings in the
 * pattern. If <tt>num_latitudes</tt> is set to zero, this number is instead determined by
 * the optional <tt>spacing</tt> parameter. (This number must be specified if <tt>num_latitudes</tt>
 * is zero.) The root position of the pattern is given by <tt>root</tt>.  The pattern
 * coordinates for spacing are specified in the order (longitude, latitude).
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(0, 0, -5);
 * int num_latitudes = 10;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_create_spherical(pat, center, 
 *     num_latitudes, root));
 *
 * // Create a pyramid
 * double height = 0.5;
 * double maj_rad = 1.0;
 * double min_rad = 0.2;
 * double top = 0.0;
 * int num_sides = 3;
 * BODY* pyramid = NULL;
 * check_outcome(result = api_make_pyramid(height, maj_rad, 
 *     min_rad, top, num_sides, pyramid));
 *
 * // Apply the pattern to the pyramid
 * check_outcome(result = api_pattern_apply_to_entity(pyramid, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> Both the spacing and the number of latitudes is zero.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param center
 * pattern center.
 * @param num_latitudes
 * number of latitudinal rings in the pattern.
 * @param root
 * position mapped to pattern sites.
 * @param spacing
 * desired spacing for pattern elements.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_spherical
(
    pattern*&           pat,
    const SPAposition&  center,
    int                 num_latitudes,
    const SPAposition&  root,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern parallel to a surface.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional pattern of <tt>num_u</tt> by <tt>num_v</tt> elements,
 * equally spaced in parameter space, upon the surface and parameter range
 * associated with the face <tt>in_face</tt>. The argument <tt>root</tt>
 * specifies the position (which can be on or off the pattern seed entity, as
 * desired) to be mapped to the pattern sites. The pattern can be extended to the
 * face boundary by setting <tt>on_boundary</tt> to TRUE. By default, pattern members are
 * oriented identically to one another, but will follow the surface normal if <tt>u_dir</tt>
 * and <tt>v_dir</tt> are given. In that case, these vectors specify the directions,
 * relative to the seed entity, that are mapped to the <tt>u-</tt> and <tt>v-</tt>directions of the
 * face.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a toroidal face
 * SPAposition center(0, 0, 0);
 * double major_radius = 12.0;
 * double minor_radius = 5.0;
 * double tu_start = 0.0;
 * double tu_end = 360.0;
 * double sv_start = 0.0;
 * double sv_end = 360.0;
 * SPAvector normal(0, 0, 1);
 * FACE* face = NULL;
 * check_outcome(result = api_face_torus(center, major_radius,
 *     minor_radius, tu_start, tu_end, sv_start, sv_end,
 *     &normal, face));
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int u_num = 10;
 * int v_num = 20;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_create_on_surface(pat, face, u_num, 
 *     v_num, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Errors:</b> The number of u- or v-values is less than one, or <tt>u_dir</tt> is specified
 * without specifying <tt>v_dir</tt> (or vice-versa), or a NULL face is given.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_face
 * guide face.
 * @param num_u
 * u-direction elements.
 * @param num_v
 * v-direction elements.
 * @param root
 * position mapped to the pattern sites.
 * @param on_boundary
 * flag to begin and end on face boundary.
 * @param u_dir
 * direction mapped to u-direction.
 * @param v_dir
 * direction mapped to v-direction.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_surface
(
    pattern*&           pat,
    FACE*               in_face,
    int                 num_u,
    int                 num_v,
    const SPAposition&  root,
    logical             on_boundary     = FALSE,
    const SPAvector&    u_dir           = *(SPAvector*)NULL_REF,
    const SPAvector&    v_dir           = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates a pattern parallel to a surface.
 * <br><br>
 * <b>Role:</b> Creates a two-dimensional pattern of <tt>num_u</tt> by <tt>num_v</tt> elements,
 * equally spaced in parameter space, upon the surface and parameter range
 * indicated by <tt>in_surf</tt> and <tt>param_range</tt>. The argument <tt>root</tt>
 * specifies the position (which can be on or off the pattern seed entity, as
 * desired) to be mapped to the pattern sites. The pattern can be extended to the
 * face boundary by setting <tt>on_boundary</tt> to TRUE. By default, pattern members are
 * oriented identically to one another, but will follow the surface normal if <tt>u_dir</tt>
 * and <tt>v_dir</tt> are given. In that case, these vectors specify the directions,
 * relative to the seed entity, that are mapped to the <tt>u-</tt> and <tt>v-</tt>directions of the
 * face.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a hemispherical surface
 * SPAposition origin(0, 0, 0);
 * double radius = 20.0;
 * double lo_start = 0.0;
 * double lo_end = 90.0;
 * double la_start = 0.0;
 * double la_end = 360.0;
 * SPAvector normal(0, 0, 1); 
 * FACE* face = NULL;
 * check_outcome(result = api_face_sphere(origin, radius,
 *     lo_start, lo_end, la_start, la_end, &normal, face)); 
 * const surface& surf = face->geometry()->equation();
 * SPApar_box param_range;
 * sg_get_face_par_box(face, param_range);
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * int u_num = 8;
 * int v_num = 6;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_create_on_surface(pat, surf, 
 *     param_range, u_num, v_num, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * check_outcome(result = api_del_entity(face));</pre>
 * <b>Errors:</b> The number of u- or v-values is less than one, or <tt>u_dir</tt> is specified
 * without specifying <tt>v_dir</tt> (or vice-versa), or a NULL face is given.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * created pattern.
 * @param in_surf
 * guide surface.
 * @param face_range
 * range of surface.
 * @param num_u
 * u-direction elements.
 * @param num_v
 * v-direction elements.
 * @param root
 * position mapped to the pattern sites.
 * @param on_boundary
 * flag to begin and end on face boundary.
 * @param u_dir
 * direction mapped to u-direction.
 * @param v_dir
 * direction mapped to v-direction.
 * @param in_trans
 * input transform.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_on_surface
(
    pattern*&           pat,
    const surface&      in_surf,
    const SPApar_box&   face_range,
    int                 num_u,
    int                 num_v,
    const SPAposition&  root,
    logical             on_boundary     = FALSE,
    const SPAvector&    u_dir           = *(SPAvector*)NULL_REF,
    const SPAvector&    v_dir           = *(SPAvector*)NULL_REF,
    const SPAtransf&    in_trans        = *(SPAtransf*)NULL_REF,
	AcisOptions*        ao              = NULL);

// Pattern-modification functions

/**
 * Creates a new pattern by applying an alternating keep-filter to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies an alternating keep-filter to an existing pattern,
 * merging with any existing filter or, optionally (with <tt>merge = FALSE</tt>), replacing
 * it. The arguments <tt>keep1</tt> and <tt>keep2</tt> are successive Boolean keep values, and <tt>which_dim</tt>
 * specifies the dimension in which the filter is applied.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * logical keep1 = FALSE;
 * logical keep2 = TRUE;
 * int which_dim = 1;
 * check_outcome(result = 
 *     api_pattern_modify_filter_alternating(pat, keep1, keep2, which_dim));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The input pattern is NULL, the period is less than one, or the specified
 * dimension is not consistent with the pattern dimensionality.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param keep1
 * 1st keep value.
 * @param keep2
 * 2nd keep value.
 * @param which_dim
 * dimension for filter.
 * @param merge
 * merge or replace flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_filter_alternating
(
    pattern*            pat,
    logical             keep1,
    logical             keep2,
    int                 which_dim,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying an alternating scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies an alternating scale to an existing pattern, merging
 * with any existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * arguments <tt>scale1</tt> and <tt>scale2</tt> give the alternating scale values.
 * The integer <tt>which_dim</tt> specifies the dimension
 * in which the scaling is applied. The position <tt>root</tt> specifies the neutral point
 * about which the scaling takes place (i.e., the point on the seed entity that
 * remains fixed while the entity's dimensions are altered). All scale values must
 * be greater than zero.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * double scale1 = 0.8;
 * double scale2 = 1.2;
 * int which_dim = 1;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_alternating(pat,  scale1, scale2, which_dim, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> A scale value is negative or zero.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param scale1
 * 1st scale value.
 * @param scale2
 * 2nd scale value.
 * @param which_dim
 * dimension for scaling.
 * @param root
 * position about which scaling is applied.
 * @param merge
 * merge or replace flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_alternating
(
    pattern*            pat,
    double              scale1,
    double              scale2,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying an alternating scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies an alternating scale to an existing pattern, merging
 * with any existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * arguments <tt>scale1</tt> and <tt>scale2</tt> give the alternating scale values, which
 * are given as vectors to allow for nonuniform scaling. The integer <tt>which_dim</tt> specifies the dimension
 * in which the scale is applied. The position <tt>root</tt> specifies the neutral point
 * about which the scaling takes place (i.e., the point on the seed entity that
 * remains fixed while the entity's dimensions are altered). All scale vectors must
 * have exclusively positive components.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_rings = 4;
 * double spacing = 4.0;
 * SPAvector& start = *(SPAvector*)NULL_REF;
 * logical not_rotate = FALSE;
 * logical hex_symmetry = TRUE;
 * check_outcome(result = api_pattern_create_polar_grid(pat, center, normal, 
 *     num_rings, spacing, start, not_rotate, hex_symmetry));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Modify the pattern
 * SPAvector scale1(1, 2, 4);
 * SPAvector scale2(1, 4, 1);
 * int which_dim = 0;
 * SPAposition root = center - SPAvector(0, 0, height / 2.0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_alternating(pat, scale1, scale2, which_dim, root));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> A specified scale vector has a component that is zero or negative.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param scale1
 * 1st scale value.
 * @param scale2
 * 2nd scale value.
 * @param which_dim
 * scaling dimension.
 * @param root
 * position about which scaling is applied.
 * @param merge
 * merge/replace flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_alternating
(
    pattern*            pat,
    const SPAvector&    scale1,
    const SPAvector&    scale2,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a linear scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a linear scale, from <tt>first_scale</tt> to <tt>last_scale</tt>,
 * to an existing
 * pattern, merging with any existing scaling or, optionally (with <tt>merge = FALSE</tt>),
 * replacing it. The integer <tt>which_dim</tt> specifies the dimension in which the scale
 * is applied. The position <tt>root</tt> specifies the neutral point about which the
 * scaling takes place (i.e., the point on the seed entity that remains fixed while
 * the entity's dimensions are altered). Both <tt>first_scale</tt> and <tt>last_scale</tt> must be
 * greater than zero.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * double begin_scale = 0.5;
 * double end_scale = 2.0;
 * int which_dim = 0;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_linear(pat, begin_scale, end_scale, which_dim, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param first_scale
 * first scale.
 * @param last_scale
 * second scale.
 * @param which_dim
 * dimension for scaling.
 * @param root
 * position for scaling.
 * @param merge
 * merge flag acis options.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_linear
(
    pattern*            pat,
    double              first_scale,
    double              last_scale,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a linear scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a linear scale, from <tt>first_scale</tt> to <tt>last_scale</tt> (which
 * are given as vectors to allow for nonuniform scaling), to an existing
 * pattern, merging with any existing scaling or, optionally (with <tt>merge = FALSE</tt>),
 * replacing it. The integer <tt>which_dim</tt> specifies the dimension in which the scale
 * is applied. The position <tt>root</tt> specifies the neutral point about which the
 * scaling takes place (i.e., the point on the seed entity that remains fixed while
 * the entity's dimensions are altered). Both <tt>first_scale</tt> and <tt>last_scale</tt>
 * must have exclusively positive components.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a block
 * SPAposition pt1(0, 0, 0);
 * SPAposition pt2(80, 80, 10);
 * BODY* block = NULL;
 * check_outcome(result = api_solid_block(pt1, pt2, block));
 *
 * // Create a hole within the body
 * double sep = 80 / 4;
 * SPAposition bottom (sep, sep, -10);
 * SPAposition top(sep, sep, 20);
 * double maj_rad = 4;
 * double min_rad = 4;
 * BODY* tool = NULL;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, tool));
 * check_outcome(result = api_subtract(tool, block));
 *
 * // Create a pattern
 * pattern* pat = NULL; 
 * SPAvector x_vec(sep, 0, 0);
 * int num_x = 3;
 * SPAvector y_vec(0, sep, 0);
 * int num_y = 3;
 * check_outcome(result = api_pattern_create_linear(
 *     pat, x_vec, num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * pattern* pat2 = NULL;
 * SPAvector first_scale(1, 1, 1);
 * double sqrt2 = sqrt(2.0);
 * SPAvector last_scale(sqrt2, sqrt2, 1);
 * int which_dim = 0;
 * SPAposition root(sep, sep, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_linear(pat, first_scale, last_scale, which_dim, root));
 *
 * // Modify the pattern again
 * which_dim = 1;
 * check_outcome(result = 
 *     api_pattern_modify_scale_linear(pat, first_scale, last_scale, which_dim, root));
 *
 * // Apply the pattern to a face
 * ENTITY_LIST faces;
 * check_outcome(result = api_get_faces(block, faces));
 * FACE* face = (FACE*)faces[0];
 * check_outcome(result = api_pattern_apply_to_entity(face, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param first_scale
 * first scale.
 * @param last_scale
 * second scale.
 * @param which_dim
 * dimension for scaling.
 * @param root
 * position for scaling.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_linear
(
    pattern*            pat,
    const SPAvector&    first_scale,
    const SPAvector&    last_scale,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a periodic keep-filter to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a periodic keep-filter to an existing pattern, merging
 * with any existing keep law or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * argument <tt>keep</tt> is the Boolean list of successive keep values, so that the size of
 * the list (<tt>period</tt>) is the periodicity of the filter. The integer <tt>which_dim</tt>
 * specifies the dimension within which the filter is applied.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 6;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 12;
 * SPAvector z_vec(0, 0, 3.0);
 * int num_z = 4;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y, z_vec, num_z));
 *
 * // Modify the pattern
 * logical keep[3];
 * keep[0] = TRUE;
 * keep[1] = TRUE;
 * keep[2] = FALSE;
 * int which_dim = 1;
 * check_outcome(result = 
 *     api_pattern_modify_filter_periodic(pat, keep, 3, which_dim));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * modified pattern.
 * @param keep
 * array of keep values.
 * @param period
 * number of keep values.
 * @param which_dim
 * dimension for filter.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_filter_periodic
(
    pattern*            pat,
    const logical*      keep,
    int                 period,
    int                 which_dim,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a periodic scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a periodic scale to an existing pattern, merging with
 * any existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * argument <tt>scale</tt> is the list of the successive scale values, so that the size of the list
 * (<tt>period</tt>) is the periodicity of the scaling pattern. The integer <tt>which_dim</tt>
 * specifies the dimension in which the scale is applied. The position <tt>root</tt>
 * specifies the neutral point about which the scaling takes place (i.e., the point
 * on the seed entity that remains fixed while the entity's dimensions are altered).
 * All scale values in the list must be greater than zero.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * double scale[4];
 * scale[0] = 0.5;
 * scale[1] = 1.5;
 * scale[2] = 1.0;
 * scale[3] = 2.0;
 * int period = 4;
 * int which_dim = 0;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_periodic(pat, scale, period, which_dim, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param scale
 * array of scale values.
 * @param period
 * number of scale values.
 * @param which_dim
 * dimension for scaling.
 * @param root
 * base position.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_periodic
(
    pattern*            pat,
    const double*       scale,
    int                 period,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a periodic scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a periodic scale to an existing pattern, merging with
 * any existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * argument <tt>scale</tt> is the list of the successive scale values (given as a
 * vector list to allow for nonuniform scaling), so that the size of the list
 * (<tt>period</tt>) is the periodicity of the scaling pattern. The integer <tt>which_dim</tt>
 * specifies the dimension in which the scale is applied. The position <tt>root</tt>
 * specifies the neutral point about which the scaling takes place (i.e., the point
 * on the seed entity that remains fixed while the entity's dimensions are altered).
 * All scale vectors in the list must have exclusively positive components.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAposition center(5, 0, 0);
 * SPAvector normal(0, 1, 0);
 * int num_angular = 12;
 * int num_axial = 11;
 * double spacing = 2.0;
 * check_outcome(result = api_pattern_create_cylindrical(pat, center, 
 *     normal, num_angular, num_axial, spacing));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Rotate the prism
 * double angle = M_PI / 2.0;
 * SPAvector axis(1, 0, 0);
 * SPAtransf rotation = rotate_transf(angle, axis);
 * check_outcome(result = api_transform_entity(prism, rotation));
 *
 * // Modify the pattern
 * SPAvector scale[4];
 * scale[0] = SPAvector(1, 1, 1);
 * scale[1] = SPAvector(2, 1, 1);
 * scale[2] = SPAvector(1, 1, 1);
 * scale[3] = SPAvector(0.5, 1, 1);
 * int period = 4;
 * int which_dim = 1;
 * SPAposition root(0.5, 0, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_periodic(pat, scale, period, which_dim, root));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param scale
 * array of scale values.
 * @param period
 * number of scale values.
 * @param which_dim
 * dimension for scaling.
 * @param root
 * base position.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_periodic
(
    pattern*            pat,
    const SPAvector*    scale,
    int                 period,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a random keep-filter to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a random keep-filter to an existing pattern, merging
 * with any existing filter or, optionally (with <tt>merge = FALSE</tt>), replacing it. The
 * argument <tt>fraction</tt> determines the fraction of pattern elements that are kept.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 12;
 * SPAvector y_vec(0, 4.0, 0);
 * int num_y = 12;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * double keep_fraction = 0.5;
 * check_outcome(result = api_pattern_modify_filter_random(pat, keep_fraction));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * modified pattern.
 * @param keep_fraction
 * approximate fraction of elements to keep.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_filter_random
(
    pattern*            pat,
    double              keep_fraction,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by adding random offsets to an existing pattern.
 * <br><br>
 * <b>Role:</b> Creates a new pattern by adding random offsets at each site of
 * an existing pattern. The components of the <tt>amplitude</tt> argument specify the
 * magnitudes of the maximum offsets that are imposed in each dimension.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * SPAvector amplitude(1.0, 0.5, 4.0);
 * check_outcome(result = api_pattern_modify_offset_random(pat, amplitude));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * modified pattern.
 * @param amplitude
 * maximum displacements in 3 dimensions.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_offset_random
(
    pattern*            pat,
    const SPAvector&    amplitude,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying random rotations at each site of an existing pattern.
 * <br><br>
 * <b>Role:</b> Creates a new pattern by applying random rotations at each site
 * of an existing pattern, using <tt>root</tt> as the position on the seed entity about
 * which the rotation is to occur. The default arguments yield a totally random
 * rotation. If the user specifies the <tt>tilt_dir</tt> and/or <tt>axial_dir</tt> arguments, the
 * former gives the direction about which the interval <tt>tilt_range</tt> is applied, while
 * the latter gives the direction about which the interval <tt>axial_range</tt> is applied.
 * If the <tt>tilt_dir</tt> argument is not orthogonal to <tt>axial_dir</tt>, only its orthogonal
 * component is used.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * SPAposition root(0, 0, 0);
 * SPAinterval axial_range(0, 2 * M_PI);
 * SPAvector axial_dir(1, 0, 0);
 * SPAinterval tilt_range(0, 0);
 * SPAvector tilt_dir(0, 0, 1);
 * check_outcome(result = 
 *     api_pattern_modify_orient_random(pat, root, axial_range, axial_dir, tilt_range, tilt_dir));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param root
 * root position.
 * @param axial_range
 * range of axial rotation angles.
 * @param axial_dir
 * axis for tilt.
 * @param tilt_range
 * tilt range.
 * @param tilt_dir
 * tilt direction.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_orient_random
(
    pattern*            pat,
    const SPAposition&  root            = SPAposition(0, 0, 0),
    const SPAinterval&  axial_range     = SPAinterval(0.0, 2.0 * M_PI),
    const SPAvector&    axial_dir       = *(SPAvector*)NULL_REF,
    const SPAinterval&  tilt_range      = SPAinterval(0, M_PI),
    const SPAvector&    tilt_dir        = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a random scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a random scale to an existing pattern, merging with any
 * existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The arguments
 * <tt>min_scale</tt> and <tt>max_scale</tt> place limits upon the scale values. The
 * integer <tt>which_dim</tt> specifies the dimension in
 * which the scaling is applied. The position <tt>root</tt> specifies the neutral point about
 * which the scaling takes place (i.e., the point on the seed entity that remains
 * fixed while the entity's dimensions are altered). Both <tt>min_scale</tt> and <tt>max_scale</tt>
 * must be greater than zero.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre>
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(4.0, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 2.0, 0);
 * int num_y = 10;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * double min_scale = 0.5;
 * double max_scale = 2.0;
 * SPAposition root(0, 0, 0);
 * check_outcome(result = 
 *     api_pattern_modify_scale_random(pat, min_scale, max_scale, root));
 *
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 *
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> One or more of the specified scaling factors is zero or negative.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param min_scale
 * lower bound to the applied scale values.
 * @param max_scale
 * upper bound to the applied scale values.
 * @param root
 * root position.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_random
(
    pattern*            pat,
    double              min_scale,
    double              max_scale,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Creates a new pattern by applying a random scale to an existing pattern.
 * <br><br>
 * <b>Role:</b> Applies a random scale to an existing pattern, merging with any
 * existing scaling or, optionally (with <tt>merge = FALSE</tt>), replacing it. The arguments
 * <tt>min_scale</tt> and <tt>max_scale</tt> place limits upon the scale values, and are given as
 * vectors to allow for nonuniform scaling. The integer <tt>which_dim</tt> specifies the dimension in
 * which the scale is applied. The position <tt>root</tt> specifies the neutral point about
 * which the scaling takes place (i.e., the point on the seed entity that remains
 * fixed while the entity's dimensions are altered). Both <tt>min_scale</tt> and <tt>max_scale</tt>
 * must have exclusively positive components.
 * <br><br>
 * The following code snippet shows an example of how this API can be used.
 * <pre><br>
 * // Create a block
 * SPAposition pt1(-15, -15, -5);
 * SPAposition pt2(90, 90, 5);
 * BODY* block = NULL;
 * check_outcome(result = api_solid_block(pt1, pt2, block));
 *
 * // Create a hole within the block
 * pt1 = SPAposition(-5, -5, -5);
 * pt2 = SPAposition(5, 5, 5);
 * BODY* tool = NULL;
 * check_outcome(result = api_solid_block(pt1, pt2, tool));
 * check_outcome(result = api_subtract(tool, block));
 *
 * // Create a pattern
 * pattern* pat = NULL;
 * SPAvector x_vec(25, 0, 0);
 * int num_x = 4;
 * SPAvector y_vec(0, 25, 0);
 * int num_y = 4;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 *
 * // Modify the pattern
 * SPAvector scale1(0.5, 0.2, 1.0);
 * SPAvector scale2(2.0, 1.0, 1.0);
 * SPAposition root(0, 0, 0);
 * check_outcome(result = api_pattern_modify_scale_random(pat, scale1, scale2, root));
 *
 * // Apply the pattern to the hole
 * ENTITY_LIST faces;
 * check_outcome(result = api_get_faces(block, faces));
 * FACE* face = (FACE*)faces[0];
 * check_outcome(result = api_pattern_apply_to_entity(face, pat));
 *
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> One or more of the specified scaling vectors has a component that is negative or zero.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern to be modified.
 * @param min_scale
 * lower bound to the applied scale values.
 * @param max_scale
 * upper bound to the applied scale values.
 * @param root
 * root position.
 * @param merge
 * merge flag.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_scale_random
(
    pattern*           pat,
    const SPAvector&    min_scale,
    const SPAvector&    max_scale,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Finds the bump associated with a given face or loop.
 * <br><br>
 * <b>Role:</b> Finds the bump associated with the face or loop specified by
 * <tt>seed</tt>, and returns a list of its faces and loops in <tt>loops</tt> and <tt>faces</tt>. The
 * extent of the bump's definition may be limited by including a <tt>no_cross_list</tt> of
 * faces.
 * <br><br>
 * <b>Errors:</b> The <tt>seed</tt> entity used to find the bump is neither a face nor a loop.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param seed
 * input face or lump.
 * @param faces
 * faces belonging to bump.
 * @param loops
 * loops belonging to bump.
 * @param no_cross_list
 * faces limiting bump's extent.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_find_bump(
    const ENTITY*       seed,
    ENTITY_LIST&        faces,
    ENTITY_LIST&        loops,
    ENTITY_LIST&        no_cross_list   = *(ENTITY_LIST*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * Returns the array of transforms associated with a specified pattern.
 * <br><br>
 * <b>Role:</b> Allocates and returns the array of transforms connecting the first element
 * of the pattern <tt>pat</tt> to each of its other elements. The argument <tt>num</tt>
 * returns the size of the <tt>transforms</tt> array (i.e., the number of pattern elements
 * minus one). The caller is responsible for deleting the array when it is no longer needed.
 * <br><br>
 * <b>Errors:</b> The number of elements in the pattern is less than two.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose transforms are desired.
 * @param transforms
 * the array of transforms.
 * @param num
 * the number of transforms (i.e., the array size).
 * @param ao
 * acis options.
 **/

DECL_KERN outcome api_pattern_query_transformations(
    const pattern*  pat,
    SPAtransf*&     transforms,
    int&            num,
	AcisOptions*    ao          = NULL);

/**
 * Suppresses an element, specified by its index, of a given pattern.
 * <br><br>
 * <b>Role:</b> Returns FALSE if the element has already been removed.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a cylinder
 * SPAposition bottom (0, 0, 0);
 * SPAposition top(0, 0, -5);
 * double maj_rad = 30;
 * double min_rad = 30;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, cylinder));
 * 
 * // Create a hole in the cylinder
 * BODY* tool = NULL;
 * bottom = SPAposition(0, 0, 5);
 * top = SPAposition(0, 0, -10);
 * maj_rad = 2;
 * min_rad = 2;
 * check_outcome(result = api_solid_cylinder_cone(
 *     bottom, top, maj_rad, min_rad, maj_rad, NULL, tool));
 * check_outcome(result = api_subtract(tool, cylinder));
 * 
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_radial = 4;
 * int num_angular = 6;
 * double spacing = 8.0;
 * check_outcome(result = api_pattern_create_radial(pat, center, normal,
 *     num_radial, num_angular, spacing));
 * 
 * // Modify the pattern
 * int index = 0;
 * check_outcome(result = api_pattern_modify_element_remove(pat, index));
 * 
 * // Apply the pattern to a face
 * ENTITY_LIST faces;
 * check_outcome(result = api_get_faces(cylinder, faces));
 * FACE* face = (FACE*)faces[0];
 * check_outcome(result = api_pattern_apply_to_entity(face, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param index
 * index of the pattern element to be suppressed.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_remove(
    pattern*            pat,
    int                 index,
 	AcisOptions*        ao          = NULL);

/**
 * Suppresses an element, specified by its pattern coordinates, of a given pattern.
 * <br><br>
 * <b>Role:</b> Returns FALSE if the element has already been removed.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int number = 5;
 * double spacing = 5.0;
 * check_outcome(result = api_pattern_create_polar_grid(pat, center, normal, 
 *     number, spacing));
 * 
 * // Modify the pattern	
 * double coords[3];
 * coords[0] = 0;
 * coords[1] = 0;
 * coords[2] = 0;
 * check_outcome(result = api_pattern_modify_element_remove(pat, coords));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height,
 *     maj_rad, min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * </pre>
 * <b>Errors:</b> The specified pattern is NULL, or a NULL coordinate array is specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param coords
 * pattern coordinates of the element to be suppressed.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_remove(
    pattern*            pat,
    const double*       coords,
 	AcisOptions*        ao          = NULL);

/**
 * Restores an element, specified by its index, to a given pattern.
 * <br><br>
 * <b>Role:</b> When an element has been suppressed from a pattern, either individually
 * via <tt>api_pattern_modify_element_remove</tt> or by the application of a keep law, this function
 * can restore that element to the pattern.  It returns FALSE if the element is already
 * present in the pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param index
 * index of the pattern element to be restored.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_restore(
    pattern*            pat,
    int                 index,
 	AcisOptions*        ao          = NULL);

/**
 * Restores an element, specified by its pattern coordinates, to a given pattern.
 * <br><br>
 * <b>Role:</b> When an element has been suppressed from a pattern, either individually
 * via <tt>api_pattern_modify_element_remove</tt> or by the application of a keep law, this function
 * can restore that element to the pattern. It returns FALSE if the element is already
 * present in the pattern.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAvector x_vec(2, 0, 0);
 * int num_x = 4;
 * SPAvector y_vec(0, 3, 0);
 * int num_y = 4;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 * 
 * // Modify the pattern 	
 * law* keep = NULL; 
 * check_outcome(result =  api_str_to_law("X + Y &lt; 4", &keep));
 * check_outcome(result = api_pattern_modify_filter(pat, keep));
 * 
 * // Modify the pattern again	
 * double coords[3];
 * coords[0] = 3;
 * coords[1] = 3;
 * coords[2] = 0;
 * check_outcome(result = api_pattern_modify_element_restore(pat, coords));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height,
 *     maj_rad, min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * keep-&gt;remove();
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL, or a NULL coordinate array is specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param coords
 * pattern coordinates of the element to be restored.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_restore(
    pattern*            pat,
    const double*       coords,
 	AcisOptions*        ao          = NULL);

/**
 * Transforms an element, specified by its index, of a given pattern.
 * <br><br>
 * <b>Role:</b> Returns FALSE if the given transform is the identity.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param index
 * index of the pattern element to be moved.
 * @param move
 * transform used to move the element.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_transform(
    pattern*            pat,
    int                 index,
    const SPAtransf&    move,
 	AcisOptions*        ao          = NULL);

/**
 * Transforms an element, specified by its pattern coordinates, of a given pattern.
 * <br><br>
 * <b>Role:</b> Returns FALSE if the given transform is the identity.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAvector x_vec(2, 0, 0);
 * int num_x = 8;
 * SPAvector y_vec(0, 3, 0);
 * int num_y = 8;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 * 
 * // Modify the pattern	
 * double coords[3];
 * coords[0] = 3;
 * coords[1] = 2;
 * coords[2] = 0;
 * SPAvector disp(0.4, 1.2, 0.5);
 * SPAtransf move = translate_transf(disp);
 * check_outcome(result = api_pattern_modify_element_transform(pat, coords, move));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height,
 *     maj_rad, min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL, or a NULL coordinate array is specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param coords
 * pattern coordinates of the element to be moved.
 * @param move
 * transform used to move the element.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_transform(
    pattern*            pat,
    const double*       coords,
    const SPAtransf&    move,
 	AcisOptions*        ao          = NULL);

/**
 * Uniformly scales an element, specified by its index, of a given pattern.
 * <br><br>
 * <b>Role:</b> The scaling of the element indicated by <tt>coords</tt> has
 * <tt>root_position</tt> as its fixed point.  If <tt>merge</tt> is FALSE, the scale
 * replaces any existing scaling rather than merging with it.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param index
 * index of the pattern element to be scaled.
 * @param scale
 * scale value applied to the element.
 * @param root_position
 * position about which the scaling is to be performed.
 * @param merge
 * flag to merge with the existing scale (TRUE) or to replace it (FALSE).
 * @param ao
 * acis options
**/
DECL_KERN outcome api_pattern_modify_element_scale(
    pattern*            pat,
    int                 index,
    double              scale,
    const SPAposition&  root_position,
    logical             merge       = TRUE,
 	AcisOptions*        ao          = NULL);

/**
 * Uniformly scales an element, specified by its pattern coordinates, of a given pattern.
 * <br><br>
 * <b>Role:</b> The scaling of the element indicated by <tt>coords</tt> has
 * <tt>root_position</tt> as its fixed point.  If <tt>merge</tt> is FALSE, the scale
 * replaces any existing scaling rather than merging with it.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_radial = 3;
 * int num_angular = 5;
 * double spacing = 3.0;
 * check_outcome(result = api_pattern_create_radial(pat, center, normal, 
 *     num_radial, num_angular, spacing));
 * 
 * // Modify the pattern	
 * double coords[3];
 * coords[0] = 0;
 * coords[1] = 0;
 * coords[2] = 0;
 * double scale = 1.5;
 * SPAposition root(0, 0, 2);
 * check_outcome(result = api_pattern_modify_element_scale(pat, coords, scale, root));
 * 
 * // Create a spherical face upon a block
 * SPAposition pt1(-10, -10, 0);
 * SPAposition pt2(10, 10, 2);
 * BODY* block = NULL;
 * check_outcome(result = api_solid_block(pt1, pt2, block));
 * center = SPAposition(0, 0, 2);
 * double radius = 1.0;
 * BODY* sph = NULL;
 * check_outcome(result = api_solid_sphere(center, radius, 
 *     sph));
 * check_outcome(result = api_unite(sph, block));
 * 
 * // Color the bump
 * ENTITY_LIST faces;
 * check_outcome(result = api_get_faces(block, faces));
 * 
 * // Apply the pattern to the face
 * FACE* face = (FACE*)faces[0];
 * check_outcome(result = api_pattern_apply_to_entity(face, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL, a NULL coordinate array is specified, or
 * invalid coordinates are specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param coords
 * pattern coordinates of the element to be scaled.
 * @param scale
 * scale value applied to the element.
 * @param root_position
 * position about which the scaling is to be performed.
 * @param merge
 * flag to merge with the existing scale (TRUE) or to replace it (FALSE).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_scale(
    pattern*            pat,
    const double*       coords,
    double              scale,
    const SPAposition&  root_position,
    logical             merge       = TRUE,
 	AcisOptions*        ao          = NULL);

/**
 * Non-uniformly scales an element, specified by its index, of a given pattern.
 * <br><br>
 * <b>Role:</b> The scaling of the element indicated by <tt>index</tt> has
 * <tt>root_position</tt> as its fixed point.  If <tt>merge</tt> is FALSE, the scale
 * replaces any existing scaling rather than merging with it.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param index
 * index of the pattern element to be scaled.
 * @param scale
 * scale vector applied to the element.
 * @param root_position
 * position about which the scaling is to be performed.
 * @param merge
 * flag to merge with the existing scale (TRUE) or to replace it (FALSE).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_scale(
    pattern*            pat,
    int                 index,
    const SPAvector&    scale,
    const SPAposition&  root_position,
    logical             merge       = TRUE,
 	AcisOptions*        ao          = NULL);

/**
 * Non-uniformly scales an element, specified by its pattern coordinates, of a given pattern.
 * <br><br>
 * <b>Role:</b> The scaling of the element indicated by <tt>coords</tt> has
 * <tt>root_position</tt> as its fixed point.  If <tt>merge</tt> is FALSE, the scale
 * replaces any existing scaling rather than merging with it.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL; 
 * SPAposition center(0, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num_radial = 7;
 * int num_angular = 4;
 * double spacing = 3.0;
 * check_outcome(result = api_pattern_create_radial(pat, center, normal, 
 *     num_radial, num_angular, spacing));
 * 
 * // Modify the pattern	
 * double coords[3];
 * coords[0] = 0;
 * coords[1] = 0;
 * coords[2] = 0;
 * SPAposition root(0, 0, 0);
 * SPAvector scale(2, 4, 8);
 * check_outcome(result = api_pattern_modify_element_scale(pat, coords, scale, root));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height,
 *     maj_rad, min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL, a NULL coordinate array is specified, or
 * invalid coordinates are specified.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be modified.
 * @param coords
 * pattern coordinates of the element to be scaled.
 * @param scale
 * scale vector applied to the element.
 * @param root_position
 * position about which the scaling is to be performed.
 * @param merge
 * flag to merge with the existing scale (TRUE) or to replace it (FALSE).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_element_scale(
    pattern*            pat,
    const double*       coords,
    const SPAvector&    scale,
    const SPAposition&  root_position,
    logical             merge       = TRUE,
 	AcisOptions*        ao          = NULL);

/**
 * Returns a pattern that is a specified transform of a given pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be transformed.
 * @param in_transf
 * the transform to apply to the input pattern.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_transform(
    pattern*            pat,
    const SPAtransf&    in_transf,
	AcisOptions*        ao          = NULL);

/**
 * Returns a pattern that is a mirror of a given pattern.
 * <br><br>
 * <b>Role:</b> The mirror of a pattern is the concatenation of a pattern with its reflection.
 * The reflecting plane is defined by the user-supplied root position and normal.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL;
 * SPAtransf t[5];
 * t[0] = translate_transf(SPAvector(0, 0, 0));
 * t[1] = translate_transf(SPAvector(2, 2, 0));
 * t[2] = translate_transf(SPAvector(4, 0, 0));
 * t[3] = translate_transf(SPAvector(0, 4, 0));
 * t[4] = translate_transf(SPAvector(4, 4, 0));
 * check_outcome(result = api_pattern_create_from_list(pat, t, 5));
 * 
 * // Mirror the pattern	
 * SPAposition root(0, 0, 0);
 * SPAvector normal = SPAvector(1, 1, 0);
 * check_outcome(result = api_pattern_modify_mirror(pat, root, normal));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Position the prism
 * SPAtransf move = translate_transf(SPAvector(1, 2, 3));
 * check_outcome(result = api_transform_entity(prism, move));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be mirrored.
 * @param root
 * position on the mirror plane.
 * @param normal
 * normal to the mirror plane.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_mirror(
    pattern*            pat,
    const SPAposition&  root,
    const SPAvector&    normal,
	AcisOptions*        ao          = NULL);

/**
 * Returns a pattern that is a reflection of a given pattern.
 * <br><br>
 * <b>Role:</b> The reflecting plane is defined by the user-supplied root position and normal.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat1 = NULL; 
 * SPAposition center1(10, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int number = 8;
 * logical not_rotate = FALSE;
 * SPAposition root(1, 0, 0);
 * double angle = M_PI;
 * check_outcome(result = api_pattern_create_elliptical(pat1, center1, 
 *     normal, number, not_rotate, root, angle));
 * 
 * // Reflect the pattern	
 * root = SPAposition(0, 0, 0);
 * normal = SPAvector(1, 0, 0);
 * pattern* pat2 = NULL;
 * check_outcome(result = api_pattern_copy(pat2, pat1));
 * check_outcome(result = api_pattern_modify_reflect(pat2, root, normal));
 * 
 * // Create an irregular "prism"
 * int length_pnts = 4;
 * SPAposition array_pts[4];
 * BODY* wire = NULL;
 * array_pts[0] = SPAposition(1, 0, 0);
 * array_pts[1] = SPAposition(2, 0, 0);
 * array_pts[2] = SPAposition(1, 2.5, 0);
 * array_pts[3] = SPAposition(1, 0, 0);
 * check_outcome(result = api_make_wire(NULL, length_pnts, 
 *     array_pts, wire));
 * ENTITY_LIST faces;
 * check_outcome(result = api_cover_wires(wire, *(surface*)NULL_REF, faces));
 * SPAvector path(0.5, 0.5, 0.5);
 * sweep_options options;
 * BODY* prism1 = NULL;
 * check_outcome(result = api_sweep_with_options(faces[0], path, &options, prism1));
 * check_outcome(result = api_get_owner(faces[0], (ENTITY*&)prism1));
 * 
 * // Copy the prism
 * BODY* prism2 = NULL;
 * check_outcome(result = api_copy_body(prism1, prism2));
 * 
 * // Apply the patterns to the prisms
 * check_outcome(result = api_pattern_apply_to_entity(prism1, pat1));
 * check_outcome(result = api_pattern_apply_to_entity(prism2, pat2));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat1));
 * check_outcome(result = api_pattern_destroy(pat2));</pre>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be reflected.
 * @param root
 * position on the plane of reflection.
 * @param normal
 * normal to the mirror plane.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_reflect(
    pattern*            pat,
    const SPAposition&  root,
    const SPAvector&    normal,
	AcisOptions*        ao          = NULL);

/**
 * Returns a pattern that is the composition of two given patterns.
 * <br><br>
 * <b>Role:</b> The result is a single pattern whose effect is equivalent to applying
 * the first pattern (<tt>pat1</tt>) followed by the second (<tt>pat2</tt>) to the
 * seed pattern element.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat1 = NULL; 
 * SPAvector x_vec1(10, 0, 0);
 * int num_x1 = 4;
 * SPAvector y_vec1(0, 10, 0);
 * int num_y1 = 5;
 * check_outcome(result = api_pattern_create_linear(pat1, x_vec1, 
 *     num_x1, y_vec1, num_y1));
 * 
 * // Create another pattern	
 * pattern* pat2 = NULL; 
 * SPAvector x_vec2(100, 0, 0);
 * int num_x2 = 3;
 * SPAvector y_vec2(0, 100, 0);
 * int num_y2 = 2;
 * check_outcome(result = api_pattern_create_linear(pat2, x_vec2, 
 *     num_x2, y_vec2, num_y2));
 * 
 * // Compose the patterns	
 * check_outcome(result = api_pattern_modify_compose(pat1, pat2));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat1));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat1));
 * check_outcome(result = api_pattern_destroy(pat2));</pre>
 * <b>Errors:</b> Either of the specified patterns is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param inner_pat
 * the pattern to be modified.
 * @param outer_pat
 * the outer pattern to be applied to inner_pat.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_compose(
    pattern*        inner_pat,
    const pattern*  outer_pat,
	AcisOptions*    ao          = NULL);

/**
 * Returns a pattern that is the concatenation of two given patterns.
 * <br><br>
 * <b>Role:</b> The result is a single pattern whose size is the sum of the
 * number of elements of the two input patterns, <tt>pat1</tt> and <tt>pat2</tt>.
 * the effect is that of applying the first pattern to the first set of elements
 * and the second pattern to the rest. Optionally, the tail of the first set
 * may be connected to the head of the second set by supplying a transform
 * <tt>in_trans</tt>.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat1 = NULL; 
 * SPAvector x_vec1(3, 0, 0);
 * int num_x1 = 5;
 * check_outcome(result = api_pattern_create_linear(pat1, x_vec1, num_x1));
 * 
 * // Create another pattern	
 * pattern* pat2 = NULL; 
 * SPAposition center(-5, 0, 0);
 * SPAvector normal(0, 0, 1);
 * int num = 8;
 * check_outcome(result = api_pattern_create_elliptical(pat2, center, normal, num));
 * 
 * // Concatenate the patterns	
 * check_outcome(result = api_pattern_modify_concatenate(pat1, pat2));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat1));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat1));
 * check_outcome(result = api_pattern_destroy(pat2));</pre>
 * <b>Errors:</b> Either of the specified patterns is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param head_pat
 * the pattern at the head of the concatenation.
 * @param tail_pat
 * the pattern at the tail of the concatenation.
 * @param in_trans
 * transform by which head_pat and tail_pat are related (optional).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_concatenate(
    pattern*            head_pat,
    const pattern*      tail_pat,
    const SPAtransf&    in_trans    = *(const SPAtransf*)NULL_REF,
	AcisOptions*        ao          = NULL);

/**
 * Creates a displacement pattern from an array of positions.
 * <br><br>
 * <b>Role:</b> The positions in the array <tt>in_positions</tt> are considered in
 * a <i>relative</i> sense only. That is, the displacement from element <i>n</i> to
 * element <i>(n + 1)</i> is determined by the vector
 * <tt>in_positions[n + 1] - in_positions[n]</tt>. If one wants the seed element to
 * move from its original position, it is necessary to specify in addition the
 * root position via <tt>in_root_position</tt>, which defines the associated displacement
 * of the seed.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * SPAposition pos[9];
 * pos[0] = SPAposition(0, 0, 3);
 * pos[1] = SPAposition(0, 6, 3);
 * pos[2] = SPAposition(6, 0, 3);
 * pos[3] = SPAposition(6, 6, 3);
 * pos[4] = SPAposition(3, 3, 0);
 * pos[5] = SPAposition(0, 0, -3);
 * pos[6] = SPAposition(0, 6, -3);
 * pos[7] = SPAposition(6, 0, -3);
 * pos[8] = SPAposition(6, 6, -3);
 * pattern* pat = NULL;
 * check_outcome(result = api_pattern_create_from_list(pat, pos, 9));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the created pattern.
 * @param in_positions
 * the array of positions defining the pattern.
 * @param num_positions
 * the number of elements in the <tt>in_positions</tt> array.
 * @param in_root_position
 * the root position of the pattern (optional).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_from_list(
    pattern*&           pat,
    const SPAposition*  in_positions,
    int                 num_positions,
    const SPAposition&  in_root_position    = *(const SPAposition*)NULL_REF,
	AcisOptions*        ao                  = NULL);

/**
 * Creates a pattern from an array of transforms.
 * <br><br>
 * <b>Role:</b> The transforms in the array <tt>in_transforms</tt> are considered in
 * a <i>relative</i> sense only. That is, the transform from element <i>n</i> to
 * element <i>(n + 1)</i> is determined by the product
 * <tt>in_transforms[n].inverse() * in_transforms[n + 1]</tt>. If one wants the seed element
 * to be transformed, it is necessary to specify in addition the
 * root transform via <tt>in_root_transform</tt>, which defines the transformation of
 * the seed.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * SPAposition origin[9];
 * origin[0] = SPAposition(0, 0, 3);
 * origin[1] = SPAposition(0, 6, 3);
 * origin[2] = SPAposition(6, 0, 3);
 * origin[3] = SPAposition(6, 6, 3);
 * origin[4] = SPAposition(3, 3, 0);
 * origin[5] = SPAposition(0, 0, -3);
 * origin[6] = SPAposition(0, 6, -3);
 * origin[7] = SPAposition(6, 0, -3);
 * origin[8] = SPAposition(6, 6, -3);
 * 
 * SPAunit_vector x_axis[9];
 * x_axis[0] = SPAunit_vector(1, 0, 0);
 * x_axis[1] = SPAunit_vector(1, 0, 0);
 * x_axis[2] = SPAunit_vector(1, 0, 0);
 * x_axis[3] = SPAunit_vector(1, 0, 0);
 * x_axis[4] = SPAunit_vector(0, 1, 0);
 * x_axis[5] = SPAunit_vector(0, 1, 0);
 * x_axis[6] = SPAunit_vector(0, 1, 0);
 * x_axis[7] = SPAunit_vector(0, 1, 0);
 * x_axis[8] = SPAunit_vector(0, 1, 0);
 * 
 * SPAunit_vector y_axis[9];
 * y_axis[0] = SPAunit_vector(0, 1, 0);
 * y_axis[1] = SPAunit_vector(0, 1, 0);
 * y_axis[2] = SPAunit_vector(0, 1, 0);
 * y_axis[3] = SPAunit_vector(0, 1, 0);
 * y_axis[4] = SPAunit_vector(1, 0, 0);
 * y_axis[5] = SPAunit_vector(0, 0, 1);
 * y_axis[6] = SPAunit_vector(0, 0, 1);
 * y_axis[7] = SPAunit_vector(0, 0, 1);
 * y_axis[8] = SPAunit_vector(0, 0, 1);
 * 
 * SPAtransf t[9];
 * 
 * for (int i = 0; i &lt; 9; i++)
 *     t[i] = coordinate_transf(origin[i], x_axis[i], y_axis[i]);
 * 
 * pattern* pat = NULL;
 * SPAposition root(0, 0, 0);
 * SPAvector disp = origin[0] - root;
 * SPAtransf root_transf = translate_transf(disp);
 * check_outcome(result = api_pattern_create_from_list(pat, t, 9, root_transf));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));</pre>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the created pattern.
 * @param in_transforms
 * the array of transforms defining the pattern.
 * @param num_positions
 * the number of elements in the <tt>in_transforms</tt> array.
 * @param in_root_transform
 * the root transform of the pattern (optional).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_from_list(
    pattern*&           pat,
    const SPAtransf*    in_transforms,
    int                 num_transforms,
    const SPAtransf&    in_root_transform   = *(const SPAtransf*)NULL_REF,
	AcisOptions*        ao                  = NULL);

/**
 * Creates a pattern from a set of laws.
 * <br><br>
 * <b>Role:</b> A law pattern may be created from a set of laws specifying translation
 * (<tt>in_trans_law</tt>), rotation (<tt>in_xrot_law</tt>, <tt>in_yrot_law</tt>,
 * <tt>in_zrot_law</tt>), scaling (<tt>in_scale_law</tt>), and filtering
 * (<tt>in_keep_law</tt>), along with an optional transform to be applied to the seed
 * pattern element, <tt>in_root_transform</tt>. Among the supplied laws, at least
 * one must have a <i>domain</i> attached to it - a one-, two-, or three-dimensional
 * set of integers for which pattern transforms will be generated. [For example, an
 * <i>m</i> by <i>n</i> linear grid of pattern elements might take a two-dimensional domain
 * whose indices range from 0 to <i>(m - 1)</i> in one direction and from 0 to <i>(n - 1)</i>
 * in the other.] If the pattern involves no rotation, the rotation laws may be omitted; if
 * there is rotation without reflection, <tt>in_zrot_law</tt> may be omitted. Similarly,
 * the scale and keep laws may be left out if no scaling or filtering is involved. The
 * translation and rotation laws should return three-dimensional values; the keep law should
 * return a logical value. The scale law can return either a one-dimensional value, for
 * uniform scaling, or a three-dimensional value, for non-uniform scaling. Finally, it is
 * important to keep in mind that the pattern transforms are composed in the order <tt>SRT</tt>
 * (scaling first, followed by rotation/reflection, followed by translation).
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern from laws	
 * law* trans_vec = NULL;
 * law* x_vec = NULL;
 * law* y_vec = NULL;
 * check_outcome(result = 
 *     api_str_to_law("DOMAIN(VEC(10*X, 5*Y, 0), 0, 5, 0, 4)", &trans_vec));
 * check_outcome(result = 
 *     api_str_to_law("VEC(COS(X*PI/4), SIN(X*PI/4), 0)", &x_vec));
 * check_outcome(result = 
 *     api_str_to_law("VEC(0,0,1)", &y_vec));
 * pattern* pat = NULL;
 * check_outcome(result = api_pattern_create_from_laws(pat, trans_vec, x_vec, y_vec));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * trans_vec->remove();
 * x_vec->remove();
 * y_vec->remove();</pre>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the created pattern.
 * @param in_trans_law
 * the law defining the pattern translations.
 * @param in_xrot_law
 * the law defining the pattern x-axis rotation.
 * @param in_yrot_law
 * the law defining the pattern y-axis rotation.
 * @param in_zrot_law
 * the law defining the pattern z-axis rotation.
 * @param in_scale_law
 * the law defining the pattern scaling.
 * @param in_keep_law
 * the law defining the pattern filtering.
 * @param in_root_transform
 * the root transform of the pattern.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_create_from_laws(
    pattern*&           pat,
    law*                in_trans_law,
    law*                in_xrot_law         = NULL,
    law*                in_yrot_law         = NULL,
    law*                in_zrot_law         = NULL,
    law*                in_scale_law        = NULL,
    law*                in_keep_law         = NULL,
    const SPAtransf&    in_root_transform   = *(const SPAtransf*)NULL_REF,
	AcisOptions*        ao                  = NULL);

/**
 * Returns a copy of a specified pattern.
 * <br><br>
 * <b>Role:</b> The copy is independent of the original pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param copy_pat
 * the pattern copy (output).
 * @param pat
 * the pattern to be copied (input).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_copy(
    pattern*&           copy_pat,
    const pattern*      pat,
	AcisOptions*        ao                  = NULL);

/**
 * Decrements the use-count of a given pattern and deletes the pattern if it is no longer used.
 * <br><br>
 * <b>Role:</b> Patterns are use-counted. To avoid memory leaks, the user should call
 * this function when a pattern is no longer needed. When the use count drops to zero,
 * this function will destruct the pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to be released.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_destroy(
    pattern*           pat,
	AcisOptions*       ao          = NULL);

/**
 * Returns a pattern that is the result of filtering a given pattern by a keep law.
 * <br><br>
 * <b>Role:</b> After a pattern has been constructed, it may be modified by applying
 * a keep_law to filter out selected elements. The supplied keep law should return
 * a logical value when evaluated, and have a dimensionality that is consistent with
 * the pattern dimensions. It will merge with any existing keep law unless <tt>merge</tt>
 * is set to FALSE, in which case the new keep law will replace the old.
 * <br><br>
 * The following code snippet gives an example of using this API.
 * <pre>
 * // Create a pattern	
 * pattern* pat = NULL;
 * SPAvector x_vec(2, 0, 0);
 * int num_x = 5;
 * SPAvector y_vec(0, 2, 0);
 * int num_y = 5;
 * check_outcome(result = api_pattern_create_linear(pat, x_vec, 
 *     num_x, y_vec, num_y));
 * 
 * // Create a keep law and apply it to the pattern	
 * law* keep1 = NULL; 
 * check_outcome(result = 
 *     api_str_to_law("EVEN?(y)", &keep1));
 * check_outcome(result = api_pattern_modify_filter(pat, keep1));
 * 
 * // Create another keep law and merge it	with the pattern
 * law* keep2 = NULL; 
 * check_outcome(result = 
 *     api_str_to_law("EVEN?(x)", &keep2));
 * check_outcome(result = api_pattern_modify_filter(pat, keep2));
 * 
 * // Create a prism
 * double height = 1.0;
 * double maj_rad = 1.0;
 * double min_rad = 0.5;
 * int num_sides = 3;
 * BODY* prism = NULL;
 * check_outcome(result = api_make_prism(height, maj_rad, 
 *     min_rad, num_sides, prism));
 * 
 * // Apply the pattern to the prism
 * check_outcome(result = api_pattern_apply_to_entity(prism, pat));
 * 
 * // Clean up
 * check_outcome(result = api_pattern_destroy(pat));
 * keep1->remove();
 * keep2->remove();</pre>
 * <b>Errors:</b> The specified pattern is NULL, or the keep law is NULL with merge requested..
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * the pattern to which the keep law is applied.
 * @param keep_law
 * the keep law to be applied.
 * @param merge
 * flag to merge with the existing filter (TRUE) or to replace it (FALSE).
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_filter(
    pattern*        pat,
    law*            keep_law,
    logical         merge       = TRUE,
	AcisOptions*    ao          = NULL);

/**
* Returns a pattern that is the result of scaling a given pattern by a scale law.
* <br><br>
* <b>Role:</b> After a pattern has been constructed, it may be modified by applying
* a <tt>scale_law</tt>. The supplied law should return a one- or three-dimensional value
* when evaluated, depending upon whether uniform or non-uniform scaling is desired.
* It should have a dimensionality that is consistent with the pattern dimensions. The
* new scaling will merge with any existing scaling unless <tt>merge</tt> is set to
* FALSE, in which case the new scaling will replace the old. The scaling will be
* performed about the origin unless an alternative <tt>root_position</tt> is supplied.
* <br><br>
* The following code snippet gives an example of using this API.
* <pre>
* // Create a pattern	
* pattern* pat = NULL;
* SPAvector x_vec(2, 0, 0);
* int num_x = 5;
* SPAvector y_vec(0, 2, 0);
* int num_y = 5;
* check_outcome(result = api_pattern_create_linear(pat, x_vec, 
*     num_x, y_vec, num_y));
* 
* // Create a scale law and apply it to the pattern	
* law* scale1 = NULL; 
* check_outcome(result = 
*     api_str_to_law("(y + 1)/5", &scale1));
* SPAposition root(0, 0, 0);
* check_outcome(result = api_pattern_modify_scale(pat, scale1, root));
* 
* // Create another scale law and merge it with the pattern
* law* scale2 = NULL; 
* check_outcome(result = 
*     api_str_to_law("(5 - x)/5", &scale2));
* check_outcome(result = api_pattern_modify_scale(pat, scale2, root));
* 
* // Create a prism
* double height = 1.0;
* double maj_rad = 1.0;
* double min_rad = 0.5;
* int num_sides = 3;
* BODY* prism = NULL;
* check_outcome(result = api_make_prism(height, maj_rad, 
*     min_rad, num_sides, prism));
* 
* // Apply the pattern to the prism
* check_outcome(result = api_pattern_apply_to_entity(prism, pat));
* 
* // Clean up
* check_outcome(result = api_pattern_destroy(pat));
* scale1->remove();
* scale2->remove();</pre>
* <b>Errors:</b> The specified pattern is NULL, or the scale law is NULL with merge requested.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
* the pattern to which the scale law is applied.
* @param scale_law
* the scale law to be applied.
* @param root_position
* position about which the scaling is to be performed.
* @param merge
* flag to merge with the existing scale (TRUE) or to replace it (FALSE).
* @param ao
* acis options.
**/
DECL_KERN outcome api_pattern_modify_scale(
    pattern*            pat,
    law*                scale_law,
    const SPAposition&  root_position   = SPAposition(0, 0, 0),
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * Sets the root transformation of a given pattern to a specified transform.
 * <br><br>
 * <b>Role:</b> The seed element of a pattern remains fixed unless a root
 * transform is supplied. Use this function to supply one, if desired.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose root transform is set.
 * @param root_transf
 * root transform to be applied to the pattern.
 * @param ao
 * acis options.
 **/
DECL_KERN outcome api_pattern_modify_root_transformation(
    pattern*            pat,
    const SPAtransf&    root_transf,
	AcisOptions*        ao              = NULL);

/**
 * Returns the root transformation of a given pattern.
 * <br><br>
 * <b>Role:</b> The seed element of a pattern remains fixed unless a root
 * transformation is supplied.  Use this function get the root transformation
 * attached to the specified pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose root transformation is queried.
 * @param root_transf
 * the specified pattern's root transformation.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_query_root_transformation(
    const pattern*      pat,
    SPAtransf&          root_transf,
	AcisOptions*        ao              = NULL);

/**
 * Returns the number of elements in a given pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose size is queried.
 * @param size
 * the size of the specified pattern.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_query_size(
    const pattern*      pat,
    int&                size,
	AcisOptions*        ao              = NULL);

/**
 * Returns the dimensionality of the given pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose dimensionality is queried.
 * @param dim
 * the dimensionality of the specified pattern.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_query_coordinate_dimension(
    const pattern*      pat,
    int&                dim,
	AcisOptions*        ao              = NULL);

/**
 * Returns an array of pattern coordinate arrays for members of the specified pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose dimensionality is queried.
 * @param size
 * the size of the specified pattern.
 * @param dim
 * the dimensionality of the specified pattern.
 * @param coords
 * the array of pattern coordinate arrays.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_query_coordinates(
    const pattern*      pat,
    int&                size,
    int&                dim,
    double**&           coords,
	AcisOptions*        ao              = NULL);

/**
 * Returns an array of pattern indices for members of the specified pattern.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param pat
 * pattern whose dimensionality is queried.
 * @param size
 * the size of the specified pattern.
 * @param indices
 * the array of indices.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_query_element_indices(
    const pattern*      pat,
    int&                size,
    int*&               indices,
	AcisOptions*        ao              = NULL);

/**
 * Converts a pattern into an entity for the purposes of saving to and restoring from a SAT file.
 * <br><br>
 * <b>Role:</b> In order to make patterns more persistent and to share them from
 * session to session, they can be turned into APATTERN instances, which are
 * derived from ENTITY and are saved and restored.
 * <br><br>
 * <b>Errors:</b> The specified pattern is NULL.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param in_pat
 * pattern to convert.
 * @param out_ent
 * instance returned.
 * @param ao
 * acis options.
**/
DECL_KERN outcome api_pattern_to_entity(
	pattern* in_pat,
	ENTITY*& out_ent,
	AcisOptions *ao = NULL);


/** @} */
#endif //PATTERN_TYPES_HXX

