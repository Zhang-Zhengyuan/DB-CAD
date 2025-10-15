/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// 
/*******************************************************************/
#if !defined( COVERAPI_HXX )
#define COVERAPI_HXX
#include "base.hxx"
class BODY;
class WIRE;
class FACE;
class ENTITY_LIST;
class EDGE;
class SPAposition;
class SPAvector;
class surface;
class cover_options;
#include "dcl_covr.h"
#include "api.hxx"
/**
 * @file coverapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup COVRAPI Covering APIs
 *
 * \ingroup ADVSURF Advanced Surfacing
 * @{
 */

/**
 * Initializes the Covering Component library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_COVR outcome api_initialize_covering();

/**
 * Terminates the Covering Component library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_COVR outcome api_terminate_covering();

/**
 * Covers one or more circuits of edges with a face or faces.
 * <br><br>
 * <b>Role:</b> This API covers one or more circuits of edges with a face or faces.
 * The faces(s) are returned via the <tt>cover_options</tt> object.
 * <br><br>
 * Each input <tt>ENTITY_LIST</tt> must contain edges forming a closed circuit - an error is
 * issued if the edges do not form a closed circuit.
 * <br><br>
 * The created face(s) are placed in the first shell of the
 * body that owns the first edge in the list (or in a new shell of that body if it
 * has no shell).
 * <br><br>
 * <tt>api_cover_circuits</tt> can be given a <tt>surface</tt>; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If the covering surface is not given,
 * i.e., a surface is not specified in the <tt>cover_options</tt> object, the API tries to fit a plane.
 * If no single plane is found, the API tries to fit a spline to the user specified circuits.
 * If no spline can be found, the API tries to fit distinct planes to
 * each group of circuits that are coplanar, creating a separate face
 * for each plane. If a surface cannot be found for a face, that face is returned
 * with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * The <tt>cover_options</tt> object can be used to specify a gap tolerance.  If this tolerance is specified,
 * the API will cover external circuits that are planar within tolerance, and tolerize the boundary geometry of the
 * resulting face.
 * Similarly, if the user specifies a covering surface and a gap tolerance, any exterior circuit within tolerance
 * of the specified surface will be covered, and the resulting face will be tolerized.  The <tt>cover_options</tt> object
 * can be queried after the covering operation to find the new faces.
 * <br><br>
 * <dl>
 * <b>Errors:</b> An entity in a list is not an <tt>EDGE</tt>.
 * <dd>The edges in a list are not topologically connected.</dd>
 * <dd>The edges in a list do not form a closed circuit.</dd>
 * </dl>
 * <b>Limitations:</b> This API does not handle bodies which contain both wires and faces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_circuits
 * number of circuits to be covered.
 * @param array_circuits
 * array of <tt>ENTITY_LIST</tt> pointers, each list contains a circuit of edges.
 * @param cov_opts
 * <tt>cover_options</tt> object, can be used to specify covering surface and gap tolerance
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_circuits(
			int num_circuits,				// number of circuits
			ENTITY_LIST * array_circuits[],	// array of pointers to lists of edges
											// denoting circuits to be covered
			cover_options* cov_opts = NULL,
			AcisOptions* ao = NULL
	);

/**
 * Creates faces by covering all simple loops of external edges of a sheet body.
 * <br><br>
 * <b>Role:</b> Returns a list of faces via the <tt>cover_options</tt> object. This API attempts
 * to cover all external circuits of a sheet body (closed, connected groups of
 * edges that have only one coedge) with a face or faces. Given two circuits, the
 * result can be either a face with two loops or two distinct faces. Two faces can
 * either share one surface or they can have distinct surfaces.
 * <br><br>
 * If no faces are covered, the API returns an empty list, and a bad outcome.
 * <tt>api_cover_sheet</tt> can be given a <tt>surface</tt> using the <tt>cover_options</tt> 
 * object; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If the covering surface is not given,
 * the API tries to fit a plane to all external circuits.
 * If no single plane is found, the API tries to fit a spline to the external
 * circuits. If no spline can be found, the API tries to fit distinct planes to
 * each group of circuits that are coplanar, creating a separate face
 * for each plane. If a surface cannot be found for a face, that face is returned
 * with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * If a planar surface is given or found, the API arranges the loops into distinct
 * connected faces for each surface. The new faces are placed in the first shell of
 * the given body and pointers to the new faces are returned in a list.
 * <br><br>
 * This API does not cover wire edges.
 * <br><br>
 * The <tt>cover_options</tt> object can be used to specify a gap tolerance.  If this tolerance is specified,
 * the API will cover external circuits that are planar within tolerance, and tolerize the boundary geometry of the
 * resulting face.
 * Similarly, if the user specifies a covering surface and a gap tolerance, any exterior circuit within tolerance
 * of the specified surface will be covered, and the resulting face will be tolerized.  The <tt>cover_options</tt> object
 * can be queried after the covering operation to find the new faces.
 * <br><br>
 * <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sheet
 * sheet body to be covered
 * @param cov_opts
 * <tt>cover_options</tt> object, can be used to specify covering surface and gap tolerance
 * @param ao
 * acis options.
 **/

