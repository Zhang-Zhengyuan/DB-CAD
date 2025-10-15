/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//         ACIS Sweep Component - Deprecated Interfaces
/*******************************************************************/
#ifndef DEPRECATED_SWEEPAPI_HXX
#define DEPRECATED_SWEEPAPI_HXX

class BODY;
class WIRE;
class FACE;
class COEDGE;
class ENTITY_LIST;
class SPAposition;
class surface;
class SPAunit_vector;
class SPAvector;
class sweeper;
class extrude_options;


class SPAdouble_vector;
class SPAunit_vector_list;
class SPAposition_vector;
class SPAvector_list;

#include "api.hxx"
#include "dcl_swp.h"
#include "logical.h"
#include "off_wire.hxx"
#include "swp_opts.hxx"
#include "swp_anno.hxx"

// ******************************************************************************
// THE FOLLOWING FUNCTIONS ARE OBSOLETE SWEEP APIS. INTERNALLY THEY USE 
// API_SWEEP_WITH_OPTIONS. THEY ARE *NOT* SUPPORTED OR DOCUMENTED!
//  
// ******************************************************************************

/**
 * @nodoc
 * <b>Obsolete:</b> use Booleans on shell bodies instead.
 * <br><br>
 * <b>Role:</b> This API pierces a sheet body through a sheet to make a new loop
 * in the face. This API is obsolete. Booleans on shell bodies do the same thing
 * and handle singular cases that <tt>api_pierce_sheet</tt> doesn't handle.
 * <br><br>
 * Both sheet objects must be "positive" and planar.
 * <br><br>
 * The smaller sheet must be completely enclosed within the bigger sheet, with
 * non-intersecting boundaries. The small sheet body is deleted afterwards.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to big or small body is NULL or not to a sheet body.
 * <dd> &nbsp;Sheets not coincident and planar.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param big_sheet
 * larger sheet (will contain hole).
 * @param small_sheet
 * smaller sheet (defines hole).
 * @param face
 * returns resulting face.
 * @param ao
 * ACIS options.
 **/

