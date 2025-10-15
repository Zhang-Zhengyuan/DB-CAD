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
//    Define class ID_ATTRIB to allow identification of ENTITYS in a table
//
//----------------------------------------------------------------------
#ifndef id_attr_hxx
#define id_attr_hxx
#include "dcl_part.h"
#include "dcl_part.h"
#include "logical.h"
#include "attr_sti.hxx"
#include "idtable.hxx"
#include "part.hxx"
#include <time.h>
/**
 * @file id_attr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PARTAPI
 *
 * @{
 */
//======================================================================

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ID_ATTRIB, PART)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
extern DECL_PART int ID_ATTRIB_TYPE;
/**
 * @nodoc
 */
#define ID_ATTRIB_LEVEL (ATTRIB_ST_LEVEL + 1)
/**
 * Allows identification of entities in a table.
 */
class DECL_PART ID_ATTRIB: public ATTRIB_ST {

private:
    entity_id_t TheEntityId;
    part_handle ThePartHandle;
	int m_nRevision;
	logical m_generateCallbacks;
	time_t m_time_stamp;

	// We make PART a friend of ID_ATTRIB so that we can make the method to
	// set the ENTITY id private, but still set it when we add an ENTITY
	// to a PART.
	friend class PART;
	void set_part(part_handle ph);
	void set_id(entity_id_t id, logical update_history = TRUE);
	virtual logical copyable() const;
	virtual logical savable() const;

	friend class ent_hash_entry;
	void update_time_stamp();

protected:
	void private_set_entity( ENTITY *ent );
public:

// Default constructor - used in backup
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back
 * and history management.
 */
    ID_ATTRIB();

// Create an entity identifier
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * @param owner
 * owner of the entity.
 * @param p_handle
 * handle for this part.
 */
    ID_ATTRIB(ENTITY* owner, part_handle p_handle);
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
// Operations on the identifier
/**
 * Returns the identifier.
 */
    entity_id_t get_id() const {return TheEntityId;}

// Get the PART that the ENTITY belongs to
/**
 * Returns the part to which the entity belongs.
 */
    PART* get_part() const {return TheEntityId ? ThePartHandle.Part() : NULL;}

// Update the id revision.  This is caled when the owner ENTITY is
// modified.  It backs up the ID_ATTRIB and increments the id revision.
/**
 * Updates the <tt>ID</tt> revision.
 * <br><br>
 * <b>Role:</b> This is called when the owner entity is modified.
 * It backs up the <tt>ID_ATTRIB</tt> and increments the <tt>ID</tt> revision.
 */
	void update_revision();

// rollback notification
// tbrv
/**
 * @nodoc
 */
   virtual void roll_notify(BULLETIN_TYPE, ENTITY*);

// generate or not generate callbacks.  Returns the previous state
/**
 * Sets the flag indicating whether or not to generate callbacks.
 * <br><br>
 * @param doCallbacks
 * new flag value.
 */
	logical generate_callbacks( logical doCallbacks);
/**
 * @nodoc
 */
	time_t get_time_stamp() { return (time_t)m_time_stamp; }
/**
 * Returns <tt>TRUE</tt> if this can be deleted.
 */
	logical deletable() const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ID_ATTRIB, PART)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
//======================================================================

// Find the ID_ATTRIB attached to an ENTITY
/**
* Finds the <tt>ID</tt> attribute for an entity.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param ent
* entity.
**/

DECL_PART ID_ATTRIB* find_ID_ATTRIB(ENTITY *ent );


/** @} */
#endif
