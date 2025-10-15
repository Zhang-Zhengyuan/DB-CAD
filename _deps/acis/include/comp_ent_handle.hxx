/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COMPONENT_ENTITY_HANDLE_CLASS
#define COMPONENT_ENTITY_HANDLE_CLASS

#include "acis.hxx"
#include "dcl_asm.h"
#include "ent_handle_holder.hxx"
#include "comp_handle_holder.hxx"

class entity_handle;
class component_handle;
class asm_restore_seq_no_mgr;
class asm_model;

/**
* @file comp_ent_handle.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

// customers are NOT allowed to derive from component_entity_handle
/**
 * Assembly modeling proxy for the occurrence of an ENTITY within an assembly component.
 * <b>Role:</b> A component_entity_handle class object represents an ENTITY within a component of an assembly tree.
 * It is needed to, for example, specify the results of a pick operation.  Because a model can be included in
 * an assembly multiple times, an ENTITY pointer is not sufficient to uniquely specify, for example,
 * a particular face within the front left wheel of a car assembly.  Adding a component identifier to the ENTITY
 * identifier resolves the ambiguity, resulting in a unique identifier.
 * <br>
 * A component_entity_handle is associated with an entity_handle and a component handle, which together can
 * be used to specify a unique topological element of an assembly.
 * <br>
 * A component entity handle is owned by the asm_model object corresponding to the component's root model.  Component
 * entity handle objects are use-counted by component_entity_handle_list and component_entity_handle_holder objects; a component_entity_handle is
 * destroyed when a cleanup() routine is called on it when its use count is zero.
 * <br>
 * @see component_entity_handle_list, component_entity_handle_holder
 */
class DECL_ASM component_entity_handle : public ACIS_OBJECT
{
private:
	entity_handle_holder ent_hldr;	// entity handle holder
	component_handle_holder comp_hldr;	// component handle holder
	int use_cnt;

	// only managing asm_model is allowed to create or destroy component entity handles
	// make friend for access
	friend class asm_model;
	component_entity_handle(entity_handle* entity, component_handle* component);
	~component_entity_handle();
	// notify handle when owning model goes away
	void release_owning_model();
	void restore( entity_handle* ent_seq_num, component_handle* comp_seq_num );

	// internal utility functions
	friend class component_entity_handle_holder;		// these are the only classes allowed to modify the use count
	friend class component_entity_handle_list_eng;
	void add_count();
	void rem_count();

public:

	// gets model managing the component entity
/**
 * Returns the model which owns the component entity,
 * which is also the start model for the component's model reference path.
 */
	asm_model* get_owning_model() const;

/**
 * Returns a logical indicating whether any of the entity_handles used to define the component_entity correspond to deleted entities,
 * A value of FALSE indicates that at least one handle refers to an invalid ENTITY.
 * Note that the validity of a component_entity can be affected by a roll operation in another history stream,
 * so a component_entity_handle that is currently invalid may become valid again at some future time.
 */
	logical is_valid() const;

/**
 * Returns a pointer to the component_handle associated with the component entity.
 */
	component_handle* component() const;
/**
 * Returns a const pointer to the entity_handle associated with the component entity.
 */
	entity_handle const* entity() const;
/**
 * Returns a pointer to the entity_handle associated with the component entity.
 */
	entity_handle* entity();

// un-doc'd methods: INTERNAL USE ONLY
/**
 * @nodoc
 */
	component_entity_handle & operator=(component_entity_handle const & other);

/**
 * @nodoc
 */
	int use_count() { return use_cnt; };

	// destroy the handle if its use count is zero
/**
 * @nodoc
 */
	logical cleanup();

	// true if both handles match
/**
 * @nodoc
 */
	logical operator==(component_entity_handle const & other);

	// might add models to global save model list
/**
 * @nodoc
 */
	void save();

/**
 * @nodoc
 */
	void fixup(asm_restore_seq_no_mgr* mgr);
};

 /** @} */
#endif // COMPONENT_ENTITY_HANDLE_CLASS

