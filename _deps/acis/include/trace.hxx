/* ORIGINAL: 3dt2.1/geomhusk/trace.hxx */
// $Id: trace.hxx,v 1.6 2000/12/26 18:46:27 products Exp $
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
//    Implement ENTER_FUNCTION macro for tracing program execution
//
//----------------------------------------------------------------------

#ifndef trace_hxx
#define trace_hxx
#include "dcl_kern.h"

//======================================================================

#ifdef DEBUG_TRACE
extern DECL_KERN "C" void DbgPrintf( const char* fmt, ... );
#define ENTER_FUNCTION(name) DbgPrintf("Enter function: %s", name)
#else
#define ENTER_FUNCTION(name)
#endif

/*======================================================================*/
#endif
