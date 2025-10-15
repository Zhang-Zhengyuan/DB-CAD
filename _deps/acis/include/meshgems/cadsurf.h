#ifndef __MESHGEMS_CADSURF_H__
#define __MESHGEMS_CADSURF_H__

#include <meshgems/meshgems.h>
#include <meshgems/cadsurf_short_names.h>
#include <meshgems/cadsurf_old_names.h>

/**

 \defgroup cadsurf meshgems_cadsurf_session_t
 @{

CADSurf creates the mesh of a composite parametric surface while
conforming to a prescribed size map.  To this end, analytical
functions defining the surface, as well as their first and possibly
second derivatives, must be externally provided (for instance by a CAD
system). Also, each patch must be described by its 2D parametric
domain.  CADSurf can also optionally create the mesh of a discrete CAD
(STL-like geometries...).

The method used in CADSurf is based on an incremental Delaunay
approach adapted to a Riemannian metric.


\anchor cadsurf_limitations
## Limitations

Since CADSurf meshes a CAD geometry by meshing each CAD face in a
parametric space, it cannot be guaranteed to deliver a satisfying
resulting mesh when the input data is not compatible with the
algorithms it implements.  For example : 

- When the parametric space definition is not correct. Most often,
  when the surface parametric curves bounding the CAD-face are
  self-intersecting or open.

- When tiny curves are present in the geometry or in its parametric
space representation.  (A curve being considered as tiny when smaller
than 1e-6 relatively to the geometry or to the CAD face parametric
representation it belongs to).

- When the parametrization of a CAD face is varying too fast (most
often when second order derivatives are locally converging to 0 or
diverging to infinity).

- When the requested cell size is locally too fine (smaller than 1e-6
relatively to the geometry size).

On the other hand, even with bad input data, the implemented
algorithms are expected to either produce a result or to stop with a
proper error message.


\anchor cadsurf_dependencies
## Dependencies

Beside the OS libraries, CADSurf depends on the dynamic libraries from:
- Itself : mg-cadsurf,
- the MeshGems core : meshgems, meshgems_stubs,
- the Tetra component : see \ref tetra_dependencies.

## CADSurf integration

We describe below the basic steps to generate a mesh with CADSurf :

- a \ref meshgems_cadsurf_sessiont_t shall be created with \ref
meshgems_cadsurf_session_new()

- It is then necessary to provide one of the following input data:
  - a continuous CAD geometry of type \ref cad with
  \ref meshgems_cadsurf_set_cad()
  - a continuous CAD geometry with a partial discretization of
  type \ref dcad with \ref meshgems_cadsurf_set_dcad() 
  - a discrete CAD geometry of type \ref mesh with 
  \ref meshgems_cadsurf_set_mesh(). 
- It is possible to drive precisely the mesh generation process in
CADSurf with some parameters through the \ref
meshgems_cadsurf_set_param() function.  The list can be found in \ref
cadsurf_parameters.
- The meshing process can then be started with 
\ref meshgems_cadsurf_compute_mesh().
- It is then possible to retrieve the generated output mesh of
  type \ref meshgems_mesh_t through the \ref meshgems_cadsurf_get_mesh()
  function. Once the data embeded in this mesh has been retrieved, the 
  \ref meshgems_cadsurf_regain_mesh() function shall be called to give
  this mesh back to the CADSurf session. 
- Finally, the \ref meshgems_cadsurf_session_t shall be destroyed with \ref
  meshgems_cadsurf_session_delete().

## CADSurf data associativity

The tags (attributes) present in the input geometry entities provided as 
input to CADSurf are propagated in the generated output \ref meshgems_mesh_t :
- A triangle/quadrangle in the output mesh inherits its tag from its
  originating geometric entity (CAD face or triangle).
- An edge in the output mesh inherits its tag from its
  originating geometric entity (CAD edge or mesh edge).
- A vertex in the output mesh inherits its tag from its
  originating geometric entity (CAD vertex or mesh vertex) or has 
  null tag if it was created by CADSurf inside an input entity.


\defgroup cadsurf_parameters CADSurf parameters
\ingroup cadsurf

Description of CADSurf parameters.

CADSurf parameters can be set in a given session with the \ref
meshgems_cadsurf_set_param  function.

The array below gives an alphabetical list of all CADSurf parameters,
with their name, type, default value and a descritpion.
 In this array : 

- ``D`` is the length of the diagonal of the box bounding the CAD
 object.

- ``r`` means that the value of the corresponding option can be
 followed by a ``r`` to define a value relative to ``D``.  For example
 ``global_physical_size = 0.01r`` means that the desired meshing size
 is of 1% of ``D``.

- The bool type is either "yes" or "no" (but "1", "true", "0", "false"
 are also accepted).


<table>
<caption> List of CADSurf parameters </caption>
<tr>
  <th> Parameter name </th>
  <th> Type </th>
  <th> Default value </th>
  <th> Comment </th>
</tr>

<tr>
  <td> allow_patch_independent </td>
  <td> bool </td>
  <td> no </td>
  <td> This parameter can be used to activate the more complete patch
  independent meshing, by generating the mesh on global user-defined 
  hyperpatches.</td>
</tr>

<tr>
  <td> anisotropic_ratio </td>
  <td> real </td>
  <td> 0 </td>
  <td> This parameter defines the maximum
  anisotropic ratio of the metric governing the anisotropic meshing
  process. The default value of 0 means that the metric (and thus the
  generated elements) can be arbitrarily stretched. </td>
</tr>

<tr>
  <td> bad_surface_element_aspect_ratio </td>
  <td> real </td>
  <td> 1000 </td>
  <td> This parameter defines the aspect ratio triggering the bad element 
  classification for the ``force_bad_surface_element_removal`` parameter. </td>
</tr>

<tr>
  <td> chordal_error </td>
  <td> real(r) </td>
  <td> ``D`` </td>
  <td> This variable defines the
  tolerance of a geometric mesh in terms of chordal error. In other
  words, it determines the maximum desired distance between a triangle
  and its supporting CAD surface. Clearly, the smaller this distance
  is, the closer the mesh is to the exact surface. </td>
</tr>

<tr>
  <td> closed_geometry </td>
  <td> bool </td>
  <td> no </td>
  <td> This option describes whether the working geometry is expected to be closed or not.
  When activated, this option helps CADSurf to treat the dirtiest geometries. </td>
</tr>

<tr>
  <td> compute_ridges </td>
  <td> bool </td>
  <td> yes </td>
  <td> If this option is deactivated, CADSurf will not try to preserve lines defined
  by a sharp angle in the input discrete geometry. Only input ridges, free edges, 
  non manifold edges and separation betwen zones with different attributes will be 
  respected (if the ``tags`` parameter is set to ``respect``)</td>
</tr>

<tr>
  <td> correct_surface_intersections </td>
  <td> bool </td>
  <td> yes </td>
  <td> If this option is activated, CADSurf will try to prevent all surface
  intersections. This particularly useful if volume filling is considered in a later stage. </td>
</tr>

<tr>
  <td> create_tag_on_collision </td>
  <td> bool </td>
  <td> yes </td>
  <td> If this option is activated, CADSurf will create new tags to 
  describe tag collisions (when it locally changes the topology, depending 
  on the patch independent options). When this option is not activated, 
  only one tag is preserved while the other one is dropped. </td>
</tr>

<tr>
  <td> debug </td>
  <td> bool </td>
  <td> no </td>
  <td> If ``debug = yes`` CADSurf will be very verbose and will output
  some intermediate files in the working directory. This option is mainly meant
  for Distene support issues. </td>
</tr>

<tr>
  <td> discard_input_topology </td>
  <td> bool </td>
  <td> no </td>
  <td> When activated, the CAD preprocessor of CADSurf computes the CAD topology
  from scratch, without considering the topological information contained in
  the original CAD. Else the input topology is used. </td>
</tr>

<tr>
  <td> element_generation </td>
  <td> char </td>
  <td> triangle </td>
  <td> This variable determines the type of the mesh elements to be generated.
  Accepted values are :
  - ``triangle`` for triangle mesh,
  - ``quad_dominant`` for quadrangle dominant mesh.
  - ``full_quad`` for pure quadrilateral mesh.
  - ``local`` for local choice of the element generation mode. It requires the use
  of a \ref meshgems_element_generation_map_t. </td>
</tr>

<tr>
  <td> element order </td>
  <td> char </td>
  <td> linear </td>
  <td> This variable determines the order of the mesh elements to be generated.
  Accepted values are :
  - ``linear`` for a linear mesh,
  - ``quadratic`` for a quadratic mesh. </td>
</tr>

<tr>
  <td> enforce cad_edge_sizes </td>
  <td> bool </td>
  <td> no </td>
  <td> Relaxes the given sizemap constraint around CAD edges to allow a
  better element quality and a better geometric approximation. It is 
  only useful in combination with the ``gradation`` option.  </td>
</tr>

<tr>
  <td> force_bad_surface_element_removal </td>
  <td> bool </td>
  <td> no </td>
  <td> This patch independent correction option can be activated to 
  remove the bad surface elements (often called slivers) from the 
  generated mesh, without taking into account the tags (attributes)
  specification. </td>
</tr>

<tr>
  <td> geometric_approximation_angle </td>
  <td> real </td>
  <td> 8. </td>
  <td> This variable defines an angle ``theta`` (in degrees) which represents
  the tolerance of a geometric mesh for surfaces. In other words, ``theta`` is
  the limiting angle between the plane of a triangle of the mesh and each of
  the tangent planes at the three vertices. Understandably, the smaller this
  angle is, the closer the mesh is to the exact surface, and the denser
  the resulting mesh is. </td>
</tr>

<tr>
  <td> geometric_size_mode </td>
  <td> char </td>
  <td> none </td>
  <td> If the value is ``global`` then the geometrical size is global, and the 
  mesher will use the geometrical approximation provided.
  If the value is ``local`` then the size is locally computed, on curves 
  and surfaces, using the associated geometric sizemap functions.
  If the value is ``none``, no geometrical sizes will be computed. </td>
</tr>

<tr>
  <td> global_physical_size </td>
  <td> real(r) </td>
  <td> ``D/100`` </td>
  <td> This option defines the constant global size when using a global physical 
  size. </td>
</tr>

<tr>
  <td> gradation </td>
  <td> real </td>
  <td> 1e30 </td>
  <td> Controls the mesh gradation in order to improve the shape quality and
  smoothness of the generated mesh.  It represents the
  maximum ratio between the lengths of two adjacent edges. This
  parameter must be greater than 1. The closer it is to 1, the
  smoother the resulting mesh will be. </td>
</tr>

<tr>
  <td> jacobian_rectification_respect_geometry </td>
  <td> bool </td>
  <td> yes </td>
  <td> This parameter determines whether or not the geometry accuracy is more
  important than the negative Jacobian correction. When this
  parameter is set to ``no``, CADSurf is allowed to lose the exact CAD-mesh
  associativity in order to correct the last negative Jacobians. </td>
</tr>

<tr>
  <td> manifold_geometry </td>
  <td> bool </td>
  <td> no </td>
  <td> This option describes whether the working geometry is expected to be manifold or not.
  When activated, this option helps CADSurf to handle the dirtiest geometries. </td>
</tr>

<tr>
  <td> max_number_of_threads </td>
  <td> int </td>
  <td> 4 </td>
  <td> This parameter controls the maximum number of threads allowed to be used to 
  generate the surface mesh. Please not that this capability of the code is only available
  on Linux and Windows 7, and requires the underlying CAD kernel to be thread safe. </td>
</tr>

<tr>
  <td> max_size </td>
  <td> real(r) </td>
  <td> ``D/5``</td>
  <td> This parameter defines the (global) maximum cell size desired. </td>
</tr>

<tr>
  <td> merge_edges </td>
  <td> bool </td>
  <td> yes </td>
  <td> When activated, this option allows the CAD preprocessor of CADSurf
   to optimize the geometry by merging some small edges. </td>
</tr>

<tr>
  <td> metric </td>
  <td> char </td>
  <td> isotropic </td>
  <td> This option can be set to either ``isotropic``
  or ``anisotropic`` and determines whether CADSurf will generate
  an isotropic or anisotropic mesh. </td>
</tr>

<tr>
  <td> min_size </td>
  <td> real(r) </td>
  <td> ``1e-3*D`` for an isotropic metric and ``1e-5*D`` for an anisotropic one.</td>
  <td> This parameter defines the (global) minimum cell size desired.  </td>
</tr>

<tr>
  <td> optimisation </td>
  <td> bool </td>
  <td> yes </td>
  <td> If this option is activated, CADSurf will optimize the mesh in order to get
  better shaped elements, during a process which respects the patch independent 
  options. This optimisation cannot be fully performed when 
  ``correct_surface_intersections`` is set to ``yes``. </td>
</tr>

<tr>
  <td> optimisation_level </td>
  <td> char </td>
  <td> standard </td>
  <td> This parameter can be used to decrease 
  the level of optimisation performed by CADSurf in order to gain some meshing time.
  To do so, just set it to ``low``. It will then slightly decrease the efforts spent to 
  optimise the mesh quality and also very sligthly decrease the respect of the metric. </td>
</tr>

<tr>
  <td> optimise_tiny_edges </td>
  <td> bool </td>
  <td> no </td>
  <td>  This patch-independent correction option can be activated to remove the
  tiny edges (defined by the option ``tiny_edge_optimisation_length``) from the
  generated mesh when it improves the local mesh quality, without taking 
  into account the tags (attributes) specifications. </td>
</tr>

<tr>
  <td> periodic_tolerance </td>
<td> char(r) </td>
  <td> ``1e-5*D``</td>
  <td> This parameter defines the maximum size difference between two periodic 
  edges and also the maximum distance error between two periodic entities. </td>
</tr>

<tr>
  <td> physical_size_mode </td>
  <td> char </td>
  <td> none </td>
  <td> If the value is ``global``, the physical size is global, and the mesher 
  will use the global physical size provided. If the value is ``local``, the
  size is locally computed, on curves and  surfaces, using the associated physical
  sizemap functions. If the value is ``none``, no physical size will be computed. </td>
</tr>

<tr>
  <td> process_3d_topology </td>
  <td> bool </td>
  <td> no </td>
  <td> This can be used to enable/disable the CADSUrf topology cleanup 
  processing. Please note that deactivating it while providing no input
  topology or a wrong one will lead to erroneous meshes. </td>
</tr>

<tr>
  <td> rectify_jacobian </td>
  <td> bool </td>
  <td> yes </td>
  <td>The quadratic elements generation is part of the CADSurf
  meshing process which inserts the extra element nodes on the CAD.
  This parameter determines whether or not CADSurf will try to 
  correct all elements of the surface mesh with negative 
  Jacobians by moving the internal element nodes of the mesh. </td>
</tr>

<tr>
  <td> remove_duplicate_cad_faces </td>
  <td> bool </td>
  <td> yes </td>
  <td> Defines the behavior of CADSurf regarding the duplicate CAD
  faces. By default, it merges the duplicate CAD faces. This behavior
  can be deactivated by using this option. </td>
</tr>

<tr>
  <td> remove_tiny_edges </td>
  <td> bool </td>
  <td> no </td>
  <td> This patch-independent correction option can be activated
  to remove the tiny (nano) edges from the generated mesh, without
  taking into account the tags (attributes) specifications. </td>
</tr>

<tr>
  <td> required_entities </td>
  <td> char </td>
  <td> respect </td>
  <td> The required entities control the
  correction operations. Accepted values for this parameter are :
  - ``respect`` : CADSurf is not allowed to alter any
  required entity, even for correction purposes,
  - ``ignore : CADSurf will ignore the required entities
 in its processing,
  - ``clear`` : CADSurf will clear any required status for
  the entities. There will not be any entity marked as required in the
  generated mesh. </td>
</tr>

<tr>
  <td> respect_geometry</td>
  <td> bool </td>
  <td> yes </td>
  <td> This patch independent option can be deactivated to allow CADSurf to 
  lower the geometry accuracy in its patch independent process. </td>
</tr>

<tr>
  <td> sewing_tolerance </td>
  <td> real(r) </td>
  <td> ``5e-4*D``</td>
  <td> This parameter is the tolerance of the assembly. It rarely requires to be tuned. </td>
</tr>

<tr>
  <td> surface_intersection_processing_max_cost </td>
  <td> real </td>
  <td> 15 </td>
  <td>  If ``correct_surface_intersections = yes``, this parameter gives
  the time the user is ready to spend in the intersection prevention process. 
  For example, if set to "3", CADSurf will not spend more time in the
  intersection removal process than 3 times the time required to mesh
   without processing the intersections. </td>
</tr>

<tr>
  <td> surface_proximity_layers </td>
  <td> int </td>
  <td> 1 </td>
  <td> Sets the desired number of element layers between different geometric and reference 
  structures. </td>
</tr>

<tr>
  <td> surface_proximity_ratio </td>
  <td> real </td>
  <td> 1. </td>
  <td> Defines a multiplicative coefficient for size computing in the surface proximity 
  process of CADSurf. </td>
</tr>

<tr>
  <td> tags </td>
  <td> char </td>
  <td> respect </td>
  <td> The tag (attribute) system controls the optimisation
  process. Accepted values for this parameter are :
  - ``respect`` : the CAD tags will be preserved and unaltered by the
  optimisation operations,
  - ``ignore`` : the CAD tags will be ignored by the optimisation operations
  but they will still be present in the output mesh whenever possible,
  - ``clear`` : CADSurf will clear any tag on any entity and optimise  accordingly.
  There will not be any tag in the generated mesh. </td>
</tr>

<tr>
  <td> scaled_jacobian_threshold_value </td>
  <td> real </td>
  <td> 0.001 </td>
  <td> This parameter can be used to define what is an invalid Jacobian, when
  generating a quadratic mesh. By default, an element whose Jacobian is less
  than ``0.001`` will be considered as invalid. </td>
</tr>

<tr>
  <td> tiny_edge_avoid_surface_intersections </td>
  <td> bool </td>
  <td> yes </td>
  <td> This option defines the priority between the tiny feature
  suppression and the surface intersection prevention. By default, CADSurf gives
  the priority to the surface intersection prevention rather than to tiny edge or
  bad surface element removal. These mesh features are then removed only if it 
  does not lead to surface intersections. This behaviour can be deactivated by
  setting this parameter to ``no``, giving priority to the tiny edge or bad 
  surface element removal. </td>
</tr>

<tr>
  <td> tiny_edge_length </td>
  <td> real(r) </td>
  <td> ``1e-8*D`` </td>
  <td> This parameter defines the minimal length under which an edge is 
  considered to be a tiny one (see the ``remove_tiny_edges`` option). </td>
</tr>

<tr>
  <td> tiny_edge_optimisation_length </td>
  <td> real(r) </td>
  <td> ``1e-6*D`` </td>
  <td> This parameter defines the minimal length under which an edge is 
  considered to be a tiny one to be optimised out by the
  ``optimise_tiny_edges`` option. </td>
</tr>

<tr>
  <td> tiny_edge_respect_geometry </td>
  <td> bool </td>
  <td> no </td>
  <td> This option defines the behaviour of the tiny edge removal algorithm
  regarding volume collapse. By default, all tiny edges will be removed,
  regardless of any potential volume collapse. When this option is activated,
  it will prevent volume from being collapsed during the tiny edge removal
  process. </td>
</tr>

<tr>
  <td> use_absolute_jacobian </td>
  <td> bool </td>
  <td> no </td>
  <td> This option can be used to change the Jacobian computation when
  generating a quadratic mesh. By default, CADSurf computes a scaled Jacobian.
  If an absolute Jacobian is used, then a Jacobian will be considered 
  as invalid as soon as it is negative. </td>
</tr>

<tr>
  <td> use_surface_proximity </td>
  <td> bool </td>
  <td> no </td>
  <td> Activates the surface proximity detection in CADSurf. </td>
</tr>

<tr>
  <td> use_volume_proximity </td>
  <td> bool </td>
  <td> no </td>
  <td> Activates the volume proximity detection in CADSurf. </td>
</tr>

<tr>
  <td> verbose </td>
  <td> int </td>
  <td> 3 </td>
  <td> Defines the level of verbosity of CADSurf. This value is
  an integer and must be between 0 (almost no message getting through) and
  10 (maximum verbosity). </td>
</tr>

<tr>
  <td> volume_gradation </td>
  <td> real </td>
  <td> 1e30 </td>
  <td> Controls the mesh volume gradation, which can improve the shape 
  quality of a volume mesh built afterward, specially in thin volume areas.
  This parameter must be greater than 1, and should be greater or equal to
  the value of the surface gradation. The closer it is to 1, the smoother
  the final volume mesh will be. This option requires an MG-Tetra license. </td>
</tr>

<tr>
  <td> volume_proximity_layers </td>
  <td> int </td>
  <td> 1 </td>
  <td> Sets the desired number of element layers between different facing surfaces. </td>
</tr>

<tr>
  <td> volume_proximity_ratio </td>
  <td> real </td>
  <td> 1. </td>
  <td> Defines a multiplicative coefficient for size computing in the volume proximity 
  process of CADSurf. </td>
</tr>

</table>


*/


