/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "dcl_stitch.h"
#include "logical.h"

class FACE;
class ENTITY_LIST;
class ENTITY;
class EDGE;
class VERTEX;

#if !defined (BHL_TRAVERSE_HXX)
#define BHL_TRAVERSE_HXX

DECL_STITCH void
get_edges_from_face(FACE* , ENTITY_LIST& );

DECL_STITCH void
get_entities_of_type(int , ENTITY* , ENTITY_LIST& );

DECL_STITCH void
get_edges_around_vertex( VERTEX* , ENTITY_LIST& );

DECL_STITCH logical
get_edges_around_two_vertices (VERTEX* , VERTEX* , ENTITY_LIST&,
							   const logical consider_tol=FALSE ,
							   const double toler = 0,
							   const logical same_face = FALSE,
							   const logical eliminate_fix_stage = FALSE);

DECL_STITCH int
bhl_get_no_total_edges(ENTITY_LIST&);

DECL_STITCH void
get_edges_linked_to_edges(EDGE*,ENTITY_LIST &);

#endif
