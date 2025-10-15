/* ORIGINAL: acis2.1/sg_husk/query/ededint.hxx */
// $Id: ededint.hxx,v 1.11 2002/08/09 17:19:03 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( EDGE_EDGE_INT_CLASS )
#define EDGE_EDGE_INT_CLASS

//STI HDR_INDEP: nvp
#include "dcl_intr.h"
#include "position.hxx"
class ENTITY;

// dummy class declarations to satify the compiler temporarily.
class EDGE;
class VERTEX;

class SPAposition;

// New classes added to add more meaning to the edge_entity_relation
// class.

// Classify the intersection between two edges.
enum EDGE_EDGE_REL {

	edge_edge_unknown,		// No comment on the relation.
	edge_edge_normal,		// Normal angular intersection.	
	edge_edge_tangent,		// Edges are tangent, or ( anti-tangeant )
							// but are not coincident, note that when
							// this is set, there is a good chance that
							// fuzzy is also set.
	edge_edge_coin			// Edges are coincident over a region.
};

// Edge-Edge intersection data Class,  This is used to represent the 
// intersection between two edges.
class DECL_INTR EDGE_EDGE_INT : public ACIS_OBJECT {
private:

	// Internal Debug routine.
	void internal_debug( 
			FILE *,
			const char *head = NULL	// Leader string
		);


	// Also, the information about the fuzzy intersections is
	// kept private for now.
	
	logical			fuzzy;	// If this is set true, than the edges do not
							// intersect very cleanly( this is used to
							// represent a fuzzy tangency i.e., the edges
							// intersect each other at a low angle.  There
							// is a region at the intersection point the edges
							// are with in SPAresabs.  So, this is not a clean
							// coincidence however.
							// This region is determined by the low_param and
							// high_param for this_edge.
	double			low_param;	// Fuzzy intersection low end with respect to
								// this_edge.
	double			high_param;	// Fuzzy intersection high end with respect to
								// this edge.

public:
	EDGE_EDGE_INT *next;	// Next intersection in the list.

	EDGE	*this_edge;		// One edge of the intersection
	EDGE	*other_edge;	// Other edge of the intersection


	ENTITY  *this_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt this edge, i.e, if this   
							// intersection point is geometrically same as the
							// vertex of the the edge, than this variable 
							// points to it.  Other wise, it points to the 
							// other edge.

	ENTITY  *other_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt other edge, i.e, if this   
							// intersection point is geometrically same as the
							// vertex of the other edge, than this variable 
							// points to it.  Other wise, it points to the 
							// other edge.


	SPAposition int_point;		// Object space intersection point.

	double	param1;			// Parameter value of the intersection on this edge.
	double	param2;			// Parameter value of the intersection on this edge.


	EDGE_EDGE_REL	low_rel;// Relationship between the edges on the lower
							// SPAparameter side of the intersection with respect
							// to this edge.
	
	EDGE_EDGE_REL	high_rel;// Relationship between the edges on the higher
							// SPAparameter side of the intersection with respect
							// to this edge.


	// Main constructor
	EDGE_EDGE_INT(
			EDGE_EDGE_INT *,	// Next intersection to chain up
			EDGE *,				// This edge
			EDGE *,				// Other Edge
			SPAposition const &,	// Intersection point
			double,				// this edge param
			double				// Other edge param
		);


	// Utility lose, function, which deletes the list recursively.
	// Note to call this routine on the start of the list.
	void lose();


	// Debug routine.
	void debug( 
			FILE *,
			const char *head = NULL	// Leader string
		);
};

#endif
