/*******************************************************************/
/*    Copyright (c) 1989-2021 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef scm_err_info_hxx
#define scm_err_info_hxx

#include "logical.h"
#include "scheme.hxx"

class error_info_base;

logical is_Scm_Error_Info_Base(ScmObject r);
error_info_base* get_Scm_Error_Info_Base(ScmObject r);
ScmObject make_Scm_Error_Info_Base(error_info_base* err_obj);
const char* get_Severity_Message(int err_info_severity);

#endif
