/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dmexcept.hxx
// created  : October 21, 1998
// author   : J Hapholdt
// modified : GW Celniker
// synopsis : Portable macros that support exception handling.
//            This design and these macros were taken originally
//            from the ACIS project and "made" ACIS independent.
#ifndef DM_EXCEPT_HXX
#define DM_EXCEPT_HXX

#define DM_FREE_STORE_OVERFLOW              -24

#define DM_EXCEPTION_ACCESS_VIOLATION		     -1
#define DM_EXCEPTION_DATATYPE_MISALIGNMENT	 -2
#define DM_EXCEPTION_BREAKPOINT				         -3
#define DM_EXCEPTION_SINGLE_STEP			         -4
#define DM_EXCEPTION_ARRAY_BOUNDS_EXCEEDED	 -5
#define DM_EXCEPTION_FLT_DENORMAL_OPERAND	  -6
#define DM_EXCEPTION_FLT_DIVIDE_BY_ZERO		   -7
#define DM_EXCEPTION_FLT_INEXACT_RESULT		   -8
#define DM_EXCEPTION_FLT_INVALID_OPERATION	 -9
#define DM_EXCEPTION_FLT_OVERFLOW			        -10
#define DM_EXCEPTION_FLT_STACK_CHECK		      -11
#define DM_EXCEPTION_FLT_UNDERFLOW			       -12
#define DM_EXCEPTION_INT_DIVIDE_BY_ZERO		   -13
#define DM_EXCEPTION_INT_OVERFLOW			        -14
#define DM_EXCEPTION_PRIV_INSTRUCTION		     -15
#define DM_EXCEPTION_IN_PAGE_ERROR			       -16
#define DM_CONTROL_C_EXIT					              -17
#define DM_UNEXPECTED_EXCEPTION				         -18

#define DM_SIGINT_FAULT																					-19
#define DM_SIGSEGV_FAULT																				-20
#define DM_SIGFPE_FAULT																					-21
#define DM_SIGBUS_FAULT																					-22
#define DM_SIGSOFT_FAULT																				-23

#ifdef DM_USE_DM_EXCEPTIONS

#include <setjmp.h>
#include "dsstdef.hxx"   // #define DECL_DM   // #define DECL_DM

#ifndef logical
#define logical int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef int DM_err_mess_type;

DECL_DM void DM_error_begin();
DECL_DM void DM_error_end();
DECL_DM void DM_sys_error( DM_err_mess_type err_num );

// Declare a class to contain a jmp_buf, and also an 
// indication of whether it is initialised or not, so 
// that we can avoid longjumping to a bogus state. 
// Also, use buffer_init as a indicator whether we
// are at the outermost exception begin/end block and 
// should NOT continue propagating an exception.

class DECL_DM DM_error_save {

public:
	logical buffer_init;           // TRUE =resignal errors
                                // FALSE=don't resignal errors
	jmp_buf buffer;                // for set_jmp,long_jmp
                                //   location to resume processing
                                //   after a long_jmp
 DM_error_save() :              // Default Constructor
		buffer_init( FALSE) {}

} ; // end class DM_error_save 

// Use a global variable to mark run-time execution block state
//
// dm_error_mark.buffer_init = TRUE when executing code nested
//    within any number of exception blocks.
// dm_error_mark.buffer_init = FALSE when executing code
//    not nested within an exception block.

extern DECL_DM DM_error_save dm_error_mark ;

// The class DM_exception_save is used to save and 
// restore the exception block state stored in the
// global variable dm_error_mark.  Every time
// an exception block is entered, 
// 1.) a local DM_exception_block is created which 
//     stores the current global dm_error_mark state.  
// 2.) Then the global dm_error_mark.buffer_init value
//     is set to TRUE.
// Whenever the exception block is exited, the 
// local DM_exception_block variable goes out of scope
// and its destructor is executed (a C++ language feature).
// This destructor is used to restore the 
// dm_error_mark.buffer_init value.
//
// The consequence of this design is that code may
// be written which transfer execution control out
// of an exception block in any manner
// (using return, break, continue, and goto)
// and the global state will remain accurate.
// 
// The DM_exception_save automatic variables are 
// declared in the macro DM_EXCEPTION_TRY and
// go out of scope in the macro DM_EXCEPTION_END

