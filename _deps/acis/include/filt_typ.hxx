/* ORIGINAL: 3dt2.1/scheme/filt_typ.hxx */
// $Id: filt_typ.hxx,v 1.4 2000/12/26 18:46:08 products Exp $
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
//    Procedures for accessing a scheme entity filter
//
//----------------------------------------------------------------------

#ifndef filt_typ_hxx
#define filt_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class entity_filter;

//======================================================================

logical is_Scm_Entity_Filter(ScmObject);
entity_filter* get_Scm_Entity_Filter(ScmObject);
ScmObject make_Scm_Entity_Filter(entity_filter*);

//======================================================================
#endif
