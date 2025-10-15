/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( LOP_HUSK_API_ROUTINES )
#define LOP_HUSK_API_ROUTINES
#include "dcl_lop.h"
#include "api.hxx"
#include "lop_opts.hxx"

/**
 * \defgroup LOCALOPS Local Operations
 *
 * \defgroup LOPAPI Local Operations
 * \ingroup LOCALOPS
 */
/**
 * \defgroup LOPTWEAK Tweak
 * \ingroup LOPAPI
 *
 * \defgroup LOPMOVE Move
 * \ingroup LOPAPI
 *
 * \defgroup LOPOFFSET Offset
 * \ingroup LOPAPI
 *
 * \defgroup LOPTAPER Taper
 * \ingroup LOPAPI
 *
 * \defgroup LOPSHEETEXT Extend Sheet
 * \ingroup LOPAPI
 *
 */
/**
 * @file lop_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup LOPAPI Local Operations 
 *
 * @{
 */
class outcome;
class BODY;
class FACE;
class EDGE;
class VERTEX;
class SPAposition;
class SPAunit_vector;
class SPAtransf;
class SURFACE;
class CURVE;
class APOINT;
class ENTITY_LIST;
class detect_feature_options;

/**
 * Initializes the local operations library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_LOP outcome api_initialize_local_ops();

/**
 * Terminates the local operations library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_LOP outcome api_terminate_local_ops();
/** @} */
/**
 * \addtogroup LOPTWEAK
 *
 * @{
 */
/**
 * Tweaks an array of faces to new surfaces.
 * <b>Technical Article:</b> <i>[Tweak Faces](http://doc.spatial.com/articles/t/w/e/Tweak_Faces_b431.html)</i>
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with supplied surfaces, 
 * taking the supplied reverse flags into consideration.
 * Choose reverse flags carefully, as resulting body is not checked. 
 * Tweak will replace the original surface(s) with the user-supplied tool surface(s). 
 * The reverse bit depends only on the relationship between the corresponding surfaces. 
 * It does not depend on the face sense of the tweak face(s). If the reverse bit is set to TRUE,
 * then the FACE sense is switched; otherwise, it stays the same.
 * <br><br>
 * The <tt>box_low</tt> and <tt>box_high</tt> arguments are used to limit any geometrical intersections
 * and so improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted, defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * The topology changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * Mergeable edges and vertices on the supplied faces will be merged out.
 * <br><br>
 * Edges and vertices between faces that are tweaked to the same geometry will also be merged out.
 * <br><br>
 * Two edge vertices on the supplied faces between edges on the same geometrical non-analytic intersection curve,
 * which are not presently mergeable elsewhere, will be merged out by <tt>lop:tweak-faces</tt> 
 * if the option <tt>lop_merge_vertex</tt> is set on, which is the default setting.
 * <br><br>
 * Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).
 * <br><br>
 * Isolated single edge loops may degenerate exactly to a point or be removed altogether, and their faces
 * will be deleted when appropriate.
 * <br><br>
 * Edges in multi-edge loops may degenerate exactly to a point and will be removed.
 * <br><br>
 * Where an edge exactly degenerates to a point in a multi-edge loop, the edges either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid 
 * and degenerate loops. Degenerate loops will be deleted, and their faces when appropriate.
 * <br><br>
 * When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.
 * <br><br>
 * The option <tt>lop_ff_int</tt> switches on face-face intersection checking on the result body.
 * The default for the option is off.
 * If the option is switched on and a face-face intersection is found in the body, the operation fails.
 * <br><br>
 * The self intersecting bodies repair is controlled by the @href lop_options class. It is enabled by default.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * The geometry changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces 
 * in the supplied faces and their neighbors if the option <tt>lop_convert_pipe</tt> is set on, 
 * which is the default setting.
 * <br><br>
 * <b>Errors:</b> Some of the following errors will return a <tt>standard_error_info</tt> object 
 * in the API <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object 
 * is returned).
 * <br><br>
 * At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Faces must be valid, non-duplicate, and from the same body, or error <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surfaces must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.
 * <br><br>
 * Body must be manifold, or error <tt>LOP_TWK_NON_MANIFOLD (COEDGE*)</tt>.
 * <br><br>
 * Body must be solid, or error <tt>LOP_TWK_FREE_EDGE (COEDGE*)</tt>.
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently, or errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <br>
 * Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.
 * <br><br>
 * Unpermitted topology changes, if detected, will produce the error <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.
 * <br><br>
 * Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Limitations:</b> Body must be manifold and solid.
 * <br><br>
 * No other topology changes than those documented above are permitted.
 * <tt>Tweak_faces</tt> does not extend the geometry of the tool surfaces or their neighbors (unlike the other 
 * local operations which do).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied)
 * @param reverse
 * flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_faces(
								 int const    nface,
								 FACE*        face[],
								 SURFACE*     tool_surface[],
								 int          reverse[],
								 const SPAposition&  box_low,
								 const SPAposition&  box_high,
								 lop_options* pLopts,
								 AcisOptions* ao = NULL
								 );

/**
 * Tweaks an array of faces to new surfaces.
 * <b>Technical Article:</b> <i>[Tweak Faces](http://doc.spatial.com/articles/t/w/e/Tweak_Faces_b431.html)</i>
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with supplied surfaces, 
 * taking the supplied reverse flags into consideration.
 * Choose reverse flags carefully, as resulting body is not checked. Tweak will replace the original 
 * surface(s) with the user-supplied tool surface(s). The reverse bit depends only on the relationship 
 * between the corresponding surfaces. It does not depend on the face sense of the tweak face(s). 
 * If the reverse bit is set to TRUE, then the FACE sense is switched;
 * otherwise, it stays the same.
 * <br><br>
 * If curves and edges are supplied, then curves are used to replaced tangent, 
 * near tangent or free edge curves. If there are any free edges (sheet boundaries) 
 * whose faces are being tweaked, then the new curves must be supplied.
 * <br><br> 
 * If points and vertices are supplied, then these points are used to replace vertices,
 * which are located at tangent or near tangent intersections of edge curves.
 * <br><br>
 * The box_low and box_high arguments are used to limit any geometrical intersections,
 * and so improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * The topology changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * Mergeable edges and vertices on the supplied faces will be merged out.
 * <br><br>
 * Edges and vertices between faces that are tweaked to the same geometry will also be merged out.
 * <br><br>
 * Two edge vertices on the supplied faces between edges on the same geometrical non analytic intersection curve,
 * which are not presently mergeable elsewhere, will be merged out by <tt>lop:tweak-faces</tt>
 * if the option <tt>lop_merge_vertex</tt> is set on, which is the default setting.
 * <br><br>
 * Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).
 * <br><br>
 * Isolated single edge loops may degenerate exactly to a point or be removed altogether and their faces
 * will be deleted when appropriate.
 * <br><br>
 * Edges in multi-edge loops may degenerate exactly to a point and will be removed.
 * <br><br>
 * Where an edge exactly degenerates to a point in a multi edge loop, the edges either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid 
 * and degenerate loops. Degenerate loops will be deleted, and their faces when appropriate.
 * <br><br>
 * When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.
 * <br><br>
 * The option <tt>lop_ff_int</tt> switches on face face intersection checking on the result body.
 * The default for the option is off.
 * If the option is switched on and a face face intersection is found in the body, the operation fails.
 * <br><br>
 * The self intersecting bodies repair is controlled by the @href lop_options class. It is enabled by default.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * The geometry changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces 
 * the supplied faces and their neighbors if the option <tt>lop_convert_pipe</tt> is set on, 
 * which is the default setting.
 * <br><br>
 * <b>Errors:</b> Some of the following errors will return a <tt>standard_error_info</tt> object in the 
 * API <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object is returned).
 * <br><br>
 * At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Faces must be valid, non-duplicate and from the same body, or error <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surfaces must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.
 * <br><br>
 * Body must be manifold, or error <tt>LOP_TWK_NON_MANIFOLD (COEDGE*)</tt>.
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently or the intersection has to 
 * prespecified by supplying tool curves, otherwise errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <br>
 * Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.
 * <br><br>
 * Unpermitted topology changes, if detected, will produce the error <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.
 * <br><br>
 * Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Limitations:</b> Body must be manifold.
 * <br><br>
 * No other topology changes than those documented above are permitted.
 * <tt>Tweak_faces</tt> does not extend the geometry of the tool surfaces or their neighbors (unlike the other 
 * local operations which do).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param reverse
 * flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param nedge
 * number of edges.
 * @param edge
 * edges being tweaked.
 * @param tool_curve
 * new curves.
 * @param nvertex
 * number of vertices.
 * @param vertex
 * vertices being tweaked.
 * @param tool_point
 * new points.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_faces(int const nface, 
						FACE *face[],
						SURFACE *tool_surface[],
						int reverse[], 
						int const nedge, 
						EDGE *edge[],
						CURVE *tool_curve[],
						const int nvertex,
						VERTEX *vertex[],
						APOINT *tool_point[],
						SPAposition &box_low,
						SPAposition &box_high,
						lop_options* pLopts,
						AcisOptions * ao = NULL);



/**
 * @nodoc
 * Tweaks an array of faces to new surfaces.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with supplied surfaces, 
 * taking the supplied reverse flags into consideration.
 * Choose reverse flags carefully, as resulting body is not checked. Tweak will replace the original surface(s) with the 
 * user supplied tool surface(s). The reverse bit depends only on the relationship between the corresponding surfaces. 
 * It does not depend on the face sense of the tweak face(s). If the reverse bit is set to TRUE, 
 * then the FACE sense is switched; otherwise, it stays the same.
 * <br><br>
 * The box_low and box_high arguments are used to limit any geometrical intersections
 * and so improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * The topology changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * Mergeable edges and vertices on the supplied faces will be merged out
 * <br><br>
 * Edges and vertices between faces that are tweaked to the same geometry will also be merged out.
 * <br><br>
 * Two edge vertices on the supplied faces between edges on the same geometrical non analytic intersection curve,
 * which are not presently mergeable elsewhere, will be merged out by <tt>lop:tweak-faces</tt>
 *  if the option <tt>lop_merge_vertex</tt> is set on,
 * which is the default setting.
 * <br><br>
 * Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).
 * <br><br>
 * Isolated single edge loops may degenerate exactly to a point or be removed altogether and their faces
 * will be deleted when appropriate.
 * <br><br>
 * Edges in multi edge loops may degenerate exactly to a point and will be removed.
 * <br><br>
 * Where an edge exactly degenerates to a point in a multi edge loop, the edges either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid and degenerate loops.
 * Degenerate loops will be deleted, and their faces when appropriate.
 * <br><br>
 * When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.
 * <br><br>
 * The option <tt>lop_ff_int</tt> switches on face face intersection checking on the result body.
 * The default for the option is off.
 * If the option is switched on and a face face intersection is found in the body, the operation fails.
 * <br><br>
 * The option <tt>lop_repair_self_int</tt> switches on the repairing of self intersections on the resulting body.
 * The default for the option is off.
 * If the option is switched on <tt>api_repair_body_self_ints</tt> is called at the end of successful tweaks 
 * and supplied with the changed body faces.
 * <br><br>
 * If the repair of self intersections is required, the options <tt>lop_check_invert</tt> and <tt>lop_ff_int</tt> 
 * are disabled internally so that self-intersection checks are avoided and possibly an invalid body is passed to 
 * <tt>api_repair_body_self_ints</tt> with no error raised.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * The geometry changes performed by <tt>tweak_faces</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces in the supplied 
 * faces and their neighbors if the option <tt>lop_convert_pipe</tt> is set on, which is the default setting.
 * <br><br>
 * <b>Errors:</b> Some of the following errors will return a <tt>standard_error_info</tt> object in the API <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object is returned).
 * <br><br>
 * At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Faces must be valid, non-duplicate and from the same body, or error <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surfaces must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.
 * <br><br>
 * Body must be manifold, or error <tt>LOP_TWK_NON_MANIFOLD (COEDGE*)</tt>.
 * <br><br>
 * Body must be solid, or error <tt>LOP_TWK_FREE_EDGE (COEDGE*)</tt>.
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently, or errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <br>
 * Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.
 * <br><br>
 * Unpermitted topology changes, if detected, will produce the error <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.
 * <br><br>
 * Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Limitations:</b> Body must be manifold and solid.
 * <br><br>
 * No other topology changes than those documented above are permitted.
 * <tt>Tweak_faces</tt> does not extend the geometry of the tool surfaces or their neighbors (unlike the other 
 * local operations which do).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param reverse
 * flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_faces(
								 int const    nface,
								 FACE*        face[],
								 SURFACE*     tool_surface[],
								 int          reverse[],
								 SPAposition  box_low,
								 SPAposition  box_high,
								 AcisOptions* ao = NULL
								 );



/**
 * Tweaks an array of faces to a body.
 * <b>Technical Article:</b> <i>[Tweak to Body](http://doc.spatial.com/articles/t/w/e/Tweak_to_Body_eff7.html)</i>
 * <br><br>
 * <b>Role:</b> Replaces supplied faces with the face or faces of the supplied sheet body. 
 * There is no one-to-one correspondence. 
 * <br><br>
 * The <tt><i>box_low</i></tt> and <tt><i>box_high</i></tt> arguments are used to limit 
 * any geometrical intersections, and thus, improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces, and if omitted, defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br>
 * The topology changes performed by <tt>tweak_to_body</tt> on the tweaked faces and their neighbors are:
 * <ul>
 * <li>Mergeable edges and vertices on the supplied faces will be merged out.</li>
 * <li>Edges and vertices between faces that are tweaked to the same geometry will also be merged out.</li>
 * <li>Two edge vertices on the supplied faces between edges on the same geometrical non-analytic 
 * intersection curve, which are not presently mergeable elsewhere, will be merged out by 
 * <tt>lop:tweak-to-body</tt> if the option <tt>lop_merge_vertex</tt> is enabled (default setting).</li>
 * <li>Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).</li>
 * <li>Isolated single-edge loops may degenerate exactly to a point or be removed altogether, and their faces
 * will be deleted when appropriate.</li>
 * <li>Edges in multi-edge loops may degenerate exactly to a point, and will be removed.</li>
 * <li>Where an edge exactly degenerates to a point in a multi-edge loop, the edges on either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid and 
 * degenerate loops. Degenerate loops will be deleted, as well as their faces when appropriate.</li>
 * <li>When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.</li>
 * <li>The option <tt>lop_ff_int</tt> switches on face-face intersection checking on the result body 
 * (option default is <b>off</b>).
 * If the option is switched on and a face-face intersection is found in the body, the operation fails.</li>
 * <li>The self-intersecting bodies repair is controlled by the @href lop_options class (default is <b>on</b>).</li>
 * </ul>
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br>
 * The geometry changes performed by <tt>tweak_to_body</tt> on the tweaked faces and their neighbors are:
 * <br><br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces 
 * in the supplied faces and their neighbors if the option <tt>lop_convert_pipe</tt> is set <b>on</b> (default).
 * <br><br>
 * <b>Errors:</b><br>
 * Some of the following errors will return a <tt>standard_error_info</tt> object in the API <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object is 
 * returned).
 * <br>
 * <ul>
 * <li>At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.</li>
 * <li>Faces must be valid, non-duplicate, and from the same body and 
 * should not be from target body, or error <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.</li>
 * <li>Supplied target body must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.</li>
 * <li>Body must be manifold, or error <tt>LOP_TWK_NON_MANIFOLD (COEDGE*)</tt>.</li>
 * </ul>
 * <br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently, or errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <ul>
 * <li>Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.</li>
 * <li>Unpermitted topology changes, if detected, will produce the error 
 * <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.</li>
 * <li>Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b><br>
 * Body must be manifold.
 * <br>
 * No other topology changes than those documented above are permitted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nFace
 * Specifies the number of faces.
 * @param face
 * Specifies the faces to be tweaked.
 * @param targetBody
 * Specifies the Body to which to tweak.
 * @param boolType
 * Specifies the type of boolean you want performed. Valid values are Add (<b>0</b>) 
 * or Subtract (<b>1</b>).
 * @param numPoints
 * Specifies the number of test points.
 * @param testPoints
 * Specifies the test positions to use to determine which cell of the tweaked body to keep.
 * @param boxLow
 * Specifies the start of the intersection box to be used.
 * @param boxHigh
 * Specifies the end of the intersection box to be used.
 * @param pLopts
 * Specifies local operations options. NULL argument will set the default values.
 * @param ao
 * Specifies ACIS options.
 **/
