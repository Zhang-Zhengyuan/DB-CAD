// $Id: ac_opts.hxx,v 1.1 2002/07/11 14:41:26 btomas Exp $
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
//    declare procedures for working with class split_and_smooth_disc_options in scheme
//
//----------------------------------------------------------------------

#ifndef split_typ_hxx
#define split_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class split_and_smooth_disc_options;

//======================================================================

logical                         is_Scm_SplitSmooth_Opt(ScmObject);
split_and_smooth_disc_options *get_Scm_SplitSmooth_Opt(ScmObject);
ScmObject                     make_Scm_SplitSmooth_Opt(split_and_smooth_disc_options *eq);

//======================================================================
#endif
