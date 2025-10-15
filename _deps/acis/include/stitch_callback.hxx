/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _STITCH_CALLBACK__HXX_
#define _STITCH_CALLBACK__HXX_

#include <stdio.h>
#include "dcl_stitch.h"
#include "body.hxx"

// ***********************************************************************
// Functions for STITCH callback
// ***********************************************************************

typedef void (*HH_STITCH_OPT_CALLBACK) (BODY*);

// Function for setting the stitch callback pointer
DECL_STITCH void
hh_set_stitch_opt_callback(HH_STITCH_OPT_CALLBACK funcp);

// Function for getting the stitch callback pointer
DECL_STITCH HH_STITCH_OPT_CALLBACK
hh_get_stitch_opt_callback();


#endif //_STITCH_CALLBACK__HXX_