DECL_LOP outcome api_tweak_to_body(int const nFace,
						FACE* face[],
						BODY *targetBody,
						int boolType,
						const int numPoints,
						const SPAposition* testPoints,
						const SPAposition &boxLow,
						SPAposition &boxHigh,
						lop_options *pLopts,
						AcisOptions *ao=NULL);

/**
 * Tweaks an open sheet body to a target body.
 * <b>Technical Article:</b> <i>[Tweak to Body](http://doc.spatial.com/articles/t/w/e/Tweak_to_Body_eff7.html)</i>
 * <br><br>
 * <b>Role:</b> Extends a sheet body until it intersects the target body and booleans them together. 
 * There is no one-to-one correspondence. 
 * <br><br>
 * The <tt><i>box_low</i></tt> and <tt><i>box_high</i></tt> arguments are used to limit 
 * any geometrical intersections, and thus, improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the target body and if omitted, defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br>
 * The topology changes performed by <tt>tweak_to_body</tt> on the free-edge loops and their neighbors are:
 * <ul>
 * <li>Mergeable edges and vertices on the supplied free-edge loops will be merged out.</li>
 * <li>Two edge vertices on the supplied free-edge loops between edges on the same geometrical non analytic 
 * intersection curve, which are not presently mergeable elsewhere, will be merged out by <tt>lop:tweak-faces</tt> 
 * if the option <tt>lop_merge_vertex</tt> is enabled (default).</li>
 * <li>Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).</li>
 * <li>Isolated single edge loops may degenerate exactly to a point or be removed altogether and their faces
 * will be deleted when appropriate.</li>
 * <li>Edges in multi-edge loops may degenerate exactly to a point and will be removed.</li>
 * <li>Where an edge exactly degenerates to a point in a multi-edge loop, the edges either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid and 
 * degenerate loops.
 * Degenerate loops will be deleted, as well as their faces, when appropriate.</li>
 * <li>When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.</li>
 * <li>The option <tt>lop_ff_int</tt> switches on face-face intersection checking on the result body 
 * (option default is <b>off</b>).
 * If the option is switched <b>on</b> and a face-face intersection is found in the body, the operation fails.</li>
 * <li>The self-intersecting bodies repair is controlled by the @href lop_options class. It is enabled by default.</li>
 * </ul>
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br>
 * The geometry changes performed by <tt>tweak_to_body</tt> on the free-edge loops and their neighbors are:
 * <br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces in the 
 * supplied faces and their neighbors if the option <tt>lop_convert_pipe</tt> is set to <b>on</b> (default).
 * <br><br>
 * <b>Errors:</b> Some of the following errors will return a <tt>standard_error_info</tt> object in the API 
 * <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object 
 * is returned).
 * <ul>
 * <li>At least one edge must be supplied, or error <tt>LOP_TWK_NO_EDGE_SUPPLIED</tt>.</li>
 * <li>Edges must be valid, non-duplicate, from different free-edge loops, from the same body and 
 * should not be from the target body, or error <tt>LOP_TWK_BAD_EDGE_SUPPLIED (EDGE*)</tt>.</li>
 * <li>Supplied target body must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.</li>
 * </ul>
 * <br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently, or errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <ul>
 * <li>Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.</li>
 * <li>Unpermitted topology changes, if detected, will produce the error <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.</li>
 * <li>Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b> <br>
 * Body must be manifold.
 * <br>
 * No other topology changes than those documented above are permitted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available 
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nEdge 
 * Specifies the number of free edges being passed in. Each free-edge corresponds to a different loop.
 * @param edges
 * Specifies the edges which are part of the free-edge loops being tweaked.
 * @param targetBody
 * Specifies the body to which to tweak.
 * @param boolType
 * Specifies the type of boolean you want performed, which can be Add (<b>0</b>) or Subtract (<b>1</b>).
 * @param numPoints
 * Specifies the number of test points.
 * @param testPoints
 * Specifies the test positions to use to determine which cell of the tweaked body to keep.
 * @param boxLow
 * Specifies the start of the intersection box to be used.
 * @param boxHigh
 * Specifies the end of the intersection box to be used.
 * @param pLopts
 * Specifies the Local Operations options. NULL argument will set the default values.
 * @param ao
 * Specifies the ACIS options.
 **/
DECL_LOP outcome api_tweak_to_body(
					    int nEdge,
						EDGE* edges[],
						BODY *targetBody,
						int boolType,
						const int numPoints,
						const SPAposition* testPoints,
						const SPAposition &boxLow,
						SPAposition &boxHigh,
						lop_options *pLopts,
						AcisOptions *ao=NULL);




