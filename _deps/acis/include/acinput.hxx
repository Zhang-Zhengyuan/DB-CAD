/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __ACINPUT_HXX__
#define __ACINPUT_HXX__
#include "base.hxx"
#include "dcl_base.h"
#include "tlkit_cb.hxx"
// Header for input_callback
//
// The input_callback implements standard input for ACIS.  Anytime
// a standard C input call is made within ACIS, that call will be
// redirected through this callback class.  At that point the platform
// specific implementation of input_callback will obtain the input
// from its natural input stream.  Look in 3dt/main/<platform>/*stdio.cxx
// for the natural implementation of input_callback.
/**
 * \defgroup ACISCALLBACKS Callback Functions
 * \ingroup ACISBASE
 *
 */
/**
 * @file acinput.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

/**
 * Creates the standard input for ACIS.
 * <br><br><b>Role: </b>The <tt>input_callback</tt> implements standard input for ACIS.
 * Any time a standard C input call is made within ACIS, that call is redirected
 * through this callback class. At that point the platform specific implementation
 * of <tt>input_callback</tt> obtains the input from its natural input stream.
 */
class DECL_BASE input_callback : public toolkit_callback
{

	friend class input_callback_list;

protected:
	/**
	*The file pointer to which this callback relates
	*/
	FILE *fp;

	/**
	* Destroys this callback.
	*/
	virtual ~input_callback();

public:
	// Constructor - remember the file to which this callback applies.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.<br><br>
 * @param in_fp
 * file pointer.
 */
	input_callback(FILE *in_fp)
	{fp = in_fp;}

	// Input methods
/**
 * Gets input character from input stream.
 */
	// Returns the next character from the input stream.
	virtual int do_getc();
/**
 * Pushes character back onto input stream.<br><br>
 * @param c
 * character to push back.
 */
	// Pushes the character back onto the input stream
	virtual int do_ungetc(int c);
/**
 * Returns a non-zero if end of file reached.
 */
	// Returns non-zero if end of file has occured.
	virtual int do_feof();
/**
 * Returns a non-zero if an error has occurred.
 * <br><br><b>Role: </b>Clears end of file and error indicators.
 */
	// Returns non-zero if an error has occured.
	virtual int do_ferror();
/**
 * Clears the end of file and error indicators.
 */
	// Clears the end of file and error indicators.
	virtual void do_clearerr();
};


// Header for input_callback_list
//
// The input_callback_list implements a callback list.  This
// list provides the redirection necessary to allow the standard C
// input routines to be called regardless of platform.
// This is an internal class.

/**
 * @nodoc
 */
class DECL_BASE input_callback_list : public toolkit_callback_list
{
public:
	void add(input_callback *cb) { toolkit_callback_list::add(cb); }
	void append(input_callback *cb) { toolkit_callback_list::append(cb); }

	// These methods call the coresponding method for the first class
	// in the callback list which supports the specified file.
	// If no such class exists, the standard OS function is called.

	int do_getc(FILE *fp);
	int do_ungetc(FILE *fp, int c);
	int do_feof(FILE *fp);
	int do_ferror(FILE *fp);
	void do_clearerr(FILE *fp);
};


/**
 * Registers a new input callback for ACIS.<br><br>
 * @param cb
 *  callback to register.
 */
DECL_BASE void add_acis_input_cb(input_callback *cb);

/**
 * Unregisters a new input callback for ACIS.<br><br>
 * @param cb
 *  callback to register.
 */
DECL_BASE void remove_acis_input_cb(input_callback *cb);
/** @} */
#endif // __ACINPUT_HXX__
