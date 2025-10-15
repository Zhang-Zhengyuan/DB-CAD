/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// 22-Jul-2003 nay: Added set_rel() into shell_lump class. (72184)
/*******************************************************************/

// Header file defining data structures private to the Boolean
// operator code, but required by more than one phase.

#if !defined( BOOL_ATTRIB )
#define BOOL_ATTRIB

#include "logical.h"

#include "acis.hxx"
#include "dcl_bool.h"

#include "at_sys.hxx"

#include "tophdr.hxx"
#include "shell.hxx"

#include "position.hxx"

/**
* @file at_bool.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class ENTITY_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_INTCOED, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_INTEDGE, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_INTVERT, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_INTGRAPH, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Intersection graph attributes linking graph entities with the
// relevant body entities.

// The wires of the graph consist of coedges, edges and vertices,
// together with their geometries. Each of these topological entites
// carries exactly one attribute, recording information relevant to
// its role in the later stages of Booleans. All these attributes
// are cleaned out during the latter stages of Booleans, as they cease
// to be useful.

// Attribute for linking intersection coedges with the body faces to
// which they will become attached, and for specifying the face
// containment value in the neighbourhood of the coedge.

class FACE;
class COEDGE;

// Classification values for the adjacent face with respect to the
// other body.

// **** Note that the number of values and their order is assumed by
// **** bool3/bool3.cc, as in that file a face_body_rel is used as an
// **** index in an array of logicals. Any change here must be
// **** reflected in that file.

/**
* Specifies the relationship between face and body entities.
* @param face_body_unknown
* face or body is unknown.
* @param face_body_inside
* face or body is inside.
* @param face_body_outside
* face or body is outside.
* @param face_body_symmetric
* face or body is symmetric.
* @param face_body_antisymmetric
* face or body is antisymmetric.
* @param face_body_retain
* retain face or body.
* @param face_body_discard
* discard face or body.
* @param face_body_inside_maybe
* "soft" value that can be superseded.
* @param face_body_outside_maybe
* "soft" value that can be superseded.
**/

enum face_body_rel {
	face_body_unknown,
	face_body_inside,
	face_body_outside,
	face_body_symmetric,
	face_body_antisymmetric,
	face_body_retain,	// used for non-Boolean purposes to mean
						// "retain this face whatever the operation".
	face_body_discard,	// the converse of "face_retain"
	face_body_inside_maybe,  //  "Soft" values which can be superceded
	face_body_outside_maybe
};

//  Function to print equivalent strings for enums for use in debug output.
// tbrv
/**
 * @nodoc
 */
DECL_BOOL const char* face_body_rel_text(face_body_rel);

// Classification values for the intersection coedge.
// tbrv
/**
 * @nodoc
 */
enum coedge_type {
	edge_class,			// coincident with a body edge
	boundary_class,		// starts on an edge, may or may not
						// end on an edge, otherwise properly within
						// the face
	face_class,			// starts properly within its face, but
						// may end on an edge
	unknown_class		// what it says
};

// Indicate the current confidence level in the face-body containment.
// tbrv
/**
 * @nodoc
 */
enum face_body_conf {
	face_body_unset,		// no information yet known
	face_body_unconfirmed,	// some containment known, but it
							// may be contradicted by later
							// values
	face_body_confirmed		// containment evaluated globally
							// as a result of a contradiction while
							// unconfirmed. The containment may be
							// "unknown" to indicate that later
							// evaluation is mandatory.
};

/**
* @nodoc
*/
enum edge_degen_type
{
	degen_at_start,
	degen_at_end,
	not_degen
};

// tbrv
/**
 * @nodoc
 */
