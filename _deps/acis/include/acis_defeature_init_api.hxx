/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _ACIS_DEFEATURE_INIT_API_HXX
#define _ACIS_DEFEATURE_INIT_API_HXX

// Acis Headers
#include "kernapi.hxx"
#include "api.err"

#include "dcl_defeature.h"
/**
 * \defgroup DFTAPI Defeaturing
 *
 */
/**
* @file acis_defeature_init_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DFTAPI
 *
 * @{
 */


//-------------------------------------------------------------------------------

//api for initializing Acis Defeaturing
/**
* Initializes the defeature library.
* <br><br>
* <b>Role:</b> This API initializes the defeature component.
* It must be called before calling any defeature APIs.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Defeaturing
 * <br><br>
**/
DECL_DEFEATURE outcome api_initialize_defeature();

//api for terminating Acis Defeaturing

/**
* Terminates the defeature library.
* <br><br>
* <b>Role:</b> This API terminates Defeaturing and frees its internal data.
* Further calls made to Defeaturing may cause unpredictable consequences.
* <br><br>
* <b>Effect:</b> System Routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Defeaturing
 * <br><br>
**/
DECL_DEFEATURE outcome api_terminate_defeature();
/**
 *.
 */
DECL_DEFEATURE logical initialize_defeature();
/**
 *.
 */
DECL_DEFEATURE logical terminate_defeature();
/**
 *.
 */
DECL_DEFEATURE logical Initialize_FRhole();

/** @} */

#endif // _ACIS_DEFEATURE_INIT_API_H

