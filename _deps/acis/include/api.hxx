/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for outcome and macros used in API routines.
// This header must be included directly (or via api/misc.hxx) in
// any application code that calls API routines.  The macros will also
// be useful when applications call Acis routines directly.
#if !defined( API_HEADER )
#define API_HEADER
#include <stdio.h>
#include "dcl_kern.h"
//#include "api.hxx"
#include "logical.h"
#include <setjmp.h>
#include "bullsmal.hxx"
#include "debugmsc.hxx"
#include "errorsys.hxx"
#include "acis_options.hxx"
#include "vers.hxx"
class BULLETIN_BOARD;
#include "err_info.hxx"
#include "err_info_list.hxx"
#include "problems_list.hxx"

//#define GME_KERNEL_API

/**
 * @file api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISHISTORY
 *
 * @{
 */

// Simple controls on API functioning.
/**
 * @nodoc
 */
#define api_checking_on (api_check_on())

/**
 * @nodoc
 */
DECL_KERN void set_api_checking( logical );

/**
 * Determines the status of checking and returns <tt>TRUE</tt> if it is on, otherwise <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
**/
DECL_KERN logical api_check_on();

// Define a class for the result of API calls. This will be adaptable
// in the future to allow extra information, but for now contains an
// error number to indicate success/failure of the API routine, together
// with a bulletin board pointer.

// STI swa (17Mar98) -- added pointer to error_info object to allow
// return of additional error information in outcome class.  Also added
// member functions to handle the pointer properly.
class BULLETIN_BOARD;
/**
 * @nodoc
 */
#define API_SUCCESS SPA_NO_ERROR

/**
 * Defines a class for holding the results of API calls.
 * <br>
 * <b>Role:</b> This class contains an error number to indicate the success or failure
 * of the API routine, together with a bulletin board pointer. This class 
 * also contains <tt>error_info</tt> objects which encapsulate errors or problems that the
 * API encountered.
 *
 * @see BULLETIN_BOARD, error_info, error_info_list
 */
class DECL_KERN outcome : public ACIS_OBJECT {
	// Error number, 0 for successful outcome.

	err_mess_type quant;

	// Pointer to bulletin board for API call that produced the outcome.

	BULLETIN_BOARD *bb_ptr;

    // pointer to error_info object

    error_info *e_info;

	// pointer to a list of problems
	problems_list* problems_ptr;

	// method to instantiate a problems list, if necessary
	problems_list* get_problems_ptr();

public:

    // new constructors for use with error_info objects
/**
 * Constructs an <tt>outcome</tt> object from the specified parameters.
 * <br><br>
 * @param i
 * error number.
 * @param e
 * pointer to error information.
 */

    outcome( err_mess_type i = API_SUCCESS, error_info *e = NULL );
	outcome(const char* gme, err_mess_type i = API_SUCCESS, error_info *e = NULL );
/**
 * Constructs one <tt>outcome</tt> object from another (copy constructor).
 * <br><br>
 * @param o
 * reference to another outcome.
 */
	outcome( const outcome& o );
	outcome(const char* gme, const outcome& o );

    // destructor needs to remove any error objects
/**
 * C++ destructor, deleting a <tt>outcome</tt>.
 */
    ~outcome();

	// Data reading routines
/**
 * Returns the error number of this <tt>outcome</tt>.
 */
	err_mess_type error_number() const { return quant; }

/**
 * Returns the bulletin board pointer used by this <tt>outcome</tt>.
 */
	BULLETIN_BOARD *bb() const { return bb_ptr; }

    // error_info routines
/**
 * Returns a pointer to the fatal error information of this <tt>outcome</tt>.
 */
	error_info *gme_get_error_info() const;
	#ifdef GME_KERNEL_API
	error_info *get_error_info() const{
		return gme_get_error_info();
	}
	#else
	error_info *get_error_info() const;
	#endif

/**
 * @nodoc
 */
	// This method is for internal use only.
	// Sets the error information of this outcome to the contents of the input item.
	void gme_set_error_info( error_info *e );
	#ifdef GME_KERNEL_API
	void set_error_info(error_info* e) {
		gme_set_error_info(e);
	}
	#else
	void set_error_info( error_info *e );
	#endif
    // new assignment operator to handle error_info use count
/**
 * Copies state of another <tt>outcome</tt> into this <tt>outcome</tt> (assignment operator)
 * <br><br>
 * @param o
 * outcome whose data is to be assigned.
 */
    outcome& operator=(const outcome& o);
	outcome& gme_operator_equal(const outcome& o);

