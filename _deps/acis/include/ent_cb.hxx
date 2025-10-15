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
//    Define the entity_callback class.  The entity_callback class is a
//    base class from which you can derive other classes which will
//    recieve notification of changes to entities  To use this class,
//    derive a new class from entity_calback, and overload the virtual
//    'execute' method.  To have your callback get notified, add it
//    to the entity callback list (which you can get by calling
//    get_entity_callback_list()
//----------------------------------------------------------------------
#ifndef ent_cb_hxx
#define ent_cb_hxx
#include "dcl_part.h"
#include "logical.h"
class ENTITY;
#include "tlkit_cb.hxx"
/**
 * @file ent_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PARTAPI
 *
 * @{
 */


//======================================================================

// Type of notification

// For internal use only
/**
 * These are the types of notifications that entity callback functions receive.
 * @param pm_Create_Entity
 * Notification that an entity has been created.
 * @param pm_Delete_Entity
 * Notification that an entity has been deleted.
 * @param pm_Roll_Create_Entity
 * Notification that the creation of an entity has been un-done.
 * @param pm_Roll_Delete_Entity
 * Notification that the deletion of an entity has been un-done.
 * @param pm_Roll_Update_Entity
 * Notification that the modification of an entity has been has been undone.
 * @param pm_Part_Delete_Delete_Entity
 * Notification that the entity has been modified by deletion of its containing part.
 * @param pm_Update_Entity
 * Notification that an entity has been modified.
 * @param pm_Remove_Entity
 * Notification that an entity has been removed from a part.
 * @see entity_callback.
 */
typedef enum {
    pm_Create_Entity,		// Invoked when an ENTITY is added to a PART
    pm_Delete_Entity,
    pm_Roll_Create_Entity,
    pm_Roll_Delete_Entity,
	pm_Roll_Update_Entity,
	pm_Part_Delete_Delete_Entity,
	pm_Update_Entity,		// Entity has been updated somehow.
	pm_Remove_Entity		// Entity has been removed from the part. This
							// is used to differentiate simple removal
							// from outright Delete, which is specified
							// by pm_Delete_Entity.
} entity_event_type;

//======================================================================
/**
 * Provides notification of changes to entities.
 * <br>
 * <b>Role:</b> C++ callbacks are similar to Scheme hooks.
 * They are developer-defined functions that the system calls at predefined times.
 * <br><br>
 * This class is a base class from which other classes are derived, which
 * receives notification of changes to entities.  To use this class, derive a
 * new class from <tt>entity_callback</tt>, and overload the virtual execute method.
 * To activate the callback, add it to the entity callback list by
 * calling <tt>get_entity_callback_list</tt>.
 * <br>
 * <dl>
 * <dt>Entity callbacks are called when:</dt>
 * <dd>A new entity is created.</dd>
 * <dd>An entity is deleted.</dd>
 * <dd>An entity is created by the roll back system and rolls back over a delete
 * or rolls forward over a create.</dd>
 * <dd>An entity is deleted by the roll back system and rolls back over a
 * create or rolls forward over a delete.</dd>
 * <dd>An entity is changed by the rollback system.</dd>
 * <dd>The part containing the entity is deleted.</dd>
 * <dd>The entity is removed from its part.</dd>
 * <dd>The entity is updated in some other way.</dd>
 * </dl>
 * <br>
 * The <tt>execute</tt> method is passed a pointer to the <tt>ENTITY</tt> in question and an
 * <tt>entity_event_type</tt> enumerator specifying the type of operation being performed.
 * The method returns no value.
 */
class DECL_PART entity_callback : public toolkit_callback
{

public:
/**
 * C++ constructor, creating an <tt>entity_callback</tt>.
 */
	entity_callback() {}
/**
 * C++ destructor, deleting an <tt>entity_callback</tt>.
 */
	virtual ~entity_callback() {}

	// This is the only interesting method.  To create a entity change callback,
	// derive a new class from entity_callback, and overload this virtual method
	// to do what you want to do.
/**
 * Executes the callback by updating the rendering appropriate to the specified entity and event.
 * <br><br>
 * <b>Role:</b> The base class implementation effectively does nothing.
 * <br><br>
 * @param en_type
 * the event that has changed the entity directly or through its part.
 * @param ent
 * the entity whose rendering is to be updated.
 */
	virtual void execute(entity_event_type en_type, ENTITY* en );
};

//======================================================================
// This is a list of entity_callbacks that should be notified whenever
// an entity is changed.

// The entity_callback_list is derived from toolkit_callback_list and
// just provides a typesafe interface to it and the execute method.
/**
 * Provides a type-safe interface to the <tt>toolkit_callback_list</tt> class and the execute command.
 * <br><b>Role:</b> This class derives from the <tt>toolkit_callback_list</tt> class,
 * and it provides a type-safe interface to <tt>toolkit_callback_list</tt> class and the execute command.
 * @see PART
 */
class DECL_PART entity_callback_list : public toolkit_callback_list
{
	logical in_part_delete;
public:
/**
 * C++ constructor, creating an <tt>entity_callback_list</tt>.
 */
	entity_callback_list();
/**
 * Adds a callback to an <tt>entity_callback_list</tt>.
 * <br><br>
 * @param cb
 * callback.
 */
	void add(entity_callback* cb) { toolkit_callback_list::add(cb); }
/**
 * Appends a callback to an <tt>entity_callback_list</tt>.
 * <br><br>
 * @param cb
 * entity callback.
 */

	void append(entity_callback* cb) { toolkit_callback_list::append(cb); }
/**
 * Sets the part delete flag on or off.
 * <br><br>
 * @param f
 * set flag on or off?
 */
	void set_part_delete_flag( logical f ) { in_part_delete = f; }
/**
 * Gets the part delete flag.
 * <br><br>
 * @return the part delete flag
 */
	logical get_part_delete_flag() { return in_part_delete; }
/**
 * Calls the execute method of each callback in the list.
 * <br><br>
 * @param en_type
 * reason.
 * @param en
 * entity.
 */
	void execute(entity_event_type en_type, ENTITY* en);
};

/**
 * @nodoc
 */
// Constructor
inline entity_callback_list::entity_callback_list() : in_part_delete(0) {}


//======================================================================
// Get the global list of entity callbacks.  This is used when you want
// to add a new callback to the list or call the execute functions.

/**
* Gets a global list of entity callbacks.
* <br><br>
* <b>Effect:</b> Read-only
*/
DECL_PART entity_callback_list& get_entity_callback_list();

//======================================================================
// This interface is obsolete and should not be used any longer.

// type declaration for an Entity_Callback


// Function Pointer

// For internal use only
/**
 * @nodoc
 */
typedef void (*Entity_Callback)(ENTITY*, entity_event_type);

// procedure to register an Entity_Callback

// For internal use only
/**
 * @nodoc
 */
DECL_PART void register_Entity_Callback(Entity_Callback);


/** @} */

#endif
