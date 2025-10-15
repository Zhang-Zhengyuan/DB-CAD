/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef UNSTCHNM_SUPPORT_HNDL_CLASS
#define UNSTCHNM_SUPPORT_HNDL_CLASS

#include "dcl_bool.h"
#include "mmgr.hxx"
#include "position.hxx"
#include "container_utils.hxx"

#include "entity.hxx"
#include "lists.hxx"
#include <map>
using namespace std;

/*
* The class unstchnm_support_handle is a handle contains iterator of map returned from api_unstitch_nonmani.
* Map contains the data between the deleted non-manifold entities to the newly created manifold entities.
* The class is designed as an <i>abstract base class< / i> that cannot be
* instantiated, copied or assigned, it can only be deleted.
* Internally the data is accessed through public inheritance.
* The user takes ownership of deletion of instance. */


class DECL_BOOL unstchnm_support_handle : public ACIS_OBJECT
{

protected:
	  
	//----------------------------------------------------------------------

	unstchnm_support_handle() {}

	unstchnm_support_handle(const unstchnm_support_handle&) {}

	//----------------------------------------------------------------------

public:
	/**
	* The desctructor is public, indicating the user takes ownership of instances.
	*/
	virtual ~unstchnm_support_handle() = 0;

	//---------------------------------------------------------------------------------

	virtual map<ENTITY*, ENTITY_LIST>::iterator getMapBegin()  = 0;

	virtual map<ENTITY*, ENTITY_LIST>::iterator getMapEnd() = 0;

	virtual ENTITY_LIST & at(ENTITY * first) = 0;

	virtual map<ENTITY*, ENTITY_LIST>::iterator find(ENTITY * first) = 0;

	//---------------------------------------------------------------------------------
};

#endif  // UNSTCHNM_SUPPORT_HNDL_CLASS



