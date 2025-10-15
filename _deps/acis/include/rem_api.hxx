/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Header for rem_husk api routines.
/*******************************************************************/
#if !defined( REM_HUSK_API_ROUTINES )
#define REM_HUSK_API_ROUTINES
#include "api.hxx"
#include "dcl_rem.h"
/**
* @file rem_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup REMAPI Remove Faces
 *
 * \ingroup LOCALOPS
 * @{
 */
// Forward declarations
class outcome;
class FACE;
class LOOP;
class EDGE;
class SPAposition;
class lop_options;
/**
 * Initializes the removal library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_REM outcome api_initialize_face_removal();
/**
 * Terminates the removal library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_REM outcome api_terminate_face_removal();
/**
 * Removes an array of faces, growing the adjacent faces to fill the gap.
 * <b>Technical Article:</b> <i>[Remove Faces](http://doc.spatial.com/articles/r/e/m/Remove_Face_77ec.html)</i>
 * <br><br>
 * <b>Role:</b> Curves and surfaces of edges and faces surrounding the removed faces, and which end in
 * the user supplied box, are extended by a length approximately equal to twice the diagonal
 * of the supplied box.
 * SPAposition(0,0,0) can be used as default value for both box-low
 * and box-high when user is unable to specify a valid box.
 * <br><br>
 * Similarly the intersections required to produce new edges and vertices, are limited by the
 * size of the user supplied box.
 * <br><br>
 * Thus the user should supply a box likely to contain the changed faces. The box should account for body transform.
 * An overly large box will result in wasted computation. The default box is twice the body box of the original body.
 * <br><br>
 * Note that there may be no solution to the gap filling problem, or no solution using adjacent
 * faces only.
 * If the function detects these circumstances it issues an error and leaves the body unchanged.
 * <br><br>
 * Adjacent faces left infinite and with no loops will be deleted, example, the tops of cylindrical
 * bosses having the cylindrical face removed.
 * <br><br>
 * If the faces to be removed are isolated from one another, they are removed in the order supplied.
 * <br><br>
 * The body shells and lumps are corrected at the end of the operation, should they have been split.
 * <br><br>
 * New shells of different solidity to the original shell are deleted, example, void shells made
 * from a shell originally solid.
 * <br><br>
 * <b>Errors:</b> 
 * Some of the following errors result in an <tt>ENTITY</tt>, which indicates where the error occurs,
 * being highlighted.
 * <br><br>
 * The <tt>ENTITY</tt> type follows the error message below.
 * <br><br>
 * <dl>
 * <dt>At least one face must be supplied or error: </dt>
 * <dd><tt>REM_NO_FACE</tt> "no faces supplied"</dd>
 * <br><br>
 * <dt>Non-duplicate valid faces must be supplied from the same shell or error: </dt>
 * <dd><tt>REM_BAD_FACE</tt> "face(s) supplied invalid, duplicate or from different shells"</dd>
 * <br><br>
 * <dt>A moat of faces must exist around the faces being removed (see limitations for exceptions) or error:</dt>
 * <dd><tt>REM_FREE_EDGE</tt> "free edge detected"</dd>
 * <br><br>
 * <dt>The faces being removed and their neighboring faces must be manifold or error:</dt>
 * <dd><tt>REM_NON_MANIFOLD</tt> "non-manifold edge detected" </dd>
 * <br><br>
 * <dt>All faces in a shell or lump may be removed, so long as another shell remains, or error:</dt>
 * <dd><tt>REM_LAST_SHELL</tt> "only shell in body would be lost"</dd>
 * <br><br>
 * <dt>Box must be valid if supplied, or error:</dt>
 * <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
 * <br><br>
 * <dt>Adjacent faces must be able to combine to fill the gap left by the removed faces or error: </dt>
 * <dd><tt>REM_NO_SOLUTION</tt> "gap cannot be filled"</dd>
 * <br><br>
 * <dt>Non-adjacent faces are affected during healing of the gap left by removed faces: </dt>
 * <dd><tt>REM_REMOTE_FACE_INTR</tt> "remove operation affects remote face(s)"</dd>
 * <br><br>
 * <dt>Internal Algorithmic problems produce the error: </dt>
 * <dd><tt>REM_INTERNAL</tt> "internal error"</dd>
 * </dl>
 * <br><br>
 * <b>Limitations:</b> The faces being removed and their neighboring faces must be manifold. 
 * The faces to be removed cannot be on the boundary of a sheet, except if they are blend faces or if the free edges form an internal gap. 
 * <br><br>
 * Some growing faces may shrink if necessary, but not so far as their boundaries. The healing
 * process must be entirely within the faces adjacent to those removed.
 * <br><br>
 * No checks are made to see if the faces grown intersect with other non growing faces in the model.
 * Checks that the growing faces intersect with one another is not rigorously done between
 * unconnected regions of growing faces.
 * <br><br>
 * If the geometry of the edges lateral to the face(s) being removed do not have a well-defined extension, 
 * this can cause the API to fail.
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
 * faces being removed.
 * @param box_low
 * start of intersection and extension box to be used. The box should account for body transform.
 * @param box_high
 * end of intersection and extension box to be used. The box should account for body transform..
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_REM outcome api_remove_faces(
								  int const    nface,
								  FACE*        face[],
								  const SPAposition&  box_low,
								  const SPAposition&  box_high,
								  lop_options* pLopts,
								  AcisOptions* ao = NULL
								  );
/**
 * @nodoc
 * Removes an array of faces, growing the adjacent faces to fill the gap.
 * <b>Role:</b> Curves and surfaces of edges and faces surrounding the removed faces, and which end in
 * the user supplied box, are extended by a length approximately equal to twice the diagonal
 * of the supplied box.
 * <br><br>
 * Similarly the intersections required to produce new edges and vertices, are limited by the
 * size of the user supplied box.
 * <br><br>
 * Thus the user should supply a box likely to contain the changed faces. The box should account for body transform.
 * An overly large box will result in wasted computation. The default box is twice the body box of the original body.
 * <br><br>
 * Note that there may be no solution to the gap filling problem, or no solution using adjacent
 * faces only.
 * If the function detects these circumstances it issues an error and leaves the body unchanged.
 * <br><br>
 * Adjacent faces left infinite and with no loops will be deleted, example, the tops of cylindrical
 * bosses having the cylindrical face removed.
 * <br><br>
 * If the faces to be removed are isolated from one another, they are removed in the order supplied.
 * <br><br>
 * The body shells and lumps are corrected at the end of the operation, should they have been split.
 * <br><br>
 * New shells of different solidity to the original shell are deleted, example, void shells made
 * from a shell originally solid.
 * <br><br>
 * For more information refer to the Technical Article <i>[Remove Faces](http://doc.spatial.com/articles/r/e/m/Remove_Face_77ec.html)</i>.
 * <br><br>
 * <b>Errors:</b> Some of the following errors result in an <tt>ENTITY</tt>, which indicates where the error occurs,
 * being highlighted.
 * <br><br>
 * The <tt>ENTITY</tt> type follows the error message below.
 * <br><br>
 * <dl>
 * <dt>At least one face must be supplied or error: </dt>
 * 	<dd><tt>REM_NO_FACE</tt> "no faces supplied"</dd>
 * <br><br>
 * <dt>Non-duplicate valid faces must be supplied from the same shell or error: </dt>
 * <dd><tt>REM_BAD_FACE</tt> "face(s) supplied invalid, duplicate or from different shells"</dd>
 * <br><br>
 * <dt>A moat of faces must exist around the faces being removed (see limitations for exceptions) or error:</dt>
 * <dd><tt>REM_FREE_EDGE</tt> "free edge detected"</dd>
 * <br><br>
 * <dt>The faces being removed and their neighboring faces must be manifold or error:</dt>
 * <dd><tt>REM_NON_MANIFOLD</tt> "non-manifold edge detected" </dd>
 * <br><br>
 * <dt>All faces in a shell or lump may be removed, so long as another shell remains, or error:</dt>
 * <dd><tt>REM_LAST_SHELL</tt> "only shell in body would be lost"</dd>
 * <br><br>
 * <dt>Box must be valid if supplied, or error:</dt>
 * <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
 * <br><br>
 * <dt>Adjacent faces must be able to combine to fill the gap left by the removed faces or error: </dt>
 * <dd><tt>REM_NO_SOLUTION</tt> "gap cannot be filled"</dd>
 * <br><br>
 * <dt>Non-adjacent faces are affected during healing of the gap left by removed faces: </dt>
 * <dd><tt>REM_REMOTE_FACE_INTR</tt> "remove operation affects remote face(s)"</dd>
 * <br><br>
 * <dt>Internal Algorithmic problems produce the error: </dt>
 * <dd><tt>REM_INTERNAL</tt> "internal error"</dd>
 * </dl>
 * <br><br>
 * <b>Limitations:</b> The faces being removed and their neighboring faces must be manifold. 
 * The faces to be removed cannot be on the boundary of a sheet, except if they are blend faces or if the free edges form an internal gap. 
 * <br><br>
 * Some growing faces may shrink if necessary, but not so far as their boundaries. The healing
 * process must be entirely within the faces adjacent to those removed.
 * <br><br>
 * No checks are made to see if the faces grown intersect with other non growing faces in the model.
 * Checks that the growing faces intersect with one another is not rigorously done between
 * unconnected regions of growing faces.
 * <br><br>
 * If the geometry of the edges lateral to the face(s) being removed do not have a well-defined extension, 
 * this can cause the API to fail.
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
 * faces being removed.
 * @param box_low
 * start of intersection and extension box to be used. The box should account for body transform.
 * @param box_high
 * end of intersection and extension box to be used. The box should account for body transform.
 * @param ao
 * ACIS options.
 **/
