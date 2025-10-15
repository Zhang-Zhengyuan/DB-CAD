/* ORIGINAL: 3dt2.1/scheme/ray_typ.hxx */
// $Id: ray_typ.hxx,v 1.8 2002/08/09 17:15:14 jeff Exp $
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
//    declare procedures for working with class pick_ray in scheme
//
//----------------------------------------------------------------------

#ifndef ray_typ_hxx
#define ray_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class pick_ray;
class SPAposition;
class SPAvector;

//======================================================================

logical is_Scm_Ray(ScmObject);
pick_ray get_Scm_Ray(ScmObject);
ScmObject make_Scm_Ray(const pick_ray&);
ScmObject make_Scm_Ray(const SPAposition&, const SPAvector&);

//======================================================================
#endif
