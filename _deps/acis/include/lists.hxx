/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for lists.
// This class implements a variable-length list of entities.
// It provides a constructor (which creates an empty list) and
// destructor, functions to add an entity (only if not already there),
// and look one up by pointer value, and to count the number of
// entities listed.  Also provides an overloaded "[]" operator for
// access by SPAposition.

// The current implementation uses hashing so that look up is fast
// provided lists are not very long; it is also efficient for very
// short lists and for repeated lookups of the same entity.

#if !defined( LISTS_HXX )
#define LISTS_HXX

#include "acis.hxx"
#include "dcl_kern.h"
#include "dcl_base.h"
#include "logical.h"
#include "list_header.hxx"
#include "lists_iterator.hxx"

/**
 * @file lists.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */

class ENTITY;
class LIST_HEADER;
/**
 * Implements a variable length list of entities with set and index behavior.
 * <br>
 * <b>Role:</b> This class provides a constructor (which creates an empty list), a destructor (which returns used memory),
 * a function to add an entity if not already in the list (i.e. set behavior), a function to remove an entry (leaving a tombstone),
 * a function to look up an entity by pointer value or index, a function to count the number of entries in the list (including tombstones),
 * and a function to return the number of live entries in the list (iteration_count) excluding the tombstones. 
 * Also provides an overloaded <tt>"[ ]"</tt> index operator for access by position.
 * <br><br>
 * The preferred way of accessing the data items is through <tt>ENTITY_LIST::init</tt> method, which rewinds the list;
 * and <tt>ENTITY_LIST::next</tt> method, which returns the next undeleted item. Using <tt>init</tt> and <tt>next</tt>
 * will avoid removed entries, i.e., tombstones.
 * <br><br>
 * The current implementation uses hashing. This results in efficient searching, inserting, and deleting of entries.
 * Furthermore, this data structure has been tuned so that it is efficient for both small lists and large lists,
 * as well as repeated lookups of the same entity.
 * <br><br>
 * <pre>
 * ENTITY_LIST my_face_list;
 * api_cover_sheet(sheet_body, new_surface,
 *  	my_face_list);
 * ENTITY* my_list_item
 * my_face_list.init();
 * while ((my_list_item=my_face_list.next()) != NULL){
 * 	if is_FACE(my_list_item){
 * 	.
 *	.
 * 	.
 * 	}
 *  }
 * </pre>
 * <br>
 * <b>Implementation Notes:</b>
 * <br><br>
 * An <tt>ENTITY_LIST</tt> is not an <tt>ENTITY</tt>. Therefore, it cannot be saved to <tt>SAT/SAB</tt>,
 * nor can it "roll" within history.
 * <br><br>
 * If an <tt>ENITTY</tt> in added to the <tt>ENTITY_LIST</tt> then subsequently lost, a stale pointer
 * will reside within the <tt>ENTITY_LIST</tt>. In other words, the <tt>ENTITY_LIST</tt> has no
 * knowledge of the lifetime of any of its entries.
 * <br><br>
 * Refer to the <tt>SPACOLLECTION</tt> entity if you need a container that stores a list of entities,
 * can be saved/restored, can roll through history, and has knowledge of the lifetime of its members.
 * <br><br>
 * Using the <tt>first</tt> and <tt>next</tt> methods will result in efficient list traversal 
 * and will avoid tombstone entries. Using the array-operator to index entries is efficient for
 * front-to-back list traversal, but is less efficient when randomly accessing entries. Also,
 * indexing using the array-operator <i>will</i> return tombstone entries.
 * <br><br>
 * If you decide to use the array operator for indexing, you can detect tombstone entries by 
 * comparing the returned pointer against the constant <tt>LIST_ENTRY_DELETED</tt> (-1).
 * <br><br>
 * To obtain the count of entries in the list minus tombstones, use the <tt>iteration_count</tt> 
 * method. The <tt>count</tt> method will return the count of entries in the list including
 * tombstones.
 * <br><br>
 * @see SPACOLLECTION, DEBUG_LIST, EE_LIST, HISTORY_STREAM, entity_notification_handler, restore_data
 */
class DECL_KERN ENTITY_LIST : public ACIS_OBJECT {
	friend class EE_LIST;	// An ENTITY version of ENTITY_LIST

	LIST_HEADER *header;

public:

