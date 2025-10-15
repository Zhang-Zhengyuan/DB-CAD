/* ORIGINAL: acis2.1/kerndata/attrib/at_sys.hxx */
/* $Id: at_sys.hxx,v 1.7 2002/08/09 17:15:16 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Attribute declaration for a private container attribute.  Each
// application developer receives one of these customised for himself.
// All attributes specific to the application developer are then made
// derived classes of this attribute, ensuring that different
// developers can assign identifiers independently without mutual
// interference.

// This one is for Acis system use.

#if !defined( ATTRIB_SYS_CLASS )
#define ATTRIB_SYS_CLASS

#include "dcl_kern.h"
#include "attrib.hxx"

/**
* @file at_sys.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */


// This attribute type is a derived class of ATTRIB.


extern DECL_KERN int ATTRIB_SYS_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_SYS_LEVEL (ATTRIB_LEVEL + 1)

/**
 * \class ATTRIB_SYS
 * Organization base attribute class for the ACIS kernel.
 * <br>
 * <b>Role:</b> The base class from which all attributes defined in the ACIS kernel are derived.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_SYS, KERN);
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


/** @} */
#endif
