/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Main definition file for Acis error system.

// The major entries are sys_error and sys_warning, which take as
// argument an error indication, whose type is also defined here.
// Sys_error never returns, but takes a long jump to a previously-set
// place (normally in the API). Sys_warning returns normally, but leaves
// lying around an indication of the fact that the warning has happened
// for the application to inspect if it is interested.

// When an application recovers control after an API call, any error
// is returned in the "outcome" for inspection. Warnings are not
// returned directly, but the routine get_warnings may be used to
// obtain a list of all warnings issued by the API routine. In either
// case find_error_message may be used to obtain a pointer to a
// textual error message corresponding to the error or warning.

// On entry to an API routine, certain system signals need to be trapped
// in order to ensure tidy return from the API routine, and on exit
// the previous state must be restored. A special case is user
// interrupt, which is passed on to the caller's interrupt routine if
// there is one, after cleaning up (rolling back) the effects of the
// aborted API routine. Since there are certain critical sections within
// the rollback mechanism where such cleanup would be impossible, there
// are routines to raise and lower a "critical" flag, and interrupts
// are deferred until the end of the critical section.

#if !defined( ERROR_SYS )
#define ERROR_SYS

#include "dcl_base.h"
#include "logical.h"
#include <setjmp.h>
#include "err_info_base.hxx"
#include "debugmsc.hxx"

//#define GME_BASE_ERRORBASE

/**
 * \defgroup ACISERRORMANAGEMENT Error Management
 * \ingroup ACISBASE
 *
 */
/**
* @file errorbase.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

// Runtime checking ensuring API_ENDs are executed
#include "chkmacro.hxx"


// ====== Functions implemented in errorsys.cxx


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Signals ACIS errors and interrupts.
 * <br><br>
 * <b>Role:</b> Calling <tt>sys_error</tt> will invoke a C++ exception to be thrown. Hence, control
 * is transferred to the responsible <tt>catch</tt> block. This can be your own <tt>catch</tt> block,
 * an ACIS <tt>EXCEPTION_CATCH</tt>, or an <tt>API_END</tt> (which contains an <tt>EXCEPTION_CATCH</tt>).
 * <br><br>
 * <i><b>Note:</b> Since this causes an exception, if no catching mechanism is provided, the application
 * will abort.</i>
 * <br><br>
 * @param mess_num
 * error message number.
 * <br><br>
 * @see err_mess_type, sys_warning
 */
DECL_BASE void sys_error( err_mess_type mess_num );
DECL_BASE void gme_sys_error( err_mess_type mess_num );


/**
 * Signals an ACIS warning.
 * <br><br>
 * <b>Role:</b> Reports unexpected, nonfatal events that occurred during the course
 * of the execution. It saves the error code given as input in an <tt>err_mess_type</tt> global
 * array. Like <tt>sys_error</tt>, this routine checks for the crash option. If the option is on,
 * it prints the message associated with the current error code. Otherwise, the
 * application must process this warning.
 * <br><br>
 * @param mess_num
 * error message number.
 * <br><br>
 * @see err_mess_type, sys_error
 */
DECL_BASE void sys_warning( err_mess_type mess_num);
DECL_BASE void gme_sys_warning( err_mess_type mess_num);

#ifdef __cplusplus
}
#endif

// Allow registration of an abort callback
/**
 * @nodoc
 */
typedef int (*acis_abort_callback)();
/**
 * @nodoc
 */
DECL_BASE acis_abort_callback register_abort_callback( acis_abort_callback);


// STI swa (17Mar98) -- Added over-loaded versions of sys_error
// that set the global error info object pointer before calling
// the usual version of sys_error.  These new versions are added
// for convienence to allow the user to pass additional
// information through the sys_error longjmp easily. In
// particular, API_SYS_BEGIN has been modified to look at
// the global error_info_base pointer on error and put its contents
// into the returned outcome.

// Set the global error_info_base object pointer to a given error_info_base object.

/**
 * Signals ACIS errors and interrupts.
 * <br><br>
 * <b>Role:</b> Calling <tt>sys_error</tt> will invoke a C++ exception to be thrown. Hence, control
 * is transferred to the responsible <tt>catch</tt> block. This can be your own <tt>catch</tt> block,
 * an ACIS <tt>EXCEPTION_CATCH</tt>, or an <tt>API_END</tt> (which contains an <tt>EXCEPTION_CATCH</tt>).
 * <br><br>
 * <i><b>Note:</b> Since this causes an exception, if no catching mechanism is provided, the application
 * will abort.</i>
 * <br><br>
 * This version of <tt>sys_error</tt> has a <tt>error_info_base</tt> parameter. This allows the caller
 * to specify additional information about the error.
 * <br><br>
 * @param mess_num
 * error message number.
 * @param e_info
 * extra information.
 * <br><br>
 * @see err_mess_type, sys_warning, EXCEPTION_CATCH
 */
