/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_LIST_OPTIONS_HXX
#define ASM_LIST_OPTIONS_HXX

#include "base.hxx"
#include "dcl_asm.h"

/**
* @file asm_list_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * Enumeration to indicate whether a list of pointers to use-counted objects holds those objects by incrementing their use counts.
 * <br>
 * <b>Role:</b> <tt>asm_hold_state</tt> is an enumerated type used by <tt>asm_list_options</tt> to specify 
 * whether the list should hold a use count for its entries.
 * <br><br>
 * The class objects asm_model_list, entity_handle_list, component_handle_list, and component_entity_list have the
 * ability to "hold" their entries, i.e. to increment the use counts of their entries.  Whether or not a list
 * is holding its entries is indicated by its hold state.  If a list is holding its entries, their use count 
 * is incremented by one.
 * <br>
 * @param ASM_NO_HOLD
 * indicates that the list does not hold its entries.
 * @param ASM_HOLD
 * indicates that the list holds its entries.
 * <br><br>
 * @see asm_model_list, entity_handle_list, component_handle_list, component_entity_handle_list, asm_list_options
 **/

enum asm_hold_state
{
	ASM_NO_HOLD,
	ASM_HOLD
};


/**
 * Class object to encapsulate requests for optional behaviors of assembly modeling list objects.
 * <br>
 * <b>Role:</b> <tt>asm_list_options</tt> is used to request the optional "holding" and "entry counting" 
 * behaviors for an assembly modeling list object.  A list which is holding its entries will increment their
 * use count.  A list which is counting its entries will maintain an integer cardinality for each entry in 
 * the list which is adjusted by the "add" and "remove" methods.  An entry will only be removed from the list
 * when its cardinality goes to zero.
 * <br><br>
 * @see asm_model_list, entity_handle_list, component_handle_list, component_entity_handle_list, asm_hold_state
 */

class DECL_ASM asm_list_options : public ACIS_OBJECT
{
	logical entry_count_flg;
	asm_hold_state hold_state;
public:

/**
 * Default constructor - counting and holding are OFF by default
 */
	asm_list_options();

	logical get_count_flag() const;
	void set_count_flag(logical value);

	asm_hold_state get_hold_state() const;
	void set_hold_state(asm_hold_state value);
};


 /** @} */
#endif // ASM_LIST_OPTIONS_HXX

