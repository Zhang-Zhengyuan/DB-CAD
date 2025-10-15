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
//    Define the roll_callback class.  The roll_callback class is
//    used to get notification about the start and end of a roll
//    operation.  You can derive new classes from roll_callback when you
//    have to perform special actions when a roll occurs.
//----------------------------------------------------------------------

#ifndef roll_cb_hxx
#define roll_cb_hxx

//======================================================================

#include "dcl_part.h"
#include "tlkit_cb.hxx"
/**
 * @file roll_cb.hxx
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
 * The type of notification of a callback for a roll operation.
 * @param Before_Roll
 * We are about to do a roll operation.
 * @param After_Roll
 * We are done with a roll operation.
 */
typedef enum {
    Before_Roll,
    After_Roll
} roll_event_type;

/**
 * Notifies the user about the start and the end of a roll operation.
 * <br>
 * <b>Role:</b> C++ callbacks are similar to Scheme hooks. They are developer-defined
 * functions that the system calls at predefined times.
 * <br><br>
 * This class obtains notification about the start and end of a roll operation.
 * To perform special actions when a roll occurs, derive new classes from
 * the <tt>roll_callback</tt> class.
 * <br><br>
 * To inform the system about a roll callback, call <tt>register_Roll_Callback</tt> with
 * the address of the function to be called. More than one roll callback can be
 * registered at the same time. The order in which roll callbacks are called is undefined.
 * <br><br>
 * The system calls all registered roll callbacks before and after all roll operations.
 * <br><br>
 * The <tt>roll_event_type</tt> enumerator specifies whether a roll is about to happen or has just completed.
 * A <tt>roll_callback</tt> returns no value.
 */
class DECL_PART roll_callback : public toolkit_callback
{

public:
/**
 * C++ constructor, creating a <tt>roll_callback</tt>.
 */
	roll_callback() {}
/**
 * C++ destructor, deleting a <tt>roll_callback</tt>.
 * <br><br>
 * <b>Role:</b> Do not call this method; callbacks are deleted by the remove
 * method of the <tt>toolkit_callback_list</tt> class.
 */
	virtual ~roll_callback() {}

	// This is the only interesting method.  To create a roll callback,
	// derive a new class from roll_callback, and overload this virtual method
	// to do what you want to do.
/**
 * The base class version is not interesting.
 * <br><br>
 * <b>Role:</b> To create a roll change callback, derive a new class from
 * <tt>roll_callback</tt> and overload this virtual method to handle roll operation notifications.
 * <br><br>
 * @param roll_type
 * reason.
 */
	virtual void execute(roll_event_type roll_type);
};

//======================================================================
// This is a list of roll_callbacks that should be notified whenever
// the active entity is changed or modified.

// The roll_callback_list is derived from toolkit_callback_list and
// just provides a typesafe interface to it and the execute method.
/**
 * Provides a type-safe interface to the <tt>toolkit_callback_list</tt> and the execute method.
 * <br>
 * <b>Role:</b> This class derives from the <tt>toolkit_callback_list</tt> class, and it
 * provides a type-safe interface to it and the execute method.
 */
class DECL_PART roll_callback_list : public toolkit_callback_list
{
public:
/**
 * Adds a roll callback to the list.
 * <br><br>
 * @param cb
 * roll callback.
 */
	void add(roll_callback* cb) { toolkit_callback_list::add(cb); }
/**
 * Appends a roll callback.
 * <br><br>
 * @param cb
 * roll callback.
 */
	void append(roll_callback* cb) { toolkit_callback_list::append(cb); }
/**
 * Calls the execute method of each callback in the list.
 * <br><br>
 * @param roll_type
 * reason.
 */
	void execute(roll_event_type roll_type);
};

//======================================================================
// Get the global list of roll callbacks.  This is used when you want
// to add a new callback to the list.

/**
* Gets a list of roll callbacks.
* <br><br>
*  Use this method to add a new callback to the list.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_PART roll_callback_list& get_roll_callback_list();

//======================================================================
// This interface is obsolete and should not be used any longer.

// type declaration for a Roll_Callback

//Function pointer
// For internal use only
/**
 * @nodoc
 */
typedef void (*Roll_Callback)(roll_event_type);

// procedure to register a Roll_Callback
// For internal use only
/**
 * @nodoc
 */
DECL_PART void register_Roll_Callback(Roll_Callback);


/** @} */
#endif
