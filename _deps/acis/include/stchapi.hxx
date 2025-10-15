/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef STITCHER_API_HEADER
#define STITCHER_API_HEADER

// Base includes
#include "logical.h"

// Kern includes
#include "api.hxx"
#include "lists.hxx"
#include "acis_options.hxx"
#include "vlists.hxx"
#include "mmgr.hxx"

// Stitch includes
#include "dcl_stitch.h"

/**
 * \defgroup STITCHAPI Stitching
 * Stitching provides a way to connect unconnected faces to make sheet or solid
 * bodies that are topologically complete. It involves merging pairs of 
 * coincident edges to a single edge and groups of coincident vertices to a 
 * single vertex.
 * @{
 * \defgroup STITCH_HANDLES Stitch Handles
 * Handles for stitch input and stitch output for a better interface.
 * \defgroup STITCH_OPTIONS Stitch Options
 * Options to control functionalities of stitching APIs.
 * \defgroup STITCH_CONTAINERS Stitch Containers
 * Containers to query results after calling stitching APIs.
 * @}
 */
/**
 * @file stchapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */

// Forward declarations
class outcome;
class BODY;
class ENTITY_LIST;
class FACE;
class EDGE;


/**
 * Specifies the type of stitching to be performed by the stitching operation.
 * @param TOLERANT_STITCH_TYPE
 * Perform tolerant stitching.
 * @param EXACT_STITCH_TYPE
 * Perform exact stitching.
 * @param EDGE_TOLERANT_STITCH_TYPE
 * Perform tolerant stitching on a user-specified list of edges.
 * @param MT_STITCH_TYPE
 * Used for multithreaded stitching APIs
 */
enum STITCH_TYPE
{
	TOLERANT_STITCH_TYPE,
	EXACT_STITCH_TYPE,
	EDGE_TOLERANT_STITCH_TYPE,
	POLYHEDRAL_STITCH_TYPE,
	MT_STITCH_TYPE
};
/**
 * Specifies how a stitching operation handles coincident faces.
 * @param SPASTITCH_COIN_ERROR
 * Throw an error if coincident faces are found.
 * @param SPASTITCH_COIN_STITCH
 * Stitch pairs of coincident faces.
 * @param SPASTITCH_COIN_SKIP
 * Do not stitch pairs of coincident faces.
 */
enum STITCH_COIN_MODES
{
	SPASTITCH_COIN_ERROR,
	SPASTITCH_COIN_STITCH,
	SPASTITCH_COIN_SKIP
};




/**
 * \ingroup STITCH_OPTIONS
 * Abstract base class from which specific stitch options classes, such as exact_stitch_options, 
 * tolerant_stitch_options and edge_tolstitch_options, and mt_stitch_options are derived.
 * @par Role
 * This class can not be instantiated and should not be derived from by customers. 
 * Objects derived from stitch_options are passed into @href api_stitch to determine the type
 *  of stitching and to pass additional parameters, such as the split_option parameter for exact stitching. 
 * The type of stitching performed is determined by the concrete type of the stitch_options object passed in.
 * @see edge_tolstitch_options, exact_stitch_options, tolerant_stitch_options, mt_stitch_options
 */
class DECL_STITCH stitch_options : public ACIS_OBJECT {

public:

	// For internal use only.
/**
 * @nodoc
 */
	virtual STITCH_TYPE get_stitch_type() const = 0;

};

/**
 * \ingroup STITCH_OPTIONS
 * Specifies that @href api_stitch should use exact stitching.
 * <br>
 * <b>Role:</b> An <tt>exact_stitch_options</tt> object is passed as an argument to
 * <tt>api_stitch</tt> to specify stitching of faces with edges strictly within
 * the ACIS tolerance <tt>SPAresabs</tt>.
 * <br><br>
 * Conversely, object of type <tt>tolerant_stitch_options</tt> specifies stitching
 * of faces with edges that are not close enough to be within the ACIS tolerance
 * <tt>SPAresabs</tt> but are "reasonably" close.
 * <br><br>
 * The value returned by the <tt>get_split_option()</tt> method controls whether or not the
 * stitch operation inserts vertices (thereby splitting existing edges) in order to create
 * edges that are coincident along their entire length (and therefore can be stitched).
 * @see stitch_options
 */
class DECL_STITCH exact_stitch_options : public stitch_options {
private:
	logical m_split;

public:
	// Constructor.
/**
 * C++ constructor.
 * <br><br>
 * <b>Role:</b>This constructor requests memory for this object and initializes
 * the object option to stitch only the faces whose edges are identical along
 * their entire length (i.e., <tt>get_split_option()</tt> will return <tt>FALSE</tt>).
 */
	exact_stitch_options();

	// For internal use only.
/**
 * @nodoc
 */
	STITCH_TYPE get_stitch_type() const;

	// Get the split option.
/**
 * This method returns the current value of the <tt>split</tt> option.
 * <br><br>
 * <b>Role:</b> A return value of <tt>FALSE</tt> indicates that the edges of faces
 * that are stitched must be identical along their entire length. (This is the default value.)
 * <br><br>
 * A return value of <tt>TRUE</tt> indicates that the edges of faces that are to be stitched may be split 
 * in order to match coincident edges. Coincident edges on single-sided faces and of incompatible orientation
 * (i.e., face normals are in opposite directions) are not split.
 * <br><br>
 * The default value for <tt>split</tt> option is <tt>FALSE</tt>.
 */
	logical get_split_option() const;

	// Set the split option.

/**
 * This method accepts either <tt>TRUE</tt> or <tt>FALSE</tt> to set the <tt>split</tt> option.
 * <br><br>
 * <b>Role:</b> A value of <tt>TRUE</tt> indicates that the edges of faces that
 * are stitched may be split in order to match coincident edges. Coincident edges
 * on single-sided faces and of incompatible orientation (i.e., face normals are in 
 * opposite directions) are not split.
 * <br><br>
 * A value of <tt>FALSE</tt> indicates that the edges of faces that are stitched
 * must be identical along their entire length.
 * <br><br>
 * The default value for <tt>split</tt> option is <tt>FALSE</tt>.
 * <br><br>
 * @param val
 * option to split partially coincident edges.
 */
	void set_split_option(logical val);
};

/**
 * \ingroup STITCH_OPTIONS
 * Specifies that @href api_stitch and @href api_stitch_nonmanifold should use tolerant stitching.
 * <br>
 * <b>Role:</b> An object of type <tt>tolerant_stitch_options</tt> is passed as
 * an argument to @href api_stitch and @href api_stitch_nonmanifold 
 * to specify stitching of faces with edges where edges have gaps greater than the ACIS tolerance 
 * <tt>SPAresabs</tt>.
 * <br><br>
 * Conversely, an object of type <tt>exact_stitch_options</tt> allows stitching
 * of faces with edges that are strictly within <tt>SPAresabs</tt>.
 * <br><br>
 * The class <tt>edge_tolstitch_options</tt> has been derived from the <tt>tolerant_stitch_options</tt> class.
 * @see stitch_options
 */
class DECL_STITCH tolerant_stitch_options : public stitch_options {

public:

/**
 * @nodoc
 */
	double m_sliver_param;

	// Constructor
/**
 * Default constructor for memory allocation and initialization of the class object.
 */
	tolerant_stitch_options();

	// For internal use only.
/**
 * @nodoc
 */

	/**
	 * Specifies mode of non-manifold stitching
	 * @param NM_IGNORE
	 * Do not attempt non-manifold stitching. 
	 * @param NM_DETECT
	 * Detect edges that will result into non-manifold edges when stitched later.
	 * @param NM_STITCH
	 * Attempt non-manifold stitching.
	 */
	enum NM_PROCESSING_MODE
	{
		NM_IGNORE,
		NM_DETECT,
		NM_STITCH
	};

	STITCH_TYPE get_stitch_type() const;

	// Get max stitch tol
/**
 * Returns the value of the maximum stitch tolerance parameter set by <tt>set_max_stitch_tol()</tt>.
 * <br><br>
 * <b>Role:</b> A negative value means that a maximum stitch tolerance has not
 * been set and that @href api_stitch and @href api_stitch_nonmanifold 
 * will use its internal algorithm for determining the maximum stitch tolerance.  Pairs of edges with gaps 
 * greater than the maximum stitch tolerance value will not be stitched by the API.
 * <br><br>
 * The default value for the maximum stitch tolerance is -1.
 */
	// double get_max_stitch_tol();
	virtual double get_max_stitch_tol() const;

	// Set max stitch tol
/**
 * Allows you to override internal algorithm which determines the maximum stitch tolerance parameter.
 * <br><br>
 * <b>Role:</b> @href api_stitch and @href api_stitch_nonmanifold will not stitch 
 * pairs of edges with gaps greater than maximum stitch tolerance. <br>
 * You are recommended to set the <tt>max_stitch_tol</tt> parameter to be 
 * smaller than the minimum feature size and bigger than the maximum gap expected
 * to be stitched in the model. Otherwise the API may remove small (sliver) edges 
 * smaller than the <tt>max_stitch_tol</tt> parameter that should have been kept.
 * <br><br>
 * The default value for the maximum stitch tolerance is -1.
 * @param tol
 * maximum stitch tolerance value.
 */

