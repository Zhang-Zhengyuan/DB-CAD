/* ORIGINAL: 3dt2.1/acis3dt.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: acis3dt.h,v 1.17 2000/12/26 18:46:18 products Exp $
//----------------------------------------------------------------------
// purpose---
//    Stuff we want included in every file either in, or using, the 3d 
//    toolkit.
//
//----------------------------------------------------------------------
*/

#ifndef ACIS3DT_H
#define ACIS3DT_H

/* _MSC_VER is defined by the Visual C++ compiler.  If it is defined,
   make sure that some other symbols are defined compatibly */
#ifdef _MSC_VER

#ifndef MSVC
#define MSVC
#endif

/* If we are using the Visual C++ compiler, then the symbol NT should also
   be defined */
#ifndef NT
#define NT
#endif

/* We define this so that we do stricter type checking in the Win32 API calls */
#ifndef STRICT
#define STRICT
#endif

#ifndef _WINDOWS
#define _WINDOWS
#endif

/* Show annoying warnings about conversion from double to float
   only if the warning level is set to 4 */
#ifdef NT
#pragma warning( 4 : 4244 )
#pragma warning( 4 : 4010 )
#endif

#endif /* _MSC_VER */


/* __MWERKS__ is defined by the CodeWarrior compiler.  If it is defined,
   make sure that some other symbols are defined compatably */
#ifdef __MWERKS__

#ifndef mac
#define mac
#endif

/* 3D Toolkit defines */
#define PCACIS

/* Basic Rendering Husk defines */
#define STI_VERSION
#define DRVR_SP

#endif	/* __MWERKS__ */

/*
// The folowing things are defined in wincvt.h for ACIS.  For ACIS 1.6
// they are planning on providing a standard ACIS include file which
// defines these for all platforms.  At that time, we will just include
// that file here.
*/

/*
// The standard acis include file is acis.hxx, a file that has been in
// acis since the beginning.  So we just include it here.
*/

#ifdef STDIO_IMPLEMENTOR
#define NO_ACIS_IO_REDIRECTION
#endif

#include "acis.hxx"

#define NEW_FACET

/*
// constants used by the toolkit
*/

typedef enum TKStatus	{ tkSuccess, tkFailure } TKStatus;

#endif
