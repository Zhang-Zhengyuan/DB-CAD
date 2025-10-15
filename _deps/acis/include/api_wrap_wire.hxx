/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef API_WRAP_WIRE_HXX
#define API_WRAP_WIRE_HXX

/**
 * @file api_wrap_wire.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup WARPAPI
 *
 * @{
 */

#include "api.hxx"
#include "alltop.hxx"
#include "position.hxx"
#include "unitvec.hxx"
#include "lists.hxx"
#include "acis_options.hxx"
#include "dcl_warp.h"
class wrap_wire_options;

/**
* \brief Wraps a wire around a solid body while preserving the arc-lengths of the edges
* \in the wire.
*  \ingroup WARPAPI
*
* \param input_wire_body the planar wire body to be wrapped.
* \param initial_target_face the face to start wrapping the wire body around.
* \param initial_begin_position position to start wrapping.
* \param initial_wrap_direction direction to wrap around.
* \param output_wires list of wrapped wire bodies.
* \param wopts wrap_wire_options (currently not used)
* \param aopts AcisOptions, specifies journalling and versioning behavior
*
* <b>Limitations</b><br><br>
*  Input wire body must be planar and not have branches. Currently only planar, conical and developable spline faces
*  can be wrapped around.
* 
**/


DECL_WARP outcome api_wrap_wire( 
				BODY* input_wire_body, 
				FACE* initial_target_face, 
				SPAposition const& initial_begin_position, 
				SPAunit_vector const& initial_wrap_direction, 
				ENTITY_LIST& output_wires,
				wrap_wire_options* wopts=NULL,
				AcisOptions* aopts=NULL);


/** @} */
#endif
