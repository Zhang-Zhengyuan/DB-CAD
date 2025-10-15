/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GLOBAL_UTIL_SUMMARY2_HXX
#define GLOBAL_UTIL_SUMMARY2_HXX

#include "dcl_stitch.h"

DECL_STITCH void
global_message_init();

DECL_STITCH void
global_message_append(char *);

DECL_STITCH void
print_summary();

char*
bhl_print_double(double this_double);

#endif
