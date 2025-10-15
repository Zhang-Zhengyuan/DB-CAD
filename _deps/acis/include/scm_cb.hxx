/* ORIGINAL: 3dt2.1/scheme/scm_cb.hxx */
// $Id: scm_cb.hxx,v 1.5 2000/12/26 19:05:56 products Exp $
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
//    Declare callback procedures for the scheme interpreter
//----------------------------------------------------------------------

#ifndef SCM_CB_HXX
#define SCM_CB_HXX

//----------------------------------------------------------------------

// type of Scheme error callback.  The character string is the text
// of the error message which is passed to the callback procedure.
typedef void (*SchemeErrorCallback)(const char*);

// procedure to register a Scheme error callback.  Registered callbacks
// are called when an error occurs while evaluating a Scheme expression.
// You may register more than one of these.
void register_SchemeErrorCallback(SchemeErrorCallback);

// type of Scheme exit callback.  This is a procedure which will get
// called when the Scheme (exit) procedure is executed.
typedef void (*SchemeExitCallback)(int);

// register a Scheme exit callback.  There is only one exit procedure.
// If you call this more than once, only the last procedure registered
// is called.
void register_SchemeExitCallback(SchemeExitCallback);

//----------------------------------------------------------------------
#endif


