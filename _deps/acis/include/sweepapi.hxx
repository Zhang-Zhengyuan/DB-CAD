/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                     ACIS Sweep Component
/*******************************************************************/
#ifndef SWEEPAPI_HXX
#define SWEEPAPI_HXX

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

/**
 * @file sweepapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup SWPAPI Sweeping APIs
 * \ingroup ADVSURF Advanced Surfacing
 * @{
 */

/**
 * Initializes the sweep library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SWP outcome api_initialize_sweeping();

/**
 * Terminates the sweep library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SWP outcome api_terminate_sweeping();

/**
 * Sweeps the given profile along a given edge or a wire.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Sweeping](http://doc.spatial.com/articles/s/w/e/Component~Sweeping_31eb.html)</i>, 
 * <i>[Sweeping with Options](http://doc.spatial.com/articles/s/w/e/Sweeping_with_Options_7371.html)</i>
 * <br><br>
 * <b>Role:</b> This API sweeps along a planar or nonplanar path, and
 * implements the combined functionality of many obsolete APIs. Refer
 * to the file <tt>scm/scmext/smoke/sweep_examples_scm.cpp</tt> 
 * for C++ examples illustrating the <tt>sweep_with_options</tt> 
 * functionality.
 * <br><br>
 * Sweep options can be set through the @href sweep_options class. 
 * The class has data fields representing all of the options that can be 
 * performed while sweeping. The data structure within this class establishes 
 * default values that can be used if no special sweep options are desired.
 * If sweep options are desired, the methods of this class can be used to
 * populate the data fields.
 * <br><br>
 * The <tt>api_sweep_with_options</tt> function has four overloaded instances:
 * <ul>
 * <li> <b>Sweep a distance.</b> </li>
 * <li> <b>Sweep around an axis.</b> </li>
 * <li> <b>Sweep along a vector.</b> </li>
 * <li> <b>Sweep along an edge or a wire.</b> </li>
 * </ul>
 * This signature sweeps the profile along an edge or a wire. The arguments are the profile, 
 * a pointer to an entity (the path), the sweep options, and a pointer to the new body.
 * <br><br>
 * The first argument to the <tt>api_sweep_with_options</tt> function is a profile
 * in the form of a pointer to an entity. For example, an <tt>EDGE</tt> or a <tt>FACE</tt>, which
 * may or may not belong to a body, could form the sweeping profile. A planar
 * sheet body, containing non-adjacent faces is also permissible. The API will
 * sweep each face in the sheet body along the specified path with one call to
 * the function.
 * <br><br>
 * This API will also accepts a profile in the form of a branched wire body.
 * If the solid option is set to <tt>FALSE</tt>, the API will sweep each edge of
 * the wire body along the specified path creating a sheet body. If the solid
 * option is set to <tt>TRUE</tt>, the API will cover a closed combination of edges where
 * possible. Faces resulting from covering will result in solid portions of a
 * new mixed body.
 * <br><br>
 * The next argument is a path. The path may take the form of an <tt>EDGE</tt>
 * pointer or a <tt>BODY</tt> pointer that contains only one non-branched wire. In this 
 * case, the API splits the path curve internally at all non-G<sup>1</sup> discontinuities
 * before the profile is swept. 
 * <br><br>
 * All other inputs are passed to the API through an instance of the <tt>sweep_options</tt>
 * class. The API creates and returns a body if the input profile does not already
 * belong to a body. What is returned depends on the type of profile:
 * <ul>
 * <li> <b>Face belonging to a Body:</b> When the profile is part of a body, the result of the 
 *      sweep is returned as that
 *      body. The owning body is altered and <tt>new_body</tt> points to <tt>NULL</tt>. </li>
 * <li> <b>Face not belonging to a Body:</b> When the profile is a face not already belonging to a
 *      body (a separate face or
 *      edge), a new body is created and returned in the <tt>new_body</tt> argument. The face could 
 *      be deleted or relocated inside the new body. </li>
 * <li> <b>Wire Body:</b> When the profile is a wire body, the owning body is altered and 
 *      <tt>new_body</tt> points to <tt>NULL</tt>. The altered body can be either a solid or a sheet body
 *      depending on whether or not the option to make a solid has been set, and whether
 *      or not the wire body was closed. It is neither necessary nor desired that the
 *      wires be covered. The default is to make a solid if the wire body is closed. </li>
 * <li> <b>Edge not belonging to a Body:</b> When the profile is an edge or set of independent edges,
 *      a new body is created and returned in the <tt>new_body</tt> argument.
 *      The edge remains in the model and can be deleted or used for something else.
 *      If the edge is closed, by default it makes a solid body, otherwise it makes a
 *      sheet body. It is neither necessary nor desired that the edges be covered. </li>
 * </ul>
 * <b>Exceptions:</b> When using the <tt>to_face</tt> option, the result of the sweep will be
 * returned in the <tt>new_body</tt> argument. The face set as the <tt>to_face</tt> will remain.
 * <br><br>
 * When using the <tt>sweep_to_body</tt> option, the result of the sweep will be returned in
 * the <tt>new_body</tt> argument. The pointer to the <tt>new_body</tt> will be the same address as
 * the body being swept to. The body being swept to will be deleted. If you wish to
 * keep that body, you must first make a copy of it. If you wish to unite the sweep
 * and the body being swept to, use the <tt>bool_type</tt> option set to <tt>UNITE</tt> to keep the
 * result as one body.
 * <br><br>
 * <b>Limitations:</b> This API does not guarantee that the body will not be self-intersecting 
 * for certain paths or for certain conditions in sweeping with draft or scale.
 * However, this API does gurantee that individual surfaces will not be self-intersecting if 
 * the sweep option <tt>self_int_test</tt> is set to <tt>TRUE</tt>. 
 * If you want to avoid self-intersecting bodies, then set the global option <tt>careful</tt> to <tt>TRUE</tt>; 
 * more checking will be done at the expense of performance.
 * For profiles that are not perpendicular to the path, and for profile edges that collapse 
 * during sweeping, this API may fail to give correct results.
 * For sweeping with scale, API expects profile to be planar and perpendicular to the path start.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param profile
 * profile to sweep.
 * @param path
 * path to sweep along.
 * @param opts
 * sweep options.
 * @param new_body
 * swept body returned.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_sweep_with_options(
			ENTITY        *profile,
			ENTITY        *path,
			sweep_options *opts,
			BODY          *&new_body,
			AcisOptions   *ao= NULL
			);

/**
 * Sweeps the given profile a specified distance.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Sweeping](http://doc.spatial.com/articles/s/w/e/Component~Sweeping_31eb.html)</i>, 
 * <i>[Sweeping with Options](http://doc.spatial.com/articles/s/w/e/Sweeping_with_Options_7371.html)</i>
 * <br><br>
 * <b>Role:</b> This API sweeps along a planar or nonplanar path, and implements
 * the combined functionality of many obsolete APIs. Refer to the file
 * <tt>scm/scmext/smoke/sweep_examples_scm.cpp</tt> for C++ examples
 * illustrating the <tt>sweep_with_options</tt> functionality.
 * <br><br>
 * Sweep options can be set through the @href sweep_options class. The class has
 * data fields representing all of the options that can be performed while
 * sweeping. The data structure within this class establishes default values
 * that can be used if no special sweep options are desired. If sweep options
 * are desired, the methods of this class can be used to populate the data
 * fields.
 * <br><br>
 * The <tt>api_sweep_with_options</tt> function has four overloaded instances:
 * <ul>
 * <li> <b>Sweep a distance.</b> </li>
 * <li> <b>Sweep around an axis.</b> </li>
 * <li> <b>Sweep along a vector.</b> </li>
 * <li> <b>Sweep along an edge or a wire.</b> </li>
 * </ul>
 * This signature sweeps the profile a specified distance. The arguments are the profile, 
 * the distance, the sweep options, and a pointer to the new body.
 * <br><br>
 * The first argument to the <tt>api_sweep_with_options</tt> function is a profile
 * in the form of a pointer to an entity. The profile must be either a planar <tt>FACE</tt> 
 * or a planar sheet body, which contains only non-adjacent faces. The API will
 * sweep each face in the sheet body the specified distance with one call to
 * this function.
 * <br><br>
 * The next argument is a path. The path is specified as a distance. 
 * <br><br>
 * All other inputs are passed to the API through an instance of the <tt>sweep_options</tt>
 * class. The API creates and returns a body if the input profile does not already
 * belong to a body. What is returned depends on the type of profile:
 * <ul>
 * <li> <b>Face belonging to a Body:</b> When the profile is part of a body, the result of the 
 *      sweep is returned as that
 *      body. The owning body is altered and <tt>new_body</tt> points to <tt>NULL</tt>. </li>
 * <li> <b>Face not belonging to a Body:</b> When the profile is a face not already belonging to a
 *      body (a separate face or
 *      edge), a new body is created and returned in the <tt>new_body</tt> argument. The face could 
 *      be deleted or relocated inside the new body. </li>
 * </ul>
 * <b>Exceptions:</b> When using the to_face option, the result of the sweep will be
 * returned in the <tt>new_body</tt> argument. The face set as the <tt>to_face</tt> will remain.
 * <br><br>
 * When using the <tt>sweep_to_body</tt> option, the result of the sweep will be returned in
 * the <tt>new_body</tt> argument. The pointer to the <tt>new_body</tt> will be the same address as
 * the body being swept to. The body being swept to will be deleted. If you wish to
 * keep that body, you must first make a copy of it. If you wish to unite the sweep
 * and the body being swept to, use the <tt>bool_type</tt> option set to <tt>UNITE</tt> to keep the
 * result as one body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param profile
 * profile to sweep.
 * @param distance
 * distance to sweep.
 * @param opts
 * sweep options.
 * @param new_body
 * swept body returned.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_sweep_with_options(
			ENTITY*        profile,
			double         distance,
			sweep_options* opts,
			BODY*          &new_body,
			AcisOptions*   ao = NULL
			);

/**
 * Sweeps the given profile along the specified vector.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Sweeping](http://doc.spatial.com/articles/s/w/e/Component~Sweeping_31eb.html)</i>, 
 * <i>[Sweeping with Options](http://doc.spatial.com/articles/s/w/e/Sweeping_with_Options_7371.html)</i>
 * <br><br>
 * <b>Role:</b> This API sweeps along a planar or nonplanar path, and implements
 * the combined functionality of many obsolete APIs. Refer to the file
 * <tt>scm/scmext/smoke/sweep_examples_scm.cpp</tt> for C++ examples
 * illustrating the <tt>sweep_with_options</tt> functionality.
 * <br><br>
 * Sweep options can be set through the @href sweep_options class. The class has
 * data fields representing all of the options that can be performed while
 * sweeping. The data structure within this class establishes default values
 * that can be used if no special sweep options are desired. If sweep options
 * are desired, the methods of this class can be used to populate the data
 * fields.
 * <br><br>
 * The <tt>api_sweep_with_options</tt> function has four overloaded instances:
 * <ul>
 * <li> <b>Sweep a distance.</b> </li>
 * <li> <b>Sweep around an axis.</b> </li>
 * <li> <b>Sweep along a vector.</b> </li>
 * <li> <b>Sweep along an edge or a wire.</b> </li>
 * </ul>
 * This signature sweeps the profile along the specified vector. The arguments are the profile, 
 * the vector to sweep along, the sweep options, and a pointer to the new body.
 * <br><br>
 * The first argument to the <tt>api_sweep_with_options</tt> function is a profile
 * in the form of a pointer to an entity. For example, an <tt>EDGE</tt> or a <tt>FACE</tt>, which
 * may or may not belong to a body, could form the sweeping profile. A planar
 * sheet body, containing non-adjacent faces is also permissible. The API will
 * sweep each face in the sheet body along the specified path with one call to
 * the function.
 * <br><br>
 * This API will also accepts a profile in the form of a branched wire body.
 * If the solid option is set to <tt>FALSE</tt>, the API will sweep each edge of
 * the wire body along the specified path creating a sheet body. If the solid
 * option is set to <tt>TRUE</tt>, the API will cover a closed combination of edges where
 * possible. Faces resulting from covering will result in solid portions of a
 * new mixed body.
 * <br><br>
 * The next argument is a path. The path is a three-dimensional vector. This requires 
 * the use of the default rail law (without twist).
 * <br><br>
 * All other inputs are passed to the API through an instance of the <tt>sweep_options</tt>
 * class. The API creates and returns a body if the input profile does not already
 * belong to a body. What is returned depends on the type of profile:
 * <ul>
 * <li> <b>Face belonging to a Body:</b> When the profile is part of a body, the result of the 
 *      sweep is returned as that
 *      body. The owning body is altered and <tt>new_body</tt> points to <tt>NULL</tt>. </li>
 * <li> <b>Face not belonging to a Body:</b> When the profile is a face not already belonging to a
 *      body (a separate face or
 *      edge), a new body is created and returned in the <tt>new_body</tt> argument. The face could 
 *      be deleted or relocated inside the new body. </li>
 * <li> <b>Wire Body:</b> When the profile is a wire body, the owning body is altered and 
 *      <tt>new_body</tt> points to <tt>NULL</tt>. The altered body can be either a solid or a sheet body
 *      depending on whether or not the option to make a solid has been set, and whether
 *      or not the wire body was closed. It is neither necessary nor desired that the
 *      wires be covered. The default is to make a solid if the wire body is closed. </li>
 * <li> <b>Edge not belonging to a Body:</b> When the profile is an edge or set of independent edges,
 *      a new body is created and returned in the <tt>new_body</tt> argument.
 *      The edge remains in the model and can be deleted or used for something else.
 *      If the edge is closed, by default it makes a solid body, otherwise it makes a
 *      sheet body. It is neither necessary nor desired that the edges be covered. </li>
 * </ul>
 * <b>Exceptions:</b> When using the <tt>to_face</tt> option, the result of the sweep will be
 * returned in the <tt>new_body</tt> argument. The face set as the <tt>to_face</tt> will remain.
 * <br><br>
 * When using the <tt>sweep_to_body</tt> option, the result of the sweep will be returned in
 * the <tt>new_body</tt> argument. The pointer to the <tt>new_body</tt> will be the same address as
 * the body being swept to. The body being swept to will be deleted. If you wish to
 * keep that body, you must first make a copy of it. If you wish to unite the sweep
 * and the body being swept to, use the <tt>bool_type</tt> option set to <tt>UNITE</tt> to keep the
 * result as one body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param profile
 * profile to sweep.
 * @param vec
 * vector to sweep along.
 * @param opts
 * sweep options.
 * @param new_body
 * swept body returned.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_sweep_with_options(
			ENTITY*        profile,
			const SPAvector     &vec,
			sweep_options* opts,
			BODY*          &new_body,
			AcisOptions*   ao= NULL
			);

/**
 * Sweeps the given profile around the specified axis.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Sweeping](http://doc.spatial.com/articles/s/w/e/Component~Sweeping_31eb.html)</i>, 
 * <i>[Sweeping with Options](http://doc.spatial.com/articles/s/w/e/Sweeping_with_Options_7371.html)</i>
 * <br><br>
 * <b>Role:</b> This API sweeps along a planar or nonplanar path, and implements
 * the combined functionality of many obsolete APIs. Refer to the file
 * <tt>scm/scmext/smoke/sweep_examples_scm.cpp</tt> for C++ examples
 * illustrating the sweep_with_options functionality.
 * <br><br>
 * Sweep options can be set through the @href sweep_options class. The class has
 * data fields representing all of the options that can be performed while
 * sweeping. The data structure within this class establishes default values
 * that can be used if no special sweep options are desired. If sweep options
 * are desired, the methods of this class can be used to populate the data
 * fields.
 * <br><br>
 * The <tt>api_sweep_with_options</tt> function has four overloaded instances:
 * <ul>
 * <li> <b>Sweep a distance.</b> </li>
 * <li> <b>Sweep around an axis.</b> </li>
 * <li> <b>Sweep along a vector.</b> </li>
 * <li> <b>Sweep along an edge or a wire.</b> </li>
 * </ul>
 * This signature sweeps the profile around the specified axis. The arguments are the profile, 
 * the position of the sweep axis, the vector of the sweep axis, the sweep options, and a pointer to
 * the new body.
 * <br><br>
 * The first argument to the <tt>api_sweep_with_options</tt> function is a profile
 * in the form of a pointer to an entity. For example, an <tt>EDGE</tt> or a <tt>FACE</tt>, which
 * may or may not belong to a body, could form the sweeping profile. A planar
 * sheet body, containing non-adjacent faces is also permissible. The API will
 * sweep each face in the sheet body along the specified path with one call to
 * the function.
 * <br><br>
 * This API will also accepts a profile in the form of a branched wire body.
 * If the solid option is set to <tt>FALSE</tt>, the API will sweep each edge of
 * the wire body along the specified path creating a sheet body. If the solid
 * option is set to <tt>TRUE</tt>, the API will cover a closed combination of edges where
 * possible. Faces resulting from covering will result in solid portions of a
 * new mixed body.
 * <br><br>
 * The next argument is a path. The path takes the form of an axis to sweep around 
 * (specified by a position and a vector). This path requires the default rail law
 * (without twist).
 * <br><br>
 * All other inputs are passed to the API through an instance of the <tt>sweep_options</tt>
 * class. The API creates and returns a body if the input profile does not already
 * belong to a body. What is returned depends on the type of profile:
 * <ul>
 * <li> <b>Face belonging to a Body:</b> When the profile is part of a body, the result of the 
 *      sweep is returned as that
 *      body. The owning body is altered and <tt>new_body</tt> points to <tt>NULL</tt>. </li>
 * <li> <b>Face not belonging to a Body:</b> When the profile is a face not already belonging to a
 *      body (a separate face or
 *      edge), a new body is created and returned in the <tt>new_body</tt> argument. The face could 
 *      be deleted or relocated inside the new body. </li>
 * <li> <b>Wire Body:</b> When the profile is a wire body, the owning body is altered and 
 *      <tt>new_body</tt> points to <tt>NULL</tt>. The altered body can be either a solid or a sheet body
 *      depending on whether or not the option to make a solid has been set, and whether
 *      or not the wire body was closed. It is neither necessary nor desired that the
 *      wires be covered. The default is to make a solid if the wire body is closed. </li>
 * <li> <b>Edge not belonging to a Body:</b> When the profile is an edge or set of independent edges,
 *      a new body is created and returned in the <tt>new_body</tt> argument.
 *      The edge remains in the model and can be deleted or used for something else.
 *      If the edge is closed, by default it makes a solid body, otherwise it makes a
 *      sheet body. It is neither necessary nor desired that the edges be covered. </li>
 * </ul>
 * <b>Exceptions:</b> When using the <tt>to_face</tt> option, the result of the sweep will be
 * returned in the <tt>new_body</tt> argument. The face set as the <tt>to_face</tt> will remain.
 * <br><br>
 * When using the <tt>sweep_to_body</tt> option, the result of the sweep will be returned in
 * the <tt>new_body</tt> argument. The pointer to the <tt>new_body</tt> will be the same address as
 * the body being swept to. The body being swept to will be deleted. If you wish to
 * keep that body, you must first make a copy of it. If you wish to unite the sweep
 * and the body being swept to, use the <tt>bool_type</tt> option set to <tt>UNITE</tt> to keep the
 * result as one body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ent
 * face wire or edge.
 * @param root
 * point on an axis to sweep around.
 * @param axis
 * the direction of an axis to sweep around.
 * @param opts
 * sweep options.
 * @param new_body
 * swept body returned.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_sweep_with_options(
			ENTITY*        ent,
			const SPAposition    &root,
			const SPAvector      &axis,
			sweep_options* opts,
			BODY*          &new_body,
			AcisOptions*   ao = NULL
			);

/**
 * Projects a wire body onto a manifold or nonmanifold body.
 * <br><br>
 * <b>Role:</b> The projection creates a set of tabulated surfaces corresponding to
 * each edge of the input wire body, using the projection
 * direction and distance, and imprints the resulting sheet body onto the target body.
 * <br><br>
 * The projection is imprinted onto the <tt>body</tt>. This means, for example, a circle
 * could be projected onto the face of a block and then the resulting circular face
 * swept to make a post on the block.
 * <br><br>
 * <tt>api_project_wire</tt> is implemented using a sweep and an imprint. First
 * the <tt>wire_body</tt> is swept by the distance <tt>dist</tt> in the direction specified by <tt>dir</tt>,
 * then the swept body and the given <tt>body</tt> are imprinted. It is not necessary that
 * the wire body being projected be planar.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to wire is <tt>NULL</tt> or not to a wire body.
 * <dd>&nbsp;Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.</dd>
 * <dd>&nbsp;Zero length direction vector specified.</dd>
 * <dd>&nbsp;Distance not greater than zero.</dd>
 * </dl>
 * <b>Limitations:</b> Because the process of projecting a wire body onto a body
 * generates a swept surface, any condition that causes an invalid sweep will fail. All
 * possible intersections within the projection distance will be found and imprinted
 * on the sheet body.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_body
 * wire body to be projected.
 * @param body
 * body to project onto.
 * @param dir
 * direction to project.
 * @param dist
 * projection distance.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_project_wire(
			BODY*               wire_body,
			BODY*                body,
			const SPAunit_vector &dir,
			double               dist,
			AcisOptions*         ao= NULL
		);

/**
  * Offsets a face's loops to form a new body.
 * <br><br>
 * <b>Role:</b> The <tt>gap</tt> argument can have values of <tt>arc</tt>, <tt>corner</tt>, or <tt>natural</tt>, and
 * indicates the method employed to close the offset gap.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_face
 * face to be offset.
 * @param offset_dist
 * distance to offset.
 * @param outbody
 * offset body.
 * @param gap
 * offset gap type, default is <tt>arc</tt>.
 * @param ao
 * ACIS options.
 **/