	// void set_max_stitch_tol(double tol);
	virtual void set_max_stitch_tol(double tol);

/**
 * Destructor to release the allocated memory when the object goes out of scope.
 */
	virtual ~tolerant_stitch_options();

	// Get the number of coincident face clusters.
/**
 * Returns the number of coincident face clusters that were identified during stitching.
 * <br><br>
 * <b>Role:</b> If @href api_stitch and @href api_stitch_nonmanifold 
 * could not identify any coincident clusters, this method returns 0. For coincident face clusters to be 
 * identified during stitching, user needs to set the coincident face detection mode to either <tt>SPASTITCH_COIN_SKIP</tt> or
 * <tt>SPASTITCH_COIN_STITCH</tt> by using the method <tt>set_stch_coincident_face_handling_mode()</tt>.
 * For more details into the behavior of the API with different modes
 * of coincident faces detection, refer to the documentation of method
 * <tt>set_stch_coincident_face_handling_mode()</tt>.
 */
	int get_number_of_coincident_face_clusters() const;

	// Initializes the coincident face cluster list.
/**
 * Initializes the coincident face cluster list.
 * <br><br>
 * <b>Role:</b> This function is to be called before beginning to obtain the coincident face
 * clusters one at a time using function <tt>get_next_coincident_face_cluster()</tt>.
 */
	void init_coincident_face_cluster_list() const;

	// Return the next cluster.
/**
 * Returns the next coincident face cluster.
 * <br><br>
 * <b>Role:</b> Use this function for obtaining the next cluster in the internal
 * coincident face clusters list. The first call to this method should be
 * preceded by a call to <tt>init_coincident_face_cluster_list()</tt>. Each successive
 * call to this method will then return a unique cluster from an internal stored
 * list of coincident face clusters. The method returns <tt>NULL</tt> when no more clusters remain
 * to be returned. You should not delete the <tt>ENTITY_LIST</tt> returned by this method because 
 * it points to memory owned by the <tt>tolerant_stitch_options</tt> object.
 */
	ENTITY_LIST* get_next_coincident_face_cluster() const;

	// Sets the enum STITCH_COIN_MODES to get/process the coincident face cluster.
/**
 * Sets the enum @href STITCH_COIN_MODES to <tt>SPASTITCH_COIN_ERROR</tt>, <tt>SPASTITCH_COIN_STITCH</tt>, or <tt>SPASTITCH_COIN_SKIP</tt> for doing coincident face clustering in tolerant stitch.
 * <br><br>
 * <b>Role:</b> The behavior of @href api_stitch and @href api_stitch_nonmanifold depend upon the current
 * mode for handling coincident faces. 
 * <br><br>
 * If you pass <tt>SPASTITCH_COIN_SKIP</tt> and if @href api_stitch and @href api_stitch_nonmanifold detects coincident 
 * faces, it will not stitch the two faces together along the edge, and process of stitching continues. The 
 * coincident face pair is recorded internally and this API throws a warning. You are recommended
 * to use this setting. 
 * In this case, this API will return coincident face clusters (if found), that can be obtained using methods:
 * <pre>
 * init_coincident_face_cluster_list();
 * get_next_coincident_face_cluster();
 * </pre>
 * <br>
 * If you pass <tt>SPASTITCH_COIN_ERROR</tt> and if this API detects coincident faces, 
 * it will just fail with <tt>COINCIDENT_FACE</tt> error, without returning any coincident face
 * clusters. The state of the model is rolled back to the unstitched state.
 * <br><br>
 * If you pass <tt>SPASTITCH_COIN_STITCH</tt>, then and if this API detects coincident face,
 * then it will stitch the two faces together along the edge and process of stitching continues. 
 * The coincident face pair is recorded internally and this API throws a warning.  Since this produces
 * back-to-back faces, downstream ACIS operations may fail. Hence, it is recommended that you do not use this option.
 * In the coincident stitching mode, the API will return coincident face clusters (if found), that can be obtained using methods:
 * <pre>
 * init_coincident_face_cluster_list();
 * get_next_coincident_face_cluster();
 * </pre>
 * <br><br>
 * The default mode for handling coincident faces is <tt>SPASTITCH_COIN_ERROR</tt>.
 * @param mode
 * An enum to set the coincident face clustering behavior in tolerant stitching to
 * <tt>SPASTITCH_COIN_ERROR</tt>, <tt>SPASTITCH_COIN_STITCH</tt>, or <tt>SPASTITCH_COIN_SKIP</tt>.
 */
	void set_stch_coincident_face_handling_mode(STITCH_COIN_MODES mode);

/**
 * Returns the mode under which <tt>api_stitch</tt> and <tt>api_stitch_nonmanifold</tt> will operate for identifying coincident face clusters.
 * <br><br>
 * <b>Role:</b> The possible return values of this function are <tt>SPASTITCH_COIN_SKIP</tt>,
 * <tt>SPASTITCH_COIN_ERROR</tt> or <tt>SPASTITCH_COIN_STITCH</tt>. <br><br>
 * The return value indicates the mode in which @href api_stitch and @href api_stitch_nonmanifold 
 * will operate in the event that it detects coincident  faces. To change the value of this enum, 
 * use function <tt>set_stch_coincident_face_handling_mode()</tt>. The description of what each
 * of these modes mean is described in detail in the description of the function
 * <tt>set_stch_coincident_face_handling_mode()</tt>.
 * <br><br>
 * The default mode in which the API will operate is
 * <tt>SPASTITCH_COIN_ERROR</tt>. To change this, use function
 * <tt>set_stch_coincident_face_handling_mode()</tt>.
 */
	STITCH_COIN_MODES get_stch_coincident_face_handling_mode() const;

/**
 * Accepts either <tt>TRUE</tt> or <tt>FALSE</tt> to indicate to <tt>api_stitch</tt> and <tt>api_stitch_nonmanifold</tt> whether or 
 * not to allow void shells in the output.
 * <br><br>
 * <b>Role:</b> A value of <tt>TRUE</tt> indicates that tolerant stitch is allowed to make void shells 
 * if it finds one shell inside the other. A value of <tt>FALSE</tt> indicates that @href api_stitch and @href api_stitch_nonmanifold 
 * shall not output any void shells. All shells in the output will be peripheral shells.
 * <br><br>
 * The default value for <tt>allow_void_shells</tt> is <tt>TRUE</tt>.
 * <br><br>
 * NOTE: When using an <tt>edge_tolstitch_options</tt> object, if <tt>fix_body_orientation</tt> option 
 * is set to <tt>FALSE</tt>, 
 * then edge stitching shall ignore the <tt>allow_void_shells</tt> option. <tt>api_stitch</tt> will not change 
 * the orientation of all shells based on the <tt>allow_void_shells</tt> option. The orientation of all shells in 
 * output shall remain the same to that in the input, irrespective of the <tt>allow_void_shells</tt> option. 
 * Refer to @href edge_tolstitch_options for help with the <tt>fix_body_orientation</tt> option. 
 */	
	void set_allow_void_shells(logical choice);

/**
 * Returns the current value of the <tt>allow_void_shells</tt> option. 
 * <br><br>
 * <b>Role:</b> If <tt>FALSE</tt> is returned, @href api_stitch and @href api_stitch_nonmanifold 
 * will not output any void shells. All shells in the output will be peripheral shells.
 * If <tt>TRUE</tt> is returned, these functions are allowed to make void shells if they find one shell inside of another. 
 * <br><br>
 * The default value of <tt>allow_void_shells</tt> is <tt>TRUE</tt>.
 */
	
	logical get_allow_void_shells()const;

