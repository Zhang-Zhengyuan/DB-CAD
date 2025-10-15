/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Detect Blend Options
/*******************************************************************/

#ifndef DETECT_FEATURE_OPTS_HXX
#define DETECT_FEATURE_OPTS_HXX

/*******************************************************************/
// Include files
#include "dcl_blnd.h"	// DECL_BLND
#include "acis.hxx"
#include "acis_options.hxx"
#include <ostream>
/*******************************************************************/

/**
 * \defgroup DETECTFEATURE Detect Feature
 */

/**
 * @file detect_feature_opts.hxx
 */

/** 
 * \addtogroup DETECTFEATURE
 * \brief Declared at <detect_feature_opts.hxx>
 * @{
 */

/**
 * @nodoc
 */
class detect_blend_options_data;

/**
 * Options for blend detection.
 * <br>
 * <b>Role:</b> This class contains information to be used to control
 * the behaviour of blend feature detection involved in api_detect_blends, api_detect_protrusion, api_detect_depression.
 * <br>
 * <table border=0 cellspacing=0 cellpadding=8 >
 * <tr>
 * <td valign = "top"><tt>rad_compare_allowance</tt></td>
 * <td valign = "top"> Radius comparison allowance. This is used in comparison of radii of adjacent blends 
 *  to qualify them to be part of the same constant radius blend sequence.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>rad_cvtr_allowance</tt></td>
 * <td valign = "top"> Curvature radius allowance. Not implemented yet, for future use.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>add_vblends_in_seq</tt></td>
 * <td valign = "top"> Add vertex blends in sequence. Flag to decide whether vertex blend faces
 *  are to be included in blend sequence or not. By default, this is set to true.</td>
 * </tr>
 * </table>
*/

class DECL_BLND detect_blend_options : public ACIS_OBJECT
{

public: 
	/**
	* C++ allocation constructor requests memory for this object but does not populate it.
	*/
	detect_blend_options(double rad_compare_allowance = -1.0, 
						 double rad_cvtr_allowance = -1.0, 
						 bool add_vblends_in_seq = true);

	/**
	* C++ default destructor.
	*/
	virtual ~detect_blend_options();

	/**
	* Set the radius comparison allowance to be used during construction of a blend sequence.
	* @param tol
	* tolerance 
	*/
	void set_radius_comparison_allowance(double tol);

	/**
	* Set the blend's radius of curvature allowance. This functionality is for future use.
	* @param tol
	* tolerance 
	*/
	void set_radius_cvtr_allowance(double tol);

	void output(std::ostream& os) const;

	///\cond
	static double get_default_radius_allowance();
	static double get_default_radius_cvtr_allowance();
	///\endcond

private : 

	detect_blend_options_data* _data;

	friend class detect_blend_options_data;

};


/**
* @nodoc
*/
class detect_chamfer_options_data;

/**
* Options for chamfer detection.
* <br>
* <b>Role:</b> This class contains information to be used to control
* the behaviour of chamfer feature detection involved in api_detect_chamfers.
* <br>
* <table border=0 cellspacing=0 cellpadding=8 >
* <tr>
* <td valign = "top"><tt>supports_max_angle</tt></td>
* <td valign = "top">Specifies the maximum angle (in degrees) between chamfer supports.
* The default value is 105.</td>
* </tr>
* </table>
*/

class DECL_BLND detect_chamfer_options : public ACIS_OBJECT
{

public:

	/**
	* C++ allocation constructor requests memory for this object but does not populate it.
	*/
	detect_chamfer_options(double supports_max_angle = 105.0);

	/**
	* C++ default destructor.
	*/
	virtual ~detect_chamfer_options();

	/**
	* Sets the value of the <tt>supports_max_angle</tt> parameter to input value in degrees.
	* Can be called only before chamfer recognition.
	* @param angle
	* new value.
	**/
	void set_supports_max_angle(double angle);

	void output(std::ostream& os) const;

	///\cond
	static double get_default_supports_max_angle();
	///\endcond

private:

	detect_chamfer_options_data* _data;

	friend class detect_chamfer_options_data;

};


/**
 * @nodoc
 */
class detect_feature_options_data;


/**
 * Options for feature detection and blend detection. 
 * <br>
 * <b>Role:</b> This class contains information to be used to control
 * the behaviour of feature detection involved in api_detect_blends, api_detect_protrusion, api_detect_depression.
 *
 * <br><br>
 * <table border=0 cellspacing=0 cellpadding=8 >
   * <tr>
 * <td valign = "top"><tt>add_periphery_blends</tt></td>
 * <td valign = "top"> Add periphery blends. Flag to control whether the blends situated at boundary
 * of depression or protrusion feature are to be included in the list of feature faces.
 * The default value of this option is TRUE, hence periphery blends are included in the return list.
 * </td> 
 * </tr>
 * <tr>
 * <td valign = "top"><tt>smoothness_allowance</tt></td>
 * <td valign = "top"> Edge smoothness allowance. This is used for determining smoothness of an edge of a feature-face. 
 * The value of this allowance decides the threshold of the sine value of the angle between the normals
 * of the faces shared by a smooth edge. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>detect_blend_options</tt></td>
 * <td valign = "top"> Options to control the detection of blends.</td> 
 * </tr>
 * <tr>
 * <td valign = "top"><tt>detect_chamfer_options</tt></td>
 * <td valign = "top"> Options to control the detection of chamfers.</td>
 * </tr>
 * </table>
**/

class DECL_BLND detect_feature_options : public AcisOptions 
{

public: 
	/**
	* C++ allocation constructor requests memory for this object but does not populate it.
	* When add_periphery_blends is set true, detect_blend_options object is constructed internally.
	* 
	*/
	detect_feature_options( double smoothness_allowance = -1.0, 
							bool add_periphery_blends = true);


	/*
	* C++ allocation constructor requests memory for this object but does not populate it.
	*/
	detect_feature_options( detect_blend_options const* db_opts,
							double smoothness_allowance = -1.0 );

	/*
	* C++ allocation constructor requests memory for this object but does not populate it.
	*/
	detect_feature_options(detect_chamfer_options const* db_opts,
		double smoothness_allowance = -1.0);


	/**
	* C++ default destructor.
	*/
	virtual ~detect_feature_options();

	/**
	* This method returns handle of <tt>detect_blend_options</tt> only when add_periphery_blends is set to true. 
	* Otherwise, the return value is NULL. Fetching detect_blend_options allows user to specify the periphery blend face properties.
	*/
	detect_blend_options* detect_blend_opts() const;

	/**
	* This method returns handle of <tt>detect_chamfer_options</tt> 
	*/
	detect_chamfer_options* detect_chamfer_opts() const;

	/**
	* This method facilitates accommodation of existing AcisOptions if any, in the detect feature options.
	*/
	void set_acis_options(AcisOptions * ao);

	void output(std::ostream& os) const;

	///\cond
	static double get_default_smoothness_allowance();
	///\endcond

private:

	detect_feature_options_data *_data;
	
	friend class detect_feature_options_data ;

};


/** @} */
#endif  // DETECT_FEATURE_OPTS_HXX
