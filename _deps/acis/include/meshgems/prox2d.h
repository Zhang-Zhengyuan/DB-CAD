#ifndef __PROX2D_H__
#define __PROX2D_H__

#include <meshgems/meshgems.h>

struct meshgems_proximity2D_session_t_;

/**
 * \typedef proximity2D_session_t
 * Opaque type to store session data.
 */
typedef struct meshgems_proximity2D_session_t_ meshgems_proximity2D_session_t;

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new proximity2D_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_proximity2D_session_t *meshgems_proximity2D_session_new(meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in]	prs	: the prox2D session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_proximity2D_session_delete(meshgems_proximity2D_session_t *prs);

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	prs	: the prox2D session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_set_message_callback(meshgems_proximity2D_session_t *prs,
									    meshgems_message_callback_t cb,
									    void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	prs	: the prox2D session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_set_interrupt_callback(meshgems_proximity2D_session_t *prs,
									      meshgems_interrupt_callback_t cb,
									      void *user_data);

/**
 * Sets required surface mesh and entities.
 *
 * Defines the skins (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	prs	: the prox2D session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_set_surface_mesh(meshgems_proximity2D_session_t *prs, meshgems_mesh_t *m);

/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li debug mode,
 * @li verbosity level,
 * @li growth value (gradation/propagation),
 * @li ...
 *
 * @param[in]	prs	: the prox2D session.
 * @param[in]	option_name	: the option name whose value will be changed
 * @param[in]	option_value	: new option value
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_set_param(meshgems_proximity2D_session_t *prs,
								 const char *param_name,
								 const char *param_value);

/**
 * Compute proximity2D.
 *
 * @param[in]	prs	: the prox2D session. This session must have been
 *   given a surface mesh with proximity2D_set_surface_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_compute(meshgems_proximity2D_session_t *prs);

/**
 * Gets the proximity2D isotropic size at a mesh vertex
 * @param[in]	prs	: the prox2D session
 * @param[in]	ivtx	: index of the desired vertex from 1 to nbvtx
 * @param[out]	size	: size of the vertex
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_get_vertex_size(meshgems_proximity2D_session_t *prs, meshgems_integer ivtx, meshgems_real *size);
MESHGEMS_METHOD meshgems_status_t meshgems_proximity2D_get_vertex_metric(meshgems_proximity2D_session_t *prs, meshgems_integer ivtx, meshgems_real *met);

/* status_t RANGE from +-99000000 to +-99999999 reserved for Proximity2D (see status.h) */

#ifndef MESHGEMS_CODE_PROXIMITY_BASE 
#define MESHGEMS_CODE_PROXIMITY_BASE   99000000
#endif

#define MESHGEMS_STATUS_PROXIMITY_ABS_START MESHGEMS_CODE_PROXIMITY_BASE
#define MESHGEMS_STATUS_PROXIMITY_ABS_END MESHGEMS_STATUS_PROXIMITY_ABS_START+999999

#define MESHGEMS_PROXIMITY_ERRCODE(x) (-(MESHGEMS_STATUS_PROXIMITY_ABS_START+(x)))
#define MESHGEMS_PROXIMITY_WARCODE(x) (MESHGEMS_STATUS_PROXIMITY_ABS_START+(x))

#endif
