/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Blending Annotations

#ifndef __BL_ENT_AN_HXX__
#define __BL_ENT_AN_HXX__
#include "dcl_blnd.h"
#include "blndanno.hxx"
/**
* @file bl_ent_an.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( BLEND_ANNO_ENTITY, BLND )

#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//-----------------------------------------------------------------------
//	Blending Annotations -- Entity Blend
//-----------------------------------------------------------------------

#define BLEND_ANNO_ENTITY_LEVEL (BLEND_ANNOTATION_LEVEL + 1)
extern DECL_BLND int BLEND_ANNO_ENTITY_TYPE;

// Forward declaration for BLEND_ANNO_ENTITY helper class

class bl_anno_entity_helper;

// TODO:  Distinguish left support from right support.

/**
 * Implements annotation for blend. 
 * <br>
 * <b>Role:</b> This annotation class is used to associate blend support
 * entities with the blend face, generated as the result of a blending
 * operation. 
 * <br><br>
 * The <tt>BLEND_ANNO_ENTITY</tt> treats blend support entities as input
 * entities. The result of blending operation, the blend face, is the output of
 * this annotation. The <tt>BLEND_ANNO_ENTITY</tt> has multiple input entities
 * (blend supports) and a single output entity (blend face). 
 * @see BLEND_ANNOTATION, ANNOTATION, ENTITY
 */
class DECL_BLND BLEND_ANNO_ENTITY : public BLEND_ANNOTATION {

private:
	
	// pointer to helper object

	bl_anno_entity_helper *pbl_anno_ent_helper; 


protected:

	enum {
		e_face = 0,
		e_blend_supports = 1,
		e_num_datums
		};
public:
	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL( e_num_datums )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( BLEND_ANNO_ENTITY, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 * Internal use only.
	 */
	BLEND_ANNO_ENTITY( FACE *face = NULL );

	
	BLEND_ANNO_ENTITY& 
	operator=( const BLEND_ANNO_ENTITY &ref_bl_anno_ent );
	
	// Data Retrieval -- All return an ENTITY* which may actually be an 
	// ATTRIB_TAG, EE_LIST or the type suggested by the method name
	/**
	 * Returns the blend face.
	 */
	ENTITY *face() const { return ents[ e_face ]; }

	/**
	 * Returns the blend support entities.<br>
	 * <b>Role:</b> Returns an EE_LIST, which contains list of support entities.
	 * @see EE_LIST
	 */
	ENTITY *blend_supports() const { return ents[ e_blend_supports ]; }

	// These routines simplify access to the list of support entities.
	/**
	 * Returns the count of support entities. 
	 */
	int num_blend_supports() const;

	/**
	 * Returns the i'th support entity of the blend.
	 */

	ENTITY *blend_support( int i ) const;

	// Data accumulation
	/**
	 * Internal function.
	 * @nodoc
	 */
	void add_support( ENTITY *support );

	// Notifications passed on via ATTRIB_ANNOTATION
	/**
	 * @nodoc
	 */
	virtual void split_member(
				ENTITY *member_ent,	// the member entity being split
				ENTITY *new_ent	    // "new entity" arising from split
				);

	/**
	 * @nodoc
	 */

    virtual void merge_member(
				ENTITY *member_entity,	// the first entity being merged
				ENTITY *other_entity,	// "other entity"
				logical	delete_member	// TRUE if first entity will be deleted
				);
};

 /** @} */
#endif //__BL_ENT_AN_HXX__
