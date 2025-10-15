/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for Warp Husk api routines.
#ifndef WARP_API_HXX
#define WARP_API_HXX
#include "dcl_warp.h"
#include "api.hxx"
/**
 * \defgroup WARPAPI Entity Warping
 *
 * Entity warping (sometimes simply called "warping") consists of bending, stretching, twisting, nonuniform scaling, 
 * and generalized warping. Generalized warping uses a law that defines a continuous mapping from R<sup>3</sup> to R<sup>3</sup>.
 */
/**
 * @file warp_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup WARPAPI
 *
 * @{
 */
class ENTITY;
class FACE;
class BODY;
class SPAposition;
class SPAunit_vector;
class law;
class curve_law;
class multiple_law;
class SPAbox;
//======================================================================

/**
 * Initializes the space warping library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
*/
 DECL_WARP outcome api_initialize_warp();

/**
 * Terminates the space warping library.
* <br>
* <b>Effect:</b> System routine
* <br>
* <b>Journal:</b> Not Available
* <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
*/

 DECL_WARP outcome api_terminate_warp();

/**
 * Modifies a body based on the given input law.
 * <br><br>
 * <b>Role:</b> The geometry of the input body is warped as specified by the given law
 * that defines a continuous mapping from R<sup>3</sup> to R<sup>3</sup>.
 * <br><br>
 * For more information on generalized space warping see the Technical Article <i>Warping Using a Law</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_body
 * body to warp.
 * @param in_law
 * law defining the warp.
 * @param opts
 * ACIS options.
 */
 DECL_WARP outcome api_space_warp(
            BODY* in_body,			    // Body to be warped
            law* in_law,			    // Warping law
            AcisOptions* opts = NULL    // version and journal options
		);


// Replace all faces and edges of the body to contain analytics or bsplines.
/**
 * @nodoc
 * <b>Obsolete:</b> Instead, use @href api_simplify_entity.<br><br>
 * Simplifies the faces and edges of a body.
 * <br><br>
 * <b>Role:</b> The faces and edges of an entity are replaced by analytic surfaces and curves,
 * if possible. Otherwise, they are replaced by their B-spline approximations.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_body
 * body to be simplified.
 * @param ao
 * ACIS options.
 */
 DECL_WARP outcome api_simplify_body(
		    BODY* in_body,					// Body to be simplified
			AcisOptions* ao = NULL
		);


// Replace face and edges of the face to contain analytics or bsplines.
/**
 * @nodoc
 * <b>Obsolete:</b> Instead, use @href api_simplify_entity.<br><br>
 * Simplifies the face and the edges of the face.
 * <br><br>
 * <b>Role:</b> The face and edges of the face are replaced by analytic surfaces and curves,
 * if possible. Otherwise, they are replaced by their B-spline approximations.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_face
 * face to be simplified.
 * @param reparam
 * for future use.
 * @param ao
 * ACIS options.
 */
 DECL_WARP outcome api_simplify_face(
		    FACE *in_face,				// Face to be simplified
			logical reparam = FALSE,    // For future use
			AcisOptions *ao = NULL
		);

