/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef WARP_UTILS_HXX
#define WARP_UTILS_HXX
#include "dcl_oper.h"
#include "logical.h"
#include "base.hxx"
class EDGE;
class outcome;
class SPAvector;
class AcisOptions;
/**
 * @file edge_utils.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup COVRAPI
 *
 * @{
 */
/**
 * Moves the position of an edge inside the limits of a sheet body.
 * <br><br>
 * <b>Role:</b> This API creates a parallel edge at the specified distance by creating a new edge
 * with the same geometry and extending it the specified distance.
 * After this step, it is imprinted in the sheet body and the old edge is merged.
 * The optional <tt>extend</tt> argument extends the edge to the boundaries of the surface.
 * Otherwise, it only moves the edge without adding any extra length.
 * <br><br>
 * <b>Limitations</b>: Only straight lines and ellipses may be moved.
 * An edge can only be moved along its adjacent faces and the body has to be a planar sheet body.
 * <br><br>
 * <b>Effect</b>: Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edge
 * edge to be moved
 * @param dist_vector
 * offset distance
 * @param extend
 * extend or not default
 * @param ao
 * acis options such as journal,version
 **/
DECL_OPER outcome api_move_edge(
			EDGE *edge,
			SPAvector	dist_vector,
			logical extend = TRUE,
			AcisOptions *ao = NULL
	);
/**
 * Moves the position of an edge inside the limits of a sheet body.
 * <br><br>
 * <b>Role:</b> This API creates a parallel edge at the specified distance by creating a new edge
 * with the same geometry and extending it the specified distance.
 * After this step, it is imprinted in the sheet body and the old edge is merged.
 * The optional <tt>extend</tt> argument extends the edge to the boundaries of the surface.
 * Otherwise, it only moves the edge without adding any extra length.
 * <br><br>
 * <b>Limitations</b>: Only straight lines and ellipses may be moved.
 * An edge can only be moved along its adjacent faces and the body has to be a planar sheet body.
 * <br><br>
 * <b>Effect</b>: Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edge
 * edge to be moved
 * @param dist
 * offset distance
 * @param extend
 * extend or not default
 * @param ao
 * acis options such as journal,version
 **/
DECL_OPER outcome api_move_edge(
			EDGE *edge,
			double	dist,
			logical extend = TRUE,
			AcisOptions *ao = NULL
	);
/** @} */
#endif


