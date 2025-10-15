/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    WIRE utilities
/*******************************************************************/
#ifndef wire_qry_hxx
#define wire_qry_hxx
#include "dcl_kern.h"
#include "logical.h"
/**
 * @file wire_qry.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */
class WIRE;
class SPAposition;
class SPAunit_vector;
class COEDGE;
class ENTITY_LIST;
class AcisOptions;
class outcome;
/**
* Determines if a <tt>WIRE</tt> is planar.
* <br><br>
* <b>Role:</b> An alternate signature of this API accepts a wire as a chain of coedges.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* @param this_wire
* given wire.
* @param root_point
* returned root point of plane.
* @param normal
* returned normal of plane.
* @param apply_transf
* <tt>TRUE</tt> to apply the body's transform to the results.
* @param use_winding_number
* logical for whether or not to use the winding number, default <tt>TRUE</tt>.
* NOTE: The <tt>use_winding_number</tt> flag has been deprecated as of R17. 
* Setting <tt>use_winding_number</tt> to <tt>FALSE</tt> can give inaccurate results.
**/

DECL_KERN logical is_planar_wire(WIRE* this_wire, SPAposition& root_point, SPAunit_vector& normal,
								logical apply_transf = TRUE, logical use_winding_number = TRUE );

/**
* Determines if a <tt>WIRE</tt> is planar.
* <br><br>
* <b>Role:</b> This signature of this API accepts a chain of coedges instead of a wire.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* @param this_coedge
* given coedge.
* @param root_point
* returned root point of plane.
* @param normal
* returned normal of plane.
* @param apply_transf
* <tt>TRUE</tt> to apply the body's transform to the results.
* @param use_winding_number
* logical for whether or not to use the winding number, default <tt>TRUE</tt>.
* NOTE: The <tt>use_winding_number</tt> flag has been deprecated as of R17. 
* Setting <tt>use_winding_number</tt> to <tt>FALSE</tt> can give inaccurate results.
**/
DECL_KERN logical is_planar_wire(COEDGE* this_coedge, SPAposition& root_point, SPAunit_vector& normal,
								logical apply_transf = TRUE, logical use_winding_number = TRUE );

/**
* Gets the plane of a 3D <tt>WIRE</tt>.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* @param this_wire
* input WIRE.
* @param root_point
* returned root point of plane.
* @param normal
* returned normal of plane.
* @param apply_transf
* <tt>TRUE</tt> to apply the body's transform to the results.
**/
DECL_KERN logical get_wire_plane(WIRE* this_wire, SPAposition& root_point, SPAunit_vector& normal,
								logical apply_transf = TRUE);


/**
* Efficiently compute equal arc length samples from a set of one or more contiguous <tt>EDGEs</tt>. The input edges must be contiguous within
* SPAresabs, or the corresponding TVERTEX tolerances.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* @param edges
* input EDGEs.
* @param num_samp
* input number of samples wanted; must be >= 2.
* @param points
* returned SPApositions. Note: before calling, points must be allocated of size num_samp. The caller owns points, and must delete at the end of its lifetime.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_KERN outcome 
api_arc_len_samples_from_edges(ENTITY_LIST const& edges, int num_samp, SPAposition* points, AcisOptions* ao=NULL);

/**
 * @nodoc
 */

DECL_KERN logical is_linear_wire(
    WIRE*	 		this_wire,
	SPAposition        &point,
	SPAunit_vector     &direction,
	logical         &degenerate
    );
/** @} */
#endif
