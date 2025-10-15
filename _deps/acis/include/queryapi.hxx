/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef QUERYAPI_HXX
#define QUERYAPI_HXX

// ------------------------------------------------------------------------------------------------

/**
 * \defgroup MODELANALYSIS Model Analysis
 *      \brief Details of Model Analysis functionality
 *			\defgroup INTRAPIRAYTEST Ray Testing
 *			\brief Test for hits between a ray and entities
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPICONTAINMENT Entity Containment
 *			\brief Determine the containment relationship between a point or EDGE and an ENTITY
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPIDISTCLASHCLEAR Entity Distance, Clash and Clearance
 *			\brief Determine the proximity of two entities  
 *				\ingroup MODELANALYSIS
 *				\defgroup INTRAPIDISTANCE Entity Distance Calculations
 *				\brief Determine the distance between two entities
 *					\ingroup INTRAPIDISTCLASHCLEAR
 *				\defgroup INTRAPICLASH Entity Clash Calculations
 *				\brief Determine whether two faces or two entities clash
 *					\ingroup INTRAPIDISTCLASHCLEAR
 *				\defgroup INTRAPICLEARANCE Entity Clearance Calculations
 *				\brief Determine the clearance of two entities 
 *					\ingroup INTRAPIDISTCLASHCLEAR
 *			\defgroup INTRAPIPHYSICALPROPS Physical (Mass) Properties
 *			\brief Determine the mass properties (volume, centroid, and inertia) of a BODY
 *				\ingroup MODELANALYSIS
 *
 */

/**
 * @file queryapi.hxx
 * @brief Contains interfaces related to Model Query
 */

// ------------------------------------------------------------------------------------------------

#include "dcl_query.h"
#include "api.hxx"
#include "alltop.hxx"
#include "ptcont.hxx"
#include "ent_pt_dist_opts.hxx"
#include "detect_match_opts.hxx"

class APOINT;
class mass_props;
class mass_props_options;
class edge_entity_rel;
class point_entity_rel;
class SPAposition;
class SPAunit_vector;
class ENTITY_LIST;
class ray;
class entity_hit_list;
class rayfire_options;
class param_info;
class body_clash_result;
class detect_match_input_handle;
class detect_match_output_handle;
class detect_match_opts;
class n_body_clash_results;

// ------------------------------------------------------------------------------------------------
/*! \addtogroup MODELANALYSIS
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/**
 * Initializes the Query library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_QUERY outcome api_initialize_query();

/**
 * Terminates the Query library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_QUERY outcome api_terminate_query();

/** @} */

// ------------------------------------------------------------------------------------------------
/** \addtogroup INTRAPICONTAINMENT
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/**
 * Determines the relationship between a given <tt>EDGE</tt> and a given <tt>APOINT</tt>, <tt>EDGE</tt>, <tt>FACE</tt>, or <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> This API determines the relationship between an edge and an
 * entity.
 * <br><br>
 * The relationships typically indicate whether the edge lies inside, outside, or
 * on the entity.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to <tt>EDGE</tt> is <tt>NULL</tt> or not to an <tt>EDGE</tt>.
 * <dd>&nbsp;Pointer to entity is <tt>NULL</tt>.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param edge
 * the given edge.
 * @param entity
 * the given entity.
 * @param rel
 * the relationship between an edge and an entity.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_edent_rel(
			EDGE            *edge,	    // Edge
			ENTITY          *entity,    // Entity
			edge_entity_rel *& rel,		// Relation Retuned
		    AcisOptions     *ao = NULL);


/**
 * Determines the relationship of given point to a given entity (<tt>POINT</tt>, <tt>EDGE</tt>, <tt>FACE</tt>, or <tt>BODY</tt>).
 * <br><br>
 * <b>Role:</b> This API determines the relationship between a point and an
 * entity. Typically, the relationship indicates whether the given point lies on,
 * inside, or outside of the given entity.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to point is <tt>NULL</tt> or not to an <tt>APOINT</tt>.
 * <dd>&nbsp;Pointer to entity is <tt>NULL</tt>.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Read only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param point
 * given point.
 * @param entity
 * given entity.
 * @param rel
 * relationship between a point and an entity returned.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_ptent_rel(
            APOINT              *point,	    // Point
			ENTITY              *entity,    // Entity
			point_entity_rel    *&rel,		// Relation Retuned
		    AcisOptions         *ao = NULL);


/**
 * Determines whether the given point lies inside, outside, or on the boundary of a given body.
 * <br><br>
 * <b>Role:</b> This API tests the point (given in global body space) against
 * the body, returning a <tt>point_containment</tt> value of <tt>point_inside</tt>, <tt>point_outside</tt>,
 * <tt>point_boundary</tt>, or <tt>point_unknown</tt>.
 * <br><br>
 * A call to this API will cause boxes to be computed, causing a model change and
 * the generation of a bulletin board.
 * <br><br>
 * If the logical <tt>use_boxes</tt> is <tt>TRUE</tt>, bounding boxes will be utilized, increasing
 * the performance when the body is definitely not a void.  If there is a chance the body is a
 * void, set to flag to <tt>FALSE</tt> so classification is correct.
 * <br><br>
 * To make the process read-only, wrap the call to <tt>api_point_in_body</tt> in an 
 * API_NOP_BEGIN / API_NOP_END block:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_point_in_body(...)
 * API_NOP_END</pre>
 * <br><br>
 * <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param test_point
 * point to test.
 * @param target_body
 * body of interest.
 * @param pc
 * inside, outside, boundary, unknown returned.
 * @param use_boxes
 * Use bounding boxes.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_point_in_body(
            SPAposition const   &test_point,	    // point to be tested
			BODY                *target_body,	    // body for test
			point_containment   &pc,                // relationship ("in", "out", "on")
            logical             use_boxes   = TRUE, // use boxes
		    AcisOptions         *ao         = NULL);
DECL_QUERY outcome gme_api_point_in_body(
            SPAposition const   &test_point,	    // point to be tested
			BODY                *target_body,	    // body for test
			point_containment   &pc,                // relationship ("in", "out", "on")
            logical             use_boxes   = TRUE, // use boxes
		    AcisOptions         *ao         = NULL);


/**
 * 判断一个face是否在一个body内，用于布尔运算操作。
 * 该接口仅适用于判断face全在body内或全在body外的情况，允许有部分在边界上。对于face部分在body内部、部分在外部的情况，无法准确判断。
 * 该接口的原理是取面非边界的一点进行点的判断。
 * @param test_face 待测试的face
 * @param target_body 待测试的body
 * @param pc 判断结果，在内部、外部、未知。
 * @param use_boxes 是否使用包围盒粗略判断。
 * @param ao ACIS选项
 * @return api调用结果outcome
 */
DECL_QUERY outcome gme_api_face_in_body(
            FACE* const   &test_face,	    // point to be tested
			BODY                *target_body,	    // body for test
			face_containment   &fc,                // relationship ("in", "out", "on")
            logical             use_boxes   = TRUE, // use boxes
		    AcisOptions         *ao         = NULL);

/** @} */

