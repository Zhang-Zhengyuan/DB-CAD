/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//	Skinning Annotations
/*******************************************************************/
#ifndef __SKINANNO_HXX__
#define __SKINANNO_HXX__
#include "dcl_skin.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "face.hxx"
#include "coedge.hxx"
#include "vertex.hxx"
/**
 * @file skinanno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SKINLOFTAPI
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNOTATION, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNO_LAT_EDGE, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNO_LAT_FACE, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNO_START_FACE, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNO_END_FACE, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SKIN_ANNO_START_EDGE, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SKIN_ANNO_END_EDGE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
#define SKIN_ANNOTATION_LEVEL ( ANNOTATION_LEVEL + 1)
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNOTATION_TYPE;
/**
 * Implements the base class for skinning annotations.<br>
 * <b>Role:</b> This is the base class for skinning annotations. 
 * More specific skin annotation classes are derived from this class. 
 */
class DECL_SKIN SKIN_ANNOTATION: public ANNOTATION {
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
	ENTITY_FUNCTIONS( SKIN_ANNOTATION, SKIN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
	/**
	 * @nodoc
	 */
	SKIN_ANNOTATION() {};
};
/**
 * @nodoc
 * This function is used to get the original loft entity
 * by jumping over successive copies and splits that were
 * done to the original entity for doing the lofting 
 * operations.
 */
DECL_SKIN ENTITY *get_original_loft_entity(ENTITY *final_entity);

//-----------------------------------------------------------------------
//	Skinning Annotations -- lateral coedge. 
//	The lateral coedge as far as the user can see is a single coedge connecting 
//	the corresponding vertices (inputs entities) of the input wires. However, 
//	actually the lateral coedge is composed of several coedges (output entities). 
//	Note that we will annotate the lateral coedges before adding the convexity
//	points
//-----------------------------------------------------------------------
/**
 * @nodoc
 */
#define SKIN_ANNO_LAT_EDGE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_LAT_EDGE_TYPE;
/**
 * Defines the annotation class for a lateral edge that corresponds to the profile vertices connected
 * by that lateral edge in skinning.<br>
 * <b>Role:</b> This class helps to identity all the profile vertices from which a lateral edge is made in skinning.
 * Each lateral edge of the skin body, when constructed, is annotated via this annotation class. <br><br>
 * Note that skin annotates the lateral edges before adding the convexity points.<br><br>
 * Inputs:<br>
 * Profile vertices - VERTEX<br><br>
 * Outputs:<br>
 * Lateral edges - EDGE<br><br>
 * @see EDGE, VERTEX
 */
class DECL_SKIN SKIN_ANNO_LAT_EDGE : public SKIN_ANNOTATION {
protected:
	/**
	 * @nodoc
	 */
	enum {
		e_vertices, // Input entities
		e_edges,	// Output entities
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
	ENTITY_FUNCTIONS(SKIN_ANNO_LAT_EDGE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
	/**
	 * @nodoc
	 * C++ constructor to create an object. The inputs and output needs to added separately.
	 */
	SKIN_ANNO_LAT_EDGE(
		)
	{
		ents[e_vertices] = NULL;	// Input vertices are accumulated via add_vertex()
		ents[e_edges] = NULL;		// Output edges are accumulated via add_edge()
	}
	/**
	 * @nodoc
	 * Adds an input vertex that corresponds to the output lateral edge
	 */
	void add_vertex(VERTEX* vertex);
	/**
	 * @nodoc
	 * Adds a lateral edge of the outcoming skinned body
	 */
	void add_edge(EDGE* edge);
	/**
	 * Returns all the input vertices that corresponds to the lateral edges annotated by this object. The returned
	 * entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* vertices() const
	{ return ents[e_vertices]; }

	/**
	 * Returns all the lateral edges annotated by this object. The returned
	 * entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* edges() const
	{ return ents[e_edges]; }
};
//-----------------------------------------------------------------------
//	Skinning Annotations -- lateral face
//	Each lateral face of the skin body, when construced, is annotated via 
//	this annotation class. The input entities for the lateral face are the 
//	corresponding coedge on each of the input profiles. The output entity 
//	is the lateral face itself.
//-----------------------------------------------------------------------
/**
 * @nodoc
 */
#define SKIN_ANNO_LAT_FACE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_LAT_FACE_TYPE;
/**
 * Defines the annotation class for a lateral face (skinned face) that corresponds
 * to the profile coedges from which the lateral face is skinned. <br>
 * <b>Role:</b> This class helps to identify all those coedges from which a lateral face is skinned. Each lateral
 * face of the skin body, when constructed, is annotated via this annotation class. <br><br>
 * The input entities for the lateral face are the corresponding coedge on each of the input profiles.
 * The output entity is the lateral face itself.<br><br>
 * Inputs: <br>
 * Profile coedges - COEDGE <br>
 * Outputs: <br>
 * Lateral face - FACE<br><br>
 * @see COEDGE, FACE
 */
class DECL_SKIN SKIN_ANNO_LAT_FACE : public SKIN_ANNOTATION {
protected:
   /**
     * @nodoc
	 */
	enum {
		e_coedges,	// Input entities 
		e_face,		// Output entity 
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
	ENTITY_FUNCTIONS(SKIN_ANNO_LAT_FACE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
	/**
	 * @nodoc
	 * C++ constructor to create an object of this class. 
	 */
	SKIN_ANNO_LAT_FACE(
		FACE* face = NULL
		)
	{
		ents[e_coedges] = NULL;	// Input coedges are accumulated via add_coedge()
		ents[e_face] = face;	// Output face set here
	}
	/**
	 * @nodoc
	 */
	void add_coedge(COEDGE* coedge);
	/**
	 * Returns all the input profile coedges that corresponds to the lateral face annotated by this object. The returned
	 * entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* coedges() const
	{ return ents[e_coedges]; }

	/**
	 * Returns the annotated lateral face
	 */
	FACE* face() const
	{ return (FACE *)ents[e_face]; }
};
//-----------------------------------------------------------------------
//	Skinning Annotations -- start face
//	The start faces of the skin body, when construced, are annotated via 
//	this annotation class. The input entities for the start face are the 
//	coedges of the first/last wires as appropriate. The output entities
//	are the start faces themselves. We do not assume that only one start
//	face caps the ends. This way when we upgrade the end capping algorithm
//	we will not be in trouble due to annotations.
//-----------------------------------------------------------------------
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_START_FACE_TYPE;
/**
 * @nodoc
 */
#define SKIN_ANNO_START_FACE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)
/** 
 * Defines the annotation class for the start face that corresponds
 * to the coedges of the first profile that is used for skinning.<br>
 * <b>Role:</b> This class helps to identify all those coedges (of the first profile)
 * that was used to make the start face. The start face of the skin body, 
 * when constructed if the "solid" skin option set to TRUE, is annotated via this annotation class.<br><br> 
 * The input entities for the start face are the coedges of the first profile. 
 * The output entity is the start face. <br><br>
 * Inputs: <br>
 * All coedges of the first profile - COEDGE <br>
 * Outputs: <br>
 * Start face - FACE<br><br>
 * @see COEDGE, FACE
 */
class DECL_SKIN SKIN_ANNO_START_FACE : public SKIN_ANNOTATION
{
protected:
	/**
	 * @nodoc
	 */
	enum {
		e_coedges,	// Input entities 
		e_faces,	// Output entities 
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
	ENTITY_FUNCTIONS(SKIN_ANNO_START_FACE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	SKIN_ANNO_START_FACE(
		FACE *face = NULL)
	{
		ents[e_coedges] = NULL;	// Input coedges are accumulated via add_coedge()
		ents[e_faces] = face;
	}
	/**
	 * @nodoc
	 */
	void add_coedge(COEDGE* coedge);
	/**
	 * Returns the start face of skinned body. i.e. It returns the face that caps the first profile used for
	 * skinning
	 */
	FACE* face() const
	{ return (FACE *) ents[e_faces]; }
	/**
	 * Returns all the input profile coedges that corresponds to the start (capping) face annotated
	 * by this object. The returned entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* coedges() const
	{ return ents[e_coedges]; }
};

//-----------------------------------------------------------------------
//	Skinning Annotations -- end face
//	Each end face of the skin body, when construced, is annotated via 
//	this annotation class. The input entities for the end faces are the 
//	coedges of the first/last wires as appropriate. The output entities
//	are the end faces themselves. We do not assume that only one end face
//	caps the ends. This way when we upgrade the end capping algorithm we
//	wont be in trouble due to annotations.
//-----------------------------------------------------------------------

/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_END_FACE_TYPE;
/**
 * @nodoc
 */
#define SKIN_ANNO_END_FACE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)
/**
 * Defines the annotation class for the end face that corresponds to the coedges of the 
 * last profile used for skinning.<br>
 * <b>Role:</b> This class helps to identify all the coedges (of the last profile)
 * that was used to make the end face. The end face of the skin body when constructed 
 * if the "solid" skin-option is set to TRUE, is annotated via this annotation class. <br><br>
 * The input entities for the end face are the coedges of the last profile.
 * The output entity is the end face.<br><br>
 * Inputs: <br>
 * All coedges of the last profile - COEDGE <br>
 * Outputs: <br>
 * End face - FACE<br><br>
 * @see COEDGE, FACE
 */
class DECL_SKIN SKIN_ANNO_END_FACE : public SKIN_ANNOTATION
{
protected:
	/**
	 * @nodoc
	 */
	enum {
		e_coedges,	// Input entities 
		e_faces,	// Output entities 
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
	ENTITY_FUNCTIONS(SKIN_ANNO_END_FACE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	SKIN_ANNO_END_FACE(
		FACE *face = NULL)
	{
		ents[e_coedges] = NULL;	// Input coedges are accumulated via add_coedge()
		ents[e_faces] = face;
	}
	/**
	 * @nodoc
	 */
	void add_coedge(COEDGE* coedge);
	/**
	 * Returns the end face of skinned body. i.e. It returns the face that caps the last profile used for
	 * skinning.
	 */
	FACE* face() const
	{ return (FACE *) ents[e_faces]; }
	/**
	 * Returns all the coedges (of the last profile used for skinning) that corresponds
	 * to the end (capping) face annotated by this object. The returned entity
	 * may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* coedges() const
	{ return ents[e_coedges]; }
};
//-----------------------------------------------------------------------
//	Skinning Annotations -- start edge
//	The start coedge of the skin body, corresponding to the starting 
//  transverse face when constructed, is annotated via this annotation class. 
//	The input entities for the start edges are the vertices of the first 
//	wire. The output entities are the start edges themselves.
//-----------------------------------------------------------------------
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_START_EDGE_TYPE;
/**
 * @nodoc
 */
#define SKIN_ANNO_START_EDGE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)
/**
 * Defines the annotation class relating to a edge on the start (capping) face. <br>
 * <b>Role:</b>This class helps to identify the original profile coedge to which an edge of the
 * start face corresponds. Every coedge of the first profile used for skinning, corresponding to the start 
 * face when constructed, is annotated via this annotation class.<br><br>
 * The input entity is a coedge of the first profile used for skinning.
 * The output entity is an edge of the start face.<br><br>
 * Inputs: <br>
 * Coedge of the first profile - COEDGE <br>
 * Outputs: <br>
 * Corresponding edge on the start face - EDGE <br><br>
 * @see EDGE
 */
class DECL_SKIN SKIN_ANNO_START_EDGE : public SKIN_ANNOTATION
{
protected:
	enum {
		e_wire_edges,		// Input entities
		e_start_edge,		// Output entities
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
	ENTITY_FUNCTIONS(SKIN_ANNO_START_EDGE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	SKIN_ANNO_START_EDGE(
		EDGE *edge = NULL)
	{
		ents[e_wire_edges] = NULL;	// Input vertices are accumulated via add_vertex()
		ents[e_start_edge] = edge;		// Output edges are accumulated via add_edge()
	}
	/**
	 * @nodoc
	 */
	void add_wire_edge(COEDGE* coedge);
	/**
	 * Returns the input coedge that corresponds to an edge of the start face, 
	 * which is annotated by this object. The returned entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* wire_edges() const { return ents[e_wire_edges]; }
	/**
	 * Returns the start edge (an edge on the start face) that is annotated by this object. 
	 */
	EDGE* start_edge() const { return (EDGE *) ents[e_start_edge]; }
};
//-----------------------------------------------------------------------
//	Skinning Annotations -- end edge
//	The end coedge of the skin body, corresponding to the ending transverse 
//	face when constructed, is annotated via this annotation class. The input 
//	entities for the end edges are the vertices of the last wire. The output 
//	entities are the end edges themselves.
//-----------------------------------------------------------------------
/**
 * @nodoc
 */
extern DECL_SKIN int SKIN_ANNO_END_EDGE_TYPE;
/**
 * @nodoc
 */
#define SKIN_ANNO_END_EDGE_LEVEL (SKIN_ANNOTATION_LEVEL + 1)

/**
 * Defines the annotation class relating to an edge on the end (capping) face. <br>
 * <b>Role:</b>This class helps to identify the original profile coedge to which an edge
 * of the end face corresponds. Every coedge of the last profile used for skinning, corresponding to the end 
 * face when constructed, is annotated via this annotation class.<br><br>
 * The input entities are coedges of the last profile used for skinning.
 * The output entity is the edge of the end face.<br><br>
 * Inputs: <br>
 * Coedge of the last profile - COEDGE <br>
 * Outputs: <br>
 * Corresponding edge on the end face - EDGE <br>
 * @see EDGE
 */
class DECL_SKIN SKIN_ANNO_END_EDGE : public SKIN_ANNOTATION
{
protected:
	/**
	 * @nodoc
	 */
	enum {
		e_wire_edges,		// Input entities
		e_end_edge,			// Output entity
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
	ENTITY_FUNCTIONS(SKIN_ANNO_END_EDGE, SKIN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
	/**
	 * @nodoc
	 */
	SKIN_ANNO_END_EDGE(
		EDGE * edge = NULL)
	{
		ents[e_wire_edges] = NULL;		// Input vertices are accumulated via add_vertex()
		ents[e_end_edge] = edge;		// Output edges are accumulated via add_edge()
	}
	/**
	 * @nodoc
	 * Input data accumlation. The only way to set input data
	 */
	void add_wire_edge(COEDGE* coedge);
	/**
	 * Returns the input coedge corresponds to an edge on the end face, which
	 * is annotated by this object. The returned entity may actually be an ATTRIB_TAG or an EE_LIST
	 */
	ENTITY* wire_edges() const { return ents[e_wire_edges]; }

	/**
	 * Returns the end edge (an edge on the end face) that is annotated by this object. 
	 */
	EDGE* end_edge() const { return (EDGE *)ents[e_end_edge]; }
};
/** @} */
#endif //__SKINANNO_HXX__

