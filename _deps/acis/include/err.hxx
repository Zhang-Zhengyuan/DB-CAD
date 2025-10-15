/* ORIGINAL: acis2.1/kernutil/d3_utl/err.hxx */
/* $Id: err.hxx,v 1.15 2002/03/06 17:24:14 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ERRH
#define ERRH

// This file contains macros for standardising error messages. It contains 
// four macros:
//
//	ERR_assert( test, string ) - if 'test' is TRUE, then a warning message 
//				is printed out.
//
//	ERR_warning( string ) - to be used where the algorithm is not affected, 
//           			but some peculiarity requires attention. 
//
//	ERR_fatal( string )   - to be used where continuation is impossible. 
//
//	ERR_test( string )    - to be used when the test code detects a fault. 
//
// In the standalone version, the string will be output to D3_cerr and also 
// output as a debug message at level 0. Both messages will be prefixed with 
// a standard error format, and by the file name.
// 
// In the acis version, ERR_warning should become a sys_warning and ERR_fatal 
// should become a sys_error (causing a longjump). ERR_test should never be 
// used in the acis version. 
//

#ifdef D3_DEBUG

#include "stream.hxx"
#include "msc.hxx"

#define ERR_assert_prefix "***   ERROR - ASSERTION   "

//extern void	allow_break();

#ifdef NT
#define ERR_assert( test, string ) 					\
    if ( test )								\
        {								\
	DEB_info( "\n" << ERR_assert_prefix << DEB_filename() << 	\
		  ": " << string << "   ***\n\n" );			\
	allow_break(Assert_break);					\
        }
#else
#define ERR_assert( test, string ) 					\
    if ( test )								\
        {								\
	D3_cerr << ERR_assert_prefix << DEB_filename() << ": " << 	\
						string << "   ***\n\n";	\
	DEB_out( 0, ERR_assert_prefix << DEB_filename() << ": " << 	\
					string << "   ***\n\n" );	\
	allow_break(Assert_break);					\
        }
#endif

#else

#define ERR_assert( test, string )
 
#endif


#define ERR_test_prefix "***   ERROR - TEST   "

#define ERR_suffix "   ***\n\n"

#if defined D3_DEBUG

#include "stream.hxx"
#include "deb.hxx"

#define ERR_warning_prefix "***   ERROR - WARNING   "

#define ERR_fatal_prefix "***   ERROR - FATAL   "

#ifdef NT
#define ERR_warning( string ) 						\
    {                                                               	\
    DEB_info( "\n" << ERR_warning_prefix << DEB_filename() << ": " << 	\
	      string << ERR_suffix );					\
    allow_break(Warning_break);						\
    }

#else
#define ERR_warning( string ) 						\
    {                                                              	\
    D3_cerr << ERR_warning_prefix << DEB_filename() << ": " <<	 	\
	string << ERR_suffix;						\
    DEB_out( 0, ERR_warning_prefix << DEB_filename() << ": " << 	\
	     string << ERR_suffix );					\
    allow_break(Warning_break);						\
    }
#endif

#ifndef NT
#define ERR_fatal( string ) 						\
	{								\
	D3_cerr << ERR_fatal_prefix << DEB_filename() << ": " << 	\
					string << ERR_suffix;		\
	DEB_out( 0, ERR_fatal_prefix << DEB_filename() << ": " << 	\
					string << ERR_suffix );		\
	allow_break(Fatal_break);					\
	}

#else
#define ERR_fatal( string ) 						\
	{								\
	DEB_info( "\n" << ERR_fatal_prefix << DEB_filename() << ": " << \
						string << ERR_suffix );	\
	allow_break(Fatal_break);					\
	}
#endif

#ifndef NT
#define ERR_test( string ) 						\
	{								\
	D3_cerr << ERR_test_prefix << DEB_filename() << ": " << 	\
					string << ERR_suffix;		\
	DEB_out( 0, ERR_test_prefix << DEB_filename() << ": " << 	\
					string << ERR_suffix );		\
	}
#else
#define ERR_test( string ) 						\
	{								\
	DEB_info( "\n" << ERR_test_prefix << DEB_filename() << ": " << 	\
					string << ERR_suffix );		\
	}
#endif


#else

#define ERR_warning( string )

#define ERR_fatal( string )

#define ERR_test( string )						\
        {								\
        char *filename = __FILE__;					\
	int last_slash = -1;						\
        for ( int index = 0; filename[ index ] != 0; index++ )	      	\
	    if ( filename[ index ] == '/' || filename[ index ]== '\\' )	\
	        last_slash = index;					\
        filename = filename + last_slash + 1;				\
	printf( ERR_test_prefix "Test has failed at line %d of file %s"	\
		ERR_suffix, __LINE__, filename );			\
	}
#endif

#define ERR_NOT_CROSSING_LINES	"Local intersection structure misinterpretation"
#define ERR_NOT_CONIC_CENTRE	"Start point is not at conic centre"
#define ERR_ROOT_TYPE		"Cannot classify root entry/leave type"
#define ERR_CSI_SUBDIVISION	"CSI subdivision has been aborted"
#define ERR_CCI_SUBDIVISION	"CCI subdivision has been aborted"
#define ERR_CSI_DISTANCE	"Cannot find distance from curve to surface"
#define ERR_SSI_SUBDIVISION	"SSI subdivision has been aborted"
#define ERR_CURVE_NOT_BSPLINE	"Curve supplied to CSI is not B-spline"
#define ERR_SURFACE_NOT_BSPLINE	"Surface supplied to CSI is not B-spline"
#define ERR_CSI_ABORTED		"CSI error, may have missed intersections"
#define ERR_CCI_ABORTED		"CCI error, may have missed intersections"
#define ERR_EXPLORE_BOUNDARY	"Intersection curve not terminated properly"
#define ERR_EXPLORE_FAILURE	"Intersection curve cannot be marched"
#define ERR_NULL_CURVE		"Intersection curve has less than two points"
#define ERR_SURFACE_SINGULARITY	"Singular point on surface"
#define ERR_SURFACES_NOT_PARA	"Neither surface supplied to SSI is parametric"
#define ERR_STEP_VERIFICATION	"Unable to verify marched step during SSI"
#define ERR_INTCURVE_EVAL	"Failure to evaluate point on intcurve"
#define ERR_VBL_BOUNDARY \
                        "Boundary surface has singularity and cannot be blended"
#define ERR_VBL_SURFACE		"Blend surface has singularity at point"
#define ERR_SVEC_BAD_SINGULARITY \
                           "Untreatable surface singularity encountered at SVEC"
#define ERR_OFFSET_NOT_VALID \
                           "SSI Surface Offset too big for local surface curvature"
#define ERR_OFFSETS_TANGENT \
                           "SSI Offset Surface intersection is tangent"

#endif
