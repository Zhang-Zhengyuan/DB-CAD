/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining the blend attribute for constant radius rounds.
#if !defined( BLEND_CONST_ROUND )
#define BLEND_CONST_ROUND
/**
 * @file ro_const.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
#include "acis.hxx"
#include "dcl_blnd.h"
#include "bl_att.hxx"
#include "at_sys.hxx"
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_CO_RO_FBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_CONST_ROUND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Define a circular "rolling-ball" blend of constant radius.

extern DECL_BLND int ATTRIB_CONST_ROUND_TYPE;
#define ATTRIB_CONST_ROUND_LEVEL (ATTRIB_FFBLEND_LEVEL + 1)

/**
 * Defines a circular rolling-ball blend of constant radius.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_CONST_ROUND</tt> is the blend attribute for constant radius rounds. This
 * attribute records a circular, rolling ball blend of constant radius.
 * <br><br>
 * Finding the geometry of rounds proceeds as follows:
 * <br><br>
 * If the surfaces of the faces of the edge being blended each have simple offsets (i.e., the
 * surfaces being offset are not spline surfaces or elliptical cones), and the spine curve given
 * by their intersection is an ellipse or straight line, the spine is projected to each original
 * surface to get the spring curves.
 * <br><br>
 * The spine and spring curves together with the blend radius determine the blend surface;
 * however, if the offset surfaces are not simple or the spine is an intcurve, rolling ball
 * surfaces are found by positioning the ball at a small number of points along the blend and
 * then interpolating a pipe surface between those points.
 * <br><br>
 * This logic has the advantage that in many cases where the spine of a rolling ball surface is
 * simple, the computation of the pipe surface is more robust especially where the radius of the
 * rolling ball is close to the local radii of curvature on the contracted surfaces.
 */
class DECL_BLND ATTRIB_CONST_ROUND: public ATTRIB_FFBLEND {
private:

	double	radius_data;	// blend radius is always positive

protected:

	// Account for transforms present on the body within blending.

	virtual void trans_data( SPAtransf const & );

public:

	// Returns TRUE if constant offset blend
	// This function returns positive left_offset and right_offset 
	// values irrespective of blend convexity.
/**
 * @nodoc
 */
	virtual logical is_constant_offset( 
						double &left_offset 
							= *( double *)NULL_REF, 
						double &right_offset 
							= *( double *)NULL_REF 
						) const;

	// Returns TRUE if chamfer is flat
/**
 * @nodoc
 */
	virtual logical is_flat_chamfer() const;
	

