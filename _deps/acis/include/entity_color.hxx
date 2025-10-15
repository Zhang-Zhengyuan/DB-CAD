/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Access functions to strip useful data out of RH_LIGHT and 
// entities.
/*******************************************************************/
#ifndef __rbase_entity_color_hxx__
#define __rbase_entity_color_hxx__
// ACIS includes
#include "dcl_rb.h"
#include "logical.h"
#include "entity.hxx"
// Toolkit includes
#include "rgbcolor.hxx"
DECL_RB rgb_color set_default_color(rgb_color);
DECL_RB rgb_color get_default_color();
DECL_RB rgb_color set_default_background_color(rgb_color);
DECL_RB rgb_color get_default_background_color();
DECL_RB rgb_color set_default_face_color(rgb_color);
DECL_RB rgb_color get_default_face_color();
DECL_RB rgb_color set_default_edge_color(rgb_color);
DECL_RB rgb_color get_default_edge_color();
DECL_RB rgb_color set_default_vertex_color(rgb_color);
DECL_RB rgb_color get_default_vertex_color();

DECL_RB rgb_color set_highlight_color(rgb_color);
DECL_RB rgb_color get_highlight_color();
DECL_RB rgb_color set_active_wcs_color(rgb_color);
DECL_RB rgb_color get_active_wcs_color();

DECL_RB rgb_color set_entity_color(ENTITY*, rgb_color);
DECL_RB rgb_color get_entity_color(const ENTITY*);
DECL_RB rgb_color get_entity_color(const ENTITY*,
								   logical	   inherit,   // Traverse up the tree to find the inherited color.
								   logical&   found       // Flag to indicate that the color was explicitly set.
);

#endif
