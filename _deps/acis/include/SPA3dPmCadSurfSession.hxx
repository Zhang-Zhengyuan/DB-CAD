/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmCadSurfSession_hxx
#define SPA3dPmCadSurfSession_hxx

#include "SPA3dPmSessions.hxx"

/**
* @file SPA3dPmCadSurfSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>CadSurfSession</tt> class is responsible for the functionality of a MeshGems CadSurf session.
* <tt>CadSurfSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>CadSurfSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>CadSurfSession()</tt> creates
* an object that points to no implementation, and thus it is considered an uninitialized object.
* In case of an error, the class methods throw exceptions.
* All indexes are based at 1.
*
The array below gives an alphabetical list of all parameters for this operator,
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
<caption align="top"> List of CADSurf parameters </caption>
<tr>
<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
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
<td> If "debug = yes"` CADSurf will be very verbose and will output
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
- "triangle" for triangle mesh,
- "quad_dominant" for quadrangle dominant mesh.
- "full_quad" for pure quadrilateral mesh
- "local" for local choice of the element generation mode. It requires the use
of a \ref meshgems_element_generation_map_t. </td>
</tr>

<tr>
<td> element_order </td>
<td> char </td>
<td> linear </td>
<td> This variable determines the order of the mesh elements to be generated.
Accepted values are :
- "linear" for a linear mesh,
- "quadratic" for a quadratic mesh. </td>
</tr>

<tr>
<td> enforce_cad_edge_sizes </td>
<td> bool </td>
<td> no </td>
<td> Relaxes the given sizemap constraint around CAD edges to allow a
better element quality and a better geometric approximation. It is
only useful in combination with the "gradation" option.  </td>
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
<td> This variable defines an angle "theta" (in degrees) which represents
the tolerance of a geometric mesh for surfaces. In other words, "theta" is
the limiting angle between the plane of a triangle of the mesh and each of
the tangent planes at the three vertices. Understandably, the smaller this
angle is, the closer the mesh is to the exact surface, and the denser
the resulting mesh is. </td>
</tr>

<tr>
<td> geometric_size_mode </td>
<td> char </td>
<td> none </td>
<td> If the value is "global" then the geometrical size is global, and the
mesher will use the geometrical approximation provided.
If the value is "local" then the size is locally computed, on curves
and surfaces, using the associated geometric sizemap functions.
If the value is "none", no geometrical sizes will be computed. </td>
</tr>

<tr>
<td> global_physical_size </td>
<td> real(r) </td>
<td> D/100 </td>
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
parameter is set to "no", CADSurf is allowed to lose the exact CAD-mesh
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
<td> max_size </td>
<td> real(r) </td>
<td> D/5</td>
<td> This parameter defines the (global) maximum cell size desired. </td>
</tr>

<tr>
<td> max_number_of_points_per_patch </td>
<td> int </td>
<td>  100000 </td>
<td>
This parameter controls the maximum amount of points MeshGems-
CADSurf is allowed to generate on a single CAD patch. For an automatic gestion of the memory, one
can set this parameter to "0".
</td>
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
<td> This option can be set to either "isotropic"
or "anisotropic" and determines whether CADSurf will generate
an isotropic or anisotropic mesh. </td>
</tr>

<tr>
<td> min_size </td>
<td> real(r) </td>
<td> "1e-3*D" for an isotropic metric and "1e-5*D" for an anisotropic one.</td>
<td> This parameter defines the (global) minimum cell size desired.  </td>
</tr>

<tr>
<td> optimisation </td>
<td> bool </td>
<td> yes </td>
<td> If this option is activated, CADSurf will optimize the mesh in order to get
better shaped elements, during a process which respects the patch independent
options. This optimisation cannot be fully performed when
"correct_surface_intersections" is set to "yes". </td>
</tr>

<tr>
<td> optimise_tiny_edges </td>
<td> bool </td>
<td> no </td>
<td>  This patch-independent correction option can be activated to remove the
tiny edges (defined by the option "tiny_edge_optimisation_length") from the
generated mesh when it improves the local mesh quality, without taking
into account the tags (attributes) specifications. </td>
</tr>

<tr>
<td> periodic_tolerance </td>
<td> char(r) </td>
<td> 1e-5*D</td>
<td> This parameter defines the maximum size difference between two periodic
edges and also the maximum distance error between two periodic entities. </td>
</tr>

<tr>
<td> physical_size_mode </td>
<td> char </td>
<td> none </td>
<td> If the value is "global", the physical size is global, and the mesher
will use the global physical size provided. If the value is "local", the
size is locally computed, on curves and  surfaces, using the associated physical
sizemap functions. If the value is "none", no physical size will be computed. </td>
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
-respect: CADSurf is not allowed to alter any
required entity, even for correction purposes,
-ignore : CADSurf will ignore the required entities
in its processing,
-clear : CADSurf will clear any required status for
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
<td> 5e-4*D</td>
<td> This parameter is the tolerance of the assembly. It rarely requires to be tuned. </td>
</tr>

<tr>
<td> surface_intersection_processing_max_cost </td>
<td> real </td>
<td> 15 </td>
<td>  If "correct_surface_intersections = yes", this parameter gives
the time the user is ready to spend in the intersection prevention process.
For example, if set to "3", CADSurf will not spend more time in the
intersection removal process than 3 times the time required to mesh
without processing the intersections. </td>
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
<td> volume_gradation </td>
<td> real </td>
<td> 1e30 </td>
<td> Controls the mesh volume gradation, which can improve the shape
quality of a volume mesh built afterward, specially in thin volume areas.
This parameter must be greater than 1, and should be greater or equal to
the value of the surface gradation. The closer it is to 1, the smoother
the final volume mesh will be. This option requires an MG-Tetra license. </td>
</tr>
</table>
*/
class DECL_SPA3dpm CadSurfSession : public MgSession
{
public:

