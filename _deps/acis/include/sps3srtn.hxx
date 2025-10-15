/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//  This file contains prototypes of routines which construct, manipulate,
//  interrogate a spline surface.  The routines are categorised in the
//  following order:
//
//      *   CONSTRUCTION    ( surface constructors )
//      *   SET_FUNCTIONS   ( surface modifiers )
//      *   INTERROGATION   ( surface interrogators )
//      *   OBSOLETE        ( obsolete routines )
//      *   REMOVED ROUTINES( Routines which will be removed in this release ).
//

#if !defined( SG_BS3_SURFACE_ROUTINES )
#define SG_BS3_SURFACE_ROUTINES
#include "base.hxx"
#include "dcl_spl.h"
#include "logical.h"
#include "bs3surf.hxx"
#include "bs3curve.hxx"
class SPAinterval;
#include "vector.hxx"
#include "unitvec.hxx"
#include "position.hxx"
/**
 * @file sps3srtn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3SURFACEAPI
 *
 * @{
 */
class curve;
class ENTITY_LIST;
// CONSTRUCTION:
// *****************************************************************
//  Spline Surface Construction Routines prototypes
// *****************************************************************
/**
 * @nodoc For internal use only
 */
DECL_SPLINE void bs3_surface_determine_form_u( bs3_surface bs );


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE void bs3_surface_determine_form_v( bs3_surface bs );

// Create a 3D B-spline surface specified as a sequence of control
// points and an associated knot vectors and other information as follows:
//
//	INPUT:
//		degree_u	:	Input the surface degree in u direction.
//		rational_u	:	Input TRUE, if the surface has to rational in
//						u-direction, make sure to supply the weights too.
//		form_u		:	Input any one of the following values:
//							0	:	surface is "open 	" in u-direction.
//							1	:	surface is "closed 	" in u-direction.
//							2	:	surface is "periodic" in u-direction.
//						   else	:	Surface form is determined.
//		pole_u		:	Input any one of the following values
//							0	:	surface is not singular at both u ends.
//							1	:	surface is singular at u-start.
//							2	:	surface is singular at u-end.
//							3	:	surface is singular at bot u-start & u-end.
//					*(int *)NULL_REF:	Surface singularity is determined.
//		num_ctrlpts_u:	Input the number of control points in u-direction
//		degree_v	:	Input the surface degree in v direction.
//		rational_v	:	Input TRUE, if the surface has to rational in
//						v-direction, make sure to supply the weights too.
//		form_v		:	Input any one of the following values:
//							0	:	surface is "open 	" in v-direction.
//							1	:	surface is "closed 	" in v-direction.
//							2	:	surface is "periodic" in v-direction.
//						   else	:	Surface form is determined.
//		pole_v		:	Input any one of the following values
//							0	:	surface is not singular at both v ends.
//							1	:	surface is singular at v-start.
//							2	:	surface is singular at v-end.
//							3	:	surface is singular at bot v-start & v-end.
//					*(int *)NULL_REF:	Surface singularity is determined.
//		num_ctrlpts_v:	Input the number of control points in v-direction
//		ctrlpts		:	Array of positions input which specify the control
//						points.  The order in which the control points are
//						input is "[num_u][num_v]".  So the length of the
//						array is (num_u * num_v).
//		weights		:	This is ignored, if the surface is non-rational in both
//						u and v directions.  But if the surface is rational in
//						either/both directions this has to be a array of doubles
//						( double(num_u*num_v) order "[num_u][num_v]" ) which
//						specify the weights.
//		ctrlpt_tol	:	Not Used.
//		num_u_knots	:	Input the number of knots in u-direction.
//		num_v_knots	:	Input the number of knots in v-direction.
//		uknots		:	Array of doubles ( double[num_u_knots] ) containing the
//						surface knots in u-direction
//		vknots		:	Array of doubles ( double[num_v_knots] ) containing the
//						surface knots in v-direction
//	ACIS requires that any bs3-surface to have multiple end_knots upto degree+1
//	such that the surface interpolates the boundaries.  However if one does not
//	give the knots in such a manner than the first
//	"num_ctrlpts_u/v - degree_u/v + 1" knots are only used.  However if one
//	does give the multiple end knots, than the number of knots have to be
//	" num_ctrlpts_u/v + degree_u/v + 1".
//
//		knot_tol	:	This is tolerance values used to distinguish knots.
//
//	OUTPUT:
//		A bs3_surface is returned as a function return.  This will be
//		NULL if the construction is unsuccessfull.
//
//	Restrictions:
//	------------
//		*	The knots input have to be in an strictly increasing order.
//

/**
* Creates a B-spline %surface from a collection of control points and knot vectors.
* <br><br>
* <b>Role:</b> The B-spline is defined by the given sequence of control points and
* knots. If the argument <tt>rational_u</tt> is <tt>TRUE</tt>, the %surface is rational in the <i>u</i>
* parameter; if <tt>FALSE</tt>, it is not. Similarly, for <tt>rational_v</tt>.
* <br><br>
* The argument <tt>form_u</tt> specifies whether the %surface is open (0), closed (1), or
* periodic (2) in the <i>u</i> direction. Similarly for <tt>form_v</tt>.
* <br><br>
* The argument <tt>pole_u</tt> indicates whether or not the %surface has a singularity at
* the <i>u</i>-min or <i>u</i>-max parameter boundaries according to the following:
* <br><br>
* 0 = No singularity at <i>u</i>-min or <i>u</i>-max boundary
* <br>
* 1 = Has a singularity at the <i>u</i>-min boundary
* <br>
* 2 = Has a singularity at the <i>u</i>-max boundary
* <br>
* 3 = Has a singularity at both boundaries
* <br><br>
* Similarly for <tt>pole_v</tt>.
* <br><br>
* The control points are contained in an array of positions. The <i>v</i> index varies
* first. That is, a row of <i>v</i> control points for the first <i>u</i> value is found first.
* Then, the row of <i>v</i> control points for the next <i>u</i> value. If the %surface is rational
* in either parameter, it is considered a rational %surface and the associated weights
* are in the array of doubles. The values in this array are in the same sequential
* order as the control points.
* <br><br>
* The <tt>point_tol</tt> tolerance value determines when two control points are identical and
* the <tt>knot_tol</tt> tolerance value performs the same function for the knot sequence.
* <br><br>
* <b>Limitations:</b> The knots input have to be in an strictly increasing order.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param degree_u
* degree in u.
* @param rational_u
* rational in u.
* @param form_u
* type in u.
* @param pole_u
* pole in u.
* @param num_ctrlpts_u
* number of control points in u.
* @param degree_v
* degree in v.
* @param rational_v
* rational in v.
* @param form_v
* type in v .
* @param pole_v
* pole in v.
* @param num_ctrlpts_v
* number of control points in v.
* @param ctrlpts
* position control points.
* @param weights
* weights.
* @param ctrlpt_tol
* tolerance to determine if two control points are the same.
* @param num_knots_u
* number of knots in u.
* @param knots_u
* knots in u.
* @param num_knots_v
* number of knots in v.
* @param knots_v
* knots in v.
* @param knot_tol
* tolerance to determine if two knots are the same
**/
DECL_SPLINE bs3_surface
bs3_surface_from_ctrlpts(
	int					degree_u,
	logical				rational_u,
	int					form_u,
	int					&pole_u,
	int					num_ctrlpts_u,
	int					degree_v,
	logical				rational_v,
	int					form_v,
	int					&pole_v,
	int					num_ctrlpts_v,
	const SPAposition	 	ctrlpts[],
	const double	 	weights[],
	double				ctrlpt_tol,
	int					num_knots_u,
	const double		knots_u[],
	int					num_knots_v,
	const double		knots_v[],
	double				knot_tol
);


