/* ORIGINAL: acis2.1/kernbool/merge/merge.hxx */
/* $Id: merge.hxx,v 1.13 2002/08/09 17:22:09 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Interface routines for merging bodies.

#if !defined( MERGE_HEADER )
#define MERGE_HEADER

#include "dcl_bool.h"
#include "logical.h"
#include "base.hxx"

class ENTITY;
class BODY;
class WIRE;
class LUMP;
class SHELL;
class FACE;
class EDGE;
class VERTEX;
class SURFACE;
class ENTITY_LIST;

// Slightly useful utility function to test two surfaces for equality
// for the purposes of merging. Always errs on the safe side, saying
// "not equal" if there is any doubt.

DECL_BOOL logical same_surfaces(
			SURFACE *,
			logical,		// TRUE if the reverse sense of the first
							// surface is of interest
			SURFACE *,
			logical	,		// TRUE if the reverse sense of the second
							// surface is of interest
			double = SPAresnor, // angular tolerance
			logical = TRUE   // use old spline comparison
		);

DECL_BOOL void sg_join_two_edges_about_vertex(
	VERTEX *join_vertex,
	ENTITY_LIST &edges,
	EDGE * &resulting_edge,
	ENTITY_LIST &lost_list,
	logical join_c1
	);

// ***************************************************************************
//
// Function Deprecation Note:
// The declarations for merge_edge_list and merge_vertex_list were moved from 
// merge.hxx to merge_prot.hxx in Mar 2017, then moved back to merge.hxx in 
// Aug 2017 at the request of a customer. (170803-000009)
//
// These two functions are NOT recommended for general use by customers.
//
// ***************************************************************************

// Optional test functions that can be provided to be called whenever
// two faces are to be merged to determine which is to be retained.
// Each function should return TRUE to retain the first argument face,
// FALSE to retain the second.

typedef logical (*select_face)( FACE *, FACE * );

// Same to choose which of two edges to keep.

typedef logical (*select_edge)( EDGE *, EDGE * );


DECL_BOOL void 
merge_edge_list(
			ENTITY_LIST &,					// edge list
			ENTITY_LIST &,					// constructed vertex list
			select_face = NULL,
			double = SPAresnor,				// angular tolerance
			int keep_second_face_flag = -1,	// way to pick faces to keep, 
                                           // -1 means unset 
                                           //  0 means keep first,
                                           //  1 means keep second
			double aggressive_merge_tol = 0.0 /* SPAresabs */	// tolerance for agressive merging. If zero,
																// aggressive merging does not occur.
		);

DECL_BOOL void 
merge_vertex_list(
			ENTITY_LIST &,	// vertex list
			select_edge = NULL,
			ENTITY_LIST & = *(ENTITY_LIST *) NULL_REF,	// lost entities
			double = SPAresnor, // angular tolerance
			logical tolerant_test = TRUE
		);

#endif
