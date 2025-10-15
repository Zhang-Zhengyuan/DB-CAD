#ifndef __MESHGEMS_TETRA_H__
#define __MESHGEMS_TETRA_H__

#include <meshgems/meshgems.h>
#include <meshgems/tetra_short_names.h>
#include <meshgems/tetra_old_names.h>

/**

 \defgroup tetra meshgems_tetra_session_t
 @{

The Tetra component is an automatic tetrahedral mesh generator based
on the constrained VORONOI-DELAUNAY method. The software is capable of
dealing with arbitrary three-dimensional domains and of generating
meshes that are adapted to the various problems of scientific
computing, e.g.  Computational Fluid Dynamics, Computational
Structural Mechanics, Computational ElectroMagnetics, thermal problems
and so on.

Starting from a surface triangular mesh that represents the boundary
of the object, the mesh generator generates a tetrahedral mesh by
filling the corresponding volume while preserving the input surface
mesh exactly. This enables one to assemble several domains which were
meshed separately and independently. 

The surface mesh creation is not included in the generator but is to
be provided by CAD-CAM software packages or other surface mesh
generating tools, like for example the MeshGems CADSurf component.

\anchor tetra_limitations
## Limitations

Since MG-Tetra generates a constrained Delaunay tetrahedrization of a
given input surface mesh, it cannot be guaranteed to deliver a
satisfying resulting mesh when the input data is not compatible with
the algorithms it implements. For example :
- When the input surface mesh contains self-intersecting triangles or
edges,
- when the input surface mesh contains vertices too close to each
others or to an edge or a triangle (their distance is smaller than
1e-4 relatively to the geometry size),
- when triangles with bad aspect ratio (greater than 100) are present
in the input, 
- when some non-neighbour triangles are too close or present a too
strong size discrepancy,
- when the input surface mesh contains too many co-cyclic vertices.

On the other hand, even with bad input data, the implemented
algorithms are expected to either produce a result or to stop with a
proper error message.


\anchor tetra_dependencies
## Dependencies

Beside the OS libraries, Tetra depends on the dynamic libraries from:
- Itself : mg-tetra,
- the MeshGems core : meshgems, meshgems_stubs,
- the Intel compiler runtimes (on Windows only) : mmd, ifcoremd, ifportmd.


## Tetra integration

We describe below the basic steps to generate a mesh with Tetra :

- a \ref meshgems_tetra_session_t shall be created with \ref
meshgems_tetra_session_new()

- It is then necessary to provide the input surface mesh
  of type \ref mesh with \ref meshgems_tetra_set_surface_mesh(). 
- It is possible to drive precisely the mesh generation process in
Tetra with some parameters through the \ref
meshgems_tetra_set_param() function.  The list can be found in \ref
tetra_parameters.
- The meshing process can then be started with 
\ref meshgems_tetra_compute_mesh().
- It is then possible to retrieve the generated output mesh of
  type \ref meshgems_mesh_t through the \ref meshgems_tetra_get_mesh()
  function. Once the data embeded in this mesh has been retrieved, the 
  \ref meshgems_tetra_regain_mesh() function shall be called to give
  this mesh back to the Tetra session. 
- Finally, the \ref meshgems_tetra_session_t shall be destroyed with \ref
  meshgems_tetra_session_delete().

## Tetra data associativity

The surface entities of the \ref meshgems_mesh_t provided as input to Tetra
are strictly preserved in the generated output \ref meshgems_mesh_t. As a
consequence, the associativity between the input and output data is
straightforward.

\defgroup tetra_parameters Tetra parameters
\ingroup tetra

Description of Tetra parameters.

Tetra parameters can be set in a given session with the \ref
meshgems_tetra_set_param  function.

The array below gives an alphabetical list of all Tetra parameters,
with their name, type, default value and a descritpion.  In this array
, the bool type is either "yes" or "no" (but "1", "true", "0", "false"
are also accepted).


<table>
<caption> List of Tetra parameters </caption>
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
  <td> This parameter changes the verbosity level of Tetra, between 0 and
    10.  The higher it is, the more messages  Tetra will send
    through the message callback.</td>
</tr>

<tr>
  <td> components </td>
  <td> char </td>
  <td> outside_components </td>
  <td> Set this parameter to ``all`` if you want all connex component to be meshed
   or to ``outside_components`` if you only want the main one.</td>
</tr>

<tr>
  <td> centralpoint </td>
  <td> bool </td>
  <td> yes </td>
  <td> Set this parameter to  ``no`` to remove initial central point.
  By default, the initial central point is created to speed-up 
  the mesh generation process.</td>
</tr>

<tr>
  <td> max_error_count </td>
  <td> integer </td>
  <td> 1 </td>
  <td> This option sets the maximum number of error Tetra will send
   to the message callback in the boundary regeneration step.
</td>
</tr>

<tr>
  <td> min_size </td>
  <td> real </td>
  <td> -1 </td>
  <td> Set the desired minimum edge size in the generated mesh. 
  A value of ``-1``, means no min edge length.
</td>
</tr>

<tr>
  <td> max_size </td>
  <td> real </td>
  <td> -1 </td>
  <td> Set the desired maximum edge size in the generated mesh.
  A value of ``-1``, means no max edge length.
</td>
</tr>

<tr>
  <td> force_max_size </td>
  <td> bool </td>
  <td> no </td>
  <td> Force the desired maximum edge size in the generated mesh given by 
  max_size
  A value of ``no``, means no max edge length necessary reached.
</td>
</tr>

<tr>
  <td> gradation </td>
  <td> real </td>
  <td> 1.05 </td>
  <td> Set the desired maximum ratio between 2 adjacent tetrahedra edges.
  The closer it is to ``1``, the more uniform the mesh will be.</td>
</tr>

<tr>
  <td> max_number_of_threads </td>
  <td> integer </td>
  <td> 4 </td>
  <td> This parameter is used to set the maximal number of threads the software
  can use in parallel for the multithreading optimization mode.</td>
</tr>

<tr>
  <td> pthreads_mode </td>
  <td> char </td>
  <td> none </td>
  <td> This parameter sets the optimization mode when using multithread
  capabilities. By default, only a sequential optimization is performed.
  The value of this parameter can be:
  - ``safe``: this is slower than the sequential mode, but the quality 
  of the mesh can only improve ;
  - ``aggressive``: this is faster than the sequential mode, but the
  quality of the resulting mesh may be altered compared to the sequential mode.  </td>
</tr>

<tr>
  <td> optimisation_level </td>
  <td> char </td>
  <td> standard </td>
  <td> This parameter sets the desired optimization level. Valid optimisation
  levels are in increasing order of «quality vs speed» ratio:
  - ``light`` : optimisation using edge/faces swaps only;
  - ``standard`` : «usual» optimisation. This provides the best compromise between
  speed and quality;
  - ``strong`` :  adds three more sweeps of strong optimisation (combination of 
  edge/face swaps, creation of points improving quality, and point
  smoothing) which can be performed as long as the target quality
  computed by the program is not reached. </td>
</tr>

 <tr>
  <td> sliver_angle
  <td> real </td>
  <td> 5. </td>
  <td> Set the angle in degrees under which the smooth off slivers parameter
  is applied in the meshgems_tetra_optimise_volume_sliver function. </td>
</tr>

<tr>
  <td> split_overconstrained_elements
  <td> bool </td>
  <td> no </td>
  <td> Splits element having all their vertices on the input surface.
       A value of no means that no correction is applied
       A value of yes means that correction is applied. </td>
</tr>

<tr>
  <td> split_overconstrained_tetrahedra
  <td> bool </td>
  <td> no </td>
  <td> split the tetrahedra which have at least two faces on the surface.
       A value of no means that no correction is applied
       A value of yes mans that correction is applied. </td>
</tr>

<tr>
  <td> element_order </td>
  <td> char </td>
  <td> linear </td>
  <td> This parameter determines the order of the mesh elements to be generated.
  Accepted values are ``linear`` for linear mesh or ``quadratic`` for quadratic mesh.
  Please note that if the input mesh is quadratic this parameter must be set to
  quadratic to avoid errors. </td>
</tr>

<tr>
  <td> force_output_quadratic\mesh </td>
  <td> bool </td>
  <td> no </td>
  <td> Can be used to force Tetra to return the quadratic mesh even if some negative
   Jacobians are remaining.</td>
</tr>

<tr>
  <td> rectify_jacobian </td>
  <td> bool </td>
  <td> yes </td>
  <td>Allow Tetra to correct some nodes so as to make the 
  Jacobian of element strictly positive when generating a quadratic mesh. </td>
</tr>

<tr>
  <td> jacobian_rectification_respect_input_surface_mesh </td>
  <td> bool </td>
  <td> yes </td>
  <td> This parameter can be used to allow or not Tetra to perform some changes in the input
  surface in order to obtain strictly positiv Jacobians in the generated quadratic mesh. 
  The default is ``yes``, which means that the surface nodes are unchanged and that the correction
  of nodes is only applied in the volume.</td>
</tr>

<tr>
  <td> boundary_regeneration </td>
  <td> char </td>
  <td> standard </td>
  <td> This parameter defines the boundary regeneration strategy.
  The value of the parameter can be:
  - ``standard`` :  the standard boundary regeneration algorithm is
  used (this is the default).
  - ``safe`` : various methods are tried as long as the regeneration
  phase fails (including the alternate boundary recovery mode).
  This strategy can be time consuming and can still fail in
  very difficult cases.
  - ``safe_multithread`` : same strategie as safe mode but run using 4
  threads to reduce time consumption
  -``recovery`` : alternate boundary recovery version is activated.</td>
</tr>

<tr>
  <td> use_volume_proximity </td>
  <td> bool </td>
  <td> no </td>
  <td> If activated, Tetra will take into account the proximity information of
  the input mesh and generate meshes where the size depends on distance between
  opposite mesh entities. </td>
</tr>

<tr>
  <td> volume_proximity_layers </td>
  <td> integer </td>
  <td> 2 </td>
  <td>  Defines the number of desired layers between two opposite mesh entities
  when a volume proximity control is requested. This value must be greater or
  equal to 2. </td>
</tr>

</table>

*/



