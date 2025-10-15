/* ORIGINAL: acis2.1/sg_husk/cover/cover.hxx */
/* $Id: cover.hxx,v 1.11 2002/04/19 14:31:34 sallen Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for cover routines.

// These routines cover closed wires to make sheets, and sheets to
// make solids.

#if !defined( COVER_HDR_DEF )
#define  COVER_HDR_DEF

#include "dcl_covr.h"
// ywoo 08Jan01: added the header file.
#include "logical.h"
// ywoo: end

class BODY;
class WIRE;
class FACE;
class LOOP;
class LUMP;
class COEDGE;

class ENTITY_LIST;

class surface;
class generic_graph;


// Set surface if given or attempt to find a plane that contains
// loops of given face.  Re-arrange loops into properly connected
// faces.

DECL_COVR logical
apply_surface( FACE*, surface const *, ENTITY_LIST& );


// Routine to cover a circuit of connected edges.  It adds a
// new coedge to each edge (unless the edge is a wire when the
// existing loopless coedge is used - and may be reversed) and 
// places each new coedge in a loop, newly made, which is returned.
// Orients coedges forward along the sequence of edges and checks 
// that the given edges are connected.

DECL_COVR LOOP*
cover_circuit(	ENTITY_LIST& );


// Routine to cover a collection of circuits of connected edges.  
// It makes a new loop for each circuit and places the new loops 
// into a new face.  If the pointer to a (lower_case) surface 
// is not NULL, makes an upper-case surface and sets it in 
// the face with sense FORWARD.  All edges must belong to one
// body.  The new face is added to the first shell of the body.
// The routine may be used to cover wire or sheet bodies a face 
// at a time or to make internal faces for cellular bodies.

DECL_COVR void
cover_circuits(	int,
				ENTITY_LIST*[],
				surface const *,
				ENTITY_LIST& );


//	Routine to find and cover all simple circuits of external 
//	edges of a sheet.  Returns an entity list of new faces 
//	made (will be empty if no new faces are made).  New faces
//	will have no surfaces if none can be found to fit their edges
//	and vertices.  
//	If surface found or given is planar, re-arranges loops into
//	connected faces.
//	NB: Does not cover wire edges.

// STI ywoo (5/4/00): new option "multiple_cover"
DECL_COVR void
cover_sheet( BODY*, surface const *, ENTITY_LIST& ,logical = FALSE);
// STI ywoo: end

// Cover one or more closed slice-wires.  Slice-wires are made by 
// the first phase of boolean operations.  They carry attributes
// giving inside-outside data for each coedge (two coedges per edge).
// After covering, these attributes are removed, and each edge has
// only one coedge.

DECL_COVR void
cover_slice_wires( BODY*, surface const *, ENTITY_LIST& );


// Routine to cover a set of closed wires supplied as the wires of
// a wire body, with one or more faces.  It attempts to find a 
// surface from the vertices and edges of the wires;
// if none is found, the surface(s) of the face(s) is left NULL.
// Returns a list of faces made.
// NB: the routine assumes the wires are co-planar and closed and
// correctly oriented (metal on the left of their coedges).

DECL_COVR void
cover_wires( BODY*, surface const *, ENTITY_LIST&, logical = FALSE);


// Routine to cover a single closed wire with a faces.  It attempts to find a 
// surface from the vertices and edges of the wire;
// if none is found, the surface of the face is left NULL.
// Returns the face made.
// NB: the routine assumes the wire is planar and closed and
// correctly oriented (metal on the left of the coedges).

DECL_COVR void
cover_wire( WIRE*, surface const *, FACE *& );

DECL_COVR void 
cover_loops_of_planar_wires(ENTITY_LIST& wires, BODY *&sheet);

DECL_COVR logical
edges_on_surface(const ENTITY_LIST& edges, surface *surf);

#endif
