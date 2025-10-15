/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ENTITY_MGR_CLASS
#define ENTITY_MGR_CLASS

#include "dcl_asm.h"

class asm_model;
class ENTITY;
class ENTITY_LIST;
class HISTORY_STREAM;
class default_entity_mgr;
class asm_model_list;
class ASM_MODEL_REF;
class ASM_ASSEMBLY;
class acis_pm_entity_mgr;
class SPAbox;
class outcome;
class asm_event_info;

#include "asm_model_info.hxx"
#include "asm_event_type.hxx"

/**
* @file asm_model_entity_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

// Abstract class from which customers derive wrappers for their objects which manage a model's entities.
/**
 * Interface class to wrap customer "part model" and "assembly model" objects.
 * <br><br>
 * <b>Role:</b> Provides an interface for messages between the ACIS Assembly Modeling and customer 
 * part management system.
 * <br><br>
 * This is the base class from which customers may derive their own entity manager objects. 
 * It is assumed that customers who are not using the ACIS part manager will have their own objects 
 * that manage the entities within a part. The asm_model_entity_mgr class provides an interface for 
 * assembly modeling to query the customer "part" objects and to pass messages (such as 
 * activate/deactivate) to them. The mechanism for this is for the customer to derive a sub-class 
 * of <tt>asm_model_entity_mgr</tt> that wraps a customer "part" object, and implement the virtual
 * query and notification methods. Each entity manager object in memory has a corresponding 
 * <tt>asm_model</tt> object. Spatial provides four standard subclasses of <tt>asm_model_entity_mgr</tt>:
 * <br><br>
 * <b><tt>default_entity_mgr</tt>:</b> This is a basic implementation for use during save and restore 
 * of assemblies by customers who have their own assembly modeling system, and so do not need to derive 
 * their own sub-class. It should <i>not</i> be used for actual part management.
 * <br><br>
 * <b><tt>acis_pm_entity_mgr</tt>:</b> This is a subclass of <tt>asm_model_entity_mgr</tt> that wraps 
 * an ACIS <tt>PART</tt> object, and is intended to be used by customers who are using the ACIS part 
 * manager. Customers may need to further derive from this class in order to (for example) interface 
 * with their rendering system.
 * <br><br>
 * <b><tt>acis_ha_part_entity_mgr</tt>:</b> This is a subclass of <tt>acis_pm_entity_mgr</tt> that is
 * intended to be used by customers who are using both the ACIS part manager and the HOOPS/ACIS bridge.  
 * It adds functionality that maintains the HOOPS scene graph corresponding to the assembly model.
 * Customers may need to further derive from this class in order to manage views of the model.
 * <br><br>
 * <b><tt>acis_scm_entity_mgr</tt>:</b> This is a subclass of <tt>acis_ha_part_entity_mgr</tt> that is 
 * used to interface with the ACIS Scheme Toolkit's rendering system. It adds view management to the 
 * capabilities of the <tt>acis_ha_part_entity_mgr</tt>.
 * <br><br>
 * @see default_entity_mgr, acis_pm_entity_mgr, acis_ha_part_entity_mgr, acis_scm_entity_mgr, asm_model, 
 * entity_mgr_factory
 */
class DECL_ASM asm_model_entity_mgr : public ACIS_OBJECT
{
	friend class asm_model;
	friend class ASM_MODEL_REF;
	friend void mgr_call_unbind(asm_model_entity_mgr*);

	asm_model* model_ptr; // model corresponding to this mgr

	
protected:

