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
//    declare procedures for working with class SPA_spring_back_options in scheme
//
//----------------------------------------------------------------------

#ifndef spring_back_opts_typ_hxx
#define spring_back_opts_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class SPA_spring_back_options;

//======================================================================

logical is_Scm_Spring_Back_Opts(ScmObject);
SPA_spring_back_options* get_Scm_Spring_Back_Opts(ScmObject);
ScmObject make_Scm_Spring_Back_Opts(SPA_spring_back_options* opts);

//======================================================================
#endif
