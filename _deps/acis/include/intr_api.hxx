/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
* ACIS Compound
*/
#if !defined(INTR_API_HXX)
#define INTR_API_HXX
#include "dcl_compound.h"
#include "api.hxx"
#include "entity_handle.hxx"
#include "vers.hxx"
/**
 * @file intr_api.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
* <summary>Check body entity.</summary>
* <param name="body">Input body entity handle.</summary>
* <param name=""></summary>
* <param name="ao"ACIS Options.</param>
*/
DECL_COMPOUND outcome api_check_body(entity_handle* body, const int& level, logical& ok, AcisOptions* ao = NULL);
/** @} */
#endif
