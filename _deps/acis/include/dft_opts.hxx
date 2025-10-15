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
//    declare procedures for working with class defeature_options in scheme
//
//----------------------------------------------------------------------

#ifndef dft_typ_hxx
#define dft_typ_hxx

#include "scheme.hxx"
#include "logical.h"
#include "acis_defeature_options.hxx"
#include "trace.hxx"
#include "gc_obj.hxx"

class defeature_options;

//======================================================================

logical is_Scm_Defeature_Opt(ScmObject);
defeature_options *get_Scm_Defeature_Opt(ScmObject);
ScmObject make_Scm_Defeature_Opt(defeature_options *eq);

//======================================================================
#endif
