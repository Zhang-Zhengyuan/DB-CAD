/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SEGMENTATION_OPTIONS_CLASS
#define SEGMENTATION_OPTIONS_CLASS

#include "dcl_cstr.h"
#include "mmgr.hxx"

class segmentation_options_impl;

/**
* @file seg_options.hxx
* \addtogroup CSTR
* \addtogroup
* @{
*/

/**
 * Specifies that @href api_compute_points_segments and @href api_make_wire_from_points_segments should use segmentation_options.
 * <br>
 * <b>Role:</b> An object of type <tt> segmentation_options </tt> is passed as
 * an argument to @href api_compute_points_segments and @href api_make_wire_from_points_segments
 * This option controls the segmentation of points.
 * 
**/


class DECL_CSTR segmentation_options : public ACIS_OBJECT
{

protected:

	segmentation_options_impl* m_impl;
	
	segmentation_options ( segmentation_options const& input );
	
	segmentation_options& operator= ( segmentation_options const& input );

public:
	// ********************************************************************
	//Constructor
	segmentation_options ();

	//Destructor
	~segmentation_options ();
	
	// ********************************************************************
	/** 
	*  Set Maximum desired angle between segments.
	*/
	void setSegAngle ( const double angle );

	/**
	* Get Maximum desired angle between segments.
	*/
	double getSegAngle () const;

	// ********************************************************************
	/**
	* Set Maximum desired tolerance for the check of collinearity.
	*/
	void setPositionTol ( const double tolerance );

	/**
	* Get Maximum desired tolerance for the check of collinearity.
	*/
	double getPositionTol () const;

	// ********************************************************************
	/** 
	*  Set Minimum angle of an arc segment.
	*/
	void setMinArcAngle ( const double angle );

	/**
	*  Get Minimum angle of an arc segment.
	*/
	double getMinArcAngle () const;

	// ********************************************************************
	/**
	* Set Minimum acceptable radius of a circular arc.
	*/
	void setMinRadius ( const double radius );
	/**
	* Get Minimum acceptable radius of a circular arc.
	*/
	double getMinRadius () const;

	// ********************************************************************
	/** 
	* Set Maximum acceptable radius of a circular arc.
	*/
	void setMaxRadius ( const double radius );
	/**
	* Get Maximum acceptable radius of a circular arc.
	*/
	double getMaxRadius () const;

	// ********************************************************************

};

#endif
