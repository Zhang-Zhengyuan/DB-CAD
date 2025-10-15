// $Id: parthusk.hxx,v 1.6 2000/12/26 18:55:17 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Support routines for part husk.
//
//----------------------------------------------------------------------

#ifndef parthusk_hxx
#define parthusk_hxx

#include "dcl_part.h"
#include "logical.h"

// This procedure initializes the part husk
DECL_PART logical initialize_part_manager();

// This procedure closes the part husk
DECL_PART logical terminate_part_manager();

//======================================================================
#endif
