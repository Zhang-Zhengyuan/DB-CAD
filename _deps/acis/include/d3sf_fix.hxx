// $Id: d3sf_fix.hxx,v 1.2 2002/05/09 23:53:49 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GFIXH 
#define GFIXH

#include "bs3.hxx"
#include "chk_stat.hxx"

// This function makes a surface which is almost singular into a truly 
// singular one.  It modifies the B-spline vertices by a maximum distance of 
// 'max' in three-space, and returns the maximum distance it has moved any 
// vertex. 

DECL_KERN logical bs3_surface_fix_degeneracy( bs3_surface bs3,
									  const check_fix &fix,
									  check_fix &fixed );

#endif
