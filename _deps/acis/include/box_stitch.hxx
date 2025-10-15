/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_BOX)
#define BHL_BOX

#include "base.hxx"
#include "dcl_stitch.h"
#include "box.hxx"

class ENTITY;
class LOOP;
class FACE;
class EDGE;
class LUMP;
class BODY;

DECL_STITCH SPAbox
hh_get_entity_box(	ENTITY* ent, SPAtransf const * = NULL );

SPAbox
get_loop_box_tol(   LOOP* , double);

SPAbox
get_face_box_tol(   FACE* , double);

DECL_STITCH SPAbox
get_edge_box_tol(   EDGE* , double);

DECL_STITCH SPAbox
get_pos_box_tol(const SPAposition&  , double);

SPAbox
get_lump_box_tol(   LUMP* , double);

SPAbox
get_body_box_tol(   BODY* , double);

DECL_STITCH double
get_stitch_box_tol(FACE* );

#endif