DECL_BASE void sys_error(	err_mess_type mess_num,
							error_info_base *e_info );
DECL_BASE void gme_sys_error(	err_mess_type mess_num,
							error_info_base *e_info );

// STI swa end

/**
 * Resets the number of warnings to 0.
 * <br><br>
 * <b>Role:</b> This is done automatically by <tt>error_begin()</tt>, but may be useful in special circumstances.
 */
DECL_BASE void init_warnings();
DECL_BASE void gme_init_warnings();
/**
 * Obtains the warnings list.
 * <br><br>
 * @param warning_list
 * Returned list of warnings.
 */
DECL_BASE int get_warnings( err_mess_type *& warning_list );
DECL_BASE int gme_get_warnings( err_mess_type *& warning_list );

// ====== Functions implemented in find_error.cxx
/**
 * Gets the message string for an error number.
 * <br><br>
 * <b>Role:</b> Returns <tt>"unknown error"</tt> if the error number is invalid.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param err_num
 * error number.
 */
DECL_BASE char const *find_err_mess( err_mess_type err_num );
DECL_BASE char const *gme_find_err_mess( err_mess_type err_num );
/**
 * Returns the identifier string for an error number.
 * <br><br>
 * <b>Role:</b> Returns <tt>"UNKNOWN"</tt> if the error number is invalid.
 * <br><br>
 * @param err_num
 * error number.
 */
DECL_BASE char const *find_err_ident( err_mess_type err_num );
DECL_BASE char const *gme_find_err_ident( err_mess_type err_num );

/**
 * Translates the error number to a string containing the name of the module associated with the given error number.
 * <br><br>
 * @param err_num
 * error number.
 */
#ifdef GME_BASE_ERRORBASE
char const *find_err_module( err_mess_type err_num );
#else
DECL_BASE char const *find_err_module( err_mess_type err_num );
#endif // DEBUG
DECL_BASE char const *gme_find_err_module( err_mess_type err_num );
/**
 * Prints the message associated with the current error number in a simple format for debugging purposes.
 * <br><br>
 * @param msg
 * string that will prefix the message.
 * @param err_num
 * error number.
 * @param fptr
 * file handle where the message will be printed.
 */
DECL_BASE void print_warnerr_mess(
			char const * msg,
			err_mess_type err_num,
			FILE * fptr
		);
DECL_BASE void gme_print_warnerr_mess(
			char const * msg,
			err_mess_type err_num,
			FILE * fptr
		);

/**
 * @nodoc
 */
inline DECL_BASE void print_warning_mess(
			err_mess_type err_num,
			FILE *fp
		)
{
	print_warnerr_mess( "Warning", err_num, fp );
}

/**
 * @nodoc
 */
inline DECL_BASE void print_error_mess(
			err_mess_type err_num,
			FILE *fp
		)
{
	print_warnerr_mess( "Error", err_num, fp );
}


// ====== Functions implemented in interrupt.cxx