	// helper function to test if entity's top-level owner is owned by entity manager
	// implemented by calling get_top_level_entities
	virtual logical owns_entity(ENTITY const* entity) const;

/**
 * Unbinds the manager from the owning model.
 * <br><br>
 * <b>Role:</b> This function is used internally by ACIS to unbind an entity manager
 * from its model, usually when the model is being cleaned up or during model destruction.
 * It calls <tt>clear_vf</tt>, calls <tt>unbind_vf</tt>, unbinds the manager from the model and, if
 * <tt>owned_by_model_vf</tt> returns <tt>TRUE</tt>, calls the destructor of the entity manager.
 */
	void unbind();

// virtual functions intended to be overridden by customer mgr
/**
 * Returns a pointer to the assembly entity, if any, owned by this entity manager.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 */
	virtual ASM_ASSEMBLY* get_assembly_ptr_vf() const = 0;

/**
 * Transfers ownership of an assembly entity to this entity manager.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.  
 * <br><br>
 * @param assembly_ptr
 * pointer to the new assembly entity. If NULL, the entity manager should delete its old assembly 
 * pointer, if it owns one, using <tt>api_del_entity</tt> or <tt>api_del_entity_list</tt>.  
 * If non-<tt>NULL</tt> and the entity manager already owns an assembly, this routine
 * should call <tt>sys_error</tt>.
 */
	virtual void set_assembly_ptr_vf( ASM_ASSEMBLY* assembly_ptr ) = 0;

/**
 * Returns an entity list containing pointers to all top-level entities owned by the entity manager.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 * <br><br>
 * If the <tt>include_assemblies</tt> flag is <tt>FALSE</tt>, then the assembly entity
 * (if one is owned by this entity manager) should be excluded from the list.
 * <br><br>
 * @param ents
 * list of top-level entities owned by the entity manager. An entity is top-level if its 
 * <tt>owner</tt> method returns <tt>NULL</tt>.
 * @param include_assemblies
 * indicates whether the entity manager's assembly pointer (if any) should be included in the list.
 */
	virtual void get_top_level_entities_vf(ENTITY_LIST& ents, logical include_assemblies) const = 0;

/**
 * Asks a subclass to put itself into the "empty" state.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.  
 * <br><br>
 * This routine is intended to ensure that all deletions to be done by this entity manager are 
 * done before static destruction. It will be called by <tt>unbind</tt>, as part of unbinding 
 * the entity mananager from its associated <tt>asm_model</tt> object.
 * <br><br>
 * This routine should delete all entities and history streams owned by the <tt>asm_model_entity_mgr</tt> 
 * subclass object, <i>including</i> the assembly (if any) and all backup copies of these objects 
 * owned by the object's history stream. Deleting entities is done by calling @href api_del_entity 
 * or @href api_del_entity_list; deleting backup copies is done by deleting delta states from the 
 * history stream.
 * <br><br>
 * Note that the assembly entity is always owned by the <tt>asm_model_entity_mgr</tt> subclass object, 
 * and so must always be deleted by this routine.
 * <br><br>
 * Note that the history stream and/or entities may be owned by the customer's entity management object 
 * (which is wrapped by the <tt>asm_model_entity_mgr</tt> subclass object) rather than by the 
 * <tt>asm_model_entity_mgr</tt> subclass object.  
 * Any such objects owned by the customer's entity management object need not be deleted by this 
 * routine; instead they may be deleted by the customer's entity management object, with the caveat 
 * that the customer must ensure that any delta states containing backup copies of entities owned by 
 * the <tt>asm_model_entity_mgr</tt> subclass object are deleted. Practically speaking, this means that
 * this routine should either call @href api_delete_history, <tt>delete_all_delta_states</tt> with a 
 * <tt>keep_stream</tt> flag of <tt>TRUE</tt>, or @href api_asm_prune_assembly_history if the
 * <tt>asm_model_entity_mgr</tt> subclass object has ever owned an assembly object, whether or not it 
 * has been deleted. Note that this will result in a side effect for other models whose entity managers 
 * share this one's history stream, namely that their history and entities will be deleted as well.
 * <br><br>
 * For the case of the <tt>acis_pm_entity_mgr</tt> supplied with
 * the ACIS Part Management component, the <tt>asm_model_entity_mgr</tt> subclass object referred to 
 * above is an <tt>acis_pm_entity_mgr</tt> object, while the customer's entity management object 
 * referred to above is the corresponding <tt>PART</tt> object. In that case, this routine
 * could be implemented by calling the <tt>PART::clear</tt> method, which deletes all entities and 
 * delta states associated with the <tt>PART</tt>.
 * For a more sophisticated example implementation using <tt>api_prune_history</tt>, refer to the 
 * actual implementation of <tt>acis_pm_entity_mgr::clear_vf</tt> in the example source files.
 * <br><br>
 * Entities should be deleted with @href api_del_entity_list.
 * <br><br>
 * History streams should be deleted with @href api_delete_history.
 * <br><br>
 * Note that the entity manager destructor should <i>not</i> delete any entities, because 
 * it may be called during static destruction after the history streams have already been deleted.
 * <br><br>
 * This routine will <i>not</i> be called within a <tt>MODEL_BEGIN/MODEL_END</tt> block, so it is the 
 * subclass' responsibility to ensure that operations are performed in the correct history stream, 
 * to use <tt>API_BEGIN/API_END</tt> blocks where necessary,
 * and to ensure that the history stream is reset to its original value upon exit.
 */
	virtual void clear_vf()=0;

