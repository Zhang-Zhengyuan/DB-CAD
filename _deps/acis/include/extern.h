/* ORIGINAL: 3dt2.1/scheme/elk/extern.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: extern.h,v 1.18 2002/08/16 17:34:57 products Exp $
//----------------------------------------------------------------------
// purpose---
//    Declare public functions
//
//----------------------------------------------------------------------
*/
#ifndef EXTERN_H
#define EXTERN_H

#include "funcproto.h"
#include "scmobject.h"

/* This include file declares all objects exported by the interpreter
 * kernel that may be used by extensions.
 */

C_LINKAGE_BEGIN

/* Autoloading
 */
extern ScmObject P_Autoload P_((ScmObject, ScmObject));

/* Bignums
 */
extern ScmObject Make_Uninitialized_Bignum P_((int));
extern int Bignum_Normalize_In_Place P_((struct S_Bignum *));

/* Boolean operators
 */
extern ScmObject P_Booleanp P_((ScmObject));
extern ScmObject P_Not P_((ScmObject));
extern ScmObject P_Eq P_((ScmObject, ScmObject));
extern ScmObject P_Eqv P_((ScmObject, ScmObject));
extern ScmObject P_Equal P_((ScmObject, ScmObject));
extern ScmObject P_Empty_List_Is_False P_((ScmObject));
extern int Eqv P_((ScmObject, ScmObject));
extern int Equal P_((ScmObject, ScmObject));

/* Characters
 */
extern ScmObject Make_Char P_((int));
extern ScmObject P_Charp P_((ScmObject));
extern ScmObject P_Char_Upcase P_((ScmObject));
extern ScmObject P_Char_Downcase P_((ScmObject));
extern ScmObject P_Char_Eq P_((ScmObject, ScmObject));
extern ScmObject P_Char_Less P_((ScmObject, ScmObject));
extern ScmObject P_Char_Greater P_((ScmObject, ScmObject));
extern ScmObject P_Char_Eq_Less P_((ScmObject, ScmObject));
extern ScmObject P_Char_Eq_Greater P_((ScmObject, ScmObject));
extern ScmObject P_Char_CI_Eq P_((ScmObject, ScmObject));
extern ScmObject P_Char_CI_Less P_((ScmObject, ScmObject));
extern ScmObject P_Char_CI_Greater P_((ScmObject, ScmObject));
extern ScmObject P_Char_CI_Eq_Less P_((ScmObject, ScmObject));
extern ScmObject P_Char_CI_Eq_Greater P_((ScmObject, ScmObject));
extern ScmObject P_Char_Upper_Casep P_((ScmObject));
extern ScmObject P_Char_Lower_Casep P_((ScmObject));
extern ScmObject P_Char_Alphabeticp P_((ScmObject));
extern ScmObject P_Char_Numericp P_((ScmObject));
extern ScmObject P_Char_Whitespacep P_((ScmObject));
extern ScmObject P_Char_To_Integer P_((ScmObject));
extern ScmObject P_Integer_To_Char P_((ScmObject));

/* Continuations
 */
extern ScmObject P_Call_With_Current_Continuation P_((ScmObject));
extern ScmObject P_Dynamic_Wind P_((ScmObject, ScmObject, ScmObject));
extern ScmObject P_Control_Pointp P_((ScmObject));
extern ScmObject P_Control_Point_Environment P_((ScmObject));

/* Scheme strings --> C Strings
 */
extern char *Get_String P_((ScmObject));
extern char *Get_Strsym P_((ScmObject));

/* Debugging
 */
extern ScmObject P_Backtrace_List P_((int, ScmObject*));

/* Dump
 */
extern ScmObject P_Dump P_((ScmObject));

/* Lexical bindings, environments
 */
extern ScmObject P_The_Environment P_((void));
extern ScmObject P_Define P_((ScmObject));
extern ScmObject P_Set P_((ScmObject));
extern ScmObject P_Environment_To_List P_((ScmObject));
extern ScmObject P_Define_Macro P_((ScmObject));
extern ScmObject P_Boundp P_((ScmObject));
extern ScmObject P_Global_Environment P_((void));
extern ScmObject P_Environmentp P_((ScmObject));
extern ScmObject The_Environment, Global_Environment;

/* Error handling
 */