DECL_SWP outcome api_pierce_sheet(
	BODY*        big_sheet,
	BODY*        small_sheet,
	FACE*        &face,
	AcisOptions* ao = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a face along a path. This should be accomplished
 * using <tt>api_sweep_with_options</tt>. This will accept the same face and path,
 * and with the <tt>rigid</tt> sweep option set to TRUE, the same result can
 * be obtained.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided
 * face to sweep. <tt>FORWARD</tt> selects the side which coincides with the face
 * normal. <tt>REVERSED</tt> selects the opposite side.
 * <br><br>
 * The path is specified as the first <tt>COEDGE</tt> in a closed, NULL terminated, or
 * self-terminated (wire) <tt>COEDGE</tt> path.
 * <br><br>
 * It is not necessary that the face be planar.
 * <br><br>
 * The terminal face will have the same geometry as the original face (except it
 * will be transformed).
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;The face has an open loop.</dd>
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> If the sweep path is closed, the faces aren't merged out;
 * however, it doesn't make sense to do a rigid sweep of a face along a closed path.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sw_face
 * face to be swept.
 * @param which_side
 * which side of the double-sided face to sweep.
 * @param path
 * sweep path.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_face_path(
	FACE*          sw_face,
	logical        which_side,
	COEDGE*        path,
	sweep_options* opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a face along a vector. This should be
 * accomplished using <tt>api_sweep_with_options</tt>. This will accept the same face and
 * sweep vector, and with the <tt>rigid</tt> sweep option set to TRUE, the same result can
 * be obtained.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * It is not necessary that the face be planar.
 * <br><br>
 * The terminal face will have the same geometry as the original face (except it
 * will be transformed).
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Zero length vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>FACE</tt> must already be part of a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sw_face
 * face to be swept.
 * @param which_side
 * which side of the double-sided face to sweep.
 * @param sw_vec
 * sweep vector.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_face_vec(
	FACE*           sw_face,
	logical         which_side,
	const SPAvector &sw_vec,
	sweep_options*  opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a wire along a path. This should be accomplished
 * using <tt>api_sweep_with_options</tt>. This will accept the same wire body and
 * path, and with the <tt>rigid</tt> sweep option set to TRUE, the same result can
 * be obtained.
 * <br><br>
 * The wire can be open or closed, and it is not necessary that it be planar.
 * <br><br>
 * The path is specified as the first <tt>COEDGE</tt> in a closed, NULL terminated, or
 * self-terminated (wire) <tt>COEDGE</tt> path.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> Even if the path is closed, a rigid sweep for an open sweep wire
 * results in a sweep that is not closed. To work around this, use the open wire as
 * the path and the path as the sweep wire. The API does not detect all situations
 * which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_body
 * wire body to be swept.
 * @param path
 * sweep path.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_wire_path(
	BODY*          wire_body,
	COEDGE*        path,
	sweep_options* opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a wire along a path. This should be accomplished
 * using <tt>api_sweep_with_options</tt>. This will accept the same wire body and path,
 * and with the <tt>rigid</tt> sweep option set to TRUE, the same result can be obtained.
 * <br><br>
 * The wire can be open or closed, and it is not necessary that it be planar.
 * <br><br>
 * The path is specified as the first <tt>COEDGE</tt> in a closed, NULL terminated, or
 * self-terminated (wire) <tt>COEDGE</tt> path.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> Even if the path is closed, a rigid sweep for an open sweep wire
 * results in a sweep that is not closed. To work around this, use the open wire as
 * the path and the path as the sweep wire. The API does not detect all situations
 * which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire to be swept.
 * @param path
 * sweep path.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_wire_path(
	WIRE*          wire,
	COEDGE*        path,
	sweep_options* opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a wire along a direction. This should be
 * accomplished using <tt>api_sweep_with_options</tt>. This will accept the same wire body
 * and sweep vector, and with the <tt>rigid</tt> sweep option set to TRUE, the same result
 * can be obtained.
 * <br><br>
 * The wire may be open or closed, and it is not necessary that it be planar.
 * <br><br>
 * The API does not check to see if the sweep is valid.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The API does not detect all situations which may result in an
 * invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_body
 * wire body to be swept.
 * @param sw_vec
 * sweep vector.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_wire_vec(
	BODY*           wire_body,
	const SPAvector &sw_vec,
	sweep_options*  opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Rigidly sweeps a wire along a direction. This should be
 * accomplished using <tt>api_sweep_with_options</tt>. This will accept the same wire body
 * and sweep vector, and with the <tt>rigid</tt> sweep option set to TRUE, the same result
 * can be obtained.
 * <br><br>
 * The wire may be open or closed, and it is not necessary that it be planar.
 * <br><br>
 * The API does not check to see if the sweep is valid.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The API does not detect all situations which may result in an
 * invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire to be swept.
 * @param sw_vec
 * sweep vector.
 * @param opts
 * sweep options.
 **/
DECL_SWP outcome api_rsw_wire_vec(
	WIRE*           wire,
	const SPAvector &sw_vec,
	sweep_options*  opts = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps an open or closed chain of coedges around an axis given
 * by a point and direction. This functionality should be implemented with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * about an axis.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.)
 * <br><br>
 * If <tt>steps</tt> is zero, the profile is swept along a circular arc starting on the
 * profile and extending through the specified rotation angle. Otherwise, the
 * profile is swept along a linear path containing the specified number of segments.
 * <br>The interior segments are chords of the arc subtending an angle of
 * (<tt>angle</tt>/(<tt>steps </tt>- 1)). The first and last segments are each half of one of these
 * chords.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * <dd>&nbsp;Negative number of steps specified.</dd>
 * <dd>&nbsp;The profile is not planar. The axis is not in the plane of the profile.</dd>
 * <dd>&nbsp;Draft angle overflow/underflow. (<tt>&gt; pi/2</tt> or <tt>&lt; -pi/2</tt>).</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain entity list.
 * @param point
 * point on axis.
 * @param direction
 * direction of axis.
 * @param angle
 * angle of sweep (radians).
 * @param steps
 * number of times face pauses.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_axis(
	ENTITY_LIST const    &chain,
	SPAposition const    &point,
	SPAunit_vector const &direction,
	double angle,
	int    steps,
	double draft_angle,
	int    draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps an open or closed chain of coedges around an axis given
 * by a point and direction. This functionality should be implemented with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * about an axis.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.)
 * <br><br>
 * If <tt>steps</tt> is zero, the profile is swept along a circular arc starting on the
 * profile and extending through the specified rotation angle. Otherwise, the
 * profile is swept along a linear path containing the specified number of segments.
 * The interior segments are chords of the arc subtending an angle of
 * (<tt>angle</tt>/(<tt>steps </tt>- 1)). The first and last segments are each half of one of these
 * chords.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * <dd>&nbsp;Negative number of steps specified.</dd>
 * <dd>&nbsp;The profile is not planar. The axis is not in the plane of the profile.</dd>
 * <dd>&nbsp;Draft angle overflow/underflow. (<tt>&gt; pi/2</tt> or <tt>&lt; -pi/2</tt>).</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * list of coedges to be swept.
 * @param point
 * point on axis.
 * @param direction
 * direction of axis.
 * @param angle
 * angle of sweep (radians).
 * @param steps
 * number of times face pauses.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_axis(
	COEDGE*              chain,
	SPAposition const    &point,
	SPAunit_vector const &direction,
	double angle,
	int    steps,
	double draft_angle,
	int draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a face or a wire along a path with twist. This
 * functionality should be accomplished with <tt>api_sweep_with_options</tt>. Sweep options
 * can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The side of the face to use is determined by the API. The twist angle is
 * specified as a sweep option.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The path is specified as the first <tt>COEDGE</tt> in a closed, NULL terminated, or
 * self-terminated (wire) <tt>COEDGE</tt> path.
 * <br><br>
 * The twist angle indicates how much the profile is to be rotated as it is swept
 * along the path. This angle is divided equally between the segments of the sweep
 * path, regardless of their relative arc lengths.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to chain or path is NULL or not to a <tt>COEDGE</tt>.
 * <dd>&nbsp;Profile is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The faces aren't merged out if the sweep path is closed and a
 * nonzero twist angle is specified. If the path has any spline segments in it,
 * they must be planar. The path must always start in the plane of the profile. The
 * API does not detect all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sw_chain
 * profile (face loop or a wire coedge).
 * @param log
 * which side of a double-sided face to sweep.
 * @param path
 * sweep path.
 * @param twist
 * twist angle in radians.
 **/
DECL_SWP outcome api_sw_chain_path(
	COEDGE* sw_chain,
	logical log,
	COEDGE* path,
	double	twist
);
/**
 * @nodoc
 * Provides detailed sweep functionality which might be easier to implement
 * using @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a face or a wire along a path with a rail. This
 * functionality may optionally be accomplished with <tt>api_sweep_with_options</tt>. Sweep
 * options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The <tt>FACE</tt> or <tt>WIRE</tt> is swept along the given path resulting in some new lateral
 * faces and a terminal face which has some translated geometry of the original
 * face.
 * <br><br>
 * The sense flag (which_side) indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The path is specified as the first <tt>COEDGE</tt> in a closed, NULL terminated, or
 * self-terminated (wire) <tt>COEDGE</tt> path.
 * <br><br>
 * The <tt>api_sw_chain_path_options</tt> calls the <tt>Sweep_Options::fix_up</tt> method, which
 * fills out any remaining elements of its data structure with default values. As
 * part of this, <tt>api_make_rails</tt> is called, which establishes the default rails to
 * use. Anything passed into <tt>api_sw_chain_path_options</tt> in the manner of rail law
 * sweep options overrides all or part of the default rail array.
 * <br><br>
 * The twist angle indicates how much the profile is to be rotated as it is swept
 * along the path. This angle is divided equally between the segments of the sweep
 * path, regardless of their relative arc lengths.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to chain, path or rail is NULL or not to a <tt>COEDGE</tt>.
 * <dd>&nbsp;Profile is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The faces aren't merged out if the sweep path is closed and a
 * nonzero twist angle is specified. The path must always start in the plane of the
 * profile.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sw_chain
 * profile (face loop or a wire coedge).
 * @param path
 * rail path.
 * @param swopt
 * sense flag.
 * @param broom
 * Optional for internal use only.
 **/

DECL_SWP outcome api_sw_chain_path_options(
	COEDGE*        sw_chain,
	COEDGE*        path,
	sweep_options* swopt,
	sweeper*       broom = NULL
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a chain of coedges along given path onto given surface to
 * make a sheet. This functionality should be accomplished with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a path onto a surface.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Profile is not planar.</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Profile is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. If the path has
 * any spline segments in it, they must be planar. The path must always start in
 * the plane of the profile. The API does not detect all situations which may
 * result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain entity list.
 * @param path
 * path face is swept along.
 * @param surf
 * given surface.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_surface(
	ENTITY_LIST   &chain,
	COEDGE*       path,
	surface const &surf,
	double        draft_angle,
	int           draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a chain of coedges along given path onto given surface to
 * make a sheet. This functionality should be accomplished with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a path onto a surface.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Profile is not planar.</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Profile is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. If the path has
 * any spline segments in it, they must be planar. The path must always start in
 * the plane of the profile. The API does not detect all situations which may
 * result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain of coedges.
 * @param path
 * path face is swept along.
 * @param surf
 * given surface.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_surface(
	COEDGE *chain,
	COEDGE *path,
	surface const &surf,
	double draft_angle,
	int    draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a (planar) chain along a given vector. This functionality
 * should be accomplished with <tt>api_sweep_with_options</tt>. Sweep options can be set
 * through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a vector.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. The API does
 * not detect all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain of coedges to be swept.
 * @param sweep_vector
 * sweeping direction.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_vec(
	ENTITY_LIST const &chain,
	SPAvector const   &sweep_vector,
	double            draft_angle,
	int               draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a (planar) chain along a given vector. This functionality
 * should be accomplished with <tt>api_sweep_with_options</tt>. Sweep options can be set
 * through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a vector.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * <dd>&nbsp;Chain pointer is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. The API does
 * not detect all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain of coedges to be swept.
 * @param sweep_vector
 * sweeping direction.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_vec(
	COEDGE*         chain,
	SPAvector const &sweep_vector,
	double          draft_angle,
	int             draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b><br> Sweeps an open or closed chain of coedges along a path given by
 * a wire or a wire body. This functionality should be accomplished with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a wire.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire body.</dd>
 * <dd>&nbsp;For other errors refer to <tt>api_sw_face_wire</tt>.</dd>
 * <dd>&nbsp;Pointer to chain is NULL or is not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire.</dd>
 * <dd>&nbsp;For other errors refer to <tt>api_sw_face_wire</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. If the path has
 * any spline segments in it, they must be planar. The path must always start in
 * the plane of the profile. The API does not detect all situations which may
 * result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain to be swept.
 * @param wire
 * wire body face is swept along.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type Natural.
 **/
DECL_SWP outcome api_sw_chain_wire(
	ENTITY_LIST const &chain,
	BODY*             wire,
	double            draft_angle,
	int               draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps an open or closed chain of coedges along a path given by
 * a wire or a wire body. This functionality should be accomplished with
 * <tt>api_sweep_with_options</tt>. Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * This API sweeps the <tt>WIRE</tt> or <tt>FACE</tt> identified by the first <tt>COEDGE</tt> in the list
 * along a wire.
 * <br><br>
 * The use of coedge pointers is an older, lower-level form of controlling the
 * sweep. It is recommended that the higher level face or wire body specification
 * be used. If the coedge pointer was derived from an original face or wire body,
 * the use of that entity for sweeping is suggested. (<tt>api_get_owner</tt> can be used to
 * work from the coedge pointers back to the original entity.) If it is not
 * possible to work with entities other than the coedge pointers, the use of
 * <tt>api_sw_chain_path_options</tt> is recommended.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The first entity in the chain list is not a <tt>COEDGE</tt>.
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire body.</dd>
 * <dd>&nbsp;For other errors refer to <tt>api_sw_face_wire</tt>.</dd>
 * <dd>&nbsp;Pointer to chain is NULL or is not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire.</dd>
 * <dd>&nbsp;For other errors refer to <tt>api_sw_face_wire</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE </tt>or <tt>FACE </tt>must already be part of a <tt>BODY</tt>. If the path has
 * any spline segments in it, they must be planar. The path must always start in
 * the plane of the profile. The API does not detect all situations which may
 * result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param chain
 * chain to be swept.
 * @param wire
 * wire body face is swept along.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_chain_wire(
	COEDGE *chain,
	WIRE *wire,
	double draft_angle,
	int draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a (planar) face of a sheet or solid, around an
 * axis. This functionality should be implemented with <tt>api_sweep_with_options</tt>.
 * Sweep options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * If the angle is 2 <tt>pi</tt>, the body must be a sheet (one face) or solid lamina (two
 * faces).
 * <br><br>
 * If <tt>steps</tt> is zero, the profile is swept along a circular arc starting on the
 * profile and extending through the specified rotation angle. Otherwise, the
 * profile is swept along a linear path containing the specified number of segments.
 * The interior segments are chords of the arc subtending an angle of
 * (<tt>angle</tt>/(<tt>steps </tt>- 1)). The first and last segments are each half of one of these
 * chords.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner.<br> Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * <dd>&nbsp;Negative number of steps specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>FACE</tt> must already be part of a <tt>BODY</tt>. The API does not detect
 * all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
 * face to be swept.
 * @param which_side
 * which side of the face to pick if the face is a double-sided face.
 * @param point
 * point on axis.
 * @param direction
 * direction of axis.
 * @param angle
 * angle of sweep (radians).
 * @param steps
 * number of times face pauses.
 * @param draft_angle
 * draft angle.
 * @param draft_type
 * draft type: Natural.
 **/
DECL_SWP outcome api_sw_face_axis(
	FACE*                face,
	logical              which_side,
	SPAposition const    &point,
	SPAunit_vector const &direction,
	double               angle,
	int                  steps,
	double               draft_angle,
	int                  draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a (planar) face along its normal by a given
 * distance. This should be accomplished using <tt>api_sweep_with_options</tt>. This will
 * accept the same face and distance arguments, and the other options can be set
 * through the <tt>sweep:options</tt> class.
 * <br><br>
 * The distance argument is taken to be in global space of face-body.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Absolute distance not greater than <tt>SPAresabs</tt>.</dd>
 * <dd>&nbsp;Checks for global self-intersections for sweep with draft.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>FACE</tt> must already be part of a <tt>BODY</tt>. The API does not detect
 * all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
  * face to be swept.
  * @param which_side
  * which side of a double-sided face to sweep.
  * @param distance
  * sweeping distance (along face normal).
  * @param draft_angle
  * angle at which lateral faces will be inclined to the sweep direction.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_face_norm(
	FACE*   face,
	logical which_side,
	double distance,
	double draft_angle,
	int draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a planar face along a path to a given surface.
 * This should be accomplished using <tt>api_sweep_with_options</tt>. This will accept the
 * same face and path arguments, and the other options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Pointer to path is NULL or not to a <tt>COEDGE</tt>.</dd>
 * <dd>&nbsp;Face is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>FACE</tt> must already be part of a <tt>BODY</tt>. If the path has any
 * spline segments in it, they must be planar. The path must always start in the
 * plane of the profile. The API does not detect all situations which may result in
 * an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
  * face to be swept.
  * @param which_side
  * which side of the face to pick if the face is a double-sided face.
  * @param path
  * path face is swept along.
  * @param surf
  * given surface.
  * @param draft_angle
  * draft angle.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_face_surface(
	FACE*         face,
	logical       which_side,
	COEDGE*       path,
	surface const &surf,
	double        draft_angle,
	int           draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a (planar) face along given vector. This should
 * be accomplished using <tt>api_sweep_with_options</tt>. This will accept the same face and
 * vector arguments, and the other options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Zero length sweep vector specified.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The <tt>WIRE</tt> or <tt>FACE</tt> must already be part of a <tt>BODY</tt>. The API does
 * not detect all situations which may result in an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
  * face to be swept.
  * @param which_side
  * which side of a double-sided face to sweep.
  * @param sweep_vector
  * sweeping direction.
  * @param draft_angle
  * angle at which lateral faces will be inclined to the sweep direction.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_face_vec(
	FACE*           face,
	logical         which_side,
	SPAvector const &sweep_vector,
	double          draft_angle,
	int             draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a face along a path given by a wire. This should
 * be accomplished using <tt>api_sweep_with_options</tt>. This will accept the same face and
 * wire body arguments, and the other options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire body.</dd>
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire.</dd>
 * <dd>&nbsp;Face is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The path must start in the plane of the profile (face). Draft
 * angle must be 0 if the path is closed. If the path has any spline segments in it,
 * they must be planar. The API does not detect all situations which may result in
 * an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
  * face to be swept.
  * @param which_side
  * which side of the face to pick if the face is a double-sided face.
  * @param wire
  * wire body along which face is swept (must have one or more edges).
  * @param draft_angle
  * draft angle to be applied during sweeping.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_face_wire(
	FACE*   face,
	logical which_side,
	BODY*   wire,
	double  draft_angle,
	int     draft_type = 2
);


/**
 * @nodoc
 * <b>Obsolete:</b> use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> This API sweeps a face along a path given by a wire. This should
 * be accomplished using <tt>api_sweep_with_options</tt>. This will accept the same face and
 * wire body arguments, and the other options can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * The sense flag <tt>(which_side)</tt> indicates which side of a double-sided face to sweep.
 * <tt>FORWARD</tt> selects the side which coincides with the face normal. <tt>REVERSED</tt> selects
 * the opposite side.
 * <br><br>
 * The draft angle indicates the angle at which the profile expands or contracts as
 * it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner.<br> Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc.<br> Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is NULL or not to a <tt>FACE</tt>.
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire body.</dd>
 * <dd>&nbsp;Pointer to wire is NULL or not to a wire.</dd>
 * <dd>&nbsp;Face is not planar.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The path must start in the plane of the profile (face). Draft
 * angle must be 0 if the path is closed. If the path has any spline segments in it,
 * they must be planar. The API does not detect all situations which may result in
 * an invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
  * face to be swept.
  * @param which_side
  * which side of the face to pick if the face is a double-sided face.
  * @param wire
  * wire body along which face is swept (must have one or more edges).
  * @param draft_angle
  * draft angle to be applied during sweeping.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_face_wire(
	FACE*   face,
	logical which_side,
	WIRE*   wire,
	double  draft_angle,
	int     draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> Use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a (planar) wire around an axis by given angle. This
 * functionality should be implemented with <tt>api_sweep_with_options</tt>. Sweep options
 * can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * To close an open wire before sweeping, perpendiculars are dropped to the axis
 * and joined. The wire is covered to create a lamina and one of the resulting
 * faces is swept around an axis to create a solid body.
 * <br><br>
 * If <tt>steps</tt> is zero, the profile is swept along a circular arc starting on the
 * profile and extending through the specified rotation angle. Otherwise, the
 * profile is swept along a linear path containing the specified number of segments.
 * <br>The interior segments are chords of the arc subtending an angle of
 * (<tt>angle</tt>/(<tt>steps </tt>- 1)). The first and last segments are each half of one of these
 * chords. The draft angle indicates the angle at which the profile expands or
 * contracts as it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. <br>Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * <dd>&nbsp;Wire is not planar.</dd>
 * <dd>&nbsp;Angle not in range -2 <tt>pi</tt> to 2 <tt>pi</tt>.</dd>
 * <dd>&nbsp;Draft angle not in range <tt>(-pi/2+SPAresnor, pi/2-SPAresnor)</tt>.</dd>
 * <dd>&nbsp;Plane of wire does not contain axis.</dd>
 * <dd>&nbsp;Cannot have nonzero draft angle for sweep angle of 2 <tt>pi</tt>.</dd>
 * <dd>&nbsp;Cannot sweep wire that crosses axis.</dd>
 * <dd>&nbsp;Cannot sweep wire that touches axis everywhere.</dd>
 * <dd>&nbsp;Wire coedges not head to tail.</dd>
 * <dd>&nbsp;Cannot cover wire.</dd>
 * <dd>&nbsp;Failed to cover sheet.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The API does not detect all situations which may result in an
 * invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param body
  * wire body to be swept.
  * @param point
  * point on axis.
  * @param direction
  * direction of axis.
  * @param angle
  * angle of sweep (radians).
  * @param steps
  * number of times wire pauses during sweep.
  * @param draft_angle
  * angle at which lateral faces will be inclined to the sweep direction.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_wire_axis_sol(
	BODY*                body,
	SPAposition const    &point,
	SPAunit_vector const &direction,
	double               angle,
	int                  steps,
	double               draft_angle,
	int                  draft_type = 2
);
/**
 * @nodoc
 * <b>Obsolete:</b> Use @href api_sweep_with_options instead.
 * <br><br>
 * <b>Role:</b> Sweeps a (planar) wire around an axis by given angle. This
 * functionality should be implemented with <tt>api_sweep_with_options</tt>. Sweep options
 * can be set through the <tt>sweep:options</tt> class.
 * <br><br>
 * To close an open wire before sweeping, perpendiculars are dropped to the axis
 * and joined. The wire is covered to create a lamina and one of the resulting
 * faces is swept around an axis to create a solid body.
 * <br><br>
 * If <tt>steps</tt> is zero, the profile is swept along a circular arc starting on the
 * profile and extending through the specified rotation angle. Otherwise, the
 * profile is swept along a linear path containing the specified number of segments.
 * <br>The interior segments are chords of the arc subtending an angle of
 * (<tt>angle</tt>/(<tt>steps </tt>- 1)). The first and last segments are each half of one of these
 * chords. The draft angle indicates the angle at which the profile expands or
 * contracts as it is swept.
 * <br><br>
 * The optional draft type indicates how corners should be handled if the profile
 * is expanded:
 * <br><br>
 * 0 &nbsp;&nbsp;Extended/corner. <br>Closes gaps with linear extensions. The edges in the
 * expanded profile should be extended to their intersection point. The extensions
 * are then swept back along the path and intersected to produce two new faces and
 * a new edge.
 * <br><br>
 * 1 &nbsp;&nbsp;Round/arc. <br>Closes gaps with arcs. An arc that is tangent to the two
 * edges is created and swept to create a closing face.
 * <br><br>
 * 2 &nbsp;&nbsp;Natural. Closes gaps with circular extensions of arcs.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is NULL or not to a wire body.
 * <dd>&nbsp;Zero length axis vector specified.</dd>
 * <dd>&nbsp;Wire is not planar.</dd>
 * <dd>&nbsp;Angle not in range -2 <tt>pi</tt> to 2 <tt>pi</tt>.</dd>
 * <dd>&nbsp;Draft angle not in range <tt>(-pi/2+SPAresnor, pi/2-SPAresnor)</tt>.</dd>
 * <dd>&nbsp;Plane of wire does not contain axis.</dd>
 * <dd>&nbsp;Cannot have nonzero draft angle for sweep angle of 2 <tt>pi</tt>.</dd>
 * <dd>&nbsp;Cannot sweep wire that crosses axis.</dd>
 * <dd>&nbsp;Cannot sweep wire that touches axis everywhere.</dd>
 * <dd>&nbsp;Wire coedges not head to tail.</dd>
 * <dd>&nbsp;Cannot cover wire.</dd>
 * <dd>&nbsp;Failed to cover sheet.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> The API does not detect all situations which may result in an
 * <br>invalid body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
  * wire to be swept.
  * @param point
  * point on axis.
  * @param direction
  * direction of axis.
  * @param angle
  * angle of sweep (radians).
  * @param steps
  * number of times wire pauses during sweep.
  * @param draft_angle
  * angle at which lateral faces will be inclined to the sweep direction.
  * @param draft_type
  * draft type: Natural.
  **/
DECL_SWP outcome api_sw_wire_axis_sol(
	WIRE*                wire,
	SPAposition const    &point,
	SPAunit_vector const &direction,
	double               angle,
	int                  steps,
	double               draft_angle,
	int                  draft_type = 2
);



#endif
