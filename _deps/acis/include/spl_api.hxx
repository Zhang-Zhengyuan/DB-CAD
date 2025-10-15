/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//             INITIALIZATION OF SPLINE FUNCTIONS
/*******************************************************************/
#if !defined( SPLINE_API_ROUTINES )
#define SPLINE_API_ROUTINES
#include "dcl_spl.h"
#include "api.hxx"
/*******************************************************************/
/**
* @file spl_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

/**
 * Initializes the spline library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
*/
DECL_SPLINE outcome api_initialize_spline();

/**
 * Terminates the spline library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
*/
DECL_SPLINE outcome api_terminate_spline();

/*! @} */
#endif
