/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining the blend attribute for variable radius blends.
#if !defined( BLEND_VARIABLE )
#define BLEND_VARIABLE
#include <stdio.h>
#include "acis.hxx"
#include "dcl_blnd.h"
#include "logical.h"
#include "bs2curve.hxx"
#include "bl_att.hxx"
#include "ss_b_spl.hxx"
#include "edge.hxx"
#include "usecount.hxx"
/**
 * @file v_bl_att.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

// Forward class declarations.

class curve;
class ffblend_geom;
class ENTITY_LIST;
class ENTITY;
class FACE;
class CURVE;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_VAR_BLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// tbrv
/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_VAR_BLEND_TYPE;
#define ATTRIB_VAR_BLEND_LEVEL (ATTRIB_FFBLEND_LEVEL + 1)

/**
 * Defines the blend attribute for variable radius blends.
 * <br>
 * @see SPAinterval, CURVE, EDGE
 */
class DECL_BLND ATTRIB_VAR_BLEND: public ATTRIB_FFBLEND {
public:

	// Constructors.

// If the defining curve is NULL, the geometry of the owning edge is used.
// If the SPAparameter SPAinterval is NULL, the range of the owning edge is used.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_VAR_BLEND(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param entity
 * owning edge.
 * @param curve1
 * defining curve,geometry of edge if null.
 * @param prange
 * param. range, entire defining curve if NULL.
 * @param sbk_start
 * start setback.
 * @param sbk_end
 * end setback.
 * @param lface
 * left face.
 * @param rface
 * right face.
 * @param fedge
 * 1st edge in edge sequence.
 * @param ledge
 * last edge in edge sequence.
 * @param start_sbdiff
 * setback difference at start.
 * @param end_sbdiff
 * setback difference at end.
 * @param start_sbdiff_set
 * start setback difference flag.
 * @param end_sbdiff_set
 * end setback difference flag.
 * @param start_stop_ang
 * start of stop angle.
 * @param end_stop_ang
 * end of stop angle.
 */

