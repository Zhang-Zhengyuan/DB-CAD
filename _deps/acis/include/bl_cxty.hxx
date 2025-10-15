/* ORIGINAL: abl2.1/blend/kernbool/entent/bl_cxty.hxx */
/* $Id: bl_cxty.hxx,v 1.3 2000/12/26 18:25:45 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


//
// Modification---
//
//  10-Jun-05 puc   Added 'bl_smooth' type.
//	08-Mar-96 bd	Add acis.hxx include.
//

#ifndef BL_CXTY
#define BL_CXTY

#include "acis.hxx"
/**
* @file bl_cxty.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

// The notion of convexity needs defining for entity-entity blending.

/**
 * Defines the convexity of a blend.
 * @param bl_convexity_unknown
 * unknown convexity.
 * @param bl_convex
 * blend is convex.
 * @param bl_concave
 * blend is concave.
 **/

enum bl_convexity 
    { 
    bl_convexity_unknown, 
    bl_convex, 
    bl_concave,
    bl_smooth
    };

 /** @} */
#endif