extern DECL_BOOL int ATTRIB_INTCOED_TYPE;
#define ATTRIB_INTCOED_LEVEL (ATTRIB_SYS_LEVEL + 1)
/**
 * Defines an attribute for linking intersection graph entities with the relevant body entities.
 * <br>
 * <b>Role:</b> Defines an attribute for linking intersection coedges with body faces to which
 * they will attach. It is private to the Boolean operator code, but is required by more than
 * one phase.<br><br>
 * The wires of the graph consist of coedges, edges and vertices, together with their geometries.
 * Each of these entities carries exactly one attribute, recording information relevant to its
 * role in the later stages of Booleans. All these attributes are cleaned out during the latter
 * stages of Booleans, as they cease to be useful.<br><br>
 * <tt>ATTRIB_INTCOED</tt> is attached to each intersection coedge (in both blank and tool wires), and
 * contains:<br><br>
 * - A pointer to the face on the body that the coedge corresponds.<br>
 * - The relationship between the portion of that face to the left of the coedge and in its
 * neighborhood, and the surface of the other body. In this context, left refers to a coordinate
 * system that the coedge direction is forward and the normal to the face is upward. (Throughout
 * ACIS the face that a coedge is attached is on its left.) This relationship is of an enumerated
 * type, and takes the values:
 * <br><br>
 * <table width="80%">
 * <tr><td width="25%" valign="top"><tt>face_body_inside</tt></td><td> - Specifies the face/body is inside.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_body_outside</tt></td><td> - Specifies the face/body is outside.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_body_retain</tt></td><td> - Used for non-Boolean purposes to retain the operation.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_body_discard</tt></td><td> - Used for non-Boolean purposes to discard the operation.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_body_symmetric</tt></td><td> - If face lies in the surface of the other body, specifies that the normals
 *                                             are in opposite directions.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_body_antisymmetric</tt></td><td> - If face lies in the surface of the other body, specifies that the
 *                                                 normals are in the same direction.</td></tr>
 * </tr>
 * </table>
 * <br>
 * - A classification of the coedge with respect to the face that it lies; this is another enumerated
 * type, and takes the values:
 * <br><br>
 * <table width="80%">
 * <tr><td width="25%" valign="top"><tt>edge_class</tt></td><td> - The coedge lies wholly on boundary of face.</td></tr>
 * <tr><td width="25%" valign="top"><tt>boundary_class</tt></td><td> - The coedge lies within the face, but its start vertex lies on the boundary.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_class</tt></td><td> - The coedge lies within the face, and its start vertex is properly within the face.</td></tr>
 * </table>
 * <br>
 * - A pointer to a coedge on the body face, the exact meaning depending upon the coedge
 * classification:
 * <br><br>
 * <table width="80%">
 * <tr>
 * <tr><td width="25%" valign="top"><tt>edge_class</tt></td><td> - Corresponding coedge of the edge that this coedge lies.</td></tr>
 * <tr><td width="25%" valign="top"><tt>boundary_class</tt></td><td> - The coedge lies on body face passing through the start vertex of intersection
 *                                        coedge. If this start vertex lies on a vertex of the face, the coedge of the face that starts
 *                                        at this vertex is chosen.</td></tr>
 * <tr><td width="25%" valign="top"><tt>face_class</tt></td><td> - Pointer to an attribute on a coedge on this same geometric edge that has a body
 *                                    face coincident with this coedge's. A flag is set <tt>TRUE</tt> when the body face of
 *                                    this coedge is processed.</td></tr>
 * </table>
 * @see COEDGE, ENTITY
 */
class DECL_BOOL ATTRIB_INTCOED: public ATTRIB_SYS {
private:
	ENTITY *body_entity_ptr;// Normally the face of the body on which
							// this coedge lies, but the edge if this
							// part of the graph arises from a wire.

	face_body_rel face_rel_data;
							// Containment of this face with respect
							// to the other body.

	coedge_type type_data;	// Classification of the intersection
							// edge with respect to the current body

	COEDGE *body_coedge_ptr;// If the start vertex of this coedge
							// lies on an edge of the face, points to
							// the coedge linking the edge to the face.
							// If the start vertex is at a vertex of
							// the face, this coedge is the one which
							// starts at the vertex.

	ATTRIB_INTCOED *coin_attrib_ptr;
							// If the face relationship indicates a
							// coincidence (face_body_symmetric or
							// _antisymmetric), this points to the
							// attribute attached to the coedge on the
							// same geometric edge associated with the
							// corresponding face on the other body.
							// Otherwise this pointer is meaningless
							// (and is normally NULL).

	logical face_seen_data;	// Used during Boolean Stage 1, this is
							// initially FALSE, but is set TRUE when
							// the body face to which this attribute
							// refers is processed. At the end of
							// Stage 1, if there are attributes
							// remaining which haven't been processed,
							// the containments of the other body
							// faces are unreliable, and so must be
							// evaluated explicitly.

	face_body_conf conf_data;// Indicates the confidence to be placed
							// in the value of face_rel_data during
							// the construction of the intersection
							// graph.

