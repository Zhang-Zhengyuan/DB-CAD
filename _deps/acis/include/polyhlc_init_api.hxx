/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef polyhlc_init_api_hxx
#define polyhlc_init_api_hxx
#include "dcl_polyhlc.h"
#include "api.hxx"

/**
 * @file polyhlc_init_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup POLYHLAPI
 * @{
 */

/**
* Initializes the polyhedral component library.
* <br><br>
* <b>Role:</b> This API initializes the polyhedral component.
* It must be called before calling any polyhedral APIs.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
**/
DECL_POLYHLC outcome api_initialize_polyhlc();

/**
* Terminates the polyhedral component library.
* <br><br>
* <b>Role:</b> This API terminates the polyhedral component and frees its internal data.
* Further calls made to polyhedral APIs may cause unpredictable consequences.
* <br><br>
* <b>Effect:</b> System Routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
**/
DECL_POLYHLC outcome api_terminate_polyhlc();

/** @} */
#endif