#ifdef __cplusplus
extern "C" {
#endif

// Routine to initialise fault traps.

/**
 * Enables or disables exceptions when floating point errors occur.
 * <br><br>
 * <b>Role:</b> Floating point errors, such as division by zero, can effect
 * the integrity of a model. ACIS therfore utilizes hardware capabilities to
 * identify and handle floating point errors accordingly. This is a state flag
 * which remains in affect util the next call of this function.
 * <br><br>
 * @param set_default
 * logical enabled, TRUE is the default.
 */
DECL_BASE void set_default_exception_control( logical set_default = TRUE );


/**
 * Establishes ACIS signal handling.
 * <br><br>
 * <b>Role:</b> Each call to <tt>error_begin</tt> must be offset by a
 * corresponding call to <tt>error_end</tt>. Calls to these two routines may be nested. The
 * outermost call to <tt>error_begin</tt> establishes signal handlers, resets the warning count,
 * and resets the error hardness level. Each call to <tt>error_begin</tt> increments the error
 * level.
 */
DECL_BASE void error_begin();
DECL_BASE void gme_error_begin();

// Restore the caller's fault traps before returning from an API
// routine.

/**
 * Resets signal handling.
 * <br><br>
 * <b>Role:</b>  Each call to <tt>error_end</tt> decrements the error level. When
 * the error level reaches zero, <tt>error_end</tt> resets the signal handlers to those that
 * were in effect when the corresponding call to <tt>error_begin</tt> was made. If a user
 * interrupt was seen, the application's interrupt signal handler is called.
 */
DECL_BASE void error_end();
DECL_BASE void gme_error_end();

// Indicate the start of a critical section in which user interrupts
// are to be deferred. These may be stacked indefinitely deeply.

/**
 * Inhibits processing of user interrupts.
 * <br><br>
 * <b>Role:</b> Each call to <tt>error_harden</tt> increments
 * the error hardness level. User interrupts are temporarily ignored while the error
 * hardness level is greater than zero. All other signals and errors are processed
 * normally.
 */
DECL_BASE void error_harden();
DECL_BASE void gme_error_harden();
// Indicate the end of a critical section. If the number of "soften"s
// exactly matches the number of "harden"s since the last "begin", then
// act upon any deferred user interrupt.
/**
 * Enables processing of user interrupts.
 * <br><br>
 * <b>Role:</b> Each call to <tt>error_soften</tt> decrements the
 * error hardness level. When the error hardness level reaches zero, any user
 * interrupt that was ignored is processed.
 */
DECL_BASE void error_soften();
DECL_BASE void gme_error_soften();

DECL_BASE void gme_test_init();
/**
 * @nodoc
 */
class critical_section_block {
public:

	critical_section_block() { error_harden(); }
	~critical_section_block() { error_soften(); }
};


/**
 * @nodoc
 */
DECL_BASE logical error_hard();

/**
 * @nodoc
 */
DECL_BASE logical acis_interrupted();
DECL_BASE logical gme_acis_interrupted();
/**
 * @nodoc
 */
DECL_BASE void interrupt_acis();
DECL_BASE void gme_interrupt_acis();
/**
 * @nodoc
 */
DECL_BASE void uninterrupt_acis();
DECL_BASE void gme_uninterrupt_acis();
/**
 * @nodoc
 */
DECL_BASE void set_stack_limit( size_t);
/**
 * @nodoc
 */
DECL_BASE void check_stack_use();
/**
 * @nodoc
 */
DECL_BASE size_t get_stack_high_water_mark();
/**
 * @nodoc
 */
#ifdef INTERNAL_DEBUG_CHECKS
#define STKLIM_ENTER check_stack_use();
#else
# define STKLIM_ENTER
#endif
/**
 * @nodoc
 */
DECL_BASE void set_pending_error( err_mess_type);
/**
 * @nodoc
 */
DECL_BASE err_mess_type get_pending_error();
/**
 * @nodoc
 */
DECL_BASE void set_unstoppable_error( err_mess_type);

#ifdef __cplusplus
}
#endif

// Declare a class to contain a jmp_buf, and also an indication of
// whether it is initialised or not, so that we can avoid longjumping
// to a bogus state.
/**
 * @nodoc
 */
class DECL_BASE error_save : public ACIS_OBJECT {
public:
	// knt 10 Aug 2006. 64-bit managed code apps that build against ACIS and
	// use EXCEPTION blocks fail to compile because the compiler complains
	// about alignment issues with the jmp_buf. We can just remove this 
	// for MS builds as we use SEH for MS platforms anyway.
#ifndef _MSC_VER
	jmp_buf buffer;
#endif
	logical buffer_init;
	logical throwable;
	error_save() : buffer_init(FALSE), throwable(TRUE) {}
};

/**
 * @nodoc
 */
DECL_BASE error_save& get_error_mark();
DECL_BASE error_save& gme_get_error_mark();

// The class exception_save is used to save and restore the error context.
// A class is used to allow statements (such as return, break, continue, and
// goto) which transfer control outside the EXCEPTION_BEGIN/END block to be
// used.
/**
 * @nodoc
 */
class DECL_BASE exception_save : public ACIS_OBJECT
{
private:
	error_save error_save_mark;
	logical saved;

public:
	// Construct an uninitialised object.

	exception_save()
	{
		saved = FALSE;
	}

	// Destructor unwinds error handling, but only if it has been
	// wound up.
	
	//@todo 开启GME_ERROR会影响现有acis异常处理的使用，为避免对开发造成影响，暂不开启。

