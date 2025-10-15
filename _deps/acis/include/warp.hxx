// $Id: warp.hxx,v 1.18 2002/08/09 17:30:52 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef WARP_HXX
#define WARP_HXX

#include "dcl_warp.h"
#include "logical.h"
#include "base.hxx"

class BODY;
class FACE;
class law;
class ENTITY_LIST;
class ENTITY;
class SPAposition;
class SPAunit_vector;

// WARNING: Don't use this version anymore because it only works on faces, not wires.
// Use any of the versions below instead.
void DECL_WARP space_warp_old( ENTITY_LIST &face_list,
						   law         *in_law,
						   BODY        *given_body=NULL,
						   logical		do_simplify = TRUE,
						   logical		do_refacet = TRUE);

// This warps all faces and wires in the body.
logical DECL_WARP space_warp( BODY *in_body,
					       law  *in_law,
                           logical do_split = TRUE,
                           logical transform_owning_bodies = TRUE,
                           logical do_simplify = TRUE, 
                           ENTITY_LIST &no_simplify_list = *(ENTITY_LIST*)NULL_REF); 

// Whenever partial space warping is required,
// call this once instead of calling space_warp multiple times.
void DECL_WARP partial_space_warp( 
				ENTITY_LIST &face_list,
				ENTITY_LIST &edge_list,
				law         **face_laws, // the number of these must be the same as the number of faces
				law         **edge_laws, // the number of these must be the same as the number of edges
                logical transform_owning_bodies = TRUE,
                logical do_simplify = TRUE); 

// This version auto calculates the edge_list and edge_laws.
// Don't use this unless you are sure that there are no wire edges that need warping.
void DECL_WARP partial_space_warp( 
				ENTITY_LIST &face_list,
				law         **face_laws, // the number of these must be the same as the number of faces
                logical transform_owning_bodies = TRUE,
                logical do_simplify = TRUE,
                ENTITY_LIST &no_simplify_list = *(ENTITY_LIST*)NULL_REF); 

// The same law is duplicated for all faces and edges.
void DECL_WARP partial_space_warp( 
				ENTITY_LIST &face_list,
				ENTITY_LIST &edge_list,
				law *the_law,
                logical transform_owning_bodies = TRUE,
                logical do_simplify = TRUE); 

void DECL_WARP split_for_warp( ENTITY_LIST &face_list2,
				 BODY        *in_body);

/*
// Returns the distance from the specified axis to the farthest point
// on the bounding SPAbox of the given entity.
// Defined in oper/operator/sg_husk/api/api_twist.cxx
double DECL_WARP get_range_to_axis(const ENTITY *my_entity, 
								const SPAposition & origin,
								const SPAunit_vector & axis);

// Imprints a circular disc of the give origin, axis, and radius onto the entity.
// Defined in oper/operator/sg_husk/api/api_twist.cxx
void DECL_WARP imprint_plane(const ENTITY *my_entity, 
								const SPAposition & origin,
								const SPAunit_vector & axis,
								double radius);*/

void DECL_WARP simplify_entity(
    ENTITY* in_ent  // Entity to be simplified
    );

#endif


