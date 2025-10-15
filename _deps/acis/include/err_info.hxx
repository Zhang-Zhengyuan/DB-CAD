/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( ERROR_INFO_HXX )
#define ERROR_INFO_HXX

#include "dcl_kern.h"
#include "err_info_base.hxx"
#include "entity.hxx"
#include "vlists.hxx"
#include "lists.hxx"

class error_info_list;
class entity_proxy_list;
class SPAstr;
class aux_data_holder;
class i_aux_data_holder;

/**
 * @file err_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

/**
 * Class for storing entity-based ACIS error information.
 * <br>
 * <b>Role:</b> This class is derived from <tt>error_info_base</tt>.  In addition to holding the
 * error number and severity associated with a problem, failure, or error encountered during
 * an operation, instances of <tt>error_info</tt> are designed to hold pointers to an entity or
 * entities involved in the failure.  They can therefore be used by API functions that return entity-based
 * information in their <tt>outcome</tt>s.
 * <br><br>
 * <b>Error System process</b>
 * <br><br>
 * 1. At the start of each API function, a global variable pointer to an <tt>error_info_base</tt> object is set
 * to <tt>NULL</tt>.
 * <br>
 * 2. Before @href sys_error is called, the global pointer is set to contain the relevant
 * <tt>error_info_base</tt> object.
 * <br>
 * 3. At the end of the API function, before the <tt>outcome</tt> object is returned, the global variable is examined,
 * and if nonempty, an <tt>error_info</tt> instance is added to the <tt>outcome</tt>.
 * <br><br>
 * Two overloaded versions of the function <tt>sys_error</tt> set a global pointer to an <tt>error_info</tt>
 * object. One version is passed an <tt>error_info</tt> object, whereas the other creates a <tt>standard_error_info</tt>
 * object when <tt>sys_error</tt> is passed one or two <tt>ENTITY</tt>s. The <tt>standard_error_info</tt> class is
 * derived from <tt>error_info</tt>.
 * <br><br>
 * In the Local Operations, Remove Faces, and Shelling Components, the <tt>error_info</tt> object returns an
 * <tt>ENTITY</tt> that further specifies where the local operation first fails, when such information
 * is available. A <tt>standard_error_info</tt> object is adequate for use in these components, and
 * more detailed information could be returned, if necessary, by deriving a new class.
 *
 * @see error_info, spa_outcome_severity_type, outcome, ENTITY
 */
class DECL_KERN error_info : public error_info_base
{
public:

	/**
	 * C++ default constructor to construct an <tt>error_info</tt> object with no entities in it.
	 */
	error_info();
	error_info(const char * gme);

	/**
	 * C++ constructor to construct an <tt>error_info</tt> object with the 
	 * specified error number, error severity and a list of entities.
	 * <br><br>
	 * @param err_mess
	 * <tt>err_mess_type</tt> indicating the error.
	 * @param err_severity
	 * error severity
	 * @param entities
	 * list of entities
	 */
	error_info(err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					const ENTITY_LIST& entities);
	error_info(const char * gme,err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					const ENTITY_LIST& entities);
	/**
	 * C++ constructor to construct an <tt>error_info</tt> object with 
	 * the specified error number, error severity and a maximum of three entities.
 	 * <br><br>
	 * @param err_mess
	 * <tt>err_mess_type</tt> indicating the error.
	 * @param err_severity
	 * error severity
	 * @param ent1
	 * first entity 
	 * @param ent2
	 * second entity 
	 * @param ent3
	 * third entity 
	 */
	error_info(err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					ENTITY *ent1 = NULL, ENTITY *ent2 = NULL, ENTITY *ent3 = NULL);
	error_info(const char * gme,err_mess_type err_mess, 
					spa_outcome_severity_type err_severity,
					ENTITY *ent1 = NULL, ENTITY *ent2 = NULL, ENTITY *ent3 = NULL);

