/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef HLCAPI_HXX
#define HLCAPI_HXX

// ------------------------------------------------------------------------------------------------


#include "dcl_hlc.h"
#include "api.hxx"

/**
 * @nodoc
 * Initializes the HLC library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_HLC outcome api_initialize_hlc();

/**
 * @nodoc
 * Terminates the HLC library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_HLC outcome api_terminate_hlc();

/** @} */

// ------------------------------------------------------------------------------------------------

#endif // HLCAPI_HXX
