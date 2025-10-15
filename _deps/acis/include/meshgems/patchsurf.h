#ifndef __MESHGEMS_PATCHSURF_H__
#define __MESHGEMS_PATCHSURF_H__

#include <meshgems/meshgems.h>
#include <meshgems/patchsurf_short_names.h>

/**
 * Opaque type to store patchsurf data.
 */
struct meshgems_patchsurf_session_t_;
typedef struct meshgems_patchsurf_session_t_ meshgems_patchsurf_session_t;


/**
 * Simple constructor.
 * @param ctx : the user context this session will be attached.
 * @return a new patchsurf_session or NULL
 */
MESHGEMS_METHOD meshgems_patchsurf_session_t *meshgems_patchsurf_session_new(meshgems_context_t *ctx);


/**
 * Destructor. Frees all data allocated by this session.
 */
MESHGEMS_METHOD void meshgems_patchsurf_session_delete(meshgems_patchsurf_session_t *pss);



/**
 * Set a parameter.
 * @param pss (in) : the working patchsurf session.
 * @param option_name (in) : the option name whose value will be changed
 * @param option_value (in) : new option value
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_set_param(meshgems_patchsurf_session_t *pss,
							 const char *option_name,
							 const char *option_value);


/**
 * Allocate a buffer suited to handle any request on any elementary data in a PatchSurf session.
 * @param[in] pss	: the working patchsurf session.
 * @return a pointer
 */
MESHGEMS_METHOD void *meshgems_patchsurf_calloc_generic_buffer(meshgems_patchsurf_session_t *pss);



/**
 * Free a buffer allocated by meshgems_patchsurf_calloc_generic_buffer.
 * @param[in] v	: the working buffer.
 * @return a pointer
 */
MESHGEMS_METHOD void meshgems_patchsurf_free_generic_buffer(void *v);


/**
 * Get a parameter.
 * @param pss (in) : the working patchsurf session.
 * @param option_name (in) : the option name whose value will be returned
 * @param option_value (out) : the option value
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_get_param(meshgems_patchsurf_session_t *pss,
							 const char *option_name,
							 char *option_value);


/**
 * Set the message callback function.
 * @param pss (in) : the working patchsurf session.
 * @param cb (in) : the message callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_set_message_callback(meshgems_patchsurf_session_t *pss,
								    meshgems_message_callback_t cb,
								    void *user_data);


/**
 * Set the interrupt callback function.
 * @param pss (in) : the working patchsurf session.
 * @param cb (in) : the interrupt callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_set_interrupt_callback(meshgems_patchsurf_session_t *pss,
								      meshgems_interrupt_callback_t cb,
								      void *user_data);

/**
 * Set the boundary mesh data.
 * @param pss (in) : the working patchsurf session.
 * @param msh (in) : the mesh data (made of vertices and edges)
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_set_boundary_mesh(meshgems_patchsurf_session_t *pss, meshgems_mesh_t *msh);


/**
 * Specify as internal the given mesh edge.
 * @param pss (in) : the working patchsurf session.
 * @param ie (in) : the index of the boundary edge (in the input mesh) to be set as internal
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_boundary_edge_is_internal(meshgems_patchsurf_session_t *pss, meshgems_integer ie);

/**
 * Set the patch parameterisation.
 * @param pss (in) : the working patchsurf session.
 * @param surf (in) : the parametric definition of this surface (see meshgems/cad.h)
 * @param user_data (in) : a generic user pointer
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_set_parameterisation(meshgems_patchsurf_session_t *pss, meshgems_cad_surf_t surf, void *user_data);


/**
 * Compute a mesh with current session configuration and data.
 * @param pss (in) : the working patchsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_compute_mesh(meshgems_patchsurf_session_t *pss);


/**
 * Returns the mesh within current session.
 * @param pss (in) : the working patchsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_get_mesh(meshgems_patchsurf_session_t *pss, meshgems_mesh_t **msh);


/**
 * Gives the output mesh back to the current session.
 * @param pss (in) : the working patchsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_patchsurf_regain_mesh(meshgems_patchsurf_session_t *pss, meshgems_mesh_t *msh);


/* End of cache data API */



/** Private part of the patch mesher interface. Only MESHGEMS products should use it. **/

#endif
