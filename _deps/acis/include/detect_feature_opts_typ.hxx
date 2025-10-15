/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Declare procedures for working with class sweep_options in scheme
/*******************************************************************/
#ifndef DETECT_FEATURE_OPTS_TYP_HXX
#define DETECT_FEATURE_OPTS_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

//*******************************************************************
//@ Forward declaration
class detect_feature_options;
//*******************************************************************

// Following functions are used by schemes
logical      is_Scm_Detect_Ftr_Opts(ScmObject);
detect_feature_options* get_Scm_Detect_Ftr_Opts(ScmObject);

//*******************************************************************
//@ Forward declaration
class detect_blend_options;
//*******************************************************************

// Following functions are used by schemes
logical      is_Scm_Df_Blend_Opt(ScmObject);
detect_blend_options* get_Scm_Df_Blend_Opt(ScmObject);

//*******************************************************************
//@ Forward declaration
class detect_match_options;
//*******************************************************************

// Following functions are used by schemes
logical      is_Scm_Detect_Match_Opt(ScmObject);
detect_match_options* get_Scm_Detect_Match_Opt(ScmObject);

//*******************************************************************
//@ Forward declaration
class detect_chamfer_options;
//*******************************************************************

// Following functions are used by schemes
logical      is_Scm_Df_Chamfer_Opt(ScmObject);
detect_chamfer_options* get_Scm_Df_Chamfer_Opt(ScmObject);

//*******************************************************************
#endif // DETECT_FEATURE_OPTS_TYP_HXX
