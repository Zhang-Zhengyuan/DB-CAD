/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// Declare procedures for working with class make_sweep_path_options
// in Scheme.
//----------------------------------------------------------------------

#ifndef msp_opts_hxx
#define msp_opts_hxx

#include "scheme.hxx"
#include "logical.h"

class make_sweep_path_options;

//======================================================================

logical is_Scm_Sweep_Path_Opt( ScmObject );
make_sweep_path_options* get_Scm_Sweep_Path_Opt( ScmObject );
ScmObject make_Scm_Sweep_Path_Opt( make_sweep_path_options* );

//======================================================================
#endif
