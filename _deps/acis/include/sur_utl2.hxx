/* $Id: sur_utl2.hxx,v 1.1 2001/06/05 16:56:39 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// A handy header file for declaring surface utilities, just so that
// they don't all have to get stuffed into other more pervasisve header
// files, such as surdef.hxx or spldef.hxx.

#ifndef SUR_UTL_HEADER 
#define SUR_UTL_HEADER

#include "dcl_kern.h"
#include "logical.h"

class spline;


// Utility to refit a spline approximation to the given fitol and return
// a new spline if this succeeds, or NULL if it fails. In either case,
// the original surface is left entirely untouched.

// If the spline already satisfies the fitol argument, it also returns
// NULL.

// If the regeneration of the approximation is to be *forced*, even if
// the fitol is currently good enough, the final "force" flag should be
// passed as TRUE.

// Code resides in spldef.cxx.

DECL_KERN spline *refit_spline(
					   spline const &s,
					   double fitol,
					   logical force = FALSE
					   );


#endif
