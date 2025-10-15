/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ACIS_SCHEME__HXX
#define ACIS_SCHEME__HXX

/*
* Elk implements Scheme Language. Elk3.0 is the last available version
* Source code for elk3.0 is in scm/scheme/elk. All these are 'c' files
*
* Acis uses elk through an intermediate layer of functions. These functions
* are implemented by the files in scm/scheme directory.
*
* 
* This file AcisScheme.hxx is intended to provide a single point interface for 
* accessing Acis. No file in scm/scheme or scm/scheme/elk  directory should 
* #include any Acis file directly.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef STANDALONE_SCHEME

#include <stdarg.h>

#ifdef __cplusplus
	#define DECL 	extern "C"
#else
	#define DECL	extern 
#endif

/* Commonly used include files - from Acis Hierarchy */
#include "logical.h"
#include "static_types.hxx"


/* Functions used by Scheme but defined by Acis */

DECL void ENTER_FUNCTION( char *FuncName ) ; 
DECL void journal_string(const char* string,	unsigned limit) ;
DECL void journal_add_char( char ) ;

#undef DECL

#else 

/* These are valid when Scheme is linked with Acis */

#include "logical.h"
#include "static_types.hxx"
#include "acis3dt.h"
#include "acis.hxx"

#ifdef __cplusplus

#include "api.hxx"
#include "errorbase.err"
#include "trace.hxx"

#include "tensor.hxx"
#include "law.hxx"
#include "law.hxx"

#endif


#endif /* STANDALONE_SCHEME */
#endif

