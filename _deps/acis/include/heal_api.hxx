/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined (HEAL_API)
#define HEAL_API
#include "logical.h"
#include "api.hxx"
#include "dcl_heal.h"

class outcome;
class BODY;
class tighten_gaps_options;
/**
 * \defgroup HEALAPI Healing Models
 *
 * It is recommended to follow the flow described in the [Recommended_Healing_Workflow](http://doc.spatial.com/articles/r/e/c/Recommended_Healing_Workflow_4b0b.html) technical article for healing models. * 
 * \defgroup HEALAPIDEP Healing Models (Deprecated)
 * \ingroup HEALAPI
 *
 */
/**
 * @file heal_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
// ***********************************************************************
// API for Initialize and Terminate Healing
// ***********************************************************************

// Initialization API for healer husk.
/**
 * Initializes the HEAL library.
 * <br><br>
 * <b>Role:</b> This API initializes the HEAL library, which must be done before
 * any other HEAL APIs are called. This API does <i>not</i> initialize the healing process.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
**/
DECL_HEAL outcome api_initialize_healing();

// Termination API for healer husk.
/**
 * Terminates the HEAL library.
 * <br><br>
 * <b>Role:</b> This API terminates the HEAL library, which must be done after
 * all HEAL APIs are called. This API does <i>not</i> terminate the healing process. 
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
**/
DECL_HEAL outcome api_terminate_healing();
/** @} */

/**
 * \addtogroup HEALAPI
 *
 * @{
 */
/**
 * The API attempts to tighten gaps in the BODY that are bigger than the supplied
 * desired_gap_tightness value.
 * <b>Technical Article:</b> <i>[Tightening Gaps](http://doc.spatial.com/articles/t/i/g/Tightening_Gaps_3413.html)</i>
 * <br><br>
 * <b>Role:</b> The API attempts to tighten gaps in the <tt>BODY</tt> that are
 * bigger than supplied <tt>desired_gap_tightness</tt> value. A user is encouraged
 * to set the <tt>desired_gap_tightness</tt> value to the desired manufacturability
 * tolerance of the user application.<br>
 * The API uses the relevant existing algorithms of geometry building
 * for tightening gaps. For more details, refer to the "Tightening Gaps"
 * technical article in the "Healing Component" section.<br>
 * An <tt>EDGE</tt> is said to have a gap bigger than <tt>desired_gap_tightness</tt>
 * value when its underlying <tt>curve</tt> does not lie on all its topologically
 * adjoining <tt>surface</tt>s within <tt>desired_gap_tightness</tt>. Similarly,
 * a <tt>VERTEX</tt> is said to have a gap bigger than <tt>desired_gap_tightness</tt>
 * value when it does not lie on all its topologically adjoining <tt>curve</tt>s
 * and <tt>surface</tt>s within <tt>desired_gap_tightness</tt>. In order to
 * tighten such gaps the API might have to make modifications to the geometry of
 * <tt>FACE</tt>s, <tt>EDGE</tt>s and <tt>VERTEX</tt>s.
 * <br><br>
 *
 * <b>Problem reporting by <tt>api_tighten_gaps:</tt></b><br>
 * In case the API is not able to tighten some of the <tt>EDGE</tt>s/<tt>VERTEX</tt>s
 * within <tt>desired_gap_tightness</tt>, then <tt>outcome</tt> of the API, will
 * contain <tt>error_info</tt> objects reporting problem <tt>COULD_NOT_TIGHTEN_GAP</tt>.
 * These <tt>error_info</tt> objects can be obtained by calling
 * <tt>outcome::get_error_info_list()</tt>. Each such <tt>error_info</tt>
 * object will also contain a pointer to the respective <tt>EDGE</tt>/<tt>VERTEX</tt>
 * associated with the problem.<br>
 * This API has failsafe behavior. This means that if <tt>outcome::encountered_errors()</tt>
 * returns <tt>TRUE</tt>, and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then
 * it means that the API has encountered error(s), yet it proceeded further
 * successfully. In this case, the <tt>BODY</tt> is not guaranteed to be usable
 * for further modelling operations.<br>
 * For more details on problem reporting and failsafe behavior, refer to the
 * documentation of <tt>outcome</tt> and "Failsafe Behavior" technical
 * article in the "Kernel Component".<br><br>
 *
 * <b>Errors:</b><br>
 * <tt>INVALID_DESIRED_GAP_TIGHTNESS</tt> : Desired gap tightness less than
 * SPAresabs.<br>
 * <tt>WIRE_BODY_NOT_SUPPORTED</tt> : Wire body not supported<br>
 * <br><br>
 *
 * <b>Limitations:</b><br>
 * 1) The API cannot be guaranteed to tighten every gap within the
 * <tt>desired_gap_tightness</tt> value provided by the user. All the existing
 * limitations of "Geometry Building" will hold. For more details on limitations
 * of "Geometry Building", refer to [Healing Limitations](http://doc.spatial.com/articles/h/e/a/Healing_Limitations_da4e.html)technical article.<br>
 * 2) In order to tighten the gaps in a region of a <tt>BODY</tt> within
 * <tt>desired_gap_tightness</tt> value, the API may have to modify the geometry
 * in the surrounding regions even though some of the gaps may be already tight
 * within user supplied <tt>desired_gap_tightness</tt>.<br>
 * 3) The API is not supported on wires.<br><br>
 *
 * <b>Effect:</b><br>
 * In addition to modifying the geometry for tightening gaps, the API may have
 * to do the following other changes in the model in order to return back a valid
 * <tt>BODY</tt>.<br>
 * 1) Removal of sliver geometry that interfere with the surrounding valid
 * geometry.<br>
 * 2) splitting <tt>EDGE</tt>s at convexity points.
 * <br><br>
 *
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param inp_body
 * Input <tt>BODY</tt> to the API <tt>api_tighten_gaps</tt>. The API will try
 * to tighten the gaps in this <tt>BODY</tt> within the user supplied
 * <tt>desired_gap_tightness</tt> value.
 * @param desired_gap_tightness
 * Input parameter to the API <tt>api_tighten_gaps</tt>. It is a double, that
 * denotes the gap tightness value that a user desires in the body. For example, a
 * user can set in their desired <tt>desired_gap_tightness</tt> value based on
 * the manufacturability requirement of downstream application. The minimum
 * value of the parameter <tt>desired_gap_tightness</tt> is SPAresabs. If the
 * parameter <tt>desired_gap_tightness</tt> set by the user is less than
 * SPAresabs, then the API <tt>api_tighten_gaps</tt> will throw an error
 * <tt>INVALID_DESIRED_GAP_TIGHTNESS</tt> and rollback.
 * @param ao
 * ACIS options.
 * <br><br>
 */
