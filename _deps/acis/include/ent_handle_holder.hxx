/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ENTITY_HANDLE_HOLDER_HXX
#define ENTITY_HANDLE_HOLDER_HXX

#include "base.hxx"
#include "dcl_asm.h"

/**
* @file ent_handle_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */
class entity_handle;
class entity_handle_list;


/**
 * Holds a pointer to an entity_handle.
 * <br>
 * <b>Role:</b> The entity_handle_holder object is a container for an entity_handle object which automates
 * use counting for the entity_handle object.  Clients of entity_handle objects should use this holder
 * object to ensure that the entity_handle object is not deleted when cleanup is performed.  Cleanup is performed
 * by cleanup asmi routines (asmi_cleanup, asmi_model_cleanup, asmi_model_cleanup_tree, asmi_cleanup_handles, asmi_model_cleanup_handles), 
 * which examine all entity_handle objects which are managed 
 * (owned) by a set of asm_model objects and destroy any object with a use count of zero, i.e. which are not being held by an 
 * entity_handle_holder object or by an entity_handle_list object with the ASM_HOLD behavior set.
 * <br><br>
 * @see entity_handle, asm_model
 */

class DECL_ASM entity_handle_holder : public ACIS_OBJECT
{
	entity_handle* hdl_ptr;

	// special routines to handle restore, don't adjust use count when dealing with sequence numbers
	friend class component_handle;		// component handle is special because it is private member of asm_model which contains private entity_handle_holder
	friend class component_entity_handle;		// component entity handle is special because it is private member of asm_model which contains private entity_handle_holder
	friend DECL_ASM void read_ent_hdl_hldr( entity_handle_holder& );
	void set_sequence_num( entity_handle* seq_num );
	friend DECL_ASM void fixup_ent_hdl_hldr( entity_handle_holder& );
	void fixup_ptr( entity_handle* hdl );

public:
/**
 * Default constructor.
 * <br>
 * <b>Role:</b> Sets the entity_handle pointer to NULL.
 * <br><br>
 */
	entity_handle_holder();

/**
 * Constructor.
 * <br>
 * <b>Role:</b> Sets the entity_handle pointer.  If this pointer is not NULL, the use count is incremented.
 * <br><br>
 * @param hdl
 * pointer to entity_handle to hold.
 */
	entity_handle_holder( entity_handle* hdl );

/**
 * Destructor.
 * <br>
 * <b>Role:</b> Decrements the use count of the entity_handle object.
 * <br><br>
 */
	~entity_handle_holder();

/**
 * Assignment operator.
 * <br>
 * <b>Role:</b>  Copies the entity_handle pointer.  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid entity_handle pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl_hldr
 * entity_handle_holder.
 */
	entity_handle_holder& operator=( const entity_handle_holder& hdl_hldr );

/**
 * Sets the entity_handle pointer.
 * <br>
 * <b>Role:</b>  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid entity_handle pointer, the corresponding use count is decremented.
 * <br><br>
 * @param hdl
 * pointer to entity_handle to hold.
 */
	void set( entity_handle* hdl );

/**
 * Returns the entity_handle pointer.
 * <br><br>
 */
	entity_handle* get() const;

/**
 * Releases the entity_handle pointer.
 * <br>
 * <b>Role:</b> Decrements the use count of the entity_handle object and sets the entity_handle pointer to NULL.
 * <br><br>
 */
	void release();
};


 /** @} */
#endif // ENTITY_HANDLE_HOLDER_HXX

