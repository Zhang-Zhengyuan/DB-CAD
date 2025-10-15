/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifdef mac
#ifdef logical
#define logical_was_defined 1
#undef logical
#endif

#define FileInfo mac_FileInfo
#ifdef MacX
#include <Carbon/Carbon.h>
#else
#include <Carbon.h>
#endif

#undef check
#undef FileInfo

#ifdef logical_was_defined
#undef logical_was_defined
#include "logical.h"
#endif

#endif

