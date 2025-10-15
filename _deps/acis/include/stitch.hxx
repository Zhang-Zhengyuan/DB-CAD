/* ORIGINAL: acis2.1/sg_husk/stitch/stitch.hxx */
/* $Id: stitch.hxx,v 1.9 2000/12/26 18:26:42 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef STITCH_HXX
#define STITCH_HXX

#include "dcl_bool.h"
#include "logical.h"

class BODY;
class ENTITY_LIST;
class COEDGE;
class EDGE;

//-------------------------------------------------------------------
//	Main stitching routine
//-------------------------------------------------------------------
DECL_BOOL logical
sg_stitch(
			BODY*,	// Body 1 (will persist)
			BODY*,	// Body 2 (can be same as #1)
			logical
		);

//-------------------------------------------------------------------
//-------------------------------------------------------------------
DECL_BOOL logical
imprint_and_stitch(
			BODY*,	// Body 1 (will persist)
			BODY*	// Body 2 (can be same as #1)
		);

//-------------------------------------------------------------------
//	Alternate stitching routine if coedge partners already
// 		saved in a list
//-------------------------------------------------------------------
DECL_BOOL logical
sg_stitch_with_list(
			BODY*,	// Body 1 (will persist)
			BODY*,	// Body 2 (can be same as #1)
			ENTITY_LIST&,	// List of coedges from 1
			ENTITY_LIST&	// List of coedges from 2
		);

//-------------------------------------------------------------------
//	Coedge partner routine, called directly from sweep
//-------------------------------------------------------------------
DECL_BOOL logical
sg_make_partners(
			COEDGE*,		// coedge 1
			COEDGE*,		// coedge 2
			BODY*,			// Body of coedge 2
			logical = FALSE	// Needs be true for external call
		);

//---------------------------------------------------------------
//	sg_potential_partners -	check if two coedges could be 
//	partners, i.e. if they have	identical edge geometry (forward
//	or reversed...check for bad	orientations later).
//---------------------------------------------------------------
DECL_BOOL logical
sg_potential_partners( 
		EDGE* e1,	
		EDGE* e2,
		ENTITY_LIST& verts1
	);

//-----------------------------------------------------------------
//  unstitch_nonmanifold - non-manifold body in, manifold bodies out
//      self-non-manifold edges are not decomposed.
//-----------------------------------------------------------------
DECL_BOOL void
unstitch_nonmanifold(
				BODY *in,		// input body, possibly NM
				BODY*&lumps,	// manifold lumps only here
				BODY*&sheets,	// manifold sheets (in lumps)
				BODY*&lamina,	// all lamina in lumps
				BODY*&wires		// wires made 2-manifold
		);

#endif
