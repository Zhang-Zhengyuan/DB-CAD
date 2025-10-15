/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ACIS_PM_ENTITY_MGR_FACTORY_CLASS
#define ACIS_PM_ENTITY_MGR_FACTORY_CLASS

#include "dcl_part.h"
#include "entity_mgr_factory.hxx"
#include "vlists.hxx"

class PART;


/**
* @file acis_pm_entity_mgr_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup PARTAPI
 *
 * @{
 */

/**
 * Concrete entity manager factory class for creating <tt>acis_pm_entity_mgr</tt> objects.
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

class DECL_PART acis_pm_entity_mgr_factory : public entity_mgr_factory
{
protected:
	mutable VOID_LIST new_parts;
	PART* get_part_from_list() const;

/**
 * @nodoc
 */
	virtual asm_model_entity_mgr* create_entity_mgr_vf(asm_model_info const & model_info, 
	                                                   HISTORY_STREAM* share_stream = NULL); 

public:
/**
 * Default constructor.
 */
	acis_pm_entity_mgr_factory();
/**
 * Parametrized constructor.
 * <br><br>
 * <b>Role:</b> Constructs an <tt>acis_pm_entity_mgr_factory</tt> object.
 * A list of <tt>PART</tt>
 * objects may be passed in; the factory will check the list for empty parts that
 * are not already bound to an entity manager and re-use those, if any.
 * <br><br>
 * @param seed_parts
 * list of seed parts to check for re-use.
 */
	acis_pm_entity_mgr_factory(const VOID_LIST& seed_parts);


/**
 * Adds parts to the factory's seed parts list.
 * <br><br>
 * @param parts
 * list of seed parts to be added to the factory.
 */
	void seed_parts( const VOID_LIST& parts );

};

/** @} */
#endif // ACIS_PM_ENTITY_MGR_FACTORY_CLASS