DECL_HEAL outcome api_tighten_gaps (BODY* inp_body,
									const double& desired_gap_tightness,
									AcisOptions * ao = NULL);


/**
* The API attempts to tighten gaps in the <tt>TEDGE</tt>s and <tt>TVERTEX</tt>s of the input entities that are bigger than the supplied
* desired_gap_tightness value in the @href tighten_gaps_options object.
* <b>Technical Article:</b> <i>[Tightening Gaps](http://doc.spatial.com/articles/t/i/g/Tightening_Gaps_3413.html)</i>
* <br><br>
* <b>Role:</b> The API attempts to tighten gaps in the <tt>TEDGE</tt>s and <tt>TVERTEX</tt>s of the <tt>input entities</tt> that have tolerances
* bigger than supplied <tt>desired_gap_tightness</tt> value in the tighten_gaps_options object. Refer to the documentation of @href tighten_gaps_options class for more details.<br>
* In order to tighten such gaps_<tt>api_tighten_gaps</tt> might have to make modifications to the geometry of <tt>FACE</tt>s, <tt>EDGE</tt>s and <tt>VERTEX</tt>s of the supplied input entities and those in the surrounding regions<br><br>
* The input entities can be a single body, single or multiple lumps, shells, faces, edges and vertices.The input entities should be of the same type. 
* If the input entities are <tt>TEDGEs</tt> or <tt>TVERTEXs</tt>, they may be deleted during the process.<br><br>
* The API uses the relevant existing algorithms of geometry building for tightening gaps.<br>
*
* <b>Problem reporting by <tt>api_tighten_gaps:</tt></b><br>
* In case the API is not able to tighten some of the <tt>TEDGE</tt>s/<tt>TVERTEX</tt>s
* within <tt>desired_gap_tightness</tt> associated with input entities, then <tt>outcome</tt> of the API, will
* contain <tt>error_info</tt> objects reporting problem <tt>COULD_NOT_TIGHTEN_GAP</tt>.
* These <tt>error_info</tt> objects can be obtained by calling
* <tt>outcome::get_error_info_list()</tt>. Each such <tt>error_info</tt>
* object will also contain a pointer to the respective <tt>EDGE</tt>/<tt>VERTEX</tt>
* associated with the problem.<br> The untightened edges and vertices can also be queried from the tighten_gaps_options object.
* This API has failsafe behavior. This means that if <tt>outcome::encountered_errors()</tt>
* returns <tt>TRUE</tt>, and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then
* it means that the API has encountered error(s), yet it proceeded further
* successfully. In this case, the <tt>BODY</tt> is not guaranteed to be usable
* for further modelling operations.<br>
* For more details on problem reporting and failsafe behavior, refer to the
* documentation of <tt>outcome</tt> and "Failsafe Behavior" technical
* article in the "Kernel Component".<br><br>
*
* <b>Errors:</b><br>
* <tt>INVALID_DESIRED_GAP_TIGHTNESS</tt> : Desired gap tightness less than
* SPAresabs.<br>
* <tt>WIRE_BODY_NOT_SUPPORTED</tt> : Wire body not supported<br>
* <tt>EXPECTED_ONE_BODY_TO_TIGHTEN</tt> : Single input body expected <br>
* <tt>HEAL_WRONG_INPUT</tt>: Expected BODY, LUMP, SHELL, FACE, EDGE or VERTEX <br>
* <tt>HEAL_DIFF_OWNER_BODY</tt> : Input entities should belong to same body
* <br><br>
*
* <b>Limitations:</b><br>
* 1) The API cannot be guaranteed to tighten every gap within the
* <tt>desired_gap_tightness</tt> value provided by the user. All the existing
* limitations of "Geometry Building" will hold. For more details on limitations
* of "Geometry Building", refer to [Healing Limitations](http://doc.spatial.com/articles/h/e/a/Healing_Limitations_da4e.html) technical article.<br>
* 2) In order to tighten the gaps in a region of a <tt>BODY</tt> within
* <tt>desired_gap_tightness</tt> value, the API may have to modify the geometry
* in the surrounding regions even though some of the gaps may be already tight
* within user supplied <tt>desired_gap_tightness</tt>.<br>
* 3) The API is not supported on wires.<br><br>
*
* <b>Effect:</b><br>
* In addition to modifying the geometry for tightening gaps, the API may have
* to do the following other changes in the model in order to return back a valid
* <tt>BODY</tt>.<br>
* 1) Removal of sliver geometry that interfere with the surrounding valid
* geometry.<br>
* 2) splitting <tt>EDGE</tt>s at convexity points.
* <br><br>
*
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param input_entities
* Input <tt>ENTITY_LIST</tt> to the API <tt>api_tighten_gaps</tt>. The API will try
* to tighten the gaps in the entities in this <tt>ENTITY_LIST</tt> within the user supplied
* <tt>desired_gap_tightness</tt> value in the tighten_gaps_options object.
* @param tighten_gaps_options
* Input parameter to the API <tt>api_tighten_gaps</tt>. It is a pointer to tighten_gaps_options class where a user can supply 
* the desired gap tightness value in the input entities. For example, a
* user can set in their desired <tt>desired_gap_tightness</tt> value based on
* the manufacturability requirement of downstream application. The minimum
* value of the parameter <tt>desired_gap_tightness</tt> is SPAresabs. If the
* parameter <tt>desired_gap_tightness</tt> set by the user is less than
* SPAresabs, then the API <tt>api_tighten_gaps</tt> will throw an error
* <tt>INVALID_DESIRED_GAP_TIGHTNESS</tt> and rollback. A user can also choose to tighten only tangent analytic junctions, or isospline junctions
* or sharp junctions or a combination using the tighten_gaps_options class.
* @param ao
* ACIS options.
* <br><br>
*/
DECL_HEAL
outcome api_tighten_gaps(ENTITY_LIST& input_entities,
	tighten_gaps_options* tgopts,
	AcisOptions* ao = nullptr);


/** @} */
#endif

