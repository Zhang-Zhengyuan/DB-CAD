// $Id: tube_typ.hxx,v 1.3 2000/12/26 19:05:38 products Exp $
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
//    declare procedures for working with class tube_options in scheme
//
//----------------------------------------------------------------------

#ifndef tube_typ_hxx
#define tube_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class tube_options;

//======================================================================

logical is_Scm_Tube_Opt(ScmObject);
tube_options *get_Scm_Tube_Opt(ScmObject);
ScmObject make_Scm_Tube_Opt(tube_options *tube_opt);
int get_Scm_Tube_Opt_List(ScmObject list,tube_options **&opts);
logical is_Scm_Tube_Opt_List( ScmObject list );

//======================================================================
#endif