/**
* Creates a B-spline %surface from a collection of control points and knot vectors, using a periodic representation.
* <br><br>
* <b>Role:</b> The B-spline is defined by the given sequence of control points and
* knots. If the argument <tt>rational_u</tt> is <tt>TRUE</tt>, the %surface is rational in the <i>u</i>
* parameter; if <tt>FALSE</tt>, it is not. Similarly, for <tt>rational_v</tt>.
* <br><br>
* The argument <tt>form_u</tt> specifies whether the %surface is open (0), closed (1), or
* periodic (2) in the <i>u</i> direction. Similarly for <tt>form_v</tt>.
* <br><br>
* The argument <tt>pole_u</tt> indicates whether or not the %surface has a singularity at
* the <i>u</i>-min or <i>u</i>-max parameter boundaries according to the following:
* <br><br>
* 0 = No singularity at <i>u</i>-min or <i>u</i>-max boundary
* <br>
* 1 = Has a singularity at the <i>u</i>-min boundary
* <br>
* 2 = Has a singularity at the <i>u</i>-max boundary
* <br>
* 3 = Has a singularity at both boundaries
* <br><br>
* Similarly for <tt>pole_v</tt>.
* <br><br>
* The control points are contained in an array of positions. The <i>v</i> index varies
* first. That is, a row of <i>v</i> control points for the first <i>u</i> value is found first.
* Then, the row of <i>v</i> control points for the next <i>u</i> value. If the %surface is rational
* in either parameter, it is considered a rational %surface and the associated weights
* are in the array of doubles. The values in this array are in the same sequential
* order as the control points.
* <br><br>
* The <tt>point_tol</tt> tolerance value determines when two control points are identical and
* the <tt>knot_tol</tt> tolerance value performs the same function for the knot sequence.
* <br><br>
* The only difference between this function and @href bs3_surface_from_ctrlpts is that this function
* expects the input for a %surface periodic in <i>u</i> and/or <i>v</i> to to be in 'periodic' form.
* That is, given a knot vector in <i>u</i> or <i>v</i> of length N+1, there will be N control points
* in that direction, with distinct first and last control points. The knot vector must be unclamped
* (multiplicity 1) at its start. The difference between the first and last input knot values determines
* the parametric period of the %surface in the relevant parameter.
* <br><br>
* Note that the %surface constructed from periodic input will still be clamped at both ends.
* <br><br>
* <b>Limitations:</b> The knots input have to be in an strictly increasing order.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param degree_u
* degree in u.
* @param rational_u
* rational in u.
* @param form_u
* type in u.
* @param pole_u
* pole in u.
* @param num_ctrlpts_u
* number of control points in u.
* @param degree_v
* degree in v.
* @param rational_v
* rational in v.
* @param form_v
* type in v .
* @param pole_v
* pole in v.
* @param num_ctrlpts_v
* number of control points in v.
* @param ctrlpts
* position control points.
* @param weights
* weights.
* @param ctrlpt_tol
* tolerance to determine if two control points are the same.
* @param num_knots_u
* number of knots in u.
* @param knots_u
* knots in u.
* @param num_knots_v
* number of knots in v.
* @param knots_v
* knots in v.
* @param knot_tol
* tolerance to determine if two knots are the same
**/
DECL_SPLINE bs3_surface
bs3_surface_from_periodic_ctrlpts(
	int					degree_u,
	logical				rational_u,
	int					form_u,
	int					&pole_u,
	int					num_ctrlpts_u,
	int					degree_v,
	logical				rational_v,
	int					form_v,
	int					&pole_v,
	int					num_ctrlpts_v,
	const SPAposition	 	ctrlpts[],
	const double	 	weights[],
	double				ctrlpt_tol,
	int					num_knots_u,
	const double		knots_u[],
	int					num_knots_v,
	const double		knots_v[],
	double				knot_tol
);

// Routine which interpolates a mesh points.  Also, one can optionally
// specify  the u and v tangent directions to be interpolated at this points.
//
//	INPUT:
//		num_u	:	Number of points to interpolated in u direction
//		num_v	:	Number of points to interpolated in v direction
//		pts		:	Array of positions( order "[num_v][num_u]", size
//					"num_v*num_u" ), which specify the points to be
//					interpolated.
//		du_s	:	Array of unit_vectors("num_v") which specify the
//					u-tangent direction at "vstart".  This may be NULL.
//		du_e	:	Array of unit_vectors("num_v") which specify the
//					u-tangent direction at "vend".  This may be NULL.
//		dv_s	:	Array of unit_vectors("num_u") which specify the
//					v-tangent direction at "ustart".  This may be NULL.
//		dv_e	:	Array of unit_vectors("num_u") which specify the
//					v-tangent direction at "uend".  This may be NULL.
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	Restrictions:
//		*	No two Adjacent points to be interpolated can be same with
//			in tolerance.  However the interpolated points can be same at
//			start and end for closed surfaces, so does other types of
//			configurations are allowed which result in self-intersecting
//			surfaces.

/**
* Interpolates a mesh of points.
* <br><br>
* <b>Role:</b> Interpolates a mesh of points, optionally specifying the <i>u</i> and <i>v</i>
* tangent directions along the edges of the %surface.
* <br><br>
* The points are stored in a single dimension array with <i>u</i> varying first, then <i>v</i>.
* The interpolation scheme is cubic in both the <i>u</i> and <i>v</i> directions. The start and
* end derivatives must all be specified or all be <tt>NULL</tt>.
* <br><br>
* <b>Limitations:</b> No two adjacent points to be interpolated can be same with in
* tolerance. However the interpolated points can be same at start and end for
* closed surfaces, so other types of configurations are allowed which result in
* self-intersecting surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param num_u
* number of points in u.
* @param num_v
* number of points in v.
* @param pts
* points [num_u][num_v].
* @param unit_vectordu_s
* u derivatives at start [num_v].
* @param unit_vectordu_e
* u derivatives at end [num_v].
* @param unit_vectordv_s
* v derivatives at start [num_u].
* @param unit_vectordv_e
* v derivatives at end [num_u].
**/

DECL_SPLINE bs3_surface
bs3_surface_intp(
	int					num_u,			// number of points in u direction
	int					num_v,			// number of points in v direction
	const SPAposition		pts[],			// array of xyz points
	const SPAunit_vector	unit_vectordu_s[],			// unit tangents at start in u dir
	const SPAunit_vector	unit_vectordu_e[],			// unit tangents at end in u dir
	const SPAunit_vector	unit_vectordv_s[],			// unit tangents at start in v dir
	const SPAunit_vector	unit_vectordv_e[]			// unit tangents at end in v dir
);



// Routine which returns a bs3_surface whic is a Fit to a mesh of points
// with in the specified tolerance, with optionally specified start and
// end derivatives in u directions only.
//
//	INPUT:
//		fittol	:	The resulting surface is within this tolerance to the
//					points to be interpolated
//		num_u	: 	Number of points to be fit in u direction.
//		num_v	: 	Number of points to be fit in u direction.
//		pts		:	Array of positions( order "[num_u][num_v]", size
//					"num_u*num_v" ), which specify the points to be
//					fitted.
//		du_s	:	Array of unit_vectors(size "num_v") which specify the
//					u-tangent direction at "vstart".  This may be NULL.
//		du_e	:	Array of unit_vectors(size "num_v") which specify the
//					u-tangent direction at "vend".  This may be NULL.
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	Restrictions:
//		*	No two Adjacent points to be fitted can be same with in tolerance.
//			However the fit points can be same at start and end for closed
//			surfaces, so does other types of configurations are allowed which
//			result in self-intersecting	surfaces.

/**
* Fits a mesh of points to a <tt>bs3_surface</tt>.
* <br><br>
* <b>Role:</b> Fits a mesh of points using optionally specified start and end
* derivatives in the <i>u</i> direction only. The start and end derivatives must all
* be specified or all be <tt>NULL</tt>.
* <br><br>
* The control points are contained in an array of positions. The <i>v</i> index varies
* first. That is, a row of <i>v</i> control points for the first <i>u</i> value is specified
* first. Then, the row of <i>v</i> control points for the next <i>u</i> value. The (<tt>num_v</tt>) <i>u</i>
* tangent vectors are specified in increasing <i>v</i> order.
* <br><br>
* <b>Errors:</b> If an error occurs, this routine returns a <tt>NULL</tt> %surface.
* <br><br>
* <b>Limitations:</b> The distance between any two adjacent points to be fitted can not
* be within tolerance. However the fit points can be the same at the start and
* end for closed surfaces, and similarly for other types of configurations which
* result in self-intersecting surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param fittol
* fitting tolerance.
* @param num_u
* number of points in u direction.
* @param num_v
* number of points in v direction.
* @param pts
* array of xyz points.
* @param du_s
* unit tangents at start in u dir.
* @param du_e
* unit tangents at end in u dir.
**/

