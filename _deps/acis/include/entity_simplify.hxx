/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// bmt 26Jun08 - Add option to limit surface testing to face box, plus a bit of padding
#if !defined (ENTITY_SIMPLIFY)
#define ENTITY_SIMPLIFY
#include "logical.h"
#include "api.hxx"
#include "dcl_heal.h"
/**
 * @file entity_simplify.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPI
 *
 * @{
 */
/**
 * This class contains parameters that govern the behavior of @href api_simplify_entity.
 * <br>
 * <b>Role:</b>  The following table lists parameters available in the @href simplify_options class.
 * The constructor of @href simplify_options initializes all the parameters to their default value. 
 * Refer to @href api_simplify_entity documentation for the parameter default values. 
 * You can use the <tt>set</tt> functions mentioned below to change any of the 
 * parameters and the <tt>get</tt> functions to obtain the current value of the parameter set into a 
 * @href simplify_options object. The acceptable value for each parameter is indicated in
 * the following table:
 * <br><br>
 * <table width = 100% border = 1>
 * <tr>
 * <td width="16%" valign="top"><b>Parameters</b></td>
 * <td width="30%" valign="top"><b>GET routines</b></td>
 * <td width="30%" valign="top"><b>SET routines</b></td>
 * <td width="12%" valign="top"><b>Acceptable values</b></td>
 * <td width="8%" valign="top"><b>Default values</b></td>
 * <td width="%" valign="top"><b>Description</b></td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>simplification_tol</tt></td>
 * <td width="30%" valign="top">simplification_tol()</td>
 * <td width="30%" valign="top">set_simplification_tol(double)</td>
 * <td width="12%" valign="top">-1 (default), <br>any positive value.</td>
 * <td width="8%" valign="top"><tt>SPAresabs</tt></td>
 * <td width="%" valign="top">api_simplify_entity replaces the %spline geometry by analytic geometry only if possible within this user-specified tolerance.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>max_radius</tt></td>
 * <td width="30%" valign="top">max_radius()</td>
 * <td width="30%" valign="top">set_max_radius(double)</td>
 * <td width="12%" valign="top">-1 (default), <br>any positive value.</td>
 * <td width="8%" valign="top"><tt>SPAresabs/SPAresnor</tt></td>
 * <td width="%" valign="top">api_simplify_entity will not make any analytic geometry whose radius is greater than the value of this parameter.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_curve_simplification</tt></td>
 * <td width="30%" valign="top">do_curve_simplification()</td>
 * <td width="30%" valign="top">set_do_curve_simplification(int)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">1 (ON)</td>
 * <td width="%" valign="top">%curve simplification is attempted only if this parameter is ON.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_surface_simplification</tt></td>
 * <td width="30%" valign="top">do_surface_simplification()</td>
 * <td width="30%" valign="top">set_do_surface_simplification(int)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">1 (ON)</td>
 * <td width="%" valign="top">%surface simplification is attempted only if this parameter is ON.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_elliptical_cylinder_simplification</tt></td>
 * <td width="30%" valign="top">do_elliptical_cylinder_simplification()</td>
 * <td width="30%" valign="top">set_do_elliptical_cylinder_simplification(logical)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">1 (ON)</td>
 * <td width="%" valign="top">elliptical cylinder simplification is attempted only if this parameter is ON. Note that this option has an effect only if <tt>do_surface_simplification</tt> is ON.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_elliptical_cone_simplification</tt></td>
 * <td width="30%" valign="top">do_elliptical_cone_simplification()</td>
 * <td width="30%" valign="top">set_do_elliptical_cone_simplification(logical)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">1 (ON)</td>
 * <td width="%" valign="top">elliptical %cone simplification is attempted only if this parameter is ON. Note that this option has an effect only if <tt>do_surface_simplification</tt> is ON.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_approximate</tt></td>
 * <td width="30%" valign="top">do_approximate()</td>
 * <td width="30%" valign="top">set_do_approximate(int)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">0 (OFF)</td>
 * <td width="%" valign="top">option to replace the existing procedural geometry by its approximate B-Spline geometry, in case simplification with analytic geometry is not possible within the user-specified tolerance.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_force_simplification</tt></td>
 * <td width="30%" valign="top">do_force_simplification()</td>
 * <td width="30%" valign="top">set_do_force_simplification(int)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">0 (OFF)</td>
 * <td width="%" valign="top">If this parameter is ON, then api_simplify_entity will simplify a %surface or %curve (if possible within the user-specified tolerance) even if simplification would result in invalidities in the entity.</td>
 * </tr>
 * <tr>
 * <td width="16%" valign="top"><tt>do_canonic_simplification</tt></td>
 * <td width="30%" valign="top">do_canonic_simplification()</td>
 * <td width="30%" valign="top">set_do_canonic_simplification(int)</td>
 * <td width="12%" valign="top">-1 (default),<br>0 (OFF),<br>1 (ON).</td>
 * <td width="8%" valign="top">0 (OFF)</td>
 * <td width="%" valign="top">Canonic %surface simplification is attempted only if this parameter is ON.</td>
 * </tr>
 * </table>
 * <br>
 * A sample snippet code to illustrate the usage is given below: <br>
 * <pre>
 * simplify_options simp_opts;
 * simp_opts.set_do_curve_simplification(0); // Switch OFF curve simplification
 * outcome result = api_simplify_entity (body, &simp_opts);
 * </pre>
 * <br> 
 *
 * @see api_simplify_entity
 * 
 */