// ------------------------------------------------------------------------------------------------
/** \addtogroup INTRAPIRAYTEST
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/**
 * Gets a list of hits of a specified entity type by firing a ray at a body.
 * <br><br>
 * <b>Role:</b> This API fires a ray at a body from the given ray point in the
 * given ray direction, with given ray radius and returns a list containing
 * entities hit by the ray. Only the entities specified by <tt>type_wanted</tt> will be
 * returned; all other hits are discarded.
 * <br><br>
 * The parameter value of the ray where each entity was hit is returned in
 * <tt>ray_parameters</tt>. The caller is responsible for deleting this array.
 * <br><br>
 * A call to this API may cause boxes to be computed, changing the model and
 * creating a bulletin board. To make the process read-only, wrap the call 
 * to @href api_get_ents in an API_NOP_BEGIN / API_NOP_END block:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_get_ents(...)
 * API_NOP_END</pre>
 * <br><br>
 * to roll the modeler back to its state before <tt>api_get_ents</tt> was called.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
 * <dd>&nbsp;Zero length ray direction vector specified.</dd>
 * <dd>&nbsp;Ray radius not greater than zero.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ray_point
 * start point of ray.
 * @param ray_direction
 * direction of the ray.
 * @param ray_radius
 * radius of ray.
 * @param type_wanted
 * type of entities wanted.
 * @param target_body
 * body at which ray is to be fired.
 * @param entities_hit
 * entities hit by ray returned.
 * @param ray_parameters
 * ray parameters returned.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_get_ents(
            SPAposition const       & ray_point,    // ray_point,
		    SPAunit_vector const    & ray_direction,    // ray_direction
		    double                  ray_radius,	        // ray_radius
		    int                     type_wanted,        // type_wanted
		    BODY                    * target_body,	    // target_body
		    ENTITY_LIST             & entities_hit,	    // entities_hit
		    double                  *& ray_parameters,  // ray_parameters
	        AcisOptions             *ao = NULL);

/**
 * Gets the list of entities that are hit when a ray is fired at an input ENTITY.
 * <br><br>
 * <b>Role:</b> This API fires a ray at input ENTITY from the given ray point in the
 * given ray direction with given ray radius. It returns an entity_hit_list, which is a list 
 * of entity_hit objects. Each entity_hit consists of an entity and a parameter value of the
 * hit point along the ray. The entity_hits are returned in increasing parameter order.
 * <br><br>
 * The ray tests all faces, edges and vertices of the target entity.
 * Only entities in the forward direction along the ray can be hit, except that hits "behind" 
 * the initial position can be returned, but only those that are within the ray radius of the 
 * initial position. <tt>hits_wanted</tt> indicates the maximum number of hits to return. To 
 * return all hits, specify <tt>hits_wanted</tt> as zero.
 * <br><br>
 * When several connected entities are wanted, for example, all edges of a face, pick one
 * entity, for instance, the face, and the others are found by following the model
 * pointers.
 * <br><br>
 * If a ray intersects a FACE and one of its EDGEs within the same intersection with the ray 
 * cylinder, then a single EDGE hit would be returned. Similarly, if the ray intersected a 
 * VERTEX within the same intersecion, the single VERTEX hit will be returned.
 * <br><br>
 * If a ray has more than one distinct intersection with a FACE or an EDGE, one hit will be
 * returned for each distinct intersection.
 * <br><br>
 * The @href rayfire_options class controls optional behavior of this API. Such optional behavior
 * includes setting a maximum of one hit returned per ENTITY, or returning only hits of a specified type.
 * <br><br>
 * To pick edges or vertices, it is often helpful to increase the ray radius, but
 * to pick a face, keep the ray radius small to avoid picking edges or vertices of
 * the face. 
 * <br><br>
 * A call to this API will cause bounding boxes to be computed, causing a model 
 * change and the creation of a bulletin board. To make the process read-only, 
 * wrap the call to @href api_ray_fire in an API_NOP_BEGIN API_NOP_END block:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_ray_fire(...)
 * API_NOP_END</pre>
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to entity is <tt>NULL</tt>.
 * <dd>&nbsp;Zero length <tt>ray_direction</tt> specified.</dd>
 * <dd>&nbsp;Ray radius less than <tt>SPAresabs</tt>.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param target_entity
 * Specifies the target entity.
 * @param test_ray
 * Specifies the ray to be fired. This includes the ray start, direction, radius and maximum number of hits required.
 * @param entity_hit_list
 * Specifies the list of entity_hits to be returned.
 * @param ro
 * Specifies the rayfire_options object for optional behavior.
 * @param ao
 * Specifies ACIS options such as versioning and journaling.
 **/

DECL_QUERY outcome api_ray_fire(
							   ENTITY* target_entity, 
							   ray const& test_ray, 
							   entity_hit_list& hits, 
							   rayfire_options* ro=NULL, 
							   AcisOptions* ao=NULL
							   );
DECL_QUERY outcome gme_api_ray_fire(
							   ENTITY* target_entity,
							   ray const& test_ray,
							   entity_hit_list& hits,
							   rayfire_options* ro=NULL,
							   AcisOptions* ao=NULL
							   );

/**
 * Gets the list of entities that are hit when a ray is fired at an ENTITY_LIST.
 * <br><br>
 * <b>Role:</b> This API fires a ray at the entities in an ENTITY_LIST from the given ray point in the
 * given ray direction with given ray radius. It returns an entity_hit_list, which is a list 
 * of entity_hit objects. Each entity_hit consists of an entity and a parameter value of the
 * hit point along the ray. The entity_hits are returned in increasing parameter order.
 * <br><br>
 * The ray tests all faces, edges and vertices of all entities in the target entity list.
 * Only entities in the forward direction along the ray can be hit, except that hits "behind" 
 * the initial position can be returned, but only those that are within the ray radius of the 
 * initial position. <tt>hits_wanted</tt> indicates the maximum number of hits to return. To 
 * return all hits, specify <tt>hits_wanted</tt> as zero.
 * <br><br>
 * When several connected entities are wanted, for example, all edges of a face, pick one
 * entity, for instance, the face, and the others are found by following the model
 * pointers.
 * <br><br>
 * If a ray intersects a FACE and one of its EDGEs within the same intersection with the ray 
 * cylinder, then a single EDGE hit would be returned. Similarly, if the ray intersected a 
 * VERTEX within the same intersecion, the single VERTEX hit will be returned.
 * <br><br>
 * If a ray has more than one distinct intersection with a FACE or an EDGE, one hit will be
 * returned for each distinct intersection.
 * <br><br>
 * The @href rayfire_options class controls optional behavior of this API. Such optional behavior
 * includes setting a maximum of one hit returned per ENTITY, or returning only hits of a specified type.
 * <br><br>
 * To pick edges or vertices, it is often helpful to increase the ray radius, but
 * to pick a face, keep the ray radius small to avoid picking edges or vertices of
 * the face. 
 * <br><br>
 * A call to this API will cause bounding boxes to be computed, causing a model 
 * change and the creation of a bulletin board. To make the process read-only, 
 * wrap the call to @href api_ray_fire in an API_NOP_BEGIN API_NOP_END block:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_ray_fire(...)
 * API_NOP_END</pre>
 * <br><br>
 * <dl>
 * <b>Errors:</b> If any input entity is <tt>NULL</tt>.
 * <dd>&nbsp;Zero length <tt>ray_direction</tt> specified.</dd>
 * <dd>&nbsp;Ray radius less than <tt>SPAresabs</tt>.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param target_entitylist
 * Specifies the list of target entities.
 * @param test_ray
 * Specifies the ray to be fired. This includes the ray start, direction, radius and maximum number of hits required.
 * @param entity_hit_list
 * Specifies the list of entity_hits to be returned.
 * @param ro
 * Specifies the rayfire_options object for optional behavior.
 * @param ao
 * Specifies ACIS options such as versioning and journaling.
 **/

