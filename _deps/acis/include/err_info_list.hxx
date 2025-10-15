/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( ERROR_INFO_LIST_HXX )
#define ERROR_INFO_LIST_HXX

#include "base.hxx"
#include "dcl_base.h"
#include "vlists.hxx"
#include "err_info.hxx"
#include "err_info_base_list.hxx"

/**
* @file err_info_list.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

/**
 * Acts as a collection class for <tt>error_info</tt> objects, that is, it implements a variable length list 
 * of <tt>error_info</tt> objects.
 * <br>
 * <b>Role:</b> The class provides a list mechanism for storing a variable length list of <tt>error_info</tt>
 * objects. By default, the constructor of <tt>error_info_list</tt> makes an empty list with no <tt>error_info</tt> 
 * objects in it. The class provides the interface that is necessary to add or remove an <tt>error_info</tt> object
 * and to traverse through the list to get the <tt>error_info</tt> objects stored in it.
 * <br><br>
 * The correct way of accessing the stored <tt>error_info</tt> objects is to call <tt>error_info_list::init</tt>
 * that rewinds the list and then call <tt>error_info_list::next</tt> to iterate
 * through the list till <tt>error_info_list::next</tt> returns <tt>NULL</tt>.
 * <br><br>
 * This class takes the ownership of all the <tt>error_info</tt> objects that are added to it. So any call
 * to <tt>error_info_list::add</tt> would increment the use count of <tt>error_info</tt> object that is added 
 * given that the <tt>error_info</tt> object that is being added does not exist in the list already. 
 * Similarly, when an <tt>error_info</tt> object that is stored by the <tt>error_info_list</tt> is removed from 
 * the list, then <tt>error_info_list</tt> would decrement the use count of the <tt>error_info</tt> object 
 * that is being removed. Since the <tt>error_info_list</tt> owns all the <tt>error_info</tt> objects 
 * that it stores, it decrements the use count of all the contained <tt>error_info</tt> objects 
 * when the <tt>error_info_list::clear</tt> is called or the <tt>error_info_list</tt> is destructed.
 * In general, customers need not worry about the reference counting of <tt>error_info</tt> stored by
 * <tt>error_info_list</tt>, unless they want to explicitly call <tt>add</tt> on the <tt>error_info</tt> object.
 * 
 * @see error_info
 */
class DECL_KERN error_info_list : public readonly_error_info_base_list
{
public:

	/**
	 * Returns a pointer to the first undeleted <tt>error_info</tt> object and 
	 * updates the iterator correctly for the <tt>next</tt> method.
	 */
	const error_info* first() const;
	const error_info* gme_first() const;
	
	/**
	 * Returns a pointer to an <tt>error_info</tt> object in list order.
	 * <br><br>
	 * <b>Role</b>: It returns the <tt>error_info</tt> most closely following the one that returned
	 * by the last call to <tt>next</tt>, except that if the last returned value
	 * was <tt>NULL</tt>, then the value of <tt>next</tt> is undefined. In case <tt>next</tt> is called 
	 * after <tt>init</tt>, then it returns the first entry in the list. If the list
	 * is empty, it returns <tt>NULL</tt>.
	 * <br><br>
	 * <b>Usage</b>: Call <tt>init</tt> once, then <tt>next</tt> repeatedly until it returns <tt>NULL</tt>.
	 */
	const error_info* next() const;
	const error_info* gme_next() const;

	/**
	 * Returns a count of how many <tt>error_info</tt> objects are pointed to by the list.
	 */
	int count() const;
	int gme_count() const;

	/**
	 * Adds an <tt>error_info</tt> object to the list if not already there.
	 * <br><br>
	 * @param ei
	 * <tt>error_info</tt> to add
	 */
	void add(const error_info* ei);
	void gme_add(const error_info* ei);

	/**
	 * Adds an <tt>error_info_list</tt> by adding all the individual <tt>error_info</tt> objects that are
	 * contained in the incoming <tt>error_info_list</tt>.
	 * <br><br>
	 * @param ei
	 * <tt>error_info_list</tt> to add
	 */
	void add(const error_info_list& ei);
	void gme_add(const error_info_list& ei);

};


/**
 * This function will add to <tt>filtered_eil</tt> only those
 * <tt>error_info</tt> objects from the given <tt>raw_eil</tt> that are of the given type.
 * <br><br>
 * @param raw_eil
 * input <tt>error_info_list</tt>.
 * @param err_num
 * the desired <tt>err_mess_type</tt>. The <tt>error_info</tt> objects of this type will be put into <tt>filtered_eil</tt>.
 * @param filtered_eil
 * <tt>error_info_list</tt> to which the function will add <tt>error_info</tt> objects from the 
 * incoming <tt>error_info_list</tt> that are of the given <tt>err_mess_type</tt>.
 */
 
DECL_KERN void err_mess_type_filtered_list(const error_info_list& raw_eil, 
										   err_mess_type err_num,
										   error_info_list& filtered_eil);

/** @} */
#endif