class DECL_DM DM_exception_save
{
  private:
	  DM_error_save error_save_mark;

  public:
	  DM_exception_save()     //      Default Constructor
                           // eff: store global state in local
                           //      variable
                           {
		                           DM_error_begin();
                             error_save_mark = dm_error_mark ;
	                          }
	  ~DM_exception_save()    //      Destructor
                           // eff: restore local copy of state
                           //      to global variable
                           {
		                           dm_error_mark = error_save_mark;
		                           DM_error_end();
	                          }
} ; // end class DM_exception_save


// if the exception handling mechanism has not been selected
#if !defined( DM_UNIX_EXCEPTION_TRAP )
#if !defined( DM_CPLUSPLUS_EXCEPTION_TRAP )
#if !defined( DM_STRUCTURED_EXCEPTION_TRAP )

// Select the exception handling mechanism.
// For Windows NT with exception handling use C++ exception
//   handling, (try,catch,throw).
// For Windows NT without exception handling use structured
//   exception handling, (__try,__catch,__throw).
// For everything else(including unix) use (setjmp/longjmp).

// _MSC_VER is always defined when compiled under Windows NT
//     and defines the compile version.  It has a value of
//     1100 for Microsoft Visual C++ 5.0.
// _CPPUNWIND is defined for code compiled with
//     the /GX option (Enable Exception Handling).

#if defined( _MSC_VER )
# if defined( _CPPUNWIND )
#  define DM_CPLUSPLUS_EXCEPTION_TRAP
# else
#  define DM_STRUCTURED_EXCEPTION_TRAP
# endif
#else
# define DM_UNIX_EXCEPTION_TRAP
#endif

#endif // DM_UNIX_EXCEPTION_TRAP
#endif // DM_CPLUSPLUS_EXCEPTION_TRAP
#endif // DM_STRUCTURED_EXCEPTION_TRAP

// Now define the exception handling macros
// variables of the DM_EXCEPTION macros
//
// The exception mechanism uses 5 variables, 2 of which may
// be checked and used by application code, and 3 of which
// are completely managed by the DM_EXCEPTION macro set.
// 
// resignal_no:          The value of an exception.
//                       0=no exception, else exception number.
//                       May be checked or cleared by an application
//                       to control resignaling of exceptions.
// always_clean:         The input arg to DM_EXCEPTION_CATCH which
//                       must evaluate to TRUE or FALSE.
//                       TRUE=execute catch block code normally
//                            and when exceptions are thrown.
//                       FALSE=execute catch block code only
//                            when exceptions are thrown.                       
// dm_error_mark:        Never accessed by an Application.
//                       Used to automatically stop resignalling
//                       of exceptions at the outer most
//                       exception block.
// exception_save_mark:  Never accessed by an Application.
// ex_number:            Never accessed by an Application.
//
// note:        In a DM_EXCEPTION_TRY block whose value
//              always_clean is TRUE, the value of resignal_no
//              may be checked to see if an exception has been
//              thrown.  In this way, a catch block can
//              written which always frees scratch memory
//              used by the subroutine, and may additionally
//              free any other memory that needs to be cleaned
//              up to prevent a memory leak in the advent of
//              a thrown exception.
//
// note: Exceptions are resignaled whenever exception_no
//       is non-zero, and the global buffer_init value
//       is TRUE.  Resignaling may be stoped by an application
//       by setting resignal_no = 0 within a catch block.            
//
// usage: Any variable assigned heap memory by a call to
//        new or malloc should be declared between the
//        DM_EXCEPTION_BEGIN and the DM_EXCEPTION_TRY
//        blocks.  The code that makes the new or malloc
//        call needs to be in the DM_EXCEPTION_TRY block.
//        Code to free that memory should be added to the 
//        DM_CATCH_BLOCK. Subroutines that use scratch memory,
//        that is heap memory freed in the same subroutine
//        that allocated it should always use a 
//        DM_EXCEPTION_CATCH( TRUE ) block (equivalent to
//        a DM_EXCEPTION_CATCH_TRUE).  Subroutines
//        which allocate heap memory that persists beyond
//        the scope of the subroutine should have a
//        DM_EXCEPTION_CATCH( FALSE ) block (equivalent to
//        DM_EXCEPTION_CATCH_FALSE).  Subroutines
//        that do both will need to write an exception block
//        that looks as follows:
//
//        double *
//        typical_subroutine 
//        {
//          // return value
//          double *return_array = NULL ;
//          DM_EXCEPTION_BEGIN
//            // all local variables to be assigned heap memory
//            double *scratch_array = NULL ;
//          DM_EXCEPTION_TRY
//            // any other local variables
//            // make the heap memory assignements
//            return_array = new double[10] ;
//            if(!return_array) throw(DM_FREE_STORE_OVERFLOW) ;
//            
//            scratch_array = new double[10] ;
//            if(!scratch_array) throw(DM_FREE_STORE_OVERFLOW) ;
//
//            // additional application code goes here
//       
//          DM_EXCEPTION_CATCH( TRUE )
//            // free all scratch references
//            if(scratch_array) { delete [] scratch_array ;
//                                scratch_array = NULL ;
//                              }
//            // additional frees in case of an exception
//            if(resignal_no)
//              {
//                if(return_array) { delete [] return_array ;
//                                   return_array = NULL ;
//                                 }
//              }
//          DM_EXCEPTION_END
//          return(return_array) ;
//        } // end typical_subroutine