DECL_REM outcome api_remove_faces(
								  int const    nface,
								  FACE*        face[],
								  SPAposition  box_low,
								  SPAposition  box_high,
								  AcisOptions* ao = NULL
								  );


/**
 * Removes an array of EDGEs in a WIRE body by extending the adjacent EDGEs and intersecting to fill the gap.
 * <b>Technical Article:</b> <i>[Remove Wire Edges](http://doc.spatial.com/articles/r/e/m/Remove_Wire_Edges_afde.html)</i>
 * <br><br>
 *  <b>Role:</b> The array of EDGEs supplied to the API must all belong to the same WIRE body but do not have to connect. 
 * The functionality of the API will remove the EDGEs from the WIRE body and extend the neighboring EDGEs (naturally) and intersect 
 * to close the gap. If no intersection exists, then the API will return "gap cannot be filled". The API, additionally, takes a bounding 
 * box as input. If an EDGE/EDGE intersection falls outside of the bounding box, then the API will, again, fail. If the API detects either
 * of these circumstances it leaves the body unchanged. If the intersection involves two identical curves such as straights, then a VERTEX
 * is placed between the two EDGEs based on an average distance.  
 * <br><br>
 * The API works on both open and closed WIRES. If an open WIRE is given and the EDGE to be removed is either of the EDGEs making up the
 * ends of the WIRE, the EDGE will simply be removed from the WIRE body. If the WIRE body contains only one EDGE and that EDGE is asked
 * to be removed, the WIRE body will be deleted. 
 * <br><br> 
 * <b>Errors:</b> Some of the following errors result in <tt>EDGE(s)</tt> being highlighted, which indicates where the error occurs.
 *<br><br>
 * The <tt>ENTITY</tt> type follows the error message below.
 * <br><br>
 *<dl>
 * <dt>At least one EDGE must be supplied: </dt>
 *    <dd><tt>REM_NO_EDGE</tt> "no EDGEs supplied"</dd>
 *<br><br>
 * <dt>WIRE body must be manifold:</dt>
 * <dd><tt>REM_NO_SOLUTION</tt> "non-manifold edge detected" </dd>
 *<br><br>
 * <dt>Box must be valid if supplied:</dt>
 * <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
 *<br><br>
 *<dt>Adjacent edges must be able to fill the gap left by the removed edges: </dt>
 *<dd><tt>REM_NO_SOLUTION</tt> "gap cannot be filled"</dd>
 *<br><br>
 *<dt>Internal Algorithmic problems: </dt>
 *<dd><tt>REM_INTERNAL</tt> "internal error"</dd>
 *</dl>
 * <br><br>
 * <b>Limitations:</b> WIRE body must be manifold.
 * <br><br>
 * If the geometry of the EDGEs adjacent to the EDGE(s) being removed does not have a well-defined extension, 
 * this can cause the API to fail.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nedge
 * number of edges.
 * @param edge
 * edges being removed.
 * @param box_low
 * start of intersection and extension box to be used. 
 * @param box_high
 * end of intersection and extension box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
DECL_REM outcome api_remove_wire_edges(
								       int const    nedge,
								       EDGE**       edge,
								       const SPAposition&  box_low,
								       const SPAposition&  box_high,
									   lop_options* pLopts,
								       AcisOptions* ao = NULL
									   );

/**
 * Removes an open gap, growing the adjacent faces to fill the gap.
 * <b>Technical Article:</b> <i>[Remove Open Gap](http://doc.spatial.com/articles/r/e/m/Remove_Open_Gap_01ed.html)</i>
 * <br><br>
 * <b>Role:</b> Curves and surfaces of edges and faces surrounding the open gap, and which end in
 * the user-supplied box, are extended by a length approximately equal to twice the diagonal
 * of the supplied box.
 * <br><br>
 * Similarly, the intersections required to produce new edges and vertices are limited by the
 * size of the user-supplied box.
 * <br><br>
 * Thus, the user should supply a box likely to contain the changed edges. An overly large box 
* will result in wasted computation. The default box is twice the body box of the original body.
 * <br><br>
 * Note that there may be no solution to the gap filling problem, or no solution using adjacent
 * faces only.
 * If the function detects these circumstances, it issues an error and leaves the body unchanged.
 * <br><br>
 * Adjacent faces left infinite and with no loops will be deleted.
 * <br><br>
 * The body shells and lumps are corrected at the end of the operation, if they have been split.
 * <br><br>
 * New shells of different solidity to the original shell are deleted, for example, void shells made
 * from a shell originally a solid.
 * <br><br>
 * <b>Errors:</b> Some of the following errors result in an <tt>ENTITY</tt>, which indicates where the error occurs:
 * <br><br>
 * The <tt>ENTITY</tt> type follows the error message below.
 * <br><br>
 * <dl>
 * <dt>At least one edge must be supplied or error: </dt>
 * <dd><tt>REM_NO_EDGE</tt> "no edges supplied"</dd>
 * <br><br>
 * <dt>A moat of faces around the gap must exist or error:</dt>
 * <dd><tt>REM_FREE_EDGE</tt> "free edge detected"</dd>
 * <br><br>
 * <dt>The faces surrounding the gap must be manifold or error:</dt>
 * <dd><tt>REM_NON_MANIFOLD</tt> "non-manifold edge detected"</dd>
 * <br><br>
 * <dt>Box must be valid if supplied, or error:</dt>
 * <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
 * <br><br>
 * <dt>Adjacent faces must be able to combine to fill the gap left by the removed faces or error: </dt>
 * <dd><tt>REM_NO_SOLUTION</tt> "gap cannot be filled"</dd>
 * <br><br>
 * <dt>Internal Algorithmic problems produce the error: </dt>
 * <dd><tt>REM_INTERNAL</tt> "internal error"</dd>
 * </dl>
 * <br><br>
 * <b>Limitations:</b> <br>A moat of faces around the gap must exist. The faces surrounding the gap must be manifold.
 * <br><br>
 * Some growing faces may shrink if necessary, but not so far as their boundaries. The healing
 * process must be entirely within the faces adjacent to those removed.
 * <br><br>
 * No checks are made to see if the faces grown intersect with other non-growing faces in the model.
 * Checks that the growing faces intersect with one another is not rigorously done between
 * unconnected regions of growing faces.
 * <br><br>
 * If the geometry of the edges lateral to the gap being removed does not have a well-defined extension, 
 * this can cause the API to fail.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral 
 * <br><br>
* @param edge
 * Specifies the edge which is part of the open gap that is being removed.
 * @param box_low
 * Specifies the start of an intersection and extension box to be used. 
 * @param box_high
 * Specifies the end of an intersection and extension box to be used. 
 * @param pLopts
 * Specifies local operations options. NULL argument will set the default values.
 * @param ao
 * Specifies ACIS options such as versioning and journaling.
 **/
