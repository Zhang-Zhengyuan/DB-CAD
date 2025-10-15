/* ORIGINAL: 3dt2.1/scheme/err_utl.hxx */
// $Id: err_utl.hxx,v 1.11 2001/04/03 17:58:21 jeff Exp $
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
//    declare error handling utilities
//
//----------------------------------------------------------------------

#ifndef err_utl_hxx
#define err_utl_hxx

//======================================================================
#ifndef _EXPORT_MACRO
#define _EXPORT_MACRO
#endif

#ifndef SKIP_FORWARD_DECLARATIONS
#include "AcisScheme.hxx"
#endif

//----------------------------------------------------------------------

#ifndef STANDALONE_SCHEME
_EXPORT_MACRO void bad_outcome(outcome&);
_EXPORT_MACRO err_mess_type get_error_number();
_EXPORT_MACRO void set_error_number(err_mess_type);
#endif
_EXPORT_MACRO void fatal(const char* error_string ...);
_EXPORT_MACRO void error(const char* error_string ...);
_EXPORT_MACRO void warning(const char* error_string ...);
_EXPORT_MACRO void message(const char* error_string ...);
_EXPORT_MACRO void message_no_newline(const char* error_string ...);
_EXPORT_MACRO void Init_Last_Outcome();
_EXPORT_MACRO void Terminate_Last_Outcome();
_EXPORT_MACRO void register_outcome(const outcome&);
_EXPORT_MACRO void display_error_entities(const outcome&);


//======================================================================
#endif
