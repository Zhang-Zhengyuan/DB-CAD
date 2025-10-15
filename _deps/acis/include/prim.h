/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __PRIM_H__
#define __PRIM_H__

#include "AcisScheme.hxx"
#include "kernel.h"


struct Prim_Init {
    ScmObject (*fun)();
    char *name;
    int minargs, maxargs;
    enum discipline disc;
};

#ifdef __cplusplus
extern "C" {
#endif

struct Prim_Init* Get_Scm_Primitives();

#ifdef __cplusplus
}
#endif

#endif
