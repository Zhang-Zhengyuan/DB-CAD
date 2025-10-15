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
//    declare procedures for working with class decomp_options in scheme
//
//----------------------------------------------------------------------

#ifndef decomp_scm_opts_hxx
#define decomp_scm_opts_hxx

#include "scheme.hxx"
#include "logical.h"

class decomp_options;

logical			 is_Scm_Decomp_Opt(ScmObject);
decomp_options	*get_Scm_Decomp_Opt(ScmObject);
ScmObject		 make_Scm_Decomp_Opt(decomp_options*);

#endif
