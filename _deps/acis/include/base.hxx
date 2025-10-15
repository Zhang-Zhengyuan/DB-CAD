/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* Main modeller header, containing system-wide information */
/* For obvious reasons, changes to this file have very serious */
/* knock-on effects */
#if !defined( BASE_HEADER_INCLUDED )
#define BASE_HEADER_INCLUDED

/**
 * \defgroup ACISBASE Base Functionality
 *
 */
/**
 * @file base.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISBASE
 *
 * @{
 */
#include "dcl_base.h"
#include "safe.hxx"

#if defined(__cplusplus) && defined(BASE_COMPAT)
    /* BASE_COMPAT is a temporary directive that allows
    // a transition period to the new type names.       */
    #define vector		SPAvector
    #define unit_vector	SPAunit_vector
    #define position	SPAposition
    #define matrix		SPAmatrix
    #define transf		SPAtransf
    #define interval	SPAinterval
    #define par_pos		SPApar_pos
    #define par_vec		SPApar_vec
    #define par_dir		SPApar_dir
    #define par_box		SPApar_box
    #define par_transf	SPApar_transf
    #define box			SPAbox
    #define nvector		SPAnvector
    #define parameter	SPAparameter
    #define resabs		SPAresabs
    #define resnor		SPAresnor
    #define resfit		SPAresfit
    #define resmch		SPAresmch
    #define null		SPAnull
#endif /* BASE_COMPAT and __cplusplus */
#include "mmgr.hxx"
#include "logical.h"

#ifdef _MSC_VER
    /**
     * @nodoc
     * Dummy function used to avoid compilation errors due to not used variables.
     */
    #define SPAUNUSED(a) a;
#else
    /**
     * @nodoc
     * Dummy function used to avoid compilation errors due to not used variables.
     */
    #define SPAUNUSED(a)
#endif

/* Added typedefs and/or headers for the intptr_t and uintptr_t types.
//  These are ANSI C/C++ "scaleable" types large enough to hold a
//  32bit or 64bit address respectively.
//  This block of code should be reduced to a few common header includes
//  in the near future.
*/

#if defined (mac)
    #include <stdint.h>
    /**
     * @nodoc
     */
    #define name2(a,b) a##b
#endif

#if !defined (OS_Linux)
    #include <stddef.h>
#endif

#if defined (_MSC_VER)
    #if !defined (_INTPTR_T_DEFINED)
        #if defined (_WIN64)
	        /**
	         * @nodoc
	         */
            typedef signed __int64 intptr_t;
	        /**
	         * @nodoc
	         */
            typedef unsigned __int64 uintptr_t;
        #else
	        /**
	         * @nodoc
	         */
            typedef signed int intptr_t;
            /**
	         * @nodoc
	         */
            typedef unsigned int uintptr_t;
        #endif
    #endif
#endif

/* ARG 6th April 2000. The "inttypes.h" header file only exists on
// Solaris 7 (and possibly 2.6), but breaks the build on 2.5.1 and earlier.
// I have provided explicit typedefs for all solaris versions, since I don't
// know how to distinguish between them. This code is taken from
// <sys/int_types.h> on Solaris 7.
*/

#if defined (solaris)
    #if defined(_LP64) || defined(_I32LPx)
	    typedef long			intptr_t;
	    typedef unsigned long		uintptr_t;
    #else
	    typedef	int			intptr_t;
	    typedef	unsigned int		uintptr_t;
    #endif
#endif

#if defined (hp700) || defined (sgi) || defined (aix)
    #include <inttypes.h>
#endif

#if defined (osf1)
 typedef signed long intptr_t;
 typedef unsigned long uintptr_t;
#endif

#if defined (linux)
    #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
        #include <inttypes.h>
    #else
       typedef signed int intptr_t;
       typedef unsigned int uintptr_t;
    #endif
#endif

