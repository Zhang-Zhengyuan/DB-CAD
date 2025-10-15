/* ORIGINAL: part/pmhusk/part_cb.hxx */
// $Id: part_cb.hxx,v 1.1 2002/05/22 16:43:49 mgoertz Exp $
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
//    Define the part_callback class.  The part_callback class is 
//    used to get notification about the start and end of a roll
//    operation.  You can derive new classes from part_callback when you
//    have to perform special actions when a roll occurs.
//----------------------------------------------------------------------

#ifndef part_cb_hxx
#define part_cb_hxx

//======================================================================

#include "dcl_part.h"
#include "tlkit_cb.hxx"

class PART;

//======================================================================

// Type of notification
typedef enum {
    pm_Create_Part,   // Invoked when a PART is created
    pm_Activate_Part, // Invoked when a PART is activated
    pm_Delete_Part,   // Invoked when a PART is deleted
	pm_Change_Part    // Invoked when an ENTITY in the PART is changed
} part_event_type;

class DECL_PART part_callback : public toolkit_callback
{

public:
	part_callback() {}
	virtual ~part_callback() {}

	// This is the only interesting method.  To create a roll callback,
	// derive a new class from part_callback, and overload this virtual method
	// to do what you want to do.
	virtual void execute(part_event_type, PART*);
};

//======================================================================
// This is a list of part_callbacks that should be notified whenever
// a part is created or destroyed.

// The part_callback_list is derived from toolkit_callback_list and
// just provides a typesafe interface to it and the execute method.
class DECL_PART part_callback_list : public toolkit_callback_list
{
public:
	void add(part_callback* cb) { toolkit_callback_list::add(cb); }
	void append(part_callback* cb) { toolkit_callback_list::append(cb); }
	void remove(part_callback* cb) { toolkit_callback_list::remove(cb); }

	void execute(part_event_type, PART*);
};

//======================================================================
// Get the global list of part callbacks.  This is used when you want
// to add a new callback to the list.

DECL_PART part_callback_list& get_part_callback_list();

//======================================================================
// This interface is obsolete and should not be used any longer.

// Procedure to register a part_callback
DECL_PART void register_Part_Callback(part_callback* cb);
DECL_PART void unregister_Part_Callback(part_callback* cb);

//======================================================================
#endif
