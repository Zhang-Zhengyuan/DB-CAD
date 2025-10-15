/* ORIGINAL: acis2.1/kernutil/polyadic/polyadic.hxx */
/* $Id: polyadic.hxx,v 1.8 2000/12/26 18:48:29 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class to support a form of polyadic "virtual" function. Each derived version
// of the several "virtual" argument types needs an integer type identifier. The
// polyadic function class forms an n-dimensional "table" (as lists of lists
// currently) indexed on these type codes, and provides a lookup function.
// A client program defines a class derived from this one, containing any
// extra data required for the specific function to be called - generally just
// a pointer to the required function.

#if !defined( POLYADIC_FUNCTION )
#define POLYADIC_FUNCTION

#include "acis.hxx"
#include "dcl_kern.h"

// The type of the header which refers to the whole table. This needs to
// be a simple type, so that it is initialised (to NULL) before class
// initialisers are called.

class polyadic_function_header;
typedef polyadic_function_header *polyadic_function;
class polyadic_function_entry;

DECL_KERN polyadic_function_entry *find_polyadic_function_entry( int, int const *, 
	polyadic_function );

// Now the implementation class itself. This simply contains some structure
// and the two type codes. Anything useful has to be supplied by the client
// in a derivation of this class.

class DECL_KERN polyadic_function_entry : public ACIS_OBJECT {
private:
	// Structure for the table.

	polyadic_function_entry *next;
	polyadic_function_entry *list;

	// The type code represented by this entry.

	int type;

	// Back pointers used by the destructor.

	polyadic_function_entry *owner;
	polyadic_function *head;

protected:
	// Uninitialised constructor used during table construction.

	polyadic_function_entry();

	// Function to construct the table hierarchy for an instance of a
	// polyadic function. Called by the public constructor, and also by
	// derived class constructors when it is convenient to separate
	// object construction and initialisation.

	void construct_entry( int, int const *, polyadic_function & );

public:
	polyadic_function_entry( int, int const *, polyadic_function & );
	~polyadic_function_entry();

	// Function to find the indexed entry, given the table pointer.

	friend DECL_KERN polyadic_function_entry *find_polyadic_function_entry(
			int,
			int const *,
			polyadic_function
		);
};

// Specialisation of the polyadic class for monadic functions. Virtual
// functions are designed to do this for you, but require everything to be
// declared at the class level, which can mess up module hierarchies.

typedef polyadic_function monadic_function;

class DECL_KERN monadic_function_entry: public polyadic_function_entry {
public:
	monadic_function_entry( int, monadic_function & );
};

DECL_KERN monadic_function_entry *find_monadic_function_entry(
							int, monadic_function );

// Specialisation of the polyadic class for diadic functions, which is what
// we really want them for in Acis. This simply makes the interface for the
// client code a little easier.

typedef polyadic_function diadic_function;

class DECL_KERN diadic_function_entry: public polyadic_function_entry {
public:
	diadic_function_entry( int, int, diadic_function & );
};

DECL_KERN diadic_function_entry *find_diadic_function_entry(
							int, int, diadic_function );

#endif
