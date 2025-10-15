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
//    Scheme level interfaces/data types declarations for model comparison functionality.
//----------------------------------------------------------------------

#ifndef MODEL_COMPARISON_SCM_HXX
#define MODEL_COMPARISON_SCM_HXX

#include "scheme.hxx"

class model_comparison_options;
class model_comparison_results;

// Scheme functions related to model_comparison_options
logical is_Scm_Model_Comparison_Options(ScmObject r);
ScmObject make_Scm_Model_Comparison_Options(model_comparison_options* mc_opt);
model_comparison_options* get_Scm_Model_Comparison_Options(ScmObject r);


// Scheme functions related to model_comparison_results
logical is_Scm_Model_Comparison_Results(ScmObject r);
void release_Scm_Model_Comparison_Results(ScmObject r);
model_comparison_results* get_Scm_Model_Comparison_Results(ScmObject r);
ScmObject make_Scm_Model_Comparison_Results(model_comparison_results* model_comp_res);

#endif
