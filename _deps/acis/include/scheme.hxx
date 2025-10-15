/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//     C++ include file for interfacing with scheme
#ifdef NT
#pragma warning( disable : 4786 )
#endif // NT
#ifndef scheme_hxx
#define scheme_hxx
#include <stdio.h>
#include <string.h>
#include "base.hxx"
#include "logical.h"
#include "ccat.hxx"
#include "scheme.h"
/**
 * \defgroup SCMAPI Scheme Toolkit Support
 *
 * Scheme ACIS Interface Driver Extension demonstration application (Scheme AIDE).
 */
/**
 * @file scheme.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <scheme.hxx>
 * @{
 */
// Functions for testing the type of a scheme object
/**
 * Determines if a scheme object is  of type boolean
 */
 inline logical is_Scm_Boolean(ScmObject o) {return (TYPE(o) == T_Boolean);}
/**
 * Determines if a scheme object is  of type number
 */
 inline logical is_Scm_Number(ScmObject x) {
    long t = TYPE(x);
    return t == T_Flonum || t == T_Fixnum  || t == T_Bignum ? TRUE : FALSE;}
/**
 * Determines if a scheme object is  of type scheme number array
 */
logical is_Scm_Number_Array(ScmObject);
/**
 * Determines if a scheme object is  of type pair
 */
inline logical is_Scm_Pair(ScmObject o) {return (TYPE(o) == T_Pair);}
/**
 * Determines if a scheme object is  of type integer
 */
inline logical is_Scm_Integer(ScmObject x) {
    return TYPE(x) == T_Fixnum || TYPE(x) == T_Bignum ? TRUE : FALSE;}
/**
 * Determines if a scheme object is  of type long
 */
inline logical is_Scm_Long(ScmObject x) {
    return is_Scm_Integer(x);
}
/**
 * Is scheme object of type 64 bit integer
 */
#if defined(_M_IA64) || defined(_M_AMD64) || defined(WIN64) || defined(_WIN64) || defined(_M_X64)
inline logical is_Scm_Integer64(ScmObject x) {
    return is_Scm_Integer(x);
}
#endif
/**
 * Determines if a scheme object is  type hoops key
 */
inline long is_Scm_Hoops_Key(ScmObject o) {return is_Scm_Integer(o);}
/**
 * Determines if a scheme object is  of type integer array
 */
 logical is_Scm_Integer_Array(ScmObject);
/**
 * Determines if a scheme object is  of type long array
 */
inline logical is_Scm_Long_Array(ScmObject o) { return is_Scm_Integer_Array(o); }
/**
 * Determines if a scheme object is  of type character
 */
inline logical is_Scm_Char(ScmObject c) {return (TYPE(c) == T_Character);}

/**
 * Determines if a scheme object is  of type scheme symbol
 */
inline logical is_Scm_Symbol(ScmObject s) {return (TYPE(s) == T_Symbol);}
/**
 * Determines if a scheme object is  of type string
 */
inline logical is_Scm_String(ScmObject s) {
	return (TYPE(s) == T_String) || (TYPE(s) == T_Symbol);
}
/**
 * Determines if a scheme object is  of type scheme list
 */
inline logical is_Scm_List(ScmObject o) {
    return EQ(scm_True, P_Listp(o)) ? TRUE : FALSE;}

/**
 * Determines if a scheme object is  of type null
 */
inline logical is_Scm_Null(ScmObject o) {return (TYPE(o) == T_Null);}
/**
 * Is scheme object of type gvector
 */
inline logical is_Scm_Vector(ScmObject v) {return (TYPE(v) == T_Vector);}

/**
 * Determines if a scheme object is of type procedure
 */
inline logical is_Scm_Procedure(ScmObject p) {
    long t = TYPE(p);
    return t == T_Compound || t == T_Primitive ? TRUE : FALSE;
}
/**
 * Get the minum number of arguments the scheme extension requires
 */
inline int get_Scm_Procedure_MinArgs(ScmObject p) {
	long t = TYPE(p);
	return t == T_Compound ?
		COMPOUND(p)->min_args
	:
	    t == T_Primitive ?
		    PRIM(p)->minargs
        :
	        Wrong_Type_Combination(p, "procedure");
}

/**
 * Get the maximum number of arguments the scheme extension requires
 */
inline int get_Scm_Procedure_MaxArgs(ScmObject p) {
	long t = TYPE(p);
	return t == T_Compound ?
		COMPOUND(p)->max_args
	:
	    t == T_Primitive ?
		    PRIM(p)->maxargs
        :
	        Wrong_Type_Combination(p, "procedure");
}
/**
 * Converts scheme boolean into logical
 */
