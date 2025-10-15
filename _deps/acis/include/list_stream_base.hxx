/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LIST_STREAM_BASE_HXX
#define LIST_STREAM_BASE_HXX

#include <stdio.h>
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"

/**
* @nodoc
**/
class SPA_TEMP_FILE;

/**
* @nodoc
**/
class list_stream_options;

/**
* @nodoc
**/
class DECL_BASE list_stream_base : public ACIS_OBJECT 
{
	// File support
	SPA_TEMP_FILE* m_fptr;

	// Memory support
	void* m_mptr; 
	int m_current_size;
	
	int m_break;

	char* m_index_buffer;
	logical m_index_valid;
	
	logical read( int index, void* buffer);
	logical write( const void *buffer, int index = -1);

	int m_element_size; //size of the elements in the stream

	int m_count; //number of elements in the stream
	int m_iteration_count; //Currently live elements

	int m_use_count;

private:

	list_stream_base();
	list_stream_base& operator=( const list_stream_base&);

protected:

	logical make_tombstone_list( int size );
	logical swap( list_stream_base& ls );
	int max_memory_elements() { return m_break; }

public:
	/**
	* @nodoc
	**/
	list_stream_base(int element_size, list_stream_options* opts = NULL);
	/**
	* @nodoc
	**/
	list_stream_base( const list_stream_base& );
	/**
	* @nodoc
	**/
	virtual ~list_stream_base();
	/**
	* @nodoc
	**/
	int add(void* element);
	/**
	* @nodoc
	**/
	logical remove(int index);
	/**
	* @nodoc
	**/
	logical modify(int index, void* p);
	/**
	* @nodoc
	**/
	logical get(int index, void* p);	
	/**
	* @nodoc
	**/
	logical next_from(int& iter, void* p);
	/**
	* @nodoc
	**/
	logical previous_from(int& iter, void* p);
	/**
	* @nodoc
	**/
	void* operator[](int index);
	/**
	* @nodoc
	**/
	logical last_index_valid() const { return m_index_valid; }
	/**
	* @nodoc
	**/
	int count() const { return m_count; }
	/**
	* @nodoc
	**/
	int iteration_count() const { return m_iteration_count; }

	// only compact if the use count is zero so we don't screw up
	// references that are holding on to specific indices in the
	// list stream.
	/**
	* @nodoc
	**/
	logical compact();

	// increment the use count
	/**
	* @nodoc
	**/
	void add_ref();

	// decrement the use count and delete if the use count goes to zero
	// If keep is TRUE, then the object is not deleted.  This is useful
	// to allow compaction only when there are no references which are holding
	// on to specific indices which would go out of date.
	/**
	* @nodoc
	**/
	void remove_ref( logical keep = FALSE );
	/**
	* @nodoc
	**/
	int ref_count() const { return m_use_count; }
};

#endif //LIST_STREAM_BASE_HXX