DECL_QUERY outcome api_ray_fire(
							   ENTITY_LIST& target_entitylist, 
							   ray const& test_ray, 
							   entity_hit_list& hits, 
							   rayfire_options* ro=NULL, 
							   AcisOptions* ao=NULL
							   );

/** @} */

/** \addtogroup INTRAPIDISTANCE
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/**
* Gets the minimum distance between two entities and the closest positions on those entities.
* @par Technical Article
* <i>[Object Relationships](http://doc.spatial.com/articles/o/b/j/Component~Object_Relationships_6947.html)</i>
* @par Role
* Using the two input entities, this API finds a position on each
* entity such that the distance between the two is the minimum distance. The
* distance as well as the two positions are returned. Supported entities include
* <tt>VERTEX</tt>, <tt>EDGE</tt>, <tt>LOOP</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>.
* @par Note
* The closest positions returned in <tt>pos1</tt> and <tt>pos2</tt> are not guaranteed to be unique. Optionally,
* if <tt>ent1_info</tt> and <tt>ent2_info</tt> are supplied, then the information returned in them is also not guaranteed to
* be unique. If <tt>ent1_info</tt>'s or <tt>ent2_info</tt>'s <tt>param_info_type</tt> is <tt>ent_is_edge</tt> 
* the parameter <tt>t</tt> is computed with respect to the underlying <tt>curve</tt>.
* @par Effect
* Read-only
* @par Journal
* Available
* @par Product(s)
* 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
* @param ent1 1st entity
* @param ent2 2nd entity
* @param pos1 1st position
* @param pos2 2nd position
* @param distance distance value
* @param ent1_info parameter values on first entity
* @param ent2_info parameter values on second entity
* @param ao ACIS options
**/
DECL_QUERY outcome api_entity_entity_distance(
            ENTITY      *ent1,
			ENTITY      *ent2,
			SPAposition &pos1,
			SPAposition &pos2,
			double      &distance,
			param_info  &ent1_info = *(param_info *) NULL_REF,
			param_info  &ent2_info = *(param_info *) NULL_REF,
		    AcisOptions *ao = NULL);


/**
* The only options class for all EED variants. 
* @par Technical Article
* <i>[Entity-Entity Distance Options](http://doc.spatial.com/articles/e/n/t/Entity-Entity_Distance_e88a.html)</i>
* @par Role
* It determines the EED variant and provides all neccessary information.
* It uses the PIMPL (pointer to implementation) idiom for implementation hiding. 
* It has private copy constructor and assignment operator to prevent copying and assigning. 
* It has public members for setting and getting a tranform and/or distance thresholds. 
* Depending one, or the other, or both, or none are set, internally the appropriate EED variation
* is chosen.
*/
class DECL_QUERY eed_options
	: public ACIS_OBJECT
{
	struct eed_options_impl;
	eed_options_impl* impl;
	eed_options(const eed_options&);
	eed_options& operator=(const eed_options&);

public:
	eed_options();
	~eed_options();
	/**
	* Set the transform for EED variants with a transform.
	* @param[in] tr The transform specifying the transformed space to compute the distance in.
	*/
	void set_transform(const SPAtransf& tr);
	/**
	* Set the distance thresholds for EED variants with distance classes.
	* @param[in] eps0 The first distance threshold, must be greater than 0 and smaller than the second threshold.
	* @param[in] eps1 The second distance threshold, must be greater than the first distance threshold.
	*/
	void set_distance_class_thresholds(double eps0, double eps1);
	/**
	* Get the provided transform.
	* @return The transform specifying the transformed space to compute the distance in.
	*/
	const SPAtransf& get_transform() const;
	/**
	* Get the provided distance thresholds.
	* @param[in] i The index indicating first (0) or second (1) distance threshold.
	* @return The i-th distance threshold.
	*/
	double get_distance_class_threshold(int i) const;
	/// @cond
	enum EED_TYPE { EED, TEED, CEED, CTEED };
	EED_TYPE get_eed_type() const;
	/// @endcond
};

/**
* The only output handle for all EED variants. 
* @par Technical Article
* <i>[Entity-Entity Distance Output Handle](http://doc.spatial.com/articles/e/n/t/Entity-Entity_Distance_e88a.html)</i>
* @par Role
* It stores all computed results. It is implemented as an abstract base class for implementation hiding. 
* It has protected constructor, copy constructor, and assignment operator. 
* Only the destructor is public, indicating that the user takes the ownership of the object.
*/
class DECL_QUERY eed_output_handle
	: public ACIS_OBJECT
{
protected:
	eed_output_handle();
	eed_output_handle(const eed_output_handle&);
	eed_output_handle& operator=(const eed_output_handle&);
public:
	/**
	* The desctructor is public, indicating the user takes ownership of instances. 
	*/
	virtual ~eed_output_handle() = 0;
};

/**
* The only interface to query the results from an @href eed_output_handle for all EED variants. 
* @par Technical Article
* <i>[Entity-Entity Distance Output Query](http://doc.spatial.com/articles/e/n/t/Entity-Entity_Distance_e88a.html)</i>
* @par Role
* It can be used for querying the computed results from the @href eed_output_handle.
* The constructor takes a pointer to an eed_output_handle to query from. 
* Depending on the EED variant, determined via the @href eed_options object,
* one can query the distance, closest positions and associated @href param_info objects for EED and EED-T, 
* or the distance class for EED-C and EED-CT. 
* If an inappropriate query is performed a sys_error will be thrown.
*/
class DECL_QUERY eed_output_query
	: public ACIS_OBJECT
{
	const eed_output_handle* oh;
public:
	/**
	* Constructor takes a pointer to an @href eed_output_handle.
	* @param[in] oh The output handle to query the results from.
	*/
	eed_output_query(const eed_output_handle *oh);
	/**
	* Get distance for EED or EED with a transform. 
	* @throw A sys_error will be thrown if distance class thresholds were provided in @href eed_options.
	* @return The distance between the two given entities.
	*/
	double get_distance() const;
	/**
	* Get the distance class for EED with distance classes or EED with distance classes and a transform.. 
	* @throw A sys_error will be thrown if no distance class thresholds were provided in @href eed_options.
	* @return The distance class the distance between the two given entities lies in.
	*/
	int get_distance_class() const;
	/**
	* Get closest points for EED or EED-T.
	* A sys_error will be thrown if queried for EED-C or EED-CT.
	* @param[in] i The index indicating the closest position on the first (0) or second (1) given entity.
	* @return The closest position on the i-th entity.
	*/
	SPAposition get_position(int i) const;
	/**
	* Get @href param_info objects associated to closest positions for EED or EED-T.
	* A sys_error will be thrown if queried for EED-C or EED-CT.
	* @param[in] i The index indicating the param_info object for the first (0) or second (1) given entity.
	* @return The param_info object associated to the closest position on the i-th entity.
	*/
	param_info get_param_info(int i) const;
};

