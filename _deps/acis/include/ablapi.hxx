/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( AB_HUSK_API_ROUTINES )
#define AB_HUSK_API_ROUTINES
#include "dcl_abl.h"
#include "acis.hxx"
#include "api.hxx"
/**
 * \defgroup ABLAPI Advanced Blending
 * \ingroup BLNDAPI
 *
 */
/**
 * @file ablapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ABLAPI
 *
 * @{
 */
class outcome;
class SPAposition;

class ENTITY;
class ENTITY_LIST;
class CURVE;
class EDGE;

#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "var_rad.hxx"
#include "blnd_sec.hxx"
#include "bl_inst.hxx"
#include "bl_cxty.hxx"

/**
* Initializes the advanced blending library.
* <br><br>
* <b>Role:</b> Initializes the advanced blending library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
**/
DECL_ABL outcome api_initialize_advanced_blending();


/**
* Terminates the advanced blending library.
* <br><br>
* <b>Role:</b> Terminates the advanced blending library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
**/
DECL_ABL outcome api_terminate_advanced_blending();


//
// General comments:
// In each routine declared here, all arguments that are
// passed as pointers become the property of the blend,
// and should not be deleted by the caller after calling
// the routine.
//

// Set an edge-sequence ABH blend on a list of edges,
// where the radius is given explicitly by a var_radius object.
// A calibration curve must be provided to parameterize the radius
// function, and to define the slicing plane ...
/**
* Sets an implicit variable radius blend on each of a list of edges or vertices. 
* The blend radius is described by one or two variable radius objects.
* <br><br>
* <b>Technical Articles:</b> <i>[Advanced Blending](http://doc.spatial.com/articles/a/d/v/Component~Advanced_Blending_90cb.html)</i>, 
* <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
* <br><br>
* <b>Role:</b> Implicit variable radius blends are attached as attributes to
* each of the edges or vertices in the list.
* <br>
* The blend radius is described by the <tt>var_radius</tt> object(s).
* <br><br>
* Setback at an end of an edge determines where the blend is to be stopped short
* of the vertex at the edge end. It is only significant when the vertex is blended.
* <br><br>
* If start stop angle or end stop angle values are specified, the edge blend is stopped and capped 
* with a stop plane at the given setback distance. This stop plane is constructed such that the angle 
* between stop plane and edge direction, about the line passing through end points of blend 
* cross section, is equal to the specified stop angle value at that end. 
* A stop angle value of 180 degrees is treated as a special case. In such a case, stop plane is 
* constructed such that it passes through an end vertex and end points of a blend cross section 
* at given setback distance.
* <br><br>
* <b>Limitations:</b> Entity-entity blends cannot create a stopped blend.
* The stopping location is based upon the edge being blended; thus, entity-entity blends do not have
* this defined location.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param entities
* entities to be blended.
* @param left_rad
* left radius function.
* @param right_rad
* right radius function, same as left if NULL.
* @param cross_sec
* cross section, circular if NULL.
* @param calibration_curve
* defining curve for the blend.
* @param first_edge
* first edge in smooth sequence.
* @param last_edge
* last edge in smooth sequence.
* @param face_edge1
* edge in a face to interpolate.
* @param face_edge2
* edge in other face to interpolate.
* @param smooth1
* tangency flag if face_edge1 not NULL.
* @param smooth2
* tangency flag if face_edge2 not.
* @param start_setback
* setback at start of edge.
* @param end_setback
* setback at end of edge.
* @param loft_edge
* curve defining lofted cross section.
* @param start_sbdiff
* setback difference at start.
* @param end_sbdiff
* setback difference at end.
* @param start_sbdiff_set
* setback difference set at start.
* @param end_sbdiff_set
* setback difference set at end.
* @param start_stop_ang
* start stop angle.
* @param end_stop_ang
* end stop angle.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_set_abh_blends(
		ENTITY_LIST const &entities,			// edges to be rounded
		var_radius *left_rad,					// left radius object
		var_radius *right_rad = NULL,			// right radius object, same as left if NULL
		var_cross_section *cross_sec = NULL,	// default CIRCULAR if NULL.
		CURVE *calibration_curve = NULL,		// calibration curve
		EDGE *first_edge = NULL,				// first edge in smooth sequence
		EDGE *last_edge = NULL,					// last edge in smooth sequence
		ENTITY *face_edge1 = NULL,				// first support edge
		ENTITY *face_edge2 = NULL,				// second support edge
		logical smooth1 = TRUE,					// whether tangent at first support edge
		logical smooth2 = TRUE,					// whether tan at second support edge
		double start_setback = 0,				// start setback
		double end_setback = 0,					// end setback
		EDGE *loft_edge = NULL,					// loft edge
		double start_sbdiff = 0,				// setback diff at start
		double end_sbdiff = 0,					// setback diff at end
		logical start_sbdiff_set = TRUE,		// setback diff set at start
		logical end_sbdiff_set = TRUE,			// setback diff set at end
		double start_stop_ang = 0,				// start stop angle
		double end_stop_ang = 0,				// end stop angle
		AcisOptions *ao = NULL
		);


// Set a constant radius entity-entity blend on a body. ent0 and
// ent1, the two entities actually being blended together, must be
// specified. Convexity of the blend, and a "help point", a rough
// starting point for the centre of the ball must also be provided.
// The help point should be provided in global coordinates; the routine
// itself will convert it into body coordinates.
/**
* Creates a constant radius entity-entity blend.
* <b>Technical Articles:</b> <i>[Advanced Blending](http://doc.spatial.com/articles/a/d/v/Component~Advanced_Blending_90cb.html)</i>, 
* <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
* <br><br>
* <b>Role:</b> Sets an implicit remote (constant or round radius)
* entity-entity blend on a body. The two entities that are to be blended,
* the convexity of the blend, and a rough starting point for the center of the
* ball must be provided.
* <br><br>
* <b>Errors:</b> Radius less than -<tt>SPAresabs</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param body
* body to be blended.
* @param ent0
* one of the entities to be rolled against.
* @param ent1
* the other entity to be rolled against.
* @param cxty
* indicates convexity type: <tt>bl_convex</tt> or <tt>bl_concave</tt>.
* @param hlp_pos
* rough starting point, given in global coordinates.
* @param radius
* blend radius.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_set_ee_cr_blend(
		BODY *body,					// body to be blended
		ENTITY *ent0,				// entities to blend between
		ENTITY *ent1,
		bl_convexity cxty,			// bl_convex or bl_concave
		SPAposition const &hlp_pos,	// rough starting point for center of ball
		double radius,				// blend radius
		AcisOptions *ao = NULL
		);


/**
* Creates a constant radius entity-entity round blend between two sets of 
* support entities.
* <b>Technical Articles:</b> <i>[Advanced Blending](http://doc.spatial.com/articles/a/d/v/Component~Advanced_Blending_90cb.html)</i>, 
* <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
* <br><br>
* <b>Role:</b> Sets an implicit constant radius entity-entity round 
* blend on the owner body when two sets of support entities are provided. 
* Use @href api_fix_blends to create the explicit blends. Support 
* entities in each set should be smoothly connected faces. The blend radius to 
* be given should be positive.
* <br><br>
* <b>Limitations:</b> <br>
* <ul>
* <li>All entities in the support lists must be faces. </li>
* <li>All faces must belong to the same body.</li>
* <li>Same face cannot be added to more than one support list.</li>
* </ul>
* <br><br>
* <b>Errors:</b><br>
* <ul>
* <li>Support entity list has an entity which is not a <tt>FACE</tt>.</li>
* <li>Support entities do not belong to the same parent body.</li>
* <li>Radius is smaller than <tt>SPAresabs</tt>.</li>
* </ul>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param left_ents
* first set of support entities to be rolled against.
* @param right_ents
* the other set of support entities to be rolled against.
* @param radius
* blend radius.
* @param help_pos
* rough starting point, in global coordinates indicating a location on blend spine
* @param cxty
* indicates convexity type: <tt>bl_convex</tt> or <tt>bl_concave</tt>.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_set_ee_cr_blend(
		ENTITY_LIST const &left_ents,		// left entities
		ENTITY_LIST const &right_ents,		// right entities
		double radius,						// blend radius
		SPAposition const &help_pos = *(SPAposition const *)NULL_REF,	// rough starting point for center of ball
		bl_convexity cxty = bl_convexity_unknown,	// bl_convex or bl_concave
		AcisOptions *ao = NULL
		);

// Set a variable-radius entity-entity blend
// with an explicit variable radius function.
// The radius function is defined along the
// defining curve (or "drive" curve).

/**
* Creates a variable radius entity-entity blend. The blend radius is described 
* by one or two variable radius objects.
* <br><br>
* <b>Role:</b> Sets a variable radius entity-entity blend, with an explicit
* variable radius function, on a body. The two entities which are to be blended,
* the convexity of the blend, and a rough starting point for the center of the
* ball must be provided, along with specification of the variable radius function
* for the blend and a defining curve for the entire sequence. The radius function
* is applied to the entire sequence of edges being blended. If a second radius
* function is given, then the first is used as the left radius and the second is
* the right. Left and right are defined by the direction of the defining curve,
* from outside the body. A cross section object can also be provided; the default
* is circular.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param body
* body to be blended.
* @param ent0
* one of the entities to be rolled against.
* @param ent1
* the other entity to be rolled against.
* @param cxty
* indicates convexity type: <tt>bl_convex</tt> or <tt>bl_concave</tt>.
* @param hlp_pos
* rough starting point, given in global coordinates.
* @param def_curve
* defining curve for the blend.
* @param left_rad
* left radius function.
* @param right_rad
* right radius function, same as left if NULL.
* @param vcs
* cross section.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_set_ee_vr_blend(
		BODY *body,							// body to be blended
		ENTITY *ent0,						// entities to blend between
		ENTITY *ent1,
		bl_convexity cxty,					// bl_convex or bl_concave
		SPAposition const &hlp_pos,			// rough starting point for center of ball
		curve const &def_curve,				// defining ("drive") curve
		var_radius *left_rad,				// left radius function
		var_radius *right_rad = NULL,		// same as left if null
		var_cross_section *vcs = NULL,		// cross section object
		AcisOptions *ao = NULL
		);


// Set a blend instruction attribute on some entity of the blended body.
// Instruction may be: cap, run_out, roll_on. The entity could typically
// be a face, vertex or edge. Passing the  action "unknown" deletes ALL
// instructions on the owning entity.

// A SPAposition may be passed to associate with the instruction to resolve
// ambiguities. Only one instruction per owner is allowed to refer to
// any particular SPAposition, and likewise, only one instruction per owner
// may have the SPAposition absent. Any existing ones are deleted to ensure
// this.

// The SPAposition should be provided in global coordinates; the routine
// itself will convert it into body coordinates (for this reason the
// body must also be supplied as an argument).
/**
* Sets a blend instruction on the indicated entity to be obeyed by the general 
* entity-entity blending algorithm.
* <br><br>
* <b>Role:</b> This API places a blend instruction attribute (<tt>ATT_BL_INST</tt>) on
* the given entity, recording in it the given action and position. No more than
* one instruction attribute is allowed at the same position. If the action passed
* is "unknown," then all instructions on the entity are deleted.
* <br><br>
* The blend instruction, or action, must be one of the values defined for the
* enumerated type <tt>blend_transition</tt>. The entity could typically be a face, vertex
* or edge. Passing the action <tt>blend_unknown</tt> deletes all instructions on the owning
* entity.
* <br><br>
* A position in global coordinates may be provided to associate with the
* instruction to resolve ambiguities. Only one instruction per owner is allowed to
* refer to any particular position, and likewise, only one instruction per owner
* may be specified without a position.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param body
* body to be blended.
* @param owner
* entity to own the instruction.
* @param action
* action instruction: <tt>blend_cap</tt>, <tt>blend_runout</tt>, <tt>blend_rollon</tt>, 
* or <tt>blend_unknown</tt>.
* @param p
* position (in global coordinates) to associate with the instruction to resolve ambiguities; 
* this may be NULL.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_set_inst_blend(
		BODY *body,			// blended body
		ENTITY *owner,		// entity with which the spring curve will interact
		blend_transition action,	// blend_cap, blend_runout, blend_rollon,
							// or blend_unknown
		SPAposition const &p = *(SPAposition const *)NULL_REF,
							// SPAposition indicator to resolve ambiguities
		AcisOptions *ao = NULL
		);

/**
 * Makes a sheet body of cylinder/torus/pipe surfaces around
 * the given sequence of edges.
 * <br><br>
 * <b>Role:</b> The intention is that this sheet is used
 * only in the subsequent construction of spring curves (refer to 
 * @href api_abh_slice and @href api_abh_imprint). Cylindrical extension surfaces
 * are stuck onto the ends of the sheet (if the edge sequence is not
 * closed) to ensure that the sheet body exceeds the blank body SPAbox.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
 * @param blank
 * original body.
 * @param radius
 * offset distance.
 * @param edge_list
 * smooth edge sequence to be offset.
 * @param sheet
 * output sheet body containing offset surface.
 * @param ao
 * ACIS options such as versioning and journaling.
 */
