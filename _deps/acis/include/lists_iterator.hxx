/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( LISTS_ITERATOR_HXX )
#define LISTS_ITERATOR_HXX

#include <iterator>

#include "acis.hxx"
#include "dcl_kern.h"
#include "entity.hxx"

/**
 * @file lists_iterator.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */
 
 class ENTITY_LIST_ITERATOR_IMPL;
 
 /**
  * Implements a forward iterator for traversing the <tt>ENTITY_LIST</tt>.
  * <br>
  * <b>Role:</b> This class provides a custom forward iterator for traversing the ENTITY_LIST using iterator method and also using C++ 
  * ranged based for loop. This also supports some of the STL algorithms which uses forward iterator.
  * <br><br>
  * Call the constructor with the first parameter as the <tt>ENTITY_LIST</tt> for which the iterator is to be constructed and second parameter
  * as boolean argument <tt>true</tt> for start iterator and <tt>false</tt> for end iterator. You can than use ++ operator to traverse the list
  * from start to end.
  * <br><br>
  * @see ENTITY_LIST
  */
 class DECL_KERN ENTITY_LIST_ITERATOR : public ACIS_OBJECT
{	
public:

	/**
	 * C++ constructor, creating an <tt>ENTITY_LIST_ITERATOR</tt>.
	 * @param l
	 * <tt>ENTITY_LIST</tt> whose iterator is required.
	 * @param first
	 * parameter to decide construction of start or end iterator.
	 */
	ENTITY_LIST_ITERATOR(ENTITY_LIST const& l, bool first);

	/**
	 * C++ destructor, deleting an <tt>ENTITY_LIST_ITERATOR</tt>.
	 */
	~ENTITY_LIST_ITERATOR();
	
	/**
	 * C++ copy constructor, which copies the <tt>ENTITY_LIST_ITERATOR</tt> and constructs new one.
	 */
	ENTITY_LIST_ITERATOR(const ENTITY_LIST_ITERATOR&);

	/**
	 * <tt>Assignment</tt> operator for <tt>ENTITY_LIST_ITERATOR</tt>. 
	 */
	ENTITY_LIST_ITERATOR& operator=(const ENTITY_LIST_ITERATOR&);

	/**
	 * <tt>Derefencing</tt> operator returns the element of the <tt>ENTITY_LIST</tt> pointed by the <tt>ENTITY_LIST_ITERATOR</tt>.
	 */
	ENTITY* operator*() const;

	/**
	 * <tt>Increment</tt> operator moves the position of the <tt>ENTITY_LIST_ITERATOR</tt> forward.
	 */
	ENTITY_LIST_ITERATOR const& operator++() const;

	/**
	 * <tt>Relational</tt> operator determines if two <tt>ENTITY_LIST_ITERATOR</tt>s are not equal.
	 * @param other
	 * Another iterator to compare with the current iterator for equality. 
	 */
	bool operator!=(ENTITY_LIST_ITERATOR const& other) const;

	/**
	 * <tt>Relational</tt> operator determines if two <tt>ENTITY_LIST_ITERATOR</tt>s are equal.
	 * @param other
	 * Another iterator to compare with the current iterator for equality.
	 */
	bool operator==(ENTITY_LIST_ITERATOR const& other) const;

private:
	ENTITY_LIST_ITERATOR_IMPL* mIter;
		
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = ENTITY;
	using pointer = ENTITY*;
	using reference = ENTITY&;
};

 
/** @} */
#endif
