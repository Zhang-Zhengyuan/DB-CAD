/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DEPRECATED_MASSPR_HXX
#define DEPRECATED_MASSPR_HXX

#include "acis.hxx"
#include "dcl_cstr.h"
#include "api.hxx"
#include "position.hxx"

class BODY;
class tensor;

/**
* @nodoc
* <b>Obsolete:</b> use @href api_body_mass_props instead.
* Determines the mass properties of a body.
* <br><br>
* <b>Role:</b> This API function finds the volume, center of gravity and moments of
* inertia of the given body. The center of gravity is calculated with respect to
* the working coordinate system. The moments of inertia are also calculated with
* respect to the working coordinate system, which means that the first and second
* moments will change if the body is transformed.
* <br><br>
* The <tt>api_body_mass_pr</tt> function has two overloaded versions. This version allows the user
* to specify a root point and normal direction for the projection %plane.
* <br><br>
* The principal axes are defined with respect to the working coordinate system. The
* direction of each axis is defined by a unit vector. Each axis should be
* orthogonal to the other two axes.
* <br><br>
* The principal moments of inertia are with respect to the coordinate system
* defined by the center of gravity and the principal axes. (These three values
* should be relatively constant under translation and rotation  transformations,
* although their order may change depending on the principal axes.)
* <br><br>
* The major diagonal of the inertia tensor returned contains values for the
* inertias about each axis. For the inertia about the <i>x</i>-axis:
* <pre>(integral((y*y + z*z)dV)</pre>
* the off-diagonal terms are for the products of inertia; for example:
* <pre>(integral)((x*y)dV)</pre>
* The projection %plane is specified by a point and normal (in global body space).
* For speed, choose the %plane so that as many faces as possible project on to it
* as lines; i.e., are edge-on to it. To improve the accuracy of the result, set
* the %plane to pass through a point within the body; generally, this is the
* mid-point of the body's box.
* <br><br>
* The <tt>selector</tt> determines which properties are computed:
* <br>
* <table width = 100%>
* <tr width = 100%>
* <td width = 5%>0</td>
* <td>All properties (volume, center of gravity, and moments of inertia) are calculated.</td>
* </tr>
* <tr>
* <td>1</td>
* <td>Only volume and center of gravity are calculated.</td>
* </tr>
* <tr>
* <td>2</td>
* <td>Only volume is calculated.</td>
* </tr>
* </table>
* <br>
* Set the requested <tt>accuracy</tt> to the desired relative error. For
* example, entering 0.01 requests a maximum error of one percent. The application
* should find the mass properties for several requested accuracies and compare the
* results since there is a limitation to the current algorithm in that it does
* have a bound on how precisely the algorithm can calculate mass properties, due
* to hard coded convergence criteria in the functions.  If one tightens the
* relative error value and the mass properties values don't change, that means it
* is as close as can be achieved.
* <br><br>
* The values for mass properties would be expected to be different depending on
* the normal to the projection %plane for many types of objects.  One should
* attempt to select a projection %plane that will simplify the body as much as
* possible.  For instance, if the body were an extrusion, one would choose a %plane
* perpendicular to the extrusion direction.  This makes the numerical integration
* simpler.
* <br><br>
* Generally, double-sided (sheet or embedded) faces are ignored when calculating
* mass properties - The assumption being that a sheet of zero thickness has a zero
* volume. This behavior can be altered by changing the value of the
* <tt>sheet_thickness</tt> argument from its default of 0.0 to a positive value. This will
* cause <tt>api_body_mass_pr</tt> to treat double-sided sheets in the body as being
* approximations to thin-shell bodies with the given thickness, and contributions
* to the mass properties will be calculated for them also. Some caveats apply when
* using this:
* <br><br>
* 1) The approximation is only valid for 'small' values of the thickness. The
* range of thickness that can be considered 'small' is application dependent.
* <br>
* 2) Double-sided faces that meet non-tangentially or intersect will
* introduce errors as there is a double contribution to the mass properties from
* those regions of the faces that are within the thickness value of each other.
* The inside/outside flag for double-sided faces is taken into account, so a
* double-sided face embedded in a solid will reduce the volume if it is given a
* thickness value. Note that this means that an isolated 'inside' double-sided
* sheet will have a negative volume.
* <br><br>
* Although computation of mass properties does not make substantive changes to a
* model, boxes may be found and set into the model, creating a bulletin board. To
* make the process "read-only" in the application, call <tt>api_note_state</tt> before
* calling <tt>api_body_mass_pr</tt>, then call the following to roll the model back to its
* state before <tt>api_body_mass_pr</tt> was called:
* <pre>
* DELTA_STATE* ds = NULL;
* api_note_state(ds);
* api_change_state(ds);
* api_delete_ds(ds);</pre>
* <dl>
* <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
* <dd>&nbsp;A zero-length normal is given.</dd>
* <dd>&nbsp;A negative accuracy is requested.</dd>
* <dd>&nbsp;An invalid selector value is specified.</dd>
* <dd>&nbsp;A negative sheet thickness is specified.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param body
* body to be examined.
* @param root_proj_pl
* root point of projection %plane.
* @param normal_proj_pl
* normal of projection %plane.
* @param selector
* properties computed.
* @param req_rel_accy
* requested relative accuracy.
* @param volume
* returned volume.
* @param cofg
* returned center of gravity.
* @param inertia
* returned tensor (3 X 3).
* @param p_moments
* returned moments of inertia.
* @param p_axes
* returned axes for moments.
* @param est_rel_accy_achieved
* relative error.
* @param sheet_thickness
* thickness to apply to double sided sheets.
* @param ao
* ACIS options.
**/
DECL_CSTR outcome api_body_mass_pr(
	BODY* body,				            // object to be measured
	const SPAposition& root_proj_pl,	// root point of projection plane
	const SPAunit_vector& normal_proj_pl,// normal to projection plane
	int selector,				        // selector for highest order integral
										// required: 2 for volume only, 1 for
										// volume and centroid, 0 for volume,
										// centroid and inertia tensor.
	double req_rel_accy,				// required precision (as proportion of
										// volume)
	double& volume,			            // volume returned
	SPAposition& cofg,			        // centroid returned
	tensor& inertia,			        // inertia tensor returned
	double p_moments[3],			    // principal moments of inertia
										// returned
	SPAunit_vector p_axes[3],	        // principal axes of inertia returned
	double& est_rel_accy_achieved,	    // estimate of relative precision achieved
	double sheet_thickness = 0.0,       // Thickness for modeling double-sided
										// sheets with mass
	AcisOptions* ao = NULL
);