/**
 * Tweaks a list of free-edge loops to new surfaces.
 * <b>Technical Article:</b> <i>[Tweak Open Circuits](http://doc.spatial.com/articles/t/w/e/Tweak_Open_Circuits_725c.html)</i>
 * <br><br>
 * <b>Role:</b> Covers each free-edge loop with a new face having the supplied surface as the geometry, 
 * taking the supplied reverse flags into consideration.
 * Choose reverse flags carefully, as resulting body is not checked.
 * <br><br>
 * The  <tt><i>box_low</i></tt> and <tt><i>box_high</i></tt> arguments are used to limit any geometrical 
 * intersections, and thus, improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces, and if omitted, defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br>
 * The topology changes performed by <tt>tweak_open_circuits</tt> on the tweaked loops and their neighboring faces are:
 * <ul>
 * <li>Mergeable edges and vertices on the supplied loops will be merged out.</li>
 * <li>Two-edge vertices on the supplied loops between edges on the same geometrical non-analytic intersection 
 * curve, which are not presently mergeable elsewhere, will be merged out by @href api_tweak_open_circuits 
 * if the option <tt>lop_merge_vertex</tt> is set <b>on</b> (default).</li>
 * <li>Vertices with more than three edges may split into two or more vertices during a local operation.
 * Single inverted faces, needed to fill any gap in the body, will be made (if two or more inverted faces 
 * are needed, the operation will fail).</li>
 * <li>Isolated single-edge loops may degenerate exactly to a point or be removed altogether and their faces
 * will be deleted when appropriate.</li>
 * <li>Edges in multi-edge loops may degenerate exactly to a point and will be removed.</li>
 * <li>Where an edge exactly degenerates to a point in a multi-edge loop, the edges on either side will be 
 * detected becoming fully or partially coincident, and the loop repaired by splitting it into valid and 
 * degenerate loops. Degenerate loops will be deleted, as well as their faces when appropriate.</li>
 * <li>When faces have been deleted, their shells and lumps will be repartitioned and split, if necessary.</li>
 * <li>The option <tt>lop_ff_int</tt> switches on face-face intersection checking on the result body (option 
 * default is <b>off</b>).
 * If the option is switched <b>on</b> and a face-face intersection is found in the body, the operation fails.</li>
 * <li>The self-intersecting bodies repair is controlled by the @href lop_options class. It is enabled by default.</li>
 * </ul>
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br>
 * The geometry changes performed by @href api_tweak_open_circuits on the tweaked faces and their neighbors are:
 * <br>
 * <tt>pipe_spl_sur</tt> spline surfaces will be converted to <tt>rb_blend_spl_sur</tt> spline surfaces in the 
 * supplied faces and their neighbors if the option <tt>lop_convert_pipe</tt> is <b>on</b> (default).
 * <br><br>
 * <b>Errors:</b> Some of the following errors will return a <tt>standard_error_info</tt> object in the API 
 * <tt>outcome</tt>.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error message below.
 * (When the <tt>ENTITY</tt> is new and will be lost on rollback, no <tt>standard_error_info</tt> object is 
 * returned).
 * <ul>
 * <li>At least one edge (which specifies one free edge loop) must be supplied, or error <tt>LOP_TWK_NO_EDGE</tt>.</li>
 * <li>Edges must be valid, non-duplicate, belong to different loops and from the same body, or error 
 * <tt>LOP_TWK_BAD_EDGE_SUPPLIED (EDGE*)</tt>.</li>
 * <li>Tool surfaces must be valid, or error <tt>LOP_TWK_BAD_SURFACE (FACE*)</tt>.</li>
 * <li>Body must be manifold, or error <tt>LOP_TWK_NON_MANIFOLD (COEDGE*)</tt>.</li>
 * </ul>
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 48%>New surface geometries must intersect consistently, or errors</td>
 * <td valign = "top"><tt>LOP_TWK_NO_EDGE (EDGE*)</tt> no solution for an edge.</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td valign = "top"><tt>LOP_TWK_NO_VERT (VERTEX*)</tt> no solution for a vertex.</td>
 * </tr>
 * </table>
 * <ul>
 * <li>Internal algorithmic problems produce error <tt>LOP_TWK_INTERNAL</tt>.</li>
 * <li>Unpermitted topology changes, if detected, will produce the error <tt>LOP_TWK_TOPOL_CHANGE (FACE*)</tt>.</li>
 * <li>Box, if supplied, must be valid, or error <tt>LOP_BAD_BOX</tt>.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b> Body must be manifold and solid.
 * <br><br>
 * No other topology changes than those documented above are permitted.
 * @href api_tweak_open_circuits does extend the geometry of the tool surfaces or their neighbors (like other 
 * local operations).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nEdge
 * Specifies the number of edges. Each edge specifies one unique loop. Edges cannot be from the same loop.
 * @param edges
 * Indicates the edges specifying loops which are being tweaked.
 * @param tool_surfaces
 * Specifies new surfaces.
 * @param reverse
 * Specifies the flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param box_low
 * Specifies the start of the intersection box to be used.
 * @param box_high
 * Specifies the end of the intersection box to be used.
 * @param lopts
 * Specifies Local Operations options. NULL argument will set the default values.
 * @param ao
 * Specifies ACIS options.
 **/

DECL_LOP outcome api_tweak_open_circuits(
				int nEdge, 
				EDGE *edges[],
				SURFACE *tool_surfaces[],
				int reverse[], 
                const SPAposition& box_low,
                const SPAposition& box_high,
				lop_options* lopts,
				AcisOptions* ao = NULL);
/** @} */
/**
 * \addtogroup LOPTAPER
 *
 * @{
 */