#if defined( DM_CPLUSPLUS_EXCEPTION_TRAP )

// C++ try/catch exception handling.

#if defined( _MSC_VER )
#include <eh.h>
typedef int (__cdecl *DoTranslateExceptionProc)(unsigned int code, _EXCEPTION_POINTERS *exceptionData);
DoTranslateExceptionProc __cdecl DM_register_do_translate_exception_proc(DoTranslateExceptionProc newProc);
#endif

class DM_exception {
public:
	DM_err_mess_type mess;

	DM_exception( DM_err_mess_type m ) { mess = m; }
};

#define DM_EXCEPTION_BEGIN \
	{ \
		DM_err_mess_type resignal_no = 0; \
		{

// Declarations of local variables to be cleaned up go here

#define DM_EXCEPTION_TRY \
			DM_exception_save exception_save_mark; \
			dm_error_mark.buffer_init = TRUE; \
			try {

// Normal processing code goes here. Note that the initial semicolon
// allows the last item in the try block to be a label. Apparently someone
// wants it.

// run DM_EXCEPTION_CATCH_TRUE to always clean up scratch memory
// with or without an exception being thrown.
#define DM_EXCEPTION_CATCH_TRUE \
			; } catch ( DM_exception ex ){ \
				resignal_no = ex.mess; \
			} { 

// Interrupt/error cleanup code goes here

// run DM_EXCEPTION_CATCH_FALSE to clean up allocated memory
// only in case of a exception.
#define DM_EXCEPTION_CATCH_FALSE \
			; } catch ( DM_exception ex ){ \
				resignal_no = ex.mess; \
			} \
			if( resignal_no ) { 

// Interrupt/error cleanup code goes here

#define DM_EXCEPTION_CATCH( always_clean ) \
			; } catch ( DM_exception ex ){ \
				resignal_no = ex.mess; \
			} \
			if( always_clean || resignal_no ) { 

// Interrupt/error cleanup code goes here

// old code line was
// 		if( resignal_no != 0 && dm_error_mark.buffer_init) 
#define DM_EXCEPTION_END \
			; } \
		} \
		if( resignal_no ) \
			DM_sys_error( resignal_no); \
	}

