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
//	Define a generic attribute type that can mark a face or entity
//	to not be rendered.
//
//----------------------------------------------------------------------

#ifndef _NO_REND_HXX
#define _NO_REND_HXX

#include "dcl_rb.h"
#include "attr_sti.hxx"
/**
 * @file no_rend.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(NORENDER_ATTRIB, RB)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_RB int NORENDER_ATTRIB_TYPE;
#define NORENDER_ATTRIB_LEVEL (ATTRIB_ST_LEVEL + 1)

// Declare the class.
/**
 * Defines a generic attribute type that can mark a face or entity to not be rendered.
 */
class DECL_RB NORENDER_ATTRIB: public ATTRIB_ST {

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example,
 * <tt>x=new NORENDER_ATTRIB(...)</tt>), because this reserves the memory on the heap, a
 * requirement to support roll back and history management.
 * <br><br>
 * Declares the generic attribute type which marks a face or entity to not be rendered.
 * <br><br>
 * @param ent
 * entity pointer.
 */
	NORENDER_ATTRIB( ENTITY *ent = NULL);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( NORENDER_ATTRIB, RB)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
};
/**
 * Finds and returns a <tt>NORENDER_ATTRIB</tt> attached to an <tt>ENTITY</tt>.
 * <br><br>
 * @param ent
 * entity pointer.
 */
DECL_RB NORENDER_ATTRIB* find_NORENDER_ATTRIB( const ENTITY* ent);
/** @} */
#endif
