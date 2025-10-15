/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmTetraSession_hxx
#define SPA3dPmTetraSession_hxx

#include "SPA3dPmSessions.hxx"

/**
* @file SPA3dPmTetraSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>TetraSession</tt> class is responsible for the functionality of a MeshGems Tetra session.
* <tt>TetraSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>TetraSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>TetraSession()</tt> creates
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
<caption align="top"> Tetra Parameters </caption>
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
<td> Set this parameter to "all" if you want all connex component to be meshed
or to "outside_components" if you only want the main one.</td>
</tr>

<tr>
<td> centralpoint </td>
<td> bool </td>
<td> yes </td>
<td> Set this parameter to  "no" to remove initial central point.
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
A value of "-1", means no min edge length.
</td>
</tr>

<tr>
<td> max_size </td>
<td> real </td>
<td> -1 </td>
<td> Set the desired maximum edge size in the generated mesh.
A value of "-1", means no max edge length.
</td>
</tr>

<tr>
<td> gradation </td>
<td> real </td>
<td> 1.05 </td>
<td> Set the desired maximum ratio between 2 adjacent tetrahedra edges.
The closer it is to "1", the more uniform the mesh will be.</td>
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
- "safe": this is slower than the sequential mode, but the quality
of the mesh can only improve ;
- "aggressive": this is faster than the sequential mode, but the
quality of the resulting mesh may be altered compared to the sequential mode.  </td>
</tr>

<tr>
<td> optimisation_level </td>
<td> char </td>
<td> standard </td>
<td> This parameter sets the desired optimization level. Valid optimisation
levels are in increasing order of quality vs speed ratio:
- "light" : optimisation using edge/faces swaps only;
- "standard" : usual optimisation. This provides the best compromise between
speed and quality;
- "strong" :  adds three more sweeps of strong optimisation (combination of
edge/face swaps, creation of points improving quality, and point
smoothing) which can be performed as long as the target quality
computed by the program is not reached. </td>
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
surface in order to obtain strictly positive Jacobians in the generated quadratic mesh.
The default is "yes", which means that the surface nodes are unchanged and that the correction
of nodes is only applied in the volume.</td>
</tr>

<tr>
<td> boundary_regeneration </td>
<td> char </td>
<td> standard </td>
<td> This parameter defines the boundary regeneration strategy.
The value of the parameter can be:
- "standard" :  the standard boundary regeneration algorithm is
used (this is the default).
- "safe" : various methods are tried as long as the regeneration
phase fails. This strategy can be time consuming and can still fail in
very difficult cases. </td>
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
class DECL_SPA3dpm TetraSession : public MgSession
{
public:

	/**
	* Returns the native Distene tetra session pointer belonging to the session referred to by this object.
	*/
	meshgems_tetra_session_t *GetMeshgemsSessionPtr();
};

}

/** @} */
#endif
