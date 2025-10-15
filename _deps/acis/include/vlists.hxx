/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for lists of void*.

// This class implements a variable-length list of void*'s.
// It provides a constructor (which creates an empty list) and
// destructor, functions to add an void* (only if not already there),
// and look one up by pointer value, and to count the number of
// void*'s listed.  Also provides an overloaded "[]" operator for
// access by SPAposition.

// The current implementation uses hashing so that look up is fast
// provided lists are not very long; it is also efficient for very
// short lists and for repeated lookups of the same void*.


#if !defined( VLISTS )
#define VLISTS

#include "dcl_base.h"
#include "logical.h"
#include "mmgr.hxx"
/**
 * @file vlists.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */
class LIST_HEADER;

/**
 * Creates a variable-length list of <tt>void*</tt>'s.
 * <br>
 * <b>Role:</b> This class implements a variable-length list of <tt>void*</tt>'s. It
 * provides a constructor (which creates an empty list) and destructor
 * functions to add a <tt>void*</tt> (only if not already there), to look one up by
 * pointer value, and to count the number of <tt>void*</tt>'s listed. It also provides
 * an overloaded <tt>[]</tt> operator for access by position.
 * <br><br>
 * For best performance in loops that step through this list, have the loops
 * increment rather than decrement the index counter. Internal operations for
 * methods like <tt>operator[]</tt> and <tt>remove</tt> store the index counter from the previous
 * operation allowing faster access to the next indexed item when indexing up.
 * <br><br>
 * The current implementation uses hashing so that look up is fast provided
 * lists are not very long; it is also efficient for very short lists and for
 * repeated lookups of the same <tt>void*</tt>.
 */
class DECL_BASE VOID_LIST : public ACIS_OBJECT {

	LIST_HEADER *header;	// points to the actual list - this way
							// we can change it all without recompiling

 public:

/**
 * C++ constructor, creating a <tt>VOID_LIST</tt>.
 */
	VOID_LIST();		// constructor for generic lists

/**
 * C++ destructor, deleting a <tt>VOID_LIST</tt>.
 */
	~VOID_LIST();		// destructor

	// STL pcp 05-Nov-1999 added explicit operator= and copy
	// constructor which copy the whole list (complete with deleted
	// entries, if any, so that the indices in the copy match those in
	// the original).  Use with care...

/**
 * Copy constructor which copies the whole list (complete with deleted entries, if any, so that the indices in the copy match those in the original).
 * <br><br>
 * @param vd
 * entity to copy.
 */
	VOID_LIST (VOID_LIST const& vd);

/**
 * Explicit operator.
 * <br><br>
 * @param vd
 * pointer.
 */
	VOID_LIST& operator= (VOID_LIST const& vd);
	// STL pcp 05-Nov-1999 ends

	// Empty a list ready for construction of a new one.

/**
 * Empties a list ready for construction of a new one.
 */
	void clear();


	// Add an item to the list if not already there, and
	// always return the index.

/**
 * Adds an item to the list if not already there, and always returns the index.
 * <br><br>
 * @param vp
 * void pointer.
 */
	int add( void * vp);


	// Search for an item in the list.

/**
 * Searches for an item in the list, and returns its index number, or -1 if it is not there.
 * <br><br>
 * @param item
 * item to find.
 */
	int lookup( void const * item) const;


	// Delete an item from the list. This does not free space, and
	// leaves a tombstone in the linear list which count() and
	// operator[] will notice, but subsequently lookup() will not
	// find this item, nor will init()/next(). The return value is
	// the lookup() value of the old entry.

/**
 * Deletes an item from the list.
 * <br><br>
 * <b>Role:</b> This does not free space, and leaves a tombstone in the linear
 * list which <tt>count</tt> and <tt>operator []</tt> will notice. But <tt>lookup</tt> will not find
 * this item, nor will <tt>init</tt> or <tt>next</tt>. The return value is the <tt>lookup</tt> value of the old entry.
 * <br><br>
 * @param item
 * pointer to item.
 */
	int remove( void const * item);


	// Count how many item*'s there are in the list (including
	// deleted entries).

/**
 * Count how many <tt>item*</tt>'s there are in the list including deleted entries.
 */
	int count() const;

	// Count how many entities there are in the list
	// not including deleted entries.  Uses the iterator

/**
 * Counts how many entities there are in the list not including deleted entries - uses the iterator.
 */
	int iteration_count() const;

	// Return the indexed item, or NULL if the index is out of
	// range, or LIST_ENTRY_DELETED if the indexed entry has been
	// deleted.

/**
 * Returns the indexed item, or <tt>NULL</tt> if <tt>index</tt> is out of range, or <tt>LIST_ENTRY_DELETED</tt> if the indexed entry has been deleted.
 * <br><br>
 * @param index
 * index number.
 */
	void *operator[]( int index) const;


	// Return item*'s in list order, ignoring deleted items. Call
	// init() once, then next() repeatedly until it returns NULL.
	// Note that next() returns the undeleted item most closely
	// following the one most recently returned by next() or
	// operator[], except that if that value was NULL the value
	// of next() is undefined.

/**
 * Returns item*'s in list order, ignoring deleted items.
 * <br><br>
 * <b>Role:</b> Call <tt>init</tt> once, then <tt>next</tt> repeatedly until it returns <tt>NULL</tt>.
 * Note that <tt>next</tt> returns the undeleted item most closely following the one
 * most recently returned by <tt>next</tt> or <tt>operator []</tt>, except that if that value
 * was <tt>NULL</tt>, the value of <tt>next</tt> is undefined.
 */
	void init() const;

/**
 * Returns item*'s in list order, ignoring deleted items.
 * <br><br>
 * <b>Role:</b> Call <tt>init</tt> first time, then <tt>next</tt> repeatedly until it returns
 * <tt>NULL</tt>. Note that <tt>next</tt> returns the undeleted item most closely following
 * the one most recently returned by <tt>next</tt> or <tt>operator []</tt>, except that if that
 * value was <tt>NULL</tt> the value of <tt>next</tt> is undefined.
 */
	void *next() const;

 /**
  * Returns the next non deleted entry after the index given without affecting the member variables used by <tt>init</tt> and <tt>next</tt>.
  * <br><br>
  * <b>Role:</b> This allows clients to create iterators that can be multiply instantiated and which run
  * independently of each other. This is accomplished simply by giving the the user the appropriate
  * variables to save themselves.
  * <br><br>
  * @param from_index
  * integer.
  */
	void *next_from(int &from_index) const;

/**
 * Returns the size of the list.
 * <br><br>
 * @param countSelf
 * list or header toggle.
 */
	int byte_count(logical countSelf = TRUE) const;

/**
  * Sorts the list based upon the user-supplied comparison function.
  * <br><br>
  * <b>Role:</b>  Implements the runtime qsort function to sort the list and expects the input compare funcion
  * to have the same behavior as described for qsort. The return value should be less than zero if entry 1
  * is less than entry 2, greater than zero if entity 1 is greater than entity 2, and zero if they are the same.
  * Tombstones are removed before sorting and do not have to be considered by the compare function. 
  * The sorted list is obviously re-indexed, which may occur even when the list does not actually 
  * require sorting changes.
  * <br><br>
  * @param compare_func
  * comparison function
  */
	void sort( int (*compare_func)(const void* ent1, const void* ent2 ) );

};
/**
 * @nodoc
 * This is the constant that is returned when an indexed lookup on an <tt>ENTITY_LIST</tt> is out of range, 
 * or the indexed entry has been deleted.
 */
extern DECL_BASE void * const LIST_ENTRY_DELETED;

/** @} */
#endif
