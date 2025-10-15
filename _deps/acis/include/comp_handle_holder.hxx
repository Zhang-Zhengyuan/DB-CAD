/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COMPONENT_HANDLE_HOLDER_HXX
#define COMPONENT_HANDLE_HOLDER_HXX

#include "base.hxx"
#include "dcl_asm.h"

/**
* @file comp_handle_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */
class component_handle;


/**
 * Holds a pointer to an <tt>component_handle</tt>.
 * <br>
 * <b>Role:</b> The component_handle_holder object is a container for a component_handle object which automates
 * use counting for the component_handle object.  Clients of component_handle objects should use this holder
 * object to ensure that the component_handle object is not deleted when cleanup is performed. Cleanup is performed
 * by cleanup asmi routines (asmi_cleanup, asmi_model_cleanup, asmi_model_cleanup_tree, asmi_cleanup_handles, asmi_model_cleanup_handles), 
 * which examine all component_handle objects which are managed 
 * (owned) by a set of asm_model objects and destroy any object with a use count of zero, i.e. which are not being held by an 
 * component_handle_holder object or by an component_handle_list object with the ASM_HOLD behavior set.
 * <br><br>
 * @see component_handle, asm_model
 */

class DECL_ASM component_handle_holder : public ACIS_OBJECT
{
	component_handle* hdl_ptr;

	// special routines to handle restore, don't adjust use count when dealing with sequence numbers
	friend class component_handle;		// component handle is special because it is private member of asm_model which contains private component_handle_holder
	friend class component_entity_handle;		// component entity handle is special because it is private member of asm_model which contains private component_handle_holder
	friend DECL_ASM void read_comp_hdl_hldr( component_handle_holder& );
	void set_sequence_num( component_handle* seq_num );
	friend DECL_ASM void fixup_comp_hdl_hldr( component_handle_holder& );
	void fixup_ptr( component_handle* hdl );

public:
/**
 * Default constructor.
 * <br>
 * <b>Role:</b> Sets the component_handle pointer to NULL.
 * <br><br>
 */
	component_handle_holder();

/**
 * Constructor.
 * <br>
 * <b>Role:</b> Sets the component_handle pointer.  If this pointer is not NULL, the use count is incremented.
 * <br><br>
 * @param hdl
 * pointer to component_handle to hold.
 */
	component_handle_holder( component_handle* hdl );

/**
 * Destructor.
 * <br>
 * <b>Role:</b> Decrements the use count of the component_handle object.
 * <br><br>
 */
	~component_handle_holder();

/**
 * Assignment operator.
 * <br>
 * <b>Role:</b>  Copies the component_handle pointer.  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid component_handle pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl_hldr
 * component_handle_holder.
 */
	component_handle_holder& operator=( const component_handle_holder& hdl_hldr );

/**
 * Sets the component_handle pointer.
 * <br>
 * <b>Role:</b>  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid component_handle pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl
 * pointer to component_handle to hold.
 */
	void set( component_handle* hdl );

/**
 * Returns the component_handle pointer.
 * <br><br>
 */
	component_handle* get() const;

/**
 * Releases the component_handle pointer.
 * <br>
 * <b>Role:</b> Decrements the use count of the component_handle object and sets the component_handle pointer to NULL.
 * <br><br>
 */
	void release();
};

 /** @} */
#endif // COMPONENT_HANDLE_HOLDER_HXX

