// COPYRIGHT DASSAULT SYSTEMES 1989

#ifndef DECL_PHLV5

#if defined( __SpaPhlV5 ) ||  defined( __SpaACIS ) || defined( __SPAAcisDs )
#define BUILDING_LOCAL_FILE
#endif

#include "importexport.h"
#ifdef IMPORT_EXPORT_SYMBOLS
# ifdef BUILDING_LOCAL_FILE
#  define DECL_PHLV5 EXPORT_SYMBOL
# else
#  define DECL_PHLV5 IMPORT_SYMBOL
# endif
#else
# define DECL_PHLV5
#endif

/* force link in VC++ */

#if !defined( CONCAT )
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

#ifndef SPA_NO_AUTO_LINK
# ifndef BUILDING_LOCAL_FILE
#  if defined(_MSC_VER)
#   if defined( SPA_INTERNAL_BUILD ) || defined( NOBIGLIB )
#    define spa_lib_name "SpaPhlV5"
#   else
#    define spa_lib_name "SpaPhlV5"
#   endif
#   if defined( _DEBUG ) && !defined( SPA_INTERNAL_BUILD )
#    pragma comment( lib, CONCAT( spa_lib_name, "d.lib" ))
#   else
#    pragma comment( lib, CONCAT( spa_lib_name, ".lib" ))
#   endif
#  endif
# endif
#endif


#undef BUILDING_LOCAL_FILE
#undef spa_lib_name

#endif /* DECL_PHLV5 */
