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
//    declare procedures for working with class bool_options in scheme
//
//----------------------------------------------------------------------

#ifndef bool_scm_opts_hxx
#define bool_scm_opts_hxx

#include "scheme.hxx"
#include "logical.h"

class BoolOptions;


logical     is_Scm_Bool_Opt(ScmObject);
BoolOptions *get_Scm_Bool_Opt(ScmObject);
ScmObject   make_Scm_Bool_Opt(BoolOptions*);

class bool_match_info;

logical			is_Scm_Match_Info(ScmObject);
bool_match_info	*get_Scm_Match_Info(ScmObject);
ScmObject		make_Scm_Match_Info(bool_match_info*);

#endif