/**
 * Tapers each given face about the intersection between the surface underlying the face
 * and a given plane by a given draft angle.
 * <b>Technical Article:</b> <i>[Taper Faces](http://doc.spatial.com/articles/t/a/p/Taper_Local_Operations_Functionality_3214.html)</i>
 * <br><br>
 * <b>Role:</b> The purpose of plane taper is to facilitate extraction from a mold rather 
 * than changing the general appearance of a model.
 * So typically the draft angle supplied (specified in radians) would be quite small.
 * <br><br>
 * This local operation replaces the surfaces of supplied faces with new tapered surfaces. 
 * The new surfaces are defined by rotating the rulings along the draft direction on the 
 * supplied faces by the given draft angle about the intersection curve of the draft plane 
 * and the surfaces of the supplied faces.
 * <br><br>
 * From R11 onwards, the definition of the new surfaces made by the plane taper operation 
 * has been changed. They are defined as the envelope surfaces of cones, whose apices ride 
 * along the intersection curve of the respective faces and the taper plane, whose axes 
 * are specified by the taper plane normal, and whose half-angles are equal to the supplied 
 * draft angle.
 * <br><br>
 * Based upon this definition of the taper surface, it may be inferred that the interpretation 
 * of the supplied draft angle is different. In many cases, however, particularly when the 
 * taper surface normals are near orthogonal to the taper plane normal, the shape of the new 
 * surface will be similar to the old taper. Also note that if the <tt>AcisVersion</tt> supplied is
 * less than R11, the old taper surface will continue to be made.
 * <br><br>
 * Note that the intersection between the draft plane and the given surfaces need not lie on 
 * the face to be tapered, and that even when it does, this intersection need not be an edge 
 * of the body (unlike in the command <tt>edge_taper_faces</tt>).
 * <br><br>
 * The taper operation on the faces is essentially determined by three arguments, the faces to 
 * be tapered, the taper plane, and the draft angle.
 * <br><br>
 * The faces to be tapered are given by the first two arguments which are the number of faces 
 * and the face array.
 * The draft plane is defined by the next two arguments which are a position on the plane 
 * and a vector defining its normal.
 * The draft angle is defined by the fifth argument, a double.
 * <br><br>
 * Whether the taper is executed or just validated, it is controlled by the option <tt>validate_lop</tt>.
 * One of its settings is <tt>lop_only</tt> in which the taper is executed.
 * When the option is set to <tt>validate_only</tt>, 
 * the taper is validated, and if validation fails, the 
 * faces which failed to validate are returned in the error return mechanism.
 * <br><br>
 * The default is <tt>validate_and_lop</tt> in which validation is carried out.
 * If the validation fails, faces are returned in the error return mechanism, and the 
 * taper is not attempted.
 * If the validation is successful, it is followed by the actual taper operation.
 * <br><br>
 * Note that successful validation is no guarantee of a successful taper.
 * <br><br>
 * The <tt>box_low</tt> and <tt>box_high</tt> arguments are used to limit any geometrical  
 * intersections and so improve performance.
 * The box must contain the final faces, and if omitted, defaults to the size box.
 * <br><br>
 * Vent face behavior is set through @href lop_options.
 * The argument <tt>vent_type</tt> is of the enumeration type <tt>vent_type_id</tt>. It can take 
 * either of the values <tt>vent_type_none</tt> or <tt>vent_type_add</tt>.
 * This API uses <tt>vent_type_add</tt> as a default behavior. An invalid argument to this parameter 
 * results in the default behavior.
 * <br><br>
 * The <tt>vent_type_none</tt> functionality is used as a different way of tapering. The functionality 
 * is added to handle the tapering without vent faces. 
 * <br><br>
 * When the faces shared by "vent edges" are tangential and one of the faces is a taper face and 
 * the other is a fixed face, one of the following can result:
 * <br><br>
 * <ul>
 * <li>if <tt>vent_type_none</tt> is passed, the surface of the taper face is extended and 
 * intersected with the adjacent fixed face. This eliminates the need for the addition of vent faces.</li>
 * <li>if <tt>vent_type_add</tt> is passed, a vent face is added between the taper face and the fixed face.</li>
 * </ul>
 * <br><br>
 * <b>Notes:</b>
 * <br><br>
 * The input argument <i>draft_angle</i> is specified in radians with this API. However, the same input 
 * argument is specified in degrees with the Scheme interface <tt>lop:taper-faces</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function @href api_tweak_faces
 * <br><br>
 * <ul>
 * <li>Vent faces are added between mergeable faces, when one of them is not being tapered.</li>
 * <li>Vent faces can only be added if there is a face on the original model that shares a vertex with 
 * the mergeable edge, does not have the mergeable edge in its boundary, and that will be adjacent 
 * to the vent faces after the taper.</li>
 * <li>Vent faces can also be added at tangent edges when just one of the two faces that share the edge 
 * is being tapered.</li>
 * </ul>
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function @href api_tweak_faces
 * <br><br>
 * Valid normal must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Valid angle (between -Pi/2 and +Pi/2) must be supplied, or error <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>
 * <br>
 * (outcome points to <tt>standard_error_info</tt> containing a pointer to a FACE*).
 * <br><br>
 * Normal of face to be tapered should not be parallel to draft vector, or error <tt>LOP_TAP_NO_SURF(FACE*)</tt>.
 * <br><br>
 * Must be able to find a face adjacent to vent face if a vent face is inserted, or 
 * error <tt>LOP_TAP_NO_ADJ_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the limitations listed for the function @href api_tweak_faces .
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param point
 * draft plane point.
 * @param normal
 * draft plane normal.
 * @param draft_angle
 * draft angle specified in radians. 
 * Note that the Scheme interface <tt>lop:taper-faces</tt> has draft angle specified in degrees.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_taper_faces(
								 int const             nface,
								 FACE*                 face[],
								 SPAposition const&    point,
								 SPAunit_vector const& normal,
								 double const&         draft_angle,
								 const SPAposition&    box_low,
								 const SPAposition&    box_high,
								 lop_options*          pLopts,
								 AcisOptions*          ao        = NULL
								 );
/**
 * @nodoc
 * Tapers each given face about the intersection between the surface underlying the face
 * and a given plane by a given draft angle.
 * <br><br>
 * <b>Role:</b> The purpose of plane taper is to facilitate extraction from a mold rather 
 * than changing the general appearance of a model.
 * So typically the draft angle supplied would be quite small.
 * <br><br>
 * This local operation replaces the surfaces of supplied faces with new tapered surfaces. 
 * The new surfaces are defined by rotating the rulings along the draft direction on the 
 * supplied faces by the given draft angle about the intersection curve of the draft plane 
 * and the surfaces of the supplied faces.
 * From R11 onwards, the definition of the new surfaces made by the plane taper operation 
 * has been changed. They are defined as the envelope surfaces of cones, whose apices ride 
 * along the intersection curve of the respective faces and the taper plane, whose axes 
 * are specified by the taper plane normal, and whose half-angles are equal to the supplied 
 * draft angle.
 * Based upon this definition of the taper surface, it may be inferred that the interpretation 
 * of the supplied draft angle is different. In many cases however, particularly when the 
 * taper surface normals are near orthogonal to the taper plane normal, the shape of the new 
 * surface will be similar to the old taper. Also note that if the <tt>AcisVersion</tt> supplied is
 * less than R11 the old taper surface will continue to be made.
 * Note that the intersection between the draft plane and the given surfaces need not lie on 
 * the face to be tapered, and that even when it does, this intersection need not be an edge 
 * of the body (unlike in the command <tt>edge_taper_faces</tt>).
 * <br><br>
 * The taper operation on the faces is essentially determined by three arguments, the faces to 
 * be tapered, the taper plane  and the draft angle.
 * <br><br>
 * The faces to be tapered are given by the first two arguments which are the number of faces 
 * and the face array.
 * The draft plane is defined by the next two arguments which are a position on the plane 
 * and a vector defining its normal.
 * The draft angle is defined by the fifth argument, a double.
 * <br><br>
 * Whether the taper is executed or just validated is controlled by the option <tt>validate_lop</tt>.
 * One of its settings is <tt>lop_only</tt> in which the taper is executed.
 * When set to <tt>validate_only</tt> the taper is validated and if validation fails, the 
 * faces which failed to validate are returned in the error return mechanism.
 * The default is <tt>validate_and_lop</tt> in which validation is carried out.
 * If the validation fails, faces are returned in the error return mechanism, and the 
 * taper is not attempted.
 * If the validation is successful, it is followed by the actual taper operation.
 * <br><br>
 * Note that successful validation is no guarantee of a successful taper.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * The argument <tt>vent_type</tt> is of the enumeration type <tt>vent_type_id</tt>. It can take 
 * one of the following values: <tt>vent_type_none</tt>, <tt>vent_type_add</tt>.
 * This API uses <tt>vent_type_add</tt> as a default behavior. Invalid argument to this parameter 
 * will result in the default behavior.
 * <br><br>
 * The <tt>vent_type_none</tt> functionality is used as a different way of tapering. The functionality 
 * is added to handle the tapering without vent faces.
 * When the faces shared by "vent edges" are tangential and one of the faces is a taper face and 
 * the other is a fixed face, one of the following can result:
 * <br><br>
 * <ul>
 * <li>if <tt>vent_type_none</tt> is passed, the surface of the taper face is extended and 
 * intersected with the adjacent fixed face. This eliminates the need for the addition of vent faces.</li>
 * <li>if <tt>vent_type_add</tt> is passed, a vent face is added between the taper face and the fixed face.</li>
 * </ul>
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function @href api_tweak_faces
 * <br><br>
 * Vent faces are added between mergeable faces, when one of them is not being tapered.
 * Vent faces can only be added if there is a face on the original model that shares a vertex with 
 * the mergeable edge, does not have the mergeable edge in its boundary, and that will be adjacent 
 * to the vent faces after the taper.
 * Vent faces can also be added at tangent edges when just one of the two faces that share the edge is being tapered.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function @href api_tweak_faces
 * <br><br>
 * Valid normal must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Valid angle ( between -90 and +90 ) must be supplied, or error <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>
 * <br>
 * (outcome points to <tt>standard_error_info</tt> containing a pointer to a FACE*).
 * <br><br>
 * Normal of face to be tapered should not be parallel to draft vector, or error <tt>LOP_TAP_NO_SURF(FACE*)</tt>.
 * <br><br>
 * Must be able to find a face adjacent to vent face if a vent face is inserted, or 
 * error <tt>LOP_TAP_NO_ADJ_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the limitations listed for the function @href api_tweak_faces .
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param point
 * draft plane point.
 * @param normal
 * draft plane normal.
 * @param draft_angle
 * draft angle.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param vent_type
 * taper with or without vent faces.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_taper_faces(
								 int const             nface,
								 FACE*                 face[],
								 SPAposition const&    point,
								 SPAunit_vector const& normal,
								 double const&         draft_angle,
								 SPAposition           box_low,
								 SPAposition           box_high,
								 vent_type_id          vent_type = vent_type_add,
								 AcisOptions*          ao        = NULL
								 );
/**
 * Tapers an array of faces about an array of corresponding edges and a supplied
 * draft direction by a given draft angle.
 * <b>Technical Article:</b> <i>[Taper Faces](http://doc.spatial.com/articles/t/a/p/Taper_Local_Operations_Functionality_3214.html)</i>
 * <br><br>
 * <b>Role:</b> The purpose of edge taper is to facilitate extraction from a mould rather 
 * than changing the general appearance of a model.
 * So typically the draft angle supplied would be quite small.
 * <br><br>
 * This local operation replaces the surfaces of supplied faces with new tapered surfaces. 
 * The new surfaces are defined by rotating the rulings along the draft direction on the 
 * supplied faces by the given draft angle about the corresponding edges.
 * From R11 onwards, the definition of the new surfaces made by the edge taper operation 
 * has been changed. They are defined as the envelope surfaces of cones, whose apices ride 
 * along the respective face edge curves, whose axes are given by the supplied draft angle,
 * and whose half-angles are equal to the supplied draft angle. Based upon this definition 
 * of the taper surface, it may be inferred that the interpretation of the supplied draft 
 * angle is different. In many cases however, particularly when the taper surface normals are
 * near orthogonal to the supplied draft direction, the shape of the new surface will be 
 * similar to the old taper. Also note that if the <tt>AcisVersion</tt> supplied is less 
 * than R11 the old taper surface will continue to be made.
 * Note that the corresponding edge must lie on the face to be tapered.
 * <br><br>
 * The taper operation on the faces is determined by the number of faces, the faces, the edges,
 * the draft direction and the draft angle.
 * <br><br>
 * If a face is to be tapered about several edges, these edges must all be supplied, which 
 * means that some faces may have to be supplied more than once.
 * This will lead to a topology change (see below).
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * The box supplied must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * If the option <tt>"validate_lop"</tt> is changed to <tt>"validate_only"</tt> then 
 * <tt>api_edge_taper_faces</tt> does only a validation of the edge taper.
 * If the validation fails, the faces and edges which have caused it to fail are returned 
 * using the error return mechanism.
 * <br><br>
 * If the option is set to <tt>"validate_and_lop"</tt>, validation is carried out.
 * If the validation fails, edge and faces are returned in the error return mechanism, 
 * and the edge-taper is not attempted.
 * If the validation is successful, it is followed by the actual edge taper.
 * <br><br>
 * Note that successful validation is no guarantee of a successful edge-taper.
 * No validation is carried out with the final setting of the option, which is <tt>"lop_only"</tt>.
 * The default is <tt>"validate_and_lop"</tt>.
 * <br><br>
 * Add vent face behavior is set through <tt>lop_options</tt> :
 * The argument <tt>vent_type</tt> is of the enumeration type <tt>vent_type_id</tt>. It can take
 * one of the following values: <tt>vent_type_none</tt>, <tt>vent_type_add</tt>.
 * This API uses <tt>vent_type_add</tt> as a default behavior. Invalid argument to this 
 * parameter will result in the default behavior.
 * <br><br>
 * The <tt>vent_type_none</tt> functionality is used as a different way of tapering. The 
 * functionality is added to handle the tapering without vent faces.
 * <br><br>
 * When the faces shared by "vent edges" are tangential and one of the faces is a taper face 
 * and the other is a fixed face, one of the following can result:
 * <br><br>
 * <ul>
 * <li>if <tt>vent_type_none</tt> is passed, the surface of the taper face is extended and 
 * intersected with the adjacent fixed face. This eliminates the need for the addition of vent faces.</li>
 * <li>if <tt>vent_type_add</tt> is passed, a vent face is added between the taper face and the fixed face.</li>
 * </ul>
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * If a face is being tapered about more than one edge, a new edge will be introduced that splits 
 * the face into several bits (one per edge about which the original face is to be tapered).
 * <br><br>
 * Vent faces are added between mergeable faces, when one of them is not being tapered.
 * Vent faces can only be added if there is a face on the original model that shares a vertex with 
 * the mergeable edge, does not have the mergeable edge in its boundary, and that will be adjacent to 
 * the vent faces after the taper.
 * Vent faces can also be added at tangent edges when just one of the two faces that share the 
 * edge is being tapered.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces
 * <br><br>
 * Valid angle (between -90 and +90 degrees) must be supplied, or error <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Valid direction must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>.
 * <br><br>
 * Supplied edges must lie on corresponding faces, or error <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * Supplied edges must not be parallel to draft vector at any point, or error <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * Normal of face to be tapered should not be parallel to draft vector, or error <tt>LOP_TAP_NO_SURF (FACE*)</tt>.
 * <br><br>
 * Must be able to find a face adjacent to every vent face, or error <tt>LOP_TWK_NO_ADJ_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> In addition to the following, refer to the limitations listed for the function, 
 * @href api_tweak_faces .
 * <br><br>
 * The supplied edge curves may not be parallel to the supplied draft direction at any point.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param edge
 * edges to be tapered about.
 * @param draft_dir
 * draft direction.
 * @param draft_angle
 * draft angle.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_edge_taper_faces(
									  int const             nface,
									  FACE*                 face[],
									  EDGE*                 edge[],
									  SPAunit_vector const& draft_dir,
									  double const&         draft_angle,
									  const SPAposition&    box_low,
									  const SPAposition&    box_high,
									  lop_options*          pLopts,
									  AcisOptions*          ao        = NULL
									  );
/**
 * @nodoc
 * Tapers an array of faces about an array of corresponding edges and a supplied
 * draft direction by a given draft angle.
 * <br><br>
 * <b>Role:</b> The purpose of edge taper is to facilitate extraction from a mould rather 
 * than changing the general appearance of a model.
 * So typically the draft angle supplied would be quite small.
 * <br><br>
 * This local operation replaces the surfaces of supplied faces with new tapered surfaces. 
 * The new surfaces are defined by rotating the rulings along the draft direction on the 
 * supplied faces by the given draft angle about the corresponding edges.
 * From R11 onwards, the definition of the new surfaces made by the edge taper operation 
 * has been changed. They are defined as the envelope surfaces of cones, whose apices ride 
 * along the respective face edge curves, whose axes are given by the supplied draft angle,
 * and whose half-angles are equal to the supplied draft angle. Based upon this definition 
 * of the taper surface, it may be inferred that the interpretation of the supplied draft 
 * angle is different. In many cases however, particularly when the taper surface normals are
 * near orthogonal to the supplied draft direction, the shape of the new surface will be 
 * similar to the old taper. Also note that if the <tt>AcisVersion</tt> supplied is less 
 * than R11 the old taper surface will continue to be made.
 * Note that the corresponding edge must lie on the face to be tapered.
 * <br><br>
 * The taper operation on the faces is determined by the number of faces, the faces, the edges,
 * the draft direction and the draft angle.
 * <br><br>
 * If a face is to be tapered about several edges, these edges must all be supplied, which 
 * means that some faces may have to be supplied more than once.
 * This will lead to a topology change (see below).
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * The box supplied must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * If the option <tt>"validate_lop"</tt> is changed to <tt>"validate_only"</tt> then 
 * <tt>api_edge_taper_faces</tt> does only a validation of the edge taper.
 * If the validation fails, the faces and edges which have caused it to fail are returned 
 * using the error return mechanism.
 * <br><br>
 * If the option is set to <tt>"validate_and_lop"</tt>, validation is carried out.
 * If the validation fails, edge and faces are returned in the error return mechanism, 
 * and the edge-taper is not attempted.
 * If the validation is successful, it is followed by the actual edge taper.
 * <br><br>
 * Note that successful validation is no guarantee of a successful edge-taper.
 * No validation is carried out with the final setting of the option, which is <tt>"lop_only"</tt>.
 * The default is <tt>"validate_and_lop"</tt>.
 * <br><br>
 * The argument <tt>vent_type</tt> is of the enumeration type <tt>vent_type_id</tt>. It can take
 * one of the following values: <tt>vent_type_none</tt>, <tt>vent_type_add</tt>.
 * This API uses <tt>vent_type_add</tt> as a default behavior. Invalid argument to this 
 * parameter will result in the default behavior.
 * <br><br>
 * The <tt>vent_type_none</tt> functionality is used as a different way of tapering. The 
 * functionality is added to handle the tapering without vent faces.
 * When the faces shared by "vent edges" are tangential and one of the faces is a taper face 
 * and the other is a fixed face, one of the following can result:
 * <br><br>
 * <ul>
 * <li>if <tt>vent_type_none</tt> is passed, the surface of the taper face is extended and 
 * intersected with the adjacent fixed face. This eliminates the need for the addition of vent faces.</li>
 * <li>if <tt>vent_type_add</tt> is passed, a vent face is added between the taper face and the fixed face.</li>
 * </ul>
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * If a face is being tapered about more than one edge, a new edge will be introduced that splits 
 * the face into several bits (one per edge about which the original face is to be tapered).
 * <br><br>
 * Vent faces are added between mergeable faces, when one of them is not being tapered.
 * Vent faces can only be added if there is a face on the original model that shares a vertex with 
 * the mergeable edge, does not have the mergeable edge in its boundary, and that will be adjacent to 
 * the vent faces after the taper.
 * Vent faces can also be added at tangent edges when just one of the two faces that share the 
 * edge is being tapered.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces
 * <br><br>
 * Valid angle (between -90 and +90 degrees) must be supplied, or error <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Valid direction must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>.
 * <br><br>
 * Supplied edges must lie on corresponding faces, or error <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * Supplied edges must not be parallel to draft vector at any point, or error <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * Normal of face to be tapered should not be parallel to draft vector, or error <tt>LOP_TAP_NO_SURF (FACE*)</tt>.
 * <br><br>
 * Must be able to find a face adjacent to every vent face, or error <tt>LOP_TWK_NO_ADJ_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> In addition to the following, refer to the limitations listed for the function, 
 * @href api_tweak_faces .
 * <br><br>
 * The supplied edge curves may not be parallel to the supplied draft direction at any point.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param edge
 * edges to be tapered about.
 * @param draft_dir
 * draft direction.
 * @param draft_angle
 * draft angle.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used
 * @param vent_type
 * taper with or without vent faces.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_edge_taper_faces(
									  int const             nface,
									  FACE*                 face[],
									  EDGE*                 edge[],
									  SPAunit_vector const& draft_dir,
									  double const&         draft_angle,
									  SPAposition           box_low,
									  SPAposition           box_high,
									  vent_type_id          vent_type = vent_type_add,
									  AcisOptions*          ao        = NULL
									  );

/**
 * Inserts a ruled face which tangentially meets the supplied face, and which covers the region 
 * in the shadow cast from a light source.
 * <b>Technical Article:</b> <i>[Taper Faces](http://doc.spatial.com/articles/t/a/p/Taper_Local_Operations_Functionality_3214.html)</i>
 * <br><br>
 * <b>Role:</b> The purpose of shadow taper is to facilitate extraction from a mold rather than 
 * changing the general appearance of a model.
 * So typically the angle supplied would be very small.
 * <br><br>
 * Inserts a ruled face which tangentially meets the supplied face, and which covers the region 
 * that is in the shadow that is cast from a light source which is directed from an infinite 
 * point and is at a given angle from the supplied draft direction.
 * <br><br>
 * Replaces regions of the face to be shadow tapered where the angle between the normal and the 
 * draft vector is greater than the complement of the given angle (that is , greater than 90 - 
 * given angle) by ruled faces where the angle between the draft vector and the normal is exactly 
 * the complement of the given angle.
 * Thus the ruled faces meet the face to be tapered tangentially and are bounded by the 
 * intersections with either neighboring faces or the same face.
 * The regions to be replaced are referred to as "being in shadow" with respect to the draft 
 * vector and the given angle.
 * <br><br>
 * The complement was chosen rather than the given angle itself in order to keep shadow tapering 
 * in line with taper faces and edge taper, where a small angle provokes a smaller change than a 
 * large angle.
 * <br><br>
 * The shadow taper operation on the faces is determined by the number of faces, the faces, the 
 * direction and the angle.
 * <br><br>
 * The draft direction is defined by a vector and the draft angle is defined by a double.
 * <br><br>
 * The <tt>box_low</tt> and <tt>box_high</tt> arguments are used to limit any geometrical 
 * intersections and so improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * Almost every shadow taper will result in a topology change as the parts of the body that are 
 * "in shadow" are replaced by ruled faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid direction must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Valid angle (at least 0 and less than 90 degrees) must be supplied or error 
 * <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>
 * <br>
 * (outcome points to <tt>standard_error_info</tt> containing a pointer to a FACE*).
 * <br><br>
 * <b>Limitations:</b> In addition to the following, refer to the Limitations listed for the 
 * function, @href api_tweak_faces
 * <br><br>
 * More than one face that is in the shadow cannot be deleted.
 * <br><br>
 * In general, the model must contain a face that is adjacent to the face with the silhouettes 
 * and that is intersected by the new face.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param draft_dir
 * draft direction.
 * @param draft_angle
 * draft angle.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_shadow_taper_faces(
										int const             nface,
										FACE*                 face[],
										SPAunit_vector const& draft_dir,
										double const&         draft_angle,
										const SPAposition&    box_low,
										const SPAposition&    box_high,
										lop_options*          pLopts,
										AcisOptions*          ao = NULL
										);
/**
 * @nodoc
 * Inserts a ruled face which tangentially meets the supplied face, and which covers the region 
 * in the shadow cast from a light source.
 * <br><br>
 * <b>Role:</b> The purpose of shadow taper is to facilitate extraction from a mold rather than 
 * changing the general appearance of a model.
 * So typically the angle supplied would be very small.
 * <br><br>
 * Inserts a ruled face which tangentially meets the supplied face, and which covers the region 
 * that is in the shadow that is cast from a light source which is directed from an infinite 
 * point and is at a given angle from the supplied draft direction.
 * <br><br>
 * Replaces regions of the face to be shadow tapered where the angle between the normal and the 
 * draft vector is greater than the complement of the given angle (that is , greater than 90 - 
 * given angle) by ruled faces where the angle between the draft vector and the normal is exactly 
 * the complement of the given angle.
 * Thus the ruled faces meet the face to be tapered tangentially and are bounded by the 
 * intersections with either neighboring faces or the same face.
 * The regions to be replaced are referred to as "being in shadow" with respect to the draft 
 * vector and the given angle.
 * <br><br>
 * The complement was chosen rather than the given angle itself in order to keep shadow tapering 
 * in line with taper faces and edge taper, where a small angle provokes a smaller change than a 
 * large angle.
 * <br><br>
 * The shadow taper operation on the faces is determined by the number of faces, the faces, the 
 * direction and the angle.
 * <br><br>
 * The draft direction is defined by a vector and the draft angle is defined by a double.
 * <br><br>
 * The <tt>box_low</tt> and <tt>box_high</tt> arguments are used to limit any geometrical 
 * intersections and so improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * Almost every shadow taper will result in a topology change as the parts of the body that are 
 * "in shadow" are replaced by ruled faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid direction must be supplied, or error <tt>LOP_TAP_BAD_NORMAL</tt>.
 * <br><br>
 * Valid angle (at least 0 and less than 90 degrees) must be supplied or error 
 * <tt>LOP_TAP_BAD_ANGLE</tt>.
 * <br><br>
 * Surface must be able to be tapered as requested, or error <tt>LOP_TAP_NO_SURF</tt>
 * <br>
 * (outcome points to <tt>standard_error_info</tt> containing a pointer to a FACE*).
 * <br><br>
 * <b>Limitations:</b> In addition to the following, refer to the Limitations listed for the 
 * function, @href api_tweak_faces
 * <br><br>
 * More than one face that is in the shadow cannot be deleted.
 * <br><br>
 * In general, the model must contain a face that is adjacent to the face with the silhouettes 
 * and that is intersected by the new face.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tapered.
 * @param const&draft_dir
 * draft direction.
 * @param draft_angle
 * draft angle.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_shadow_taper_faces(
										int const             nface,
										FACE*                 face[],
										SPAunit_vector const& draft_dir,
										double const&         draft_angle,
										SPAposition           box_low,
										SPAposition           box_high,
										AcisOptions*          ao = NULL
										);
/** @} */
/**
 * \addtogroup LOPMOVE
 *
 * @{
 */
