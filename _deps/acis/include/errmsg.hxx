/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ERRMSG_HXX
#define ERRMSG_HXX
// Define macro for external entry definition.
#include "base.hxx"
#include "dcl_base.h"
#include "err_info_base.hxx"

//#define GME_BASE_ERRMSG

/**
* @file errmsg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

class message_loader;

// Classes to define messages for the error message system.
// Code defining instances of these classes is generated from the message
// files by the build tools.

/**
 * An integral value used by the ACIS message and error system to signify a
 * message code. This is typically seen used with sys_warning, sys_error, and
 * the message module system.
 * <br>
 * @see sys_warning, sys_error, message_module
 */
typedef int err_mess_type;


/**
 * Message list item.
 * <br>
 * <b>Role:</b> A structure to contain one identifer/message pair.
 * @see message_module
 */
typedef struct {
	char const *ident;		// A short identifier string for the message
	char const *message;	// A more detailed message string
} message_list;


/**
 * Contains all messages for a module.
 * @see message_list
 */
class DECL_BASE message_module : public ACIS_OBJECT {

	// Befriend the message_loader class which needs to have full access
	// to load messages from an external database.
	friend class message_loader;

private:

	char const *module_name;		// The module name (e.g. sg_husk/sweep)
	int index;						// A unique number assigned to this module
	int msg_cnt;					// The number of messages in this module
	message_list const *messages;	// The messages for this module
	message_module *next_mm;		// The next module

	int loaded;						// was this module loaded by a message_loader
	message_loader *my_loader;		// the message_loader which did the load

public:
	/**
	 * C++ constructor, creating a <tt>message_module</tt> using the specified parameters.
	 * <br><br>
	 * @param name
	 * name of the module.
	 * @param message
	 * The array of messages for this module, terminated by an entry with a NULL identifier.
	 */
	message_module(	char const * name,
					message_list const * message );
	message_module(	const char* gme,char const * name,
					message_list const * message );

	/**
	 * C++ destructor, deleting a <tt>message_module</tt>.
	 */
	~message_module();


	/**
	 * Returns the next message module.
	 */
	message_module * next_message_module() { return next_mm; }

	/**
	 * Generates an error number from an offset into a module's message list.
	 * <br><br>
	 * <b>Role:</b> The ACIS build tools generate a header file containing symbolic definitions
	 * of error numbers using this method.
	 * <br><br>
	 * @param offset
	 * Offset into module's message list.
	 */
	err_mess_type gme_message_code( int offset ) const;
	#ifdef GME_BASE_ERRMSG
	err_mess_type message_code(int offset) const {
		return gme_message_code(offset);
	}
	#else
	err_mess_type message_code( int offset ) const;
	#endif
	/**
	 * Returns the module's name.
	 */
	char const *module() const;
	char const *gme_module() const;
	/**
	 * Returns the message module (unique) index.
	 */
	int message_index()		{	return index;	}

	/**
	 * Returns a pointer to the message data for an offset into a module's message list.
	 * <br><br>
	 * @param offset
	 * Offset into module's message list.
	 */
	message_list const * message( int offset ) const;
	message_list const * gme_message( int offset ) const;

	// Static functions to initiate load and unload of all message_modules.
	// Loading is handled by a message_loader which interfaces to an external database.

	/**
	 * This static function initiates load of all <tt>message_modules</tt>.
	 * <br><br>
	 * <b>Role:</b> Loading is handled by a <tt>message_loader</tt> which interfaces to an external database.
	 * This function does nothing if the <tt>message_module</tt> is already loaded.
	 */
	static void loadAll();

	/**
	 * This static function initiates unload of all <tt>message_modules</tt>.
	 * <br><br>
	 * <b>Role:</b> Unloading is handled by a <tt>message_loader</tt> which interfaces to an external database.
	 * This may be used to release memory resources used by the strings.
	 */
	static void unloadAll();

	// Functions used by the message_loader class, and deliberately kept private.

	/**
	 * Returns the address of the char* pointer so the caller can assign their own string into the <tt>message_list</tt>.
	 * <br><br>
	 * @param idx
	 * list index.
	 */
	char **messageForUpdate(int idx);
	char **gme_messageForUpdate(int idx);

	/**
	 * This function loads the message module.
	 */
	void load();
	void gme_load();
	/**
	 * Remove this module from the linked list.
	 * <br><br>
	 * <b>Role:</b> If there is no previous module in the list,
	 * then the element being removed is the head element.
	 */
	void unload();
	void gme_unload();
};

/**
 * @nodoc
 */
DECL_BASE message_list const * gme_get_message( err_mess_type err_num );
#ifdef GME_BASE_ERRMSG
message_list const* get_message(err_mess_type err_num);
#else
DECL_BASE message_list const * get_message( err_mess_type err_num );
#endif // GME_BASE_ERRMSG

DECL_BASE char const * gme_get_module( err_mess_type err_num );
/**
 * Replace an error message with a custom message in the message_module system.
 * <br><br>
 * <b>Role:</b> An application may want to replace the error messages associated with error numbers 
 * in the ACIS message_module system with custom or translated message. This can be accomplished by 
 * calling the set_message function. This function accepts an error number and a reference to a 
 * new message to associate with the number. The function also returns the pointer to the original string 
 * so that one can reset the message if desired. The message_module system is not responsible for 
 * any memory allocations associated with the installed messages and care should be taken to assure 
 * that the lifetime of the installed messages is either longer than that of the message_module system 
 * or that they are un-installed at some point. Restoring the original string or installing a NULL string 
 * will avoid potential problems during termination.
 * @param err_num
 * The error number.
 * @param err_mess
 * The new error message
 */
DECL_BASE const char * set_message( err_mess_type err_num, const char * err_mess);
DECL_BASE const char * gme_set_message( err_mess_type err_num, const char * err_mess);
/** @} */
#endif

