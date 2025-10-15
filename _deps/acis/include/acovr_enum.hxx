/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 07-Feb-03 Naming consitency: acover ==> acovr in enums
//
// bmt 22-Oct-03 extend enums acovr_continuity_level, acovr_tol_type for G2
// 
// bmt 29-Oct-03 add acovr_conflict_type enum
//
#ifndef ACOVR_ENUM_H
#define ACOVR_ENUM_H
/**
* @file acovr_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISADVCOVR
 *
 * @{
 */

/**
 * Used to specify the continuity (G0 or G1) requirement that will be enforced by an edge constraint.
 * <br>
 * @param acovr_default_continuity
 * use default continuity.
 * @param acovr_G0
 * enforce G0 continuity.
 * @param acovr_G1
 * enforce G1 continuity.
 */

enum acovr_continuity_level {
		acovr_default_continuity,
		acovr_G0,
		acovr_G1
} ;
/**
 * Used to specify the type of gap (position or tangent) that is referred.
 * <br>
 * @param acovr_pos
 * position gap.
 * @param acovr_tan
 * tangent gap.
 */
enum acovr_tol_type {
	 acovr_pos,
	 acovr_tan
} ;

/**
 * Used to specify the type of conflict that is referred.
 * <br>
 * @param G1_edge_edge_conflict
 * adjacent edges not G1, conflicts with user-input tangency data.
 * @param G2_edge_edge_conflict
 * adjacent edges not G2, conflicts with user-input tangency data.
 */
enum acovr_conflict_type {
	unknown_conflict,
	G1_edge_edge_conflict,
	G2_edge_edge_conflict
};

 /** @} */
#endif   //ACOVR_ENUM_H
