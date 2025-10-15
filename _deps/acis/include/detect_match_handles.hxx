/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Detect Match Handles - public data
/*******************************************************************/

#ifndef DETECT_MATCH_HANDLES_HXX
#define DETECT_MATCH_HANDLES_HXX
#include "api.hxx"
#include "dcl_query.h"
/**
 * \addtogroup DETECTFEATURE
 * \brief Declared at <detect_match_handles.hxx>
 * @{

 */

/**
 * Provides ability to communicate the input to the detect_match_body_query class.
 * <br>
 * <b>Role:</b> This is a handle class that encapsulates the input provided
 * to make the matching query
 */
class DECL_QUERY detect_match_input_handle : public ACIS_OBJECT
{
public:
	MMGR_FREELIST_THIS
	virtual ~detect_match_input_handle() = 0;
protected:
	detect_match_input_handle();
private:
	detect_match_input_handle(const detect_match_input_handle&);
	detect_match_input_handle& operator=(const detect_match_input_handle&);
};

/**
 * Provides ability to communicate the intermediate state of the input elements to the query function.
 * <br>
 * <b>Role:</b> This is a handle class that can be used to make multiple queries
 * based on the same input elements to find the matches for different seed values
 */
class DECL_QUERY detect_match_output_handle : public ACIS_OBJECT
{
public:
	MMGR_FREELIST_THIS
	/**
	 * Provides the ability to release the resources held by detect_match_output_handle.
	 * <br>
	 * <b>Role:</b> Destructor of detect_match_output_handle class - releases the resources held for making the matching calls.
	 */
	virtual ~detect_match_output_handle();
protected:
	detect_match_output_handle();
};
/** @} */
#endif
