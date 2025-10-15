/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmHybridCfdSession_hxx
#define SPA3dPmHybridCfdSession_hxx

#include "SPA3dPmSessions.hxx"
#include "SPA3dPmHybridCfdSizeSpecification.hxx"

/**
* @file SPA3dPmHybridCfdSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>HybridCfdSession</tt> class is responsible for the functionality of a MeshGems Hybrid_Cfd session.
* <tt>HybridCfdSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>HybridCfdSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>HybridCfdSession()</tt> creates
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
<caption align="top"> Hybrid CFD Parameters </caption>
<tr>
<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
</tr>

<tr>
<td> boundary_layer_imprinting </td>
<td> boolean </td>
<td> no </td>
<td> Sets whether to modify the input surface in the extrusion directions to
improve the quality of boundary layers. Default value is no.
</td>
</tr>

<tr>
<td> boundary_layer_ridge_angle </td>
<td> real </td>
<td> 30.0 </td>
<td> Sets the ridge angle of detection (in degree). Default value is 30.0.
</td>
</tr>

<tr>
<td> boundary_layer_smoothing </td>
<td> boolean </td>
<td> yes </td>
<td> Sets whether to curve the extrusion directions to improve the quality
and preserve the extrusion pattern in the volume. Default value is yes.
</td>
</tr>

<tr>
<td> complex_enforce_face_conformity </td>
<td> boolean </td>
<td> yes </td>
<td> Sets whether to enforce the conformity of the mesh with pyramids.
Default value is yes.
</td>
</tr>

<tr>
<td> components </td>
<td> string </td>
<td> all </td>
<td> Sets the desired set of components to be meshed. Possible values are:
- all: all the volume components will be meshed.
- outside_components: only the outermost components will be meshed.
Default value is all.
</td>
</tr>

<tr>
<td> element_generation </td>
<td> string </td>
<td> regular </td>
<td> Sets how the elements will be generated in priority. Possible values are:
- regular: generate equilateral tetrahedra in the metric.
- orthogonal: generate orthogonal elements.
- size_driven: select the best choice according to the metric.
Default value is regular.
</td>
</tr>

<tr>
<td> element_target </td>
<td> string </td>
<td> hexa </td>
<td> Sets what elements will be generated in priority. Possible values are:
- tetra: generate tetrahedra in priority.
- prism: generate prisms in priority.
- hexa: generate hexahedra in priority.
Default value is hexa.
</td>
</tr>

<tr>
<td> gradation </td>
<td> real </td>
<td> 1.05 </td>
<td> Sets the desired maximum ratio between 2 adjacent tetrahedra edges. The closer it is to
"1", the more uniform the mesh will be. Default value is "1.05"
</td>
</tr>

<tr>
<td> internal_points </td>
<td> boolean </td>
<td> yes </td>
<td> Sets whether to insert internal vertices inside the volume or not. Default value is
yes.
</td>
</tr>

<tr>
<td> max_number_of_threads </td>
<td> integer </td>
<td> 4 </td>
<td> Sets the maximum number of computing threads to launch for the meshing
process. Default value is "4". Maximum is limited to "8".
</td>
</tr>

<tr>
<td> max_edge_length </td>
<td> real </td>
<td> 0 </td>
<td> Sets the maximum edge length for the generated volume mesh. Default value is
"0", meaning that there is no maximum edge length.
</td>
</tr>

<tr>
<td> max_size </td>
<td> real </td>
<td> 0 </td>
<td> Sets the desired maximum cell size for the generated volume mesh. This will drive the
maximum edge length but does imply that all volume edges will respect this maximum size.
Default value is "0", meaning that there is no maximum size.
</td>
</tr>

<tr>
<td> metric </td>
<td> string </td>
<td> isotropic </td>
<td> Sets the type of metric governing the mesh generation. Possible values are :
- isotropic : the metric induces the same size in all directions.
- anisotropic : the metric induces different sizes depending on the directions.
Default value is isotropic.
</td>
</tr>

<tr>
<td> metric_propagation </td>
<td> string </td>
<td> in_all_directions </td>
<td> Sets the metric propagation behaviour. Possible values are :
- in_all_directions : propagate the metric in all directions.
- in_direction : propagate the metric only in the direction of propagation.
- toward_isotropic : the propagation will tend to create an isotropic metric.
Default value is in_all_directions.
</td>
</tr>

<tr>
<td> min_edge_length </td>
<td> real </td>
<td> 0 </td>
<td> Sets the minimum edge length for the generated volume mesh. Default value is "0",
meaning that there is no minimum edge length.
</td>
</tr>

<tr>
<td> min_size </td>
<td> real </td>
<td> 0 </td>
<td> Sets the desired minimum cell size for the generated volume mesh. This will drive the
minimum edge length but does imply that all volume edges will respect this minimum size.
Default value is "0", meaning that there is no minimum size.
</td>
</tr>

<tr>
<td> number_of_subdomains </td>
<td> integer </td>
<td> 0 </td>
<td> Sets the desired number of parallel subdomains. This parameter only exists
for the shared memory version. Default value is "0", meaning that this value will be automatically
determined.
</td>
</tr>

<tr>
<td> optimisation </td>
<td> boolean </td>
<td> yes </td>
<td> Sets whether to optimise mesh quality or not. Default value is "yes", meaning that
some time will be spent to optimise the resulting volume mesh quality.
</td>
</tr>

<tr>
<td> optimisation_level </td>
<td> string </td>
<td> standard </td>
<td> Sets the desired optimisation level regarding mesh quality. Possible values are,
in increasing order of quality versus speed ratio :
- light
- standard
- strong
The default value is standard. The optimisation levels are strictly incremental. It means that for a
given input mesh, increasing the optimisation level can only lead to equal or better quality for the
generated output mesh.
</td>
</tr>

<tr>
<td> parallel_strategy </td>
<td> string </td>
<td> reproducible_given_max_number_of_threads </td>
<td> Sets the desired parallel strategy, influencing the level of reproducibility of the
result and the potential performances. Possible values for strategy are (in decreasing order for
reproducibility and increasing order for performances):
- reproducible: result is entirely reproducible but performances may not be optimal.
- reproducible_given_max_number_of_threads: result is reproducible for a given number of allowed threads.
- aggressive: result may not be reproducible but all parallel optimizations are allowed.
Default value is reproducible_given_max_number_of_threads.
</td>
</tr>

<tr>
<td> split_overconstrained_edges </td>
<td> boolean </td>
<td> no </td>
<td> Sets whether to split over-constrained edges or not. An edge is
considered as overconstrained when it connects two surface vertices but does not belong to the surface.
Default value is no.
</td>
</tr>

<tr>
<td> split_overconstrained_elements </td>
<td> boolean </td>
<td> no </td>
<td> Sets whether to split over-constrained elements or not.
An element is considered as overconstrained when at least two of its faces belong to the surface or when
at least one of its edges connects two surface vertices but does not belong to the surface. Default
value is no. Activating this option is equivalent to activating both split_overconstrained_edges
and split_overconstrained_tetrahedra.
</td>
</tr>

<tr>
<td> split_overconstrained_tetrahedra </td>
<td> boolean </td>
<td> no </td>
<td> Sets whether to split over-constrained tetrahedra or not. A
tetrahedra is considered as overconstrained when at least two of its faces belong to the surface. Default
value is no.
</td>
</tr>

<tr>
<td> verbose </td>
<td> integer </td>
<td> 3 </td>
<td> This parameter changes the verbosity level of MeshGems-Hybrid CFD, between 0 and 10.
The higher it is, the more messages MeshGems-Hybrid CFD will send through the message callback.
Default is 3.
</td>
</tr>

<tr>
<td> volume_proximity_layers </td>
<td> real </td>
<td> 0 </td>
<td> This parameter sets the desired minimum number of tetrahedra layers
inside the volume. Default is 0 (no minimum).
</td>
</tr>

</table>
*/
class DECL_SPA3dpm HybridCfdSession : public MgSession
{
public:

	/**
	* Returns the native Distene hybrid_cfd session pointer belonging to the session referred to by this object.
	*/
	meshgems_hybrid_cfd_session_t *GetMeshgemsSessionPtr();

	/**
	* Returns the size-specification object. This can be used to add boundary layer constraints to the operator.
	*/
	HybridCfdSizeSpecification SizeSpecification();

};

}

/** @} */
#endif
