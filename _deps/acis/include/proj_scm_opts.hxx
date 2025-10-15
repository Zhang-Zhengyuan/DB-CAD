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
//    declare procedures for working with class project_options in scheme
//
//----------------------------------------------------------------------

#ifndef proj_scm_opts_hxx
#define proj_scm_opts_hxx

#include "scheme.hxx"
#include "logical.h"

class project_options;


logical     is_Scm_Proj_Opt(ScmObject);
project_options *get_Scm_Proj_Opt(ScmObject);
ScmObject   make_Scm_Proj_Opt(project_options*);

#endif

