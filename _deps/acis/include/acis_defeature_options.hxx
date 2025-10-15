/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _ACIS_DEFEATURE_OPTIONS_HXX
#define _ACIS_DEFEATURE_OPTIONS_HXX

#include "dcl_defeature.h"

// Acis Headers
#include "kernapi.hxx"
#include "lists.hxx"
#include "api.err"
#include "check.hxx"

#include "acis_defeature_api.hxx"
/**
 * @file acis_defeature_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DFTAPI
 *
 * @{
 */

class FRSFeatureData;


/**
 * Enumerates the various defeaturing feature types.
 * <br>
 * <b>Role:</b> This enumeration affects various features held by the defeature_options object.
 * The defeature_options object uses these features to communicate the type of feature to which a list of faces belongs.
 * This values of this enumeration are:
 * <ul>
 * <li><b><tt>defeature_type_unknown</tt></b>: The type of feature is unknown.</li>
 * <li><b><tt>defeature_type_blend</tt></b>: The feature is a blend.</li>
 * <li><b><tt>defeature_type_chamfer</tt></b>: The feature is a chamfer.</li>
 * <li><b><tt>defeature_type_hole</tt></b>: The feature is a hole.</li>
 * </ul>
 * @see defeature_options
 **/
enum defeature_feature_type 
{
	defeature_type_unknown, 
	defeature_type_blend,
	defeature_type_chamfer,
	defeature_type_hole
};

/**
 * Options for local operations.
 * <br>
 * <b>Role:</b> The defeature_options object fulfills three roles:
 * <ul>
 * <li>It manages the settings for parameters that control whether a particular shape is recognized as a feature
 *	(for example, the maximum diameter of holes) and provides reasonable default values.</li>
 * <li>It controls whether @href api_find_and_remove_features will perform feature recognition, feature removal, or both.</li>
 * <li>It provides an interface for you to manage and view the features that are to be removed.
 *  It provides methods to obtain the number of features recognized and obtain a list of faces corresponding to each feature.
 *  It also allows you to specify features that will not be removed by calling the @href keep_face method on any face in the 
 *  feature.  Finally it allows you to query the number and faces corresponding to features which were marked for removal
 *  but not removed (through the @href get_num_failed_features and @href get_failed_feature methods).</li>
 * </ul>
 *	In general, the parameters managed by the options object are managed through <tt>get_*</tt> and <tt>set_*</tt> methods.  
 *  For example, the <tt>hole_max_diameter</tt> parameter is controlled by the methods @href get_hole_max_diameter 
 *  and @href set_hole_max_diameter. Calling a <tt>set_*</tt> method without any arguments resets the parameter to its 
 *  default value.
 * <br>
 *  The parameters of the defeature_options class are listed in the following table. Each of these parameters has a 
 *  <tt>set</tt> and <tt>get</tt> method.
 * <br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>blends_max_radius</tt></td>
 * <td valign = "top">Specifies the maximum radius of blends to recognize. 
 * The default value is 10.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>blends_num_curvature_samples</tt></td>
 * <td valign = "top">Specifies the number of sample points used for testing 
 * the blend as Constant Radius or Variable Radius blend. 
 * The default value is 1.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>blend_supports_max_angle</tt></td>
 * <td valign = "top">Specifies the maximum angle (in degrees) between blend supports. 
 * The default value is 175.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>chamfers_max_width</tt></td>
 * <td valign = "top">Specifies the maximum width of chamfers to recognize. 
 * The default value is 10.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>chamfer_supports_max_angle</tt></td>
 * <td valign = "top">Specifies the maximum angle (in degrees) between chamfer supports. 
 * The default value is 105.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>chamfer_support_width_ratio</tt></td>
 * <td valign = "top">Specifies the minimum ratio of width of support face to width of chamfer. 
 * The default value is 1.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>recognize_blends</tt></td>
 * <td valign = "top">Specifies that blend recognition should be done. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>recognize_chamfers</tt></td>
 * <td valign = "top">Specifies that chamfer recognition should be done. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>recognize_holes</tt></td>
 * <td valign = "top">Specifies that hole recognition should be done. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>hole_max_diameter</tt></td>
 * <td valign = "top">Specifies the maximum diameter of holes to recognize. 
 * The default value is 10.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>recognize_spherical_holes</tt></td>
 * <td valign = "top">Specifies that spherical hole recognition should be done. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>keep_faces</tt></td>
 * <td valign = "top">Specifies list of faces that are to be kept and not removed by the feature removal. 
 * The default value is an empty list.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>pause_after_recognize</tt></td>
 * <td valign = "top">Specifies that processing stops after the feature recognition stage. 
 * The default value is <tt>FALSE</tt>.</td>
 * </tr>
 * </table> 
 * <br>
**/
class DECL_DEFEATURE defeature_options : public ACIS_OBJECT
{
private:
	/**
	* Flag that specifies if processing stops after feature recognition.
	**/
	logical	pause_after_recognize;