#if ! defined (PRIMITIVE_ERROR_IS_DEFINED)
extern void Primitive_Error P_((ELLIPSIS));
#endif

#if ! defined (FATAL_ERROR_IS_DEFINED)
extern void Fatal_Error P_((ELLIPSIS));
#endif

extern void Range_Error P_((ScmObject));
extern void Panic P_((const char*));
extern ScmObject P_Error P_((int, ScmObject*));
extern ScmObject P_Reset P_((void));
extern const char *Error_Tag;    /* will be removed in the near future */
extern const char *Get_Error_Tag P_((void));
extern void Set_Error_Tag P_((const char *));
extern void Set_App_Name P_((char *));

/* Interrupts
 */
extern ScmObject P_Disable_Interrupts P_((void));
extern ScmObject P_Enable_Interrupts P_((void));
extern void Signal_Exit P_((int));

/* Features
 */
extern ScmObject P_Features P_((void));
extern ScmObject P_Featurep P_((ScmObject));
extern ScmObject P_Provide P_((ScmObject));
extern ScmObject P_Require P_((int, ScmObject*));

/* Memory allocation, garbage collection
 */
extern int GC_Debug;
extern ScmObject Alloc_Object P_((int, int, int));
extern int Register_Before_GC P_((void (*)(void)));
extern int Register_After_GC P_((void (*)(void)));
extern ScmObject P_Collect P_((void));
extern ScmObject P_Garbage_Collect_Status P_((int, ScmObject *));
#ifdef GENERATIONAL_GC
    extern ScmObject P_Collect_Incremental P_((void));
#endif

/* Files and ports
 */
extern ScmObject Curr_Input_Port, Curr_Output_Port;
extern ScmObject Standard_Input_Port, Standard_Output_Port;
extern int Reset_IO P_((int));
extern ScmObject P_Current_Input_Port P_((void));
extern ScmObject P_Current_Output_Port P_((void));
extern ScmObject P_Input_Portp P_((ScmObject));
extern ScmObject P_Output_Portp P_((ScmObject));
extern ScmObject P_Open_Input_File P_((ScmObject));
extern ScmObject P_Open_Output_File P_((ScmObject));
extern ScmObject P_Open_Input_Output_File P_((ScmObject));
extern ScmObject P_Eof_Objectp P_((ScmObject));
extern ScmObject P_With_Input_From_File P_((ScmObject, ScmObject));
extern ScmObject P_With_Output_To_File P_((ScmObject, ScmObject));
extern ScmObject P_Call_With_Input_File P_((ScmObject, ScmObject));
extern ScmObject P_Call_With_Output_File P_((ScmObject, ScmObject));
extern ScmObject P_Open_Input_String P_((ScmObject));
extern ScmObject P_Open_Output_String P_((void));
extern ScmObject P_Port_File_Name P_((ScmObject));
extern ScmObject P_Tilde_Expand P_((ScmObject));
extern ScmObject P_File_Existsp P_((ScmObject));
extern ScmObject P_Close_Input_Port P_((ScmObject));
extern ScmObject P_Close_Output_Port P_((ScmObject));
extern ScmObject P_Port_Line_Number P_((ScmObject));
extern ScmObject Terminate_File P_((ScmObject));
extern ScmObject Make_Port P_((int, FILE*, ScmObject));
extern int Path_Max P_((void));

/* Loading of files
 */
extern ScmObject P_Load P_((int, ScmObject*));
extern void Load_Source_Port P_((ScmObject));
extern void Load_File P_((char *));

/* Pairs and lists
 */
