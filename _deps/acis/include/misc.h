/* ORIGINAL: 3dt2.1/scheme/elk/misc.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: misc.h,v 1.12 2002/01/30 17:32:58 skenny Exp $
//----------------------------------------------------------------------
// purpose---
//    Miscelaneous declarations
//
//----------------------------------------------------------------------
*/
#ifndef MISC_H
#define MISC_H

#include "funcproto.h"
#include "scmobject.h"

#ifndef __GNUC__
#  define __asm__ asm
#endif

#include<math.h>

#ifndef HUGE
#  define HUGE HUGE_VAL
#endif


/* Arithmetic shift right for compilers that don't sign extend:
 */
#if (-1 >> 1) < 0
#  define ASR(n,s)    ((n) >>= (s))
#else
#  define NBITS(v)    ((sizeof v) * 8)
#  define HIBIT(v,n)  (NBITS(v) - (n))
#  define ASR(n,s)    ((n) >>= (s),\
			 ((n) & (1 << (HIBIT((n),(s)) - 1)) ?\
			    ((n) |= ~(((unsigned)1 << HIBIT((n),(s))) - 1)) :\
			    (n)))
#endif


C_LINKAGE_BEGIN
extern ScmObject scm_False2;
C_LINKAGE_END

#define Nullp(x)    (TYPE(x) == T_Null)
#define Truep(x)    (!EQ(x,scm_False) && !EQ(x,scm_False2))
#define Car(x)      PAIR(x)->car
#define Cdr(x)      PAIR(x)->cdr
#define Cons        P_Cons
#define Begin       P_Begin
#define Assq(x,y)   General_Assoc(x,y,0)
// Print与google_test冲突，改为_Print
#define _Print(x)    General_Print_Object (x, Curr_Output_Port, 0)
#define Numeric(t)  (t == T_Fixnum || t == T_Flonum || t == T_Bignum)

#define Whitespace(c) (c == ' ' || c == '\t' || c == '\014' || c == '\n' || c == '\r')
#define Delimiter(c) (c == ';' || c == ')' || c == '(' || c == '"')


/* Align heap addresses */
#ifdef ALIGN_8BYTE
/* STIPORT jmb/jdh osf1 longs and pointers have 64 bits ! */
#  define ALIGN(ptr) ((ptr) = (char *)(((size_t)(ptr) + 7) & ~7))
#else
#  define ALIGN(ptr) ((ptr) = (char *)(((size_t)(ptr) + 3) & ~3))
#endif

/* Normalize stack addresses */
/* STIPORT jmb/jdh osf1 longs and pointers have 64 bits ! */
#define NORM(addr)  ((size_t)(addr) + delta)


/* Used in special forms: */
C_LINKAGE_BEGIN
extern int Tail_Call;
C_LINKAGE_END

#define TC_Prolog   register _t = Tail_Call
#define TC_Disable  Tail_Call = 0
#define TC_Enable   Tail_Call = _t


/* Macros to be used by readers registered with Define_Reader().
 * They operate on variables c, port, f, and str.
 */
#define Reader_Getc {\
    c = str ? String_Getc (port) : getc (f);\
    if (c == '\n') PORT(port)->lno++;\
}

#define Reader_Ungetc {\
    if (str) String_Ungetc (port,c); else (void)ungetc (c,f);\
    if (c == '\n') if (PORT(port)->lno > 1) PORT(port)->lno--;\
}

#define Reader_Tweak_Stream {\
    if (!str && (feof (f) || ferror (f))) clearerr (f);\
}

#define Reader_Sharp_Eof {\
    Reader_Tweak_Stream;\
    Reader_Error (port, "end of file after `#'");\
}

#endif
