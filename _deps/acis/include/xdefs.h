/* ORIGINAL: 3dt2.1/main/xwindows/xdefs.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: xdefs.h,v 1.5 2000/12/26 19:05:45 products Exp $
//----------------------------------------------------------------------
// purpose---
//    Defines for X Windows
//
//----------------------------------------------------------------------
*/

#ifndef xdefs_h
#define xdefs_h

/*======================================================================*/

#include <stdio.h>
#include <stdarg.h>

#ifndef IN_XSTDIO_CXX

#ifdef putchar
#undef putchar
#endif

#ifdef putc
#undef putc
#endif

#ifdef getc
#undef getc
#endif

#ifdef ungetc
#undef ungetc
#endif

#define exit	windows_exit
#define fflush  windows_fflush
#define fprintf windows_fprintf
#define vfprintf windows_vfprintf
#define fputc	windows_putc
#define printf  windows_printf
#define putc	windows_putc
#define getc	windows_getc
#define ungetc	windows_ungetc
#define putchar windows_putchar
#define fwrite	windows_fwrite

#endif

#ifdef __cplusplus
extern "C" int windows_printf( const char *format, ...);
extern "C" int windows_fprintf( FILE *fp, const char *format, ...);
extern "C" int windows_vfprintf( FILE *fp, const char *format, va_list ap );
extern "C" int windows_fflush( FILE *fp );
extern "C" int windows_putchar( int c );
extern "C" int windows_putc(int c, FILE *fp );
extern "C" int windows_getc(FILE *fp );
extern "C" int windows_ungetc(int, FILE *fp );
extern "C" void windows_exit( int status );
extern "C" int windows_fwrite(char *ptr, int size, int nitems, FILE *fp);
#else 
extern int windows_printf( char *format, ...);
extern int windows_fprintf( FILE *fp, char *format, ...);
extern int windows_vfprintf( FILE *fp, char *format, va_list ap );
extern int windows_fflush( FILE *fp );
extern int windows_putchar( int c );
extern int windows_putc( int c, FILE *fp );
extern int windows_getc(FILE *fp );
extern int windows_ungetc(int, FILE *fp );
extern void windows_exit( int status );
extern int windows_fwrite(char *ptr, int size, int nitems, FILE *fp);
#endif

/*======================================================================*/
#endif
