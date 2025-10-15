/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( ATTRIB_PHL_CLASS )
#define ATTRIB_PHL_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include "dcl_kern.h"
#include "attrib.hxx"

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------
// This is a derived class of ATTRIB
#define ATTRIB_PHL_LEVEL ( ATTRIB_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------
// Identifier for the type of ATTRIB
extern DECL_KERN int ATTRIB_PHL_TYPE;

// Use ACIS macro to declare master attribute
/**
 * @nodoc 
 * Defines owning organization for other <tt>PHL</tt> attribute classes.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_PHL</tt> is the organizational class from which the <tt>PHL_ATTRIB_VW</tt> class is
 * derived. Its sole purpose is to identify this child class as belonging to <tt>PHL</tt>, and so adds
 * no new data or methods to those of <tt>ATTRIB</tt>.
 * <br><br>
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_PHL, KERN );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

#endif
