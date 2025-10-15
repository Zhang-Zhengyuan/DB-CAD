/* ORIGINAL: 3dt2.1/scheme/jrl_utl.hxx */
// $Id: jrl_utl.hxx,v 1.12 2002/03/29 20:08:00 mgoertz Exp $
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
//    Declare Journaling support functions.
//
//----------------------------------------------------------------------

#ifndef jrl_utl_hxx
#define jrl_utl_hxx
#include "logical.h"

//======================================================================
class param_string;

void journal_init( param_string&, logical = 0 );

void journal_save( const char* );

void journal_nocheck_reset();

void journal_write();

void journal_write_now(const char* string);

extern "C" void journal_add_char( char );

void journal_comment( const char* = 0, unsigned limit = ~0 );

extern void journal_result( const char* result = 0 );

void journal_make_comment();

void journal_pause();

void journal_resume();

void journal_close();

extern "C" void journal_string(const char* string,	unsigned limit = ~0);

logical journal_status_on();

logical journal_status_pause();

//======================================================================
#endif