DECL_SPLINE bs3_surface
bs3_surface_fit(
	double				fittol,			// fitting tolerance
	int					num_u,			// number of points in u direction
	int					num_v,			// number of points in v direction
	const SPAposition		pts[],			// array of xyz points
	const SPAunit_vector	du_s[],			// unit tangents at start in u dir
	const SPAunit_vector	du_e[]			// unit tangents at end in u dir
);



// This routine constructs a Ruled surface between the given two curves.
//
//	INPUT:
//		crv1	:	Curve 1 to be interpolated
//		crv2	:	Curve 2 to be interpolated
//
//	OUTPUT:
//		A bs3_surface, which interpolates the given two curves and is
//		simple ruled surface, is returned as a function return.
//

/**
* Creates a ruled %surface between two curves.
* <br><br>
* <b>Role:</b> This function can accept incompatible <tt>bs3_curves</tt>, meaning their
* knot vectors, rationality, or degree can be different.
* <br><br>
* <b>Errors:</b> If an error occurs, a <tt>NULL</tt> %surface is returned.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param crv1
* first %curve.
* @param crv2
* second %curve.
**/

DECL_SPLINE bs3_surface
bs3_surface_ruled(
	const bs3_curve& 	crv1, 	// first curve
	const bs3_curve&  	crv2 	// second curve
);




// Type define the prototypes for the evaluation function to be passed as
// input to the bs3_surface routine which constructs a bs3_surface
// approximation to a procedurally defined surface.

// return of FALSE => evaluation failed

// For internal use only
/**
 * @nodoc
 */
typedef logical (*pt_eval_fn_t) (
   double	u, 		// u param of evaluation point
   double	v, 		// v param of evaluation point
   void		*data, 	// data you passed to bs3_surface_proc, if any
   SPAposition	&pt 	// OUT: model space SPAposition at given uv location
);

// return of FALSE => evaluation failed

// For internal use only
/**
 * @nodoc
 */
typedef logical (*vec_eval_fn_t) (
   double	u, 		// u param of evaluation point
   double	v, 		// v param of evaluation point
   void		*data, 	// data you passed to bs3_surface_proc, if any
   SPAvector	&vec 	// OUT: SPAvector at given uv location
);

// Constructs an approximate bs3_surface to a procedurally defined surface.
//
//	INPUT:
//		u_interval	:	U Parameter range of the definition surface over which
//						the approximate is	constructed.
//		v_interval	:	V Parameter range of the definition surface over which
//						the approximate is	constructed.
//		pt_fn		:	Evaluator function for the surface defined, which must
//						implemented before this routine is called.  This
//						function has to be implemented to return a point on the
//						surface at a given (u,v) value.  The function prototype
//						must is as follows:
//						logical pt_fn(
//								double		u,		// u SPAparameter
//								double		v,		// v SPAparameter
//								void		*data,	// Any other data
//								SPAposition	&pt		// SPAposition returned.
//							);
//		du_fn		:	Evaluator function for the surface defined, which must
//						implemented before this routine is called.  This
//						function has to be implemented to return a u-partial on
//						the	surface at a given (u,v) value.  The function
//						prototype must is as follows:
//						logical du_fn(
//								double		u,		// u SPAparameter
//								double		v,		// v SPAparameter
//								void		*data,	// Any other data
//								SPAvector		&upar	// SPAposition returned.
//							);
//		dv_fn		:	Evaluator function for the surface defined, which must
//						implemented before this routine is called.  This
//						function has to be implemented to return a v-partial on
//						the	surface at a given (u,v) value.  The function
//						prototype must is as follows:
//						logical dv_fn(
//								double		u,		// u SPAparameter
//								double		v,		// v SPAparameter
//								void		*data,	// Any other data
//								SPAvector		&vpar	// SPAposition returned.
//							);
//		duv_fn		:	Evaluator function for the surface defined, which must
//						implemented before this routine is called.  This
//						function has to be implemented to return a uv-partial on
//						the	surface at a given (u,v) value.  The function
//						prototype must is as follows:
//						logical duv_fn(
//								double		u,		// u SPAparameter
//								double		v,		// v SPAparameter
//								void		*data,	// Any other data
//								SPAvector		&uvpar	// SPAposition returned.
//							);
//
//		data		:	This is any data that you choose to pass to evaluators
//						described above.
//		res			:	The approximate is fit to the given resolution.
//
//	OUTPUT:
//		A bs3_surface is returned which represents the given procedural
//		surface with in the tolerance "res".
//
//	NOTES:
//		User be-aware not to give too little value for "res", as this might
//		result in data explosion of the approximate surface ( A "res" value of
//		0.01 seems to work best ).

/**
* Creates an approximate B-spline %surface to a procedurally defined %surface.
* <br><br>
* <b>Role:</b> The %surface is defined over a parametric interval in <i>u</i> and <i>v</i>.
* Functions are supplied by the caller to calculate points on the %surface, tangent
* vectors, and second derivatives. This information is evaluated and used to
* define a %surface that is a fit to the evaluated data. If an error occurs, a <tt>NULL</tt>
* %surface is returned.
* <br><br>
* The following are the type definitions of the evaluation functions used in
* <tt>bs3_surface_proc</tt>.
* <br><br>
* The <tt>pt_fn</tt> evaluator function for the %surface must return a point on the %surface
* at a given (<i>u</i>,<i>v</i>) value:
* <pre>
* typedef logical
* (*pt_eval_fn_t) (  // return of FALSE =>
*       // evaluation failed
* double u,    // u param of evaluation
*       // point
* double v,    // v param of evaluation
*       // point
* void* data,    // data you passed to
*       // bs3_surface_proc
* SPAposition& pt   // OUT: model space
*       // position
*       // at given uv location
* );</pre>
* The <tt>du_fn</tt> evaluator function for the %surface must return a <i>u</i>-partial on the
* %surface at a given (<i>u</i>,<i>v</i>) value. The <tt>dv_fn</tt> evaluator function for the %surface
* must return a <i>v</i>-partial on the %surface at a given (<i>u</i>,<i>v</i>) value. The <tt>duv_fn</tt>
* evaluator function for the %surface must return a <i>uv</i>-partial on the %surface at a
* given (<i>u</i>,<i>v</i>) value. All have the form:
* <pre>
* typedef logical
* (*vec_eval_fn_t) (  // return of FALSE =>
*       // evaluation failed
* double u,    // u param of evaluation
*       // point
* double v,    // v param of evaluation
*       // point
* void* data,    // data you passed to
*       // bs3_surface_proc
* SPAvector& vec);  // OUT: vector at given uv
*       // location
* );</pre>
* <b>Limitations:</b> Do not give too small a value for <tt>res</tt>, because this can result
* in data explosion of the approximate %surface. A <tt>res</tt> value of 0.01 seems to work
* best.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param u_interval
* parameter range in u over which approximate %surface is constructed.
* @param v_interval
* parameter range in v over which approximate %surface is constructed.
* @param pt_fn
* model space point evaluator.
* @param du_fn
* ds/du evaluator.
* @param dv_fn
* ds/dv evaluator.
* @param duv_fn
* ds2/dudv evaluator.
* @param data
* data to be passed to evaluators.
* @param res
* fit resolution.
**/

DECL_SPLINE bs3_surface
bs3_surface_proc(
	SPAinterval& 		u_interval,	// SPAparameter range in u
	SPAinterval& 		v_interval,	// SPAparameter range in v
	pt_eval_fn_t 	pt_fn,		// eval fn for model space point
	vec_eval_fn_t 	du_fn,		// eval fn for ds/du
	vec_eval_fn_t 	dv_fn,		// eval fn for ds/dv
	vec_eval_fn_t 	duv_fn,		// eval fn for ds2/dudv
	void* 			data,		// data you wish to pass evaluators, if any
	double 			res 		// fit resolution
);


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE bs3_surface
bs3_surface_proc(
        int             num_u_knots,
		double* 		u_knots,
        int             num_v_knots,
        double*         v_knots,
		pt_eval_fn_t 	pt_fn,		// eval fn for model space point
		vec_eval_fn_t 	du_fn,		// eval fn for ds/du
		vec_eval_fn_t 	dv_fn,		// eval fn for ds/dv
		vec_eval_fn_t 	duv_fn,		// eval fn for ds2/dudv
		void* 			data,		// data you wish to pass evaluators, if any
		double 			res, 		// fit resolution
        int             max_iters,
        int&            number_of_iterations
);

