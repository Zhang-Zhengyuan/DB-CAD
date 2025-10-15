/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Declare Edm Offset Type
/*******************************************************************/
#ifndef edm_offset_typ_hxx
#define edm_offset_typ_hxx

#include "scheme.hxx"
#include "logical.h"
#include "edm_offset_opts.hxx"


logical      is_Scm_Edm_Offset_Opt(ScmObject);
edm_offset_options* get_Scm_Edm_Offset_Opt(ScmObject);
ScmObject    make_Scm_Edm_Offset_Opt(edm_offset_options *eq);

#endif
