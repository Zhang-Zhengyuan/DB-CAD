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
#ifndef facet_typ_hxx
#define ffacet_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class facet_options;

logical      is_Scm_Facet_Opt(ScmObject);
//logical      is_Scm_Facet_Opt_Visualization(ScmObject);
//logical      is_Scm_Facet_Opt_Seed(ScmObject);
//logical      is_Scm_Facet_Opt_Precise(ScmObject);
//logical      is_Scm_Facet_Opt_Deprecated(ScmObject);
facet_options* get_Scm_Facet_Opt(ScmObject);
ScmObject    make_Scm_Facet_Opt(facet_options *eq);

#endif