// Routine which interpolates a mesh points, u tangent, v tangents and twist
// vectors at each point.  A bs3_surface surface is constructed and returned
// as the function return.  This routine also requires knot values associated
// with each point that is interpolated in both u and v direction.
//
//	INPUT:
//		num_upts	:	Number of points to interpolated in u direction
//		num_upts	:	Number of points to interpolated in v direction
//		u_params	:	Array of doubles(size num_upts) specify the knots
//						in u-direction
//		v_params	:	Array of doubles(size num_vpts) specify the knots
//						in v-direction
//		point		:	Array of positions( order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ), which specify the points to be
//						interpolated.
//		u_tans		:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the derivatives in
//						u-direction at each point.
//		v_tans		:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the derivatives in
//						v-direction at each point.
//		twists		:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the twist vectors
//						at each point.
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	Restrictions:
//		*	No two Adjacent points to be interpolated can be same with
//			in tolerance.  However the interpolated points can be same at
//			start and end for closed surfaces, so does other types of
//			configurations are allowed which result in self-intersecting
//			surfaces.

/**
* Creates a %surface that is a bi-cubic interpolant.
* <br><br>
* <b>Role:</b> This routine interpolates a mesh of points, <i>u</i> tangent, <i>v</i>
* tangents and twist vectors at each point. A <tt>bs3_surface</tt> is constructed and
* returned as the function return. The routine also requires knot values
* associated with each point that is interpolated in both <i>u</i> and <i>v</i> direction.
* <br><br>
* The control points are contained in an array of positions. The <i>v</i> index varies
* first. That is, a row of <i>v</i> control points for the first <i>u</i> value is specified
* first. Then, the row of <i>v</i> control points for the next <i>u</i> value. The other 2D
* arrays are specified in the same order.
* <br><br>
* The size of the points, <tt>u_trans</tt>, <tt>v_trans</tt>, and twists arrays is <tt>num_upts*num_vpts</tt>,
* and the ordering is <tt>[num_upts][num_vpts]</tt>.
* <br><br>
* <b>Errors:</b> If an error occurs, the function returns a <tt>NULL</tt> %surface.
* <br><br>
* <b>Limitations:</b> No two adjacent points to be interpolated can be same within
* tolerance. However the interpolated points can be same at start and end for
* closed surfaces, so does other types of configurations are allowed which result
* in self-intersecting surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param num_upts
* number of knots in u.
* @param num_vpts
* number of knots in v.
* @param u_params
* u knots, size num_upts.
* @param v_params
* v knots, size num_vpts.
* @param points
* points on %surface implicit 2D array.
* @param u_tans
* tangent vectors in u direction - implicit 2D array.
* @param v_tans
* tangent vectors in v direction - implicit 2D array.
* @param twists
* twist vectors - implicit 2D array.
**/

DECL_SPLINE bs3_surface
bs3_surface_bicubic(
	int 		num_upts, 	// number of knots in u
	int 		num_vpts, 	// number of knots in v
	double 		u_params[], // u knots
	double 		v_params[], // v knots
	SPAposition 	points[],	// points on surface. implicit 2D array
	SPAvector 		u_tans[],  	// tangent vectors in u direction. implicit
							// 2D array
	SPAvector 		v_tans[],  	// tangent vectors in v direction. implicit
							// 2D array
	SPAvector 		twists[] 	// twist vectors. implicit 2D array
);



// Define a spline surface which interpolates four boundary curves.
// The correspondance of the curves and the surface patch is:
//
//					bottom -------> S(u,v0)
//					top-----------> S(u,v1)
//					left----------> S(u0,v)
//					right---------> S(u1,v)
//
// If an error occurs, a NULL surface is returned. The original curves
// remain.

/**
* Creates a B-spline %surface that interpolates four boundary curves.
* <br><br>
* <b>Role:</b> The correspondence of the curves and the %surface patch is:
* <br><br>
* bottom--------> S (u,v0) (oriented left to right)<br>
* top-----------> S (u,v1) (oriented left to right)<br>
* left----------> S (u0,v) (oriented bottom to top)<br>
* right---------> S (u1,v) (oriented bottom to top)
* <br><br>
* <b>Errors:</b> If an error occurs, a <tt>NULL</tt> %surface is returned, and the original curves
* remain.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bottom
* edge defining %curve.
* @param right
* edge defining %curve.
* @param top
* edge defining %curve.
* @param left
* edge defining %curve.
**/

DECL_SPLINE bs3_surface
bs3_surface_4crv(
	const bs3_curve &bottom,
	const bs3_curve &right,
	const bs3_curve &top,
	const bs3_curve &left
);




// Define a spline surface which interpolates three boundary curves.
// The end points of the curves and their directions should match up
// as follows:
//
//                            A
//                            *
//                           / \
//                         /     \
//                 left  /         \ right
//                     /             \
//                   /                 \
//                 /                     \
//             B  *-----------------------*  C
//						  base
//
//	            left should go from B to A.
//				right should go from C to A.
//				base should go from B to C.
//
//
// The base defines the u parameterization and the left and right sides
// define the v. The apex A is a singularity. If an error occurs, a NULL
// surface is returned. The original curves remain.

/**
* Creates a B-spline %surface that interpolates three boundary curves.
* <br><br>
* <b>Role:</b> The end points of the curves and their directions match up.
* The base defines the <i>u</i> parameterization and the left and
* right sides define the <i>v</i>. The apex is a singularity.
* <br><br>
* <b>Errors:</b> If an error occurs, a <tt>NULL</tt> %surface is returned. The original curves
* remain.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param base
* edge defining %curve.
* @param right
* edge defining %curve.
* @param left
* edge defining %curve.
**/

DECL_SPLINE bs3_surface
bs3_surface_3crv(
	const bs3_curve &base,
	const bs3_curve &right,
	const bs3_curve &left
);



// This routine returns the offset of a given surface by given offset
// distance.  The resulting surface is atleast with the "offset_tol" of the
// true offset-surface.
//
//	INPUT:
//		orig_surf	:	The surface to be offset.
//		offset_dist	:	The distance by which the surface is offset.  A +ve
//						distance means to offset in the direction of the
//						surface normal, and a -ve distance means to offset in
//						direction opposite to the surface normal.
//		offset_tol	:	The resulting offset-surface constructed will be
//						atleast with in this tolerance value ( 0.001  is a
//						good value ).
//
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	RESTRICTIONS:
//		*	If the surface has any flat-spots ( zero length normals ) than
//			the offset-algorithm fails and a NULL surface is returned.
//		*	If a too low offset_tol is given, a potential data explosion
//			may happen in the result.

/**
* Offsets the given %surface by offset distance.
* <br><br>
* <b>Role:</b> It is assumed that the offset does not create a degenerate
* %surface.
* <br><br>
* A +ve distance means to offset in the direction of the %surface normal, and a -ve
* distance means to offset in direction opposite to the %surface normal.
* <br><br>
* The offset %surface constructed will be at least with in the <tt>offset_tol</tt> tolerance
* value ( 0.001 is a good value ).
* <br><br>
* <b>Errors:</b> If an error occurs, a <tt>NULL</tt> %surface is returned.
* <br><br>
* <b>Limitations:</b> If the %surface has any flat-spots (zero length normals) then the
* offset-algorithm fails and a <tt>NULL</tt> %surface is returned.
* <br><br>
* If a too low <tt>offset_tol</tt> is given, a potential data explosion may happen in the
* result.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param orig_surf
* given %surface.
* @param offset_dist
* distance to offset.
* @param offset_tol
* approximate tolerance.
**/

DECL_SPLINE bs3_surface
bs3_surface_offset(
	const bs3_surface	orig_surf,
	double 				offset_dist,
	double				offset_tol
);




