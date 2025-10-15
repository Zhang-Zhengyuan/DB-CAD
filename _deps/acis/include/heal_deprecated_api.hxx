/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef heal_deprecated_api_hxx
#define heal_deprecated_api_hxx
#include "logical.h"
#include "api.hxx"
#include "dcl_heal.h"
#include "stchapi.hxx"
#include "stitch_callback.hxx" 

class outcome;
class BODY;
class ENTITY_LIST;
class FACE;

/**
* @nodoc
* Class for simplification options.
* <br>
* <b>Role:</b> The <tt>hh_simplify_options</tt> is a class that sets the parameters which govern the functionality of
* simplification algorithm.
* <br><br>
* The constructor of <tt>hh_simplify_options</tt> sets the values of all its members (parameters) as -1 which indicates "use default value"
* The parameters that can be set are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* </table>
* <br>
* The default values that are chosen by the APIs <tt>api_hh_simplify_auto</tt> and <tt>api_hh_simplify_analyze</tt> are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">The API will compute a default value for this based on its own heuristics.</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">10,000</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">1 (i.e. curve-simplification is <tt>ON</tt>)</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">1 (i.e. surface-simplification is <tt>ON</tt>)</td>
* </tr>
* </table>
* <br>
* Curve-simplification being <tt>ON</tt> means that now the spline curves in the model would be simplified
* to straights, circles and ellipses.
* <br><br>
* From R11 onwards, both curve simplification and surface simplification are done by default. In R10 and
* before, only surface simplification was done by default and there was no curve simplification.
* To make <tt>api_hh_simplify_auto</tt> behave as in R10 or before, the caller of the API will require to
* create a <tt>hh_simplify_options</tt> options object, call <tt>set_do_curve_simplification()</tt> method explicitly
* with a "0" and pass this object into the API. A sample code to illustrate this is given below:
* <pre>
* hh_simplify_options simp_opts;
* simp_opts.set_do_curve_simplification(0);
* result = api_hh_simplify_auto (body, NULL, &simp_opts);</pre>
**/
class DECL_HEAL hh_simplify_options : public ACIS_OBJECT
{
protected:
	double m_pos_tol;
	int m_do_curve_simplify;
	double m_max_radius;
	int m_do_surface_simplify;

public:
	/**
	* C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	* <br><br>
	* <b>Role:</b> Sets the values of all its members (parameters) as -1 which indicates "use default value".
	**/
	hh_simplify_options();

	// Get functions
	/**
	* Returns the value of simplification tol.
	**/
	double get_simplification_tol();
	/**
	* Returns the value of Max curve radius.
	**/
	double get_max_radius();
	/**
	* Returns the value of <tt>do_curve_simplification</tt> parameter.
	**/
	int get_do_curve_simplification();
	/**
	* Returns the value of <tt>do_surface_simplification</tt> parameter.
	**/
	int get_do_surface_simplification();

	// Set Functions

	/**
	* Sets the value of simplification tolerance.
	* <br><br>
	* @param tol
	* simplification tol.
	**/
	void set_simplification_tol(double tol);
	/**
	* Sets the value of Maximum curve radius.
	* <br><br>
	* @param radius
	* Maximum curve radius.
	**/
	void set_max_radius(double radius);
	/**
	* Sets the value to switch curve simplification <tt>ON</tt> or <tt>OFF</tt>.
	* <br><br>
	* @param val
	* Takes values -1 (use default setting), 0 (OFF), 1 (ON).
	**/
	void set_do_curve_simplification(int val);
	/**
	* Sets the value to switch surface simplification <tt>ON</tt> or <tt>OFF</tt>.
	* <br><br>
	* @param val
	* Takes values -1 (use default setting), 0 (OFF), 1 (ON).
	**/
	void set_do_surface_simplification(int val);

};

