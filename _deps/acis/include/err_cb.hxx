/* ORIGINAL: 3dt2.1/scheme/err_cb.hxx */
// $Id: err_cb.hxx,v 1.8 2000/12/26 19:05:54 products Exp $
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
//    Define the scheme_error_callback class.  The scheme_error_callback
//    class is a base class from which you can derive other classes which
//    will can be used to perform cleanup when an error occurs during
//    evaluation of a scheme expression.
//    To use this class, derive a new class from scheme_error_calback,
//    and overload the virtual 'execute' method.  To have your callback
//    get called, add it to the error callback list (which you can get by
//    calling get_scheme_error_callback_list()
//----------------------------------------------------------------------
#ifndef err_cb_hxx
#define err_cb_hxx
#include "tlkit_cb.hxx"
/**
 * @file err_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <err_cb.hxx>
 * @{
 */
/**
 * @nodoc
 */
class scheme_error_callback : public toolkit_callback
{

public:
	scheme_error_callback() {}
	virtual ~scheme_error_callback() {}
   /**
	* This is the only interesting method.  To create a error cleanup callback,
	* derive a new class from scheme_error_callback, and overload this virtual
	* method to do the cleanup for your application.
    */
	virtual void execute();
};
/**
 * Provides a type-safe interface to the <tt>toolkit_callback_list</tt> and the execute method.
 * <br>
 * <b>Role:</b> This class derives from the <tt>toolkit_callback_list</tt> class, and it
 * provides a type-safe interface to it and the execute method.
 */
class scheme_error_callback_list : public toolkit_callback_list
{
public:

/**
 * Adds a scheme error callback to the list.
 * <br><br>
 * @param cb
 * scheme error callback.
 */
	void add(scheme_error_callback* cb) { toolkit_callback_list::add(cb); }

/**
 * Appends a scheme error callback list.
 * <br><br>
 * @param cb
 * scheme error callback.
 */
	void append(scheme_error_callback* cb) { toolkit_callback_list::append(cb); }

/**
 * Cleans up the application.
 * <br><br>
 * <b>Role:</b> However, a new class must be derived from <tt>scheme_error_callback_list</tt>,
 * and this method must be overloaded.
 */
	void execute();
};
/**
 * Gets a global list of scheme error callbacks.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/

scheme_error_callback_list& get_scheme_error_callback_list();

#endif