DECL_SWP outcome api_offset_planar_face_loops(
											  FACE*        in_face,
											  double       offset_dist,
											  BODY*        &outbody,
											  sg_gap_type  gap = arc,
											  AcisOptions* ao = NULL
											);

/**
 * Extrudes a planar face or set of faces by a distance.
 * <br><br>
 * <b>Role:</b> <tt>api_extrude_planar_faces</tt> uses a highly efficient, but
 * somewhat restrictive, algorithm to extrude a planar face or a set of faces by a distance.
 * This algorithm requires the input entity to meet specific conditions (described
 * below under Limitations) but for efficiency does not check that the input entity 
 * meets these conditions. This algorithm also does not produce annotations. It is 
 * simply a highly efficient algorithm to extrude faces. 
 * <br><br>
 * If the input is a face that belongs to a body, the new faces will be added 
 * to the shell containing the face in the existing body. This must be the only 
 * face in this shell. If the input is a stand-alone face (that does not belong 
 * to a body), a new body, lump, and shell will be constructed to contain the 
 * returned faces. If the input is a body, each lump of the body must have one 
 * shell. If the shell contains a single face it will be swept as described above. 
 * If the shell contains multiple connected faces, the faces are swept simultaneously. 
 * <br><br>
 * Extrusions can be performed with a draft angle provided the resulting geometry is
 * analytic. This imposes additional restrictions on the input data which are described
 * in Limitations. The draft angle is specified using an extrusion options object. If 
 * an extrusion is performed with a draft angle, it is possible to perform a check
 * for self-intersecting geometry. This is specified using the extrusion options
 * object and it significantly impacts performance. Generally speaking, extrusions
 * should be specified that do not self-intersect and do not need to be checked for
 * self-intersections.
 * <br><br>
 * Extrusions also support the propagation of <tt>ATTRIB_CELL_PRECEDENCE</tt> attributes.
 * More specifically, cellular topology should be attached to the input sheet faces
 * with the <tt>cell_granularity</tt> option set to <tt>"face"</tt>, which will attach
 * a 2D cell to each sheet face. If <tt>ATTRIB_CELL_PRECEDENCE</tt> attributes are added
 * to these 2D cells, the attributes will propagate to the resulting 3D cells when the 
 * faces are extruded.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to entity is <tt>NULL</tt> or not to a face or body.
 * <dd>&nbsp;Distance not greater than zero.</dd>
 * </dl>
 * <b>Limitations:</b> When extruding an isolated face:
 * <ul>
 * <li> The input face or body must pass all checks at check_level 20. </li>
 * <li> The distance must be positive; i.e., 
 *      the extrusion is in direction of face normal. </li>
 * <li> The face can contain no non-manifold vertices. </li>
 * <li> The geometry of the face must be a plane. </li>
 * <li> The geometry of all the edges must be either straights or circular or 
 *      elliptical arcs. </li>
 * <li> None of the edges or vertices can be tolerant. </li>
 * <li> Each loop of the face must be closed. </li>
 * <li> Each loop of the face must enclose a finite area. </li>
 * <li> Each edge must have precisely one coedge; that is, 
 *      each edge must be a free edge. </li>
 * <li> The graph of edges can contain no branches; therefore, 
 *      there are no spur edges. </li>
 * <li> If the input entity is a face that is owned by a body, 
 *      and the owning body contains other faces or wires, 
 *      they cannot be connected to the input face and cannot intersect 
 *      with the extruded volume.  </li>
 * </ul>
 * When extruding a connected set of single-sided or double-sided faces: 
 * <ul>
 * <li> The faces can be either single-sided or double-sided; that is, 
 *      the set of faces forms either a sheet or an open, solid shell. </li>
 * <li> Each face must meet the requirements listed above for a single face, 
 *      with the following two exceptions: </li>
 * <ul>
 * <li>   An edge may have two coedges, 
 *        but the coedges must be on two different faces. </li>
 * <li>   A graph of edges can have branches, but spur edges are not allowed. </li>
 * </ul>
 * <li> The faces of the input body must have consistent face normal directions. </li>
 * <li> Each lump of the input body must have a single shell - and contain no wires, 
 *    just the planar sheet. </li>
 * <li> The input body contains no non-manifold vertices. </li> 
 * </ul>
 * When extruding with a draft angle:
 * <ul>
 * <li> The input must consist of a single face, or if there is more than one face,
 *      each face must be in a separate shell. </li>
 * <li> The draft angle and extrusion distance must be chosen such that the resulting
 *      lateral faces do not self-intersect and the topology of the top face is 
 *      the same as the bottom face.  That is, edges do not disappear and the connectivity
 *      of edges on the top face is the same as the connectivity of edges on the bottom 
 *      face. </li>
 * <li> The resulting geometry must consist entirely of analytic curves and surfaces.
 *      This implies that the edge geometry must consist entirely of straights and circles.
 *      In addition, circular edges must meet adjacent edges with tangent continuity. </li> 
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param profile
 * face or body to extrude.
 * @param dist
 * extrusion distance. Must be positive.
 * @param returned_body
 * body containing the extruded faces. May be new or pre-existing.
 * @param opts
 * extrusion options object.
 * @param ao
 * acis options.
 **/
 
 DECL_SWP outcome api_extrude_planar_faces(
		ENTITY*			profile,
		double			dist,
		BODY*&			returned_body,
		extrude_options* opts = NULL,
		AcisOptions*	ao = NULL
		);

 class make_sweep_twist_rail_options;

