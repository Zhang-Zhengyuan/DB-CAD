/* ORIGINAL: 3dt2.1/scheme/elk/intern.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: intern.h,v 1.11 2002/02/16 22:50:22 skenny Exp $
//----------------------------------------------------------------------
// purpose---
//    Declare private functions
//
//----------------------------------------------------------------------
*/
#ifndef INTERN_H
#define INTERN_H

#include "scmobject.h"

/* Functions and variables that are used by more than one source file of
 * the kernel.  Not available to extensions and applications.
 */

C_LINKAGE_BEGIN

/* autoload.c
 */
extern ScmObject V_Autoload_Notifyp;
extern ScmObject Do_Autoload P_((ScmObject, ScmObject));

/* bignum.c
 */
extern int Bignum_Zero P_((ScmObject));
extern int Bignum_Positive P_((ScmObject));
extern int Bignum_Negative P_((ScmObject));
extern int Bignum_Even P_((ScmObject));
extern ScmObject Make_Bignum P_((const char*, int, int));
extern ScmObject Integer_To_Bignum P_((int));
extern ScmObject Bignum_Divide P_((ScmObject, ScmObject));
extern ScmObject Bignum_Abs P_((ScmObject));
extern ScmObject Bignum_Plus P_((ScmObject, ScmObject));
extern ScmObject Bignum_Minus P_((ScmObject, ScmObject));
extern ScmObject Bignum_Fixnum_Multiply P_((ScmObject, ScmObject));
extern ScmObject Bignum_Multiply P_((ScmObject, ScmObject));
extern ScmObject Bignum_Fixnum_Divide P_((ScmObject, ScmObject));
extern ScmObject Double_To_Bignum P_((double));
extern ScmObject Unsigned_To_Bignum P_((unsigned));
extern ScmObject Long_To_Bignum P_((long));
extern ScmObject Integer64_To_Bignum P_((int64_t));

extern ScmObject Unsigned_Long_To_Bignum P_((unsigned long));
extern unsigned Bignum_To_Unsigned P_((ScmObject));
extern unsigned long Bignum_To_Unsigned_Long P_((ScmObject));
extern long Bignum_To_Long P_((ScmObject));
extern int64_t Bignum_To_Integer64 P_((ScmObject));

extern ScmObject Bignum_To_String P_((ScmObject, int));
extern double Bignum_To_Double P_((ScmObject));
extern int Bignum_Equal P_((ScmObject, ScmObject));
extern int Bignum_Greater P_((ScmObject, ScmObject));
extern int Bignum_Less P_((ScmObject, ScmObject));
extern int Bignum_Eq_Less P_((ScmObject, ScmObject));
extern int Bignum_Eq_Greater P_((ScmObject, ScmObject));

/* cont.c
 */
extern WIND *First_Wind, *Last_Wind;
extern ScmObject Internal_Call_CC P_((int, ScmObject));

/* dump.c
 */
extern ScmObject Dump_Control_Point;

/* env.c
 */
extern ScmObject Add_Binding P_((ScmObject, ScmObject, ScmObject));
extern ScmObject Lookup_Symbol P_((ScmObject, int));

/* error.c
 */
extern ScmObject Arg_True;
extern char *appname;

/* exception.c
 */
extern void Install_Intr_Handler P_((void));

/* heap.c
 */
#ifndef GENERATIONAL_GC
extern char *Hp, *Heap_Start, *Heap_End, *Free_Start, *Free_End;
#endif
extern int GC_In_Progress;

/* io.c
 */
extern ScmObject General_Open_File P_((ScmObject, int, ScmObject));

/* load.c
 */
extern char *Loader_Input;
extern ScmObject V_Load_Path, V_Load_Noisilyp, V_Load_Libraries;
extern void Check_Loadarg P_((ScmObject));
extern ScmObject General_Load P_((ScmObject, ScmObject));
extern void Stop_General_Load P_(());

/* list.c
 */
extern ScmObject General_Assoc P_((ScmObject, ScmObject, int));

/* main.c
 */
extern char *stkbase, *A_Out_Name;
extern int Stack_Grows_Down;
extern size_t Max_Stack;
extern int Interpreter_Initialized, Was_Dumped;
extern char *Brk_On_Dump;
extern int Verb_Load, Verb_Init, Case_Insensitive;
extern SYMTAB *The_Symbols;
extern void Exit_Handler P_((void));
#ifndef ATEXIT
/* extern void exit P_((int)); */
#endif

/* math.c
 */
extern char *Flonum_To_String P_((ScmObject));

/* proc.c
 */
extern ScmObject Sym_Lambda, Sym_Macro;
extern int Funcall_Control_Point P_((ScmObject, ScmObject, int));
extern ScmObject Make_Primitive
    P_((ScmObject(*)(ELLIPSIS), const char*, int, int, enum discipline));

/* read.c
 */
extern ScmObject Sym_Quote;
extern ScmObject Sym_Quasiquote, Sym_Unquote, Sym_Unquote_Splicing;
extern ScmObject Parse_Number P_((ScmObject, const char*, int));

/* stab.c
 */
extern SYMTAB *Snarf_Symbols P_((ELLIPSIS));  /* varying args */
extern SYMTAB *Open_File_And_Snarf_Symbols P_((char *));

/* stkmem.c
 */
#ifndef USE_ALLOCA
extern ScmObject Save_GC_Nodes P_((void));
#endif

/* string.c
 */
extern char Char_Map[];
extern ScmObject General_Make_String P_((const char*, int, int));

/* symbol.c
 */
extern ScmObject Unbound, Special, Zero, One;

/* type.c
 */
extern int Num_Types, Max_Type;

/* vector.c
 */
extern ScmObject List_To_Vector P_((ScmObject, int));

C_LINKAGE_END

#endif
