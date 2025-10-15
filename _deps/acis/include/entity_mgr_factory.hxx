/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ENTITY_MGR_FACTORY_CLASS
#define ENTITY_MGR_FACTORY_CLASS

#include "dcl_asm.h"
#include "base.hxx"
#include "logical.h"
#include "mmgr.hxx"

class asm_model;
class ENTITY_LIST;
class HISTORY_STREAM;
class asm_model_list;
class ASM_MODEL_REF;
class asm_model_entity_mgr;
class asm_model_info;

/**
* @file entity_mgr_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * Abstract class from which customers derive factory objects to create entity manager sub-class objects.
 * <br><br>
 * <b>Role:</b> Interface class to provide a factory method for creating customer entity managers.
 * <br><br>
 * An <tt>entity_mgr_factory</tt> object is used by <tt>asmi_restore_model_list</tt> to create a customer-defined 
 * entity manager object for each model in the ASAT file being read.  Customers who derive their own
 * entity manager sub-class should derive a corresponding subclass of <tt>entity_mgr_factory</tt>.
 * <br>
 * @see asm_model_entity_mgr
 */

class DECL_ASM entity_mgr_factory : public ACIS_OBJECT
{
protected:
	virtual ~entity_mgr_factory();

/**
 * Creates a new entity manager using <tt>ACIS_NEW</tt>.
 * <br><br>
 * <b>Role:</b> Subclasses <b>must</b> override this function.
 * <br><br>
 * This routine is called by <tt>asmi_restore_model list</tt>, once for each model, to create an entity manager for
 * each model.
 * <br><br>
 * @param model
 * model to which the new entity manager should be bound.  This model should not already be bound to an entity manager.
 * @param share_stream
 * if non-NULL, the new entity manager should be bound to this history stream.  Care should be taken that this
 * stream is the stream to which the entities in <tt>ents</tt> belong.
 */
	virtual asm_model_entity_mgr* create_entity_mgr_vf(asm_model_info const & model_info, 
	                                                   HISTORY_STREAM* share_stream)=0; 

/**
 * Requests that this entity manager factory destruct itself.  
 * <br><br>
 * <b>Role:</b> This function is necessary so that customer code can match allocation and deletion strategies
 * A default implementation that calls <tt>ACIS_DELETE</tt> on the <tt>this</tt> pointer is provided.
 */
	virtual void lose_vf();

public:

	asm_model_entity_mgr* create_entity_mgr(asm_model_info const & model_info, 
	                                        HISTORY_STREAM* share_stream = NULL); 

/**
 * Requests that this entity manager factory destruct itself.
 */
	void lose();
};

/**
 * Concrete entity manager factory class for creating <tt>default_entity_mgr</tt> objects.
 * <br><br>
 * <b>Role:</b> The default entity manager (and its corresponding factory) are provided for 
 * customers who do not need a full-blown entity manager integrated with their part management
 * system.  A default entity manager is essentially a wrapper for an entity list containing the
 * managed entities.  Please refer to the assembly modeling technical articles for further details.
 * <br>
 * @see default_entity_mgr
 */
class DECL_ASM default_entity_mgr_factory :public entity_mgr_factory
{
protected:

/**
 * @nodoc
 */
	virtual asm_model_entity_mgr* create_entity_mgr_vf(asm_model_info const & model_info, 
										  HISTORY_STREAM* share_stream = NULL); 

public:

/**
 * Default constructor
 */
	default_entity_mgr_factory();

/**
 * The factory will create a new history stream and pass it to each new manager if <tt>TRUE</tt>.
 */
	logical create_mgr_history;
/**
 * The manager will delete its history stream during destruction if <tt>TRUE</tt>.
 * <br><br>
 * <b>Role:</b> In general, should only be set to <tt>TRUE</tt> if <tt>create_mgr_history</tt> is also <tt>TRUE</tt>.
 */
	logical mgr_owns_history;
/**
 * The manager will delete all top-level entities (<b>except</b> <tt>ASM_ASSEMBLY</tt>) during destruction if <tt>TRUE</tt>.
 */
	logical mgr_owns_ents;

	logical mgr_prune_on_clear;
};

 /** @} */
#endif // ENTITY_MGR_FACTORY_CLASS