struct meshgems_cadsurf_session_t_;

/**
 * Opaque type to store context data.
 */
typedef struct meshgems_cadsurf_session_t_ meshgems_cadsurf_session_t;

/**
 * Simple constructor.
 * @param ctx : the user context this session will be attached.
 * @return a new cadsurf_session or NULL
 */
MESHGEMS_METHOD meshgems_cadsurf_session_t *meshgems_cadsurf_session_new(meshgems_context_t *ctx);

/**
 * Destructor. Frees all data allocated by this session
 * @param css : the cadsurf_session to destroy
 */
MESHGEMS_METHOD void meshgems_cadsurf_session_delete(meshgems_cadsurf_session_t *css);



/**
 * Set a parameter.
 * See \ref cadsurf_param for the list of available parameters.
 * @param css (in) : the working cadsurf session.
 * @param option_name (in) : the option name whose value will be changed
 * @param option_value (in) : new option value
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_param(meshgems_cadsurf_session_t *css,
							 const char *option_name,
							 const char *option_value);

/**
 * Allocate a buffer suited to handle any request on any elementary data in a CADSurf session.
 * @param[in] css	: the working cadsurf session.
 * @return a pointer
 */
MESHGEMS_METHOD void *meshgems_cadsurf_calloc_generic_buffer(meshgems_cadsurf_session_t *css);

