/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LIST_HEADER_HXX
#define LIST_HEADER_HXX

#include "base.hxx"
#include "dcl_base.h"
#include "mmgr.hxx"
#include <stdio.h>
/**
 * \defgroup ACISLISTS Lists
 * \ingroup KERNAPI
 *
 */
/**
 * @file list_header.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */
/**
 * @nodoc
 * Pseudopointer returned by operator[] if the indexed list entry has been deleted.
 */
extern DECL_BASE void * const LIST_ENTRY_DELETED;
/**
 * @nodoc
 * This defines the initial size of the m_util array
 */
#define INITIAL_SIZE 16

/**
 * Implements a variable length list of entries of type void*.
 * <br>
 * <b>Role:</b> This is a fundamental list container class used by many ACIS classes, such as ENTITY_LIST and EELIST.
 */
class DECL_BASE LIST_HEADER : public ACIS_OBJECT {
	void * m_util[INITIAL_SIZE];//main data array
	void ** m_list;				//generic list pointer
	
	int m_size;					//available entry positions
	int m_count;				//added entry count
	int m_deleted;				//deleted entry count
	int m_iter;					//current iteration location
	int m_hash;					//last computed hash index
	int m_last;					//last lookup index
	intptr_t m_high, m_low;		//lowest and highest entries
	
public:

	/**
	 * Default constructor - initializes data.
	 */
	LIST_HEADER();
	LIST_HEADER(const char* gme);
	/**
	* Copy constructor, which copies the whole list (complete with deleted entries, if any), 
	* so that the indices in the copy match those in the original.
	* @param source
	* list to copy
    */	
	LIST_HEADER(LIST_HEADER const &source);
	LIST_HEADER(const char* gme, LIST_HEADER const& source);
	/**
	 * Default destructor - frees memory.
	 */
	~LIST_HEADER();
	/**
	 * Assignment operator performs a full copy of the list, complete with tombstones for deleted entries so that the indices are the same.
	 * @param source
	 * source list
	 */
	LIST_HEADER& operator=( LIST_HEADER const & source);
	LIST_HEADER& gme_operator_assign(LIST_HEADER const& source);

	// Move constructor.

	LIST_HEADER(LIST_HEADER &&other) /*noexcept*/;
	LIST_HEADER(const char* gme, LIST_HEADER&& other);

	// Move assignment operator.

	LIST_HEADER& operator=(LIST_HEADER &&other) /*noexcept*/;
	LIST_HEADER& gme_operator_assign(LIST_HEADER&& other);

	/**
	 * Add the entry to the list and return its index.
 	 * <br><br>
	 * <b>Role:</b> A lookup is always performed when adding an entry, in order to assure that it is unique,
	 * unless the check argument is false, in which case the entry is simply added to the end.
	 * <br><br>
	 * @param entry
	 * entry to add
	 * @param check
	 * logical check for duplicates - TRUE by default
	 */
	int add( void * entry, logical check = TRUE );
	int gme_add(void* entry, logical check = TRUE);
	/**
	 * Lookup the specified entry in the list and return its index.
  	 * <br><br>
	 * @param entry
	 * entry to remove
	 */
	int lookup( void const * entry );
	int gme_lookup(void const* entry);
	/**
	 * Remove the specified entry from the list and return its index.
  	 * <br><br>
 	 * <b>Role:</b> Although the entry is removed from the list, a tombstone is left in its place.
	 * The count of the list is not reduced, the iteration_count, however, is.
 	 * <br><br>
	 * @param entry
	 * entry to remove
	 */
	int remove( void const * entry );
	int gme_remove( void const * entry );
	/**
	 * Remove the entry at the specified index from the list and return its index.
  	 * <br><br>
 	 * <b>Role:</b> Although the entry is removed from the list, a tombstone is left in its place.
	 * The count of the list is not reduced, the iteration_count, however, is.
 	 * <br><br>
	 * @param index
	 * index of entry to remove
	 */
	int remove( int index );
	int gme_remove( int index );
	/**
	 * Replace the entry at the specified index with the new entry and return the old entry.
 	 * <br><br>
	 * @param index
	 * index of entry to replace
	 * @param entry
	 * new entry
	 */
	void* replace( int index, void const * entry );
	void* gme_replace( int index, void const * entry );
	/**
	 * Return the number of entries in the list including the deleted ones (tombstones).
	 */
	int count() {				
		return m_count; 
	}	
	/**
	 * Return the number of entries in the list excluding the deleted ones (tombstones).
	 */
	int iteration_count() {		//live count without deleted entries
		return m_count - m_deleted;
	}
	/**
	 * Return the entry at the specified index using the index operator.
  	 * <br><br>
	 * @param index
	 * index of entry to return
	 */
	void *operator[]( int index);
	void *gme_operator_index( int index );
	/**
	 * Initialize the iterator to zero.
	 * <br><br>
	 * <b>Role:</b> The iterator is used to sequence through the list. 
	 * The next method, for example, returns the next live entry relative to the iterator,
	 * and increments the iterator for the next call. Resetting the iterator will cause
	 * the next method to return entries from the beginning of the list.
	 */
	void init();
	void gme_init();
	/**
	 * Return the next live entry relative to the iterator.
	 * <br><br>
	 * <b>Role:</b> The next method is the prefered method to sequence through a list.
	 * It uses the iterator to keep track of the current index.
	 * Use init to reset the iterator and cause next to start at the beginning of the list.
	 */
	void *next();
	void *gme_next();
	/**
	 *	Return the next live entry from the list relative to the supplied custom iterator.
	 * <br><br>
	 * <b>Role:</b> This provides the ability to persist a custom iterator to sequence entries 
	 * relative to an index without affecting the internal iterator. The custom iterator is incremented
	 * during the call to the next live entry.
	 * @param index
	 * index of entry to return
	 */
	void *next_from( int &index );	
	void *gme_next_from( int &index );
	/**
	 * Clear all entries from the list and reset indexes and counters for reuse.
	 */
	void clear();
	void gme_clear();
	/**
	 * Return the memory size, in bytes, of the list.
	 */
	int byte_count();
	int gme_byte_count();
        /**
         * Sorts the list based upon the user-supplied comparison function.
         * <br><br>
         * <b>Role:</b>  Implements the runtime qsort function to sort the list and expects the input compare funcion
         * to have the same behavior as described for qsort. The return value should be less than zero if entry 1
         * is less than entry 2, greater than zero if entity 1 is greater than entity 2, and zero if they are the same.
         * Tombstones are removed bofore sorting and do not have to be considered by the comprare function. 
         * The sorted list is obviously re-indexed, which may occur even when the list does not actually 
         * require sorting changes.
         * <br><br>
         * @param compare_func
         * comparison function
         */
	void sort( int (*compare_func)(const void* entry1, const void* entry2 ) );
	void gme_sort( int (*compare_func)(const void* entry1, const void* entry2 ) );

private:

	int raw_add( void const *, int);		//internal add method
	int gme_raw_add( void const *, int);	

	int raw_lookup( void const *);	//internal lookup method
	int gme_raw_lookup( void const *);
};

/** @} */
#endif