	// Sets the enum NM_PROCESSING_MODE to set the non-manifold processing mode in tolerant stitching.
/**
 * Sets the enum @href NM_PROCESSING_MODE to <tt>NM_IGNORE</tt>, <tt>NM_STITCH</tt>, or <tt>NM_DETECT</tt> for non-manifold processing in tolerant stitching.
 * <br><br>
 * <b>Role:</b> The behavior of @href api_stitch depends upon the current non-manifold processing mode. 
 * <br><br>
 * If the non-manifold processing mode is <tt>NM_IGNORE</tt>, @href api_stitch will 
 * <ul> 
 * <li> Perform manifold stitching. The API will stitch not more than two faces at an edge. </li>
 * <li> Not attempt non-manifold stitching at all. </li>
 *</ul>
 * You are recommended to use this setting if you desire only manifold stitching. This is the default mode.
 * <br><br>
 * If the mode is <tt>NM_DETECT</tt>, this API will 
 * <ul>
 * <li> Perform manifold stitching like in <tt>NM_IGNORE</tt> mode. </li>
 * <li> Detect potential edges such that when these  edges are stitched together in another stitch attempt, they will result in a non-manifold edge.</li> 
 * <li> Give out information of the potential non-manifold edges in the form of non-manifold edge clusters. </li>
 * </ul> 
  * You are recommended to use this setting if you desire manifold stitching but at the same time wish to know presence of potential non-manifold edge junctions.<br>
 * <br><br>
 * If the mode is <tt>NM_STITCH</tt>, this API will 
 * <ul>
 * <li> Perform non-manifold stitching. The API will stitch more than two faces at an edge. </li>
 * <li> Give out information of stitched non-manifold edges in the form of non-manifold edge clusters.</li>
 *</ul>
 * You are recommended to use this setting if you desire non-manifold stitching. 
 * <br><br> 
 * The non-manifold edge clusters can be obtained by using methods 
 * <pre>
 * init_nonmanifold_edge_cluster_list();
 * get_next_nonmanifold_edge_cluster();
 * </pre>
 * @param mode
 * An enum to set the non-manifold processing mode in tolerant stitching to
 * <tt>NM_IGNORE</tt>, <tt>NM_STITCH</tt>, or <tt>NM_DETECT</tt>.
 */
	void set_nonmanifold_processing_mode( NM_PROCESSING_MODE mode );

	/**
	* Get the non-manifold processing mode in tolerant stitching.
	*/
	NM_PROCESSING_MODE get_nonmanifold_processing_mode() const;

/**
 * Returns the number of non-manifold edge clusters that were identified during stitching.
 * <br><br>
 * <b>Role:</b> If @href api_stitch could not identify any non-manifold edge clusters, this method returns 0. For non-manifold edge clusters clusters to be 
 * identified during stitching, user needs to set the non-manifold processing mode to either <tt>NM_STITCH</tt> or
 * <tt>NM_DETECT</tt> by using the method <tt>set_nonmanifold_processing_mode()</tt>. 
 * When non-manifold processing mode is <tt>NM_STITCH</tt>, the edges in the clusters are non-manifold edges. 
 * When non-manifold processing mode is <tt>NM_DETECT</tt>, the edges in each cluster when stitched together in another stitch attempt, will result in a non-manifold edge.
 * For more details into the behavior of the API with different modes of non-manifold processing, refer to the documentation of method
 * <tt>set_nonmanifold_processing_mode()</tt>.
 */
	int get_number_of_nonmanifold_edge_clusters() const;
/**
 * Initializes the non-manifold edge cluster list.
 * <br><br>
 * <b>Role:</b> This function is to be called before beginning to obtain the non-manifold edge
 * clusters one at a time using function <tt>get_next_nonmanifold_edge_cluster()</tt>.
 */
	void init_nonmanifold_edge_cluster_list() const;
/**
 * Returns the next non-manifold edge cluster.
 * <br><br>
 * <b>Role:</b> Use this function for obtaining the next cluster in the internal
 * non-manifold edge clusters list. The first call to this method should be
 * preceded by a call to <tt>init_nonmanifold_edge_cluster_list()</tt>. Each successive
 * call to this method will then return a unique cluster from an internal stored
 * list of non-manifold edge clusters. The method returns <tt>NULL</tt> when no more clusters remain
 * to be returned. You should not delete the <tt>ENTITY_LIST</tt> returned by this method because 
 * it points to memory owned by the <tt>tolerant_stitch_options</tt> object.
 */
	ENTITY_LIST* get_next_nonmanifold_edge_cluster() const;

	// Returns an ENTITY_LIST of all the faces that were paired with that face
	/**
	 *  Fills the given <tt>ENTITY_LIST</tt> with all faces identified as coincident with the given face by <tt>api_stitch</tt> and <tt>api_stitch_nonmanifold</tt> under either <tt>SPASTITCH_COIN_SKIP</tt> mode or <tt>SPASTITCH_COIN_STITCH</tt> mode.
	 */
	void get_coincident_face_partners(FACE const* face, ENTITY_LIST & partner_faces) const;

	// Returns all the coincident faces in the model.
	/**
	 *  Fills the given <tt>ENTITY_LIST</tt> with all faces identified as coincident by <tt>api_stitch</tt> and <tt>api_stitch_nonmanifold</tt> under either <tt>SPASTITCH_COIN_SKIP</tt> mode or <tt>SPASTITCH_COIN_STITCH</tt> mode.
	 */
	void get_coincident_faces(ENTITY_LIST & partner_faces) const;

	friend class tolerant_stitch_options_internal;

	protected:
		double m_tolerant_max_stitch_tol;

		VOID_LIST m_concident_face_cluster_list;

		STITCH_COIN_MODES m_coin_face_handling_mode;

		VOID_LIST m_coincident_face_partner_cluster_list;

		ENTITY_LIST m_all_coin_faces;

		logical m_allow_void_shells;

		NM_PROCESSING_MODE m_nonmanifold_processing_mode;

		VOID_LIST m_nm_edge_cluster_list;

		bool m_output_nm_sheet_bodies;

		ENTITY_LIST m_ref_nm_att_list;


};


// ***********************************************************************
// API for Initialize and Terminate Stitching husk
// ***********************************************************************

// Initialization API for stitcher husk.

/**
 * Initializes the Stitch Component library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 **/

DECL_STITCH outcome api_initialize_stitching();

// Termination API for stitcher husk.

/**
 * Terminates the Stitch Component library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 **/

DECL_STITCH outcome api_terminate_stitching();

// ***********************************************************************
// Main APIs for Stitching
// ***********************************************************************

// Option Class for Restricted stitching

/**
 * Specifies that @href api_stitch should use restricted tolerant stitching.
 * <br>
 * <b>Role:</b> An object of type <tt>edge_tolstitch_options</tt> is passed as an
 * argument to <tt>api_stitch</tt> to specify stitching of faces with edges where
 * edges have gaps greater than the ACIS tolerance <tt>SPAresabs</tt>.
 * <br><br>
 * The <tt>to_be_stitched</tt> list (an argument to @href api_stitch) is a list of edges, 
 * and the faces to be stitched belong to the edges. Only the edges in the <tt>to_be_stitched</tt> 
 * list are considered for stitching. That is, when an <tt>edge_tolstitch_options</tt> object is
 * passed to <tt>api_stitch</tt>, the API function operates in <tt>EDGE_TOLERANT_STITCH</tt> mode, 
 * and the <tt>to_be_stitched</tt> list should contain only pointers to free <tt>EDGEs</tt>. 
 * A free <tt>EDGE</tt> is an edge that is associated with only one <tt>FACE</tt> and 
 * hence has only one <tt>COEDGE</tt>. 
 * @see stitch_options
 */
class DECL_STITCH edge_tolstitch_options : public tolerant_stitch_options
{
// Attributes
protected:
	logical m_scan_pre_existing_attribs;
	logical m_fix_body_orientation;
	// double m_tolerant_edge_max_stitch_tol;

// Operations
public:

	// Constructor
/**
 * Default constructor for memory allocation and initialization of the class object.
 */
	edge_tolstitch_options();
/**
 * Destructor to release the allocated memory when the object goes out of scope.
 */
	~edge_tolstitch_options();

	// For internal use only.
/**
 * @nodoc
 */
	STITCH_TYPE get_stitch_type() const;

	// Restricted stitching does not permit pre-existing stitch attributes on
	// the body.  Set scan_pre_existing_attribs to TRUE to have the stitcher
	// scan for and remove these attributes from the incoming body.
/**
 * Allows <tt>api_stitch</tt> to scan for pre-existing stitching attributes and remove them from incoming body.
 * <br><br>
 * <b>Role:</b>  Set <tt>scan_pre_existing_attribs</tt> to <tt>TRUE</tt> to have
 * @href api_stitch scan on and remove these attributes from the incoming body. If pre-existing stitching 
 * attributes exist on the incoming entities and are not removed, the behavior of the API is 
 * unpredictable.
 * <br><br>
 * @param check_in_att
 * option to set the stitcher scan.
 */
	void set_scan_pre_existing_attribs(logical check_in_att);

/**
 * Returns the current value of the <tt>scan_pre_existing_attribs</tt> option.
 * <br><br>
 * <b>Role:</b> See @href set_scan_pre_existing_attribs for more information on <tt>scan_pre_existing_attribs</tt>.
 * <br><br>
 * The default value for <tt>scan_pre_existing_attribs</tt> is <tt>FALSE</tt>.
 */
	logical do_scan_pre_existing_attribs() const;

