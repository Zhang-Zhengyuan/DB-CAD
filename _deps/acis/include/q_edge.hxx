// $Id: q_edge.hxx,v 1.6 2002/07/10 11:20:31 nadhikar Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef Q_EDGE_HXX
#define Q_EDGE_HXX

#include "dcl_kern.h"
#include "logical.h"
class EDGE;

/**
* @file q_edge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */

/**
 * @nodoc
 */
DECL_KERN void
sg_get_params(
	EDGE* edge,
	double& startp,
	double& endp
);

/**
 * @nodoc
 */
DECL_KERN logical
Vortex_Torus_1(
	const EDGE *the_edge
);


/**
 * @nodoc
 */
DECL_KERN logical
is_manifold_edge( const EDGE *edge);

/**
 * @nodoc
 */
DECL_KERN void
reverse_an_edge ( EDGE *edge);

/**
* Determines if an edge is a zero length edge.
* <br><br>
* <b>Role:</b> This function returns a logical value indicating whether the given 
* edge is a zero length edge or not. Zero length edges are illegal in ACIS. 
* This function does not distinguish between zero length edges and closed edges
* on open curves. If you need to distinguish between zero length edges and closed 
* edges on open curves, you should call @href is_closed_edge_on_open_curve on any edge
* that is found to be a zero length edge.
* <br>
* @param edge
* the edge to test.
*/
DECL_KERN logical
is_zero_length_edge( const EDGE* edge );

/**
* Determines if an edge is a closed edge on an open curve.
* <br><br>
* <b>Role:</b> This function returns a logical value indicating whether the given 
* edge is a closed edge on an open curve or not. Closed edges on open curves are 
* illegal in ACIS, but for historical reasons they are handled by some algorithms. 
* (Any non-straight edge could be considered to be closed if it is bounded by 
* a single tolerant vertex with a large enough tolerance.) 
* <br>
* @param edge
* the edge to test.
*/
DECL_KERN logical
is_closed_edge_on_open_curve( const EDGE* edge );

/** @} */
#endif