/**
* The only API for all EED variants. 
* @par Technical Article
* <i>[Entity-Entity-Distance](http://doc.spatial.com/articles/e/n/t/Entity-Entity_Distance_e88a.html)</i>
* @par Role
* It computes the distance information according to the specified EED variant.
* It takes two entities, a pointer to an @href eed_options object, 
* a pointer to an @href AcisOptions object, and creates an @href eed_output_handle object
* from which the results can be queried via a @href eed_output_query object.
* @par Journal
* Available
* @par Effect
* Read-only
* @par Products
* 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
* @param[in] ent1 The first entity.
* @param[in] ent2 The second entity.
* @param[out] oh The output handle that has all results.
* @param[in] eo The options that determine which EED variant to use and that provide all necessary information.
* @param[in] ao The ACIS option, used for versioning and journaling.
* @return The @href outcome object indicating success or failure.
*/
DECL_QUERY outcome api_entity_entity_distance(
	ENTITY* ent1,
	ENTITY* ent2,
	eed_output_handle* &oh,
	eed_options* eo = NULL,
	AcisOptions *ao = NULL);

/**
 * Gets the minimum distance between an entity and a point, and the closest position on the entity to the point.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * Using the input entity and point, this API finds a position on
 * the entity such that the distance between the two is the minimum distance. The
 * distance and the position are returned. Supported entities include <tt>VERTEX</tt>,
 * <tt>EDGE</tt>, <tt>LOOP</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>.
 * @par Note
*  The closest position returned in <tt>closest_pos</tt> is not guaranteed to be unique. Optionally,
 * if <tt>ent_info</tt> is supplied, then the information returned in this is also not guaranteed to be unique. 
 * If <tt>ent_info</tt>'s <tt>param_info_type</tt> is <tt>ent_is_edge</tt> its parameter <tt>t</tt> is computed 
 * with respect to the underlying <tt>curve</tt>.
 * @par Effect
 * Read-only
 * @par Journal
 * Available
 * @par Product(s)
 * 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * @param ent entity
 * @param in_point point
 * @param closest_pos position on entity
 * @param distance distance value
 * @param ent_info point on entity parameter info
 * @param ao ACIS options
 **/
DECL_QUERY outcome api_entity_point_distance(
            ENTITY      *ent,
			SPAposition &in_point,
			SPAposition &closest_pos,
			double      &distance,
			param_info  &ent_info = *(param_info *) NULL_REF,
		    AcisOptions *ao = NULL);

/**
 * Gets the minimum distance between an entity and a point, and the closest position on the entity to the point.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * Using the input entity and point, this API finds a position on
 * the entity such that the distance between the two is the minimum distance. The
 * distance and the position are returned. Supported entities include <tt>VERTEX</tt>,
 * <tt>EDGE</tt>, <tt>LOOP</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>.
 * Entity point distance options can be set using the @href SPAentity_point_distance_options class.
 * @par Note
 * The closest position returned in <tt>closest_pos</tt> and the information returned in <tt>ent_info</tt>
 * are not guaranteed to be unique. If <tt>ent_info</tt>'s <tt>param_info_type</tt> is <tt>ent_is_edge</tt> 
 * its parameter <tt>t</tt> is computed with respect to the underlying <tt>curve</tt>.
 * @par Effect
 * Read-only
 * @par Journal
 * Available
 * @par Product(s)
 * 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * @param ent entity
 * @param in_point point
 * @param closest_pos position on entity
 * @param distance distance value
 * @param ent_info point on entity parameter info
 * @param epdo entity point distance options
 * @param ao ACIS options
 **/

DECL_QUERY outcome api_entity_point_distance(
            ENTITY      *ent,
			SPAposition &in_point,
			SPAposition &closest_pos,
			double      &distance,
			param_info  &ent_info,
			SPAentity_point_distance_options *epdo,
		    AcisOptions *ao = NULL);

/**
 * Computes the minimum distances and the closest positions between an entity and each point in a list.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * For each point in the list and the input entity, this API finds a position on
 * the entity such that the distance between the two is the minimum distance. Supported entities include <tt>VERTEX</tt>,
 * <tt>EDGE</tt>, <tt>LOOP</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>. 
 * The minimum distance and the position are returned in the user-supplied arrays (of length <tt>num_points</tt>): 
 * <tt>distances</tt> and <tt>closest_pos</tt>. Optionally, if you supply the array <tt>ent_infos</tt> then, 
 * for each closest position, a <tt>param_info</tt> containing the closest ENTITY and the parameters of the 
 * closest position is also returned. Entity point distance options can be set using the 
 * @href SPAentity_point_distance_options class.
 * @par Note
 * The closest positions returned in the array <tt>closest_pos</tt> are not guaranteed to be unique. Optionally,
 * if the array <tt>ent_infos</tt> is supplied, then the information returned in them is also not guaranteed to be unique.
 * If <tt>ent_info</tt>'s <tt>param_info_type</tt> is <tt>ent_is_edge</tt> its parameter <tt>t</tt> is computed 
 * with respect to the underlying <tt>curve</tt>.
 * @par Effect
 * Read-only
 * @par Journal
 * Available
 * @par Product(s)
 * 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * @param ent entity
 * @param num_points number of points in the list
 * @param in_points point list
 * @param closest_pos list of closest positions on entity
 * @param distances list of distance values
 * @param ent_infos list of entity parameter info
 * @param epdo entity point distance options
 * @param ao ACIS options
 **/
DECL_QUERY outcome api_entity_point_distance(
            ENTITY                              *ent,
            int                                 num_points,
			const SPAposition                   in_points[],
			SPAposition                         closest_pos[],
			double                              distances[],
			param_info                          ent_infos[] = (param_info *) NULL_REF,
            SPAentity_point_distance_options    *epdo = NULL,
		    AcisOptions                         *ao = NULL);

/**
 * Computes the minimum distances and the closest positions between given entities and each point in a list.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * For each point in the list and the input entities, this API finds a position on
 * the entity from the given entities such that the distance between the two is the minimum distance. 
 * Supported entities include <tt>VERTEX</tt>, <tt>EDGE</tt>, <tt>LOOP</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, 
 * <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>. 
 * The minimum distance and the position are returned in the user-supplied arrays (of length <tt>num_points</tt>): 
 * <tt>distances</tt> and <tt>closest_pos</tt>. Optionally, if you supply the array <tt>ent_infos</tt> then, 
 * for each closest position, a <tt>param_info</tt> containing the closest ENTITY and the parameters of the 
 * closest position is also returned.
 * @par Note
 * The closest positions returned in the array <tt>closest_pos</tt> are not guaranteed to be unique. Optionally,
 * if the array <tt>ent_infos</tt> is supplied, then the information returned in them is also not guaranteed to be unique.
 * The API takes a @href SPAentity_point_distance_options object (optional) as input. The options object allows you to specify the computation 
 * mode and whether or not to compute signed distances. For more details refer to @href SPAentity_point_distance_options class.
 * @par Note
 * The signed distances option has no effect if any of the entity in the given entities is a <tt>VERTEX</tt>, 
 * <tt>EDGE</tt>, or a <tt>FACE</tt> and the distances returned by the API will always be non-negative. 
 * If one of <tt>ent_infos</tt>' <tt>param_info_type</tt> is <tt>ent_is_edge</tt> its parameter <tt>t</tt> is computed 
 * with respect to the underlying <tt>curve</tt>.
 * @par Effect
 * Read-only
 * @par Journal
 * Available
 * @par Product(s)
 * 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * @param entl entity list
 * @param num_points number of points in the list
 * @param in_points point list
 * @param closest_pos list of closest positions on entity
 * @param distances list of distance values
 * @param ent_infos list of entity parameter info
 * @param epdo entity point distance options
 * @param ao ACIS options
**/
DECL_QUERY outcome api_entity_point_distance(
			ENTITY_LIST const&					entl, 
			int									num_points, 
			const SPAposition					in_points[], 
			SPAposition							closest_pos[], 
			double								distances[], 
			param_info							ent_infos[] = (param_info *) NULL_REF,
			SPAentity_point_distance_options	*epdo = NULL, 
			AcisOptions							*ao = NULL);