inline logical get_Scm_Boolean(ScmObject o) {return (Truep(o) ? TRUE : FALSE);}
/**
 * Converts scheme boolean arrat into array of logical
 */
int get_Scm_Boolean_Array(ScmObject, int*&);
/**
 * Converts scheme real into c++ double
 */

inline double get_Scm_Real(ScmObject o) {return Get_Double(o);}
/**
 * Converts scheme real arrat into c++ double array
 */

int get_Scm_Real_Array(ScmObject, double*&);
/**
 * Determines if a scheme object is of type real
 */

inline logical is_Scm_Real(ScmObject o) {return Numeric(TYPE(o));}
/**
 * Determines if a Scheme object is a list of reals.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param list
 * Scheme object.
 **/
logical is_Scm_Real_List(ScmObject list);
/**
 * Converts scheme real list into c++ double array
 */
int get_Scm_Real_List(ScmObject list, double *&dbls);
/**
 *
 */
class SPAdouble_vector;
int get_Scm_Real_Vector(ScmObject list,  SPAdouble_vector  &dbls);
/**
 * @nodoc
 */
void real_ListToArray(ScmObject list, double*& array, int count);
/**
 * @nodoc
 */
double get_Scm_Parameter(ScmObject);
/**
 * @nodoc
 */
logical is_Scm_Parameter(ScmObject);
/**
 * Converts scheme integer into c++ integer
 */
inline int get_Scm_Integer(ScmObject o) {return Get_Integer(o);}
/**
 * Converts scheme long  into c++ long
 */
inline long get_Scm_Long(ScmObject o) {return Get_Long(o);}

/**
 * @nodoc
 */
inline int64_t get_Scm_Integer64(ScmObject o) {return Get_Integer64(o);}
/**
 * @nodoc
 */
#if defined(_M_IA64) || defined(_M_AMD64) || defined(WIN64) || defined(_WIN64) || defined(_M_X64)
inline int64_t get_Scm_Hoops_Key(ScmObject o) {return Get_Integer64(o);}
#else
inline long get_Scm_Hoops_Key(ScmObject o) {return Get_Long(o);}
#endif

/**
 * Converts scheme integer list into c++ integer array
 */
int get_Scm_Integer_Array(ScmObject, int*&);
/**
 * Converts scheme long list into c++ long array
 */
int get_Scm_Long_Array(ScmObject, long*&);
/**
 * Makes scheme string from c++ chars
 */
ScmObject make_Scm_String(const char* s);
/**
 * Creates a C++ const char* from a Scheme string object.
 * <br><br>
 * <b>Role:</b> This function always reuses the same space, so if it is
 * necessary to retain the string for future use, copy it into another space.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param s
 * Scheme object.
 **/
const char* get_Scm_String(ScmObject s);
/**
 * Makes scheme string from c++ wide char
 */
ScmObject make_Scm_String(SPAWCHAR_T* ws);
/**
 * Makes wchar from scheme string object
 */
SPAWCHAR_T* get_Scm_BString(ScmObject);
/**
 * Makes scheme string from wchar
 */
ScmObject make_Scm_String(const SPAWCHAR_T* mbs);
/**
 * @nodoc
 */
const SPAWCHAR_T* get_Scm_ConstMBString(ScmObject);  // Corresponds to LPCWSTR
/**
 * @nodoc
 */
inline ScmObject make_Scm_Cons(ScmObject a, ScmObject d) {return P_Cons(a,d);}
/**
 * @nodoc
 */
inline ScmObject make_Scm_Pair(ScmObject a, ScmObject d) {return P_Cons(a,d);}
/**
 * @nodoc
 */
inline ScmObject make_Scm_Boolean(logical tf) {return (tf ? scm_True : scm_False);}
/**
 * Creates scheme logical object from c++ logical
 */
ScmObject make_Scm_Boolean_List(int, logical*);
/**
 * Creates scheme real object from c++ double
 */
inline ScmObject make_Scm_Real(double x) {return Make_Reduced_Flonum(x);}
/**
 * Creates scheme real list  object from c++ double array
 */
ScmObject make_Scm_Real_List(int, double*);
/**
 * Creates scheme integer object from c++ integer
 */
inline ScmObject make_Scm_Integer(int n) {return Make_Integer(n);}
/**
 * Creates scheme long object from c++ long
 */
inline ScmObject make_Scm_Long(long l) {return Make_Long(l);}
/**
 * @nodoc
 */
inline ScmObject make_Scm_Integer64(int64_t n) {return Make_Integer64(n);}
/**
 * @nodoc
 */