/**
 * Moves an array of faces through a transform.
 * <b>Technical Article:</b> <i>[Move Faces](http://doc.spatial.com/articles/m/o/v/Move_Local_Operations_Functionality_b263.html)</i>
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces moved by a transform.
 * <br><br>
 * The move operation on the faces is determined by two arguments, the number of faces and 
 * which faces.
 * <br><br>
 * The movement is defined by the transform.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Transformation must be a rigid motion (i.e., the product of rotations and
 * translations) that is not the identity or the translation vector must be
 * non-zero, or error <tt>LOP_MOVE_BAD_TRANSFORM</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being moved.
 * @param tr
 * transform used.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_move_faces(
								int const        nface,
								FACE*            face[],
								SPAtransf const& tr,
								const SPAposition& box_low,
								const SPAposition& box_high,
								lop_options*     pLopts,
								AcisOptions*     ao = NULL
								);
/**
 * @nodoc
 * Moves an array of faces through a transform.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces moved by a transform.
 * <br><br>
 * The move operation on the faces is determined by two arguments, the number of faces and 
 * which faces.
 * <br><br>
 * The movement is defined by the transform.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Transformation must be a rigid motion (i.e., the product of rotations and
 * translations) that is not the identity or the translation vector must be
 * non-zero, or error <tt>LOP_MOVE_BAD_TRANSFORM</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being moved.
 * @param tr
 * transform used.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_move_faces(
								int const        nface,
								FACE*            face[],
								SPAtransf const& tr,
								SPAposition      box_low,
								SPAposition      box_high,
								AcisOptions*     ao = NULL
								);
/** @} */
/**
 * \addtogroup LOPOFFSET
 *
 * @{
 */							
