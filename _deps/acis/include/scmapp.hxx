/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef scmap_hxx
#define scmap_hxx
#include "part_ctx.hxx"

/**
 * @file scmapp.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <scmapp.hxx>
 * @{
 */
/**
 * Gets the active <tt>PART_CONTEXT</tt>.
 * <br><br>
 * <b>Role:</b> Use this method when adding a new <tt>ENTITY</tt> to a <tt>PART</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/
PART_CONTEXT* active_part_context();
/**
 * @nodoc
 */
void set_active_part_context(PART_CONTEXT*);
/** @} */
#endif
