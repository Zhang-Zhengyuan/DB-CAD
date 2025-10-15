/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare procedures for checking ACIS types
//
//----------------------------------------------------------------------

#ifndef blndtype_hxx
#define blndtype_hxx

#include "dcl_blnd.h"

/**
* @file blndtype.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

//======================================================================

//----------------------------------------------------------------------
// The following procedures get the type of the specified kinds of
//  ATTRIBUTEs.
/**
 * Gets the type of blend attribute.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/
DECL_BLND int get_ATTRIB_BLEND_TYPE();
/**
 * Gets the type of round attribute.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/
DECL_BLND int get_ATTRIB_ROUND_TYPE();
/**
 * Gets the type of chamfer attribute.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/
DECL_BLND int get_ATTRIB_CHAMFER_TYPE();

 /** @} */
#endif
