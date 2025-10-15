/* ORIGINAL: 3dt2.1/geomhusk/get_box.hxx */
// $Id: get_box.hxx,v 1.8 2002/08/09 17:15:15 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// STL vds 06 Jan 2003: modified get_entity_box() for tight bounding boxes

//----------------------------------------------------------------------
// purpose---
//    Declare procedure for getting the bounding SPAbox of an ENTITY_LIST
//
//----------------------------------------------------------------------

#ifndef get_box_hxx
#define get_box_hxx

#include "dcl_kern.h"
#include "api.hxx"

//======================================================================

class ENTITY_LIST;
class WCS;
class SPAposition;
class outcome;

DECL_KERN outcome get_entity_box(const ENTITY_LIST& ent_list,
                       WCS* wcs,
                       SPAposition&   min_pt,
                       SPAposition&   max_pt,
					   logical tight_box = FALSE
                       );


//======================================================================
#endif
