/* ORIGINAL: acis2.1/kernbool/blending/bl_enum.hxx */
/* $Id: bl_enum.hxx,v 1.7 2000/12/26 18:46:40 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Enumerated types used in blend functions and attributes. Extracted
// from bl_att.hxx so as to avoid having to include all the attribute
// definitions in api/routines.

#if !defined( BLEND_ENUM )
#define BLEND_ENUM

/**
* @file bl_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

// The base class contains a list of blend sheet faces constructed for
// the implicit blend, and a status indicator that notes whether the
// explicit blend geometry if any is known to agree with the implicit
// blend geometry if any.
// tbrv
/**
 * @nodoc
 */
enum blend_status {
	bl_stat_unset,
	bl_stat_agree,
	bl_stat_disagree
};

// This enumerated type encodes the convexity of an edge.
/**
 * Specifies blending convexity of an entity.
 * @param bl_ed_undefined_cvxty
 * undefined convexity.
 * @param bl_ed_convex
 * convex blend.
 * @param bl_ed_concave
 * concave.
 * @param bl_ed_convex_smooth
 * smooth convex.
 * @param bl_ed_concave_smooth
 * smooth concave.
 * @param bl_ed_smooth
 * smooth.
 * @param bl_ed_convex_cusp
 * convex cusp.
 * @param bl_ed_concave_cusp
 * concave cusp.
 * @param bl_ed_cusp
 * cusp.
 * @param bl_ed_convexity_unset
 * unset.
 **/
enum bl_ed_convexity {
	bl_ed_undefined_cvxty,
	bl_ed_convex,
	bl_ed_concave,
	bl_ed_convex_smooth,
	bl_ed_concave_smooth,
	bl_ed_smooth,
	bl_ed_convex_cusp,
	bl_ed_concave_cusp,
	bl_ed_cusp,
	bl_ed_convexity_unset
};

// This enumerated type encodes the continuity with which a blend face
// meets adjacent body faces.
/**
 * Specifies the continuity with which a blend face is to meet adjacent body faces.
 * <br><br>
 * @param unset_continuity
 * the continuity has not been specified.
 * @param position_continuous
 * G0 continuity.
 * @param slope_continuous
 * G1 continuity.
 * @param curvature_continuous
 * G2 continuity.
 **/
enum bl_continuity {
	unset_continuity,
	position_continuous,
	slope_continuous,
	curvature_continuous
};

// This enumerated type encodes blend behaviour when the rolling ball
// meets a smooth or non-smooth edge (of same or opposite convexity to
// the blend) at the start, end or middle of a blend sequence.

// Default behaviour (ACIS 1.7 and before) has been:
//
//	at the start or end of a smooth blend sequence,
// 		at a unary (one blended edge only) end, cap,
//		at a binary (two blended edges only) end, mitre,
//		at an n-ary (three or more blended edges) end, an error;
//
//	in the middle of a smooth blend sequence,
//		at a smooth edge, roll on,
//		at a non-smooth edge of same convexity as blend, cap,
//		at a non-smooth edge of opposite convexity, an error (blend too big).

// The new behaviour (post ACIS 1.7 with advanced blending) is:
//
//	at the start or end of the blend sequence,
//		as prescribed by the vertex blend attribute of the end vertex
//		(see ATTRIB_VBLEND), cap, roll-on or runout;
//
//	in the middle of a smooth blend sequence,
//		same default behaviour as ACIS 1.7 and before; or
//		as prescribed by the blend_how member for the "local" ffblend
//		attribute, roll on or cap (regardless of edge convexity or smoothness).
//

/**
 * Specifies blend behavior when the rolling ball meets a smooth or non-smooth edge at the start, middle or end of the blend sequence.
 * @param bl_how_default
 * follow default behavior.
 * @param bl_how_cap
 * cap or mitre regardless of edge convexity.
 * @param bl_how_roll_on
 * roll-on regardless of edge convexity.
 * @param bl_error
 * could be used to prevent a blend spreading too far.
 **/
enum blend_how {
	bl_how_default,		// follow default behaviour
	bl_how_cap,			// cap or mitre regardless of edge convexity
	bl_how_roll_on,		// roll-on regardless of edge convexity
	bl_error			// [could be used to prevent a blend spreading too far]
};

/*! @} */

// Class to select constant-SPAparameter direction.

/**
* Specifies the curve direction.
* @param blend_int_cur_u
* u parameter curve.
* @param blend_int_cur_v
* v parameter curve.
* @param blend_int_cur_unset
* unset.
* @param blend_int_cur_general
* neither a u or v parameter curve.
**/
enum blend_int_cur_dir {
	blend_int_cur_u,		// u SPAparameter line (v = constant)
	blend_int_cur_v,		// v SPAparameter line (u = constant)
	blend_int_cur_unset,
	blend_int_cur_general   // a general parameter curve
};


#endif