	// methods used during destruction of model/mgr pair
/**
 * Notifies the entity manager that its <tt>asm_model</tt> is breaking their association.
 * <br><br>
 * <b>Role:</b> This function is intended to allow the entity manager to
 * take action when it is being unbound from its <tt>asm_model</tt> object.
 * A default implementation of "do nothing" is provided by the base class, so it is not required 
 * that subclasses override this function.
 * <br><br>
 * @param model
 * the model breaking the association. This should agree with the <tt>asm_model_entity_mgr</tt>'s 
 * internal model pointer.
 */
	virtual void unbind_vf(asm_model* model);

/**
 * Asks a subclass if it is owned by its associated model.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.  
 * <br><br>
 * This routine is intended to allow customers to configure the ownership policy of their entity 
 * managers. If the return value is <tt>TRUE</tt>, then unbinding the manager from its associated 
 * model will result in a call <tt>ACIS_DELETE</tt> on
 * the manager. If the return value is <tt>FALSE</tt>, then the customer has responsibility
 * for ensuring that unbound manager objects are deleted.
 */
	virtual logical owned_by_model_vf() const=0; // if TRUE, model will call destructor on mgr in model's destructor (after unbinding)


/**
 * Returns a pointer to the history stream associated with the entity manager.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.  
 * <br><br>
 * This routine should return a pointer to the history stream containing the entities owned by the 
 * entity manager. It is primarily used during save and restore to determine which models share history 
 * streams. This routine should always return a non-<tt>NULL</tt> value, even if the entity manager 
 * is using a global default history stream.
 */
	virtual HISTORY_STREAM* get_history_vf() = 0; // returns history stream associated with this mgr

/**
 * Returns a list of all models that share the manager's history stream (including the manager's model).
 * <br><br>
 * <b>Role:</b> This routine should add pointers to all of the <tt>asm_model</tt> objects that own 
 * entity managers that share history with this entity manager and return a value of <tt>TRUE</tt> 
 * if successful. It is used as an optimization during save to determine which models must have their
 * entities written to the same entity segment in the ASAT file. A return value of <tt>TRUE</tt>
 * indicates that the correct <tt>sharing_models</tt> have been added to this list. 
 * A value of <tt>FALSE</tt> indicates that this routine either is not overridden or that an error 
 * has occurred, in which case the calling function will fall back on a less efficient algorithm 
 * that uses <tt>get_history_vf</tt>.
 */
	virtual logical get_models_which_share_history_vf(asm_model_list & sharing_models);

/**
 * Returns a structure containing information about the associated assembly model.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 * <br><br>
 * This routine is used to query the entity manager for information about the model that is to be 
 * saved to an ASAT file. This routine may be replaced by individual queries in the future.
 */
	virtual asm_model_info get_model_info_vf() const = 0;

/**
 * Notifies the entity manager of entities that are being added to it.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 * <br><br>
 * This method is used to give ownership of a set of entities to the entity manager.
 * If the <tt>restoring_asat</tt> flag is <tt>TRUE</tt>, then the entities are being added to the 
 * manager as part of the ASAT restore process. In this case, the model will only be in a partially 
 * constructed state, in that submodels may not have yet been read in, so that processing 
 * (such as rendering) of these entities should be deferred to the <tt>done_restoring_vf</tt> method.
 * <br><br>
 * @param ents
 * the entities being restored. Note that these may not all be top-level entities.
 * @param restoring_asat
 * indicates whether the entities are to be added to the manager as part of the ASAT restore process.
 */
	virtual void add_entities_vf(ENTITY_LIST& ents, logical restoring_asat) = 0;

/**
 * Notifies the entity manager of the completion of the restore process.
 * <br><br>
 * <b>Role:</b>  
 * This method is called at the end of the restore process, when all models that were successfully 
 * read in are in a valid state. It is intended to give the customer a chance to process the entities 
 * that were added to it by the method <tt>add_entities_vf</tt>.
 */
	virtual void done_restoring_vf();

/**
 * Requests that the entity manager make its modeling context (that is, history stream, units, 
 * tolerances) active in a nested way.
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 * <br><br>
 * This method is used to pass a request to the customer's entity management object that it make 
 * itself active, so that operations can be performed on entities owned by the entity management 
 * object. This routine is called during the <tt>MODEL_BEGIN</tt> portion of a model block.
 * <br><br>
 * @param from_model
 * the currently active model. <tt>NULL</tt> if this is the outermost call to <tt>nested_activate_vf</tt>.
 */
	virtual void nested_activate_vf(asm_model const* from_model) const = 0;

/**
 * Notifies the entity manager of changes made to its contents while the entity manager was activated.
 * <br><br>
 * <b>Role:</b> 
 * This method is used to notify the customer's entity management object of the severity of changes that
 * were made to the entities owned by the object by assembly modeling while the object was active. The
 * event type is passed by reference so that customer code can perform error recovery operations
 * that adjust the type of event. This routine is called before <tt>nested_deactivate_vf</tt> during 
 * the <tt>MODEL_END</tt> portion of a model block.
 * <br><br>
 * @param result
 * outcome object from the last API operation performed while active.
 * @param ev_type
 * severity of the change. This will have a value of <tt>ASM_BREP_GEOM_CHANGE</tt>,
 *  <tt>ASM_COMP_GEOM_CHANGE</tt>, <tt>ASM_BREP_RENDER_CHANGE</tt>, <tt>ASM_COMP_RENDER_CHANGE</tt>, 
 * <tt>ASM_OTHER_CHANGE</tt>, or <tt>ASM_NO_CHANGE</tt>.
 * <br><br>
 * @see asm_event_type
 */
	virtual void entities_changed_vf( outcome& result, asm_event_type& ev_type );

/**
 * @nodoc
 */
	virtual void entities_changed_vf( outcome& result, asm_event_type& ev_type, asm_event_info* ev_info );

/**
 * Requests that the entity manager deactivate its modeling context (that is, history stream, units, 
 * tolerances).
 * <br><br>
 * <b>Role:</b> Subclasses <i>must</i> override this function.
 * <br><br>
 * This method is used to pass a request to the customer's entity management object that it deactivate 
 * itself and reactivate the previously active object.  
 * It is called during the <tt>MODEL_END</tt> portion of a model block, after <tt>entity_changed_vf</tt>.
 * <br><br>
 * @param to_model
 * previously active model. <tt>NULL</tt> if deactivating the outermost call to <tt>nested_activate_vf</tt>.
 */
	virtual void nested_deactivate_vf(asm_model const* to_model) const = 0;


/**
 * Notifies the entity manager that a change was made to one of its sub-models.
 * <br><br>
 * <b>Role:</b>   
 * This method is used to notify the customer's entity management object that a sub-model has changed. 
 * This allows the customer object to, for example, re-render its assembly.
 * This routine is called during 
 * the <tt>MODEL_END</tt> portion of a model block, after <tt>nested_deactivate_vf</tt>.
 * It is also called by <tt>asm_model::contents_changed</tt>.
 * <br><br>
 * @param ev_type
 * severity of the change. This will have a value of <tt>ASM_BREP_GEOM_CHANGE</tt>,
 * <tt>ASM_COMP_GEOM_CHANGE</tt>, <tt>ASM_BREP_RENDER_CHANGE</tt>, <tt>ASM_COMP_RENDER_CHANGE</tt>, 
 * <tt>ASM_OTHER_CHANGE</tt>, or <tt>ASM_NO_CHANGE</tt>.
 * @param changed_model
 * indicates the sub-model that has changed.
 */
	virtual void sub_model_changed_vf( asm_event_type ev_type, asm_model* changed_model );

///////////////  downcast operators /////////////////////////////////////
///////////////  supplied to avoid depending on dynamic_cast ////////////