/**
 * Offsets an array of faces based on a given distance.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces offset by offset distance.
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset operation on the faces is determined by two arguments, the number of faces and 
 * which faces.
 * <br><br>
 * The offset distance is defined by a double.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * Refer <tt>Failsafe Offsetting</tt> section for information on obtaining a failsafe solution 
 * if all faces of the body are offsetted.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (spheres, cones, blended edges, blended vertices, and tori) are removed 
 * and the resulting wound healed by the surrounding face surfaces, before the offset.
 * <br>
 * Passing in (0,0,0) for <i>box_low</i> and <i>box_high</i> indicates that you are 
 * not specifying a box.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Faces supplied should not be double sided or error <tt>LOP_DBL_SIDED_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br>
 * Double-sided faces are invalid input.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral 
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param offset
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_offset_faces(
								  int const    nface,
								  FACE*        face[],
								  double       offset,
								  const SPAposition&  box_low,
								  const SPAposition&  box_high,
								  lop_options* pLopts,
								  AcisOptions* ao = NULL
								  );
/**
 * @nodoc
 * Offsets an array of faces based on a given distance.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces offset by offset distance.
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset operation on the faces is determined by two arguments, the number of faces and 
 * which faces.
 * <br><br>
 * The offset distance is defined by a double.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (spheres, cones, blended edges, blended vertices, and tori) are removed 
 * and the resulting wound healed by the surrounding face surfaces, before the offset.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Faces supplied should not be double sided or error <tt>LOP_DBL_SIDED_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br>
 * Double-sided faces are invalid input.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral 
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param offset
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_offset_faces(
								  int const    nface,
								  FACE*        face[],
								  double       offset,
								  SPAposition  box_low,
								  SPAposition  box_high,
								  AcisOptions* ao = NULL
								  );
/**
 * Offsets an array of faces, each face as required.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces offset.
 * Unless in the specific array, each face in the face array is offset by the default 
 * offset distance.
 * If a face is in the specific array, the face is offset by the amount in the corresponding 
 * entry in the specific distance array.
 * Note that faces with specific offsets must not be in the array of faces to be offset by 
 * the default offset distance.
 * <br><br>
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset operation on the faces is determined by four arguments, the number of faces and 
 * which faces, the number of faces with specific offsets and the faces with specific offsets.
 * <br><br>
 * The default offset distance is defined by a double.
 * The specific offset distances are defined by an array of doubles.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Refer <tt>Failsafe Offsetting</tt> section for information on obtaining a failsafe solution 
 * if all faces of the body are offsetted.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (spheres, cones, blended edges, blended vertices, and tori) are removed and 
 * the resulting wound healed by the surrounding face surfaces,before the offset.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>) or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Faces should not be repeated in default and or specific faces arrays, and faces in one array 
 * should not be present in other or error <tt>LOP_TWK_BAD_FACE</tt>.
 * <br><br>
 * Faces supplied should not be double sided or error <tt>LOP_DBL_SIDED_FACE</tt>.
 * <br><br>
 * Offset surfaces should be made on every non-radial surface supplied or error 
 * <tt>LOP_OFF_NO_SURF</tt>.
 * <br><br>
 * Degenerating radial faces are removed and the resulting gap left healed or error 
 * <tt>REM_NO_SOLUTION</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param n_def_face
 * number of faces offset by default distance.
 * @param def_face
 * array of faces being offset by default distance.
 * @param def_offset
 * default distance to offset.
 * @param n_spec_face
 * number of faces with specific offset distances.
 * @param spec_face
 * array of faces with specific offset distances.
 * @param spec_offset
 * array of specific offset distances.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_offset_faces_specific(
											int const&   n_def_face,
											FACE*        def_face[],
											double       def_offset,
											int const &  n_spec_face,
											FACE*        spec_face[],
											double       spec_offset[],
											const SPAposition&  box_low,
											const SPAposition&  box_high,
											lop_options* pLopts,
											AcisOptions* ao = NULL
											);
/**
 * @nodoc
 * Offsets an array of faces, each face as required.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces offset.
 * Unless in the specific array, each face in the face array is offset by the default 
 * offset distance.
 * If a face is in the specific array, the face is offset by the amount in the corresponding 
 * entry in the specific distance array.
 * Note that faces with specific offsets must not be in the array of faces to be offset by 
 * the default offset distance.
 * <br><br>
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset operation on the faces is determined by four arguments, the number of faces and 
 * which faces, the number of faces with specific offsets and the faces with specific offsets.
 * <br><br>
 * The default offset distance is defined by a double.
 * The specific offset distances are defined by an array of doubles.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (spheres, cones, blended edges, blended vertices, and tori) are removed and 
 * the resulting wound healed by the surrounding face surfaces,before the offset.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>) or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Faces should not be repeated in default and or specific faces arrays, and faces in one array 
 * should not be present in other or error <tt>LOP_TWK_BAD_FACE</tt>.
 * <br><br> 
 * Faces supplied should not be double sided or error <tt>LOP_DBL_SIDED_FACE</tt>.
 * <br><br>
 * Offset surfaces should be made on every non-radial surface supplied or error 
 * <tt>LOP_OFF_NO_SURF</tt>.
 * <br><br>
 * Degenerating radial faces are removed and the resulting gap left healed or error 
 * <tt>REM_NO_SOLUTION</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param n_def_face
 * number of faces offset by default distance.
 * @param def_face
 * array of faces being offset by default distance.
 * @param def_offset
 * default distance to offset.
 * @param n_spec_face
 * number of faces with specific offset distances.
 * @param spec_face
 * array of faces with specific offset distances.
 * @param spec_offset
 * array of specific offset distances.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_offset_faces_specific(
										int const&   n_def_face,
										FACE*        def_face[],
										double       def_offset,
										int const&   n_spec_face,
										FACE*        spec_face[],
										double       spec_offset[],
										SPAposition  box_low,
										SPAposition  box_high,
										AcisOptions* ao = NULL
										);
/**
 * Offsets all faces of a body based on a given distance.
 * <b>Technical Article:</b> <i>[Body Offset](http://doc.spatial.com/articles/b/o/d/Body_Offset_Local_Operations_Functionality_6044.html)</i>
 * <br><br>
 * <b>Role:</b> Offsets the faces of the supplied body by an offset distance.
 * Faces with radial surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset body operation is defined by the supplied body and a given distance.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * Refer <tt>Failsafe Offsetting</tt> section for information on obtaining a failsafe solution.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (that is , spheres, cones, blended edges, blended vertices and tori) are 
 * removed and the resulting wound healed by the surrounding face surfaces, before the offset.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>) or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Offset surfaces should be made on every non-radial surface supplied or error 
 * <tt>LOP_OFF_NO_SURF</tt>.
 * <br><br>
 * Degenerating radial faces are removed and the resulting gap left healed or error 
 * <tt>REM_NO_SOLUTION</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param body
 * body to be offset.
 * @param offset
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_offset_body(
									BODY*        body,
									double       offset,
									const SPAposition&  box_low,
									const SPAposition&  box_high,
									lop_options* pLopts,
									AcisOptions* ao = NULL
									);
 /**
 * @nodoc
 * Offsets all faces of a body based on a given distance.
 * <br><br>
 * <b>Role:</b> Offsets the faces of the supplied body by an offset distance.
 * Faces with radial surfaces which cannot be so offset are removed and the resulting wound 
 * healed by the surrounding face surfaces.
 * <br><br>
 * The offset body operation is defined by the supplied body and a given distance.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so 
 * improve performance.
 * Limiting the intersection improves performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * Mergeable edges will be retained provided they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * In addition to the following, refer to the topology changes listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Faces with radial surfaces which cannot be offset by the distance without degenerating 
 * or inverting (that is , spheres, cones, blended edges, blended vertices and tori) are 
 * removed and the resulting wound healed by the surrounding face surfaces, before the offset.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Valid offset (greater than minus the body box max side), and not a zero offset (magnitude 
 * greater than twice <tt>SPAresabs</tt>) or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Offset surfaces should be made on every non-radial surface supplied or error 
 * <tt>LOP_OFF_NO_SURF</tt>.
 * <br><br>
 * Degenerating radial faces are removed and the resulting gap left healed or error 
 * <tt>REM_NO_SOLUTION</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param body
 * body to be offset.
 * @param offset
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_offset_body(
									BODY *body,
									double offset,
									SPAposition box_low,
									SPAposition box_high,
									AcisOptions *ao = NULL
									);
/** @} */
/**
 * \addtogroup LOPTWEAK
 *
 * @{
 */

