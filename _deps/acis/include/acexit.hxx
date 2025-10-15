/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __ACEXIT_HXX__
#define __ACEXIT_HXX__

#include "base.hxx"
#include "dcl_base.h"

#include "logical.h"
#include "tlkit_cb.hxx"

// Header for exit_callback
//
// The exit_callback implements standard exit for ACIS.  Anytime
// a standard C exit call is made within ACIS, that call will be
// redirected through this callback class.  At that point the platform
// specific implementation of exit_callback will obtain the exit
// from its natural exit stream.  Look in 3dt/main/<platform>/*stdio.cxx
// for the natural implementation of exit_callback.

/**
 * @file acexit.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

/**
 * Executes standard exit for ACIS.
 * <br><br><b>Role: </b>The <tt>exit_callback</tt> implements standard exit for ACIS. Any time
 * a standard C exit call is made within ACIS, that call is redirected through
 * this callback class. At that point the platform specific implementation of
 * <tt>exit_callback</tt> obtains the exit from its natural exit stream.
 */
class DECL_BASE exit_callback : public toolkit_callback
{

	friend class exit_callback_list;

protected:
    /**
	* Destroys this callback.
	*/
	virtual ~exit_callback();

public:
	// Returns FALSE if the standard exit should not be called
	// after all exit callbacks are called.

/**
 * Returns FALSE if the standard exit should not be called after all exit callbacks are called.<br><br>
 * @param callback
 *  type of callback.
 */
	virtual logical execute(int callback);
};

// Header for exit_callback_list
//
// The exit_callback_list implements a callback list.  This
// list provides the redirection necessary to allow the standard C
// exit routines to be called regardless of platform.
// This is an internal class.

/**
 * @nodoc
 */
class exit_callback_list : public toolkit_callback_list
{
public:
	void add(exit_callback *cb) { toolkit_callback_list::add(cb); }
	void append(exit_callback *cb) { toolkit_callback_list::append(cb); }

	void execute(int);
};
/**
 * Registers a new exit callback for ACIS.<br><br>
 * @param cb
 *  callback to register.
 */
DECL_BASE void add_acis_exit_cb(exit_callback *cb);
/**
 * Unregisters an exit callback for ACIS.<br><br>
 * @param cb
 *  callback to register.
 */
DECL_BASE void remove_acis_exit_cb(exit_callback *cb);
/** @} */
#endif // __ACEXIT_HXX__
