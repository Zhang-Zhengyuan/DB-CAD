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
#if !defined(BLEND_API_HXX)
#define BLEND_API_HXX
#include "dcl_compound.h"
#include "api.hxx"
#include "entity_handle.hxx"
#include "length.hxx"
/**
 * @file blend_api.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
* <summary>Blends edges of a solid using round blends.</summary>
* <param name="edge">Edge to blend</param>
* <param name="radius_length">Blend radius.</param>
* <param name="ao">ACIS options.</param>
**/
DECL_COMPOUND outcome api_blend_edge(entity_handle* edge, const double& radius_length, AcisOptions* ao = NULL);
/**
* <summary>Chamfers edges of a solid using same range.</summary>
* <param name="edge">Edge to blend</param>
* <param name="range_length">Chamfer range.</param>
* <param name="ao">ACIS options.</param>
**/
DECL_COMPOUND outcome api_chamfer_edge(entity_handle* edge, const double& range_length, AcisOptions* ao = NULL);
/** @} */
#endif

