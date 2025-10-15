/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining the blend attribute for constant range
// chamfers.
#if !defined( BLEND_CONST_CHAMFER )
#define BLEND_CONST_CHAMFER
/**
 * @file ch_const.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
#include "acis.hxx"
#include "dcl_blnd.h"
#include "bl_att.hxx"
#include "api.hxx"
#include "usecount.hxx"
#include "cvc.hxx"
#include "bl_cxty.hxx"

class ATT_BL_ENT_ENT;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_CONST_CHAMFER, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Define a flat chamfer blend.  An edge chamfer is a ruled surface
// between two "spring curves", one on each lateral surface.  A
// spring curve is found by projecting the "spine curve" to each
// lateral face.  The spine curve is the intersection of the two
// lateral surfaces offset by constant distances chosen to give
// the desired ranges (which may be different for the two surfaces).
// This interpretation of range has the advantage that the chamfer
// shrinks as the angle between the faces aproaches pi.  "left" and
// "right" are relative to the edge to which the (implicit) blend
// is attached.

extern DECL_BLND int ATTRIB_CONST_CHAMFER_TYPE;
#define ATTRIB_CONST_CHAMFER_LEVEL (ATTRIB_FFBLEND_LEVEL + 1)

/*
// tbrv
*/
/**
 * Defines a flat chamfer blend.
 * <br>
 * <b>Role:</b> This class defines a flat chamfer blend. An edge chamfer is a ruled surface
 * between two spring curves, one on each lateral surface. A spring curve is found by projecting
 * the spine curve to each lateral face. The spine curve is the intersection of the two lateral
 * surfaces offset by constant distances chosen to give the desired ranges (which may be
 * different for the two surfaces).
 * <br><br>
 * This interpretation of range has the advantage that the chamfer shrinks as the angle between
 * the faces approaches pi. The left and right are relative to the edge to which the (implicit)
 * blend is attached.
 * <br><br>
 * <tt>ATTRIB_CONST_CHAMFER</tt> records left and right ranges, i.e. the approximate extent of the chamfer
 * to either side of the blended edge. The ranges are exact for lateral surfaces that cut a plane
 * normal to the chamfered edge in straight lines.
 */

class DECL_BLND ATTRIB_CONST_CHAMFER : public ATTRIB_FFBLEND 
{
public:

