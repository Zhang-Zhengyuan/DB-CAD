/* ORIGINAL: 3dt2.1/scheme/ro_typ.hxx */
// $Id: ro_typ.hxx,v 1.1 2002/03/22 21:23:16 jmb Exp $
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
//    Procedures for accessing a scheme rendering object
//
//----------------------------------------------------------------------

#ifndef ro_typ_hxx
#define ro_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class RenderingObject;

//======================================================================

logical is_Scm_RenderingObject(ScmObject);
RenderingObject* get_Scm_RenderingObject(ScmObject);
ScmObject make_Scm_RenderingObject(RenderingObject*);

// Not to be used by any functionality other than ro:delete.

void destroy_Scm_RenderingObject(ScmObject);

//======================================================================
#endif
