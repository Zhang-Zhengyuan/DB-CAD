/* ORIGINAL: 3dt2.1/scheme/part_typ.hxx */
// $Id: part_typ.hxx,v 1.7 2000/12/26 19:05:55 products Exp $
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
//    declare procedures for working with class PART* in scheme
//
//----------------------------------------------------------------------

#ifndef part_typ_hxx
#define part_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class PART;

//======================================================================

logical is_Scm_Part(ScmObject);
PART* get_Scm_Part(ScmObject);
ScmObject make_Scm_Part(const PART*);

//======================================================================
#endif
