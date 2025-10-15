/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef MT_STITCH_HNDL_HXX
#define MT_STITCH_HNDL_HXX

#include "acis.hxx"
#include "dcl_stitch.h"

class ENTITY_LIST;

/**
* @file mt_stitch_hndl.hxx
* \addtogroup STITCHAPI
* \addtogroup STITCH_HANDLES
*
* @{
*/

/**
* \ingroup STITCH_HANDLES
* The class that is a handle to the stitch input. 
* @par Technical Article
* <i>[Stitch Handles](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Stitching_Handles)</i>
* @par Role
* It implements the handle that is the main input of @href api_stitch.
* It is created by @href api_stitch_make_input. It can be also used as input
* for @href api_stitch_manage_coins and @href api_stitch_preview.
* The class is designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted. This is done in
* order to hide implementation. Internally the data is accessed through public 
* inheritance. 
*/
class DECL_STITCH stitch_input_handle
	: public ACIS_OBJECT
{
protected:
	stitch_input_handle();
	stitch_input_handle(const stitch_input_handle&);
	stitch_input_handle& operator=(const stitch_input_handle&);
public:
	virtual ~stitch_input_handle() = 0;
	///\cond
	ENTITY_LIST& get_bodies();
	///\endcond
};

/**
* \ingroup STITCH_HANDLES
* The class is a handle to the stitch output.
* @par Technical Article
* <i>[Stitch Handles](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Stitching_Handles)</i>
* @par Role
* It implements the handle that is the output of @href api_stitch.
* It is further used by @href api_stitch_query. The class is designed as an
* <i>abstract base class</i> that cannot be instantiated, copied or assigned,
* it can only be deleted. This is done in order to hide implementation. 
* Internally the data is accessed through public inheritance. 
*/
class DECL_STITCH stitch_output_handle
	: public ACIS_OBJECT
{
protected:
	stitch_output_handle();
	stitch_output_handle(const stitch_output_handle&);
	stitch_output_handle& operator=(const stitch_output_handle&);
public:
	virtual ~stitch_output_handle() = 0;
	///\cond
	ENTITY_LIST& get_bodies();
	///\endcond
};

/** @} */

#endif // MT_STITCH_HNDL_HXX
