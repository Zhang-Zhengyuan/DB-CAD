/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Enumerated types used for vertex blend attributes. Extracted
// from vbl_att.hxx so as to avoid having to include all the attribute
// definitions in api/routines.
#if !defined( VBLEND_ENUM )
#define VBLEND_ENUM
/**
 * @file vbl_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
// This enumerated type encodes blend behaviour at a vertex attached to one
// or more blended edges as follows:
//
//	bl_v_unset or
//	bl_v_blend		means a vertex blend
//	bl_v_bi_blend	used internally to mark an interior vertex of a blend
//					sequence, for which more blend sheet faces will be made
//	bl_v_cusp		notes that two blended edges (meeting in a cusp at the
//					vertex) and	zero, one or more unblended smooth edges
//					meet in the vertex, for which a special vertex blend
//					boundary must be made
//
//	These last three values only occur post ACIS 1.7 with advanced blending:
//
//	bl_v_cap		means cap or mitre at a vertex joined to one or two
//					blended edges (same as default behaviour with no
//					attribute on the vertex at the end of a blend sequence)
//	bl_v_roll_on	means close off the open end using edge-face or edge-edge
//					blends
//	bl_v_runout		means close off the open end using a variable-radius blend

/**
 * Specifies blend behavior at a vertex attached to one or more blended edges.
 * @param bl_v_unset
 * vertex blend.
 * @param bl_v_bi_blend
 * used internally to mark an interior vertex of a blend sequence for which more blend sheet faces will be made.
 * @param bl_v_cusp
 * notes that two blended edges meeting in a cusp at the vertex and zero, one or more unblended smooth edges meet in the vertex, for which a special vertex blend boundary must be made.
 * @param bl_v_blend
 * vertex blend.
 * @param bl_v_cap
 * means cap or mitre at a vertex joined to one or two blended edges (same as default behavior with no attribute on the vertex at the end of a blend sequence).
 * @param bl_v_roll_on
 * means close off the open end using edge-face or edge-edge blends.
 * @param bl_v_runout
 * means close off the open end using a variable-radius blend.
 **/
enum bl_v_property {
	bl_v_unset,
	bl_v_bi_blend,
	bl_v_cusp,
	bl_v_blend,
	bl_v_cap,
	bl_v_roll_on,
	bl_v_runout
};

// This enumerated type signifies whether setbacks at a vertex blend are to be
// calculated automatically or not (and allows for a choice of method to be
// offered in future if desired).
/**
 * Specifies autosetback behavior at a blend vertex.
 * @param bl_v_auto_unset
 * standard computation of autosetback at vertex blend.
 * @param bl_v_auto_linear
 * method takes a "linear" view in computing autosetback at vertex blend.
 */
enum bl_v_auto_property {
	bl_v_auto_unset,
	bl_v_auto_linear	// method takes a "linear" view of vertex blend
};

/** @} */
#endif
