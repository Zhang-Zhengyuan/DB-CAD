/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SPLIT_API_HXX )
#define SPLIT_API_HXX

#include "dcl_bool.h"
#include "logical.h"
#include "api.hxx"
#include "boolean_handles.hxx"
#include "container_utils.hxx"
#include "interval.hxx"

class BODY;
class FACE;
class EDGE;
class WIRE;
class ENTITY_LIST;
class split_wire_options;
class SPAposition_vector;

class face_subdivision;
class subdivide_face_options;

class face_subdivision_impl;
class subdivide_face_options_impl;

/** 
 *  @file split_api.hxx
 * 
 *  \addtogroup BOOLADVSPLITS
 *  \brief Declared at <split_api.hxx>, SPAbool
 *  @{
 */

/**
* Splits periodic faces along isoparameter curves to ensure they are well formed.
* <br><br>
* <b>Role:</b> This function splits periodic faces of the given entity along
* <i>u</i> and/or <i>v</i> isoparameter curves to ensure they are well formed.
* This means that each periodic face has no edges that cross the parameter
* seam. Generally speaking, this function can split a periodic face once, to create 
* a periodic face with a seam edge, or twice, to create two open faces. More
* precisely, this function will split a periodic face along one or two isoparametric
* curves. Multiple faces can result from any such split, depending upon the 
* boundaries of the original face. The input entity should be a @href BODY, 
* @href LUMP, @href SHELL, or @href FACE.
* <br><br>
* Refer to the documentation for the option <tt>new_periodic_splitting</tt>,
* which controls how periodic faces are split. Option values of 0 or 1 cause 
* periodic faces to be split twice. Option values of 2 or 3 cause periodic faces
* to be split once. Option values of 1 or 3 will allow periodic faces on analytic 
* or %spline surfaces to be split. Option values of 0 or 2 will allow periodic faces 
* on only analytic surfaces to be split. 
* <br><br>
* <b>Errors:</b> <tt>ent</tt> is a NULL pointer.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D InterOp, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param ent
* (in) Entity whose periodic faces are to be split.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_split_periodic_faces(
			ENTITY *ent,					// entity on which seams added for periodic surfaces
			AcisOptions* ao =NULL
		);

// STI let (9/98): Added a new API function
/**
 * Splits the edges of an entity at surface poles.
 * <br><br>
 * <b>Role:</b> This API function examines each face in the given entity and splits
 * edges that pass through a pole of the face's %surface. (If a PCURVE passes through
 * a pole of a %surface, it will most likely have a G0 discontinuity at the pole which
 * will cause problems for algorithms that use PCURVEs.) The input entity should
 * be a @href BODY, @href LUMP, @href SHELL, or @href FACE.
 * <br><br>
 * <b>Errors:</b> <tt>blank</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp, 3D ACIS Exchange, 3D Viz Exchange
 * <br><br>
 * @param blank
 * (in/out) Entity whose edges are to be split.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_split_edges_at_poles (
			ENTITY* blank,              // entity on which edges are to be split
			AcisOptions* ao =NULL
		);

/**
 * Splits a face along isoparametric lines at G1 or G2 discontinuities.
 * <br><br>
 * <b>Role:</b> This function splits a face along the <i>u</i> and/or <i>v</i> isoparametric
 * lines at parameters of G1 or G2 discontinuity. The list
 * of new faces is returned via the argument <tt>split_faces</tt>.
 * <br><br>
 * If the edges of the input face have discontinuities of order 1 or 2 
 * at locations other than where the surface has such discontinuities,
 * then splitting the edges using @href api_split_edge_at_disc
 * is recommended before calling this function. Otherwise,
 * the correct result of this function cannot be guaranteed.
 * <br><br>
 * <b>Errors:</b> Face does not contain discontinuity information.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li> If the supplied face is an independent face (that is, it has no body, lump,
 * or shell), this function will return a list of faces that share edges and do not belong to
 * a shell, lump or body.</li>
 * <li> This function splits faces only at discontinuities in the parameter range of the face.</li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp, 3D ACIS Exchange, 3D Viz Exchange
 * <br><br>
 * @param face
 * (in) Face to be split.
 * @param split_faces
 * (out) List of resulting faces.
 * @param cont_order
 * (in) Set to 1 to remove G1 discontinuities, 2 to remove G2 discontinuities.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_split_face_at_disc(
											FACE*		 face,					// face to be split
											ENTITY_LIST& split_faces,			// resulting faces
											int			 cont_order  = 1,		// G1 or G2
											AcisOptions* ao	         = NULL
											);

/**
 * Splits a given wire using a set of points.
 * <br><br>
 * <b>Role:</b> This API splits the given wire using given set of points to be referred as split points.   
 * The split points can lie on the wire or some distance away. 
 * <br><br>
 * For each split point, mapping position on the wire body is found, that can lie either on an edge or on a vertex.
 * If the mapped position happens to lie on an edge, then that edge will be split. 
 * If it lies on a vertex, then there will be no change in the wire body.  
 * <br><br>
 * If a split point maps to multiple positions, then the API will choose any one of them.
 * In such cases vertex will be preferred. 
 * <br><br>
 * <b>Errors:</b> The entity is not a wire body .
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li> More than one split points mapping to same position on the wire may create bad output. </li>
 * <li> If the split points are not close enough to the wire, then the output of the API may not be consistent. </li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
 * <br><br>
 * @param wire 
 * (in/out) Wire body given for splitting.
 * @param points
 * (in) The set of given points.
 * @param swo
 * (in) Split wire specific options	(for future use)
 * @param ao
 * (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_split_wire_edges ( 
			BODY* wire,								//Input Wire for spliting
			const SPAposition_vector &points,	    //Input vector of SPAposition
			split_wire_options *swo = NULL,   // Currently this argument will not be used.
			const AcisOptions* ao = NULL		    //Input acis options
			);

/**
 * Subdivides a given face using a  @href face_subdivision object, and splits the face.
 * <br><br>
 * <b>Role:</b> This API subdivides the given face using parameters supplied to 
 * @href face_subdivision object. Using the subdivisions found, the FACE will be 
 * split by default. The newly created faces will be returned through @href 
 * subdivide_face_output_handle. Default behavior of splitting can be controlled by setting an 
 * option @href subdivide_face_options. If splitting is not done, then the wire body 
 * representing subdivisions will be returned using @href subdivide_face_output_handle.<br>
 * To query the output faces or wire etc. from @href subdivide_face_output_handle use 
 * @href subdivide_face_output_query.<br>
 * This API supports the face that do not have owner( free face ).
  * <br><br>
 * Note that it is entirely possible that no subdivisions of face exist. This happens 
 * in case of the parameters given are outside face. In such situation, the function will 
 * return a successful @href outcome, but subdivide_face_output_handle <tt>sfoh</tt>
 * will be NULL. This allows an application to distinguish between a failed subdivision
 * operation and non-existence of subdivisions.
 * <br><br>
 * For additional information refer to the Technical Article 
 * <i>Additional_Boolean_Operations#Subdividing_Face_along_Multiple_Iso-parametric_Curves</i>.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>face</tt> is a NULL pointer or does not point to a @href FACE.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param face 
 * (in) Input face to be subdivided.
 * @param subdivision 
 * (in) Object representing how to subdivide.
 * @param sfo
 * (in) Options to control face subdivision.	
 * @param sfoh
 * (out) Handle to the output of subdivide face operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome
api_subdivide_face( FACE *face,
					face_subdivision const &subdivision,
					subdivide_face_output_handle *&sfoh,
					subdivide_face_options const *sfo = NULL,
					AcisOptions const *ao = NULL  );

/**
* This class provides user interfaces to define how to subdivide the FACE.
* Object of this class would be created by user on stack.
**/

