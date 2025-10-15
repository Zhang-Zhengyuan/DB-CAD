/* ORIGINAL: 3dt2.1/geomhusk/vrtx_utl.hxx */
// $Id: vrtx_utl.hxx,v 1.7 2002/08/09 17:15:15 jeff Exp $
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
//    Utility procedures for VERTEXs
//----------------------------------------------------------------------

#ifndef vrtx_utl_hxx
#define vrtx_utl_hxx

#include "dcl_kern.h"

class VERTEX;
class SPAposition;

//======================================================================

// Get the SPAposition of a VERTEX.  This will take into account the
// transform of the body that the VERTEX belongs to (if any)
DECL_KERN SPAposition get_vertex_position(VERTEX* vertex);

//======================================================================
#endif
