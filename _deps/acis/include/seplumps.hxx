/* ORIGINAL: acis2.1/kerndata/seplumps/seplumps.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* @(#)seplumps.hxx	1.1	29 Jan 1996 */

// Utilities for ensuring the validity of the top-level body structure:
// shells should be connected faces and/or wires, lumps should be connected
// point sets.

#include "dcl_eulr.h"

class BODY;

// Check each putative shell of a body, and find whether it represents one or
// more connected sets of faces. If the latter, separate each connected set of
// faces into a new shell. All adjacency information must be present in the
// data structure - no geometric tests are made.

DECL_EULR void separate_body_shells(
			BODY *
		);

// Take a body (which may contain several lumps) and sort out the
// shells of each lump into possibly several disjoint lumps, each with
// possible voids. It is assumed that the shells of each original
// lump can be separated out in this way - that is each void shell is
// inside at least one peripheral shell, and no peripheral shell is
// inside another unless it is also inside a void which is inside the
// other.

DECL_EULR void separate_body_lumps(
			BODY *
		);
