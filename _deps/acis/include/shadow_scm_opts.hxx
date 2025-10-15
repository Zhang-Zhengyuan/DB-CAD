/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//---------------------------------------------------------------------------
// purpose---
//    declare procedures for working with make_shadow_options class in scheme
//
//---------------------------------------------------------------------------

#ifndef shadow_scm_opts_hxx
#define shadow_scm_opts_hxx

#include "logical.h"
#include "scheme.hxx"

class make_shadow_options;

logical is_Scm_Shadow_Opt( ScmObject );
make_shadow_options* get_Scm_Shadow_Opt( ScmObject );
ScmObject make_Scm_Shadow_Opt( make_shadow_options* );

#endif
