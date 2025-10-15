/* ORIGINAL: acis2.1/kernutil/subtype/subtype.hxx */
/* $Id: subtype.hxx,v 1.13 2002/08/16 19:28:36 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include <string.h>

// Classes and functions to support a general mechanism for
// representing subtypes of objects in save files, allowing unknown
// subtypes to be read and written back, and allowing shared objects
// to be shared in the save file.

//
// Modification---
//	01-Oct-97 bd	Move add_ref(), remove_ref() implementations to subtype.cxx
//

#if !defined( SHARED_IO_DEFS )
#define SHARED_IO_DEFS

#include "dcl_kern.h"
#include "logical.h"

#include "debugmsc.hxx"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end
/**
* @file subtype.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class SizeAccumulator;
class minimize_helper;
class subtype_object;
class restore_subtype_def;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *dispatch_restore_subtype(char const *,
												   char const * = NULL ,logical = FALSE);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN restore_subtype_def *search_subtype_table(char const * );

// Master object from which all subtype objects must be derived. This
// contains a use count, in case the object is sharable, and defines
// two virtual functions and a destructor.
/**
 * Defines the master object from which all subtype objects must be derived.
 * <br>
 * <b>Role:</b> This class defines the master object from which all subtype
 * objects must be derived. This object contains a use count (in case the object
 * is shareable) and defines two virtual functions and a destructor.
 */
class DECL_KERN subtype_object : public ACIS_OBJECT
{
	int reference_count;

	// An id for subtypes. Starts at 1, increments, and rolls-over
	// when it maxes out the unsigned int. Required for the caching system.
	// Should not be used otherwise. An id of zero is reserved for subtypes
	// in an invalid (non-id) state.
	unsigned int stid;

public:
	// Use count manipulation.
/**
 * Uses count manipulation.
 */
	void add_ref();
/**
 * Removes a reference.
 */
	void remove_ref();
/**
 * Returns the use count.
 */
	int ref_count() { return reference_count; };
/**
 * @nodoc
 */
	unsigned int subtype_id( void ) const { return stid; } 

/**
 * Returns <tt>TRUE</tt> if there is more than one reference - else, it returns <tt>FALSE</tt>.
 */
	logical mult_ref() const;

	// Constructor which initialises the use count.
/**
 * C++ constructor, creating a <tt>subtype_object</tt>.
 */
	subtype_object();

	// Virtual destructor, for those objects which need it.
/**
 * C++ destructor, deleting a <tt>subtype_object</tt> and all those objects that need it.
 */
	virtual ~subtype_object();

/**
 * Returns <tt>TRUE</tt> if this object type is unknown to the system - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Role:</b> This method returns <tt>FALSE</tt> as the default, so except for the system
 * unknown subtype, the method should be omitted from derived class definitions.
 */
	virtual logical unknown_type() const;

/**
 * Returns <tt>TRUE</tt> if this object is a placeholder, otherwise it returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Role:</b> This method is used exclusively during multithreaded restore.
 * Derived classes need not implement.
 */
	virtual logical placeholder_type() const;

	// Return the type of this object.
/**
 * Returns the type of <tt>subtype_object</tt>.
 */
	virtual int type() const = 0;

	// Return a pointer to a static string, being the externally-
	// meaningful type name for this subtype.
/**
 * Returns a pointer to a static string, which is the externally-meaningful type name for this subtype.
 */
	virtual char const *type_name() const = 0;

	// Test whether two subtype objects are the same. This is used for
	// sharing on restoration of old save files, and there is no
	// serious harm if equal values are treated as unequal, so strict
	// equality can be used quite safely.
/**
 * Determines if two subtype objects are the same.
 * <br><br>
 * <b>Role:</b> Use this method for sharing on restoration of old save files.
 * <br><br>
 * @param obj
 * subtype-object.
 */
	virtual logical operator==( subtype_object const &obj ) const;
/**
 * <tt>TRUE</tt> if two subtype objects are not the same.
 * <br><br>
 * <b>Role:</b> Use this method for sharing on restoration of old save files.
 * <br><br>
 * @param rhs
 * subtype-object.
 */
	logical operator!=( subtype_object const &rhs ) const {
		return !(*this == rhs );
	}