/**
 * Modifies an entity by bending it about a specified axis.
 * <br><br>
 * <b>Role:</b> The neutral root, bending axis, and bending direction together define
 * the neutral %plane for the bending operation.  The neutral %plane is the location
 * along which material is neither stretched nor compressed during bending&mdash;material
 * above the neutral %plane (along the bending direction) is compressed, while material
 * below the neutral %plane is stretched.  The choice of a neutral %plane will vary with
 * the type of material to be bent.
 * <br><br>
 * The argument <tt>neutral_root</tt> is a position that lies on the neutral %plane.
 * <br><br>
 * The argument <tt>bend_axis</tt> is a unit vector that defines the rotational axis
 * of the bend.
 * <br><br>
 * The argument <tt>bend_direction</tt> is an auxiliary unit vector that defines the
 * direction of the bend.
 * <br><br>
 * Together with the neutral root, the bending axis (<tt>ba</tt>) and bending direction
 * (<tt>bd</tt>) define a bending %plane whose normal is the vector <tt>ba x bd</tt>.
 * (The bending axis and bending direction should be perpendicular to one another.)
 * This cross product additionally defines the "positive" and "negative" sides of the
 * entity to be bent.  (As remarked below, the positive side is that which is bent during
 * fixed-end bending.)
 * <br><br>
 * The argument <tt>radius</tt> is a real number that specifies the radius of the
 * bend, as measured from the center of the bend to the neutral %plane.
 * <br><br>
 * The argument <tt>angle</tt> is the angle, in radians, indicating the extent of
 * the bend.
 * <br><br>
 * The argument <tt>width</tt> is the width of bend region, as measured on the
 * neutral %plane.  Together, the values of <tt>radius</tt>, <tt>angle</tt>,
 * and <tt>width</tt> are used to determine the region to be bent.  Because only
 * two of these are independent, being related by
 * <tt>width = radius x angle</tt>, specifying <tt>width</tt>
 * can be considered optional.  More generally, assigning a zero angle or a non-positive
 * radius or width implies that the corresponding parameter is to be computed from the
 * other two.  Assigning meaningful values to all three generates an error.
 * If only <tt>radius</tt> is given a valid value, the entire entity is bent.
 * <br><br>
 * The flag <tt>f_center_bend</tt> (optional) specifies whether the desired bend is a
 * center bend or a fixed-end bend.  It controls the final orientation of the bend
 * entity.  When set to <tt>TRUE</tt>, the material on both sides of the bending %plane
 * is equally bent by one-half the <tt>angle</tt> value.  When not set, or set to
 * <tt>FALSE</tt>, the negative side of the bend is fixed (that is, only the positive
 * side is bent by the specified angle).  This mode of bending is called fixed-end
 * bending.
 * <br><br>
 * For cases where localized bending is desired, the (optional) array of positions
 * <tt>bend_regions</tt> may be used to specify the portions of the entity to bend.
 * One or more positions can be provided.  These positions must be either within the
 * components to be bent, or on their faces.  The given positions are tested to
 * determine if they are within the bending region, and must lie on a face in order to
 * be used.  If any of the bend positions is invalid (outside the bending region or
 * outside the entity and its faces), no bending is performed.
 * <br><br>
 * If the width of the specified bend region is small, a bend may fail no matter how
 * small the bend angle.  Specifically, when this width is comparable to the tolerances
 * of nearby entities, it becomes impossible even to define such a region.
 * <br><br>
 * For more information on bending operations see the Technical Article <i>Entity Bending</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_entity
 * entity to bend.
 * @param neutral_root
 * location on the neutral %plane.
 * @param bend_axis
 * bend axis.
 * @param bend_direction
 * bend direction.
 * @param radius
 * bend radius.
 * @param angle
 * bend angle (in radians) = 0.
 * @param width
 * bend width = 0 (<tt>radius x angle</tt>).
 * @param f_center_bend
 * bend is symmetric about the bend direction if <tt>TRUE</tt>.
 * @param n_points
 * number of positions in the <tt>bend_regions</tt> array.
 * @param bend_regions
 * array of positions on the faces to be bent (for localized bending).
 * @param opts
 * ACIS options.
 */
 DECL_WARP outcome api_bend_entity(
			ENTITY          *in_entity,		        // entity to bend
			SPAposition     &neutral_root,          // neutral plane location
			SPAunit_vector  &bend_axis,             // bending axis
			SPAunit_vector  &bend_direction,        // bending direction
			double          radius,			        // bend radius
			double          angle,			        // bend angle (in radian)
			double          width = -1,             // bend width (=radius*angle)
			logical         f_center_bend = FALSE,  // bending plane remains fixed
											        // if true
			int             n_points = 0,           // n positions given
			SPAposition     *bend_regions = NULL,   // positions on faces to be bent
			AcisOptions     *opts = NULL            // version and journal options
		);


