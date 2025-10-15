/* ORIGINAL: 3dt2.1/pmhusk/part_utl.hxx */
// $Id: part_utl.hxx,v 1.18 2001/12/31 20:39:24 jmb Exp $
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

#ifndef part_utl_hxx
#define part_utl_hxx

//======================================================================

class PART;
class PART_CONTEXT;
class ENTITY;
class ENTITY_LIST;
class entity_filter;
class outcome;
class FileInterface;

#include "api.hxx"
#include "entityid.hxx"
#include "partmisc.hxx"
#include "hashpart.hxx"
#include "logical.h"


//======================================================================

// Get the active PART
PART* get_active_part(logical create_if_none = TRUE);

// Set the active PART
void set_active_part(PART*);

//======================================================================

// Support the api routines

// Create a new PART.  This procedure actually creates both a PART and
// a PART_CONTEXT, and adds the PART to the PART_CONTEXT.
PART* pm_create_part(
    unsigned int size = DEFAULT_PART_SIZE
    );

// Delete a PART.  If the PART has an associated PART_CONTEXT, then that
// is deleted also.
void pm_delete_part(PART*);

// Add an ENTITY to a PART
entity_id_t pm_add_entity(
    ENTITY*,
    PART*
    );

// Remove an ENTITY from its PART
outcome pm_remove_entity(
    ENTITY*
    );

// Clear a PART (delete all ENTITIES)
void pm_part_clear(
	PART*
	);

// Get the ID and PART for an ENTITY (create it if necessary)
void pm_create_entity_id(
    ENTITY*,
    entity_id_t&,
    PART*&,
    ENTITY * given_owner = NULL
    );

//----------------------------------------------------------------------
// Purpose---
//   set auto_display_mode flag for the active RENDERING_MANAGER 
//----------------------------------------------------------------------
logical set_auto_display_mode( logical new_state);

//----------------------------------------------------------------------
// Purpose---
//   get auto_display_mode flag of the active RENDERING_MANAGER 
//----------------------------------------------------------------------
logical get_auto_display_mode();

//---------------------------------------------------------------------
// Since these to procedures have to do with views, they could logically
// be put somewhere in gihusk.  Since there is already a connection
// gihusk and pmhusk through the rendering_context though, they are
// put here.
class SchemeView;
void add_to_part_context(const SchemeView&, PART*);
PART* get_part_from_view(const SchemeView&);
PART* get_part_from_part_context(PART_CONTEXT*);

//======================================================================
#endif
