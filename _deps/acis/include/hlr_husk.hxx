/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef hlr_husk_hxx
#define hlr_husk_hxx
#include "dcl_hlr.h"
#include "api.hxx"

/**
 * @file hlr_husk.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PHLV5API
 *
 * \ingroup VISMODULE
 * @{
 */
/**
 * Initializes the hidden line removal library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS PHL V5
 * <br><br>
**/
DECL_HLR outcome api_initialize_hlr();

/**
 * Terminates the hidden line removal library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS PHL V5
 * <br><br>
**/
DECL_HLR outcome api_terminate_hlr();

/** @} */
#endif
