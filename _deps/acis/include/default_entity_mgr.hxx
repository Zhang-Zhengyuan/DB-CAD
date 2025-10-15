/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DEFAULT_ENTITY_MGR_CLASS
#define DEFAULT_ENTITY_MGR_CLASS

#include "dcl_asm.h"
#include "lists.hxx"
#include "asm_model_entity_mgr.hxx"
#include "asm_model_info.hxx"
#include "asm_event_type.hxx"

class asm_model;
class asm_model_list;
class asm_event_info;

/**
* @file default_entity_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

// Concrete default derivation of asm_model_entity_mgr without intrinsic part management capabilities
/**
 * Concrete entity manager class that simply wraps an <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b> The default entity manager (and its corresponding factory) are provided for 
 * customers who do not need a full-blown entity manager integrated with their part management
 * system.  A default entity manager is essentially a wrapper for an entity list containing the
 * managed entities.  Note that the default entity manager only provides rudimentary part management
 * capabilities; it is intended only to remember a list of entities and a history stream and to delete them
 * when appropriate.  Please refer to the assembly modeling technical articles for further details.
 * <br>
 * @see default_entity_mgr
 */
class DECL_ASM default_entity_mgr : public asm_model_entity_mgr
{
	ENTITY_LIST ents; // non-assembly entities in the part
	ASM_ASSEMBLY* asm_ptr;
	asm_model_info my_model_info;
	logical owns_ents;
	logical owns_stream;
	logical prune_on_clear;
	HISTORY_STREAM* my_stream;

	mutable double old_resabs;
	mutable double old_resnor;
	mutable HISTORY_STREAM* old_stream;

protected:
/**
 * @nodoc
 */
	virtual ASM_ASSEMBLY* get_assembly_ptr_vf() const;
/**
 * @nodoc
 */
	virtual void set_assembly_ptr_vf( ASM_ASSEMBLY* assembly_ptr );
/**
 * @nodoc
 */
	virtual void get_top_level_entities_vf(ENTITY_LIST & entities, logical include_assemblies) const; // appends to list, called by get_entities()
/**
 * @nodoc
 */
	virtual void clear_vf();

/**
 * @nodoc
 */
	virtual asm_model_info get_model_info_vf() const;

//	virtual void unbind_vf(asm_model*  model );

/**
 * @nodoc
 */
	virtual HISTORY_STREAM* get_history_vf();

/**
 * @nodoc
 */
	virtual logical get_models_which_share_history_vf(asm_model_list & sharing_models);

/**
 * @nodoc
 */
	virtual void add_entities_vf(ENTITY_LIST& ents, logical restoring_asat);
//	virtual void register_restored_entities_vf(ENTITY_LIST& ents);

/**
 * @nodoc
 */
	virtual void nested_activate_vf(asm_model const* from_model) const;
/**
 * @nodoc
 */
	virtual void nested_deactivate_vf(asm_model const* to_model) const;
/**
 * @nodoc
 */
	virtual void entities_changed_vf( outcome& result, asm_event_type& ev_type, asm_event_info* ev_info );
/**
 * @nodoc
 */
	virtual logical owned_by_model_vf() const; // if TRUE, model will call destructor on mgr in model's destructor (after unbinding)
/**
 * @nodoc
 */
	virtual default_entity_mgr* default_cast_vf();

public:
/**
 * Default constructor - will bind to the input history stream if it is non-<tt>NULL</tt>.
 * <br><br>
 * @param mgr_stream
 * history stream to be bound to this manager.
 */
	default_entity_mgr(HISTORY_STREAM* mgr_stream = NULL); // default constructor - does not take ownership, uses default stream if mgr_stream == NULL
/**
 * Destructor
 */
	~default_entity_mgr();

/**
 * Specifies whether the <tt>default_entity_mgr</tt> owns the entities that it contains.
 * <br><br>
 * <b>Role:</b> By default, the <tt>default_entity_mgr</tt> owns all entities that have been registered with it.  This method allows
 * the application to override this behavior.  Note that the <tt>default_entity_mgr</tt> <b>always</b> owns its <tt>ASM_ASSEMBLY</tt> object, if present.
 * <br><br>
 * @param new_value
 * ownership flag.  <tt>FALSE</tt> indicates that the entities are un-owned.
 */
	void set_owns_ents(logical new_value);
/**
 * Specifies whether the <tt>default_entity_mgr</tt> owns the history stream to which it is bound.
 * <br><br>
 * <b>Role:</b> By default, the <tt>default_entity_mgr</tt> owns the history stream to which it is bound.  This method allows
 * the application to override this behavior.  It is the application's responsibility to ensure that a history stream does
 * not have more than one owner.
 * <br><br>
 * Note that the implementation of <tt>default_entity_mgr::get_models_which_share_history_vf</tt> 
 * assumes that no other models share history with this entity manager if <tt>owns_stream</tt> is <tt>TRUE</tt>.
 * Customers who intentionally break this behavior must derive a subclass of <tt>default_entity_mgr</tt> that
 * overrides the implementation of <tt>get_models_which_share_history_vf</tt>.
 * <br><br>
 * @param new_value
 * ownership flag.  <tt>FALSE</tt> indicates that the history stream is un-owned.
 */
	void set_owns_stream(logical new_value);
	/**
	* Specifies whether the history stream is prunes as a part of the clear_vf.  pruning history streams is a slow operation,
	* but it helps clean up memory sooner. Default value is to prune on clear.  It may be more optimal not to prune,
	* but to delete the entire history stream when you are done with the assembly model.
	**/
	void set_prune_on_clear(logical new_value);

/**
 * Adds an entity to the list of entities being managed by this <tt>default_entity_mgr</tt>.
 * <br><br>
 * @param ent
 * entity to be added.
 */
	void add_entity(ENTITY* ent);

/**
 * Associates model information with the <tt>default_entity_mgr</tt>.
 * <br><br>
 * @param model_info
 * new model_info to be used.
 */
	void set_model_info( asm_model_info const& model_info );

	// overrides of virtual methods
};

 /** @} */
#endif // DEFAULT_ENTITY_MGR_CLASS
