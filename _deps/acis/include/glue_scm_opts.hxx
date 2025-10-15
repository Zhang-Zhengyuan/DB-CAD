/* $Id: glue_scm_opts.hxx,v 1.1 2001/07/16 17:45:19 atan Exp $ */

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
//    declare procedures for working with class glue_options in scheme
//
//----------------------------------------------------------------------

#ifndef glue_scm_opts_hxx
#define glue_scm_opts_hxx

#include "scheme.hxx"
#include "logical.h"

class glue_options;


logical			is_Scm_Glue_Opt(ScmObject);
glue_options	*get_Scm_Glue_Opt(ScmObject);
ScmObject		make_Scm_Glue_Opt(glue_options*);



#endif
