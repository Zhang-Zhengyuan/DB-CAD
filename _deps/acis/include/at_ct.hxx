/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// This attribute is designed for use with husks
/*******************************************************************/
#if !defined( ATTRIB_CT_CLASS )
#define ATTRIB_CT_CLASS
#include "dcl_kern.h"
#include "attrib.hxx"
/**
 * @file at_ct.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */
/**
 * @nodoc
 */
extern DECL_KERN int ATTRIB_CT_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_CT_LEVEL (ATTRIB_LEVEL + 1)
/** @} */
/**
 * \addtogroup ACISQUERIES
 * @{
 */
/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_CT</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_CT</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_KERN is_ATTRIB_CT( const ENTITY *ent );
/** @} */
/**
 * \addtogroup ACISATTRIBUTES
 * @{
 */
/**
 * \class ATTRIB_CT
 * Defines a root attribute for the Cellular Topology Component.
 * <br>
 * <b>Role:</b> All Cellular Topology Component attributes are derived from this class.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_CT, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/** @} */
#endif
