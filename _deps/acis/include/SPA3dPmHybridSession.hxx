/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmHybridSession_hxx
#define SPA3dPmHybridSession_hxx

#include "SPA3dPmSessions.hxx"

/**
* @file SPA3dPmHybridSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>HybridSession</tt> class is responsible for the functionality of a MeshGems Hybrid session.
* <tt>HybridSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>HybridSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>HybridSession()</tt> creates
* an object that points to no implementation, and thus it is considered an uninitialized object.
* In case of an error, the class methods throw exceptions.
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
<caption align="top"> Hybrid Parameters </caption>
<tr>

<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
</tr>

<tr>
<td> add_multinormals </td>
<td> bool </td>
<td> no </td>
<td>  If the value is "yes" then Hybrid will compute
* extra normals direction at concave ridges (and corners)..</td>
</tr>


<tr>
<td> boundary_layer_geometric_progression </td>
<td> real </td>
<td> 1.0 </td>
<td> This option allows the user to set the growth parameter used to compute the height of each layer. </td>
</tr>


<tr>
<td> boundary_layer_global_initial_height </td>
<td> real </td>
<td> .1</td>
<td> This option allows the user to set the height of the first layer when the boundary_layer_size_mode parameter is set to "global".
The default value is 0.1 the size of the surface element. </td>
</tr>

<tr>
<td> boundary_layer_max_element_angle </td>
<td> real </td>
<td> 165</td>
<td> This option sets the maximum internal angles of elements (in
degrees). This setting applies to the boundary layer elements only. </td>
</tr>

<tr>
<td> boundary_layer_imprinting</td>
<td> boolean </td>
<td> no</td>
<td> If the value of this parameter is "yes", this activates the imprinting of
the boundary layers on some referenced surface.
</td>
</tr>

<tr>
<td> boundary_layer_snapping</td>
<td> boolean </td>
<td> no</td>
<td> If the value of this parameter is "yes", this activates the snapping of the
boundary layers to some referenced surface.
</td>
</tr>

<tr>
<td> boundary_layer_size_mode</td>
<td> string </td>
<td> global</td>
<td> This option sets the behavior for the boundary layer sizes.
If the value is:
- global : the boundary layer global initial height is used to compute the layer heights.
- local : a user-defined function is used to set the local size
</td>
</tr>

<tr>
<td> collision_mode</td>
<td> string </td>
<td> stop</td>
<td> This option sets the behavior in case of collision between layers.
If the value is:
- decrease : this keeps the number of desired layer but decreases the height of the layers to avoid
collision
- stop : this stops locally the layers generation; the number of generated layers may differ from the
specified desired number
</td>
</tr>


<tr>
<td> element_generation</td>
<td> string </td>
<td> tetra_dominant </td>
<td> If the value is:
- tetra dominant : prismatic or hexahedral elements are generated in the boundary layers, with
tetrahedra in the remaining volume.
- hexa dominant : prismatic or hexahedral elements are generated in the boundary layers, with
mixture of hexahedra and tetrahedra in the remaining volume
- extrusion only : only prismatic or hexahedral elements near the boundary are generated. The
remaining volume is not filled.
- cartesian core : cartesian core with a mixture of tetrahedra and pyramids in the remaining volume.
The edge length of the generated hexahedra is given by the parameter global_physical_size.
</td>
</tr>


<tr>
<td> global_physical_size</td>
<td> real </td>
<td> mean surface size </td>
<td>This parameter defines the constant global size used when a cartesian core is requested.
</td>
</tr>

<tr>
<td> gradation</td>
<td> real </td>
<td> 2 </td>
<td>Sets the desired maximum ratio between 2 adjacent edges. It applies only to the edges
which belong to the tetrahedra. The closer it is to "1", the more uniform the mesh will be. Default
value is "2.0".
</td>
</tr>

<tr>
<td> max_number_of_threads</td>
<td> integer </td>
<td> 4 </td>
<td>This option allows Hybrid to run in parallel using a multithread
mode. This option specifies the number of threads to be used (this should be the number of cores
or processors of a shared memory parallel architecture). This option might not be available on all
platforms.
</td>
</tr>

<tr>
<td> multinormal_angle_threshold</td>
<td> integer </td>
<td> 30 </td>
<td>Sets the maximum angle between the multiple normals at opening
(concave) ridges. This parameter is used when the add_multinormals is set to "yes".
</td>
</tr>

<tr>
<td> normal_direction</td>
<td> integer </td>
<td> 1 </td>
<td>This option describes whether Hybrid should use the surface normals
or the opposite of the surface normals. If the value is:
- 1 : this means the layers grow in the same direction as the normals to the surface
- -1 : this means the layers grow in the inverse opposite of the normals to the surface
Default is 1.
Note that Hybrid assumes that the normals to the surface are oriented towards the inside
of the domain.
</td>
</tr>

<tr>
<td> number_of_boundary_layers</td>
<td> integer </td>
<td> 0 </td>
<td>This option allows the user to set the number of boundary layers.
</td>
</tr>

<tr>
<td> smooth_normals </td>
<td> boolean </td>
<td> no </td>
<td>
If the value is "yes" then Hybrid will smooth normals at closed (convex)
ridges and corners.
</td>
</tr>

<tr>
<td> verbose </td>
<td> integer </td>
<td> 10 </td>
<td>
This parameter allows the user to raise the amount of information sent to the callback during
the mesh generation process.
</td>
</tr>
</table>
*/
class DECL_SPA3dpm HybridSession : public MgSession
{
public:

	/**
	* Returns the native Distene hybrid session pointer belonging to the session referred to by this object.
	*/
	meshgems_hybrid_session_t *GetMeshgemsSessionPtr();
};

}

/** @} */
#endif
