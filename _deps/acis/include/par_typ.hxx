/* ORIGINAL: 3dt2.1/scmext/par_typ.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// @(#)par_typ.hxx	1.8 10/07/94
//----------------------------------------------------------------------
// purpose---
//    Define scheme par-pos data type
//
//----------------------------------------------------------------------

#ifndef par_typ_hxx
#define par_typ_hxx

#include "scheme.hxx"

#include "param.hxx"

//======================================================================

logical is_Scm_Par_Pos(ScmObject);
logical is_Scm_Par_Pos_List(ScmObject);

SPApar_pos get_Scm_Par_Pos(ScmObject);
int get_Scm_Par_Pos_List(ScmObject l, SPApar_pos*& pts);

ScmObject make_Scm_Par_Pos(double u, double v);
ScmObject make_Scm_Par_Pos(const SPApar_pos&);
ScmObject make_Scm_Par_Pos_List(int npts, const SPApar_pos *pts);

// Set the coordinates of a scheme Par_Pos
void set_Scm_Par_Pos(ScmObject, const SPApar_pos&);

//======================================================================
#endif

