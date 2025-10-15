// $Id: patternent.hxx,v 1.5 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
//----------------------------------------------------------------------
// purpose---
//    define a APATTERN class
//----------------------------------------------------------------------
#ifndef PATTERNENT_HXX
#define PATTERNENT_HXX

//======================================================================

#include "dcl_kern.h"
#include "entity.hxx"
#include "api.hxx"

/**
* @file patternent.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class pattern;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( APATTERN, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//----------------------------------------------------------------------

//======================================================================

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int APATTERN_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define APATTERN_LEVEL 1

// Routine to restore a TEXT_ENT entity from disc (declared here to make it
// a friend and static).

/**
 * @nodoc
 */

LOCAL_PROC ENTITY* APATTERN_restore_data();


// TEXT_ENT declaration proper.

/**
 * .
 */
class DECL_KERN APATTERN: public ENTITY {

private:
	// the pattern definition

	pattern* pattern_def;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( APATTERN, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

public:

    //----------------------------------------
    // Constructors

	APATTERN();
	APATTERN(pattern*);
 	APATTERN(APATTERN*);

    //----------------------------------------
    // Destructor
    //  The destructor of APATTERN is private.  To delete a APATTERN entity, use the
	//	lose method.
    /**
     *.
     */
	pattern* def() const { return pattern_def; }
	/**
     *.
     */
    void set_def(pattern* in_pat);
	/**
     *.
     */
    void operator*=(const SPAtransf&);
};

//----------------------------------------------------------------------

/**
 * .
 */

DECL_KERN logical is_PATTERN (const ENTITY* ent);


//======================================================================
/*! @} */
#endif

