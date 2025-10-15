/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Declare procedures for working with class facet_options in scheme
/*******************************************************************/
#ifndef align_typ_hxx
#define align_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class align_options;

logical      is_Scm_Align_Opt(ScmObject);
align_options* get_Scm_Align_Opt(ScmObject);
ScmObject    make_Scm_Align_Opt(align_options *eq);

#endif
