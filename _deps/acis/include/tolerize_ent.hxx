/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for Tolerize API routines.

#ifndef TOLERIZE_ENT_APIS_HXX
#define TOLERIZE_ENT_APIS_HXX

#include "kernapi.hxx"

/*******************************************************************/
/**
 * \defgroup KERNAPI Kernel Functionality
 *
 */

/**
 * @file tolerize_ent.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup KERNAPI ACIS Kernel
 *
 * @{
 */


/**
 * \addtogroup ACISTOLERANT
 *
 * @{
 */

/**
 * Replaces an <tt>EDGE</tt> with a tolerant <tt>EDGE</tt> (<tt>TEDGE</tt>).
 * <br><br>
 * <b>Role:</b> Replaces an <tt>EDGE</tt> and its <tt>COEDGE</tt>, <tt>VERTEX</tt>, respectively with a
 * tolerant edge (<tt>TEDGE</tt>), tolerant coedges (<tt>TCOEDGE</tt>), and tolerant vertices (<tt>TVERTEX</tt>).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param this_edge
 * edge to replace.
 * @param this_tedge
 * new tolerant edge.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_replace_edge_with_tedge(
	EDGE*        this_edge,
	TEDGE*       &this_tedge,
	AcisOptions* ao = NULL);

/**
 * Replaces a <tt>VERTEX</tt> with a tolerant <tt>VERTEX</tt> (<tt>TVERTEX</tt>).
 * <br><br>
 * <b>Role:</b> Replaces a <tt>VERTEX</tt> with a <tt>TVERTEX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param this_vertex
 * vertex to replace.
 * @param this_tvertex
 * new tolerant vertex.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_replace_vertex_with_tvertex(
	VERTEX*      this_vertex,
	TVERTEX*     &this_tvertex,
	AcisOptions* ao = NULL);

/**
 * Replaces a <tt>TEDGE</tt> with a normal <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> Replaces a <tt>TEDGE</tt> and its <tt>TCOEDGE</tt> and <tt>TVERTEX</tt>, respectively
 * with a normal <tt>EDGE</tt>,<tt>COEDGE</tt>, and <tt>VERTEX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param this_tedge
 * tolerant tedge to replace.
 * @param this_edge
 * new edge.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_replace_tedge_with_edge(
	TEDGE *this_tedge,
	EDGE * &this_edge,
	AcisOptions *ao = NULL);

/**
 * Replaces a <tt>TVERTEX</tt> with a normal <tt>VERTEX</tt>.
 * <br><br>
 * This API does not update the surrounding edges or faces, and may likely cause 
 * geometric errors. Spatial recommends that you not employ this API 
 * without Technical Support.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param this_tvertex
 * tolerant vertex to replace.
 * @param this_vertex
 * new vertex.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_replace_tvertex_with_vertex(
	TVERTEX*     this_tvertex,
	VERTEX*      &this_vertex,
	AcisOptions* ao = NULL);


/**
 * Optimize (minimizes) the <tt>TVERTEX</tt> tolerance at the ends of a <tt>EDGE</tt> or <tt>TEDGE</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param this_edge
 * edge to optimize.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_KERN outcome api_optimize_tvertex_tolerance(
	EDGE*        this_edge,
	AcisOptions* ao = NULL);

/**
 * @nodoc
 * Checks whether <tt>EDGE</tt>s have errors that require them to be made tolerant, and optionally performs this conversion.
 * <br><br>
 * <b>Role:</b> Checks the <tt>EDGE</tt>s in the given <tt>ENTITY_LIST</tt> for gaps worse than the
 * specified tolerance. Any such <tt>EDGE</tt>s are added to the <tt>bad_edges</tt> list. If
 * <tt>new_edges</tt> is given, such <tt>EDGE</tt>s are converted into <tt>TEDGE</tt>s, and the related
 * <tt>VERTEX</tt> are converted to <tt>TVERTEX</tt> if necessary. It is essential that on spherical faces, edges be split at
 * pole crossings before calling <tt>api_tolerize_entity</tt>. This is done using <tt>api_split_edges_at_poles</tt>.
 * <br><br>
 * The <tt>bad_edges</tt> and <tt>new_edges</tt> lists are mapped so that <tt>bad_edges[i]</tt> is converted
 * into <tt>new_edges[i]</tt>.
 * <br><br>
 * If the <tt>stop_immediately</tt> flag is <tt>TRUE</tt>, processing stops after the first bad edge
 * is found.
 * <br><br>
 * The worst_entity and <tt>worst_error</tt> always get set, even if the error in question
 * was sufficiently small that the entity reported is not actually "bad".
 * <br><br>
 * Note that <tt>api_tolerize_entity</tt> normally converts "bad" <tt>EDGE</tt>s into tolerant ones.
 * This function is only needed to check for "bad" edges where none of the
 * adjacent edges there needed to be made tolerant.
 * <br><br>
 *
 * This API has a fail-safe behavior (i.e. it attempts to do as much as possible and not fail, even in cases when it encounters
 * errors during the operation). This API is broken up internally into atomic operations i.e. checking the error of an individual <tt>EDGE</tt> and tolerizing it.
 * On an event of a recoverable error this API will undo the current atomic operation that failed due to this error,
 * raise a <tt>sys_warning</tt> with the same error message, and proceed further. Note that the fail-safe behavior can be switched OFF
 * by pushing a value of <tt>TRUE</tt> onto the option "careful" before calling this API (and popping it after the call).
 * When the fail-safe behavior is switched off, this API will fail and roll back to its initial state when the first error is encountered.<br><br>
 *
 * A user can determine the result of this API using the following:<br>
 * <dl><dd>
 * a. If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>, then the API has fully succeeded. i.e. errors were not encountered.<br>
 * b. If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>, and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
 * encountered error(s), yet it proceeded further successfully. In this case, <tt>ENTITY_LIST &new_edges</tt> is not guaranteed
 * to be usable and any edges that are not tolerized (due to an error condition) will continue to have gap errors.<br>
 * c. If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and rolled the model back to the state before the API was called. <br>
 * </dd></dl><br>
 *
 * This API puts into the <tt>outcome</tt> returned, the information about any
 * error or problem that it encounters during its operation.
 * For more details, refer to the documentation of <tt>outcome</tt> and the technical article.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param edges
 * input edges.
 * @param bad_edges
 * bad edge list.
 * @param worst_entity
 * worst entity.
 * @param worst_error
 * worst error.
 * @param tol
 * given tolerance if <tt>TRUE</tt>, stop after first bad edge is found.
 * @param stop_immediately
 * if <tt>TRUE</tt>, stop after the first bad edge is found.
 * @param new_edges
 * tolerant edges.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_check_edge_errors(
					  ENTITY_LIST const &edges,
					  ENTITY_LIST       &bad_edges,
					  ENTITY*           &worst_entity,
					  double            &worst_error,
					  double            tol              = SPAresabs,
					  logical           stop_immediately = FALSE,
					  ENTITY_LIST       &new_edges       = *(ENTITY_LIST *) NULL_REF,
					  AcisOptions*      ao               = NULL);


/**
 * @nodoc
 * Checks the <tt>VERTEX</tt> in the given <tt>ENTITY_LIST</tt> for gaps worse than the specified tolerance.
 * <br><br>
 * <b>Role:</b> Checks the vertices in the given list for gaps worse than the
 * specified tolerance. Any such vertices are added to the <tt>bad_vertices</tt> list. If
 * <tt>new_vertices</tt> is given, such vertices are converted into tolerant vertices.
 * <br><br>
 * The <tt>bad_vertices</tt> and <tt>new_vertices</tt> lists are mapped so that <tt>bad_vertices[i]</tt> is
 * converted into <tt>new_vertices[i]</tt>.
 * <br><br>
 * If the <tt>stop_immediately</tt> flag is <tt>TRUE</tt>, processing stops after the first bad
 * vertex is found.
 * <br><br>
 * The <tt>worst_entity</tt> and <tt>worst_error</tt> always get set, even if the error in question
 * was sufficiently small that the entity reported is not actually "bad."
 * <br><br>
 * <tt>api_tolerize_entity</tt> normally converts "bad" vertices into tolerant ones. This
 * function is only needed to check for "bad" vertices where none of the adjacent
 * edges needed to be made tolerant.
 * <br><br>
 *
 * This API has a fail-safe behavior (i.e. it attempts to do as much as possible and not fail, even in cases when it encounters
 * errors during the operation). This API is broken up internally into atomic operations i.e. checking the error of an individual <tt>VERTEX</tt> and tolerizing it.
 * On an event of a recoverable error this API will undo the current atomic operation that failed due to this error,
 * raise a <tt>sys_warning</tt> with the same error message, and proceed further. Note that the fail-safe behavior can be switched OFF
 * by pushing a value of <tt>TRUE</tt> onto the option "careful" before calling this API (and popping it after the call).
 * When the fail-safe behavior is switched off, this API will fail and roll back to its initial state when the first error is encountered.<br><br>
 *
 * A user can determine the result of this API using the following:<br>
 * <dl><dd>
 * a. If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>, then the API has fully succeeded. i.e. errors were not encountered.<br>
 * b. If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>, and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
 * encountered error(s), yet it proceeded further successfully. In this case, <tt>ENTITY_LIST &new_vertices</tt> is not guaranteed
 * to be usable and any vertices that are not tolerized (due to an error condition) will continue to have gap errors.<br>
 * c. If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and rolled the model back to the state before the API was called. <br>
 * </dd></dl>
 * <br>
 *
 * This API puts into the <tt>outcome</tt> returned, the information about any
 * error or problem that it encounters during its operation.
 * For more details, refer to the documentation of <tt>outcome</tt> and the technical article.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param vertices
 * input vertex list.
 * @param bad_vertices
 * bad vertex list.
 * @param worst_entity
 * worst entity.
 * @param worst_error
 * worst error.
 * @param tol
 * given tolerance if <tt>TRUE</tt>, stop after first bad vertex is found.
 * @param stop_immediately
 * if <tt>TRUE</tt>, stop processing after the first bad vertex is found
 * @param new_vertices.
 * tolerant vertices.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_check_vertex_errors(
					  ENTITY_LIST const &vertices,
					  ENTITY_LIST       &bad_vertices,
					  ENTITY*           &worst_entity,
					  double            &worst_error,
					  double            tol              = SPAresabs,
					  logical           stop_immediately = FALSE,
					  ENTITY_LIST       &new_vertices    = *(ENTITY_LIST *) NULL_REF,
					  AcisOptions*      ao               = NULL);


/**
 * \addtogroup ACISTOLERANT
 *
 * @{
 */

