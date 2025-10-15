/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//               Header for repair body intersection routines.
/*******************************************************************/
#if !defined( RBI_HUSK_API_ROUTINES )
#define RBI_HUSK_API_ROUTINES
/**
 * @file rbi_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup RBIAPI Repair Body Intersections
 *
 * \ingroup LOCALOPS
 * @{
 */
#include "dcl_rbi.h"
#include "api.hxx"
class outcome;
class BODY;
class FACE;
class SPAposition;
/**
 * Initializes the repair body self intersections library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_RBI outcome api_initialize_rbi();
/**
 * Terminates the repair body self intersection library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_RBI outcome api_terminate_rbi();
/**
 * Repairs self intersections in the supplied body.
 * <br><br>
 * <b>Role:</b> Self-intersecting bodies have mutually intersecting faces.
 * These are faces that intersect in places other than any of the common boundaries between them.
 * <br><br>
 * This API is intended to repair body self-intersections made by Local Ops or Shelling.
 * This API does not address topology changes that prevent these operations from completing,
 * but does repair self-intersections that result from the completed operations.
 * <br><br>
 * Body self-intersections made by other operations (such as sweeping or blending) may also
 * be repaired if the body is in a suitable state.
 * <br><br> 
 * The following assumptions are made:
 * <br><br>
 * <ul>
 * <li>The set of faces known to have been changed by the operation are supplied with the body.</li>
 * <li>The remaining faces in the body fall into a set of unchanged faces, and the faces in this
 * set must not intersect one another unless along common boundary edges or vertices.</li>
 * <li>The set of changed faces may be empty.
 * When the set of changed faces is empty it defaults to all the faces in the body, and the
 * unchanged faces to an empty set.
 * <li>The body consists of a number of solid shells, with a distinct inside and outside, crossing 
 * from one side to the other only being possible by passing through a face.  
 * <li>The shells must not be non-manifold.
 * <li>The body shells may intersect.
 * <li>The body shells may self-intersect.
 * <li>Faces may have intersecting loops.
 * <li>Faces may have self-intersecting loops.
 * <li>Faces must not have self-intersecting surfaces.
 * <li>Edges must not have self-intersecting curves.
 *</li>
 * </ul>
 * <br>
 * <dl>
 * <b>Errors:</b> <br> Api returns following errors:
 * <br><br>
 * <dd> Null body supplied gives <tt>RBI_NO_BODY</tt>.</dd>
 * <dd>&nbsp;Duplicate faces gives <tt>RBI_BAD_FACE</tt>.</dd>
 * <dd>&nbsp;Non manifold body gives <tt>RBI_NON_MANIFOLD</tt>.</dd>
 * <dd>&nbsp;Free ( sheet ) edge gives <tt>RBI_FREE_EDGE</tt>.</dd>
 * <dd>&nbsp;Algorithmic failure gives <tt>RBI_INTERNAL</tt> or <tt>RBI_BAD_OP_ON_ATT</tt>.</dd>
 * <dd>&nbsp;Current limitation gives <tt>RBI_NOT_IMPLEMENTED</tt>.</dd>
 * <dd>&nbsp;Unrepairable face loop gives <tt>RBI_IRREP_FACE_LOOP</tt>.</dd>
 * <dd>&nbsp;All faces in body being deleted gives <tt>RBI_NO_LUMP_LEFT</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> 
 * <ul>
 * <li>The directions of open edges on periodic curves must be correct.
 * Local Operations and Shelling will do this if option <tt>lop_repair_self_int</tt> is on or option
 * <tt>lop_check_invert</tt> is off.
 * <br><br>
 * <li>The input body is assumed to consist entirely of valid connected faces with no free edges.
 * <br><br>
 * <li>It is assumed that the body was formerly valid and has become self-intersecting as a result of
 * one or more faces passing through other faces during a local operation, or becoming partially
 * or completely coincident with the other faces.
 * Alternatively, the body is assumed to be composed of valid faces connected such that
 * traversing it from any start face will always reach the same side of the start face.
 * <br><br>
 * <li>The faces involved may be in different shells or lumps.
 * <br><br>
 * <li>The faces themselves must be valid, except that they may be self-intersecting, or have
 * inconsistent loops, or both.
 * <br><br>
 * <li>Self-intersecting faces have some edges that intersect in places other than any common
 * boundary vertices between them.
 * However, the geometry of the faces is assumed to not intersect itself.
 * An example of a self-intersecting face is the side face to a vertical face of a block
 * tapered such that the top face should disappear.
 * <br><br>
 * <li>Faces with inconsistent loops have faces which are unbounded by their loops or have loops
 * that do not contain one another.
 * An example of such a face is the top face of a block when a side face is tapered such that
 * the top should disappear.
 * <br><br>
 * <li>As the repair method is based on Boolean operations, faces are split and do not grow, except
 * when they merge with other faces.
 * <br><br>
 * <li>To have <tt>api_repair_body_self_ints</tt> remove any faces that are given reversed senses during the
 * operation, set option <tt>rbi_remove_inv_face</tt> to <tt>TRUE</tt>, and  <tt>face removal</tt> algorithm will be
 * called at the end of the operation on those inverted faces, and will remove them by growing adjacent
 * faces.
 * </li>
 * </ul>
 * <br><br>
 * <b>NOTE:</b>
 * <br><br>
 * In the beginning, when RBI was created, it was not called by default when performing a Local Operation. 
 * However, it was possible to enable RBI within a Local Operation by setting the option @href lop_repair_self_int to TRUE.
 * As of R15, RBI is enabled within Local Operations by default. Strictly speaking, the control of RBI is now governed by 
 * the creation of a default @href lop_options object, whose repair_self_intersections flag is set to TRUE. 
 * To perform a Local Op without RBI, supply a @href lop_options object with repair_self_intersections flag set to FALSE.
 * <br><br>
 * It is recommendable to evoke RBI through Local Operations or Shelling to repair self-intersection, rather than producing a 
 * self-intersecting body by Local Operations and then call RBI.
 * <br><br>
 * Now RBI can repair exclusive sheet bodies. If the option, 'lop_allow_free_edges' is turned ON and input body is a sheet body,
 * api will not throw RBI_FREE_EDGE exception, instead of that algorithm with try to repair the body. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param body
 * body to be repaired.
 * @param nface
 * number of faces changed since the body did not self-intersect.
 * @param faces
 * faces changed since the body did not self-intersect.
 * @param ao
 * ACIS options.
 **/
