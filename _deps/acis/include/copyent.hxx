/* ORIGINAL: 3dt2.1/geomhusk/copyent.hxx */
// $Id: copyent.hxx,v 1.8 2002/08/09 17:15:14 jeff Exp $
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
//    Procedure for copying entities which belond to a solid withough
//    copying the entire solid.
//
//----------------------------------------------------------------------

#ifndef cpoyent_hxx
#define cpoyent_hxx

#include "dcl_kern.h"
#include "api.hxx"
#include "acis_options.hxx"

//======================================================================

class VERTEX;
class CURVE;
class SURFACE;
class EDGE;
class FACE;
class WIRE;
class ENTITY;
class SPAtransf;
class outcome;

//======================================================================

DECL_KERN VERTEX* copy_vertex( VERTEX* v, const SPAtransf& tform = *( const SPAtransf *) NULL_REF );
DECL_KERN CURVE* copy_curve( CURVE *the_curve, const SPAtransf& tform = *( const SPAtransf *) NULL_REF );
DECL_KERN SURFACE* copy_surface( SURFACE *the_surface, const SPAtransf& tform = *( const SPAtransf *) NULL_REF );
DECL_KERN EDGE* copy_edge( EDGE* edge, const SPAtransf& tform = *( const SPAtransf *) NULL_REF );
DECL_KERN FACE* copy_face( FACE* the_face, const SPAtransf& tform = *( const SPAtransf *) NULL_REF);
DECL_KERN WIRE* copy_wire( WIRE* the_wire, const SPAtransf& tform = *( const SPAtransf *) NULL_REF);
DECL_KERN outcome copy_single_entity(ENTITY*, ENTITY*&, AcisOptions* ao=NULL);

//======================================================================
#endif
