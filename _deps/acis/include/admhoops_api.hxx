/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ADMHOOPS_DRAWENG_API_H
#define ADMHOOPS_DRAWENG_API_H

#include "dcl_admhoops.h"
#include "api.hxx"  // class outcome  // class outcome
/**
 * \defgroup ADMHOOPS HOOPS Drawing Interface
 * \ingroup ACISDM
 *
 */
/**
* @file admhoops_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */

/**
* Terminates the admhoops library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
**/
DECL_ADMHOOPS outcome api_terminate_admhoops();

/**
* Initializes the admhoops library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
**/
DECL_ADMHOOPS outcome api_initialize_admhoops();

/** @} */
#endif //ADMHOOPS_DRAWENG_API_H

