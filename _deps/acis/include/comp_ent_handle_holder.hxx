/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COMPONENT_ENTITY_HANDLE_HOLDER_HXX
#define COMPONENT_ENTITY_HANDLE_HOLDER_HXX

#include "base.hxx"
#include "dcl_asm.h"

/**
* @file comp_ent_handle_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */
class component_entity_handle;


/**
 * Holds a pointer to a <tt>component_entity_handle</tt>.
 * <br>
 * <b>Role:</b> The component_entity_handle_holder object is a container for an component_entity_handle object which automates
 * use counting for the component_entity_handle object.  Clients of component_entity_handle objects should use this holder
 * object to ensure that the component_entity_handle object is not deleted when cleanup is performed. Cleanup is performed
 * by cleanup asmi routines (asmi_cleanup, asmi_model_cleanup, asmi_model_cleanup_tree, asmi_cleanup_handles, asmi_model_cleanup_handles), 
 * which examine all component_entity_handle objects which are managed 
 * (owned) by a set of asm_model objects and destroy any object with a use count of zero, i.e. which are not being held by an 
 * component_entity_handle_holder object or by an component_entity_handle_list object with the ASM_HOLD behavior set.
 * <br><br>
 * @see component_entity_handle, asm_model
 */

class DECL_ASM component_entity_handle_holder : public ACIS_OBJECT
{
	component_entity_handle* hdl_ptr;

	// special routines to handle restore, don't adjust use count when dealing with sequence numbers
	friend class component_entity_handle;		// component entity handle is special because it is private member of asm_model which contains private component_handle_holder
	friend DECL_ASM void read_comp_ent_hdl_hldr( component_entity_handle_holder& );
	void set_sequence_num( component_entity_handle* seq_num );
	friend DECL_ASM void fixup_comp_ent_hdl_hldr( component_entity_handle_holder& );
	void fixup_ptr( component_entity_handle* hdl );

public:
/**
 * Default constructor.
 * <br>
 * <b>Role:</b> Sets the <tt>component_entity_handle</tt> pointer to NULL.
 * <br><br>
 */
	component_entity_handle_holder();

/**
 * Constructor.
 * <br>
 * <b>Role:</b> Sets the <tt>component_entity_handle</tt> pointer.  If this pointer is not NULL, the use count is incremented.
 * <br><br>
 * @param hdl
 * pointer to component_entity_handle to hold.
 */
	component_entity_handle_holder( component_entity_handle* hdl );

/**
 * Destructor.
 * <br>
 * <b>Role:</b> Decrements the use count of the <tt>component_entity</tt>_handle object.
 * <br><br>
 */
	~component_entity_handle_holder();

/**
 * Assignment operator.
 * <br>
 * <b>Role:</b>  Copies the <tt>component_entity_handle</tt> pointer.  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid <tt>component_entity_handle</tt> pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl_hldr
 * component_entity_handle_holder.
 */
	component_entity_handle_holder& operator=( const component_entity_handle_holder& hdl_hldr );

/**
 * Sets the <tt>component_entity_handle</tt> pointer.
 * <br>
 * <b>Role:</b>  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid <tt>component_entity_handle pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl
 * pointer to component_entity_handle to hold.
 */
	void set( component_entity_handle* hdl );

/**
 * Returns the <tt>component_entity_handle</tt> pointer.
 * <br><br>
 */
	component_entity_handle* get() const;

/**
 * Releases the <tt>component_entity_handle</tt> pointer.
 * <br>
 * <b>Role:</b> Decrements the use count of the component_entity_handle object and sets the component_entity_handle pointer to NULL.
 * <br><br>
 */
	void release();
};

 /** @} */
#endif // COMPONENT_ENTITY_HANDLE_HOLDER_HXX

