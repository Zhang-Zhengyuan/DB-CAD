/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( ENTITY_PHL_CLASS )
#define ENTITY_PHL_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include "dcl_kern.h"

#include "entity.hxx"


// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// This is a derived class of ENTITY
#define ENTITY_PHL_LEVEL ( ENTITY_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ENTITY
extern DECL_KERN int ENTITY_PHL_TYPE;

// Use ACIS macro to declare master entity
/**
 * Defines owning organization for other <tt>PHL</tt> entity classes.
 * <br>
 * <b>Role</b>: All classes representing permanent objects in the <tt>PHL</tt> model (except attributes) derive
 * from the base class <tt>ENTITY_PHL</tt>, which is derived from the <tt>ENTITY</tt> class.
 * <br><br>
 * The <tt>ENTITY_PHL</tt> class does not represent any specific object within the modeler, but rather allows
 * consistent performance of operations, such as system debugging, change records (bulletin board), roll
 * back, attributes, and model archiving and communication, which are generic member functions of the
 * <tt>ENTITY</tt> class.
 * <br><br>
 * @see ENTITY
 */
MASTER_ENTITY_DECL( ENTITY_PHL, KERN );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


#endif
