/* ORIGINAL: 3dt2.1/geomhusk/wire_utl.hxx */
// $Id: wire_utl.hxx,v 1.13 2002/08/09 17:21:36 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    WIRE utilities
//
//----------------------------------------------------------------------

#ifndef wire_utl_hxx
#define wire_utl_hxx

#include "dcl_cstr.h"
#include "api.hxx"
#include "lists.hxx"
#include "acis_options.hxx"

//======================================================================

#include "logical.h"
class ENTITY;
class ENTITY_LIST;
class BODY;
class WIRE;
class EDGE;
class ENTITY_LIST;
class SPAposition;
class SPAunit_vector;
class outcome;
class SPAtransf;

//======================================================================

DECL_CSTR outcome create_wire_from_edge_list(const ENTITY_LIST&, BODY *&, AcisOptions* ao = NULL);
DECL_CSTR outcome get_curves_from_wires(ENTITY*, ENTITY_LIST&);
DECL_CSTR outcome copy_solid_edge_ent( EDGE*, ENTITY*&, SPAtransf);
DECL_CSTR logical make_entity_wire_body(ENTITY*, BODY*&);
DECL_CSTR logical valid_entity_to_make_wire_body(ENTITY*);
DECL_CSTR logical get_wire_start_position(ENTITY*, SPAposition&);
DECL_CSTR logical get_wire_end_position(ENTITY*, SPAposition&);
DECL_CSTR int organize_edges_to_wires(int     n_ed, // return the number of wire bodies
									  EDGE   *edge_array[], 
									  BODY **&out_wires);
DECL_CSTR void repair_dupicated_vertices(BODY *);

//======================================================================
#endif
