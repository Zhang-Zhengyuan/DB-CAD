/* ORIGINAL: acis2.1/kernutil/errorsys/signal.hxx */
/* $Id: signal.hxx,v 1.9 2002/01/30 17:23:25 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define the type of the signal-catching functions supplied to
// signal and ssignal. These are nearly always functions taking a
// single int argument (being the signal number), and returning
// either void or an int (being a logical value to select whether
// or not to try to continue after the exception condition). For
// a hardware signal, a void return is the more common, for a
// software signal the choice is about 50-50.

// There is very little consistency between different implementations
// in respect of how these function types are declared, though SIG_PF
// is a fairly common typedef for a hardware signal-catcher, and
// SIG_PFI for the software catcher, if this is different.

#if !defined( SIGNAL_TYPE )
#ifdef __cplusplus
extern "C" {
#endif
#include <signal.h>

// STIPORT TAC this is where the macro LS_STAMP is that allows us
// to distinguish between Irix 5.2 and 5.3
#ifdef sgi
#include <stamp.h>
#endif

// HP 9000 series 700. Should this apply to other HPUX machines
// as well ?

#ifdef hp700
typedef void (*SIG_TYP)(int);
#define SIGNAL_TYPE SIG_TYP
typedef int (*SSIG_TYP)(int);
#define SSIGNAL_TYPE SSIG_TYP
#endif

#if defined(linux) || defined(__linux__)
#define SIGNAL_TYPE __sighandler_t
#define SSIGNAL_TYPE __sighandler_t
#endif


// Silicon Graphics IRIS, etc.

// STIPORT TAC sgi4 added
#if defined( sgi ) || defined( sgi4 )
#define SIGNAL_TYPE SIG_PF
#ifndef sgi // used to read sgi5, now it's sgi4 and sgi5
typedef int (*SSIG_TYP)(int);
#else
// STIPORT TAC The prototype for signal() changed between IRIX 5.2 and 5.3
// STIPORT shd - from stamp.h
#if (MS_STAMP > 3 ) || ((MS_STAMP == 3) && (LS_STAMP >= 19))
typedef int (*SSIG_TYP)(int);
#else
typedef int (*SSIG_TYP)(...);
#endif
#endif
#define SSIGNAL_TYPE SSIG_TYP
#endif

// Suns of various types

#ifdef sun
#define SIGNAL_TYPE SIG_PF
#ifdef solaris
typedef int (*SIG_PFI)( int );
#endif
#define SSIGNAL_TYPE SIG_PFI
#endif

// AIX

#ifdef aix
typedef void (*SIG_PF)( int );
#define SIGNAL_TYPE SIG_PF
#define SSIGNAL_TYPE SIG_PF
#endif

// MS Visual C++

#if defined ( _MSC_VER ) 
typedef void (__cdecl *SIG_PFV)(int);
#define SIGNAL_TYPE SIG_PFV
#define SSIGNAL_TYPE SIG_PFV
#define SIGNAL_CLASS extern "C" _CRTIMP
#endif

// STIPORT TAC added osf1, which look just like ultrix
// osf1
// STIPORT TAC on osf1 OS versions prior to 3.0, SSIG_PF was
// the same as SIG_PF, which is how the man page still reads.
#ifdef osf1
typedef void (*SIG_PF)( int );
typedef int (*SSIG_PF)( int );
#define SIGNAL_TYPE SIG_PF
#define SSIGNAL_TYPE SSIG_PF
#endif

// STIPORT TAC macintosh
// Ye Olde Power Mac
#if defined(MacX) || defined(OS_Darwin)
#define SIGNAL_TYPE sig_t
#define SSIGNAL_TYPE sig_t
#elif defined (mac)
//#define SIGNAL_TYPE _Sigfun *
//#define SSIGNAL_TYPE _Sigfun *
// CW11 changed the headers to the Metrowerks standard library
#define SIGNAL_TYPE std::__signal_func_ptr
#define SSIGNAL_TYPE std::__signal_func_ptr
#endif

// Catchall which may work for some other systems.

#ifndef SIGNAL_TYPE
#define SIGNAL_TYPE SIG_PF
#ifdef SIG_PFI
#define SSIGNAL_TYPE SIG_PFI
#else
#define SSIGNAL_TYPE SIG_PF
#endif
#endif

// Most signal and ssignal functions are in their machine's C library,
// so they must be declared 'extern "C"', but there are one or two
// exceptions above.
 
#ifndef SIGNAL_CLASS
#define SIGNAL_CLASS extern "C"
#endif
#ifndef SSIGNAL_CLASS
#define SSIGNAL_CLASS extern "C"
#endif
 
// Now redeclare signal() and ssignal(). This serves two purposes:
// if they are already declared, it checks that we have the above
// right for each machine. If either (in particular ssignal) is not
// declared in the headers, as seems to occur on some Apollo and SGI
// machines, this furnishes the declaration.

// *** Do not comment out or conditionalise these declarations - if
// *** they lead to multiply-defined functions with "C" linkage,
// *** it is probably because the definitions of SIGNAL_TYPE and
// *** SSIGNAL_TYPE above for your machine are wrong. Correct those,
// *** and all should be well.


// STIPORT TAC macintosh MWCWG5 considers signal() declared, and considers
// this to be illegal overloading
#if !defined( mac ) && !defined(linux) && !defined(OS_Darwin)
SIGNAL_CLASS SIGNAL_TYPE signal( int, SIGNAL_TYPE ); 
#endif

// STIPORT TAC macintosh no such function
// STI porting 10-11-99: solaris considers ssignal() declared , and considers
// this to be overloading.
// STI-porting 10-14-99: added osf1, same reason as solaris.
#if !defined ( mac ) && !defined ( solaris ) && ! defined ( osf1 ) && !defined(linux) && !defined(OS_Darwin)
SSIGNAL_CLASS SSIGNAL_TYPE ssignal( int, SSIGNAL_TYPE );
#endif

#ifdef __cplusplus
}
#endif

#endif

