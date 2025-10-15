/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Detect Match Query - public data
/*******************************************************************/

#ifndef DETECT_MATCH_QUERIES_HXX
#define DETECT_MATCH_QUERIES_HXX

/**
 * \addtogroup DETECTFEATURE
 * \brief Declared at <detect_match_queries.hxx>
 * @{

 */


/**
 * Provides ability to query for matches between input seed and candidates list passed to input handle, 
 * based on a matching criteria specified in detect_match_body_criteria.
 * <br>
 * <b>Role:</b> This is a query-interface class that performs matches between seed and candidates.
 * get_matching_bodies is the interface which can be used to query for matches based on the 
 * matching criteria specified in detect_match_body_criteria
 */

class DECL_QUERY detect_match_body_query
{
public: 
	/**
	* Constructor takes an detect_match_output_handle* and creates the query class.
	*/
	detect_match_body_query( const detect_match_output_handle * dmoh);

     
	virtual ~detect_match_body_query();

	/**
	* Provides the ability to query for matching bodies based on the seed and body matching criteria.
	*/
	ENTITY_LIST get_matching_bodies(BODY* seed, const detect_match_body_criteria *dmbo = NULL) const;

private:
	const detect_match_output_handle *_dmoh;
};

/** @} */
#endif