class DECL_HEAL simplify_options : public ACIS_OBJECT
{
protected:

	double m_simplify_pos_tol;
	double m_max_radius;
	int m_do_curve_simplification;
	int m_do_surface_simplification;
	logical m_allow_elliptical_cylinder_simplification;
	logical m_allow_elliptical_cone_simplification;
	int m_do_approximate;
	int m_do_force_simplification;
	int m_limit_surfs_to_faces;
	logical m_do_canonic_simplification;
	

public:
/**
 * C++ initialize constructor requests memory for this object and initializes the values of all the parameters as -1. 
 * <br><br>
 * <b>Role:</b> The constructor initializes the values of all the parameters as -1. A "-1" value of a parameter 
 * instructs <tt>api_simplify_entity</tt> to use the default value of the particular parameter.
 **/
	simplify_options();

	// Get functions
/**
 * Returns the value of the <tt>simplification_tol</tt> parameter.
 **/
	double simplification_tol() const;
/**
 * Returns the value of the <tt>max_radius</tt> parameter.
 **/
	double max_radius() const;
/**
 * Returns the value of the <tt>do_curve_simplification</tt> parameter.
 **/
	int do_curve_simplification() const;
/**
 * Returns the value of the <tt>do_surface_simplification</tt> parameter.
 **/
	int do_surface_simplification() const;
/**
 * Returns the value of the <tt>do_elliptical_cylinder_simplification</tt> parameter.
 **/
	logical do_elliptical_cylinder_simplification() const;
/**
 * Returns the value of the <tt>do_elliptical_cone_simplification</tt> parameter.
 **/
	logical do_elliptical_cone_simplification() const;
/**
 * Returns the value of the <tt>do_approximate</tt> parameter.
 **/
	int do_approximate() const;
/**
 * Returns the value of the <tt>do_force_simplification</tt> parameter.
 **/
	int do_force_simplification() const;
/**
 * Returns <tt>TRUE</tt> if face simplification will limit consideration of the %surface to the face's parameter range.
 **/
	logical do_limit_surfs_to_faces() const;

/**
* Returns the value of the <tt>do_canonic_simplification</tt> parameter
**/
	logical do_canonic_simplification() const;

	// Set Functions

/**
 * Sets the value of the <tt>simplification_tol</tt> parameter.
 * <br><br>
 * @param tol
 * simplification tolerance.
 **/
	void set_simplification_tol(double tol);
/**
 * Sets the value of the <tt>max_radius</tt> parameter.
 * <br><br>
 * @param radius
 * Maximum curve radius.
 **/
	void set_max_radius(double radius);
/**
 * Sets the value of the <tt>do_curve_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_curve_simplification(int val);
/**
 * Sets the value of the <tt>do_surface_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_surface_simplification(int val);
/**
 * Sets the value of the <tt>do_elliptical_cylinder_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_elliptical_cylinder_simplification(logical allow_elliptical_cylinder);
/**
 * Sets the value of the <tt>do_elliptical_cone_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_elliptical_cone_simplification(logical allow_elliptical_cone);

/**
 * Sets the value of the <tt>do_approximate</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_approximate(int val);
/**
 * Sets the value of the <tt>do_force_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
 * <br><br>
 * @param val
 * Takes values -1 (default), 0 (OFF), 1 (ON).
 **/
	void set_do_force_simplification(int val);

/**
 * Sets the value switching whether face simplification limits consideration of the %surface to the face's parameter range.
* <br><br>
 * @param val
 * Takes values -1 (use default setting), 0 (OFF), 1 (ON).
 **/
	void set_do_limit_surfs_to_faces(int val);

/**
* Sets the value of the <tt>do_canonic_simplification</tt> parameter to <tt>ON</tt> or <tt>OFF</tt>.
* <br><br>
* @param val
* Takes values -1 (default), 0 (OFF), 1 (ON).
**/
	void set_do_canonic_simplification(int val);

/**
   * @nodoc
 */
	void populate_defaults();
};

