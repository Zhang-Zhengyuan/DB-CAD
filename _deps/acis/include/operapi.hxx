/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( OPERAPI_HXX )
#define OPERAPI_HXX
#include "base.hxx"
class BODY;
class WIRE;
class EDGE;
class COEDGE;
class ENTITY;
class ENTITY_LIST;
class SPAposition;
class SPAunit_vector;
class law;
class entity_with_ray;
#include "dcl_oper.h"
#include "api.hxx"
#include "logical.h"
/**
 * @file operapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup COVRAPI
 *
 * @{
 */
/**
 * Determines the area of a planar wire or of all planar wires in a body.
 * <br><br>
 * <b>Role</b>: If the wire is open, the API assumes a straight line between start and end
 * points to compute the area.
 * <br><br>
 * <b>Errors</b>: Pointer to wire is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * Pointer to wire is <tt>NULL</tt> or not to a wire
 * <br><br>
 * <b>Limitations</b>: If there is more than one wire in the given body, only the first
 * wire is considered. If the wire is not planar, or there is any other error
 * condition, a zero value is returned. This API is not guaranteed to give correct
 * results for self intersecting wires.
 * <br><br>
 * <b>Effect</b>: Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param body
 * wire body for which the area is calculated
 * @param area
 * area returned
 * @param ao
 * acis options such as journal,version
 **/
DECL_OPER outcome api_wire_area(
			BODY* body,					// Wire body whose area is to be found
			double &area			// Area of the wire body returned
		, AcisOptions* ao = NULL
	);
/**
 * Determines the area of a planar wire.
 * <br><br>
 * <b>Role</b>: If the wire is open, the API assumes a straight line between start and end
 * points to compute the area.
 * <br><br>
 * <b>Errors</b>: Pointer to wire is <tt>NULL</tt> or not to a wire.
 * <br><br>
 * <b>Limitations</b>: If the wire is not planar, or there is any other error
 * condition, a zero value is returned. This API is not guaranteed to give correct
 * results for self intersecting wires.
 * <br><br>
 * <b>Effect</b>: Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire body for which the area is calculated
 * @param area
 * area returned
 * @param ao
 * acis options such as journal,version
 **/
DECL_OPER outcome api_wire_area(
			WIRE* wire,					// Wire whose area is to be found
			double &area			// Area of the wire body returned
		, AcisOptions* ao = NULL
	);
/** @} */
#endif
