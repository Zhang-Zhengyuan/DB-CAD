/* ORIGINAL: 3dt2.1/scmext/surf_typ.hxx */
// $Id: surf_typ.hxx,v 1.4 2000/12/26 18:46:12 products Exp $
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
//    Declare procedures for working with surface objects in scheme
//
//----------------------------------------------------------------------

#ifndef surf_typ_hxx
#define surf_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class surface;

//======================================================================

logical is_Scm_Surface(ScmObject);
surface* get_Scm_Surface(ScmObject);
ScmObject make_Scm_Surface(surface*);

//======================================================================
#endif
