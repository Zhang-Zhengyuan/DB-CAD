/* ORIGINAL: 3dt2.1/scheme/sgrd_typ.hxx */
// $Id: sgrd_typ.hxx,v 1.4 2000/12/26 18:31:00 products Exp $
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
//    Define scheme spline surface data type
//
//----------------------------------------------------------------------

#ifndef sgrd_typ_hxx
#define sgrd_typ_hxx

#include "logical.h"
#include "scheme.hxx"

#include "splgrid.hxx"

//======================================================================

logical     is_Scm_Splgrid(ScmObject s);
splgrid   *get_Scm_Splgrid(ScmObject scmgrid);
ScmObject make_Scm_Splgrid(splgrid *grid);

//======================================================================
#endif
