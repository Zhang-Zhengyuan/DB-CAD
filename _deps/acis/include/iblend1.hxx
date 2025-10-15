/* ORIGINAL: acis2.1/kernbool/blend1/iblend1.hxx */
/* $Id: iblend1.hxx,v 1.41 2002/08/09 17:28:25 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Declarations of internal routines used in blends stage 1.

// Adele Modifications:
//
//  27-Nov-18 aia4  Added member function near_tangent_laterals for segend to 
//					detect in any of the lateral edges if present are near tangent or not.
//  08-May-05 puc   Added member variables to flag segend parameter space info
//                  valid / invalid. 79400.
//  06-May-05 puc   Changes for segend coincidence detection in case of
//                  tolerant models.
//  26-Apr-05 puc   Changes for blended edges split by prop/scar edges. 79288.
//  15-Sep-04 puc   Changes for better handling spring edges coincident with edges
//                  on support faces. 76732.
//  03-Mar-04 syh	Added new relation to spring_int_rel to indicate out-of-face
// 					point on the spring curve. (Multiple Cap/Mitre.)
//  09-Jan-04 puc   Modified processing of segends for extracting spine information.
//  29-Dec-03 puc   Fix for 73701, Changes for tolerant models. Enhanced  
//                  tolerance aware sidecap creation functionality for non-fuzzy
//                  mitres.
//  14-Nov-03 puc   Changes for near-tangent mate tolerant stitching (Bug 73236), 
//                  enhanced code to detect near-tangencies
//  04-Nov-03 puc   Part of the changes made for entity-entity coupling project,
//                  Added misc. functions
//  28-Oct-03 puc   Part of the changes made for entity-entity coupling project,
//                  Changes to specify intercepts for outer faces.

//
// Modification---
//  08-Sep-03 puc   Fix for 72583, Added bl_mitre_find_sidecap_curve to replace
//  cur_from_sur_sur_thru_pnt_crv.
//  12-Mar-03 syh   Added cur_from_sur_sur_thru_pnt_crv to find intersection
//  	through point and intersects given curve. Used in mitring.
//	07-Feb-01 gct	Changed comment for check_sphere_next_attrib, as I
//		have greatly changed the content of this fn.
//  04-Jul-00 gct	Added new fns. make_single_bi_blend and segends_match.
//	13-Jun-00 gct	Added extra argument called_from_fill_gap to
//		edge_ffblend_segments fn. and added new fn. check_sphere_next_attrib.
//  12-Oct-99 GCT   Added arguments to fns. make_curve_on_surface and
//					make_par_int_curve.
//	31-Mar-99 bd	Pass blend1_data instead of statics.
//		Also move init..., do_one... and concl... from here to iblend1.hxx.
//	04-Mar-99 bd	Return new list index from move_bl_attrib_to_list_end().
//		(Don't check the error inside it).
//	26-Jan-99 bd	Return error flag from move_bl_attrib_to_list_end().
//	28-Oct-98 dap	Make ent_att_list arg in build_.._ffsegment non-const.
//	13-Oct-98 dap	Extra arg for edge_ffblend_segments.
//	02-Oct-98 bd	New routine blend_pre_process_seq().
//	11-Sep-98 icb	bl_merge_vertex changed to be global
//	24-Feb-98 dap	Extra mitre_at_start/end args added to
//		edge_ffblend_segments, process_ffints.
//	05-Jan-98 bd	Change args to set_left_incpt() and set_right_incpt(),
//		for changes in make_rb_blend_geom() to deal with local ops.
//	18-Dec-97 dap	Added new bl_edge_.._smooth functions, removed .._smooth_opp.
//	03-Jul-97 icb	List of blend atts added as argument of routine simple
//		and 2 args added to routine edge_ffblend_segments (v22test20).
//	29-May-97 icb/bd	New routine find_var_blend_seq().
//	17-Apr-97 bd	New segend methods left_pos(), right_pos(),
//		set_left_incpt() and set_right_incpt().
//

#if !defined( BLEND1_INT )
#define BLEND1_INT

#include "dcl_blnd.h"
#include "bl_att.hxx"

class ATTRIB_BLEND;
class ATTRIB_FFBLEND;
class ATTRIB_VAR_BLEND;
class ATTRIB_VBLEND;
class ATTRIB_CONST_ROUND;
class ATT_BL_ENT_ENT;

class BODY;
class COEDGE;
class CURVE;
class EDGE;
class ENTITY;
class ENTITY_LIST;
class FACE;
class PCURVE;
class VERTEX;
class ffblend_geom;
class vblend_geom;
class SPAbox;
class curve;
class ellipse;
class surf_surf_int;
class face_face_int;
class intcurve;
class intercept;
class SPAparameter;
class SPApar_pos;
class pcurve;
class point_cur;
class SPAposition;
class segend;
class ffsegment;
class vsegend;
class vsegment;
class surface;
class SPAunit_vector;
class SPAvector;
class blend_seq;
class blend_edge;
class support_entity;
class ATT_BL_SEG;
class graph_impl_atts;

class SizeAccumulator;


enum extremity {ext_start, ext_end};

// Enumerated type to indicate whether a surface, curve or point
// has been found (e.g. for offset of surface or spine curve/point).

enum bl_geom_type {bl_surf, bl_cur, bl_pnt, bl_nix};

// Return true if simple curve and surface lead to non-simple blend.

DECL_BLND logical axial_cir_on_tor(surface const *, curve const *);
DECL_BLND logical axial_str_on_cone(surface const *, curve const *);

// bl_edge_param_smooth_opp removed - no one uses it.

// Check edge at SPAparameter value is smooth. Note that this calls
// bl_edge_param_convex which makes it expensive on non-trivial
// geometries (at least compared to bl_edge_mid_smooth which can used
// cached convexity values). bl_edge_mid_smooth (or the start/end
// versions below) should always be preferred if possible.

DECL_BLND logical 
bl_edge_param_smooth(
	EDGE*,
	double,
	bl_ed_convexity & = 
        * (bl_ed_convexity *) NULL_REF
    );

// Return whether the middle (and therefore the entire length, excluding
// the very end points) of an edge is smooth.

DECL_BLND logical 
bl_edge_mid_smooth(
    EDGE *,
    bl_ed_convexity & = 
        * (bl_ed_convexity *) NULL_REF,
	double tol = SPAresnor
    );

// For completeness, provide the start/end versions of the above too.
// These are also efficient (after the first call, at any rate) because
// the convexity information is cached on the edge.

DECL_BLND logical 
bl_edge_start_smooth(
    EDGE *,
    bl_ed_convexity & = 
        * (bl_ed_convexity *) NULL_REF
    );

DECL_BLND logical 
bl_edge_end_smooth(
    EDGE *,
    bl_ed_convexity & = 
        * (bl_ed_convexity *) NULL_REF
    );

// Remove each mergeable bi-valent vertex and one of its edges
// (whilst extending the other edge) from a graph of blended entities.
// Remove the blend attributes of the deleted edges and vertices
// from the entity list supplied.

DECL_BLND void bl_merge( ENTITY_LIST & );

// Test whether a vertex is mergeable (i.e only two edges attached,
// with compatible geometries), and merge it out if so. Return TRUE
// if the vertex deleted, FALSE otherwise.  Remove its blend
// attribute from the given list of blend attributes (if any).

DECL_BLND logical
bl_merge_vertex(
    VERTEX *test_vert,
    ENTITY_LIST &bl_attrib_list = 
        * (ENTITY_LIST *) NULL_REF,
    logical only_check = FALSE // but don't modify
    );

// Returns TRUE if a rolling ball vertex blend is possible at the start
// vertex of the coedge, AND the given coedge is the "distinguished"
// edge, that is, the one of larger blend radius in the univex case, or
// the one of different convexity in the mixed convexity case. So one
// (of the 3) edges at this vertex should return TRUE. For the special
// case where the vertex is univex and all radii identical, all edges
// will return TRUE.
// If &ents given as null, disregards membership of list of blended entities.

DECL_BLND logical 
rb_vbl_distinguished_edge(
	COEDGE *,
	ENTITY_LIST const & = 
        * (ENTITY_LIST *) NULL_REF
    );

// Find a range describing a portion of an ellipse when exactly
// one piece of the ellipse lies within the SPAbox; if more than one
// piece lies within the SPAbox, return the range for the piece whose
// mid-point is nearest the given point; if no piece lies within or
// touches the SPAbox, return an empty SPAinterval.

DECL_BLND SPAinterval 
box_ellipse(
    ellipse const &, 
    SPAbox const &, 
    SPAposition const &
    );

DECL_BLND ATTRIB_BLEND *
blended_in_list(
	ENTITY*,
	ENTITY_LIST const & = 
        * (ENTITY_LIST *) NULL_REF
    );

// Make a four-sided blend face.  The sequence is vert-coed-vert etc.
// Each existing vertex is given by a coedge whose start is the vertex.

DECL_BLND FACE *
build_new_face(
	int,			// number of sides
	logical [],		// true if edge  geometry is not to be trimmed
	SPAposition [],	// must be given if vertex does not exist
	curve const*[],	// non-null if side exists
	PCURVE *[],		// non-null if side exists and surface parametric
	PCURVE *[],		// non-null if pcurve known for right-hand side of new edge
	surface *,		// surface for new face
	BODY *,			// sheet body
	COEDGE *[],		// non-null if vertex exists
	COEDGE *[],		// non-null if side exists (updated)
	ENTITY_LIST &	// faces for later deletion
    );

DECL_BLND ENTITY_LIST
build_sheet_face_from_ffsegment(
	ffsegment *,
	BODY *,
	COEDGE *,
	COEDGE *,
	bl_ed_convexity,
	logical,
	logical,
	logical,
	logical,
	ENTITY_LIST &
    );

DECL_BLND SPAbox box_from_segend(segend *);

// Find SPAparameter value lying in primary range.

DECL_BLND double primary_value(double, SPAinterval);

// Bring SPAparameter of periodic curve to its primary range with special
// action at ends of range.

DECL_BLND double 
bring_to_range(
	double param,
	curve const *cur,
	logical const &favour_low = 
		* (logical const *) TRUE
    );

// Decide if the given point lies on the edge, within its bounds. Gets
// called only when the edge is not closed periodic. Will be neatened in
// due course. (Note edge_cur has the same sense as the edge.)

logical 
pos_on_edge( 
		EDGE *edge, 
		curve const *edge_cur,
		SPAposition const &pos, 
		SPAparameter &edge_param
		);

// Find a curves of intersection of surf_a and surf_b that pass through pos1 and
// pos2, and reverse those whose tangent at pos_1 has an opposite direction to
// (pos_2 - pos_1). If more than one curve remains and near_pos is given, choose
// the curve whose mid point between pos1 and pos2 lies closest to near_pos.

DECL_BLND surf_surf_int *
cur_from_sur_sur_thru_pnt(
	surface const *,	// surf_a
	surface const *,	// surf_b
	SPAposition const &,	// thru pos1
	SPAposition const &,	// thru_pos2
	SPAposition const &,	// near_pos
	SPAbox ,
	SPAparameter &,
	SPAparameter &
    );

// Find a curve of intersection of surf_a and surf_b that pass through thru_pos
// and intersects curve "cur", and nearest to near_pos. Reverse intersection if
// tangent at thru_pos has an opposite direction to (int_pos - thru_pos). If
// more than one intersection curves are found, select one such that distance
// between point of intersection and given position is minimum.

DECL_BLND surf_surf_int *
cur_from_sur_sur_thru_pnt_crv(
    surface const *surf_a,       // surface to intersect
	surface const *surf_b,       // surface to intersect
	SPAposition const &thru_pos, // point through which intersection should pass
	curve const *cur,		 // curve through which intersection should pass  
	SPAinterval const &intr,     // range for the coedge. 
	SPAposition const &near_pos, // find intersection such that int_pos is
								 // nearest to this position. 
	SPAbox const &bb_box,        // region of intersest
	SPAposition  &int_pos,       // point of intersection.
	SPAparameter &int_par,		 // parameter on "cur" at point of intersection. 
	SPAparameter &par_thru,      // parameter on interesection curve
								 // corresponding to thru_pos 
	SPAparameter &par_int,       // parameter on intersection curve
								 // corresponding to int_pos 
	double tol
	);

// Function used by mitring with partial side capping code. 
// In situations where the  two blend sheet coedges on one side of the vertex being 
// mitred do not coincide at the intervening edge, Given 
// a) the furthest and closest blend sheet coedges at one side of the vertex being mitred
// b) the blend surface corresponding to the furthest blend sheet coedge
// c) the surface we should cap against
// d) an approximate point of intersection of the capping curve and the closest sheet
//    coedge
// find the capping curve starting at the point of intersection between the furthest
// sheet coedge and the intervening blank edge and ending on the closest sheet coedge. 

DECL_BLND surf_surf_int *
bl_mitre_find_sidecap_curve(
    surface const *furthest_blend_surf,// surface to intersect
	surface const *closest_cap_surf,   // surface to intersect
	COEDGE  *furthest_sheet_coedge,    // coedge furthest from the mitred vertex when traveling 
	                                   // along the intervening edge
	COEDGE  *closest_sheet_coedge,     // coedge closest from the mitred vertex when traveling 
	                                   // along the intervening edge
	SPAposition const &furthest_pos,   // point through which intersection should pass
	SPAbox const &bb_box,              // region of intersest
    logical int_pos_approximate,       // whether the supplied intersection between closest
                                       // sheet coedge and furthest blend surface is approximate
	SPAposition  &int_pos,             // approx. point of intersection, updated to the actual
	                                   // intersection point by this function
	SPAparameter &int_par,		       // parameter on closest sheet coedge at point of intersection
	                                   // ( note this is coedge parameter and NOT curve parameter )
	SPAparameter &par_thru,            // parameter on interesection curve
								       // corresponding to thru_pos 
	SPAparameter &par_int,             // parameter on intersection curve
								       // corresponding to int_pos 
	double tol                         // tolerance for accepting/rejecting the intersection curve
	);

DECL_BLND void  bl_reverse_ssi( surf_surf_int *ss_int );

// Return true if any two adjacent edges at the vertex meet in a cusp.

DECL_BLND logical cuspate(VERTEX *);
DECL_BLND logical vertex_cuspate(VERTEX *);



DECL_BLND void del_ffi(face_face_int *);

// Complete end geometry of sheet by setting a SPAparameter curve to join
// the known ends.

DECL_BLND logical 
do_sheet_open_end(
	segend *,
	logical,
	ATTRIB_FFBLEND *,
	COEDGE *,
	COEDGE *
    );

DECL_BLND SPAposition 
pos_from_segend( segend * );

DECL_BLND SPAunit_vector 
dir_from_segend(
	segend *,
	logical,	// true for start
	logical		// true if convex
    );

DECL_BLND plane 
def_plane_from_segend(
	segend *,
	logical,	// true for start
	logical		// true if convex
    );

DECL_BLND logical cross_vertex(segend *);

// Find all segments for blend geometry and blend faces.

DECL_BLND ffsegment * 
edge_ffblend_segments(
	ffblend_geom *,
	ATTRIB_FFBLEND*,
	BODY *sheet_body,
	segend *start_mate = NULL,
	segend *end_mate = NULL,
	logical open_at_start = FALSE,	
	logical open_at_end = FALSE,	
	logical univex_at_start = FALSE,
	logical univex_at_end = FALSE,	
	logical simple_at_start = FALSE,
	logical simple_at_end = FALSE,	
    logical mitre_at_start = FALSE,	
    logical mitre_at_end = FALSE,	
	blend_seq *seq = NULL,	// for vertex bi-blends, the sequence should be passed
							// (only used by the new code)
	logical called_from_fill_gap = FALSE
    );

DECL_BLND segend *
ext_segend_of_eval_bl_edge(
	ATTRIB_FFBLEND *,	// blend attribute of evaluated blended edge
	COEDGE *,		// use start of coedge of the evaluated blended edge
	logical = TRUE	// true if the evaluated blended edge has same
					// the sense as the edge for which the segend is
					// sought (in the edge-edge case); ignored in
					// the vertex-edge case
    );

segend *
segend_for_uneval_bl_edgevert(
	ATTRIB_VBLEND *,		// blend attribute of unevaluated blended vertex
	ENTITY_LIST const &,	// list of atts of ents being evald
	COEDGE *,				// start of coedge denotes which end of the edge
	logical					// true if result wanted as viewed from the edge;
							// false if as viewed from the vertex
    );


DECL_BLND FACE *face_from_segend(segend *, logical);	// logical true for left

DECL_BLND logical 
fill_gap_in_chain(
	ffsegment *seg,
	ATTRIB_FFBLEND *att,
	BODY *sheet_body,
 	ENTITY_LIST const & = *(ENTITY_LIST *)NULL_REF	// temp default
    );

// Return given coedge if its start vertex is a bi-blend (or a coedge
// beyond if there are intervening prop bi-blend vertices (connected
// only to prop edges and blended edges); otherwise return null. Note
// all but one of a sequence of blended edges joined by prop bi-blends
// for later deletion. From ACISR15, the smooth edge count etc. is
// returned for the end vertex of the prop sequence and not the end of
// the input coedge.

DECL_BLND logical
find_bi_blend(
	    COEDGE *start_coedge, 			// inspect vertex at start of coedge
	    int &num_smooth_eds_at_start,	// number of smooth edges at vertex returned
	    EDGE *&smooth_ed_at_start,		// first smooth edge found
	    ENTITY_LIST	&susp_atts, 		// edges to be deleted
        COEDGE *&new_start_coedge       // actual start coedge, if prop bi-blend
        );

// Function to check the start of the supplied coedge to see if
// there is a smooth edge joining it, and if so, to make up a
// bi-blend attribute at the vertex.

DECL_BLND void 
make_single_bi_blend( COEDGE *bl_coedge );

// Check any suspended attributes that meet the start of the supplied
// coedge with tangent continuity to find out if the suspension should
// be lifted.

DECL_BLND void 
unsuspend_bl_atts(
	COEDGE *bl_coedge, 		// Coedge to search from start of
	ENTITY_LIST const &ents	// list of bl atts of ents being evald
    );

// This function should only be called for a blended edge that has
// a degenerate sphere for its blend geometry. Check for next or
// previous smooth edges along the blend sequence from the supplied
// edge, and if found, check if these blended edges share the same
// support face as the spherical blend, on its non-degenerate side.
// If there is exactly one such edge found, return TRUE. Otherwise,
// return FALSE. This function was initially set to perform more
// testing than this, as it used to be the fix to bugs r8200 and
// r7376, but this fix broke other blends, and so I have now found
// a better fix to these bugs.

DECL_BLND logical 
check_sphere_next_attrib(
    EDGE *bl_edge,		// Edge to search either side of
    ffblend_geom *ffbl_geom
    );

// Given a variable radius blend attribute, find an entire sequence
// of smoothly connected variable radius blends.
// The direction of the list will be the same as that of given attribute.
// This also returns the directions of the first and last blends
// relative to the given blend, for convenience.
// Also for convenience, it returns a flag indicating whether the entire
// smooth sequence consists of variable radius blends; i.e. whether or not
// the sequence starts or ends at a const-rad blend.

DECL_BLND void 
find_var_blend_seq(
    ATTRIB_VAR_BLEND *,	// var_blend_att
    ENTITY_LIST &,		// sequence of var-blend attributes returned
    logical &,			// first_fwd
    logical &,			// last_fwd
    logical &			// all_vars
    );

// Find the body edge which trims an open spring curve, from the
// appropriate intercept record. Easy if the intercept refers directly
// to an edge, a bit more effort if it refers to a face (as happens
// if the spring curve degenerates to a point). Returns the SPAparameter
// value along the edge as well as the edge itself.

DECL_BLND void 
find_spring_edge(
	intercept const &,
	logical,
	VERTEX *,
	EDGE *&,
	double &
    );

DECL_BLND surf_surf_int *
find_cross_curve(
	surface const&,
	surface const&,
	SPAposition const&,
	SPAposition const&,
	SPAposition const&,
	SPAbox const & = *(SPAbox *)NULL_REF
    );

// Finds details of the mating configuration at the start of
// an unevaluated blended coedge returning the result in a segend.
// Left and right of the segend returned are oriented correctly for
// an unevaluated edge leaving the common vertex.

DECL_BLND segend *
find_edge_mate(
	COEDGE *,
	ENTITY_LIST const &,
	logical = FALSE			// ignore unevaluated biblend at end
    );

DECL_BLND SPAposition find_lateral_point(logical, curve const&, SPAparameter);

DECL_BLND logical 
find_cuspate_incpt(
	ATTRIB_VBLEND *,		// vertex_blend_att
	ENTITY_LIST const &,	// ents
	COEDGE *,				// this_coedge
	logical,				// cw
	intercept &,			// incpt_found
	FACE *&					// other face
    );

// Find mating topology or geometry for one or two unblended edges at a
// blended cuspate vertex, obtaining a segend as result

DECL_BLND segend *
find_cuspate_vertex_mate(
	ATTRIB_VBLEND *,	// blend attribute of vertex
	ENTITY_LIST const &,
	COEDGE *,			// coedge leaving vertex
	COEDGE * = NULL		// further unblended coedge leaving vertex
    );

// Find mating topology or geometry for a blended edge at a blended
// vertex, obtaining a segend as result.

DECL_BLND segend *
find_vertex_mate(
	ATTRIB_VBLEND *,	// blend attribute of vertex
	ENTITY_LIST const &,
	COEDGE *			// coedge leaving vertex
    );

// Find the blend attributes of the two sequences coming into a bi-blend
// or mitre vertex.

DECL_BLND void 
find_mitre_atts( 
    VERTEX *bibl_v, 
    ENTITY_LIST const &all_atts,
	COEDGE **this_c, 
    COEDGE **other_c, 
    ATTRIB_FFBLEND **this_att, 
    ATTRIB_FFBLEND **other_att 
    );

// Form mitre at vertex where two near-tangent blended edges meet.
// It is assumed that the two blend faces given for mitring will have
// cross curves at there ends.

DECL_BLND void 
form_fuzzy_biblend_mitre(
	VERTEX *bl_vertex,
	ENTITY_LIST &del_faces
    );

// Get already-made blend geometry for a blended edge,
// or face pair.

DECL_BLND ffblend_geom *
get_ffblend_geometry( ATTRIB_FFBLEND * );

// Get already-made blend geometry for a blended vertex.

DECL_BLND vblend_geom *
get_vblend_geometry( ATTRIB_VBLEND * );

// Create a defining curve, radius functions, etc, in a sequence of blends.
// Intended to be part of initialise_blend_one().

DECL_BLND void blend_pre_process_seq( blend_seq * );

// Check whether two coedges are coincident.

logical
coinc( COEDGE *this_coed, COEDGE *that_coed );

// Join sheet faces that coincide in an edge.

DECL_BLND logical 
join_sheet_faces(
	segend *,			// start_mate
	segend *,			// end_mate
	ENTITY_LIST &		// faces for later deletion
    );

// As above, but called when the two faces to be joined are in
// fact portions of the same sphere. The faces may have been
// constructed so as to overlap and need special care.

DECL_BLND logical 
join_sheet_faces_on_sphere(
	segend *,
	segend *,
	ENTITY_LIST &
    );

DECL_BLND segend * 
look_ahead_for_mate(
	ENTITY *,
	ENTITY *,
	segend *,
	ffsegment *,
	ATTRIB_FFBLEND *,
	logical const & = *(logical const *)NULL_REF
    );

// Joins coedges from two sheet faces.

DECL_BLND logical 
join_sheet_faces_w_coeds(
	COEDGE *,
	COEDGE *,
	ENTITY_LIST &
    );


// Attempts to re-set the edge pointed to by the passed vertex.

void set_new_edge_for_vertex( VERTEX* , EDGE* );

// Removes a coedge from the loop, assuming this can
// be done consistently.

void remove_coedge_from_loop( COEDGE* , ENTITY_LIST* del_faces = NULL );

// STI let (01/01): Change the enum name from param_info to bl_param_info
// to prevent a possible conflict with the param_info class in intr.

enum bl_param_info{
	param_increase,
	param_decrease,
	param_closest,
	param_info_unknown
	};

DECL_BLND curve *
make_curve_on_surface(
	SPAposition	const &pos_a,
	SPAposition	const &pos_b,
	surface	const *this_surf,
	SPAvector const &pos_a_dir = 
        * (SPAvector const *) NULL_REF,	// tangent at pos_a
	SPAvector const &pos_b_dir = 
        * (SPAvector const *) NULL_REF,	// tangent at pos_b
	SPApar_pos const &pp_a_guess = 
        * (SPApar_pos const *) NULL_REF,	
	SPApar_pos const &pp_b_guess = 
        * (SPApar_pos const *) NULL_REF,	
	surface	*&aux_surf = 
        * (surface **) NULL_REF,		
	pcurve *&pcur_made = 
        * (pcurve **) NULL_REF,	
    bl_param_info u_param_info = 
        param_info_unknown,		// u param info along curve
    bl_param_info v_param_info = 
        param_info_unknown		// v param info along curve
    );

DECL_BLND curve * 
make_par_int_curve(
	SPAposition	const &pos_a,
	SPAposition	const &pos_b,
	surface	const *this_surf,
	SPAvector const &pos_a_dir = 
        * (SPAvector const *) NULL_REF,	// tangent at pos_a
	SPAvector const &pos_b_dir = 
        * (SPAvector const *) NULL_REF,	// tangent at pos_b
	SPApar_pos const &pp_a_guess = 
        * (SPApar_pos const *) NULL_REF,	
	SPApar_pos const &pp_b_guess = 
        * (SPApar_pos const *) NULL_REF,	
	surface	*&aux_surf = 
        * (surface **) NULL_REF,		
	pcurve *&pcur_made = 
        * (pcurve **) NULL_REF,	
    bl_param_info u_param_info = 
        param_info_unknown,		// u param info along curve
    bl_param_info v_param_info = 
        param_info_unknown		// v param info along curve
    );

DECL_BLND void 
make_sheet_faces_for_vertex( 
    VERTEX *, 
    BODY *, 
    blend1_data & 
    );

DECL_BLND void 
make_cross_pcurve(
    segend *, 
    ffblend_geom *, 
    logical = TRUE
    );

DECL_BLND ffblend_geom * 
make_ffblend_geometry(
	ATTRIB_FFBLEND *,	// ffblend_att
	logical,			// open_at_start
	logical,			// open_at_end
	segend * = NULL,	// start segend if any
	segend * = NULL,	// end segend if any
    double & = *(double*)NULL_REF // reduced_resabs
    );

// Create the blend geometry for a non-simple constant radius blend,
// using an rb_blend_spl_sur or srf_srf_v_bl_spl_sur.

ffblend_geom * 
make_procedural_blend_geom(
	    ATTRIB_FFBLEND const *att,
		double left_offset,
		double right_offset,
		var_cross_section const &cs,
	    ffblend_geom *ffbl_geom,	// might contain analytic spine/spring crvs.
	    logical open_at_start,		
	    logical open_at_end,		
	    segend *start_segend,
	    segend *end_segend,
	    SPAbox const &region		
        );

// Older version of the above function, only used for 
// constant round blends.

ffblend_geom *
make_rb_blend_geom_old(
		ATTRIB_CONST_ROUND *att,
		logical start,
		logical end,
		segend *start_seg,
		segend *end_seg,
		ffblend_geom *spine = NULL	// spine if available
		);

// The final logical gets set to TRUE if the call processes *both* blend
// faces *and* stitches them up.

DECL_BLND logical 
mitre_sheet_end(
    segend *, 
    segend *, 
    logical &
    );

DECL_BLND int 
move_bl_attrib_to_list_end(
	ATTRIB_BLEND *blend_att,		
	graph_impl_atts &imp_atts,		
	ENTITY *old_owner = NULL		
    );

// Given a mitred vertex and the blend attributes of one of the incident
// edges being blended, find the blend face ending/beginning at the vertex's
// end.

DECL_BLND FACE * 
find_sheet_face_for_mitre( 
    ATTRIB_FFBLEND *bl_edge_att,
	VERTEX * mitre_vtx 
    );

DECL_BLND int num_edges_at_vertex(VERTEX*);

// Return true if end of given blended edge is connected to no other
// blended edge.

DECL_BLND logical open_extrem(EDGE*, double, extremity, ENTITY_LIST const &);

// The optional argument pos_off_edge is set to TRUE if an
// intersection was found but it is NOT on the edge (in contrast to the
// routine as a whole which returns TRUE only if the intersection found
// IS on the edge). Note that if pos_off_edge is returned TRUE, then
// cur_param and this_pos are both set properly. DAP 12/7/95.

DECL_BLND logical
pos_from_edge_sur(
	EDGE *this_edge,
	surface const *this_surf,
	SPAbox const &bb_box,
	SPAposition const &near_pos,
	SPAparameter &cur_param,
	SPAposition &this_pos,
	logical &pos_off_edge 
        = *(logical *) NULL_REF	
	);

// Process left and right intercept lists.

DECL_BLND void process_ffints
(
	EDGE			*,	// bl_edge
	COEDGE			*,	// fwd_coedge
	COEDGE			*,	// bwd_coedge
	FACE			*,	// left_face
	FACE			*,	// right_face
	ATTRIB_FFBLEND	*,	// ffbl_att
	segend			*,	// start_segend
	segend			*,	// end_segend
	logical,			// open_at_start
	logical,			// open_at_end
	logical,			// univex_at_start
	logical,			// univex_at_end
	logical,			// simple_at_start
	logical,			// simple_at_end
	logical,			// mitre_at_start
	logical,			// mitre_at_end
	logical,			// var_rad_blend
	SPAinterval,			// var_rad_range
	curve const		*,	// left_spring_curve
	curve const		*,	// right_spring_curve
	face_face_int	*&,	// left_ff_int
	face_face_int	*&	// right_ff_int
);

DECL_BLND logical process_open_end
(
	COEDGE *,		// sheet coedge (on interior side of sheet face)
					// giving start of capped portion of sheet boundary
	COEDGE *,		// sheet coedge (on interior side of sheet face)
					// giving end of capped portion of sheet boundary
	ffsegment *,	// descriptor of this end of the blend sheet
	logical,		// TRUE if at start of segment, FALSE for end
	bl_ed_convexity,
	logical & = *(logical *)NULL_REF,	// returned as true if 1st coedge deleted
	logical & = *(logical *)NULL_REF	// returned as true if 2nd coedge deleted
);

DECL_BLND void process_univex_open_end(
	COEDGE *,		// sheet coedge at open, univex end, currently
					// attached to an edge with no geometry
	FACE *,
	FACE *,
	ffsegment *,	// descriptor of this end of the blend sheet
	logical			// TRUE if at start of segment, FALSE for end
);

DECL_BLND logical reflex		// reflex if first_end_dir * second_start_dir % nor < 0
(
	COEDGE *,	// first coedge
	COEDGE *,	// second coedge
	logical	= TRUE	// if true, find normal at end of first coedge,
					// otherwise at start of second coedge
);

// Compare the end positions of two segends and test if their end
// positions match up. For the moment we make this test identical
// to that used in replace_segend.

DECL_BLND logical segends_match(
						const segend *segend1,
						const segend *segend2
						);

// Delete segend given by old_segend, then make it refer to new_segend.
// This routine will test geometric consistency if asked and can copy
// back in-face details from segend being replaced if needed.  At present
// it is always called with testing on and copy-back off.

DECL_BLND void replace_segend(
	segend *&,		// new_segend - will be left as null if replace occurs
	segend *&,		// old_segend
	logical = TRUE,	// test
	logical = FALSE	// copy_back
);

DECL_BLND logical same_dir(SPAunit_vector, SPAunit_vector, double cvtr = 0);

DECL_BLND logical same_dir(COEDGE*, COEDGE*);

// Return true if the vertex lies on the boundary of a sheet i.e. if one or
// more of its coedges have null partners.

DECL_BLND logical sheet_v(VERTEX *);

// True if end of edge given by start of coedge is connected smoothly
// to just one edge.

DECL_BLND logical simple( COEDGE *, ENTITY_LIST const & );

DECL_BLND logical smooth_intercept(intercept const &);

// Bring a chain of face-face-ints to standard form.

DECL_BLND face_face_int *standardise_ffi_list
(
	face_face_int *&,
	curve const *,
	COEDGE * = NULL
);

// Find segment for blend geometry and mating sheet faces at a blended
// vertex.

DECL_BLND vsegment *
vertex_blend_vsegment(
    ATTRIB_VBLEND *,
	segend **segs = NULL, 
	int &num_segs = *(int*)NULL_REF
    );

DECL_BLND logical vertex_smooth(VERTEX *this_vertex);

DECL_BLND logical 
vertex_univex( 
        VERTEX *this_vertex, 
        bl_ed_convexity &vtx_cvty =
            * ( bl_ed_convexity * ) NULL_REF
        );

DECL_BLND logical zero_width_segend(segend *);

// Find the blend face created for the edge at the end specified by the mitre_vtx
// If in addition the entire list of attributes is specified, searches for the 
// faces created by entity-entity blends.

FACE *
bl_find_blend_face_at_end( 
	ATTRIB_FFBLEND *bl_edge_att,
	COEDGE *mitre_vtx,
	ENTITY_LIST const &att_list =
        * ( ENTITY_LIST * ) NULL_REF,
	logical partial_ok = FALSE // whether end face created by edge blending is sufficient
	                           // for cases where entity-entity blends are created at ends
	);

// Find the cross edge at the end (specified by mitre_vtx ) of this 
// evaluated blend attributed 

ATT_BL_SEG *
bl_find_cross_at_end( 
	ATTRIB_FFBLEND *att, 
	COEDGE *mitre_vtx,
    logical partial_ok = FALSE, // whether end face created by edge blending is sufficient
                                // for cases where entity-entity blends are created at ends
    ENTITY_LIST const &att_list =
        * ( ENTITY_LIST * ) NULL_REF
	);

// Find the support entities of the given blend entities, this is needed as the blend
// faces created when processing an edge can have different supports.

logical 
bl_find_blend_supports(
    FACE *f,
	support_entity *&left_support,
	support_entity *&right_support,
    logical &same_sense =
        * (logical *) NULL_REF
	);

// Check if the given sequence contains variable radius blends, in addition checks
// whether the sequence is entirely constant / variable type.

logical
var_blend_sequence(
	blend_seq *seq,
	logical &all_const = 
        * ( logical * ) NULL_REF,
	logical &all_var = 
        * ( logical * ) NULL_REF
	);

// Check if an entity-entity attribute was created to process this edge and 
// return it if requested.

logical 
abl_entity_entity_switched( 
		 blend_edge *bl_edge,   // Edge being blended
         ATT_BL_ENT_ENT *&ent_att =
            * ( ATT_BL_ENT_ENT ** ) NULL_REF
		 );

// Find the cross coedge corresponding to the segend.

ATT_BL_SEG *
bl_cross_from_segend( segend *this_seg, ffsegment *this_ff_seg );

logical 
bl_extend_cu_to_box( 
    curve &cu, 
    SPAbox const &bx, 
    logical start 
    );

// Classify points on a spring curve with respect to a face on which
// the curve lies.

enum spring_int_rel
{
	si_unknown,
	si_in_face,		// point on spring curve lies within face
	si_on_edge,		// spring curve cuts but is not along an edge
	si_at_vertex,	// spring curve enters/leaves face thru a vertex
	si_along_edge,	// spring curve lies along an edge of the face
	si_out_face		// spring curve lies outside the face. 
};

// Class to hold details of a point on a spring curve lying in a face.

class DECL_BLND intercept : public ACIS_OBJECT
{
public:

    // This flag is relevant for intercepts corresponding to external 
    // faces only. 
    // Tells us whether the spring grazes the support face boundary
    // and comes back into the face and so the blend will not rollon
    // to a new entity. 
    // NOTE: The "si_ent" will point to the internal coedge in this 
    // case.

    logical         internal;       

	spring_int_rel	si_rel;			// relation of point to bb face
	ENTITY			*si_ent;		// bb face or edge|vertex coed
	SPAparameter	edge_par;		// if on edge, edge SPAparameter
	SPApar_pos		face_pars;		// if on spline face, face params.
	SPAposition		pos;			// SPAposition of point
	SPAunit_vector	dir;			// spring curve direction at pos
	COEDGE			*sheet_vertex;	// matching corner of sheet face
									// (start vertex of coedge)
    double          incpt_tol;      // sheet vertex tolerance.
    logical         face_pars_set;
    logical         edge_par_set;

	// Constructors.

	intercept();

	intercept(
		SPAposition const &,
		FACE* = NULL,
		SPAunit_vector const & = null_unitvec,
		SPApar_pos const & = 
            * (const class SPApar_pos *) NULL_REF,
		logical is_in_face = TRUE,	// Whether the point is inside the face.
        double tol = SPAresabs
	  );

	intercept( 
		face_face_int *ffi, 
		logical entering, 
		logical fill_external = FALSE, // whether the external entities
		                               // should be filled
        double tol = SPAresabs
		);

	intercept(
		COEDGE*,
		SPAposition const &,
		SPAparameter const &,
		SPAunit_vector const & = null_unitvec,
		logical fill_external = FALSE, // whether the external entities
		                               // should be filled
        double tol = SPAresabs
	  );

    intercept &copy_blank_info( intercept const &rhs );

	// Nascent tolerant modelling support: return TRUE if sitting on a
	// tolerant blank entity, e.g. a TVERTEX or a TEDGE.

	logical tolerant() const;

	// The tolerance of the blend sheet vertex at this point.

    double intercept_tol() const;

	// Whether the given face is a lateral face of the intercept. If the
	// intercept is at a boundary, considers all the faces sharing that
	// boundary.

	logical is_lateral_face( FACE *lat_f ) const;

	// Whether the other intercept (found by the adjacent blend at a mitre
	// or bi-blend) is on the shared boundary between two supports on the 
	// same side of the blends.

	logical adjacent( 
				intercept const &other,
				logical acwise,             // whether to search acwise.   
				VERTEX *blank_v = NULL,     // end of the sequence.
				logical &this_far =			// Whether this incpt is farther
					* (logical *) NULL_REF  // away from the blank vertex.				                        
				) const;

	// Whether the intercepts from two adjacents blends intersect at the
	// same point on the boundary.

	logical coincident( intercept const &other ) const;

	// Whether this intercept lies on the boundary of a sheet body being
	// blended.

	logical on_sheet_boundary() const;

	// Whether this intercept is at a bi-blend junction.

	logical at_bi_blend( SPAposition const &spine_pos ) const;

	// Print out an intercept.

	void debug( FILE * = debug_file_ptr ) const;
};

// Class to hold details of the side of a segment.  It is used to
// record points where a spring curve crosses non-smooth edges.
// If cur_null is true, means that curve sequence is curve-null-curve ..
// where changes occur at given intercepts; if false, sequence is
// null-curve-null etc.

class DECL_BLND segside : public ACIS_OBJECT
{
public:

	intercept *incpt_array;
	logical	cur_null;

	// Constructors.

	segside();	// construct an empty segside

	// Construct a segside for an array of n intercepts.

	segside(int, intercept *);

	// Destructor.

	~segside();

	int num_incpts() const { return n_incpts; }

	// Print out a segside.

	void debug( FILE * = debug_file_ptr ) const;

private:
	
	int	n_incpts;
};


// Class to hold details of an end of a segment.  This can be pure
// geometry when the neighbouring sheet face has not yet been
// constructed, or can be a reference to the (external) coedge of
// the adjacent sheet.  The spring curve entries are only used for
// a blended vertex.  Left and right are defined with respect to the
// blended edge.  rem_cross_cur is set to true (findmate.cxx) if the
// segend edge could be the edge at a blended 3-edge vertex (if univex,
// the edge with biggest blend or if mixed, the edge with unique
// convexity).

class DECL_BLND segend : public ACIS_OBJECT
{
public:
	segend			*next;
	COEDGE			*cross_sh_coed;		// matching side of sheet face
	intercept		left_incpt;
	intercept		right_incpt;
	SPAparameter	spine_par;
	SPAposition		spine_pos;
	SPAposition     ref_pos;
	SPAunit_vector	spine_dir;
	curve			*cross_cur;
	pcurve			*cross_pcur;		// for blend body surface beyond
	pcurve			*vbl_cross_pcur;
	ffblend_geom	*cross_ffbl_geom;
	curve			*spring_cur;
	pcurve			*spring_pcur;		// for blend body surface beyond
	pcurve			*vbl_spring_pcur;
	surface			*spring_aux_surf;
	VERTEX			*blend_vertex;
	EDGE			*blend_edge;
	logical			spine_info_set;
	logical			rem_cross_cur;

	// Constructors.

	segend();	// construct an empty segend

	segend(
		SPAparameter const &spine_par,
		SPAposition const &spine_pos,
		SPAposition const &ref_pos,
		SPAunit_vector const &spine_dir,
		intercept const &left_i,
		intercept const &right_i,
		curve *cross_cur,
		EDGE *blend_edge = NULL,
		VERTEX *blend_vertex = NULL	
		);

	// copy constructor
	segend(const segend& se);

	// Destructor.

	~segend();

	// Utilities

	logical is_external( void ) const;
	logical is_internal( void ) const { return ! is_external(); }

	// Retrieve spine info.

	logical update_spine_info( 
						logical convex,
						logical start,
						ffsegment *seg = NULL
						);

	logical update_spine_info(
						ATT_BL_SEG *seg_att,
						logical start
						);

	SPAposition spine_point( void ) const;
	SPAunit_vector spine_direction( void ) const;

	SPAposition ref_point( void ) const;

	double spine_param( void ) const;

	// Retrieve the left or right contact positions.

	SPAposition left_pos( logical at_start ) const;
	SPAposition right_pos( logical at_start ) const;

	// Retrieve the left or right offset information. Returns -1.
	// if the offset could not be calculated. Note that this approach
	// doesn't give correct results for ruled chamfers as the spine 
	// curve is not setup properly.

	double left_offset( logical at_start, logical convex ) const;
	double right_offset( logical at_start, logical convex ) const;

	// Set face_pars, pos and dir in right_incpt.
	// incpt.dir is set to the base surface normal.

	logical set_left_incpt( logical at_start, surface const &base_srf );
	logical set_right_incpt( logical at_start, surface const &base_srf );

	// Nascent tolerant modelling support: return TRUE if either
	// intercept is at a "tolerant" location on the blank, e.g. on a
	// TVERTEX or TEDGE.

	logical tolerant() const;

    // Check if this segend corresponds to a pointy end of a blend face.
    
    logical pointy_end() const;

	// Print out a segend.

	void debug( FILE * = debug_file_ptr ) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

	// Returns whether this segend is at bi-blend junction i.e. intercepts on 
	// smooth edges.

	logical at_bi_blend() const;

	// Returns whether any of the lateral edges if present are near tangent

	logical near_tangent_laterals(logical at_start = TRUE) const;

protected:

	logical set_incpt(surface const &base_srf, intercept &incpt, 
                SPAposition const &segend_pos);
};



// Two useful utilities for testing whether segends needs to be joined
// together.

// Test whether 2 segends are "precisely" coincident, i.e. intercepts
// are within SPAresabs or less of one another. Optional SPAparameter
// resolution allows an additional test on spine parameters if these are
// known (for historical reasons only). Return TRUE if the segends
// match up.

DECL_BLND logical 
segends_coincident(
            segend *this_seg,
            segend *other_seg,
            ffsegment *this_ff_seg = NULL,
            ffsegment *other_ff_seg = NULL,
            double respar = -1
            );

// If 2 segends did *not* pass the "precisely" coincident test, look to
// see whether they have intercepted near-tangent (and maybe tolerant)
// edges and must therefore be regarded as meeting even though the
// tolerance locally will end up being more than SPAresabs. Return TRUE if
// this is so. (Note: returns FALSE for "precisely" coincident segends.)

DECL_BLND logical 
segends_near_tangent(
            segend *this_end,
            segend *other_end,
            ffsegment *this_seg = NULL,
            ffsegment *other_seg = NULL
            );

// Blend move_sheet_corner_actual Status Condition Recommend Split.

#define BLD_MSCA_STATUS_COND_RECMD_SPLIT 3

// Blend move_sheet_corner_actual Status Condition Edge Deleted
 
#define BLD_MSCA_STATUS_COND_EDGE_DEL 4 

// Performs the actual moving of the sheet corner.

enum move_corner_result 
    { 
    move_corner_failed, 
    move_corner_succeeded, 
    move_corner_edge_deleted, 
    move_corner_unset 
    };

DECL_BLND move_corner_result
move_sheet_corner_actual(
			EDGE *spring_edge,
			VERTEX *this_vertex,
			VERTEX *other_spring_vertex,
			COEDGE *spring_coedge,
			SPAposition const &new_pos,		// new SPAposition
			int &status_cond,
			void **status_parm
			);

// Class that records the portion of a blend where the ball touches
// two faces.

class DECL_BLND ffsegment : public ACIS_OBJECT
{
public:
	ffsegment		*next;
	ffsegment		*previous;
	ATTRIB_FFBLEND	*ffbl_attrib;	// blend from which segment derives
	FACE			*left_face;
	FACE			*right_face;
	ffblend_geom	*ffbl_geom;
	COEDGE			*left_coed;		// only set if a spring curve lies
	COEDGE			*right_coed;	// on a blend body edge
	segend			*start;			// start of segment
	segend			*end;			// end of segment
	segside			*left_segside;
	segside			*right_segside;
	FACE			*sheet_face;	// corresponding sheet face

	// Constructor.

	ffsegment(
		ffsegment*,			// prev seg
		ATTRIB_FFBLEND*,	// blend from which segment derives
		FACE *,				// left face
		FACE *,				// right face
		ffblend_geom*,		// blend surf, spring curs, aux surfs
		segend *,			// start segend
		segend *			// end segend
	);

	// Destructor.

	~ffsegment();

	// Print out an ffsegment.

	void debug( FILE * = debug_file_ptr ) const;
};

DECL_BLND void debug_ffseg_chain(ffsegment *, FILE * = debug_file_ptr);

// Class that records a vertex blend.

class DECL_BLND vsegment : public ACIS_OBJECT
{
public:
	ATTRIB_VBLEND	*vbl_attrib;	// blend from which vsegment derives
	vblend_geom		*vbl_geom;
	segend			*segend_chain;	// chain counterclockwise round vtx
	FACE			*sheet_face;	// corresponding sheet face

	// Constructors.

	vsegment(
		ATTRIB_VBLEND*,		// blend from which segment derives
		vblend_geom*,		// blend surf, spring curs, aux surfs
		segend*				// start of chain of segends
	);

	// Destructor.  Deletes chain of segends.

	~vsegment();

	// Print out a vsegment.

	void debug( FILE * = debug_file_ptr ) const;
};



// Class to record an SPAinterval of a curve.  Defined so that extra
// complications caused by closed curves and by testing against
// respar, a change in SPAparameter equivalent to SPAresabs, can be localised.
// Note that the SPAinterval always runs forward along the curve from
// the SPAinterval low to high, but that the SPAparameter value at low
// may be less than that at high if the curve is closed and the
// wrap-around point is contained within the SPAinterval.
// Allows intervals to be unbounded at the start (ci_end_only) or
// unbounded at the end (ci_start_only).
// [The set of functions are just those needed now.]

enum ci_type {ci_finite, ci_all, ci_null, ci_start_only, ci_end_only};

#define UNIVERSAL TRUE
#define EMPTY FALSE

class curve_interval;
DECL_BLND curve_interval operator&(curve_interval const &, curve_interval const &);
DECL_BLND logical operator&&(curve_interval const &, curve_interval const &);
DECL_BLND curve_interval operator-(curve_interval const & );
DECL_BLND logical same_curve( curve_interval const &, curve_interval const &);
DECL_BLND logical disjoint( curve_interval const &, curve_interval const & );


class DECL_BLND curve_interval : public ACIS_OBJECT
{
public:

	// Constructors

	curve_interval(
		SPAparameter const &low,
		SPAparameter const &high,
		curve const *cu
		);

	// Create an empty or universal curve interval.

	curve_interval(		
		logical universal,
		curve const *cu = NULL
		);

	// Create a interval bounded at one end only,

	curve_interval(		
		logical start,		// True if bounded at the start
		SPAparameter const &prm,
		curve const *cu = NULL
		);

	curve_interval(
		SPAinterval const &r,
		curve const *cu
		);

	// Negate an SPAinterval.

	friend DECL_BLND curve_interval operator-( curve_interval const & );

	// Return useful information.

	logical empty() const { return type == ci_null; }
	logical finite() const { return type == ci_finite; }
	logical universal() const { return type == ci_all; }
	logical start_unbounded() const { return type == ci_end_only; }
	logical end_unbounded() const { return type == ci_start_only; }
	logical start_bounded() const
		{ return type == ci_finite || type == ci_start_only; }
	logical end_bounded() const
		{ return type == ci_finite || type == ci_end_only; }
	curve const *base_curve() const { return curve_ptr; }

	friend DECL_BLND logical same_curve(
		curve_interval const &, curve_interval const &);
	logical small() const;

	SPAparameter start_pt() const;	// error if unbounded at start
	SPAparameter end_pt() const;	// error if unbounded at end

	// Set the ends of the interval.

	void set_start_pt( SPAparameter const &p );
	void set_end_pt( SPAparameter const &p );

	// These functions give an error if called for a non-finite
	// SPAinterval.

	SPAparameter mid_pt() const;
	double length() const;

	// Whether the overlap with the other interval is more than one
	// connected regions.

	logical multiple_overlap( curve_interval const & ) const;

	// Find the SPAinterval of overlap.  An error ensues if the given
	// curve intervals are not based on the same curve.

	friend DECL_BLND curve_interval operator&(
		curve_interval const &, curve_interval const &);

	// Determine whether two curve_intervals overlap.  We test with
	// respect to respar, to accommodate touching cases.

	friend DECL_BLND logical operator&&(
		curve_interval const &, curve_interval const &);

	// Return true if two intervals are disjoint i.e. do not overlap
	// or touch.

	friend DECL_BLND logical disjoint(
		curve_interval const &,
		curve_interval const &
	  );

	// Determine whether a point lies within a curve_interval.
	// Returns true if this SPAinterval is universal or if given value
	// lies within the SPAinterval (expanded by respar at each end).

	logical operator>>( double ) const;

	// Test whether a point is at the start or end of a curve_interval
	// to within respar.

	logical at_start( SPAparameter const &t ) const;
	logical at_end( SPAparameter const &t ) const;


	// Print out details of a curve_interval.

	void debug( FILE * = debug_file_ptr ) const;

private:

	SPAparameter low;
	SPAparameter high;
	ci_type type;
	curve const *curve_ptr;

};



#endif

