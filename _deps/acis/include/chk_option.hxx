/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#include "dcl_kern.h"
#include "option.hxx"

// STL apc 04Feb04 This is a temporary option to control the operation of the
// new checks being added for R14. The plan is that it will become on by
// default in R16, and removed entirely (checks will be permanently on)
// in R17.
// STL ajr 14Jun05 Switched r14_checks to TRUE by default for R16 release.
extern DECL_KERN option_header r14_checks;

// 3DPLM/RCW, 19 Jun 2007, CODE STARTS
// Project : Improve checker project (ICP).
// Description : This is a temporary option to control the operation of the
// new gap checking function for R18.
extern DECL_KERN option_header check_edge_gaps;

// DM 14Oct08
// Option to control checking in API calls (via check_body)
extern DECL_KERN option_header api_low_level_checking;