DECL_RBI outcome api_repair_body_self_ints(
								   BODY *body,
								   int const nface,
								   FACE *faces[],
								   AcisOptions *ao = NULL);
/**
 * Removes faces and repairs the body.
 * <br><br>
 * <b>Role:</b> This API attempts to remove the specified faces from the body, and reclose a solid
 * by growing the remaining adjacent faces, and intersecting them amongst themselves and with remote
 * faces.
 * <br><br>
 * This operation cannot be be guaranteed to succeed.
 * For example removing any face from a cube will fail as the remaining surfaces do not enclose a volume.
 * <br><br>
 * If an empty box is supplied, the function will compute a suitable box.The adjacent face surfaces
 * are generally extended so as to have no boundaries in the box. This avoids solutions with detached
 * faces on the same surface.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Face(s) supplied invalid, duplicate or from different bodies gives <tt>RBI_BAD_FACE</tt>.
 * <dd>&nbsp;Non manifold edge in body supplied gives <tt>RBI_NON_MANIFOLD</tt>.</dd>
 * <dd>&nbsp;Free or Sheet edge in body supplied gives <tt>RBI_FREE_EDGE</tt>.</dd>
 * <dd>&nbsp;Internal error gives <tt>RBI_INTERNAL</tt>.</dd>
 * <dd>&nbsp;No lumps left in body gives <tt>RBI_NO_LUMP_LEFT</tt>.</dd>
 * <dd>&nbsp;No faces supplied gives <tt>RBI_NO_FACE</tt>.</dd>
 * <dd>&nbsp;Body cannot be reclosed gives <tt>RBI_NO_SOLUTION</tt>.</dd>
 * <dd>&nbsp;Invalid box supplied gives <tt>RBI_BAD_BOX</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> Supplied body must be manifold and solid.
 * Remote faces are not extended.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nface
 * number of faces to be removed.
 * @param faces
 * faces to be removed.
 * @param box_low
 * start of intersection box.
 * @param box_high
 * end of intersection box.
 * @param ao
 * ACIS options.
 **/
DECL_RBI outcome api_remove_and_repair_body(
									int const nface,
									FACE *faces[],
									SPAposition box_low,
									SPAposition box_high,
									AcisOptions *ao = NULL);
/** @} */
#endif

