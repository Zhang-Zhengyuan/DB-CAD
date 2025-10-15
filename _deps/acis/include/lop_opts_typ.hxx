/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Declare procedures for working with class sweep_options in scheme
/*******************************************************************/
#ifndef lop_typ_hxx
#define lop_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class lop_options;

logical      is_Scm_Lop_Opt(ScmObject);
lop_options* get_Scm_Lop_Opt(ScmObject);
ScmObject    make_Scm_Lop_Opt(lop_options *eq);

#endif