/**
* @nodoc
* <b>Obsolete:</b> use @href api_body_mass_props instead.
* Determines the mass properties of a body.
* <br><br>
* <b>Role:</b> This API function finds the volume, center of gravity and moments of
* inertia of the given body. The center of gravity is calculated with respect to
* the working coordinate system. The moments of inertia are also calculated with
* respect to the working coordinate system, which means that the first and second
* moments will change if the body is transformed.
* <br><br>
* The <tt>api_body_mass_pr</tt> function has two overloaded versions. This version
* chooses an appropriate projection %plane automatically.
* <br><br>
* The principal axes are defined with respect to the working coordinate system. The
* direction of each axis is defined by a unit vector. Each axis should be
* orthogonal to the other two axes.
* <br><br>
* The principal moments of inertia are with respect to the coordinate system
* defined by the center of gravity and the principal axes. (These three values
* should be relatively constant under translation and rotation  transformations,
* although their order may change depending on the principal axes.)
* <br><br>
* The major diagonal of the inertia tensor returned contains values for the
* inertias about each axis. For the inertia about the <i>x</i>-axis:
* <pre>(integral((y*y + z*z)dV)</pre>
* the off-diagonal terms are for the products of inertia; for example:
* <pre>(integral)((x*y)dV)</pre>
* The projection %plane is specified by a point and normal (in global body space).
* For speed, choose the %plane so that as many faces as possible project on to it
* as lines; i.e., are edge-on to it. To improve the accuracy of the result, set
* the %plane to pass through a point within the body; generally, this is the
* mid-point of the body's box.
* <br><br>
* The <tt>selector</tt> determines which properties are computed:
* <br>
* <table width = 100%>
* <tr width = 100%>
* <td width = 5%>0</td>
* <td>All properties (volume, center of gravity, and moments of inertia) are calculated.</td>
* </tr>
* <tr>
* <td>1</td>
* <td>Only volume and center of gravity are calculated.</td>
* </tr>
* <tr>
* <td>2</td>
* <td>Only volume is calculated.</td>
* </tr>
* </table>
* <br>
* Set the requested <tt>accuracy</tt> to the desired relative error. For
* example, entering 0.01 requests a maximum error of one percent. The application
* should find the mass properties for several requested accuracies and compare the
* results since there is a limitation to the current algorithm in that it does
* have a bound on how precisely the algorithm can calculate mass properties, due
* to hard coded convergence criteria in the functions.  If one tightens the
* relative error value and the mass properties values don't change, that means it
* is as close as can be achieved.
* <br><br>
* The values for mass properties would be expected to be different depending on
* the normal to the projection %plane for many types of objects.  One should
* attempt to select a projection %plane that will simplify the body as much as
* possible.  For instance, if the body were an extrusion, one would choose a %plane
* perpendicular to the extrusion direction.  This makes the numerical integration
* simpler.
* <br><br>
* Generally, double-sided (sheet or embedded) faces are ignored when calculating
* mass properties - The assumption being that a sheet of zero thickness has a zero
* volume. This behavior can be altered by changing the value of the
* <tt>sheet_thickness</tt> argument from its default of 0.0 to a positive value. This will
* cause <tt>api_body_mass_pr</tt> to treat double-sided sheets in the body as being
* approximations to thin-shell bodies with the given thickness, and contributions
* to the mass properties will be calculated for them also. Some caveats apply when
* using this:
* <br><br>
* 1) The approximation is only valid for 'small' values of the thickness. The
* range of thickness that can be considered 'small' is application dependent.
* <br>
* 2) Double-sided faces that meet non-tangentially or intersect will
* introduce errors as there is a double contribution to the mass properties from
* those regions of the faces that are within the thickness value of each other.
* The inside/outside flag for double-sided faces is taken into account, so a
* double-sided face embedded in a solid will reduce the volume if it is given a
* thickness value. Note that this means that an isolated 'inside' double-sided
* sheet will have a negative volume.
* <br><br>
* Although computation of mass properties does not make substantive changes to a
* model, boxes may be found and set into the model, creating a bulletin board. To
* make the process "read-only" in the application, call <tt>api_note_state</tt> before
* calling <tt>api_body_mass_pr</tt>, then call the following to roll the model back to its
* state before <tt>api_body_mass_pr</tt> was called:
* <pre>
* DELTA_STATE* ds = NULL;
* api_note_state(ds);
* api_change_state(ds);
* api_delete_ds(ds);</pre>
* <dl>
* <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
* <dd>&nbsp;A zero-length normal is given.</dd>
* <dd>&nbsp;A negative accuracy is requested.</dd>
* <dd>&nbsp;An invalid selector value is specified.</dd>
* <dd>&nbsp;A negative sheet thickness is specified.</dd>
* </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param body
* body to be examined.
* @param selector
* properties computed.
* @param req_rel_accy
* requested relative accuracy.
* @param volume
* returned volume.
* @param cofg
* returned center of gravity.
* @param inertia
* returned tensor (3 X 3).
* @param p_moments
* returned moments of inertia.
* @param p_axes
* returned axes for moments.
* @param est_rel_accy_achieved
* relative error.
* @param sheet_thickness
* thickness to apply to double sided sheets.
* @param ao
* ACIS options.
**/
DECL_CSTR outcome api_body_mass_pr(
	BODY* body,				        // object to be measured
	int selector,				    // selector for highest order integral
									// required: 2 for volume only, 1 for
									// volume and centroid, 0 for volume,
									// centroid and inertia tensor.
	double req_rel_accy,		    // required precision (as proportion of
									// volume)
	double& volume,			        // volume returned
	SPAposition& cofg,			    // centroid returned
	tensor& inertia,			    // inertia tensor returned
	double p_moments[3],			// principal moments of inertia
									// returned
	SPAunit_vector p_axes[3],	    // principal axes of inertia returned
	double& est_rel_accy_achieved,  // estimate of relative precision achieved
	double sheet_thickness = 0.0,   // Thickness for modeling double-sided
									// sheets with mass
	AcisOptions* ao = NULL
);


#endif

