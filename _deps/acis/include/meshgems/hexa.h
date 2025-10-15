#ifndef __MESHGEMS_HEXA_H__
#define __MESHGEMS_HEXA_H__

#include <meshgems/meshgems.h>
#include <meshgems/hexa_short_names.h>
#include <meshgems/hexa_old_names.h>

/**

 \page sessions Sessions

 \htmlonly <hr width=50%> \endhtmlonly

 \section session_hexa Hexa Session

 An hexa session object (of type hexa_session) is an object describing an Hexa run.
 It makes the link between the input mesh and parameters, the meshing process and the resulting mesh recovery.

 */

/*
 * \defgroup hexa Hexa Session
 *@{*/

struct meshgems_hexa_session_t_;

/**
 * Opaque type to store session data.
 */
typedef struct meshgems_hexa_session_t_ meshgems_hexa_session_t;

/**
 * Get the meshgems hexa major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hexa_get_version_major(void);

/**
 * Get the meshgems hexa minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hexa_get_version_minor(void);

/**
 * Get the meshgems hexa patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_hexa_get_version_patch(void);

/**
 * Get the meshgems hexa version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_hexa_get_version_string(void);


/**
 * \weakgroup hexa Module HEXA
 *@{*/

/**
 * \defgroup hexa_init_del Hexa Session Constructor and Destructor
 * \ingroup hexa
 *@{*/

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new hexa_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_hexa_session_t *meshgems_hexa_session_new(
		meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in]	hxs	: the hexa session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_hexa_session_delete(meshgems_hexa_session_t *hxs);

/*@}*/

