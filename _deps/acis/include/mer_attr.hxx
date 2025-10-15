/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef mer_attr_hxx
#define mer_attr_hxx


#include "dcl_bool.h"
#include "attr_sti.hxx"
#include "logical.h"

/**
* @file mer_attr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class ENTITY_LIST;
class outcome;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(NO_MERGE_ATTRIB, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//======================================================================

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_BOOL int NO_MERGE_ATTRIB_TYPE;
#define NO_MERGE_ATTRIB_LEVEL (ATTRIB_ST_LEVEL + 1)

// Declare the class.
/**
 * Specifies a user-defined attribute that signals that the entity is not to be merged out of the body. 
 * <b><i>Currently, this attribute is only recognized when attached to edges or vertices.</i></b>
 * @see api_set_no_merge_attrib, api_remove_no_merge_attrib
 */
class DECL_BOOL NO_MERGE_ATTRIB: public ATTRIB_ST {

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * new operator inherited from the <tt>ENTITY</tt> class (for example,<br><br>
 * <tt>x = new NO_MERGE_ATTRIB(...))</tt>,<br><br> because this reserves the memory on the
 * heap, a requirement to support roll back and history management.
 * <br><br>
 * @param ent
 * owning entity.
 */
	NO_MERGE_ATTRIB( ENTITY *ent = NULL);
/**
 * @nodoc
 */
    virtual logical pattern_compatible() const;

	// Functions called to aid attribute migration during modelling
	// operations.

/**
 * @nodoc
 */
	virtual void split_owner( ENTITY * );

/**
 * @nodoc
 */
	virtual void merge_owner( ENTITY *, logical );

	// Declare all the standard attribute functions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( NO_MERGE_ATTRIB, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

//======================================================================

// Doc not needed as we have APIs api_set_no_merge_attrib and
// api_remove_no_merge_attrib.  find should not be needed by customers.

/**
 * @nodoc
 */
DECL_BOOL NO_MERGE_ATTRIB* find_NO_MERGE_ATTRIB(const ENTITY*);
/**
 * @nodoc
 */
DECL_BOOL outcome set_NO_MERGE_ATTRIB(ENTITY_LIST&);

/**
 * @nodoc
 */
DECL_BOOL outcome remove_NO_MERGE_ATTRIB(ENTITY_LIST&);


//======================================================================
#endif