/**
 * Tolerizes an entity as needed.
 * <br><br>
 * <b>Role:</b> This API finds all the edges and vertices in a part which have gaps larger
 * than <tt>SPAresabs</tt> and converts them into tolerant entities.
 * <br><br>
 * The <tt>opts</tt> object allows you to remap pointers, that is, to
 * look up the pointer to the tolerant vertex or edge that replaced
 * a given vertex or edge. This is necessary because when an edge or vertex is converted
 * into a tolerant entity, any pointers to it become stale.
 * <br><br>
 * It may be desirable to limit the size of gaps which will be filled in by tolerization; for example,
 * if some edge or vertex has a gap larger than the width of the model. The @href set_max_gap
 * method of the options object allows you to do this. If an edge or a vertex has a gap
 * exceeding the maximum gap, the <tt>outcome</tt> returned by the API will have its <tt>proceeded_after_errors</tt> 
 * flag set, and the <tt>error_info_list</tt> will contain information about which entities this happened to.
 * <br><br>
 * You may elect to repair <tt>TEDGEs</tt> after tolerizing by using the @href set_do_tedge_repair 
 * method of the @href tolerize_entity_opts object.  If tedge repair fails, the tedge involved will
 * be reported in the API outcome.
 * <br><br>
 * The API has failsafe behavior; that is, if it does not tolerize entities with gaps larger
 * than <tt>max_gap</tt>, or fails to repair a tedge, bulletin board rollback does not occur.
 * <br><br>
 * The API is able to check that tvertex and tedge tolerances exceed the gaps, but
 * this may have a negative performance impact. By default, the API ignores gaps on
 * tolerant vertices and edges. The @href set_ignore_tentities method of the options object
 * allows you to change this.
 * <br><br>
 * <b>Limitations</b>: The output data in the options object is
 * cleared each time the API is called.
 * <br>
 * If there is an edge in the input model such that the
 * start and end vertices are separated by less distance
 * than the gap attached to the vertices, a model with
 * overlapping tvertices will occur.
 * <br><br>
 * <b>Errors:</b> None 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param ent
 * Specifies the pointer to the entity being tolerized.
 * @param opts
 * Specifies the @href tolerize_entity_options object.
 * @param ao
 * Specifies ACIS options such as versioning and journaling.
 **/
DECL_KERN outcome api_tolerize_entity(ENTITY* ent, tolerize_entity_opts* opts=NULL, AcisOptions* ao=NULL);
/** @} */
#endif	//TOLERIZE_ENT_APIS_HXX
