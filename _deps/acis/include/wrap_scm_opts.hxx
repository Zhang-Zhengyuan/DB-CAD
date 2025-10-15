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
//    declare procedures for working with wrap_options class in scheme
//
//---------------------------------------------------------------------------

#ifndef WRAP_SCM_OPTS_HXX
#define WRAP_SCM_OPTS_HXX

#include "logical.h"
#include "scheme.hxx"

class wrap_options;
class wrap_orientation_ref;

logical is_Scm_Wrap_Opt( ScmObject );
wrap_options* get_Scm_Wrap_Opt( ScmObject );
ScmObject make_Scm_Wrap_Opt( wrap_options* );

logical is_Scm_Wrap_Orientation_Ref( ScmObject );
wrap_orientation_ref* get_Scm_Wrap_Orientation_Ref( ScmObject );
ScmObject make_Scm_Wrap_Orientation_Ref( wrap_orientation_ref* );

#endif