// API for simplification
/**
 * API for curve and surface simplification.
 * <br><b>Technical Article:</b> <i>[Geometry Simplification](http://doc.spatial.com/articles/g/e/o/Geometry_Simplification_ca66.html)</i>
 * <br><br>
 * <b>Role:</b> <tt>api_simplify_entity</tt> replaces the geometry of the faces and edges under the given entity 
 * by analytic surfaces and curves, if possible within the user-specified tolerance. The API also provides 
 * the user with an option to replace the existing procedural geometry by its approximate B-Spline geometry, 
 * in case simplification with analytic geometry is not possible within the user-specified tolerance.
 * <br><br>
 * The API may have to make the edges it simplified tolerant edges. If the incoming entity 
 * already has tolerant edges, then the API may have to update the tolerances of those edges. 
 * <br><br>
 * During its processing, if the API finds that simplification of a particular <tt>surface</tt> or a <tt>curve</tt> 
 * would result in an entity with invalidities (those which cannot be fixed by making the entity tolerant), 
 * then the API will refuse to simplify that particular <tt>surface</tt> or <tt>curve</tt>. This is the default behavior of 
 * the API. The API however provides the user with an option to force simplification, in which case the API 
 * would go ahead and simplify the <tt>surface</tt> or <tt>curve</tt> (if possible within the user-specified tolerance) even 
 * if simplification would result in invalidities in the entity.
 * <br><br>
 * The API is <b>Failsafe</b>. The API has been broken up internally into a sequence of repetitive atomic 
 * operations (for example, an atomic operation could be simplification of a single face or a single edge). 
 * If a particular operation raises an error, then the API undoes the atomic operation, warns of the failure, 
 * and proceeds with subsequent atomic operations. This allows the API to complete with a partial success rather 
 * than total success or failure. Note that if the API encounters errors, then the entity resulting from the API 
 * should be examined for problems associated with the geometry or topology that might have caused the error(s). 
 * <br><br>
 * Setting the <tt>careful</tt> option to <tt>TRUE</tt> will turn off Failsafe behavior, in which case the model is completely 
 * rolled back on the occurrence of the first unhandled error. 
 * <br><br>
 * <b>Limitations: </b> The API will not simplify a spline face to an elliptical cone with a degenerate edge.<br><br>
 * <b>Errors:</b> 
 * The API shall error out in the cases when the <tt>ENTITY</tt> passed by a user does not have any <tt>FACE</tt> or <tt>EDGE</tt> 
 * or <tt>TEDGE</tt> under it (e.g., a <tt>COEDGE</tt> or <tt>VERTEX</tt>), or if it is a non-topological entity (e.g., a <tt>CURVE</tt> or <tt>SURFACE</tt>). 
 * <br><br>
 * <b>Effect:</b> 
 * <br>
 * <dl><dd>
 * 1. The API may have to make the edges in the simplified entity tolerant if needed. If the incoming entity 
 * already has tolerant edges, then the API may have to update the tolerances of those edges.
 * <br><br>
 * 2. If the user passes an <tt>EDGE</tt> and if the <tt>EDGE</tt> gets simplified and made tolerant, then the API will 
 * delete the incoming <tt>EDGE</tt> and return the new <tt>TEDGE</tt> pointer that replaces the <tt>EDGE</tt> pointer.
 * <br><br>
 * 3. It should be noted that in addition to replacing the spline geometry of <tt>FACE</tt>/<tt>EDGE</tt>/<tt>TEDGE</tt> by analytic 
 * geometry, the API may need to additionally modify the following, which may not be explicitly evident from 
 * the API name. 
 * <br><br>
 * <dl><dd>
 * i. <tt>COEDGE</tt> geometry (in case the old pcurve does not suit the simplified <tt>FACE</tt>/<tt>EDGE</tt>).
 * <br>
 * ii. <tt>TCOEDGE</tt> geometry (in case the old pcurve or 3D geometry of <tt>TCOEDGE</tt> does not suit the simplified <tt>FACE</tt>/<tt>TEDGE</tt>)
 * <br>
 * iii. Tolerances of <tt>TEDGEs</tt>.
 * <br>
 * iv. Tolerances of <tt>TVERTEXs</tt>.
 * <br>
 * </dl></dd>
 * </dl></dd>
 * <br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param input_entity
 * <tt>ENTITY *& input_entity</tt>. The API takes in an <tt>ENTITY</tt> pointer (by reference). The API attempts 
 * to simplify the faces and edges under this given entity. For example, if a <tt>FACE</tt> is supplied, the API 
 * will attempt to simply the <tt>FACE</tt> and all the <tt>EDGEs</tt>/<tt>TEDGEs</tt> in this <tt>FACE</tt>. In case the user passes a <tt>BODY</tt>, 
 * the API attempts to simplify all the FACEs and the <tt>EDGEs</tt>/<tt>TEDGEs</tt> in the body. If the user passes an <tt>EDGE</tt> 
 * and if the <tt>EDGE</tt> gets simplified and made tolerant, then the API will delete the incoming <tt>EDGE</tt> and return 
 * the new <tt>TEDGE</tt> pointer that replaces the <tt>EDGE</tt> pointer.
 * <br><br>
 * @param simp_options
 * <tt>simplify_options* simp_options</tt>. The API takes in a pointer to an object of <tt>simplify_options</tt> class.
 * This class contains parameters that govern the behavior of the API. These data fields can be set using 
 * the interface provided by this class. If a user explicitly passes a <tt>NULL</tt>, the behavior of the API would 
 * be as if it was given a <tt>simplify_options</tt> object with default value of parameters.
 * <br><br>
 * The constructor of <tt>simplify_options</tt> sets the values of all the parameters as -1. The value of -1 for any 
 * of the parameters of <tt>simplify_options</tt> indicates to <tt>api_simplify_entity</tt> that it should use default values 
 * for those. The default values that are chosen by the API are: 
 * <br><br>
 * <dl><dd>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
 * <td valign = "top">  <tt>SPAresabs</tt></td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>max_radius</tt></td>
 * <td valign = "top">  <tt>(SPAresabs/SPAresnor)</tt></td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_curve_simplification</tt></td>
 * <td valign = "top">  1 (that is, curve-simplification is <tt>ON</tt>)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_surface_simplification</tt></td>
 * <td valign = "top">  1 (that is, surface-simplification is <tt>ON</tt>)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_elliptical_cylinder_simplification    </tt></td>
 * <td valign = "top">  <tt>TRUE</tt> (that is, elliptical cylinder simplification is <tt>ON</tt>)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_elliptical_cone_simplification    </tt></td>
 * <td valign = "top">  <tt>TRUE</tt> (that is, elliptical cone simplification is <tt>ON</tt>)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_approximate</tt></td>
 * <td valign = "top">  <tt>FALSE</tt></td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_force_simplification</tt></td>
 * <td valign = "top">  <tt>FALSE</tt> (that is, the API would refuse to simplify if it causes invalidities)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>do_canonic_simplification</tt></td>
 * <td valign = "top">  <tt>FALSE</tt> (that is, the API would refuse to simplify existing analytic surfaces to a lesser complex analytic surface if found within user specified tolerances.)</td>
 * </tr>
 * </table>
 * </dl></dd>
 * <br>
 * A sample snippet code to illustrate the usage is given below: <br>
 * <pre>
 * simplify_options simp_opts;
 * simp_opts.set_do_curve_simplification(0); // Switch OFF curve simplification
 * outcome result = api_simplify_entity (body, &simp_opts);
 * </pre>
 * For more details on how to get and set these parameters, refer to the documentation for the @href simplify_options class.
 * @param ao
 * <tt>AcisOptions*  ao </tt> Pointer to an object of @href AcisOptions class.
 * <br>
 * @see simplify_options, AcisOptions
 **/
DECL_HEAL outcome  api_simplify_entity ( 	ENTITY *& input_entity, simplify_options* simp_options = NULL, AcisOptions*  ao = NULL );
/** @} */
#endif
