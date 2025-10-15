/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (HH_STRUCTS2_HXX)
#define HH_STRUCTS2_HXX

#include "dcl_stitch.h"

//----------------------------------------------------------------------------

// RNC: 16 Jun 99 enum for body healing status is defined, which is a data member in
// respective aggregate attribute class.
enum MODULE_HEAL_STATUS
{
	DID_NOT_ENTER = 0,
	ANALYSIS = 1,
	CALCULATE = 2,
	FIX = 3,
	COMPLETE = 4
};
//
#endif
