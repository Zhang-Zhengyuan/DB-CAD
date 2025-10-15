/*******************************************************************/
/*    Copyright (c) 2016-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#pragma once

// Guy Mar13,2020 BTS#102500: Allow Carl Zeiss to remove the face the PIF-cache in their bulletin-board search routines.

#include "dcl_intr.h"
#include "logical.h"

class ENTITY;

// Utility to remove a ATT_PIF_CACHE from a FACE if there is one.
// If the logical flag is TRUE, it removes all ATT_PIF_CACHEs from every face in the body.
// Returns TRUE if it removed anything.
DECL_INTR logical remove_pif_cache(ENTITY const *, logical all_faces = FALSE);