	// Test whether all face normals in the final stitched model need to be reversed.
	// Set to TRUE if the possibility exists that one or more of the unstitched face
	// normals point inwards (which would result in the stitched model becoming a void).
	// If the stitch is being performed on faces that are known to all have correctly
	// pointing face normals (because of an unhook operation, for example,
	// this option can be set to FALSE.

/**
 * Checks whether all face normals in the final stitched model need to be reversed.
 * <br><br>
 * <b>Role:</b> Set to <tt>TRUE</tt> if the possibility exists that one or more
 * of the unstitched face normals point inwards (which would result in the stitched
 * model becoming a void). 
 * <p>If the stitch is being performed on faces that are known to have
 * correctly pointing face normals (because of an unhook operation, for
 * example), <tt>fix_body_orientation</tt> can be set to <tt>FALSE</tt>. </p>
 * <p> NOTE : If <tt>fix_body_orientation</tt> option is set to <tt>FALSE</tt>, restricted tolerant stitching will not change 
 * the orientation of the shells based on the <tt>allow_void_shells</tt> option. The orientation of all shells
 * in output will remain same, irrespective of the <tt>allow_void_shells</tt> option value. 
 * Refer @href tolerant_stitch_options for help on <tt>allow_void_shells</tt> option.<br><br>
 * </p>
 * @param fix_orientation
 * option to reverse the direction of face normal.
 */
	void set_fix_body_orientation(logical fix_orientation);

/**
 * Returns the current value for the <tt>fix_body_orientation</tt> option.
 * <br><br>
 * <b>Role:</b> See @href set_fix_body_orientation for more information on <tt>fix_body_orientation</tt>. 
 * <br><br>
 * The default value for <tt>fix_body_orientation</tt> is <tt>TRUE</tt>.
 */
	logical get_fix_body_orientation() const;

	// Get max stitch tol
/**
 * Returns the value of the maximum stitch tolerance parameter set by <tt>set_max_stitch_tol()</tt>.
 * <br><br>
 * <b>Role:</b> A negative value means that the maximum stitch tolerance has not
 * been set and that @href api_stitch will use its internal algorithm for determining
 * the maximum stitch tolerance.  Pairs of edges with gaps greater than the maximum
 * stitch tolerance value will not be stitched by the API.
 * <br><br>
 * The default value for the maximum stitch tolerance is -1.
 */
	double get_max_stitch_tol() const;

	// Set max stitch tol
/**
 * Allows you to override internal algorithm which determines the maximum stitch tolerance parameter.
 * <br><br>
 * <b>Role:</b> @href api_stitch will not stitch pairs of edges with gaps greater than 
 * maximum stitch tolerance. 
 * <br><br>
 * You are recommended to set the <tt>max_stitch_tol</tt> parameter to be 
 * smaller than the minimum feature size and bigger than the maximum gap expected
 * to be stitched in the model. Otherwise the API may remove small (sliver) edges 
 * smaller than the <tt>max_stitch_tol</tt> parameter that should have been kept.
 * <br><br>
 * @param tol
 * maximum stitch tolerance value.
 */
	void set_max_stitch_tol(double tol);

};

