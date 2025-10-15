// $Id: sec_typ.hxx,v 1.4 2000/12/26 19:06:26 products Exp $
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
//    declare procedures for working with loft sections in scheme
//
//----------------------------------------------------------------------

#ifndef sec_typ_hxx
#define sec_typ_hxx

#include "scheme.hxx"
#include "logical.h"

struct Loft_Connected_Coedge_List;


//======================================================================

logical is_Scm_Section(ScmObject);
Loft_Connected_Coedge_List *get_Scm_Section(ScmObject);
ScmObject make_Scm_Section(Loft_Connected_Coedge_List *sec);
int get_Scm_Section_List(ScmObject  list,Loft_Connected_Coedge_List **&secs);
logical is_Scm_Section_List( ScmObject list );

//======================================================================
#endif