/**
 * Free a buffer allocated by meshgems_cadsurf_calloc_generic_buffer.
 * @param[in] v	: the working buffer.
 * @return a pointer
 */
MESHGEMS_METHOD void meshgems_cadsurf_free_generic_buffer(void *v);

/**
 * Get a parameter.
 * @param css (in) : the working cadsurf session.
 * @param option_name (in) : the option name whose value will be returned
 * @param option_value (out) : the option value
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_param(meshgems_cadsurf_session_t *css,
							 const char *option_name,
							 char *option_value);


/**
 * Set the message callback function.
 * @param css (in) : the working cadsurf session.
 * @param cb (in) : the message callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_message_callback(meshgems_cadsurf_session_t *css,
								    meshgems_message_callback_t cb,
								    void *user_data);

/**
 * Set the interrupt callback function.
 * @param css (in) : the working cadsurf session.
 * @param cb (in) : the interrupt callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_interrupt_callback(meshgems_cadsurf_session_t *css,
								      meshgems_interrupt_callback_t cb,
								      void *user_data);

/**
 * Set the CAD data.
 * @param css (in) : the working cadsurf session.
 * @param c (in) : the CAD data
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_cad(meshgems_cadsurf_session_t *css, meshgems_cad_t *c);

/**
 * Set the DCAD data.
 * @param css (in) : the working cadsurf session.
 * @param dc (in) : the DCAD data
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_dcad(meshgems_cadsurf_session_t *css, meshgems_dcad_t *dc);

/**
 * Set the mesh data.
 * @param css (in) : the working cadsurf session.
 * @param msh (in) : the mesh data
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_mesh(meshgems_cadsurf_session_t *css, meshgems_mesh_t *msh);


/**
 * Set the iso sizemap on CAD faces.
 * @param css (in) : the working cadsurf session.
 * @param fsize (in) : the sizemap function
 * @param user_data (in) : the user pointer fsize will be given
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_sizemap_iso_cad_face(meshgems_cadsurf_session_t *css,
									     meshgems_sizemap_iso_cad_face_t fsize,
									     void *user_data);

/**
 * Set the iso sizemap on CAD edges not associated to cad faces.
 * @param css (in) : the working cadsurf session.
 * @param esize (in) : the sizemap function
 * @param user_data (in) : the user pointer esize will be given
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_sizemap_iso_cad_edge(meshgems_cadsurf_session_t *css,
									     meshgems_sizemap_iso_cad_edge_t esize,
									     void *user_data);
/**
 * Set the iso sizemap on CAD points.
 * @param css (in) : the working cadsurf session.
 * @param psize (in) : the sizemap function
 * @param user_data (in) : the user pointer psize will be given
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_sizemap_iso_cad_point(meshgems_cadsurf_session_t *css,
									      meshgems_sizemap_iso_cad_point_t psize,
									      void *user_data);

/**
 * Set the sizemap.
 * @param css (in) : the working cadsurf session.
 * @param sizemap (in) : the sizemap structure
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_sizemap(meshgems_cadsurf_session_t *css,
								meshgems_sizemap_t *sizemap);

/**
 * Update the sizemap.
 * @param css (in) : the working cadsurf session.
 * @param sizemap (in) : the updated sizemap structure
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_update_sizemap(meshgems_cadsurf_session_t *css,
								meshgems_sizemap_t *sizemap);

/**
 * Set the element_generation_map.
 * @param css (in) : the working cadsurf session.
 * @param element_generation_map (in) : the element_generation_map structure
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_element_generation_map(meshgems_cadsurf_session_t *css,
								meshgems_element_generation_map_t *element_generation_map);

/**
 * Compute a mesh with current session configuration and data.
 * @param css (in) : the working cadsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_compute_mesh(meshgems_cadsurf_session_t *css);

/**
 * Update the mesh according to the updated sizemaps.
 * @param css (in) : the working cadsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_update_mesh(meshgems_cadsurf_session_t *css);


/**
 * Returns the mesh within current session
 * @param css (in) : the working cadsurf session
 * @param msh (out) : the current mesh generated in the session
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_mesh(meshgems_cadsurf_session_t *css, meshgems_mesh_t **msh);

/**
 * Returns the sizemap used to generate the surface mesh
 * @param css (in) : the working cadsurf session
 * @param sizemap (out) : the current sizemap used by the session
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_sizemap(meshgems_cadsurf_session_t *css, meshgems_sizemap_t **sizemap);

/**
 * Gives the output mesh back to the current session.
 * @param css (in) : the working cadsurf session.
 * @param msh (in) : the mesh to give back to the cadsurf session.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_regain_mesh(meshgems_cadsurf_session_t *css, meshgems_mesh_t *msh);


/**
 * Get the vertex location count on cad vertices.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on vertex for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_vertex_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on cad vertices.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the cad vertex, the vertex is located on
 * @param exact (out) : an integer indicating whether the given coordinates are exact or approximate.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_vertex(meshgems_cadsurf_session_t *css, meshgems_integer i,
										 meshgems_integer j, meshgems_integer *uid, meshgems_integer *exact);

/**
 * Get the vertex location count on analytical cad edges.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on analytical edge for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_edge_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on analytical cad edges.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the edge the vertex is located on
 * @param t (out) : the t parameter of the vertex location on this edge
 * @param exact (out) : an integer indicating whether the given coordinates are exact or approximate. For
 * exemple, on an edge shared by two cad faces, a vertex may not have all locations exact. When exact is true,
 * it means that ( curve(t) ==  the xyz coordinates of the vertex in the output mesh)
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_edge(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_real *t, meshgems_integer *exact);

/**
 * Get the vertex location count on discrete cad edges.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on analytical edge for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_edge_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on discrete cad edges.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the edge the vertex is located on
 * @param begin (out) : the first extremity of the segment on which lies the vertex in this discrete parametrisation
 * @param end (out) : the second extremity of the segment on which lies the vertex in this discrete parametrisation
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_edge(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_integer *begin, meshgems_integer *end);

/**
 * Get the vertex location count on analytical cad wires.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on wire for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_wire_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on analytical cad wires.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the wire the vertex is located on
 * @param t (out) : the t parameter of the vertex location on this wire
 * @param exact (out) : an integer indicating whether the given coordinates are exact or approximate. When exact is true,
 * it means that ( curve(t) ==  the xyz coordinates of the vertex in the output mesh)
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_wire(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_real *t, meshgems_integer *exact);

/**
 * Get the vertex location count on discrete cad wires.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on wire for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_wire_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on discrete cad wires.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the wire the vertex is located on
 * @param begin (out) : if the vertex belongs to a discrete cad definition, the first extremity of the segment in this discrete parametrisation
 * @param end (out) : if the vertex belongs to a discrete cad definition, the second extremity of the segment in this discrete parametrisation
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_wire(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_integer *begin, meshgems_integer *end);

/**
 * Get the vertex location count on analytical cad faces.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on face for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_face_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on analytical cad faces.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the face the vertex is located on
 * @param uv (out) : the uv parameter of the vertex location on this face
 * @param exact (out) : an integer indicating whether the given coordinates are exact or approximate. For
 * exemple, on an edge shared by two cad faces, a vertex may not have all locations exact. When exact is true,
 * it means that ( surf(uv) ==  the xyz coordinates of the vertex in the output mesh)
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_face(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_real *uv, meshgems_integer *exact);

/**
 * Get the vertex location count on discrete cad faces.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param count (out) : the number of location on face for this vertex.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_face_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);

/**
 * Get the vertex location on discrete cad faces.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the vertex index in the output mesh you want to query.
 * @param j (in) : the vertex location index you want to query.
 * @param uid (out) : the uid (unique id) of the face the vertex is located on
 * @param itri (out) : the index of the triangle in the discrete parameterisation on which belongs the vertex,
 * @param iqua (out) : the index of the quadrangle in the discrete parameterisation on which belongs the vertex,
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_location_on_discrete_face(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j,
									       meshgems_integer *uid, meshgems_integer *itri, meshgems_integer *iqua);

/**
 * Get the number of remaining intersecting edges in the resulting mesh.
 * @param css (in) : the working cadsurf session.
 * @param count (out) : the number of intersecting edges of the resulting mesh.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_intersecting_edge_count(meshgems_cadsurf_session_t *css, meshgems_integer *count);

/**
 * Get the index of the intersecting edge.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the edge index you want to query.
 * @param e (out) : the index of the i-th intersecting edge of the resulting mesh.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_intersecting_edge(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *e);

/**
 * Get the number of remaining intersecting faces in the resulting mesh.
 * @param css (in) : the working cadsurf session.
 * @param count (out) : the number of intersecting faces of the resulting mesh.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_intersecting_face_count(meshgems_cadsurf_session_t *css, meshgems_integer *count);

/**
 * Get the index of the intersecting face.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : the face index you want to query.
 * @param f (out) : the index of the i-th intersecting face of the resulting mesh.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t cadsurf_get_intersecting_face(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *f);

/**@}*/

