/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// base/baseutil/b_strutl.hxx
// an architecture indepndent string
// compare definition
//
#ifndef BAS_STR_UTILS_HXX
#define BAS_STRL_UTILS_HXX

#include <string.h>
#ifdef aix
#include <strings.h>
#endif

#if defined( _MSC_VER ) || (defined( mac ) && !defined( MacX ))
# define STRICMP  _stricmp
# define STRNICMP _strnicmp
#else
# define STRICMP strcasecmp
# define STRNICMP strncasecmp
#endif
#ifdef mac
#include "bmac_utl.hxx"
#endif

#endif

