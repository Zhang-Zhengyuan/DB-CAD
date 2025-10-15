/* ORIGINAL: acis2.1/kerndata/data/usecount.hxx */
// $Id: usecount.hxx,v 1.11 2001/03/16 03:55:14 jsloan Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// -----------------------------------------------------------------------------
// Additional macros to be used in ENTITYs with use counts and the ENTITYs
// that refer to them.  These are needed because making save/restore of the
// history of use counts work is non-trivial.
// -----------------------------------------------------------------------------

#ifndef USE_COUNT_MACROS
#define USE_COUNT_MACROS

// This file uses some of the macros in entity.hxx and so must be
// included after it.  We include it here to be sure.

#include "dcl_kern.h"
#include "entity.hxx"
#include "savres_small.hxx"
//extern DECL_KERN logical restoring_history;

// Use this inside a class declaration to declare that this ENTITY
// refers to one or more ENTITYs with use counts.

#define USE_COUNT_REFERENCE_DECL \
ROLL_NOTIFY_DECL

// Use this inside a USE_COUNT_REFERENCE_NOTIFY_DEF to define the use
// count related portion of the roll_notify method for an ENTITY that
// refers to an ENTITY with a use count.  Parameters indicate ...
//
//	USE_COUNTED how to get a pointer to the use counted ENTITY from
//	an ENTITY of THIS() type, 
//
//	ADD_FUNC the use counted ENTITY's function to increment the use
//	count
//
//	REMOVE_FUNC the use counted ENTITY's function to decrement the
//	use count

// This is separated from USE_COUNT_REFERENCE_NOTIFY_DEF for the
// convienience of classes refering to more than one ENTITY with a use
// count or ENTITYs with other reasons for having a roll_notify
// method.

#define USE_COUNT_REFERENCE_NOTIFY_INTERNAL(USE_COUNTED,ADD_FUNC,REMOVE_FUNC) \
	switch(type) { \
	case DELETE_BULLETIN: \
		if (USE_COUNTED != NULL) USE_COUNTED->ADD_FUNC; \
		break; \
	case CREATE_BULLETIN: \
		if (USE_COUNTED != NULL) USE_COUNTED->REMOVE_FUNC; \
		break; \
	case CHANGE_BULLETIN: \
	{ \
		THIS()* other_ent = (THIS()*) other; \
		if (USE_COUNTED != other_ent->USE_COUNTED) { \
			if (other_ent->USE_COUNTED != NULL) other_ent->USE_COUNTED->ADD_FUNC; \
			if (USE_COUNTED != NULL) USE_COUNTED->REMOVE_FUNC; \
		} \
	} \
		break; \
	case NO_BULLETIN: \
		break; \
	}

// Use this in a class definition for an ENTITY that refers to an
// ENTITY with a use count.  Parameters indicate ...  
//
//	USE_COUNTED how to get a pointer to the use counted ENTITY from
//	an ENTITY of THIS() type, 
//
//	ADD_FUNC the use counted ENTITY's function to increment the use
//	count
//
//	REMOVE_FUNC the use counted ENTITY's function to decrement the
//	use count

#define USE_COUNT_REFERENCE_NOTIFY_DEF(USE_COUNTED,ADD_FUNC,REMOVE_FUNC) \
ROLL_NOTIFY_DEF \
USE_COUNT_REFERENCE_NOTIFY_INTERNAL(USE_COUNTED,ADD_FUNC,REMOVE_FUNC)


// Use this inside a public portion of the class declaration of a use
// counted ENTITY;

#define USE_COUNTED_DECL \
private: \
	int use_count_data; \
public: \
	ROLL_NOTIFY_DECL \
	virtual int use_count() const; \
	virtual logical is_use_counted() const; \
	virtual void set_use_count(int val); \
\
	/* Use count manipulation. Either add or subtract one use, and if \
	 * subtraction causes the use count to fall to zero, then lose \
	 * the record. \
	 */ \
\
	virtual void add( );\
	virtual void remove( logical lose_if_zero = TRUE ); 
		

// Use this in the definition of a use counted ENTITY

#define USE_COUNTED_DEF \
} \
void THIS()::add() { \
	if( ! get_restoring_history() ) { \
		++use_count_data; \
	} \
} \
\
void THIS()::remove( logical lose_if_zero ) { \
	if (--use_count_data == 0 && lose_if_zero ) \
		lose(); \
} \
\
int THIS()::use_count() const { \
	return use_count_data; \
} \
\
void THIS()::set_use_count(int val) { \
	use_count_data = val; \
} \
\
logical THIS()::is_use_counted() const { \
	return TRUE; \
\
ROLL_NOTIFY_DEF \
	if( type == CHANGE_BULLETIN ) { \
		THIS()* other_ent = (THIS()*) other; \
		other_ent->use_count_data = use_count_data; \
	} 


// Finally, if the use counted entity used USE_COUNT_DECL and
// USE_COUNT_DEF, as is normal, the refering entity can have a simpler
// syntax to define it's notify function

#define USE_COUNT_REFERENCE_DEF(USE_COUNTED) \
USE_COUNT_REFERENCE_NOTIFY_DEF(USE_COUNTED,add(),remove(FALSE))

// As an example of the most common use, here is an abbreviated
// version of the VERTEX/APOINT pair.

// USAGE_EXAMPLE
//
// In vertex.hxx
// -------------
//
//include "kerndata/data/usecount.hxx"
//class VERTEX : public ENTITY {
//
//	// Vertex data
//
//	public:
//
//	USE_COUNT_REFERENCE_DECL
//
//	// Other vertex functions
//
//};
//
// In vertex.cxx
// -------------
//
//#define THIS_LIB KERN
//#define THIS() VERTEX
//#define PARENT_LIB KERN
//#define PARENT() ENTITY
//
//ENTITY_DEF( VERTEX_NAME )
//	// Code to Print out a readable description of this vertex.
//
//USE_COUNT_REFERENCE_DEF(geometry())
//
//	// Other _DEFs to implement VERTEX
//
//TERMINATE_DEF
//
//
// In point.hxx
// ------------
//
//include "kerndata/data/usecount.hxx"
//class POINT : public ENTITY {
//
//	// point data
//
//public:
//
//	USE_COUNTED_DECL
//
//	// Other point methods
//
//};
//
// In point.cxx
// ------------
//
//#define THIS_LIB KERN
//#define THIS() APOINT
//#define PARENT_LIB KERN
//#define PARENT() ENTITY
//
//ENTITY_DEF( APOINT_NAME )
//	// Code to Print out a readable description of this point.
//
//USE_COUNTED_DEF
//
//	// Other _DEFs to implement POINT
//
//TERMINATE_DEF
//
// And, don't forget to set use_count_data to 0 in all constructors.
//
// End USAGE_EXAMPLE

#endif
