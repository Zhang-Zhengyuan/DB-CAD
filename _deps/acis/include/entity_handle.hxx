/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ENTITY_HANDLE_CLASS
#define ENTITY_HANDLE_CLASS

#include "acis.hxx"
#include "dcl_kern.h"

class ENTITY;
class ENTITY_LIST;
class asm_model;
class entity_proxy;

/**
 * \defgroup ACISENTITYOPS Entity Operations
 * \ingroup KERNAPI
 *
 */
/**
 * @file entity_handle.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISENTITYOPS
 *
 * @{
 */

// customers are NOT allowed to derive from entity_handle

/**
 * Assembly modeling proxy for an ENTITY object.
 * <b>Role:</b> An entity_handle class object serves as a proxy for an ENTITY object in the context of assembly mode.
 * It is used to allow objects outside of the ENTITY's history stream to refer to the ENTITY without running the
 * risk of de-referencing a pointer which has been made invalid by a roll operation on the ENTITY's history stream.
 * An entity handle can be queried for a pointer to its corresponding ENTITY object; if the ENTITY has been lost 
 * or destructed the returned pointer value will be NULL, allowing client code to check for valid pointers.
 * <br>
 * An entity handle is owned by the asm_model object containing the ENTITY corresponding to the handle.  Entity
 * handle objects are use-counted by entity_handle_list and entity_handle_holder objects; an entity_handle is
 * destroyed when a cleanup() routine is called on it when its use count is zero.
 * <br>
 * @see entity_handle_list, entity_handle_holder
 */
class DECL_KERN entity_handle : public ACIS_OBJECT
{
private:
	friend void save_entity_handle(entity_handle* hdl, ENTITY_LIST const & seg_list, int sequence_no);

	entity_proxy* pproxy;	// entity_proxy to which this handle is bound
	asm_model* pmodel;		// pointer to model in which entity lives
	int use_cnt;

	// only managing asm_model is allowed to create or destroy entity handles
	// make friend for access
	friend class asm_model;
	entity_handle(asm_model* model, ENTITY* pent, logical restoring = FALSE);
	friend logical sg_asm_entity_handle_cleanup( entity_handle* eh );
	~entity_handle();
	// notify handle that owning model is going away
	void release_owning_model();

	// internal utility functions
	friend class entity_handle_holder;		// these are the only classes allowed to modify the use count
	friend class entity_handle_list_eng;
	void add_count();
	void rem_count();

public:

// methods to request actual ENTITY pointer
// this may cause part to be swapped back into memory if representation management is being used
/**
 * Returns a const pointer to the <tt>ENTITY</tt> associated with this handle.
 */
	const ENTITY* entity_ptr() const; // request const version of actual pointer

/**
 * Returns a pointer to the <tt>ENTITY</tt> associated with this handle.
 */
	      ENTITY* entity_ptr(); // request actual pointer

/**
 * Returns a pointer to the <tt>asm_model</tt> which owns this handle.
 */
	asm_model* get_owning_model() const;

/**
 * Returns a logical indicating whether the entity handle is bound to a live entity.
 */
	logical is_valid() const;

// un-doc'd methods: INTERNAL USE ONLY
/**
 * @nodoc
 */
	int use_count() { return use_cnt; };

/**
 * @nodoc
 */
	void set_ptr(ENTITY* new_ptr);
/**
 * @nodoc
 */
	void clear_ptr(); // INTERNAL USE ONLY

/**
 * @nodoc
 */
	void fixup(ENTITY* array[]);

};
/** @} */
#endif // ENTITY_HANDLE_CLASS

