/* ORIGINAL: 3dt2.1/scheme/scm_cbp.hxx */
// $Id: scm_cbp.hxx,v 1.6 2000/12/26 19:05:56 products Exp $
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
//    Declare private procedures for calling scheme callbacks.  These
//    procedures are only intended for use inside the Scheme interpreter.
//----------------------------------------------------------------------

#ifndef SCM_CBP_HXX
#define SCM_CBP_HXX

#include "scm_cb.hxx"

//----------------------------------------------------------------------

extern void call_SchemeErrorCallbacks(const char* msg);
extern void call_SchemeExitCallback(int exit_code);

//----------------------------------------------------------------------
#endif
