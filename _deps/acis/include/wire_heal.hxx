/* $Id: wire_heal.hxx,v 1.11 2002/08/09 17:27:52 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined(__WIRE_HEAL_HXX)
#define __WIRE_HEAL_HXX

#include "logical.h"
#include "api.hxx"
#include "lists.hxx"
#include "dcl_covr.h"
#include "convert.hxx"

class EDGE;
class SPAposition;
class SPAvector;
class BODY;
// ywoo 08Jan01: added the class.
class WIRE;
// ywoo: end

// wire healing version #2

DECL_COVR BODY * heal_edges_to_regions(ENTITY_LIST &, double, double, int=FALSE, FILE *fptr=NULL);
DECL_COVR BODY * unite_edges(ENTITY_LIST &edges, FILE *);
DECL_COVR void   construct_free_edges(ENTITY_LIST &edges, double tol=SPAresfit);
DECL_COVR BODY * make_containing_disk(BODY *wbdy, EDGE *&disk_ed);

DECL_COVR void refit_edge(EDGE *in_edge);
DECL_COVR void get_free_vertices(ENTITY_LIST &in_verts, ENTITY_LIST &open_verts);

#endif // !defined(__WIRE_HEAL_HXX)