DECL_REM outcome api_remove_open_gap(
				EDGE *edge,
                const SPAposition& box_low,
                const SPAposition& box_high,
				lop_options* pLopts,
				AcisOptions * ao);

/**
 * Removes an open gap consisting of two open circuits, specified by the input edges, growing the adjacent faces to fill the gap.
 * <b>Technical Article:</b> <i>[Remove Open Gap](http://doc.spatial.com/articles/r/e/m/Remove_Open_Gap_01ed.html)</i>
 * <br><br>
 * <b>Role:</b> Curves and surfaces of edges and faces surrounding the open gap, and which end in
 * the user-supplied box, are extended by a length approximately equal to twice the diagonal
 * of the supplied box.
 * <br><br>
 * Similarly the intersections required to produce new edges and vertices, are limited by the
 * size of the user-supplied box.
 * <br><br>
 * Thus, the user should supply a box likely to contain the changed faces. The box should account for body transform. An overly large box
 * will result in wasted computation. The default box is twice the body box of the original body.
 * <br><br>
 * Note that there may be no solution to the gap filling problem, or no solution using adjacent
 * faces only.
 * If the function detects these circumstances, it issues an error and leaves the body unchanged.
 * <br><br>
 * Adjacent faces left infinite and with no loops will be deleted, for example, the tops of cylindrical
 * bosses.
 * <br><br>
 * The body shells and lumps are corrected at the end of the operation, if they have been split.
 * <br><br>
 * New shells of different solidity to the original shell are deleted, for example, void shells made
 * from a shell originally a solid.
 * <br><br>
 * <b>Errors:</b> Some of the following errors result in an <tt>ENTITY</tt>, which indicates where the error occurs:
 * <br><br>
 * The <tt>ENTITY</tt> type follows the error message below.
 * <br><br>
 * <dl>
 * <dt>At least one edge must be supplied or error: </dt>
 * <dd><tt>REM_NO_EDGE</tt> "no edges supplied"</dd>
 * <br><br>
 * <dt>A moat of faces around the gap must exist or error:</dt>
 * <dd><tt>REM_FREE_EDGE</tt> "free edge detected"</dd>
 * <br><br>
 * <dt>The faces surrounding the gap must be manifold or error:</dt>
 * <dd><tt>REM_NON_MANIFOLD</tt> "non-manifold edge detected"</dd>
 * <br><br>
 * <dt>Box must be valid if supplied, or error:</dt>
 * <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
 * <br><br>
 * <dt>Adjacent faces must be able to combine to fill the gap left by the removed faces or error: </dt>
 * <dd><tt>REM_NO_SOLUTION</tt> "gap cannot be filled"</dd>
 * <br><br>
 * <dt>Non-adjacent faces are affected during healing of the gap: </dt>
 * <dd><tt>REM_REMOTE_FACE_INTR</tt> "remove operation affects remote face(s)"</dd>
 * <br><br>
 * <dt>Internal Algorithmic problems produce the error: </dt>
 * <dd><tt>REM_INTERNAL</tt> "internal error"</dd>
 * </dl>
 * <br><br>
 * <b>Limitations:</b> A moat of faces around the gap must exist. The faces surrounding the gap must be manifold.
 * <br><br>
 * Some growing faces may shrink if necessary, but not so far as their boundaries. The healing
 * process must be entirely within the faces adjacent to those removed.
 * <br><br>
 * No checks are made to see if the faces grown intersect with other non-growing faces in the model.
 * Checks that the growing faces intersect with one another is not rigorously done between
 * unconnected regions of growing faces.
 * <br><br>
 * If the geometry of the edges lateral to the gap being removed do not have a well-defined extension, 
 * this can cause the API to fail.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral 
 * <br><br>
 * @param edge1
 * Specifies the edge which is part of the first open gap being removed.
 * @param edge2
 * Specifies the edge which is part of the second open gap being removed, can be NULL.
 * @param box_low
 * Specifies the start of intersection and extension box to be used. The box should account for body transform.
 * @param box_high
 * Specifies the end of intersection and extension box to be used.The box should account for body transform.
 * @param pLopts
 * Specifies the local operations options. NULL argument will set the default values.
 * @param ao
 * Specifies ACIS options such as versioning and journaling.
 **/