class DECL_BOOL face_subdivision : public ACIS_OBJECT
{
public:
	
	/**
	* Destructor.
	**/

	~face_subdivision();

	/**
	* @nodoc
	* Copy constructor. This could be costly.
	**/

	face_subdivision( face_subdivision const & );

	/**
	* Creates the object that defines how to subdivide the FACE.<br> 
	* Iso-parametric lines of the @href surface of @href FACE, at 
	* given u and v parameters will be used for subdividing the FACE.
	**/

	static face_subdivision make( SPAdouble_vector const &u_params,
								  SPAdouble_vector const &v_params );
	/**
	* Creates the object that defines how to subdivide the FACE.<br>
	* The given u_range or v_range will be divided equally using number of divisions.
	* e.g. <tt>no_of_u_divisions</tt> = 2 means <tt>u_range</tt> will be divided in two parts. 
	* If the number of divisions is less than 2, error will be thrown. <br>
	* The u and v parameters found after equal divisions of the range, will be used to 
	* generate iso-parametric lines of the @href surface of @href FACE for subdividing the FACE.
	**/

	static face_subdivision make( SPAinterval const &u_range,
								  unsigned no_of_u_divisions,
								  SPAinterval const &v_range,
								  unsigned no_of_v_divisions );

private:
	
	// Constructor.

