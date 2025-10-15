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
//	Space Warping Annotations
//-----------------------------------------------------------------------

/**
 * @file warpanno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup WARPAPI
 *
 * @{
 */

#ifndef __WARPANNO_HXX__
#define __WARPANNO_HXX__

#include "dcl_warp.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "face.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(WARP_ANNOTATION, WARP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(WARP_ANNO_FACE, WARP)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// First an organization class within the annotation hierarchy
// There is no data common to all local operations annotations, but we
// include this class anyway for organizations purposes

// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as 
// the base.

/**
 * @nodoc
 */
#define WARP_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
/**
 * @nodoc
 */
extern DECL_WARP int WARP_ANNOTATION_TYPE;

/**
 * Implements the annotations for space-warped entities.
 * <br>
 * <b>Role:</b> This annotation class is used to associate entities generated
 * by a space-warping operation with the original entities.  Currently, only 
 * <tt>FACE</tt>s are so annotated, via the derived <tt>WARP_ANNO_FACE</tt> class.
 * @see ANNOTATION, WARP_ANNO_FACE, ENTITY, EE_LIST, ATTRIB_TAG
 */
class DECL_WARP WARP_ANNOTATION: public ANNOTATION {
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
	ENTITY_FUNCTIONS( WARP_ANNOTATION, WARP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

/**
 * Constructs a <tt>WARP_ANNOTATION</tt>.
 */
	WARP_ANNOTATION() {};
};

//-----------------------------------------------------------------------
//	Warp Annotations -- Annotate faces that have been warped or transformed.
//-----------------------------------------------------------------------

/**
 * @nodoc
 */
#define WARP_ANNO_FACE_LEVEL (WARP_ANNOTATION_LEVEL + 1)
/**
 * @nodoc
 */
extern DECL_WARP int WARP_ANNO_FACE_TYPE;

/**
 * Implements the annotations for space-warped faces.
 * <br>
 * <b>Role:</b> This annotation class is used to associate faces generated
 * by a space-warping operation with the original faces.  Its base class is
 * <tt>WARP_ANNOTATION</tt>.
 * <br><br>
 * Objects of this annotation class have a single input (warped_face) face and one
 * or more output (transformed_face) faces.  As with all annotation inputs, the 
 * values of the former are actually stored within <tt>ATTRIB_TAG</tt>s.
 * <br><br>
 * Retrieval of space-warped faces is made by means of the <tt>warped_faces</tt> and
 * <tt>transformed_faces</tt> methods.  As multiple faces may be generated from a
 * single original, the <tt>ENTITY</tt> returned by the latter may be an 
 * <tt>EE_LIST</tt>.
 * @see ANNOTATION, WARP_ANNOTATION, ENTITY, FACE, EE_LIST, ATTRIB_TAG
 */
class DECL_WARP WARP_ANNO_FACE : public WARP_ANNOTATION {
protected:
    enum {
		e_warped_faces, 
		e_transformed_faces,
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
        ENTITY_FUNCTIONS(WARP_ANNO_FACE, WARP)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

/**
 * Constructs a <tt>WARP_ANNO_FACE</tt> referring to optional original and space-warped
 * faces.
 * <br><br>
 * @param warped_face
 * the original face to which <tt>this</tt> warp annotation refers.
 * @param transformed_face
 * the space-warped face to which <tt>this</tt> warp annotation refers.
 */
	WARP_ANNO_FACE(FACE* warped_face = NULL, FACE* transformed_face = NULL)
	{
		ents[e_warped_faces] = warped_face;
		ents[e_transformed_faces] = transformed_face;
		//hook_members();
	}
	// Data Retrieval

	// Return the FACE or EE_LIST of FACEs for the warped_faces.
/**
 * Returns a pointer to the original face that was space-warped.
 * <br><br>
 * <b>Role:</b> The returned entity pointer will be an <tt>ATTRIB_TAG</tt>
 * identifying the original face which has been warped.
 */
	ENTITY* warped_faces() const { return ents[e_warped_faces]; }

	// Return the FACE or EE_LIST of FACEs for the transformed_faces.
/**
 * Returns a pointer to faces generated as a result of the space-warping operation.
 * <br><br>
 * <b>Role:</b> The returned entity pointer will be an <tt>EE_LIST</tt>
 * if more than one face was generated by space-warping the original.
 */
	ENTITY* transformed_faces() const { return ents[e_transformed_faces]; }

    // Data changing -- For internal use when building the annotation
/**
 * @nodoc
 */
	void add_warped_face(ENTITY* e) { add_output_entity(ents[e_warped_faces], e); }
/**
 * @nodoc
 */
	void add_transformed_face(ENTITY* e) { add_output_entity(ents[e_transformed_faces], e); }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
/**
 * @nodoc
 */
	virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);
};

/**
 * @nodoc
 */
void AnnotateFaces(ENTITY_LIST & transform_list, ENTITY_LIST & bend_list); //internal use only
/** @} */
#endif /* __WARPANNO_HXX__ */