	// Set the bb pointer
/**
 * Sets the bulletin board pointer of this <tt>outcome</tt>.
 * <br><br>
 * @param bb
 * bulletin board pointer.
 */
	void set_bb( BULLETIN_BOARD *bb ) { bb_ptr = bb; }

/**
 * Temporarily Ignore the failure of a nested API call.
 * <br><br>
 * <b>Role:</b> The integrity of a model is preserved by 'rolling' the failed <tt>BULLETIN_BOARD</tt>.
 * Nested API failures, however, may be temporarily ignored, until the outermost API block deals with the error.
 */
	void ignore();

	// Test success of an outcome.
/**
 * Returns <tt>TRUE</tt> if this <tt>outcome</tt> represents a successful result; otherwise, returns <tt>FALSE</tt>.
 */
	logical ok() const { return quant == API_SUCCESS; }

	// Print out details of an outcome.
/**
 * Outputs information about this <tt>outcome</tt> to the debug file or to another specified file.
 * <br><br>
 * <b>Role:</b> Sends the output to <tt>debug_file_ptr</tt> if no file is specified.
 * <br><br>
 * @param fp
 * FILE to which the information is to be written (optional).
 */
	void debug( FILE *fp = debug_file_ptr ) const;
	void gme_debug( FILE *fp = debug_file_ptr ) const;

/**
 * Returns <tt>TRUE</tt> if the <tt>outcome</tt> does not represent a successful result or if a fail-safe routine encountered error(s) and continued; otherwise returns <tt>FALSE</tt>.
 */
	logical encountered_errors() const;
	logical gme_encountered_errors() const;

	/**
	 * Adds into the incoming list <tt>error_info_list& errors</tt>, all the <tt>error_info</tt>
	 * objects that are stored by this <tt>outcome</tt> object, including the fatal one that is 
	 * returned by outcome::get_error_info().
	 * <br><br>
	 * @param errors
	 * error_info_list to which the error_info objects are added into.
	 */
	void get_error_info_list(error_info_list& errors) const;
	void gme_get_error_info_list(error_info_list& errors) const;
	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Sets the error_info_list stored in this outcome
	void set_error_info_list(error_info_list const& errors);
	void gme_set_error_info_list(error_info_list const& errors);

	/**
	 * @nodoc
	 */
	// This method is for internal use only
	void add_problems_list( problems_list* problems );

	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Add a non-fatal problem (error_info) to the outcome's error_info_list
	void add_problem(error_info *problem);
	
};

// The SPAparameter is an automatic object, presumably with an interesting constructor
// and destructor, which is declared outside the try block,
// but inside a pair of brackets.  Such an object can setup and tear down
// stuff as it wishes.
/**
 * @nodoc
 */
#define __ERROR_BEGIN_INTERNAL__(object) \
	outcome result(0); /* Was in API_SYS_BEGIN */ \
	err_mess_type error_no = 0; \
	problems_list_prop problems_prop; \
	EXCEPTION_BEGIN \
	object; \
	EXCEPTION_TRY

#define __GME_ERROR_BEGIN_INTERNAL__(object) \
	outcome result("gme",0); /* Was in API_SYS_BEGIN */ \
	err_mess_type error_no = 0; \
	GME_EXCEPTION_BEGIN \
	object; \
	GME_EXCEPTION_TRY
/**
 * @nodoc
 */
#define __ERROR_END_INTERNAL__ \
	EXCEPTION_CATCH_FALSE \
		/* note, this error_no is the one declared in the EXCEPTION_BEGIN macro. \
		 * Not the one from __ERROR_BEGIN_INTERNAL__.  For compatibility, we will capture it \
		 * from the result below \
		 */ \
		result = outcome(error_no, ERROR_INFO_PTR); \
	EXCEPTION_END_NO_RESIGNAL \
	error_no = result.error_number(); error_no = error_no;

#define __GME_ERROR_END_INTERNAL__ \
	GME_EXCEPTION_CATCH_FALSE \
		/* note, this error_no is the one declared in the EXCEPTION_BEGIN macro. \
		 * Not the one from __ERROR_BEGIN_INTERNAL__.  For compatibility, we will capture it \
		 * from the result below \
		 */ \
		result = outcome(error_no, GME_ERROR_INFO_PTR); \
	GME_EXCEPTION_END_NO_RESIGNAL \
	error_no = result.error_number(); error_no = error_no;
// Start and close off bulletin boards, not used directly, but in the following
// system macros. (The delete function doesn't seem to be used anywhere.)