	// will only be non-NULL if concrete type is "default_entity_mgr" (supplied by Spatial)
	// customers must not override this function
/**
 * Dynamic down-cast to <tt>default_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> Downcasts <tt>this</tt> pointer to a <tt>default_entity_mgr</tt> if the
 * concrete type of <tt>this</tt> is <tt>default_entity_mgr</tt> or a sub-class.
 * <br><br>
 * This routine should be overridden by customer classes that are directly derived from 
 * <tt>asm_model_entity_mgr</tt>.
 * A default implementation of <tt>NULL</tt> is provided for non-customer classes, or for 
 * customer subclasses that are derived from other Spatial entity managers (that is, that are 
 * derived from Spatial-supplied subclasses of <tt>asm_model_entity_mgr</tt>).
 */
	virtual default_entity_mgr* default_cast_vf();

	// will only be non-NULL if concrete type is "acis_pm_entity_mgr" (supplied by Spatial)
	// customers must not override this function
/**
 * Dynamic down-cast to <tt>acis_pm_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> Downcasts <tt>this</tt> pointer to an <tt>acis_pm_entity_mgr</tt> if the
 * concrete type of <tt>this</tt> is <tt>acis_pm_entity_mgr</tt> or a sub-class.
 * <br><br>
 * Returns <tt>NULL</tt> if <tt>this</tt> is not an <tt>acis_pm_entity_mgr</tt>. This
 * routine must not be overridden by customers.
 */
	virtual acis_pm_entity_mgr* acis_pm_cast_vf();