/**
* If this option header is set to FALSE, the single threaded entity point distance algorithm will be used even if
* ACIS has multiple worker threads in use.
**/
DECL_QUERY extern option_header epd_allow_multithreading;

/**
 * Determines if two entities are "touching" (the distance between them is less than 2 * SPAresabs).
 * <br><br>
 * <b>Role:</b> Using the two input entities, this procedure determines if they
 * are closer than 2 * <tt>SPAresabs</tt>, defaulting to <tt>find_entity_entity_distance</tt> if
 * faster checks are inconclusive. If the distance is less than 2 * <tt>SPAresabs</tt>, it
 * returns <tt>TRUE</tt>, otherwise <tt>FALSE</tt>. Supported entities include <tt>VERTEX</tt>, <tt>EDGE</tt>, <tt>LOOP</tt>,
 * <tt>FACE</tt>, <tt>WIRE</tt>, <tt>SHELL</tt>, <tt>LUMP</tt>, and <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent1
 * 1st entity.
 * @param ent2
 * 2nd entity.
 * @param touch
 * touching or not.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_entity_entity_touch(
            ENTITY      *ent1,
			ENTITY      *ent2,
			logical     &touch,
		    AcisOptions *ao = NULL);

/**
 * Determines the point on face nearest a given point in space.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param from_point
 * point from which the nearest point is sought.
 * @param face
 * face on which to find the nearest point.
 * @param to_point
 * nearest point returned.
 * @param ao
 * ACIS options.
 **/
DECL_QUERY outcome api_find_cls_ptto_face(
			const SPAposition   & from_point,   // from_point
			FACE                * face,		    // face
			SPAposition         & to_point,     // to_point
		    AcisOptions         *ao = NULL);


/** @} */

// ------------------------------------------------------------------------------------------------
/** \addtogroup INTRAPIPHYSICALPROPS
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/**
* Determines the mass properties of a body.
* <b>Technical Article:</b> <i>[Physical Properties](http://doc.spatial.com/articles/p/h/y/Component~Physical_Properties_3416.html)</i>
* <br><br>
* <b>Role:</b> This API finds the mass properties requested by the user.  These
* may include the volume, center of gravity (centroid), inertia tensor, principal
* moments, and principal axes of the given body. Options controlling the calculation 
* are specified by means of an (optional) <tt>mass_props_options</tt> object; the results 
* are returned in the form of a <tt>mass_props</tt> object.  Please refer to the 
* documentation of these two classes for details on setting options and retrieving the 
* calculated results, respectively.
* <br><br>
* When the <tt>mass_props_options</tt> argument is omitted, the following default 
* behavior is followed:
* <br><br>
* 1) projection-plane information is selected by the API;
* <br>
* 2) volume, centroid, and inertia are all calculated;
* <br>
* 3) the requested relative accuracy is one-percent;
* <br>
* 4) sheets are treated as having zero thickness; and,
* <br>
* 5) errors are thrown when voids or open, one-sided sheets are encountered.
* <br><br>
* The user should provide a <tt>mass_props_options</tt> object configured otherwise
* if some alternate behavior is preferred.
* <br><br>
* Although computation of mass properties does not make substantive changes to a
* model, bounding boxes may be calculated and cached in the model, creating a bulletin board. 
* To make the process read-only, wrap the call to <tt>api_body_mass_props</tt> 
* in an API_NOP_BEGIN / API_NOP_END block:
* <pre>
* outcome out(0);
* API_NOP_BEGIN
* out = api_body_mass_props(...)
* API_NOP_END</pre>
* <br><br>
* <b>Errors:</b> 
* <ul>
* <li>The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.</li>
* <li>A zero-length normal is specified by the <tt>mpo</tt> argument. </li>
* <li>A negative accuracy is requested by the <tt>mpo</tt> argument. </li>
* <li>A negative sheet thickness is specified by the <tt>mpo</tt> argument. </li>
* <li>A void or an open, single-sided sheet was encountered. [The user may avoid
* this type of error by calling <tt>mpo-&gt;set_one_sided_sheet_opt()</tt> with
* an argument of <tt>AS_2SIDED</tt> or <tt>AS_SOLID</tt>, as appropriate. Refer to the 
* documentation of this method for further details.]</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
* <br><br>
* @param body
* body to be examined.
* @param mp
* mass properties computed by the API.
* @param mpo
* options object used to configure non-default behavior.
* @param ao
* acis options.
**/
DECL_QUERY outcome api_body_mass_props(
			BODY                *body,				
			mass_props          &mp,				
			mass_props_options  *mpo = NULL,			
		    AcisOptions         *ao = NULL);


/**
* Determines the area of a face, shell, lump, or body.
* <br><br>
* <b>Role:</b> Calculates the area of a face, shell, lump, or body. The
* <tt>req_rel_accy</tt> is passed as a fractional number greater than zero.
* <br><br>
* <b>Errors:</b> The pointer to the entity is <tt>NULL</tt> or does not point to a body, lump,
* shell, or face. Negative accuracy requested.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* face, shell, lump, or body.
* @param req_rel_accy
* requested relative accuracy.
* @param area
* returned area of the entity.
* @param est_rel_accy_achieved
* estimate of relative accuracy returned.
* @param ao
* ACIS options.
**/
DECL_QUERY outcome api_ent_area(
			ENTITY* ent,					// object (face, shell, lump or body) to be measured
			double req_rel_accy,			// relative precision desired
			double& area,					// area returned
			double& est_rel_accy_achieved,	// estimate of relative precision achieved
			AcisOptions* ao = NULL
	);


