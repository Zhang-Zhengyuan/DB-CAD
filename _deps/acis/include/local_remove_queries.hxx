/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Header for queries on local_remove_output_handle.
/*******************************************************************/

#if !defined( LOCAL_REM_QUERIES )
#define LOCAL_REM_QUERIES

#include "api.hxx"
#include "dcl_rem.h"

/**
* @file local_remove_queries.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \defgroup REMAPI Remove Faces
*
* \ingroup LOCALOPS
* @{
*/

class local_remove_output_handle;

/**
* This class allows user to query the output of a local remove operation. It can fetch
* the newly created entities during the remove operation. A remove tvertex operation
* can lead to creation of one or more precise VERTEXs and EDGEs in the region of the
* TVERTEX getting removed. 
*/

class DECL_REM local_remove_output_query
{
public: 
	/**
	* Constructor to create an object of this class given a local_remove_output_handle.
	*/
	local_remove_output_query( const local_remove_output_handle * roh);

	virtual ~local_remove_output_query();

	/**
	* Returns the newly created ENTITYs, that the VERTEXs and EDGEs (if any) from the local remove operation.
	*/

	ENTITY_LIST get_new_entities() const;

private:

	// For internal use only!

	const local_remove_output_handle *_roh;
};

#endif	//LOCAL_REM_QUERIES