	#ifdef GME_BASE_ERRORBASE
	~exception_save()
	{
		if (saved) {
			gme_get_error_mark() = error_save_mark;
			gme_error_end();
		}
	}  
	void begin();

	#else
	~exception_save()
	{
		if (saved) {
			get_error_mark() = error_save_mark;
			error_end();
		}
	}

	// Mark the beginning of the error-handling section. This may not
	// be where the object is constructed, so that destructors can be
	// called in the correct order.
	void begin()
	{
		if (!saved) {
			error_begin();
			error_save_mark = get_error_mark();
			saved = TRUE;
		}
	}
	#endif 
    void gme_begin();
};


// C++ try-catch.
// Hardware exceptions appear as thrown Acis error messages, encapsulated
// in an exception object.

/**
 * @nodoc
 */
class acis_exception /* : public ACIS_OBJECT no memory managment for this class */ {
public:
	err_mess_type mess;
	error_info_base*   info;
	const char * file;
	int line;

	acis_exception(const char * gme, err_mess_type m, error_info_base* i = NULL, const char * f = NULL, int l = 0)
		: mess(m), info(i), file(f), line(l) {
		if(NULL!=i) i->gme_add();
	}

	acis_exception(const char * gme, const acis_exception &old)
		: mess(old.mess), info(old.info), file(old.file), line(old.line) {
		if(info) info->gme_add();
	}

	error_info_base* gme_set_info( error_info_base* i, const char * f = NULL, int l = 0 ) {
		SPAUNUSED(f)
		SPAUNUSED(l)
		if ( i != info ) {
			if ( info ) info->gme_remove();
			info = i;
			if ( info ) info->gme_add();
		}

#ifdef SPA_DEBUG
		if( f) {
		file = f;
		line = l;
		}
#endif

		return info;
	}

    acis_exception& gme_operator_assign(const acis_exception& o) {
        if(this==&o){
            //赋值给自身
            return *this;
        }
        mess = o.mess;
        gme_set_info(o.info);
		file = o.file;
		line = o.line;
        return *this;
    }

	#ifdef GME_BASE_ERRORBASE		
    acis_exception( err_mess_type m, error_info_base* i = NULL, const char * f = NULL, int l = 0)
		: acis_exception("gme", m, i, f, l) {
	}

	acis_exception(const acis_exception &old)
		: acis_exception("gme", old) {
	}

	~acis_exception() {
		if(info) info->gme_remove(); // 析构函数直接改为使用gme函数
	}

	error_info_base* set_info( error_info_base* i, const char * f = NULL, int l = 0 ) {
		return gme_set_info(i, f, l);
	}

	acis_exception& operator=(const acis_exception& o) {
		return gme_operator_assign(o);
	}
	#else
    acis_exception( err_mess_type m, error_info_base* i = NULL, const char * f = NULL, int l = 0)
		: mess(m), info(i), file(f), line(l) {
		if(NULL!=i) i->add();
	}

	acis_exception(const acis_exception &old)
		: mess(old.mess), info(old.info), file(old.file), line(old.line) {
          if(info) info->add();
	}

	~acis_exception() {
		if(info) info->remove();
	}

    error_info_base* set_info( error_info_base* i, const char * f = NULL, int l = 0 ) {
		SPAUNUSED(f)
		SPAUNUSED(l)
        if ( i != info ) {
            if ( info ) info->remove();
            info = i;
            if ( info ) info->add();
        }

#ifdef SPA_DEBUG
		if( f) {
		file = f;
		line = l;
		}
#endif

        return info;
    }

    acis_exception& operator=(const acis_exception& o) {
        mess = o.mess;
        set_info(o.info);
		file = o.file;
		line = o.line;
        return *this;
    }
	#endif
};
/**
 * Exception block begin.
 */