extern ScmObject P_Cons P_((ScmObject, ScmObject));
extern ScmObject P_Car P_((ScmObject));
extern ScmObject P_Cdr P_((ScmObject));
extern ScmObject P_Set_Car P_((ScmObject, ScmObject));
extern ScmObject P_Set_Cdr P_((ScmObject, ScmObject));
extern ScmObject P_Listp P_((ScmObject));
extern ScmObject P_List P_((int, ScmObject*));
extern ScmObject P_Length P_((ScmObject));
extern ScmObject P_Nullp P_((ScmObject));
extern ScmObject P_Pairp P_((ScmObject));
extern ScmObject P_Cxr P_((ScmObject, ScmObject));
extern ScmObject P_Cddr P_((ScmObject));
extern ScmObject P_Cdar P_((ScmObject));
extern ScmObject P_Cadr P_((ScmObject));
extern ScmObject P_Caar P_((ScmObject));
extern ScmObject P_Cdddr P_((ScmObject));
extern ScmObject P_Cddar P_((ScmObject));
extern ScmObject P_Cdadr P_((ScmObject));
extern ScmObject P_Cdaar P_((ScmObject));
extern ScmObject P_Caddr P_((ScmObject));
extern ScmObject P_Cadar P_((ScmObject));
extern ScmObject P_Caadr P_((ScmObject));
extern ScmObject P_Caaar P_((ScmObject));
extern ScmObject P_Caaaar P_((ScmObject));
extern ScmObject P_Caaadr P_((ScmObject));
extern ScmObject P_Caadar P_((ScmObject));
extern ScmObject P_Caaddr P_((ScmObject));
extern ScmObject P_Cadaar P_((ScmObject)); 
extern ScmObject P_Cadadr P_((ScmObject));
extern ScmObject P_Caddar P_((ScmObject));
extern ScmObject P_Cadddr P_((ScmObject));
extern ScmObject P_Cdaaar P_((ScmObject));
extern ScmObject P_Cdaadr P_((ScmObject));
extern ScmObject P_Cdadar P_((ScmObject));
extern ScmObject P_Cdaddr P_((ScmObject));
extern ScmObject P_Cddaar P_((ScmObject));
extern ScmObject P_Cddadr P_((ScmObject));
extern ScmObject P_Cdddar P_((ScmObject));
extern ScmObject P_Cddddr P_((ScmObject));
extern ScmObject P_Append P_((int, ScmObject*));
extern ScmObject P_Append_Set P_((int, ScmObject*));
extern ScmObject P_Last_Pair P_((ScmObject));
extern ScmObject P_Reverse P_((ScmObject));
extern ScmObject P_Reverse_Set P_((ScmObject));
extern ScmObject P_List_Tail P_((ScmObject, ScmObject));
extern ScmObject P_List_Ref P_((ScmObject, ScmObject));
extern ScmObject P_Assq P_((ScmObject, ScmObject));
extern ScmObject P_Assv P_((ScmObject, ScmObject));
extern ScmObject P_Assoc P_((ScmObject, ScmObject));
extern ScmObject P_Memq P_((ScmObject, ScmObject));
extern ScmObject P_Memv P_((ScmObject, ScmObject));
extern ScmObject P_Member P_((ScmObject, ScmObject));
extern ScmObject P_Make_List P_((ScmObject, ScmObject));
extern ScmObject Copy_List P_((ScmObject));
extern ScmObject Copy_List_Iterative P_((ScmObject));
extern int Fast_Length P_((ScmObject));
extern ScmObject Const_Cons P_((ScmObject, ScmObject));

/* Startup and termination
 */
extern ScmObject P_Command_Line_Args P_((void));
extern ScmObject P_Command_Line_String P_((void));
extern ScmObject P_Exit P_((int, ScmObject*));
extern void Elk_Init P_((int, char **av, int, char *));

/* Numbers
 */
extern ScmObject Make_Integer P_((int));
extern ScmObject Make_Unsigned P_((unsigned));
extern ScmObject Make_Long P_((long));
extern ScmObject Make_Unsigned_Long P_((unsigned long));
extern ScmObject Make_Integer64 P_((int64_t));

