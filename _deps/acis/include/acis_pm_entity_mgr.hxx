/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ACIS_PM_ENTITY_MGR_CLASS
#define ACIS_PM_ENTITY_MGR_CLASS

#include "dcl_part.h"
#include "asm_model_entity_mgr.hxx"

class PART;
class ENTITY_LIST;
class asm_model;
class asm_model_list;
class SPAvoid_ptr_array;
class acis_ha_part_entity_mgr;
class acis_scm_entity_mgr;

/**
 * @file acis_pm_entity_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup PARTAPI
 *
 * @{
 */


/**
 * Concrete entity manager class for using the ACIS Part Management component in conjunction
 * with ACIS Assembly Modeling.
 * <br><br>
 * <b>Role:</b> The <tt>acis_pm_entity_mgr</tt> (and its corresponding factory) are provided for 
 * customers who use the ACIS Part Management component.  The implementations of <tt>acis_pm_entity_mgr</tt>
 * and <tt>acis_pm_entity_mgr_factory</tt> are provided as example code.  It is possible that customers
 * will need to further derive from <tt>acis_pm_entity_mgr</tt> in order to integrate ACIS Assembly Modeling
 * into their rendering systems; an example of this is the <tt>acis_scm_entity_mgr</tt> subclass.
 * Please refer to the assembly modeling technical articles for further details.
 * <br>
 * @see acis_pm_entity_mgr, acis_scm_entity_mgr
 */
class DECL_PART acis_pm_entity_mgr : public asm_model_entity_mgr
{
	asm_model_info my_model_info;

protected:
/**
 * @nodoc
 */
	PART* the_PART;
/**
 * @nodoc
 */
	 mutable PART* previous_PART;

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
	virtual void get_top_level_entities_vf(ENTITY_LIST & ents, logical include_assemblies) const; // appends to list, called by get_entities()
/**
 * @nodoc
 */
	virtual void clear_vf();
/**
 * @nodoc
 */
	virtual asm_model_info get_model_info_vf() const;

/**
 * @nodoc
 */
	virtual void add_entities_vf(ENTITY_LIST& ents, logical restoring_asat);
//	virtual void register_restored_entities_vf(ENTITY_LIST& ents);

/**
 * @nodoc
 */
	virtual HISTORY_STREAM* get_history_vf();

/**
 * @nodoc
 */
	virtual logical get_models_which_share_history_vf(asm_model_list & sharing_models);

	// current implementation has part delete the asm_model_entity_mgr when it gets destroyed
	//virtual void unbind_vf(asm_model* model ); // gives customer opportunity to delete mgr when all model_refs are destroyed

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
	virtual logical owned_by_model_vf() const; // if TRUE, model will call destructor on mgr in model's destructor (after unbinding)
/**
 * @nodoc
 */
	virtual acis_pm_entity_mgr* acis_pm_cast_vf();
/**
 * @nodoc
 */
	virtual acis_ha_part_entity_mgr* acis_ha_part_cast_vf();
/**
 * @nodoc
 */
	virtual void* pm_customer_cast_vf();

public:
/**
 * Default constructor.
 * <br><br>
 * <b>Role:</b> Constructs an unbound <tt>acis_pm_entity_mgr</tt> object.  If a <tt>PART</tt>
 * object is specified, it will be bound to the new entity manager.
 * <br><br>
 * @param part
 * part to be bound.
 */
	acis_pm_entity_mgr(PART* part = NULL);
/**
 * Destructor.
 */
	~acis_pm_entity_mgr();

/**
 * Gets the <tt>PART</tt> bound to this entity manager.
 */
	PART* get_part() const;

/**
 * Binds this entity manager to the specified <tt>PART</tt>.
 * @param part
 * part to be bound.
 */
	void set_part(PART* part);

/**
 * Changes the <tt>model_info</tt> information associated with the entity manager's bound <tt>PART</tt>.
 */
	void set_model_info( asm_model_info const & context );

	// overrides of virtual methods

	// will only be non-NULL if concrete type is "acis_scm_entity_mgr" (supplied by Spatial)
	// customers must not override this function
/**
 * Dynamic down-cast to <tt>acis_ha_part_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> Downcasts <tt>this</tt> pointer to an <tt>acis_ha_part_entity_mgr</tt> if the
 * concrete type of <tt>this</tt> is <tt>acis_scm_entity_mgr</tt> or a sub-class.
 * <br><br>
 * Returns <tt>NULL</tt> if <tt>this</tt> is not an <tt>acis_ha_part_entity_mgr</tt>.  This
 * method must not be overridden by customers.
 */
	acis_ha_part_entity_mgr* acis_ha_part_cast();

	// customers who sub-class from acis_pm_entity_mgr may override this method to
	// return a pointer to this.  Since Spatial will never override this function
	// in a Spatial-supplied sub-class of acis_pm_entity_mgr, customers may then use a
	// non-NULL value returned from customer_cast() to indicate that this is
	// a customer-derived sub-class and cast the pointer to the appropriate value.
/**
 * Dynamic down-cast to a customer sub-type (represented by <tt>void*</tt>).
 * <br><br>
 * <b>Role:</b> The <tt>customer_cast</tt> method is the mechanism for customers to determine whether a pointer to an
 * <tt>acis_pm_entity_mgr</tt> is pointing to a customer-defined subclass of <tt>acis_pm_entity_mgr</tt> or a subclass provided by
 * Spatial.  Customers who derive their own subclass of <tt>acis_pm_entity_mgr</tt> must override this routine to return the
 * <tt>this</tt> pointer of the object.  If this method returns a non-<tt>NULL void*</tt>, then the customer will know that the pointer can
 * safely be down-cast to the customer-defined subclass of <tt>acis_pm_entity_mgr</tt>.  A Spatial-defined subclass of <tt>acis_pm_entity_mgr</tt>
 * will <b>never</b> return a non-<tt>NULL</tt> value from this method.
 */
	void* pm_customer_cast();
};


/** @} */

#endif // ACIS_PM_ENTITY_MGR_CLASS
