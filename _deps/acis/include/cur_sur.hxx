/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( cur_sur_ENUM )
#define cur_sur_ENUM
#include "enum_tbl.hxx"
#if defined D3_STANDALONE || defined D3_DEBUG
#include "stream.hxx"
#endif
/**
* @file cur_sur.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
/**
* Number of closed forms for blend surfaces
* @param OPEN
* open.
* @param CLOSED
* closed.
* @param PERIODIC
* periodic.
* @param CLOSURE_UNSET
* unspecified closure.
**/
enum closed_forms { OPEN, CLOSED, PERIODIC, CLOSURE_UNSET };
/**
 * Level at which approximating surface data is output to save files.
 * @param save_approx_full
 * Save the full bs3_curve / surface.
 * @param save_approx_summary
 * Save a summary of the bs3_curve / surface.
 * @param save_approx_none.
 * Do not save the bs3_curve or surface at all.
 * @param save_approx_historical
 * Save as before version 5.0.
 * @param save_approx_optimal
 * Let the system make the decision, based on specific curve or surface type.
 */
enum save_approx_level
{
	save_approx_full,
	save_approx_summary,
	save_approx_none,
	save_approx_historical,
	save_approx_optimal
};
/**
 * @nodoc
 */
enum unmarked_discon_type { NO_UNMARKED_DISCONTS, G1_UNMARKED_DISCONTS, G2_UNMARKED_DISCONTS, G1_AND_G2_UNMARKED_DISCONTS};
/**
 * @nodoc
 * Already defined.
 */
extern 	const enum_table	closed_form_map;
/**
 * @nodoc
 * Already defined.
 */
extern 	const enum_table	save_approx_map;
#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */
DECL_KERN	D3_ostream &operator<<( D3_ostream &os, const closed_forms& cl );
/**
 * @nodoc
 */
DECL_KERN	D3_istream &operator>>( D3_istream &is, closed_forms& cl );
#endif
/** @} */
#endif