/**
 * Creates a rail law for sweeping subject to the given twist vectors.
 * <br><br>
 * <b>Role:</b> This API creates a rail law which can be used to control
 * the orientation of the profile as it moves along the path. It smoothly
 * interpolates the twist vectors at the given parameters.
 * <br><br>
 * <b>Limitations:</b> There should at least be two parameters. First 
 * parameter in the list must be equal to the parameter of the start point
 * of the given edge within <tt>SPAresnor</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param path
 * EDGE on which rail law is defined
 * @param params
 * list of edge parameters
 * @param twist
 * twist vectors perpendicular to the path at the given parameters
 * @param twist_rail
 * rail law returned by this API
 * @param mstr_opts
 * Place holder for additional options which may be added in the future
 * @param ao
 * ACIS options
 */
DECL_SWP outcome 
api_make_sweep_twist_rail(
    EDGE* path,
	SPAdouble_vector const& params,
	SPAunit_vector_list const& twist,
	law*& twist_rail,
	make_sweep_twist_rail_options* mstr_opts = NULL,
	AcisOptions const* ao = NULL
	);


class make_sweep_path_options;

/**
 * Constructs a path useful for creating a swept surface.
 * <br><br>
 * <b>Role:</b> This API creates a path which interpolates the given points.
 * Finer control may be exercised using @href make_sweep_path_options object.
 * <br><br>
 * <b>Limitations:</b> There should at least be two points.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param pts
 * ordered list of positions
 * @param path
 * edge returned by this API
 * @param mspo
 * path options for finer control
 * @param ao
 * ACIS options
 */