/**
 * Stitches together the given entities.
 * <br><br>
 * <b>Role:</b> @href api_stitch stitches together the given entities based on the mode specified by the <tt>stitch_options</tt> object given as input.<br>
 * The three modes are:
 * <br>
 * <ul>
 *	<li><tt> EXACT_STITCH </tt></li>
 *	<li><tt> TOLERANT_STITCH </tt></li>
 *	<li><tt> EDGE_TOLERANT_STITCH </tt></li>
 * </ul>
 * Depending on the mode under which this API works the input entities may be 
 * <ul>
 * <li> BODYs </li>
 * <li> free FACEs </li>
 * <li> EDGEs </li>  
 * </ul>
 * This API also takes an <tt>AcisOptions</tt> object as input. <br><br>
 * The output of this API is a list of bodies. This API returns an <tt>outcome</tt> object that contains information of errors and problems encountered 
 * while stitching. <br>
 * Refer to the <b>Parameters</b> section below for details of input and 
 * output parameters.
 * <br><br>
 * The behavior exhibited by the API in each of the three modes is as specified under :- <br><br>
 * <b> EXACT_STITCH </b> (Exact Stitching)
 * <br><br>
 * An <tt>exact_stitch_options</tt> object needs to be passed using the <tt>stitch_options argument</tt> for the API to operate in this mode.<br><br>
 * Under this mode, this API accepts only a list of 
 * <ul>
 * <li> <tt>BODYs</tt> </li>
 *</ul> 
 * The API stitches edges that are coincident within <tt>SPAresabs</tt>. 
 * <br><br>
 * <b> TOLERANT_STITCH </b> (Tolerant Stitching)
 * <br><br>
 * A <tt>tolerant_stitch_options</tt> object needs to be passed using the <tt>stitch_options</tt> argument for API to operate in this mode.<br><br>
 * Under this mode, this API accepts a list of 
 * <ul>
 * <li> <tt>BODYs</tt> </li>
 * <li> free <tt>FACEs</tt> </li>. 
 * <li> or a combination of both </li>.
 * </ul><br>
 * NOTE :- A free <tt>FACE</tt> is a <tt>FACE</tt> that is a top-level 
 * entity and has no owner. Note that free <tt>FACEs</tt> are not legal in ACIS, but this API still accepts 
 * them. However, we recommend that this API be passed only <tt>BODYs.</tt><br><br>
 * The API stitches edges that are close within the user specified tolerance. The <b>Maximum Stitch Tolerance</b> section elaborates the importance of the user
 * specified tolerance. <br><br>
 * Under this mode, the API:
 * <br><br>
 * <ul>
 * 		<li> Performs stitching based on the @href NM_PROCESSING_MODE set in <tt>tolerant_stitch_options</tt> object.
 *		<li> During manifold stitching, the API makes <tt>DOUBLE_SIDED</tt> faces for an output sheet body and <tt>SINGLE_SIDED</tt> faces for an output solid body.</li>
 *		<li> Refer the technical article for properties of output bodies during non-manifold stitching.
 *		<li> Detects coincident (back-to-back) and partially coincident faces during stitching. 
 * 			 This detection depends on the coincident face detection mode set in <tt>tolerant_stitch_options</tt> object. 
 *			 The <b>Coincident Face Detection</b> section elaborates the various coincident face detection modes.</li>
 *		<li> Removes zero-area faces lying in the regions that are likely to be stitched.</li>
 *		<li> Adjusts the orientation of faces by reversing the coedges in its loops. 
 *			This may be needed in order to have consistent face normal directions across adjacent 
 * 			faces and also to have face normals point away from material side in solids.</li>
 *		<li> Orients and rearranges shells to solve shell containments. A flag controls whether this API 
 *			is allowed to make void shells or not. The <b>Shell Containment Solving</b> section 
 *	 		elaborates on the behavior of this API.</li>
 * </ul>
 * <br>
 * <b>EDGE_TOLERANT_STITCH</b> (Restricted Tolerant Stitching) 
 * <br><br>
 * An <tt>edge_tolstitch_options</tt> object needs to be is passed using the <tt>stitch_options</tt> argument for API to operate in this mode.<br><br>
 * Under this mode, this API accepts a list of 
 * <ul> 
 * <li> <tt>EDGEs</tt> </li>
 * </ul><br>
 * Only the  <tt>EDGEs</tt> appearing in the list are chosen as candidates for stitching.<br><br>
 * While performing manifold sticthing, this API accepts only free <tt>EDGEs</tt>.  A free <tt>EDGE</tt> is an edge that is associated with only one <tt>FACE</tt> and hence, has only 
 * one <tt>COEDGE</tt>. Furthermore, the top-level owner of each given free <tt>EDGE</tt> should either be 
 * a free <tt>FACE</tt> or should be a <tt>BODY</tt>.<br><br>
 * While performing non-manifold stitching, this API accepts <tt>EDGEs</tt> that are associated with one or more <tt>FACE</tt>(s) .
 * <br><br>
 * The functional behavior of this API under this mode is the same as that in <tt>tolerant_stitch</tt> mode.<br><br>
 * There is just one subtle difference; in restricted tolerant stitching, adjustment of 
 * face normals and rearrangement of shells is controlled by a flag. By default, the face normals 
 * are adjusted so that the stitched model is not void. If you are confident that all the 
 * face normals on the incoming faces are already correct, then this functionality can be switched
 * off by using the flag. If this functionality is switched off, the flag to allow this API 
 * to make void shells will have no impact. This API will not change the orientation of the shells.
 * <br><br>
 * <b>Shell Containment Solving</b>
 * <br><br>
 * @href api_stitch orients and rearranges the shells to solve shell containments. 
 * Refer to the topic <i>Shell Containment Solving</i> in the <i>Tolerant Stitching</i> Technical Article
 * for details on how stitching rearranges shells in the body(s).<br>
 * A flag controls whether this API shall be allowed to make void shells or not. This API, by default,
 * is allowed to make void shells. If this API is not allowed to make void shells, then the 
 * output shall not contain void shells. All shells in the output will be peripheral shells. 
 * New bodies shall be made in this case, if required. For details on how to set this flag, 
 * refer to documentation of @href tolerant_stitch_options.
 * <br><br>
 * <b>Caution:  </b>The shell solving mechanism assumes that every shell would either be completely 
 * inside or completely outside another shell. If shells partially intersect, then the output 
 * of the shell solving is unpredictable. Thus, if you are not sure if the shells are going to  
 * have a clean containment, then we advise that you disallow stitching from returning void shells.
 * <br><br>
 * <b>Maximum Stitch Tolerance</b>
 * <br><br>
 * In <tt>TOLERANT_STITCH</tt> and <tt>EDGE_TOLERANT_STITCH</tt> mode, you can specify the maximum stitch tolerance 
 * value by setting the <i>max_stitch_tol</i> parameter in the corresponding <tt>stitch_options</tt> object. 
 * For details of how to set this parameter, refer to documentation for @href tolerant_stitch_options.
 * <br><br>
 * Accepatable values of <i>max_stitch_tol</i> parameter:-<br>
 * <ul>
 * <li>more than or equal to <tt>SPAresabs</tt>. </li>
 * </ul>
 * Unaccepatable values of <i>max_stitch_tol</i> parameter:-<br>
 * <ul>
 * <li>less than <tt>SPAresabs</tt>. The API will fail with an error <tt>INVALID_STITCH_MAX_TOL</tt> and the state of the model will roll back to its 
 * initial state. </li>
 * </ul>
 * Recommended values of <i>max_stitch_tol</i> parameter:-<br>
 * <ul>
 * <li> Smaller than the minimum feature size and greater than the maximum gap to be stitched in the model.</li>
 * </ul>
 * Since the API removes edges that are smaller than  <i>max_stitch_tol</i> parameter, if the  max_stitch_tol value happens to be greater than the minimum 
 * feature size this API may remove edges that should have been kept. Also note that if that maximum gap is greater than the minimum feature size in the model, 
 * this API might face problems during stitching.<br><br>
 * If the max_stitch_tol is not set, the API internally uses heuristics to set up 
 * the necessary internally used stitch tolerances.
 * <br><br>
 * The only way to specify the <i>max_stitch_tol</i> parameter to this API is through @href tolerant_stitch_options. 
 * In <tt>TOLERANT_STITCH</tt> and <tt>EDGE_TOLERANT_STITCH</tt> mode, this API internally uses heuristics to set up 
 * the necessary internally used stitch tolerances. For more details refer to the <i>Stitching Tolerances</i> 
 * portion of the <i>Healing Tolerances</i> documentation.
 * <br><br>
 * <b>Coincident Face Detection</b>
 * <br><br>
 * This API detects coincident (back-to-back) and partially coincident faces during 
 * stitching. This detection depends on the coincident face detection mode set in 
 * <tt>tolerant_stitch_options</tt> or <tt>edge_tolstitch_options</tt> object passed to 
 * this API. The API is not guaranteed to detect all coincident faces. 
 * It detects coincident faces only if it encounters them in the process of stitching. The coincident faces are recorded and returned as coincident face clusters.<br><br>
 * For example, if face A is coincident with face B, and face A is coincident with face C, then faces A, B, C form one cluster. 
 * The exact methods to obtain the clusters of coincident faces can be found in the description of @href tolerant_stitch_options class.<br><br>
 * If api_stitch comes across a pair of coincident faces while attempting to stitch these faces along an edge, the behavior of the API in the three different modes is as given below
 * <br><br>
 * <ul>
 *		<li><tt>SPASTITCH_COIN_SKIP</tt> mode: Under this option, if @href api_stitch comes across a pair of coincident faces, 
 *			<ul>
 *			<li> The two faces are not stitched together along the edge. </li>
 *			<li> The coincident face pair is recorded internally and these are returned as one or more face-clusters each of which contains a 
 *			set of coincident faces detected during stitching. </li>
 *			<li> A warning of COINCIDENT_FACE is thrown. </li>
 *			<li> Once a coincident pair is detected and recorded, stitching continues. </li>
 *			</ul><br>
 *          Although coincident faces are not stitched to each other in this mode, the coincident faces may be stitched 
 *			independently to other faces. Hence, the coincident faces may ultimately belong to the same body after stitching. This can result in an invalid body. 
 *          You may obtain information about coincident faces after the first call to @href api_stitch and 	manage it appropriately. For more details, refer to the section 
 *			<i>Recommended Healing Workflow</i>.</li><br><br>
 *		<li><tt>SPASTITCH_COIN_STITCH</tt> mode: Under this option 
 *          <ul>
 *			<li> The two faces are stitched together along the edge. </li>
 *			<li> The coincident face pair is recorded internally and these are returned as one or more face-clusters each of which contains a 
 *			set of coincident faces detected during stitching. </li>
 *			<li> A warning of COINCIDENT_FACE is thrown.</li>
 *			<li> Once a coincident pair is stitched and recorded, stitching continues. </li>
 *			</ul><br>
 *			Because this produces back-to-back stitched faces or partially stitched faces,
 *			it is likely downstream ACIS operations will fail; therefore, this mode is strongly discouraged.</li><br><br>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode: Under this option 
 *			<ul> 
 *			<li> The API will immediately return with the error COINCIDENT_FACE.</li>
 *			<li> The state of the model is rolled back to the unstitched state.</li>
 *			</ul></li>
 *</ul>
 *<br>
 * Nearly coincident faces are geometrically valid, but they could be indications of possibly 
 * unintended design. Stitching nearly coincident faces can produce sharp wedge-like shapes. 
 * Such faces are stitched but reported in the <tt>outcome</tt> as a "nearly coincident faces" problem 
 * encountered by this API. For more details, refer to the section <b>Coincident Faces Detection</b>. 
 * <br><br>
 * <b>Note: </b>For information on how <tt>SPASTITCH_COIN_SKIP</tt>, <tt>SPASTITCH_COIN_STITCH</tt>, and 
 * <tt>SPASTITCH_COIN_ERROR</tt> modes interact 
 * with <tt>careful</tt> global option, refer to the section <b>Failsafe Behavior</b> of this API.
 * <br><br>
 * <b>Failsafe Behavior</b><br><br>
 * The API, @href api_stitch , has a failsafe behavior (for example, this API attempts to do as much as possible 
 * and not fail, even in cases when it encounters geometry and topology related errors). On an event 
 * of a recoverable error, this API undoes the current atomic transaction that failed due to this 
 * error, raises a sys_warning with the same error message, and proceeds further. <br>
 * The failsafe behavior of this API does not apply to "irrecoverable" errors, such as: 
 * <br><br>
 * <ul>
 *		<li><tt>INVALID_STITCH_MAX_TOL</tt></li>
 *		<li><tt>COINCIDENT_FACES (in SPASTITCH_COIN_ERROR mode)</tt></li>
 *		<li><tt>INPUT_NOT_AN_EDGE </tt></li>
 *		<li><tt>EDGE_ALREADY_STITCHED</tt></li>
 *		<li><tt>EDGE_HAS_NO_FACE</tt></li>
 *		<li><tt>NOTHING_TO_STITCH</tt></li>
 *		<li><tt>FACE_WITH_OWNER</tt></li>
 *		<li><tt>UNACCEPTABLE_ENTITY </tt></li>
 *		<li><tt>IMPROPER_STITCH_OPTION</tt></li>
 *		<li><tt>EDGE_HAS_FACE_WITH_NO_BODY</tt></li>
 *</ul>
 *<br>
 * These errors tend to indicate a programmatic mistake in the parameters being passed into this API, 
 * rather than geometric or topological errors in the bodies being stitched. Note that the failsafe 
 * behavior can be switched OFF by <i>pushing</i> a value of <tt>TRUE</tt> onto the global option <tt>careful</tt> before calling 
 * @href api_stitch (and <i>popping</i> it after the call). When the failsafe behavior is switched off, 
 * this API will fail and roll back to its initial state when the first error is encountered.
 * <br><br>
 * Coincident faces detection by this API also has a failsafe behavior. This API will roll when 
 * coincident faces are detected and <tt>careful</tt> option is set to <tt>TRUE</tt>.<br> 
 * When <tt>careful</tt> is <tt>TRUE</tt>, this API will behave as given:
 * <br><br>
 *<ul>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode - This API will roll and exit immediately.</li>
 *		<li><tt>SPASTITCH_COIN_SKIP and SPASTITCH_COIN_STITCH</tt> modes - It will record the coincident face information 
 *			and continue stitching and roll the model back to its initial state before exiting. 
 *			This API keeps the model unstitched, but provides the coincident face information.</li>
 *
 *</ul>
 *<br>
 * When the <tt>careful</tt> option is set to <tt>FALSE</tt>, this API will behave as given:
 * <br><br>
 *<ul>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode - This API will roll the model back to its initial state.</li>
 *</ul>
 *<br>
 *<b>Outcome of api_stitch and Problem Reporting</b><br><br>
 * You can determine the result of this API using the following:<br><br>
 *<ul>
 *		<li>If outcome::encountered_errors() returns <tt>FALSE</tt>, then this API has fully succeeded; that is, 
 *			errors were not encountered.</li>
 *		<li> If outcome::encountered_errors() returns <tt>TRUE</tt>, and outcome::ok() returns <tt>TRUE</tt>, then 
 *			this API has encountered error(s), yet it proceeded further successfully. 
 *			In this case, the bodies in the <tt>output_bodies</tt> list are not guaranteed to be usable for 
 *			further modelling operations.</li>
 *		<li>If outcome::ok() returns <tt>FALSE</tt>, then this API has failed and rolled the model back to 
 *			the state before this API was called.</li>
 *</ul>
 *<br>
 * This API puts information about any error or problem that it encounters during its operation into 
 * the <tt>outcome</tt> returned. For more details, refer to the documentation of @href outcome and the section 
 * <b>Failsafe Behavior</b>. This API reports following errors and problems:
 * <br><br>
 * <b> Errors</b>
 * <br><br>
 * <ul>
 *		<li>Input list has an entity which is not an <tt>EDGE</tt></li>
 *		<li>Input edge is already stitched</li>
 *		<li>Input edge is not connected to any face</li>
 *		<li>Input edges have stitch attributes already</li>
 *		<li>Coincident faces found</li>
 *		<li>Stitch max tol less than <tt>SPAresabs</tt></li>
 *		<li>Nonmanifold edges encountered</li>
 *		<li>Invalid topology encountered</li>
 *		<li>Unsupported topology encountered</li>
 *		<li>Nothing to stitch</li>
 *		<li>Improper stitch_option</li>
 *		<li>Encountered face with owner</li>
 *		<li>Top level owner of <tt>EDGE</tt> is neither a <tt>FACE</tt> nor a <tt>BODY</tt></li>
 *		<li>Shell not peripheral</li>
 *		<li>Shell reversal failed</li>
 *		<li>Shell containment check failed</li>
 *		<li>Shell orientation check failed</li>
 *		<li>Free face contains information of other faces</li>
 * </ul>
 * <br>
 * <b>Problems</b>
 * <br><br>
 * <ul>
 *		<li>Nearly coincident faces</li>
 *</ul>
 * <br>
 * <b>Limitations</b>
 * <br><br>
 * <ul>
 *		<li>In <tt>EXACT_STITCH</tt> mode, this API will operate only on <tt>BODYs</tt>.</li>
 *		<li>In <tt>TOLERANT_STITCH</tt> mode, this API will operate only on <tt>BODYs</tt> or free <tt>FACEs</tt>. 
 *			A free <tt>FACE </tt>is a <tt>FACE</tt> that is a top-level entity and has no owner. Note that free FACEs
 *			are not legal in ACIS, but this API still accepts them. It is recommended, however, that 
 *			this API be passed only <tt>BODYs.</tt></li>
 *		<li>In <tt>EDGE_TOLERANT_STITCH</tt> mode, this API will operate only on free <tt>EDGEs</tt>.
 *			A free <tt>EDGE</tt> is an edge that is associated with only one <tt>FACE</tt> and hence has only one <tt>COEDGE</tt>. Further the 
 *			top level owner of each given free or solid <tt>EDGE</tt> should either be a free <tt>FACE</tt>
 *			or should be a <tt>BODY.</tt></li>
 *		<li>If <tt>FACEs</tt> to be stitched are intended to become multiple partially or fully overlapping 
 *			<tt>BODYs</tt> (for example, connected parts in an assembly), the <tt>FACEs</tt> for each 
 *			<tt>BODY</tt> should be separately sent to this API. If all the <tt>FACEs</tt> are passed 
 *			together to this API, then the behavior of API is unpredictable. This holds true for all 
 *			modes of stitching.	</li>
 *		<li>During non-manifold stitching, this API does not handle sidedness and containment accurately. This API never marks any DOUBLE_SIDED face as BOTH_INSIDE. If you intend to stitch a model to make an internal partition 
 *			embedded in a solid, then although this API will stitch the faces, it will not get the containment correct. So such a stitched model will report check errors. 
 *			 In order to correct such a model, manually set the containment of the internal partition faces to BOTH_INSIDE.
 *		<li>If maximum gap is greater than the minimum feature size in the model, this API might face problems 
 *			during stitching. For details, refer to the section <b>Maximum Stitch Tolerance</b>.</li>
 * </ul>
 * <br>
 * <b>Effect:
 * </b><br> 
 * Changes model. This API may remove edges smaller than maximum
 * stitch tolerance and faces with zero area. This API may also split incoming
 * edges in the process of stitching.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param to_be_stitched
 * This is an input argument.
 * <ul>
 *		<li> In <tt>EXACT_STITCH</tt> mode, <tt>to_be_stitched</tt> should 
 *			contain only pointers to <tt>BODYs</tt>.</li>
 *		<li> In <tt>TOLERANT_STITCH</tt> mode, <tt>to_be_stitched</tt> should 
 *			contain only pointers to <tt>BODYs</tt> or free <tt>FACEs</tt>. A free <tt>FACE</tt> is 
 *			a <tt>FACE</tt> that is a top-level entity and has no owner. Note that free <tt>FACEs</tt>
 *			are not legal in ACIS, but the API accepts them as "construction topology." It is recommended, however,
 *			that the API be passed only <tt>BODYs</tt>.</li>	
 *		<li> In <tt>EDGE_TOLERANT_STITCH</tt> mode, <tt>to_be_stitched</tt> 
 *			should contain only pointers to free <tt>EDGEs</tt>. A free <tt>EDGE</tt> is an edge that is associated 
 *			with only one <tt>FACE</tt> and hence has only one <tt>COEDGE</tt>. 
 *			Further the top level owner of each given free <tt>EDGE</tt> should
 *			either be a free <tt>FACE</tt> or a <tt>BODY</tt>.</li>
 *</ul>
 * @param output_bodies
 * This is an output argument. The output from this API is only bodies, which 
 * will be returned in <tt>output_bodies</tt>. This is 
 * the complete set of bodies that belong to the user. In <tt>TOLERANT_STITCH</tt>
 * mode and <tt>EDGE_TOLERANT_STITCH</tt> mode, if free <tt>FACEs</tt> are passed
 * into the API, then the API will wrap these free <tt>FACEs</tt> in new 
 * <tt>BODYs</tt>, which will also be returned in the <tt>output_bodies</tt>
 * list. Note that any new <tt>BODYs</tt> created by the API will also be returned 
 * back in the <tt>new_bodies</tt> list.
 *
 * @param new_bodies
 * This is an output argument. <tt>new_bodies</tt> will contain all
 * the new bodies resulting from the API. This list will be a subset of 
 * <tt>output_bodies</tt>. This list is provided so that it is 
 * convenient to the user to find out the new bodies that the user will have to 
 * take ownership of (for example, the users may have to register new bodies in their
 * application and/or do memory management for those).
 *
 * @param sopts
 * This is an input argument. <tt>sopts</tt> encapsulates optional user
 * parameters that govern the behavior of <tt>api_stitch</tt>. <tt>sopts</tt> 
 * must be a pointer to a valid object of either of 
 * class <tt>exact_stitch_options</tt>, or class <tt>tolerant_stitch_options</tt> or 
 * class <tt>edge_tolstitch_options</tt>. The object defines the mode of @href api_stitch.
 * <ul>
 *		<li>The API operates in the <tt>EXACT_STITCH</tt> mode for an <tt>exact_stitch_options</tt> object.</li>
 *		<li>The API operates in the <tt>TOLERANT_STITCH</tt> mode for a <tt>tolerant_stitch_options</tt> object. </li>
 *		<li>The API operates in the <tt>EDGE_TOLERANT_STITCH</tt> mode for an <tt>edge_tolstitch_options</tt> object </li>
 *</ul>
 *<br>
 * There are different options available in each of these sub-classes of class <tt>stitch_options</tt>, 
 * for the different modes ofstitching. This argument <tt>sopts</tt> cannot be 
 * passed in as a <tt>NULL</tt> pointer. Further class <tt>tolerant_stitch_options</tt> or class <tt>edge_tolstitch_options</tt>
 * also support methods to query coincident-faces related information, if needed,
 * after the API completes.
 *
 * @param aopts
 * ACIS options.
 * <br><br>
 * @see stitch_options, edge_tolstitch_options, exact_stitch_options, tolerant_stitch_options
 **/

