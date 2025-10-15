/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmCleanerSession_hxx
#define SPA3dPmCleanerSession_hxx

#include "SPA3dPmSessions.hxx"

/**
* @file SPA3dPmCleanerSession.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

/**
* The <tt>CleanerSession</tt> class is responsible for the functionality of a MeshGems Cleaner session.
* <tt>CleanerSession</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>CleanerSession</tt> object does not duplicate the underlying session implementation, it only increases
* the number of light objects referencing the actual session. The default constructor <tt>CleanerSession()</tt> creates
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
<caption align="top"> Cleaner Parameters </caption>
<tr>
<th> Parameter name </th>
<th> Type </th>
<th> Default value </th>
<th> Comment </th>
</tr>

<tr>
<td> folding_angle </td>
<td> real </td>
<td>  </td>
<td> Sets the threshold angle below which 2 connected triangles are
considered overlapping. Reduce this value if model contains sharp angles below
this threshold that must be kept.</td>
</tr>

<tr>
<td> min_hole_size </td>
<td> real </td>
<td>  </td>
<td> Sets the surface size threshold below which holes are filled.
By default, no holes are filled.</td>
</tr>

<tr>
<td> number_of_passes </td>
<td> integer </td>
<td>  </td>
<td> Sets the number of passes for the fix mode. The accepted values are:
- 1 : analyses and fixes mesh with only the first stage of the cleaning procedure
- >1 : analyses and fixes mesh with multiple pass cleaning procedure.
</td>
</tr>

<tr>
<td> max_elapsed_time </td>
<td> real </td>
<td>  not limited</td>
<td> Sets the maximum elapsed time desired for treatment. By default, elapsed time is
not limited.
</td>
</tr>


<tr>
<td> overlap_angle </td>
<td> real </td>
<td>  not limited</td>
<td> Sets the angle below which 2 unconnected triangles are considered overlapping.
</td>
</tr>

<tr>
<td> overlap_distance </td>
<td> real </td>
<td>  not limited</td>
<td> Sets the distance below which 2 unconnected triangles are considered overlapping.
</td>
</tr>

<tr>
<td> remesh_planes </td>
<td> boolean </td>
<td>  </td>
<td>  Inserts vertices on planes to improve mesh quality. May be useful for poor quality
triangulations (eg. STL or DXF triangulations)
</td>
</tr>

<tr>
<td> required_entities </td>
<td> string </td>
<td>  </td>
<td>  Sets the required entities processing. The accepted values are:
- respect : obey requirements;
- ignore : do not take requirements into account (NOT IMPLEMENTED);
- clear : remove all requirements.
</td>
</tr>

<tr>
<td> resolution_length </td>
<td> real </td>
<td>  </td>
<td>  Sets the length threshold below which an edge is collapsed provided the tolerance
displacement is respected. Sets the tolerance displacement to 1/5 of this size.
</td>
</tr>

<tr>
<td> sewing_tolerance </td>
<td> real </td>
<td>  </td>
<td> Sets the surface size threshold below which sewing of gaps is performed. By
default, gaps are not sewn.
</td>
</tr>

<tr>
<td> split_bridges </td>
<td> boolean </td>
<td>  </td>
<td> Splits single contacts between two surfaces, i.e. contacts by one vertex or by one edge.
The possible values are:
- no (default);
- yes (or all); only edges and only points.
Note: It is strongly recommended not to use this option by default: only use it on specific cases that
require it. The advised use of this option is as follows:
- start with value no;
- only if the result obtained is not satisfactory, use value only points;
- only if none of the two values above provided acceptable corrections, use all or only edges as a last resort.
</td>
</tr>

<tr>
<td> tags </td>
<td> string </td>
<td>  </td>
<td> Sets if tags are used while processing the mesh. The accepted values are:
- respect : uses tags
- clear : clears tags
</td>
</tr>

<tr>
<td> tolerance_displacement </td>
<td> real </td>
<td>  </td>
<td> Sets the displacement threshold below which modification is allowed and
the distance below which vertices are glued together. Unused in collision resolution. It is set to
resolution length if it is lower.
</td>
</tr>

<tr>
<td> topology </td>
<td> real </td>
<td>  </td>
<td> Sets the applicable fixing operations. The accepted values are:
- ignore : applies all fixing operations
- respect : disables fixing operations which induce topology modifications
</td>
</tr>

<tr>
<td> verbose </td>
<td> integer </td>
<td>3 </td>
<td>
This parameter allows the user to raise the amount of information sent to the callback during the mesh generation process.
</td>
</tr>

<tr>
<td> wires </td>
<td> string </td>
<td>3 </td>
<td>
Sets if wires and volume vertices are used while processing the mesh. The accepted values are:
- respect: uses wires and volume vertices
- clear: clears wires and volume vertices
</td>
</tr>
</table>
*/
class DECL_SPA3dpm CleanerSession : public MgSession
{
public:

	/**
	* Returns the native Distene cleaner session pointer belonging to the session referred to by this object.
	*/
	meshgems_cleaner_session_t *GetMeshgemsSessionPtr();
};

}

/** @} */
#endif
