// $Id: copyanno.hxx,v 1.16 2002/08/09 17:15:19 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------
// Purpose---
//    Annotations of what happens during copy
//-----------------------------------------------------------------------

#ifndef __COPYANNO_HXX__
#define __COPYANNO_HXX__

#include "dcl_kern.h"
#include "annotation.hxx"

/**
* @file copyanno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISANNOTATIONS
 *
 * @{
 */

#define COPY_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
extern DECL_KERN int COPY_ANNOTATION_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( COPY_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * Defines the annotation class for the copy operation.<br><br>
 * <b>Role:</b> This class identifies the original and the copied entity after
 * a copy operation.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>Original - ENTITY</dd>
 * <dd>Copy - ENTITY</dd>
 * </dl>
 * As with all inputs, the values are actually stored as
 * <tt>ATTRIB_TAGs</tt> which refer to the input entity.
 * @see ANNOTATION, ENTITY
 */

class DECL_KERN COPY_ANNOTATION: public ANNOTATION {
protected:
	enum {
		e_source,
		e_copy,
		e_num_datums
	};
public:
	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL(e_num_datums)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( COPY_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:
    /**
    * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
    * <br><br>
    * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
    * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_EFINT(...)</tt>), because this reserves
    * the memory on the heap, a requirement to support roll back and history management.
    * <br><br>
    * @param source
    * Original (ENTITY).
    * @param copy
    * Copy (ENTITY).
    */
	COPY_ANNOTATION(ENTITY* source = NULL, ENTITY* copy = NULL)
	{
		ents[e_source] = copy_ATTRIB_TAG(source,this);
		ents[e_copy] = copy;
		//hook_members();
	};

// Public functions to doc.

    /**
    * Returns the original ENTITY* which may actually be an 
	* ATTRIB_TAG, EE_LIST, FACE, EDGE or VERTEX.
    */
	ENTITY* source() const { return ents[e_source]; }

    /**
    * Returns the target ENTITY* which may actually be an 
	* ATTRIB_TAG, EE_LIST, FACE, EDGE or VERTEX.
    */
	ENTITY* copy() const { return ents[e_copy]; }

    /**
    * Adds the annotation to the original.
    */
	void add_source(ENTITY* s) { add_input_entity(ents[e_source], s); }
	
    /**
    * Adds the annotation to the copied item.
    */
    void add_copy(ENTITY* c) { add_output_entity(ents[e_copy], c); }
	
    /**
    * Removes the annotation from the original item.
    */
    void remove_source(ENTITY* s) { remove_input_entity(ents[e_source], s); }
	
    /**
    * Removes the annotation from the copied item.
    */
    void remove_copy(ENTITY* c) { remove_output_entity(ents[e_copy], c); }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	/**
    * @nodoc
    */
    virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	// STI rr (10/16/01) begin: Added the following virtual function.
	// See source code for details. Do not document.
	/**
    * @nodoc
    */
    virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);
	// STI rr end.

    /**
    * @nodoc
    */
	virtual void member_lost_hook(const ENTITY* member);
};
/** @} */
#endif //__COPYANNO_HXX__
