/*******************************************************************/
/*    Copyright (c) 1989-2021 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MID_SHEET_BODY_OPTIONS
#define MID_SHEET_BODY_OPTIONS

#include "mmgr.hxx"
#include "dcl_shl.h"

class mid_sheet_body_options_impl;
class SPAbox;

/**
 * @file mid_sheet_body_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup SHLAPI Shelling
 *
 * \ingroup LOCALOPS
 * @{
 */

 /**
  * This class contains parameters that govern the behavior of @href api_make_mid_sheet_body.
  * <br><br>
  * <b>Technical Articles:</b> <i>[Mid Sheet Body](https://doc.spatial.com/articles/m/i/d/Mid_Sheet_Generation_90eb.html)</i>
  * <br><br>
  * <b>Role:</b> Below are lists of parameters available in the @href mid_sheet_body_options class.
  * The constructor of @href mid_sheet_body_options initializes all the parameters to their default value.
  * You can use the <tt>set</tt> functions to change any of the parameters and the <tt>get</tt> functions
  * to obtain the current value of the parameter set into a @href mid_sheet_body_options object. <br>
  * <ul>
  * <li> <b>box</b><br>
  * low/high diagonal corner of the box to be used as an intersection limit.</li><br>
  * </ul>
  * <br><br>
  *
  * @see api_make_mid_sheet_body
  *
 **/
class DECL_SHL mid_sheet_body_options : public ACIS_OBJECT
{
	mid_sheet_body_options_impl* _mid_sheet_body_opts_impl;

public:

	/**
	*  Constructs an <tt>mid_sheet_body_options</tt> object.
	**/
	mid_sheet_body_options();

	/**
	* Destructs an <tt>mid_sheet_body_options</tt> object.
	**/
	~mid_sheet_body_options();

	/**
	* @nodoc
	* Copy Constructor of <tt>mid_sheet_body_options</tt>.
	**/
	mid_sheet_body_options(const mid_sheet_body_options& mid_sheet_body_opts);

	/**
	* @nodoc
	* Assignment Operator of <tt>mid_sheet_body_options</tt>.
	**/
	const mid_sheet_body_options& operator=(const mid_sheet_body_options&);

public:

	/**
	*  Return the box of the operation.
	**/
	const SPAbox& get_box()const;

	/**
	*  Set box for operation.
	**/
	void set_box(const SPAbox& box);

	/**
	* @nodoc
	*  Return max pair thickness
	**/
	double get_max_pair_thickness()const;

	/**
	* @nodoc
	*  maximum pair thickness value used pair detection.
	*  for default value = -1.0, pair detected automatically otherwise max_pair_thickness used for detection.
	**/
	void set_max_pair_thickness(double max_pair_thickness = -1.0);
};
/*! @} */
#endif