DECL_STITCH outcome api_stitch (
		const ENTITY_LIST& to_be_stitched,
		ENTITY_LIST& output_bodies,
		ENTITY_LIST& new_bodies,
        stitch_options* sopts,
		AcisOptions* aopts = NULL
);

/**
 * Performs tolerant, non-manifold stitching between parting faces and part bodies. This API 
 * specifically supports mold and die design processes.
 * <br><br>
 * <b>Role:</b> This API performs non-manifold stitching between parting faces and primary bodies. 
 * <br><br>
 *<ul>
 *	<li> This API accepts parting faces in the <tt>parting_faces</tt> list and primary bodies
 *		in the <tt>primary_bodies</tt> list. You can pass free FACEs or sheet BODYs (containing one or 
 *		more faces) as parting faces, although it is recommended that they be passed as 
 *		sheet BODYs (even if they contain single faces), because free FACEs are
 *		not legal in ACIS.</li>
 *	<li> This API performs manifold and non-manifold stitching among the parting faces.</li>
 *	<li> This API performs manifold and non-manifold stitching between the parting faces and the primary bodies.</li>
 *	<li> This API does not perform any stitching among the primary bodies or within any 
 *		given primary body; if entities that are intended to be primary bodies need to be stitched, 
 *		they should be stitched using @href api_stitch before calling <tt>api_stitch_nonmanifold</tt>. 
 *		The <tt>parting_faces</tt> list cannot be empty. If you supply an empty <tt>parting_faces</tt> list, 
 *		then the API reports back the error "Parting face list is empty". However, the <tt>primary_bodies</tt> list 
 *		may be empty, in which case the API proceeds to stitch only the parting faces.</li>
 *	</ul>
 * <br>
 * Refer to the topic <i>Non-manifold Stitching</i> in the <i>Tolerant Stitching</i> Technical Article for an example of 
 * a primary body with its parting faces. 
 * <br><br>
 * <b> Maximum Stitch Tolerance </b> 
 * <br><br>
 * You can specify the maximum stitch tolerance value by setting the <i>max_stitch_tol</i> 
 * parameter in the <tt>tolerant_stitch_options</tt> object. For details on how to set this parameter, 
 * refer to the @href tolerant_stitch_options documentation.
 * <br><br>
 * You may set the <i>max_stitch_tol</i> parameter to a value more than or equal to <tt>SPAresabs</tt>.
 * If you set the maximum stitch tolerance to a value less than <tt>SPAresabs</tt>, then the API will 
 * fail with an error INVALID_STITCH_MAX_TOL and the state of the model will roll back 
 * to its initial state. The only way to specify the <i>max_stitch_tol</i> parameter to the API
 * is through @href tolerant_stitch_options. If you do not set any value in <i>max_stitch_tol</i>, 
 * then the API uses heuristics to choose a <i>max_stitch_tol</i> by itself. However, 
 * this choice may not be suitable to the entities being stitched. The API may remove small
 * (sliver) edges smaller than the <i>max_stitch_tol</i> parameter. Hence you are strongly 
 * recommended to set the <i>max_stitch_tol</i> parameter to be smaller than the minimum feature 
 * size and greater than the maximum gap to be stitched in the model. Note that if maximum gap
 * is greater than the minimum feature size in the model, this API might face problems during stitching.
 * Refer to the topic <i>Stitching</i> in the Technical Articles 
 * for details on the internal tolerances used by the stitching algorithm.
 * <br><br>
 * <b>Coincident Face Detection </b>
 * <br><br>
 * The API, @href api_stitch_nonmanifold , detects coincident (back-to-back) and partially coincident faces during 
 * stitching. This detection depends on the coincident face detection mode set in 
 * @href tolerant_stitch_options object. <tt>api_stitch_nonmanifold</tt> is not guaranteed to detect all coincident faces. 
 * It detects coincident faces only if it encounters them in the process of stitching.The detection and processing of the 
 * coincident faces by this API depends upon the mode set in @href tolerant_stitch_options object passed to 
 * this API. The behavior under different modes is as mentioned below: 
 * <br><br>
 * <ul>
 *		<li><tt>SPASTITCH_COIN_SKIP</tt> mode: Under this option, while attempting to stitch a pair of
 *			faces along an edge, if @href api_stitch_nonmanifold comes across a pair of coincident faces, it will not 
 *			stitch the two faces together along the edge, and stitching continues. The coincident 
 *			face pair is recorded internally, and this API throws a warning of COINCIDENT_FACE. 
 *			This API optionally returns a list of one or more face-clusters, each of which contains a 
 *			set of coincident faces detected during stitching. For example, if face A is coincident 
 *			with face B, and face A is coincident with face C, then faces A, B, C form one cluster. 
 *			The exact methods to obtain the clusters of coincident faces can be found in the 
 *			description of @href tolerant_stitch_options class.</li>
 *		<li><tt>SPASTITCH_COIN_STITCH</tt> mode: Under this option, while attempting to stitch a pair 
 *			of faces along an edge, if @href api_stitch_nonmanifold comes across a pair of coincident faces, it will 
 *			stitch the two faces together along the edge, and stitching continues. 
 *			The coincident face pair is recorded internally, and this API throws a warning of 
 *			COINCIDENT_FACE. This API optionally returns a list of one or more face-clusters, each of 
 *			which contains a set of coincident faces detected during stitching. For example, if face A
 *			is coincident with face B, and face A coincident with face C, then faces A, B, C form 
 *			one cluster. The exact methods to obtain the clusters of coincident faces can be found 
 *			in the description of the @href tolerant_stitch_options class. Because this produces back-to-back stitched faces,
 *			it is likely downstream ACIS operations will fail; therefore, this mode is strongly discouraged.</li>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode: Under this option, while attempting to stitch a pair 
 *			of faces along an edge, if @href api_stitch_nonmanifold comes across a pair of coincident faces, it 
 *			will immediately return with the error COINCIDENT_FACE. The state of the model is rolled 
 *			back to the unstitched state.</li>
 * </ul>
 * <br>
 * Nearly coincident faces are geometrically valid, but they could be indications of possibly 
 * unintended design. Stitching nearly coincident faces can produce sharp wedge-like shapes. 
 * Such faces are stitched, but reported in the <tt>outcome</tt> as a "nearly coincident faces" problem 
 * encountered by this API. For more details, refer to the topic <i>Coincident Face Detection</i> 
 * in the <i>Tolerant Stitching</i> Technical Article. 
 * <br><br>
 * <b>Note</b>:  For information on how <tt> SPASTITCH_COIN_SKIP, SPASTITCH_COIN_STITCH and SPASTITCH_COIN_ERROR</tt> modes interact 
 * with the <tt>careful</tt> global option, refer to the section <b>Failsafe Behavior</b> below.
 * <br><br> 
 * <b>Shell Containment Solving</b>
 * <br><br>
 * This API orients and rearranges the shells to solve shell containments. 
 * Refer to the topic <i>Shell Containment Solving</i> in the <i>Tolerant Stitching</i> Technical Article
 * for details about how stitching rearranges shells in the body(s).
 * A flag controls whether this API is allowed to make void shells. This API, 
 * by default, is allowed to make void shells. If it is not allowed to make void shells,
 * then the output will not contain void shells. All shells in the output will be 
 * peripheral shells. New bodies will be made in this case, if required. For details 
 * on how to set this flag, refer to @href tolerant_stitch_options documentation.
 * <br><br>
 * <b>Caution:</b> 
 * <br><br>
 * <ul>
 * <li> The shell solving mechanism assumes that every shell would either be completely inside or completely
 * outside another shell. If shells partially intersect, then the output of shell solving
 * is unpredictable. If you are uncertain whether the shells will have 
 * a clean containment, then we advise that you disallow stitching from returning void 
 * shells. </li>
 * <li> This API marks all the faces of the incoming solid bodies <tt>SINGLE_SIDED</tt>. 
 * The faces of the incoming sheet bodies are marked <tt>DOUBLE_SIDED</tt> with containment 
 * as <tt>BOTH_OUTSIDE</tt>. Refer to the section <b>Limitations</b> below.</li>
 * </ul>
 * <br>
 * <b>Failsafe Behavior</b>
 * <br><br>
 * The API, @href api_stitch_nonmanifold , has a failsafe behavior (for example, this API will attempt to do as much as possible 
 * and not fail, even in cases when it encounters geometry and topology related errors). On an event 
 * of a recoverable error, this API will undo the current atomic transaction that failed due to this 
 * error, raise a sys_warning with the same error message, and proceed further. <br>
 * The failsafe behavior of this API does not apply to "irrecoverable" errors, such as:
 * <br><br>
 * <ul>
 *		<li><tt>INVALID_STITCH_MAX_TOL</tt></li>
 *		<li><tt>COINCIDENT_FACES</tt> (in <tt>SPASTITCH_COIN_ERROR</tt> mode)</li>
 *		<li><tt>INPUT_NOT_AN_EDGE </tt></li>
 *		<li><tt>NOTHING_TO_STITCH</tt></li>
 *		<li><tt>FACE_WITH_OWNER</tt></li>
 *		<li><tt>UNACCEPTABLE_ENTITY </tt></li>
 *		<li><tt>IMPROPER_STITCH_OPTION</tt></li>
 *		<li><tt>PARTING_FACE_LIST_EMPTY</tt></li>
 *</ul>
 *<br>
 * These errors tend to indicate a programmatic mistake in the parameters being passed into this API, 
 * rather than geometric or topological errors in the bodies being stitched. Note that the failsafe 
 * behavior can be switched OFF by <i>pushing</i> a value of <tt>TRUE</tt> onto the global option <tt>careful</tt> before calling 
 * @href api_stitch_nonmanifold (and <i>popping</i> it after the call). When the failsafe behavior is switched off, 
 * this API fails and rolls back to its initial state when the first error is encountered.
 * <br><br>
 * Coincident faces detection by this API also has a failsafe behavior. This API rolls when 
 * coincident faces are detected and the <tt>careful</tt> option is set to <tt>TRUE</tt>.<br> 
 * When <tt>careful</tt> is <tt>TRUE</tt>, this API behaves as given:
 * <br><br>
 *<ul>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode - This API rolls and exits immediately.</li>
 *		<li><tt>SPASTITCH_COIN_SKIP and SPASTITCH_COIN_STITCH</tt> modes - The API records the coincident face information, 
 *			continues stitching, and rolls the model back to its initial state before exiting. 
 *			This API keeps the model unstitched, but provides the coincident face information. </li>
 *
 *</ul>
 *<br>
 * When the <tt>careful</tt> option is set to <tt>FALSE</tt>, this API behaves as given:
 * <br><br>
 *<ul>
 *		<li><tt>SPASTITCH_COIN_ERROR</tt> mode - This API rolls the model back to its initial state.</li>
 *</ul>
 *<br>
 *<b>Outcome of <tt>api_stitch</tt> and Problem Reporting</b>
 * <br><br>
 * You can determine the result of this API using the following:
 * <br><br>
 *<ul>
 *		<li>If outcome::encountered_errors() returns <tt>FALSE</tt>, then this API has fully succeeded; that is,  
 *			errors were not encountered.</li>
 *		<li> If outcome::encountered_errors() returns <tt>TRUE</tt>, and outcome::ok() returns <tt>TRUE</tt>, 
 *			this means that this API has encountered error(s), yet it proceeded further successfully. 
 *			In this case, the bodies in the <tt>output_bodies</tt> list are not guaranteed to be usable for 
 *			further modelling operations.</li>
 *		<li>If outcome::ok() returns <tt>FALSE</tt>, this API has failed and rolled the model back to 
 *			the state before this API was called.</li>
 *</ul>
 *<br>
 * This API puts information about any error or problem that it encounters during its operation into 
 * the outcome returned. For more details, refer to the @href outcome documentation and the section 
 * <b>Failsafe Behavior</b> above. This API reports the following errors and problems:
 * <br><br>
 *	<b> Errors</b>
 * <br><br>
 *<ul>
 *		<li>Coincident faces found</li>
 *		<li>Stitch max tol less than <tt>SPAresabs</tt></li>
 *		<li>Nonmanifold edges encountered</li>
 *		<li>Invalid topology encountered</li>
 *		<li>Unsupported topology encountered</li>
 *		<li>Nothing to stitch</li>
 *		<li>Improper stitch_option</li>
 *		<li>Encountered face with owner</li>
 *		<li>Top level owner of <tt>EDGE</tt> is neither a <tt>FACE</tt> nor a <tt>BODY</tt></li>
 *		<li>Shell not peripheral</li>
 *		<li>Shell reversal failed</li>
 *		<li>Shell containment check failed</li>
 *		<li>Shell orientation check failed</li>
 *		<li>Free face contains information of other faces</li>
 *		<li>Parting faces list is empty</li>
 *</ul>
 * <br>
 * <b>Problems</b>
 * <br><br>
 * <ul>
 *		<li>Nearly coincident faces</li>
 *</ul>
 *<br>
 * <b>Limitations</b>
 * <br><br>
 *<ul>
 *		<li>This API operates only on BODYs or free FACEs. A free FACE is a FACE 
 *			that is a top-level entity and has no owner. Note that free FACEs are not 
 *			legal in ACIS, but this API still accepts them. We recommend, however, 
 *			that this API be passed only BODYs. </li>
 *		<li>This API does not support vertex stitching. Refer to the topic <i>Non-manifold Stitching</i> 
 *			in the <i>Tolerant Stitching</i> Technical Article for an illustration.</li>
 *		<li>This API does not always handle sidedness and containment of faces 
 *			accurately. If a user provides sheet bodies (or free faces) in parting faces
 *			list and primary bodies list for non-manifold stitching, then all the faces
 *			of the stitched non-manifold body returned by this API might be marked 
 *			<tt>DOUBLE_SIDED</tt> with containment as <tt>BOTH_OUTSIDE</tt>, even if a portion of the 
 *			non-manifold body encloses finite volume. However, if the user provides one 
 *			or more solid bodies (with or without sheet bodies) in the primary bodies
 *			list to be stitched with parting faces list, then all the faces of the 
 *			incoming solid bodies will be marked <tt>SINGLE_SIDED</tt>. The faces of the incoming
 *			sheet bodies will be marked <tt>DOUBLE_SIDED</tt> with containment as <tt>BOTH_OUTSIDE</tt>.</li>
 *		<li>This API never marks any <tt>DOUBLE_SIDED</tt> face as <tt>BOTH_INSIDE</tt>. If you intend
 *			to stitch a model to make an internal partition embedded in a solid, then 
 *			although this API will stitch the faces, it will not get the containment correct. 
 *			So such a stitched model will report check errors. In order to correct such
 *			a model, manually set the containment of the internal 
 *			partition faces to <tt>BOTH_INSIDE</tt>. Refer to the topic <i>Non-manifold Stitching</i> 
 *			in the <i>Tolerant Stitching</i> Technical Article for an illustration.</li>
 *		<li>If maximum gap is greater than the minimum feature size in the model, this API might face problems 
 *			during stitching. For details, refer to the <b>Maximum Stitch Tolerance</b> section.</li>
 *		<li> The only acceptable non-manifold processing mode in tolerant_stitch_options for this API is NM_STITCH. The API will 
*            ignore the non-manifold processing mode in tolerant_stitch_options if set to NM_DETECT 
 *			 or NM_IGNORE. A warning will be issued for this and the API will continue with non-manifold stitching.</li>
 *</ul>
 *<br>
 * <b>Effect:
 * </b><br> 
 * Changes model. This API may remove edges smaller than maximum stitch tolerance. 
 * This API may also split incoming edges in the process of stitching. 
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param parting_faces 
 * This is an input argument. <tt>parting faces</tt> should contain only pointers to 
 * BODYs or free FACEs. A free FACE is a FACE that is a top-level entity and 
 * has no owner. Note that free FACEs are not legal in ACIS, but the API still 
 * accepts them. We recommend, however, that the API be passed only BODYs.
 * Refer to the topic <i>Non-manifold Stitching</i> for an 
 * illustration of primary body and parting faces.
 *
 * @param primary_bodies
 * This is an input argument. <tt>primary_bodies</tt> should contain only 
 * pointers to BODYs or free FACEs. Refer to the topic <i>Non-manifold Stitching</i> 
 * in the <i>Tolerant Stitching</i> Technical Article for an illustration of primary body and parting faces.
 * 
 * @param output_bodies
 * This is an output argument. 
 * The output from this API is only bodies, which 
 * will be returned in <tt>output_bodies</tt>. This is 
 * the complete set of bodies that belongs to the user. Note that any new <tt>BODYs</tt> 
 * created by the API will also be returned in the <tt>new_bodies</tt> list.
 * 
 * @param new_bodies
 * This is an output argument. <tt>new_bodies</tt> will contain all
 * the new bodies resulting from the API. This list will be a subset of 
 * <tt>output_bodies</tt>. This convenient list allows you to find new bodies 
 * of which you may have to take ownership (for example, you may have to register new bodies in your 
 * application and/or do memory management for them).
 *
 * @param sopts
 * This is an input argument. <tt>sopts</tt> encapsulates optional user
 * parameters that govern the behavior of <tt>api_stitch_nonmanifold</tt>. <tt>sopts</tt> 
 * must be a pointer to a valid object of class <tt>tolerant_stitch_options</tt>. Further class 
 * <tt>tolerant_stitch_options</tt> also support methods to query coincident-faces related information, 
 * if needed, after the API completes.
 *
 * @param aopts
 * ACIS options.
 * <br><br>
 * @see tolerant_stitch_options
 **/

DECL_STITCH outcome api_stitch_nonmanifold (
		const ENTITY_LIST& parting_faces,
		const ENTITY_LIST& primary_bodies,
		ENTITY_LIST& output_bodies,
		ENTITY_LIST& new_bodies,
        tolerant_stitch_options* sopts,
		AcisOptions* aopts = NULL
);

/** @} */
#endif // (STITCHER_API_HEADER)
//----------------------------------------------------------------------------

