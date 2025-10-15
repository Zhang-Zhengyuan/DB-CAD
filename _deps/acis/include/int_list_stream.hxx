/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef INT_LIST_STREAM_HXX
#define INT_LIST_STREAM_HXX

#include "dcl_base.h"
#include "list_stream.hxx"
#include "logical.h"

/**
 * @file int_list_stream.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *
 *
 */
/**
 * \addtogroup ACISLISTS
 *
 * @{
 */
/**
* Container class derived from list_stream that stores integers.
* <br>
* <b>Role:</b> This is a container for integers that is based
* on the list_stream capabilities. This class inherits all the base
* list_stream functionality, such as adding, removing, and iterating the
* elements of the list. See the list_stream documentation for more details.
**/

class DECL_BASE integer_list_stream : public list_stream<int>
{
	integer_list_stream& operator=( const integer_list_stream&);

public:

	/**
	* Default constructor initializes the list_stream object.
	* <br><br>
	* @param opts
	* list_stream_options object
	**/	
	integer_list_stream( list_stream_options* opts = NULL);

	/**
	* Find the index of a particular value in the list.
	* <br>
	* <b>Role:</b> Use this method to find the location (array index)
	* of the first occurance of a specific value in a list.
	* The returned value is TRUE if the value is found and the 
	* index variable passed by reference will contain the index
	* of the found value in the list.
	**/
	logical find_first( int val, int& index );
};


/**
* Container class with sorting capabiliteis derived from integer_list_stream.
* <br>
* <b>Role:</b> Use this class to maintain sorted list values, which
* often allow for more efficient processing. This class extends the
* functionality inherited from integer_list_stream with efficient sorting
* routines.
**/
class DECL_BASE sortable_integer_ls : public integer_list_stream
{
	logical _is_sorted;

	// strictly speaking remove() can change the state from having
	// duplicates to not having duplicates, but we don't want to
	// slow down remove by checking; if the list is already sorted
	// but no longer has duplicates, we only lose that sort()
	// has to make one unnecessary pass to remove duplicates - this
	// is presumably a rare case
	logical _no_duplicates;

	int _lower_bound;
	int _upper_bound;

	int _last_value_added;

private:

	sortable_integer_ls& operator=( const sortable_integer_ls&);

	// for internal testing purposes only
	friend void sortable_integer_ls__remove_duplicates_test( char* );
	friend void sortable_integer_ls__sort_test( char* );
	friend DECL_BASE void sortable_integer_ls_specific_sort(sortable_integer_ls& sils, const char* sort_type);

	void remove_duplicates();

	void insertion_sort_subset( int start_index, int end_index, logical allow_duplicates );
	void insertion_sort( logical allow_duplicates );

	void quick_sort_partition( int left_index, int right_index );
	void quick_sort( logical allow_duplicates );

	void count_sort();

	logical swap( sortable_integer_ls& sils );

	friend logical save_sortable_index_list(void*);

public:
	/**
	* Default constructor initializes the list_stream object.
	* <br><br>
	* @param opts
	* list_stream_options object
	**/	
	sortable_integer_ls(list_stream_options* opts = NULL);
	/**
	* Copy constructor.
	**/
	sortable_integer_ls( const sortable_integer_ls& );
	/**
	* Add a value to the end of the list.
	* @param val
	* value to add
	**/
	int add( int val );
	/**
	* Remove the value at the specified array index from the list leaving a tombstone.
	* @param index
	* index of element to mark as removed
	**/	
	logical remove( int index );
	/**
	* Replace the element at the specified array index with the input value.
	* @param index
	* array index
	* @param val
	* new value
	**/
	logical modify( int index, int& val );
	/**
	* Find the index of a particular value in the list.
	* <br>
	* <b>Role:</b> Use this method to find the location (array index)
	* of the first occurance of a specific value in a list.
	* The returned value is TRUE if the value is found and the 
	* index variable passed by reference will contain the index
	* of the found value in the list.
	**/
	logical find_first( int val, int& index );
	/**
	* Sort the values in the list.
	* @param allow_duplicates
	* Flag to control whether duplicates should be removed.
	**/
	void sort( logical allow_duplicates = FALSE );
	/**
	* Returns TRUE if the list is sorted.
	**/
	logical is_sorted() const { return _is_sorted; }
};

/** @} */

#endif //INT_LIST_STREAM_HXX
 
