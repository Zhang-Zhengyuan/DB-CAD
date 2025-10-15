/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef outcome_typ_hxx
#define outcome_typ_hxx

#include "logical.h"
#include "scheme.hxx"

class outcome;

logical is_Scm_Outcome(ScmObject);
outcome& get_Scm_Outcome(ScmObject);
ScmObject make_Scm_Outcome(const outcome&);

#endif
