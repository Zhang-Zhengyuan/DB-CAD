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
#ifndef api_aprox_typ_hxx
#define api_aprox_typ_hxx

class SPA_EDGE_approx_options;
class SPA_COEDGE_approx_options;

#include "scheme.hxx"
#include "logical.h"


logical is_Scm_EDGE_approx_options(ScmObject r);
logical is_Scm_COEDGE_approx_options(ScmObject r);

SPA_EDGE_approx_options* get_Scm_EDGE_approx_options(ScmObject r);
SPA_COEDGE_approx_options* get_Scm_COEDGE_approx_options(ScmObject r);

ScmObject make_Scm_EDGE_approx_options(SPA_EDGE_approx_options *approxOpts);
ScmObject make_Scm_COEDGE_approx_options(SPA_COEDGE_approx_options *approxOpts);


#endif