/**
* Locates area, center of area, second moments, and principal axes of a planar face.
* <br><br>
* <b>Role:</b> Results are given in the local coordinate space of the body.
* <br><br>
* <dl>
* <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a planar face.
* <dd>&nbsp;Negative accuracy requested.</dd>
* </dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param face
* planar face to be examined.
* @param req_rel_accy
* requested relative accuracy.
* @param area
* area of face returned.
* @param centre
* center of face returned.
* @param moment_a
* moment of inertia about axis a returned.
* @param moment_b
* moment of inertia about axis b returned.
* @param axis_a
* axis a returned.
* @param axis_b
* axis b returned.
* @param est_rel_accy_achieved
* estimate of relative accuracy returned.
* @param ao
* ACIS options.
**/
DECL_QUERY outcome api_planar_face_pr(
			FACE* face,						// face to be measured
			double req_rel_accy,			// relative precision desired
			double& area,					// area returned
			SPAposition &centre,			// centroid returned
			double& moment_a,				// first principal moment
			double& moment_b,				// second principal moment
			SPAunit_vector& axis_a,			// first principal axis
			SPAunit_vector& axis_b,			// second principal axis
			double& est_rel_accy_achieved,	// estimate of relative accuracy achieved
			AcisOptions* ao = NULL
	);


/** @} */

// ------------------------------------------------------------------------------------------------
/** \addtogroup INTRAPICLASH
 *  \brief Declared at <queryapi.hxx>
 *  @{
 */

/** 
 * Enumeration of the different ways in which a pair of <tt>EDGE</tt> or
 * <tt>FACE</tt> entities can be classified during clash detection.
 * @param ENTITY_CLASH_UNKNOWN
 * Whether the entities clash has not yet been established.
 * @param ENTITY_CLASH_NONE
 * Entities do not clash at all.
 * @param ENTITY_CLASH_UNCLASSIFIED
 * Entities do clash, but more detail about how they clash is not available.
 * @param ENTITY_CLASH_COI_INSIDE
 * FACE entities which clash such that they are coincident over their region
 * of overlap, with face normals in the same direction. This implies that their
 * owning bodies share volume.
 * @param ENTITY_CLASH_COI_OUTSIDE
 * FACE entities which clash such that they are coincident over their region
 * of overlap, with face normals in the opposite directions.
 * @param ENTITY_CLASH_AINB
 * Indicates that entity A of a pair meets entity B from its material side, but
 * does not penetrate entity B.
 * @param ENTITY_CLASH_BINA
 * Indicates that entity B of a pair meets entity A from its material side, but
 * does not penetrate entity A.
 * @param ENTITY_CLASH_TOUCH
 * Indicates that entity A meets entity B from its non-material side, but 
 * A and B do not penetrate each other
 * @param ENTITY_CLASH_INTERLOCK
 * Indicates some part of the interior of each body is inside the other, 
 * and some part of the interior of each body is outside the other
 */
enum entity_clash_type {
	ENTITY_CLASH_UNKNOWN,
	ENTITY_CLASH_NONE,
	ENTITY_CLASH_UNCLASSIFIED,
	ENTITY_CLASH_COI_INSIDE,
	ENTITY_CLASH_COI_OUTSIDE,
	ENTITY_CLASH_AINB,
	ENTITY_CLASH_BINA,
	ENTITY_CLASH_TOUCH,
	ENTITY_CLASH_INTERLOCK
};

/*
 * @nodoc
 */
enum entity_clash_relation {
	ENTITY_RELATION_UNKNOWN,
	ENTITY_RELATION_INSIDE,
	ENTITY_RELATION_BOUNDARY
};


/** 
 * Enumeration of the different levels of detail/speed available for
 * @href api_clash_bodies.
 * @param CLASH_EXISTENCE_ONLY
 * Detect only whether or not the bodies clash.
 * @param CLASH_CLASSIFY_BODIES
 * Analyse the bodies sufficiently to be able to classify how they clash.
 * @param CLASH_CLASSIFY_SUB_ENTITIES
 * Analyse every pair of clashing boundary entities in the bodies.
 */
enum clash_mode {
	CLASH_EXISTENCE_ONLY,
	CLASH_CLASSIFY_BODIES,
	CLASH_CLASSIFY_SUB_ENTITIES
};

/**
 * Enumeration of the different behavior settings for @href api_clash_bodies.
 * @param CLASH_IGNORE_WIRES
 * Do not include any wires present in the bodies being tested when looking for clashes.
 * @param CLASH_REJECT_WIRES
 * Report a failed operation if any wires are present in the bodies being tested.
 */
enum clash_behaviour {
	CLASH_IGNORE_WIRES,
	CLASH_REJECT_WIRES
};

/**
 * @nodoc
 * Functionality under development
 */
class DECL_QUERY entity_clash_options : public ACIS_OBJECT 
{ 
	protected: 
		double m_clash_tolerance; 

	public: 
		// For internal use
		/*
		 * @nodoc
		 */
		entity_clash_options();

		// For internal use
		/*
		 * @nodoc
		 */
		virtual ~entity_clash_options() {};
		
		virtual double get_clash_tolerance() const; 
		
		virtual void set_clash_tolerance(double tol);
};

/**
 * @nodoc
 * Functionality under development
 */
class DECL_QUERY body_clash_options : public entity_clash_options 
{ 
	protected: 
		clash_mode m_mode;
		clash_behaviour m_behaviour;
		
	public: 

		/**
		 * Default constructor
		 */
		body_clash_options();

		/**
		 * Destructor
		 */
		virtual ~body_clash_options() {};
		
		virtual clash_mode get_clash_mode() const;
		
		virtual clash_behaviour get_clash_behaviour() const;
		
		virtual void set_clash_mode(clash_mode mod);

		virtual void set_clash_behaviour(clash_behaviour behaviour);
};

/**
 * @nodoc
 * Functionality under development
 */
class DECL_QUERY face_clash_options : public entity_clash_options
{
	public:
		/**
		 * Default constructor
		 */
		face_clash_options();

		/**
		 * Destructor
		 */
		virtual ~face_clash_options() {};
};
		
/**
 * @nodoc
 * Functionality under development
 */
DECL_QUERY outcome api_clash_bodies(
			BODY* body_a,
			BODY* body_b,
			body_clash_result& clash_result,
			body_clash_options* clashopts,
			AcisOptions* ao = NULL);

