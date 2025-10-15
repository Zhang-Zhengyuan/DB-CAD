/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( EXTRUDE_OPTIONS_CLASS )
#define EXTRUDE_OPTIONS_CLASS

// Include Files:
#include "acis.hxx"
#include "acis_options.hxx"
#include "dcl_swp.h"
#include "debugmsc.hxx"
#include "logical.h"

/**
 * @file ext_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SWPAPI
 *
 * @{
 */


/**
 * 	Provides a data structure for planar extrusion operations to be used in the function 
 *  @href api_extrude_planar_faces.
 * <br>
 * <b>Role:</b> Provides a data structure for extrusion operations to be used in the function
 * <tt>api_extrude_planar_faces</tt>.
 * The options include:
 * <ul>
 * <li> <tt>draft_angle</tt>: Specifies the draft angle in radians. Default is 0.0. </li>
 * </ul>
 */
class DECL_SWP extrude_options : public ACIS_OBJECT
{
	double  draft_angle;
	logical self_int_test;
	logical self_int_found;

public:

   /**
    * C++ allocation constructor requests memory for this object but does not populate it.
    */
	extrude_options();

   /**
    * C++ destructor, deletes a <tt>extrude_options</tt> object.
    */
	~extrude_options();

   /**
    * Gets the draft angle.
    */
	double get_draft_angle() const;

   /**
	* @nodoc
	* For future use.
    * Gets whether self-intersection checks will be performed.
    */
	logical get_self_int_test() const;

   /**
	* @nodoc
	* For future use.
    * Gets whether a self-intersection was found.
    */
	logical get_self_int_found() const;

   /**
    * Sets the draft angle.
    * <b>Role:</b> Sets the angle (in radians) with which the profile is to draft out (positive) 
	* or in (negative) while extruding. Default is 0.0. 
    * @param angle
    * draft angle.
    */
	void set_draft_angle(double angle);

   /**
	* @nodoc
	* For future use.
    * Sets whether self-intersection checks will be performed.
    * <b>Role:</b> Sets whether self-intersection checks will be performed. 
	* Default is <tt>FALSE</tt>.
    * @param do_check
    * perform check or not.
    */
	void set_self_int_test(logical do_check);

   /**
	* @nodoc
	* For internal use only!
    */
	void set_self_int_found(logical found);

};

/** @} */
#endif

