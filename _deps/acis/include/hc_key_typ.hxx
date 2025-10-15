// $Id: iunk_typ.hxx,v 1.5 2001/03/22 22:49:05 products Exp $
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
//    Define scheme color data type
//
//----------------------------------------------------------------------

#ifndef HC_KEY_typ_hxx
#define HC_KEY_typ_hxx

// include files---
#include "scheme.hxx"
#include "logical.h"

#include "hc.h"

//======================================================================

logical is_Scm_HC_KEY(ScmObject);
HC_KEY get_Scm_HC_KEY(ScmObject);
HC_KEY get_Scm_HC_KEY(int argc,ScmObject *argv,int view_arg_num);
ScmObject make_Scm_HC_KEY(HC_KEY);

//======================================================================
#endif