/**
* @nodoc
* Initializes the body for healing.
* <br><br>
* <b>Role:</b> This API must be called before the healing process is begun. It
* attaches aggregate attributes to the body. These attributes are used to store
* such information as the tolerances for the various healing phases.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_init_body_for_healing(BODY* inp_body, AcisOptions* ao = NULL);

/**
* @nodoc
* Terminates the healing process for a body.
* <br><br>
* <b>Role:</b> This API must be called after the healing process is complete.
* It detaches the aggregate attributes (that contain such things as the tolerances
* for the various healing phases) from the body.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_end_body_for_healing(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* Automatically heals the input body (performs all phases of the healing process) using intelligently selected tolerances.
* <br><br>
* <b>Role:</b> This API automatically sets tolerances (based on analysis of the
* body) and heals the body by executing all the healing phases sequentially. This
* API calls the APIs for preprocess, automatic geometry simplification, automatic
* stitching, automatic geometry building, and postprocess:
* <br>
* <ul>
* <li> @href api_hh_preprocess </li>
* <li> @href api_hh_simplify_auto </li>
* <li> @href api_hh_stitch_auto </li>
* <li> @href api_hh_geombuild_auto </li>
* <li> @href api_hh_postprocess </li>
* </ul>
* The above sequence of APIs follows the behavior of <tt>api_hh_auto_heal</tt>
* upto ACIS version R10. From R11 onwards, <tt>api_hh_simplify_auto</tt>
* performs curve simplification by default. Therefore, in order to have the above
* sequence of APIs behave same as <tt>api_hh_auto_heal</tt>, one must switch off curve
* simplification while calling <tt>api_hh_simplify_auto</tt>. The details of how to do so
* are present in the documentation of <tt>api_hh_simplify_auto</tt>.
* <br><br>
* All the phases and subphases of the healing process are performed automatically.
* Each healing phase analyzes the body first, then sets the best values for the
* tolerances and options used by that healing phase. (These tolerances can be
* modified by callbacks.) Next, the healing phase calculates the new
* geometry/topology and applies the changes to the body.
* <br><br>
*
* From R13 and above versions, this API makes the edges and vertices
* tolerant, if needed at the end of healing during its postprocess phase.
* <br><br>
*
* The API <tt>api_hh_init_body_for_healing</tt> should be called to attach the aggregate
* healing attributes to the body before performing automatic healing. The healing
* attributes are retained on the healed body after automatic healing is done to
* hold the healing results. This allows the user to modify such things as
* tolerances and redo any or all of the healing process if desired. The API
* <tt>api_hh_end_body_for_healing</tt> should be called after healing is completed to
* remove the attributes from the body.
* <br><br>
* The optional argument <tt>ao</tt> helps enable journaling and versioning options.
* <br><br>
* <b>Limitations: </b> The automatic geometry simplification phase of autoheal does not support
* simplification of spline surfaces to elliptical cylinders and elliptical cones.<br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_auto_heal(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
*
*/
[[deprecated("Deprecated API")]]
DECL_HEAL void api_hh_reset_healer();
/**
* @nodoc
* Automatically executes the analyze, calculate, and fix stages of the geometry simplification phase.
* <br><br>
* <b>Role:</b> The APIs for the analyze, calculate, and fix stages of geometry
* simplification are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* The optional argument <tt>ao</tt> helps enable journaling and versioning options.
* <br><br>
* The optional argument <tt>hh_simplify_options</tt> object sets the parameters which govern
* the functionality of <tt>api_hh_simplify_auto</tt>. The constructor of
* <tt>hh_simplify_options</tt> sets the values of all the parameters as -1.
* The parameters that can be set are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* </table>
* <br>
* The value of -1 for any of these 4 parameters indicates to <tt>api_hh_simplify_auto</tt> that it should use
* default values for those. The default values that are chosen by the api are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">The API will compute a default value for this based on its own heuristics.</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">10,000</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">1 (i.e. curve-simplification is <tt>ON</tt>)</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">1 (i.e. surface-simplification is <tt>ON</tt>)</td>
* </tr>
* </table>
* <br>
* Curve-simplification being <tt>ON</tt> means that now the spline curves in the model would be simplified
* to straights, circles and ellipses.
* <br><br>
* From R11 onwards, both curve simplification and surface simplification are done by default. In R10 and
* before, only surface simplification was done by default and there was no curve simplification.
* To make <tt>api_hh_simplify_auto</tt> behave as in R10 or before, the caller of the API will require to
* create a <tt>hh_simplify_options</tt> options object, call <tt>set_do_curve_simplification()</tt> method explicitly
* with a "0" and pass this object into the API. A sample code to illustrate this is given below:
* <pre>
* hh_simplify_options simp_opts;
* simp_opts.set_do_curve_simplification(0);
* result = api_hh_simplify_auto (body, &simp_opts, NULL);</pre>
* <br><br>
* <b>Limitations: </b> The API does not support simplification of spline surfaces to elliptical cylinders and elliptical cones.<br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param simp_opts
* simplification options.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_auto(BODY* inp_body, hh_simplify_options *simp_opts, AcisOptions* ao = NULL);
/**
* @nodoc
* Automatically executes the analyze, calculate, and fix stages of the geometry simplification phase.
* <br><br>
* <b>Role:</b> The APIs for the analyze, calculate, and fix stages of geometry
* simplification are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* From R11 onwards <tt>api_hh_simplify_auto()</tt> will do curve-simplification by default. To make this API
* behave as in R10 or before, the caller would need to call the new API
* <tt>api_hh_simplify_auto(BODY* inp_body, hh_simplify_options *simp_opts, AcisOptions* ao = NULL)</tt>;
* with curve simplifification turned <tt>OFF</tt>. The details of how to switch <tt>OFF</tt> curve simplification are
* mentioned in the documentation of the above mentioned API.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_auto(BODY* inp_body, AcisOptions* ao = NULL);

/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes the body for geometry simplification.
* <br><br>
* <b>Role:</b> This API analyzes the body and intelligently sets values of
* required options and tolerances for geometry simplification. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres). If the body is fully analytic, this API
* sets a flag in the simplification aggregate attribute indicating that no
* geometry simplification is needed.
* <br><br>
* The optional argument <tt>ao</tt> helps enable journaling and versioning options.
* <br><br>
* The optional argument <tt>hh_simplify_options</tt> object has been added since ACIS version R11.
* The successive calls to <tt>api_hh_simplify_calculate</tt> and <tt>api_hh_simplify_fix</tt> will take into account
* the parameters set into the <tt>hh_simplify_options</tt> object passed into <tt>api_hh_simplify_analyze</tt>.
* The constructor of <tt>hh_simplify_options</tt> sets the values of all the parameters as -1.
* The parameters that can be set are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">&nbsp;</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">Takes values -1 (use default setting), 0 (<tt>OFF</tt>), 1 (<tt>ON</tt>).</td>
* </tr>
* </table>
* <br>
* The value of -1 for any of these 4 parameters indicates to API that it should use
* default values for those. The default values that are chosen by the api are:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 20%><tt>simplify_pos_tol</tt></td>
* <td valign = "top">The API will compute a default value for this based on its own heuristics.</td>
* </tr>
* <tr>
* <td valign = "top"><tt>max_radius</tt></td>
* <td valign = "top">10,000</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_curve_simplification</tt></td>
* <td valign = "top">1 (i.e. curve-simplification is <tt>ON</tt>)</td>
* </tr>
* <tr>
* <td valign = "top"><tt>do_surface_simplification</tt></td>
* <td valign = "top">1 (i.e. surface-simplification is <tt>ON</tt>)</td>
* </tr>
* </table>
* <br>
* Curve-simplification being <tt>ON</tt> means that now the spline curves in the model would be simplified
* to straights, circles and ellipses.
* <br><br>
* From R11 onwards, both curve simplification and surface simplification are done by default. In R10 and
* before, only surface simplification was done by default and there was no curve simplification.
* To make <tt>api_hh_simplify_analyze</tt> behave as in R10 or before the caller of the API will require to
* create a <tt>hh_simplify_options</tt> options object, call <tt>set_do_curve_simplification()</tt> method explicitly
* with a "0" and pass this object into the API. A sample code to illustrate this is given below:
* <pre>
* hh_simplify_options simp_opts;
* simp_opts.set_do_curve_simplification(0);
* result = api_hh_simplify_analyze (body, &simp_opts, NULL);</pre>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param simp_opts
* simplification options.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_analyze(BODY* inp_body, hh_simplify_options *simp_opts, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes the body for geometry simplification.
* <br><br>
* <b>Role:</b> This API analyzes the body and intelligently sets values of
* required options and tolerances for geometry simplification. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres). If the body is fully analytic, this API
* sets a flag in the simplification aggregate attribute indicating that no
* geometry simplification is needed.
* <br><br>
* From R11 onwards <tt>api_hh_simplify_analyze()</tt> will set curve-simplification <tt>ON</tt> in simplification aggregate
* attributes indicating that curve simplification will be done by default. To make this API
* behave as in R10 or before, the caller would need to call the new API
* <tt>api_hh_simplify_analyze(BODY* inp_body, hh_simplify_options *simp_opts, AcisOptions* ao = NULL)</tt>;
* with curve simplifification turned <tt>OFF</tt>. The details of how to switch <tt>OFF</tt> curve simplification are
* mentioned in the documentation of the above mentioned API.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_analyze(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates the simplified analytic forms of the splines.
* <br><br>
* <b>Role:</b> This API performs the calculation stage of geometry
* simplification. Geometry simplification attempts to simplify NURBS surfaces into
* analytic forms. Spline surfaces are converted wherever possible to planes,
* cylinders, cones, tori, and spheres.
* <br><br>
* The new geometry is stored in attributes (attached to individual faces) so that
* the user may compare the new and the old geometry. The fix stage must be used to
* apply (fix) the new geometry to the body.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_calculate(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Executes the fix stage of geometry simplification.
* <br><br>
* <b>Role:</b> This API performs the fix stage of geometry simplification.
* Geometry simplification attempts to simplify NURBS surfaces into analytic forms
* (planes, cylinders, cones, tori, and spheres).
* <br><br>
* After the calculate stage, the new geometry is placed in the attributes attached
* to the body and the user can compare the old and new geometry. The fix stage
* applies (fixes) the new geometry to the body and stores the old geometry in the
* attributes.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_fix(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Cleans up the individual healing attributes related to the geometry simplification phase.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_simplify_cleanup(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* Forces the given surface to an analytic surface (cylinder).
* <br><br>
* <b>Role:</b> This API performs force simplification of splines. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_face
* input face.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_force_simplify_to_cylinder(FACE* inp_face, AcisOptions* ao = NULL);
/**
* @nodoc
* Forces the given surface to an analytic surface (cone).
* <br><br>
* <b>Role:</b> This API performs force simplification of splines. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_face
* input face.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_force_simplify_to_cone(FACE* inp_face, AcisOptions* ao = NULL);
/**
* @nodoc
* Forces the given surface to an analytic surface (plane).
* <br><br>
* <b>Role:</b> This API performs force simplification of splines. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_face
* input face.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_force_simplify_to_plane(FACE* inp_face, AcisOptions* ao = NULL);

/**
* @nodoc
* Forces the given surface to an analytic surface (sphere).
* <br><br>
* <b>Role:</b> This API performs force simplification of splines. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_face
* input face.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_force_simplify_to_sphere(FACE* inp_face, AcisOptions* ao = NULL);
/**
* @nodoc
* Forces the given surface to an analytic surface (torus).
* <br><br>
* <b>Role:</b> This API performs force simplification of splines. Geometry
* simplification attempts to simplify NURBS surfaces into analytic forms (planes,
* cylinders, cones, tori, and spheres).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_face
* input face.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_force_simplify_to_torus(FACE* inp_face, AcisOptions* ao = NULL);
/**
* @nodoc
* Automatically executes the analyze, calculate, and fix stages of the stitching phase.
* <br><br>
* <b>Role:</b> The APIs for the analyze, calculate, and fix stages of stitching
* are called sequentially. Intelligent tolerances that are recommended by the
* analyze stage are used in the calculate stage. Stitching attempts to pair up
* edges of free faces and stitch them together. If no stitching is needed, this
* API sets a flag in the stitching aggregate attribute indicating this.
* <br><br>
*
* In R12 and later versions, this API has a <b>fail-safe</b> behavior (i.e. it attempts to do
* as much as possible and not fail, even in cases when it encounters geometry and topology
* related errors).
* On an event of a recoverable error this API will undo the current atomic transaction that
* failed due to this error, raise a sys_warning with the same error message, and proceed further.
* The fail-safe behavior of this API does not apply to "irrecoverable" errors, such as
* <tt>NOT_BODY</tt>, <tt>UNACCEPTABLE_MODEL</tt> and <tt>INVALID_STITCH_MAX_TOL</tt>.
* Note that the fail-safe behavior can be switched OFF by pushing a value of
* <tt>TRUE</tt> onto the option "careful" before calling this API
* (and popping it after the call). When the fail-safe behavior is switched off,
* this API will fail and roll back to its initial state when the first
* error is encountered.<br><br>
*
* In R12 and later versions, this API will fail with the error
* <tt>COINCIDENT_FACES</tt>, roll and exit immediately when coincident
* faces are detected if "careful" option is set to <tt>TRUE</tt>.  If the careful option is set
* to <tt>FALSE</tt> ( default ), when this API comes across a pair of coincident faces
* it will not stitch the two faces together along the edge, and the stitching continues.
* In R10 and later versions, if coincident faces are detected during the
* stitching process the API will fail with the error <tt>COINCIDENT_FACES</tt>.
* For pre-R10 versions, the call to this API passing in coincident faces will
* lead to undefined behavior.<br><br>
*
* A user can determine the result of this API using the following:<br>
* <dl><dd>
*  a.	If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>,
*  then the API has fully succeeded. i.e. errors were not encountered.<br>
*  b.	If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>,
*  and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
*  encountered error(s), yet it proceeded further successfully. In this case,
*  the out coming <tt>BODY *inp_body</tt> is not guaranteed to be usable.<br>
*  c.	If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and
*  rolled the model back to the state before the API was called.
* </dd></dl>
* <br><br>
*
* <b>Effect:</b> Changes model. This API may remove edges smaller than maximum stitch
* tolerance. This API may also split incoming edges in the process of stitching.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_stitch_auto(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes the topology of the input model for stitching.
* <br><br>
* <b>Role:</b> This API analyzes the body and intelligently sets values of
* required options and tolerances for stitching. Stitching attempts to pair up
* edges of free faces and stitch them together. If no stitching is needed, this
* API sets a flag in the stitching aggregate attribute indicating this.
* <br><br>
*
* In R12 and later versions, this API has a <b>fail-safe</b> behavior (i.e. it attempts to do
* as much as possible and not fail, even in cases when it encounters geometry and topology
* related errors).
* On an event of a recoverable error this API will undo the current atomic transaction that
* failed due to this error, raise a sys_warning with the same error message, and proceed further.
* The fail-safe behavior of this API does not apply to "irrecoverable" errors, such as
* <tt>NOT_BODY</tt> and <tt>UNACCEPTABLE_MODEL</tt>. Note that the fail-safe
* behavior can be switched OFF by pushing a value of
* <tt>TRUE</tt> onto the option "careful" before calling this API
* (and popping it after the call). When the fail-safe behavior is switched off,
* this API will fail and roll back to its initial state when the first
* error is encountered.<br><br>
*
* A user can determine the result of this API using the following:<br>
* <dl><dd>
*  a.	If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>,
*  then the API has fully succeeded. i.e. errors were not encountered.<br>
*  b.	If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>,
*  and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
*  encountered error(s), yet it proceeded further successfully. In this case,
*  the out coming <tt>BODY *inp_body</tt> is not guaranteed to be usable.<br>
*  c.	If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and
*  rolled the model back to the state before the API was called.
* </dd></dl>
* <br><br>
*
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp ACIS Healing
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_stitch_analyze(BODY* inp_body, AcisOptions* ao = NULL);


/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Builds new topology of the body, to the extent possible, to pair up edges of free faces.
* <br><br>
* <b>Role:</b> This API performs the calculate stage of stitching. Stitching
* attempts to pair up edges of free faces and stitch them together. This process
* changes the body by doing such things as splitting and merging edges. This API
* builds the topology of the body to the extent possible. It performs unhook,
* geometry cleaning, and tries to stitch the unshared edges. It considers all
* unshared edges for pairing. Individual attributes containing the partner edge
* information are attached to edges.
* <br><br>
* The new topology is stored in attributes (attached to individual faces) so that
* the user may compare the new and the old topology. The fix stage must be used to
* apply (fix) the new topology to the body.
* <br><br>
*
* In R12 and later versions, this API has a <b>fail-safe</b> behavior (i.e. it attempts to do
* as much as possible and not fail, even in cases when it encounters geometry and topology
* related errors).
* On an event of a recoverable error this API will undo the current atomic transaction that
* failed due to this error, raise a sys_warning with the same error message, and proceed further.
* The fail-safe behavior of this API does not apply to "irrecoverable" errors, such as
* <tt>NOT_BODY</tt> and <tt>INVALID_STITCH_MAX_TOL</tt>.
* Note that the fail-safe behavior can be switched OFF by pushing a value of
* <tt>TRUE</tt> onto the option "careful" before calling this API
* (and popping it after the call). When the fail-safe behavior is switched off,
* this API will fail and roll back to its initial state when the first
* error is encountered.<br><br>
*
* In R12 and later versions, this API will fail with the error <tt>COINCIDENT_FACES</tt>,
* roll and exit immediately when coincident faces are detected if "careful"
* option is set to <tt>TRUE</tt>.  If the careful option is set
* to <tt>FALSE</tt> ( default ), when this API comes across a pair of coincident faces,
* it skips pairing of their edges (so that fix stage will not stitch those edges)
* and continues processing the remaining edges. In R10 and later versions, if coincident faces are detected during the stitching
* process the API will fail with the error <tt>COINCIDENT_FACES</tt>. For pre-R10 versions,
* the call to this API passing in coincident faces will lead to undefined behavior.<br><br>
*
* A user can determine the result of this API using the following:<br>
* <dl><dd>
*  a.	If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>,
*  then the API has fully succeeded. i.e. errors were not encountered.<br>
*  b.	If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>,
*  and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
*  encountered error(s), yet it proceeded further successfully. In this case,
*  the out coming <tt>BODY *inp_body</tt> is not guaranteed to be usable.<br>
*  c.	If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and
*  rolled the model back to the state before the API was called.
* </dd></dl>
* <br><br>
*
* <b>Effect:</b> Changes model. This API may remove edges smaller than maximum stitch
* tolerance. This API may also split incoming edges in the process of stitching.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp ACIS Healing
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_stitch_calculate(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Executes the fix stage of stitching.
* <br><br>
* <b>Role:</b> This API performs the fix stage of stitching. Stitching attempts
* to pair up edges of free faces and stitch them together. This process changes
* the body by doing such things as splitting and merging edges.
* <br><br>
* After the calculate stage, the new topology is placed in the attributes attached
* to the body and the user can compare the old and new topology. The fix stage
* applies (fixes) the new topology to the body and stores the old topology in the
* attributes.
* <br><br>
*
* In R12 and later versions, this API has a <b>fail-safe</b> behavior (i.e. it attempts to do
* as much as possible and not fail, even in cases when it encounters geometry and topology
* related errors).
* On an event of a recoverable error this API will undo the current atomic transaction that
* failed due to this error, raise a sys_warning with the same error message, and proceed further.
* The fail-safe behavior of this API does not apply to "irrecoverable" errors, such as
* <tt>NOT_BODY</tt>.
* Note that the fail-safe behavior can be switched OFF by pushing a value of
* <tt>TRUE</tt> onto the option "careful" before calling this API
* (and popping it after the call). When the fail-safe behavior is switched off,
* this API will fail and roll back to its initial state when the first
* error is encountered.<br><br>
*
* A user can determine the result of this API using the following:<br>
* <dl><dd>
*  a.	If <tt>outcome::encountered_errors()</tt> returns <tt>FALSE</tt>,
*  then the API has fully succeeded. i.e. errors were not encountered.<br>
*  b.	If <tt>outcome::encountered_errors()</tt> returns <tt>TRUE</tt>,
*  and <tt>outcome::ok()</tt> returns <tt>TRUE</tt>, then it means that the API has
*  encountered error(s), yet it proceeded further successfully. In this case,
*  the out coming <tt>BODY *inp_body</tt> is not guaranteed to be usable.<br>
*  c.	If <tt>outcome::ok()</tt> returns <tt>FALSE</tt>, then the API has failed and
*  rolled the model back to the state before the API was called.
* </dd></dl>
* <br><br>
*
* <b>Effect:</b> Changes model. This API may remove edges smaller than maximum stitch
* tolerance. This API may also split incoming edges in the process of stitching.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_stitch_fix(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Cleans up the individual healing attributes related to the stitching phase.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body
* @param ao
* acis options
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_stitch_cleanup(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* Automatically executes the analyze, calculate, and fix stages of the geometry building phase.
* <br><br>
* <b>Role:</b> The APIs for the analyze, calculate, and fix stages of the
* geometry building phase are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage. The geometry
* building phase performs all the geometry related healing operations, including
* fixing of edge geometries by intersections, snapping surfaces for fixing
* tangencies, and refitting spline surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp ACIS Healing
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_geombuild_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes the geometry of the body and attaches attributes to bad edges,vertices, and coedges.
* <br><br>
* <b>Role:</b> This API performs the analyze stage of the geometry building
* phase. The geometry building phase performs all the geometry related healing
* operations, including fixing of edge geometries by intersections, snapping
* surfaces for fixing tangencies, and refitting spline surfaces.
* <br><br>
* A check is run on the body and the inaccurate geometries are marked with
* attributes. A invalid edge is one in which the edge curve does not lie on the
* underlying surfaces to ACIS tolerance. A vertex is marked bad if it does not lie
* on the edges or faces which are incident on it. A bad coedge is one whose pcurve
* does not match with the edge curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_geombuild_analyze(BODY *inp_body, AcisOptions* ao = NULL);


/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates new geometry for all the bad geometry that was marked by the geometry building analyze stage.
* <br><br>
* <b>Role:</b> This API performs the calculate and fix stages of the geometry
* building phase. The geometry building phase performs all the geometry related
* healing operations, including fixing of edge geometries by intersections,
* snapping surfaces for fixing tangencies, and refitting spline surfaces.
* <br><br>
* This API calculates the geometry in the model using all the geometry building
* subphases:
* <br><br>
* <table width = 100%>
* <tr width = 100%>
* <td valign = "top" width = 25%>Analytic solver subphase</td>
* <td valign = "top">Handles the analytic geometries.</td>
* </tr>
* <tr>
* <td valign = "top">Isospline solver subphase</td>
* <td valign = "top">Handles tangential splines that are on isoparametric lines.</td>
* </tr>
* <tr>
* <td valign = "top">Sharp edge subphase</td>
* <td valign = "top">Handles intersections of nontangential (sharp) edges.</td>
* </tr>
* <tr>
* <td valign = "top">Generic spline solver subphase</td>
* <td valign = "top">Handles tangential spline junctions that are non-isoparametric.</td>
* </tr>
* <tr>
* <td valign = "top">Wrap up subphase </td>
* <td valign = "top">Adds pcurves.</td>
* </tr>
* </table>
* <br>
* The new geometry is applied (fixed) to the body and the old geometry is placed
* on the model in attributes so that the user may compare the new and the old
* geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp ACIS Healing
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_geombuild_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Cleans up the individual healing attributes related to the geometry building phase.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_geombuild_cleanup(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the geometry of the body and attaches attributes to bad edges,vertices, and coedges.
* <br><br>
* <b>Role:</b> This API performs a check on the geometry of a body.This API does not set any tolerances.This API is
* useful at the end of healing to check results of the geometry stage. The results
* of this check are stored in the aggregate attribute and can be obtained using
* the member function: <tt>ATTRIB_HH_AGGR_GEOMBUILD::output_analysis_results()</tt>
* <br><br>
* A check is run on the body and the inaccurate geometries are marked with
* attributes. A invalid edge is one in which the edge curve does not lie on the
* underlying surfaces to ACIS tolerance. A vertex is marked bad if it does not lie
* on the edges or faces which are incident on it. A bad coedge is one whose pcurve
* does not match with the edge curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_geombuild_check(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Automatically executes the analyze and calculate stages of the analytic solver subphase of geometry building.
* <br><br>
* <b>Role:</b> This API performs all of the stages of the analytic solver
* subphase of the geometry building phase. The analytic solver subphase attempts
* to heal all edges and vertices shared by analytic surfaces.
* <br><br>
* The APIs for the analyze and calculate stages of the analytic solver subphase of
* geometry building are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analytic_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes edges lying on analytic surfaces.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* analyze stage of the analytic solver subphase of geometry building. The analytic
* solver subphase attempts to heal all edges and vertices shared by analytic
* surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analytic_analyze(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates and applies the new geometry of analytic surfaces and the corresponding curves and vertices.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* calculate and fix stages of the analytic solver subphase of geometry building.
* The analytic solver subphase attempts to heal all edges and vertices shared by
* analytic surfaces.
* <br><br>
* This API calculates all analytic geometries. Tangency constraints are solved
* with the aid of a solver. The new geometry is applied (fixed) to the body and
* the old geometry is placed on the model in attributes so that the user may
* compare the new and the old geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analytic_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Automatically executes the analyze and calculate stages of the isospline solver subphase of geometry building.
* <br><br>
* <b>Role:</b> This API performs all of the stages of the isospline solver
* subphase of the geometry building phase. The isospline solver attempts to heal
* all edges shared by tangential isoparametric surfaces (e.g., the intersection
* curve is an isoparametric curve of both splines in the intersection).
* <br><br>
* The APIs for the analyze and calculate stages of the isospline solver subphase
* of geometry building are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_isospline_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes isoparametric edges between spline surfaces.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* analyze stage of the isospline solver subphase of geometry building. The
* isospline solver attempts to heal all edges shared by tangential isoparametric
* surfaces (e.g., the intersection curve is an isoparametric curve of both splines
* in the intersection).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_isospline_analyze(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates all the new geometry of spline surfaces which intersect tangentially at isospline junctions.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* calculate and fix stages of the isospline solver subphase of geometry building.
* The isospline solver attempts to heal all edges shared by tangential
* isoparametric surfaces (e.g., the intersection curve is an isoparametric curve
* of both splines in the intersection).
* <br><br>
* This API calculates isoparametric junctions of spline geometries intersecting
* tangentially. The new geometry is applied (fixed) to the body and the old
* geometry is placed on the model in attributes so that the user may compare the
* new and the old geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_isospline_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);

/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Automatically executes the analyze and calculate stages of the sharp edge subphase of geometry building.
* <br><br>
* <b>Role:</b> This API performs all of the stages of the sharp edge solver
* subphase of the geometry building phase. The sharp edge solver attempts to heal
* all edges and vertices that are shared by surfaces that intersect sharply. This
* includes nontangential surface junctions.
* <br><br>
* The APIs for the analyze and calculate stages of the sharp edge solver subphase
* of geometry building are called sequentially. Intelligent tolerances that are
* recommended by the analyze stage are used in the calculate stage.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_sharp_edge_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes sharp edges that intersect nontangentially.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* analyze stage of the sharp edge solver subphase of geometry building. The sharp
* edge solver attempts to heal all edges and vertices that are shared by surfaces
* that intersect sharply. This includes nontangential surface junctions.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_sharp_edge_analyze(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates sharp edge geometries of spline surfaces.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* calculate and fix stages of the sharp edge solver subphase of geometry building.
* The sharp edge solver attempts to heal all edges and vertices that are shared by
* surfaces that intersect sharply. This includes nontangential surface junctions.
* The new geometry is applied (fixed) to the body and the old geometry is placed
* on the model in attributes so that the user may compare the new and the old
* geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_sharp_edge_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Automatically executes the analyze and calculate stages of the generic spline solver subphase of geometry building.
* <br><br>
* <b>Role:</b> This API performs all of the stages of the generic spline solver
* subphase of the geometry building phase. The generic spline solver attempts to
* heal generic tangential spline junctions, (e.g., the intersection curve is not
* an isoparametric curve of both splines in the intersection).
* <br><br>
* The APIs for the analyze and calculate stages of the generic spline solver
* subphase of geometry building are called sequentially. Intelligent tolerances
* that are recommended by the analyze stage are used in the calculate stage.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_gen_spline_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes generic spline intersections.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* analyze stage of the generic spline solver subphase of geometry building. The
* generic spline solver attempts to heal generic tangential spline junctions, (e.g.
* , the intersection curve is <i>not</i> an isoparametric curve of both splines in the
* intersection).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_gen_spline_analyze(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Calculates the new geometry of spline surfaces that intersect tangentially at non-isoparametric junctions.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* calculate and fix stages of the generic spline solver subphase of geometry
* building. The generic spline solver attempts to heal generic tangential spline
* junctions, (e.g., the intersection curve is <i>not</i> an isoparametric curve of both
* splines in the intersection).
* <br><br>
* This API uses surface fitting algorithms to calculate the new geometry needed to
* heal non-isoparametric junctions of spline geometries intersecting tangentially.
* The new geometry is applied (fixed) to the body and the old geometry is placed
* on the model in attributes so that the user may compare the new and the old
* geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_gen_spline_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Automatically executes the analyze and calculate stages of the wrap up subphase of geometry building.
* <br><br>
* <b>Role:</b> The APIs for the analyze and calculate stages of the wrap up
* subphase (which handles pcurve geometry) are called sequentially. Intelligent
* tolerances that are recommended by the analyze stage are used in the calculate
* stage.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_wrapup_auto(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Analyzes coedge pcurve geometry.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* analyze stage of the wrap up subphase of geometry building. The wrap up subphase
* recomputes the pcurve geometry of unhealed coedges.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_wrapup_analyze(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b>
* <br><br>
* Calculates the pcurves in the model.
* <br><br>
* <b>Role:</b> This API is used in the geometry building phase. It performs the
* calculate and fix stages of the wrap up subphase of geometry building. This API
* recalculates pcurves on spline surfaces. The new geometry is applied (fixed) to
* the body and the old geometry is placed on the model in attributes so that the
* user may compare the new and the old geometry.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_wrapup_calc_fix(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* Executes initial cleanup of the model to be healed.
* <br><br>
* <b>Role:</b> This API removes such things from the body as zero-length edges,
* zero-area faces, and duplicate vertices. This should be executed before any healing
* operations (except autoheal, which performs this phase).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_preprocess(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* Executes the final cleanup of the healed model.
* <br><br>
* <b>Role:</b> This API performs postprocessing of the healed data. It performs
* such operations as removal of zero-length edges and zero-area faces, and
* correction of negative area faces, duplicate vertices, and edge groups.
* <br><br>
* From R13 and above versions, this API makes the edges and vertices
* tolerant, if needed.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_postprocess(BODY* inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete. From R13 and above versions,
* <tt>api_hh_postprocess</tt> and <tt>api_hh_auto_heal</tt> make edges and
* vertices tolerant, if needed. So <tt>api_hh_make_tolerant</tt> is not needed
* to be called after a call to any of these two APIs.
* <br><br>
* Converts unhealed edges to tolerant edges.
* <br><br>
* <b>Role:</b> This API examines all the edges and vertices in the body
* and converts each unhealed edge (or a vertex) into a tolerant edge (or a vertex).
* This API should be executed before <tt>api_hh_end_body_for_healing</tt>
* in order to use the healing attributes.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_make_tolerant(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the vertices of the input body for errors and stores results in attributes attached to the bad vertices.
* <br><br>
* <b>Role:</b> This API checks all the vertices of the input body. The results
* are attached to any bad vertices as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Does the vertex lie on the corresponding edges?</li>
* <li>Do the edges meet at the vertex?</li>
* <li>Does the vertex lie on the corresponding surfaces?</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_vertices(BODY *inp_body, AcisOptions* ao = NULL);

/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the edges of the input body for errors and stores results in attributes attached to the bad edges.
* <br><br>
* <b>Role:</b> This API checks all the edges of the input body. The results are
* attached to any bad edges as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Checks curve geometry</li>
* <li>Determines convexity</li>
* <li>Checks length</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_edges(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the coedges of the input body for errors and stores results in attributes attached to the bad coedges.
* <br><br>
* <b>Role:</b> This API checks all the coedges of the input body. The results
* are attached to any bad coedges as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Does the coedge lie on the corresponding face surface?</li>
* <li>If the coedge contains a pcurve, does the domain of the pcurve correspond with the edge?</li>
* <li>Does the coedge have a partner?</li>
* <li>If the coedge contains a pcurve, is the pcurve within tolerance of the edge?</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_coedges(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the loops of the input body for errors and stores results in attributes attached to the bad loops.
* <br><br>
* <b>Role:</b> This API checks all the loops of the input body. The results are
* attached to any bad loops as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Is the loop closed?</li>
* <li>Checks loop orientation</li>
* <li>Do the loop coedges have gaps?</li>
* <li>Does the loop self-intersect?</li>
* <li>Checks for correct parameter range of the coedges</li>
* <li>Checks that the coedges lie on the face surface</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_loops(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the faces of the input body for errors and stores results in attributes attached to the bad faces.
* <br><br>
* <b>Role:</b> This API checks all the faces of the input body. The results are
* attached to any bad faces as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Checks loops</li>
* <li>Checks surface</li>
* <li>Checks face area</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_faces(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the shells of the input body for errors and stores results in attributes attached to the bad shells.
* <br><br>
* <b>Role:</b> This API checks all the shells of the input body. The results
* are attached to any bad shells as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Checks that the shell is closed</li>
* <li>Checks shell orientation</li>
* <li>Checks if shell represents a single volume</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_shells(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the lumps of the input body for errors and stores results in attributes attached to the bad lumps.
* <br><br>
* <b>Role:</b> This API checks all the lumps of the input body. The results are
* attached to any bad lumps as attributes.
* <br><br>
* The tests include:
* <ul>
* <li>Checks shells for closure</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_lumps(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Checks the input body for errors and stores results in attributes attached to the bad entities.
* <br><br>
* <b>Role:</b> This API checks the input body for errors. The tests include all
* the tests performed by the individual analyze APIs for the various specific
* types of entities (e.g., <tt>api_hh_analyze_coedges</tt>). The results are attached to
* any bad entities as attributes.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_analyze_body(BODY *inp_body, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all edges in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all edges in the input body that were
* marked as bad during a healing analysis operation. Refer to the descriptions of
* the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad edges.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_edges(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all vertices in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all vertices in the input body that
* were marked as bad during a healing analysis operation. Refer to the
* descriptions of the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad vertices.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_vertices(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all coedges in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all coedges in the input body that
* were marked as bad during a healing analysis operation. Refer to the
* descriptions of the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad coedges.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_coedges(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all loops in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all loops in the input body that were
* marked as bad during a healing analysis operation. Refer to the descriptions of
* the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad loops.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_loops(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all faces in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all faces in the input body that were
* marked as bad during a healing analysis operation. Refer to the descriptions of
* the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad faces.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_faces(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all shells in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all shells in the input body that
* were marked as bad during a healing analysis operation. Refer to the
* descriptions of the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad shells.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_shells(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);
/**
* @nodoc
* <b>Obsolete:</b> This API is obsolete.
* <br><br>
* Returns a list of all lumps in the input body that were marked as bad during healing analysis.
* <br><br>
* <b>Role:</b> This API returns a list of all lumps in the input body that were
* marked as bad during a healing analysis operation. Refer to the descriptions of
* the various analysis APIs for the tests that are performed.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param list
* list of bad lumps.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_get_bad_lumps(BODY *inp_body, ENTITY_LIST &list, AcisOptions* ao = NULL);

/**
* @nodoc
* Sets the option to start storing the history of every entity during healing.
* <br><br>
* <b>Role:</b> This API can be used to switch set the option to store the
* history of main events that every entity undergoes during healing. If the
* <tt>option_value</tt> is <tt>TRUE</tt>, the history is stored in readable text form in attributes.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param inp_body
* input body.
* @param option_value
* store history if TRUE.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]
DECL_HEAL outcome api_hh_store_entity_details(BODY *inp_body, logical option_value, AcisOptions* ao = NULL);
/**
* @nodoc
* Returns the history of an entity during healing.
* <br><br>
* <b>Role:</b> This API returns a description of the major changes that the
* given entity (an <tt>EDGE</tt> or <tt>FACE</tt>) underwent during healing in readable text format.
* The description gets stored during healing of each entity (only if the option to
* store the history is put on - refer <tt>api_hh_store_entity_details</tt>). Output is <tt>NULL</tt>
* if no history is found. It is the responsibility of the caller to delete memory
* allocated to <tt>char* history</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D InterOp
* <br><br>
* @param entity
* input edge or face.
* @param history
* history of entity.
* @param ao
* acis options.
**/
[[deprecated("Deprecated API")]]   
DECL_HEAL outcome api_hh_get_entity_details(ENTITY *entity, char* &history, AcisOptions* ao = NULL);
/**
* @nodoc
* Function for setting the Log File pointer (default value is NULL)
* For internal use only.
*/
DECL_HEAL int hh_set_bhl_log_file(FILE*);
/**
* @nodoc
* Function for getting the current value of the Log File pointer
* For internal use only.
*/
DECL_HEAL FILE* hh_get_bhl_log_file(void);
/**
* @nodoc
* Function for getting the current value of the Log File pointer
* For internal use only.
*/
DECL_HEAL void hh_reset_bhl_log_file(void);
/**
* @nodoc
* Function for setting the simplification callback pointer
* For internal use only.
*/
typedef void(*HH_SIMPLIFY_OPT_CALLBACK) (BODY*);
/**
* @nodoc
* For internal use only.
*/
DECL_HEAL void hh_set_simplify_opt_callback(HH_SIMPLIFY_OPT_CALLBACK funcp);
/**
* @nodoc
* Function for getting the simplification callback pointer
* For internal use only.
*/
DECL_HEAL HH_SIMPLIFY_OPT_CALLBACK hh_get_simplify_opt_callback();
/**
* @nodoc
* Function for setting the geombuild callback pointer
* For internal use only.
*/
typedef void(*HH_GEOMBUILD_OPT_CALLBACK) (BODY*);
/**
* @nodoc
* For internal use only.
*/
DECL_HEAL void hh_set_geombuild_opt_callback(HH_GEOMBUILD_OPT_CALLBACK funcp);
/**
* @nodoc
* Function for getting the geombuild callback pointer
* For internal use only.
*/
DECL_HEAL HH_GEOMBUILD_OPT_CALLBACK hh_get_geombuild_opt_callback();
/**
* @nodoc
**/
DECL_HEAL outcome api_fix_face_coedge_sense(
	ENTITY *ent,
	AcisOptions *ao = NULL);
#endif

