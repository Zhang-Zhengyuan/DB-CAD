/* ORIGINAL: 3dt2.1/scheme/hist_typ.hxx */
// $Id: hist_typ.hxx,v 1.5 2000/12/26 19:05:55 products Exp $
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
//    define history data type in scheme
//
//----------------------------------------------------------------------

#ifndef hist_typ_hxx
#define hist_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "efilter.hxx"

class PART;
class HISTORY_STREAM;

//----------------------------------------------------------------------
// routines for operating on histories
//----------------------------------------------------------------------

logical is_Scm_History(ScmObject);

HISTORY_STREAM* get_Scm_History(ScmObject);

ScmObject make_Scm_History(HISTORY_STREAM*, PART*);

void call_history_creation_hook(HISTORY_STREAM*);

//======================================================================
#endif











