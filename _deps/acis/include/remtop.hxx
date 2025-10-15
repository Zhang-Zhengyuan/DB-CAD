/* ORIGINAL: acis2.1/kerndata/remtop/remtop.hxx */
/* $Id: remtop.hxx,v 1.8 2000/12/26 18:46:59 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Functions to remove higher-level topological entities from their
// owners, and optionally delete them and owners which are as a result
// empty. Entities below the LOOP level (i.e. coedges, edges, vertices)
// are too interdependent to eliminate in this generic way. In all cases,
// the boxes of all the owners are removed, as they will have to be
// recalculated.

#include "dcl_kern.h"
#include "logical.h"

class BODY;
class LUMP;
class SHELL;
class SUBSHELL;
class WIRE;
class FACE;
class LOOP;
class ENTITY_LIST;

// STI let (2/97): Consolidated these funtions into a single function:
// reset_boxes(ENTITY*) found in getbox.hxx
/*
// Remove the bounding SPAbox from an entity and all its parents.

// Remove the SPAbox from the given loop and all its parents.

DECL_KERN void unbox_loop( LOOP * );


// Remove the SPAbox from the given face and all its parents.

DECL_KERN void unbox_face( FACE * );


// Remove the SPAbox from the given wire and all its parents.

DECL_KERN void unbox_wire( WIRE * );


// Remove the SPAbox from the given subshell and all its parents.

DECL_KERN void unbox_subshell( SUBSHELL *, SHELL * );


// Remove the SPAbox from the given shell and all its parents.

DECL_KERN void unbox_shell( SHELL * );


// Remove the SPAbox from the given lump and all its parents.

DECL_KERN void unbox_lump( LUMP * );


// Remove the SPAbox from the given body.

DECL_KERN void unbox_body( BODY * );

*/
// STI let: end

// Extract the higher-level topological entities from their owners.
// Optionally newly-empty parents are removed.

// Extract a loop from its enclosing face.

DECL_KERN void extract_loop( LOOP *, logical = FALSE );


// Extract a face from its enclosing shell or subshell.

DECL_KERN void extract_face( FACE *, logical = FALSE );

// Extract a list of faces from enclosing shells or subshells.

DECL_KERN void extract_face( ENTITY_LIST const&, logical = FALSE );

// Extract a wire from its enclosing shell subshell or body.

DECL_KERN void extract_wire( WIRE *, logical = FALSE );


// Extract a subshell from its enclosing shell or subshell.

DECL_KERN void extract_subshell( SUBSHELL *, SHELL *, logical = FALSE );


// Extract a shell from its enclosing lump.

DECL_KERN void extract_shell( SHELL *, logical = FALSE );


// Extract a lump from its enclosing body.

DECL_KERN void extract_lump( LUMP *, logical = FALSE );

// Delete the higher-level topological entities from their owners,
// continuing to delete anything which is now empty

// Remove a loop from its enclosing face. The loop itself has had
// all its dependents eliminated already. *** Does not remove the face
// even if it has no loops, as a loopless face is meaningful ***

DECL_KERN void remove_loop( LOOP * );


// Remove a face from its enclosing shell or subshell if it is
// empty. In any case, however, remove the bounding boxes as the
// face has changed.  If the owning shell or subshell then has no 
// content, remove it too.

DECL_KERN void remove_face( FACE * );

// Remove a list of faces from their enclosing shell or subshell if 
// empty. In any case, however, remove the bounding boxes as the
// faces have changed.  If the owning shell or subshell then has no 
// content, remove it too.

DECL_KERN void remove_faces( ENTITY_LIST const& );

// Remove a wire from its enclosing shell subshell or body if it is
// empty. In any case, however, remove the bounding boxes as the
// face has changed.  If the owning shell or subshell then has no 
// content, remove it too.

DECL_KERN void remove_wire( WIRE * );


// Remove a subshell from its enclosing shell or subshell if it is
// empty. In any case, however, remove the bounding boxes as the
// subshell has changed.  If the owning shell or subshell then has no 
// faces, remove it too.

DECL_KERN void remove_subshell( SUBSHELL *, SHELL * );


// Remove a shell from its enclosing lump if it is empty. In any
// case, however, remove the bounding boxes as the shell has changed.
// If the owning lump then has no remaining shells, remove it too.

DECL_KERN void remove_shell( SHELL * );


// Remove a lump from its enclosing body if it is empty. In any
// case, however, remove the bounding boxes as the lump list has
// changed.
// **** Does not delete body even if it now has no lumps ****

DECL_KERN void remove_lump( LUMP * );