	// Test whether one subtype objects is in the set of the other subtype. This is used for
	// merging.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual logical operator>>( subtype_object const &rhs ) const {
		return *this == rhs;
	}

	// Write this object to a save file. Just deals with bracketing,
	// but calls save_data() for the meat.

	// STIBUGFIX erich approx_save begin
/**
 * Saves a subtype object with identifier and brackets, and enters it in the current tag table.
 * <br><br>
 * <b>Role:</b> If it is already there, just puts out a reference.
 */
	virtual void save() const;
	// STIBUGFIX erich approx_save end

	// Write the data for this object to a save file.
/**
 * Saves the information for the <tt>subtype_object</tt> to a save file.
 */
	virtual void save_data() const = 0;

	// Write this object in readable form, for debugging.
/**
 * Writes the <tt>subtype_object</tt> in readable form for debugging.
 * <br><br>
 * <b>Role:</b> If logical is <tt>TRUE</tt>, brief output is produced; if logical is <tt>FALSE</tt>,
 * long output is produced.
 * <br><br>
 * @param leader
 * leader for second and subsequent lines of output.
 * @param output
 * TRUE for brief output, FALSE for full output.
 * @param fp
 * file name.
 */
	virtual void debug(
				char const *leader,	// leader for second and subsequent
								// lines of output
				logical output,		// TRUE to produce brief output (if
								// relevant), FALSE for full output
				FILE * fp = debug_file_ptr
			) const = 0;

	// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*) {};
#ifdef INTERNAL_DEBUG_CHECKS
/**
 * @nodoc
 **/
	int get_debug_tag() const;
#endif
};
// Restore mechanism for subtype objects. Static declarations of
// objects of this class form themselves into a table containing
// the external (string) identifier of the particular subtype,
// together with a pointer to the correct restore routine. The
// generic restore routine reads the external identifier, and
// switches according to the table. Note that the save routine
// doesn't need any of this stuff, as the virtual function mechanism
// works fine for that.

// The table will probably be short, so can be simply a linear list,
// for ease of implementation. We keep this implementation private,
// so that we might some time have a more exotic version.
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef subtype_object *(*rest_subtype_fn_type)();
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN restore_subtype_def : public ACIS_OBJECT {
private:
	// subtype name
	char const *name;			
	// restore function
	rest_subtype_fn_type rest_fn;
	// linked list navigation
	restore_subtype_def *next;

public:
	restore_subtype_def( char const *, rest_subtype_fn_type );
	~restore_subtype_def();

	void add_to_subtype_table( void );
	void remove_from_subtype_table( void );

	// Restore an object of the referenced type, using the stored
	// restore function.

	subtype_object *restore();

	// Functions to search the table and call appropriate action
	// routines.

	friend DECL_KERN subtype_object *dispatch_restore_subtype(
									char const *,
									char const *,
									logical			// STI joe
								);
	friend DECL_KERN restore_subtype_def *search_subtype_table(
									char const * );
};

// Functions to maintain an indexed pointer table, used for saving
// and restoring shared data structure resources.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void init_subtype_io_table( logical is_restoring);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void empty_subtype_io_table(logical is_restoring);

// STIBUGFIX erich approx_save begin
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN int lookup_subtype_io_table( subtype_object const * );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN int add_to_subtype_io_table( subtype_object * );
// STIBUGFIX erich approx_save end
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN int assign_subtype_number();


/**
 * @nodoc
 */
class subtype_io_table_block {
	logical is_restoring;
public:
	subtype_io_table_block(logical restoring);
	~subtype_io_table_block();
};
/*! @} */
#endif
