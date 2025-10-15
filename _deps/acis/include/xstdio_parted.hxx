/* ORIGINAL: 3dt2.1/main/xwindows/xstdio.hxx */
// $Id: xstdio_parted.hxx,v 1.1 2002/07/18 15:04:32 skenny Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
//----------------------------------------------------------------------
// purpose---
//    include file for wstdio.h
//
//----------------------------------------------------------------------
*/

#ifndef wstdio_h
#define wstdio_h

/* Header file for use with WSTDIO.c */

#define _WSTDIO_DEFINED
#include <stdio.h>

extern void XstdioInit(Widget, int (*)(const char *, int) );
extern int xstdioClose();
extern void XstdioOutput(FILE *fp);

#endif
