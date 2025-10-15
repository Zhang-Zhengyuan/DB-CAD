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
//    Declare a class that allows an application to easily notify its
//    sub-systems of ENTITY events.
//
//----------------------------------------------------------------------
#ifndef ent_not_hxx
#define ent_not_hxx
#include "dcl_kern.h"
#include "lists.hxx"

/**
 * @file ent_not.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

//======================================================================

class ENTITY;
class ENTITY_LIST;

//======================================================================
// The entity_notification_handler provides an interface to allow an
// application to decide what to do when ENTITYs are created, deleted
// or changed either during a roll or not.  A derived version of this
// class uses the information to connect a PART_CONTEXT up with a PART.
// The PART_CONTEXT can then decide how the ENTITYs will be displayed.
//
/**
 * Allows an application to notify its sub-systems of <tt>ENTITY</tt> events.
 * <br>
 * <b>Role:</b>The <tt>entity_notification_handler</tt> provides an interface to allow
 * an application to decide what to do when <tt>ENTITYs</tt> are created, deleted or
 * changed either during a roll or not. A derived version of this class uses
 * the information to connect a <tt>PART_CONTEXT</tt> with a <tt>PART</tt>. The <tt>PART_CONTEXT</tt> can
 * then decide how the <tt>ENTITYs</tt> will be displayed.
 *<br><br>
 * @see ENTITY_LIST
 */

class DECL_KERN entity_notification_handler : public ACIS_OBJECT
{
protected:

//======================================================================

// This ENITITY_LIST provides a location to store <tt>ENTITYs</tt> during a roll
// operation.  This will allow the application to delay the processing of
// ENTITYs until after the roll has occured.

	ENTITY_LIST roll_list;

public:
/**
 * C++ constructor, creating an <tt>entity_notification_handler</tt>.
 */
 	entity_notification_handler();
/**
 * C++ destructor, deleting an <tt>entity_notification_handler</tt>.
 */
 	virtual ~entity_notification_handler();

	// The create, delete and update entity methods allow the handler to
	// decide what to do when a given entity event has occured.
/**
 * Notifies the handler of an entity update event, to allow the handler to decide what to do when a given entity event has occurred.
 * <br><br>
 * @param ent
 * entity to be created.
 */
 	virtual void create_entity( ENTITY *ent) = 0;
/**
 * Notifies the handler of an entity delete event, to allow the handler to decide what to do when a given entity event has occurred.
 *<br><br>
 * @param ent
 * entity to be deleted.
 */
 	virtual void delete_entity( ENTITY *ent) = 0;
/**
 * Notifies the handler of an entity update event, to allow the handler to decide what to do when a given entity event has occurred.
 *<br><br>
 * @param ent
 * entity to be updated.
 */
 	virtual void update_entity( ENTITY *ent) = 0;

	// The roll methods store the ENTITYs given into a list to be handled
	// at a later time.
/**
 * Stores the <tt>ENTITY</tt> given into a list, to be handled at a later time.
 *<br><br>
 * @param ent
 * entity to be created.
 */
 	virtual void roll_create( ENTITY *ent);
/**
 * Stores the <tt>ENTITY</tt> given into a list, to be handled at a later time.
 *<br><br>
 * @param ent
 * entity to be deleted.
 */
 	virtual void roll_delete( ENTITY *ent);
/**
 * Adds the given <tt>ENTITY</tt> to the <tt>roll_list</tt>.
 * <br><br>
 * @param ent
 * entity to be updated.
 */
 	virtual void roll_update( ENTITY *ent);
/**
 * Removes the <tt>ENTITY</tt> from the <tt>roll_list</tt>.
 * <br><br>
 * @param ent
 * entity to remove.
 */
 	virtual void remove_roll_entity( ENTITY *ent);

	// These methods are called when a roll operation is about to begin
	// and when it has completed.
/**
 * Clears the <tt>roll_list</tt> in preparation for the roll operation.
 */
 	virtual void before_roll();

	// The derived class must decide how the rolled ENTITYs should be
	// processed.
/**
 * Clears the <tt>roll_list</tt> after the roll operation.
 * <br><br>
 * <b>Role:</b> The derived classes must decide how the rolled <tt>ENTITYs</tt> should be processed.
 */
 	virtual void after_roll() = 0;
};


//======================================================================
/** @} */
#endif
