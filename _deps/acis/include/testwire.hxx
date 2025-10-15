/* ORIGINAL: acis2.1/kernwire/wire/kwirrtn.hxx */
/* $Id: testwire.hxx,v 1.8 2000/12/26 18:48:33 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( TESTWIRE_HXX )
#define  TESTWIRE_HXX

#include "dcl_kern.h"
#include "logical.h"

class VERTEX;
class WIRE;
class ENTITY_LIST;

/**
* @file testwire.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */

// Description of get_odd_wires_at_vertex
//
//		Get coedges owned by a shell and linked to a vertex into
//		a supplied entity list.
//
//	Arguments:
//		VERTEX*
//		ENTITY_LIST&
//
//	Returns:	updated list
//
//

/**
 * @nodoc
 */
DECL_KERN void get_odd_wires_at_vertex(
			VERTEX *,
			ENTITY_LIST &
		  );

//	Description of isolated_vertex:
//
//		Routine to test if given vertex is an isolated vertex, 
//		embedded or not.
//
//		Uses the convention that next and previous pointers of 
//		the coedge are both null for a single unembedded wire 
//		vertex, or both set to the coedge for a single embedded 
//		vertex.
//
//	Arguments:
//		VERTEX*		vertex
//
//	Returns:
//		logical		TRUE if isolated vertex
//
//	Errors: none
//
/**
* Determines if a vertex is an isolated vertex.
* <br><br>
* <b>Role:</b> This function returns a logical value indicating whether the given vertex
* is an isolated vertex or not. Because a non-manifold vertex connecting multiple edge groups
* (or "separation surfaces") can be an isolated vertex on one separation surface and a
* non-isolated vertex on another separation surface, this function always returns <tt>FALSE</tt>
* for non-manifold vertices connecting multiple separation surfaces. 
* <br><br>
* An isolated vertex is represented by a NULL edge; that is, an edge whose geometry pointer is NULL.
* The start and end vertex pointers of the edge point to the isolated vertex. The partner pointer of
* the coedge on the edge should also be NULL. If the vertex is an isolated, unembedded wire vertex, the
* next and previous pointers of the coedge are NULL. If the vertex represents a degenerate loop on a 
* face (for instance, at the apex of a cone) the next and previous pointers of the coedge point to the coedge. 
* <br>
* @param vert
* the vertex to test.
*/
DECL_KERN logical isolated_vertex( VERTEX* vert );



// Description of spur_vertex:
//
//		Test if given vertex is a spur vertex, embedded or not.
//
//	Arguments:
//		VERTEX*		vertex
//
//	Returns:
//		logical		TRUE if spur vertex
//
//	Errors: none
//

/**
 * @nodoc
 */
DECL_KERN logical spur_vertex( VERTEX* );


// Test if a simple (unbranched) wire is closed.

/**
 * @nodoc
 */
DECL_KERN logical closed_wire( WIRE* );

//  STIBUGFIX 2302 TAM:  begin
// Test if a wire is branched

/**
 * @nodoc
 */
DECL_KERN logical branched_wire( WIRE* );

//  STIBUGFIX 2302 TAM:  end

/** @} */
#endif