	ATTRIB_VAR_BLEND(
				ENTITY * entity = NULL,		// owner
				CURVE *	curve1	= NULL,		// defining curve, edge geom if null
				SPAinterval * prange = NULL,		// param range, all if null
				double sbk_start = 0,				// setback at start
				double sbk_end = 0,				// setback at end
				FACE * lface = NULL,			// left face
				FACE * rface = NULL,			// right face
				EDGE * fedge = NULL,			// first edge in smooth sequence
				EDGE * ledge = NULL,			// last edge in smooth sequence
				double start_sbdiff = 0.0,        // setback diff at start
				double end_sbdiff = 0.0,          // setback diff at end
				logical start_sbdiff_set = FALSE, // setback diff set at start
				logical end_sbdiff_set = FALSE,    // setback diff set at end
			    double start_stop_ang = 0.0,	   // start stop angle
			    double end_stop_ang = 0.0		   // end stop angle
			);


	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b>	Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_VAR_BLEND(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param blnd
 * existing blend.
 */
	ATTRIB_VAR_BLEND( ATTRIB_VAR_BLEND const * blnd);

/**
 * Copies a blend (setting implicit blend data only and without attaching the copy to an entity).
 */
	virtual ATTRIB_BLEND* copy() const;

/**
 * Returns TRUE if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;

// Simple read function for the data members.

// Return the defining curve.
// tbrv
/**
 * @nodoc
 */
	CURVE	*def_curve();

// Return the form of the variable radius function.
// tbrv
/**
 * @nodoc
 */
	virtual radius_forms radius_form()		const	{ return rad_form; }

// Return the start radius of a 'two-ends' form radius.
/**
 * Returns the radius at the beginning of the blend.
 */
	double	start_rad()				const	{ return start_radius; }

// Return the end radius of a 'two-ends' form radius.
/**
 * Returns the radius at the end of the blend.
 */
	double	end_rad()				const	{ return end_radius; }


// Return the radius function as a bs2_curve.
/**
 * Returns the radius function as a constant <tt>bs2_curve</tt>.
 */
	virtual bs2_curve const radius_bs2() const;

// Return the left radius function as a bs2_curve.
/**
 * Returns the left radius function as a constant <tt>bs2_curve</tt>.
 */
	virtual bs2_curve const left_radius_bs2() const;

// Return the right radius function as a bs2_curve.
/**
 * Returns the right radius function as a constant <tt>bs2_curve</tt>.
 */
	virtual bs2_curve const right_radius_bs2() const;

// The same three, but allow the radius function to be changed:
/**
 * Returns the radius function as a <tt>bs2_curve</tt>, in a form that can be modified.
 */
	virtual bs2_curve radius_bs2_for_update();
/**
 * Returns the left radius function as a <tt>bs2_curve</tt>, in a form that can be modified.
 */
	virtual bs2_curve left_radius_bs2_for_update();
/**
 * Returns the right radius function as a <tt>bs2_curve</tt>, in a form that can be modified.
 */
	virtual bs2_curve right_radius_bs2_for_update();

// BLP, 12/16/03
// Function to determine whether this attribute is an advanced
// var rad chamfer attribute.  Because of where ATTRIB_ADV_VAR_BLEND
// and ATTRIB_VAR_BLEND are defined (abl and blnd) it is often
// hard to get to this information.  Therefore, I am making this
// virtual function.
/**
 * @nodoc
 */
	virtual logical is_ADV_VAR_CHAMFER(); 


// Return whether the left and right radii are different.
/**
 * Returns TRUE if the left and right offsets are different.
 */
	logical		different_radii()	const	{ return two_radii; }

// Return whether the radius function has been calibrated.
/**
 * Returns TRUE if the parameterization of a two-ends blend has been calibrated; otherwise, it returns FALSE.
 */
	logical two_ends_calibrated()	const	{ return calibrated_2_ends; }

// Return the start SPAparameter of the radius SPAparameter range.
/**
 * Returns the radius start parameter.
 */
	double	rad_start_param()		const	{ return rad_t0; }

// Return the end SPAparameter of the radius SPAparameter range.
/**
 * Returns the radius end parameter.
 */
	double	rad_end_param()			const	{ return rad_t1; }


// Return the blend surface, which may be null or incomplete.
// tbrv
/**
 * @nodoc
 */
	var_blend_spl_sur *blend_splsur()	const	{ return blend_srf; }

// Return the attribute's list of v_bl_contacts objects.
// tbrv
/**
 * @nodoc
 */
	const list_of_contacts *contact_list()	const	{ return &slice_list; }

// Attach any efints for this attribute
// tbrv
/**
 * @nodoc
 */
	virtual logical attach_ints(BODY *blend_body, BODY *blank_body);

// Return the first edge in the sequence of edge to be blended.

/**
 * Returns the first edge in the sequence of edges to be blended.
 */
	EDGE *edge_seq_start()			const;

// Set the first edge in the sequence of edges to be blended.

/**
 * Set the first edge in the sequence of edges to be blended.
 * Returns the previous first edge in the sequence.
 */
	EDGE  *set_edge_seq_start( EDGE * );


// Return the last edge in the sequence of edge to be blended.
/**
 * Returns the last edge in the sequence of edges to be blended.
 */
	EDGE *edge_seq_end()			const;

// Set the last edge in the sequence of edges to be blended.

/**
 * Set the last edge in the sequence of edges to be blended.
 * Returns the previous last edge in the sequence.
 */
	EDGE *set_edge_seq_end( EDGE * );

// Return the sense of the defining curve with respect to the blend surface.
/**
 * Returns the sense of the defining curve with respect to the blend surface.
 */
	REVBIT edge_sense() const { return sense_data; }

// Other inquiry functions.

// Return a measure of the size of a blend at the start or end of the radius.
/**
 * Returns a measure of the largest size of a cross-section of the blend.
 * <br><br>
 * <b>Role:</b> For constant radius, <tt>blend_size</tt> returns the radius.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	virtual double	blend_size( logical at_start ) const;

// Return a measure of the size of a blend at the start or end vertex.
/**
 * Returns a measure of the size of a blend at the start or end vertex.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	virtual double	blend_size_at_vtx( logical at_start ) const;

	// Returns TRUE if constant offset blend
	// This function returns positive left_offset & right_offset 
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

// Decide whether a blend has a zero radius all along its length.
/**
 * Returns TRUE if the blend is zero everywhere along its length.
 */
	virtual logical	is_zero() const;

// Returns -1 is radius function is not constant round, otherwise
// the constant radius value
/**
 * Returns -1 if the radius function is not constant round; otherwise it returns the constant radius value.
 */
    virtual double is_constant_round() const;

// Limits the radius function to the radius start and end SPAparameter - used on
// adjacent variable radius blends when replacing a variable radius blend with
// a constant blend
/**
 * Limits the radius function to the radius start and end parameter.
 * <br><br>
 * <b>Role:</b> Used on adjacent variable radius blends when replacing a variable radius blend with a
 * constant blend.
 */
    virtual void limit_radius_to_edge();

// The following three methods should only be called after
// the blend_spl_sur has been created.
/**
 * Returns TRUE if the blend is circular.
 * <br><br>
 * <b>Role:</b>	Returns the expected defaults so only unusually derived classes need to worry
 * about returning something different.
 */
	virtual logical is_circular() const;

/**
 * Returns TRUE if the blend end radius is zero(less than <tt>SPAresabs</tt>) at the start point if <tt>at_start</tt> is TRUE, or at the end point if <tt>at_start</tt> is FALSE.
 * <br><br>
 * <b>Role:</b>	Returns the expected defaults so only unusually derived classes need to worry about
 * returning something different.
 * <br><br>
 * @param at_start
 * at start.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius(
		logical at_start, double tol = SPAresabs ) const;
/**
 * Returns TRUE if the blend end radius slope is zero(less than <tt>SPAresabs</tt>) at the start point if <tt>at_start</tt> is TRUE, or at the end point if <tt>at_start</tt> is FALSE.
 * <br><br>
 * <b>Role:</b>	This returns the expected defaults so only unusually derived classes need to worry about
 * returning something different.
 * <br><br>
 * @param at_start
 * at start.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_rad_slope(
		logical at_start, double tol = SPAresabs ) const;

	// Return whether the left face means we need to compute some
	// reordered blends.  
	// NOTE: Derived versions of these should invoke the parent's
	// corresponding method first, to check for "impossible".

/**
 * Returns whether reordered blends should be computed.
 * <br><br>
 * <b>Role:</b> Returns ro_const_radius (needs reordering), ro_not_reqd (doesn't), or 
 * ro_impossible (reordering is impossible).
 * Derived versions should invoke the parent's corresponding method first, to check for "impossible".
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        left_reorder(logical &convex = *(logical*)NULL_REF);

/**
 * Returns whether reordered blends should be computed.
 * <br><br>
 * <b>Role:</b> Returns ro_const_radius (needs reordering), ro_not_reqd (doesn't), or 
 * ro_impossible (reordering is impossible).
 * Derived versions should invoke the parent's corresponding method first, to check for "impossible".
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        right_reorder(logical &convex = *(logical*)NULL_REF);


// Return the v-SPAparameter range of the entire blend surface that
// will be created from this attribute.  This includes any extensions
// on both ends.  Before the blend surface is constructed, this will
// return the range of the defining curve, i.e. the same as base_v_range().

/**
 * Returns the v-parameter range.
 */
	virtual SPAinterval v_param_range() const;

// Return the basic v-SPAparameter range of the blend.
// This is simply the range of the edge being blended.

/**
 * Returns the basic v-parameter range of the blend, which is simply the range of the edge being blended.
 */
    SPAinterval base_v_range( logical ignore_prop = FALSE ) const;

// Return the SPAparameter range of the radius function.
// This returns the start and end parameters of the entire sequence
// of edges that share this radius function, if the radius function
// is calibrated.  If not calibrated, it returns the base v_range.

/**
 * Returns the parameter range of the radius function.
 * <br><br>
 * <b>Role:</b> This returns the start and end parameters of the entire sequence of edges that
 * share this radius function, if the radius function is calibrated. If not calibrated,
 * it returns the base <tt>v_range</tt>.
 */
	SPAinterval rad_param_range();

// Return a bs2_curve representation of the radius function(s).
// This might be approximate for certain exotic radius functions.
// These create a new bs2_curve.

/**
 * Returns a <tt>bs2_curve</tt> representation of the radius function.
 * <br><br>
 * <b>Role:</b> This might be approximate for certain exotic radius functions. This creates a new
 * <tt>bs2_curve</tt>.
 */
	virtual bs2_curve make_radius_bs2() const {return make_left_radius_bs2();}
/**
 * Returns a <tt>bs2_curve</tt> representation of the left radius function.
 * <br><br>
 * <b>Role:</b>This might be approximate for certain exotic radius functions. This creates a new
 * <tt>bs2_curve</tt>.
 */
	virtual bs2_curve make_left_radius_bs2() const;
/**
 * Returns a <tt>bs2_curve</tt> representation of the right radius function.
 * <br><br>
 * <b>Role:</b>This might be approximate for certain exotic radius functions. This creates a new
 * <tt>bs2_curve</tt>.
 */
	virtual bs2_curve make_right_radius_bs2() const;

// Determine whether another blend can mate end-to-end with this blend.
/**
 * Tests whether two blends are compatible for mating end to end.
 * <br><br>
 * @param battr
 * blend attribute.
 */
	logical same_blend( ATTRIB_VAR_BLEND * battr) const;

// Member-setting functions.
// These ensure that the attribute is safely backed up before making the change.
/**
 * Sets the left and right radius.
 * <br><br>
 * @param lrad
 * left radius.
 * @param rrad
 * right radius, if different than left.
 */
	virtual logical	set_radius(
			            var_radius * lrad,		 // left radius
			            var_radius * rrad = NULL // right radius if different
			            );
/**
 * Sets the radius to a <tt>TWO_ENDS</tt> form.
 * <br><br>
 * @param radius1
 * should be TWO_ENDS.
 * @param srad
 * start radius.
 * @param erad
 * end radius.
 */
	virtual logical	set_radius(
			            radius_forms radius1,    // should be TWO_ENDS
			            double left_srad,        // left start radius
			            double left_erad,	     // left end radius
                        double right_srad = -1., // right start radius
                        double right_erad = -1.  // right end radius
			            );
/**
 * Sets the radius to a <tt>FUNCTIONAL</tt> form.
 * <br><br>
 * @param frad
 * should be FUNCTIONAL.
 * @param crad
 * radius function.
 */
	virtual logical	set_radius(
			            radius_forms frad,		       // should be FUNCTIONAL
			            bs2_curve	left_crad,		   // left radius function
                        bs2_curve   right_crad = NULL  // right radius function
			            );

// Set whether the left and right radii are the same.
// flag is true if different.
/**
 * Sets whether the left and right radii are the same.
 * <br><br>
 * <b>Role:</b> Logical flag is TRUE, if different.
 * <br><br>
 * @param flag
 * flag for difference.
 */
	void set_rads_different( logical flag ) { this->two_radii = flag; }

/**
 * Sets the interval for the v-range.
 * <br><br>
 * @param new_range
 * interval.
 */
	void set_v_range( SPAinterval new_range );

// set the defining curve.
// This re-parameterization the attribute with respect to the new defining
// curve, and fixes up the blend_spl_sur accordingly, if present.

/**
 * Sets the defining curve.
 * <br><br>
 * <b>Role:</b>This reparameterizes the attribute with respect to the new defining curve,
 * and fixes up the <tt>blend_spl_sur</tt> accordingly, if present.
 * <br><br>
 * @param cur1
 * new defining curve.
 */
	void set_def_curve( CURVE * cur1);

// Set the blend spl_sur.  This does not delete what's there,
// the caller must do that if desired.
/**
 * Sets the blend spline surface.
 * <br><br>
 * @param blend_sf
 * blend surface.
 */
	void set_blend_splsur( var_blend_spl_sur * blend_sf );

	// Return if an entity-entity blend can be done for this type of
	// blend.

/**
 * Returns TRUE if an entity-entity blend can be done for this type of blend.
 */
    virtual logical ent_ent_blend_implemented() const;

// If the left or right faces as given by mating information at the
// start differ from those of the edge, the left and right faces
// in the attribute are reset to those given by the mate so that blend geometry
// is found for mating surfaces.  In this case, the v-SPAparameter range of the
// blend surface must be updated from the faces given by the mating information.

/**
 * Finds the parameter range that corresponds to the extent at the edge being blended.
 * <br><br>
 * <b>Role:</b> A segend holds information about the blend sheet segment.
 * <br><br>
 * @param seg1
 * segend.
 * @param seg2
 * segend.
 */
	void update_v_range_from_faces( segend * seg1, segend * seg2);

// Remove any calculated geometry: ffblend_geom and slice list.

/**
 * Removes any calculated geometry including the blend surface, the slice list, and the <tt>ffblend_geom</tt> object.
 */
	virtual void remove_ffblend_geometry();

// Add an end slice.
// This will generally be called while a different edge is being blended,
// when this end slice was found incidentally to finding that blend's end slice.
// This happens when the vertex is blended, and spring curves must be
// intersected.  It saves the effort of intersecting spring curves twice.

/**
 * Adds an end slice.
 * <br><br>
 * <b>Role:</b> This method is usually called while a different edge is being blended when
 * this end slice was found incidentally to finding that blend's end slice. This happens
 * when the vertex is blended, and spring curves must be intersected. It saves the effort
 * of intersecting spring curves twice.
 * <br><br>
 * @param new_end
 * new end.
 * @param at_start
 * start vertex.
 */
	void add_end_slice( v_bl_contacts *new_end, logical at_start );


	// Create a radius object from the information in the attribute.

/**
 * Creates a radius object, <tt>var_radius</tt>, from the information in the attribute.
 */
	var_radius *make_radius_object() const;

// The next one exists for creating the second radius function.
// In most cases, both radius functions will be the same,
// and will simply point to the same object.

/**
 * Creates the second radius function.
 * <br><br>
 * <b>Role:</b> If left and right radii are the same, this returns the pointer passed in.
 * <br><br>
 * @param first_rad_object
 * first radius object.
 */
	var_radius *make_radius_object( var_radius *first_rad_object ) const;

	// Create a section object from the information in the attribute.

/**
 * Creates a section object, <tt>var_cross_section</tt>, from the information in the 
 * attribute.
 */

	virtual var_cross_section *make_section_object() const;

// Create the blend surface from the information in the attribute.
/**
 * Creates the blend surface from the information in the attribute.
 */
	virtual void make_blend_surf();


// Calibrate (or un-calibrate) the two-end radius range.
/**
 * Calibrates the two-end radius range.
 * <br><br>
 * <b>Role:</b> A segend holds information about the blend sheet segment.
 * <br><br>
 * @param s
 * start parameter.
 * @param e
 * end parameter.
 */
	virtual logical calibrate_radius( segend *s = NULL, segend *e = NULL );
/**
 * Un-calibrates the two-end radius range.
 */
	virtual void uncalibrate_radius();

// If this blend joins smoothly with another blend on either end,
// extend the def curve and radius function so that they will match
// those of the neighbor's when evaluating off the end.

// STI bd at TSL May 1997:
// Break extend_def into extend_def_crv and extend_rad.
// extend_def_crv() must happen before the radius is calibrated,
// and extend_rad() after.
// Also, they now return logicals, and no longer need segends.
//	virtual void extend_def( segend *s = NULL, segend *e = NULL );

/**
 * If this blend joins smoothly with another blend on either end, we must extend the <tt>def_crv</tt> and radius function so that they will match those of the neighbor's when evaluating off the end.
 * <br><br>
 * <b>Role:</b> We need to extend the <tt>def_crv</tt> first, then calibrate, then extend the radius function.
 * These might replace the defining curve and the radius functions in the blend surface, but leaves
 * the originals in the attribute. That is necessary to enable one blend to extend into a neighbor
 * that has already been extended.
 */
	virtual logical extend_def_crv();
/**
 * Extend the radius function into the neighbors.
 * <br><br>
 * <b>Role:</b> The logical return value does not say whether or not the radius was extended.
 * It returns false if there appears to be a problem that would produce inaccurate results.
 */
	virtual logical extend_rad();


// Return the start or end vertex.
/**
 * Returns the start or end vertex.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	VERTEX *end_vertex( logical at_start );

// Find the blend attribute from "next" edge, in the left or right face.
// at_start and left/right flags are returned for the other blend for convenience.

/**
 * Finds the blend attribute from next edge in the left or right face.
 * <br><br>
 * <b>Role:</b> This method returns the <tt>at_start</tt> and left/right flags for the other blend for
 * convenience.
 * <br><br>
 * @param this_at_start
 * this start vertex.
 * @param this_left_srf
 * this left surface.
 * @param other_at_start
 * other start vertex.
 * @param other_left_srf
 * other left surface.
 */
	ATTRIB_FFBLEND *
	find_next_blend( logical  this_at_start,  logical  this_left_srf,
					  logical *other_at_start, logical *other_left_srf );

// Find the blend attribute from next smoothly-joining edge.
// The edges must meet smoothly and the end radii must match.
// at_start flag is returned for the other blend for convenience.
// If the attribute has no owning edge (e.g., an automatically-created
// bi-blend), the optional segend might help to find an answer.

/**
 * Finds the blend attribute from next smoothly-joining edge.
 * <br><br>
 * <b>Role:</b> The edges must meet smoothly and the end radii must match. This method
 * returns the at_start flag for the other blend for convenience. If the attribute has
 * no owning edge (e.g., an automatically-created bi-blend), the optional segend might
 * help to find an answer.
 * <br><br>
 * @param this_at_start
 * this start vertex.
 * @param other_at_start
 * other start vertex.
 * @param seg
 * optional, to find owner.
 */
	ATTRIB_FFBLEND *
	find_next_smooth_blend( logical this_at_start, logical *other_at_start,
			segend *seg = NULL );

// Determine whether the vertex is blended.
/**
 * Determines if the start or end vertex is blended.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	logical vtx_is_blended( logical at_start );

// Determine whether this blend ends in a bi-blend.
/**
 * Returns TRUE if the given blend ends an <tt>bi_blend</tt>; otherwise, it returns FALSE.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	logical is_bi_blend( logical at_start );

// Return the SPAparameter value at a setback.
/**
 * Returns the parameter value at a setback.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	double setback_param( logical at_start );


// Return the end of the defining curve.

/**
 * Returns the end of the defining curve.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	SPAposition def_crv_end_pos( logical at_start );

// Return the tangent at the end of the defining curve.
/**
 * Returns the tangent at the end of the defining curve.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	SPAvector def_crv_end_tan( logical at_start );


// Take a slice at a v-SPAparameter.
// This is a wrapper for the var_blend_spl_sur method of the same name.
/**
 * Takes a slice at a v-parameter.
 * <br><br>
 * <b>Role:</b> This is a wrapper for the blend-surface method of the same name.
 * <br><br>
 * @param v_param
 * v parameter.
 * @param n_derivs
 * number of derivatives.
 * @param guess
 * guess value.
 * @param approx_ok
 * approximate results OK.
 * @param repeat_order
 * repeat order.
 */
	v_bl_contacts *
	eval_springs(	double v_param,
					int		n_derivs,
					v_bl_contacts *guess = NULL,
					logical	approx_ok    = FALSE,
					logical	repeat_order = FALSE
				);


// Return start or end radius.
// This is a wrapper for the var_blend_spl_sur method with a very similar name.
/**
 * Returns the start or end radius.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	virtual double eval_end_radius( logical at_start );

// Return radius slope (dr/dv) at start or end.
// This is a wrapper for the var_blend_spl_sur method with a very similar name.
/**
 * Returns the slope (dr/dv) at the start or end.
 * <br><br>
 * @param at_start
 * start vertex.
 */
	virtual double eval_end_rad_slope( logical at_start );

// Intersect one of our spring curves with that of an adjacent edge blend.

/**
 * Intersects one of the spring curves with an adjacent edge blend.
 * <br><br>
 * @param this_at_start
 * curve at start.
 * @param this_left_right
 * curve left or right.
 * @param other_blend
 * face to face blend.
 * @param other_at_start
 * curve at start.
 * @param other_left_right
 * curve left or right.
 * @param other_slice
 * blend contacts other slice.
 */
	v_bl_contacts *
	int_spring_crvs( logical this_at_start,
					 logical this_left_right,
					 ATTRIB_FFBLEND *other_blend,
					 logical other_at_start,
					 logical other_left_right,
					 v_bl_contacts **other_slice
				);


// Calculate the geometry of the blend surface.
// The geometry is returned in a ffblend_geom object.

/**
 * Creates the blend geometry. A segend holds information about the blend sheet segment.
 * <br><br>
 * <b>Role:</b> A <tt>segend</tt> holds information about the blend sheet segment.
 * <br><br>
 * @param open_at_start
 * open at start.
 * @param open_at_end
 * open at end.
 * @param start_segend
 * start segend.
 * @param end_segend
 * end segend.
 * @param b_box
 * bounding box.
 */
	virtual ffblend_geom *
	find_ffblend_geometry(
		logical open_at_start,
		logical open_at_end,
		segend  *start_segend,
		segend  *end_segend,
		SPAbox const & b_box = *(SPAbox const *)NULL_REF,
        double & reduced_resabs = *(double*)    NULL_REF
	);

// Find the spine.
 /**
  * Creates the spine.
  * <br><br>
  * <b>Role:</b> A <tt>segend</tt> holds information about the blend sheet segment.
  * <br><br>
  * @param open_at_start
  * open at start.
  * @param open_at_end
  * open at end.
  * @param start_segend
  * start segend.
  * @param end_segend
  * end segend.
  * @param b_box
  * bounding box.
  */
	virtual ffblend_geom *
	find_ffblend_spine( logical open_at_start, logical open_at_end, segend * start_segend,
						segend * end_segend,
						SPAbox const & b_box = *(SPAbox const *)NULL_REF );

// Find spring curves.
// The curves returned have the same sense as the spine.

/**
 * Creates the spring curves.
 * <br><br>
 * <b>Role:</b> The curves returned have the same sense as the spine. A <tt>segend</tt> holds information about the blend sheet segment.
 * <br><br>
 * @param spine_ffg
 * contains spine.
 * @param open_at_start
 * open at start.
 * @param open_at_end
 * open at end.
 * @param start_segend
 * start segend.
 * @param end_segend
 * end segend.
 * @param b_box
 * bounding box.
 */
	virtual ffblend_geom *find_ffblend_spring_curves(
			ffblend_geom *spine_ffg,
			logical open_at_start,
			logical open_at_end,
			segend  *start_segend,
			segend  *end_segend,
			SPAbox const & b_box = *(SPAbox const *)NULL_REF
		);

// Find cross curve at given v SPAparameter.

/**
 * Creates a cross curve at a given v-parameter.
 * <br><br>
 * @param v_param
 * v parameter.
 */
	curve *find_ffblend_cross_curve( double v_param );

// Find cross curve in given plane (must be normal to spine and
// spring curves).  The curve found runs from left to right across
// the blend.

/**
 * Finds the cross curve in given plane (normal to spine and spring curves).
 * <br><br>
 * <b>Role:</b> The curve found runs from left-to-right across the blend.
 * <br><br>
 * @param spine1
 * contains spine.
 * @param plane1
 * plane.
 * @param b_box
 * bounding box.
 * @param first
 * first point.
 * @param second
 * second point.
 * @param third
 * third point.
 */
	virtual curve *find_ffblend_cross_curve(
			ffblend_geom *spine1,
			plane const & plane1,
			SPAbox const & b_box,
			SPAposition & first,		// left_pos, if known
			SPAposition & second,		// right_pos, if known
			SPAposition &	third	// spine_pos, if known
		  );

// Given blend boundary details in a ffblend_geom, find the
// blend surface and return it and the given boundary details
// in a ffblend_geom.

/**
 * Finds the blend surface and returns it and the given boundary details in a <tt>ffblend_geom</tt>, given the blend boundary details in a <tt>ffblend_geom</tt>.
 * <br><br>
 * @param spine1
 * contains spine.
 */
	virtual ffblend_geom *find_ffblend_surface(ffblend_geom *spine1);


	// The rest of the generic stuff.

// Functions called to aid attribute migration during modelling operations.

/**
 * Notifies the <tt>ATTRIB_VAR_BLEND</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
 * The default action is to do nothing. This function is supplied by the application whenever
 * it defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param entity
 * new entity.
 */
	virtual void split_owner( ENTITY * entity);
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.
/**
 * Notifies the <tt>ATTRIB_VAR_BLEND</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b>	The application has the chance to delete or otherwise modify the attribute.
 * After the merge, this owner will be deleted if the logical deleting owner is TRUE,
 * otherwise it will be retained and other entity will be deleted. The default action is to
 * do nothing. This function is supplied by the application whenever it defines a new attribute,
 * and is called when a merge occurs.
 * <br><br>
 * @param entity
 * given entity.
 * @param del_owner
 * deleting owner.
 */
	virtual void merge_owner( ENTITY *entity, logical del_owner);
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

/**
 * Notifies the <tt>ATTRIB_VAR_BLEND</tt> that its owner is about to be transformed.
 * <br><br>
 * <b>Role:</b>	The application has the chance to transform the attribute. The default action
 * is to do nothing. This function is supplied by the application whenever it defines a new attribute,
 * and is called when a transformation occurs.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void trans_owner( SPAtransf const & trans);
									// the owner of this attribute is
									// being transformed

// Account for transforms present on the body within blending.
/**
 * @nodoc
 */
	virtual void trans_blend( logical, logical );

// Clean up potentially bad edge sequence info.
/**
 * @nodoc
 */
	ATTRIB *move( ENTITY *new_entity );

// Test two blends for equality.
// tbrv
/**
 * @nodoc
 */
	virtual logical operator==( ATTRIB_BLEND const& ) const;

// Return a measure of continuity at blend spring curves
// i.e. unset_continuity, position_continuous, slope_continuous,
// or curvature_continuous.
/**
 * Returns a measure of the continuity of the blend where it meets its supporting entities.
 */
	virtual bl_continuity continuity( int sup_index ) const;

// Reverse left-to-right properties of an asymmetric blend.
/**
 * Reverses the left-to-right properties of an asymmetric blend.
 */
	virtual void reverse();

// Set the attribute member sense_data, the sense of the edge
//	with respect to the defining curve
/**
 * Sets the attribute member <tt>sense_data</tt>, the sense of the edge, with respect to the defining curve.
 */
	void set_edge_sense();

// If this blend is part of a blend sequence, and it is being removed
// (e.g. replaced by a 'single' blend), then the sequence must be
// adjusted to start and end before and after this blend.

/**
 * If this blend is part of a blend sequence, and it is being removed, then the sequence must be adjusted to start and end before and after this blend.
 */
	void update_sequence_for_lose();

// Set new sequence information in an attribute.
// Also sets the sense_data and initial SPAparameter ranges.

/**
 * Set new sequence information in an attribute.
 * <br><br>
 * <b>Role:</b> Also sets the sense data and initial parameter ranges.
 * <br><br>
 * @param new_start_edge
 * starting edge.
 * @param new_end_edge
 * ending edge.
 * @param cal_crv
 * calibration curve.
 */
	void set_new_sequence(
			EDGE *new_start_edge,
			EDGE *new_end_edge,
			CURVE *cal_crv
		);

/**
 * @nodoc
 */

    void  set_support_faces( 
		FACE *nleft_face,
		FACE *nright_face,
		logical reversed
		);

// After setting a new blend on an entity, we must check it and neighboring
// blends for blend sequences, interactions with const-rad blends, etc.


/**
 * After setting a new blend on an entity, we must check it and neighboring blends for blend sequences, interactions with const-rad blends, etc.
 */
	void fixup_existing_blends();


	// enquire
	/**
	 * @nodoc
	 */
	virtual blend_debug_info* enquire(FILE*fp);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_VAR_BLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

// Link to use count rollback maintenance.

	/**
	 * @nodoc
	 */
	USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

private:

    // The defining curve.  This will usually be the geometry of the owning edge,
    //	but other possibilities are enabled.
    // Currently we're using a CURVE, because of use_count().

	CURVE *def_CURVE;
	SPAinterval v_range;

	// Whether the next two parameters are valid.

	logical calibrated_2_ends;	

	// Exactly where the radius function parameterization starts and ends

	double rad_t0;				
	double rad_t1;				

    // The radius functions.

	radius_forms rad_form;

	// More may be added to these.
	// There must be methods to set the main radius functions from these.

    // Two-ends form:

	double	start_radius;	// radius_1 and radius_2 are the same.
	double	end_radius;

    // Functional form:
	// These are pointers so that, if the left and right radii are the same,
	// they will just point to the same function.

	bs2_curve radius_fcn_1;
	bs2_curve radius_fcn_2;
	logical	two_radii;

    // Maintain a pointer to the (incomplete) blend surface.
    // It contains radius & section objects, and whatever else.
    // It is a transient ptr, only used during the construction process,
    // and thus need not be saved/restored.

	var_blend_spl_sur *blend_srf;

    // List of slices.

	list_of_contacts slice_list;

    // Start and end edges in a sequence to be blended that all share
    // the same radius function.  One or both may be this blend's edge.

	EDGE *first_edge;
	EDGE *last_edge;

    // Indicates whether the def_CURVE is forward or reversed
	// from the owning edge ( and hence the blend's) direction

	REVBIT sense_data; 

    // The following are no longer used by ATTRIB_VAR_BLEND,
    // but we'll retain the slots for convenience (.sat, etc.)
    // and possible future use.

	double width_data;

	cross_section_forms section_form;
	double left_thumbwt;
	double right_thumbwt;

    // Helper for find_ffblend_geometry().
    // Check that the attrib has all it needs to create its geometry.

	virtual logical set_for_blending();

    // Helper for make_radius_object().

	virtual var_radius *local_make_radius_object( int ) const;

    // Helpers for the set_radius() methods.

	void clean_out();
	void set_surface_radius();


    // Marching stuff.

	logical march( double tol );
	logical march_step( 
                v_bl_contacts *prev_slice,
				logical init,
				double *next_param 
                );
	SPAinterval march_range();

	v_bl_contacts *get_end_slice( 
                        logical at_start, 
                        logical open_end,
                        segend &existing_end 
                        );
	
    v_bl_contacts *get_vert_blend_slice( logical at_start );
	
    v_bl_contacts *get_extreme_slice( 
                        logical at_start, 
                        logical open_end, 
                        segend &existing_end 
                        );

	// Function to obtain extreme (outermost) marching slice by
	// implementing point perp logic on the extended defining curve.
	// If the slice obtained is not the outermost, then get it by old
	// method of var_blnd_spl_sur->eval_spring(..). btq.

	v_bl_contacts *get_outermost_slice(
						segend *extreme_segend,
						v_bl_contacts *guess_slice,
						double slice_param,
						logical extreme_at_start,
						logical open_at_extreme
						);

	ffblend_geom *construct_geom( void );

    // Helper functions for calibrate_radius().

    // Find a value, if any, to which the start or end slope of this
    // blend must be constrained.  Returns NULL if no constraints.

	double *find_end_slope( logical at_start );

    // Try to find a radius calibration SPAparameter at one end.
    // If this is a 'terminal' end (either at the end of a sequence of
    // blends that share a single radius function, or not in such a
    // sequence), then do the work of finding the calibration value.
    // If this is not a terminal end, then check the terminal blend
    // in that direction.  If that blend is already calibrated, return
    // its end value, otherwise return failure; leave it to the higher
    // level routine to deal with calibrating the terminal ends.
    //
    // This returns a pointer to the terminal blend, for convenience.
    // It might be the same as 'this'.

	logical find_end_rad_param(
				logical at_start,
				segend *seg,
				double *param,
				ATTRIB_VAR_BLEND **end_blend
			    );

    // Calibrate the v-SPAparameter range.

	logical calibrate_v_params();

	SPAinterval v_range_from_edge( logical ignore_prop = FALSE ) const;

	void show_marching_failure(err_mess_type slice_error) const;

protected:

	// Account for transforms present on the body within blending.

	virtual void trans_data( SPAtransf const & );

    // The older version of geometry construction based on spring_int_cur.

    ffblend_geom *construct_geom_old( void );

	// Use part of construct_geom_old to generate a bs3 surface
	// from the slice list, so that we can examine the slices.

	int debug_slices_to_file( void );

	// Sets the ffblend attribute on given edge

	virtual void set_ffblend(EDGE *, set_ffblend_input &);

	// Function to determine edge_sense wrt defining curve, to be used
	// in public function set_edge_sense

	int decide_edge_sense() const;

};


//	=====    End of the class.  The following aren't members:

// Like a constructor: creates an attribute on the entities in a list.

/**
 * @nodoc
 */
DECL_BLND void
set_var_blends(
		ENTITY_LIST const&,	// list of edges
		double,			// start radius (always positive)
		double,			// end radius (always positive)
		double,			// start setback
		double,			// end setback
		CURVE * = NULL,	// calibration_curve,
		EDGE * = NULL,  // first edge in smooth sequence
		EDGE * = NULL,  // last edge in smooth sequence
		double = 0,		// setback difference (r - l) at start
		double = 0,		// setback difference (r - l) at end
		logical = TRUE,	// setback difference set at start
		logical = TRUE,	// setback difference set at end
		double = 0,		// start stop angle
		double = 0		// end stop angle
	);

/**
 * @nodoc
 */
DECL_BLND double
set_mid_slope(
		double dr1,
		double dr2,
		double dt1,
		double dt2
	);

/**
 * @nodoc
 */
DECL_BLND 
void set_legal_range_for_hl_marching(ATTRIB_BLEND *att);

/**
 * @nodoc
 */
DECL_BLND ATTRIB_VAR_BLEND * find_var_ffblend_attrib( ENTITY *owner );
/** @} */
#endif
