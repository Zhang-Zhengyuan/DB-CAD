/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef rough_align_hxx
#define rough_align_hxx

#include "dcl_fct.h"
#include "mmgr.hxx"
#include "api.hxx"

class rough_align_options_impl;

/**
* rough_align_options is used to control the behavior of rough_align_entity_to_points
**/
class DECL_FCT rough_align_options: public ACIS_OBJECT
{
	rough_align_options_impl* _impl;
	rough_align_options(rough_align_options const& other );
	rough_align_options& operator=(rough_align_options const& other );
public:
	rough_align_options();
	~rough_align_options();

	/**
	* If this is set, then rough_align_entity_to_points
	* will automatically apply the best transform it can find to move the body to the points.
	*
	* It is set to true by default.
	**/
	void	set_automatically_transform(logical);

	/**
	* Returns whether the function rough_align_entity automatically transforms the body to
	* the points when called.
	**/
	logical get_automatically_transform();
	
	/**
	* returns best transform of body to align it with points.
	* Must be called after rough_align_entity_to_points
	**/
	SPAtransf get_transform();

	/**
	*@nodoc
	**/
	//internal use only.
	rough_align_options_impl* get_impl();
};

class ENTITY;
class ENTITY_LIST;
class SPAposition;
/**
* Finds a transform to approximately align the input entity to the array of points given.
* It assumes that the points taken are samples over the entire entity.
**/
DECL_FCT outcome rough_align_entity_to_points
		(
			ENTITY* entity_to_align,
			int const num_points,
			SPAposition const* points,
			rough_align_options& options
		);

/**
* Finds a transform to approximately align the input entities to the array of points given.
* It assumes that the points taken are samples over all the entities.
**/
DECL_FCT outcome rough_align_entity_to_points
		(
			ENTITY_LIST const& entities_to_align,
			int const num_points,
			SPAposition const* points,
			rough_align_options& options
		);
#endif