/**
 * Starts the API bulletin board.
 * <br><br>
 * <b>Role:</b> This API function is not intended to be called directly by the
 * application, but rather via the <tt>API_BEGIN</tt> macro.
 * <br><br>
 * This routine may be used with <tt>api_bb_end</tt> to bracket a sequence of API calls so
 * that they produce a single bulletin board. Its effect is cumulative, so that when
 * there are nested calls to <tt>api_bb_begin</tt> and <tt>api_bb_end</tt>, only the
 * outermost pair of calls takes effect. In this way, a new API routine may call existing
 * API routines and appears to the caller like any other API routine in its handling of
 * bulletin boards.
 * <br><br>
 * It should normally be called with an argument of <tt>TRUE</tt>, but if called with <tt>FALSE</tt>,
 * the current bulletin board (if any) is "stacked", and a new one started anyway.
 * The corresponding <tt>api_bb_end</tt> rolls back and deletes this bulletin board, and
 * reinstates the stacked one for more changes.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * @param linear
 * flag to select linear or distributed history stream.
**/
DECL_KERN void api_bb_begin(logical linear = TRUE);

/**
 * Terminates the API bulletin board.
 * <br><br>
 * <b>Role:</b> This API function is not intended to be called directly by the
 * application, but rather via the <tt>API_END</tt> macro.
 * <br><br>
 * It is used with <tt>api_bb_begin</tt> to bracket a sequence of API calls so that they
 * produce a single bulletin board. Its effect is cumulative, so that when there are
 * nested calls to <tt>api_bb_begin</tt> and <tt>api_bb_end</tt>, only the outermost pair
 * of calls takes effect. In this way, a new API routine may call existing API routines and
 * appears to the caller like any other API routine in its handling of bulletin
 * boards. It should normally be called with the second argument set to <tt>TRUE</tt>.
 * <br><br>
 * Provided option <tt>logging</tt> is on and a bulletin board is already being constructed
 * and it matches the initial call to <tt>api_bb_begin</tt>, this routine ends the current
 * bulletin board, setting the success or failure as recorded in the given outcome
 * into the bulletin board, and setting a reference to the bulletin board into the
 * outcome.
 * <br><br>
 * It then decrements a flag to say that a bulletin board is being constructed
 * (unless the second argument is <tt>FALSE</tt>).
 * <br><br>
 * If <tt>delete_stacked_bb</tt> is <tt>TRUE</tt>, a stacked bulletin board that results from a
 * successful <tt>API_TRIAL</tt> block will be deleted.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param result
 * the current outcome.
 * @param linear
 * flag to select linear or distributed history stream.
 * @param delete_stacked_bb
 * flag to delete a stacked bulletin board.
**/
DECL_KERN void api_bb_end(
			outcome &result,
			logical linear = TRUE,
            logical delete_stacked_bb  = FALSE
		);

/**
 * Deletes bulletin boards.
 * <br><br>
 * <b>Role:</b> This API is not intended to be called directly by the
 * application. If a current bulletin board exists and has been ended and marked as
 * unsuccessful, this function rolls back the model by undoing the changes recorded
 * in the bulletin board, and then deletes the bulletin board (so freeing up the
 * space occupied by old versions of records).
 * <br><br>
 * <b>Effect:</b> System routine.
**/
DECL_KERN void api_bb_delete();


// STI swa (17Mar98) -- made a new API_SYS_BEGIN that initializes
// a global error_info object pointer, and uses this pointer
// when making an outcome following an error longjmp
//
//#define API_SYS_BEGIN \
//	outcome result; \
//	/* api_error_begin(); */ \
//	__ERROR_BEGIN_INTERNAL__ \
//	if (error_no != 0) { \
//		result = outcome( error_no ); \
//	} else {

// Note:  We had some brief concern about using automatic objects in these
//        macros because that would not be compatible with C.  However
//        Earlier versions of __ERROR_BEGIN_INTERNAL__/END used API_MACRO_CHECKER,
//        which uses an automatic apiMacroChecker.  Thus, these
//        macros have not been C compatible for some time.  We will
//        encourage people to do sensitive code in C++, but if
//        push ever comes to shove, we can use techniques similar to
//        what is done in except.cxx and except.h to bridge the gap.
//        (That code does not appear to be used anywhere either so ...)


// Manipulate a signal trap for user interrupt, to clean up bulletin
// board, etc.

// Macros for beginning and ending API routines.  All calls to Acis
// routines need to be bracketed by operations to set up the error
// system, and then to reset it to its prior state.  Anything which
// affects the model also require to initialise and close down the
// bulletin board.  We provide macros to simplify the source code:

//		API_BEGIN, API_END for ordinary api routines which return
//						a bulletin board
//		API_SYS_BEGIN, API_SYS_END for api system routines, generally
//						those which manipulate bulletin boards and
//						rollback, and which therefore do not return
//						bulletin boards.
//		API_NOP_BEGIN, API_NOP_END for api routines that wish to use
//						bulletin board facilities but also wish to
//						leave the bulletin board in the same state
//						it was in before API_NOP_BEGIN.
//      API_TRIAL_BEGIN, API_TRIAL_END for code that uses a heursitic
//                      that may fail.  The coder may check the result
//                      to see if a more robust algorithm is needed.
//                      In case of failure, the model will be rolled
//                      back as if the _NOP_ macros had been used.
//                      In the case of success, the model will be retained
//                      as if the unadorned api macros had been used.

// Set up the bulletin board, then the error system and the application
// callback flag.

// STI dgh - add exception checking at API level

// STI aed: Add error_begin/end so user interrupt handler does not get
//			called until after the Bulletin Board is closed.

/**
 * @nodoc
 */
#define API_SYS_BEGIN_INTERNAL(object) \
	set_global_error_info(); \
	__ERROR_BEGIN_INTERNAL__(object)

#define GME_API_SYS_BEGIN_INTERNAL(object) \
	gme_set_global_error_info(); \
	__GME_ERROR_BEGIN_INTERNAL__(object)
/**
 * @nodoc
 */
DECL_KERN void nested_state_check();

/**
 * @nodoc
 */
#define API_SYS_END_INTERNAL \
	__ERROR_END_INTERNAL__

#define GME_API_SYS_END_INTERNAL \
	__GME_ERROR_END_INTERNAL__
/**
 * Protects the contained logic block with ACIS exception handling without rollback behavior.
 * <br><br>
 * <b>Role:</b> The <tt>API_SYS_BEGIN/END</tt> macros provide a core set of functionality, 
 * with respect to the other API macros (<tt>API_BEGIN/END</tt>, <tt>API_TRIAL_BEGIN/END</tt>, etc...), 
 * and are of limited use outside of ACIS code. 
 * These macros do not engage ACIS history functionality 
 * and consequently cannot bracket code that has the potential to create or modify ACIS entities. 
 * They simply initiate the ACIS error recovery system, through the exception handling system, 
 * and are used in very controlled situations where the overhead of history mechanics are undesired 
 * due to performance considerations or in internal history code that manipulates history data. 
 * They do not provide the model integrity capabilities, through rollback, 
 * of the other API macros and therefore more closely resemble the ACIS exception handling macros 
 * (<tt>EXCEPTION_BEGIN/TRY/CATCH/END</tt>). 
 * Use these with care.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_SYS_BEGIN \
	API_SYS_BEGIN_INTERNAL(nested_state_check())

/**
 * Protects the contained logic block with ACIS exception handling without rollback behavior.
 * <br>
 * <i><b>Note:</b> See @href API_SYS_BEGIN for more information.
 */
#define API_SYS_END \
	API_SYS_END_INTERNAL \
	problems_prop.process_result( result, PROBLEMS_LIST_PROP_ONLY, FALSE );

// STI jmb: Use api_bb_save class so the appropriate balancing of code
//          occurs regardless of how the api block is exited.
/*
// tbrv
*/
/**
 * @nodoc
 */
class api_bb_save : public ACIS_OBJECT {
public:
  enum e_bb_type{
	normal,
	nop,
	trial
  };
private:
  e_bb_type  bb_type;        // What style of bb handling do we want.
  outcome&   result;         // The result will be put here before api_bb_end
  logical    was_logging;    // The trial and nop cases depend on our
                             // bulletin board logging, so force it on.
public:
  api_bb_save(outcome& r, e_bb_type bt) :
	bb_type(bt),
	result(r),
	was_logging(logging)
  {
	if(bb_type != normal)
	  set_logging(TRUE);
	api_bb_begin(bb_type == normal);
  }
  ~api_bb_save() {
	api_bb_end(result, bb_type != nop, !was_logging);
	set_logging(was_logging);
  }

// Assignment operator could not be generated warning
// is taken care of by adding a private one.  PRS
private:
	api_bb_save& operator=(const api_bb_save&) {return *(api_bb_save *)NULL_REF;}
};

