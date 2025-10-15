/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define debugging routines and list handling for debugging data
// structures.
#if !defined( DEBUG_ENTITY )
#define DEBUG_ENTITY
#include "dcl_kern.h"
#include "logical.h"
#include "lists.hxx"
#include "map.hxx"
#include "context_cb.hxx"
#include "debugmsc.hxx"
/**
 * \defgroup ACISDEBUG Debug
 * \ingroup KERNAPI
 *
  */
/**
* @file debug.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDEBUG
 *
 * @{
 */
class ENTITY;
class TRANSFORM;
class SPAbox;
class SPApar_box;

/**
 * Defines a simple list pointer, which allows all active lists to be scanned.
 * <br>
 * <b>Role</b>:This class defines a simple list pointer, which allows all active lists
 * to be scanned.
 * @see ENTITY_LIST
 */

/**
 * Defines a simple list pointer, which allows all active lists to be scanned.
 * <br>
 * <b>Role</b>:This class defines a simple list pointer, which allows all active lists
 * to be scanned.
 * @see ENTITY_LIST
 */

/**
 * Defines a simple list pointer, which allows all active lists to be scanned.
 * <br>
 * <b>Role:</b> This class defines a simple list pointer, which allows all active lists
 * to be scanned.
 * @see ENTITY_LIST
 */

class DECL_KERN DEBUG_LIST : public ACIS_OBJECT {
	// Simple list pointer, allowing all active lists to be scanned.

	DEBUG_LIST *next_list;

	// Name for entity type in the list, for labelling output.

	char *title;

	// List of entities to be printed out.

	ENTITY_LIST contents;

	// The number of entities from the list already displayed -
	// subsequently-displayed entities may add more to this list, so
	// we need to know where to start next time.

	unsigned number_printed;

public:
	// Constructor makes an empty list.
/**
 * C++ initialize constructor.
 * <br><br>
 * <b>Role:</b> Requests memory for this object, initialize its members, and then link it
 * at the end of the chain of headers. The type argument is in three classes:
 * <br> <tt>NULL</tt> pointer - The type name is to be obtained from the first entity in the list
 * <br> null string - Same as <tt>NULL</tt> pointer, except that when performing debug printout,
 * the list is not printed
 * <br> non-null string - The type name to be used when identifying the list
 * <br><br>
 * @param type
 * type name.
 */
 	DEBUG_LIST( char const *type );

	// Destructor clears the list, and sets the owning pointer to NULL.
/**
 * C++ destructor, deleting a <tt>DEBUG_LIST</tt>.
 */
 	virtual ~DEBUG_LIST();

	// Search for the given entity in the list, optionally adding
	// it, and returning the index number.
/**
 * Searches for the given entity in the list, optionally adding it, and returning the index number.
 * <br><br>
 * @param entity
 * entity to lookup.
 * @param list_option
 * add to list option.
 */
 	int lookup( ENTITY const *entity, logical list_option );

	// Count the entities in the list.
/**
 * Counts the entities in the list.
 */
 	int count() const;

	// Obtain the indexed entity from the list.
/**
 * Obtains the indexed entity from the list.
 * <br><br>
 * @param index
 * index number of entity.
 */
 	ENTITY const *fetch( int index ) const;

	// Display each entity in the list, starting at "number_printed".
/**
 * Displays each entity in the list, starting at the <tt>number_printed</tt>.
 * <br><br>
 * @param fname
 * file name.
 */
 	logical debug_list( FILE *fname );

	// Determine the total space occupied by all the entities in
	// the list (though not including subsidiary structure).
/**
 * Determines the total space occupied by all of the entities in the list.
 * <br><br>
 * <b>Role:</b> It does not include subsidiary structures.
 */
 	virtual unsigned size_list() const;

	// Display one entity from the list. This is an anachronism, but
	// is retained for backwards compatibility in this release. In
	// future, debug_list() will call ENTITY::debug_ent() directly.
/**
 * Displays one entity from the <tt>DEBUG_LIST</tt>.
 * <br><br>
 * @param entity
 * entity.
 * @param fname
 * file name.
 */
 	virtual void debug( ENTITY const *entity, FILE *fname ) const;

