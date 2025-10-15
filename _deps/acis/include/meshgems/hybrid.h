#ifndef __MESHGEMS_HYBRID_H__
#define __MESHGEMS_HYBRID_H__

#include "meshgems/meshgems.h"
#include "meshgems/hybrid_short_names.h"

/**
 * \defgroup hybrid meshgems_hybrid_session_t
 *@{


 The Hybrid component is an automatic meshing component designed to create automatically  solid meshes out of closed surface meshes.
 The Hybrid component Hybrid will generate solid meshes made with conforming hybrid elements, and does not require any interaction with the user to create meshes.

 The Hybrid component is designed to enforce the following features:

- Boundary layer meshing, with specific patterns around sharp angles. The user basically needs to specify the location of the layer, the size of the first layer, the number of layers, and the size progression between a layer and the next.
- Hexcore meshing with variable element size or with a constant global physical size.
- Tetrahedra filling with pyramids to make the mesh conformal.

To achieve that goal, Hybrid combines an extrusion method, a modified octree, a constrained Delaunay tetrahedrization and a pyramid generation algorithm.

Note that:

- Hybrid does not take account of the edges or the required entities that might be present in the input surface mesh.
- The boundary of the solid mesh created will be strictly identical to the supporting surface mesh, except when the imprinting of boundary layer on symmetry planes is requested.

The surface mesh creation is not included in the generator but is to be provided by CAD-CAM software packages or other surface mesh generating tools, like for example the MeshGems CADSurf component.

\anchor hybrid_limitations
## Limitations
 Since Hybrid starts from a closed surface mesh to build a volume mesh, it cannot be guaranteed
 to deliver a satisfying resulting mesh when the input data is not compatible with
 the algorithms it implements. For example :

- When the input surface does not define a volume, is self-intersecting, is non-conforming,
is non-manifold, or has inconsistent normals (normals are said consistent if all the normals point outside or inside the domain to be meshed).
- When the input surface contains free edges or free faces.
- When the input surface mesh contains vertices too close to each others or to an edge or to a triangle (their distance is smaller than 1e-4 relatively to the geometry size).
- When triangles with bad aspect ratio (greater than 100) are present in the input.
- When some non-neighbour triangles are too close or present a too strong size discrepancy.


On the other hand, even with bad input data, the implemented algorithms are expected
 to either produce a result or to stop with a proper error message.


\anchor hybrid_dependencies
## Dependencies

Beside the OS libraries, The Hybrid component depends on the dynamic libraries from:
- Itself : mg-hybrid,
- the MeshGems core : meshgems, meshgems_stubs,
- the following MeshGems components : mg-tetra, mg-hexa, mg-cadsurf
- the Intel compiler runtimes (on Windows only) : mmd, ifcoremd, ifportmd.


## Hybrid integration

We describe below the basic steps to generate a mesh with Hybrid :

- a \ref meshgems_hybrid_session_t shall be created with \ref
meshgems_hybrid_session_new()

- It is then necessary to provide the input surface mesh
  of type \ref mesh with \ref meshgems_hybrid_set_surface_mesh().
- It is possible to drive precisely the mesh generation process in
Hybrid with some parameters through the \ref
meshgems_hybrid_set_param() function.  The list can be found in \ref
hybrid_parameters.
- The meshing process can then be started with
\ref meshgems_hybrid_compute_mesh().
- It is then possible to retrieve the generated output mesh of
  type \ref meshgems_mesh_t through the \ref meshgems_hybrid_get_mesh()
  function. Once the data embeded in this mesh has been retrieved, the
  \ref meshgems_hybrid_regain_mesh() function shall be called to give
  this mesh back to the Tetra session.
- Finally, the \ref meshgems_hybrid_session_t shall be destroyed with \ref
  meshgems_hybrid_session_delete().

## Hybrid data associativity

If no imprinting is requested, the surface entities of the \ref meshgems_mesh_t provided as input to Hybrid
are strictly preserved in the generated output \ref meshgems_mesh_t. As a
consequence, the associativity between the input and output data is straightforward.

\defgroup hybrid_parameters Hybrid parameters
\ingroup hybrid

Description of Hybrid parameters.

Hybrid parameters can be set in a given session with the \ref
meshgems_hybrid_set_param  function.

The array below gives an alphabetical list of all Hybrid parameters,
with their name, type, default value and a descritpion.  In this array
, the bool type is either "yes" or "no" (but "1", "true", "0", "false"
are also accepted).


<table>
<caption> List of Hybrid parameters </caption>
<tr>
  <th> Parameter name </th>
  <th> Type </th>
  <th> Default value </th>
  <th> Comment </th>
</tr>

<tr>
  <td> verbose </td>
  <td> integer </td>
  <td> 3 </td>
  <td> This parameter changes the verbosity level of Hybrid, between 0 and 10.
  The higher it is, the more messages  Hybrid will send
  through the message callback.</td>
</tr>

<tr>
  <td> add_multinormals </td>
  <td> bool </td>
  <td> no </td>
  <td>  If the value is \b yes then Hybrid will compute extra normals direction
  at concave ridges (and corners). </td>
</tr>

<tr>
  <td> boundary_layer_geometric_progression </td>
  <td> real </td>
  <td> 1.0 </td>
  <td>  This option allows the user to set the growth parameter used to compute the height of each layer.</td>
</tr>

<tr>
  <td> boundary_layer_global_initial_height </td>
  <td> real </td>
  <td>  0.1 * the size of the surface element </td>
  <td>  This option allows the user to set the height of the first layer when the
  \b boundary_layer_size_mode parameter is set to \b global.
  </td>

</tr>

<tr>
  <td> boundary_layer_max_element_angle </td>
  <td> real </td>
  <td>  165 </td>
  <td>  This option sets the maximum internal angles of elements (in degrees). This setting
        applies to the boundary layer elements only.   </td>

</tr>

<tr>
  <td> boundary_layer_size_mode </td>
  <td> string </td>
  <td>  global </td>
  <td>
  - If the value is \b global, then the \b boundary_layer_global_initial_height parameter is used to compute the layer heights.
  - If the value is \b local, a \b user-defined \b function is used to set the local size.
   </td>

</tr>

<tr>
  <td> boundary_layer_imprinting </td>
  <td> bool </td>
  <td>  no </td>
  <td>  If the value of this parameter is \b yes,
  this  activates the imprinting of the boundary layers on some referenced surface.
   </td>

</tr>

<tr>
  <td> boundary_layer_snapping </td>
  <td> bool </td>
  <td>  no </td>
  <td>  If the value of this parameter is \b yes,
  this activates the snapping of the boundary layers to some referenced surface.
   </td>

</tr>

<tr>
  <td> collision_mode </td>
  <td> string </td>
  <td>  stop </td>
  <td>
  - If the value is \b decrease, this keeps the number of desired layer but decreases the height of the layers to avoid collision
  - If the value is \b stop, this stops locally the layers generation; the number of generated layers may differ from the specified desired number
  </td>

</tr>

<tr>
  <td> element_generation </td>
  <td> string </td>
  <td>  tetra_dominant </td>
  <td>  If the value is:

- \b tetra_dominant : prismatic or hexahedral elements are generated in the boundary layers, with tetrahedra in the remaining volume.
- \b hexa_dominant : prismatic or hexahedral elements are generated in the boundary layers, with mixture of hexahedra and tetrahedra in the remaining volume.
- \b extrusion_only : only prismatic or hexahedral elements near the boundary are generated. The remaining volume is not filled.
- \b cartesian_core : cartesian core with a mixture of tetrahedra and pyramids
             in the remaining volume. The edge length of the generated hexahedra is given by the parameter \OptionNameSansglobal_physical_size.

  </td>

</tr>

<tr>
  <td> global_physical_size</td>
  <td> real </td>
  <td>  mean surface size </td>
  <td>  This parameter defines the constant global size used when a cartesian core is requested.
  </td>

</tr>

<tr>
  <td> gradation</td>
  <td> real </td>
  <td>  2 </td>
  <td>   Sets the desired maximum ratio between 2 adjacent edges. It applies only to the edges which belong to the tetrahedra. The closer it is to
``1``, the more uniform the mesh will be.
  </td>

</tr>

<tr>
  <td> max_number_of_threads</td>
  <td> integer </td>
  <td>  4 </td>
  <td>  This option allows hybrid to run in parallel using a multithread mode.
  This option specifies the number of threads to be used (this should be the number of cores or processors of a shared memory parallel architecture).
  This option might not be available on all platforms.

  </td>

</tr>

<tr>
  <td> multinormal_angle_threshold</td>
  <td> real </td>
  <td>  30 </td>
  <td>   Sets the maximum angle (in degrees) between the multiple normals at opening (concave) ridges.
  This parameter is used when the \b add_multinormals is set to \b yes.

  </td>

</tr>


<tr>
  <td> normal_direction</td>
  <td> integer </td>
  <td>  1 </td>
  <td>
  This option describes whether Hybrid should use the surface normals or the opposite of the surface normals.
	- If the value is \b 1, this means the layers grow in the same direction as the normals to the surface
	- If the value is \b -1, this means the layers grow in the inverse opposite of the normals to the surface

  Note that Hybrid assumes that the normals to the surface are oriented towards the inside of the domain.

  </td>

</tr>

<tr>
  <td> number_of_boundary_layers</td>
  <td> integer </td>
  <td>  0 </td>
  <td>  This option allows the user to set the number of boundary layers.
  </td>

</tr>

<tr>
  <td> smooth_normals</td>
  <td> bool </td>
  <td>  no </td>
  <td>  If the value is \b yes then Hybrid will smooth normals at closed (convex) ridges and corners.
  </td>

</tr>

</table>

*/