#if defined (ACIS64) || defined (_WIN64)
    #define INTCAST (int)
    #ifdef _WIN64
        #define PTRtoINT(SPAptr) ((int)((__int64)(SPAptr)))
        #define INTEXTEND (__int64)
    #else
        #define PTRtoINT(SPAptr) ((int)((long)(SPAptr)))
        #define INTEXTEND (long)
    #endif
#else
    /* ugf 5/28/2014 - using (int)(long) so linux_a build works. Would prefer better design */
    #define PTRtoINT(SPAptr) ((int)((long)(SPAptr)))
    #define INTCAST
    #define INTEXTEND
#endif

#if defined (_WIN64) && (defined (BUILDING_ACIS) || defined(_MK_FWNAME_))
    #pragma warning( error: 4244 )
    #pragma warning( error: 4267 )
    #pragma warning( error: 4311 )
    #pragma warning( error: 4312 )
#endif

#ifdef _MSC_VER
# define SPAWCHAR_T __wchar_t
#else
#include <wchar.h>
# define SPAWCHAR_T wchar_t
#endif

#ifdef __cplusplus
    /** @} */
    /**
     * \addtogroup ACISMEMORYMANAGEMENT
     *
     * @{
     */
/**
     * Prototype of the complex allocator.
     * @param size
     * Size of allocation.
     * @param alloc_type
     * Type of allocation.
     * @param alloc_call
     * Style of allocation.
     * @param alloc_file
     * File that performed the allocation (usually __FILE__).
     * @param alloc_line
     * Line number of allocation (usually __LINE__).
     * @param alloc_file_index
     * Must be '&alloc_file_index'.
     */
    typedef void *(*complex_allocator_fn)(size_t size, AcisMemType alloc_type, AcisMemCall alloc_call, const char *alloc_file, int alloc_line, int *alloc_file_index);

    /**
     * Prototype of the complex destructor.
     * @param SPAptr
     * Pointer to memory to be freed.
     * @param alloc_call
     * Style of deallocation.
     * @param size
     * Size of deallocation.
     */
    typedef void (*complex_destructor_fn)(void *SPAptr, AcisMemCall alloc_call, size_t size);

    /**
     * Prototype of the raw allocator.
     * @param size
     * Size of allocation.
     */
    typedef void *(*raw_allocator_fn)(size_t size);

    /**
     * Prototype of the raw destructor.
     * @param SPAptr
     * Pointer to memory to be freed.
     */
    typedef void (*raw_destructor_fn)(void *SPAptr);

    /**
     * The base configuration object.
     * <br>
     * <b>Role:</b> When passed to the function <tt>initialize_base</tt>, this class contains
     * data members that allow the caller to specify flags to turn on/off memory auditing
     * and freelist functionality. The defaults have been chosen to provide the expected
     * desired behavior, so only users with special needs should need to explicitly
     * change the settings.
     * <br><br>
     * <tt>enable_freelists</tt> controls the use of ACIS freelisting. By default, <tt>enable_freelists</tt>
     * is set to TRUE, which is almost always faster than not freelisting. Disabling
     * freelists might make sense if you decide to replace our "raw" allocator/deallocator
     * with your own memory system that has a "smart" heap management system.
     * <br><br>
     * <tt>enable_audit_leaks</tt> controls the leak tracking facilities provided by the ACIS
     * memory manager.  By default, this is FALSE, since leak tracking incurs a performance
     * penalty. When set to TRUE, the ACIS memory tracking facilities are enabled. Leaks and other
     * memory statistics are gathered and sent to "mmgr.log". (The name of the output file can
     * be modified through the "mmgrfile" option.)
     * <br><br>
     * <tt>enable_audit_logs</tt> controls the logging of all ACIS allocations/deallocations.
     * By default, this is set FALSE, since logging all allocations is particularly
     * expensive.
     * <br><br>
     * The <tt>raw_allocator</tt> and <tt>raw_desctructor</tt> are function pointers within the
     * <tt>base_configuration</tt> object. If provided when calling <tt>initialize_base</tt>, the ACIS
     * Memory Manager will use these functions for all ACIS heap allocations
     * (including memory required to implement freelisting and leak tracking).
     * <br><br>
     * Specifying a <tt>raw_allocator</tt> and <tt>raw_destructor</tt> does not prevent ACIS from freelisting
     * or managing memory. If these function pointers are provided and ACIS freelisting
     * is enabled, the ACIS freelisting mechanism will use the <tt>raw_allocator</tt> and
     * <tt>raw_destructor</tt> for managing memory required for freelisting. If user wishes to
     * wholly replace ACIS memory management, there are two options:
     * <br><br>
     * 1)&nbsp;provide a <tt>raw_allocator</tt> and <tt>raw_destructor</tt>, and set <tt>enable_freelists</tt>,
     * <tt>enable_audit_logs</tt>, and <tt>enable_audit_leaks</tt>to FALSE; or
     * <br>
     * 2)&nbsp;use the <tt>complex_allocator</tt> and <tt>complex_destructor</tt>.
     * <br><br>
     * If a <tt>raw_allocator</tt> and <tt>raw_destructor</tt> are not provided, then by default, ACIS
     * will use the C-runtime functions "malloc" and "free" as the <tt>raw_allocator</tt> and
     * <tt>raw_destructor</tt>, respectively.
     * <br><br>
     * The <tt>complex_allocator</tt> and <tt>complex_destructor</tt> are function pointers that the
     * customer may initialize within the <tt>base_configuration</tt> and pass to <tt>initialize_base</tt>.
     * This allows the customer to intercept all heap allocations/deallocations without
     * ACIS having any knowledge of the allocation/deallocation. In contrast to the
     * <tt>raw_allocator</tt>, which describes the low-level allocation function which ACIS uses
     * for allocating memory, the <tt>complex_allocator</tt> replaces the ACIS memory management
     * system altogether.  The prototype of the <tt>complex_allocator</tt> provides enough
     * information to implement a memory management system as sophisticated as the ACIS
     * memory manager. If the <tt>complex_allocator</tt> and <tt>complex_destructor</tt> are provided,
     * all other <tt>base_configuration</tt> fields are ignored.
     * @see initialize_base
     */
    class DECL_BASE base_configuration {

    public:
	    /**
	     * Freelists flag.
	     */
	    logical enable_freelists;

	    /**
	     * Audit leaks flag.
	     */
	    logical enable_audit_leaks;

	    /**
	     * Audit logs flag.
	     */
	    logical enable_audit_logs;

	    /**
	     * Raw allocator.
	     */
	    raw_allocator_fn raw_allocator;

	    /**
	     * Raw destructor.
	     */
	    raw_destructor_fn raw_destructor;

	    /**
	     * Complex allocator.
	     */
	    complex_allocator_fn complex_allocator;

	    /**
	     * Complex destructor.
	     */
	    complex_destructor_fn complex_destructor;

            /**
	     * @nodoc
	     */
            base_configuration();
            base_configuration(logical);
            base_configuration( const char* gme);
    };
    /** @} */
    /**
     * \addtogroup ACISBASE
     *
     * @{
     */
    /**
     * Initializes the Base library.
     * <br><br>
     * <b>Role:</b> This direct interface call allows the caller to control
     * low-level ACIS preferences.  This includes the ability to control
     * freelisting, leak tracking, and even replacing the memory management
     * system altogether. See the <tt>base_configuration</tt> class documentation for
     * full details on what can be controlled.
     * The return value will be TRUE on success and FALSE when an error occurs. See the
     * Memory Management Technical Article for more information.
     * <br><br>
     * Note that if you plan to use this call to customize ACIS, the call
     * to <tt>initialize_base</tt> must precede the call to <tt>api_start_modeller</tt>.
     * <br><br>
     * @param base_config
     * configuration preferences.
     * <br><br>
     * @see base_configuration, terminate_base
     */
    DECL_BASE logical initialize_base( base_configuration * base_config = NULL);

    DECL_BASE logical gme_initialize_base( base_configuration * base_config = NULL);

    /**
     * Terminates the Base library.
     * <br><br>
     * <b>Role:</b> This direct interface call is the complement to
     * <tt>initialize_base</tt>. If you are using <tt>initialize_base</tt>, note that it must
     * precede the call to <tt>api_start_modeller</tt>.  Correspondingly, calling
     * <tt>terminate_base</tt> must follow the call to <tt>api_stop_modeller</tt>.
     * <br><br>
     * @see initialize_base
     */
    DECL_BASE logical terminate_base();

    DECL_BASE logical gme_terminate_base();

    /**
     * Returns TRUE if initialize_base has been called.
     * @see initialize_base
     */
    DECL_BASE logical base_initialized();

    DECL_BASE logical gme_base_initialized();

