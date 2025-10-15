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
//    Scheme level interfaces/data types declarations for remove functionality.
//----------------------------------------------------------------------

#ifndef REM_SCM_TYP_HXX
#define REM_SCM_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

class local_remove_options;
class local_remove_output_handle;

//S_Local_Remove_Opt functions

logical					is_Scm_Local_Remove_Opt(ScmObject);
local_remove_options*			get_Scm_Local_Remove_Opt(ScmObject);
ScmObject				make_Scm_Local_Remove_Opt(local_remove_options*);

//S_Local_Remove_Hnd functions

logical					is_Scm_Local_Remove_Hnd ( ScmObject );
local_remove_output_handle*	get_Scm_Local_Remove_Hnd ( ScmObject);
ScmObject				make_Scm_Local_Remove_Hnd (local_remove_output_handle*);
void					release_Scm_Local_Remove_Hnd ( ScmObject );

#endif // REM_SCM_TYP_HXX