struct meshgems_hybrid_session_t_;

/**
 * Opaque type to store session data.
 */
typedef struct meshgems_hybrid_session_t_ meshgems_hybrid_session_t;
/*@}*/
/**
 * Get the meshgems hybrid major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hybrid_get_version_major(void);

/**
 * Get the meshgems hybrid minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hybrid_get_version_minor(void);

/**
 * Get the meshgems hybrid patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hybrid_get_version_patch(void);

/**
 * Get the meshgems hybrid version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_hybrid_hybrid_version_string(void);

/**
 * \addtogroup hybrid
 *@{*/

/** @name Constructor and Destructor
 */
/** @{*/
/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new hybrid_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_hybrid_session_t *meshgems_hybrid_session_new(
		meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in]	hxs	: the hybrid session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_hybrid_session_delete(
		meshgems_hybrid_session_t *hxs);

/**@}*/

/** @name Setting message and interrupt callbacks
 *
 */
/**@{*/
/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	hxs	: the hybrid session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_message_callback(
		meshgems_hybrid_session_t *hxs, meshgems_message_callback_t cb,
		void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	hxs	: the hybrid session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_interrupt_callback(
		meshgems_hybrid_session_t *hxs, meshgems_interrupt_callback_t cb,
		void *user_data);

/**@}*/

/** @name Setting the input data
 * The following functions are to be used to set the input data, which are:
 * - the input mesh,
 * - the parameters through the \ref meshgems_hybrid_set_param() function,
 * - the boundary layer size specifications; two methods are provided to specify the initial height of the boundary layers and the regions where the layers have to be built, a global method or a local method.
 * The computation of boundary layers depends on the chosen option, which is governed by the parameter boundary\_layer\_size\_mode.
 */
/**@{*/
/**
 * Sets required surface mesh and entities.
 *
 * Defines the skins (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	hxs	: the hybrid session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_surface_mesh(
		meshgems_hybrid_session_t *hxs, meshgems_mesh_t *m);


/**
 * Sets volume mesh and entities to be optimised.
 *
 * Defines the input volume mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	hxs	: the hybrid session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_volume_mesh(meshgems_hybrid_session_t *hys,
meshgems_mesh_t *m);

/**
 * Sets a sizemap.
 *
 * @param[in]	hxs	: the hybrid session;
 * @param[in]	s : the sizemap.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_sizemap(
		meshgems_hybrid_session_t *hxs, meshgems_sizemap_t *s);

/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li verbose level;
 * @li octree's subdivision level;
 * @li element's size;
 * @li sub-domain mode generation;
 * @li sharp angle threshold;
 * @li geometric approximation threshold;
 * @li ...
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	param_name : the option name whose value will be changed;
 * @param[in]	option_value : new option value.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_param(
		meshgems_hybrid_session_t *hxs, const char *param_name,
		const char *option_value);

/**
 * Sets boundary layers local parameters.
 *
 * This function allows setting:
 * @li a reference defining a part of the surface where boundary layers are requested;
 * @li the height of the first layer on this part;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	blref : a reference defining a part of the surface where boundary layers are requested;
 * @param[in]	height : the height of the first layer;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_initial_height_on_surface_tag(
		meshgems_hybrid_session_t *hxs, meshgems_integer blref,
		meshgems_real height);


/**
 * Sets boundary layer imprinting parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers have to be imprinted;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	iref : a reference defining a part of the surface where boundary layers are requested;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layer_imprinting_tag(
		meshgems_hybrid_session_t *hxs, meshgems_integer iref);

/**
 * Sets boundary layer snapping parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers have to be imprinted;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	iref : a reference defining a part of the surface where boundary layers are requested;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layer_snapping_tag(
		meshgems_hybrid_session_t *hxs, meshgems_integer iref);

/**@}*/

/** @name Generating or optimizing a mesh
 *
 */
/**@{*/
/**
 * Generate a mesh.
 *
 * @param[in]	hxs : the hybrid session. This session must have been
 *   given a surface mesh with hybrid_set_surface_mesh().
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_compute_mesh(
		meshgems_hybrid_session_t *hxs);


/**
 * optimize a mesh.
 *
 * @param[in]	hxs : the hybrid session. This session must have been
 *   given a volume mesh with hybrid_set_volume_mesh().
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_optimise_volume(meshgems_hybrid_session_t *hys);

/**@}*/

/** @name Getting the ouput data
 *
 */
/**@{*/
/**
 * Returns the mesh within current session.  Session cannot be
 * modified afterwards unless mesh is released.
 *
 * @param[in]	hxs : the hybrid session;
 * @param[out]	msh : the \ref mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_mesh(
		meshgems_hybrid_session_t *hxs, meshgems_mesh_t **msh);

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	hxs	: the hybrid session
 * @param[in]	msh	: the \ref mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_regain_mesh(
		meshgems_hybrid_session_t *hxs, meshgems_mesh_t *msh);

/**
 * Get the layer numbering of tetrahedron.
 *
 * @param[in]	hxs	: the hybrid session
 * @param[in]	ie	: the element index
 * @param[out]	lay	: the layer numbering of the ie tetrahedron,
 * 					  or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_tetrahedron_layer_numbering(
		meshgems_hybrid_session_t *hxs, integer ie, integer *lay);

/**
 * Get the surface tag of a boundary layer tetrahedron.
 *
 * @param[in]    hxs    : the hybrid session
 * @param[in]    ie    : the element index
 * @param[out]    tag    : the surface tag that is the source of the ie BL tetrahedron,
 *                       or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_tetrahedron_layer_surface_tag(
        meshgems_hybrid_session_t *hxs, integer ie, integer *tag);

/**
 * Get the layer numbering of pyramid.
 *
 * @param[in]	hxs	: the hybrid session
 * @param[in]	ie	: the element index
 * @param[out]	lay	: the layer numbering of the ie pyramid,
 * 					  or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_pyramid_layer_numbering(
		meshgems_hybrid_session_t *hxs, integer ie, integer *lay);

/**
 * Get the surface tag of a boundary layer pyramid.
 *
 * @param[in]    hxs    : the hybrid session
 * @param[in]    ie    : the element index
 * @param[out]    tag    : the surface tag that is the source of the ie BL pyramid,
 *                       or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_pyramid_layer_surface_tag(
        meshgems_hybrid_session_t *hxs, integer ie, integer *tag);

/**
 * Get the layer numbering of prism.
 *
 * @param[in]	hxs	: the hybrid session
 * @param[in]	ie	: the element index
 * @param[out]	lay	: the layer numbering of the ie tetrahedron,
 * 					  or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_prism_layer_numbering(
		meshgems_hybrid_session_t *hxs, integer ie, integer *lay);

/**
 * Get the surface tag of a boundary layer prism.
 *
 * @param[in]    hxs    : the hybrid session
 * @param[in]    ie    : the element index
 * @param[out]    tag    : the surface tag that is the source of the ie BL prism,
 *                       or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_prism_layer_surface_tag(
        meshgems_hybrid_session_t *hxs, integer ie, integer *tag);

/**
 * Get the layer numbering of hexahedron.
 *
 * @param[in]	hxs	: the hybrid session
 * @param[in]	ie	: the element index
 * @param[out]	lay	: the layer numbering of the ie tetrahedron,
 * 					  or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_hexahedron_layer_numbering(
		meshgems_hybrid_session_t *hxs, integer ie, integer *lay);

/**
 * Get the surface tag of a boundary layer hexaedron.
 *
 * @param[in]    hxs    : the hybrid session
 * @param[in]    ie    : the element index
 * @param[out]    tag    : the surface tag that is the source of the ie BL hexahedron,
 *                       or 0 if the element is not part of the boundary layer mesh
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_get_hexahedron_layer_surface_tag(
        meshgems_hybrid_session_t *hxs, integer ie, integer *tag);


/**
 * Get the number of boundary layer vertices
 *
 * @param[in]	hxs : the hybrid session;
 * @param[out]	nblv : the  number of boundary layer vertices
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_hybrid_get_boundary_layer_vertex_count(
		meshgems_hybrid_session_t *hxs, meshgems_integer *nblv);

/**
 * Get the number of boundary layer informations
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	i : the ith boundary layer vertex
 * @param[out]	blvertex_info :
 * 							blvertex_info[0] : the global index of the ith boundary layer vertex
 * 							blvertex_info[1] : the surface vertex which is the origin of ith boundary layer vertex
 * 							blvertex_info[2] : the layer numbering  of the ith boundary layer vertex
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_hybrid_get_boundary_layer_vertex_information(
		meshgems_hybrid_session_t *hxs, meshgems_integer i, meshgems_integer *blvertex_info);


/**@}*/

/*@}*/

/*@}*/

/*@}*/

/* Private and undocumented methods */

/**
 * Sets boundary layers imprinting surface.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers have to be imprinted;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	iref : a reference defining a part of the surface where boundary layers are requested;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layers_imprint_id(
		meshgems_hybrid_session_t *hxs, meshgems_integer iref);


/**
 * Sets boundary layer imprinting parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers have to be imprinted;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	iref : a reference defining a part of the surface where boundary layers are requested;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layers_imprint_tags(
		meshgems_hybrid_session_t *hxs, meshgems_integer iref);

/**
 * Sets boundary layers parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers are requested;
 * @li the direction of the boundary layers;
 * @li the number of layers;
 * @li the height of the first layer;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	blref : a reference defining a part of the surface where boundary layers are requested;
 * @param[in]	nbref : number of references
 * @param[in]	in_outward : the direction of the boundary layers;
 * @param[in]	height : the height of the first layer;
 * @param[in]	nblayer : the number of boundary layers ;
 * @param[in]	geometric_growth : the geometric progression between successive layers;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layers_parameter_onref(
		meshgems_hybrid_session_t *hxs, meshgems_integer blref,
		meshgems_integer in_outward, meshgems_real height,
		meshgems_integer nblayer, meshgems_real geometric_growth);

/**
 * Sets a boundary layers surface id.
 *
 * This function allows setting:
 * @li a references defining a part of the surface where boundary layers are requested;
 *
 * @param[in]	hxs : the hybrid session;
 * @param[in]	blref : a reference defining a part of the surface where boundary layers are requested;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layers_surface_id(
		meshgems_hybrid_session_t *hxs, meshgems_integer iref);

typedef status_t (*meshgems_boundary_layer_height_t)(real hn, integer n, real *hn1, void * user_data);

MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_set_boundary_layers_height(
		meshgems_hybrid_session_t *hxs, meshgems_boundary_layer_height_t f, void * u);

/**
 * Get the meshgems hybrid ident string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_hybrid_get_version_ident_string(void);

MESHGEMS_METHOD meshgems_status_t meshgems_hybrid_session_set_banner(
		meshgems_hybrid_session_t *hxs, meshgems_integer type);

MESHGEMS_METHOD meshgems_hybrid_session_t * __sub_proccess_hybrid_session_new(
		meshgems_context_t *ctx);



#endif