	/**
	 * @nodoc
	 */
	// This method is for internal use only.
	// Constructor to copy from error_info_base
	error_info(const error_info_base &ei);
	error_info(const char * gme,const error_info_base &ei);

	/**
	 * C++ destructor, deleting an <tt>error_info</tt>.
	 */
	virtual ~error_info();


	/**
	 * Returns the id number for the class <tt>error_info</tt>.
	 */
	static int id();

	/**
	 * Returns the id number for whichever class this object happens to be. If this
	 * object is of the class <tt>error_info</tt>, then the id number returned by this method
	 * would be equal to the one returned by <tt>error_info::id</tt>.
	 */
	virtual int type() const;

	/**
	 * Adds all the alive entities that belong to this <tt>error_info</tt> to the
	 * incoming list of <tt>ENTITY</tt>s. Note that this method does not clear the given 
	 * list of entities.
	 * @param entities
	 * <tt>ENTITY_LIST</tt> to which the alive entities are added.
	 */
	void get_entities_alive(ENTITY_LIST& entities) const;
	void gme_get_entities_alive(ENTITY_LIST& entities) const;

	/**
	 * Queries the auxiliary data provided by this <tt>error_info</tt> object. Returns <tt>NULL</tt> if no 
	 * additional/auxiliary information is available. Note that the object (i.e., the <tt>i_aux_data_holder</tt>) 
	 * returned by this function is owned by the caller and the memory needs to be cleaned up after use.
	 */
	i_aux_data_holder *get_aux_data() const;

	/**
	 * Adds an entity to this <tt>error_info</tt>, if not already there.
	 * @param ent
	 * pointer to an <tt>ENTITY</tt> that is to be added. 
	 * This should not point to a <tt>NULL</tt> or a dead entity.
	 */
	void add_entity(ENTITY *ent);
	void gme_add_entity(ENTITY *ent);

	/**
	 * Adds entities to this <tt>error_info</tt>, if not already there.
	 * <br><br>
	 * Any dead entity in the given list would not get added. 
	 * @param entities
	 * <tt>ENTITY_LIST</tt> to add
	 */
	void add_entities(const ENTITY_LIST& entities);
	void gme_add_entities(const ENTITY_LIST& entities);

	/**
	 * Adds all the reasons of this <tt>error_info</tt> to the given list, <tt>err_reasons</tt>.
	 * In some cases, <tt>error_info</tt> may need to provide some additional information 
	 * about the errors or problems that would have caused the error that is indicated
	 * by this <tt>error_info</tt> object. These additional errors and problems are stored as 'reasons'.
	 * For example, an <tt>error_info</tt> object encapsulating the error 'Operation
	 * failed due to check errors' might include the list of check errors in the
	 * 'reasons' list.
	 * <br><br>
	 * @param err_reasons
	 * <tt>error_info_list</tt> to which the reasons are added
	 */
	void reasons(error_info_list& err_reasons) const;
	void gme_reasons(error_info_list& err_reasons) const;

	/**
	 * @brief 添加reasons.
	 * @param ei
	 */
	void gme_add_reason(error_info* ei);

protected:

	// To remove an entity from this error_info given an entity pointer
	void remove_entity(ENTITY *ent);
	void gme_remove_entity(ENTITY *ent);

	// Remove all the entities from this error_info
	void remove_all_entities();
	void gme_remove_all_entities();

	virtual error_info* error_info_cast_vf();

	// This method is for internal use only
	// Adds a reason to this error_info
	virtual void add_reason_vf(error_info_base* eib);

	// This method is for internal use only
	// Adds a list of reasons
	virtual void add_reasons_vf(const error_info_base_list& eibl);

private:

	VOID_LIST m_ent_ids; // List of tag_id_type
	entity_proxy_list* m_epl;
	aux_data_holder *_holder;
	//@todo 目前认为entity_proxy_list* m_epl的效果与ENTITY_LIST类似，暂时使用ENTITY_LIST进行替代。
	ENTITY_LIST gme_m_entity_list; // 实体列表
	error_info_list *gme_m_reasons; // reasons列表