DECL_COVR outcome api_cover_sheet(
			BODY* sheet,
			cover_options* cov_opts=NULL,
			AcisOptions* ao = NULL
	);

/**
 * Covers a circuit of wire edges and creates a single face.
 * <br><br>
 * <b>Role:</b> The given wire must be a simple circuit of wire edges.
 * <br><br>
 * If the covering surface is not specified using the <tt>cover_options</tt> object,
 * the API tries to fit a plane. If no plane is found, <tt>api_cover_wire</tt> tries to fit a spline. 
 * If a surface cannot be found for a face, that face is returned with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * The new face is placed in a new shell of the wire's body and the wire is deleted.
 * <br><br>
 * The <tt>cover_options</tt> object can be used to specify a gap tolerance.  If this tolerance is specified,
 * the API will cover external circuits that are planar within tolerance, and tolerize the boundary geometry of the
 * resulting face. If the cover is not planar the API will cover external circuit with a spline surface within tolerance 
 * and raise a warning DISCONN_EDS stating "edge circuit is not connected".
 * Similarly, if the user specifies a covering surface and a gap tolerance, any exterior circuit within tolerance
 * of the specified surface will be covered, and the resulting face will be tolerized.  The <tt>cover_options</tt> object
 * can be queried after the covering operation to find the face created by the operation.
 * <br><br>
 * <dl>
 * <b>Errors:</b> The input body is not a wire body.
 * <dd> The input wire body does not contain a circuit</dd>
 * </dl>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire to be covered.
 * @param cov_opts
 * <tt>cover_options</tt> object, can be used to specify covering surface and gap tolerance.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_wire(
			WIRE* wire,
			cover_options* cov_opts = NULL,
			AcisOptions* ao = NULL
	);

/**
 * Covers circuits of wire edges and creates faces.
 * <br><br>
 * <b>Role:</b> The given body must be composed of one or more wires with each
 * wire being a simple loop of wire edges. 
 * <br><br>
 * If the covering surface is not specified using the <tt>cover_options</tt> object,
 * the API tries to fit a plane. If no plane is found, <tt>api_cover_wire</tt> tries to fit a spline surface.  
 * <br><br>
 * If a planar surface is given or found, the API arranges the loops into connected
 * faces. The treatment of nested loops is sensitive to the orientation of the loops; a pair
 * of oppositely oriented loops, for example, is converted to a single face with a periphery
 * and a hole, while a pair of overlapping faces is the result if the loops are similarly
 * oriented.  In all cases, the new faces are placed in a new shell of the given body and all wires
 * of the body are deleted. 
 * <br><br>
 * A <tt>cover_options</tt> object can be used to specify a gap tolerance.  If this tolerance is specified,
 * the API will cover external circuits that are planar within tolerance, and tolerize the boundary geometry of the
 * resulting face.
 * Similarly, if the user specifies a covering surface and a gap tolerance, any exterior circuit within tolerance
 * of the specified surface will be covered, and the resulting face will be tolerized.  The <tt>cover_options</tt> object
 * can be queried after the covering operation to find the new faces.
 * <br><br>
 * <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a wire body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_body
 * wire body to be covered.
 * @param cov_opts
 * <tt>cover_options</tt> object, can be used to specify covering surface and gap tolerance.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_wires(
			BODY * wire_body,				// wire body to be covered
			cover_options* cov_opts,
			AcisOptions* ao = NULL
	);

/**
 * Covers one or more circuits of edges with a face or faces.
 * <br><br>
 * <b>Role:</b> This API covers one or more circuits of edges with a face or faces.
 * The face(s) are returned via the <tt>faces_list</tt>.
 * <br><br>
 * Each input <tt>ENTITY_LIST</tt> must contain edges forming a closed circuit: an error is
 * issued if the edges do not form a closed circuit.
 * <br><br>
 * The created face(s) are placed in the first shell of the
 * body that owns the first edge in the list (or in a new shell of that body if the body
 * has no shell).
 * <br><br>
 * <tt>api_cover_circuits</tt> can be given a <tt>surface</tt>; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If a <tt>surface</tt> is not given
 * (i.e., <tt>&surf</tt> is <tt>NULL</tt>) the API tries to fit a plane.
 * If no single plane is found, the API tries to fit a spline to the external
 * circuits. If no spline can be found, the API tries to fit distinct planes to
 * each group of circuits that are coplanar, creating a separate face
 * for each plane. If a surface cannot be found for a face, that face is returned
 * with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * <dl>
 * <b>Errors:</b> An entity in a list is not an <tt>EDGE</tt>.
 * <dd>&nbsp;The edges in a list are not connected.</dd>
 * <dd>&nbsp;The edges in a list do not form a closed circuit.</dd>
 * </dl>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_circuits
 * number of circuits to be covered.
 * @param array_circuits
 * array of <tt>ENTITY_LIST</tt> pointers, each list contains a circuit of edges.
 * @param surf
 * surface for covering (can be NULL).
 * @param faces_list
 * returned new list of faces.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_circuits(
			int num_circuits,				// number of circuits
			ENTITY_LIST *array_circuits[],	// array of pointers to lists of edges
											// denoting circuits to be covered
			const surface& surf,
			ENTITY_LIST &faces_list,
			AcisOptions* ao = NULL
	);

/**
 * Creates faces by covering all simple loops of external edges of a sheet body.
 * <br><br>
 * <b>Role:</b> Returns a list of faces via the <tt>faces_list</tt>. This API attempts
 * to cover all external circuits of a sheet body (closed, connected groups of
 * edges that have only one coedge) with a face or faces. Given two circuits, the
 * result can be either a face with two loops or two distinct faces. Two faces can
 * either share one surface or they can have distinct surfaces.
 * <br><br>
 * If no faces are covered, the API returns an empty list.
 * <tt>api_cover_sheet</tt> can be given a <tt>surface</tt>; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If the covering surface is not given
 * (i.e., <tt>&surf</tt> is <tt>NULL</tt>) the API tries to fit a plane to all external circuits.
 * If no single plane is found, the API tries to fit a spline to the external
 * circuits. If no spline can be found, the API tries to fit distinct planes to
 * each group of circuits that are coplanar, creating a separate face
 * for each plane. If a surface cannot be found for a face, that face is returned
 * with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * If a planar surface is given or found, the API arranges the loops into distinct
 * connected faces for each surface. The new faces are placed in the first shell of
 * the given body and pointers to the new faces are returned in a list.
 * <br><br>
 * This API does not cover wire edges.
 * <br><br>
 * When ACIS makes a <tt>BODY</tt> from a <tt>FACE</tt> using <tt>api_cover_sheet</tt>, it makes a
 * single-sided face body. A single-sided face body is really a solid, not a sheet
 * (infinitely thin) body. When a face body is single-sided, ACIS considers the body 
 * to be a solid which extends from the back
 * side of the face out to infinity with ill-defined boundaries extending where the
 * edges of the original face extend backward. Subsequent operations such as
 * Booleans may not work, depending on how the single-sided face body is being used.
 * <br><br>
 * When the flag <tt>multiple_cover</tt> is set to <tt>TRUE</tt>, <tt>api_cover_sheet</tt> first identifies all the
 * separate loops of external edges. Next, it checks if each of the loops can be
 * covered with a separate planar surface. If so, the result is
 * multiple faces. Otherwise, it tries to cover the loop as it does without the option.
 * <br><br>
 * The <tt>cover_options</tt> object can be used to specify a gap tolerance.  If this tolerance is specified,
 * the API will cover external circuits that are planar within tolerance, and tolerize the boundary geometry of the
 * resulting face.
 * Similarly, if the user specifies a covering surface and a gap tolerance, any exterior circuit within tolerance
 * of the specified surface will be covered, and the resulting face will be tolerized.  The <tt>cover_options</tt> object
 * can be queried after the covering operation to find the new faces.
 * <br><br>
 * <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sheet
 * sheet body to be covered.
 * @param surf
 * surface for covering (can be <tt>NULL</tt>).
 * @param faces_list
 * returned new list of faces.
 * @param multiple_cover
 * option to cover with multiple planes.
 * @param ao
 * acis options.
 * @param cov_opts
 * <tt>cover_options</tt> object, can be used to specify covering surface and gap tolerance.
 **/