 /**
  * C++ constructor, creating an <tt>ENTITY_LIST</tt>.
  */
	ENTITY_LIST();		// constructor for generic lists

 /**
  * C++ destructor, deleting an <tt>ENTITY_LIST</tt>.
  */
	~ENTITY_LIST();		// destructor

 /**
  * C++ constructor, creating an <tt>ENTITY_LIST</tt> from an array.
  */
	ENTITY_LIST( int count, ENTITY **entity_array, logical check = TRUE );

 /**
  * C++ constructor, creating an <tt>ENTITY_LIST</tt> from a single ENTITY.
  */
	ENTITY_LIST( ENTITY* );

/**
  * C++ copy constructor, which copies the whole list (complete with deleted entries, if any), so that the indices in the copy match those in the original.
  * <br><br>
  * @param list_copy
  * list to copy.
  */
	ENTITY_LIST (ENTITY_LIST const& list_copy);

 /**
  * <tt>Assignment</tt> operator performs a full copy of the list, complete with tombstones for deleted entries so that the indices are the same.
  * <br><br>
  * @param entity_list
  * entity list.
  */
	ENTITY_LIST& operator= (ENTITY_LIST const &entity_list);

	// Move constructor

	ENTITY_LIST(ENTITY_LIST &&other) /*noexcept*/;

	// Move assignment operator

	ENTITY_LIST& operator=(ENTITY_LIST &&other) /*noexcept*/;

/**
 * Clear all entries from the list and reset indexes and counters for reuse.
 */
	void clear();

 /**
  * Adds an entity to the list and returns its index number.
  * <br><br>
  * <b>Role:</b> If the check flag is set to <tt>FALSE</tt>, then the lookup that assures a unique entry is skipped.
  * <br><br>
  * @param entity_name
  * entity name.
  * @param check
  * check unique.
  */
	int add( ENTITY *entity_name, logical check = TRUE );

 /**
  * Adds the entities of an <tt>ENTITY_LIST</tt> to this <tt>ENTITY_LIST</tt>.
  * <br><br>
  * <b>Role:</b> If the check flag is <tt>FALSE</tt>, then the lookup that assures a unique entry is skipped.
  * <br><br>
  * @param entity_list
  * entity list.
  * @param check
  * check unique.
  */
	void add( ENTITY_LIST const &entity_list, logical check = TRUE );

	// Search for an ENTITY in the list, inserting it if not
	// already there and logical argument is TRUE. This will
	// be withdrawn soon in favour of add() and the other
	// lookup().

 /**
  * @nodoc
  */
	int lookup( ENTITY *, logical );


	// Search for an ENTITY in the list.

 /**
  * Lookup the specified ENTITY in the list and return its index.
  * <br><br>
  * @param entry
  * entry to lookup
  */
	int lookup( ENTITY const * ) const;


	// Delete an entity from the list. This does not free space, and
	// leaves a tombstone in the linear list which count() and
	// operator[] will notice, but subsequently lookup() will not
	// find this entity, nor will init()/next(). The return value is
	// the lookup() value of the old entry.

/**
 * Remove the specified entry from the list and return its index.
 * <br><br>
 * <b>Role:</b> Although the entry is removed from the list, a tombstone is left in its place.
 * The count of the list is not reduced, the iteration_count, however, is.
 * <br><br>
 * @param entity_name
 * entry to remove
 */
	int remove( ENTITY const *entity_name );

 /**
  * Removes entities in the given list from the list; however, it does not free space.
  * <br><br>
  * <b>Role:</b> Although the entries are removed from the list, tombstones are left in their place.
  * The count of the list is not reduced, the iteration_count, however, is.
  * <br><br>
  * @param entity_list
  * entity list.
  */
	void remove( ENTITY_LIST const &entity_list );

	// STI let (r3891): added another removal method that is more efficient
	// if the ENTITY's index value is already known.

/**
 * Remove the entry at the specified index from the list and return its index.
 * <br><br>
 * <b>Role:</b> Although the entry is removed from the list, a tombstone is left in its place.
 * The count of the list is not reduced, the iteration_count, however, is.
 * <br><br>
 * @param index
 * index of entry to remove.
 */
	int remove( int index);


	// Count how many entities there are in the list (including
	// deleted entries).

	/**
	 * Returns the number of entries in the list including the deleted ones (tombstones).
	 */
	int count() const;

