/* $Id: sanity.hxx,v 1.9 2002/08/09 17:19:04 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef SANITY_HXX
#define SANITY_HXX

#include <stdio.h>
#include "dcl_intr.h"
#include "insanity_list.hxx"

class ENTITY;
class FACE;
class COEDGE;
class EDGE;
class PCURVE;
class ENTITY_LIST;
class spline;
class BODY;
class LUMP;
class SHELL;
class WIRE;
class LOOP;
class VERTEX;
class TRANSFORM;
class SURFACE;

//------------------------------------------------------------------------------
// check_entity 
// Perform all tests on the specified entity. Return a list of entities
// which fail the tests. Output test results to the indicated file.
//------------------------------------------------------------------------------
DECL_INTR 
insanity_list*
check_entity_internal(
	const ENTITY* ent
//	ENTITY_LIST* insane_list,
//	FILE* fptr
);

//------------------------------------------------------------------------------
// sg_check_edge_forms 
// Determine if the curve forms (open, closed, periodic) are set 
// correctly. Optionally, repair any forms that are incorrectly set.
//------------------------------------------------------------------------------
DECL_INTR insanity_list*
sg_check_edge_forms(
	EDGE *edge
);

//------------------------------------------------------------------------------
// sg_check_pcurve_form 
// Determine if the pcurve forms (open, closed, periodic) are set 
// correctly. Optionally, repair any forms that are incorrectly set.
//------------------------------------------------------------------------------
DECL_INTR insanity_list*
sg_check_pcurve_form(
	COEDGE *coedge 
/*	PCURVE* PC */
);

//------------------------------------------------------------------------------
// sg_check_surface_forms
// Determine if the surface forms (open, closed, periodic) are set 
// correctly. Optionally, repair any forms that are incorrectly set.
//------------------------------------------------------------------------------
DECL_INTR insanity_list*
sg_check_surface_forms(
	FACE *f
/*	spline& spl */
);

// ywoo 7Aug01:
// major checking functions
DECL_INTR int sg_check_body(BODY*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_lump(LUMP*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_shell(SHELL*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_wire(WIRE*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_face(FACE*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_loop(LOOP*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_coedge(COEDGE*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_edge(EDGE*, const SPAtransf*, insanity_list*);
DECL_INTR int sg_check_vertex(VERTEX*, const SPAtransf*, insanity_list*);

// subsequent checking functions
DECL_INTR insanity_list* check_face_order(EDGE *e);
DECL_INTR insanity_list* sg_check_transform( TRANSFORM* T);
DECL_INTR insanity_list* sg_check_surface( SURFACE* srf);
DECL_INTR insanity_list* sg_check_face_area( FACE* face);
DECL_INTR insanity_list* sg_check_face_sense( FACE* face );
DECL_INTR insanity_list* sg_check_face_loops( FACE* f );
DECL_INTR insanity_list* sg_check_degenerate_spline(FACE *f);
DECL_INTR insanity_list* sg_check_face_pcurves( FACE* face );
DECL_INTR insanity_list* sg_check_conical_face( FACE* face );
DECL_INTR insanity_list* sg_check_face_approximation( FACE *face );
DECL_INTR insanity_list* sg_check_surface_param_test( FACE *face );
DECL_INTR insanity_list* sg_check_surface_transform_test( FACE *face );
DECL_INTR insanity_list* sg_check_surface_point_perp_test( FACE *face );
DECL_INTR insanity_list* sg_check_surface_extend_test( FACE *face );
DECL_INTR insanity_list* sg_check_surface_derivs_test( FACE *face );
DECL_INTR insanity_list* sg_check_face_box( FACE* face );
//DECL_INTR insanity_list* check_edge_convexity(EDGE *edge);
DECL_INTR insanity_list* sg_check_edge_curve(EDGE *edge);
DECL_INTR insanity_list* sg_check_edge_box( EDGE* edge );
// ywoo: end
DECL_INTR logical sg_check_face_pcurve(COEDGE* coedge, insanity_list *list);

// uq2 - print sanity context (checked topology count)
DECL_INTR void print_sanity_context(FILE* = debug_file_ptr);

#endif
