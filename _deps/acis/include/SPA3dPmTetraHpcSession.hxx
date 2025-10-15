/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmTetraHpcSession_hxx
#define SPA3dPmTetraHpcSession_hxx

#include "SPA3dPmSessions.hxx"
#include "SPA3dPmTetraHpcSizeSpecification.hxx"

/**
* @file SPA3dPmTetraHpcSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>TetraHpcSession</tt> class is responsible for the functionality of a MeshGems TetraHpc session.
* <tt>TetraHpcSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>TetraHpcSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>TetraHpcSession()</tt> creates
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
<caption align="top"> Tetra HPC Parameters </caption>
<tr>
<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
</tr>

<tr>
<td> gradation </td>
<td> real </td>
<td> 1.05 </td>
<td> Sets the desired maximum ratio between 2 adjacent tetrahedra edges. The closer it is to
"1", the more uniform the mesh will be.
</td>
</tr>

<tr>
<td> internal_vertices </td>
<td> yes or no </td>
<td> yes </td>
<td> Sets whether to insert internal vertices inside the volume or not.
</td>
</tr>

<tr>
<td> max_number_of_threads </td>
<td> int </td>
<td> 4 </td>
<td> Sets the maximum number of computing threads to launch for the meshing process.
</td>
</tr>

<tr>
<td> max_size </td>
<td> real </td>
<td> 0 </td>
<td> Sets the desired maximum edge length for the generated volume mesh. Default value is "0", meaning that
there is no maximum edge length.
</td>
</tr>

<tr>
<td> min_size </td>
<td> real </td>
<td> 0 </td>
<td> Sets the desired  minimum edge length for the generated volume mesh. Default value is "0", meaning that
there is no minimum edge length.
</td>
</tr>

<tr>
<td> number_of_subdomains </td>
<td> int </td>
<td> 0 </td>
<td> Sets the desired number of parallel subdomains. This parameter only exists for the shared memory version.
Default value is "0", meaning that this value will be automatically determined.
</td>
</tr>

<tr>
<td> parallel_strategy </td>
<td> string </td>
<td> reproducible given max number of threads </td>
<td> Sets the desired parallel strategy, influencing the level of reproducibility of the
(result and the potential performances. Possible values for strategy are (in decreasing order for
reproducibility and increasing order for performances):
- reproducible: result is entirely reproducible but performances may not be optimal.
- reproducible given max number of threads: result is reproducible for a given number of allowed threads.
- aggressive: result may not be reproducible but all parallel optimizations are allowed.
Default value is reproducible given max number of threads.
</td>
</tr>


<tr>
<td> verbose </td>
<td> int </td>
<td> 3</td>
<td>This parameter changes the verbosity level of MeshGems-Tetra HPC, between 0 and 10.
The higher it is, the more messages MeshGems-Tetra HPC will send through the message callback.
</td>
</tr>
</table>
*/
class DECL_SPA3dpm TetraHpcSession : public MgSession
{
public:

	/**
	* Returns the native Distene tetra_hpc session pointer belonging to the session referred to by this object.
	*/
	meshgems_tetra_hpc_session_t *GetMeshgemsSessionPtr();

	/**
	* Returns the size-specification object. This can be used to add boundary layer constraints to the operator.
	*/
	TetraHpcSizeSpecification SizeSpecification();

};

}

/** @} */
#endif
