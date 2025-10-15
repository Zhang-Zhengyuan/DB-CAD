/* ORIGINAL: 3dt2.1/scheme/load_utl.hxx */
// $Id: load_utl.hxx,v 1.5 2000/12/26 19:05:55 products Exp $
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
//    Declare procedures for loading files
//
//----------------------------------------------------------------------

#ifndef load_utl_hxx
#define load_utl_hxx

#include <stdio.h>

#ifdef NT
#include <windows.h>
#endif

//======================================================================

extern "C" int file_exists(const char* name);
extern "C" int load_if_exists(const char*,const char*,const char*);
extern "C" int load_startup_file(const char* = NULL);

//======================================================================
#endif