struct meshgems_tetra_session_t_;

/**
 * \typedef tetra_session_t
 * Opaque type to store session data.
 */
typedef struct meshgems_tetra_session_t_ meshgems_tetra_session_t;

/**
 * Simple constructor.
 *
 * @param[in]	ctx	: the context this session is attached to.
 *
 * @retval "a new tetra_session_t" on success;
 *
 * @retval NULL on failure.
 */
MESHGEMS_METHOD meshgems_tetra_session_t *meshgems_tetra_session_new(meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in]	tms	: the tetra session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_tetra_session_delete(meshgems_tetra_session_t *tms);

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ...
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	cb	: the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_message_callback(meshgems_tetra_session_t *tms,
								     meshgems_message_callback_t cb,
								     void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to 
 * end the ongoing session call.
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	cb	: the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in]	user_data	: the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_interrupt_callback(meshgems_tetra_session_t *tms,
								       meshgems_interrupt_callback_t cb,
								       void *user_data);

/**
 * Give the current session belongs to the given num_thread
 *
 * @param[in]	numthread	: the given thread.
 * @param[out]	tms	:  the attached tetra_session.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t  meshgems_tetra_get_current_session(meshgems_integer numthread, 
								      meshgems_tetra_session_t **tms) ;

/**
 * Sets required surface mesh and entities.
 *
 * Defines the skin (external and internal) mesh (vertices and
 * surface elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_surface_mesh(meshgems_tetra_session_t *tms, meshgems_mesh_t *m);

/**
 * Sets input volume mesh and required surface entities.
 *
 * Defines the volume  of mesh (vertices and
 * elements) as well as all the required entities, "floating" or not
 * (vertices, edges or elements).
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	m	: the \ref mesh_t object holding the required vertices and elements.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_volume_mesh(meshgems_tetra_session_t *tms, meshgems_mesh_t *m);

/**
 * Sets the sizemap on the input surface/volume mesh.
 *
 * Defines the sizemap on the input surface or volume mesh. The sizemap must
 * be defined on mesh vertices and isotropic (type meshgems_sizemap_type_t
 * meshgems_sizemap_type_iso_mesh_vertex).
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	s	: the \ref sizemap_t object holding the input sizemap.
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_sizemap(meshgems_tetra_session_t *tms, meshgems_sizemap_t *s);

/**
 * Sets a parameter.
 *
 * This function allows setting:
 * @li debug mode, 
 * @li verbosity level, 
 * @li growth value (gradation/propagation), 
 * @li ...
 *
 * @param[in]	tms	: the tetra session.
 * @param[in]	param_name	: the option name whose value will be changed
 * @param[in]	param_value	: new option value
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_set_param(meshgems_tetra_session_t *tms, 
							  const char *param_name,
							  const char *param_value);

/**
 * Generate a minimal mesh before boundary integrity (boite)
 *
 * @param[in]	tms	: the tetmesh session. This session must have been
 *   given an input mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_mesh_before_boundary_integrity(meshgems_tetra_session_t *tms);

/**
 * Generate a minimal mesh  after boundary integrity (with the boundary entities and the bounding box (boite))
 *
 * @param[in]	tms	: the tetmesh session. This session must have been
 *   given an input mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_mesh_after_boundary_integrity(meshgems_tetra_session_t *tms);

/**
 * Generate a minimal mesh with the boundary entities
 *
 * @param[in]	tms	: the tetra session. This session must have been
 *   given an input mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_mesh_boundary(meshgems_tetra_session_t *tms);

/**
 * Insert the volume vertices inside the mesh
 *
 * @param[in]	tms	: the tetra session. This session must have been
 *   given an input mesh.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_insert_volume_vertices(meshgems_tetra_session_t *tms);


/**
 * Optimise the overconstrained volume elements (ie with at least 2 faces on the boundary)
 * present in the generated volume mesh
 *
 * @param[in]	tms	: the tetra session.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_optimise_volume_overconstrained(meshgems_tetra_session_t *tms);

/**
 * Optimise the sliver elements present in the generated volume mesh
 *
 * @param[in]	tms	: the tetra session.
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_optimise_volume_sliver(meshgems_tetra_session_t *tms);

/**
 * Sequential optimisation for  a volume mesh (regular optimisation).
 *
 * @param[in]	tms	: the tetra session. This session must have been
 *   given a surface mesh with tetra_set_surface_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_optimise_volume_regular(meshgems_tetra_session_t *tms);

/**
 * Generate a mesh.
 *
 * @param[in]	tms	: the tetra session. This session must have been
 *   given a surface mesh with tetra_set_surface_mesh()
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_compute_mesh(meshgems_tetra_session_t *tms);

/**
 * Returns the mesh generated within current session.  Session should not be
 * modified afterwards until mesh is released.
 *
 * @param[in]	tms	: the tetra session
 * @param[out]	msh	: the \ref mesh_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_get_mesh(meshgems_tetra_session_t *tms, meshgems_mesh_t **msh);

/**
 * Gives the mesh control back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	tms	: the tetra session
 * @param[in]	msh	: the \ref mesh_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_regain_mesh(meshgems_tetra_session_t *tms, meshgems_mesh_t *msh);

/**
 * Returns the sizemap on the mesh generated within current session.  Session should not be
 * modified afterwards until sizemap is released.
 *
 * @param[in]	tms	: the tetra session
 * @param[out]	sizemap	: the \ref sizemap_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_get_sizemap(meshgems_tetra_session_t *tms, meshgems_sizemap_t **sizemap);

/**
 * Gives the sizemap back to the current session.  Session and
 * object can be modified afterwards.
 *
 * @param[in]	tms	: the tetra session
 * @param[in]	sizemap	: the \ref sizemap_t (is freed and \b unusable afterwards).
 *
 * @retval STATUS_OK or another STATUS_* in case of error 
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_regain_sizemap(meshgems_tetra_session_t *tms, meshgems_sizemap_t *sizemap);

/**
 * Backward compatibility interface. Should not be used anymore.
 */

