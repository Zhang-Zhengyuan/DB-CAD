/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ADMGI_DRAWENG_API_H
#define ADMGI_DRAWENG_API_H

#include "dcl_admgi_draweng.h"
#include "api.hxx"  // class outcome  // class outcome

/**
 * \defgroup ADMGI GI Drawing Interface
 * \ingroup ACISDM
 *
 */
/**
* @file admgi_draweng_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMGI
 *
 * @{
 */
/**
* Terminates the admgi_draweng library.
* <br>
* <b>Role</b>: Terminates the admgi_draweng library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
**/

DECL_DMGI_DRAWENG outcome api_terminate_admgi_draweng();
/**
* Initializes the admgi_draweng library.
* <br>
* <b>Role</b>: Initializes the admgi_draweng library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
**/
DECL_DMGI_DRAWENG outcome api_initialize_admgi_draweng();

/** @} */
#endif //ADMGI_DRAWENG_API_H