/**
 * Fixes an edge prior to a tweak.
 * <br><br>
 * <b>Role:</b> Places <tt>ATTRIB_LOP_EDGE</tt> on edge containing new geometry for the 
 * edge to be used during the next tweak.
 * <tt>NULL</tt> new geometry implies that the geometry must not change during the tweak.
 * Data for an edge from the most recent call referring to it applies.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ed
 * edge to be fixed.
 * @param cu
 * new geometry (may be NULL).
 * @param correct_dir
 * specifies whether curve direction is correct ( if curve is provided )
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_tweak_fix_edge(
									EDGE *ed,
									CURVE *cu,
									logical correct_dir,
									AcisOptions *ao = NULL
									);

 /**
 * @nodoc
 * Fixes an edge prior to a tweak.
 * <br><br>
 * <b>Role:</b> Places <tt>ATTRIB_LOP_EDGE</tt> on edge containing new geometry for the 
 * edge to be used during the next tweak.
 * <tt>NULL</tt> new geometry implies that the geometry must not change during the tweak.
 * Data for an edge from the most recent call referring to it applies.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ed
 * edge to be fixed.
 * @param cu
 * new geometry (may be NULL).
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_tweak_fix_edge(
									EDGE *ed,
									CURVE *cu,
									AcisOptions *ao = NULL
									);
/**
 * Fixes a vertex prior to a tweak.
 * <br><br>
 * <b>Role:</b> Places <tt>ATTRIB_LOP_VERTEX</tt> on vertex containing new geometry for 
 * the vertex to be used during the next tweak.
 * <tt>NULL</tt> new geometry implies that the geometry must not change during the tweak.
 * Data for an vertex from the most recent call referring to it applies.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param vt
 * vertex to be fixed.
 * @param ap
 * new geometry (may be NULL).
 * @param set_tolerant
 * specifies whether to set this vertex as tolerant
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_fix_vertex( VERTEX *vt,
									   APOINT *ap,
									   logical set_tolerant,
									   AcisOptions *ao = NULL);

/**
 * @nodoc
 * Fixes a vertex prior to a tweak.
 * <br><br>
 * <b>Role:</b> Places <tt>ATTRIB_LOP_VERTEX</tt> on vertex containing new geometry for 
 * the vertex to be used during the next tweak.
 * <tt>NULL</tt> new geometry implies that the geometry must not change during the tweak.
 * Data for an vertex from the most recent call referring to it applies.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param vt
 * vertex to be fixed.
 * @param ap
 * new geometry (may be NULL).
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_fix_vertex( VERTEX *vt,
									   APOINT *ap,
									   AcisOptions *ao = NULL);


/**
 * Sweeps an array of faces along a path defined by the faces adjacent the given faces.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces moved along the sweep path.
 * The sweep path is determined by a face that is not in that set of faces given but is 
 * adjacent to at least one of those faces in the set.
 * This face must determine the sweep path.
 * This means that it must be a cone, cylinder, plane, torus, <tt>sweep_spl_sur</tt>, 
 * <tt>rot_spl_sur</tt>, <tt>sum_spl_sur</tt>, or an offset of one of these surfaces.
 * <br><br>
 * The optional intersection box limits the size of intersections between surfaces which 
 * might otherwise be very long.
 * It cannot be used to choose faces.
 * Its main purpose is to speed up complicated cases where the intersection curves might 
 * be very long, thus improving performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Some of the following errors will return a <tt>standard_error_info</tt> object in the API 
 * outcome.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error 
 * message below.
 * <br><br>
 * Distance must be non-zero, or error <tt>LOP_MOVE_BAD_TRANSFORM</tt>.
 * <br><br>
 * Rail and path information on adjacent swept face must be good, or error 
 * <tt>LOP_SWP_BAD_RAIL_OR_PATH (FACE *)</tt>.
 * <br><br>
 * <b>Limitations:</b> If the first face is not planar and does not intersect the sweep path,
 * ambiguous results may occur.
 * <br><br>
 * Refer to @href api_tweak_faces for more limitations.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being moved.
 * @param dist
 * distant to sweep faces.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_sweep_more(
								int          nface,
								FACE*        face[],
								double       dist,
								const SPAposition&  box_low,
								const SPAposition&  box_high,
								lop_options* pLopts,
								AcisOptions* ao = NULL
								);
 /**
 * @nodoc
 * Sweeps an array of faces along a path defined by the faces adjacent the given faces.
 * <br><br>
 * <b>Role:</b> Replaces surfaces of supplied faces with surfaces moved along the sweep path.
 * The sweep path is determined by a face that is not in that set of faces given but is 
 * adjacent to at least one of those faces in the set.
 * This face must determine the sweep path.
 * This means that it must be a cone, cylinder, plane, torus, <tt>sweep_spl_sur</tt>, 
 * <tt>rot_spl_sur</tt>, <tt>sum_spl_sur</tt>, or an offset of one of these surfaces.
 * <br><br>
 * The optional intersection box limits the size of intersections between surfaces which 
 * might otherwise be very long.
 * It cannot be used to choose faces.
 * Its main purpose is to speed up complicated cases where the intersection curves might 
 * be very long, thus improving performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * Some of the following errors will return a <tt>standard_error_info</tt> object in the API 
 * outcome.
 * In these cases, the type of <tt>ENTITY</tt> returned, in parentheses, follows the error 
 * message below.
 * <br><br>
 * Distance must be non-zero, or error <tt>LOP_MOVE_BAD_TRANSFORM</tt>.
 * <br><br>
 * Rail and path information on adjacent swept face must be good, or error 
 * <tt>LOP_SWP_BAD_RAIL_OR_PATH (FACE *)</tt>.
 * <br><br>
 * <b>Limitations:</b> If the first face is not planar and does not intersect the sweep path,
 * ambiguous results may occur.
 * <br><br>
 * Refer to @href api_tweak_faces for more limitations.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being moved.
 * @param dist
 * distant to sweep faces.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_sweep_more( int          nface,
                                 FACE*        face[],
                                 double       dist,
                                 SPAposition  box_low,
                                 SPAposition  box_high,
                                 AcisOptions* ao = NULL);
/**
 * Extends an array of surfaces for use in <tt>api_tweak_faces</tt>.
 * <br><br>
 * <b>Role:</b> Given an array of surfaces and the an array of faces that the surfaces will
 * be tweaked into, the surfaces are automatically extended so that the tweak has a greater 
 * chance of being successful.
 * <br><br>
 * The curves underlying edges between two faces that are not directly involved in the tweak 
 * but adjacent to a face being tweaked may also be extended.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Faces must be valid, non-duplicate and from the same body, or error 
 * <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surfaces must be valid or error<tt> LOP_TWK_BAD_SURFACE(FACE*)</tt>.
 * <br><br>
 * Box, if supplied must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_extend_faces( int const nface,
                                         FACE *face[],
                                         SURFACE *tool_surface[],
                                         SPAposition box_low,
                                         SPAposition box_high,
                                         AcisOptions *ao = NULL);
/**
 * Extends an array of surfaces for use in <tt>api_tweak_faces</tt>.
 * <br><br>
 * <b>Role:</b> Given an array of surfaces and the an array of faces that the surfaces will
 * be tweaked into, the surfaces are automatically extended so that the tweak has a greater 
 * chance of being successful. Supplying a list of edges and their corresponding tweak curves, 
 * will help the algorithm come up with a better extension. The curves and edges which are 
 * supplied to the API have to be the image of the original edges after the tweak.
 * <br><br>
 * The curves underlying edges between two faces that are not directly involved in the tweak 
 * but adjacent to a face being tweaked may also be extended.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * At least one face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Faces must be valid, non-duplicate and from the same body, or error 
 * <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surfaces must be valid or error<tt> LOP_TWK_BAD_SURFACE(FACE*)</tt>.
 * <br><br>
 * Box, if supplied must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param nedge
 * number of edges for which the new geometry is know.
 * @param edge
 * edges for which the new geometry is know.
 * @param tool_curve
 * new curves.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/

DECL_LOP outcome api_tweak_extend_faces(const int &nface,
										FACE *face[], 
										SURFACE *tool_surface[], 
										const int &nedge,
										EDGE *edge[], 
										CURVE *tool_curve[],
										const SPAposition& box_low,
										const SPAposition& box_high,
										lop_options* pLopts,
										AcisOptions * ao= NULL);


/**
 * @nodoc
 * Adds attributes to edges completing the initial stage of tweak.
 * <br><br>
 * <b>Role:</b> Completes the initial stage of a tweak where attributes are added to edges.
 * Returns a list of edges that have more than one possible solution so the user may pick
 * which solution the tweak uses in the next stage.
 * <br><br>
 * To complete the tweak, use <tt>api_tweak_faces</tt>, which will continue the tweak 
 * without duplicating the work already done.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections.
 * Limiting the intersection improves performance.
 * The box must contain the final faces.
 * If omitted, it defaults to the size box.
 * <br><br>
 * Refer to the function @href api_tweak_faces for a complete description.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> Refer to the Errors listed for the function @href api_tweak_faces
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param reverse
 * flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param multiple_sols
 * edges with multiple solutions.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_faces_init(
									  int const    nface,
									  FACE*        face[],
									  SURFACE*     tool_surface[],
									  int          reverse[],
									  ENTITY_LIST& multiple_sols,
									  SPAposition  box_low,
									  SPAposition  box_high,
									  AcisOptions* ao = NULL
									  );
/**
 * Adds attributes to edges completing the initial stage of tweak.
 * <br><br>
 * <b>Role:</b> Completes the initial stage of a tweak where attributes are added to edges.
 * Returns a list of edges that have more than one possible solution so the user may pick
 * which solution the tweak uses in the next stage.
 * <br><br>
 * To complete the tweak, use <tt>api_tweak_faces</tt>, which will continue the tweak 
 * without duplicating the work already done.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections.
 * Limiting the intersection improves performance.
 * The box must contain the final faces.
 * If omitted, it defaults to the size box.
 * <br><br>
 * Refer to the function @href api_tweak_faces for a complete description.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> Refer to the Errors listed for the function @href api_tweak_faces
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces.
 * @param face
 * faces being tweaked.
 * @param tool_surface
 * tool surfaces in the model space (transform applied).
 * @param reverse
 * flags to reverse new surfaces; zero = leave as-is, nonzero = reverse.
 * @param multiple_sols
 * edges with multiple solutions.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_faces_init(
									  int const    nface,
									  FACE*        face[],
									  SURFACE*     tool_surface[],
									  int          reverse[],
									  ENTITY_LIST& multiple_sols,
									  const SPAposition&  box_low,
									  const SPAposition&  box_high,
									  lop_options* pLopts,
									  AcisOptions* ao = NULL
									  );
/**
 * Specifies an edge solution that a subsequent tweak will use.
 * <br><br>
 * <b>Role:</b> After a call to <tt>api_tweak_faces_init</tt>, attributes containing 
 * <tt>EDGE</tt> solution curves are attached to <tt>EDGE</tt>s involved in the tweak.
 * This function allows the user to specify which of multiple solutions a subsequent tweak will use.
 * It is possible to choose a solution that forces the tweak to fail or make a bad model.
 * It is also possible to specify incompatible solutions.
 * Therefore, the user must take care when using this function, particularly when specifying 
 * solutions on more than one <tt>EDGE</tt>.
 * <br><br>
 * If a curve is supplied, this curve will be used as the new edge geometry (if the curve is 
 * one of the possible solutions in the edge attribute) and the variable <tt>solution_no</tt> 
 * is ignored.
 * If no curve is supplied, <tt>solution_no</tt> represents the index of the curve to be used.
 * <br><br>
 * To query the possible solutions on an edge, use <tt>api_tweak_query_edge_solutions</tt>.
 * <br><br>
 * To complete the tweak after any number of edge solutions have been specified, use 
 * <tt>api_tweak_faces</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> If the edge does not have a <tt>LOP_EDGE_ATTRIB</tt> attribute, that is,
 * the edge was not involved in the initial stages of a tweak, or the solution number is not 
 * valid, <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edge
 * edge being specified.
 * @param soln_no
 * the solution curve to use when curve is non-NULL.
 * @param curve
 * the new edge geometry.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_pick_edge_solution( 
												EDGE *edge,
												int soln_no,
												CURVE *curve = NULL ,
												AcisOptions *ao = NULL
												);
/**
 * Returns an <tt>ENTITY_LIST</tt> of <tt>CURVEs</tt> that are possible solutions for an 
 * <tt>EDGE's</tt> geometry in a tweak.
 * <br><br>
 * <b>Role:</b> After a call to <tt>api_tweak_faces_init</tt>, attributes containing edge 
 * solution curves are attached to edges involved in the tweak.
 * This function allows the user to obtain a list of the possible solutions for an <tt>EDGE</tt>.
 * <br><br>
 * The <tt>CURVEs</tt> returned in <tt>curve_list</tt> are created on the heap and it is the 
 * caller's responsibility to lose them.
 * <br><br>
 * To select a possible solution on the edge, use <tt>api_tweak_pick_edge_solution</tt>.
 * <br><br>
 * To complete the tweak after any number of edge solutions have been specified, use 
 * <tt>api_tweak_faces</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> If there is no LOP attribute on the <tt>EDGE</tt>, <tt>LOP_TWK_NO_EDGE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edge
 * edge being queried.
 * @param curve_list
 * list of possible solutions.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_tweak_query_edge_solutions( 
												EDGE *edge,
												ENTITY_LIST &curve_list,
												AcisOptions *ao = NULL);
/** @} */
/**
 * \addtogroup LOPAPI
 *
 * @{
 */