	/**
	* Recognize blends flag.
	**/
	logical	do_blends;

	/**
	* Maximum radius of blends to recognize.
	**/
	double	blend_max_radius;

	/**
	* The number of sample points used for testing the blend as Constant Radius or Variable Radius blend.
	**/
	int	blend_num_curvature_samples;

	/**
	* Recognize chamfers flag.
	**/
	logical	do_chamfers;

	/**
	* Maximum width of chamfers to recognize.
	**/
	double	cham_max_width;

	/**
	* Recognize holes flag.
	**/
	logical	do_holes;

	/**
	* Maximum diameter of holes to recognize.
	**/
	double	hole_max_dia;

	/**
	* Maximum angle between blend supports.
	**/
	double blend_supports_max_angle;

	/**
	* Include spherical holes.
	**/
	logical	include_spherical_holes;

	/**
	* Maximum angle between chamfer supports.
	**/
	double chamfer_supports_max_angle;

	/**
	* Minimum ratio of width of support face to width of chamfer
	**/
	double chamfer_support_width_ratio;

	/**
	* Holds all the feature list info set by FR.
	**/
	FRSFeatureData* feature_data;
	
	/**
	* Holds list of faces to keep.
	**/
	ENTITY_LIST keep_faces;

	// for internal use
	/**
	* @nodoc
	**/
	FRSFeatureData* get_FRSFeatureData() const;
	void set_FRSFeatureData(FRSFeatureData* featureData);

protected:
	/**
	* Copy constructor.
	*/
	defeature_options(const defeature_options& copy);

	/**
	* Assignment operator.
	*/
	defeature_options& operator=(const defeature_options& copy);


public:

	/**
	* C++ Default constructor requests memory for this object and assigns default values.
	**/
	defeature_options();

	/**
	* C++ Default destructor.
	**/
	~defeature_options();

	/**
	* Gets the status of the <tt>pause_after_recognize</tt> flag.
	**/
	logical get_pause_after_recognize ();

	/**
	* Sets the status of the <tt>pause_after_recognize</tt> flag. 
	* Can be called only before feature recognition.
	* @param flag 
	* new value
	**/
	outcome set_pause_after_recognize (logical flag);

    /**
	* Gets the status of the <tt>recognize_blends</tt> flag.
	**/
	logical get_recognize_blends() const;	

	/**
	* Sets the status of the <tt>recognize_blends</tt> flag to the default value of <tt>TRUE</tt>. 
	* Can be called only before feature recognition.
	**/
	outcome set_recognize_blends();

	/**
	* Sets the status of the <tt>recognize_blends</tt> flag to the input value.
	* Can be called only before feature recognition.
	* @param flag 
	* new value
	**/
	outcome set_recognize_blends(logical flag);
	
	/**
	* Gets the value of the <tt>blends_max_radius</tt> parameter. 
	* This value is the maximum radius of the blends that are recognized and removed.
	**/
	double get_blends_max_radius() const;	

	/**
	* Sets the value of the <tt>blends_max_radius</tt> parameter to be the default value of 10. 
	* Can be called only before feature recognition.
	**/
	outcome set_blends_max_radius();

	/**
	* Sets the value of the <tt>blends_max_radius</tt> parameter to the input value. 
	* Can be called only before feature recognition.
	* @param radius 
	* new value
	**/
	outcome set_blends_max_radius(double radius);

	/**
	* Gets the number of sample points used for testing blends as Constant Radius or Variable Radius blends. 
	**/
	int get_blends_num_curvature_samples() const;

