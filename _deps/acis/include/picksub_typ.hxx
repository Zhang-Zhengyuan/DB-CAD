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

#ifndef picksub_typ_hxx
#define picksub_typ_hxx

//======================================================================
#include "scheme.hxx"
#include "logical.h"
#include "iscmpicksub.hxx"

logical is_Scm_PickSubscriber(ScmObject p);
ISCMPickSubscriber* get_Scm_PickSubscriber(ScmObject p);
ScmObject make_Scm_PickSubscriber(ISCMPickSubscriber* ps);

//======================================================================
#endif
