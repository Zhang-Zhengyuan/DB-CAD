/* ORIGINAL: 3dt2.1/scmext/view_typ.hxx */
// $Id: view_typ_pm.hxx,v 1.1 2002/07/18 15:04:30 skenny Exp $
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
//    Scheme acess to views.
//
//----------------------------------------------------------------------

#ifndef gen_view_typ_hxx
#define gen_view_typ_hxx

//======================================================================
#include "scheme.hxx"
#include "logical.h"

#include "wndo_dt.hxx"
#include "gen_view.hxx"

logical is_Scm_View(ScmObject);
SchemeView* get_Scm_View(ScmObject view_arg);
SchemeView* get_Scm_View(int argc,ScmObject *argv,int view_arg_num);
ScmObject make_Scm_View(SchemeView* view);
ScmObject make_Scm_View(window_handle);

//======================================================================
#endif
