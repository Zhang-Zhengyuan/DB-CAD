/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DEPRECATED_RAYTEST_HXX
#define DEPRECATED_RAYTEST_HXX

#include <stdio.h>
#include "dcl_query.h"
#include "intrapi.hxx"

class SPAposition;
class ENTITY;
class FACE;
class SPAtransf;
class EDGE;
class VERTEX;
class param_info;
class hit;

/**
* @nodoc
*/

/**
* Gets the list of entities that are hit when a ray is fired at a body.
* <br><br>
* <b>Role:</b> This API fires a ray at a body from the given ray point in the
* given ray direction with given ray radius. It returns a list that contains the <i>n</i>
* entities nearest the ray point (where <i>n</i> is the number of hits recorded) and an
* array that holds the <i>n</i> parameter values of the points along the ray. The ray tests
* all faces, edges, and vertices of the lumps and wires of the body for recording the
* hits. Only entities in the forward direction along the ray can be hit. <tt>hits_wanted</tt>
* indicates the maximum number of hits to return. To return all hits, specify
* <tt>hits_wanted</tt> as zero.
* <br><br>
* If <tt>ray_parameters</tt> is not <tt>NULL</tt>, it must point to an array of doubles large enough
* to hold all of the returned parameters. When <tt>ray_parameters</tt> is specified as <tt>NULL</tt>,
* an array large enough to hold the returned parameters is allocated and its
* address is returned. The caller is responsible for deleting the array.
* <br><br>
* When several connected entities are wanted, for example, all edges of a face, pick one
* entity, for instance, the face, and the others are found by traversing the topology
* pointers.
* <br><br>
* If the ray, that is, the straight line from a given point and a given direction, hits
* the face at a point at least the length of the ray radius from any edge or vertex
* of the face, it returns the face. If it hits an edge of a face or passes within ray
* radius of the edge, it returns the edge. If it passes within ray radius of a vertex,
* it returns the vertex.
* <br><br>
* If the ray lies along the surface (planar or ruled) of a face and crosses the interior
* of the face, the edges or vertices of the face hit by the ray are returned. If there
* are no edges or vertices, the face is returned.
* <br><br>
* The option <tt>ray_test_control</tt> influences
* the behavior of the API. For details on the various values the option can take and
* the corresponding API behavior, refer to the documentation of the
* <tt>ray_test_control</tt> option.
* <br><br>
* To pick edges or vertices, it is often helpful to increase the ray radius, but
* to pick a face, keep the ray radius small to avoid picking edges or vertices of
* the face.
* <br><br>
* If the ray hits an entity more than once, only the hit at the smallest ray parameter is
* returned.
* <br><br>
* A call to this API will cause bounding boxes to be computed, causing a model
* change and the creation of a bulletin board. To make the process read-only,
* wrap the call to @href api_ray_test_body in an API_NOP_BEGIN API_NOP_END block:
* <pre>
* outcome out(0);
* API_NOP_BEGIN
* out = api_ray_test_body(...)
* API_NOP_END</pre>
* <br><br>
* <dl>
* <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
* <dd>&nbsp;Zero length <tt>ray_direction</tt> specified.</dd>
* <dd>&nbsp;Ray radius less than <tt>SPAresabs</tt>.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param ray_point
* Specifies the starting point of ray.
* @param ray_direction
* Specifies the direction of the ray.
* @param ray_radius
* Specifies the radius of the ray.
* @param hits_wanted
* Specifies the number of hits requested.
* @param target_body
* Specifies the target body.
* @param entities_hit
* Specifies the list of entities hit by the ray returned.
* @param ray_parameters
* Specifies the parameters of the ray returned.
* @param ao
* Specifies ACIS options such as versioning and journaling.
**/
DECL_QUERY outcome api_ray_test_body(
	SPAposition const       &ray_point,	        // ray_point
	SPAunit_vector const    &ray_direction,     // ray_direction
	double                  ray_radius,	        // ray radius
	int                     hits_wanted,        // number of hits wanted
	BODY                    *target_body,       // target_body
	ENTITY_LIST             &entities_hit,      // entities_hit
	double                  *&ray_parameters,   // array of ray_parameters (left on heap)
	AcisOptions             *ao = NULL);

/**
* @nodoc
*/