 /**
  * Returns the number of live entities in the list not including deleted entries.
  */
	int iteration_count() const;

 /**
  * Returns the entity at the given index from the list.
  * <br><br>
  * <b>Role:</b> This method returns NULL if the given index is out of range,
  * or the constant LIST_ENTRY_DELETED if the indexed entity was removed from the list.
  * <br><br>
  * @param index
  * integer.
  */
	ENTITY *operator[]( int index) const;


	// Return entities in list order, ignoring deleted items. Call
	// init() once, then next() repeatedly until it returns NULL.
	// Note that next() returns the undeleted item most closely
	// following the one most recently returned by next() or
	// operator[], except that if that value was NULL the value
	// of next() is undefined.

 /**
  * Initializes the iterator, which is used by the next method, to the beginning of the list.
  */
	void init() const;

 /**
  * Returns the first undeleted (live) entry and updates the iterator correctly for the next method.
  */
	ENTITY *first() const;


 /**
  * Returns the next undeleted (live) entry.
  */
	ENTITY *next() const;

	// Return the next non deleted entry after the index given
	// without affecting the member variables used by init and
	// next.  This allows clients to create iterators that can
	// be multiply instantiated and which run independently of
	// each other. This is accomplished simply by giving the
	// the user the appropriate variables to save them selves.

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
	ENTITY *next_from(int &from_index) const;

	// Reverse the order of an ENTITY_LIST.
	// If the flag is TRUE, it compresses out deleted entries.

 /**
  * Reverses the order of the entity list.
  * <br><br>
  * <b>Role:</b> If the compress flag is <tt>TRUE</tt>, deleted entities are removed.
  * <br><br>
  * @param compress
  *  remove deleted entities.
  */
	void reverse( logical compress = TRUE );

	// STI ROLL
	// Returns the size in bytes of this class. Does not include the
	// the size of the individual ENTITIES themselves.

 /**
  * Gets an array of the entities in the list.
  * <br><br>
  * <b>Role:</b> The default returns the an array that must be freed by using <tt>ACIS_DELETE [] STD_CAST</tt>
  * <br><br>
  * @param entity_array
  * pointer of array to be used if provided.
  * @param array_count
  *  number of entities in array.
  * @param tombstones
  *  add tombstones to array.
  */
	ENTITY **
	array( ENTITY ** entity_array = NULL,
					   int &array_count = *(int *)NULL_REF,
					   logical tombstones = FALSE ) const;


/**
  * Returns the size in bytes of this class.
  * <br><br>
  * <b>Role:</b>  It does not include the size of the individual <tt>ENTITIES</tt> themselves.
  * <br><br>
  * @param countSelf
  * count self or not.
  */
	int byte_count(logical countSelf = TRUE) const;
    // STI ROLL

/**
  * Sorts the list based upon the user-supplied comparison function.
  * <br><br>
  * <b>Role:</b>  Implements the runtime qsort function to sort the list and expects the input compare funcion
  * to have the same behavior as described for qsort. The return value should be less than zero if entity 1
  * is less than entity 2, greater than zero if entity 1 is greater than entity 2, and zero if they are the same.
  * Tombstones are removed bofore sorting and do not have to be considered by the comprare function. 
  * The sorted list is obviously re-indexed, which may occur even when the list does not actually 
  * require sorting changes.
  * <br><br>
  * @param compare_func
  * comparison function
  */
	void sort( int (*compare_func)(const void* ent1, const void* ent2 ) );

};

/**
 * Returns the start iterator of the <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b>  It creates and return the <tt>ENTITY_LIST_ITERATOR</tt> pointing to the begining of <tt>ENTITY_LIST</tt>.
 * <br><br>
 * @param el
 * <tt>ENTITY_LIST</tt> whose iterator is required.
 */
DECL_KERN ENTITY_LIST_ITERATOR begin(ENTITY_LIST const& el);

/**
 * Returns the end iterator of the <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b>  It creates and return the <tt>ENTITY_LIST_ITERATOR</tt> pointing to the end of <tt>ENTITY_LIST</tt>.
 * <br><br>
 * @param el
 * <tt>ENTITY_LIST</tt> whose iterator is required.
 */
DECL_KERN ENTITY_LIST_ITERATOR end(ENTITY_LIST const& el);

/** @} */
#endif
