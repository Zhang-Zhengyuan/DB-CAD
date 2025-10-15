/* $Id: surfchk.hxx,v 1.5 2002/08/09 17:15:27 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SRFCHK_H
#define SRFCHK_H

// History:
//
// 04-mar-1998	pmalrais	New code.
// used to fix 4874 and 4878
//

class surface;
class SPAinterval;

#include "dcl_kern.h"
// ywoo 08Jan01: added the header file.
#include "logical.h"
// ywoo: end

// Check a surface for illegalities of the type where it screws up or
// doubles back on itself. Return TRUE if the curve is ok, FALSE if not.


// This code is taken from the surface checker, but the approximating
// curve, continuity and self-intersection tests are all removed.

DECL_KERN logical check_surface_ok( surface const &surf );


//Check to see if we have a invalid surface. That is the u and the v vectors on 
//the surface are in the same or oppostite direction.
//Returns TRUE: if they are. FALSE: other wise

DECL_KERN logical check_for_bad_surface_u_v (surface *surf);

#endif