/**
* Gets the list of entities that are hit when a ray is fired at one or more entities.
* <br><br>
* <b>Role:</b> This API fires a ray at an array of entities from the given ray
* point in the given ray direction with the given ray radius. It returns a list
* that contains the <i>n</i> entities nearest the ray point (where <i>n</i> is the number of
* hits recorded) and an array that holds the <i>n</i> parameter values of the points
* along the ray. Only entities in the forward direction along the ray can be hit.
* <tt>hits_wanted</tt> indicates the maximum number of hits to return. To return all hits,
* specify <tt>hits_wanted</tt> as zero.
* <br><br>
* If <tt>ray_parameters</tt> is not <tt>NULL</tt>, it must point to an array of doubles large enough
* to hold all of the returned parameters. When <tt>ray_parameters</tt> is specified as <tt>NULL</tt>,
* an array large enough to hold the returned parameters is allocated and its
* address is returned. The caller is responsible for deleting the array.
* <br><br>
* When several connected entities are wanted, for example, all edges of a face, pick one
* entity, for instance, the face, and the others are found by following the model
* pointers.
* <br><br>
* If the ray, that is, the straight line from a given point and a given direction,
* hits the face at a point at least the length of the ray radius from any edge
* or vertex of the face, it returns the face. If it hits an edge of a face or
* passes within ray radius of the edge, it returns the edge. If it passes within
* ray radius of a vertex, it returns the vertex.
* <br><br>
* If the ray lies along the surface (planar or ruled) of a face and crosses the
* interior of the face, the edges or vertices of the face hit by the ray are returned.
* If there are no edges or vertices, the face is returned.
* <br><br>
* The option <tt>ray_test_control</tt>
* influences the behavior of the API. For details on the various values the option
* can take and the corresponding API behavior, refer to the documentation of
* the <tt>ray_test_control</tt> option.
* <br><br>
* To pick edges or vertices, it is often helpful to increase the ray radius, but
* to pick a face, keep the ray radius small to avoid picking edges or vertices of
* the face.
* <br><br>
* If the ray hits an entity more than once, only the hit at the smallest ray
* parameter is returned.
* <br><br>
* The input array of entities can be bodies, a set of faces already selected in
* some way, wires, lumps, shells, subshells, faces, edges, or vertices.
* <br><br>
* A call to this API may cause bounding boxes to be computed, causing a model
* change and the creation of a bulletin board. To make the process read-only,
* wrap the call to <tt>api_ray_test_ents</tt> in an API_NOP_BEGIN API_NOP_END block:
* <pre>
* outcome out(0);
* API_NOP_BEGIN
* out = api_ray_test_ents(...)
* API_NOP_END</pre>
* <br><br>
* <dl>
* <b>Errors:</b> Zero length <tt>ray_direction</tt> specified.
* <dd>&nbsp;Ray radius less than <tt>SPAresabs</tt>.</dd>
* <dd>&nbsp;Entity pointer in array is <tt>NULL</tt>.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param ray_point
* Specifies the starting point of the ray.
* @param ray_direction
* Specifies the direction of the ray.
* @param ray_radius
* Specifies the radius of the ray.
* @param hits_wanted
* Specifies the number of hits requested.
* @param n_target_ents
* Specifies the number of target entities.
* @param target_ents
* Specifies the array of target entities.
* @param entities_hit
* Specifies the list of entities hit returned.
* @param ray_parameters
* Specifies the parameters of the ray returned.
* @param ao
* Specifies the ACIS options such as versioning and journaling.
**/
DECL_QUERY outcome api_ray_test_ents(
	SPAposition const       &ray_point,	    // ray_point
	SPAunit_vector const    &ray_direction, // ray_direction
	double                  ray_radius,		// ray radius
	int                     hits_wanted,	// number of hits wanted
	int                     n_target_ents,  // number of target entities
	ENTITY*                 target_ents[],  // array of target entities
	ENTITY_LIST             &entities_hit,  // entities_hit
	double                  *&ray_parameters,// array of ray_parameters (left on heap)
	AcisOptions             *ao = NULL);

/**
* @nodoc
*/

/**
* Gets the list of hits when a ray is fired at a body.
* <br><br>
* <b>Role:</b> This API fires a ray at a body from the given ray point in the
* given ray direction with a given ray radius. It returns a list that contains the <i>n</i>
* hits nearest the ray point (where <i>n</i> is the number of hits recorded).
* The ray tests all faces, edges, and vertices of the lumps and wires of the
* body for recording the hits. Only entities in the forward direction along
* the ray can be hit. <tt>hits_wanted</tt> indicates the maximum number of hits to
* return. To return all hits, specify <tt>hits_wanted</tt> as zero. The list of
* hits is created on the heap and it is the responsibility of the caller to
* delete the hits in this list.
* <br><br>
* When several connected entities are wanted, for example, all edges of a face, pick one
* entity, for instance, the face, and the others are found by traversing the topology
* pointers.
* <br><br>
* If the ray, that is, the straight line from a given point and a given direction,
* hits the face at a point at least the length of the ray radius from any edge
* or vertex of the face, it returns the face. If the ray hits an edge of a face
* or passes within ray radius of the edge, it returns the edge. If the ray passes
* within ray radius of a vertex, it returns the vertex.
* <br><br>
* If the ray lies along the surface (planar or ruled) of a face and crosses the
* interior of the face, the edges or vertices of the face hit by the ray are
* returned. If there are no edges or vertices, the face is returned.
* <br><br>
* The option <tt>ray_test_control</tt>
* influences the behavior of the API. For details on the various values the option
* can take and the corresponding API behavior, refer to the documentation of
* the <tt>ray_test_control</tt> option.
* <br><br>
* To pick edges or vertices, it is often helpful to increase the ray radius. To
* pick a face, keep the ray radius small to avoid picking edges or vertices of the
* face.
* <br><br>
* If the ray hits an entity more than once, only the hit at the smallest ray
* parameter is returned.
* <br><br>
* A call to this API will cause bounding boxes to be computed, causing a model
* change and the creation of a bulletin board. To make the process read-only,
* wrap the call to @href api_raytest_body in an API_NOP_BEGIN API_NOP_END block:
* <pre>
* outcome out(0);
* API_NOP_BEGIN
* out = api_raytest_body(...)
* API_NOP_END</pre>
* <br><br>
* <dl>
* <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
* <dd>&nbsp;Zero length <tt>ray_direction</tt> specified.</dd>
* <dd>&nbsp;Ray radius less than <tt>SPAresabs</tt>.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param ray_point
* Specifies the starting point of the ray.
* @param ray_direction
* Specifies the direction of the ray.
* @param ray_radius
* Specifies the radius of the ray.
* @param hits_wanted
* Specifies the number of hits requested.
* @param target_body
* Specifies the target body.
* @param hit_list
* Specifies the list of entities hits along the ray returned.
* @param ao
* Specifies the ACIS options such as versioning and journaling.
**/
DECL_QUERY outcome api_raytest_body(
	SPAposition const       &ray_point,     // ray_point
	SPAunit_vector const    &ray_direction, // ray_direction
	double                  ray_radius,     // ray radius
	int                     hits_wanted,    // number of hits wanted
	BODY                    *target_body,   // target_body
	hit                     *&hit_list,     // list of hits (left on heap)
	AcisOptions             *ao = NULL);

