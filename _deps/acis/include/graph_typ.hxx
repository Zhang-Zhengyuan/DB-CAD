// $Id: graph_typ.hxx,v 1.6 2000/12/26 18:46:08 products Exp $
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
//    declare procedures for working with graphs in scheme
//
//----------------------------------------------------------------------

#ifndef graph_typ_hxx
#define graph_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class generic_graph;
class gvertex;

//======================================================================

logical is_Scm_Graph(ScmObject);
generic_graph *get_Scm_Graph(ScmObject);
ScmObject make_Scm_Graph(generic_graph *in_graph);
char * GetStringWithScmNames(generic_graph *gg);
void get_vertex_name(gvertex const * v, char *name);

//======================================================================
#endif
