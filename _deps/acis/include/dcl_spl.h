/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "dcl_kern.h"

/*
#ifndef DECL_SPLINE

#if defined( __kernel ) || defined( __SpaACIS ) || defined( __SPAAcisDs )
#define BUILDING_LOCAL_FILE
#endif

#ifdef _MSC_VER
# ifdef BUILDING_LOCAL_FILE
#  define DECL_SPLINE __declspec(dllexport)
# else
#  define DECL_SPLINE __declspec(dllimport)
# endif
#else
# define DECL_SPLINE
#endif

#ifndef SPA_NO_AUTO_LINK
# ifndef BUILDING_LOCAL_FILE
#  if defined(_MSC_VER)
#   if defined( SPA_INTERNAL_BUILD ) || defined( NOBIGLIB )
#    pragma comment( lib, "kernel.lib" )
#   else
#    ifdef _DEBUG
#     pragma comment( lib, "SpaACISd.lib" )
#    else
#     pragma comment( lib, "SpaACIS.lib" )
#    endif
#   endif
#  endif
# endif
#endif

#undef BUILDING_LOCAL_FILE

#endif
*/