	// Main constructor. Only one range value may be given, in
	// which case it is used for both left and right.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_CONST_CHAMFER(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param ent_owner
 * entity owner.
 * @param left_range
 * left range.
 * @param right_range
 * right range.
 * @param set_start
 * setback at start.
 * @param set_end
 * setback at end.
 * @param range1
 * range.
 * @param range2
 * range.
 * @param set_diff_start
 * setback diff set at start.
 * @param set_diff_end
 * setback diff set at end.
 * @param angle_start
 * stop angle start.
 * @param angle_end
 * stop angle end.
 */
	ATTRIB_CONST_CHAMFER(
				ENTITY *ent_owner = NULL,   	// owner
				double left_range = 0,			// left range
				double right_range = 0,			// right range
				double set_start = 0,			// setback at start
				double set_end = 0,				// setback at end
				double range1 = 0,				// setback diff at start
				double range2 = 0,				// setback diff at end
				logical set_diff_start = TRUE,	// setback diff set at start
				logical set_diff_end = TRUE,	// setback diff set at end
				double angle_start = 0,			// start stop angle
				double angle_end = 0			// end stop angle
				);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_CONST_CHAMFER(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param attr
 * existing attribute
 */
	ATTRIB_CONST_CHAMFER( ATTRIB_CONST_CHAMFER const *attr );

/**
 * Copies a blend, setting the implicit blend data only and without attaching the copy to an entity.
 */
	virtual ATTRIB_BLEND *copy() const;

	// Simple read functions for the data members.
/**
 * Returns the value for the left range.
 */
	double left_range() const { return left_range_data; }
/**
 * Returns the value for the right range.
 */
	double right_range() const { return right_range_data; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

/**
 * Sets the value for the left range.
 * <br><br>
 * @param ran_val
 * range value.
 */
	void set_left_range( double left_val );
/**
 * Sets the value for the right range.
 * <br><br>
 * @param ran_val
 * range value.
 */
	void set_right_range( double right_val );

	// Whether the chamfer ranges are to be interprted along the
	// support face ( fixed distance from the blended edge ) or 
	// along the support normals.

	logical is_fixed_distance() const { return fixed_distance; } 

/**
 * @nodoc
 */

	// Sets the value of the left and right support faces.
	// It also swaps the right and left blend data if 'reversed' 
	// flag is set to TRUE.

   virtual void set_support_faces( 
							FACE *nleft_face,   // left support face
							FACE *nright_face,  // right support face
							logical reversed    // flag to indicate whether the blend data 
												// needs to be swapped between left and right sides
							);

	// Returns if we can do this chamfer
/**
 * Returns TRUE if this chamfer is supported.
 */
	logical is_supported_chamfer() const;

	// Reverse left-to-right properties.
/**
 * Reverses the left-to-right properties.
 */
	virtual void reverse();

	// Functions called to aid attribute migration during modelling
	// operations.

/**
 * Notifies the <tt>ATTRIB_CONST_CHAMFER</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
 * The default action is to do nothing. This function is supplied by the application whenever it
 * defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param ent
 * new entity.
 */
	virtual void split_owner( ENTITY *ent );
									
	// merge_owner is handled by the parent class.

	// virtual void merge_owner( ENTITY *, logical );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// Test two blends for equality.
/**
 * Tests two blends for equality.
 * <br><br>
 * @param blnd
 * blend.
 */
	virtual logical operator==( ATTRIB_BLEND const& blnd ) const;

/**
 * Notifies the <tt>ATTRIB_CONST_CHAMFER</tt> that its owner is about to be transformed.
 * <br><br>
 * <b>Role:</b> The application has the chance to transform the attribute. The default action is
 * to do nothing. This function is supplied by the application whenever it defines a new
 * attribute, and is called when a transformation occurs.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void trans_owner( SPAtransf const &trans );

	// Account for transforms present on the body within blending.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void trans_blend( logical, logical );

	// Return a measure of continuity at blend spring curves
	// i.e. unset_continuity, position_continuous, slope_continuous,
	// or curvature_continuous.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual bl_continuity continuity( int sup_index ) const;

	// Return a measure of the size of the blend (supposedly a measure
	// of the maximum offset of the spine from the underlying surfaces).

/**
 * Returns the size of the blend at the start or end.
 * <br><br>
 * <b>Role:</b> The logical argument is irrelevant with a constant radius blend.
 * <br><br>
 * @param at_start
 * at start?.
 */
	virtual double blend_size( logical at_start = TRUE ) const;

	// Return a measure of the edge - spring_curve distance.

/**
 * Returns a measure of a typical distance between the spring curves.|
 * <br><br>
 * @param edge
 * coedge.
 */
	virtual double lateral_size( COEDGE *edge ) const;

	// Inquiry functions concerning size and shape.

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

/**
 * Returns TRUE if the chamfer is circular.
 * <br><br>
 * <b>Role:</b> Returns the expected defaults so only unusually derived classes need to worry
 * about returning something different.
 */
	virtual logical is_circular() const { return FALSE; }
/**
 * Returns TRUE if the chamfer end radius is zero (less than <tt>resabs</tt>) at the start point if <tt>at_start</tt> is TRUE, or at the end point if <tt>at_start</tt> is FALSE.
 * <br><br>
 * <b>Role:</b> Returns the expected defaults so only unusually derived classes need to worry
 * about returning something different.
 * <br><br>
 * @param at_start
 * at start.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius(
							logical at_start,	
							double tol = SPAresabs 
						  ) const
	{ 
		SPAUNUSED( at_start )
		return left_range_data < tol && right_range_data < tol; 
	}

	// Find the spine curve or point.

/**
 * Finds the spline curve or point(A <tt>segend</tt> holds information about the blend sheet segment).
 * <br><br>
 * @param at_start
 * open at start?.
 * @param at_end
 * open at end?.
 * @param start_seg
 * start segend.
 * @param end_seg
 * end segment.
 * @param bound_box
 * bounding box.
 */
	virtual ffblend_geom *find_ffblend_spine(
									logical at_start,			// open at start
									logical at_end,				// open at end
									segend *start_seg,			// start segend if any
									segend *end_seg,			// end segend if any
									SPAbox const &bound_box = 
										* (SPAbox const *) NULL_REF
									);

	// Find the spring curves.  They have the same sense as the spine.

/**
 * Finds the spring curves, which have the same sense as the spine (A <tt>segend</tt> holds information about the blend sheet segment).
 * <br><br>
 * @param spine
 * contains spine.
 * @param at_start
 * open at start?.
 * @param at_end
 * open at end?.
 * @param start_seg
 * start segend.
 * @param end_seg
 * end segend.
 * @param bound_box
 * bounding box.
 */
	virtual ffblend_geom *find_ffblend_spring_curves(
									ffblend_geom *spine,		// contains spine
									logical at_start,			// open at start
									logical at_end,				// open at end
									segend *start_seg,			// start segend if any
									segend *end_seg,			// end segend if any
									SPAbox const &bound_box = 
										* (SPAbox const *) NULL_REF
									);

	// Find the cross curve in given plane (normal to spine and spring
	// curves).  The curve found runs from left to right across the
	// blend.

/**
 * Finds the cross curve in given plane (normal to spine and spring curves).
 * <br><br>
 * <b>Role:</b> The curve found runs from left-to-right across the blend.
 * <br><br>
 * @param spine
 * contains spine.
 * @param pla
 * plane.
 * @param bound_box
 * bounding box.
 * @param pt1
 * first point on plane.
 * @param pt2
 * second point on plane.
 * @param pt3
 * third point on plane.
 */
	virtual curve *find_ffblend_cross_curve(
									ffblend_geom *spine,
									plane const &pla,
									SPAbox const &bound_box,
									SPAposition &pt1 = 
										*(SPAposition *)NULL_REF,
									SPAposition &pt2 = 
										*(SPAposition *)NULL_REF,
									SPAposition &pt3 = 
										*(SPAposition *)NULL_REF
									);

	// Given blend boundary details in a ffblend_geom, find the
	// blend surface and return it and the given boundary details
	// in a ffblend_geom.

/**
 * Finds the blend surface and returns it and the given boundary details in a <tt>ffblend_geom</tt>, given the blend boundary details in a <tt>ffblend_geom</tt>.
 * <br><br>
 * @param spine
 * contains spine.
 */
	virtual ffblend_geom *find_ffblend_surface( ffblend_geom *spine );


    // Find blend geometry only for special case(s).
    // If not a special case, call ATTRIB_FFBLEND::find_ffblend_geometry
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual ffblend_geom *find_ffblend_geometry(
										logical open_at_start = FALSE,
										logical open_at_end = FALSE,            
										segend *start_segend = NULL,            
										segend *end_segend = NULL,
										SPAbox const &r = 
											* (SPAbox const *) NULL_REF,
										double &modified_resabs = 
											* (double*) NULL_REF
										);

	// Remove any cached geometry data. 
/**
 * @nodoc
 */
	virtual void remove_ffblend_geometry();

	// Return if an entity-entity blend can be done for this type of
	// blend.

/**
 * @nodoc
 */
	CURVE const *def_curve() const { return _def_curve; }
	// FDC QMV TEMP CODE
	logical def_curve_alignment()const { return _def_cur_align; }
/**
 * @nodoc
 */
	CURVE *def_curve_editable() { return _def_curve; }

/**
 * @nodoc
 */
	void set_defining_CURVE( CURVE *calib_curve );

/**
 * @nodoc
 */
	SPAinterval const &base_range() const { return _base_range; }

/**
 * Returns TRUE if an entity-entity blend can be done for this type of blend.
 */
    virtual logical ent_ent_blend_implemented() const { return FALSE; }

	// Returns an implicit entity-entity blend attribute for child
	// propgation, i.e. face rollover. The attribute has a NULL owning
	// entity in it.
	/**
	 * @nodoc
	 */

	ATT_BL_ENT_ENT* make_ent_ent_att() const;

	// enquire
	/**
	 * @nodoc
	 */
	virtual blend_debug_info* enquire(FILE*fp);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_CONST_CHAMFER, BLND )

#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
	/**
	 * @nodoc
	 */
	USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

protected:

	// Account for transforms present on the body within blending.

	virtual void trans_data( SPAtransf const &trf );

	// Determine the actual supports of the chamfer. In case of fixed distance
	// chamfers, also finds the actual pair of supports.

    logical find_chamfer_offsets(
							segend const *start_segend,
							segend const *end_segend,
							SPAbox const &region,
							double &left_offset,
							double &right_offset,
							logical find_supports,
							FACE *&left_face,
							FACE *&right_face
						    ) const;

	// Save the chamfer offset data.

	void set_chamfer_offsets( 
						logical set_offsets,
						double left_offset = -1.,
						double right_offset = -1.
						);

	// Sets the ffblend attribute on given edge

	virtual void set_ffblend(EDGE *, set_ffblend_input &);

private:

	// Whether the ranges should be interpreted as fixed distance from
	// the blended edge.

	logical fixed_distance;

	double	left_range_data;
	double	right_range_data;
	double	max_offset;

	// The actual support offsets.

    double  left_offset_data;
    double  right_offset_data;
    logical chamfer_offset_set;

	// Calibration curve

	CURVE *_def_curve;

	// Indicates whether the def_curve is aligned with the owning edge. 
	// TRUE means along and FALSE means against.
	// Strictly for internal debugging purpose only.

	logical _def_cur_align;

	// Parametric range of this attrib within the calibration
	// curve.

	SPAinterval _base_range;
};

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_BLND void 
set_const_chamfer( 
		ENTITY *blnd_edge, 
		double left_range, 
		double right_range 
		);

// Very handy little function to find the (first) ATTRIB_CONST_CHAMFER 
// on any given entity.

/**
 * @nodoc
 */
ATTRIB_CONST_CHAMFER *find_const_chamfer_attrib( ENTITY *ent );

// Make a chamfer attribute and set it on all entities i.e. edges in
// the list.  Set a copy of it on the first edge in the list and then
// on the other edges having regard to their connectivity (edges
// connected head to tail or tail to head have left and right the same,
// those connected head to head or tail to tail have left and right
// reversed).  Lastly delete the attribute first made.
// Setback at an end of an edge determines where the blend is to
// be stopped short of the vertex at the edge end.  It is only
// significant when the vertex is blended.

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_BLND void 
set_const_chamfers(
		ENTITY_LIST const &edges,	
		double left_range,			
		double right_range,			
		double start_setback,		
		double end_setback,			
		double setback_diff_at_start = 0.,	
		double setback_diff_at_end = 0.,	
		logical setback_diff_set_at_start = TRUE,
		logical setback_diff_set_at_end = TRUE,	
		double start_stop_angle = 0.,			
		double end_stop_angle = 0.			
		);

// Set fixed distance chamfers. The chamfer ranges are interpreted
// as spring distance values from the calibration curve.

/**
 * @nodoc
 */

DECL_BLND outcome 
set_fixed_distance_chamfers(
		ENTITY_LIST const &edges,	
		double left_range,			
		double right_range,			
		double start_setback,		
		double end_setback,			
		double setback_diff_at_start = 0.,	
		double setback_diff_at_end = 0.,	
		logical setback_diff_set_at_start = TRUE,
		logical setback_diff_set_at_end = TRUE,	
		double start_stop_angle = 0.,			
		double end_stop_angle = 0.			
		);


// Make a constant chamfer attribute and set it on an existing face that is
// to be regarded as a blend face.  Every argument but the first may be
// defaulted in which case this routine attempts to deduce the blend
// properties from the given face and its geometry.  If the deduction cannot
// be made, returns false and does not set an attribute on the face.
// Otherwise removes any existing explicit blend attribute on the given face
// and its related support attributes, and then sets a new explicit blend
// attribute on the given face, sets support attributes on the support faces,
// given or deduced, and returns true.

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_BLND logical 
set_exp_const_chamfer(
				FACE *bl_face,
				FACE *left_face = NULL,
				FACE *right_face = NULL,
				double const &left_range = 
					* (double *) NULL_REF,	
				double const &right_range = 
					* (double *) NULL_REF,	
				logical const &blend_cvxty = 
					* (logical *)NULL_REF,	
				plane const &mid_plane = 
					* (plane *) NULL_REF		
				);

/** @} */
#endif