/**
 * Determines whether two bodies clash, and optionally how they clash if so.
 * <br><br>
 * <b>Role:</b> This API tests for a clash between two input bodies. The bodies
 * clash if there is a point on one body which is within SPAresabs of the other
 * body. This can be applied to closed solid bodies or sheet bodies. It 
 * can also produce information about the way in which the bodies clash, and
 * generate a full list of all of the pairs of boundary entities on the bodies
 * that clash.
 * <br><br>
 * The results of the clash test are returned in an object of type @href body_clash_result
 * passed into the API.
 * <br><br>
 * The API has three different modes of operation to allow users to trade off
 * level of detail against performance:
 * <br><br>
 * <ul>
 * <li><tt>CLASH_EXISTENCE_ONLY</tt>:<br>
 *      The API will do as little work as necessary to ascertain whether or not
 *      the bodies clash, with no analysis of the way that they clash. The
 *      result type will always be <tt>CLASH_NONE</tt> (bodies do not clash) or
 *      <tt>CLASH_UNCLASSIFIED</tt> (bodies do clash), and no boundary entity pairs or
 *      containing body will be returned.
 * </li>
 * <br><br>
 * <li><tt>CLASH_CLASSIFY_BODIES</tt>:<br>
 *      The API will do as little work as necessary to determine the way in
 *      which the bodies clash. The result type will be one of <tt>CLASH_NONE</tt>,
 *      <tt>CLASH_COINCIDENT</tt>, <tt>CLASH_CONTAINED</tt>, <tt>CLASH_CONTAINED_ABUTS</tt>, 
 *      <tt>CLASH_ABUTS</tt>,
 *      or <tt>CLASH_INTERLOCK</tt>. If one body is contained within the other, the
 *      containing_body() member will be set to the body that contains the other.
 *      No sub-entity pairs will be returned.
 * </li>
 * <br><br>
 * <li><tt>CLASH_CLASSIFY_SUB_ENTITIES</tt>:<br>
 *      The API will classify every pair of faces and wire edges in the bodies
 *      that clash, and return not only the classification for the bodies, but
 *      also a list of all sub-entity pairs that clash with their individual
 *      classifications. For each sub-entity pair returned, the "a" entry in
 *      each pair will be owned by <tt>body_a</tt> and the "b" entry will be 
 *      owned by <tt>body_b</tt>.
 * </li>
 * </ul>
 * <br><br>
 * <b>Relative speed of modes:</b>
 * In the worst case (no clash), there should be no measurable difference in time
 * taken to compute the clashes using different modes. Hence, the <em>guaranteed</em>
 * performance in any particular case is only that <tt>CLASH_EXISTENCE_ONLY</tt> calculation
 * will be no slower than either other mode, and <tt>CLASH_CLASSIFY_BODIES</tt> will be no
 * slower than <tt>CLASH_CLASSIFY_SUB_ENTITIES</tt>.
 * <br><br>
 * On average, however, it can be expected that the <tt>CLASH_EXISTENCE_ONLY</tt> operation will
 * be faster than <tt>CLASH_CLASSIFY_BODIES</tt>, which will in turn be faster than
 * <tt>CLASH_CLASSIFY_SUB_ENTITIES</tt>.
 * <br><br>
 * <b>Limitations:</b>
 * Open solid bodies are not supported. It is not possible to determine definitively
 * whether such bodies clash in an efficient manner. For performance reasons,
 * this API <em>does not</em> reject open solid bodies, and in many cases will produce
 * correct results, but the results returned are not guaranteed to be accurate,
 * or for different versions of ACIS to return the same results.
 * <br><br>
 * Currently, wire bodies and wires present in mixed-dimension bodies are not
 * supported. The default behavior is that any wires in the input bodies are
 * ignored, but this can be changed, for example, for debug builds of applications, such
 * that the API will fail if any wires are present.
 * <br><br>
 * <b>Degenerate cases:</b>
 * <ul>
 * <li>If <tt>body_a</tt> and <tt>body_b</tt> point to the same entity, then a clash 
 *     result of <tt>CLASH_COINCIDENT</tt> will be returned. In this case, a list of entity clashes
 *     is not returned. However, note that if <tt>body_b</tt> points to an <em>exact
 *     copy</em> of <tt>body_a</tt>, then the same result will be returned, but entity
 *     clash data will also be generated.</li>
 * <li>If one or both of <tt>body_a</tt> and <tt>body_b</tt> is <tt>NULL</tt>, then a clash result 
 *     of <tt>CLASH_UNKNOWN</tt> will be returned.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * <b>Journal: </b> Available
 * @param body_a
 * (in) First body to be tested for clashes.
 * @param body_b
 * (in) Second body to be tested for clashes.
 * @param result
 * (out) Results of clash test.
 * @param mode
 * (in) Operation mode for clash detection.
 * @param behavior
 * (in) Behavior modifier for clash detection operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 * <br><br>
 */
DECL_QUERY outcome api_clash_bodies(
            BODY                *body_a,
            BODY                *body_b,
            body_clash_result   &clash_result,
            clash_mode          mode = CLASH_CLASSIFY_SUB_ENTITIES,
            clash_behaviour     behaviour = CLASH_IGNORE_WIRES,
            AcisOptions         *ao = NULL);


/**
 * Determines the pairs of bodies that clash within a given list of bodies, and optionally how they clash if so.
 * <br><br>
 * <b>Role:</b> This API tests for clashes between bodies in an input list of bodies. The bodies
 * clash if there is a point on one body which is within SPAresabs of the other
 * body. This can be applied to closed solid bodies or sheet bodies. It
 * can also produce information about the way in which the bodies clash, and
 * generate a full list of all of the pairs of boundary entities on the bodies
 * that clash.
 * <br><br>
 * The results of the clash tests are returned in an object of type @href n_body_clash_results
 * passed into the API. This object keeps track of the pairs of bodies that constitute a clash.
 * It stores details of the body clashes for the bodies that clash. The level of detail
 * desired can be specified using the @href clash_mode. All the body clashes done as part of the operation
 * will correspond to the given level of detail.
 * <br><br>
 * Similar to @href api_clash_bodies, this API has three different modes of operation to allow users to trade off
 * level of detail against performance:
 * <br><br>
 * <ul>
 * <li><tt>CLASH_EXISTENCE_ONLY</tt>:<br>
 *      The API will do as little work as necessary to ascertain whether or not
 *      two bodies clash, with no analysis of the way that they clash. The
 *      result type will always be <tt>CLASH_NONE</tt> (bodies do not clash) or
 *      <tt>CLASH_UNCLASSIFIED</tt> (bodies do clash), and no boundary entity pairs or
 *      containing body will be returned.
 * </li>
 * <br><br>
 * <li><tt>CLASH_CLASSIFY_BODIES</tt>:<br>
 *      The API will do as little work as necessary to determine the way in
 *      which two bodies clash. The result type will be one of <tt>CLASH_NONE</tt>,
 *      <tt>CLASH_COINCIDENT</tt>, <tt>CLASH_CONTAINED</tt>, <tt>CLASH_CONTAINED_ABUTS</tt>,
 *      <tt>CLASH_ABUTS</tt>,
 *      or <tt>CLASH_INTERLOCK</tt>. If one body is contained within the other, the
 *      containing_body() member will be set to the body that contains the other.
 *      No sub-entity pairs will be returned.
 * </li>
 * <br><br>
 * <li><tt>CLASH_CLASSIFY_SUB_ENTITIES</tt>:<br>
 *      The API will classify every pair of faces and wire edges in the bodies
 *      that do clash, and return not only the classification for the bodies, but
 *      also a list of all sub-entity pairs that clash with their individual
 *      classifications. For each sub-entity pair returned, the "a" entry in
 *      each pair will be owned by <tt>body_a</tt> and the "b" entry will be
 *      owned by <tt>body_b</tt>.
 * </li>
 * </ul>
 * <br><br>
 * The API has failsafe behaviour, that is, if a body clash operation fails between a particular
 * pair of bodies, the API would record the failure and continue performing clashes on the rest 
 * of the input bodies. The details of the individual failures can be obtained using the @href outcome
 * object returned by the API.
 * <br><br>
 * The API can perform body clashes in a multi-threaded manner if the option, <tt>thread_hot_n_body_clash</tt> is set to TRUE.
 * <br><br>
 * <b>Limitations:</b>
 * Open solid bodies are not supported. It is not possible to determine definitively
 * whether such bodies clash in an efficient manner. For performance reasons,
 * this API <em>does not</em> reject open solid bodies, and in many cases will produce
 * correct results, but the results returned are not guaranteed to be accurate,
 * or for different versions of ACIS to return the same results.
 * <br><br>
 * Currently, wire bodies and wires present in mixed-dimension bodies are not
 * supported. The default behavior is that any wires in the input bodies are
 * ignored, but this can be changed, for example, for debug builds of applications, such
 * that the API will fail if any wires are present.
 * <br><br>
 * <b>Degenerate cases:</b>
 * <ul>
 * <li>If two bodies point to the same entity, then a clash
 *     result of <tt>CLASH_COINCIDENT</tt> will be returned. In this case, a list of entity clashes
 *     is not returned for that particular pair of bodies. However, note that if one body points to an <em>exact
 *     copy</em> of another, then the same result will be returned, but entity
 *     clash data will also be generated.</li>
 * <li>If one or both of the bodies in a clash operation is <tt>NULL</tt>, then a clash result
 *     of <tt>CLASH_UNKNOWN</tt> will be returned for that pair.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * <b>Journal: </b> Available
 * @param body_list
 * (in) List of bodies to be checked for clashes.
 * @param clash_results
 * (out) Results of clash tests between the input bodies. Stores a list of pairs of bodies that clash.
 * @param mode
 * (in) Operation mode for clash detection.
 * @param behavior
 * (in) Behavior modifier for clash detection operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 * <br><br>
 */