// construct a surface that interpolates ("skins") between a sequence of
// curves.
//
//	INPUT:
//		curves	:	This is an entity-list of curves ( each of the entry in
//					this entity list must be an bs3_curve ).
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	RESTRICTIONS:
//		*	If the curve which are to be skinned cannot be made compatible
//			than a NULL surface is returned.
//		*	Also, experience has pointed that the below algorithm does not
//			handle rational curves correctly.  So limit calling this routine
//			with only non-rational input.

// surface made from interpolating between curve

/**
* Creates a %surface that interpolates (<tt>skins</tt>) between a sequence of <tt>bs3_curves</tt>.
* <br><br>
* <b>Role:</b> It does not handle coincident ends or first-order
* discontinuities.
* <br><br>
* <b>Errors:</b> If the curves cannot be made compatible, or if an error occurs, a <tt>NULL</tt>
* %surface is returned.
* <br><br>
* <b>Limitations:</b> Rational curves may not be handled properly. Use this function
* for non-rational curves.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param curves
* ordered list of bs3_curves.
**/

DECL_SPLINE bs3_surface
bs3_surface_skin_curves(
   	ENTITY_LIST& curves 	// ordered list of bs3_curves to
							// interpolate between
);




// This routine constructs a ruled surface from a point to a curve.
//
//  INPUT:
//      p   :   Point from which the ruled surface start.
//      crv :   Curve at which the ruled surface ends.
//
//  OUTPUT:
//      A bs3_surface is returned as a function return.
//
//  RESTRICTIONS:
//      *   Unexpected results for the case where the point is on the
//          curve.
//

/**
* Creates a triangular ruled %surface from a point to a %curve.
* <br><br>
* <b>Errors:</b> If an error occurs, a <tt>NULL</tt> %surface is returned.
* <br><br>
* <b>Limitations:</b> The point must not be on the %curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param p
* point on boundary of %surface.
* @param crv
* boundary %curve.
**/

DECL_SPLINE bs3_surface
bs3_surface_rule_from_pt(
	const SPAposition&     p,      // The given point
	const bs3_curve     crv     // The given curve
);




// See : bs3_surface_sweep1
// make a surface by sweeping a shape curve along a path, with twist controlled
// by a rail curve.
//	The sweep Surface is constructed as follows:
//		The sweep surface is the surface traced by sweeping the given shape
//	curve along the given path curve, where the twist of the shape is specified
//	by the given rail curve.  The surface constructed is only an approximate
//	one.
//
//	Some definitions:
//	----------------
//	Frenet-Frame:
//		* Any point on the path specifies the origin of the frenet frame
//		* The path tangent direction specifies the z-axis
//		* The x-axis is specified by SPAvector between the points of equal
//		  SPAparameter value on path and rail curves.
//		* The y-axis is then computed using the right-hand rule ( z * x ).
//
//
//	INPUT:
//		shape	:	curve to be swept
//		path	:	curve along which the shape is swept
//		rail	:	guide curve (controls the twist of the swept surface).
//
//	OUTPUT:
//		A bs3_surface which satisfies the sweep definition is returned as
//		a function return.
//
//	RESTRICTIONS/ASSUMPTIONS:
//		*	The shape, path and rail curves must be planar.
//		*	The path curve must start in the plane of the shape curve.
//		* 	The path and the rail curve must be compatible.
//		*	The path curve must be perpendicular to the shape curve plane at
//			the start of the path.
//		*	The path and the rail curves are in object space, however the
//			shape curve	has to be in the local space of the path-rail ( the
//			local-space is defined by the frenet frame at the start of
//			the path.
//
//	NOTE:
//		Use should be calling bs3_surface_sweep1, if he does not want to
//		bother about transforming the shape into the local-coordinate space
//		of the path-rail.


// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE bs3_surface
bs3_surface_sweep(
	const bs3_curve 	shape,	// curve to be swept
  	const bs3_curve 	path,	// path curve--locus of local
								// coordinate origin
   	const bs3_curve 	rail 	// rail curve--controls twist
);





// make a surface by sweeping a shape curve along a path, with twist controlled
// by a rail curve.
//	The sweep Surface is constructed as follows:
//		The sweep surface is the surface traced by sweeping the given shape
//	curve along the given path curve, where the twist of the shape is specified
//	by the given rail curve.  The surface constructed is only an approximate
//	one.
//
//	Some definitions:
//	----------------
//	Frenet-Frame:
//		* Any point on the path specifies the origin of the frenet frame
//		* The path tangent direction specifies the z-axis
//		* The x-axis is specified by SPAvector between the points of equal
//		  SPAparameter value on path and rail curves.
//		* The y-axis is then computed using the right-hand rule ( z * x ).
//
//
//	INPUT:
//		shape		:	curve to be swept
//		path		:	curve along which the shape is swept
//		rail		:	guide curve (controls the twist of the swept surface).
//		input_nor	:	Optional, It is used when the path curve is a line
//						bspline.
//
//	OUTPUT:
//		A bs3_surface which satisfies the sweep definition is returned as
//		a function return.
//
//	RESTRICTIONS/ASSUMPTIONS:
//		*	The shape, path and rail curves must be planar.
//		*	The path curve must start in the plane of the shape curve.
//		* 	The path and the rail curve must be compatible.
//		*	The path curve must be perpendicular to the shape curve plane at
//			the start of the path.
//
//	KLUDGE:
//		*	The input_nor when used is negated for certain consistency reasons
//			in the sweep algorithm.  At that time this seemed like the easiest
//			fix to this.  This should be cleaned-up in the both the sweep-code
//			and here at some point.
//

// For internal use only
/**
 * @nodoc
 */

DECL_SPLINE bs3_surface
bs3_surface_sweep1(
	bs3_curve	shape,		// Shape Curve
	bs3_curve	path,		// Path curve - also called as center curve
	bs3_curve	rail,		// rail curve
	SPAunit_vector &input_nor	// Input normal for line bspline
);





// Routine which interpolates a mesh points, u tangent, v tangents, uv tangents
// and also the corresponding knots.  This a simple hermite interpolant.
//
//	INPUT:
//		nu			:	Number of points to interpolated in u direction
//		nu			:	Number of points to interpolated in v direction
//		pts			:	Array of positions( order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ), which specify the points to be
//						interpolated.
//		u_partials	:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the derivatives in
//						u-direction at each point.
//		v_partials	:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the derivatives in
//						v-direction at each point.
//		uv_partials	:	Array of vectors(order "[num_upts][num_vpts]", size
//						"num_upts*num_vpts" ) which specify the twist vectors
//						at each point.
//		u_kntos		:	Array of doubles(size num_upts) specify the knots
//						in u-direction
//		v_knots		:	Array of doubles(size num_vpts) specify the knots
//						in v-direction
//	OUTPUT:
//		A bs3_surface is returned as a function return.
//
//	Restrictions:
//		*	No two Adjacent points to be interpolated can be same with
//			in tolerance.  However the interpolated points can be same at
//			start and end for closed surfaces, so does other types of
//			configurations are allowed which result in self-intersecting
//			surfaces.

/**
* Creates a bi-cubic Hermite interpolant using a mesh of points, tangents, twists, and knot vectors.
* <br><br>
* <b>Role:</b> Creates a bi-cubic Hermite interpolant using a mesh of points, <i>u</i>
* tangents, <i>v</i> tangents, <i>uv</i> twists, and the corresponding knot vectors.
* <br><br>
* <b>Errors:</b> If an error occurs, the function returns a <tt>NULL</tt> %surface.
* <br><br>
* <b>Limitations:</b> No two adjacent points to be interpolated can be same within tolerance.
* However the interpolated points can be same at start and end for closed surfaces,
* so other types of configurations are allowed which result in self-intersecting surfaces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param nu
* number of points in u.
* @param nv
* number of points in v.
* @param pts
* object space points array [nu][nv].
* @param u_partials
* u partial array [nu][nv].
* @param v_partials
* v partial array [nu][nv].
* @param uv_partials
* uv partial array [nu][nv].
* @param u_knots
* u knots [nu].
* @param v_knots
* v knots [nv].
**/