extern ScmObject Make_Reduced_Flonum P_((double));
extern ScmObject Make_Flonum P_((double));
extern ScmObject P_Numberp P_((ScmObject));
extern ScmObject P_Complexp P_((ScmObject));
extern ScmObject P_Realp P_((ScmObject));
extern ScmObject P_Rationalp P_((ScmObject));
extern ScmObject P_Integerp P_((ScmObject));
extern ScmObject P_Abs P_((ScmObject));
extern ScmObject P_Zerop P_((ScmObject));
extern ScmObject P_Positivep P_((ScmObject));
extern ScmObject P_Negativep P_((ScmObject));
extern ScmObject P_Oddp P_((ScmObject));
extern ScmObject P_Evenp P_((ScmObject));
extern ScmObject P_Exactp P_((ScmObject));
extern ScmObject P_Inexactp P_((ScmObject));
extern ScmObject P_Exact_To_Inexact P_((ScmObject));
extern ScmObject P_Inexact_To_Exact P_((ScmObject));
extern ScmObject P_Inc P_((ScmObject));
extern ScmObject P_Dec P_((ScmObject));
extern ScmObject P_Generic_Equal P_((int, ScmObject*));
extern ScmObject P_Generic_Less P_((int, ScmObject*));
extern ScmObject P_Generic_Greater P_((int, ScmObject*));
extern ScmObject P_Generic_Eq_Less P_((int, ScmObject*));
extern ScmObject P_Generic_Eq_Greater P_((int, ScmObject*));
extern ScmObject P_Generic_Plus P_((int, ScmObject*));
extern ScmObject P_Generic_Minus P_((int, ScmObject*));
extern ScmObject P_Generic_Multiply P_((int, ScmObject*));
extern ScmObject P_Generic_Divide P_((int, ScmObject*));
extern ScmObject P_Quotient P_((ScmObject, ScmObject));
extern ScmObject P_Remainder P_((ScmObject, ScmObject));
extern ScmObject P_Modulo P_((ScmObject, ScmObject));
extern ScmObject P_Gcd P_((int, ScmObject*));
extern ScmObject P_Lcm P_((int, ScmObject*));
extern ScmObject P_Floor P_((ScmObject));
extern ScmObject P_Ceiling P_((ScmObject));
extern ScmObject P_Truncate P_((ScmObject));
extern ScmObject P_Round P_((ScmObject));
extern ScmObject P_Sqrt P_((ScmObject));
extern ScmObject P_Exp P_((ScmObject));
extern ScmObject P_Log P_((ScmObject));
extern ScmObject P_Sin P_((ScmObject));
extern ScmObject P_Cos P_((ScmObject));
extern ScmObject P_Tan P_((ScmObject));
extern ScmObject P_Asin P_((ScmObject));
extern ScmObject P_Acos P_((ScmObject));
extern ScmObject P_Atan P_((int, ScmObject*));
extern ScmObject P_Min P_((int, ScmObject*));
extern ScmObject P_Max P_((int, ScmObject*));
extern ScmObject P_Random P_((void));
extern ScmObject P_Srandom P_((ScmObject));
extern ScmObject P_Number_To_String P_((int, ScmObject*));
extern double Get_Double P_((ScmObject));
extern int Get_Integer P_((ScmObject));
extern unsigned Get_Unsigned P_((ScmObject));
extern long Get_Long P_((ScmObject));
extern int64_t Get_Integer64 P_((ScmObject));

extern unsigned long Get_Unsigned_Long P_((ScmObject));
extern int Get_Exact_Integer P_((ScmObject));
extern unsigned Get_Exact_Unsigned P_((ScmObject));
extern long Get_Exact_Long P_((ScmObject));
extern unsigned long Get_Exact_Unsigned_Long P_((ScmObject));

/* Onfork handlers
 */
extern int Register_Onfork P_((void (*)(void)));
extern void Call_Onfork P_((void));

/* Define_Primitive()
 */
extern int Define_Primitive P_((ScmObject (*)(ELLIPSIS), const char*, int, int,
    enum discipline));

/* Output
 */
extern ScmObject P_Write P_((int, ScmObject*));
extern ScmObject P_Display P_((int, ScmObject*));
extern ScmObject P_Write_Char P_((int, ScmObject*));
extern ScmObject P_Newline P_((int, ScmObject*));
extern ScmObject P_Format P_((int, ScmObject*));
extern ScmObject P_Clear_Output_Port P_((int, ScmObject*));
extern ScmObject P_Flush_Output_Port P_((int, ScmObject*));
extern ScmObject P_Print P_((int, ScmObject*));
extern ScmObject P_Get_Output_String P_((ScmObject));
extern int Check_Output_Port P_((ScmObject));
extern int Discard_Output P_((ScmObject));
extern void Printf (ScmObject, char *, ...);
extern int Print_Object P_((ScmObject, ScmObject, int, int, int));
extern int General_Print_Object P_((ScmObject, ScmObject, int));
extern int Format P_((ScmObject, const char*, int, int, ScmObject*));
extern int Saved_Errno;

/* Evaluator, procedures, macros
 */
