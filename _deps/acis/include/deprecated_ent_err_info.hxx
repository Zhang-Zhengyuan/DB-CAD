/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( ENT_ERROR_INFO_HXX )
#define ENT_ERROR_INFO_HXX

#include "dcl_kern.h"
#include "err_info.hxx"
#include "entity.hxx"
#include "vlists.hxx"

/**
 * @file ent_err_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

/**
 * @deprecated R20.  Use error_info instead.
 * <br><br>
 * Class for storing entity based ACIS error information.
 * <br>
 * <b>Role:</b> This class is derived from <tt>error_info</tt> and additionally stores entity information.
 * Objects of this class are used to return error information along with the associated entities.
 * <tt>entity_error_info_legacy</tt> is specifically designed to hold entity information
 * in addition to error number and severity that is represented by <tt>error_info</tt>.
 * This class can be used by those APIs that want to return entity-based error information in
 * their <tt>outcome</tt>.
 *
 * @see error_info, spa_outcome_severity_type, outcome, ENTITY
 */
class DECL_KERN entity_error_info_legacy : public error_info
{
protected:

	// To remove an entity from this entity_error_info_legacy given an entity tag
	void remove_entity_id(tag_id_type ent_id);

public:

	/**
	 * C++ default constructor to construct an <tt>entity_error_info_legacy</tt> object with no entities in it.
	 */
	entity_error_info_legacy();

	/**
	 * C++ constructor to construct an <tt>entity_error_info_legacy</tt> object with the 
	 * specified error number, error severity and a list of entities.
	 * <br><br>
	 * @param err_mess
	 * err_mess_type indicating the error.
	 * @param err_severity
	 * error severity
	 * @param entities
	 * list of entities
	 */
	entity_error_info_legacy(err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					const ENTITY_LIST& entities);
	/**
	 * C++ constructor to construct an <tt>entity_error_info_legacy</tt> object with 
	 * the specified error number, error severity and a maximum of three entities.
 	 * <br><br>
	 * @param err_mess
	 * err_mess_type indicating the error.
	 * @param err_severity
	 * error severity
	 * @param ent1
	 * first entity 
	 * @param ent2
	 * second entity 
	 * @param ent3
	 * third entity 
	 */
	entity_error_info_legacy(err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					ENTITY *ent1 = NULL, ENTITY *ent2 = NULL, ENTITY *ent3 = NULL);


	/**
	 * @nodoc
	 */
	// This method is for internal use only.
	// Constructor to copy from error_info_base
	entity_error_info_legacy(const error_info_base &ei);

	/**
	 * Returns the id number for the class <tt>entity_error_info_legacy</tt>.
	 */
	static int id();

	/**
	 * Returns the id number for whichever class this object happens to be. If this
	 * object is of the class <tt>entity_error_info_legacy</tt>, then the id number returned by this function
	 * would be equal to the one returned by <tt>entity_error_info_legacy::id()</tt>.
	 */
	virtual int type() const;

	/**
	 * Returns the number of entity ids, both alive and dead, that are referred 
	 * to by this <tt>entity_error_info_legacy</tt>. Each entity id can be 
	 * obtained by calling <tt>entity_error_info_legacy::get_entity_id()</tt>.
	 */
	virtual int get_entity_id_count() const;

	/**
	 * Returns the unique <tt>tag_id_type</tt> (entity id) of the ENTITY at the given index. 
	 * The indexing is zero-based and the maximum index that is valid is one less than the value
	 * returned by <tt>entity_error_info_legacy::get_entity_id_count()</tt>. The pointer to the
	 * entity identified by the returned <tt>tag_id_type</tt> can be obtained by calling
	 * the API <tt>api_get_entity_from_id()</tt>. Note that the entity that is identified by the
	 * returned entity id need not be alive, in such a case a call to the API 
	 * <tt>api_get_entity_from_id()</tt> with that id would return a <tt>NULL</tt> entity.
	 * @param index
	 * Index at which the entity-id is returned.
	 */
	tag_id_type get_entity_id(int index) const;
};

/** @} */

typedef error_info entity_error_info;

#endif
