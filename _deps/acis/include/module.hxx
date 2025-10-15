/* ORIGINAL: acis2.1/kernutil/debug/module.hxx */
/* $Id: module.hxx,v 1.5 2001/03/01 18:09:22 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for module-based debugging. First there is a
// module-definition class which records an integer value, which
// can be used as a debugging level indicator for the relevant
// module. This has a constructor which links it into a global chain
// which can then be inspected by the debug command, for reporting
// or changing the level.
//
// Then we have a selection of macros to make coding easier.

#ifndef MODULE_DEBUG_HXX
#define MODULE_DEBUG_HXX

#include "base.hxx" // to make sure we have ACIS_OBJECT base class

// STIPORT TAC for name2 - lifted from 1.5.1 for sgi4 (IRIX 4.0.5)
#if defined( sgi4 ) || defined( vms )
#include <generic.h>
#endif
 
// Get the useful debug routines. Not needed here, but used almost
// invariably whenever this header is used.

#include "dcl_base.h"
#include "debugmsc.hxx"

class DECL_BASE module_debug : public ACIS_OBJECT {
	module_debug *next_ptr;
	const char *module_name;
public:
	unsigned int debug_level;

	module_debug( char const *name );
	~module_debug();

	module_debug *next() { return next_ptr; }
	char const *name() { return module_name; }
};

// The macros which follow depend upon the user's having
// defined the module name, by
//			#define MODULE() <name>

// Jeff - Define USE_DEBUG_BREAKPOINTS for other targets.
//        This symbol is also used for visual breakpoints in breakpint.hxx.
#if defined(SPA_DEBUG) || defined(INTERNAL_DEBUG_CHECKS) || defined(USE_DEBUG_BREAKPOINTS)

// We need to combine the module name with some other stuff to make
// a single global identifier. This is not particularly easy, and many
// preprocessors need to be tricked.

// STI dgh -  can't use STDC on HPUX 9.0.5 as it breaks other files
//			  but do need to used this method for concatenation;
//			  for the older operating system it doesn't matter
//			  so lets always do it
#if defined( __STDC__ ) || defined (hp700) || defined (__ICL) || defined (mac)
// Any ANSI preprocessor ought to be able to do this. The only oddity
// is that the one on Solaris 2, at least, always puts a space after
// the expansion of a macro with arguments, even if it is concatenated
// with something else, so we have to put in an extra indirection, to
// get the macro expanded before it is concatenated.

// The concatenation macros are also defined in data/entity.hxx, so we
// conditionalise their definition to avoid conflict.

#if !defined( CONCAT )
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

#define HEADER_IDENTIFIER CONCAT(MODULE(),_module_header)
#define DECL_MOD(lib) CONCAT(DECL_,lib)
#else
// Just concatenating the module name macro with the remaining text
// often works. Some specific machines may need the use of the
// "generic.hxx" macro "name2", but we avoid that if possible, as it
// tends to cause knock-on effects, and not all machines have one.

#define HEADER_IDENTIFIER MODULE()_module_header
#define decl_mod() DECL_
#define DECL_MOD(lib) decl_mod()lib
#endif


// STIPORT TAC for sgi 4.0.5, from acis 1.5.1
#if defined( sgi4 ) || defined( vms )
#define HEADER_IDENTIFIER name2(MODULE(),_module_header)
#define DECL_MOD(lib) name2(DECL_,lib)
#endif
// STIPORT TAC ends here

#define MODULE_REF(lib) \
extern DECL_MOD(lib) module_debug HEADER_IDENTIFIER
#define MODULE_DEF(name, lib) \
DECL_MOD(lib) module_debug HEADER_IDENTIFIER(name)

#define DEBUG_LEVEL( level ) \
	if (HEADER_IDENTIFIER.debug_level >= level)

#else

#define MODULE_REF( lib )
#define MODULE_DEF( name, lib )

#define DEBUG_LEVEL( level ) \
	if (0)

#endif

// Define debug severities - unsigned integers

#define DEBUG_OFF 0
#define DEBUG_CALLS 10
#define DEBUG_FLOW 20
#define DEBUG_DETAILS 30
#define DEBUG_ALL 40

// Declare debug file specification

extern DECL_BASE FILE *debug_file_ptr;

// Finally global access routines

DECL_BASE module_debug *get_module_list();
DECL_BASE module_debug *find_module( char const * );
DECL_BASE int get_debug_level( char const * );
DECL_BASE char const *get_debug_level_name( unsigned );
DECL_BASE char const * const *get_all_debug_level_names();

#endif

