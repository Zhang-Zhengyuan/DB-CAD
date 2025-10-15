/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COMPONENT_HANDLE_CLASS
#define COMPONENT_HANDLE_CLASS

#include "acis.hxx"
#include "dcl_asm.h"
#include "entity_handle_list.hxx"

class entity_handle;
class entity_handle_list;
class component_handle_list;
class asm_model;
class asm_restore_seq_no_mgr;
// begin FAST_CE_LOOKUP
class component_entity_handle;
class entity_to_component_entity_map;
// end FAST_CE_LOOKUP

/**
* @file comp_handle.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

// customers are NOT allowed to derive from component_handle
/**
 * Assembly modeling proxy for an assembly component.
 * <b>Role:</b> A component_handle class object represents a component in an assembly tree.
 * A component is a path in the model reference tree, and can be represented as a start model and
 * a sequence of tip-to-tail model references (where the owninng model of the i+1'th model reference
 * is the referenced model of the i'th model reference).
 * <br>
 * A component handle is owned by the asm_model object corresponding to the component's root model.  Component
 * handle objects are use-counted by component_handle_list and component_handle_holder objects; an component_handle is
 * destroyed when a cleanup() routine is called on it when its use count is zero.
 * <br>
 * @see component_handle_list, component_handle_holder
 */
class DECL_ASM component_handle : public ACIS_OBJECT
{
private:
	asm_model* pmodel; // pointer to model in which component lives
	entity_handle_list mrefs;
	// owning model can be obtained from pmref, but we want to use pointer as flag when model is deleted
	int use_cnt;

// begin FAST_CE_LOOKUP
	entity_to_component_entity_map* component_entities;
	// routines (called by asm_model) to manage cm
	component_entity_handle* lookup_ce(entity_handle* ent);
	void add_component_entity(component_entity_handle* to_add);
	void remove_component_entity(component_entity_handle* to_remove);
	// give component_entity_handle friendship so it can register/deregister itself
	// with fast lookup when constructed/destroyed
	friend class component_entity_handle;
// end FAST_CE_LOOKUP


	// only managing asm_model is allowed to create or destroy component handles
	// make friend for access
	friend class asm_model;
	component_handle(entity_handle_list & model_refs);
	component_handle(asm_model* model);
	~component_handle();
	// notify handle when owning model goes away
	void release_owning_model();
	void restore( asm_model* owning_model, entity_handle_list & model_refs );

	// internal utility functions
	friend class component_handle_holder;		// these are the only classes allowed to modify the use count
	friend class component_handle_list_eng_base;
	void add_count();
	void rem_count();

	// property owners cache
	friend class ATTRIB_COMPONENT_PROP_OWNER;
	mutable entity_handle_list prop_owners_cache;
	mutable logical prop_owners_cache_is_valid;
	void set_property_owners_cache( const entity_handle_list& prop_owners ) const;
	logical append_property_owners_cache( entity_handle_list& prop_owners ) const;
	void invalidate_property_owners_cache() const;
	void invalidate_property_owners_cache(asm_model const* owning_model) const;
	void append_property_owners( entity_handle_list& prop_owners ) const;

	// utility to remove top mref from the path
	// returns NULL if mref list already empty
	component_handle* chop_first_mref() const;

public:

/**
 * Returns the model which owns the component,
 * which is also the start model for the component's model reference path.
 */
	asm_model* get_owning_model() const;

/**
 * Returns a logical indicating whether any of the entity_handles used to define the component correspond to deleted entities,
 * A value of FALSE indicates that at least one handle refers to an invalid ENTITY.
 * Note that the validity of a component can be affected by a roll operation in another history stream,
 * so a component_handle that is currently invalid may become valid again at some future time.
 */
	logical is_valid() const;

	// gets first model_ref in model_ref path - this will be model_ref in assembly in which component is defined
/**
 * Returns a handle for the first model reference in the component's model reference path.
 */
	entity_handle const* get_first_model_ref() const;

	// gets complete chain of model_refs that defines component
/**
 * Returns a list of handles for the component's entire model reference path.
 * <br><br>
 * @param model_ref_path
 * the list of model reference handles.
 */
	void get_model_ref_path( entity_handle_list & model_ref_path ) const;
/**
 * Returns a handle for the ii'th model reference in the component's model reference path.
 * <br><br>
 * @param ii
 * the element index
 */
	entity_handle const* get_model_ref_path_element(int ii) const;
/**
 * Returns the length of the component's model reference path.  
 * Will be zero if the path terminates at the start model.
 */
	int get_model_ref_path_length() const;

/**
 * Returns the last model in the component's model reference path.  
 * This will be the last model reference's reference model for non-zero path length.
 */
	asm_model* get_end_model() const;

// un-doc'd methods: INTERNAL USE ONLY
/**
 * @nodoc
 */
	int use_count() { return use_cnt; };

	// destroy the handle if its use count is zero
/**
 * @nodoc
 */
	logical cleanup();

	// might add models to global save model list
/**
 * @nodoc
 */
	void save();
/**
 * @nodoc
 */
	void fixup(asm_restore_seq_no_mgr* mgr);

/**
 * @nodoc
 **/
	logical contains_component( const component_handle* comp ) const;
/**
 * @nodoc
 **/
	void get_property_owners( entity_handle_list& prop_owners ) const;

/**
 * @nodoc
 **/
	void mark_property_owners_cache_invalid() const;
};

 /** @} */
#endif // COMPONENT_HANDLE_CLASS

