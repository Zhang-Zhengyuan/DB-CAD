/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RMPCUR_HDR_DEF )
#define   RMPCUR_HDR_DEF
#include "dcl_kern.h"
#include "logical.h"
class ENTITY;
/**
 * @file rm_pcu.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPCURVES
 *
 * @{
 */
/**
 * This routine removes pcurves from any topological ENTITY.
 * This should NEVER be done by the application
 * developer.  This option is provided purely for internal usage 
 * by tolerant modelling functions.
 * @param end
 * The ENTITY from which to remove pcurves.
 * @param analytic_only
 * Flags the removal of pcurves from analytical surfaces only.
 * @param do_tcoedges
 * Flags the removal of pcurves from tolerant coedges.  
 * @param reset_pattern
 * Internal Use Only.
*/
DECL_KERN void sg_rm_pcurves_from_entity( 
	ENTITY *ent, 
	logical analytic_only = FALSE,
	logical do_tcoedges = FALSE,
	logical reset_pattern = TRUE
);
/** @} */
#endif
