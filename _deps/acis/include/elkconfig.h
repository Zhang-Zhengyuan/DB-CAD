/* ORIGINAL: 3dt2.1/scheme/elk/elkconfg.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: elkconfig.h,v 1.4 2002/06/24 21:21:58 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//	  Platform specific configuration
//
//----------------------------------------------------------------------
*/
/* This file was produced by the Makefile in this directory.
 * If you want to change the value of a constant, edit ../config/system
 * or ../config/site and run make again.
 *
 * The above was done with config/system and config/site edited for hp700
 * sun4 and NT.  The result was merged using three big #ifdef's.  It is 
 * obviously a problem to define more than one of sun4, HP700 and NT. 
 * It is probably best to maintain this file manually from here on out.
 */

#ifndef CONFIG_H
#define CONFIG_H

/*
// knt 10 Aug 2006. Increased default heap size from (sizeof(void*) * 512).
// Some parts that have profuse amounts of checker output need more heap.
// Try: StitchSCM\stitch\ny_e_f-INTEROPiges.scm
*/

/* Jeff - Jan2015.  Hitting the limit again. Doubling the heap size. */

#define                HEAP_SIZE         ( 4 * sizeof(void*) * 1024 )


/*======================================================================*/
#if defined(ultrix) || defined(osf1)

#define DONT_INIT "scheme"
#define          WAITPID
#define            WAIT3
#undef            WAIT4
#define           MKTEMP
#define           TMPNAM
#define          TEMPNAM
#define           GETCWD
#define            GETWD
#define           RENAME
#define            UNAME
#define      GETHOSTNAME
#define     GETTIMEOFDAY
#define            FTIME
#define            VFORK
#define          VPRINTF
#define           DIRENT
#define           RANDOM
#define INCLUDE_UNISTD_H
#undef SYSCONF_OPEN_MAX
#define    GETDTABLESIZE
#define PATHCONF_PATH_MAX
#define      GETPAGESIZE
#undef SYSCONF_PAGESIZE
#define      BSD_SIGNALS
#undef    POSIX_SIGNALS
#ifdef osf1
#define     ALIGN_8BYTE
#else
#undef      ALIGN_8BYTE
#endif
#undef             COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef            MACH_O
#undef           CONVEX_AOUT
#undef             HP9K
#define   HPSHLIB
#define       DEBUG_DUMP
#undef     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef           USE_LD
#undef          USE_RLD
#undef          USE_SHL
#undef       USE_DLOPEN
#  define              LOAD_LIBRARIES    ""
#undef       CACHECTL_H
#define       FIONREAD_H        <sys/ioctl.h>
#  define         ATEXIT
#undef  SYMS_BEGIN_WITH   
#undef         CAN_DUMP
#  define              SEG_SIZ           SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              COFF_PAGESIZE     
#  undef  FCHMOD_BROKEN
#define           TERMIO
#define        FLUSH_BSD
#undef     FLUSH_FPURGE
#undef      FLUSH_LINUX
#undef  FLUSH_TIOCFLUSH
#define     FLUSH_TCFLSH
#if defined( osf1 )
#define             POINTER_CONSTANT_HIGH_BITS 0x140000000  
#endif
#if defined( ultrix )
#define             POINTER_CONSTANT_HIGH_BITS 0x10000000  
#endif
#undef  GENERATIONAL_GC
#define         HAS_MPROTECT
#undef    MPROTECT_MMAP
#undef     SIGSEGV_SIGINFO
#define  SIGSEGV_SIGCONTEXT
#undef        SIGSEGV_ARG4
#undef         SIGSEGV_AIX
#undef        SIGSEGV_HPUX

#define                AOUT_H            <a.out.h>
#define                SCM_DIR           "/usr/elk/runtime/scm"
#define                OBJ_DIR           "/usr/elk/runtime/obj"

#define                FIND_AOUT         defined(USE_LD) || defined(CAN_DUMP)\
				      || defined(INIT_OBJECTS)
#undef       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#undef         ANSI_CPP
#define                SYSTEMTYPE        "dec5100-ultrix4.2-cc"
#define                GETGROUPS_TYPE    int
#define                LD_NAME           "ld"
#define                LDFLAGS_SHARED    ""
#define          UTIME_H

#endif /* ultrix || osf1 */

/*======================================================================*/
#if defined(aix)

