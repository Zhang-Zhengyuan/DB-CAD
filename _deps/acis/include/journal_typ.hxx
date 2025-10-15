// $Id: journal_typ.hxx,v 1.1 2001/12/03 16:36:56 rocon Exp $
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
//   declare AcisJournal object for scheme
//
//----------------------------------------------------------------------

#ifndef ACISJOURNAL_TYPE
#define ACISJOURNAL_TYPE

#include "scheme.hxx"
#include "logical.h"

class AcisJournal;


//======================================================================

logical is_Scm_AcisJournal(ScmObject);
AcisJournal* get_Scm_AcisJournal(ScmObject);
ScmObject make_Scm_AcisJournal(AcisJournal *journal_info);
logical clear_Scm_AcisJournal(ScmObject obj);

//======================================================================
#endif
