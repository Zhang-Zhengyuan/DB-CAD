#ifndef __HIGHORDER_H__
#define __HIGHORDER_H__

#include <meshgems/meshgems.h>

struct meshgems_highorder_session_t_t;

/**
 * Opaque type to store session data.
 */
typedef struct meshgems_highorder_session_t_t meshgems_highorder_session_t;

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new meshgems_highorder_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_highorder_session_t *meshgems_highorder_session_new(meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in]	hos	: the highorder session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_highorder_session_delete(meshgems_highorder_session_t *hos);

/**
 * Sets input volume mesh and entities.
 *
 * Defines the volume (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	hos	: the highorder session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_highorder_set_volume_mesh(meshgems_highorder_session_t *hos, meshgems_mesh_t *m);

/**
 * Sets input pk surface mesh and entities.
 *
 * Defines the surface of a high order mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	hos	: the highorder session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_highorder_set_surface_mesh(meshgems_highorder_session_t *hos,
		mesh_t *m);

/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li debug mode,
 * @li verbosity level,
 * @li ...
 *
 * @param[in]	hos	: the highorder session.
 * @param[in]	option_name	: the option name whose value will be changed
 * @param[in]	option_value	: new option value
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_highorder_set_param(meshgems_highorder_session_t *hos,
	const char *param_name, const char *param_value) ;


MESHGEMS_METHOD status_t meshgems_highorder_set_boundary_layer_vertex_count(meshgems_highorder_session_t *hos, integer nblvc);
MESHGEMS_METHOD status_t meshgems_highorder_set_boundary_layer_vertex_info(meshgems_highorder_session_t *hos, integer i,integer *blvertex_info);

/**
 * Compute a highorder mesh from a P1 volume mesh.
 *
 * @param[in]	hos	: the highorder session. This session must have been
 *   given a volume mesh with meshgems_highorder_set_volume_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_highorder_compute_mesh(meshgems_highorder_session_t *hos);

/**
 * Returns the mesh within current session.  Session cannot be
 * modified afterwards unless mesh is released.
 *
 * @param[in]	hos : the highorder session;
 * @param[out]	msh : the \ref mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t meshgems_highorder_get_mesh(meshgems_highorder_session_t *hos,meshgems_mesh_t **msh);

#endif
