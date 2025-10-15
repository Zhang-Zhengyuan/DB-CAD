/* ORIGINAL: acis2.1/sg_husk/query/sgquery.hxx */
/* $Id: q_wire.hxx,v 1.9 2000/12/26 18:48:59 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define routines to interogate a wire.

#if !defined (Q_WIRE_HXX)
#define Q_WIRE_HXX

#include "dcl_kern.h"

class WIRE;
class ENTITY_LIST;


//STI let: added for edent_rel, raytesting, ct_husk, sense,... 
// Routine which gets all the edges hanging off of the wire structure.
DECL_KERN void
sg_get_coedges_of_wire(
		WIRE *,						// Given wire
		ENTITY_LIST &				// Coedge list returned
	);

// Routine which gets all the edges hanging off of the wire structure.
DECL_KERN void
sg_get_edges_of_wire(
		WIRE *,						// Given wire
		ENTITY_LIST &				// Edge list returned
	);
//STI let: end

#endif