#endif

#if defined ( _WINDOWS )
    /**
     * @nodoc
     */
    #define ANSI 1
#endif
/**
 * @nodoc
 * Define Declaration macro for items which are not exported
 */
#define DECL_NONE
/**
 * @nodoc
 * Define the precision of modeller data 
 */
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 */
#ifdef __cplusplus

#ifdef THREAD_SAFE_ACIS

    /**
     * distance criterion
     */
    extern DECL_BASE safe_floating_type<double> SPAresabs;
    /**
     * relative criterion: the smallest value 
     * which can be compared meaningfully with 1
     */
    extern DECL_BASE safe_floating_type<double> SPAresnor;
    /**
     * default resolution of curve fitting
     */
    extern DECL_BASE safe_floating_type<double> SPAresfit;
    /**
     * machine precision
     */
    extern DECL_BASE safe_floating_type<double> SPAresmch;
#else
    /**
     * distance criterion
     */
    extern DECL_BASE double SPAresabs;
    /**
     * relative criterion: the smallest value 
     * which can be compared meaningfully with 1
     */
    extern DECL_BASE double SPAresnor;
    /**
     * default resolution of curve fitting
     */
    extern DECL_BASE double SPAresfit;
    /**
     * machine precision
     */
    extern DECL_BASE double SPAresmch;

