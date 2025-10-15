/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SAVESTAT
#define SAVESTAT

#include <stdio.h>
#include "dcl_kern.h"
#include "logical.h"

// These two function save and retrieve the current state of the modeller 
// to or from a text file.
// The current state consists of option settings and global tolerance values.

DECL_KERN logical save_state(FILE *fptr);
DECL_KERN logical load_state(FILE *fptr);

#endif