	face_subdivision( face_subdivision_impl* );
	
	/**
	* @nodoc
	* Don't allow Assignment.
	**/

	face_subdivision& operator=( face_subdivision const & );

	face_subdivision_impl *_impl;
	friend class face_subdivision_query;
};

/**
* This class will provide user an interface to customize the behavior of 
* FACE subdivision operation. Object of this class would be created by user on stack.
**/

class DECL_BOOL subdivide_face_options : public ACIS_OBJECT
{
public:
	/**
	* Default Constructor.
	**/

	subdivide_face_options();

	/**
	* @nodoc
	* Copy constructor.
	**/

	subdivide_face_options( subdivide_face_options const & );

	/**
	* Destructor
	*/

	~subdivide_face_options();

	/**
	 * Sets flag to control splitting of FACE.<br>
	 * Using default constructed object, FACE will be split. If face splitting 
	 * is not desired then set <tt>split</tt> to <tt>false</tt>.
	 */
	
	void do_split( bool split );

private:

	/**
	* @nodoc
	* Don't allow Assignment.
	**/

	subdivide_face_options& operator=( subdivide_face_options const & );

	subdivide_face_options_impl *_impl;
	friend class subdivide_face_options_query;
};

/**
 * This class allows a user to query the output returned by the @href api_sudivide_face.
 */

class DECL_BOOL subdivide_face_output_query
{
public:

	/**
	 * Constructor to create an object of this class given a subdivide_face_output_handle.
	 */

	subdivide_face_output_query( subdivide_face_output_handle const *h );

	/**
	 * Gives a wire body representing the face subdivisions. <br>
	 * Wire body will be NULL, if the subdivide_face_output_handle is NULL or
	 * the FACE is split as per settings in @href subdivide_face_options.
	 */

	BODY* wire() const;

	/**
	 * Gives a List of faces that were created after splitting the FACE.<br>
	 * List of faces will be empty, if the subdivide_face_output_handle is NULL.	 
	 */
	
	unsigned faces( ENTITY_LIST &faces ) const;

	/**
	 * Destructor.
	 */

	~subdivide_face_output_query();

private:

	/**
	* @nodoc
	* Don't allow Assignment/copy constructor.
	**/
		
	subdivide_face_output_query( subdivide_face_output_query const & );

	subdivide_face_output_query& operator=( subdivide_face_output_query const & );

	// For internal use only!

	subdivide_face_output_handle const *_h;	
};

/** @} */

#endif	//SPLIT_API_HXX
