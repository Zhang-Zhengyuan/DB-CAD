/* ORIGINAL: acis2.1/ccat.hxx */
// $Id: ccat.hxx,v 1.11 2002/01/30 17:29:05 skenny Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/



//
// Define the concatenation method for pc environment.
//

#if !defined( CCAT_HEADER )
#define CCAT_HEADER

// We use generic.h to define name2, which is needed to build a unique name
// for each type of list.  Generic.h has caused problems in the distant past
// but may be better in the newest set of compilers.  If so, we can manually
// expand the list types needed. by quickly cut/paste/editing this file

// STI jmb:  Added mac and name3
// STL pcp 23-May-1999 Use __GNUC__ instead of linux here to avoid problems
// when using gcc on other platforms 
#if defined( _MSC_VER ) || defined(mac) || defined (__GNUC__) || defined (__KCC)

#define glue(x,y) x ## y
#define Glue(x,y) glue(x,y)

#if !defined ( mac ) || defined (MacX)  // This is defined in acis.hxx for mac.  We will need to clean up.
#define name2(x,y) glue(x,y)
#endif

// STI jmb: name3
#define Glue2(x,y,z)    x ## y ## z
#define glue2(x,y,z)    Glue2(x,y,z)

#define name3(x,y,z)    glue2(x,y,z)

// STI jmb: end
#else

#include <generic.h>

#endif


#endif
