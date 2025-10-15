#ifndef __MESHGEMS_CLEANER_H__
#define __MESHGEMS_CLEANER_H__

#include "meshgems/meshgems.h"
#include "meshgems/cleaner_short_names.h"

/**

 \page sessions Sessions

 \htmlonly <hr width=50%> \endhtmlonly

 \section session_cleaner Cleaner Session

 A cleaner session object (of type cleaner_session) is an object describing a Cleaner run.
 It makes the link between the input mesh and parameters, the meshing process and the resulting mesh recovery.

 */

#define cleaner_session_t_ meshgems_cleaner_session_t_

struct meshgems_cleaner_session_t_;
/**
 * \typedef meshgems_cleaner_session_t
 * Opaque type to store session data.
 */
typedef struct meshgems_cleaner_session_t_ meshgems_cleaner_session_t;


/**
 * Get the meshgems cleaner major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_cleaner_get_version_major(void);

/**
 * Get the meshgems cleaner minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_cleaner_get_version_minor(void);

/**
 * Get the meshgems cleaner patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_cleaner_get_version_patch(void);

/**
 * Get the meshgems cleaner version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_cleaner_get_version_string(void);


/**
 * \weakgroup cleaner Module CLEANER
 *@{*/

/**
 * \defgroup cleaner_init_del Cleaner Session Constructor and Destructor
 * \ingroup cleaner
 *@{*/

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new cleaner_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_cleaner_session_t
    *meshgems_cleaner_session_new(meshgems_context_t * ctx);

/**
 * Destructor.
 *
 * @param[in]	this_ses	: the cleaner session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void
meshgems_cleaner_session_delete(meshgems_cleaner_session_t * this_ses);

/*@}*/

/**
 * \defgroup cleaner_callbacks Cleaner Session Callbacks
 * \ingroup cleaner
 *@{*/

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	this_ses        : the cleaner session.
 * @param[in]	cb         	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 *  @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_set_message_callback(meshgems_cleaner_session_t *
                                      this_ses, message_callback_t cb,
                                      void *user_data);



/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to
 * end the ongoing session call.
 *
 * @param[in]	this_ses	: the cleaner session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_set_interrupt_callback(meshgems_cleaner_session_t *
                                        this_ses, interrupt_callback_t cb,
                                        void *user_data);
/*@}*/

/**
 * \defgroup cleaner_input_mesh Cleaner Session Input Parameters
 * \ingroup cleaner
 *@{*/

/**
 * Sets required surface mesh and entities.
 *
 * Defines the skins (external and internal) of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	this_ses	: the cleaner session.
 * @param[in]	m	: the mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_set_surface_mesh(meshgems_cleaner_session_t * this_ses,
                                  mesh_t * m);


/**
 * Sets the sizemap to be maintained which allows the size map
 * to follow through in a workflow with MG-Cleaner.
 * This sizemap must have as support mesh the mesh provided to the set_surface_mesh.
 *
 * @param[in]	this_ses	: the cleaner session.
 * @param[in]	s	: the \ref sizemap_t object holding the sizemap.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_set_sizemap(meshgems_cleaner_session_t * this_ses,
			     meshgems_sizemap_t * s);

/**
 *  Gets the maintained sizemap.
 *
 * @param[in] this_ses	: the cleaner session.
 * @param[out] s  : the output sizemap.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_get_sizemap(meshgems_cleaner_session_t * this_ses,
			     meshgems_sizemap_t ** s);



/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li verbose level;
 * @li checking the mesh;
 * @li fixing the mesh;
 * @li Preserving topology;
 * @li ...
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[in]	param_name : the option name whose value will be changed;
 * @param[in]	param_value : new option value.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_set_param(meshgems_cleaner_session_t * this_ses,
                           const char *param_name,
                           const char *param_value);

/*@}*/

/**
 * \defgroup cleaner_XXX Cleaner Session Actions
 * \ingroup cleaner
 *@{*/

