/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Declare Edm Tool Motion Type
/*******************************************************************/
#ifndef EDM_TOOL_MOTION_TYP_HXX
#define EDM_TOOL_MOTION_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

class edm_tool_motion;

logical is_Scm_Edm_Tool_Motion( ScmObject );
edm_tool_motion* get_Scm_Edm_Tool_Motion( ScmObject );
ScmObject make_Scm_Edm_Tool_Motion( edm_tool_motion* tool_motion );

#endif
