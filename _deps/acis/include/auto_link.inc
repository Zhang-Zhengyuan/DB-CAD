/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( SPACONCAT )
#define SPACONCAT2(a,b) a ## b
#define SPACONCAT(a,b) SPACONCAT2(a,b)
#endif

/* force link in VC++ */

#ifndef SPA_NO_AUTO_LINK
# ifndef BUILDING_LOCAL_FILE
#  if defined(_MSC_VER)
#   if defined( SPA_INTERNAL_BUILD ) || defined( NOBIGLIB )
#    pragma comment( lib, SPACONCAT( SMALL_LIB_NAME, ".lib" ) )
#   else
#    ifdef _DEBUG
#     pragma comment( lib, SPACONCAT( BIG_LIB_NAME, "d.lib" ) )
#    else
#     pragma comment( lib, SPACONCAT( BIG_LIB_NAME, ".lib" ) )
#    endif
#   endif
#  endif
# endif
#endif
