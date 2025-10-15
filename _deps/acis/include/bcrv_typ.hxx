/* ORIGINAL: 3dt2.1/scheme/bcrv_typ.hxx */
// $Id: bcrv_typ.hxx,v 1.4 2000/12/26 18:46:04 products Exp $
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
//    Define bounded_curve scheme object
//
//----------------------------------------------------------------------

#ifndef bcrv_typ_hxx
#define bcrv_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class bounded_curve;

//======================================================================

logical is_Scm_Bounded_Curve(ScmObject);
bounded_curve* get_Scm_Bounded_Curve(ScmObject);
logical get_Scm_Bounded_Curve_From_Edge(ScmObject, bounded_curve*&);
ScmObject make_Scm_Bounded_Curve(bounded_curve*);

//======================================================================
#endif
