/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_DRAW)
#define BHL_DRAW

#include "dcl_stitch.h"
#include "lists.hxx"

class ENTITY;

DECL_STITCH void
bhl_draw_init();

DECL_STITCH void
bhl_remove_color( ENTITY* );

DECL_STITCH void
bhl_highlight_list(ENTITY_LIST& entl, int hilite_color);

DECL_STITCH void
bhl_draw_entity(ENTITY* ee, int color);

DECL_STITCH void
bhl_set_color( ENTITY *this_ent, int col );

DECL_STITCH void
bhl_remove_colors_edges( ENTITY*  );

DECL_STITCH void
bhl_sketch_unshared_edges(ENTITY* );

DECL_STITCH void
bhl_draw_entity_list(ENTITY_LIST& ent_list, int col);

DECL_STITCH void
bhl_remove_colors_faces( ENTITY*  );

DECL_STITCH void
bhl_remove_edge_face_colors( ENTITY*  );

DECL_STITCH void
bhl_remove_edge_face_colors( ENTITY_LIST&  );


typedef void (*set_color_callback_fn) (ENTITY*, int);

#ifdef THREAD_SAFE_ACIS
extern DECL_STITCH safe_function_type<set_color_callback_fn> p_color_callback_fn;
#else
extern DECL_STITCH set_color_callback_fn p_color_callback_fn;
#endif

#endif
