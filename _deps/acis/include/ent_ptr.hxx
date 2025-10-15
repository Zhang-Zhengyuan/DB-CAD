/* ORIGINAL: acis2.1/kernutil/ent_ptr/ent_ptr.hxx */
// $Id: ent_ptr.hxx,v 1.5 2000/12/26 18:48:11 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header describing a pointer entity as a derived class of
// ENTITY so that these entities are, logged and rolled
// back and forth together with models.

// This class was needed because static variables can not be logged
// in the bulletin board.  To overcome this, static variables have been
// changed to non-changing handles to changing ptr_entities.  In this way, 
// the static variables are logged...

// This class is useful, as well, for a rollable version of a linked list


#if !defined( ENTITY_PTR_CLASS )
#define ENTITY_PTR_CLASS

#include "dcl_kern.h"
#include "entity.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ENTITY_PTR, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

extern DECL_KERN int ENTITY_PTR_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define ENTITY_PTR_LEVEL (ENTITY_LEVEL + 1)


class DECL_KERN ENTITY_PTR: public ENTITY {
	ENTITY       *m_pEntity;	    // pointer to the entity
	ENTITY_PTR   *m_pNext;			// Well, actually this supports a list
friend class ENTITY_PTR_LIST;
	
	// ENTITY_PTR's are used at the boundary between rollable and
	// non-rollable data.  Typical uses are in the husk interface
	// to libraries of non-acis code.  Such libraries may have internal
	// data structures corresponding to the ENTITY pointed to by
	// this ENTITY_PTR.

	void         *m_pHandle;       // Can be used to hook non-entity data.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( ENTITY_PTR, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	
 public:
	
	ENTITY_PTR( ENTITY *the_entity = NULL ); // the constructor
	
	// Access Functions
	
	void		set_ent( ENTITY *the_entity ); 
	void		set_next( ENTITY_PTR *the_next_entity );
	void		set_handle( void* the_handle );
	ENTITY*     entity()    const { return m_pEntity; }
	ENTITY_PTR* next()   const { return m_pNext; }
	void*       handle() const { return m_pHandle; }
};

#endif

