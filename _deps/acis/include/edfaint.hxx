/* ORIGINAL: acis2.1/sg_husk/query/edfaint.hxx */
// $Id: edfaint.hxx,v 1.11 2002/08/09 17:19:03 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( EDGE_FACE_INT_CLASS )
#define EDGE_FACE_INT_CLASS

//STI HDR_INDEP: nvp
#include "dcl_intr.h"
#include "param.hxx"
#include "position.hxx"

// dummy class declarations to satify the compiler temporarily.
class ENTITY;
class FACE;
class EDGE;

class SPAposition;

class SPApar_pos;


// New classes added to add more meaning to the edge_entity_relation
// class.

// Classify the intersection between An Edge and a Face.
enum EDGE_FACE_REL {

	edge_face_unknown,	// No comment on the relation.
	edge_face_in,		// Enters the face, tangentially.
	edge_face_out,		// Leaves the face, tangentially.

	edge_face_above,	// Above the face surface.  The dot-product of the
						// face-surface and the edge-curve direction at the
						// intersection is non-zero and +ve.
	edge_face_above_tan,// Just tangent to the face surface, but the rest of
						// the curve is below the face( curvature based 
						// decisions ).
	edge_face_below,	// Above the face surface.  The dot-product of the
						// face-surface and the edge-curve direction at the
						// intersection is non-zero and -ve.
	edge_face_below_tan	// Just tangent to the face surface, but the rest of
						// the curve is below the face( curvature based 
						// decisions ).
};

// Edge-Face intersection data Class,  This is used to represent the 
// intersection between an edge and a face.
class DECL_INTR EDGE_FACE_INT : public ACIS_OBJECT {
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
	EDGE_FACE_INT *next;	// Next intersection in the list.

	EDGE	*edge;			// Edge which is intersected.
	FACE	*face;			// Face with which the given edge is intersected.


	ENTITY	*this_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt this edge, i.e, if this 
							// intersection point is geometrically same as the
							// vertex of the the edge, than this variable points
							// to it.  Other wise, it points to the edge.
	ENTITY	*other_entity;	// Lowest entity_type which properly describes this
							// this intersection wrt this face, i.e, if this 
							// intersection point is geometrically same as the
							// any vertex of the face, than it points to it, 
							// otherwise, it checks to see if the intersection
							// point lies on any of the edge in the face, if
							// it is, it points to it.  The last alternative is
							// to point to the face itself.

	SPAposition int_point;		// Object space intersection point.

	double	param;			// Parameter value of the int_point on this edge.
	SPApar_pos	surf_param;		// Parameter value of the int_point on face surface.


	EDGE_FACE_REL	low_rel;// Relationship between the edge and the face 
							// on the lower SPAparameter side of the edge at the
							// intersection point.
	
	EDGE_FACE_REL	high_rel;// Relationship between the edge and the face 
							// on the higher SPAparameter side of the edge at the
							// intersection point.


	// Main constructor
	EDGE_FACE_INT(
			EDGE_FACE_INT *,	// Next intersection to chain up.
			EDGE *,				// Edge.
			FACE *,				// Face.
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