	// Simple read function for the data members.

/**
 * Returns the constant radius of the round.
 */
	double radius() const { return radius_data; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

/**
 * Sets the constant radius of the round.
 * <br><br>
 * @param rad
 * radius.
 */
	void set_radius( double rad);

	// Constructors.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, x=<tt>new ATTRIB_CONST_ROUND(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param ent_name
 * owner entity name.
 * @param rad
 * radius.
 * @param set_start
 * start setback.
 * @param set_end
 * end setback.
 * @param left_face
 * left face.
 * @param right_face
 * right face.
 * @param start_diff
 * start setback difference.
 * @param end_diff
 * end setback difference.
 * @param set_setstart
 * set start setback.
 * @param set_setend
 * set end setback.
 * @param mid_blend
 * set mid-blend behavior.
 * @param start_angle
 * stop angle start.
 * @param stop_angle
 * stop angle end.
 */
	ATTRIB_CONST_ROUND(
				ENTITY *ent_name = NULL,				// owner
				double rad = 0,							// radius
				double set_start = 0,					// setback at start
				double set_end = 0,						// setback at end
				FACE *left_face = NULL,					// left face
				FACE *right_face = NULL,				// right face
				double start_diff = 0,					// setback diff at start
				double end_diff = 0,					// setback diff at end
				logical set_setstart = TRUE,			// setback diff set at start
				logical set_setend = TRUE,				// setback diff set at end
				blend_how mid_blend = bl_how_default,	// set mid-blend behaviour
			    double start_angle = 0.0,				// start stop angle
			    double stop_angle = 0.0					// end stop angle
				);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, x=<tt>new ATTRIB_CONST_ROUND(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param attr
 * existing attribute.
 */
	ATTRIB_CONST_ROUND( ATTRIB_CONST_ROUND const *attr );

/**
 * @nodoc
 */
	virtual ATTRIB_BLEND* copy() const;

	// Reverse left-to-right properties of an asymmetric blend.
	// (Nothing to do for rounds.)

//	virtual void reverse();

	// split_owner is handled by the parent class.
//	virtual void split_owner( ENTITY * );
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.

	// merge_owner is handled by the parent class.
//	virtual void merge_owner( ENTITY *, logical );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// Test two blends for equality.

/**
 * @nodoc
 */
	virtual logical operator==( ATTRIB_BLEND const& blend ) const;

/**
 * @nodoc
 */
	virtual void trans_owner( SPAtransf const & );

	// Account for transforms present on the body within blending.
/**
 * @nodoc
 */
	virtual void trans_blend( logical, logical );

	// Return a measure of continuity at blend spring curves
	// i.e. unset_continuity, position_continuous, slope_continuous,
	// or curvature_continuous.

/**
 * @nodoc
 */
	virtual bl_continuity continuity( int sup_index ) const;

	// Test if blend is of zero width i.e. no faces need be made
	// but usually a setback will have been recorded at one or both
	// ends of the edge.

/**
 * @nodoc
 */
	virtual logical is_zero() const;

	// Return a measure of the size of the blend.

/**
 * @nodoc
 */
	virtual double blend_size( logical start) const;

	// Return whether the left face means we need to compute some
	// reordered blends.  
	// NOTE: Derived versions of these should invoke the parent's
	// corresponding method first, to check for "impossible".

/**
 * @nodoc
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        left_reorder(logical &convex = *(logical*)NULL_REF);

/**
 * @nodoc
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        right_reorder(logical &convex = *(logical*)NULL_REF);

	// Inquiry functions concerning size and shape.

/**
 * @nodoc
 */
	virtual logical zero_end_radius(
								logical start,	// at_start
								double tol = SPAresabs 
							  ) const
	{ 
		SPAUNUSED( start )
		return radius_data < tol; 
	}

	// Find the spine curve or point.
/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_spine(
									logical start,				// open at start
									logical end,				// open at end
									segend *start_seg,			// start segend if any
									segend *end_seg,			// end segend if any
									SPAbox const &bound_box = 
										* (SPAbox const *) NULL_REF
									);

	// Find the spring curves.  The curves returned have the same
	// sense as the spine.

/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_spring_curves(
										ffblend_geom *spine,
										logical start,				// open at start
										logical end,				// open at end
										segend *start_seg,			// start segend if any
										segend *end_seg,			// end segend if any
										SPAbox const &bound_box = 
											* (SPAbox const *) NULL_REF
										);

	// Find the cross curve in a given plane (normal to the spine and
	// spring curves).  The curve found runs from left to right across
	// the blend.

/**
 * @nodoc
 */
	virtual curve *find_ffblend_cross_curve(
										ffblend_geom *spine,
										plane const &pln,
										SPAbox const &bound_box,
										SPAposition &pt1 = *(SPAposition *)NULL_REF,
										SPAposition &pt2 = *(SPAposition *)NULL_REF,
										SPAposition &pt3 = *(SPAposition *)NULL_REF
										);

	// Given blend boundary details in a ffblend_geom, find the
	// blend surface and return it and the given boundary details
	// in a ffblend_geom.

/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_surface( ffblend_geom *spine );

	// Find blend geometry by marching or by offsetting, intersecting
	// and projecting.

/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_geometry(
									logical start = FALSE,			// open at start
									logical end = FALSE,			// open at end
									segend *start_seg = NULL,		// start segend if any
									segend *end_seg = NULL,			// end segend if any
									SPAbox const &bound_box = 
										* (SPAbox const *) NULL_REF,
									double & reduced_resabs = 
										* (double*) NULL_REF		// To pass back (only 
																	// set for analytic blends)
									);

	// Find blend geometry by marching.

/**
 * @nodoc
 */
	ffblend_geom *march_ffround_geometry(
									logical start,
									logical end,
									segend *start_seg,
									segend *end_seg,
									ffblend_geom *spine = NULL	// spine if available
									);

	// enquire
	/**
	 * @nodoc
	 */
	virtual blend_debug_info* enquire(FILE*fp);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_CONST_ROUND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/**
 * @nodoc
 */
DECL_BLND void 
set_const_round( ENTITY *edge, double radius );

/**
 * @nodoc
 */
DECL_BLND void 
set_const_rounds(
			ENTITY_LIST const &edges,	
			double radius,				
			double start_setback,			
			double end_setback,				
			double setback_diff_at_start = 0.,		
			double setback_diff_at_end = 0.,		
			logical setback_diff_set_at_start = TRUE,
			logical setback_diff_set_at_end = TRUE,
			double start_stop_angle = 0.,		
			double end_stop_angle = 0.,			
			blend_how bl_how = bl_how_default	
			);

// Make a constant round attribute and set it on an existing face that is
// to be regarded as a blend face.  Every argument but the first may be
// defaulted in which case this routine attempts to deduce the blend
// properties from the given face and its geometry.  If the deduction cannot
// be made, returns false and does not set an attribute on the face.
// Otherwise removes any existing explicit blend attribute on the given face
// and its related support attributes, and then sets a new explicit blend
// attribute on the given face, sets support attributes on the support faces,
// given or deduced, and returns true.

/**
 * @nodoc
 */
DECL_BLND logical 
set_exp_const_round(
			FACE *bl_face,
			FACE *left_face = NULL,	
			FACE *right_face = NULL,
			double const &radius = 
				* (double *) NULL_REF,
			logical const &cvxty = 
				* (logical *) NULL_REF,
			plane const &mid_plane = 
				* (plane *) NULL_REF,
			logical const &start_cond =		// start and end conditions
				* (logical *) NULL_REF,		// (true means open)
			logical const &end_cond = 
				* (logical *) NULL_REF	
			);

// Attach an explicit constant round face-face blend attribute to the face
// of the given coedges (and any other face split from it) and place support
// attributes on the left and right support faces.  If not given, the blend
// radius is estimated.  A def-plane and convexity are found for each
// attribute made.  If blend radius is not given, it is estimated from the
// blend face curvature.

/**
 * @nodoc
 */
DECL_BLND logical 
set_exp_co_ro_ffbl_att(
			ENTITY_LIST const &start_cross_coedges,		
			ENTITY_LIST const &end_cross_coedges,		
			ENTITY_LIST const &left_spring_coedges,		
			ENTITY_LIST const &right_spring_coedges,	
			double const &bl_radius = *(double *)NULL_REF,
			double const &len_tol = *(double *)NULL_REF,
			ENTITY_LIST &bl_atts = *(ENTITY_LIST *)NULL_REF // blend attribute(s) made
			);

// Make a constant round fblend attribute and set it on an existing face that
// is to be regarded as a fblend face.  Every argument but the first may be
// defaulted in which case this routine attempts to deduce the blend
// properties from the given face and its geometry.  If the deduction cannot
// be made, returns false and does not set an attribute on the face.
// Otherwise removes any existing explicit blend attribute on the given face
// and its related support attributes, and then sets a new explicit blend
// attribute on the given face, sets support attributes on the support faces,
// given or deduced, and returns true.

/**
 * @nodoc
 */
DECL_BLND logical 
set_exp_co_ro_fbl(
			FACE *bl_face,		
			FACE *support_face = NULL,
			logical = *(logical *)NULL_REF,			// support_on_left
			double const &radius = *(double *)NULL_REF,	
			logical const &cvxty = *(logical *)NULL_REF,
			plane const &mid_plane = *(plane *)NULL_REF,
			logical const &start_cond = *(logical *)NULL_REF,	// start and end conditions
			logical const &end_cond = *(logical *)NULL_REF		// (true means open)
			);

// Attach an explicit constant round face blend attribute to the face
// of the given coedges (and any other face split from it) and place support
// attributes on the left or right support faces.  If not given, the blend
// radius is estimated.  A def-plane and convexity are found for each
// attribute made.  If blend radius is not given, it is estimated from the
// blend face curvature.

/**
 * @nodoc
 */
DECL_BLND logical 
set_exp_co_ro_fbl_att(
				ENTITY_LIST const &start_cross_coedges,	
				ENTITY_LIST const &end_cross_coedges,	
				ENTITY_LIST const &spring_coedges,		
				logical const &support_on_left,			
				double const &bl_radius = *(double *)NULL_REF,	
				double const &len_tol = *(double *)NULL_REF,	
				ENTITY_LIST &bl_att = *(ENTITY_LIST *)NULL_REF // blend attribute(s) made
				);

/*
// Commented out icb 12.v.99  (now done in healing)

// Test a face to see if it could be a constant round face-face blend.
// If so, leave a const_round blend attribute on the face and support
// attributes on its left and right support faces.
// length_tol is used e.g. to check if an edge is nearly circular.
// short_ed_tol is used to test if an edge is to be regarded as short i.e.
// is expected to shrink to nothing during healing.

DECL_BLND logical 
test_const_round_face(
				FACE *,
				double,		// length_tol
				double,		// size
				double		// short_ed_tol
				);
*/

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_CO_RO_FBLEND_TYPE;
#define ATTRIB_CO_RO_FBLEND_LEVEL (ATTRIB_FBLEND_LEVEL + 1)

// Class is only used by healing during reblending.
/**
 * @nodoc
 */
class DECL_BLND ATTRIB_CO_RO_FBLEND: public ATTRIB_FBLEND {
private:

	double	radius_data;	// blend radius is always positive

public:

	// Simple read function for the data members.

	double radius() const { return radius_data; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_radius( double );

	// Constructors.

	ATTRIB_CO_RO_FBLEND(
				ENTITY * = NULL,			// owner, for now a face
				FACE * = NULL,				// support face
				logical = TRUE,				// true if support is on the left
				bl_ed_convexity = bl_ed_undefined_cvxty,
				plane const & = *(plane const *)NULL_REF,	// def-plane
				double = 0,					// radius of round
				blend_status = bl_stat_unset
			);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).

	ATTRIB_CO_RO_FBLEND( ATTRIB_CO_RO_FBLEND const *);

	virtual ATTRIB_BLEND* copy() const;

	// Reverse left-to-right properties of an asymmetric blend.
	// (Nothing to do for rounds.)

//	virtual void reverse();

	// split_owner is handled by the parent class.
//	virtual void split_owner( ENTITY * );
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.

	// merge_owner is handled by the parent class.
//	virtual void merge_owner( ENTITY *, logical );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// Test two blends for equality.
	virtual logical operator==( ATTRIB_BLEND const& ) const;

	virtual void trans_owner( SPAtransf const & );

	// Return a measure of continuity at blend spring curves
	// i.e. unset_continuity, position_continuous, slope_continuous,
	// or curvature_continuous.

	virtual bl_continuity continuity( int sup_index ) const;

	// Return a measure of the size of the blend.

	virtual double blend_size( logical ) const;

	// Find the spine curve or point.

	virtual ffblend_geom *find_ffblend_spine(
		logical,			// open at start
		logical,			// open at end
		segend *,			// start segend if any
		segend *,			// end segend if any
		SPAbox const & = *(SPAbox const *)NULL_REF
	  );

	// Given blend boundary details in a ffblend_geom, find the
	// blend surface and return it and the given boundary details
	// in a ffblend_geom.

	virtual ffblend_geom *find_ffblend_surface(ffblend_geom *);

	// Find blend geometry by offsetting given spring coedge to get spine
	// and hence blend surface.

	virtual ffblend_geom *find_ffblend_geometry(
		logical = FALSE,			// open at start
		logical = FALSE,			// open at end
		segend * = NULL,			// start segend if any
		segend * = NULL,			// end segend if any
		SPAbox const & = *(SPAbox const *)NULL_REF
	  );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_CO_RO_FBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
/** @} */
#endif
