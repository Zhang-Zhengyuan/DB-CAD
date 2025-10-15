/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef POINTS_SEGMENT_HNDL_CLASS
#define POINTS_SEGMENT_HNDL_CLASS

#include "dcl_cstr.h"
#include "mmgr.hxx"
#include "position.hxx"
#include "container_utils.hxx"

/**
 * Specifies the type of geometry to be fitted for a given points_segment_handle .
 * @param UNKNOWN_SEGMENT_TYPE
 * Unknown curve type.
 * @param LINE_TYPE
 * Curve fitted will be Line ( Straight ).
 * @param ARC_TYPE
 * Curve fitted will be Circular arc.
 */

enum POINTS_SEGMENT_TYPE
{
	UNKNOWN_SEGMENT_TYPE = 0,
	LINE_TYPE ,
	ARC_TYPE
};

/**
* @file points_segment_hndl.hxx
* \addtogroup CSTR
* \addtogroup 
* @{
*/

/**
* The class points_segment_handle is a handle to segments of points.
* points_segment_handle is a single node of a linked list.
* It implements the handle that is the main output of @href api_compute_points_segments.
* It implements the handle that is the main input of @href api_make_wire_from_points_segments.
* The class is designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted. 
* Internally the data is accessed through public inheritance. 
* The user takes ownership of instances.
**/


class DECL_CSTR points_segment_handle : public ACIS_OBJECT
{

protected:

	//----------------------------------------------------------------------

	points_segment_handle ();

	points_segment_handle ( const points_segment_handle& );

	points_segment_handle& operator=( const points_segment_handle& );

	//----------------------------------------------------------------------

public:
	/**
	* The desctructor is public, indicating the user takes ownership of instances.
	*/
	virtual ~points_segment_handle () = 0;
	//---------------------------------------------------------------------------------

	/** 
	* Returns next segment of the points_segment_handle linked list
	*/
	virtual points_segment_handle * nextSegment() const = 0;

	//---------------------------------------------------------------------------------
	
	/** 
	* Returns previous segment of the points_segment_handle linked list
	*/
	virtual points_segment_handle * prevSegment() const = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Returns points. Modification on the returned list doesn't modifies segment.	
	*/
	virtual void getPoints ( SPAposition_vector& positionList ) = 0;
	
	//---------------------------------------------------------------------------------
	
	/** 
	* Merge current segment with next segment	
	*/
	virtual void mergeNext () = 0;
	
	//---------------------------------------------------------------------------------
	/**
	* Split current segment at given SPAposition
	*/
	virtual void splitSegment ( SPAposition ) = 0;

	//---------------------------------------------------------------------------------
	/**
	* Split current segment at points index
	*/
	virtual void splitSegment ( int ) = 0;

	//---------------------------------------------------------------------------------
	
	/** 
	* Set type of the segment 
	*/
	virtual void setType ( POINTS_SEGMENT_TYPE type) = 0;

	//---------------------------------------------------------------------------------
	
	/** 
	* Get type of the segment 
	*/
	virtual POINTS_SEGMENT_TYPE  getType () = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Get radius of ARC_TYPE segment 
	*/
	virtual double  getArcRadius () = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Get center of ARC_TYPE segment 
	*/
	virtual SPAposition  getArcCenter () = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Get start position of segment 
	*/
	virtual SPAposition getStartPosition () = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Get end position of segment 
	*/
	virtual SPAposition getEndPosition () = 0;

	//---------------------------------------------------------------------------------

	/** 
	* Get number of points in segment
	*/
	virtual int getNumberOfPoints () = 0;

	//---------------------------------------------------------------------------------

	/**
	 * Make a points segment handle.
	 * @param[in] positionList	: List of positions.
	 * @param[in] type			: POINTS_SEGMENT_TYPE.
	 * @param[in] head			: A pointer to head of points_segment_handle list. 
	 */
	static points_segment_handle* make ( SPAposition_vector& positionList , POINTS_SEGMENT_TYPE type, points_segment_handle* head);
	
	//---------------------------------------------------------------------------------
};


//---------------------------------------------------------------------------------

/**
* Helper function to delete entire points_segment_handle linked list 
* @param[in] head	: A pointer to head of points_segment_handle list.
*/
DECL_CSTR void delete_points_segment_list ( points_segment_handle * head);

//---------------------------------------------------------------------------------

/** @} */

#endif  // POINTS_SEGMENT_HNDL_CLASS