extern ScmObject Eval P_((ScmObject));
extern ScmObject P_Eval P_((int, ScmObject*));
extern ScmObject P_Apply P_((int, ScmObject*));
extern ScmObject Funcall P_((ScmObject, ScmObject, int));
extern ScmObject P_Lambda P_((ScmObject));
extern ScmObject P_Map P_((int, ScmObject*));
extern ScmObject P_Procedure_Environment P_((ScmObject));
extern ScmObject P_Procedure_Lambda P_((ScmObject));
extern ScmObject P_For_Each P_((int, ScmObject*));
extern ScmObject P_Procedurep P_((ScmObject));
extern ScmObject P_Macro P_((ScmObject));
extern ScmObject P_Macro_Body P_((ScmObject));
extern ScmObject P_Macro_Expand P_((ScmObject));
extern ScmObject P_Primitivep P_((ScmObject));
extern ScmObject P_Compoundp P_((ScmObject));
extern ScmObject P_Macrop P_((ScmObject));
extern int Check_Procedure P_((ScmObject));

/* Delay and force
 */
extern ScmObject P_Delay P_((ScmObject));
extern ScmObject P_Force P_((ScmObject));
extern ScmObject P_Promisep P_((ScmObject));
extern ScmObject P_Promise_Environment P_((ScmObject));

/* Input
 */
extern ScmObject P_Read P_((int, ScmObject*));
extern ScmObject P_Read_Char P_((int, ScmObject*));
extern ScmObject P_Peek_Char P_((int, ScmObject*));
extern ScmObject P_Char_Readyp P_((int, ScmObject*));
extern ScmObject P_Unread_Char P_((int, ScmObject*));
extern ScmObject P_Read_String P_((int, ScmObject*));
extern ScmObject P_Clear_Input_Port P_((int, ScmObject*));
extern ScmObject General_Read P_((ScmObject, int));
extern int Check_Input_Port P_((ScmObject));
extern int Discard_Input P_((ScmObject));
extern void Define_Reader P_((int, READFUN));

/* Special forms
 */
extern ScmObject P_Quote P_((ScmObject));
extern ScmObject P_If P_((ScmObject));
extern ScmObject P_Let P_((ScmObject));
extern ScmObject P_Letseq P_((ScmObject));
extern ScmObject P_Letrec P_((ScmObject));
extern ScmObject P_Case P_((ScmObject));
extern ScmObject P_Cond P_((ScmObject));
extern ScmObject P_And P_((ScmObject));
extern ScmObject P_Or P_((ScmObject));
extern ScmObject P_Do P_((ScmObject));
extern ScmObject P_Quasiquote P_((ScmObject));
extern ScmObject P_Fluid_Let P_((ScmObject));
extern ScmObject P_Begin P_((ScmObject));
extern ScmObject P_Begin1 P_((ScmObject));

/* Strings
 */
extern ScmObject Make_String P_((const char*, int));
extern ScmObject Make_Const_String P_((const char*, int));
extern ScmObject P_Make_String P_((int, ScmObject*));
extern ScmObject P_Stringp P_((ScmObject));
extern ScmObject P_String P_((int, ScmObject*));
extern ScmObject P_String_To_Number P_((int, ScmObject*));
extern ScmObject P_String_Eq P_((ScmObject, ScmObject));
extern ScmObject P_String_Less P_((ScmObject, ScmObject));
extern ScmObject P_String_Greater P_((ScmObject, ScmObject));
extern ScmObject P_String_Eq_Less P_((ScmObject, ScmObject));
extern ScmObject P_String_Eq_Greater P_((ScmObject, ScmObject));
extern ScmObject P_String_CI_Eq P_((ScmObject, ScmObject));
extern ScmObject P_String_CI_Less P_((ScmObject, ScmObject));
extern ScmObject P_String_CI_Greater P_((ScmObject, ScmObject));
extern ScmObject P_String_CI_Eq_Less P_((ScmObject, ScmObject));
extern ScmObject P_String_CI_Eq_Greater P_((ScmObject, ScmObject));
extern ScmObject P_String_Length P_((ScmObject));
extern ScmObject P_String_Ref P_((ScmObject, ScmObject));
extern ScmObject P_String_Set P_((ScmObject, ScmObject, ScmObject));
extern ScmObject P_Substring P_((ScmObject, ScmObject, ScmObject));
extern ScmObject P_String_Copy P_((ScmObject));
extern ScmObject P_String_Append P_((int, ScmObject*));
extern ScmObject P_List_To_String P_((ScmObject));
extern ScmObject P_String_To_List P_((ScmObject));
extern ScmObject P_Substring_Fill P_((ScmObject, ScmObject, ScmObject, ScmObject));
extern ScmObject P_String_Fill P_((ScmObject, ScmObject));
extern ScmObject P_Substringp P_((ScmObject, ScmObject));
extern ScmObject P_CI_Substringp P_((ScmObject, ScmObject));

