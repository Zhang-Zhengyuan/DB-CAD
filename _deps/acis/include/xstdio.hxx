/* ORIGINAL: 3dt2.1/main/xwindows/xstdio.hxx */
// $Id: xstdio.hxx,v 1.6 2000/12/26 19:05:45 products Exp $
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

// nsb : 24 Apr 2009: InterOp was having trouble with i3dt options "-d" and "-out" combined.
// The following change fixes that problem.
#include "acinput.hxx"
#include "acoutput.hxx"
#include "acexit.hxx"

class xwin_input_cb : public input_callback
{
public:
        xwin_input_cb(FILE *in_fp) : input_callback(in_fp) {}

        virtual int do_getc();
        virtual int do_ungetc(int);
};


class xwin_output_cb : public output_callback
{
public:
        xwin_output_cb(FILE *out_fp) : output_callback(out_fp) {}

        virtual int print_string(const char *);
        virtual int flush();
};

class xwin_exit_cb : public exit_callback
{
public:
        virtual logical execute(int);
};

extern void XstdioInit(Widget, int (*)(const char *, int) );
extern int xstdioClose();
extern void XstdioOutput(FILE *fp);

#endif