/**
 * Backward compatibility interface. Should not be used anymore. Undocumented
 */

MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_cad(meshgems_cadsurf_session_t *css, meshgems_cad_t *c);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_dcad(meshgems_cadsurf_session_t *css, meshgems_dcad_t *dc);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_sizemap_iso_cad_face(meshgems_cadsurf_session_t *css,
                                                                                 meshgems_sizemap_iso_cad_face_t fsize,
                                                                                 void *user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_sizemap_iso_cad_edge(meshgems_cadsurf_session_t *css,
                                                                                 meshgems_sizemap_iso_cad_edge_t esize,
                                                                                 void *user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_sizemap_iso_cad_point(meshgems_cadsurf_session_t *css,
                                                                                  meshgems_sizemap_iso_cad_point_t psize,
                                                                                  void *user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_set_sizemap(meshgems_cadsurf_session_t *css,
                                                                    meshgems_sizemap_t *sizemap);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_get_mesh(meshgems_cadsurf_session_t *css, meshgems_mesh_t **msh);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_data_regain_mesh(meshgems_cadsurf_session_t *css, meshgems_mesh_t *msh);

#define MESHGEMS_CADSURF_ERROR(x_) (MESHGEMS_CADSURF_CODE(((x_) >= 0 ? -(x_) : (x_))))
#define MESHGEMS_CADSURF_WARNING(x_) (MESHGEMS_CADSURF_CODE(((x_) >= 0 ? (x_) : -(x_))))



/* Private and undocumented methods */

MESHGEMS_METHOD meshgems_integer meshgems_cadsurf_get_version_major(void);
MESHGEMS_METHOD meshgems_integer meshgems_cadsurf_get_version_minor(void);
MESHGEMS_METHOD meshgems_integer meshgems_cadsurf_get_version_patch(void);
MESHGEMS_METHOD const char *meshgems_cadsurf_get_version_string(void);
MESHGEMS_METHOD const char *meshgems_cadsurf_get_version_ident_string(void);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_session_set_banner(meshgems_cadsurf_session_t *css, meshgems_integer type);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_edge_location_on_edge_count(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer *count);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_edge_location_on_edge_extended(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_integer j, 
									     meshgems_integer* uid,
									     meshgems_cad_edge_t **e,
									     meshgems_cad_edge_t **ee,
									     meshgems_integer* hyperpatch_tag,
									     meshgems_integer* Is_Available,
									     meshgems_real* t1,
									     meshgems_real* t2);

/**
 * Get the uid of the CAD face on which lies the i-th triangle of the resulting mesh,
 * along with the parametric coordinates of its three vertices on this CAD face.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : index (in the mesh) of the triangle you want to study.
 * @param Is_Available (out): Describes whether the triangle face location is available.
 * 			In case of patch independent element for example, Is_Available is 0.
 * @param uid (out): the uid of the face the triangle belongs to.
 * @param uv1 (out) : the parametric coordinates of the first point of the triangle on the face uid.
 * @param uv2 (out) : the parametric coordinates of the second point of the triangle on the face uid.
 * @param uv3 (out) : the parametric coordinates of the third point of the triangle on the face uid.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_triangle_location_on_face(meshgems_cadsurf_session_t * css,
										 meshgems_integer i,
										 meshgems_integer* Is_Available,
										 meshgems_integer* uid,
										 meshgems_real* uv1,
										 meshgems_real* uv2,
										 meshgems_real* uv3
										 );

MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_triangle_location_on_face_extended(meshgems_cadsurf_session_t * css, meshgems_integer i,
											  meshgems_integer* uid, meshgems_cad_face_t **f,
											  meshgems_integer* hyperpatch_tag,
											  meshgems_integer* Is_Available,
											  meshgems_real* uv1, meshgems_real* uv2, meshgems_real* uv3
											  );

/**
 * Get the uid of the CAD face on which lies the i-th quadrilateral of the resulting mesh,
 * along with the parametric coordinates of its four vertices on this CAD face.
 * @param css (in) : the working cadsurf session.
 * @param i (in) : index (in the mesh) of the quadrilateral you want to study.
 * @param Is_Available (out):Describes whether the quadrilateral face location is available.
 * 			In case of patch independent element for example, Is_Available is 0.
 * @param uid (out): the uid of the face the quadrilateral belongs to.
 * @param uv1 (out) : the parametric coordinates of the first point of the quadrilateral on the face uid.
 * @param uv2 (out) : the parametric coordinates of the second point of the quadrilateral on the face uid.
 * @param uv3 (out) : the parametric coordinates of the third point of the quadrilateral on the face uid.
 * @param uv4 (out) : the parametric coordinates of the fourth point of the quadrilateral on the face uid.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_quadrilateral_location_on_face(meshgems_cadsurf_session_t * css,
										      meshgems_integer i,
										      meshgems_integer* Is_Available,
										      meshgems_integer* uid,
										      meshgems_real* uv1,
										      meshgems_real* uv2,
										      meshgems_real* uv3,
										      meshgems_real* uv4
										      );

MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_quadrilateral_location_on_face_extended(meshgems_cadsurf_session_t * css, meshgems_integer i,
											       meshgems_integer* uid, meshgems_cad_face_t **f,
											       meshgems_integer* hyperpatch_tag,
											       meshgems_integer* Is_Available,
											       meshgems_real* uv1, meshgems_real* uv2,
											       meshgems_real* uv3, meshgems_real* uv4
											       );

/**
 * Set the callback to be called by the cadsurf_session on thread creation.
 * @param css (in) : the working cadsurf session.
 * @param cb (in) : the callback.
 * @param user_data (in) : the user data pointer to be provided as argument to the cb callback.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_start_thread_callback(meshgems_cadsurf_session_t * css,
                                                                             meshgems_start_thread_callback_t cb,
                                                                             void *user_data);

/**
 * Set the callback to be called by the cadsurf_session on thread deletion.
 * @param css (in) : the working cadsurf session.
 * @param cb (in) : the callback.
 * @param user_data (in) : the user data pointer to be provided as argument to the cb callback.
 * @return an error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_set_stop_thread_callback(meshgems_cadsurf_session_t * css,
                                                                            meshgems_start_thread_callback_t cb,
                                                                            void *user_data);

MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_size_iso(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_real *size);
MESHGEMS_METHOD meshgems_status_t meshgems_cadsurf_get_vertex_size_aniso(meshgems_cadsurf_session_t *css, meshgems_integer i, meshgems_real size[6]);

#endif