	/**
	* Returns the native Distene cadsurf session pointers belonging to the session referred to by this object.
	* There are in fact two pointers (and two Distene cadsurf sessions), because one is for the exact (analytic and spline) bodies and the other
	* is for the polyhedral mesh bodies.
	*/
	void GetMeshgemsSessionPtr(meshgems_cadsurf_session_t *mgPtrs[2]);

	/**
	* Returns the native Distene cad pointer belonging to the session referred to by this object.
	* If this <tt>CadSurfSession</tt> was created from a <tt>Mesh</tt> object, there is no underlying cad object,
	* therefore the function returns NULL.
	*/
	meshgems_cad_t *GetMeshgemsCadPtr();

	/**
	* Convert a FACE* to faceId (tag and uid of the corresponding cad face and the corresponding mesh triangles/quadrangles).
	* Returns 0 if f is not found.
	*/
	int GetFaceId(FACE *f) const;

	/**
	* Convert an EDGE* to edgeId (tag and uid of the corresponding cad edge and the corresponding mesh edges).
	* Returns 0 if e is not found.
	*/
	int GetEdgeId(EDGE *e) const;

	/**
	* Convert a VERTEX* to vertexId (uid of the corresponding cad vertex and the corresponding mesh vertex).
	* Returns 0 if v is not found.
	*/
	int GetVertexId(VERTEX *v) const;

	/**
	* Convert a faceId (tag and uid of the corresponding cad face and the corresponding mesh triangles/quadrangles) to FACE*.
	* Returns NULL if faceId is not found.
	*/
	FACE *GetFace(int faceId) const;

	/**
	* Convert an edgeId (tag and uid of the corresponding cad edge and the corresponding mesh edges) to EDGE*.
	* Returns NULL if edgeId is not found.
	*/
	EDGE *GetEdge(int edgeId) const;

	/**
	* Convert a vertexId (uid of the corresponding cad vertex and the corresponding mesh vertex) to VERTEX*.
	* Returns NULL if vertexId is not found.
	*/
	VERTEX *GetVertex(int vertexId) const;
};

}

/** @} */
#endif
