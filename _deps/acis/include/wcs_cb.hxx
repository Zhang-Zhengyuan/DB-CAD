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
//    Define the WCS_callback class.  The WCS_callback class is a base
//    class from which you can derive other classes which will recieve
//    notification of changes to the active WCS.  To use this class,
//    derive a new class from WCS_calback, and overload the virtual
//    'execute' method to do what you want to do when the active WCS
//    changes.  To have you class invoked, create an instance of the
//    class using 'new', and ad it to the WCS callback list.
//----------------------------------------------------------------------

#ifndef wcs_cb_hxx
#define wcs_cb_hxx

//======================================================================
/**
 * @file wcs_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */
#include "dcl_kern.h"
#include "tlkit_cb.hxx"

//======================================================================

class WCS;

// Type of change.  This is passed to the callbacks execute method as
// the first argument.
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef enum {
    Active_WCS_Changed,
    Active_WCS_Modified,
	Active_WCS_Color_Changed // STI jmb
} wcs_change_type;

//----------------------------------------------------------------------
/**
 * Provides notification of changes to the <tt>WCS</tt>.
 * <br>
 * <b>Role:</b> The <tt>WCS_callback</tt> class is a base class from which classes that receive notification
 * of changes to the active <tt>WCS</tt> are derived.
 * <br><br>
 * To use this class, derive a new class from <tt>WCS_callback</tt>, and overload the virtual <tt>execute</tt> method
 * to do what is desired when the active <tt>WCS</tt> changes. To invoke the class, create an instance of the
 * class using <tt>new</tt>, and add it to the <tt>WCS_callback_list</tt>.
 */
class DECL_KERN WCS_callback : public toolkit_callback
{
public:
/**
 * C++ constructor, creating a <tt>WCS_callback</tt>.
 */
	WCS_callback() {}
/**
 * C++ destructor, deleting a <tt>WCS_callback</tt>.
 */
	virtual ~WCS_callback() {}

	// This is the only interesting method.  To create a WCS change callback,
	// derive a new class from WCS_callback, and overload this virtual method
	// to do what you want to do.
/**
 * Uses a <tt>WCS_callback</tt>, derive a new class from <tt>WCS_callback</tt> and implement this method.
 * <br><br>
 * @param reason
 * reason.
 * @param old_wcs
 * old WCS.
 * @param new_wcs
 * new WCS.
 */
	virtual void execute(wcs_change_type reason, WCS* old_wcs, WCS* new_wcs);
};

//======================================================================
// This is a list of WCS_callbacks that should be notified whenever
// the active WCS is changed or modified.

// The WCS_callback_list is derived from toolkit_callback_list and
// just provides a typesafe interface to it and the execute method.
/**
 * Provides a type-safe interface to <tt>toolkit_callback_list</tt> class and the <tt>execute</tt> command.
 * <br>
 * <b>Role:</b> The <tt>WCS_callback_list</tt> class is derived from the <tt>toolkit_callback_list</tt> and provides
 * a type-safe interface to it and the execute command.
 */
class DECL_KERN WCS_callback_list : public toolkit_callback_list
{
public:
/**
 * Adds a callback to the list.
 * <br><br>
 * @param cb
 * callback.
 */
	void add(WCS_callback* cb) { toolkit_callback_list::add(cb); }
/**
 * Appends a callback.
 * <br><br>
 * @param cb
 * callback.
 */
	void append(WCS_callback* cb) { toolkit_callback_list::append(cb); }
/**
 * Executes the new <tt>WCS_callback</tt> list.
 * <br><br>
 * @param reason
 * reason for change.
 * @param old_wcs
 * old callback list.
 * @param new_wcs
 * new callback list.
 */
	void execute(wcs_change_type reason, WCS* old_wcs, WCS* new_wcs);
};

//======================================================================
// Get the global list of WCS callbacks.  This is used when you want
// to add a new callback to the list.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN WCS_callback_list& get_WCS_callback_list();

//======================================================================
// This interface is left here to provide compatibility with the old
// interface.  It is OBSOLETE!.   Its use should be discontinued in
// favor of the object oriented interface.

// type declaration for a WCS_Change_Callback
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef void (*WCS_Change_Callback) (wcs_change_type, WCS*, WCS*);

// procedure to register a WCS_Change_Callback
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void register_wcs_change_callback(WCS_Change_Callback);

//======================================================================
/** @} */
#endif
