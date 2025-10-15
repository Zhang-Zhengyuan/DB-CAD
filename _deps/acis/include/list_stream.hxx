/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LIST_STREAM_HXX
#define LIST_STREAM_HXX

#include <stdio.h>
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "list_stream_base.hxx"

/**
 * @file list_stream.hxx
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
 * Use this class to customize the behaviour of list_stream objects.
 * <br>
 * <b>Role:</b> The main purpose of this class is to allow custom configuration
 * of the amount of physical memory used by list_stream objects before 
 * transistioning to disk based storage via temporary files.
 * Use of custom raw data files is also made possible through this
 * configuration object by specifying a file handle and the number of elements
 * stored in the file. This creates a read-only list stream that is restricted
 * to accessing the elements of the specified file.
 * <br><br>
 * @see list_stream
*/
class DECL_BASE list_stream_options : public ACIS_OBJECT
{
	int _init_max_memory;
	int _element_count;
	FILE* _list_stream_file;

public:
	/**
	* Default consturctor initializes data members to default values.
	**/
	list_stream_options();
	/**
	* Sets the maximum amount of physical memory to use.
	* <br>
	* <b>Note:</b> Memory requirements of the list that exceed the specified amount
	* are automatically serviced with via temporary fils on disk.
	* <br><br>
	* @param init_max_memory
	* amount in megabytes
	**/
	void set_init_max_memory(int init_max_memory);
	/**
	* Returns the maximum amount of physical memory that may be used.
	**/
	int get_init_max_memory() const { return _init_max_memory; }
	/**
	* Sets the number of elements in the raw data file.
	* <br><br>
	* @param element_count
	* number of elements in file
	**/	
	void set_element_count(int element_count);
	/**
	* Returns the number of elements specified to be in the raw data file.
	**/
	int get_element_count() const { return _element_count; }
	/**
	* Sets the handle to the raw data file.
	* <br><br>
	* @param list_stream_file
	* file handle
	**/	
	void set_list_stream_file(FILE* list_stream_file);
	/**
	* Returns the handle of the specified raw data file.
	**/
	FILE* get_list_stream_file() { return _list_stream_file; }
};

/**
 * Use this array based container class to manage large data sets.
 * <br>
 * <b>Role:</b> The main feature of this container class is that it utilizes
 * both physical memory and temporary disk files to store elements while
 * retaining array based behavior. The number of elements the list can hold
 * is limited only by available disk capacity. The location of elements, 
 * whether in memory or on disk, is transparent to operations on the list.
 * This class is a template base class that provides support for custom list elements.
 * <br><br>
 * @see list_stream_options
*/
template <class T>
class list_stream : public list_stream_base
{
private:

	list_stream& operator=( const list_stream&);

protected:

	logical make_tombstone_list( int size ) { return list_stream_base::make_tombstone_list(size); }
	logical swap( list_stream& ls ) { return list_stream_base::swap(ls); }
	int max_memory_elements() { return list_stream_base::max_memory_elements(); }

public:
	/**
	* Default constructor initializes the list_stream object.
	* <br><br>
	* @param opts
	* list_stream_options object
	**/	
	list_stream( list_stream_options* opts = NULL)
		: list_stream_base( sizeof(T), opts) {}
	/**
	* Copy constructor.
	**/
	list_stream( const list_stream& ls )
		: list_stream_base(ls) {}
	/**
	* Add an element to the end of the list.
	* @param element
	* element to add
	**/
	int add(T const& element) { return list_stream_base::add((void*)&element); }
	/**
	* Remove the element at the specified array index from the list leaving a tombstone.
	* @param index
	* index of element to mark as removed
	**/
	logical remove(int index) { return list_stream_base::remove(index); }
	/**
	* Replace the element at the specified array index with the input element.
	* @param index
	* element index
	* @param element
	* new element
	**/
	logical modify(int index, T const& element) { return list_stream_base::modify(index,(void*)&element); }
	/**
	* Retrieve the element at the specified array index.
	* @param index
	* element index
	* @param element
	* output element passed by reference
	**/
	logical get(int index, T& element) { return list_stream_base::get(index,(void*)&element); }
	/**
	* Retrieve the next live element after the iterator index value.
	* <b>Role:</b> Use an iterator to access the elements of the list efficiently.
	* The iterator is incremented until the next live element is encountered.
	* The iteraror value after the call is the index of the element.
	* Begin whith an iterator value of -1 to access the first live element of a list.
	* <br><br>
	* @param iterator index
	* input iterator passed by reference
	* @param element
	* output element passed by reference
	**/	
	logical next_from(int& iter, T& element) { return list_stream_base::next_from(iter,(void*)&element); }
	/**
	* Retrieve the first live element found before the iterator index value.
	* <b>Role:</b> Use an iterator to access the elements of the list efficiently.
	* The iterator is decremented until the next live element is encountered.
	* The iteraror value after the call is the index of the element.
	* <br><br>
	* @param iterator index
	* input iterator passed by reference
	* @param element
	* output element passed by reference
	**/
	logical previous_from(int& iter, T& element) { return list_stream_base::previous_from(iter,(void*)&element); }
	/**
	* Returns the element at the specified array index by value.
	* <b>Note:</b> Use with caution as the returned element is unitialized when tombstones are encountered.
	* Use last_index_valid to determine if the returned element is valid.
	* <br><br>
	* @param index
	* element index
	**/
	T operator[](int index) { return *((T*)list_stream_base::operator[](index)); }
	/**
	* Returns TRUE if the last indexed element is valid, False if a tombstone was encountered.
	**/
	logical last_index_valid() const { return list_stream_base::last_index_valid(); }
	/**
	* Returns the number of elements in the list including tombstones.
	**/
	int count() const { return list_stream_base::count(); }
	/**
	* Returns the number of live elements in the list excluding tombstones.
	**/
	int iteration_count() const { return list_stream_base::iteration_count(); }
	/**
	* Removes tombstones from the list if possible.
	* <b>Note:</b> Removing tombstonse changes the indices of list elements.
	* Tombstones can therefore only be removed if the list is singley referenced.
	* Use with caution.
	* Returns TRUE if the list was compacted.	
	**/
	logical compact() { return list_stream_base::compact(); }
	/**
	* Add a reference count to the list.
	**/
	void add_ref() { list_stream_base::add_ref(); }
	/**
	* Remove a reference count and delete the list if the use count goes to zero.
	* <b>Note:</b> If keep is TRUE, then the object is not deleted.  
	* This is useful to allow compaction.
	* <br><br>
	* @param keep
	* keep when use count goes to zero
	**/
	void remove_ref( logical keep = FALSE ) { list_stream_base::remove_ref(keep); }
	/**
	* Returns the current reference count of the list.
	**/
	int ref_count() const { return list_stream_base::ref_count(); }
};
/** @} */

#endif //LIST_STREAM_HXX

