/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Class to represent a blend running along an edge.
#ifndef BL_SUP_EDGE
#define BL_SUP_EDGE
#include "dcl_blnd.h"
#include "acis.hxx"
#include "logical.h"
#include "bl_sup.hxx"
#include "bs2curve.hxx"
/**
 * @file bl_sup_e.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
class ATTRIB_BLEND;
class EDGE;
class COEDGE;
class curve;
class CVEC;
class ed_help_pt;
class SizeAccumulator;
// There is a problem in that sometimes the edge alone is not enough to
// decide where the blend surface goes; it could go either side of the
// edge (cf. a straight line above a planar surface, with the line
// direction in the plane). These cases are disambiguated by the blend
// attribute supplying some kind of starting point for the ball, either
// rough or exact.

// Firstly, an enum to record the sense of the spring curve along the
// edge. For now, we allow 3 possibilities.
// 1. Forward. The spring curve moves always forwards along the edge,
// possibly with momentary stationary points.
// 2. Reversed. Always going backwards along the edge, possibly with
// momentary stationary points.
// 3. Stationary. The spring curve is actually a point on the edge and
// does not move at all along it. Note that a further field,
// edge_stat_cw, indicating the direction of the pivotting about the
// edge, MUST also be set in this case.
// 4. Unset, meaning not yet set. Geometry construction MUST set it.

// REMEMBER that virtually all the functions in this class are used by
// the support_coedge class. So for example, if you mean the underlying
// edge write "edge()" and NOT "(EDGE *)entity()", and so forth.

/**
* Defines the sense of the blend spring curve with respect to a support edge of an edge-face or an edge-edge blend.
* <br>
* @param bl_forward
* The spring curve moves always forwards along the edge, possibly with momentary stationary points.
* @param bl_reversed
* The spring curve always goes backwards along the edge, possibly with momentary stationary points.
* @param bl_stationary
* The spring curve is actually a point on the edge and does not move at all along it. Note that a further field, edge_stat_cw, indicating the direction of the pivoting about the edge, MUST also be set in this case.
* @param bl_sense_unset
* Not yet set. Geometry construction MUST set it.
**/
enum bl_ed_sense
{ bl_forward, bl_reversed, bl_stationary, bl_sense_unset };

// tbrv
/**
 * @nodoc
 */
class DECL_BLND support_edge : public support_entity {

public:

	// Make a support_edge, given the underlying edge we are blending against.

	support_edge( ATTRIB_BLEND *, EDGE * );

	~support_edge();

	// Copy the support_edge. Only needs to remember information which
	// may be set prior to stage 1.

	virtual support_entity *copy() const;

	// The edge we're blending against.

	virtual EDGE *edge() const;

	// The edge SPAbox.

	virtual SPAbox get_box() const;

	// The edge curve, (copied and) corrected to have the same sense as
	// the edge.

	curve *crv();

	// Geometry-making routines MUST set this. Whether the edge is
	// forward, reversed or stationary w.r.t. the blend surface.

	void set_edge_dir( bl_ed_sense dir ) { _edge_dir = dir; }
	bl_ed_sense edge_dir() const { return _edge_dir; }

	// Another overloaded implementation of set_edge_dir

	void set_edge_dir( const curve *def_curve );	

	// These are used ONLY if the edge sense is stationary. It must
	// return TRUE if and only if the blend surface is pivotting
	// clockwise about the stationary point on the edge.

	void set_edge_stat_cw( logical cw ) { _edge_stat_cw = cw; }
	logical edge_stat_cw() const { return _edge_stat_cw; }

	// The important imprinting function.

	virtual void imprint( logical convex, BODY *sheet );

	// Function to "imprint" the end vertices of the edge on the spring
	// curve.

	void imprint_vertex_pts();

	// Function to "imprint" the two laterl face of the edge on the
	// spring curve.

	void imprint_face_pts();

	// Make "dummy" blend_ints along the middle of the edge; or if
	// periodic possibly at the start or end of the spring curve (as it
	// may then not start and finish with the edge).

	virtual blend_int *make_int( double );

	// Return the SPAparameter value of the given point on the
	// underlying curve and spring curve SPAparameter. Performs a
	// relaxation.

	virtual bl_par_info param( SPAposition const &, double );

	// Return the interior coedge. This function, which is only called
	// after geometry is made, deduces the interior coedge, that is, the
	// coedge which will be "swallowed up" when the blend is fixed.

	COEDGE *interior_coed() const;

	// Set the "help point info" for the subsequent imprinting
	// algorithms. Only gets set for spline surfaces. And return it.

	void set_ed_help( ed_help_pt *hlp ) { _ed_help = hlp; }
	ed_help_pt *ed_help() const { return _ed_help; }

	// Attach faceint attributes to show that the sheet face does
	// intersect with the two faces either side of this edge. See base
	// class declaration for more details.

	virtual void attach_global_ints( COEDGE *, VERTEX *, FACE * );

	// Attach any further known non-intersections between underlying
	// entities and the blend face. We may assume any known
	// intersections to have been marked.

	virtual void attach_non_ints( FACE * );

	// The "lateral face" that will be advertised as such to the
	// existing blend stage 2 code. We return the "interior face".
	// Stage 2 only.

	virtual FACE *lateral_face() const;

	// Determine whether the indicated point is in contact with the
	// edge. Provided for variable radius blending only.

	virtual point_support_containment on_support(v_bl_contacts * , logical );

	// Check the convexity relations of the given spring coedge, i.e.
	// check the blend surface really does appear to sit inside (for
	// convex), outside (for concave), the blank. The support_edge class
	// *does* do real work here, as this is where we expect the problems
	// to lie.

	virtual logical check_cvxty( COEDGE *spr_coed );

	virtual void full_size( SizeAccumulator &, logical = TRUE ) const;

protected:

	// Optimize the imprint of the spring curve on the lateral support.

	virtual void optimize_lateral_imprint();

	// Allocate a new blend_int_edge. 

	virtual blend_int *new_blend_int( face_face_int *, COEDGE * );

private:

	// Whether the blend surface has the same sense as the underlying
	// edge, or is reversed or stationary. It records whether the blend
	// surface has the same sense as _crv stored below, which may not be
	// in *its* anticlockwise conventional sense.

	bl_ed_sense _edge_dir;

	// Meaningful ONLY if the sense above is bl_stationary, and is TRUE
	// if and only if the blend is pivoting about the stationary point
	// in the clockwise sense (ie. going clockwise as you look on it
	// with the edge direction there pointing up towards you).

	logical _edge_stat_cw;

	// The curve underlying the edge, corrected to have the same sense
	// as the edge.

	curve *_crv;

	// A bag of information to help the imprinting algorithms when the
	// blend surface is a spline surface. This gets set if and only if
	// the blend surface is a blend_spl_sur type.

	ed_help_pt *_ed_help;
};

/**
 * @nodoc
 */

void 
bl_limit_support_edge_geom( 
    curve *crv, 
    SPAinterval const &ed_range 
    );

 /** @} */
#endif