DECL_SPLINE bs3_surface
bs3_surface_hermite_intp(
	int 		nu,				// number of points in u
	int			nv,				// number of points in v
	SPAposition	*pts,			// object space points pts[nu][nv]
	SPAvector		*u_partials,	// u partial array [nu][nv]
	SPAvector		*v_partials,	// v partial array [nu][nv]
	SPAvector		*uv_partials,	// uv partial array [nu][nv]
	double		*u_knots,		// u knots [nu]
	double		*v_knots		// v knots [nv]
);





// SET_FUNCTIONS:
// *****************************************************************
//  Surface Setting routines ( Set functions  prototypes):
// *****************************************************************

// Mark the surface as being open in u

/**
* Sets the <tt>bs3_surface</tt> to be open in <tt>u</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_open_u(
	bs3_surface	surf		// Input surface
);

// Mark the surface as being open in v

/**
* Sets the <tt>bs3_surface</tt> to be open in <i>v</i>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_open_v(
	bs3_surface	surf		// Input surface
);

// Mark the surface as being closed in u

/**
* Sets the <tt>bs3_surface</tt> to be closed in <i>u</i>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_closed_u(
	bs3_surface	surf		// Input surface
);

// Mark the surface as being closed in v

/**
* Sets the <tt>bs3_surface</tt> to be closed in <i>v</i>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_closed_v(
	bs3_surface	surf		// Input surface
);

// Mark the surface as being periodic in u

/**
* Marks the %surface as being periodic in <i>u</i>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_periodic_u(
	bs3_surface	surf		// Input surface
);

// Mark the surface as being periodic in v

/**
* Marks the %surface as being periodic in <i>v</i>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param surf
* given %surface.
**/

DECL_SPLINE void
bs3_surface_set_periodic_v(
	bs3_surface	surf		// Input surface
);

// This routine adds multiple end knots to a spline surface.  This routine
// is always called for periodic surfaces.  Such that, the bspline control
// polygon always interpolates the corners of the surface.

/**
* Adds multiple end knots to a B-spline %surface.
* <br><br>
* <b>Role:</b> This routine is always called for periodic surfaces such that the
* B-spline control polygon always interpolates the corners of the %surface.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param input
* given %surface.
**/

DECL_SPLINE void
bs3_surface_add_mult_ekn(
		bs3_surface input
	);

// Add knot to a surface up to the requested multiplicity with in the given
// knot tolerance.  Return the number of knots added
// The input SPAparameter has to be with in the SPAparameter bounds of the given
// surface.  The final multiplicity of the added knot cannot be greater than
// the degree of the surface in the requested direction.
//
//  INPUT:
//      par     :   Parameter at which the knot is to be added
//      multp   :   Multiplicity requested at the added knot
//      in_sur  :   Surface which is modified.
//      u_or_v  :   Add the knot in the direction:
//                      0   :   u direction
//                      1   :   v direction
//      knot_tol:   Use this tolerance to distinguish between the knots.
//                  This is used to test wether the knot being added already
//                  exists.
//
//  OUTPUT:
//      *   The in_sur is modified in place
//      *   An integer is returned specifying the number of knots added.
//
//  KLUDGE:
//      *   Any error condition is ignored for now.
//

/**
* Adds knots to a %surface.
* <br><br>
* <b>Role:</b> Adds knot to a %surface up to the requested multiplicity with in
* the given knot tolerance. This routine returns the number of knots added.
* <br><br>
* The input parameter has to be with in the parameter bounds of the given %surface.
* The final multiplicity of the added knot cannot be greater than the degree of
* the %surface in the requested direction.
* <br><br>
* Use the tolerance to distinguish between the knots. It is used to test whether
* the knot being added already exists.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param par
* given parameter.
* @param multp
* multiplicity wanted at added knot.
* @param in_sur
* given %surface.
* @param u_or_v
* add a knot: 0=u, 1=v direction.
* @param knot_tol
* knot tolerance.
**/

DECL_SPLINE int
bs3_surface_add_knot(
    double          par,        // Input SPAparameter
    int             multp,      // Multiplicity wanted
    bs3_surface     in_sur,     // Input surface
    int             u_or_v,     // Add a knot in (= 0, u_dir; = 1, v_dir)
    double          knot_tol    // Knot tolerance
);




// INTERROGATION:
// *****************************************************************
//  Surface Interrogation routines ( Query  functions prototypes):
// *****************************************************************


// Return the dimension, degree in u and v, control points, and
// knots for a 3D B-spline surface. Also indicate if the surface
// is rational or not.
//
//	INPUT:
//		srf			:	A non-NULL pointer to a bs3_surface.
//
//	OUTPUT:
//		dim			:	Surface dimension returned ( 3, 2, 1 ).
//		rational_u	:	Set to TRUE if the input surface is rational in
//						u-direction, FALSE otherwise.
//		rational_v	:	Set to TRUE if the input surface is rational in
//						v-direction, FALSE otherwise.
//		form_u		:	Set to the following values:
//							0	:	surface is "open 	" in u-direction.
//							1	:	surface is "closed 	" in u-direction.
//							2	:	surface is "periodic" in u-direction.
//		form_v		:	Set to the following values:
//							0	:	surface is "open 	" in v-direction.
//							1	:	surface is "closed 	" in v-direction.
//							2	:	surface is "periodic" in v-direction.
//		pole_u		:	Set to the following values:
//							0	:	surface is not singular at both u ends.
//							1	:	surface is singular at u-start.
//							2	:	surface is singular at u-end.
//							3	:	surface is singular at bot u-start & u-end.
//		pole_v		:	Set to the following values:
//							0	:	surface is not singular at both v ends.
//							1	:	surface is singular at v-start.
//							2	:	surface is singular at v-end.
//							3	:	surface is singular at bot v-start & v-end.
//		num_u		:	Return the number of control points in u direction.
//		num_v		:	Return the number of control points in v direction.
//		ctrlpts		:	Array of positions returned which store the control
//						points.  The order in which the control points are
//						returned is "[num_u][num_v]".  So the length of the
//						array is (num_u * num_v).
//		weights		:	This will be NULL, if the surface is non-rational in
//						both u and v directions.  If the surface is rational in
//						either/both directions this will be a array of doubles
//						( double(num_u*num_v) order "[num_u][num_v]" ).
//		degree_u	:	Return the surface degree in u direction.
//		degree_v	:	Return the surface degree in v direction.
//
//		num_u_knots	:	Return the number of knots in u-direction.
//		num_v_knots	:	Return the number of knots in u-direction.
//		uknots		:	Array of doubles ( double[num_u_knots] ) containing the
//						surface knots in u-direction
//		vknots		:	Array of doubles ( double[num_v_knots] ) containing the
//						surface knots in v-direction
//	If the surface has multiple end knots, which it must be in ACIS, the
//	knot arrays returned have same knots at start and end upto respective
//	degrees + 1.
//

/**
* Creates arrays of control points, weights, <i>u</i> knots, and <i>v</i> knots from a B-spline %surface.
* <br><br>
* <b>Role:</b> The %surface is defined by an array of control points, weights,
* and knots in the <i>u</i> (<i>v</i>) parameter. The %surface may be rational in either <i>u</i> or <i>v</i>,
* it may be open, closed, or periodic in <i>u</i> or <i>v</i>, and it may have parametric
* singularities at the minimum or maximum parameter values in either <i>u</i> or <i>v</i>.
* <br><br>
* The control points are returned as an array of coordinates in the form (x,y,z)
* or (x,y,z).
* <br><br>
* The function creates arrays of control points, weights, <i>u</i> knots, and <i>v</i> knots. It
* is up to the application to delete these arrays.
* <br><br>
* <tt>rational_u</tt> and <tt>rational_v</tt> specify if %surface is rational (1) or not rational (0).
* <br><br>
* <tt>form_u</tt> and <tt>form_v</tt> specify if the %surface is open (0), closed (1), or periodic
* (2).
* <br><br>
* <tt>pole_u</tt> and <tt>pole_v</tt> specify if the %surface has poles at none (0), low <i>u/v</i> (1),
* high <i>u/v</i>, or both.
* <br><br>
* If the %surface has multiple end knots, the knot arrays returned have same knots
* at start and end up to respective <tt>degrees</tt> + 1.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param srf
* given %surface.
* @param dim
* returned dimension.
* @param rational_u
* returned rational in u.
* @param rational_v
* returned rational in v.
* @param form_u
* returned form in u.
* @param form_v
* returned form in v.
* @param pole_u
* returned poles in u.
* @param pole_v
* returned poles in v.
* @param num_u
* returned number of control points in u.
* @param num_v
* returned number of control points in v.
* @param ctrlpts
* returned control points in desired order.
* @param weights
* returned weights.
* @param degree_u
* returned degree in u.
* @param num_uknots
* returned number of knots in u.
* @param uknots
* returned knots in u.
* @param degree_v
* returned degree in v.
* @param num_vknots
* returned number of knots in v.
* @param vknots
* returned knots in v.
* @param use_bs3_seam_data
* for internal use only.
**/

