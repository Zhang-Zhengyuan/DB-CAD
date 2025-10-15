// $Id: pyr_anno.hxx
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
//	Pyramid Annotations
//-----------------------------------------------------------------------

#ifndef __PYRANNO_HXX__
#define __PYRANNO_HXX__

#include "dcl_cstr.h"
#include "annotation.hxx"

/**
* @file primanno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PRIMITIVE_ANNOTATION, CSTR )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

#define PRIMITIVE_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
extern DECL_CSTR int PRIMITIVE_ANNOTATION_TYPE;

/**
 * Used for primitive annotations.
 * <br>
 * <b>Role:</b> The intent of annotations is that the ordering of items in the lists,
 * with respect to the geometry produced relative to model space, will be invariant
 * across future releases of ACIS.
 * @see ENTITY
 */
class DECL_CSTR PRIMITIVE_ANNOTATION: public ANNOTATION {
protected:
enum {
		e_vertex,
		e_edge,
		e_face,
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
	ENTITY_FUNCTIONS( PRIMITIVE_ANNOTATION, CSTR )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
 * call this constructor only with the overloaded <tt>new</tt> operator, because this reserves the memory
 * on the heap, a requirement to support roll back and history management.
 */
	PRIMITIVE_ANNOTATION();

	// Data Retrieval
/**
 * Returns the number of vertices in the primitive.
 */
	const int num_vertices() const { return ((EE_LIST*) (ents[e_vertex]))->iteration_count(); }
/**
 * Given an index, returns the primitive's vertex at that index.
 * <br><br>
 * @param i
 * index.
 */
	VERTEX* vertex(int i) const { return (VERTEX*) (*((EE_LIST*)(ents[e_vertex])))[i]; }
/**
 * Returns a list of all the vertices created as part of the primitive.
 */
	EE_LIST* vertices() const { return (EE_LIST*) ents[e_vertex]; }

/**
 * Returns the number of edges in the primitive.
 */
	const int num_edges() const { return ((EE_LIST*) (ents[e_edge]))->iteration_count(); }
/**
 * Given an index, returns the primitive's edge at that index.
 * <br><br>
 * @param i
 * index.
 */
	EDGE* edge(int i) const { return (EDGE*) (*((EE_LIST*)(ents[e_edge])))[i]; }
/**
 * Returns a list of all the edges created as part of the primitive.
 */
	EE_LIST* edges() const { return (EE_LIST*) ents[e_edge]; }
/**
 * Returns the number of faces in the primitive.
 */
	const int num_faces() const { return ((EE_LIST*) (ents[e_face]))->iteration_count(); }
/**
 * Given an index, returns the primitive's face at that index.
 * <br><br>
 * @param i
 * index.
 */
	FACE* face(int i) const { return (FACE*) (*((EE_LIST*)(ents[e_face])))[i]; }
/**
 * Returns a list of all the faces created as part of the primitive.
 */
	EE_LIST* faces() const { return (EE_LIST*) ents[e_face]; }

	// add data
/**
 * Adds a <tt>VERTEX</tt> to the primitive.
 * <br><br>
 * @param v
 * VERTEX to add.
 */
	void add_vertex( VERTEX* v ){ add_output_entity( ents[ e_vertex ], v ); }
/**
 * Adds an <tt>EDGE</tt> to the primitive.
 * <br><br>
 * @param e
 * EDGE to add.
 */
	void add_edge( EDGE* e ){ add_output_entity( ents[ e_edge ], e ); }
/**
 * Adds a <tt>FACE</tt> to the primitive.
 * <br><br>
 * @param f
 * FACE to add.
 */
	void add_face( FACE* f ){ add_output_entity( ents[ e_face ], f ); }
};

#endif //__PYRANNO_HXX__
