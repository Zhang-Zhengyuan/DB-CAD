/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//$Id: //
// Header file for the base options.

#if !defined(BASEOPTS_H)
#define BASEOPTS_H

#include "dcl_base.h"
class option_header;

// base\baseutil\debug\debugmsc.cxx:
// SESSION_GLOBAL_VAR option_header abs_option( "debug_abs#olute_addresses", 0 );
extern DECL_BASE option_header abs_option;

// base\baseutil\errorsys\ctrlc.cxx:
// SESSION_GLOBAL_VAR option_header poll_for_ctrl_c_opt("ctrlc_check", FALSE);
#if defined ( POLL_FOR_CONTROL_C )
extern DECL_BASE option_header poll_for_ctrl_c_opt;
#endif

// base\baseutil\errorsys\errorbase.cxx:
// SESSION_GLOBAL_VAR option_header crash_option( "crash", FALSE );
extern DECL_BASE option_header crash_option;

// base\baseutil\errorsys\intrrupt.cxx:
// SESSION_GLOBAL_VAR option_header stack_check_limit( "stack_check_limit", (int)0);
extern DECL_BASE option_header stack_check_limit;

// base\baseutil\errorsys\intrrupt.cxx:
// SESSION_GLOBAL_VAR option_header stack_check_error( "stack_check_error", TRUE);
extern DECL_BASE option_header stack_check_error;

// base\baseutil\mmgr\mmgr_dbg.cxx:
// SESSION_GLOBAL_VAR option_header mmgr_fill( "mmgrfill", TRUE);
extern DECL_BASE option_header mmgr_fill;

// base\baseutil\mmgr\mmgr_dbg.cxx:
// SESSION_GLOBAL_VAR option_header mmgr_file( "mmgrfile", "mmgr.log");
extern DECL_BASE option_header mmgr_file;

// base\baseutil\mmgr\mmgr_dbg.cxx:
// SESSION_GLOBAL_VAR option_header mmgr_log( "mmgrlog", FALSE);
extern DECL_BASE option_header mmgr_log;

// base\baseutil\rtchecks\stackmon.cxx:
// SESSION_GLOBAL_VAR option_header stacklimit_error( "stacklimit_error", TRUE);
extern DECL_BASE option_header stacklimit_error;

// base\baseutil\timer\timer.cxx:
// SESSION_GLOBAL_VAR option_header timing_option( "timing", FALSE );
extern DECL_BASE option_header timing_option;

// base\baseutil\vector\vector.cxx:
// SESSION_GLOBAL_VAR option_header debug_full_precision("debug_full#_precision", FALSE );
extern DECL_BASE option_header debug_full_precision;


#endif
