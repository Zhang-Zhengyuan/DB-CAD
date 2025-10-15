/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_GEOM_DRAW)
#define BHL_GEOM_DRAW

#include "dcl_heal.h"
#include "draw2.hxx"


DECL_HEAL void
bhl_sketch_spline_faces(ENTITY* );

DECL_HEAL void
bhl_remove_colors_of_computed_edges(ENTITY* );

DECL_HEAL void
bhl_draw_uv_lines(ENTITY* face);

#endif
