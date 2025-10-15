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
//    Scheme level interfaces/data types declarations for repair planar slices functionality.
//----------------------------------------------------------------------

#ifndef REPAIR_PSLICES_SCM_TYP_HXX
#define REPAIR_PSLICES_SCM_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

class repair_pslices_options;
class repair_pslices_output_handle;


// Functions related to scheme type for repair_pslices_options

logical					is_Scm_Repair_Pslices_Options(ScmObject);
repair_pslices_options*	get_Scm_Repair_Pslices_Options(ScmObject);
ScmObject				make_Scm_Repair_Pslices_Options(repair_pslices_options*);

// Functions related to scheme type for repair_pslices_output_handle

logical							is_Scm_Repair_Pslices_Output_Handle(ScmObject);
repair_pslices_output_handle*	get_Scm_Repair_Pslices_Output_Handle(ScmObject);
ScmObject						make_Scm_Repair_Pslices_Output_Handle(repair_pslices_output_handle*);
void							release_Scm_Repair_Pslices_Output_Handle(ScmObject );

#endif // REPAIR_PSLICES_SCM_TYP_HXX