#define EXCEPTION_BEGIN \
	{ \
		err_mess_type resignal_no = 0; \
		acis_exception error_info_holder(0); \
        error_info_base* error_info_base_ptr = NULL; \
		{ \
			exception_save exception_save_mark;

#define GME_EXCEPTION_BEGIN \
	{ \
		err_mess_type resignal_no = 0; \
		acis_exception error_info_holder("gme",0); \
        error_info_base* error_info_base_ptr = NULL; \
		{ \
			exception_save exception_save_mark;

#if defined( _MSC_VER )
/**
 * Exception try
 */
#define EXCEPTION_TRY \
			exception_save_mark.begin(); \
			get_error_mark().buffer_init = TRUE; \
			err_mess_type error_no = 0; \
			{ \
				{ \
					try {
#define GME_EXCEPTION_TRY \
			exception_save_mark.gme_begin(); \
			gme_get_error_mark().buffer_init = TRUE; \
			err_mess_type error_no = 0; \
			{ \
				{ \
					try {
#else
/**
 * Exception try
 */
#define EXCEPTION_TRY \
			exception_save_mark.begin(); \
			get_error_mark().buffer_init = TRUE; \
			err_mess_type error_no = setjmp( get_error_mark().buffer ); \
			{ \
				if( error_no != 0 ) { \
					resignal_no = error_no; \
				} \
				else \
				{ \
					try {
#define GME_EXCEPTION_TRY \
			exception_save_mark.gme_begin(); \
			gme_get_error_mark().buffer_init = TRUE; \
			err_mess_type error_no = setjmp( gme_get_error_mark().buffer ); \
			{ \
				if( error_no != 0 ) { \
					resignal_no = error_no; \
				} \
				else \
				{ \
					try {
#endif	
/**
 * Exception catch
 * @param always_clean
 * When true, always stops here, regardless of no error ocurring.
 */
#define EXCEPTION_CATCH(always_clean) \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				if((always_clean) || error_no != 0) \
				{ \
					try {

#define GME_EXCEPTION_CATCH(always_clean) \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.gme_set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				if((always_clean) || error_no != 0) \
				{ \
					try {
// Interrupt/error cleanup code goes here. Note that the preceding "try"
// isn't very useful, but allows the compile-time state to be the same
// before and after the EXCEPTION_CATCH, allowing this macro to be omitted
// if there is no explicit cleanup code required.
/**
 * Exception block end
 */
#define EXCEPTION_END \
					} catch(acis_exception ex) { \
						if( resignal_no == 0 ) { \
							resignal_no = error_no = ex.mess; \
							error_info_base_ptr = error_info_holder.set_info(ex.info, __FILE__,__LINE__); \
						} \
					} \
				} \
			} \
		} \
		if (resignal_no != 0 || acis_interrupted()) \
			sys_error( resignal_no,error_info_base_ptr); \
	}
#define GME_EXCEPTION_END \
					} catch(acis_exception ex) { \
						if( resignal_no == 0 ) { \
							resignal_no = error_no = ex.mess; \
							error_info_base_ptr = error_info_holder.gme_set_info(ex.info, __FILE__,__LINE__); \
						} \
					} \
				} \
			} \
		} \
		gme_error_end(); \
		if (resignal_no != 0 || gme_acis_interrupted()) \
			gme_sys_error( resignal_no,error_info_base_ptr); \
	}
/**
 * Exception block end. Does not rethrow exception.
 */
#define EXCEPTION_END_NO_RESIGNAL \
					} catch(acis_exception ex) { \
						if( resignal_no == 0 ) { \
							resignal_no = error_no = ex.mess; \
							error_info_base_ptr = error_info_holder.set_info(ex.info, __FILE__,__LINE__); \
						} \
					} \
				} \
			} \
		} \
		if( acis_interrupted()) \
			sys_error( resignal_no,error_info_base_ptr); \
	}
#define GME_EXCEPTION_END_NO_RESIGNAL \
					} catch(acis_exception ex) { \
						if( resignal_no == 0 ) { \
							resignal_no = error_no = ex.mess; \
							error_info_base_ptr = error_info_holder.gme_set_info(ex.info, __FILE__,__LINE__); \
						} \
					} \
				} \
			} \
		} \
		gme_error_end(); \
		if( gme_acis_interrupted()) \
			gme_sys_error( resignal_no,error_info_base_ptr); \
	}
/**
 * Exception block end. Always catch, regardless of exception or not.
 */
#define EXCEPTION_CATCH_TRUE \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				{ \
					try {
#define GME_EXCEPTION_CATCH_TRUE \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.gme_set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				{ \
					try {
/**
 * Exception block end. Catch only if exception thrown.
 */
#define EXCEPTION_CATCH_FALSE \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				if(error_no) \
				{ \
					try {

#define GME_EXCEPTION_CATCH_FALSE \
					; \
					} catch (acis_exception ex) { \
						resignal_no = error_no = ex.mess; \
						error_info_base_ptr = error_info_holder.gme_set_info(ex.info, __FILE__,__LINE__); \
					} \
				} \
				if(error_no) \
				{ \
					try {
/** @} */
#endif
