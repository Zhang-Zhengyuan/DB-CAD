/* ORIGINAL: acis2.1/sg_husk/query/edbdint.hxx */
// $Id: edbdint.hxx,v 1.11 2002/08/09 17:19:03 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( EDGE_BODY_INT_CLASS )
#define EDGE_BODY_INT_CLASS

//STI HDR_INDEP: nvp
#include "dcl_query.h"
#include "param.hxx"
#include "position.hxx"

// dummy class declarations to satify the compiler temporarily.
class ENTITY;
class BODY;
class FACE;
class EDGE;

// bta - 20-Oct-2014, given to customer, see SPACustomInterfaces\DET001\edbdint.hxx

// New classes added to add more meaning to the edge_entity_relation
// class.

// Classify the intersection between an Edge and a Body
enum EDGE_BODY_REL {

	edge_body_unknown,	// No comment on the relation.
	edge_body_in,		// Inside the body
	edge_body_out,		// Outside the body
	edge_body_on		// On the body boundary.  Here the Edge is coincident
						// with any face of the body.
};

// Edge-Body intersection data Class,  This is used to represent the 
// intersection between an edge and a Body.
class EDGE_BODY_INT : public ACIS_OBJECT {
private:

	// Internal Debug routine.
	void internal_debug( 
			FILE *,
			const char *head = NULL	// Leader string
		);


	// Also, the information about the fuzzy intersections is
	// kept private for now.
	
	logical			fuzzy;	// If this is set true, than the edge does not
							// intersect the face surface very cleanly( this 
							// is used to represent a fuzzy tangency i.e., the 
							// edge and the face surface intersect each other 
							// at a low angle.  There is a region at the 
							// intersection point the edge and the surface are 
							// with in SPAresabs.  So, this is not a clean 
							// coincidence however.
							// This region is determined by the low_param and
							// high_param for this_edge.
	double			low_param;	// Fuzzy intersection low end with respect to
								// this_edge.
	double			high_param;	// Fuzzy intersection high end with respect to
								// this edge.

public:
	EDGE_BODY_INT *next;	// Next intersection in the list.

	EDGE	*edge;			// Edge which is intersected.
	BODY	*body;			// Body with which the given edge is intersected.


	ENTITY	*this_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt this edge, i.e, if this 
							// intersection point is geometrically same as the
							// vertex of the the edge, than this variable points
							// to it.  Other wise, it points to the edge.
	ENTITY	*other_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt this body, i.e, if this 
							// intersection point is geometrically same as the
							// any vertex of the body, than it points to it, 
							// otherwise, it checks to see if the intersection
							// point lies on any of the edge in the body, if
							// it is, it points to it.  The last alternative is
							// to point to any shell in the body.

	FACE	*face;			// Face with respect to which the following 
							// information is gathered and may be interpreted.
	SPApar_pos surf_param;		// Parameter value of the intersection point on the
							// face surface.


	SPAposition int_point;		// Object space intersection point.

	double	param;			// Parameter value of the int_point on this edge.


	EDGE_BODY_REL	low_rel;// Relationship between the edge and the BODY 
							// on the lower SPAparameter side of the edge at the
							// intersection point.
	
	EDGE_BODY_REL	high_rel;// Relationship between the edge and the face 
							// on the higher SPAparameter side of the edge at the
							// intersection point.


	// Main constructor
	EDGE_BODY_INT(
			EDGE_BODY_INT *,	// Next intersection to chain up.
			EDGE *,				// Edge.
			BODY *,				// Face.
			SPAposition const &,	// Intersection point.
			double,				// Edge param.
			SPApar_pos				// Surface param.
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