#define DONT_INIT "scheme"
#define          WAITPID
#define            WAIT3
#undef            WAIT4
#define           MKTEMP
#define           TMPNAM
#define          TEMPNAM
#define           GETCWD
#define            GETWD
#define           RENAME
#define            UNAME
#define      GETHOSTNAME
#define     GETTIMEOFDAY
#define            FTIME
#undef            VFORK
#define          VPRINTF
#define           DIRENT
#define           RANDOM
#define INCLUDE_UNISTD_H
#undef SYSCONF_OPEN_MAX
#define    GETDTABLESIZE
#define PATHCONF_PATH_MAX
#define      GETPAGESIZE
#undef SYSCONF_PAGESIZE
#define      BSD_SIGNALS
#undef    POSIX_SIGNALS
#undef      ALIGN_8BYTE
#undef             COFF
#undef            ECOFF
#define            XCOFF
#undef              ELF
#undef            MACH_O
#undef           CONVEX_AOUT
#undef             HP9K
#define   HPSHLIB
#define       DEBUG_DUMP
#define     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef           USE_LD
#undef          USE_RLD
#undef          USE_SHL
#undef       USE_DLOPEN
#  define              LOAD_LIBRARIES    ""
#define       CACHECTL_H        unused
#define       FIONREAD_H        <sys/ioctl.h>
#  undef         ATEXIT
#define  SYMS_BEGIN_WITH   '.'
#undef         CAN_DUMP
#  define              SEG_SIZ           SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              COFF_PAGESIZE     4096
#  undef  FCHMOD_BROKEN
#define           TERMIO
#define        FLUSH_BSD
#undef     FLUSH_FPURGE
#undef      FLUSH_LINUX
#undef  FLUSH_TIOCFLUSH
#define     FLUSH_TCFLSH
#define             POINTER_CONSTANT_HIGH_BITS  0x20000000
#undef  GENERATIONAL_GC
#undef         HAS_MPROTECT
#undef    MPROTECT_MMAP
#undef     SIGSEGV_SIGINFO
#undef  SIGSEGV_SIGCONTEXT
#undef        SIGSEGV_ARG4
#define         SIGSEGV_AIX
#undef        SIGSEGV_HPUX

#define                AOUT_H            <a.out.h>
#define                SCM_DIR           "/usr/elk/runtime/scm"
#define                OBJ_DIR           "/usr/elk/runtime/obj"

#define                FIND_AOUT         defined(USE_LD) || defined(CAN_DUMP)\
				      || defined(INIT_OBJECTS)
#undef       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#define         ANSI_CPP
#define                SYSTEMTYPE        "rs6000-aix3.2-cc"
#define                GETGROUPS_TYPE    gid_t
#define                LD_NAME           "ld"
#define                LDFLAGS_SHARED    ""
#define          UTIME_H

#endif /* aix */

/*======================================================================*/
#if defined(sun4) || defined(SPARC) || defined(solaris)
#define            VFORK
#define          VPRINTF
#define           DIRENT
#define           RANDOM
#ifdef solaris
#undef           RANDOM
#endif
#define INCLUDE_UNISTD_H
#define          SYSCONF
#define    GETDTABLESIZE
#ifndef solaris
#define      BSD_SIGNALS
#undef    POSIX_SIGNALS
#else
#undef      BSD_SIGNALS
#define    POSIX_SIGNALS
#endif
#define      ALIGN_8BYTE
#undef             COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef            MACH_O
#define     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef          USE_LD
#undef          USE_RLD
#undef          USE_SHL
#define LOAD_LIBRARIES ""
#define       CACHECTL_H        unused
#  undef         ATEXIT
#define  SYMS_BEGIN_WITH   '_'
#undef         CAN_DUMP
#  define              SEGMENT_SIZE      SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              TEXT_START        
#  define              DATA_START        
#  define              COFF_PAGESIZE     
#  undef  FCHMOD_BROKEN
#define           TERMIO
#define                TIME_H            <sys/time.h>
#undef  GENERATIONAL_GC
#undef     INIT_OBJECTS
#define DONT_INIT "scheme"
#define SCM_DIR ""
#define LIB_DIR ""


#undef       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#undef         ANSI_CPP

#endif /* SUN4 */
/*======================================================================*/

#ifdef hp700
#define            VFORK
#define          VPRINTF
#define           DIRENT
#undef           RANDOM
#define INCLUDE_UNISTD_H
#define          SYSCONF
#undef    GETDTABLESIZE
#define      BSD_SIGNALS
#undef    POSIX_SIGNALS
#define      ALIGN_8BYTE
#undef             COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef            MACH_O
#define     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef          USE_LD
#undef          USE_RLD
#undef          USE_SHL
#define LOAD_LIBRARIES ""
#define       CACHECTL_H        unused
#  define         ATEXIT
#undef  SYMS_BEGIN_WITH   
#undef         CAN_DUMP
#  define              SEGMENT_SIZE      SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              TEXT_START        
#  define              DATA_START        
#  define              COFF_PAGESIZE     
#  undef  FCHMOD_BROKEN
#define           TERMIO
#define                TIME_H            <sys/time.h>
#undef  GENERATIONAL_GC
#undef     INIT_OBJECTS
#define DONT_INIT "scheme"
#define SCM_DIR ""
#define LIB_DIR ""


