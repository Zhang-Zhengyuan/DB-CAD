/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef OFST_EDGES_HNDL_H
#define OFST_EDGES_HNDL_H

#include "acis.hxx"
#include "dcl_hlc.h"

class ENTITY_LIST;
class ENTITY;

/**
* @file ofst_edges_hndl.h
* \addtogroup HLC
* \addtogroup
* @{
*/

/**
* Class <tt>ofst_edges_input_handle</tt> controls the user
* Input Handle for api_offset_edges_on_faces. The main
* adjustable parameters are (1) Edge List (List of Edges ,which are to be offsetted)
* (2) Face List array (Faces which are to be considered for offsetting each given edge)
* The class is designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted.
* Internally the data is accessed through public inheritance.
* The user takes ownership of instances.
**/

class DECL_HLC ofst_edges_input_handle
	: public ACIS_OBJECT
{
protected:
	
	//----------------------------------------------------------------------

	ofst_edges_input_handle();
	
	ofst_edges_input_handle(const ofst_edges_input_handle&);
	
	ofst_edges_input_handle& operator=(const ofst_edges_input_handle&);

	//----------------------------------------------------------------------
public:
	/**
	* The desctructor is public, indicating the user takes ownership of instances.
	*/
	virtual ~ofst_edges_input_handle() = 0;

	/**
	* getter function to return list of edges , considered for offsetting
	**/
	virtual ENTITY_LIST& get_ofst_edges() = 0;
	/**
	* getter function to return list of faces , considered for offsetting
	**/

	virtual ENTITY_LIST& get_ofst_faces() = 0;

	/**
	 * Make a ofst edges input handle.
	 * @param[in] ent_list1	: list of edges
	 * @param[in] ent_list2	: array of faces.
	 */
	static ofst_edges_input_handle* make(ENTITY_LIST& edgeList,ENTITY**& faceList);
	
};


/**
* Helper function to delete input handle created for offset_edges_on_faces 
* @param[in] inpHndl	: input handle for offset_edges_on_faces
*/
DECL_HLC void delete_offset_on_faces_input(ofst_edges_input_handle * inpHndl);

//---------------------------------------------------------------------------------

/** @} */

#endif 
