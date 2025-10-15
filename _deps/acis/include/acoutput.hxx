/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __ACOUTPUT_HXX__
#define __ACOUTPUT_HXX__
#include "base.hxx"
#include "dcl_base.h"
#include "tlkit_cb.hxx"
// Header for output_callback
//
// The output_callback implements standard output for ACIS.  Anytime
// a standard C output call is made within ACIS, that call will be
// redirected through this callback class.  At that point the platform
// specific implementation of output_callback will obtain the output
// from its natural output stream.  Look in 3dt/main/<platform>/*stdio.cxx
// for the natural implementation of output_callback.

/**
 * @file acoutput.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

/**
 * Creates output callback standard output for ACIS.
 * <br><br><b>Role: </b>The <tt>output_callback</tt> implements standard output for ACIS.
 * Any time a standard C output call is made within ACIS, that call is redirected
 * through this callback class. At that point the platform-specific implementation
 * of <tt>output_callback</tt> obtains the output from its natural output stream.
 */

class DECL_BASE output_callback : public toolkit_callback
{

	friend class output_callback_list;

protected:
	/**
	*The file pointer to which this callback relates.
	*/
	FILE *fp;

	/**
	* Destroys this callback.
	*/
	virtual ~output_callback();

public:
	// Constructor - remember the file to which this callback applies.
	/**
	 * C++ constructor, creating an <tt>output_callback</tt> using the specified parameters.
	 * <br><br><b>Role: </b>Remembers the file to which this callback relates.<br><br>
	 * @param out_fp
	 * output file pointer.
	 */
	output_callback(FILE *out_fp)
	{fp =out_fp ;}

	// Output methods
	// At least one of these MUST be implemented in derived classes
	// since the default implementations call each other.

	// Writes the string to the output device.
	/**
	 * Writes string to the output device.<br><br>
	 * @param str
	 * string to write.
	 */
	virtual int print_string(const char *str);

	// Writes from array data, nitems of size to the output device.
	/**
	 * Writes from array data, n items of size to the output device.<br><br>
	 * @param data
	 * items in list.
	 * @param size
	 * output device size.
	 * @param nitems
	 * number of items.
	 */
	virtual int write_data(const void *data, int size, int nitems);

	// Called in response to fflush() for the specified file
	/**
	 * Called in response to fflush for the specified file.
	 */
	virtual int flush();
};

// Header for output_callback_list
//
// The <tt>output_callback_list</tt> implements a callback list.  This
// list provides the redirection necessary to allow the standard C
// output routines to be called regardless of platform.
// This is an internal class.

/**
 * @nodoc
 */
class output_callback_list : public toolkit_callback_list
{
public:
	void add(output_callback *cb) { toolkit_callback_list::add(cb); }
	void append(output_callback *cb) { toolkit_callback_list::append(cb); }

	int print_string(FILE *fp, const char *str);
	int write_data(FILE *fp, const void *data, int size, int nitems);

	int flush(FILE *fp);
};

/**
 * Registers a new output callback for ACIS.<br><br>
 * @param cb
 *  callback to register.
 */
DECL_BASE void add_acis_output_cb(output_callback *cb);


/**
 * Unregisters an output callback for ACIS.<br><br>
 * @param cb
 *  callback to unregister.
 */
DECL_BASE void remove_acis_output_cb(output_callback *cb);
/** @} */
#endif //__ACOUTPUT_HXX__