DECL_ABL outcome api_abh_edge_offset(
		BODY *blank,
		double radius,
		ENTITY_LIST &edge_list,
		BODY *&sheet,		
		AcisOptions *ao = NULL
		);

/**
* Creates a variable radius object with a constant radius.
* <br><br>
* <b>Role:</b> Creates a radius object with the constant radius <tt>rad</tt>. This is
* similar to standard blending with a constant radius. This radius object can then
* be used as an input to the blend API function @href api_set_abh_blends or
* @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param rad
* blend radius.
* @param rad_obj
* returned new radius object.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_constant(
		 double rad,			// blend radius
		 var_radius *&rad_obj,	// returned var_rad_object
		 AcisOptions *ao = NULL
		 );


/**
* Creates a variable radius object for a blend with the radii specified at the two ends of the blend.
* <br><br>
* <b>Role:</b> Creates a variable radius object for a blend with the radii
* specified at the two ends of the edge being blended. The blend starts at the
* left end with a radius of <tt>start_rad</tt> and tapers or expands to a radius of <tt>end_rad</tt>
* at the right end. Intermediate linearly varying radius values are implicitly
* calculated internally. This radius object can then be used as an input to the
* blend API function @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param start_rad
* radius at start.
* @param end_rad
* radius at end.
* @param rad_obj
* returned new radius object.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_two_ends(
         double start_rad,		// start radius
         double end_rad,		// end radius
         var_radius *&rad_obj,	// returned var_rad_object
		 AcisOptions *ao = NULL
		 );


/**
* Creates a variable radius object with a fixed width.
* <br><br>
* <b>Role:</b> Creates a radius object with a fixed <tt>width</tt> between the two
* spring curves of the blend. The actual blend radius is calculated implicitly
* along the blend to produce a blend with a constant cross sectional width. This
* radius object can then be used as an input to the blend API function
* @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param width
* fixed width of blend.
* @param rad_obj
* returned new radius object.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_fixed_width(
         double width,			// Constant width value of blend
         var_radius *&rad_obj,	// returned var_rad_object
		 AcisOptions *ao = NULL
		 );


/**
* Creates the two variable radius objects for a blend with a rounded chamfer cross section.
* <br><br>
* <b>Role:</b> Creates two variable radius objects that are used to create a
* blend with a rounded chamfer cross section. As the name suggests, a rounded
* chamfer is a chamfer with rounded cross section. The left and right offset
* values at the two ends are defined with respect to the defining edge. These
* radius objects can then be used as an input to the blend API function
* @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param start_left_off
* starting offset at left end of edge.
* @param end_left_off
* ending offset at left end of edge.
* @param start_right_off
* starting offset at right end of edge.
* @param end_right_off
* ending offset at right end of edge.
* @param rad_obj1
* returned new radius object 1.
* @param rad_obj2
* returned new radius object 2.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_rnd_chamfer(
         double start_left_off,		// start left offset
         double end_left_off,		// end left offset
         double start_right_off,	// start right offset
         double end_right_off,		// end right offset
         var_radius *&rad_obj1,		// returned var_rad_object
         var_radius *&rad_obj2,		// returned var_rad_object
		 AcisOptions *ao = NULL
		 );


/**
* Creates a variable radius object with a rotational ellipse cross section.
* <br><br>
* <b>Role:</b> Creates a variable radius object with a rotational ellipse cross
* section. The cross section at the start and end of the blend is specified by the
* defining parameters of the ellipse at the start (<tt>maj_start</tt>, <tt>min_start</tt>,
* <tt>ang_start</tt>) and end (<tt>maj_end</tt>, <tt>min_end</tt>, <tt>ang_end</tt>) of the blend. 
* One of the faces in
* the blend is designated as the reference face by the flag <tt>ref_face_left</tt>. The
* orientation of the ellipse between the faces it blends is determined by the two
* angles, <tt>ang_start</tt> and <tt>ang_end</tt>, which are the angles (specified in radians) made
* by the major axis with the normal of the reference face at the start and end of
* the blend. This radius object can then be used as an input to the blend API
* function @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param maj_start
* major axis of ellipse at blend start.
* @param maj_end
* major axis at blend end.
* @param min_start
* minor axis of ellipse at blend start.
* @param min_end
* minor axis at blend end.
* @param ang_start
* angle made by major axis with ref face normal at start.
* @param ang_end
* angle made by major axis with ref face normal at end.
* @param ref_face_left
* TRUE means left face is reference face.
* @param rad_obj
* returned new radius object.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_rot_ellipse(
         double maj_start,			// maj axis length at start
         double maj_end,			// major axis length at end
         double min_start,			// minor axis length at start
         double min_end,			// minor axis length at end
         double ang_start,			// angle made by maj axis with ref face normal at start
         double ang_end,			// angle made by maj axis with ref face normal at end
         logical ref_face_left,		// whether left face is reference
		 var_radius *&rad_obj,		// returned var_rad_object
		 AcisOptions *ao = NULL
		 );


// Two routines to create a radius function from <SPAposition,radius> pairs.
//
// In these two routines, the given positions must be represented in
// the same coordinate system as the input edge or calibration curve.
// For blending, the coordinate system should be that of the body being
// blended, not model space. If the calibration curve is created by
// api_smooth_edges_to_curve(), then it will be in the coordinate system
// of the body (i.e., that of the edges passed to it.) Therefore, if the
// positions are given in model space, they have to be transformed by
// the inverse of the blended body's transform before being passed to
// these routines.
/**
* Creates a variable radius object from position-radius pairs.
* <br><br>
* <b>Role:</b> Creates a variable radius object from position-radius pairs on
* the defining curve or edge. The positions and radius values are specified in
* arrays of length <tt>num_fixes</tt>. This radius object can then be used as an input to
* the blend API function @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* The calibration curve and the positions must be represented in the same
* coordinate system. For blending, the coordinate system should be that of the
* body being blended, not model space. If the calibration curve is created by
* @href api_smooth_edges_to_curve, then it will be in the coordinate system of the body
* (that is, that of the edges passed to it). Therefore, if the positions are given in
* model space, they have to be transformed by the inverse of the blended body's
* transform.
* <br><br>
* The <tt>start_slope</tt> value is interpreted as the slope (dy/dx) in 2D coordinates,
* where 0 is horizontal and 1 is 45 degrees. A <tt>NULL</tt> value means the slope is
* unspecified. The <tt>end_slope</tt> value is similar to the <tt>start_slope</tt> value.
* <br><br>
* <b>Errors:</b> If there was a problem, <tt>rad_obj</tt> is returned <tt>NULL</tt> 
* and the outcome has value <tt>API_FAILED</tt>. This will happen if less than two fixes 
* are specified, or if all positions map to the same point on the edge.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param def_crv_edge
* defining edge for the blend.
* @param num_fixes
* number of position-radius pairs.
* @param fix_positions
* array of position values.
* @param fix_radii
* array of radius values.
* @param rad_obj
* returned new radius object.
* @param start_slope
* start slope in 2D coordinates.
* @param end_slope
* end slope in 2D coordinates.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_pos_rads(
        EDGE *def_crv_edge,			// defining edge
        int num_fixes,				// number of specified positions and radii pairs
        SPAposition *fix_positions,	// array of positions
        double *fix_radii,			// array of radii corresponding to the positions
        var_radius *&rad_obj,		// returned var_rad_object
		double *start_slope = NULL,	// Value is interpreted as slope (dy/dx) in 2D
									// coordinates. 0 = horizontal, 1 = 45 degrees.
									// Pass NULL for 'unspecified'.
		double *end_slope = NULL,	// similar to start_slope.
		AcisOptions *ao = NULL
        );


/**
* Creates a variable radius object from position-radius pairs.
* <br><br>
* <b>Role:</b> Creates a variable radius object from position-radius pairs on
* the defining curve or edge. The positions and radius values are specified in
* arrays of length <tt>num_fixes</tt>. This radius object can then be used as an input to
* the blend API function @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* The calibration curve and the positions must be represented in the same
* coordinate system. For blending, the coordinate system should be that of the
* body being blended, not model space. If the calibration curve is created by
* @href api_smooth_edges_to_curve, then it will be in the coordinate system of the body
* (that is, that of the edges passed to it). Therefore, if the positions are given in
* model space, they have to be transformed by the inverse of the blended body's
* transform.
* <br><br>
* The <tt>start_slope value</tt> is interpreted as the slope (dy/dx) in 2D coordinates,
* where 0 is horizontal and 1 is 45 degrees. A <tt>NULL</tt> value means the slope is
* unspecified. The <tt>end_slope</tt> value is similar to the <tt>start_slope</tt> value.
* <br><br>
* <b>Errors:</b> If there was a problem, <tt>rad_obj</tt> is returned <tt>NULL</tt> and the outcome has
* value <tt>API_FAILED</tt>. This will happen if less than two fixes are specified, or if
* all positions map to the same point on the edge.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param calibration_curve
* defining curve for the blend.
* @param num_fixes
* number of position-radius pairs.
* @param fix_positions
* array of position values.
* @param fix_radii
* array of radius values.
* @param rad_obj
* returned new radius object.
* @param start_slope
* start slope in 2D coordinates.
* @param end_slope
* end slope in 2D coordinates.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_pos_rads(
		CURVE *calibration_curve ,  // defining curve
		int num_fixes, 				 // number of specified positions and radii pairs
		SPAposition *fix_positions,	 // array of positions
		double *fix_radii,  		 // array of radii corresponding to the positions
		var_radius *&rad_obj,    	 // returned var_rad_object
		double *start_slope = NULL,  // Value is interpreted as slope (dy/dx) in 2D
									 // coordinates. 0 = horizontal, 1 = 45 degrees.
									 // Pass NULL for 'unspecified'.
		double *end_slope = NULL,	 // similar to start_slope.
		AcisOptions *ao = NULL
        );


/**
* Creates a variable radius object from parameter-radius pairs.
* <br><br>
* <b>Role:</b> Creates a variable radius object from a set of parameter values
* along the blend and the radius values at each. This radius object can then be
* used as an input to the blend API function @href api_set_abh_blends or
* @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param num_fixes
* number of parameter radii pairs.
* @param fix_params
* array of parameter values.
* @param fix_radii
* array of radius values at given parameters.
* @param rad_obj
* returned new radius object.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_param_rads(
        int num_fixes,				// number of specified SPAparameter radii pairs
        double *fix_params,			// array of SPAparameter values
        double *fix_radii,			// array of radius values at above params
        var_radius *&rad_obj,		// returned var_rad_object
		AcisOptions *ao = NULL
		);


// Similar to api_make_radius_param_rads, but allows specification of the
// slope at the ends of the calibration curve.

/**
* Creates a variable radius object from parameter-radius pairs.
* <br><br>
* <b>Role:</b> Creates a variable radius object from a set of parameter values
* along the blend and the radius values at each parameter. Allows specification of
* the slope of the calibration curve at the start and end. The radius object
* created can then be used as an input to the blend functions @href api_set_abh_blends
* or @href api_set_ee_vr_blend.
* <br><br>
* The value of <tt>start_slope</tt> is interpreted as the slope (dy/dx) in 2D coordinates,
* where 0 is horizontal, and 1 is 45 degrees. Pass in <tt>NULL</tt> for <i>unspecified</i>.
* <tt>end_slope</tt> values are similar to <tt>start_slope values</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param num_fixes
* number of parameter radii pairs.
* @param fix_params
* array of parameter values.
* @param fix_radii
* array of radius values at given parameter.
* @param start_slope
* start slope (dy/dx) in 2D coords (0 = horizontal, 1 = 45 degrees, NULL for unspecified).
* @param end_slope
* end slope (dy/dx) in 2D coords (0 = horizontal, 1 = 45 degrees, NULL for unspecified).
* @param rad_obj
* radius object created.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_param_rads_tan(
        int num_fixes,				// number of specified SPAparameter radii pairs
        double *fix_params,			// array of SPAparameter values
        double *fix_radii,			// array of radius values at above params
		double *start_slope,		// Value is interpreted as slope (dy/dx) in 2D
									// coordinates. 0 = horizontal, 1 = 45 degrees.
									// Pass NULL for 'unspecified'.
		double *end_slope,			// similar to start_slope.
        var_radius *&rad_obj,		// returned var_rad_object
		AcisOptions *ao = NULL
		);


//    Create a variable radius object for a blend whose radius
//    function is specified by a 3D spline curve. The radius values
//    are taken from the y-coordinates of the 3D spline, and the
//    SPAparameter values are the x-coordinates. This allows the input
//    spline to be inspected visually, as a graph of y=f(x).
//    The z-coordinates are ignored internally. The SPAparameter range
//    (i.e., the range in x) of the input spline is unimportant, since
//    it will be mapped to match the edge being blended. This radius
//    object can then be used as an input to the blend API function
//    api_set_abh_blends or api_set_ee_vr_blend.
/**
* Creates a variable radius object for a blend whose radius function is specified by a 3D spline curve.
* <br><br>
* <b>Role:</b> Creates a variable radius object for a blend whose radius
* function is specified by a 3D spline curve. The radius values are taken from the
* y-coordinates of the 3D spline, and the parameter values are the x-coordinates.
* This allows the input spline to be inspected visually, as a graph of y=f(x). The
* z-coordinates are ignored internally. The parameter range (that is, the range in x)
* of the input spline is unimportant, because it will be mapped to match the edge
* being blended. This radius object can then be used as an input to the blend API
* function @href api_set_abh_blends or @href api_set_ee_vr_blend.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param rad_spl
* spline curve representing radius.
* @param rad_obj
* returned new radius object.
* @param calibration_curve
* defining curve for the blend.
* @param edge_periodic
* TRUE if edge is periodic.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_spline_rad(
         bs3_curve rad_spl,					// spline defining the rad function.
         var_radius *&rad_obj,				// returned var_rad_object
		 CURVE *calibration_curve = NULL,	// defining curve
		 logical edge_periodic = FALSE,		// edge is periodic.
		 AcisOptions *ao = NULL
		 );


/**
* Create a variable radius object for holdline blending.
* <br><br>
* <b>Role:</b> This API creates a variable radius object to be used in holdline blending.
* For more information on holdline blending, refer to the documentation for @href api_blend_holdline.
* The user of the API needs to specify a holdline in the form of an <tt>EDGE</tt> or <tt>CURVE</tt>
* entity. If holdline is specified as an <tt>EDGE</tt>, all of the edges smoothly connected to the
* specified edge will be included in the resulting holdline. If holdline is specified
* as a <tt>CURVE</tt>, only that curve will be used as the holdline. Therefore, if the user
* only wants a portion of a smoothly connected sequence of edges to be used as the
* holdline, he/she should build a <tt>CURVE</tt> out of the portion and then pass it to this
* API as holdline.
* <br><br>
* <b>Errors:</b><br>
* Pointer to holdline is <tt>NULL</tt>.<br>
* Holdline is not an <tt>EDGE</tt> or <tt>CURVE</tt> entity.
* <br><br>
* <b>Limitations:</b> The holdline <tt>EDGE</tt>/<tt>CURVE</tt> entity should be defined in the 
* same coordinate system as the edge being blended. In other words, the body containing holdline 
* should have the same tranformation (if any) as the body containing the edge to be blended.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param holdline
* EDGE or CURVE defining the holdline.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_make_radius_holdline(
         ENTITY *holdline,			// the holdline
         var_radius *&rad_obj,       // returned var_rad_object
         AcisOptions *ao = NULL
		 );


// API for offsetting edge
/**
* Creates an offset edge at a specified distance on the body parallel to a given edge.
* <br><br>
* <b>Role:</b> This API takes an edge on the body and imprints the edge
* parallel to itself on the body by the given distance. The direction of the
* imprint is provided by the logical <tt>left</tt>, which is the direction with respect to
* the underlying edge and the outward pointing vector <tt>v</tt>. The edge is first
* extended in both directions: projected upward on a plane normal to vector <tt>v</tt>;
* offset; and then reprojected on the body. If the logical <tt>sequence</tt> is specified,
* the smooth sequence of connected edges is handled. The modified body is returned.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param b
* given body.
* @param e
* edge on the body.
* @param sequence
* flag indicating whether sequence of smooth edges to be considered.
* @param dist
* offset distance.
* @param left
* direction of offset with respect to the edge.
* @param v
* outward pointing direction for the offset calculation.
* @param ao
* ACIS options such as versioning and journaling.
**/