	/**
	* Sets the number of sample points used for testing blends as Constant Radius or Variable Radius blends
	* to the default value of 1. 
	* Can be called only before feature recognition.
	**/
	outcome set_blends_num_curvature_samples();

	/**
	* Sets the number of sample points used for testing blends as Constant Radius or Variable Radius blends
	* to the given value. 
	* Can be called only before feature recognition.
	* @param numSamples 
	* new value
	**/
	outcome set_blends_num_curvature_samples(int numSamples);
	
    /**
	* Gets the status of the <tt>recognize_chamfers</tt> flag.
	**/
	logical get_recognize_chamfers() const;	

	/**
	* Sets the status of the <tt>recognize_chamfers</tt> flag to the default value of <tt>TRUE</tt>.
	**/
	outcome set_recognize_chamfers();	

	/**
	* Sets the status of the <tt>recognize_chamfers</tt> flag to the input value.
	* @param flag 
	* new value
	**/
	outcome set_recognize_chamfers(logical flag);	

	/**
	* Gets the value of the <tt>chamfers_max_width</tt> parameter. 
	* This is the maximum width of the chamfers that are recognized and removed.
	**/
	double get_chamfers_max_width() const;	

	/**
	* Sets the value of the <tt>chamfers_max_width</tt> parameter to the default value of 10. 
	* Can be called only before feature recognition.
	**/
	outcome set_chamfers_max_width();

	/**
	* Sets the value of the <tt>chamfers_max_width</tt> parameter to the input value. 
	* Can be called only before feature recognition.
	* @param width 
	* new value.
	**/
	outcome set_chamfers_max_width(double width);

	/**
	* Gets the status of the <tt>recognize_holes</tt> flag.
	**/
	logical get_recognize_holes() const;	

	/**
	* Sets the status of the <tt>recognize_holes</tt> flag to the default value of <tt>TRUE</tt>. 
	* This method can be called only before feature recognition.
	**/
	outcome set_recognize_holes();

	/**
	* Sets the status of the <tt>recognize_holes</tt> flag to the input value. 
	* This method can be called only before feature recognition.
	* @param flag 
	* new value.
	**/
	outcome set_recognize_holes(logical flag);	

	/**
	* Gets the value of the <tt>hole_max_diameter</tt> parameter. 
	* This is the maximum diameter of the holes that are recognized and removed.
	**/
	double get_hole_max_diameter() const;	

	/**
	* Sets the value of the <tt>hole_max_diameter</tt> parameter to the default value of 10. 
	* Can be called only before feature recognition.
	**/
	outcome set_hole_max_diameter();

	/**
	* Sets the value of the <tt>hole_max_diameter</tt> parameter to the input value. 
	* Can be called only before feature recognition.
	* @param diameter 
	* new value.
	**/
	outcome set_hole_max_diameter(double diameter);	

	/**
	* Gets the value of the <tt>blend_supports_max_angle</tt> parameter. 
	* This is the maximum angle (in degrees) between supports of blends that are recognized and removed.
	**/
	double get_blend_supports_max_angle() const;	

	/**
	* Sets the value of the <tt>blend_supports_max_angle</tt> parameter to the default value of 175 degrees. 
	* Can be called only before feature recognition.
	**/
	outcome set_blend_supports_max_angle();

	/**
	* Sets the value of the <tt>blend_supports_max_angle</tt> parameter to input value in degrees. 
	* Can be called only before feature recognition.
	**/
	outcome set_blend_supports_max_angle(double diameter);

	/**
	* Gets the value of the <tt>chamfer_supports_max_angle</tt> parameter. 
	* This is the maximum angle (in degrees) between supports of chamfer that are recognized and removed.
	**/

	double get_chamfer_supports_max_angle() const;	

	/**
	* Sets the value of the <tt>chamfer_supports_max_angle</tt> parameter to the default value of 105 degrees. 
	* Can be called only before feature recognition.
	**/
	outcome set_chamfer_supports_max_angle();

	/**
	* Sets the value of the <tt>chamfer_supports_max_angle</tt> parameter to input value in degrees. 
	* Can be called only before feature recognition.
	**/
	outcome set_chamfer_supports_max_angle(double angle);

	/**
	* Gets the value of the <tt>chamfer_support_width_ratio</tt> parameter. 
	* This is the minimum ratio of width of support face to width of chamfer.
	**/
	double get_chamfer_support_width_ratio() const;	

