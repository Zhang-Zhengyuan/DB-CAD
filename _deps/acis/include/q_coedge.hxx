// $Id: q_coedge.hxx,v 1.5 2002/07/10 11:17:56 nadhikar Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef Q_COEDGE_HXX
#define Q_COEDGE_HXX

#include "dcl_kern.h"
#include "logical.h"
#include "bs3curve.hxx"

class curve;
class COEDGE;
class FACE;

// Returns a curve in the direction of the coedge
DECL_KERN curve *
sg_coedge_curve(
	COEDGE *            // Coedge whose curve is returned
);

DECL_KERN bs3_curve
bs3c_from_coedge(
	const COEDGE* coedge
);

DECL_KERN logical
Vortex_Torus_2( 
	const COEDGE *coed, const FACE *this_face
);

DECL_KERN
COEDGE *last_coedge_in_ring( COEDGE *first_coedge);

#endif