#define MESHGEMS_TETRA_VERSION MESHGEMS_VERSION
#define MESHGEMS_TETRA_PATCHRE MESHGEMS_PATCHRE
#define MESHGEMS_TETRA_RELEASE MESHGEMS_RELEASE
#define MESHGEMS_TETRA_VERSION_LONG MESHGEMS_VERSION_LONG

/**
  * Get the seed of the subdomain 
  *
  * Tetrahedra tag in the output mesh are subdomain indices. The seed of each subomain is an oriented face.
  * This function lets you retrieve it for a given subdomain index. The seed face being given by the indices of 
  * its vertices.
  *
  * @param[in] tms     : the tetra session
  * @param[in] subdomain : the index (tag) of the subdomain (from 1 to nsubdomain).
  * @param[out]        seed : the indices of the triangle seeding the subdomain (integer[3])
  *
  * @retval STATUS_OK or another STATUS_* in case of error 
  *
  */
 MESHGEMS_METHOD meshgems_status_t meshgems_tetra_get_subdomain_seed(meshgems_tetra_session_t *tms, meshgems_integer subdomain, meshgems_integer *seed);
 
 /**
  * Get the number of the subdomain 
  *
  * Tetrahedra tag in the output mesh are subdomain indices. This
  * functions gives acces to the number of generated subdomains.
  *
  * @param[in] tms     : the tetra session
  * @param[out]        nsubdomain : the subdomain count
  *
  * @retval STATUS_OK or another STATUS_* in case of error 
  *
  */
 MESHGEMS_METHOD meshgems_status_t meshgems_tetra_get_subdomain_count(meshgems_tetra_session_t *tms, meshgems_integer *nsubdomain);

