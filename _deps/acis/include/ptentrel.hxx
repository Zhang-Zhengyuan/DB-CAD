/* ORIGINAL: acis2.1/sg_husk/query/ptentrel.hxx */
/* $Id: ptentrel.hxx,v 1.11 2002/08/09 17:19:03 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( POINT_ENTITY_REL_CLASS )
#define POINT_ENTITY_REL_CLASS

#include "dcl_query.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

/**
* @file ptentrel.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPICONTAINMENT
 *  \brief Declared at <ptentrel.hxx>
 *  @{
 */

// dummy class declarations to satify the compiler temporarily.
class APOINT;
class ENTITY;
class SPAtransf;

// various enumerations to portray the relation between the given
// point and the given entity

// Enumeration which describes the relation between an point and a point.
/**
 * Specifies whether a point lies inside, outside, or on the boundary of an entity.
 * @param point_in_entity
 * point lies inside the entity.
 * @param point_on_entity
 * point lies on the boundary of the entity.
 * @param point_off_entity
 * point lies outside the entity.
 **/
enum
sg_point_ent_relation {
	point_in_entity,
	point_on_entity,
	point_off_entity
};

/**
 * Relates an <tt>APOINT</tt> to an <tt>ENTITY</tt>.
 * @see APOINT, ENTITY
 */
class DECL_QUERY point_entity_rel : public ACIS_OBJECT {

private:

	point_entity_rel		*next_ptr;		// Next pointer used to represent
											// relation between a point and a
											// list of entities.

	APOINT					*point_ptr;		// Current Edge
	ENTITY					*entity_ptr;	// Current Entity

public:
/**
 * Indicates that there is no possible relation between the given point and the given entity.
 */
	logical					no_relation;	// variable which indicates
											// there is no possible relation
											// between the given point and the
											// given entity.

/**
 * Determines the relation type from the union of the point and the entity.
 * See @href sg_point_ent_relation for more details.
 */
	sg_point_ent_relation	rel_type;		// Relation type, from the union


// Constructor
/**
 * C++ constructor, creating a <tt>point_entity_rel</tt> using the specified parameters.
 * <br><br>
 * <b>Role:</b> The next pointer represents a relation between a point and a
 * list of entities.
 * <br><br>
 * @param ap
 * point.
 * @param ent
 * entity.
 * @param next
 * next pointer.
 * @param ent_trans
 * transform.
 */
	point_entity_rel( 	APOINT *ap,
						ENTITY *ent,
						point_entity_rel  *next = NULL,
						SPAtransf &ent_trans = *(class SPAtransf *)NULL_REF
			);

// Enquiry
/**
 * Determines the <tt>APOINT</tt> in the point-entity relationship.
 */
	APOINT	*point()	{ return point_ptr; }
/**
 * Determines the <tt>ENTITY</tt> in the point-entity relationship.
 */
	ENTITY	*entity() { return entity_ptr; }
/**
 * Determines the next point-entity relationship.
 */
	point_entity_rel *next() { return next_ptr; }

// Usefull set function
/**
 * Sets the next point-entity relationship.
 * <br><br>
 * <b>Role:</b> The next pointer represents a relation between a point and a
 * list of entities.
 * <br><br>
 * @param next
 * next pointer.
 */
	void set_next( point_entity_rel *next ) { next_ptr =  next; }

// Debug function
/**
 * Writes the debug output for a point-entity relationship to standard output or to the specified file.
 * <br><br>
 * @param fp
 * file name.
 * @param head
 * text heading.
 */
	void debug( FILE *fp, char *head = NULL );

// Lose function, destructor
/**
 * Posts a delete bulletin to the bulletin board indicating the instance is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The lose methods for attached attributes are also called.
 */
	void lose();

};

/** @} */

#endif
