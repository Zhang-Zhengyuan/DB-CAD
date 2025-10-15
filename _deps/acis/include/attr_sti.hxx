/* ORIGINAL: 3dt2.1/geomhusk/attr_sti.hxx */
// $Id: attr_sti.hxx,v 1.7 2002/08/09 17:15:14 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
// Attribute declaration for a private container attribute. Each
// application developer receives one of these customised for himself.
// All attributes specific to the application developer are then made
// derived classes of this attribute, ensuring that different
// developers can assign identifiers independently without mutual
// interference.
//
//----------------------------------------------------------------------

#ifndef attr_sti_hxx
#define attr_sti_hxx

//======================================================================

#include "dcl_kern.h"
#include "attrib.hxx"

/**
* @file attr_sti.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */


// This attribute type is a derived class of ATTRIB.

extern DECL_KERN int ATTRIB_ST_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_ST_LEVEL (ATTRIB_LEVEL + 1)

/**
 * \class ATTRIB_ST
 * Organization attribute from which various color, display, id, and other attributes are derived.
 * <br>
 * <b>Role:</b> This class is an attribute declaration for a private container attribute. This
 * class is derived from the ACIS base class, <tt>ATTRIB</tt>. Each application developer receives a customized
 * attribute declaration. The application developer then makes all attributes specific to the
 * application-derived classes of this attribute, ensuring that different developers can assign
 * identifiers independently without interference.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_ST, KERN );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/** @} */
#endif
