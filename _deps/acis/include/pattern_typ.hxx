/* $Id: pattern_typ.hxx,v 1.4 2001/05/02 16:49:49 jenglund Exp $ */
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
//    declare procedures for working with patterns in scheme
//
//----------------------------------------------------------------------

#ifndef pattern_typ_hxx
#define pattern_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class pattern;

//======================================================================

logical is_Scm_Pattern(ScmObject);
pattern *get_Scm_Pattern(ScmObject);
ScmObject make_Scm_Pattern(pattern *in_pattern);

//======================================================================
#endif

