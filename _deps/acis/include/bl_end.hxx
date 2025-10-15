/* $Id: bl_end.hxx,v 1.3 2000/12/26 18:25:25 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BL_END_HXX
#define BL_END_HXX

// Indent style: Acis

// History:
//
// 17-Jun-04 puc    Changes to store reordered end info in the blend graph.
// 15-Apr-98 DAP	New code.


// An enum which describes the terminating condition for a blend. The
// possibilities are:

// bl_end_unset - just a convenient value if anyone wants to note that
// they haven't figured it out yet,
// bl_end_closed - the blend will meet another blend face of the blend
// sequence,
// bl_end_internal_closed - the blend will meet another blend face 
// created for the same implicit blend attribute.
// bl_end_vertex_blend - this blend will meet a vertex blend face,
// bl_end_mitred - this blend will be mitred against another, and
// bl_end_open - this blend is expected to end with an endcap or
// something such.
// bl_end_reordered - the end of this blend needs to be reordered.
// bl_end_unknown - we are open minded about how this blend will end,
// this is the value "raw" entity-entity blends are likely to have.

// Note that the values here are mutually exclusive, so for example,
// "open" means strictly "open" and not "mitred" etc., which is slightly
// in contravention to the usual (and sometimes slightly vague) usage
// throughout the blending code. This enum, however, is more
// convenient to pass around than a bunch of logical flags, which is
// historically what has tended to happen.


enum bl_end_condition {
	bl_end_unset,
	bl_end_closed,
    bl_end_internal_closed,
	bl_end_vertex_blend,
	bl_end_mitred,
	bl_end_open,
    bl_end_reordered,
	bl_end_unknown
	};

#endif
