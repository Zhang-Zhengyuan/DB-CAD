/* ORIGINAL: acis2.1/sg_husk/make/mk_body.hxx */
/* $Id: mk_body.hxx,v 1.5 2000/12/26 18:30:52 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined (SG_MAKE_BODY)
#define SG_MAKE_BODY

#include "dcl_cstr.h"

class BODY;
class FACE;

// Make a LUMP and a SHELL for each FACE in the list of faces and insert
// them on the list of lumps on the given body (if not NULL) or create
// a new body and append them (if given body is NULL). Return the body
// that the new lumps are placed on.

DECL_CSTR BODY*
sg_mk_by_faces(
	BODY*	given_body,
	int		num_faces,
	FACE*	faces[]
	);

#endif