/**
 * Protects the contained logic block with ACIS exception handling and provides rollback behavior in the event a modeling operation fails.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_BEGIN \
	API_SYS_BEGIN_INTERNAL(api_bb_save make_bulletin_board(result, api_bb_save::normal)) \
	ACIS_EXCEPTION_CHECK("API"); \
		logical call_update_from_bb = TRUE;

#define GME_API_BEGIN \
	API_SYS_BEGIN_INTERNAL() \
// Call the application callback if appropriate, then restore the error
// system, and then sort out the bulletin board
/**
 * Protects the contained logic block with ACIS exception handling and provides rollback behavior in the event a modeling operation fails.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_END \
		if (result.ok() && call_update_from_bb) \
			update_from_bb(); \
	API_SYS_END_INTERNAL \
	problems_prop.process_result( result, PROBLEMS_LIST_PROP_ONLY, FALSE );

#define GME_API_END \
	GME_API_SYS_END_INTERNAL \
// STI aed: end

// Macros for bracketing an api function which is to be treated as
// a no operation as far as the data structure is concerned.
// API_NOP_END restores the state as of the corresponding API_NOP_BEGIN
// including any currently-open bulletin board.

// Set up a new bulletin board and then the error system

// STI aed: Add error_begin/end so user interrupt handler does not get
//			called until after the Bulletin Board is closed.

/**
 * Provides a way to use the bulletin mechanism to make changes to an <tt>ENTITY</tt>, then throw away the changes when done.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_NOP_BEGIN \
	API_SYS_BEGIN_INTERNAL(api_bb_save make_bulletin_board(result,api_bb_save::nop));

// Restore the error system and then remove the bulletin board and
// restore the previous one.
/**
 * Provides a way to use the bulletin mechanism to make changes to an <tt>ENTITY</tt>, then throw away the changes when done.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_NOP_END \
	API_SYS_END_INTERNAL \
	problems_prop.process_result( result, PROBLEMS_LIST_PROP_IGNORE, FALSE );

// STI aed: end


// Macros for defining a block of code which should be executed as
// a particular version of Acis.
/**
 * <tt>API_VERS_BEGIN</tt> is a way to combine an <tt>API_BEGIN</tt> and <tt>ALGORITHMIC_VERSION_BLOCK</tt> in a single statement.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_VERS_BEGIN(opts) \
	API_BEGIN \
	ALGORITHMIC_VERSION_BLOCK(opts ? &opts->get_version() : NULL);

/**
 * @nodoc
 */
#define API_SYS_VERS_BEGIN(opts) \
	API_SYS_BEGIN \
	ALGORITHMIC_VERSION_BLOCK(opts ? &opts->get_version() : NULL);

/**
 * @nodoc
 */
#define API_NOP_VERS_BEGIN(opts) \
	API_NOP_BEGIN \
	ALGORITHMIC_VERSION_BLOCK(opts ? &opts->get_version() : NULL);

/**
 * @nodoc
 */
#define API_TRIAL_VERS_BEGIN(opts) \
	API_TRIAL_BEGIN \
	ALGORITHMIC_VERSION_BLOCK(opts ? &opts->get_version() : NULL);

// STI jmb: begin
// Macros for bracketing an api function which is to be tested for success
// and treated as a no operation as far as the data structure is concerned
// if it fails.  If it succeeds, the bulletins are merged into the active bulletin
// board.  If if fails, the state is restored as of the corresponding API_TRIAL_BEGIN
// including any currently-open bulletin board.

// Set up a new bulletin board and then the error system

// STI aed: Add error_begin/end so user interrupt handler does not get
//			called until after the Bulletin Board is closed.

/**
 * Brackets code that may or may not have results you want to keep.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_TRIAL_BEGIN \
	API_SYS_BEGIN_INTERNAL(api_bb_save make_bulletin_board(result,api_bb_save::trial)) \
	ACIS_EXCEPTION_CHECK("API"); \
		logical call_update_from_bb = TRUE;

// Restore the error system and then remove the bulletin board and
// restore the previous one.

/**
 * Brackets code that may or may not have results you want to keep.
 * <br>
 * <b>Note:</b> Refer to the technical article <i>[API Macros](http://doc.spatial.com/articles/a/p/i/API_Macros_b22b.html)</i> for
 * a full description of this macro and its use.
 */
#define API_TRIAL_END \
		if (result.ok() && call_update_from_bb) \
			update_from_bb(); \
	API_SYS_END_INTERNAL \
	problems_prop.process_result( result, PROBLEMS_LIST_PROP_OR_IGNORE, FALSE );

// STI jmb: end

// STI jmb:  Macros for the replacement of journalling.
/**
 * @nodoc
 */
#define API_DEBUG_ENTER(name) \
	DEBUG_LEVEL(DEBUG_CALLS) \
		fprintf(debug_file_ptr, "calling %s\n", name);

/**
 * @nodoc
 */
#define API_DEBUG_EXIT(name) \
	DEBUG_LEVEL(DEBUG_FLOW) \
		fprintf(debug_file_ptr, "leaving %s: %s\n", name, \
				find_err_ident(result.error_number()));
// STI jmb: end
/** @} */
#endif
