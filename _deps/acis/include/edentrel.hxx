/* ORIGINAL: acis2.1/sg_husk/query/edentrel.hxx */
// $Id: edentrel.hxx,v 1.14 2002/08/09 17:19:03 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( EDGE_ENTITY_REL_CLASS )
#define EDGE_ENTITY_REL_CLASS

#include "dcl_query.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
#include "sgquertn.hxx"
// ywoo: end

/**
* @file edentrel.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRMAIN
 *  \brief Declared at <edentrel.hxx>
 *  @{
 */

// dummy class declarations to satify the compiler temporarily.
class EDGE;
class ENTITY;
class curve_curve_int;
class curve_surf_int;
class EDGE_EDGE_INT;
class EDGE_FACE_INT;
class EDGE_BODY_INT;

class SPAtransf;


// various enumerations to portray the relation between the given
// edge and the given entity


/**
 * This enumeration describes the relation between two edges
 * @param edges_identical
 * Edges are same in physical space.
 * @param edges_intersect
 * Edges intersect.
 * @param edges_overlap
 * Edges Overlap.
 **/
enum
sg_edge_edge_relation {
	edges_identical,			// Edges are same in physical space
	edges_intersect,			// Edges intersect.
	edges_overlap				// Edges Overlap
};

/**
 * This enumeration describes the relation between an edge and a face
 * @param completely_inside_face
 * Edge lies completely inside the face on the surface.
 * @param partially_inside_face
 * Edge is partially on the face.
 * @param on_face_boundary
 * Edge is on one or more of the boundary edges.
 * @param intersect
 * Edge intersects the face.
 **/
enum
sg_edge_face_relation {
	completely_inside_face,	// Edge lies completely inside the face on the surface.
	partially_inside_face,	// Edge is Partially on the face.
	on_face_boundary,		// edge is on one, or more of the boundaries
	intersect				// edge intersects the face.
};

/**
 * This enumeration describes the relation between an edge and a body
 * @param completely_inside_body
 * Edge is completely inside the body. (Manifold)
 * @param partially_inside_body
 * Edge is partially inside the body.
 * @param on_body_boundary
 * Edge is on one or more boundary faces of the body.
 **/
enum
sg_edge_body_relation {
	completely_inside_body,	// Edge is completely inside the body.( Manifold )
	partially_inside_body,	// Edge is partially inside the body.
	on_body_boundary		// Edge is on the boundary of one or more faces
							// of the body.
};

// Union of the above relations, which can be used to represent relation
// between an edge and any of the below entities:-
// APOINT, EDGE, FACE, BODY.

/**
 * Determines the relationship type.
 */
union
sg_edge_ent_rel_union {
	/**
	* Relation between an edge and a point.
	*/
	sg_edge_point_relation	edge_point_var;

	/**
	* Relation between two edges.
	*/
	sg_edge_edge_relation	edge_edge_var;

	/**
	* Relation between an edge and a face.
	*/
	sg_edge_face_relation	edge_face_var;

	/**
	* Relation between an edge and a body.
	*/
	sg_edge_body_relation	edge_body_var;
};

/**
 * Represents the relationship between an EDGE and an ENTITY.
 * <br>
 * <b>Role:</b> This class represents the relationship between an edge and an
 * entity. The entity must be of the type <tt>BODY</tt>, <tt>FACE</tt>, <tt>EDGE</tt>, or <tt>POINT</tt> only.
 * @see EDGE, ENTITY, curve_curve_int, curve_surf_int
 */
class DECL_QUERY edge_entity_rel : public ACIS_OBJECT {

private:

	edge_entity_rel		*next_ptr;		// Next pointer used to represent
											// relation between a edge and a
											// list of entities.

	EDGE					*edge_ptr;		// Current Edge
	ENTITY					*entity_ptr;	// Current Entity

	//STI let: add four private methods to help the constructor
	// Determine the edge-body, edge-face, edge-edge, & edge-point relations
	void edge_b_rel( EDGE *, const SPAtransf &, const SPAtransf &);
	void edge_f_rel( EDGE *, const SPAtransf &, const SPAtransf &);
	void edge_e_rel( EDGE *, const SPAtransf &, const SPAtransf &);
	void edge_p_rel( EDGE *, const SPAtransf &, const SPAtransf &);
	//STI let: end


public:
/**
 * The variable that indicates that there is no possible relation between the given edge and the given entity.
 */
	logical					no_relation;	// variable which indicates
											// there is no possible relation
											// between the given edge and the
											// given entity.
/**
 * The relationship type, which is determined from the union.
 */
	sg_edge_ent_rel_union	rel_type;		// Relation type, from the union


