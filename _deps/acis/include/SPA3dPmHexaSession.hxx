/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmHexaSession_hxx
#define SPA3dPmHexaSession_hxx

#include "SPA3dPmSessions.hxx"

/**
* @file SPA3dPmHexaSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>HexaSession</tt> class is responsible for the functionality of a MeshGems Hexa session.
* <tt>HexaSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>HexaSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>HexaSession()</tt> creates
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
<caption align="top"> Hexa Parameters </caption>
<tr>
<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
</tr>

<tr>
<td> allow_invalid_elements </td>
<td> boolean </td>
<td> no </td>
<td>
This option specifies that the mesher must conform as much as possible to the geometry (especially
the sharp edges). The resulting meshes may contain invalid elements (but with positive volumes: for
example, three vertices of an hexahedron on the same edge), for the benefit of better geometry accuracy.
</td>
</tr>

<tr>
<td> boundary_layer_size_mode </td>
<td> string </td>
<td> global </td>
<td>
This option sets the behavior for the boundary layer sizes.
If the value is:
- global : the meshgems hexa set boundary layer parameters function is used to set
the layer size
- local : the meshgems hexa set boundary layer parameters on tag function is used to
set the local boundary layer size on a referenced surface.
</td>
</tr>

<tr>
<td> components </td>
<td> string </td>
<td> all </td>
<td>
This option allows the user to mesh or not mesh internal subdomains
(such as holes). To mesh non-manifold domains, this option must be used with the
manifold_geometry option, since by default the mesher considers the input geometry
is manifold. The behaviour of this option depends on the parameter set.
- all : meshes all cavities and generates subdomains accordingly;
- outside skin only : the internal cavities are ignored, i.e. the whole domain is
meshed into a single domain and the internal baffles are ignored.
- outside components : MeshGems-Hexa keeps only the main component, e.g. if N is the number
of components there are N ? 1 unmeshed cavities in the resulting mesh.
</td>
</tr>

<tr>
<td> compute_ridges </td>
<td> boolean </td>
<td> yes </td>
<td>
When the value of this option is set to no, this option specifies
that the mesher can ignore ridges. A ridge is a geometrical entity (a sharp edge).
The resulting meshes will have better quality elements, at the price of ”smoothing”
out the geometry.
</td>
</tr>

<tr>
<td> flatten_hexa_sides </td>
<td> boolean </td>
<td> no </td>
<td>
If set to yes, this parameter activates a specific optimisation
process to flatten the sides of hexahedra.
</td>
</tr>

<tr>
<td> fill_holes </td>
<td> boolean </td>
<td> no </td>
<td>
This option activates the automatic hole filling.
</td>
</tr>

<tr>
<td> geometric_approximation_angle </td>
<td> real </td>
<td> no default </td>
<td>
This option sets the threshold angle used for the
curvature refinement. During the octree building, each octant containing triangles
that make an angle greater than the threshold will be split. The process will be
repeated until all octants contain a portion of the geometry smaller than the threshold angle.
</td>
</tr>

<tr>
<td> geometric_size_mode </td>
<td> string </td>
<td> none</td>
<td>
If the value is "global" then the geometrical size is global,
and the mesher will use the geometrical approximation angle provided.
</td>
</tr>


<tr>
<td> manifold_geometry </td>
<td> boolean </td>
<td> no </td>
<td>
This options tells the mesher to mesh non-manifold geometries
as such and generates subdomains accordingly. See components. Notes on non-manifold
geometries handling:
- when in non-manifold mode, the octree will be refined up to the maximum level around nonmanifold
edges (like around sharp angles) to improve the geometrical accuracy.
- since surface mapping between output quads and input triangles can be problematic in these
areas, we simply removed the mapping locally : the non-manifold nodes and the first neighboring
layers of nodes will not be projected on the real geometry but only freely smoothed.
Note that the geometry and the topology are only locally modified.
</td>
</tr>

<tr>
<td> min_size </td>
<td> real </td>
<td> D/(2^10 - 1) </td>
<td>
The value of this parameter (min size) is a real value which represents the minimal
size of the elements. For relative size, add a ’r’ at the end of the size (for example
0.1r corresponds to 10% of the geometry). Note that the use of this option might collide
with the simultaneous use of the parameter max_size. If both options are used, the value
of the min_size parameter will be kept and the value of the max_size parameter will be
automatically computed
</td>
</tr>

<tr>
<td> max_size </td>
<td> real </td>
<td> D/(2^6 - 1) </td>
<td>
The value of this parameter (max size is a real value which represents the
maximal size of the elements. For relative size, add a ’r’ at the end of the size (for
example 0.1r corresponds to 10% of the geometry). Note that the use of this option might
collide with the simultaneous use of the parameter min_size. If both options are used,
the value of the min_size parameter will be kept and the value of the max_size parameter
will be automatically computed.
</td>
</tr>

<tr>
<td> max_memory </td>
<td> int </td>
<td>  2 GB </td>
<td>
Sets the maximal memory size to n Mbytes, to avoid creating meshes too dense.
Default is 2 GB. If the maximal memory is reached, MeshGems-Hexa will save the volume mesh
in the current state instead of stopping the meshing process and leaving with an error
statement. This can be a way to control the overall number of hexahedra generated.
</td>
</tr>

<tr>
<td> max_number_of_threads </td>
<td> int </td>
<td>  4 </td>
<td>
This option allows MeshGems-Hexa to run in parallel using a
multithread mode. This option specifies the number of threads to be used (this should
be the number of cores or processors of a shared memory parallel architecture).
This option might not be available on all platforms.
</td>
</tr>

<tr>
<td> physical_size_mode </td>
<td> int </td>
<td>  none </td>
<td>
If the value is "local" then the size is locally computed using
the associated sizemap function. Default is "none".
</td>
</tr>

<tr>
<td> recover_sharp_angles </td>
<td> bool </td>
<td>  yes </td>
<td>
This activates the algorithm for sharp angles recovery which is
activated by default. When the parameter of this option is set to no, parts of the geometry with sharp
angles will be lost
</td>
</tr>

<tr>
<td> tags </td>
<td> string </td>
<td>  "respect" </td>
<td>
This option specifies how the mesher will consider surface attributes (references). If the parameter is
set to clear, this will facilitate the geometry recovery, but all surface attributes will be lost.
Two modes are available :
– clear : the mesher ignores the attributes and clear them.
– respect : the mesher respects all the attributes and generates pattern accordingly.
</td>
</tr>

<tr>
<td> verbose </td>
<td> int </td>
<td>  3 </td>
<td>
This parameter allows the user to raise the amount of information sent to the callback during
the mesh generation process. Default level is 3.
</td>
</tr>
</table>
*/
class DECL_SPA3dpm HexaSession : public MgSession
{
public:

	/**
	* Returns the native Distene hexa session pointer belonging to the session referred to by this object.
	*/
	meshgems_hexa_session_t *GetMeshgemsSessionPtr();

};

}

/** @} */
#endif