/**
 * Backward compatibility interface. Should not be used anymore.
 */

enum meshgems_tetra_optim_level_t_
{
  /** Do not perform any optimisation */
  MESHGEMS_TETRA_NO_OPTIMISATION = 0,
  /** Perform only light optimisation */
  MESHGEMS_TETRA_LIGHT_OPTIMISATION = 1,
  /** Perform medium optimisation */
  MESHGEMS_TETRA_MEDIUM_OPTIMISATION = 2,
  /** Perform standard optimisation */
  MESHGEMS_TETRA_STANDARD_OPTIMISATION = 3,
  /** Perform strong optimisation */
  MESHGEMS_TETRA_STRONG_OPTIMISATION = 4,
  /** Perform heavy optimisation */
  MESHGEMS_TETRA_HEAVY_OPTIMISATION = 5,
  /** Perform extrem optimisation */
  MESHGEMS_TETRA_EXTREM_OPTIMISATION = 6,
};
typedef enum meshgems_tetra_optim_level_t_ meshgems_tetra_optim_level_t;

#define MESHGEMS_STATUS_TETRA_ABS_START MESHGEMS_CODE_TETRA_BASE 
#define MESHGEMS_STATUS_TETRA_ABS_END   1999999
#define MESHGEMS_TETRA_ERRCODE(x) (-(MESHGEMS_CODE_TETRA_BASE +(x)))
#define MESHGEMS_TETRA_WARCODE(x) (MESHGEMS_CODE_TETRA_BASE +(x))


