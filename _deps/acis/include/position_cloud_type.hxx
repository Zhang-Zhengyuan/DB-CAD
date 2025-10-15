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
//   declare acis_options object for scheme
//
//----------------------------------------------------------------------

#ifndef position_cloud_type_hxx
#define position_cloud_type_hxx

#include "scheme.hxx"
#include "logical.h"
#include "SPAposition_cloud.hxx"

//======================================================================

logical is_Scm_Position_Cloud(ScmObject);
SPAposition_cloud* get_Scm_Position_Cloud(ScmObject); 
ScmObject make_Scm_Position_Cloud(SPAposition_cloud *position_cloud);

//======================================================================
#endif
