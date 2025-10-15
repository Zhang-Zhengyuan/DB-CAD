#ifndef __MESHGEMS_SURFOPT_H__
#define __MESHGEMS_SURFOPT_H__

#include <meshgems/meshgems.h>
#include <meshgems/surfopt_short_names.h>

/**

\page sessions Sessions

\htmlonly <hr width=50%> \endhtmlonly

\section session_surfopt SurfOpt Session
*/

/*
 * \defgroup surfopt SurfOpt
 *@{*/

struct meshgems_surfopt_session_t_;

/**
 * \typedef surfopt_session_t
 * Opaque type to store session data.
 */
typedef struct meshgems_surfopt_session_t_ meshgems_surfopt_session_t;

/* (----- meshgems_separator -----) */

/**
 * Get the meshgems surfopt major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_surfopt_get_version_major(void);

/**
 * Get the meshgems surfopt minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_surfopt_get_version_minor(void);

/**
 * Get the meshgems surfopt patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_surfopt_get_version_patch(void);

/**
 * Get the meshgems surfopt version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_surfopt_get_version_string(void);

/* (----- meshgems_separator -----) */

/**
 * \defgroup surfopt_init_del Session Constructor and Destructor
 * \ingroup surfopt
 *@{*/

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new surfopt_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_surfopt_session_t
    *meshgems_surfopt_session_new(meshgems_context_t * ctx);

/**
 * Destructor.
 *
 * @param[in]	prs	: the surfopt session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void
meshgems_surfopt_session_delete(meshgems_surfopt_session_t * prs);

/* (----- meshgems_separator -----) */

/*@}*/

/**
 * \defgroup surfopt_callbacks Session Callbacks
 * \ingroup surfopt
 *@{*/

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	prs	: the surfopt session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_set_message_callback(meshgems_surfopt_session_t * prs,
                                      meshgems_message_callback_t cb,
                                      void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	prs	: the surfopt session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_set_interrupt_callback(meshgems_surfopt_session_t * prs,
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
 * @param[in]	prs	: the surfopt session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_set_surface_mesh(meshgems_surfopt_session_t * prs,
                                  meshgems_mesh_t * m);



/**
 * Sets sizemap.
 *
 * Defines the sizemap.
 *
 * @param[in]	prs	: the surfopt session.
 * @param[in]	s	: the \ref sizemap_t object holding the sizemap.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_set_sizemap(meshgems_surfopt_session_t * prs,
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
 * @param[in]	prs	: the surfopt session.
 * @param[in]	option_name	: the option name whose value will be changed
 * @param[in]	option_value	: new option value
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_set_param(meshgems_surfopt_session_t * prs,
                           const char *param_name,
                           const char *param_value);

/* (----- meshgems_separator -----) */

/**
 * Compute surfopt.
 *
 * @param[in]	prs	: the surfopt session. This session must have been
 *   given a surface mesh with surfopt_set_surface_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_compute(meshgems_surfopt_session_t * prs);

/**
 * Returns the current resulting mesh from session.  Session cannot be modified
 * afterwards unless mesh is released (See meshgems_surfopt_regain_mesh below).
 *
 * @param[in]	ses	: the surfopt session
 * @param[out]	msh	: the \ref mesh_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_get_mesh(meshgems_surfopt_session_t * ses,
                          meshgems_mesh_t ** msh);

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	ses	: the surfopt session
 * @param[in]	msh	: the \ref mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_regain_mesh(meshgems_surfopt_session_t * ses,
                             meshgems_mesh_t * msh);

/**
 * Returns the current resulting sizemap from session.  Session cannot be modified
 * afterwards unless sizemap is released (See meshgems_surfopt_regain_sizemap below).
 *
 * @param[in]	ses	: the surfopt session
 * @param[out]	siz	: the \ref sizemap_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_get_sizemap(meshgems_surfopt_session_t * ses,
                             meshgems_sizemap_t ** siz);

/**
 * Gives the sizemap control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	ses	: the surfopt session
 * @param[in]	siz	: the \ref sizemap_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_regain_sizemap(meshgems_surfopt_session_t * ses,
                                meshgems_sizemap_t * siz);

/* (----- meshgems_separator -----) */

/* Private and undocumented methods */

MESHGEMS_METHOD const char
    *meshgems_surfopt_get_version_ident_string(void);
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_session_set_banner(meshgems_surfopt_session_t * prs,
                                    meshgems_integer type);
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_session_get_banner(meshgems_surfopt_session_t * prs,
                                    meshgems_integer * type);

MESHGEMS_METHOD integer meshgems_surfopt_get_EXE_status();
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_write_extra_data(meshgems_surfopt_session_t * ses,
                                  const char *extra_data_name,
                                  const char *filename);
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_get_proximity_sizemap(meshgems_surfopt_session_t * ses,
				       meshgems_sizemap_t ** siz);
MESHGEMS_METHOD meshgems_status_t
meshgems_surfopt_get_proximity_sizemap_aniso(meshgems_surfopt_session_t * ses,
					     meshgems_sizemap_t ** siz);

#endif                          /* __MESHGEMS_SURFOPT_H__ */