#define MESHGEMS_STATUS_TETRA_CANNOT_INSERT_POINTS MESHGEMS_TETRA_ERRCODE(5602)   /* 2002 */
#define MESHGEMS_STATUS_TETRA_COINCIDENT_POINTS MESHGEMS_TETRA_ERRCODE(5105)    /* 2003 */
#define MESHGEMS_STATUS_TETRA_COINCIDENT_POINTS2 MESHGEMS_TETRA_ERRCODE(5103) /* 2004 */
#define MESHGEMS_STATUS_TETRA_CANNOT_INSERT_POINT MESHGEMS_TETRA_ERRCODE(5106) /* 2012 */
#define MESHGEMS_STATUS_TETRA_COINCIDENT_POINTS3 MESHGEMS_TETRA_ERRCODE(5104) /* 2014 */
#define MESHGEMS_STATUS_TETRA_COINCIDENT_POINTS4 MESHGEMS_TETRA_ERRCODE(5107) /* 2103 */

#define MESHGEMS_STATUS_TETRA_BOUNDARY_REGENERATION_FAILURE MESHGEMS_TETRA_ERRCODE(8480) /* 3000 */
#define MESHGEMS_STATUS_TETRA_BOUNDARY_EDGE_REGENERATION_FAILURE MESHGEMS_TETRA_ERRCODE(8441) /* 3009 */
#define MESHGEMS_STATUS_TETRA_BOUNDARY_FACE_REGENERATION_FAILURE MESHGEMS_TETRA_ERRCODE(8423) /* 3019 */
#define MESHGEMS_STATUS_TETRA_BOUNDARY_FACE_REGENERATION_FAILURE2 MESHGEMS_TETRA_ERRCODE(8422) /* 3029 */
#define MESHGEMS_STATUS_TETRA_NO_DOMAIN MESHGEMS_TETRA_ERRCODE(5560) /* 3100 */
#define MESHGEMS_STATUS_TETRA_NO_DOMAIN2 MESHGEMS_TETRA_ERRCODE(5300) /* 3101 */
#define MESHGEMS_STATUS_TETRA_NO_DOMAIN3 MESHGEMS_TETRA_ERRCODE(8491) /* 3102 */
#define MESHGEMS_STATUS_TETRA_EDGE_WITH_EDGE MESHGEMS_TETRA_ERRCODE(5110) /* 3103 */
#define MESHGEMS_STATUS_TETRA_EDGE_WITH_FACE MESHGEMS_TETRA_ERRCODE(5120) /* 3104 */
#define MESHGEMS_STATUS_TETRA_POINT_WITH_FACE MESHGEMS_TETRA_ERRCODE(5150) /* 3105 */
#define MESHGEMS_STATUS_TETRA_EDGE_WITH_FACE2 MESHGEMS_TETRA_ERRCODE(5120) /* 3106 */
#define MESHGEMS_STATUS_TETRA_POINT_WITH_EDGE MESHGEMS_TETRA_ERRCODE(5160) /* 3107 */
#define MESHGEMS_STATUS_TETRA_NO_DOMAIN4 MESHGEMS_TETRA_ERRCODE(5300) /* 3209 */
#define MESHGEMS_STATUS_TETRA_TEDIOUS_REGENERATION MESHGEMS_TETRA_ERRCODE(8460) /* 3500 */

