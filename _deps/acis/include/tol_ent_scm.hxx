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
//    declare procedures for working with class tolerize_entity_opts in scheme
//
//----------------------------------------------------------------------

#ifndef tol_ent_typ_hxx
#define tol_ent_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class tolerize_entity_opts;

//======================================================================

logical is_Scm_TolEnt_Opt(ScmObject);
tolerize_entity_opts *get_Scm_TolEnt_Opt(ScmObject);
ScmObject make_Scm_TolEnt_Opt(tolerize_entity_opts *);

//======================================================================
#endif
