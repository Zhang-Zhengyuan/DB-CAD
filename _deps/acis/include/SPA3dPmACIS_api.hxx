/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmACIS_api_hxx
#define SPA3dPmACIS_api_hxx

#include "api.hxx"
#include "SPA3dPmACIS.h"

/**
* @file SPA3dPmACIS_api.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

/**
* Initializes the 3dPm Acis bridge.
* <br><br>
* <b>Role:</b> This API initializes the 3dPm Acis bridge.
* Call this function first before using any other 3dPm API functions.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
**/
DECL_SPA3dpm
outcome api_initialize_3dpm_bridge();

/**
* Terminates the 3dPm Acis bridge.
* <br><br>
* <b>Role:</b> This API terminates the 3dPm Acis bridge and frees its internal data.
* Call this function last after using the other 3dPm API functions.
* <br><br>
* <b>Effect:</b> System Routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
**/
DECL_SPA3dpm
outcome api_terminate_3dpm_bridge();

/** @} */
#endif
