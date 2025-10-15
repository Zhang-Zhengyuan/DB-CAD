// $Id: cur_off_anno.hxx,v 1.5 2000/12/26 18:55:03 products Exp $
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
//	Wire offset Annotations
//-----------------------------------------------------------------------

#ifndef __CUR_OFF_ANNO_HXX__
#define __CUR_OFF_ANNO_HXX__

#include "dcl_ofst.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "face.hxx"

#define OFST_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
extern DECL_OFST int OFST_ANNOTATION_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(OFST_ANNOTATION, OFST )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(WIRE_OFFSET_ANNO, OFST)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

class DECL_OFST OFST_ANNOTATION: public ANNOTATION {
protected:
public:
	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL_NO_ENTS()
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( OFST_ANNOTATION, OFST )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	OFST_ANNOTATION() {};
};

//-----------------------------------------------------------------------
//	Wire offset Annotations -- Annotate the edges and vertices of a wire offset
//-----------------------------------------------------------------------

#define WIRE_OFFSET_ANNO_LEVEL (OFST_ANNOTATION_LEVEL + 1)
extern DECL_OFST int WIRE_OFFSET_ANNO_TYPE;

class DECL_OFST WIRE_OFFSET_ANNO : public OFST_ANNOTATION {
protected:
    enum {
		e_original_ents,
		e_offset_ent,
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
        ENTITY_FUNCTIONS(WIRE_OFFSET_ANNO, OFST)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	WIRE_OFFSET_ANNO(ENTITY* original_ent = NULL, 
	                 ENTITY* offset_ent = NULL,
					 ENTITY* second_original_ent = NULL)
	{
		ents[e_original_ents] = copy_ATTRIB_TAG(original_ent,this);
		ents[e_offset_ent] = offset_ent;
		if (second_original_ent)
			add_input_entity(ents[e_original_ents], second_original_ent);
		//hook_members();
	}
	// Data Retrieval

   // Return the ATTRIB_TAG or EE_LIST of ATTRIB_TAGs for the original entities.
	ENTITY* original_ents() const { return ents[e_original_ents]; }

	// Return the offset entity.
	ENTITY* offset_ent() const { return ents[e_offset_ent]; }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};

#endif //__CUR_OFF_ANNO_HXX__
