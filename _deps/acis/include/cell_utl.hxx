/* ORIGINAL: 3dt2.1/geomhusk/cell_utl.hxx */
// $Id: cell_utl.hxx,v 1.8 2000/12/26 18:31:23 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// --------------------------------------------------------------------
//  Purpose---
//    Header file for Cellular Topology procedures.
//
// --------------------------------------------------------------------

#ifndef cell_utl_hxx
#define cell_utl_hxx

#include "dcl_ct.h"
#include "logical.h"
#include "api.hxx"

class  ENTITY;
class  ENTITY_LIST;
class  LUMP;

// ====================================================================

DECL_CT outcome celltop_attach(ENTITY_LIST  &body_list);
DECL_CT outcome celltop_remove(ENTITY_LIST  &body_list);
DECL_CT outcome celltop_expand(ENTITY_LIST  &body_list);
DECL_CT outcome celltop_flatten(ENTITY_LIST  &body_list);

DECL_CT void get_cell_list(LUMP  *lump, ENTITY_LIST  &cell_list);
DECL_CT void get_all_cells(ENTITY_LIST const &body_list, ENTITY_LIST  &cell_list);

// ====================================================================

#endif