/**
 * Generate a mesh.
 *
 * @param[in]	this_ses : the cleaner session. This session must have been
 *   given a surface mesh with cleaner_set_surface_mesh().
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_fix(meshgems_cleaner_session_t * this_ses);

/**
 * Generate a diagnostic of a mesh.
 *
 * @param[in]	this_ses : the cleaner session. This session must have been
 *   given a surface mesh with cleaner_set_surface_mesh().
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_check(meshgems_cleaner_session_t * this_ses);



/*@}*/
/**
 * \defgroup cleaner_output_mesh Cleaner Session Mesh Access
 * \ingroup cleaner
 *@{*/
/**
 * Returns the mesh within current session.  Session cannot be
 * modified afterwards unless mesh is released.
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[out]	m : the mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t
meshgems_cleaner_get_mesh(meshgems_cleaner_session_t * this_ses,
                          meshgems_mesh_t ** m);




/**
 * Returns the number of manifold shells detected within current
 * session.  Different manifold shells may share one common interface.
 * Must be called after meshgems_cleaner_get_mesh.
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[out]	shell_cnt : the mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t
meshgems_cleaner_get_manifold_shell_count(meshgems_cleaner_session_t * this_ses,
					  meshgems_integer * m_shell_cnt);




/**
 * Returns one mesh for the given manifold shell within current
 * session.  Must be called after meshgems_cleaner_get_mesh.
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[in]	shell_id : the shell id, valid values are between 1 and shell_cnt;
 * @param[out]	m : the mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t
meshgems_cleaner_get_manifold_shell_mesh(meshgems_cleaner_session_t * this_ses,
					 meshgems_integer m_shell_id,
					 meshgems_mesh_t ** m);




/**
 * Returns the number of shell groups detected within current session.
 * Different shell groups may share one common line or vertex.  Must
 * be called after meshgems_cleaner_get_mesh.
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[out]	shell_cnt : the mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t
meshgems_cleaner_get_shell_group_count(meshgems_cleaner_session_t * this_ses,
				       meshgems_integer * g_shell_cnt);




/**
 * Returns one mesh for the given shell group within current session.
 * Must be called after meshgems_cleaner_get_mesh.
 *
 * @param[in]	this_ses : the cleaner session;
 * @param[in]	shell_id : the shell id, valid values are between 1 and shell_cnt;
 * @param[out]	m : the mesh_t.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD status_t
meshgems_cleaner_get_shell_group_mesh(meshgems_cleaner_session_t * this_ses,
				      meshgems_integer g_shell_id,
				      meshgems_mesh_t ** m);




/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of SmallTri element. 
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_SmallTri: the SmallTri element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_SmallTri_count(meshgems_cleaner_session_t *
                                         this_ses,
                                         meshgems_integer * nb_SmallTri);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of SmallTri element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th SmallTri element;
 * @param[out]	nbelem  : the nbelem-th SmallTri element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_SmallTri(meshgems_cleaner_session_t * this_ses,
                                   meshgems_integer ivtx,
                                   meshgems_integer * nbelem);


/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of BadShape element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_BadShape: the BadShape element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_BadShape_count(meshgems_cleaner_session_t *
                                         this_ses,
                                         meshgems_integer * nb_BadShape);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of BadShape element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th BadShape element;
 * @param[out]	nbelem  : the nbelem-th BadShape element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_BadShape(meshgems_cleaner_session_t * this_ses,
                                   meshgems_integer ivtx,
                                   meshgems_integer * nbelem);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of Overlap element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_Overlap: the Overlap element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Overlap_count(meshgems_cleaner_session_t *
                                        this_ses,
                                        meshgems_integer * nb_Overlap);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of Overlap element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th Overlap element;
 * @param[out]	nbelem  : the nbelem-th Overlap element in the mesh .
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Overlap(meshgems_cleaner_session_t * this_ses,
                                  meshgems_integer ivtx,
                                  meshgems_integer * nbelem);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of FreeEdge element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_FreeEdge: the FreeEdge element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_FreeEdge_count(meshgems_cleaner_session_t *
                                         this_ses,
                                         meshgems_integer * nb_FreeEdge);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of FreeEdge element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th FreeEdge element;
 * @param[out]	nbelem  : the nbelem-th FreeEdge element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_FreeEdge(meshgems_cleaner_session_t * this_ses,
                                   meshgems_integer ivtx,
                                   meshgems_integer * nbelem);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of MultipleEdge element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_MultipleEdge: the MultipleEdge element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_MultipleEdge_count(meshgems_cleaner_session_t *
					     this_ses,
					     meshgems_integer * nb_MultipleEdge);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of MultipleEdge element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th MultipleEdge element;
 * @param[out]	nbelem  : the nbelem-th MultipleEdge element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_MultipleEdge(meshgems_cleaner_session_t * this_ses,
				       meshgems_integer ivtx,
				       meshgems_integer * nbelem);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of Inter element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_Inter: the Inter element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Inter_count(meshgems_cleaner_session_t *
                                      this_ses,
                                      meshgems_integer * nb_Inter);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of Inter element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th Inter element;
 * @param[out]	nbelem  : the nbelem-th Inter element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Inter(meshgems_cleaner_session_t * this_ses,
                                meshgems_integer ivtx,
                                meshgems_integer * nbelem);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of NearTri element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_NearTri: the NearTri element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_NearTri_count(meshgems_cleaner_session_t *
                                        this_ses,
                                        meshgems_integer * nb_NearTri);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of NearTri element.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th NearTri element;
 * @param[out]	nbelem  : the nbelem-th NearTri element in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_NearTri(meshgems_cleaner_session_t * this_ses,
                                  meshgems_integer ivtx,
                                  meshgems_integer * nbelem);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of PointFixable element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_PointFixable: the PointFixable element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_PointFixable_count(meshgems_cleaner_session_t *
                                        this_ses,
                                        meshgems_integer * nb_PointFixable);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of PointFixable array of vertices.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th PointFixable element;
 * @param[out]	nbverts : the PointFixable array of vertices in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_PointFixable_array(meshgems_cleaner_session_t * this_ses,
					     meshgems_integer ivtx,
					     meshgems_integer * nbverts);

/**
 * Keyword of the meshgems_cleaner_check function
 * Get the number of Inter_1d element.
 *
 * @param[in]	this_ses : the Cleaner session;
 * @param[out]	nb_Inter_1d: the Inter_1d element count.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Inter_1d_count(meshgems_cleaner_session_t *
                                        this_ses,
                                        meshgems_integer * nb_Inter_1d);
/**
 * Keyword of the meshgems_cleaner_check function
 * Get the list of Inter_1d array of vertices.
 *
 * @param[in]	this_ses: the cleaner session;
 * @param[in]	ivtx    : the ivtx-th Inter_1d element;
 * @param[out]	nbverts : the Inter_1d array of vertices in the mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_mesh_get_Inter_1d_array(meshgems_cleaner_session_t * this_ses,
					 meshgems_integer ivtx,
					 meshgems_integer * nbverts);



/*@}*/