/**
 * Removes all attributes that a local operation attached to <tt>EDGE</tt>s and 
 * <tt>VERTEX</tt>s contained in an entity.
 * <br><br>
 * <b>Role:</b> Removes all <tt>ATTRIB_LOP_EDGE</tt> and <tt>ATTRIB_LOP_VERTEX</tt> 
 * attributes attached to <tt>EDGE</tt>s and <tt>VERTEX</tt>s on an <tt>ENTITY</tt>.
 * These attributes are attached when using the APIs <tt>api_tweak_faces_init</tt>,
 * <tt>api_tweak_fix_edge</tt>, and <tt>api_tweak_fix_vertex</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity to be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_LOP outcome api_remove_lop_attribs( ENTITY *entity,
										AcisOptions *ao = NULL);
/** @} */
/**
 * \addtogroup LOPSHEETEXT
 *
 * @{
 */
/**
 * Extends the input list of edges from a sheet body by offsetting those along the surfaces by
 * the given distance.
 * <br><br>
 * <b>Role:</b> The boundary edges of the sheet body provided in the <tt>edgeList</tt> 
 * argument are extended by the specified offset distance. The distance can be positive or negative.
 * <br>
 * Each resulting offset edge is defined by the offset distance, with the distance 
 * measured perpendicularly to the original edge, not as the arclength along the surface on which it lies. 
 * The surfaces extend naturally, not linearly. Vent edges may be added to tangential two-edge vertices. 
 * The input edges should lie on the boundary of the sheet body. Any interior edges are ignored.
 * <br>
 * Passing in (0,0,0) for <i>boxLow</i> and <i>boxHigh</i> indicates that you are 
 * not specifying a box.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br> 
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edgeList
 * List of edges to extend.
 * @param fOffsetDistance
 * Distance in <tt>xyz</tt> space of the extension, positive or negative.
 * @param boxLow
 * Start of the intersection box to be used.
 * @param boxHigh
 * End of the intersection box to be used.
 * @param pLopts
 * Local Operations options for behavior control.
 * @param ao
 * ACIS options.
 **/
 
DECL_LOP outcome api_extend_sheetbody(ENTITY_LIST        edgeList, 
                                      const double&      fOffsetDistance, 
                                      const SPAposition& boxLow,
                                      const SPAposition& boxHigh,
                                      lop_options*       pLopts,
                                      AcisOptions*       ao = NULL);


/** @} */
/**
 * \addtogroup LOPTWEAK
 *
 * @{
 */
 /**
 * Replaces the surface of a face with given tool surface.
 * <br><br>
 * <b>Role:</b> Given a face and a tool face (or surface), replaces the face geometry with
 * the tool face (or surface) geometry.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * One face must be supplied, or error <tt>LOP_TWK_NO_FACE</tt>.
 * <br><br>
 * Supplied face must be valid or error <tt>LOP_TWK_BAD_FACE (FACE*)</tt>.
 * <br><br>
 * Tool surface must be valid or error<tt> LOP_TWK_BAD_SURFACE(FACE*)</tt>.
 * <br><br>
 * Tool surface and original face geometry must be similar or error <tt>LOP_REPLACE_FACE_TOO_DIFFERENT</tt> 
 * <br><br>
 * Box, if supplied must be valid, or error <tt>LOP_BAD_BOX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param the_face
 * face being replaced.
 * @param the_surface
 * new surface.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param lopts
 * Local Operations options for behavior control.
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_tweak_replace_face(FACE* the_face, 
                                        ENTITY *the_surface,
                                        const SPAposition& box_low,
                                        const SPAposition& box_high,
                                        lop_options* lopts,
                                        AcisOptions* ao = NULL);

 /**
 * Replaces the geometry of the input tolerant sharp edge with precise non-tolerant geometry
 * <br><br>
 * <b>Role:</b> Given a tolerant sharp edge, it is replaced with a non-tolerant edge by intersecting
 * the adjacent surfaces. 
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * NO Topology Changes
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, 
 * @href api_tweak_faces
 * <br><br>
 * One edge must be supplied, or error <tt>LOP_TWK_NO_EDGE_SUPPLIED</tt>.
 * <br><br>
 * Supplied edge must be valid or error <tt>LOP_TWK_BAD_EDGE_SUPPLIED (EDGE*)</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param the_edge
 * edge being replaced with precise geometry.
 * @param lopts
 * Local Operations options for behavior control.
 * @param ao
 * ACIS options.
 **/
 DECL_LOP outcome api_tweak_tighten_edge(EDGE*   the_edge, 
							             lop_options* lopts,
							             AcisOptions* ao = NULL);

 
 /** @} */


 /**
 * \addtogroup DETECTFEATURE
 * \brief Declared at <lop_api.hxx>
 * @{
 */
 /**
 * This API detects protrusion features on the model and returns a collection of contiguous 
 * faces defining the protrusion that includes the supplied seed entity. Protrusions include
 * bosses and islands. 
 * <br><br>
 * <b>Limitations:</b> 
 * Mixed convexity edges are not handled well.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br> 
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param seed_ent
 * seed face on the model
 * @param protrusion_faces
 * face list of detected protrusion
 * @param df_opts
 * detect feature options
 **/
 DECL_LOP outcome api_detect_protrusion(const ENTITY* seed_ent,
									    ENTITY_LIST& protrusion_faces,
										const detect_feature_options* df_opts = NULL);

 /**
 * This API detects depression features on the model and returns a collection of 
 * contiguous faces defining the depression that includes the supplied seed entity. 
 * Depressions include features like cut, pocket, slot and hole. 
 * <br><br>
 * <b>Limitations:</b> 
 * Mixed convexity edges are not handled well.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br> 
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param seed_ent
 * seed face on the model
 * @param depression_faces
 * face list of detected depression
 * @param df_opts
 * detect feature options
 **/
 DECL_LOP outcome api_detect_depression(const ENTITY * seed_ent,
									    ENTITY_LIST& depression_faces,
										const detect_feature_options* df_opts = NULL);

/** @} */

#endif