	// Allow unprivileged utilities to follow the list pointer and
	// get the entity type.
/**
 * Allows unprivileged utilities to follow the list pointer to return the next entity type.
 */
 	DEBUG_LIST *next() { return next_list; }
/**
 * Allows unprivileged utilities to follow the list pointer to return the entity type.
 */
 	char const *entity_name() { return title; }

	// Assignment operator could not be generated warning
	// is taken care of by adding a private one.  PRS
private:
	DEBUG_LIST& operator=(const DEBUG_LIST&) {return *(DEBUG_LIST *)NULL_REF;}
};


// Removed PTR_PTR_MAP MACRO.
/*
// tbrv
*/
/**
 * @nodoc
 */

class DECL_KERN intDEBUG_LIST_MAP : public VoidVoidMap {

public:

	void put(int* m, DEBUG_LIST* f) {

		VoidVoidMap::put((void*)m, (void*)f);
	}

	DEBUG_LIST* get(int* m) {

		return (DEBUG_LIST*) VoidVoidMap::get((void*)m);
	}
};

/*
// tbrv
*/
/**
 * @nodoc
 */

class DECL_KERN debug_map : public ACIS_OBJECT
{
	friend class DEBUG_LIST;

public:
	intDEBUG_LIST_MAP	map;

	// Pointer to the start of the current list of lists. Each time this
	// package is used, this starts empty (NULL). Whenever a new entity type
	// is encountered, it generates a new list header, and chains it on here.
	// The chain is ordered on time of first occurrence.
	DEBUG_LIST *		list_root;

	// Global used to determine whether to follow internal lists of
	// entities of equal status or not. This is generally TRUE if we are
	// debugging a specific entity, FALSE if we have constructed lists
	// previously.
	logical				debug_siblings;

	DEBUG_LIST* get(int* type)
	{
		return map.get(type);
	}

	void put(int* type, DEBUG_LIST* list)
	{
		map.put(type, list);
	}

	void clear(void)
	{
		map.clear();
	}
};


// Top level functions to produce data structure listings.

/**
* Print out the given entity and all its subsidiary structure.
* <br><br>
* <b>Role:</b> Prints to the specified output file the debug information for an entity 
* and all its subsidiary entities.
* <b>Effect:</b> System routine
* <br><br>
* @param SPAptr
* entity.
* @param debug_file_ptr
* file pointer.
*/
DECL_KERN void debug_entity( ENTITY const* SPAptr, FILE* = debug_file_ptr );

// Print out whatever is in the currently-active debug lists, together
// with subsidiary structure.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_lists( FILE * = debug_file_ptr );

/**
* Prints the size of the <tt>ENTITY</tt>.
* <br><br>
* <b>Role:</b> Prints to the specified output file the size of an entity and all its 
* subsidiary entities.  Output information also includes the number of
* each type of subsidiary entity.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param SPAptr
* entity.
* @param debug_file_ptr
* file pointer.
**/
DECL_KERN void debug_size( ENTITY const* SPAptr, FILE* = debug_file_ptr );

// Utilities which may be useful externally, but not very.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_all( FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void size_all( FILE * );

/**
* Clears the <tt>DEBUG_LISTs</tt>.
* <br><br>
* <b>Effect:</b> System routine
**/

DECL_KERN void clear_debug_lists();

// Functions to put out individual entries in a debug listing. Not
// all member types are covered, but a moderately useful selection.
// For anything else, use debug_title followed by fprintf guarded by
// (fp != NULL). The functions for pointers have the side-effect
// of adding the pointer to the appropriate list if appropriate.
// All these functions are safe if fp is NULL - the pointer ones still
// add to the lists, for the sake of sizing.

// Put out the first line in the entry, giving the current entity

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_header( ENTITY const *, FILE * );

// Put out the initial string for a line of debug output

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_title( char const *, FILE * );

// Global variable to select whether to follow sibling pointers or not
#ifdef THREAD_SAFE_ACIS
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN safe_integral_type<logical> debug_siblings;
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN logical debug_siblings;
#endif

#ifdef THREAD_SAFE_ACIS
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN safe_object_pointer<debug_map> dmap;
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN debug_map *dmap;
#endif

// Put out an ENTITY pointer, entering it into the lists never,
// if "debug_siblings" set, and always respectively. Preceded
// by a title, and followed by a newline.

