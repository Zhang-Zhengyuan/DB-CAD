/* ORIGINAL: acis2.1/kerndata/lists/glists.hxx */
// $Id: glists.hxx,v 1.12 2001/03/07 20:34:45 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef GENERIC_LISTS_H
#define GENERIC_LISTS_H

#include "vlists.hxx"
#include "dcl_kern.h"
#include "ccat.hxx"

// Header for list of things other than void.  Implemented by
// trivial derivation from VOID_LIST with casts as appropriate
// to allow pointers to other types of data.


// Note, We really should be deriving ENTITY_LIST from void*_list in the same
// maner as any list type is derived here from ENTITY_LIST.  That change would
// cause make to recompile a lot of files.  Mabey I'll get to that some Friday
// afternoon.

// MMGR_FIX_PRIVATE_TO_PUBLIC
#define LIST(TYPE, LIB) \
class name2(DECL_, LIB) name2(TYPE,_LIST) : public VOID_LIST { \
	name2(TYPE,_LIST)(const name2(TYPE,_LIST)&) { \
		/* private copy constructor which prevents passing lists by value \
		 * We have had a number of bugs related to doing so, since the \
		 * compiler provided version is not sufficient \
		 */ \
	} \
 public: \
 \
	name2(TYPE,_LIST)() {};		/* constructor for generic lists */ \
 \
	~name2(TYPE,_LIST)(){};		/* destructor */ \
 \
 \
	/* Empty a list ready for construction of a new one. */ \
 \
	void clear() { VOID_LIST::clear(); } \
 \
 \
	/* Add an TYPE to the list if not already there, and  */ \
	/* always return the index. */ \
 \
	int add( TYPE * e) { return VOID_LIST::add((void*) e); } \
 \
 \
	/* Search for an TYPE in the list. */ \
 \
	int lookup( TYPE const * ce) const { \
		return VOID_LIST::lookup( (void const *) ce ); \
	} \
 \
 \
	/* Delete an type from the list. This does not free space, and */ \
	/* leaves a tombstone in the linear list which count() and */ \
	/* operator[] will notice, but subsequently lookup() will not */ \
	/* find this type, nor will init()/next(). The return value is */ \
	/* the lookup() value of the old entry. */ \
 \
	int remove( TYPE const * ce ) { \
		return VOID_LIST::remove( (void const *) ce ); \
	} \
 \
 \
	/* Count how many entities there are in the list (including */ \
	/* deleted entries). */ \
 \
	int count() const { return VOID_LIST::count(); } \
 \
	/* Count how many entities there are in the list */ \
	/* not including deleted entries.  Uses the iterator */ \
 \
	int iteration_count() const { return VOID_LIST::iteration_count(); } \
 \
	/* Return the indexed entity, or NULL if the index is out of */ \
	/* range, or LIST_ENTRY_DELETED if the indexed entry has been */ \
	/* deleted. */ \
 \
	TYPE *operator[]( int i) const { \
		return (TYPE*) VOID_LIST::operator[](i); \
	} \
 \
 \
	/* Return entities in list order, ignoring deleted items. Call */ \
	/* init() once, then next() repeatedly until it returns NULL. */ \
	/* Note that next() returns the undeleted item most closely */ \
	/* following the one most recently returned by next() or */ \
	/* operator[], except that if that value was NULL the value */ \
	/* of next() is undefined. */ \
 \
	void init() const { VOID_LIST::init(); } \
	TYPE *next() const { return (TYPE*) VOID_LIST::next(); } \
}

#endif