DECL_ABL outcome api_abh_edge_project(
        BODY *b,
        EDGE *e,
		logical sequence,
	    double dist,
        logical left,
        SPAvector const &v,
		AcisOptions *ao = NULL
        );


// create a variable radius blend with cubic radius function
/**
* Creates a variable radius blend with smooth radius variation.
* <br><br>
* <b>Role:</b> This API function makes and marks an edge that can be used to
* create smooth transition variable radius blend. The radius function is a cubic
* bezier curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param in_ed
* edge to blend.
* @param r1
* start radius.
* @param r2
* end radius.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_abh_vblend (
		EDGE  *in_ed,			// edge to blend the variable radius
		double r1,              // start radius
		double r2,              // end radius
		AcisOptions *ao = NULL
		);


/**
* Creates a variable radius blend with smooth radius variation.
* <br><br>
* <b>Role:</b> This API function makes and marks an edge that can be used to
* create smooth transition variable radius blend. The radius function is a cubic
* bezier curve.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param in_eds
* edges to blend.
* @param r1
* start radius.
* @param r2
* end radius.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_abh_vblend (
		ENTITY_LIST const &eds,		// edges to blend the variable radius
		double r1,					// start radius
		double r2,					// end radius
		AcisOptions *ao = NULL
		);


/**
* Chamfers (variable and/or nonplanar) the given edges.
* <br><br>
* <b>Role:</b> This API function chamfers the given edges using the supplied
* left and right ranges, where left and right are with respect to the edge
* direction. For a chamfer with equal left and right ranges, the points where the
* chamfer surfaces meet the faces of the chamfered edge are the same as those for
* a round with radius equal to the chamfer range. If the ranges for left and right
* differ, the meeting points on the left face are the same as those for a round
* with radius equal to the left range, and the meeting points on the right face
* are the same as those for a round with radius equal to the right range.
* <br><br>
* Ranges are always positive. If the given right range is negative, a range equal
* to the left range is used, and similarly for the range values at the end of the
* edge; if the range value for the edge end is negative, the value for the
* beginning of the edge is used. The round chamfer radii are zero by default,
* which corresponds to the planar (flat) chamfer. Negative round chamfer radii
* result in a concave chamfer surface, and positive round chamfer radii result in
* a convex chamfer surface.
* <br><br>
* This API is intended to be used only for variable and/or nonplanar chamfers. To
* make a constant range planar chamfer (that is, if the last four arguments of this
* API have the default values), use @href api_chamfer_edges instead; it supports all
* types of edges.
* <br><br>
* If there are features lying inside the support region that is replaced 
* by blend, then blending attempts to retain those features. This behavior 
* can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
* fails to retain such features then it would continue to give a result 
* ignoring them. A <tt>sys_warning</tt> is raised when blending detects features, 
* but unable to retain them.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param edgs
* edges to chamfer.
* @param r1
* left range at edge beginning.
* @param r2
* right range at begin if negative equals r1.
* @param r1_end
* left range at edge end if negative equals r1.
* @param r2_end
* right range at end if negative equals r2.
* @param R1
* left round chamfer radius, zero results in flat chamfer.
* @param R2
* right round radius, zero equals flat.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_abh_chamfer_edges (
		ENTITY_LIST const &edgs,	// edges to apply const chamfer on
		double r1,					// left range
		double r2 = -1,				// right range (if negative, assumed equal r1)
		double r1_end = -1,			// left  range at the end ( <0 => =r1 )
		double r2_end = -1,			// right range at the end ( <0 => =r2 )
		double R1 = 0,				// left round chamfer radius (if =0 => flat chamfer)
		double R2 = 0,				// right round chamfer radius (if =0 => flat chamfer)
		AcisOptions *ao = NULL
		);


/**
* Applies a variable radius blend on the list of edges, simplifying the blend 
* for ends with constant blend radius.
* <br><br>
* <b>Role:</b> This API function substitutes for several calls to API functions
* which create a calibration curve and a radius object, apply the variable radius
* attribute, and, finally, fix the blend. In addition to facilitating the
* development and tracking of memory clean-up, this API function attempts to
* simplify the blend by substituting the variable radius attribute with the
* constant radius blend for the edges, within which the radius function is
* constant. If an attempt of simplification fails, this API function performs the
* regular variable radius blending.
* <br><br>
* The first four arguments (list of edges to blend, number of position-radius
* pairs, position and radius arrays) are required. Positions (specified in the
* global coordinate system) are projected on the calibration curve, composed of
* the given edges, to find the calibration curve parameter values, corresponding
* to the given radii.
* <br><br>
* The last two arguments, <tt>start_slope</tt> and <tt>end_slope</tt>, are optional 
* and make sense only for open calibration curves. They specify the derivative of 
* the radius function with respect to the parameter along the calibration curve. 
* Their value is interpreted as the slope (dR/dp) of the radius function vs. 
* parameter along the calibration curve:
* <pre>
* 0    =                horizontal
* 1    =                45 degrees
* NULL =                "unspecified"
* </pre>
* As the parameter speed is set arbitrarily (or, more accurately, depending on the
* first edge in the sequence), only zero slope value is independent of parameter
* speed and is safe and meaningful.
* <br><br>
* If there are features lying inside the support region that is replaced 
* by blend, then blending attempts to retain those features. This behavior 
* can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
* fails to retain such features then it would continue to give a result 
* ignoring them. A sys_warning is raised when blending detects features 
* but unable to retain them.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param eds
* edges to blend.
* @param num_fixes
* number of pairs.
* @param fix_positions
* array of positions.
* @param fix_radii
* array of radii.
* @param start_slope
* starting slope  value.
* @param end_slope
* ending slope value.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_blend_edges_pos_rad (
		ENTITY_LIST const &eds,			// edges to apply const chamfer on
		int const num_fixes,			// number of specified positions and radii pairs
		SPAposition  fix_positions[],	// array of positions
		double fix_radii[],				// array of radii corresponding to the positions
		double *start_slope = NULL,		// Value is interpreted as slope (dR/dp) of the radius function
										// vs. SPAparameter along the calibration curve: 0 == horizontal,
										// 1 == 45 degrees. (Only 0 is independent of SPAparameter speed.)
										// Pass NULL for 'unspecified'.
		double *end_slope = NULL,		// Same as start_slope.
		AcisOptions *ao = NULL
		);


/**
* Blends edges of a solid using a holdline.
* <br><br>
* <b>Role:</b> This API creates variable radius blends on edges using a holdline. The
* holdline is a curve that lies in the surface of one of the support faces of the blend.
* The blend surface will be tangent to that face at points along the holdline.
* The cross section of the blend surface is circular and the variable radius of the blend
* is driven by the holdline. 
* <br><br>
* The holdline can be specified by the user using either a
* <tt>CURVE</tt> or an <tt>EDGE</tt>. If specified using a <tt>CURVE</tt>, the <tt>CURVE</tt> 
* will be used as is. If specified using an <tt>EDGE</tt>, 
* either the <tt>EDGE</tt> or a smoothly connected sequence including
* the <tt>EDGE</tt> will be used to generate the holdline curve. The <tt>find_smooth_hl flag</tt>
* specifies whether to find edges smoothly connected to the specified holdline <tt>EDGE</tt>.
* The default value of this flag is TRUE (to find a smooth sequence). The edge to be
* blended is specified by an <tt>EDGE</tt> and a similar <tt>flag find_smooth_bl</tt> is used to 
* determine whether to find a smooth sequence of blend edges. The default value of this flag is
* also TRUE. 
* <br><br>
* Multiple blend sequences can be specified in a single call to the API.
* However, when doing this the user must specify the holdlines as <tt>EDGE</tt>s so that ACIS
* can attempt to match up blend sequences with holdlines using topological information.
* If two holdlines are found for a single blend sequence (for any given blend edge,
* two holdlines can be found that are a part of the edge's faces), ACIS will abort the
* operation because it will not know which one to choose. In such cases, the API should
* be called multiple times with single holdlines to eliminate ambiguities.
* <br><br>
* If there are features lying inside the support region that is replaced 
* by the blend, then blending attempts to retain those features. This behavior 
* can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
* fails to retain such features, then it would continue to give a result 
* ignoring them. A <tt>sys_warning</tt> is raised when blending detects features, 
* but unable to retain them.
* <br><br>
* <b>Errors:</b> Blend edges or holdlines are not specified correctly.
* <br><br>
* <b>Limitations:</b> The holdline should be defined in the same coordinate
* system as the edge being blended. In other words, the body containing the holdline
* should have the same tranformation (if any) as the body containing the edge to be
* blended. The user can only specify one holdline if it is specified using a <tt>CURVE</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param bl_list
* edges to be blended.
* @param hl_list
* entities defining the holdline(s).
* @param find_smooth_bl
* flag specifying whether to blend the edges smoothly connected to specified blend edges.
* @param find_smooth_hl
* flag specifying whether to use the edges smoothly connected to the specified holdline edges.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_blend_holdline(
		ENTITY_LIST const &bl_list,		// edges to be blended
		ENTITY_LIST const &hl_list,		// EDGEs or CURVEs defining holdline
		logical find_smooth_bl = TRUE,	// whether to find edges smoothly connected to blend edges
		logical find_smooth_hl = TRUE,	// whether to find edges smoothly connected to holdline(s)
		AcisOptions *ao = NULL 			// options, journal, version
		);


/**
* Performs entity-entity (face-face) blending using a holdline.
* <br><br>
* <b>Role:</b> This API creates variable radius entity-entity (face-face) blends using a holdline. 
* The holdline is a curve that lies in the surface of one of the support faces of the blend.
* The blend surface will be tangent to that face at points along the holdline.
* The cross section of the blend surface is circular and the variable radius of the blend
* is driven by the holdline. 
* <br><br>
* The holdline can be specified by the user using either a
* <tt>CURVE</tt> or an <tt>EDGE</tt>. If specified using a <tt>CURVE</tt>, the <tt>CURVE</tt> 
* will be used as is. If specified using an <tt>EDGE</tt>, 
* either the <tt>EDGE</tt> or a smoothly connected sequence including
* the <tt>EDGE</tt> will be used to generate the holdline curve. The <tt>find_smooth_hl flag</tt>
* specifies whether to find edges smoothly connected to the specified holdline <tt>EDGE</tt>.
* The default value of this flag is TRUE (to find a smooth sequence). The blend will start on 
* the two support faces specified and will roll onto other faces in the same manner general
* entity-entity blending does (refer to the documentation for entity-entity blending). 
* The help position convexity is the same as those used in general entity-entity blending.
* <br><br>
* <b>Errors:</b> Faces or holdlines are not specified correctly.
* <br><br>
* <b>Limitations:</b> 
* <ul>
* <li>The holdline should be defined in the same coordinate
* system as the edge being blended. In other words, the body containing the holdline
* should have the same tranformation (if any) as the body containing the edge to be
* blended.</li>
* <li>Only one holdline can be specified per call of the API.</li>
* <li>The help position should be specified in global coordinates.</li>
* </ul>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param face1
* first support face.
* @param face2
* second support face.
* @param hl_list
* entities defining the holdline.
* @param find_smooth_hl
* flag specifying whether to use the edges smoothly connected to the specified holdline edges.
* @param help_pos
* position in global coordinates specifying a rough location of a point on the blend spine.
* @param convexity
* convexity of the blend.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_blend_holdline_face_face(
	FACE *face1,
	FACE *face2,
	ENTITY_LIST const &hl_list,			// EDGEs or CURVEs defining holdline
	logical find_smooth_hl = TRUE,		// Whether to find edges smoothly connected to holdline(s)
	SPAposition const &help_pos = *(SPAposition*) NULL_REF,
	bl_convexity convexity = bl_convexity_unknown,
	AcisOptions *ao = NULL 				// options, journal, version
    );

// For future use, If there is more than one face in any list, these faces 
// must be part of the same network of smoothly connected faces. 

/**
* Sets a three entity blending attribute.
* <b>Technical Articles:</b> <i>[Advanced Blending](http://doc.spatial.com/articles/a/d/v/Component~Advanced_Blending_90cb.html)</i>, 
* <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
* <br><br>
* <b>Role:</b> This API sets the attribute for a variable radius three face 
* blend. Use @href api_fix_blends to create the explicit blends. The blend surface 
* will be tangent to all three support faces. The cross section of the blend surface 
* is circular and the variable radius of the blend is driven by the requirements 
* of tangency to the surfaces. The blend begins on three of the specified support 
* faces and rolls onto the other faces in the same manner as a general 
* entity-entity blend. (Refer to the technical article <i>Entity-Entity Blends</i> 
* in the <i>Advanced Blending Component</i> of the Online Help.) The help 
* position and convexity are used to resolve ambiguous cases where more than one 
* solution would be possible. 
* <br><br>
* <b>Limitations:</b> <br>
* <ul>
* <li>All entities in the three support lists must be faces.</li> 
* <li>All faces must belong to the same body.</li>
* <li>The same face cannot be added to more than one support list.</li>
* <li><tt>find_smooth_bl</tt> is assumed to be true, and blend will propagate as far as possible.</li>
* <li>This API has the capability of generating singular spline blends,
* except when two or more support faces are tangential to each other at a common point.</li>
* <li>If the support surfaces have non-G1 discontinuity, then blending will fail.</li>
* <li>Three entity blend transition onto an edge of a support face is not supported.</li>
* </ul>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param left_face_list
* list containing left support faces.
* @param right_face_list
* list containing right support faces.
* @param center_face_list
* list containing center support faces.
* @param find_smooth_bl
* if true, finds all possible smoothly connected three face blends. Currently ignored 
* and assumed to be true.
* @param help_pos
* position in global coordinates specifying a rough location of a point on the blend spine. 
* If unspecified, computed by the algorithm.
* @param convexity
* convexity of the blend, bl_convex or bl_concave. If unspecified, the 
* convexity is computed by the algorithm.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ABL outcome api_blend_three_ent(
	ENTITY_LIST const &left_face_list,
	ENTITY_LIST const &right_face_list,
	ENTITY_LIST const &center_face_list,
	logical find_smooth_bl = TRUE,
	SPAposition const &help_pos = *(SPAposition const *)NULL_REF,
	bl_convexity convexity = bl_convexity_unknown,
	AcisOptions *ao = NULL	// options, journal, version
	);


/**
* Sets a curvature continuous blending attribute.
* <b>Technical Articles:</b> <i>[Advanced Blending](http://doc.spatial.com/articles/a/d/v/Component~Advanced_Blending_90cb.html)</i>, 
* <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
* <br><br>
* <b>Role:</b> This API sets the attribute for curvature continuous blends. 
* Use @href api_fix_blends to create the explicit blends. The blend surface 
* will be curvature-continuous with both faces. The blend starts
* from two of the support faces specified and rolls onto other
* faces, provided that each is in the appropriate list, in the same
* manner as general entity-entity blending (refer to the documentation for
* entity-entity blending, @href api_set_ee_cr_blend). The help position and
* convexity are used to resolve ambiguous cases where more than one
* solution would be possible. 
* <br><br>
* <b>Limitations:</b> 
* <ul>
* <li>Each list must contain at least one face, and all entities 
* in the lists must be faces.</li>
* <li>All faces must belong to the same body; a face cannot 
* be added to more than one face list. </li>
* <li>At this time,<br>
* (a) If the support surfaces have non-G2 discontinuities, then
* blending will fail. <br>
* Exception:  If the non-G2 discontinuity is only non-G2 in the direction parallel to 
* the spring (that is, there is not a G2 discontinuity in the direction perpendicular 
* to the spring), then the blend surface will still be G2 continuous with its supports, 
* but only G1 with itself as it crosses the discontinuity.
* <br><br>
* (b) The fillet will not propagate across a G1, but non-G2 boundary. <br>
* Exception:  As above, if the non-G2 discontinuity is a G2 
* only non-G2 in the direction parallel to the spring (that is, there is not 
* discontinuity in the direction perpendicular to the spring), then the blend surface will 
* propagate, but it will only be G1 with itself as it crosses the boundary.</li>
* </ul>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param left_face_list
* list containing left support faces.
* @param right_face_list
* list containing right support faces.
* @param radius
* computes the spring curves for the fillet with this value, using the algorithm for 
* constant radius blends.
* @param help_pos
* position in global coordinates specifying a rough location of a point on the blend spine. 
* If unspecified, the value is computed by the algorithm.
* @param convexity
* convexity of the blend: <tt>bl_convex</tt> or <tt>bl_concave</tt>. If unspecified, 
* the value is computed by the algorithm.
* @param ao
* ACIS options such as versioning and journaling.

**/
DECL_ABL outcome api_set_cc_blend(ENTITY_LIST const &left_face_list,
                                  ENTITY_LIST const &right_face_list,
                                  double             radius,
                                  SPAposition const &help_pos  = *((SPAposition const *)NULL_REF),
                                  bl_convexity       convexity = bl_convexity_unknown,
                                  AcisOptions       *ao        = NULL
                                  );
/** @} */
#endif
