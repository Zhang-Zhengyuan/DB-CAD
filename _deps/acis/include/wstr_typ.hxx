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
//    S_Wstring utility functions
//
//----------------------------------------------------------------------

#ifndef wstr_typ_hxx
#define wstr_typ_hxx

#include "scheme.hxx"
#include "base.hxx"

/**
* @nodoc
*/
ScmObject make_Scm_Wstring( const SPAWCHAR_T* wstr );

/**
* @nodoc
*/
logical is_Scm_Wstring(ScmObject p);

/**
* @nodoc
*/
const SPAWCHAR_T* get_Scm_Wstring(ScmObject sp);

#endif
