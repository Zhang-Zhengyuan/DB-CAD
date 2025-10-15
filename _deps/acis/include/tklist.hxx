/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// Purpose
//		reflist provides a base class that can be used to manage a
//		list of items with reference counts.
//----------------------------------------------------------------------

#ifndef _TKLIST_HXX_
#define _TKLIST_HXX_

#include "acis.hxx"
#include "dcl_kern.h"
/**
 * @file tklist.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN tk_list_object : public ACIS_OBJECT
{
protected:
	tk_list_object *m_next;
	tk_list_object *m_prev;

	friend class tk_list;

public:
	tk_list_object() : m_next(0), m_prev(0) {}
};

/**
 * Base class that can be used to manage a list of items.
 */
class DECL_KERN tk_list  : public ACIS_OBJECT
{
protected:

	tk_list_object *m_head;		// head of list
	tk_list_object *m_tail;		// tail of list
	tk_list_object *m_cur;		// current SPAposition in the list

	int 			  m_count;		// number of items in the list

public:
	// constructor
	/**
	 * Default constructor.
	 */
	tk_list() : m_head(0), m_tail(0), m_cur(0), m_count(0) {}

	// destructor.  Destroying the list will not delete the items in the list.
	/**
	 * Deletes this object when it goes out of scope.
	 * <br><br>
	 * <b>Role:</b> Deletes this object when it goes out of scope.
     * However deleting the object would not delete the items in the list.
	 */
	~tk_list() {}

	//---------------------------------------------------------------------
	// queries into the list
	//---------------------------------------------------------------------
	/**
	 * Returns number of objects in the list.
	 */
	int count() { return m_count; }

	//---------------------------------------------------------------------
	// save and restore current SPAposition
	//---------------------------------------------------------------------
	/**
	 * Returns the current object from the list.
	 */
	tk_list_object *get_current() { return m_cur; }
	/**
	 * Sets the current object to the object referenced by the pointer.
	 * <br><br>
	 * @param obj
	 * object.
     */
	void set_current( tk_list_object *obj) { m_cur = obj; }

	//---------------------------------------------------------------------
	// ways of adding things to the list
	//---------------------------------------------------------------------

	// add something at the head of the list
	/**
	 * Adds the object to the head of the list.
	 * <br><br>
	 * @param obj
	 * object.
	 */
	void add_head( tk_list_object *obj);

	// add something at the tail of the the list
	/**
	 * Adds the object to the end of the list.
	 * <br><br>
	 * @param obj
	 * object.
	 */
	void add_tail( tk_list_object *obj);

	// add after the current SPAposition
	/**
	 * Adds the object after the current position.
	 * <br><br>
	 * @param obj
	 * object.
	 */
	void add_after( tk_list_object *obj);

	// add before the current SPAposition
	/**
	 * Adds the object before the current position
	 * <br><br>
	 * @param obj
	 * object.
	 */
	void add_before( tk_list_object *obj);

	// push object on tail of list (use list as a stack)
	/**
	 * Pushes an object to the end of the list.
	 * <br><br>
	 * @param obj
	 * object.
	 */
	void push( tk_list_object *obj) { add_tail( obj); }

	//---------------------------------------------------------------------
	// ways of deleting things from the list.  These remove things from
	// the list but do not actually delete the object data.
	//---------------------------------------------------------------------

	// delete something from the head of the list and return the pointer to
	// the deleted thing.
	/**
	 * Deletes an object from the head of the list and returns a pointer to the deleted object.
	 */
	tk_list_object *del_head();

	// delete something from the end of the list
	/**
	 * Deletes an object from the end of the list and returns a pointer to the deleted object.
	 */
	tk_list_object *del_tail();

	// delete a specific object referenced by it's pointer
	/**
	 * Deletes a specific object from the list referenced by the pointer.
	 * <br><br>
	 * @param obj
	 * object.
	 */
	tk_list_object *del_object( tk_list_object *obj);

	// delete current object
	/**
	 * Deletes the current object from the list.
	 */
	tk_list_object *del();

	// pop object off end of list (use list as a stack)
	/**
	 * Pops an object from the end of the list.
	 */
	tk_list_object *pop() { return del_tail(); }

	//---------------------------------------------------------------------
	// Search mechanisms
	//---------------------------------------------------------------------

	// move to beginning of list
	/**
	 * Sets the current object to the first object in the list.
	 */
	tk_list_object *head()
	{
		m_cur = m_head;
		return m_cur;
	}

	// move to end of list
	/**
	 * Sets the current object to the last object in the list.
	 */
	tk_list_object *tail()
	{
		m_cur = m_tail;
		return m_cur;
	}

	// move to next item in the list
	/**
	 * Moves the current object to the next item in the list.
	 */
	tk_list_object *next()
	{
		if( m_cur)
			m_cur = m_cur->m_next;
		return m_cur;
	}

	// move to prev item in the list
	/**
	 * Moves the current object to the previous item in the list.
	 */
	tk_list_object *prev()
	{
		if( m_cur)
			m_cur = m_cur->m_prev;
		return m_cur;
	}
};

// reference_object is the class of items kept track of by reference_list
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN reference_list_object : public tk_list_object
{
private:

	int m_referenceCount;			// count of the references

public:

	reference_list_object() : tk_list_object(), m_referenceCount(0) {}

	int get_references()
	{
		return m_referenceCount;
	}

	int inc_references()
	{
		return ++m_referenceCount;
	}

	int dec_references()
	{
		if( m_referenceCount)
			--m_referenceCount;
		return m_referenceCount;
	}

	int add_references( int addVal )
	{
		return ( m_referenceCount += addVal);
	}

	int sub_references( int subVal)
	{
		m_referenceCount -= subVal;
		if( m_referenceCount < 0)
			m_referenceCount = 0;
		return m_referenceCount;
	}
};

// reference list class

/*
// tbrv
*/
/**
 * @nodoc
 */

class DECL_KERN reference_list : public tk_list
{
public:
	reference_list() : tk_list() {}

	// delete any objects that have 0 reference counts.  This will actually
	// execute the delete operator for these objects.
	void del_unreferenced_objects();
};

/** @} */
#endif

