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
//    Scheme level interfaces/data types declarations for slice functionality.
//----------------------------------------------------------------------

#ifndef SLICE_SCM_TYP_HXX
#define SLICE_SCM_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

class slice_options;
class slice_output_handle;

//S_Slice_Opt functions

logical					is_Scm_Slice_Options(ScmObject);
slice_options*			get_Scm_Slice_Options(ScmObject);
ScmObject				make_Scm_Slice_Options(slice_options*);

//S_Slice_Hnd functions

logical					is_Scm_Slice_Output_Handle( ScmObject );
slice_output_handle*	get_Scm_Slice_Output_Handle( ScmObject);
ScmObject				make_Scm_Slice_Output_Handle(slice_output_handle*);
void					release_Scm_Slice_Output_Handle( ScmObject );

#endif // SLICE_SCM_TYP_HXX


