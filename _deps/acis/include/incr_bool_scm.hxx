/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#pragma once

#include <memory>

#include "scheme.hxx"

class incr_bool_handle;

ScmObject make_Scm_Incr_Bool_Handle(std::shared_ptr<incr_bool_handle> pb);
logical is_Scm_Incr_Bool_Handle(ScmObject so);
std::shared_ptr<incr_bool_handle> get_Scm_Incr_Bool_Handle(ScmObject so);
