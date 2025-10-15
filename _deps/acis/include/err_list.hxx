/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ERR_LIST_HEADER
#define ERR_LIST_HEADER

// This class adds to the basic "error_info_base" class the ability to chain
// a list of "error_info_base"s together. To make it fit properly into the
// use counting scheme, being referred to in a "next_ptr" counts as a
// use. Note that decrementing the "use" of the head of a list will
// potentially (if none have any other recorded uses) clean up the
// entire remainder of the list. Any function that returns an
// error_list_info (with a view, perhaps, to the caller passing it to
// sys_error) should arrange the head of the list to have a use of 0
// (all the remaining items down the list will typically have a use of
// 1, being each referred to by just the one previous list item ).

#include "err_info_base.hxx"

/**
 * @file err_list.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

/**
 * Chains a list of <tt>error_info_base</tt> objects together.
 * <br>
 * <b>Role:</b> This class adds chaining capability to the basic <tt>error_info_base</tt> class.
 * To make it fit properly into the use counting scheme, being referred to in a
 * <tt>next_ptr</tt> counts as a use. Note that decrementing the use of the head of a list
 * will potentially clean up the entire remainder of the list (if none have any
 * other recorded uses). Any function that returns an <tt>error_list_info</tt> (with a view,
 * perhaps, to the caller passing it to <tt>sys_error</tt>) should arrange the head of the
 * list to have a use of 0. All the remaining items down the list will typically
 * have a use of 1, being each referred to by just the one previous list item.
 * @see error_info_base
 */
class DECL_BASE error_list_info : public error_info_base {
	error_list_info *next_ptr;	// being referenced here counts as a "use"

public:

/**
 * Default constructor.
 * <br><br>
 * <b>Role:</b>	Constructs an <tt>error_list_info</tt> object and sets the <tt>next_ptr</tt> to <tt>NULL</tt>.
 */
	error_list_info();
	error_list_info(char const * gme);

/**
 * Default destructor.
 * <br><br>
 * <b>Role:</b> Destructs an <tt>error_list_info</tt> object and removes a "use" from its <tt>next_ptr</tt>.
 */
	virtual ~error_list_info();

/**
 * Returns the <tt>next_ptr</tt>.
 */
	error_list_info *next() const { return next_ptr; }

/**
 * Sets <tt>next_ptr</tt> and juggles the use counts.
 * <br><br>
 * @param list
 * given list pointer.
 */
	void set_next( error_list_info * list);
	void gme_set_next( error_list_info * list);

};

/**
 * Searches the given <tt>error_list_info</tt> list for the first entry of the given type (or <tt>NULL</tt> if none).
 * <br><br>
 * @param list
 * given list pointer.
 * @param typeno
 * type specifier.
 * @see error_info_base
 */
DECL_BASE error_list_info *find_error_list_info(
										error_list_info *list,
										int typeno
										);

DECL_BASE error_list_info *gme_find_error_list_info(
										error_list_info *list,
										int typeno
										);

/**
 * Concatenates two lists.
 * <br><br>
 * <b>Role:</b> This function appends the second list to the end of the first list 
 * and returns a pointer to the head of the combined list.
 * In general, this function will traverse to the end of the first list; 
 * therefore, if used repetitively, the first list should be reasonably short.
 * @param list1
 * first list pointer.
 * @param list2
 * second list pointer.
 * @see error_info_base
 */
DECL_BASE error_list_info *append_error_list_info(
										  error_list_info *list1,
										  error_list_info *list2
										  );

DECL_BASE error_list_info *gme_append_error_list_info(
										  error_list_info *list1,
										  error_list_info *list2
										  );

/** @} */
#endif