/**
* @nodoc
*/

/**
* Gets the list of hits when a ray is fired at one or more entities.
* <br><br>
* <b>Role:</b> This API fires a ray at an array of entities from the given ray
* point in the given ray direction with the given ray radius. It returns a list
* that contains the <i>n</i> hits nearest the ray point (where <i>n</i> is the number of hits
* recorded.) Only entities in the forward direction along the ray can be hit.
* <tt>hits_wanted</tt> indicates the maximum number of hits to return. To return all hits,
* specify <tt>hits_wanted</tt> as zero. The list of hits is created on the heap
* and it is the responsibility of the caller to delete the hits in this list.
* <br><br>
* When several connected entities are wanted, for example, all edges of a face, pick one
* entity, for instance, the face, and the others are found by traversing the topology
* pointers.
* <br><br>
* If the ray, that is, the straight line from a given point and a given direction,
* hits the face at a point at least the length of the ray radius from any edge
* or vertex of the face, it returns the face. If it hits an edge of a face or
* passes within ray radius of the edge, it returns the edge. If it passes within
* ray radius of a vertex, it returns the vertex.
* <br><br>
* If the ray lies along the surface (planar or ruled) of a face and crosses the
* interior of the face, the edges or vertices of the face hit by the ray are returned.
* If there are no edges or vertices, the face is returned.
* <br><br>
* The option <tt>ray_test_control</tt>
* influences the behavior of the API. For details on the various values the option
* can take and the corresponding API behavior, refer to the documentation of
* the <tt>ray_test_control</tt> option.
* <br><br>
* To pick edges or vertices, it is often helpful to increase the ray radius. To
* pick a face, keep the ray radius small to avoid picking edges or vertices of the
* face.
* <br><br>
* If the ray hits an entity more than once, only the hit at the smallest ray
* parameter is returned.
* <br><br>
* The input array of entities can be bodies, a set of faces already selected
* in some way, wires, lumps, shells, subshells, faces, edges, or vertices.
* <br><br>
* A call to this API will cause bounding boxes to be computed, causing a model
* change and the creation of a bulletin board. To make the process read-only,
* wrap the call to @href api_raytest_ents in an API_NOP_BEGIN API_NOP_END block:
* <pre>
* outcome out(0);
* API_NOP_BEGIN
* out = api_raytest_ents(...)
* API_NOP_END</pre>
* <br><br>
* <dl>
* <b>Errors:</b> Zero length ray_direction specified.
* <dd>&nbsp;Ray radius less than SPAresabs.</dd>
* <dd>&nbsp;Entity pointer in array is <tt>NULL</tt>.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param ray_point
* Specifies the starting point of the ray.
* @param ray_direction
* Specifies the direction of the ray.
* @param ray_radius
* Specifies the radius of the ray.
* @param hits_wanted
* Specifies the number of hits requested.
* @param n_target_ents
* Specifies the number of target entities.
* @param target_ents
* Specifies the array of target entities.
* @param hit_list
* Specifies the hits along the ray.
* @param ao
* Specifies the ACIS options such as versioning and journaling.
**/
DECL_QUERY outcome api_raytest_ents(
	SPAposition const       &ray_point,     // ray_point
	SPAunit_vector const    &ray_direction, // ray_direction
	double                  ray_radius,     // ray radius
	int                     hits_wanted,    // number of hits wanted
	int                     n_target_ents,  // number of target entities
	ENTITY                  *target_ents[], // array of target entities
	hit                     *&hit_list,     // list of hits (left on heap)
	AcisOptions             *ao = NULL);

#endif
