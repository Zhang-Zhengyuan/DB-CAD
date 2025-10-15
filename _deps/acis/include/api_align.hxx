/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef API_ALIGN_HXX
#define API_ALIGN_HXX

/**
 * @file api_align.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <api_align.hxx>
 * @{
 */

#include "api.hxx"
#include "SPAposition_cloud.hxx"
#include "dcl_fct.h"

class align_options_impl;
/**
* Interface class for modifying the behavior of @href api_align.
* <br>
* <b>Role:</b> api_align provides an effective algorithm for registering point clouds to B-Rep models.
* In general, hint points are required to roughly align the point cloud to a known location on the model.
* Then, the API uses an iterative least squares algorithm to find the best transform to move the points on
* to part of a B-Rep model.
**/

class DECL_FCT align_options: public ACIS_OBJECT
{
	align_options_impl* m_impl;

	align_options( align_options const &);
	align_options& operator=(align_options const &);
public:
	/**
	* C++ constructor
	**/
	align_options();
	/**
	* C++ destructor
	**/
	~align_options();


	/**
	* Specifies requested alignment tolerance.  Process stops once root-mean squared distance
	* of points is closer than desired distance.  This number should be set to approximately the 
	* error in the measurement device.
	**/
	void set_desired_rms_distance( double desired_dist );
	/**
	* Returns the desired rms dist.
	**/
	double get_desired_rms_distance() const;

	/**
	* If users specifies at least three hint point pairs, the API will use those to 
	* get a rough transform between the point cloud and the model.
	**/
	void add_hint_point_pair( SPAposition const& point_on_cloud, SPAposition const& approx_corresponding_point_on_model);


	/**
	* Returns an estimate of the rms distance between the
	* points and the given entities.
	**/
	double get_estimated_rms_distance() const;


	enum alignment_mode
	{
		default_align_mode,
		rough_align_only,
		fine_align_only
	};

	/**
	* Specifies alignment mode.  By default the API computes an initial guess,
	* then polishes it using an iterative algorithm.  It is possible to 
	* ask for only a guess or only iterative polishing.
	**/
	void set_alignment_mode( alignment_mode mode );

	/**
	* returns the alignment mode specified by this options object.
	**/
	alignment_mode get_alignment_mode() const;

	//internal use only.
	/**
	*@nodoc
	**/
	align_options_impl* get_impl();

	/**
	* By default the api fails if the rms distance after the least squares alignment 
	* procedure is bigger than 1/500 of the box (of the input entities) diagonal.
	*
	* This method turns off that behavior.  You will always get an alignment transform,
	* but will not be warned if it leaves a large deviation.
	*
	* This behavior is only allowed with <tt>alignment_mode</tt> equal to <tt>fine_align_only</tt>
	**/
	void set_always_answer( logical do_always_answer);
};


/**
* Attempts to compute the transform which best moves the points 
* onto the boundary of the given entities.
*
* This API is intended for workflows where point scan data
* of an object needs to be put in the same coordinate system
* as a B-Rep model of the same nominal object.
**/
DECL_FCT outcome api_align( 
			ENTITY_LIST const& entities, 
			int num_points,
			SPAposition const* points,
			SPAtransf& trans_moving_points_to_entities,
			align_options* opts=NULL,
			AcisOptions* ao=NULL );

/**
* Attempts to compute the transform which best moves the points 
* onto the boundary of the given entities.
*
* This API is intended for workflows where point scan data
* of an object needs to be put in the same coordinate system
* as a B-Rep model of the same nominal object.
**/
DECL_FCT outcome api_align( 
			ENTITY_LIST const& entities, 
			SPAposition_cloud const& cloud,
			SPAtransf& trans_moving_points_to_entities,
			align_options* opts=NULL,
			AcisOptions* ao=NULL );


/**
* This function is for experts only.  It returns the least squares transform which
* best takes Q onto P in the sense that sum_i(P[i] - Q[i]*out_transform).len_sq() is minimal.
*
* The return value is 1 if everything is ok and 0 otherwise.  This function will not throw.
**/
DECL_FCT logical best_fit_transform( 
					int num_points,
					SPAposition const* P,
					SPAposition const* Q,
					SPAtransf& out_transform );

/**
* This function is for experts only.  
*
* It returns the approximate least squares transform which
* best takes Q onto P in the sense that sum_i((P[i] - Q[i]*out_transform)%P_normals[i])^2 is minimal.
* The equations for solving this least squares problem are nonlinear.  This function returns a solution
* based on a linearizing approximation.  The approximation error goes to zero as the transform approaches
* the identity.  When using noisy data, the caller is responsible for checking the transform we get from this
* is reasonable.  If this gives an unreliable answer try using the other signature of best_fit_transform.
*
* The return value is 1 if everything is ok and 0 otherwise.  This function will not throw.
**/
DECL_FCT logical best_fit_transform( 
					int num_points,
					SPAposition const* P,
					SPAposition const* Q,
					SPAunit_vector const* P_normals,
					SPAtransf& out_transform );

/** @} */
#endif