/**
 * \defgroup hexa_callbacks Hexa Session Callbacks
 * \ingroup hexa
 *@{*/

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	hxs	: the hexa session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_message_callback(
		meshgems_hexa_session_t *hxs, meshgems_message_callback_t cb,
		void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	hxs	: the hexa session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_interrupt_callback(
		meshgems_hexa_session_t *hxs, meshgems_interrupt_callback_t cb,
		void *user_data);

/*@}*/

/**
 * \defgroup hexa_input_mesh Hexa Session Input Parameters
 * \ingroup hexa
 *@{*/

/**
 * Sets required surface mesh and entities.
 *
 * Defines the skins (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	hxs	: the hexa session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_surface_mesh(
		meshgems_hexa_session_t *hxs, meshgems_mesh_t *m);

/**
 * Sets a sizemap.
 *
 * @param[in]	hxs	: the hexa session;
 * @param[in]	s : the sizemap of type meshgems_sizemap_type_iso_mesh_vertex
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_sizemap(
		meshgems_hexa_session_t *hxs, meshgems_sizemap_t *s);

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
 * @param[in]	hxs : the hexa session;
 * @param[in]	param_name : the option name whose value will be changed;
 * @param[in]	option_value : new option value.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_param(
		meshgems_hexa_session_t *hxs, const char *param_name,
		const char *option_value);

/**
 * Sets global boundary layer parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface where boundary layers are requested;
 * @li the direction of the boundary layers;
 * @li the number of layers;
 * @li the height of the first layer;
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	blref : the references defining the parts of the surface where boundary layers are requested;
 * @param[in]	nbref : number of references
 * @param[in]	in_outward : the direction of the boundary layers;
 * @param[in]	height : the height of the first layer;
 * @param[in]	nblayer : the number of boundary layers ;
 * @param[in]	geometric_growth : the geometric progression between successive layers;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_boundary_layers_parameters(
		meshgems_hexa_session_t *hxs, meshgems_integer * blref,
		meshgems_integer nbref, meshgems_integer in_outward,
		meshgems_real height, meshgems_integer nblayer,
		meshgems_real geometric_growth);

/**
 * Sets local boundary layer parameters of a part of the surface.
 *
 * This function allows setting:
 * @li the reference defining a part of the surface where boundary layers are requested;
 * @li the direction of the boundary layers that have to be built on this part;
 * @li the height of the first layer;
 * @li the number of layers hat have to be built on this part;
 * @li the geometric progression between successive layers;
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	blref : the reference defining the parts of the surface where boundary layers are requested;
 * @param[in]	in_outward : the direction of the boundary layers;
 * @param[in]	height : the height of the first layer;
 * @param[in]	nblayer : the number of boundary layers ;
 * @param[in]	geometric_growth : the geometric progression between successive layers;
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_boundary_layer_parameters_on_tag( meshgems_hexa_session_t *hxs,
		meshgems_integer blref,
		meshgems_integer in_outward,
		meshgems_real height,
		meshgems_integer nblayer,
		meshgems_real geometric_growth);

/**
 * Sets boundary layer imprint parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface that can be imprinted by boundary layers
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	impref : the references defining the parts of the surface that can be imprinted;
 * @param[in]	nbimpref : number of references
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_set_imprinted_surface_ids(
		meshgems_hexa_session_t *hxs, meshgems_integer * impref,
		meshgems_integer nbimpref);

/**
 * Sets local level parameters.
 *
 * This function allows setting:
 * @li a list of references defining the parts of the surface on which the octree will be refined up to the defined level
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	impref : the references defining the parts of the surface ;
 * @param[in]	nbref : number of references
 * @param[in]	refl : the level of the octree used to refine around the parts of the given surface
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_hexa_set_local_level_parameters(meshgems_hexa_session_t *tms, meshgems_integer * refltab,
		meshgems_integer nbref, meshgems_integer refl);

/*@}*/

/**
 * \defgroup hexa_gene_mesh Hexa Session Actions
 * \ingroup hexa
 *@{*/

/**
 * Generate a mesh.
 *
 * @param[in]	hxs : the hexa session. This session must have been
 *   given a surface mesh with hexa_set_surface_mesh().
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_compute_volume_mesh(
		meshgems_hexa_session_t *hxs);

/*@}*/

/**
 * \defgroup hexa_output_mesh Hexa Session Mesh Access
 * \ingroup hexa
 *@{*/

/**
 * Returns the mesh within current session.  Session cannot be
 * modified afterwards unless mesh is released.
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	msh : the \ref mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_mesh(
		meshgems_hexa_session_t *hxs, meshgems_mesh_t **msh);


/**
 * Get the number of boundary layer vertices
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	nblv : the  number of boundary layer vertices
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_boundary_layer_vertex_count(
		meshgems_hexa_session_t *hxs, meshgems_integer *nblv);

/**
 * Get the number of boundary layer informations
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	lay_info :
 * 							lay_info[0] : the global index of the ith boundary layer vertex
 * 							lay_info[1] : the surface vertex which is the origin of ith boundary layer vertex
 * 							lay_info[2] : the layer numbering  of the ith boundary layer vertex
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_boundary_layer_vertex_information(
		meshgems_hexa_session_t *hxs, meshgems_integer i, meshgems_integer *blvertex_info);

/**
 * Get the number of under-refined hexahedra.
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	nbhexa : the  under-refined hexahedra count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_coarse_hexahedron_count(
		meshgems_hexa_session_t *hxs, meshgems_integer *nbhexa);

/**
 * Get the list of lost hexahedra.
 *
 * @param[in]	hxs	: the hexa session;
 * @param[out]	hexalist : the under-refined hexahedra list.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_coarse_hexahedron_list(
		meshgems_hexa_session_t *hxs, meshgems_integer *hexalist);
/*@}*/

/**
 * \defgroup hexa_mapping_mesh Hexa Session Mapping Informations Access
 * \ingroup hexa
 * These functions give acces to the dual-way surface mapping information between the triangular surface input mesh and the quadrilateral surface output mesh.
 * For each of these vertices, the same information are given:
 * @li the vertex number
 * @li the triangle (resp. quadrangle) it is projected on
 * @li the barycentric coordinates of the vertex normal projection on the triangle (resp.quadrangle)
 * @li the distance between the output(resp.input) vertex and the input (resp.output) triangle (resp. quadrangle)
 *
 *@{*/

/**
 * Get the number of output vertices on input triangles
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	nbnew2old : the number of vertices.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_vertices_on_geometric_triangles_count(
		meshgems_hexa_session_t *hxs, meshgems_integer *nbnew2old);

/**
 * Get the mapping information Output -> Input
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	index : index of a vertex on geometric triangles
 * @param[out]	id : the index of the vertex in the output mesh_t
 * @param[out]	idt : the index of the input triangle it is projected on.
 * @param[out]  coord_bary : the barycentric coordinates
 * @param[out]	dis : the distance to the triangle
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_vertices_on_geometric_triangles_info(
		meshgems_hexa_session_t *hxs, meshgems_integer index,
		meshgems_integer *id, meshgems_integer *idt, meshgems_real *coord_bary,
		meshgems_real *dis);

/**
 * Get the number of input vertices on output quadrilaterals
 *
 * @param[in]	hxs : the hexa session;
 * @param[out]	nbnew2old : the number of vertices.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_vertices_on_geometric_quadrilaterals_count(
		meshgems_hexa_session_t *hxs, meshgems_integer *nbnew2old);

/**
 * Get the mapping information Input -> Output
 *
 * @param[in]	hxs : the hexa session;
 * @param[in]	index : index of a vertex on geometric quadrilaterals
 * @param[out]	id : the index of the vertex in the input mesh_t
 * @param[out]	idq : the index of the output quadrilateral it is projected on.
 * @param[out]  coord_bary : the barycentric coordinates
 * @param[out]	dis : the distance to the quadrilateral
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_get_vertices_on_geometric_quadrilaterals_info(
		meshgems_hexa_session_t *hxs, meshgems_integer index,
		meshgems_integer *id, meshgems_integer *idq, meshgems_real *coord_bary,
		meshgems_real *dis);

/*@}*/

/*@}*/

/*@}*/

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	hxs	: the hexa session
 * @param[in]	msh	: the \ref mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_hexa_regain_mesh(
		meshgems_hexa_session_t *hxs, meshgems_mesh_t *msh);

/* Private and undocumented methods */

MESHGEMS_METHOD meshgems_status_t meshgems_hexa_compute_mesh(
		meshgems_hexa_session_t *hxs);
/**
 * Get the meshgems hexa ident string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_hexa_get_version_ident_string(void);

MESHGEMS_METHOD meshgems_status_t meshgems_hexa_session_set_banner(meshgems_hexa_session_t *hxs, meshgems_integer type);

MESHGEMS_METHOD void meshgems_hexa_io_count_bytes(meshgems_hexa_session_t *tms, char * filename);

MESHGEMS_METHOD void meshgems_hexa_io_tictac(meshgems_hexa_session_t *tms);

MESHGEMS_METHOD status_t meshgems_hexa_set_anisotropic_layer_sizes_alongZ(meshgems_hexa_session_t *tms,
		meshgems_real xyzmin[3], meshgems_real xyzmax[3],
		meshgems_real minsiz,  meshgems_real maxsiz, meshgems_real laysiz);

#endif
