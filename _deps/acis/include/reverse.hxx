/* ORIGINAL: acis2.1/kerndata/reverse/reverse.hxx */
/* $Id: reverse.hxx,v 1.7 2000/12/26 18:47:00 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Function to reverse the sense of a face, flipping all the coedge
// senses and loop order. The most awkward bit of this is to ensure
// that shared pcurves are handled correctly. This part of the task is
// split out as a separate operation, for use in other circumstances
// where all the pcurves on a face are to be modified.

#if !defined( REVERSE_HDR_DEF )
#define  REVERSE_HDR_DEF

#include "dcl_kern.h"

class FACE;
class LOOP;
class EDGE;

DECL_KERN void reverse_face( FACE *	);

DECL_KERN void reverse_loop( LOOP * );

DECL_KERN void reverse_edge( EDGE * );

// Go round all the coedges of a face, and ensure that all pcurves
// are unshared, or shared only by other coedges of the same face,
// by copying any which are shared by other things, and changing
// the coedges to point to the copies.

DECL_KERN void unshare_pcurves(
			FACE *
		);

#endif