	// This function is for internal use only.
	// Get a list of all the types of auxiliary data provided by this error info object.
	// Returns the number of types available. Returns zero if no auxiliary data is available, 
	// in that case aux_data_types will be returned NULL.
	// The memory for aux_data_types will be allocated on free-store by this function 
	// so the user is expected to cleanup. 

	unsigned int get_aux_data_types( SPAstr const *&aux_data_types ) const;

	// This function is for internal use only.
	// Queries the auxiliary data provided by this class where the aux_data type should be one of those
	// provided by get_aux_data_types() function. If the given "aux data type" is supported 
	// by this class then the "aux_data" list is filled. Returns zero if either no aux_data filled or 
	// aux_data_type is unknown, otherwise returns the number of entries added to "aux_data" argument. 
	// Note that the entries in aux_data are allocated on heap and the memory needs to be cleaned up
	// by the caller after use (except if the aux_data contains ENTITYs).

	unsigned int query_aux_data( SPAstr const &aux_data_type, VOID_LIST &aux_data ) const;

	// This function is for internal use only.
	// Get a list of all the types of special auxiliary data stored by this error info object.
	// This should include only those data types that the class want to provide - must not include
	// those types provided by the base class. Must be overridden by immediate sub-classes.
	// Returns the number of types available. Returns zero if no property is available, and in that case 
	// aux_data_types will be returned NULL. The memory for aux_data_types will be allocated on free-store 
	// by this function so the user is expected to cleanup. 

	virtual unsigned int get_special_aux_data_types( SPAstr const *&aux_data_types ) const;

	// This function is for internal use only.
	// Queries a "special" auxiliary data, that is specific to the derived class.
	// If the given the aux data type is supported by this class then the property data is filled 
	// in the "aux_data" argument. Returns zero if either no data filled or property is unknown, otherwise
	// returns the number of entries added to "aux_data" argument. Must be overridden
	// by immediate sub-classes

	virtual unsigned int query_special_aux_data( SPAstr const &aux_data_type, VOID_LIST &aux_data ) const;

	// For Internal use only!
	
	friend class entity_error_info_legacy;
	friend class Ierror_info_data_query;
	friend int get_error_info_entity_id_count_internal(const error_info_base*);
};
/**
 * @nodoc
 */
// This function is for internal use only
// Function to return the entity_count of error_info. This is a friend to class error_info_base
int get_error_info_entity_id_count_internal(error_info_base *eib);


/**
 * Gets the count of entity IDs, dead or alive, that are referred to by the given <tt>error_info</tt> object.
 * This is a helper function to determine whether an <tt>error_info</tt> object that is returned by
 * an <tt>outcome</tt> object has any entity information.
 * <br><br>
 * If this function returns a positive value, 
 * then the user may check if the <tt>error_info</tt> object is of 
 * class <tt>error_info</tt> and downcast the pointer to the 
 * <tt>error_info</tt> object to class <tt>error_info_base</tt> in order to 
 * get the entity information. This function helps avoid the 
 * downcasting operation because it tells the user beforehand whether 
 * or not the <tt>error_info</tt> object refers to any entities, dead or alive.  
 * <br><br>
 * @param eib
 * pointer to an <tt>error_info_base</tt> object
 */
DECL_KERN int get_error_info_entity_id_count(const error_info_base *eib);

DECL_KERN error_info* base_to_err_info(error_info_base*& eib);
DECL_KERN error_info* gme_base_to_err_info(error_info_base*& eib);

#define ERROR_INFO_PTR (base_to_err_info(error_info_base_ptr))
#define GME_ERROR_INFO_PTR (gme_base_to_err_info(error_info_base_ptr))
/** @} */
#endif
