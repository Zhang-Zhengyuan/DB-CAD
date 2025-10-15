/* ORIGINAL: kernel/kerndata/tolmod/tol_anno.hxx */
// $Id: tol_anno.hxx,v 1.5 2002/02/05 16:59:43 ywoo Exp $
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
//	Local Ops Annotations
//-----------------------------------------------------------------------

#ifndef __TOLANNO_HXX__
#define __TOLANNO_HXX__

#include "dcl_kern.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(TOL_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(CREATE_TOL_ANNO, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(REVERT_TOL_ANNO, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// First an organization class within the annotation hierarchy
// There is no data common to all tolerant modeling annotations, but we
// include this class anyway for organizations purposes

// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as 
// the base.

#define TOL_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
extern DECL_KERN int TOL_ANNOTATION_TYPE;

class DECL_KERN TOL_ANNOTATION: public ANNOTATION {
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
	ENTITY_FUNCTIONS( TOL_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	TOL_ANNOTATION() {};
};

//-------------------------------------------------------------------
// Create Tolerant Annotation -- Annotate newly created tolerant edges
//                               and vertices that replace non-tolerant
//                               entities.
//-------------------------------------------------------------------

#define CREATE_TOL_ANNO_LEVEL (TOL_ANNOTATION_LEVEL + 1)
extern DECL_KERN int CREATE_TOL_ANNO_TYPE;

class DECL_KERN CREATE_TOL_ANNO : public TOL_ANNOTATION {
protected:
    enum {
		e_original_ent,
        e_tolerant_ent,
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
        ENTITY_FUNCTIONS(CREATE_TOL_ANNO, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	CREATE_TOL_ANNO(ENTITY* original_ent = NULL,
                    ENTITY* tolerant_ent = NULL)
	{
		ents[e_original_ent] = copy_ATTRIB_TAG(original_ent,this);
        ents[e_tolerant_ent] = tolerant_ent;
		//hook_members();
	}

	// Addition original entities possible when replace short
	// edges or sliver faces.
	void add_original_ent(ENTITY* e) { add_input_entity(ents[e_original_ent], e); }
	void add_tolerant_ent(ENTITY* e) { add_output_entity(ents[e_tolerant_ent], e); }

	// Data Retrieval

    // Return the ATTRIB_TAG for the original entity that becomes tolerant.
	ENTITY* original_ent_tag() const { return ents[e_original_ent]; }

    // Return the tolerant entity
    ENTITY* tolerant_ent() const { return ents[e_tolerant_ent]; }

	void roll_notify(BULLETIN_TYPE type, ENTITY* other);

};


//-------------------------------------------------------------------
// Revert Tolerant Annotation -- Annotate newly created edges and 
//                               vertices that replace formerly tolerant
//                               entities.
//-------------------------------------------------------------------

#define REVERT_TOL_ANNO_LEVEL (TOL_ANNOTATION_LEVEL + 1)
extern DECL_KERN int REVERT_TOL_ANNO_TYPE;

class DECL_KERN REVERT_TOL_ANNO : public TOL_ANNOTATION {
protected:
    enum {
		e_tolerant_ent,
        e_new_ent,
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
        ENTITY_FUNCTIONS(REVERT_TOL_ANNO, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	REVERT_TOL_ANNO(ENTITY* tolerant_ent = NULL,
                    ENTITY* new_ent = NULL)
	{
		ents[e_tolerant_ent] = copy_ATTRIB_TAG(tolerant_ent,this);
        ents[e_new_ent] = new_ent;
		//hook_members();
	}
	// Data Retrieval

    // Return the ATTRIB_TAG for the original tolerant entity that becomes non-tolerant.
	ENTITY* tolerant_ent_tag() const { return ents[e_tolerant_ent]; }

    // Return the non-tolerant entity
    ENTITY* new_ent() const { return ents[e_new_ent]; }

	void roll_notify(BULLETIN_TYPE type, ENTITY* other);

};


#endif //__TOLANNO_HXX__
