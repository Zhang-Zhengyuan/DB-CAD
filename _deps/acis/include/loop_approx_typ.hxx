// $Id: blend_smoke_test.hxx,v 1.4 2002/01/28 16:53:03 rocon Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef loop_approx_typ_hxx
#define loop_approx_typ_hxx

#include "scheme.hxx"
class loop_approx_output_handle;
class SPA_LOOP_approx_options;

ScmObject make_Scm_loop_approx_handle(loop_approx_output_handle* h);
logical is_Scm_loop_approx_handle(ScmObject so);
loop_approx_output_handle* get_Scm_loop_approx_handle(ScmObject s);

ScmObject make_Scm_loop_approx_options(SPA_LOOP_approx_options const& opts);
logical is_Scm_loop_approx_options(ScmObject so);
SPA_LOOP_approx_options* get_Scm_loop_approx_options(ScmObject s);

#endif
