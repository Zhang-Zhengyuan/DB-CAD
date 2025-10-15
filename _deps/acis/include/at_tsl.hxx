/* ORIGINAL: acis2.1/kerndata/attrib/at_tsl.hxx */
/* $Id: at_tsl.hxx,v 1.7 2002/08/09 17:15:16 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Attribute declaration for a private container attribute. Each
// application developer receives one of these customised for himself.
// All attributes specific to the application developer are then made
// derived classes of this attribute, ensuring that different
// developers can assign identifiers independently without mutual
// interference.

// This one is for Spatial Corp internal use.

#if !defined( ATTRIB_TSL_CLASS )
#define ATTRIB_TSL_CLASS

#include "dcl_kern.h"
#include "attrib.hxx"

/**
* @file at_tsl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */

extern DECL_KERN int ATTRIB_TSL_TYPE;
#define ATTRIB_TSL_LEVEL (ATTRIB_LEVEL + 1)
/**
 * \class ATTRIB_TSL
 * This C++ destructor should never be called directly.
 * <br><br>
 * <b>Role:</b> Instead, applications should use
 * the overloaded lose method inherited from the <tt>ENTITY</tt> class, because this supports
 * history management. (For example, <tt>x=new ATTRIB_TSL(...)</tt> then later <tt>x->lose</tt>.)
 */
MASTER_ATTRIB_DECL( ATTRIB_TSL, KERN );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/** @} */
#endif
