/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/* ORIGINAL: 3dt2.1/scmext/colr_typ.hxx */
// $Id: colr_typ.hxx,v 1.5 2001/07/06 21:18:47 jhauswir Exp $
//----------------------------------------------------------------------
// purpose---
//    Define scheme color data type
//
//----------------------------------------------------------------------

#ifndef colr_typ_hxx
#define colr_typ_hxx

// include files---
#include "scheme.hxx"
#include "logical.h"

class rgb_color;

//======================================================================

logical is_Scm_Color(ScmObject);
rgb_color get_Scm_Color(ScmObject);
ScmObject make_Scm_Color(const rgb_color&);

//======================================================================
#endif

