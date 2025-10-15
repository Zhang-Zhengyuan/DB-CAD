/* ORIGINAL: 3dt2.1/geomhusk/acisglob.hxx */
// $Id: acisglob.hxx,v 1.6 2000/12/26 18:46:20 products Exp $
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
//    Declare procedures for accessing ACIS global data
//
//----------------------------------------------------------------------

#ifndef acisglob_hxx
#define acisglob_hxx

//======================================================================

#include "dcl_kern.h"
#include <stdio.h>

//======================================================================

DECL_KERN FILE* get_debug_file_ptr();
DECL_KERN FILE* set_debug_file_ptr(FILE* fp);

//======================================================================
#endif
