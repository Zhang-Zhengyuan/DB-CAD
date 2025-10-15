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
//    define procedures for working with clash options in scheme
//
//----------------------------------------------------------------------

#ifndef CLASH_SCM_OPTS_HXX
#define CLASH_SCM_OPTS_HXX

#include "logical.h"

class body_clash_options;

logical is_Scm_Clash_Body_Opts(ScmObject);
body_clash_options *get_Scm_Clash_Body_Opts(ScmObject);
ScmObject make_Scm_Clash_Body_Opts(body_clash_options *body_clash_opts);

class face_clash_options;

logical is_Scm_Clash_Face_Opts(ScmObject);
face_clash_options *get_Scm_Clash_Face_Opts(ScmObject);
ScmObject make_Scm_Clash_Face_Opts(face_clash_options *face_clash_opts);

// 25-June-2021 sse24 - Scheme interfaces for the n-body clash results object
class n_body_clash_results;
logical is_Scm_N_Body_Clash_Results(ScmObject r);
n_body_clash_results* get_Scm_N_Body_Clash_Results(ScmObject r);
ScmObject make_Scm_N_Body_Clash_Results(n_body_clash_results* clash_res);
void release_Scm_N_Body_Clash_Results(ScmObject r);

#endif
