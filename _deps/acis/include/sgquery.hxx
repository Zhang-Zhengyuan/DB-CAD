/* ORIGINAL: acis2.1/sg_husk/query/sgquery.hxx */
/* $Id: sgquery.hxx,v 1.20 2002/08/09 17:19:03 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


// this file defines the support for a routines to interogate
// a body.

#if !defined (SG_QUERY)

#define SG_QUERY

#include <stdio.h>
#include "dcl_query.h"
#include "position.hxx"

/**
* @file sgquery.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class ENTITY;
class EDGE;
class SPAtransf;


// Prototypes of various utility routines, which can be used by
// any one.
// Routine which obtains the transformation of the parent body of the
// following entities:-
// body, lump, shell, subshell, face, loop, coedge, edge, vertex and wire

/** \addtogroup MODELANALYSIS
 *  \brief Declared at <sgquery.hxx>
 *  @{
 */

/**
* Returns the transformation of the parent body of the the provided entity.
* <br><br>
* <b>Role:</b> Routine which obtains the transformation of the parent body of
* the following entities: BODY, LUMP, SHELL, SUBSHELL, FACE, LOOP, COEDGE, EDGE,
* VERTEX, and WIRE.
* <br><br>
* <b>Effect:</b> Read-only<br><br>
* @param entity
* entity starting point.
**/

DECL_QUERY SPAtransf &
sg_get_transform(
	ENTITY *entity
	);

/** @} */

/**
 * @nodoc
 */
// For internal use only!

DECL_QUERY SPAposition get_point_on_entity(ENTITY *ent);

// Find the closest point to a given point on a given edge
/**
 * @nodoc
 */
// For internal use only!

DECL_QUERY void
find_cls_pt_on_edge(
	const SPAposition&, 			// from point
	EDGE*, 						// given edge
	SPAposition&, 					// to point returned
	const SPAtransf&				// transformation on the edge parent.
	);


// Find the closest point to a given point on a loop/s of edges.
/**
 * @nodoc
 */
// For internal use only!

DECL_QUERY SPAposition
find_cls_pt_on_edges(
	const SPAposition&, 			// from point
	FACE*, 						// face containing the loop of edges
	const SPAtransf &				// transformation to the edges parent
	);


#endif