	/**
	* Sets the value of the <tt>chamfer_support_width_ratio</tt> parameter to input value.
	* Can be called only before feature recognition.
	**/
	outcome set_chamfer_support_width_ratio(double ratio);


	/**
	* Gets the status of the <tt>recognize_spherical_holes</tt> flag.
	**/
	logical get_recognize_sphericalholes() const;	
	
	/**
	* Sets the status of the <tt>recognize_spherical_holes</tt> flag to the default value of <tt>TRUE</tt>.
	**/
	outcome set_recognize_sphericalholes();

	/**
	* Sets the status of the <tt>recognize_spherical_holes</tt> flag to the input value.
	* @param flag 
	* new value
	**/
	outcome set_recognize_sphericalholes(logical flag);

	/**
	* Returns a clone of the object. 
	* The feature data of the defeature_options object is not cloned.
	**/
	defeature_options* clone() const;
	
	/**
	* Returns the status of the <tt>recognize_done</tt> flag.
	**/
	logical recognize_done() const;

	/**
	* Returns the status of the <tt>remove_done</tt> flag.
	**/
	logical remove_done() const;

	/**
	* Returns the number of recognized features. 
	* This method is valid when feature recognition has been completed (<tt>recognize_done</tt> is <tt>TRUE</tt>) and 
	* feature removal has not been completed  (<tt>remove_done</tt> is <tt>FALSE</tt>).
	**/
	int get_num_features() const;

	/**
	* Returns the faces and feature type of a recognized feature.
	* This method is valid when feature recognition has been completed (<tt>recognize_done</tt> is <tt>TRUE</tt>) and 
	* feature removal has not been completed  (<tt>remove_done</tt> is <tt>FALSE</tt>).
	* @param feature_id 
	* index into the list of features that were recognized.
	* @param feature_faces 
	* ENTITY_LIST that will be populated with the list of faces. 
	* @param type 
	* defeature_feature_type that will be populated with the type of feature.
	**/
	outcome get_feature(int feature_id, ENTITY_LIST& feature_faces, defeature_feature_type & type) const;

	/**
	* Returns the number of failed features.
	* This method is valid when feature removal has been completed (<tt>remove_done</tt> is <tt>TRUE</tt>).
	**/
	int get_num_failed_features() const;

	/**
	* Returns the faces and feature type of a failed feature.
	* This method is valid when feature removal has been completed (<tt>remove_done</tt> is <tt>TRUE</tt>).
	* @param feature_id 
	* index into the list of failed features.
	* @param feature_faces 
	* ENTITY_LIST that will be populated with the list of faces for the feature.
	* @param type 
	* @href defeature_feature_type that will be populated with the type of feature.
	**/
	outcome get_failed_feature(int feature_id, ENTITY_LIST& feature_faces, defeature_feature_type & type) const;

	// feature retention interface

	/**
	* Controls defeaturing by marking a face on a feature that is not to be removed.
	* @param feature_face 
	* the face on which you would like to set the <tt>keep_it</tt> flag.
	* @param keep_it 
	* <tt>logical</tt> parameter that tells the defeaturing process to keep a face during feature removal. Default is <tt>TRUE</tt>.
	**/
	void keep_face(FACE const *feature_face, logical keep_it = TRUE); 

	/**
	* Queries if a face was kept and not removed by the feature removal.
	* @param feature_face 
	* face for which the query is to be performed.
	**/
	logical kept_face(FACE const *feature_face) const;

	/**
	* Returns a list of faces that were kept and not removed by the feature removal.
	* @param kept_faces 
	* ENTITY_LIST that will be populated with the list of kept faces.
	**/
	void get_kept_faces(ENTITY_LIST& kept_faces) const;
	
	/**
	* Resets state of the options object to <tt>before recognize</tt>, clearing both recognized and failed feature lists.
	**/ 
	void clear_features();

	/**
	* @nodoc
	*/
	friend DECL_DEFEATURE outcome api_find_and_remove_features( const ENTITY_LIST& input_bodies, 
									 defeature_options* dfopts,
									 AcisOptions* aopts );

}; 
/** @} */
#endif // _ACIS_DEFEATURE_OPTIONS_H