DECL_SPLINE void
bs3_surface_to_array(
	bs3_surface srf,				// surface
	int&		dim,				// dimension
	logical&	rational_u,			// rational in u
	logical&	rational_v,			// rational in v
	int&		form_u,				// open, closed, periodic in u
	int&		form_v,				// open, closed, periodic in v
	int&		pole_u,				// pole in u0, un
	int&		pole_v,				// pole in v0, vn
	int&		num_u,				// number of points in u
	int&		num_v,				// number of points in v
	SPAposition*&	ctrlpts,			// control points in desired order
	double*&	weights,			// weights
	int&		degree_u,			// degree in u
	int&		num_uknots,			// number of knots in u
	double*&	uknots,				// knots in u
	int&		degree_v,			// degree in v
	int&		num_vknots,			// number of knots in v
	double*&	vknots,				// knots in v
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);




// Return the [i,j] control point of a given spline surface
//  INPUT:
//      in_sur      :   Surface which is interrogated
//      i           :   i'th point in u direction
//      j           :   j'th point in v direction
//  OUTUT:
//      ctrl_pos    :   Position set and returned
//      weight      :   Weight if any set and returned
//      is_rational :   Set TRUE if the given surface is rational in
//                      either/both u and v directions.
//      dimension   :   Dimension of the surface is returned ( <= 3 && > 0 )

/**
* Gets the [<i>i</i>,<i>j</i>] control point of a given B-spline %surface.
* <br><br>
* <b>Errors:</b> Returns (-1,-1,-1) on bad input: <tt>null</tt> %surface or negative indices.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param in_sur
* given %surface.
* @param i
* ith points in u direction.
* @param j
* jth points in v direction.
* @param ctrl_pos
* returned object space control point.
* @param weight
* weight if is_rational.
* @param is_rational
* set <tt>TRUE</tt> if %surface is rational in u and-or v.
* @param dimension
* dimension of the object space point, >3 is not supported.
**/

DECL_SPLINE void
bs3_surface_ij_ctrlpt(
    bs3_surface in_sur,     // Input surface
    int         i,          // I'th points in u_dir
    int         j,          // J'th points in v_dir
    SPAposition&   ctrl_pos,   // Return the object space control point
    double&     weight,     // Return weight if rational
    logical&    is_rational,// Use the weight if this logical is set TRUE
    int&        dimension,  // Dimension of the object space point returned,        
	                        // > 3 not supported
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);



// Return the i'th knot in u direction of the given spline surface
//  INPUT:
//      in_sur      :   Surface which is interrogated
//      i           :   i'th point in u direction
//      j           :   j'th point in v direction
//  OUTUT:
//      The u knot corresponding to the given i,j is returned as the
//      function return.

/**
* Gets the <i>i</i>th knot in <i>u</i> direction.
* <br><br>
* <b>Errors:</b> Returns -1 on bad input: null %surface or negative indices.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
* @param i
* ith point in u direction.
* @param j
* jth point in v direction.
**/

DECL_SPLINE double
bs3_surface_ij_knu(
    bs3_surface bs,			// Input surface
    int 		i,
    int 		j,
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);



// Return the j'th knot in v direction of the given spline surface
//  INPUT:
//      in_sur      :   Surface which is interrogated
//      i           :   i'th point in u direction
//      j           :   j'th point in v direction
//  OUTUT:
//      The v knot corresponding to the given i,j is returned as the
//      function return.

/**
* Gets the <i>i</i>th knot in <i>v</i> direction.
* <br><br>
* <b>Errors:</b> Returns -1 on bad input: null %surface or negative indices.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
* @param i
* ith point in u direction.
* @param j
* jth point in v direction.
**/

DECL_SPLINE double
bs3_surface_ij_knv(
    bs3_surface bs,		// Input surface
    int 		i,
    int 		j,
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);



// STI aed: add bs3_surface_planar
// Indicate whether surface is planar

/**
* Determines if a <tt>bs3_surface</tt> is planar.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the %surface is planar.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* %surface to check.
* @param pln_norm
* normal to the %plane.
**/

DECL_SPLINE logical
bs3_surface_planar(
	bs3_surface bs,			// Input surface
	SPAunit_vector &pln_norm	// Plane normal, if surface is planar
);
// STI aed: end



// Return the spline degree in u_direction

/**
* Gets the B-spline degree in the <i>u</i> direction.
* <br><br>
* <b>Errors:</b> Returns -1 if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

DECL_SPLINE int
bs3_surface_degree_u(
	bs3_surface	bs		// Input surface
);



// Return the spline degree in v_direction

/**
* Gets the B-spline degree in the <i>v</i> direction.
* <br><br>
* <b>Errors:</b> Returns -1 if the input %surface is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

DECL_SPLINE int
bs3_surface_degree_v(
	bs3_surface	bs		// Input surface
);

/**
* Gets the number of control points in <tt>u_direction</tt>.
* <br><br>
* <b>Errors:</b> Returns -1 if input is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

// Return the number of control points in u_direction
DECL_SPLINE int
bs3_surface_ncu(
	bs3_surface	bs,		// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);

/**
* Gets the number of control points in <tt>v_direction</tt>.
* <br><br>
* <b>Errors:</b> Returns -1 if input is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

// Return the number of control points in v direction
DECL_SPLINE int
bs3_surface_ncv(
	bs3_surface	bs,		// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);



// Return wether the surface is rational in u_direction

/**
* Determines if a %surface is rational in <i>u</i>.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the %surface is rational in <i>u</i>-direction,
* otherwise <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

DECL_SPLINE logical
bs3_surface_rational_u(
	bs3_surface	bs		// Input surface
);



// Return wether the surface is rational in v_direction

/**
* Determines if a %surface is rational in <i>v</i>.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the %surface is rational in <i>v</i>-direction,
* otherwise <tt>FALSE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* given %surface.
**/

DECL_SPLINE logical
bs3_surface_rational_v(
	bs3_surface	bs		// Input surface
);



// Returns if the given surface has multiple end-knots at ustart and
// uend.
//  INPUT:
//      in_sur  :   Surface which is tested
//  OUTPUT:
//      0   :   non mult
//      1   :   mult

/**
* Determines if %surface has multiple <i>u</i> end knots.
* <br><br>
* <b>Role:</b> If fully multiple end knots are at both the beginning and end of
* the %surface, the function returns 1; otherwise returns 0. Linear is assumed to
* be multiple.
* <br><br>
* <b>Errors:</b> Returns 0 if the input is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param in_sur
* given %surface.
**/