/* Symbols, variables, frequently used Scheme objects
 */
extern ScmObject scm_Null, scm_True, scm_False, scm_Void, Newline, Eof;
extern ScmObject Intern P_((const char*));
extern ScmObject CI_Intern P_((const char*));
extern ScmObject P_Oblist P_((void));
extern ScmObject P_Symbolp P_((ScmObject));
extern ScmObject P_Symbol_To_String P_((ScmObject));
extern ScmObject P_String_To_Symbol P_((ScmObject));
extern ScmObject P_Put P_((int, ScmObject*));
extern ScmObject P_Get P_((ScmObject, ScmObject));
extern ScmObject P_Symbol_Plist P_((ScmObject));
extern void Define_Variable P_((ScmObject*, const char*, ScmObject));
extern void Define_Symbol P_((ScmObject *, const char*));
extern ScmObject Var_Get P_((ScmObject));
extern void Var_Set P_((ScmObject, ScmObject));
extern int Var_Is_True P_((ScmObject));
extern unsigned long Symbols_To_Bits P_((ScmObject, int, SYMDESCR*));
extern ScmObject Bits_To_Symbols P_((unsigned long, int, SYMDESCR*));

/* Termination functions
 */
extern void Register_Object P_((ScmObject, GENERIC, PFO, int));
extern void Deregister_Object P_((ScmObject));
extern ScmObject Find_Object (int, GENERIC, MATCHFUN, ...);
extern void Terminate_Group P_((GENERIC));
extern void Terminate_Type P_((int));

/* Types, Define_Type()
 */
extern TYPEDESCR *Types;
extern ScmObject P_Type P_((ScmObject));
extern int Wrong_Type P_((ScmObject, int));
extern int Wrong_Type_Combination P_((ScmObject, const char*));

/* STI-porting 10-14-99  Replaced these #define's with typedef's for ANSI compliance */
/*
#define SIZE_FUNC  int (*)(ScmObject)
#define EQUAL_FUNC int (*)(ScmObject, ScmObject)
#define PRINT_FUNC void (*)(ScmObject, ScmObject, int, int, int)
#define VISIT_FUNC void (*)(ScmObject*, void (*)(ScmObject*))
*/

/* STI-porting 10-14-99  typedef's for ANSI compliance */

typedef int  (*SIZE_FUNC) (ScmObject) ;
typedef int (*EQUAL_FUNC) (ScmObject, ScmObject) ;
typedef void (*PRINT_FUNC) (ScmObject, ScmObject, int, int, int) ;
typedef void (*VISIT_FUNC) (ScmObject*, void (*)(ScmObject*)) ;

/* STIPORT TAC */
#if ! defined (DEFINE_TYPE_IS_DECLARED)
extern int
	Define_Type P_((
	int,
	const char*,
	SIZE_FUNC,
	int,
	EQUAL_FUNC,
	EQUAL_FUNC,
	PRINT_FUNC,
	VISIT_FUNC));
#endif

/* Vectors
 */
extern ScmObject Make_Vector P_((int, ScmObject));
extern ScmObject Make_Const_Vector P_((int, ScmObject));
extern ScmObject P_Make_Vector P_((int, ScmObject*));
extern ScmObject P_Vectorp P_((ScmObject));
extern ScmObject P_Vector P_((int, ScmObject*));
extern ScmObject P_Vector_Length P_((ScmObject));
extern ScmObject P_Vector_Ref P_((ScmObject, ScmObject));
extern ScmObject P_Vector_Set P_((ScmObject, ScmObject, ScmObject));
extern ScmObject P_Vector_To_List P_((ScmObject));
extern ScmObject P_List_To_Vector P_((ScmObject));
extern ScmObject P_Vector_Fill P_((ScmObject, ScmObject));
extern ScmObject P_Vector_Copy P_((ScmObject));

C_LINKAGE_END
#endif
	