	//  Field notification of rollbacks, to allow maintenance of
	//  a list of outstanding objects of this class, used for
	//  automatic cleanup of decorated bodies.
	void roll_notify( BULLETIN_TYPE, ENTITY * );

public:
/**
 * Returns <tt>TRUE</tt> if this can be deep copied.
 */
    virtual logical pattern_compatible() const;
	virtual logical savable() const;
                                    // returns TRUE
	// Simple read functions for the data members.
/**
 * Returns the face of the body on which this coedge lies.
 */
	FACE *face() const;
/**
 * Pick out an edge entity.
 */
	EDGE *edge() const;
/**
 * Pick out a body entity.
 */
	ENTITY *body_entity() const { return body_entity_ptr; }
/**
 * Returns the containment of this face with respect to the other body.
 */
	face_body_rel face_rel() const { return face_rel_data; }
/**
 * Returns the classification of the intersection edge with respect to the current body.
 */
	coedge_type type() const { return type_data; }
/**
 * Points to the coedge linking the edge to the face if the start vertex of this coedge lies on an edge of the face.
 * <br><br>
 * <b>Role:</b> If the start vertex is at a vertex of the face, this coedge is the one
 * that starts at the vertex.
 */
	COEDGE *body_coedge() const { return body_coedge_ptr; }
/**
 * Points to the attribute attached to the coedge.
 * <br><br>
 * <b>Role:</b> Points to the attribute attached to the coedge on the same geometric
 * edge associated with the corresponding face on the other body if the face
 * relationship indicates a coincidence (<tt>face_body_symmetric</tt> or <tt>face_body_antisymmetric</tt>);
 * otherwise, the pointer is meaningless and returns <tt>NULL</tt>.
 */
	ATTRIB_INTCOED *coin_attrib() const { return coin_attrib_ptr; }
/**
 * Used during Boolean Stage 1.
 * <br><br>
 * <b>Role:</b> Returns <tt>FALSE</tt>, but it is <tt>TRUE</tt> when the body
 * face to which this attribute refers is processed. At the end of Stage 1, if
 * there are attributes remaining that have not been processed,the containments
 * of the other body faces are unreliable, and so must be evaluated explicitly.
 */
	logical face_seen() const { return face_seen_data; }
/**
 * Simple read function to return the data associated with the confidence to be placed in the value of <tt>face_rel_data</tt> during the construction of the intersection graph.
 */
	face_body_conf conf() const { return conf_data; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.
/**
 * Sets the face adjacent to the graph coedge.
 * <br><br>
 * @param new_face
 * new face.
 */
	void set_face( FACE *new_face );
/**
 * Sets a new edge.
 * <br><br>
 * @param new_edge
 * edge name.
 */
	void set_edge( EDGE *new_edge );
/**
 * Sets a new body entity.
 * <br><br>
 * @param new_entity
 * entity name.
 */
	void set_body_entity( ENTITY *new_entity );
/**
 * Sets the relationship of the coedge attribute.
 * <br><br>
 * @param new_rel
 * new relationship.
 * @param conf
 * confirmation?
 */
	void set_face_rel(
				face_body_rel new_rel,
				face_body_conf conf = face_body_unconfirmed
			);
/**
 * Sets the graph coedge type.
 * <br><br>
 * The graph coedge type is one of the following: <tt>edge_class</tt> if it lies
 * on the edge of the face, <tt>boundary_class</tt> if it starts on the boundary,
 * or <tt>face_class</tt> if it starts in the interior.
 * <br><br>
 * @param graph_coedge
 * coedge type.
 */
	void set_type( coedge_type graph_coedge );
/**
 * Sets a new coedge pointer after the edge split occurs in <tt>bool2</tt>.
 * <br><br>
 * @param new_coedge
 * new coedge.
 */
	void set_body_coedge( COEDGE *new_coedge );
/**
 * Sets a new coincident face attribute pointer.
 * <br><br>
 * @param new_attrib
 * ATTRIB_INTCOED.
 */
	void set_coin_attrib( ATTRIB_INTCOED *new_attrib, bool partner = false );
/**
 * Marks this coedge's body face as having been processed.
 */
	void set_face_seen();
	// STL amt
/**
 * Resets pointers to entities in the first list to the corresponding entities in the second list.
 * <br><br>
 * @param old_ents
 * old entity list.
 * @param new_ents
 * new entity list.
 */
	void transfer(
								const ENTITY_LIST& old_ents,
								const ENTITY_LIST& new_ents);

	// Main constructor.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTCOED(...))</tt>, because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param coedge_owner
 * coedge owner.
 * @param adj_ent
 * adjacent entity.
 * @param adj_rel
 * adjacent relationship.
 * @param adj_edge
 * adjacent coedge.
 * @param adj_type
 * adjacent type.
 */

	ATTRIB_INTCOED(
				COEDGE *coedge_owner = NULL,
				ENTITY *adj_ent = NULL,
				face_body_rel adj_rel = face_body_unknown,
				COEDGE *adj_edge = NULL,
				coedge_type adj_type = unknown_class
			);

	// Constructor to duplicate an existing attribute when a coedge
	// is split.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTCOED(...))</tt>, because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param co_edge
 * coedge.
 * @param att_intcoed
 * ATTRIB_INTCOED.
 */
	ATTRIB_INTCOED(
				COEDGE *co_edge,
				ATTRIB_INTCOED *att_intcoed
			);

	// Standard attribute function definitions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_INTCOED, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Attribute for linking intersection edges with the intersecting
// entities.

class EDGE;

// tbrv
/**
 * @nodoc
 */
extern DECL_BOOL int ATTRIB_INTEDGE_TYPE;
#define ATTRIB_INTEDGE_LEVEL (ATTRIB_SYS_LEVEL + 1)
/**
 * Defines an attribute for linking intersection edges with the intersecting entities.
 * <br>
 * <b>Role:</b> This class defines an attribute for linking intersection edges with the
 * intersecting entities. This is a private class to the Boolean operator code, but is
 * required by more than one phase.<br>
 * <tt>ATTRIB_INTEDGE</tt> is attached to each intersection edge, and contains the following
 * information for each body (<tt>this_body</tt> and <tt>other_body</tt>, meaning at this stage blank body
 * and tool body respectively):<br><br>
 * - A pointer to the entity on the body that this edge corresponds; an <tt>EDGE</tt> if it lies
 * coincident with some part of that edge, otherwise the face in which it lies.<br>
 * - The sense relating the intersection edge and the body edge if the body entity is an
 * <tt>EDGE</tt>. This sense is <tt>FORWARD</tt> if the two edges are in the same direction, <tt>REVERSED</tt> if they
 * are in opposite directions.<br>
 * - A pointer to one of the coedges belonging to this edge in the wire corresponding to the
 * tool body.<br>
 * - A pointer to a <i>partner</i> <tt>ATTRIB_INTEDGE</tt> attribute, which at this stage must be <tt>NULL</tt>.
 * The following functions are defined for <tt>ATTRIB_INTEDGE</tt>.
 * @see COEDGE
 */
class DECL_BOOL ATTRIB_INTEDGE: public ATTRIB_SYS {
private:
	struct {
		ENTITY *entity;			// a face or an edge
		REVBIT sense;			// only used if entity is an edge
	} this_body_data,	// data belonging to body owning this graph,
						// in early stages the blank body
	  other_body_data;	// same for the other body involved.

	// Pointer to the coedges relating to the tool body, transferred
	// to the partner edge when this is constructed.

	COEDGE *tool_coedge_ptr;

	// Pointer to the corresponding attribute on the graph for the other body,
	// NULL at first when there is only the one.

	ATTRIB_INTEDGE *partner_ptr;

	// Flag to indicate that this graph edge has derived from a fuzzy
	// region on an edge of one or both bodies. If so, containments
	// will be unreliable, and so should be derived from coherence
	// with adjacent edges.

	logical fuzzy_int_data;	

	edge_degen_type ed_type = not_degen;

public:

	edge_degen_type get_ed_type() { return ed_type; }

	void set_ed_type( edge_degen_type type ) { ed_type = type; }

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
/**
 * Returns the edge for this body.
 */
	ENTITY *this_entity() const { return this_body_data.entity; }
/**
 * Returns the sense data for this body.
 */
	REVBIT this_sense() const { return this_body_data.sense; }
/**
 * Returns the edge for the other body.
 */
	ENTITY *other_entity() const { return other_body_data.entity; }
/**
 * Returns the sense data for the other body.
 */
	REVBIT other_sense() const { return other_body_data.sense; }
/**
 * Sets whether this graph edge derives from a fuzzy region on an edge of one or both bodies.
 * <br><br>
 * <b>Role:</b> If so, containments are unreliable and should be derived from coherence with
 * adjacent edges.
 */
	logical fuzzy_int() const { return fuzzy_int_data; }

	// Pointer to the coedges relating to the tool body, transferred
	// to the partner edge when this is constructed.
/**
 * Points to the coedges relating to the tool body, transferred to the partner edge when this is constructed.
 */
	COEDGE *tool_coedge() const { return tool_coedge_ptr; }

	// Pointer to the corresponding attribute on the graph for the other body,
	// NULL at first when there is only the one.
/**
 * Points to the corresponding attribute on the graph for the other body.
 * <br><br>
 * <b>Role:</b> This value is <tt>NULL</tt> when there is only one.
 */
	ATTRIB_INTEDGE *partner() const { return partner_ptr; }

	// Functions for setting selected data.
/**
 * Sets the data associated with this body.
 * <br><br>
 * @param new_ent
 * new entity.
 * @param new_sense
 * new sense data.
 */
	void set_this_body( ENTITY *new_ent, REVBIT new_sense);
/**
 * Sets the data associated with the other body.
 * <br><br>
 * @param new_ent
 * new entity.
 * @param new_sense
 * new sense data.
 */
	void set_other_body( ENTITY *new_ent, REVBIT new_sense);
/**
 * Sets the tool coedge pointer.
 * <br><br>
 * @param new_coedge
 * new coedge.
 */
    void set_tool_coedge( COEDGE *new_coedge );
/**
 * Sets whether this graph edge derives from a fuzzy region on an edge of one or both bodies.
 * <br><br>
 * <b>Role:</b> Once set, this flag does not need to be reset.
 */
	void set_fuzzy_int();
/**
 * Resets pointers to entities in the first list to the corresponding entities in the second list.
 * <br><br>
 * @param old_entlist
 * old entity list.
 * @param new_entlist
 * new entity list.
 */
	// STL amt
	void transfer(
								const ENTITY_LIST& old_entlist,
								const ENTITY_LIST& new_entlist);

	// Constructor to duplicate an existing attribute when constructing
	// the duplicate wire. The new and the old attributes become
	// partners of each other.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTEDGE(...))</tt>, because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param edge_owner
 * edge owner.
 * @param atr_copy
 * copy.
 */
	ATTRIB_INTEDGE( EDGE *edge_owner = NULL, ATTRIB_INTEDGE *atr_copy = NULL );

	// Standard attribute functions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_INTEDGE, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	* @nodoc	
	*/
		friend class bgraph_attrib_query;

	/**
	 * @nodoc
	 */
		friend class bgeom_origins_manager;
	
private:

	// For internal use only

	virtual void copy_owner( ENTITY *copy_ent );

	virtual void to_tolerant_owner( ENTITY *tol_ent );
	
	unsigned _geom_origins : 2;
};

// Attribute for linking graph vertices with the intersection record(s)
// giving rise to them. Where there are several, one from each body
// is chosen, as the necessary information is recorded in all.

class VERTEX;

class edge_face_int;

// tbrv
/**
 * @nodoc
 */
extern DECL_BOOL int ATTRIB_INTVERT_TYPE;
#define ATTRIB_INTVERT_LEVEL (ATTRIB_SYS_LEVEL + 1)

/**
 * Defines an attribute for linking graph vertices with the intersection record(s) giving rise to them.
 * <br>
 * <b>Role:</b> This class defines an attribute for linking graph vertices with the
 * intersection record(s) giving rise to them. Where there are several, one from each
 * body is chosen, as the necessary information is recorded in all.<br><br>
 * <tt>ATTRIB_INTVERT</tt> is attached to each intersection vertex, and contains the following
 * information for each body (as for <tt>ATTRIB_INTEDGE</tt> attributes):<br><br>
 * - A pointer to the entity on the body that the vertex corresponds - a <tt>VERTEX</tt> if it
 * lies coincident with that body vertex, an <tt>EDGE</tt> if it lies on that edge and not at either
 * end, or <tt>NULL</tt> if it lies properly within a face.<br>
 * - The parameter value along the edge, if the entity pointed to is an <tt>EDGE</tt>, undefined otherwise.<br>
 * - A pointer to a partner <tt>VERTEX</tt> (not to an <tt>ATTRIB_INTVERT</tt> attribute), which at this stage must
 * be <tt>NULL</tt>.<br>
 * @see VERTEX
 */
class DECL_BOOL ATTRIB_INTVERT: public ATTRIB_SYS {
private:
	struct {
		ENTITY *entity;			// an edge or a vertex
		double edge_param;		// only used if entity is an edge,
								// gives the SPAparameter value of the
								// point along the edge.
		edge_face_int *ef_int;	// the primary edge-face intersection
								// record for this vertex in this body.
								// Only meaningful during the
								// construction of the intersection
								// graph (bool1).
		// STI TOL_MOD jmg: added for tolerant modeling so we can split the coedges.
		COEDGE *coedge_ptr;// Coedge used to create the vertex
						   // only used if entity is a tedge.
		// STI TOL_MOD end:
	} this_body_data,	// data belonging to body owning this graph,
						// in early stages the blank body
	  other_body_data;	// same for the other body involved.

	VERTEX *partner_ptr;// Vertex created on duplicate graph.
						// only used during graph duplication.
    //nadhikary May 02
	//This info we need in bool2 at the time of splitting an edge.
    logical body_edge_crumble;
    logical body_vertex_crumble;

public:
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Data reading
/**
 * Returns this entity, which is an edge or a vertex.
 */
	ENTITY *this_entity() { return this_body_data.entity; }
/**
 * Returns this edge parameter.
 * <br><br>
 * <b>Role:</b> This is used only if entity is an edge because it gives the parameter
 * value of the point along the edge.
 */
	double this_edge_param() { return this_body_data.edge_param; }
/**
 * Returns this edge-face intersection.
 * <br><br>
 * <b>Role:</b> The edge-face intersection is the primary edge-face intersection record
 * for this vertex in this body. This is only meaningful during the construction of the
 * intersection of the graph (<tt>bool1</tt>).
 */
	edge_face_int *this_ef_int() { return this_body_data.ef_int; }
/**
 * Returns a pointer to this coedge.
 */
	COEDGE *this_coedge() { return this_body_data.coedge_ptr; }
/**
 * Returns the other entity, which is an edge or a vertex.
 */
	ENTITY *other_entity() { return other_body_data.entity; }
/**
 * Returns the other edge parameter.
 * <br><br>
 * <b>Role:</b> This is used only if entity is an edge because it gives the parameter
 * value of the point along the edge.
 */
	double other_edge_param() { return other_body_data.edge_param; }
/**
 * Returns the other edge-face intersection.
 * <br><br>
 * <b>Role:</b> The edge-face intersection is the primary edge-face intersection record
 * for this vertex in this body. This is only meaningful during the construction of the
 * intersection of the graph (<tt>bool1</tt>).
 */
	edge_face_int *other_ef_int() { return other_body_data.ef_int; }
/**
 * Returns a pointer to the other coedge.
 */
	COEDGE *other_coedge() { return other_body_data.coedge_ptr; }
/**
 * Returns the vertex created on the duplicate graph.
 */
	VERTEX *partner() { return partner_ptr; }

/**
 * Moves this <tt>ATTRIB_INTVERT</tt> to a new owning <tt>ENTITY</tt>.
 * <br><br>
 * @param new_entity
 * the new owning ENTITY.
 */
	ATTRIB *move( ENTITY *new_entity );

// tbrv
/**
 * @nodoc
 */
	logical get_body_edge_crumble_info();
/**
 * @nodoc
 */
    logical get_body_vertex_crumble_info();

    // Selected modifications. Each function ensures that the
	// attribute is backed up before any change.
/**
 * Sets the properties of the body owning this graph; in the early stages, this is the blank body.
 * <br><br>
 * @param ent
 * this entity.
 * @param edge_para
 * this edge parameter.
 * @param edge_face
 * this edge-face int.
 */

	void set_this_body( ENTITY *ent, double edge_para, edge_face_int *edge_face );
/**
 * Sets the properties of the other body involved in the intersection.
 * <br><br>
 * @param ent
 * other entity.
 * @param edge_para
 * other edge parameter.
 * @param edge_face
 * other edge-face int.
 */
	void set_other_body( ENTITY *ent, double edge_para, edge_face_int *edge_face );
/**
 * Sets the vertex created on a duplicate graph, which is used only during graph duplication.
 * <br><br>
 * @param ver
 * vertex.
 */
	void set_partner( VERTEX *ver );
/**
 * Kills this edge-face intersection.
 * <br><br>
 * <b>Role:</b>  The edge-face intersection is the primary edge-face intersection record for
 * this vertex in this body. This is only meaningful during the construction of the intersection
 * of the graph (<tt>bool1</tt>).
 */
	void kill_this_ef_int();
/**
 * Kills the other edge-face intersection.
 * <br><br>
 * <b>Role:</b>  The edge-face intersection is the primary edge-face intersection record for
 * this vertex in this body. This is only meaningful during the construction of the intersection
 * of the graph (<tt>bool1</tt>).
 */
	void kill_other_ef_int();
/**
 * Sets the pointer to this coedge.
 * <br><br>
 * @param new_coedge
 * new coedge.
 */
	void set_this_coedge( COEDGE *new_coedge );
/**
 * Sets the pointer to the other coedge.
 * <br><br>
 * @param new_coedge
 * new coedge.
 */
	void set_other_coedge( COEDGE *new_coedge );
// tbrv
/**
 * @nodoc
 */
    void set_body_edge_crumble_info(logical);
// tbrv
/**
 * @nodoc
 */
    void set_body_vertex_crumble_info(logical);

	// STL amt
/**
 * Resets pointers to entities in the first list to the corresponding entities in the second list.
 * <br><br>
 * @param old_ents
 * old entity list.
 * @param new_ents
 * new entity list.
 */
	void transfer(
								const ENTITY_LIST& old_ents,
								const ENTITY_LIST& new_ents);


	// Constructor for all the data.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new
 * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTVERT(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll back and
 * history management.
 * <br><br>
 * @param ver
 * vertex owner.
 * @param th_ent
 * this entity.
 * @param th_edge
 * this edge parameter.
 * @param th_edge_face
 * this edge-face int.
 * @param ot_ent
 * other entity.
 * @param ot_edge
 * other edge parameter.
 * @param ot_edge_face
 * other edge-face int.
 * @param th_coedge
 * this coedge.
 * @param ot_coedge
 * other coedge.
 * @param log
 * logical.
 * @param bvc
 * logical.
 */
	ATTRIB_INTVERT(
				VERTEX *ver = NULL,
				ENTITY *th_ent = NULL,
				double th_edge = 0,
				edge_face_int *th_edge_face = NULL,
				ENTITY *ot_ent = NULL,
				double ot_edge = 0,
				edge_face_int *ot_edge_face = NULL,
				COEDGE *th_coedge = NULL,
				COEDGE *ot_coedge = NULL,
				logical log = FALSE,
                logical bvc = FALSE
			);

	// Duplication constructor - the new owner is the partner of the
	// old attribute.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTVERT(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param exist_atr
 * existing attribute.
 */
	ATTRIB_INTVERT(
				ATTRIB_INTVERT *exist_atr
			);

	// Standard attribute functions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_INTVERT, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	* @nodoc
	*/

	friend class bgraph_attrib_query;

	/**
	* @nodoc
	*/

	friend class bgeom_origins_manager;

private:

	// For internal use only

	virtual void copy_owner( ENTITY *copy_ent );

	virtual void to_tolerant_owner( ENTITY *tol_ent );

	/**
	* @nodoc
	*/

	unsigned _geom_origins : 2;
};

// Attribute for recording the classification of shells which do not
// contribute to any intersection wires. It contains a list of entries
// for each body, each entry pointing to a shell of that body which
// lies entirely within a lump of the other body, and also to that
// lump. This attribute is attached to the BODY which we call the
// "intersection graph" on return from Booleans stage 1.

class SHELL;
class LUMP;
class BODY;

// Record definition for the shell-in-lump list. For each shell in each
// constituent body there is one of these records (except that for
// shells entirely outside the other body there need not be a
// corresponding record). The face-body relationship specifies the
// containment relationship between the shell and the other body - it
// is "unknown" if the shell intersects with a shell of the other body.
// For shells wholly coincident with shells on the other body, and
// consisting of faces on a single surface, there is no intersection
// wire, and so the relationship is recorded here as "symmetric" or
// "antisymmetric" as appropriate. Other coincidences come through as
// intersections, and so are handled through the intersection wire.
// Here "shell" may also mean "wire" where the object being recorded is
// a "traditional" wire contained directly in a body, with no associated
// shell.

// Where a shell coincident with one of the other body's shells, a
// pointer to that shell is also recorded; if it is entirely inside
// a lump of the other body, a pointer to one of its shells is stored.
// For an intersecting shell, or one outside the other body (if
// recorded in this list) the "other shell" pointer is NULL.

/**
 * Records the classification of shells or wires that do not contribute to any intersection.
 * <br>
 * <b>Role:</b> This class records the definition for the shell-in-lump list. For each shell
 * in each constituent body, there is one of these records (except that for shells entirely
 * outside the other body there need not be a corresponding record). The face-body relationship
 * specifies the containment relationship between the shell and the other body; it is "unknown"
 * if the shell intersects with a shell of the other body. For shells wholly coincident with
 * shells on the other body and consisting of faces on a single surface, there is no intersection
 * wire, and so the relationship is recorded here as "symmetric" or "antisymmetric" as appropriate.
 * Other coincidences come through as intersections, and so are handled through the intersection wire.
 * <br><br>
 * Where a shell is inside a lump, or coincident with one of its shells, a pointer to that lump
 * (there can be only one) is recorded here. For an intersecting shell, or one outside the other
 * body (if recorded in this list) the lump pointer is <tt>NULL</tt>.
 * <br><br>
 * This class is private to the boolean operator code, but is required by more than one phase.
 * @see ATTRIB_INTGRAPH, ENTITY
 */
class DECL_BOOL shell_lump : public ACIS_OBJECT {
private:
	shell_lump *next_ptr;
	ENTITY *entity_ptr;			// shell or wire being described
	logical blank_entity_data;	// TRUE if the shell or wire is in the
								// blank body, FALSE if it is in
								// the tool.
	ENTITY *other_entity_ptr;	// shell or wire of the other body
								// coincident with this shell or wire,
								// or the lump of the other body inside
								// which the shell lies. NULL if the shell
								// is outside everything, or there is
								// one or more intersection wire
								// detailing its interaction.
	face_body_rel rel_data;		// relationship of the shell to the
								// lump(s) of the other body.

public:
	// Read the data.

/**
 * Returns a pointer to the next <tt>shell_lump</tt> in the list of <tt>shell_lumps</tt>.
 */
	shell_lump *next() const { return next_ptr; }
/**
 * Return the entity being described if it is a shell.
 * <br><br>
 * <b>Role:</b> If the entity is a wire, return its shell; otherwise, return <tt>NULL</tt>.
 */
	SHELL *shell() const;
/**
 * Return the entity being described if it is a <tt>WIRE</tt>; otherwise, return <tt>NULL</tt>.
 */
	WIRE *wire() const;
/**
 * Return the shell or wire being described.
 */
	ENTITY *entity() const { return entity_ptr; }
/**
 * Returns <tt>TRUE</tt> if the shell belongs to the blank body.
 * <br><br>
 * <b>Role:</b> It returns <tt>FALSE</tt> if the shell belongs to the tool body.
 */
	logical blank_shell() const { return blank_entity_data; }
/**
 * Returns <tt>TRUE</tt> if the shell or wire belongs to the blank body, <tt>FALSE</tt> if it belongs to the tool body.
 */
	logical blank_entity() const { return blank_entity_data; }
/**
 * Returns the shell of the other body that is coincident with this shell or one shell of the lump of the other body inside which the shell lies.
 * <br><br>
 * <b>Role:</b> This method returns <tt>NULL</tt> if the shell is outside everything, or there is one or
 * more intersection wire detailing its interaction.
 */
	SHELL *other_shell() const;
/**
 * Return the other entity being described if it is a <tt>LUMP</tt>; otherwise, return <tt>NULL</tt>.
 */
	LUMP *other_lump() const;
/**
 * Return the other entity being described if it is a <tt>WIRE</tt>; otherwise, return <tt>NULL</tt>.
 */
	WIRE *other_wire() const;
/**
 * Return the other shell or wire.
 */
	ENTITY *other_entity() const { return other_entity_ptr; }
/**
 * Returns a pointer to the lump of the other body.
 * <br><br>
 * <b>Role:</b> Same as <tt>other_lump</tt>.
 */
	LUMP *lump() const;			// historical use to mean other_lump() or
							    // other_shell()->lump()
/**
 * Normally the information in a <tt>shell_lump</tt> is not changed once it has been initialized, but chop sometimes splits lumps, thus requiring the information to be updated.
 * <br><br>
 * @param lump_update
 * lump to be updated.
 */
	void reset_lump(LUMP *lump_update);
/**
 * Returns the relationship of the shell to the lumps of the other body.
 */
	face_body_rel rel() const { return rel_data; }
   // STL amt
	void set_rel(face_body_rel rel) { rel_data = rel; } //nay
/**
 * Resets pointers to entities in the first list to the corresponding entities in the second list.
 * <br><br>
 * @param old_ents
 * old entity list.
 * @param new_ents
 * new entity list.
 */
	void transfer(
								const ENTITY_LIST& old_ents,
								const ENTITY_LIST& new_ents);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param shl_lump
 * shell-lump list.
 * @param shl_des
 * shell being described.
 * @param blank_tool
 * TRUE if blank-body shell,FALSE if tool-body shell.
 * @param ot_ent
 * other entity.
 * @param shl_rel
 * shell-lump relationship.
 */
	shell_lump(
			shell_lump *shl_lump,
			SHELL *shl_des,
			logical blank_tool,
			LUMP * ot_ent = NULL,
			face_body_rel shl_rel = face_body_unknown
		);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param shl_lump
 * shell-lump list.
 * @param shl_des
 * shell or wire being described.
 * @param blank_tool
 * TRUE if blank-body shell,FALSE if tool-body shell.
 * @param eot_ent
 * other entity.
 * @param shl_rel
 * shell-lump relationship.
 */
	shell_lump(
			shell_lump *shl_lump,
			ENTITY *shl_des,
			logical blank_tool,
			ENTITY *eot_ent = NULL,
			face_body_rel shl_rel = face_body_unknown
		);

// tbrv
/**
 * @nodoc
 */
	friend class ATTRIB_INTGRAPH;
};

// tbrv
/**
 * @nodoc
 */
extern DECL_BOOL int ATTRIB_INTGRAPH_TYPE;
#define ATTRIB_INTGRAPH_LEVEL (ATTRIB_SYS_LEVEL + 1)

/**
 * Defines an attribute for classifying shells and lumps of two bodies participating in a Boolean operation.
 * <br>
 * <b>Role:</b> This class maintains a linked list of shell-lump objects. It is private to the Boolean
 * operator code, but is required by more than one phase.
 * @see shell_lump
 */
class DECL_BOOL ATTRIB_INTGRAPH: public ATTRIB_SYS {
private:
	shell_lump *sl_list_ptr;

public:
/**
 * Returns a shell lump list.
 */
	shell_lump *sl_list() { return sl_list_ptr; }
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_INTGRAPH(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param body_name
 * body name.
 * @param lump_list
 * shell lump list.
 */
	ATTRIB_INTGRAPH(
				BODY * body_name = NULL,
				shell_lump * lump_list = NULL
			);
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_INTGRAPH, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	// Change the containments in the shell-lump list to reflect
	// a negated tool body.
/**
 * Change the containments in the shell-lump list to reflect a negated tool body.
 */
	void negate_tool_cont();
};

#endif
