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

#ifndef IMPRINT_SCM_TYP_HXX
#define IMPRINT_SCM_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

class imprint_options;
class imprint_output_handle;


// Functions related to scheme type for imprint_options

logical					is_Scm_Imprint_Options(ScmObject);
imprint_options*		get_Scm_Imprint_Options(ScmObject);
ScmObject				make_Scm_Imprint_Options(imprint_options*);

// Functions related to scheme type for imprint_output_handle

logical					is_Scm_Imprint_Output_Handle(ScmObject);
imprint_output_handle*	get_Scm_Imprint_Output_Handle(ScmObject); // uq2 12/14/17 - behaves like a weak pointer, no use-count increment
ScmObject				make_Scm_Imprint_Output_Handle(imprint_output_handle*);
//void					release_Scm_Imprint_Output_Handle(ScmObject ); // uq2 12/14/17 - unused and no real need

#endif // IMPRINT_SCM_TYP_HXX