#define MESHGEMS_STATUS_TETRA_SMALL_VOLUME MESHGEMS_TETRA_ERRCODE(5690) /* 9000 */
#define MESHGEMS_STATUS_TETRA_NEGATIVE_VOLUME MESHGEMS_TETRA_ERRCODE(5692) /* 9001 */
#define MESHGEMS_STATUS_TETRA_NEGATIVE_VOLUME2 MESHGEMS_TETRA_ERRCODE(5693) /* 9002 */
#define MESHGEMS_STATUS_TETRA_SMALL_VOLUME2 MESHGEMS_TETRA_ERRCODE(5104) /* 9003 */
#define MESHGEMS_STATUS_TETRA_BAD_TETRA MESHGEMS_TETRA_ERRCODE(5691) /* 9100 */
#define MESHGEMS_STATUS_TETRA_BAD_TRIANGLE MESHGEMS_TETRA_ERRCODE(5620) /* 9102 */
#define MESHGEMS_STATUS_TETRA_BAD_TRIANGLE2 MESHGEMS_TETRA_ERRCODE(5621) /* 9112 */


MESHGEMS_METHOD meshgems_integer meshgems_tetra_get_version_major(void);
MESHGEMS_METHOD meshgems_integer meshgems_tetra_get_version_minor(void);
MESHGEMS_METHOD meshgems_integer meshgems_tetra_get_version_patch(void);
MESHGEMS_METHOD const char* meshgems_tetra_get_version_string(void);
MESHGEMS_METHOD const char *meshgems_tetra_get_version_ident_string(void);
MESHGEMS_METHOD  char* meshgems_tetra_newcoderr_str(int num_thread, int coderr);

/* Private and undocumented methods */

/**
  * Set the maximum number of thread
  *
  * WARNING : Must be set before the first tetra_session_new function call
  *
  * This function set the maximum number of threads which can be used with
  * This number is associated with the maximum number of tetra session.
  *
  * @param[in] nbthreads     : the maximum number of thread which can be used
  * @param[out]         : none - internal initialisations
  *
  * @retval STATUS_OK or another STATUS_* in case of error
  *
  */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_init_num_threads(meshgems_integer *nbthreads);

/** PRIVATE
  * Initialize all the fortran threads
  * This function is private
  *
  * @param[in] nbthreads     : the maximum number of thread which can be used
  * @param[out]         : none - the needed values have been initialized
  *
  * @retval STATUS_OK or another STATUS_* in case of error
  *
  */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_init_threads(meshgems_integer *nbthreads);

