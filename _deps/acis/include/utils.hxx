/* ORIGINAL: acis2.1/sg_husk/utils/utils.hxx */
/* $Id: utils.hxx,v 1.9 2000/12/26 18:30:57 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


// Prototypes for some utility routines that might be usefull
// else where in the code.

#if !defined(SG_UTILS_HDR_DEF)
#define SG_UTILS_HDR_DEF

#include "dcl_cstr.h"

class BODY;

// Routine which converts all the faces in a body in to double sided
// faces.
DECL_CSTR void
sg_body_to_2d(
	BODY*			// Input body
);

// Routine which converts all the faces in a body in to single sided
// faces.
DECL_CSTR void
sg_body_to_1d(
	BODY*			// Input body
);

#endif