#undef       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#undef         ANSI_CPP

#endif /* HP700 */
/*======================================================================*/

#if defined(NT) || defined(linux)
#undef            VFORK
#define          VPRINTF
#define           DIRENT
#undef           RANDOM
#undef INCLUDE_UNISTD_H
#undef          SYSCONF
#undef    GETDTABLESIZE
#undef      BSD_SIGNALS
#undef    POSIX_SIGNALS
#define      ALIGN_8BYTE
#define             COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef            MACH_O
#define     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef           USE_LD
#undef          USE_RLD
#undef          USE_SHL
#  define              LOAD_LIBRARIES    ""
#define       CACHECTL_H        unused
#undef         ATEXIT
#define  SYMS_BEGIN_WITH   '_'
#undef         CAN_DUMP
#  define              SEGMENT_SIZE      SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              TEXT_START        
#  define              DATA_START        
#  define              COFF_PAGESIZE     
#  undef  FCHMOD_BROKEN
#undef           TERMIO
#define TIME_H            <time.h>
#define MAX_STACK_SIZE (1*1024*1024) 
#undef  GENERATIONAL_GC

#undef     INIT_OBJECTS
#define DONT_INIT "scheme"
#define                SCM_DIR           "/scheme/scm"
#define                LIB_DIR           "/scheme/runtime/obj"


#define       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#define         ANSI_CPP
#endif /* NT */

/*======================================================================*/
#ifdef sgi

#undef            VFORK
#define          VPRINTF
#define           DIRENT
#define           RANDOM
#define INCLUDE_UNISTD_H
#define          SYSCONF
#define    GETDTABLESIZE
#define      BSD_SIGNALS
#undef    POSIX_SIGNALS
#define      ALIGN_8BYTE
#undef             COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef            MACH_O
#undef     USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef           USE_LD
#undef          USE_RLD
#undef          USE_SHL
#define LOAD_LIBRARIES ""
#define       CACHECTL_H        <sys/cachectl.h>
#  undef         ATEXIT
#undef  SYMS_BEGIN_WITH   
#undef         CAN_DUMP
#  define              SEGMENT_SIZE      SEGSIZ
#  define              FILE_TEXT_START   sizeof(struct exec)
#  define              MEM_TEXT_START    (PAGSIZ+sizeof(struct exec))
#  define              TEXT_LENGTH_ADJ   sizeof(struct exec)
#  define              TEXT_START        0x400000
#  define              DATA_START        0x800000
#  define              COFF_PAGESIZE     
#  undef  FCHMOD_BROKEN
#define           TERMIO
#define TIME_H            <sys/time.h>
#undef  GENERATIONAL_GC

#undef     INIT_OBJECTS
#define DONT_INIT "scheme"
#define SCM_DIR ""
#define LIB_DIR ""


#undef       WANT_PROTOTYPES
#undef         NO_PROTOTYPES
#define         ANSI_CPP

#endif /* sgi */

/*======================================================================*/


#if defined( mac ) || defined( OS_Darwin )
#undef            VFORK
#define         VPRINTF
#define          DIRENT
#undef           RANDOM
#undef INCLUDE_UNISTD_H
#undef          SYSCONF
#undef    GETDTABLESIZE
#undef      BSD_SIGNALS
#undef    POSIX_SIGNALS
#define     ALIGN_8BYTE
#define            COFF
#undef            ECOFF
#undef            XCOFF
#undef              ELF
#undef           MACH_O
#define       USE_XFLAG
#undef     CAN_LOAD_OBJ
#undef           USE_LD
#undef          USE_RLD
#undef          USE_SHL
#define  LOAD_LIBRARIES ""
#define      CACHECTL_H unused
#undef           ATEXIT
#define SYMS_BEGIN_WITH '_'
#undef         CAN_DUMP
#define    SEGMENT_SIZE SEGSIZ
#define FILE_TEXT_START sizeof(struct exec)
#define  MEM_TEXT_START (PAGSIZ+sizeof(struct exec))
#define TEXT_LENGTH_ADJ sizeof(struct exec)
#define      TEXT_START        
#define      DATA_START        
#define   COFF_PAGESIZE     
#undef    FCHMOD_BROKEN
#undef           TERMIO
#define          TIME_H <time.h>
#define  MAX_STACK_SIZE (1*1024*1024)
#undef  GENERATIONAL_GC

#undef     INIT_OBJECTS
#define       DONT_INIT "scheme"
#define         SCM_DIR "scheme:scm"
#define         LIB_DIR "scheme:runtime:obj"



#define WANT_PROTOTYPES
#undef    NO_PROTOTYPES
#define        ANSI_CPP
#endif /* mac */

/*======================================================================*/
#endif
