/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MODEL_COMPARISON_HXX
#define MODEL_COMPARISON_HXX

#include "api.hxx"
#include "dcl_hlc.h"

class FACE;
class model_comparison_options_impl;
class model_comparison_results_impl;

/**
 * @file api_model_comparison.hxx
 */

/**
* Class <tt>model_comparison_options</tt> can be used for setting parameters for @href api_model_comparison.
* The parameters are
* <br><br>
* <ul>
* <li><tt>Tolerance</tt>:<br>
*		Tolerance at which model comparison operation is to be done. The default value is 1e-4.
* </li>
* <br><br>
* <li><tt>Sample spacing</tt>:<br>
*		This determines the discretization for the sampling of the two bodies. The default value is 0.5.
* </li>
* <br><br>
* <li><tt>Angle tolerance</tt>:<br>
*		This determines the angle tolerance for the sampling of the bodies (no sample points should have normals forming
*		an angle larger than angle tolerance to their nearest neighbors). The default value is 10 (degrees).
* </li>
* </ul>
* <br><br>
**/
class DECL_HLC model_comparison_options : public ACIS_OBJECT {
	model_comparison_options_impl* m_options;
public:
	/**
	* @nodoc
	**/
	model_comparison_options();

	/**
	* @nodoc
	**/
	~model_comparison_options();

	/**
	* @nodoc
	* Copy constructor
	**/
	model_comparison_options(const model_comparison_options& source);

	/**
	* @nodoc 	
	* Assignment operator
	**/
	model_comparison_options& operator=(const model_comparison_options& source);

	/**
	* @nodoc. internal use only.
	**/
	model_comparison_options_impl* get_impl();

	/**
	* @nodoc. internal use only.
	**/
	const model_comparison_options_impl* get_impl() const { return m_options; }

	/**
	* Set the tolerance for the model comparison
	**/
	void set_tolerance(const double tol);

	/**
	* Set the sample spacing for the operation. A non-positive value provided means that the default one would be used.
	**/
	void set_sample_spacing(const double sample_res);

	/**
	* Set the angle tolerance for the sampling. A non-positive value provided means that the default one would be used.
	**/
	void set_angle_tolerance(const double angle_tolerance);

	/**
	* Get the tolerance for the model comparison
	**/
	double get_tolerance() const;

	/**
	* Get the sample spacing for the operation
	**/
	double get_sample_spacing() const;

	/**
	* Get the angle tolerance for the sampling
	**/
	double get_angle_tolerance() const;
};


/**
* Class <tt>model_comparison_results</tt> is the container for holding the results from @href api_model_comparison.
* This object can be queried by the caller to get the faces and
* boxes where there are deviations
**/
class DECL_HLC model_comparison_results : public ACIS_OBJECT {
	// Stores all the relevant results of the model comparison
	model_comparison_results_impl* m_results;
public:

	/**
	* Get the faces that contain regions of large deviations
	**/
	void get_faces(ENTITY_LIST& bad_faces) const;

	/**
	* Get the number of parboxes correspond to deviations within a given face
	**/
	int get_num_parboxes_in_face(FACE *query_face) const;

	/**
	* Get the parametric boxes for a face that correspond to regions of deviations
	**/
	void get_parametric_boxes(FACE* face, const int num_parboxes, SPApar_box* parbox_list) const;

	/**
	* Get the number of 3D boxes that are created as part of the model comparison operation
	**/
	int get_num_boxes() const;

	/**
	* Get the 3D boxes that correspond to regions of deviations
	**/
	void get_boxes(const int num_boxes, SPAbox* box_list) const;

	/**
	* Create solid blocks from the the 3D boxes calculated by the operation.
	* Note that it creates new bodies each time the routine is called, and thus, the
	* ownership of those bodies lie with the caller.
	**/
	void get_blocks(ENTITY_LIST& block_list) const;

	/**
	* @nodoc
	**/
	model_comparison_results();

	/**
	* The destructor is public, indicating the user takes ownership of instances. 
	**/
	~model_comparison_results();

	/**
	* @nodoc
	* Copy constructor
	**/
	model_comparison_results(const model_comparison_results& source);

	/**
	* @nodoc
	* Assignment operator
	**/
	model_comparison_results& operator=(const model_comparison_results& source);

	/**
	* @nodoc. internal use only.
	**/
	model_comparison_results_impl* get_impl();

	/**
	* @nodoc. internal use only.
	**/
	const model_comparison_results_impl* get_impl() const { return m_results; }
};


/**
* The API <tt>api_model_comparison</tt> compares two given bodies and determines the geometric differences between them.
* For more details, refer to the Technical Article, <i>[Model Comparison](https://doc.spatial.com/get_doc_page/articles/m/o/d/Model_Comparison_a8cb.html)</i>
* <br><br>
* <b>Role:</b> This API determines the geometric differences between two bodies. A sampling is done for the faces of the two bodies.
* These sample points are used to calculate deviations with respect to the other body. Parameters for the operation
* can be specified using an options object. The results of the operation are stored within a results handle which can be
* queried to get the relevant results.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param body1
* The first entity for the comparison
* @param body2
* The second entity for the comparison
* @param mc_results
* Results object for storing the boxes calculated
* @param mco
* Options object for controlling parameters for the operation
* @param ao
* ACIS options for versioning and journaling
**/
DECL_HLC outcome api_model_comparison(BODY* body1, BODY* body2,
	model_comparison_results*& mc_results, const model_comparison_options* mco = nullptr,
	AcisOptions* ao = nullptr);


#endif
