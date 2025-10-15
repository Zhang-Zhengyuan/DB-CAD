/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for transform_entity_options object.
/*******************************************************************/

#ifndef transform_entity_options_hxx
#define transform_entity_options_hxx

class transform_entity_options_impl;

#include "dcl_kern.h"
#include "mmgr.hxx"
#include "SPAuse_counted_impl_holder.h"

/**
* This options class controls the modifiable behavior of <tt>api_transform_entity</tt>. 
* In particular, an instance of this options class controls whether or not geometric 
* simplification is performed after performing a transformation containing a shear 
* component. The default behavior is to perform geometric simplification.
**/

class DECL_KERN transform_entity_options
{
	SPAuse_counted_impl_holder m_holder;
public:
	/**
	* Constructor.  Makes object and initializes with default values.
	**/
	transform_entity_options();

	/**
	* Allows user to specify whether geometry resulting after the transformation will be simplified.
	* This comes in to play with nonuniform scaling.  The default value is TRUE.
	*
	* Doing simplification can have a performance impact (causing the API to take longer),
	* but also may result in models being better to work with downstream.
	**/
	void set_do_simplify( logical setval );

	/**
	* Returns the value of the do simplify flag;
	**/
	logical get_do_simplify() const;

	/**
	* @nodoc. internal use only.
	**/
	transform_entity_options_impl* get_impl();

	MMGR_FREELIST_THIS;
};

#endif
