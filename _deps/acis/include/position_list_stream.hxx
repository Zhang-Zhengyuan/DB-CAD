/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef POSITION_LIST_STREAM_HXX
#define POSITION_LIST_STREAM_HXX

#include "list_stream.hxx"
#include "position.hxx"
#include "logical.h"

/**
 * @file position_list_stream.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *
 *
 */
/**
 * \addtogroup ACISLISTS
 *
 * @{
 */
/**
* Container class derived from list_stream for SPAposition objects.
* <br>
* <b>Role:</b> This is a container for SPAposition objects that is based
* on the list_stream capabilities. This class inherits all the base
* list_stream functionality, such as adding, removing, and iterating the
* elements of the list. See the list_stream documentation for more details.
**/
class DECL_BASE position_list_stream : public list_stream<SPAposition> 
{

	friend logical save_position_list(void*);

public:

	/**
	* @nodoc  un-hide the add methods of the base class
	**/
	using list_stream<SPAposition>::add;
	
	/**
	* Add a SPAposition at specified coordinates to the list.
	* <br>
	* @param x
	* x coordinate
	* @param y
	* y coordinate
	* @param z
	* z coordinate
	**/
	int add(double x, double y, double z);
};

/** @} */

#endif //POSITION_LIST_STREAM_HXX