	// customers who sub-class from asm_model_entity_mgr may override this method to
	// return a pointer to this.  Since Spatial will never override this function
	// in a Spatial-supplied sub-class of asm_model_entity_mgr, customers may then use a
	// non-NULL value returned from customer_cast() to indicate that this is
	// a customer-derived sub-class and cast the pointer to the appropriate value.
/**
 * Dynamic down-cast to a customer sub-type (represented by <tt>void*</tt>).
 * <br><br>
 * <b>Role:</b> The <tt>customer_cast</tt> method is the mechanism for customers to determine 
 * whether a pointer to an <tt>asm_model_entity_mgr</tt> is pointing to a customer-defined subclass 
 * of <tt>asm_model_entity_mgr</tt> or a subclass provided by Spatial.
 * Customers who derive their own subclass of <tt>asm_model_entity_mgr</tt> must override this routine 
 * to return the <tt>this</tt> pointer of the object. 
 * If this method returns a non-<tt>NULL void*</tt>, then the customer will know that the pointer can
 * safely be down-cast to the customer-defined subclass of <tt>asm_model_entity_mgr</tt>.  
 * A Spatial-defined subclass of <tt>asm_model_entity_mgr</tt>
 * will <i>never</i> return a non-<tt>NULL</tt> value from this method.
 */
	virtual void* customer_cast_vf();

public:
/**
 * Returns the <tt>asm_model</tt> associated with this entity manager.
 * <br><br>
 * <b>Role:</b> Returns the <tt>asm_model</tt> object to which this entity manager is bound.
 */
	asm_model* get_model() const; // model to which this mgr is bound

/**
 * @nodoc
 */
	void set_model( asm_model* model );

/**
 * Returns a pointer to the entity manager's <tt>ASM_ASSEMBLY</tt> object, if any.
 * <br><br>
 * <b>Role:</b> This routine returns a pointer to the assembly entity owned by the entity manager.  
 * It returns NULL if the entity manager does not own an assembly.
 */
	ASM_ASSEMBLY* get_assembly_ptr() const;

/**
 * @nodoc
 */
	void set_assembly_ptr( ASM_ASSEMBLY* assembly_ptr );

/**
 * @nodoc
 */
	void get_top_level_entities(ENTITY_LIST& ents, logical include_assemblies) const;

/**
 * @nodoc
 */
	asm_model_info get_model_info() const;

/**
 * @nodoc
 */
	 void add_entities(ENTITY_LIST& ents, logical restoring_asat = FALSE);
//	void register_restored_entities(ENTITY_LIST&);
/**
 * @nodoc
 */
	 void done_restoring();

/**
 * @nodoc
 */
	HISTORY_STREAM* get_history();

/**
 * @nodoc
 */
	void get_models_which_share_history(asm_model_list & sharing_models);

/**
 * @nodoc
 */
//	void clear();

///////////////  downcast operators /////////////////////////////////////
///////////////  supplied to avoid depending on dynamic_cast ////////////