#endif

#endif

/**
 * @nodoc
 */
extern DECL_BASE void *const NULL_REF;
#include "static_types.hxx"
/* Define M_PI and M_PI_2 for Pcs and the MAC */
#if defined( _MSC_VER ) || (defined( mac ) && !defined (MacX))
    #ifndef M_PI
       /**
        * Value of PI 
        */
        #define M_PI 3.14159265358979323846
    #endif

    #ifndef M_PI_2
       /**
        * Value of half PI
        */
        #define M_PI_2 (M_PI/2.0)
    #endif
#endif


/* cross-platform versions of 8-byte integers */

#ifdef _MSC_VER

typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else  // linux, mac, aix

/* inttypes.h defines int64_t and uint64_t */
#include <inttypes.h>

#endif


/* Converts several system call functions to their windows version. */
#include "acisio.h"

#ifdef USING_ACIS_MATH
    #include "acismath.h"
#endif

#if defined( _WINDOWS ) || defined ( mac )
    /**
     * @nodoc
     */
    #define deallocate_polygon deallocate_polygon_acis
#endif

/* These macros are used at the beginning of every function
// and are defined to whatever we need at the moment.
*/
#if defined(ENABLE_ACIS_TIMERS) && defined(__cplusplus)
    #include "timsystem.hxx"
#else
    /**
     * @nodoc
     */
    #define ACIS_FUNCTION(name)
#endif

/* MS header rpcndr.h redefines "small" to "char" causing all kinds of issues */
#ifdef __RPCNDR_H__
#undef small
#endif

/** @} */
#endif


