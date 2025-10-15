/* ORIGINAL: acis2.1/sg_husk/offset/ent_tool.hxx */
/* $Id: ent_tool.hxx,v 1.6 2000/12/26 18:48:55 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SG_ENTITY_TOOLS )
#define SG_ENTITY_TOOLS

#include "dcl_kern.h"

// Forward Declarations

class ENTITY;
class ENTITY_LIST;

// Seed an entity list starting with a given entity.  This fills
// the entity list with the original entity, any entities hanging
// from the original, and so on until all of the entities have
// been put on the list.

DECL_KERN void
sg_seed_list(
	ENTITY *,			// Original entity to start the seeding
	ENTITY_LIST & );	// Entity list to seed

// Seed an entity list starting with a given entity.  This fills
// the entity list with the original entity, any entities hanging
// from the original, and so on until all of the entities have
// been put on the list.  This list leaves off entities that are removed
// by another entity's lose() function.  So, this function is suitable
// for building lists of entities to delete.

DECL_KERN void
sg_seed_for_delete(
	ENTITY *,			// Original entity to start the seeding
	ENTITY_LIST & );	// Entity list to seed

// Make a copy of the entity list entries into an entity array.  It
// is assumed that the entity array is already allocated with enough
// space.  The entity pointers of each entity on the list are made to
// point to indices of the array rather than addresses.

DECL_KERN void
sg_copy_list(
	ENTITY_LIST &,		// entity list to be copied
	ENTITY *[] );		// array of copied entities

// Go through an array of entities whose entity pointers are array indices
// and change them to the appropriate addresses.

DECL_KERN void
sg_fix_list(
	ENTITY *[],			// Array of entities with index pointers
	int );				// Number of entities in the array

// Make a recursive copy of an entity

DECL_KERN ENTITY *
sg_copy_entity(
	const ENTITY * );	// Entity to copy

#endif
