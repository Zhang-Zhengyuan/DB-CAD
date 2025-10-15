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
//
// Define a base class from which callback lists can be derived.  It
// defines the toolkit_callback base class that the actual callback
// classes are derived from, and the toolkit_callback_list class which
// keeps track of a list of callbacks.
//
// This file just defines the base class which is used to keep track
// of callback lists.  To be usefull, you must derive a new class from
// both tkb and tkl which define methods that do the actual notification.
// These methods are not implemented in these base classes because the
// arguments to them will vary depending on the type of application.
//
// Normally, this class will not be directly exposed to a user of the
// 3D Toolkit,  Instead, they will derive new classes from specific
// kinds of callback classes which are derived from the classes defined
// here, and will overload the virtual execute method of the derived
// class.
//
//----------------------------------------------------------------------

#ifndef toolkit_callback_hxx
#define toolkit_callback_hxx

#include "dcl_base.h"
#include "base.hxx"

/**
* @file tlkit_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

//======================================================================

class toolkit_callback_list;

//----------------------------------------------------------------------
/**
 * Defines the <tt>toolkit_callback</tt> base class and the <tt>toolkit_callback_list</tt> class.
 * <br>
 * <b>Role:</b> The <tt>toolkit_callback</tt> class is an internal base class from which our callback
 * lists are derived. 
 * <br><br>
 * Generally, this class should not be derived from by customers; instead, customers should
 * derive new classes from specific kinds of callback classes and overloads the virtual execute 
 * method of the derived class.
 * @see toolkit_callback_list, exit_callback, input_callback, output_callback
 */
class DECL_BASE toolkit_callback /*: public ACIS_OBJECT hp700 compiler problem*/
{
private:
	toolkit_callback* m_pNextCallback;
	toolkit_callback* m_pPrevCallback;

protected:
	// When you add a callback to a list, the list assumes responsibility
	// for the callback, and will delete it when needed.  For this reason,
	// we do not want to allow callbacks to be created on the stack, so
	// the destructor is made private.
	virtual ~toolkit_callback();
	friend class toolkit_callback_list;

	void set_next(toolkit_callback* cb) { m_pNextCallback = cb; }
	void set_prev(toolkit_callback* cb) { m_pPrevCallback = cb; }

public:

	/**
	 * @nodoc
	 */
	MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * C++ constructor, creating a <tt>toolkit_callback</tt>.
 */
	toolkit_callback();

/**
 * Specifies the next callback command.
 */
	toolkit_callback *next() { return m_pNextCallback; }

/**
 * Specifies the previous callback command.
 */
	toolkit_callback *prev() { return m_pPrevCallback; }
};

//----------------------------------------------------------------------

/**
 * Stores the list of toolkit_callbacks.
 * <br>
 * <b>Role:</b> The <tt>toolkit_callback_list</tt> class stores the list of <tt>toolkit_callbacks</tt>.
 * @see toolkit_callback
 */
class DECL_BASE toolkit_callback_list : public ACIS_OBJECT
{
private:
	toolkit_callback *m_pFirstCallback;
	toolkit_callback *m_pLastCallback;

public:
/**
 * C++ constructor, creating a <tt>toolkit_callback_list</tt>.
 */
	toolkit_callback_list();
/**
 * C++ destructor, deleting a <tt>toolkit_callback_list</tt>.
 */
	virtual ~toolkit_callback_list();

protected:
	// The methods which add new callbacks to the list are protected
	// members.  The only reason for this is so that you have to
	// provide typesafe public versions in a derived class.  The methods
	// in the derived class only have to call one of these protected
	// versions in this base class.  Since you can only add objects
	// of the right type to a callback list, it is safe to do casting
	// in the other methods of a derived class.

	// Add another callback to the list. 'add' adds it to the head
	// of the list, so it will be the first callback called.  'append'
	// adds it to the end of the list.
	void add(toolkit_callback*);
	void append(toolkit_callback*);

public:
	// The guts of the remove method except for deleting the callback.
/**
 * Removes a callback from the callback list.
 * <br><br>
 * @param callb
 * callback.
 */
	void remove_no_dtor(toolkit_callback* callb);

	// Remove a callback from the list.  NOTE: This method will also
	// delete the callback.  This means that you must not delete the
	// callback yourself, and also that all callback objects must be
	// allocated on the heap using 'new'.

	// This function is virtual so derived classes can control the "ownership" policy
/**
 * Removes a callback from the callback list and deletes it.
 * <br><br>
 * <b>Role:</b> This means that you must not delete the callback yourself,
 * and also that all callback objects must be allocated on the heap using '<tt>new</tt>'.
 * <br><br>
 * @param callb
 * callback.
 */
	virtual void remove(toolkit_callback* callb);

	// Remove (and delete) all callbacks

/**
 * Removes and deletes all callbacks.
 */
	void clear();

	// Get the first callback

/**
 * Gets the first callback.
 */
	toolkit_callback *first() { return m_pFirstCallback; }
	// Find the last one, for routines that want to call in reverse order

/**
 * Gets the last callback, for routines that want to call in reverse order.
 */
	toolkit_callback *last() { return m_pLastCallback; }
};

// Implement the different ownership policy mentioned in the comment of the remove
// method above.
/**
 * @nodoc
 */
class DECL_BASE toolkit_callback_list_not_owner : public toolkit_callback_list
{
public:

	~toolkit_callback_list_not_owner();
	virtual void remove(toolkit_callback*);

};

//======================================================================
/** @} */
#endif

