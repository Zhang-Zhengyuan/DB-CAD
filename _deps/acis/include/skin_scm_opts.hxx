// $Id: skin_scm_opts.hxx,v 1.3 2000/12/26 19:06:26 products Exp $
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
//    declare procedures for working with class skin_options in scheme
//
//----------------------------------------------------------------------

#ifndef skin_typ_hxx
#define skin_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class skin_options;

//======================================================================

logical			is_Scm_Skin_Opt(ScmObject);
skin_options	*get_Scm_Skin_Opt(ScmObject);
ScmObject		make_Scm_Skin_Opt(skin_options *eq);


//======================================================================
#endif
