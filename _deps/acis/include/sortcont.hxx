/* ORIGINAL: acis2.1/sg_husk/cover/cover.hxx */
/* $Id: sortcont.hxx,v 1.4 2000/12/26 18:26:35 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SORTCONT_HXX )
#define  SORTCONT_HXX

#include "dcl_bool.h"

class FACE;

class ENTITY_LIST;


//	Sort out a collection of loops belonging to one face, into a
//	collection of faces, each containing only those loops that lie
//	immediately within.

DECL_BOOL void
sort_out_ctmt( FACE*, ENTITY_LIST& );

#endif
