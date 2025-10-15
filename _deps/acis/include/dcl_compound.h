/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
* ACIS Compound
*/
#if !defined(DCL_COMPOUND_HXX)
#define DCL_COMPOUND_HXX
/**
 * @file dcl_compound.h 
 * \defgroup ACISCOMPOUND SpaAcisCompound
 * \brief Custom ACIS extensions requested by CLI that do not exist in released ACIS.
 */
#include "importexport.h"

#if defined( __SpaAcisCompound ) 
#define BUILDING_LOCAL_FILE
#endif

#ifdef IMPORT_EXPORT_SYMBOLS
# ifdef BUILDING_LOCAL_FILE
#  define DECL_COMPOUND EXPORT_SYMBOL
# else
#  define DECL_COMPOUND IMPORT_SYMBOL
# endif
#else
# define DECL_COMPOUND
#endif


/* force link in VC++ */

#if !defined( CONCAT )
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

#ifndef SPA_NO_AUTO_LINK
#   if defined( SPA_INTERNAL_BUILD ) || defined( NOBIGLIB )
#    define spa_lib_name "SpaAcisCompound"
#   else
#    define spa_lib_name "SpaAcisCompound"
#   endif
#   if defined( _DEBUG ) && !defined( SPA_INTERNAL_BUILD )
#    pragma comment( lib, CONCAT( spa_lib_name, "d.lib" ) )
#   else
#    pragma comment( lib, CONCAT( spa_lib_name, ".lib" ) )
#   endif
#endif

#undef BUILDING_LOCAL_FILE
#undef spa_lib_name

#endif	/* DCL_COMPOUND_HXX */
