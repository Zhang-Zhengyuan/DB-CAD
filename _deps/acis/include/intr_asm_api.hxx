/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef INTR_ASM_API_HXX
#define INTR_ASM_API_HXX

#include "dcl_asm.h"
#include "api.hxx"

class component_handle;
class mass_props;
class mass_props_options;

class asm_model_list;
class component_entity_handle_list;

class SPAposition;
class SPAunit_vector;


/**
* @file intr_asm_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * Determines the mass properties of a component.
 * <br><br>
 * <b>Role:</b> This API finds the mass properties requested by the user.  These
 * may include the volume, center of gravity (centroid), inertia tensor, principal
 * moments, and principal axes of the given component. Options controlling the calculation 
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
 * <dl>
 * <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
 * <dd>&nbsp;A zero-length normal is specified by the <tt>mpo</tt> argument.</dd>
 * <dd>&nbsp;A negative accuracy is requested by the <tt>mpo</tt> argument.</dd>
 * <dd>&nbsp;A negative sheet thickness is specified by the <tt>mpo</tt> argument.</dd>
 * <dd>&nbsp;A void or an open, single-sided sheet was encountered.  [The user may avoid
 * this type of error by calling <tt>mpo->set_one_sided_sheet_opt()</tt> with
 * an argument of <tt>AS_2SIDED</tt> or <tt>AS_SOLID</tt>, as appropriate.  Please see the 
 * documentation of this method for further details.]</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be examined.
 * @param mp
 * mass properties computed by the API.
 * @param mpo
 * options object used to configure non-default behavior.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_mass_props(component_handle const* component,
										mass_props & mp,
										mass_props_options* mpo = NULL,
										AcisOptions* ao = NULL);


/**
 * Gets the list of <tt>component_entity_handles</tt> that are hit when a ray is fired at one or more assembly models.
 * <br><br>
 * <b>Role:</b> This assembly API fires a ray at an array of assembly models, 
 * already selected in some way, from the given ray point in the given ray direction
 * with the given ray radius. It returns a list that contains the <i>n</i> 
 * <tt>component_entity_handles</tt> nearest the ray point (where <i>n</i> is the number of
 * hits recorded) and an array that holds the <i>n</i> parameter values of the points
 * along the ray. Only <tt>component_entity_handles</tt> in the forward direction along 
 * the ray can be hit. <tt>hits_wanted</tt> indicates the maximum number of hits to return.
 * To return all hits, specify <tt>hits_wanted</tt> as zero.
 * <br><br>
 * If <tt>ray_parameters</tt> is not <tt>NULL</tt>, it must point to an array of doubles large enough
 * to hold all of the returned parameters. When <tt>ray_parameters</tt> is specified as <tt>NULL</tt>,
 * an array large enough to hold the returned parameters is allocated and its
 * address is returned. The caller is responsible for deleting the array. 
 * <br><br>
 * When several connected <tt>component_entity_handles</tt> are wanted, for example, all edges 
 * of a face, pick one <tt>component_entity_handle</tt>, for instance, the face, and the 
 * others are found by following the model pointers.
 * <br><br>
 * If the ray, that is, the straight line from a given point and a given direction,
 * hits the face at a point at least the length of the ray radius from any edge
 * or vertex of the face, it returns the face. If it hits an edge of a face or 
 * passes within ray radius of the edge, it returns the edge. If it passes within
 * ray radius of a vertex, it returns the vertex.
 * <br><br>
 * If the ray lies along the surface (planar or ruled) of a face and crosses the 
 * interior of the face, the edges or vertices of the face hit by the ray are 
 * returned. If there are no edges or vertices, the face is returned.
 * <br><br>
 * The option <tt>ray_test_control</tt> 
 * influences the behavior of the API. For details on the various values the 
 * option can take and the corresponding API behavior, refer to the documentation
 * of the <tt>ray_test_control</tt> option.
 * <br><br>
 * To pick edges or vertices, it is often helpful to increase the ray radius, but
 * to pick a face, keep the ray radius small to avoid picking edges or vertices of
 * the face.
 * <br><br>
 * If the ray hits an entity more than once, only the hit at the smallest ray 
 * parameter is returned.
 * <br><br>
 * A call to this assembly API will cause bounding boxes to be computed, causing a model
 * change and the creation of a bulletin board. To make the process read-only, 
 * wrap the call @href asmi_raytest_ents in an API_NOP_BEGIN API_NOP_END block:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = asmi_raytest_ents(...)
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
 * @param targets
 * Specifies the array of target assembly models.
 * @param ceh_hit
 * Specifies the list of <tt>component_entity_handles</tt> hits returned.
 * @param ray_parameters
 * Specifies the parameters of the ray returned.
 * @param ao
 * Specifies the ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_raytest_ents(
		SPAposition const&		ray_point,
		SPAunit_vector const&	ray_direction,
		double				ray_radius,
		int					hits_wanted,
		asm_model_list const&     targets,
		component_entity_handle_list& ceh_hit,
		double*&                ray_parameters,
	    AcisOptions * ao=NULL);


 /** @} */
#endif // INTR_ASM_API_HXX

