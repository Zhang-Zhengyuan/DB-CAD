// $Id: sw_opts.hxx,v 1.4 2000/12/26 19:08:47 products Exp $
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
//    declare procedures for working with class sweep_options in scheme
//
//----------------------------------------------------------------------

#ifndef sw_typ_hxx
#define sw_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class sweep_options;

//======================================================================

logical is_Scm_Sweep_Opt(ScmObject);
sweep_options *get_Scm_Sweep_Opt(ScmObject);
ScmObject make_Scm_Sweep_Opt(sweep_options *eq);

//======================================================================
#endif
