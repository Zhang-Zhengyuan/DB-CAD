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
//    Define detect_match_input_handle and detect_match_output_handle scheme object
//----------------------------------------------------------------------

#ifndef match_handle_typ_hxx
#define match_handle_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class detect_match_input_handle;
class detect_match_output_handle;
class detect_match_body_criteria;

//======================================================================

logical is_Scm_Detect_Match_Input_Handle(ScmObject);
detect_match_input_handle* get_Scm_Detect_Match_Input_Handle(ScmObject);
ScmObject make_Scm_Detect_Match_Input_Handle(detect_match_input_handle*);

//======================================================================

logical is_Scm_Detect_Match_Output_Handle(ScmObject);
detect_match_output_handle* get_Scm_Detect_Match_Output_Handle(ScmObject);
ScmObject make_Scm_Detect_Match_Output_Handle(detect_match_output_handle*);

//======================================================================

logical is_Scm_Detect_Match_Body_Criteria(ScmObject);
detect_match_body_criteria* get_Scm_Detect_Match_Body_Criteria(ScmObject);
ScmObject make_Scm_Detect_Match_Body_Criteria(detect_match_body_criteria* );

//======================================================================

#endif // match_handle_typ_hxx