	// will only be non-NULL if concrete type is "default_entity_mgr" (supplied by Spatial)
	// customers must not override virtual function
/**
 * Dynamic down-cast to <tt>default_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> Downcasts <tt>this</tt> pointer to a <tt>default_entity_mgr</tt> if the
 * concrete type of <tt>this</tt> is <tt>default_entity_mgr</tt> or a sub-class.
 * <br><br>
 * Returns <tt>NULL</tt> if this is not a <tt>default_entity_mgr</tt>.
 */
	virtual default_entity_mgr* default_cast();

	// will only be non-NULL if concrete type is "acis_pm_entity_mgr" (supplied by Spatial)
	// customers must not override virtual function
/**
 * Dynamic down-cast to <tt>acis_pm_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> Downcasts <tt>this</tt> pointer to an <tt>acis_pm_entity_mgr</tt> if the
 * concrete type of <tt>this</tt> is <tt>acis_pm_entity_mgr</tt> or a sub-class.
 * <br><br>
 * Returns <tt>NULL</tt> if this is not an <tt>acis_pm_entity_mgr</tt>.
 */
	virtual acis_pm_entity_mgr* acis_pm_cast();

	// customers who sub-class from asm_model_entity_mgr may override this method to
	// return a pointer to this.  Since Spatial will never override this function
	// in a Spatial-supplied sub-class of asm_model_entity_mgr, customers may then use a
	// non-NULL value returned from customer_cast() to indicate that this is
	// a customer-derived sub-class and cast the pointer to the appropriate value.
/**
 * Dynamic down-cast to a customer sub-type (represented by <tt>void*</tt>).
 * <br><br>
 * <b>Role:</b> The <tt>customer_cast</tt> method is the mechanism for customers to determine whether 
 * a pointer to an <tt>asm_model_entity_mgr</tt> is pointing to a customer-defined subclass of 
 * <tt>asm_model_entity_mgr</tt> or a subclass provided by Spatial.
 * If this method returns a non-<tt>NULL void*</tt>, then the customer will know that the pointer can
 * safely be down-cast to the customer-defined subclass of <tt>asm_model_entity_mgr</tt>. 
 * A Spatial-defined subclass of <tt>asm_model_entity_mgr</tt>
 * will <i>never</i> return a non-<tt>NULL</tt> value from this method.
 */
	virtual void* customer_cast();

/**
 * Destructor.
 */
	virtual ~asm_model_entity_mgr();
/**
 * Default constructor.
 */
	asm_model_entity_mgr();
};

/**
 * @nodoc
 */
DECL_ASM void sg_asm_set_tolerances( asm_model_info& model_info );

 /** @} */
#endif // ENTITY_MGR_CLASS
