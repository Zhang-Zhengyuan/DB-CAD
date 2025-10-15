/* ORIGINAL: lop/lop_husk/tools/lopanno.hxx */
// $Id: loptanno.hxx,v 1.8 2002/08/09 17:27:01 jeff Exp $
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

#ifndef __LOPTANNO_HXX__
#define __LOPTANNO_HXX__

#include "dcl_lopt.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "face.hxx"
#include "edge.hxx"
#include "vertex.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( EDGE_TAPER_ANNO_FACE, LOPT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(LOPT_ANNOTATION, LOPT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(TWEAK_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SPLIT_VERTEX_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SHADOW_TAPER_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SINGULAR_VERTEX_ANNO, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// First an organization class within the annotation hierarchy
// There is no data common to all local operations annotations, but we
// include this class anyway for organizations purposes

// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as 
// the base.

#define LOPT_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
extern DECL_LOPT int LOPT_ANNOTATION_TYPE;

class DECL_LOPT LOPT_ANNOTATION: public ANNOTATION {
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
	ENTITY_FUNCTIONS( LOPT_ANNOTATION, LOPT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	LOPT_ANNOTATION() {};
};

//-----------------------------------------------------------------------
//	Tweak Annotations -- Annotate faces that have been tweaked.
//-----------------------------------------------------------------------

#define TWEAK_ANNO_FACE_LEVEL (LOPT_ANNOTATION_LEVEL + 1)
extern DECL_LOPT int TWEAK_ANNO_FACE_TYPE;

class DECL_LOPT TWEAK_ANNO_FACE : public LOPT_ANNOTATION {
protected:
    enum {
		e_tweaked_faces,
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
        ENTITY_FUNCTIONS(TWEAK_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	TWEAK_ANNO_FACE(FACE* tweaked_face = NULL)
	{
		ents[e_tweaked_faces] = tweaked_face;
		//hook_members();
	}
	// Data Retrieval

   // Return the FACE or EE_LIST of FACEs for the tweaked_faces.
	ENTITY* faces() const { return ents[e_tweaked_faces]; }

    // Data changing -- For internal use when building the annotation
	void add_face(ENTITY* e) { add_output_entity(ents[e_tweaked_faces], e); }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};

//-----------------------------------------------------------------------
//	Edge Taper Split Face Annotations -- Annotates new faces that result 
//                                       from splits due to edge tapering
//                                       the same face about multiple 
//                                       edges.
//-----------------------------------------------------------------------

#define EDGE_TAPER_ANNO_FACE_LEVEL (LOPT_ANNOTATION_LEVEL + 1)
extern DECL_LOPT int EDGE_TAPER_ANNO_FACE_TYPE;

class DECL_LOPT EDGE_TAPER_ANNO_FACE : public LOPT_ANNOTATION {
protected:
    enum {
		e_taper_edge,
        e_split_face,
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
        ENTITY_FUNCTIONS(EDGE_TAPER_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	EDGE_TAPER_ANNO_FACE(EDGE* taper_edge = NULL,
                         FACE* split_face = NULL)                       
	{
		ents[e_taper_edge] = copy_ATTRIB_TAG(taper_edge,this);
        ents[e_split_face] = split_face;
		//hook_members();
	}
	// Data Retrieval

   // Return the ATTRIB_TAG for the taper edge that is split.
	ENTITY* taper_edge_tag() const { return ents[e_taper_edge]; }

    // Return the taper edge if it is live, otherwise NULL
    EDGE* taper_edge() const { 
        ENTITY* actual = get_actual_live_entity(ents[e_taper_edge]);
		return (EDGE*) (is_EDGE(actual) ? actual : NULL); 
    }

    // Return the split face or EE_LIST
    ENTITY* split_face() const { return ents[e_split_face]; }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};

//-----------------------------------------------------------------------
//	Vertex Split Face Annotations -- Annotates new faces that result 
//                                   from vertex splits, e.g. offsetting
//                                   a four edge vertex.
//-----------------------------------------------------------------------

#define SPLIT_VERTEX_ANNO_FACE_LEVEL (LOPT_ANNOTATION_LEVEL + 1)
extern DECL_LOPT int SPLIT_VERTEX_ANNO_FACE_TYPE;

class DECL_LOPT SPLIT_VERTEX_ANNO_FACE : public LOPT_ANNOTATION {
protected:
    enum {
		e_split_vertex,
        e_split_face,
        e_new_face,
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
        ENTITY_FUNCTIONS(SPLIT_VERTEX_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	SPLIT_VERTEX_ANNO_FACE(VERTEX* split_vertex = NULL,
                           FACE* split_face = NULL,
                           FACE* new_face = NULL)                       
	{
		ents[e_split_vertex] = copy_ATTRIB_TAG(split_vertex,this);
        ents[e_split_face] = copy_ATTRIB_TAG(split_face,this);
        ents[e_new_face] = new_face;
		//hook_members();
	}
	// Data Retrieval

    // Return the ATTRIB_TAG for the vertex that is split (and most likely
    // deleted).
	ENTITY* split_vertex_tag() const { return ents[e_split_vertex]; }

    // Return the ATTRIB_TAG or EE_LIST for the face that is split
    ENTITY* split_face_tag() const { return ents[e_split_face]; }

    // Return the new face or EE_LIST
    ENTITY* new_face() const { return ents[e_new_face]; }

    // Data setting
    void add_split_face(FACE *split_face) { add_input_entity(ents[e_split_face], split_face); }

    void add_new_face(FACE *new_face) { add_output_entity(ents[e_new_face], new_face); }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};


//-----------------------------------------------------------------------
//	Shadow Face Annotations -- Annotates shadow tapered faces.
//
//-----------------------------------------------------------------------

#define SHADOW_TAPER_ANNO_FACE_LEVEL (LOPT_ANNOTATION_LEVEL + 1)
extern DECL_LOPT int SHADOW_TAPER_ANNO_FACE_TYPE;

class DECL_LOPT SHADOW_TAPER_ANNO_FACE : public LOPT_ANNOTATION {
protected:
    enum {
		e_silhouette_face,
        e_silhouette_edge,
        e_shadow_face,
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
        ENTITY_FUNCTIONS(SHADOW_TAPER_ANNO_FACE, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	SHADOW_TAPER_ANNO_FACE(FACE* silh_face = NULL,
                           EDGE* silh_edge = NULL,
                           FACE* shad_face = NULL)                       
	{
		ents[e_silhouette_face] = copy_ATTRIB_TAG(silh_face,this);
        ents[e_silhouette_edge] = silh_edge;
        ents[e_shadow_face] = shad_face;
		//hook_members();
	}
	// Data Retrieval

    // Return the ATTRIB_TAG for the silhouette face
	ENTITY* silhouette_face_tag() const { return ents[e_silhouette_face]; }

    // Return the silhouette edge or EE_LIST
    ENTITY* silhouette_edge() const { return ents[e_silhouette_edge]; }

    // Return the shadow face or EE_LIST
    ENTITY* shadow_face() const { return ents[e_shadow_face]; }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};

//-----------------------------------------------------------------------
//	Singular Vertex Annotations -- Annotates new vertices and edges that 
//                                 placed at singularities on a face. 
//-----------------------------------------------------------------------

#define SINGULAR_VERTEX_ANNO_LEVEL (LOPT_ANNOTATION_LEVEL + 1)
extern DECL_LOPT int SINGULAR_VERTEX_ANNO_TYPE;

class DECL_LOPT SINGULAR_VERTEX_ANNO : public LOPT_ANNOTATION {
protected:
    enum {
		e_singular_face,
        e_vertex,
        e_edge,
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
        ENTITY_FUNCTIONS(SINGULAR_VERTEX_ANNO, LOPT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	SINGULAR_VERTEX_ANNO(FACE* singular_face = NULL,
                         VERTEX* vertex = NULL,
                         EDGE* edge = NULL)                       
	{
		ents[e_singular_face] = copy_ATTRIB_TAG(singular_face,this);
        ents[e_vertex] = vertex;
        ents[e_edge] = edge;
		//hook_members();
	}
	// Data Retrieval

    // Return the ATTRIB_TAG for the singular face 
	ENTITY* singular_face_tag() const { return ents[e_singular_face]; }

    // Return the vertex at the singularity
    ENTITY* vertex() const { return ents[e_vertex]; }

    // Return the edge at the singularity
    ENTITY* edge() const { return ents[e_edge]; }

    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);
};

#endif //__LOPTANNO_HXX__
