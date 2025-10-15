/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define routines to interogate a vertex.
// STL nay 15 Nov 2007: Added Vortex_Torus_1 with one degenerate edge.
// STL vds 06 Jan 2004: Add transformations for the geometry (73455).


#if !defined (Q_VERT_HXX)
#define Q_VERT_HXX

#include "dcl_kern.h"
// ywoo 08Jan01: added the header file and the class.
#include "logical.h"
class ENTITY_LIST;
// ywoo: end
class SPAposition;
class SPAtransf;
class VERTEX;
class COEDGE;
class FACE;
class EDGE;


//STI let: added for ct_husk, sanity, stitching, etc... 
// Routine which determines if two coedges are in the same group
// about this vertex.  

DECL_KERN logical
same_edge_group(
		VERTEX *,				// Given vertex
		COEDGE *, 				// Coedge defining group
		COEDGE * 				// Coedge being tested
	);

// Stricter version of same_edge_group
DECL_KERN logical
strict_same_edge_group(
    EDGE    *e1,
    logical  fwd1,
    EDGE    *e2,
    logical  fwd2
    );

// Return a list of edges which share the given vertex.
// Only edges reachable from the vertex via edges and
// coedges are returned.

DECL_KERN void
sg_q_edges_around_vertex(
	VERTEX*			v,
	ENTITY_LIST*	edge_list
	);

// Return a list of coedges which share the given vertex.
// Only coedges reachable from the vertex via edges and
// coedges are returned.

DECL_KERN void
sg_q_coedges_around_vertex(
	VERTEX*			v,
	ENTITY_LIST*	coedge_list
	);


DECL_KERN void
nb_faces_around_vertex(
	VERTEX*			v,
	ENTITY_LIST*	face_list
	);

DECL_KERN logical
Vortex_Torus_2(
	const VERTEX *the_vertex
	);

DECL_KERN logical
Vortex_Torus_2(
	const VERTEX *vert, const FACE *this_face 
	);

//Vortex torus with one degenerate edge. Caller should make sure that
//vert belongs to this face.
DECL_KERN logical
Vortex_Torus_1(
	const VERTEX *vert, 
    const FACE *this_face 
	);

// Add a transformation for the geometry.
DECL_KERN logical
Vortex_Torus_0(
	const VERTEX *the_vertex,
	const COEDGE *the_coedge,
	const SPAtransf& xfm
	);

DECL_KERN logical
Vortex_Torus_0(
	const SPAposition& the_point,
	const COEDGE *the_coedge,
	const SPAtransf& xfm
	);

//  Utilities to maintain validity at non-manifold vertices on those rare
//  occasions it needs to be done.

DECL_KERN void
sg_detect_nm_increase_on_split(
    EDGE  *old_ed,
    EDGE  *new_ed
    );

DECL_KERN void
sg_detect_nm_decrease_on_merge(
    EDGE  *merged_ed
    );

// Utility function to query if a vertex is non-manifold. The entity_list
// gets populated with the faces connected to the supplied vertex
DECL_KERN logical vertex_is_nonmanifold(VERTEX  *vx, ENTITY_LIST &l);

#endif
