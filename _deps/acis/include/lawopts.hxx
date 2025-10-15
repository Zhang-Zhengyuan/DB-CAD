/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//$Id: //
// Header file for the law options.

#if !defined(LAWOPTS_H)
#define LAWOPTS_H

#include "dcl_law.h"
class option_header;

// law\lawutil\law_base.cxx:
// SESSION_GLOBAL_VAR option_header law_mem_check("law_mem_check", FALSE);
extern DECL_LAW option_header law_mem_check;

// law\lawutil\sw_curve_util.cxx:
// SESSION_GLOBAL_VAR option_header fast_euler ("fast_euler", TRUE); 
extern DECL_LAW option_header fast_euler;

#endif
