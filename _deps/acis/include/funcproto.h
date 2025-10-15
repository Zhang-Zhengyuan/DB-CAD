/* ORIGINAL: 3dt2.1/scheme/elk/funcprto.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: funcproto.h,v 1.1 2001/04/03 17:54:06 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//    Define C versus C++ function prototypes for scheme
//
//----------------------------------------------------------------------
*/
/* These definitions are not tunable.  Do not change them.
 */

#ifndef FUNCPROTO_H
#define FUNCPROTO_H

#if __STDC__ || defined(__cplusplus)
#  undef WANT_PROTOTYPES
#endif

#ifdef WANT_PROTOTYPES
#  define ELK_USE_PROTOTYPES
#  define ELLIPSIS
#endif

#ifdef __cplusplus
#  define ELK_USE_PROTOTYPES
#  define ELLIPSIS ...
#  define C_LINKAGE_BEGIN extern "C" {
#  define C_LINKAGE_END   }
#else
#  define C_LINKAGE_BEGIN
#  define C_LINKAGE_END
#endif

#if __STDC__ && !defined(__cplusplus)
#  define ELK_USE_PROTOTYPES
#  define ELLIPSIS
#endif

#ifdef NO_PROTOTYPES
#  undef ELK_USE_PROTOTYPES
#endif

#ifdef ELK_USE_PROTOTYPES
#  define P_(args) args
#else
#  define P_(args) ()
#  define ELLIPSIS
#  define const
#endif

#endif