#if defined(_M_IA64) || defined(_M_AMD64) || defined(WIN64) || defined(_WIN64) || defined(_M_X64)
inline ScmObject make_Scm_Hoops_Key(int64_t n) {return Make_Integer64(n);}
#else
inline ScmObject make_Scm_Hoops_Key(long l) {return Make_Long(l);}
#endif
/**
 * Creates scheme integer list object from c++ integer array
 */
ScmObject make_Scm_Integer_List(int, int*);
/**
 * Creates scheme long list from c++ long array
 */
ScmObject make_Scm_Long_List(int, long*);

class  tensor;
/**
 * Make scheme tensor object from tensor
 */
ScmObject make_Scm_Tensor(tensor &t);
/**
 * @nodoc
 */
inline ScmObject make_Scm_Vector(int n, ScmObject o) {return Make_Vector(n, o);}
/**
 * Make scheme void object
 */
inline ScmObject make_Scm_EmptyList() {return scm_Null;}
/**
 * @nodoc
 */
inline ScmObject make_unspecified() {return scm_Null;}
/**
 * Get car from scheme list
 */
inline ScmObject scm_Car(ScmObject o) {return P_Car(o);}
/**
 * Get cdr from schmeme list
 */
inline ScmObject scm_Cdr(ScmObject o) {return P_Cdr(o);}
/**
 * Get number of elements in scheme list
 */
inline int Scm_List_Length(ScmObject o) {Check_List(o); return Fast_Length(o);}
/**
 * @nodoc
 */
inline ScmObject Scm_List_Ref(ScmObject o, int n) {
    return P_List_Ref(o,Make_Fixnum(n));
}
/**
 * @nodoc
 */
inline int Scm_Vector_Length(ScmObject x) {
    Check_Type (x, T_Vector);
    return VECTOR(x)->size;
}
/**
 * @nodoc
 */
inline ScmObject Scm_Vector_Ref(ScmObject o, int n) {
    return P_Vector_Ref(o,Make_Fixnum(n));
}
/**
 * @nodoc
 */
inline void Scm_Vector_Set(ScmObject v, int n, ScmObject o) {
    P_Vector_Set(v, Make_Fixnum(n), o);}
/**
 * @nodoc
 */
ScmObject make_Scm_Apair(const char*, ScmObject);
/**
 * @nodoc
 */
void SchemeInit(
    int heapsize = 0,			// in Kbytes. o means use default (512)
    int case_sensitive = 0,     // = 1 to make name comparisons case sensitive
    int gc_debug = 0);          // = 1 to turn on garbage collection debugging
/**
 * @nodoc
 */
void Init_User_Scheme();
/**
 * @nodoc
 */
extern "C" void Init_Loadpath (const char* loadpath);
/**
 * @nodoc
 */
void SchemeExit();

/**
 * @nodoc
 */
logical ExitCommandCalled();

C_LINKAGE_BEGIN
/**
 * @nodoc
 */
void Scheme_Exit(int exit_code);
/**
 * @nodoc
 */
void debug_ScmObject(ScmObject, int = 0);
C_LINKAGE_END

//----------------------------------------------------------------------
// stuff for registering procedures and initialization procs with scheme

#include "scmproc.hxx"
#include "scm_init.hxx"
/**
 * Macro for registering a scheme procedure (extension)
 */
#define SCM_PROC(n1, n2, name, proc) \
    static user_scm_proc name2(proc,_SI)(n1,n2,name,(ScmObject (*)(...))proc)
/**
 * Macro for registering a scheme procedure (extension)
 */
#define SCM_PROC_EX(n1, n2, name, proc, help) \
    static user_scm_proc name2(proc,_SI)(n1,n2,name,(ScmObject (*)(...))proc, help)
/**
 * Macro for registering a scheme initialization  procedure
 */
#define SCM_INIT(proc) \
    static user_scm_init name2(proc,_SI)(proc)

//----------------------------------------------------------------------

#ifdef SPA_DEBUG
	#define DEBUG_SCHEME_OBJECT(x)		debug_ScmObject( (x))
	#define DEBUG_SCHEME_OBJECT2(x,l)	debug_ScmObject( (x), (l))
#else
	#define DEBUG_SCHEME_OBJECT(x)
	#define DEBUG_SCHEME_OBJECT2(x,l)
#endif


#ifdef THREAD_SAFE_ACIS
LOCAL_PROC int this_component_id() { return 0; }
#endif

class outcome;
ScmObject make_Scm_Error_Info_List( outcome & result );

/** @} */
#endif
