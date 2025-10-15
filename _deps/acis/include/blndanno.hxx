// $Id: blndanno.hxx,v 1.19 2002/08/09 17:28:25 jeff Exp $
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
//	Blending Annotations
//-----------------------------------------------------------------------

#ifndef __BLNDANNO_HXX__
#define __BLNDANNO_HXX__

#include "dcl_blnd.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "face.hxx"
#include "edge.hxx"
#include "vertex.hxx"
#include "splitannotation.hxx"

/**
* @file blndanno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( BLEND_ANNO_VERTEX_CHAMFER, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( BLEND_ANNO_VERTEX, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( BLEND_ANNO_EDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( BLEND_ANNOTATION, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// First an organization class within the annotation hierarchy
// There is no data common to all blend annotations, but we
// include this class anyway for organizations purposes

// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as
// the base.

#define BLEND_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)

/**
 * @nodoc
 */
extern DECL_BLND int BLEND_ANNOTATION_TYPE;

/**
 * Implements the base class for blending annotations.
 * <br>
 * <b>Role:</b> This is the base (organization) class for blending annotations.
 * More specific blend annotation classes are derived from this class.
 * @see BLEND_ANNO_EDGE, BLEND_ANNO_VERTEX, BLEND_ANNO_VERTEX_CHAMFER
 */
class DECL_BLND BLEND_ANNOTATION: public ANNOTATION {
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
	ENTITY_FUNCTIONS( BLEND_ANNOTATION, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:


/**
 * @nodoc
 */
	BLEND_ANNOTATION() {};
};

//-----------------------------------------------------------------------
//	Blending Annotations -- Edge Blend
//-----------------------------------------------------------------------

#define BLEND_ANNO_EDGE_LEVEL (BLEND_ANNOTATION_LEVEL + 1)

/**
 * @nodoc
 */
extern DECL_BLND int BLEND_ANNO_EDGE_TYPE;

/**
 * Implements the annotations for blended edges.
 * <br>
 * <b>Role:</b> Annotation is used to associate the resulting blend face with the orignal edge.
 * @see ANNOTATION
 */
class DECL_BLND BLEND_ANNO_EDGE : public BLEND_ANNOTATION {
protected:
	enum {
		e_edge,
		e_blend_face,
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
	ENTITY_FUNCTIONS(BLEND_ANNO_EDGE, BLND)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

/**
 * @nodoc
 */
	BLEND_ANNO_EDGE(
		EDGE* edge = NULL,
		FACE* blend_face = NULL
		)
	{
		ents[e_edge] = copy_ATTRIB_TAG(get_original_old_entity_from_splits(edge),this);
		ents[e_blend_face] = blend_face;
		//hook_members();
	}

// Public functions to doc.

	// Data Retrieval -- All return an ENTITY* which may actually be an
	// ATTRIB_TAG, EE_LIST or the type suggested by the method name

/**
 * Returns a pointer to the original edge that was blended.
 * <br><br>
 * <b>Role:</b> The returned entity pointer in most cases will be an <tt>ATTRIB_TAG</tt>
 * identifying the original edge which has been deleted.  If the resulting blend
 * face originated from more than one original edge, an <tt>EE_LIST</tt> will be returned
 * containing pointers to all applicable attrib tags.
 */
	ENTITY* edge() const { return ents[e_edge]; }

/**
 * Returns a pointer to the resulting blend face.
 */
	ENTITY* blend_face() const { return ents[e_blend_face]; }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);
};


//-----------------------------------------------------------------------
//	Blending Annotations -- Vertex Blend
//-----------------------------------------------------------------------

#define BLEND_ANNO_VERTEX_LEVEL (BLEND_ANNOTATION_LEVEL + 1)

/**
 * @nodoc
 */
extern DECL_BLND int BLEND_ANNO_VERTEX_TYPE;


/**
 * Implements the annotations for blended vertices.
 * <br>
 * <b>Role:</b> Annotation is used to associate the resulting blend face with the orignal vertex.
 * @see ANNOTATION
 */
class DECL_BLND BLEND_ANNO_VERTEX : public BLEND_ANNOTATION {
protected:
	enum {
		e_vertex,
		e_blend_faces,
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
	ENTITY_FUNCTIONS(BLEND_ANNO_VERTEX, BLND)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

/**
 * @nodoc
 */
	BLEND_ANNO_VERTEX(
		VERTEX* vertex = NULL
		)
	{
		ents[e_vertex] = copy_ATTRIB_TAG(vertex,this);
		ents[e_blend_faces] = NULL;
		//hook_members();
	}

	// Data accumlation
/**
 * @nodoc
 */
	void add_face(FACE* face);

// Public functions to doc.

	// Data Retrieval -- All return an ENTITY* which may actually be an
	// ATTRIB_TAG, EE_LIST or the type suggested by the method name

/**
 * Returns a pointer to the original vertex that was blended.
 * <br><br>
 * <b>Role:</b> The returned entity pointer will be an <tt>ATTRIB_TAG</tt>
 * identifying the original vertex which has been deleted.
 */
	ENTITY* vertex() const { return ents[e_vertex]; }

/**
 * Returns a pointer to the resulting blend faces.
 * <br><br>
 * <b>Role:</b> If the vertex blend resulted in a single blend face, the pointer
 * to this face will be returned.  If the vertex blend resulted in multiple
 * blend faces, these will be returned in an <tt>EE_LIST</tt>.
 */
	ENTITY* blend_faces() const { return ents[e_blend_faces]; }

	// These routines deal with the high likelyhood that the blend_faces are
	// in fact one or more FACES.  The only way in is via add_face so if there
	// is anything there it is a FACE or EE_LIST of faces.

/**
 * Returns the number of resulting blend faces.
 */
	int num_blend_faces() const;

/**
 * Returns a pointer to the resulting blend face requested.
 * <br><br>
 * @param i
 * index of blend face requested.
 */
	FACE* blend_face(int i) const;

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only

/**
 * @nodoc
 */
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};


//-----------------------------------------------------------------------
//	Blending Annotations -- Vertex Chamfer Blend
//-----------------------------------------------------------------------

#define BLEND_ANNO_VERTEX_CHAMFER_LEVEL (BLEND_ANNO_VERTEX_LEVEL + 1)

/**
 * @nodoc
 */
extern DECL_BLND int BLEND_ANNO_VERTEX_CHAMFER_TYPE;


/**
 * Implements the annotations for chamfered vertices.
 * <br>
 * <b>Role:</b> This class is derived from <tt>BLEND_ANNO_VERTEX</tt>
 * and uses the same methods for accessing the data contained within the annotation.
 * @see BLEND_ANNO_VERTEX
 */
class DECL_BLND BLEND_ANNO_VERTEX_CHAMFER : public BLEND_ANNO_VERTEX {

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
	ENTITY_FUNCTIONS(BLEND_ANNO_VERTEX_CHAMFER, BLND)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:


/**
 * @nodoc
 */
	BLEND_ANNO_VERTEX_CHAMFER(
		VERTEX* vertex = NULL
		)
	{
		ents[e_vertex] = copy_ATTRIB_TAG(vertex,this);
		ents[e_blend_faces] = NULL;
	}

/**
 * @nodoc
 */
	virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);
};


 /** @} */
#endif //__BLNDANNO_HXX__
