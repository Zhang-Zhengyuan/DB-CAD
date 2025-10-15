/*******************************************************************/
/*    Copyright (c) 2008-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// purpose---
//    declare procedures for working with class cover_options in scheme
//
//----------------------------------------------------------------------

#ifndef COVER_OPTIONS_SCM_HXX
#define COVER_OPTIONS_SCM_HXX

#include "scheme.hxx"
#include "logical.h"

class cover_options;

//======================================================================

logical is_Scm_Cover_Opt(ScmObject);
cover_options *get_Scm_Cover_Opt(ScmObject);
ScmObject make_Scm_Cover_Opt(cover_options *);

//======================================================================
#endif
