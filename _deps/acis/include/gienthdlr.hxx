/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#include "dcl_ga.h"
#include "ent_not.hxx"

//======================================================================
// The following procedures and class handle correct redisplay of things 
// after a roll operation.

class DECL_GA rb_entity_handler : public entity_notification_handler
{
public:
	rb_entity_handler() {}
	virtual ~rb_entity_handler() {}

	// The create, delete and update entity methods allow the handler to
	// decide what to do when a given entity event has occured.
	virtual void create_entity( ENTITY *) { /* Do nothing */}
	virtual void delete_entity( ENTITY *) { /* Do nothing */}
	virtual void update_entity( ENTITY *) { /* Do nothing */}

	// The roll methods store the ENTITYs given into a list to be handled
	// at a later time.
	virtual void roll_create( ENTITY *ent) { roll_list.add( ent);}
	virtual void roll_delete( ENTITY *ent) { roll_list.add( ent);}
	virtual void roll_update( ENTITY *ent) { roll_list.add( ent);}

	virtual void remove_roll_entity( ENTITY *ent) { roll_list.remove( ent);}

	// The derived class must decide how the rolled ENTITYs should be
	// processed.
	virtual void after_roll(){}
};

// Set the defered changes handler.  If you don't do this, you will
// get a standard default one.
DECL_GA void set_gi_entity_handler( rb_entity_handler *handler);

// retrieve the current defered changes handler.
DECL_GA rb_entity_handler *get_gi_entity_handler();