	// More appropriate information
/**
 * The edge-edge intersection data.
 */
	EDGE_EDGE_INT			*edrel_data;	// edge-edge intersection data
/**
 * The edge-face intersection data.
 */
	EDGE_FACE_INT			*efrel_data;	// edge-face intersection data
/**
 * The edge-body intersection data.
 */
	EDGE_BODY_INT			*ebrel_data;	// edge-body intersection data


	// This information is rather obsolete,  This is still calculated
	// shall be removed in future releases.
/**
 * The curve-curve intersection data.
 */
	curve_curve_int			*ccrel_data;	// Curve curve intersection data.
/**
 * The curve-surface intersection data.
 */
	curve_surf_int			*csrel_data;	// Curve surface intersection data.



	// Constructors

	// Default constructor

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	edge_entity_rel() {
			edge_ptr = NULL;
			entity_ptr = NULL;
			no_relation = TRUE;
			ccrel_data = NULL;
			csrel_data = NULL;
			edrel_data = NULL;
			efrel_data = NULL;
			ebrel_data = NULL;
			next_ptr = NULL;
		}

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param ed
 * edge.
 * @param ent
 * entity.
 * @param next
 * next pointer.
 * @param edge_trans
 * edge transform.
 * @param ent_trans
 * entity transform.
 * @param process_later
 * process relation later. It is recommended to set it TRUE and process the relation using process()
 * instead of processing it in constructor.
 */
	edge_entity_rel( 	EDGE * ed,
						ENTITY * ent,
						edge_entity_rel  *next = NULL,
						const SPAtransf &edge_trans = *(const class SPAtransf *)NULL_REF,
						const SPAtransf &ent_trans = *(const class SPAtransf *)NULL_REF,
						logical process_later = FALSE
			);

	/**
	* Determines the relation between the given edge and the given entity.
	* The output of this is a list of edge_entity_rel's, where each
	* one corresponds to one coedge.  Also, note that the order of 
	* the relations is as per the edge direction and starts at the start of
	* edge.  Both edge and edge/coedge senses are accounted.
	* <br><br>
	* @param edge_trans
	* edge transform.
	* @param ent_trans
	* entity transform.
	*/
	void process(const SPAtransf &edge_trans = *(const class SPAtransf *)NULL_REF,
		const SPAtransf &ent_trans = *(const class SPAtransf *)NULL_REF
	);
	

	// Enquiry functions.
/**
 * Returns the edge in the edge-entity relationship.
 */
	EDGE	*edge()	{ return edge_ptr; }
/**
 * Returns the entity in the edge-entity relationship.
 */
	ENTITY	*entity() { return entity_ptr; }

/**
 * Returns the next pointer in the edge-entity relationship.
 */
	edge_entity_rel *next() { return next_ptr; }


	// Usefull set functions
/**
 * Sets the next pointer in the edge-entity relationship.
 * <br><br>
 * @param next
 * next pointer.
 */
	void set_next( edge_entity_rel *next ) { next_ptr =  next; }
/**
 * Sets the edge in the edge-entity relationship.
 * <br><br>
 * @param edge
 * edge.
 */
	void set_edge( EDGE *edge ) { edge_ptr = edge ; }
/**
 * Sets the entity in the edge-entity relationship.
 * <br><br>
 * @param ent
 * entity.
 */
	void set_entity( ENTITY *ent ) { entity_ptr = ent; }


	// Debug function
/**
 * Writes debug information about <tt>edge_entity_rel</tt> to standard output or the specified file.
 * <br><br>
 * @param fp
 * file name.
 * @param head
 * heading text.
 */
	void debug( FILE *fp, const char *head = NULL );


	// Lose function, destructor
/**
 * Posts a delete bulletin to the bulletin board indicating the instance is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 * This is required because it has underlying entities associated with it.
 */
	void lose();

};

/** @} */

#endif