#endif // DM_CPLUSPLUS_EXCEPTION_TRAP


#if defined( DM_STRUCTURED_EXCEPTION_TRAP )

// Structured __try/__except exception handling.

#if defined( _MSC_VER )
#include <excpt.h>
#endif

#define DM_EXCEPTION_BEGIN \
	{ \
		DM_err_mess_type resignal_no = 0; \
		{

// Declarations of local variables to be cleaned up go here

#define DM_EXCEPTION_TRY \
			DM_exception_save exception_save_mark; \
			dm_error_mark.buffer_init = TRUE; \
			__try {

// Normal processing code goes here. Note that the initial semicolon
// allows the last item in the try block to be a label. Apparently someone
// wants it.

#define DM_EXCEPTION_CATCH_TRUE \
			; } __except ( 1 ) { \
				resignal_no = _exception_code(); \
			} { 

// Interrupt/error cleanup code goes here

#define DM_EXCEPTION_CATCH_FALSE \
			; } __except ( 1 ) { \
				resignal_no = _exception_code(); \
			} \
			if( resignal_no ) { 

// Interrupt/error cleanup code goes here

#define DM_EXCEPTION_CATCH( always_clean ) \
			; } __except ( 1 ) { \
				resignal_no = _exception_code(); \
			} \
			if( always_clean || resignal_no ) { 

// Interrupt/error cleanup code goes here

// old code line was
// 		if( resignal_no != 0 && dm_error_mark.buffer_init) 
#define DM_EXCEPTION_END \
			; } \
		} \
		if( resignal_no ) \
			DM_sys_error( resignal_no); \
	}

#endif // DM_STRUCTURED_EXCEPTION_TRAP


#if defined( DM_UNIX_EXCEPTION_TRAP )

// Normal Unix - setjmp/longjmp mechanism.

void DM_error_harden();
void DM_error_soften();

#define DM_EXCEPTION_BEGIN \
	{ \
		DM_err_mess_type resignal_no = 0; \
		{

// Declarations of local variables to be cleaned up go here

#define DM_EXCEPTION_TRY \
			DM_error_harden(); \
			logical in_cleanup = FALSE; \
			DM_exception_save exception_save_mark; \
			dm_error_mark.buffer_init = TRUE; \
			DM_err_mess_type error_no = setjmp( dm_error_mark.buffer ); \
			if (!in_cleanup) { \
				resignal_no = error_no; \
				if (error_no == 0) { \
					DM_error_soften();

// Normal processing code goes here. Note that the initial semicolon
// allows the last item in the try block to be a label. Apparently someone
// wants it.

#define DM_EXCEPTION_CATCH_TRUE \
				; } { \
					in_cleanup = TRUE;

#define DM_EXCEPTION_CATCH_FALSE \
				; } \
				if ( error_no != 0) { \
					in_cleanup = TRUE;

#define DM_EXCEPTION_CATCH( always_clean ) \
				; } \
				if ((always_clean) || error_no != 0) { \
					in_cleanup = TRUE;

// Interrupt/error cleanup code goes here

#define DM_EXCEPTION_END \
				; } \
			} \
		} \
		if( resignal_no ) \
			DM_sys_error( resignal_no ); \
	}

#endif // DM_UNIX_EXCEPTION_TRAP

#else /* using ACIS EXCEPTIONS */

#include "errorbase.hxx"
#include "dcl_ds.h"

#define DM_EXCEPTION_BEGIN       EXCEPTION_BEGIN
#define DM_EXCEPTION_TRY         EXCEPTION_TRY
#define DM_EXCEPTION_CATCH_TRUE  EXCEPTION_CATCH_TRUE
#define DM_EXCEPTION_CATCH_FALSE EXCEPTION_CATCH_FALSE
#define DM_EXCEPTION_END         EXCEPTION_END

DECL_DM void DM_sys_error(int err_num );

#endif /* DM_USE_DM_EXCEPTIONS */

int DS_process_error(int & resignal_no);

#endif // DM_EXCEPT_HXX
