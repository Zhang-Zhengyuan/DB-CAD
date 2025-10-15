/* ORIGINAL: acis2.1/sg_husk/sweep/swp_spl.hxx */
/* $Id: swp_enum.hxx,v 1.4 2000/12/26 18:49:03 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// History:
// Feb-06-03 jkf : Documentation review.
/*******************************************************************/
#if !defined( SWEEP_ENUM )
#define SWEEP_ENUM
/**
* @file swp_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup KERNAPI
 *
 * @{
 */
/**
 * Sweep to body boolean operation type.
 * @param UNITE
 * Creates a body which is the result of the sweep up to the location
 * of the intersection between the sweep and the fixed body.
 * This is the default setting.
 * @param INTERSECT
 * Creates one body out of the fixed body and the result of the sweep
 * up to the location of the intersection between the sweep and the
 * fixed body.
 * @param SUBTRACT
 * Creates one or more pieces resulting from the intersection of the
 * fixed body with the swept body.
 * @param LIMIT
 * Modifies the fixed body by removing material equivalent to the swept
 * body.
 * @param KEEP_BOTH
 * Creates a body equivalent to the limit type without deleting the
 * original body.
 */
enum sweep_bool_type {
	UNITE,
	INTERSECT,
	SUBTRACT,
	LIMIT,
	KEEP_BOTH
};
/** @} */
#endif