/**
 * Modifies a bent entity by rebending it about a specified axis, through a new angle.
 * <br><br>
 * <b>Role:</b> The purpose of this API is to undo a bend and replace it by a similar
 * bend, only through a different angle.  If the new angle is set to zero, the bend
 * is merely undone, that is, flattened.  The parameter <tt>new_angle</tt> specifies
 * the new bend angle; all other parameters describe the bend being replaced, and are
 * maintained for the new bend except for <tt>radius</tt>, which is recomputed according
 * to <tt>radius = width/new_angle</tt>.
 * <br><br>
 * <i>Note:</i> This API does not require the original bend to have been produced
 * using @href api_bend_entity, only that equivalent parameter values be provided as
 * input.
 * <br><br>
 * The neutral root, bending axis, and bending direction together define
 * the neutral %plane for the rebending operation.  The neutral %plane is the location
 * along which material is neither stretched nor compressed during bending&mdash;material
 * above the neutral %plane (along the bending direction) is compressed, while material
 * below the neutral %plane is stretched.  The choice of a neutral %plane will vary with
 * the type of material to be bent.
 * <br><br>
 * The argument <tt>neutral_root</tt> is a position that lies on the neutral %plane.
 * <br><br>
 * The argument <tt>bend_axis</tt> is a unit vector that defines the rotational axis
 * of the bend.
 * <br><br>
 * The argument <tt>bend_direction</tt> is an auxiliary unit vector that defines the
 * direction of the bend.
 * <br><br>
 * Together with the neutral root, the bending axis (<tt>ba</tt>) and bending direction
 * (<tt>bd</tt>) define a bending %plane whose normal is the vector <tt>ba x bd</tt>.
 * (The bending axis and bending direction should be perpendicular to one another.)
 * This cross product additionally defines the "positive" and "negative" sides of the
 * entity to be bent.  (As remarked below, the positive side is that which is bent during
 * fixed-end bending.)
 * <br><br>
 * The argument <tt>radius</tt> is a real number that specifies the radius of the
 * bend, as measured from the center of the bend to the neutral %plane.
 * <br><br>
 * The argument <tt>old_angle</tt> is the angle, in radians, indicating the extent of
 * the original bend, while <tt>new_angle</tt> specifies the angle of the replacement
 * bend.
 * <br><br>
 * The argument <tt>width</tt> is the width of bend region, as measured on the
 * neutral %plane.  Together, the values of <tt>radius</tt>, <tt>old_angle</tt>,
 * and <tt>width</tt> are used to determine the region to be rebent.  Because only
 * two of these are independent, being related by
 * <tt>width = radius x old_angle</tt>, specifying <tt>width</tt>
 * can be considered optional.  More generally, assigning a zero angle or a non-positive
 * radius or width implies that the corresponding parameter is to be computed from the
 * other two.  Assigning meaningful values to all three generates an error.
 * If only <tt>radius</tt> is given a valid value, the entire entity is rebent.
 * <br><br>
 * The flag <tt>f_center_bend</tt> (optional) specifies whether the desired rebend is a
 * center bend or a fixed-end bend.  It controls the final orientation of the bend
 * entity.  When set to <tt>TRUE</tt>, the material on both sides of the bending %plane
 * is equally bent by one-half the <tt>new_angle</tt> value.  When not set, or set to
 * <tt>FALSE</tt>, the negative side of the bend is fixed (that is, only the positive
 * side is bent by the specified angle).  This mode of bending is called fixed-end
 * bending.
 * <br><br>
 * The (optional) array of positions <tt>rebend_regions</tt> may be used to specify
 * the portions of the input entity to rebend.  (These positions must be either
 * within the components to be rebent, or on their faces.)  This input is only
 * necessary where localized rebending is desired, that is, when not everything
 * within the rebend region is to be rebent.  However, it may be thought desirable
 * to pass this array in all cases, as it is not always entirely clear which entities
 * lie within the rebend region described by the other parameters.  Note that the
 * given positions are tested to determine if they are within the rebend region; if
 * any is found to be invalid (outside the region or outside the entity and its
 * faces), no rebending is performed.
 * <br><br>
 * Rebending from an angle of <tt>2pi</tt> or greater is not supported, nor is
 * rebending a sheet body.  Furthermore, if the width of the specified bend region is small,
 * a rebend may fail no matter how small the rebend angle.  Specifically, when this width is
 * comparable to the tolerances of nearby entities, it becomes impossible even to define such
 * a region.
 * <br><br>
 * For more information on rebending operations see the <i>Rebending and Unbending</i> section
 * of the Technical Article <i>Entity Bending</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_entity
 * entity to rebend.
 * @param neutral_root
 * location on the neutral %plane.
 * @param bend_axis
 * bend axis.
 * @param bend_direction
 * bend direction.
 * @param radius
 * bend radius.
 * @param old_angle
 * old bend angle (in radians).
 * @param new_angle
 * new bend angle (in radians).
 * @param width
 * bend width = -1 (<tt>radius x old_angle</tt>).
 * @param f_center_bend
 * bend is symmetric about the bend direction if <tt>TRUE</tt>.
 * @param n_points
 * number of positions in the <tt>rebend_regions</tt> array.
 * @param rebend_regions
 * array of positions on the faces to be bent (for localized rebending).
 * @param opts
 * ACIS options.
 */
 DECL_WARP outcome api_rebend_entity(
			ENTITY*			in_entity,		        // entity to bend
			SPAposition&	neutral_root,			// neutral plane location
			SPAunit_vector&	bend_axis,				// bending axis
			SPAunit_vector&	bend_direction,			// bending direction
			double          radius,			        // bend radius
			double          old_angle,			    // old bend angle (in radians)
			double          new_angle,			    // new bend angle (in radians)
			double          width = -1,             // bend width (=radius*angle)
			logical         f_center_bend = FALSE,	// bending plane remains fixed
											        // if true
			int             n_points = 0,           // n positions given
			SPAposition*	rebend_regions = NULL,	// positions on faces to be rebent
			AcisOptions*	opts = NULL				// version and journal options
		);


