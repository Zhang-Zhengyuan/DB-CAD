/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GEOM_TRAVERSE_HEADER
#define GEOM_TRAVERSE_HEADER

#include "dcl_heal.h"
#include "traverse2.hxx"

class VERTEX;
class ENTITY_LIST;
class BODY;
class COEDGE;

// Getting edge, vertices, faces routines

void
get_coedges_around_vertex( VERTEX* , ENTITY_LIST& );

void
get_faces_around_vertex( VERTEX* , ENTITY_LIST& );

void
get_analytic_faces_around_vertex( VERTEX* , ENTITY_LIST& );

void
get_analytic_surfaces_around_vertex( VERTEX* , ENTITY_LIST& );

void
get_analytic_faces_in_body(BODY*,ENTITY_LIST&);

void
get_faces_from_edges(ENTITY_LIST &, ENTITY_LIST &);

void
get_faces_around_face(FACE*, ENTITY_LIST &);

// GSSL/RS - Reblend project
// Function exported
DECL_HEAL void
get_vertices_from_edges( ENTITY_LIST& , ENTITY_LIST& );

void
get_edges_linked_to_face(FACE*,ENTITY_LIST&);

int
bhl_get_no_total_faces(ENTITY_LIST&);

void
get_faces_linked_to_edge( EDGE* , ENTITY_LIST& );

void
get_bad_edges_of_entity(ENTITY* , ENTITY_LIST& );

void
get_coedges_of_entity(ENTITY* , ENTITY_LIST& );

// GSSL/RS - Reblend project
// Function exported
DECL_HEAL void
get_edges_of_entity(ENTITY* , ENTITY_LIST& );

logical
edge_present_btw_face(FACE*, FACE*);

DECL_HEAL FACE*
get_face_of_coedge(COEDGE * );

logical
get_edges_to_coalesce_between_2_vertices(VERTEX* , VERTEX* ,double ,ENTITY_LIST& );


#endif