/**
 * Returns the mesh with the bounding box generated within current session.
 * In general when the boundary regeneration has failed. The mesh is normalized.
 *
 * @param[in]	tms	: the tetra session
 * @param[out]	msh	: the \ref mesh_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_get_mesh_boite(meshgems_tetra_session_t *tms, meshgems_mesh_t **msh);

/**
 * Returns the mesh  with the outer box generated within current session.
 * The boundary has been regenerated and the output mesh is denormalized
 * (the initial coordinates are preserved)
 *
 * @param[in]	tms	: the tetra session
 * @param[out]	msh	: the \ref mesh_t
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_get_mesh_with_outer_box(meshgems_tetra_session_t *tms, meshgems_mesh_t **msh);

/**
 * Returns the tetrahdron which includes the point with a given coordinate within the current session.
 *
 *
 * @param[in]	tms	: the tetra session
 * @param[in]   xyz : the coordinates of the point to be located
 *
 * @retval NTET or 0 if any tetrahedron found
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_get_tetra_from_coordinates(meshgems_tetra_session_t *tms, meshgems_real *xyz);

/**
 * Print the quality and histogram of a skin mesh.
 * The output depends on the verbosity session.
 *
 * @param[in]	tms	: the tetra session
 * @param[out]		: compute the quality target tms->aspect_ratio
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_qualfa(meshgems_tetra_session_t *tms);

/**
 * Print the quality and histogram of a volume mesh.
 * The output depends on the verbosity session.
 *
 * @param[in]	tms	: the tetra session
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_outqua(meshgems_tetra_session_t *tms);

/**
 * Compute the minimum volume from a skin mesh (control volume).
 *
 * @param[in]	tms	: the tetra session
 * @param[out]		: the minimum volume
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_volcon(meshgems_tetra_session_t *tms);

/**
 * Print the volume and histogram of a volume mesh.
 * The output depends on the verbosity session.
 *
 * Q = (VTr-Vt)/VTr
 * VTr : volume of the regular tetrahedra which have the same ray as t
 * Vt : volume of the tetrahedra t
 *
 * @param[in]	tms	: the tetra session
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_outvol(meshgems_tetra_session_t *tms);

/**
 * Print the high and histogram of a volume mesh.
 * The output depends on the verbosity session.
 *
 * @param[in]	tms	: the tetra session
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_hautel(meshgems_tetra_session_t *tms);

/**
 * Compute and check the volume of each tetrahedra.
 *
 * @param[in]	tms	: the tetra session
 *
 * @retval STATUS_OK
 * @retval STATUS_WARNING if a null volume is detected
 * @retval STATUS_ERROR if a negative volume is detected
 *
 */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_check_volume_mesh(meshgems_tetra_session_t *tms);

/**
  * Print the dihedral angle and histogram of a volume mesh.
  * The output depends on the verbosity session.
  *
  * @param[in]	tms	: the tetra session
  *
  * @retval STATUS_OK or another STATUS_* in case of error
  *
  */
MESHGEMS_METHOD  meshgems_status_t  meshgems_tetra_compute_outdie(meshgems_tetra_session_t *tms);

/**
 * Compute the edges length of a tetrahedra mesh and gives statistics.
 * Generate a sizemap which includes the maximum edges length at each vertex.
 * Gives also the number of edges which are outside the given maximum size
 *
 * @param[in]	tms	: the tetra session
 * @param[out]  sizemap : the output sizemap at each vertex
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_get_edges_length_sizemap(meshgems_tetra_session_t *tms, meshgems_sizemap_t **sizemap);

/**
 * Backward compatibility interface. Should not be used anymore.
 *
 * @param[in]	tms	: the tetra session
 * @param[in]	level	: the optimisation level
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_optimise_volume(meshgems_tetra_session_t *tms, meshgems_tetra_optim_level_t level);

/**
 *  * Backward compatibility interface. Should not be used anymore.
 *
 * @param[in]	tms	: the tetra session
 * @param[in]	level	: the optimisation level
 *
 * @retval STATUS_OK or another STATUS_* in case of error
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_tetra_mesh(meshgems_tetra_session_t *tms, meshgems_tetra_optim_level_t level);

MESHGEMS_METHOD void meshgems_tetra_default_actions(meshgems_tetra_session_t *tms);
MESHGEMS_METHOD void meshgems_tetra_reset_actions(meshgems_tetra_session_t *tms);
MESHGEMS_METHOD void meshgems_tetra_set_actions(meshgems_tetra_session_t *tms, char* action);
MESHGEMS_METHOD void meshgems_tetra_unset_actions(meshgems_tetra_session_t *tms, char* action);


/*@}*/

#endif
