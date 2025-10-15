/* ORIGINAL: acis2.1/kerndata/attrib/at_macro.hxx */
/* $Id: at_macro.hxx,v 1.9 2002/01/30 17:29:05 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Macros for simplifying the definition of attributes. There are
// rather a lot of member functions required for any entity, but
// many of them are the same for all attributes (at the source level),
// and some of the others have elements in common. These macros are
// to help with this commonality.

// The definition macros depend upon the prior definition in the
// implementation file of two further macros, THIS() and PARENT(),
// which translate to the name of the attribute currently being defined
// and its immediate base attribute, respectively.

// The macros for attributes are almost identical to those for general
// entities, but are retained for backwards compatibility - attribute
// macros in fact came first.

#ifndef AT_MACRO_HXX
#define AT_MACRO_HXX

#if !defined( mac ) || defined (MacX)
#include <memory.h>
#endif

#include "entity.hxx"

// Declare the common utility routines for the attribute header file.
// This is just the same as for any other sort of entity, but we
// retain this macro for backwards compatibility.

#define ATTRIB_FUNCTIONS( name, lib ) \
	ENTITY_FUNCTIONS( name, lib )


// Define the same utility routines for the implementation file.
// Several of them have user-defined interludes, so we declare several
// macros to bracket the interludes. The first three macros must be
// used in the order that they are defined here. The remainder may
// appear in any order, except that "TERMINATE_DEF" must come last.

// Before these macros are called, the macros THIS() and PARENT()
// must be defined, with values equal to the name of the current
// entity and that of the entity from which it is immediately derived.
// The macro "xxxx_NAME", where "xxxx" is the current entity type,
// must also be defined, with value equal to a string containing the
// external identifier for the entity.

#define ATTCOPY_DEF( name ) \
	UTILITY_DEF(0) \
	ATTCOPY_FIXUP

#define ATTCOPY_DEF_ID( name, ext_id_id ) \
	UTILITY_DEF(ext_id_id) \
	ATTCOPY_FIXUP

#define ATTCOPY_FIXUP \
} \
\
void THIS()::fixup_copy( THIS() *rollback ) const { \
	PARENT()::fixup_copy( rollback );

// Here the application writer places any special action required
// to duplicate the attribute for rollback purposes. The attribute
// itself has been constructed, copied memberwise, and our base class
// fixed up suitably, but if this derived class defined subsidiary
// structure hanging off a simple pointer (a character string
// hanging off a char pointer, for example) it will need copying or
// otherwise processing here.


#define LOSE_DEF \
} \
\
void THIS()::lose() {

// Here the application writer adds any special action required to
// lose any dependent entities.


#define DTOR_DEF \
	PARENT()::lose(); \
} \
\
THIS()::~THIS()() { \
	/* Only act if this is not a derived class */ \
\
	check_destroy();

// Here the application writer places any special action required
// during attribute destruction - for example if there is a subsidiary
// data structure which itself needs to be deleted.


// "DEBUG_DEF" is defined in entity_macro.hxx

// At this point the application writer places code to write out
// useful information from the attribute (none if he does not want
// to).


// "SAVE_DEF" is defined in entity_macro.hxx

// Here the application writer inserts code to write out specific
// attribute information to the save file, and to insert any pointers
// into the list.


// "RESTORE_DEF" is defined in entity_macro.hxx

// Here the application writer inserts code to read back specific
// attribute information from the save file.


// "COPY_DEF" is defined in entity_macro.hxx

// Here the application writer inserts code to copy data items into
// the new object, using "list" to convert any pointers into indices.


// "SCAN_DEF" is defined in entity_macro.hxx

// Here the application writer inserts code to enter any pointers
// into the list.


// "FIX_POINTER_DEF" is defined in entity_macro.hxx

// Here the application writer inserts code to convert any pointers
// from array indices into array contents, where a negative index
// represents NULL. Special care must be taken with entries pointing
// to objects with use counts, for example, to ensure that the target
// is correctly updated, and that the pointer is not followed until
// it has been converted from the array index.

// "TERMINATE_DEF" is defined in entity_macro.hxx


// For convenience we also define a heading macro which combines
// the first three macros in the sequence, for the common case of
// attributes which do not need any special action in the destructor
// or in duplication (this is also partly for historical reasons).

#define ATTRIB_DEF( name ) \
ATTCOPY_DEF( name ) \
LOSE_DEF \
DTOR_DEF \
DEBUG_DEF

// **************************************************************

// Finally two macros for constructing application-developer master
// attributes. These are very stylised, and so can be constructed
// entirely by macro.


#define MASTER_ATTRIB_DECL( name, lib ) \
ENTITY_IS_PROTOTYPE ( name, lib ) \
class DECL_FUNCTION(lib) name: public ATTRIB { \
public: \
	name( ENTITY * = NULL ); \
\
	ATTRIB_FUNCTIONS( name, lib ) \
}; 

#define MASTER_ATTRIB_DEFN( description ) \
ATTRIB_DEF( description ) \
	/* No specific data to print out */ \
SAVE_DEF \
	/* No specific data to save */ \
RESTORE_DEF \
	/* No specific data to restore */ \
COPY_WITH_DEEP_COPY_DEF \
	/* STI ROLL  Deep copy capable */ \
	/* No specific data to copy */ \
SCAN_DEF \
	/* No specific pointer data */ \
FIX_POINTER_DEF \
	/* No specific pointer data */ \
TERMINATE_DEF \
\
THIS()::THIS()( ENTITY *owner ): PARENT()( owner ) { \
}

#endif