/**
 * Modifies an entity, or list of entities, by bending it according to the function that maps
 * a specified line to a specified curve.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_entity
 * entity to bend.
 * @param start
 * initial line's start point.
 * @param end
 * initial line's end point.
 * @param initial_rail
 * rail for initial line and final curve.
 * @param final_curve
 * final curve.
 * @param final_rail
 * rail for final curve.
 * @param opts
 * ACIS options.
 */
DECL_WARP outcome api_bend_to_curve_entity(
			BODY				    *in_entity,		    // entity to bend
			const SPAposition		&start,			    // initial line's start point
			const SPAposition		&end,			    // initial line's end point
			const SPAunit_vector    &initial_rail,	    // rail for initial line
			curve_law			    *final_curve,	    // final curve
			law					    *final_rail = NULL, // rail for final curve
			AcisOptions             *opts = NULL        // version and journal options
		);


/**
 * Creates a twist within a given region of a body about a specified axis and amount.
 * <br><br>
 * <b>Role:</b> Entity twisting is a specialized space-warping operation. The <tt>pos1</tt>
 * and <tt>pos2</tt> arguments represent the endpoints of an axis about which twisting is
 * to be performed, and can lie either inside or outside the <tt>body</tt>.  The region within which
 * twisting is to occur is bounded by the two planes through <tt>pos1</tt> and <tt>pos2</tt>,
 * and normal to this axis.
 * <br><br>
 * The <tt>theta1</tt> and <tt>theta2</tt> parameters are real numbers that specify, in radians,
 * the orientations of the non-twisted portions of the <tt>body</tt>.  Typically, <tt>theta1</tt> is chosen to be 0,
 * indicating that the portion of the body below the twist's starting region retains its orientation with
 * respect to the current coordinate system.  If a nonzero value for <tt>theta1</tt> is specified, on
 * the other hand, the entire <tt>body</tt> is rotated about the twist axis by the given amount before
 * the twist is performed.
 * <br><br>
 * The difference between <tt>theta1</tt> and <tt>theta2</tt> represents the amount, in radians, that the twist
 * region is warped about the axis.  Both <tt>theta1</tt> and <tt>theta2</tt> can be greater than or multiples
 * of +/- 2*pi radians in order to obtain multiple turns within the twisting region.
 * <br><br>
 * The continuity value can be 0, 1, or 2, which specify G<sup>0</sup>, G<sup>1</sup>, and G<sup>2</sup> continuity, 
 * respectively. (The interpolation function uses a linear, cubic, or quintic polynomial to obtain
 * G<sup>0</sup>, G<sup>1</sup>, or G<sup>2</sup> continuity between twisted and untwisted sections.)
 * <br><br>
 * If the width of the specified twist region is small, a twist may fail no matter how small 
 * the twist angle.  Specifically, when this width is comparable to the tolerances of nearby
 * entities, it becomes impossible even to define such a region.
 * <br><br>
 * For more information on twisting operations see the Technical Article <i>Entity Twisting</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param body
 * body to twist.
 * @param pos1
 * position at start of twist axis.
 * @param pos2
 * position at end of twist axis.
 * @param theta1
 * rotation at start of twist axis.
 * @param theta2
 * rotation at end of twist axis.
 * @param continuity
 * continuity (G<sup>0</sup>, G<sup>1</sup>, or G<sup>2</sup>) required of the twist.
 * @param opts
 * ACIS options.
 */
 DECL_WARP outcome api_twist_entity(
			ENTITY		    *body,		    // the body to twist
			SPAposition	    &pos1,		    // start position of twist axis
			SPAposition	    &pos2,		    // end position of twist axis
			double		    theta1,		    // the radians to twist at the start position
			double		    theta2,		    // the radians to twist at the end position
			int			    continuity,     // the continuity between twisted and untwisted sections (0, 1, or 2)
			AcisOptions     *opts = NULL    // version and journal options
		);