DECL_COVR outcome api_cover_sheet(
			BODY * sheet,					// sheet body to be covered
			surface const & surf,			// covering surface (can be NULL)
			ENTITY_LIST & faces_list,		// list of faces made
			logical multiple_cover = FALSE,	// option to cover a loop with multiple planes.
			AcisOptions* ao = NULL,
            cover_options const * co = NULL
	);

/**
 * Covers a circuit of wire edges and creates a single face.
 * <br><br>
 * <b>Role:</b> The given wire must be a simple circuit of wire edges.
 * <br><br>
 * <tt>api_cover_wire</tt> can be given a <tt>surface</tt>; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If the covering surface is not given
 * (i.e., <tt>&surf</tt> is <tt>NULL</tt>) the API tries to fit a plane. If no plane is found,
 * <tt>api_cover_wire</tt> tries to fit a spline. If a surface cannot be found, the face
 * is returned with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * The new face is placed in a new shell of the wire's body and the wire is deleted.
 * <br><br>
 * <tt>api_cover_wire</tt> returns the new face in the <tt>face</tt> argument.
 * <br><br>
 * The edges may have been either constructed directly or made by slicing; i.e., by
 * the first phase of a Boolean operation. To obtain suitable circuits from slicing,
 * the slicing must be performed by a single face that is not coincident with any
 * face of the body being sliced. Also, edges of the face cannot coincide with
 * edges of the body being sliced. In additiona, any attributes previously added to 
 * the wire edges by the Boolean operation must be removed from the edges.
 * <br><br>
 * When ACIS makes a <tt>BODY</tt> from a <tt>FACE</tt> using <tt>api_cover_wire</tt>, it makes a
 * single-sided face body. A single-sided face body is really a solid, not a sheet
 * (infinitely thin) body. When a face body is single-sided, ACIS considers the body 
 * to be a solid which extends from the back
 * side of the face out to infinity with ill-defined boundaries extending where the
 * edges of the original face extend backward. Subsequent operations such as
 * Booleans may not work, depending on how the single-sided face body is being used.
 * <br><br>
 * <b>Errors:</b> The pointer to a wire is <tt>NULL</tt> or does not point to a wire.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire to be covered.
 * @param surf
 * surface for covering (can be <tt>NULL</tt>).
 * @param face
 * returned new face.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_wire(
			WIRE * wire,				// wire to be covered
			surface const & surf,		// covering surface (can be NULL)
			FACE *& face,				// face made
			AcisOptions* ao = NULL
	);

/**
 * Covers circuits of wire edges and creates faces.
 * <br><br>
 * <b>Role:</b> The given body must be composed of one or more wires with each
 * wire being a simple loop of wire edges. Covering is supported for three- or
 * four-sided non-planar loops, and for <i>n</i>-sided loops if all the edges lie on the
 * same plane. Loops with five or more non-planar edges are covered under the
 * following conditions:
 * <ul>
 * <li>Degenerate edges are not allowed.</li>
 * <li>Edges which meet at an angle &gt;= 180 or &lt;= 0 degrees are not allowed.</li>
 * <li>Edges which turn 'excessively' are not allowed.</li>
 * </ul>
 * <tt>api_cover_wires</tt> can be given a surface; in that case, a corresponding <tt>SURFACE</tt>
 * is set as the resulting face's geometry. If the covering surface is not given
 * (i.e., <tt>&surf</tt> is <tt>NULL</tt>) the API tries to fit a plane. If no single plane is found,
 * <tt>api_cover_wires</tt> tries to fit a spline. If no spline can be found, <tt>api_cover_wires</tt>
 * tries to fit distinct planes to each group of circuits that are coplanar, creating
 * a separate face for each plane. If a surface cannot be found for a face, that face
 * is returned with a <tt>NULL</tt> <tt>SURFACE</tt>.
 * <br><br>
 * If a planar surface is given or found, the API arranges the loops into connected
 * faces. The treatment of nested loops is sensitive to the orientation of the loops; a pair
 * of oppositely oriented loops, for example, is converted to a single face with a periphery
 * and a hole, while a pair of overlapping faces is the result if the loops are similarly
 * oriented.  In all cases, the new faces are placed in a new shell of the given body and all wires
 * of the body are deleted. The attributes previously added to the wire edges by
 * the Boolean operation are removed from the edges.
 * <br><br>
 * The edges may have been constructed either directly or by slicing; i.e., by the
 * first phase of a Boolean operation. To obtain suitable circuits from slicing,
 * the slicing must be performed by a single face that is not coincident with any
 * face of the body being sliced. Also, edges of the face cannot coincide with
 * edges of the body being sliced. Each circuit is formed into a loop.
 * <br><br>
 * This API returns the pointers to the faces made in a list.
 * <br><br>
 * <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a wire body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_body
 * wire body to be covered.
 * @param surf
 * surface for covering (can be <tt>NULL</tt>).
 * @param faces_list
 * returned new list of faces.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_wires(
			BODY * wire_body,				// wire body to be covered
			surface const & surf,			// covering surface (can be NULL)
			ENTITY_LIST & faces_list,		// list of faces made
			AcisOptions* ao = NULL
	);

/**
 * Covers planar free edges with planar faces.
 * <br><br>
 * <b>Role:</b> <tt>api_cover_planar_edges</tt> organizes planar edges by covering them with
 * planar faces, which can then be used for solid construction operations, such as sweeping and lofting.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param eds
 * free edges to be covered.
 * @param out_bdy
 * returned body containing covered faces.
 * @param wires
 * returned wire bodies that cannot be covered.
 * @param nest
 * function is expected to handle nested loops if <tt>TRUE</tt>.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_planar_edges(
	ENTITY_LIST  &eds,			// free edges to be covered
	BODY        *&out_bdy,		// return body containing covered faces
	ENTITY_LIST  &wires,		// return wire bodies that can't be covered
	int           nest = TRUE,	// does the api expected to handle nested loops
	AcisOptions  *ao = NULL);
								// better performance with nest = FALSE

/**
 * Covers planar wires with planar faces.
 * <br><br>
 * <b>Role:</b> <tt>api_cover_planar_wires</tt> orgainizes planar wires by covering them with
 * planar faces, which can then be used for solid construction operations, such as sweeping and lofting.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wbs
 * wire bodies to be covered.
 * @param out_bdy
 * returned body containing covered faces.
 * @param out_wires
 * returned wire bodies that cannot be covered.
 * @param nest
 * function is expected  to handle nested loops if <tt>TRUE</tt>.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_planar_wires(
	ENTITY_LIST  &in_wbs,		// wire bodies to be covered
	BODY        *&out_bdy,		// return body containing covered faces
	ENTITY_LIST  &wires,		// return wire bodies that can't be covered
	int           nest = TRUE,	// does the api expected to handle nested loops
	AcisOptions  *ao = NULL);
								// better performance with nest = FALSE

/**
 * Combines two edges into a single edge.
 * <br><br>
 * <b>Role:</b> This API combines the two given edges whose ends meet 
 * at a point of G<sup>1</sup> continuity.  The edges themselves should be at least 
 * G<sup>1</sup> continuous, or the resulting edge will not work well with other 
 * ACIS operations.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edge1
 * first edge.
 * @param edge2
 * second edge.
 * @param new_edge
 * resulting edge.
 * @param outfile
 * output file for errors.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_combine_edges(EDGE *edge1,		// first edge
						            EDGE *edge2,		// second edge
						            EDGE *&new_edge,	// resulting edge
									FILE *out=NULL,		// output file for errors
									AcisOptions* ao = NULL);

/**
 * Repairs co-planar edge vertices to form regions on a planar sheet.
 * <br><br>
 * <b>Role:</b> This API repairs co-planar edge vertices within the given coincident-point
 * tolerance to form regions on a planar sheet. If the length limit is specified,
 * edges shorter than that value will be removed.
 * <br><br>
 * This API will also smooth non-G<sup>1</sup> edge geometry, which may cause
 * large shape changes in geometry with large non-G<sup>1</sup> discontinuities. It is recommended that
 * users call @href api_split_edge_at_disc (with an order argument of 1) to split non-G<sup>1</sup> edges before
 * calling <tt>api_heal_edges_to_regions</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param eds
 * input edges.
 * @param coincident_tol
 * given coincident-point tolerance.
 * @param length_limit
 * edges under the length limit will be removed.
 * @param outbdy
 * output body (may be <tt>NULL</tt> if no region is enclosed.
 * @param wire_only
 * return wire only  if <tt>TRUE</tt>.
 * @param fptr
 * debug output file acis options.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_heal_edges_to_regions(
	ENTITY_LIST &eds,					// edges to be processed
	double       coincident_tol,		// vertices within the coincident tolerance will be joined
	double       length_limit,			// edges under the length limit will be removed
	BODY       *&outbdy,				// output body (may be NULL if no region is enclosed
	int          wire_only=FALSE,		// flag to return wire only
	FILE        *fptr=NULL,				// debug file output
	AcisOptions* ao = NULL
	);

/**
 * Repairs co-planar edge vertices to form regions on a planar sheet.
 * <br><br>
 * <b>Role:</b> This API repairs co-planar edge vertices within the given coincident-point
 * tolerance to form regions on a planar sheet. If the length limit is specified,
 * edges shorter than that value will be removed.
 * <br><br>
 * This API will also smooth non-G<sup>1</sup> edge geometry, which may cause
 * large shape changes in geometry with large non-G<sup>1</sup> discontinuities. It is recommended that
 * users call @href api_split_edge_at_disc (with an order argument of 1) to split non-G<sup>1</sup> edges before
 * calling <tt>api_heal_edges_to_regions</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param eds
 * input edges.
 * @param coincident_tol
 * given coincident-point tolerance.
 * @param length_limit
 * given length limit.
 * @param outbodies
 * output bodies.
 * @param fptr
 * debug output file.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_heal_edges_to_regions(
	ENTITY_LIST &eds,					// edges to be processed
	double       coincident_tol,		// vertices within the coincident tolerance will be joined
	double       length_limit,			// edges under the length limit will be removed
	ENTITY_LIST &outbodies,				// return bodies that contains face regions
	FILE        *fptr,					// debug file output
	AcisOptions* ao = NULL
	);

/**
 * Adds all edges into a wire body using a non-regularized unite.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param eds
 * given list of edges.
 * @param outbdy
 * output body.
 * @param fptr
 * debug file.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_unite_edges(
	ENTITY_LIST &eds,	  // input edges
	BODY       *&outbdy,  // output wire body
	FILE        *fptr,   // debug output
	AcisOptions* ao = NULL);

/**
 * Creates a sheet body having a single planar face by imprinting coplanar wire bodies, each a closed circuit, onto the plane of the wires.
 * <br><br>
 * <b>Role:</b> <tt>api_cover_wire_loops</tt> covers multiple co-planar wire bodies with a sheet body
 * having a single planar face. The input is a list of wire bodies. The API will
 * create a planar face by covering the first wire body in the list using its edges to form the peripheral
 * loop; the remaining wire bodies in the list are used to form hole loops. This is
 * a purely topological manipulation, and does not involve any Boolean operations.
 * <br><br>
 * Each wire body must be a closed circuit and must not have branches. The wires
 * must not intersect.
 * <br><br>
 * <b>Limitations:</b> Each wire body should have only one wire.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wires
 * list of wire bodies to be covered.
 * @param sheet
 * returned new sheet body.
 * @param ao
 * acis options.
 **/
DECL_COVR outcome api_cover_wire_loops(
	ENTITY_LIST &wires,		// input wire bodies
	BODY		*&sheet,	// sheet body created.
	AcisOptions* ao = NULL
	);

/** @} */
#endif
