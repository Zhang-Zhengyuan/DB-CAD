#ifndef __MESHGEMS_ADAPT_H__
#define __MESHGEMS_ADAPT_H__

#include <meshgems/meshgems.h>
#include <meshgems/adapt_short_names.h>


/**

\page sessions Sessions

\htmlonly <hr width=50%> \endhtmlonly

\section session_adapt Adapt Session
*/

/*
 * \defgroup adapt Adapt
 *@{*/

struct meshgems_adapt_session_t_;

/**
 * \typedef adapt_session_t
 * Opaque type to store session data.
 */
typedef struct meshgems_adapt_session_t_ meshgems_adapt_session_t;

/* (----- meshgems_separator -----) */

/**
 * Get the meshgems adapt major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_adapt_get_version_major(void);

/**
 * Get the meshgems adapt minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_adapt_get_version_minor(void);

/**
 * Get the meshgems adapt patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_adapt_get_version_patch(void);

/**
 * Get the meshgems adapt version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_adapt_get_version_string(void);

/* (----- meshgems_separator -----) */

/**
 * \defgroup adapt_init_del Session Constructor and Destructor
 * \ingroup adapt
 *@{*/

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new adapt_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_adapt_session_t
    *meshgems_adapt_session_new(meshgems_context_t * ctx);

/**
 * Destructor.
 *
 * @param[in]	prs	: the adapt session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void
meshgems_adapt_session_delete(meshgems_adapt_session_t * prs);

/* (----- meshgems_separator -----) */

/*@}*/

/**
 * \defgroup adapt_callbacks Session Callbacks
 * \ingroup adapt
 *@{*/

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	prs	: the adapt session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_set_message_callback(meshgems_adapt_session_t * prs,
				    meshgems_message_callback_t cb,
				    void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	prs	: the adapt session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_set_interrupt_callback(meshgems_adapt_session_t * prs,
				      meshgems_interrupt_callback_t cb,
				      void *user_data);

/* (----- meshgems_separator -----) */

/*@}*/

/**
 * Sets required surface mesh and entities.
 *
 * Defines the skins (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	prs	: the adapt session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_set_surface_mesh(meshgems_adapt_session_t * prs,
				meshgems_mesh_t * m);



/**
 * Sets sizemap.
 *
 * Defines the sizemap.
 *
 * @param[in]	prs	: the adapt session.
 * @param[in]	s	: the \ref sizemap_t object holding the sizemap.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_set_sizemap(meshgems_adapt_session_t * prs,
                             meshgems_sizemap_t * s);



/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li debug mode,
 * @li verbosity level,
 * @li growth value (gradation/propagation),
 * @li ...
 *
 * @param[in]	prs	: the adapt session.
 * @param[in]	option_name	: the option name whose value will be changed
 * @param[in]	option_value	: new option value
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_set_param(meshgems_adapt_session_t * prs,
                           const char *param_name,
                           const char *param_value);

/* (----- meshgems_separator -----) */

/**
 * Compute adapt.
 *
 * @param[in]	prs	: the adapt session. This session must have been
 *   given a surface mesh with adapt_set_surface_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_compute(meshgems_adapt_session_t * prs);

/**
 * Returns the current resulting mesh from session.  Session cannot be modified
 * afterwards unless mesh is released (See meshgems_adapt_regain_mesh below).
 *
 * @param[in]	ses	: the adapt session
 * @param[out]	msh	: the \ref mesh_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_get_mesh(meshgems_adapt_session_t * ses,
                          meshgems_mesh_t ** msh);

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	ses	: the adapt session
 * @param[in]	msh	: the \ref mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_regain_mesh(meshgems_adapt_session_t * ses,
                             meshgems_mesh_t * msh);

/**
 * Returns the current resulting sizemap from session.  Session cannot be modified
 * afterwards unless sizemap is released (See meshgems_adapt_regain_sizemap below).
 *
 * @param[in]	ses	: the adapt session
 * @param[out]	siz	: the \ref sizemap_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_get_sizemap(meshgems_adapt_session_t * ses,
                             meshgems_sizemap_t ** siz);

/**
 * Gives the sizemap control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	ses	: the adapt session
 * @param[in]	siz	: the \ref sizemap_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_regain_sizemap(meshgems_adapt_session_t * ses,
                                meshgems_sizemap_t * siz);

/**
  * Get the index of the tetrehedron in the background mesh which
  * contains the given vertex (it is called a seed)
  *
  * @param[in]	ses	: the adapt session
  * @param[in]	ivtx	: the vertex number
  * @param[out]	seed	: the index of the background tetrahedra seeding the vertex (integer)
  *
  * @retval STATUS_OK or another STATUS_* in case of error 
  *
  */
MESHGEMS_METHOD meshgems_status_t 
meshgems_adapt_get_vertex_seed(meshgems_adapt_session_t * ses,
				  meshgems_integer ivtx,
				  meshgems_integer *seed);


/* (----- meshgems_separator -----) */

/* Private and undocumented methods */

MESHGEMS_METHOD const char
    *meshgems_adapt_get_version_ident_string(void);
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_session_set_banner(meshgems_adapt_session_t * prs,
                                    meshgems_integer type);
MESHGEMS_METHOD meshgems_status_t
meshgems_adapt_session_get_banner(meshgems_adapt_session_t * prs,
                                    meshgems_integer * type);

MESHGEMS_METHOD meshgems_integer meshgems_adapt_get_EXE_status(meshgems_adapt_session_t * prs);
MESHGEMS_METHOD meshgems_real meshgems_adapt_get_double_realtime(void);

#endif                          /* __MESHGEMS_ADAPT_H__ */