DECL_REM outcome api_remove_open_gap(
				EDGE *edge1,
				EDGE *edge2,
                const SPAposition& box_low,
                const SPAposition& box_high,
				lop_options* pLopts,
				AcisOptions * ao);


/**
* Removes a list of [free](http://doc.spatial.com/articles/e/d/g/Edge.html#Free_Edge), [spur](http://doc.spatial.com/articles/e/d/g/Edge.html#Spur_Edge),[prop EDGEs](http://doc.spatial.com/articles/e/d/g/Edge.html#Prop_Edge) from a sheet body by extending and intersecting the adjacent EDGEs to fill the gap. 
* <b>Technical Article:</b> <i>[Remove Edges](https://doc.spatial.com/articles/r/e/m/Remove_Edges_8dd3.html)</i>
* <br><br>
*  <b>Role:</b> The input EDGEs supplied to the API must all belong to the same shell of a sheet body but do not have to connect.
* The API will remove the EDGEs from the sheet body and extend the neighboring EDGEs (naturally) and intersect them
* to close the gap. If no intersection exists among the neighboring EDGEs, then the input EDGEs will not be removed. 
* The API also takes a bounding box as input. If intersection of neighboring EDGEs falls outside of the bounding box, then the removal of such input EDGEs will fail. 
* If the API fails to remove any input EDGE, it leaves the body unchanged. If the neighboring EDGEs involve two identical curves, then a VERTEX
* is placed between the two EDGEs based on an average distance.
* <br><br>
* The API has a failsafe behavior. When [careful](https://doc.spatial.com/articles/c/a/r/Option~Careful_bfde.html) option is FALSE, it attempts to remove as many input edges as possible. 
* The information about the encountered errors if any, can be obtained from the outcome object.
* When careful option is TRUE, the API will fail with the first remove failure.
* <br><br>
* <b>Errors:</b> Irrespective of the value of careful option, the API will always fail with appropriate errors if the input expectations are not met.
* <br><br>
* <dl>
* <dt>At least one EDGE must be supplied for removal or error: </dt>
* <dd><tt>REM_NO_EDGE</tt> "no EDGEs supplied"</dd>
* <br><br>
* <dt>Input EDGEs must belong to sheet body or error:</dt>
* <dd><tt>REM_SHEET_BODY</tt> "edge(s) supplied must belong to a sheet body" </dd>
* <br><br>
* <dt>Input EDGEs must be free, prop or spur and belong to same shell or error:</dt>
* <dd><tt>REM_BAD_SHEET_EDGE</tt> "edge(s) supplied invalid, from different shells or shared by multiple faces" </dd>
* <br><br>
* <dt>All neighboring edges and vertices of the edges to be removed must be manifold or error: </dt>
* <dd><tt>REM_NON_MANIFOLD</tt> "non-manifold edge or vertex detected" </dd>
* <br><br>
* <dt>The user supplied intersection box should be valid or error:</dt>
* <dd><tt>REM_BAD_BOX</tt> "invalid box supplied"</dd>
* <br><br>
* <dt>Adjacent edges must be able to fill the gap left by the removed edges or error: </dt>
* <dd><tt>REM_EDGE_NO_SOLUTION</tt> "No solution for given edge-removal scenario"</dd>
* </dl>
* <br><br>
* <b>Limitations:</b> The input edges can be free,prop or spur. They should not have non-manifold vertices.
* <br><br>
* Owner face or neighboring faces of input edges may shrink if necessary, but not so far as to let all edges between them degenerate to zero length.
* <br><br>
* Neighboring edges should extend and intersect. If the geometry of the neighboring EDGEs do not have a well-defined extension, this can cause the API to fail. 
* If they do not intersect, the API will fail. If a single edge is to be removed, then at least one of its neighboring edges incident on its start and end vertices should be free. 
* Similarly, if a group of connected edges are being removed, the at least one neighboring edge of the first and last edge to be removed must be free.
* <br><br>
* In order to retain spur and prop edges that are not part of the input edges list supplied to the API, attach NO_MERGE_ATTRIB to them, otherwise they may be removed by the API
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param edges
* edges being removed.
* @param box_low
* start of intersection and extension box to be used.
* @param box_high
* end of intersection and extension box to be used.
* @param pLopts
* local operations options. NULL argument will set the default values.
* @param ao
* ACIS options.
**/
DECL_REM outcome api_remove_edges(
	const ENTITY_LIST& edges,
	const SPAposition&  box_low,
	const SPAposition&  box_high,
	lop_options* pLopts = nullptr,
	AcisOptions* ao = nullptr
);

/** @} */
#endif