/**
* Prints to the output file an <tt>ENTITY</tt> pointer.
* <br><br>
* <b>Role:</b> Prints to the specified output file a formatted entity pointer, 
* which should already be contained in the debug lists.
* The pointer is preceded by a title and followed by a new line.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title
* @param SPAptr
* entity.
* @param fp
* file pointer.
**/
DECL_KERN void debug_old_pointer( char const* title, ENTITY const* SPAptr, FILE* fp );

/**
* Prints to the output file an <tt>ENTITY</tt> pointer.
* <br><br>
* <b>Role:</b> Prints to the specified output file a formatted entity pointer, 
* adding it to the debug lists if debug_siblings is <tt>TRUE</tt> and it is necessary.
* The pointer is preceded by a title and followed by a new line.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title
* @param SPAptr
* entity.
* @param fp
* file pointer.
**/
DECL_KERN void debug_sib_pointer( char const* title, ENTITY const* SPAptr, FILE* fp);

/**
* Prints to the output file an <tt>ENTITY</tt> pointer.
* <br><br>
* <b>Role:</b> Prints to the specified output file a formatted entity pointer, 
* adding it to the debug lists if it is necessary.
* The pointer is preceded by a title and followed by a new line.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title
* @param SPAptr
* entity.
* @param fp
* file pointer.
**/
DECL_KERN void debug_new_pointer( char const *title, ENTITY const *SPAptr, FILE *fp );

/**
 * @nodoc
 * Same again, but just the pointer without leader or newline.
 */
DECL_KERN void debug_old_pointer( ENTITY const *, FILE * );
/**
 * @nodoc
 */

DECL_KERN void debug_sib_pointer( ENTITY const *, FILE * );
/**
* Prints to the output file an <tt>ENTITY</tt> pointer.
* <br><br>
* <b>Role:</b> Outputs an <tt>ENTITY</tt> pointer and enters it into the list. This
* pointer can be preceded by a title and followed by a new line.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param SPAptr
* entity pointer.
* @param fp
* file pointer.
**/

DECL_KERN void debug_new_pointer( ENTITY const *SPAptr, FILE *fp );

// Put out simple objects with titles.
/**
* Prints to the output file a string value with the given title.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title.
* @param str
* string value.
* @param fp
* file pointer.
**/

DECL_KERN void debug_string( char const *title, char const *str, FILE *fp );

/**
* Prints out a real number with title and newline.
* <br><br>
* <b>Role:</b> Outputs a real number. The value can be preceded by a title and
* followed by a new line.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title.
* @param val
* real value.
* @param fp
* file pointer.
**/

DECL_KERN void debug_real( char const *title, double val, FILE *fp );

/**
* Prints an integer with the given title to the output file.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param title
* title.
* @param val
* integer value.
* @param fp
* file pointer.
**/

DECL_KERN void debug_int( char const *title, int val, FILE *fp );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_box( char const *, SPAbox const &, FILE * );
DECL_KERN void debug_par_box( char const *, SPApar_box const &, FILE * );

// Return a pointer to a static string containing a suitable blank
// title string for the second and subsequent lines of an item.

/**
* Prints to the output file a pointer to a static string containing a suitable blank title string for the second and subsequent lines of an item.
* <br><br>
* <b>Effect:</b> System routine
**/

DECL_KERN char const *debug_leader();

// ********************************************************************
// Functions retained for backwards compatibility. These will be
// withdrawn some time in the future.

// Put out a SPAbox with a standard label. Replaced by the new version
// with an explicit label.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_box( SPAbox *, FILE * );

// Put out a transform, entering it into its list, and with a
// standard label. Replaced by a normal use of debug_new_pointer.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_transform( TRANSFORM *, FILE * );

// Add an entity pointer to its list, without displaying anything.
// Replaced by use of debug_new_pointer with NULL FILE pointer.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void debug_add( ENTITY const * );


/**
 * @nodoc
 * Construct a string representation of an ENTITY pointer, giving
 * the entity type, its SPAposition in that entity's list, and the
 * pointer address. Optionally enters the pointer into its list if not
 * already there. Use debug_old/sib/new_pointer instead.
 */
DECL_KERN char *format_pointer( ENTITY const *, logical );

// Macros to use format_pointer

#define old_entity( SPAptr ) format_pointer( SPAptr, FALSE )
#define new_entity( SPAptr ) format_pointer( SPAptr, TRUE )
#define sib_entity( SPAptr ) format_pointer( SPAptr, debug_siblings )
/** @} */
#endif
