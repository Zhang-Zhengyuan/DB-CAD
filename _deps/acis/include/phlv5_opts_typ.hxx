/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// ----------------------------------------------------------------------------
// purpose---
//    Define scheme data type for phlv5 data
//-----------------------------------------------------------------------------
#ifndef phlv5_opts_typ_hxx
#define phlv5_opts_typ_hxx

#include "logical.h"
#include "scheme.hxx"

class GC_Phlv5_Opts;
class phlv5_options;

struct S_Phlv5_Options {
	ScmObject tag;
	GC_Phlv5_Opts *The_GC_Phlv5_Options;
};

#define PHLV5_OPTIONS_PTR(obj) (((struct S_Phlv5_Options*)POINTER(obj))->The_GC_Phlv5_Options->The_Data)


//----------------------------------------------------------------------
// Purpose---
//    Determine if a scheme object is a phlv5_options
//----------------------------------------------------------------------
logical
is_Scm_Phlv5_Options(
    ScmObject p
    );

//----------------------------------------------------------------------
// Purpose---
//   Get a phlv5_options from a scheme phlv5_options
//----------------------------------------------------------------------
phlv5_options*
get_Scm_Phlv5_Options(
	ScmObject phlv5d
	);

//----------------------------------------------------------------------
// Purpose---
//   Create a scheme phlv5_options from a phlv5_options
//
// NOTE:
//   This routine does not copy the phlv5_options.  When the phlv5_options
//   is no longer accessible from scheme, the scheme garbage collector
//   will lose it.  Since the phlv5_options is not copied, it must not be
//   deleted in the procedure that calls this procedure.
//----------------------------------------------------------------------
ScmObject make_Scm_Phlv5_Options(
    phlv5_options* c_phlv5d
    );

#endif
