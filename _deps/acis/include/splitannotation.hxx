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
//	Split Annotations
//-----------------------------------------------------------------------

#ifndef __SPLITANNO_HXX__
#define __SPLITANNO_HXX__

#include "dcl_kern.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"

/**
* @file splitannotation.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISANNOTATIONS
 *
 * @{
 */

//-----------------------------------------------------------------------
//	Split Annotations -- Annotate the old entity, the new entity,
//                       and (if known) the gap entity.
//-----------------------------------------------------------------------

// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as 
// the base.

#define SPLIT_ANNOTATION_LEVEL (ANNOTATION_LEVEL + 1)
extern DECL_KERN int SPLIT_ANNOTATION_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SPLIT_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


/**
* Defines the annotation class for split operations.
* <b>Role:</b>This class annotates splits. It annotates the old entity, the new entity, 
* and (if known) the gap entity. The gap entity is filled in only when it is known, and often the 
* <tt>gap_entity</tt> is NULL. In the case of vertical splits, the <tt>gap_entity</tt> is 
* dimensionally smaller than the <tt>old_entity</tt> and the <tt>new_entity</tt>. 
* Parallel splits, of the type we annotate from inside Local Ops, result 
* in dimensionally larger gap entities.<br>
* <dl>
* <dt>Inputs:</dt>
* <dd>Old entity before the split- ENTITY</dd>
* <dd>Entity lying between the original and split entities - ENTITY</dd>
* <dd>Split from the original entity - ENTITY</dd>
* </dl>
* As with all inputs, the values are actually stored as
* <tt>ATTRIB_TAGs</tt> which refer to the input entity.
* @see ANNOTATION, ENTITY
*/
class DECL_KERN SPLIT_ANNOTATION : public ANNOTATION {
protected:
    enum {
		e_old_entity,
        e_new_entity,
		e_gap_entity,
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
        ENTITY_FUNCTIONS(SPLIT_ANNOTATION, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    /**
    * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
    * <br><br>
    * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
    * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_EFINT(...)</tt>), because this reserves
    * the memory on the heap, a requirement to support roll back and history management.
    * <br><br>
    * @param old_entity
    * Old entity before the split (ENTITY).
    * @param new_entity
    * Split from the original entity (ENTITY).
    * @param gap_entity
    * Entity lying between the original and split entities (ENTITY).
    */
    SPLIT_ANNOTATION(ENTITY* old_entity = NULL,
                     ENTITY* new_entity = NULL,
                     ENTITY* gap_entity = NULL)
	{
		ents[e_old_entity] = copy_ATTRIB_TAG(old_entity,this);
        ents[e_new_entity] = new_entity;
        ents[e_gap_entity] = gap_entity;
		//hook_members();
	}
	// Data Retrieval
    
    /**
    * Return the ATTRIB_TAG or EE_LIST for the old entity that is split.
	*/
    ENTITY* get_old_ent() const { return ents[e_old_entity]; }

    /**
    * Return the entity or EE_LIST that fills the gap between the old and new entities
    */
    ENTITY* get_gap_ent() const { return ents[e_gap_entity]; }

    /**
    * Return the entity or EE_LIST for the new entity
    */
    ENTITY* get_new_ent() const { return ents[e_new_entity]; }

    /**
    * Adds the <tt>gap_entity</tt> to the list.
    */
    void add_gap_ent(ENTITY *gap_ent) { add_output_entity(ents[e_gap_entity], gap_ent); }

    /**
    * Adds the <tt>old_entity</tt> to the list.
    */
    void add_old_ent(ENTITY *old_ent) { add_input_entity(ents[e_old_entity], old_ent); }

    /**
    * Adds the <tt>new_entity</tt> to the list.
    */
    void add_new_ent(ENTITY *new_ent) { add_output_entity(ents[e_new_entity], new_ent); }

    /**
    * Removes the <tt>old_entity</tt> from the list.
    */
	void remove_old_ent(ENTITY *old_ent) { remove_input_entity(ents[e_old_entity], old_ent); }

    /**
    * Removes the <tt>new_entity</tt> from the list.
    */
	void remove_new_ent(ENTITY *new_ent) { remove_output_entity(ents[e_new_entity], new_ent); }

    /**
    * Removes the <tt>gap_entity</tt> from the list.
    */
	void remove_gap_ent(ENTITY *gap_ent) { remove_output_entity(ents[e_gap_entity], gap_ent); }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    /**
    * @nodoc
    */
	virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);
	// STI rr (10/16/01) begin: Added the following virtual function.
	// See source code for details. Do not document.
    /**
    * @nodoc
    */
	virtual void member_lost_hook(const ENTITY* member);
	// STI rr end.
};

 /**
 * Returns the original old entity (an ATTRIB_TAG if this entity is dead) found from
 * tracing back through all split annotations
 */
extern DECL_KERN ENTITY *get_original_old_entity_from_splits(ENTITY *final_new_entity);
/** @} */
#endif //__SPLITANNO_HXX__
