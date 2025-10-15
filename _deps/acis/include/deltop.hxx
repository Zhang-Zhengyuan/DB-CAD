/* ORIGINAL: acis2.1/kerndata/deltop/deltop.hxx */
/* $Id: deltop.hxx,v 1.7 2000/12/26 18:46:49 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for delete functions.

// Delete a self-contained topological entity (i.e. one which is not
// contained in a higher-level one).

#include "dcl_kern.h"
#include "logical.h"

class ENTITY;
class BODY;
class LUMP;
class SHELL;
class FACE;
class LOOP;
class COEDGE;
class EDGE;
class VERTEX;
class WIRE;

// Delete an entity. It works on any topological entity type,
// and returns TRUE if so. If the entity given is not a suitable
// type it returns FALSE.

DECL_KERN logical delete_entity( ENTITY * );


// Delete a body. Just delete all its lumps, wires, then 
// get rid of it.

DECL_KERN void delete_body( BODY * );


// Delete a lump. Just delete all its shells, then get rid of it.

DECL_KERN void delete_lump( LUMP * );


// Delete a shell. Delete all its faces and subshells, and then
// get rid of it.

DECL_KERN void delete_shell( SHELL * );


// Delete a face. Delete all its loops, and then get rid of it.

DECL_KERN void delete_face( FACE * );


// Delete a loop. Delete all its coedges, and then get rid of it.

DECL_KERN void delete_loop( LOOP * );


// Delete a coedge. Remove it from its edge ring, deleting the edge if
// it is the last, and then get rid of it.

DECL_KERN void delete_coedge( COEDGE * );


// Delete an edge. Delete or reconnect its end vertices, and then get
// rid of it.

DECL_KERN void delete_edge( EDGE * );


// Delete a vertex. Just get rid of it.

DECL_KERN void delete_vertex( VERTEX * );


// Delete a wire and all its edges, coedges and vertices.

DECL_KERN void delete_wire( WIRE * );