DECL_SPLINE int
bs3_surface_mult_eku(
    bs3_surface in_sur,	// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Determines if %surface has multiple <i>v</i> end knots.
* <br><br>
* <b>Role:</b> If fully multiple end knots are at both the beginning and end of
* the %surface, the function returns 1; otherwise returns 0. Linear is assumed to
* be multiple.
* <br><br>
* <b>Errors:</b> Returns 0 if input is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param in_sur
* given %surface.
**/
DECL_SPLINE int bs3_surface_mult_ekv(
	bs3_surface in_sur,	// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Gets the dimensionality of a %surface.
* <br><br>
* <b>Role:</b> Returns the dimensionality of the %surface. Usually this will be 3.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param in_sur
* input %surface.
**/
DECL_SPLINE int bs3_surface_dim(
	bs3_surface bs		// Input surface
);
/**
* Gets a flag indicating the existence of poles (singularities) in the <i>u</i> direction.
* <br><br>
* <b>Role:</b> Determines if the input %surface has any poles in the <i>u</i> direction
* and returns an indicator flag:
* <br><br>
* &nbsp;&nbsp;0 &nbsp;&nbsp;Surface is not singular at either end.
* <br>
* &nbsp;&nbsp;1 &nbsp;&nbsp;Surface is singular at <i>u</i>-start.
* <br>
* &nbsp;&nbsp;2 &nbsp;&nbsp;Surface is singular at <i>u</i>-end.
* <br>
* &nbsp;&nbsp;3 &nbsp;&nbsp;Surface is singular at <i>u</i>-start and <i>u</i>-end.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
**/
DECL_SPLINE int bs3_surface_poles_u(
	bs3_surface bs		// Input surface
);
/**
* Gets a flag indicating the existence of poles (singularities) in the <i>v</i> direction.
* <br><br>
* <b>Role:</b> Determines if the input %surface has any poles in the <i>v</i> direction
* and returns an indicator flag:
* <br><br>
* &nbsp;&nbsp;0 &nbsp;&nbsp;Surface is not singular at either end.
* <br>
* &nbsp;&nbsp;1 &nbsp;&nbsp;Surface is singular at <i>v</i>-start.
* <br>
* &nbsp;&nbsp;2 &nbsp;&nbsp;Surface is singular at <i>v</i>-end.
* <br>
* &nbsp;&nbsp;3 &nbsp;&nbsp;Surface is singular at <i>v</i>-start and <i>v</i>-end.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
**/
DECL_SPLINE int bs3_surface_poles_v(
	bs3_surface bs		// Input surface
);
/**
* Gets the number of knots in the <i>u</i> direction for the given %surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
**/
DECL_SPLINE int bs3_surface_nku(
	bs3_surface bs,		// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Gets the number of knots in the <i>v</i> direction for the given %surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
**/

DECL_SPLINE int bs3_surface_nkv(
	bs3_surface bs,		// Input surface
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Gets the number of knots in the <i>u</i> direction and the knot values in the <i>u</i> direction, for the given %surface.
* <br><br>
* <b>Role:</b> This function creates an array of knot points in the <i>u</i> direction
* for the given %surface. The knot multiplicity (i.e., the number of knots in the
* array with the same value), will be equal to the degree plus one at both ends of
* the array. It is the responsibility of the calling application to delete the
* knot array.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
* @param num_knots_u
* number of knots.
* @param uknots
* knot vector.
**/

DECL_SPLINE void bs3_surface_knots_u(
	bs3_surface bs,		// Input surface
	int& num_knots_u,	// number of knots
	double*& uknots,	// knot SPAvector
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);

//    Gets the number of knots in the v direction, and knot values
//    in the v direction, for the given surface.

/**
* Gets the number of knots in the <i>v</i> direction and the knot values in the <i>v</i> direction, for the given %surface.
* <br><br>
* <b>Role:</b> This function creates an array of knot points in the <i>v</i> direction
* for the given %surface. The knot multiplicity (i.e., the number of knots in the
* array with the same value), will be equal to the degree plus one at both ends of
* the array. It is the responsibility of the calling application to delete the
* knot array.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
* @param num_knots_v
* number of knots.
* @param vknots
* knot vector.
**/

DECL_SPLINE void bs3_surface_knots_v(
	bs3_surface bs,		// Input surface
	int& num_knots_v,	// number of knots
	double*& vknots,	// knot SPAvector
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Gets the number of control points in the <i>u</i> and <i>v</i> directions, and the array of control points, for the given %surface.
* <br><br>
* <b>Role:</b> This function creates an array of the control points for the
* given %surface. The length of the array is <tt>num_u*num_v</tt>. The order the control
* points are stored in the array is <tt>[u][v]</tt>, such that <i>v</i> increments more quickly.
* In other words, control point i,j, where i is the u index and j is the v index, is given by ctrlpts[num_v*i+j].
* It is the responsibility of the calling application to delete the control point
* array.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
* @param num_u
* number of u control points.
* @param num_v
* number of v control points.
* @param ctrlpts
* control points array.
**/

DECL_SPLINE void bs3_surface_control_points(
	bs3_surface bs,		// Input surface
	int& num_u,			// number of control point in u
	int& num_v,			// number of control point in v
	SPAposition*& ctrlpts,	// control point array
						// length of array is num_u*num_v
						// points stored in u,v order
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
* Gets the number of weights in the <i>u</i> and <i>v</i> directions and the array of weights for the given %surface.
* <br><br>
* <b>Role:</b> This function creates an array of weights for the given %surface.
* The length of the array is <tt>num_u*num_v</tt>. The order the weights are stored in
* the array is <tt>[u][v]</tt>, such that <i>v</i> increments more quickly. It is the responsibility
* of the calling application to delete the weights array.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param bs
* input %surface.
* @param num_u
* number of u control points.
* @param num_v
* number of v control points.
* @param weights
* array of weights.
**/
DECL_SPLINE void bs3_surface_weights(
	bs3_surface bs,		// Input surface
	int& num_u,			// number of weights in u
	int& num_v,			// number of weights in v
	double*& weights,	// weight array
						// length of array is num_u*num_v
						// points stored in u,v order
	const int use_bs3_seam_data = FALSE //in:  used for periodic geometry when initial knot multiplicty != order
);
/**
 * @nodoc
 * For internal use only
 */
enum bs3s_type { bs3s_unknown_type = 0,
				bs3s_plane_type = 1,
				bs3s_cylinder_type = 2,
				bs3s_cone_type = 3,
				bs3s_sphere_type = 4,
				bs3s_torus_type = 5,
				bs3s_srev_type = 6 };
/**
 * @nodoc
 * For internal use only
 */
DECL_SPLINE void bs3_surface_type(
	                            bs3_surface bs,     // Input surface
	                            bs3s_type& type		// surface type
                            );
/**
 * @nodoc
 * For internal use only
 */
DECL_SPLINE int bs3_surface_size(bs3_surface srf
);
/**
 *.
 */
enum bs3s_dir
{
    BS3SDIR_U  = 0,
    BS3SDIR_V  = 1,
    BS3SDIR_UV = 2
};

//
//  WARNING:  this changes the sur pointer in the bs3_surface
//
/**
* Raises the degree of the input <tt>bs3_surface</tt> in the desired direction by 1.  
* <br><br>
* <b>Role:</b> Valid directions are <tt>BS3SDIR_U</tt>, <tt>BS3SDIR_V</tt>,
* <tt>BS3SDIR_UV</tt> (both u and v degrees are raised by 1).
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param bs3
* given %surface.
* @param dir
* desired direction for degree elevation
*      choices are:  BS3SDIR_U   -- elevate in u direction
*                    BS3SDIR_V   -- elevate in v direction
*                    BS3SDIR_UV  -- elevate in both u and v
**/
DECL_SPLINE void 
bs3_surface_degree_elevate(
                           bs3_surface bs3,
                           bs3s_dir    dir
                           );


//================================================================================
//  recover seam multiplicity within a tolerance and then store
//  it off in the  bs3_surface structure (useful if the surface was
//  originally created without seam multiplicity information)
//================================================================================
// temporary fix to allow recovery of seam multiplicity for surfaces not
// created using the accepted periodic surface format.
/**
 * @nodoc
 */
DECL_SPLINE int
bs3_surface_determine_and_save_initial_seam_multiplicity(
	bs3_surface bs,                                                      
	double msTol
);

/**
* Remove redundant knots from a %surface.
* <br><br>
* <b>Role:</b> This function tests each knot for redundancy and removes it.
* Based on mode removes in <i>u</i> or <i>v</i> directions or both. 
* Recommended object space tolerance is SPAresabs
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param bs3
* given %surface.
* @param mode
* given mode.
* @param tol
* object space tolerance.
**/
DECL_SPLINE int
bs3_surface_rem_red_knots(
	bs3_surface bs3,		// Input surface
	int mode,				// Mode for removal direction
	double tol			    // tolerance 
);

/** @} */
#endif
