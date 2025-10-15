/* ORIGINAL: 3dt2.1/scheme/elk/initelk.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: initelk.h,v 1.10 2001/04/16 15:50:13 rajesh Exp $
//----------------------------------------------------------------------
// purpose---
//    Declare public functions
//
//----------------------------------------------------------------------
*/
#ifndef INITELK_H
#define INITELK_H

#include "funcproto.h"

C_LINKAGE_BEGIN

extern void Init_Type ();
extern void Init_String ();
extern void Init_Symbol ();
extern void Init_Env ();
extern void Init_Error ();
extern void Init_Exception ();
extern void Init_Io ();
extern void Init_Prim();
extern void Init_Math ();
extern void Init_Print ();
extern void Init_Auto ();
extern void Init_Heap ();
extern void Init_Load ();
extern void Init_Proc ();
extern void Init_Special ();
extern void Init_Read ();
extern void Init_Features ();
extern void Init_Hack ();
extern void Init_Terminate ();
extern void Init_Cstring ();

/* STI rr (08/29/00) begin: Delete the cstrings */
extern "C" void Terminate_Cstring();

/* STI rr (04/07/01) begin: Functions for cleanup */
/* of static char* in type.c and read.c */
extern "C" void Terminate_Read();
extern "C" void Terminate_Types();
extern "C" void Terminate_Terminate();
// STI rr end.

#ifdef __cplusplus
extern void Init_Loadpath (const char*);
#else
extern void Init_Loadpath ();
#endif

C_LINKAGE_END

#endif