/*@}*/

/*@}*/

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	this_ses: the cleaner session
 * @param[in]	msh	: the mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_regain_mesh(meshgems_cleaner_session_t * this_ses,
                             meshgems_mesh_t * msh);


/* Private and undocumented methods */

/**
 * Get the meshgems cleaner ident string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char
    *meshgems_cleaner_get_version_ident_string(void);


/**
 * Get the meshgems cleaner banner.
 * @return the banner of the called library
 */

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_session_set_banner(meshgems_cleaner_session_t * this_ses,
                                    meshgems_integer type);

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_session_mesh_write_extended_file(meshgems_integer mesh_idx,
						  void *this_ses);


MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_project_mesh_on_surface(meshgems_cleaner_session_t *this_ses, 
					 mesh_t *m);

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_get_result_element_vertex_coordinates_on_input(meshgems_cleaner_session_t *this_ses,
								meshgems_integer itri_result,
								meshgems_real *bary_xyz_result,
								meshgems_integer *itri_input,
								meshgems_real *bary_xyz_input);

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_get_result_element_vertex_coordinates_on_projected(meshgems_cleaner_session_t *this_ses,
								    meshgems_integer itri_result,
								    meshgems_real *bary_xyz_result,
								    meshgems_integer *itri_projected,
								    meshgems_real *bary_xyz_projected);

MESHGEMS_METHOD meshgems_status_t
meshgems_cleaner_get_result_element_area(meshgems_cleaner_session_t *this_ses,
					 meshgems_integer itri_result,
					 meshgems_real *area_result);

#endif