DECL_SWP outcome
api_make_sweep_path(
	SPAposition_vector const& pts,
	EDGE*& path,
	make_sweep_path_options* mspo = NULL,
	AcisOptions const* ao = NULL
	);

#include <ostream>
class make_sweep_path_options_impl;

/**
* This class allows the user to exercise finer control over the construction
* of sweep path.
* <br><br>
* <b>Role:</b> It contains the first and second derivatives of the path at 
* the given parameters.
*/
class DECL_SWP make_sweep_path_options : public ACIS_OBJECT
{
public:

/**
 * Constructs a <tt>make_sweep_path_options</tt> object (default constructor).
 */
	make_sweep_path_options();

/**
 * C++ destructor, deletes a <tt>make_sweep_path_options</tt> object. 
 */
	~make_sweep_path_options();

/**
 * Sets the parameters along the path.
 */
	void set_params( SPAdouble_vector const& params );

/**
 * Sets the derivatives of the path at the given parameters.
 */
	void set_derivs( SPAvector_list const& first_derivs,
		SPAvector_list const& second_derivs = *( SPAvector_list const* )NULL_REF );
/**
 * Returns the list of parameters.
 */
	SPAdouble_vector const& params() const;

	void output(std::ostream& os) const;

private:

	make_sweep_path_options( make_sweep_path_options const& );
	make_sweep_path_options& operator=( make_sweep_path_options const& );

	make_sweep_path_options_impl *_impl;
	friend class make_sweep_path_options_impl;
};

/** @} */
#endif