/**
 * @nodoc
 */
 DECL_WARP outcome api_bulge_entity(
			ENTITY		*body,		// the body to twist
			SPAposition	&pos1,		// start position of arrow
			SPAposition	&pos2, 		// end position of arrow
			AcisOptions *opts = NULL
			);

/**
 * Creates a stretch for a given region of a body along a specified axis.
 * <br><br>
 * <b>Role:</b> Entity stretching is a specialized space-warping operation.
 * The <tt>pos1</tt> and <tt>pos2</tt> arguments represent the endpoints of an axis along which stretching is
 * to be performed, and can lie either inside or outside the <tt>body</tt>.  The region within which
 * stretching is to occur is bounded by the two planes through <tt>pos1</tt> and <tt>pos2</tt>, and normal to
 * this axis.
 * <br><br>
 * The <tt>dis1</tt> and <tt>dis2</tt> parameters are real numbers that specify the translations of the
 * non-stretched portions of the body.  Typically, <tt>dis1</tt> is chosen to be 0, indicating that
 * the portion of the body below the stretch's starting region retains its position.  If a
 * nonzero value for <tt>dis1</tt> is specified, on the other hand, the entire body is translated
 * along the stretch axis by the given distance before the stretch is performed.
 * <br><br>
 * Let <tt>heightA</tt> be the distance between <tt>pos1</tt> and <tt>pos2</tt>.
 * <br><br>
 * Let <tt>heightB</tt> be <tt>heightA + (dis2-dis1)</tt>.
 * <br><br>
 * Let <tt>heightR</tt> be <tt>heightB/heightA</tt>.
 * <br><br>
 * Then <tt>heightR</tt> represents the amount of scaling to be applied to the stretch region of the body,
 * along the stretch axis.  (If <tt>heightR == 1</tt>, then no stretching or scaling need be applied.)
 * <br><br>
 * The <tt>continuity</tt> value refers the continuity between the stretched and unstretched sections.
 * Only 0 and 1 - indicating G<sup>0</sup> and G<sup>1</sup> continuity, respectively - are currently supported.  
 * <br><br>
 * If the width of the specified stretch region is small, a stretch may fail no matter how small 
 * the stretch distances.  Specifically, when this width is comparable to the tolerances of nearby
 * entities, it becomes impossible even to define such a region.
 * <br><br>
 * For more information on stretching operations see the Technical Article <i>Entity Stretching</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param body
 * body to stretch.
 * @param pos1
 * position at start of stretch axis.
 * @param pos2
 * position at end position of stretch axis.
 * @param dis1
 * offset distance at start of stretch axis.
 * @param dis2
 * offset distance at end of stretch axis.
 * @param continuity
 * continuity (G<sup>0</sup> or G<sup>1</sup>) required of the stretch (only 0 and 1 are currently supported).
 * @param opts
 * ACIS options.
 */
 DECL_WARP outcome api_stretch_entity(
			ENTITY		    *body,		    // the body to stretch
			const SPAposition	    &pos1,			// start position of stretch axis (defines plane1)
			const SPAposition	    &pos2,			// end position of stretch axis (defines plane2)
			double		    dis1,			// distance to offset points at plane 1
			double		    dis2,			// distance to offset points at plane 2
    	    int	            continuity,     // the continuity between stretched and unstretched sections
			AcisOptions     *opts = NULL    // version and journal options
		);

/**
 * @nodoc
 */
DECL_WARP outcome api_warp_entity_slice(
			ENTITY		    *body,		        // the body to twist
			SPAposition	    &pos0,		        // start position of warp axis
			SPAposition	    &pos1,		        // end position of warp axis
			multiple_law    *base_law0,         // 2-D law applied at pos0
			multiple_law    *base_law1 = NULL,  // 2-D law applied at pos1
			int             continuity = 0,     // desired continuity, 0, 1, or 2
			AcisOptions     *opts = NULL        // ACIS version and journal options
		);
/** @} */
#endif
