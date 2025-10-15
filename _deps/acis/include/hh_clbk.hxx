/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "dcl_stitch.h"

//-------------------------------
// Functions for abort callback
//-------------------------------

// Function for setting the Callback Function pointer for Abort
typedef int (*bhl_FuncPtr) (void);

DECL_STITCH int
hh_set_callback_function (bhl_FuncPtr bhl_Callbackfn);

// Function for getting the current value of the callback abort function
DECL_STITCH bhl_FuncPtr
hh_get_callback_function (void);


//--------------------------------------------------------------------------

