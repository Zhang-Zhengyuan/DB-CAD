/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef REM_ERROR_HXX
#define REM_ERROR_HXX
#include "dcl_rem.h"
#include "errorsys.hxx"
#include "err_info.hxx"
#include "lists.hxx"
/**
 * @file rem_error.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup REMAPI
 *  @{
 */
/**
 * Error entities from a remove face operation.
 */
class DECL_REM rem_error_info : public error_info 
{
	/**
	 * List of bad entities
	 */
	ENTITY_LIST _ents;	
public:
   /**
	* Constructor
	* @param ents
	*/
	rem_error_info( ENTITY_LIST &ents );
	/**
	 * Destructor.
	 */
	~rem_error_info();
	/**
	 * Get error entities
	 */
	ENTITY_LIST& 	ents()	{ return _ents; }

	/**
	 * Add entities to list of bad entities.
	 * @param ents 
	 * Entities to include.
	 */
	void add_ents(ENTITY_LIST &ents) { _ents.add(ents); }
	/**
	 * Returns error ID
	 */
	static 	int	id();
	/**
	 * Returns ID number for this object.
	 */
	virtual	int	type()	const;
};
/*! @} */

#endif