DECL_QUERY outcome api_n_body_clash(
			const ENTITY_LIST &body_list, 
			n_body_clash_results*& clash_results, 
			clash_mode mode = CLASH_CLASSIFY_BODIES,
			clash_behaviour behaviour = CLASH_IGNORE_WIRES,
			AcisOptions *ao = nullptr);


/**
 * @nodoc
 * Functionality under development
 */
DECL_QUERY outcome api_clash_faces(
			FACE* face_a,
			FACE* face_b,
			entity_clash_type& clash_result,
			face_clash_options* clashopts,
			AcisOptions* ao = NULL);

/**
 * Determines whether and how two faces clash.
 * <br><br>
 * <b>Role:</b> This API tests for a clash between two faces, which may be isolated or part
 * of the same or different shells, lumps or bodies. The faces clash if there is a point on
 * one face which is within SPAresabs of the other face. The result of the clash test is
 * returned as an @href entity_clash_type value. The value will be ENTITY_CLASH_NONE if the
 * faces do not clash, or one of the other values (except ENTITY_CLASH_UNKNOWN) if the faces
 * do clash.
 * <br><br>
 * Note that if faces from different bodies (or lumps or shells) do not clash, this does not
 * imply that the bodies (...) themselves do not clash, or indeed that either face does not
 * clash with the other body. It is not possible to determine in all cases whether or not two
 * bodies clash by analysing the clashes between all face pairs of the bodies. This API is
 * provided for an application to test for face clashes in specific controlled circumstances
 * (perhaps looking for a clash between specific features of two bodies), not for general
 * body-body clash determination. For general clash-testing, use @href api_clash_bodies.
 * <br><br>
 * <b>Limitations:</b> None
 * <br><br>
 * <b>Degenerate cases:</b>
 * <ul>
 * <li>If either input face pointer is NULL, the API will return ENTITY_CLASH_UNKNOWN.</li>
 * <li>If the input face pointers point to the same face, then the API will return results
 * equivalent to those if the faces were geometrically identical but distinct C++ objects.
 * The correct results in such a case is ENTITY_CLASH_COI_INSIDE if the face is single-sided,
 * and ENTITY_CLASH_COI_OUTSIDE if the face is double-sided.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* <b>Journal: </b> Available
 * @param face_a
 * (in) First face to be tested for clash.
 * @param face_b
 * (in) Second face to be tested for clash.
 * @param clash_result
 * (out) Result of face clash.
 * @param ao
 * (in) ACIS options such as version and journal.
 * <br><br>
 */
DECL_QUERY outcome api_clash_faces(
            FACE                *face_a,
            FACE                *face_b,
            entity_clash_type   &clash_result,
            AcisOptions         *ao = NULL);

/** @} */
class detect_match_options;
/**
 * \addtogroup DETECTFEATURE
 * \brief Declared at <queryapi.hxx>
 * @{

 */

/**
 * Detects matching entities for a given seed entity from set of candidate entities.
 * <br><br>
 * <b>Role:</b> For the given seed entity and detection criteria, this API detects matching entities
 * from the given set of candidate entities. If the list of candidates is empty, then the candidate 
 * entities are obtained from the owner of the seed entity through topological queries
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product:</b> 3D ACIS Modeler
 * <br><br>
 * <b>Errors:</b>
 * <br>
 * <tt>NON_ANALYTIC_INPUT:</tt> Input entity does not have analytic geometry
 * <br><br>
 *<b>Limitations:</b> Only FACEs with analytic geometry are supported as seed entity
 * @param seed
 * the seed entity to be used for detection
 * @param candidates
 * the candidate entities from which matches are to be found
 * @param matches
 * the set of matching entities obtained
 * @param mfg
 * set of matching filters that designate the detection criteria
 * @param dmo
 * detect match options
 * @param ao
 *ACIS options for versioning and journaling
 * <br><br>
 */

DECL_QUERY outcome api_detect_match( const ENTITY* seed,
					   const ENTITY_LIST& candidates,		
					   const match_filter_group& mfg,
					   ENTITY_LIST& matches,
					   detect_match_options* dmo = NULL,
					   const AcisOptions *ao = NULL
					   );

/**
 * Creation of a detect_match_input_handle which is used further downstream from matching bodies.
 * <br><br>
 * <b>Role:</b> Given a set of candidate bodies, validation of input and creation of detect_match_input_handle
 * which is required for downstream matching queries
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not available
 * <br><br>
 * <b>Product:</b> 3D ACIS Modeler
 * <br><br>
 * <b>Errors:</b>
 * <br>
 * @param candiate_bodies
 * list of input bodies that are used as candidates in performing body match
 * @param dmih
 * handle to the validated input from which we will query for a match with respect to a seed body
 */

class DECL_QUERY detect_match_input_maker
{
public:
      static outcome for_bodies( 
                                    const ENTITY_LIST& candidate_bodies,
                                    detect_match_input_handle *& dmih
                                    );
};

/**
 * Performs match detection based on the information provided in detect_match_input_handle.
 * <br><br>
 * <b>Role:</b> For the given seed body and criteria, this API detects matching bodies. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product:</b> 3D ACIS Modeler
 * <br><br>
 * <b>Errors:</b>
 * <br>
 * <b>Limitations:</b> Only solid bodies are treated as valid inputs. 
 * @param dmih
 * handle to the input candidate bodies from which we will query for a match
 * @param dmoh
 * handle to the intermediate data which is generated based on the candidate bodies in dmih
 * @param dmo
 * detect match options (for future use)
 * @param ao
 * ACIS options for versioning and journaling
 * <br><br>
 */

DECL_QUERY outcome api_detect_match( 
                                    detect_match_input_handle * dmih,
                                    detect_match_output_handle *& dmoh,
									const detect_match_opts* dmo = NULL,
                                    const AcisOptions * ao = NULL
                                    );

/** @} */
// ------------------------------------------------------------------------------------------------


#endif // QUERYAPI_HXX
