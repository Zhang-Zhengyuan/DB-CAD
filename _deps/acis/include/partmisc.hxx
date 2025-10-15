/* ORIGINAL: 3dt2.1/pmhusk/part_utl.hxx */
// $Id: partmisc.hxx,v 1.12 2000/12/26 18:55:17 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Define Part Management utility procedures
//----------------------------------------------------------------------

#ifndef partmisc_hxx 
#define partmisc_hxx 

//======================================================================

class PART;
class PART_CONTEXT;
class ENTITY;
class ENTITY_LIST;
class entity_filter;
class outcome;
class view3d;
class FileInterface;
class asm_model;

#include "dcl_part.h"
#include "api.hxx"
#include "entityid.hxx"
#include "hashpart.hxx"
#include "logical.h"

class ID_ATTRIB;

//======================================================================

// get the id attrib for an entity
DECL_PART ID_ATTRIB* get_entity_id_attrib(
    ENTITY* ent
    );

// Support the api routines

// Find an ENTITY given an ID and a PART
DECL_PART ENTITY* part_lookup_entity(
    entity_id_t,
    PART*
    );

// Get the ID and PART for an ENTITY
DECL_PART logical part_entity_id_and_part(
    ENTITY*,
    entity_id_t&,
    PART*&
    );

// Get all ENTITIES from a PART
DECL_PART void part_entities(
    PART*,
    ENTITY_LIST&,
    entity_filter*
    );

// Create a new PART.  
DECL_PART PART* part_create(
    unsigned int size = DEFAULT_PART_SIZE
    );

// Delete a PART.  
DECL_PART void part_delete(PART*);

// Add an ENTITY to a PART
DECL_PART entity_id_t part_add_entity(
    ENTITY*,
    PART*
    );

// Remove an ENTITY from its PART
DECL_PART outcome part_remove_entity(
    ENTITY*
    );

// Return the assembly model associated with the active PART
DECL_PART asm_model* get_model_from_active_part();


// Add bgraph to PART
DECL_PART void part_add_bgraph(PART*);

#endif
