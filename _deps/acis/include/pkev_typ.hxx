/* ORIGINAL: 3dt2.1/scmext/pkev_typ.hxx */
// $Id: pkev_typ.hxx,v 1.6 2001/11/30 20:28:16 jmb Exp $
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
//    Define pick-event data type in scheme
//
//----------------------------------------------------------------------

#ifndef pkev_typ_hxx
#define pkev_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class PickEvent;

logical is_Scm_Pick_Event(ScmObject);
PickEvent get_Scm_Pick_Event(ScmObject);
ScmObject make_Scm_Pick_Event(const PickEvent&);

//======================================================================
#endif
