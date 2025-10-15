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

#ifndef epd_result_list_type_hxx
#define epd_result_list_type_hxx

#include "scheme.hxx"
#include "logical.h"
#include "SPAepd_result.hxx"

//======================================================================

logical is_Scm_epd_result_list(ScmObject);
SPAepd_result_list* get_Scm_epd_result_list(ScmObject); 
ScmObject make_Scm_epd_result_list(SPAepd_result_list const& list);  // makes shallow copy of input list and attaches it to a scheme object.

//======================================================================
#endif
