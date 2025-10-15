/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( LOPTAPI_HXX )
#define LOPTAPI_HXX
#include "dcl_lopt.h"
#include "loptapi.hxx"
#include "api.hxx"
/**
 * @file loptapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup LOCALOPS
 *  @{
 */
/**
 * Initializes the local operation tools library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
 DECL_LOPT outcome api_initialize_lopt_ops();
/**
 * Terminates the local operation tools library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_LOPT outcome api_terminate_lopt_ops();

/*! @} */
#endif
