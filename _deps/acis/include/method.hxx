/* ORIGINAL: acis2.1/kerndata/data/method.hxx */
/* $Id: method.hxx,v 1.8 2000/12/26 18:46:47 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// STI aed: New support for Runtime virtual methods

// Header for Runtime Virtual Method control structures


#ifndef METHOD_HXX
#define METHOD_HXX
/**
* @file method.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */
#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"

class METHOD_ARGS;

// Define type for functions used to implement Runtime methods

/*
// tbrv
// knt - this typedef needs better docs for it's args.
*/
/**
 * Method function.
 * @param pv
 * pointer.
 * @param args
 * method args.
 */
typedef logical (*MethodFunction)(void * pv, METHOD_ARGS const & args);

// Define a class used to maintain a master list of methods. Each method
// is identified by its name and argument list ID string and is assigned
// a unique index into the method table for each class which supports
// runtime methods.

// Developers should not need to deal directly with this class.
// They should use the METHOD_ID class to add and look up methods.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN METHOD_ENTRY : public ACIS_OBJECT
{
private:

	// Method name
	char *Name;

	// Argument list ID string
	char *ArgID;

	// Index for implementation function in method tables
	int Index;

// STI aed: Add use count so entries get cleaned up
	// Number of METHOD_IDs and METHOD_TABLES which use this entry
	int UseCount;
// STI aed: end

	// Pointer to next method in master list
	METHOD_ENTRY *Next;

public:

	// Insert a new method into the master list
	METHOD_ENTRY(
		const char *name,		// Method name
		const char *arg_id,		// Argument list ID
		METHOD_ENTRY **prev		// Position at which to insert entry
		);

// STI aed: Add methods to track use and destroy unused entries
	// Remove entry from master list and free data
	~METHOD_ENTRY();

	// Increment use count
	void add() { UseCount++; }

	// Decrement use count and destroy if unused
	void remove();

	// Find the entry in the master list for an index
	static METHOD_ENTRY *lookup(
		int idx					// Entry index
		);
// STI aed: end

	// Find the entry in the master list for a method
	static METHOD_ENTRY *lookup(
		const char *name,		// Method name
		const char *arg_id		// Argument list ID
		);

// STI aed: make query methods const
	// Get the method name for an entry
	//const char *name() { return Name; }
	const char *name() const { return Name; }

	// Get the Argument list ID string for an entry
	//const char *arg_id() { return ArgID; }
	const char *arg_id() const { return ArgID; }

	// Get the table index value for an entry
	//int index() { return Index; }
	int index() const { return Index; }

	// Get the pointer to the next entry in the master list
	//METHOD_ENTRY *next() { return Next; }
	METHOD_ENTRY *next() const { return Next; }
// STI aed: end

	// Set the pointer to the next entry in the master list
	void set_next(METHOD_ENTRY *next) { Next = next; }
};


// Define a class to identify runtime methods. The first instance of this
// class with a given method name and argument list ID adds the method to
// the master list. Later instances with the same name and argument list
// use the entry created by the first instance.

/**
 * Identifies runtime methods.
 * <br>
 * <b>Role:</b> This class identifies runtime methods. The first instance of this class with a given
 * method name and argument list ID adds the method to the master list. Later instances with the same
 * name and argument list use the entry created by the first instance.
 *<br><br>
 * @see METHOD_ENTRY
 */

class DECL_KERN METHOD_ID : public ACIS_OBJECT
{
private:

	// Pointer to the method's entry in the master list
	METHOD_ENTRY *Entry;

public:

// STI aed: Add simple constructors and destructor
	// Default constructor

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	METHOD_ID() { Entry = NULL; }

	// Copy constructor
 /**
  * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
  *<br><br>
  * @param id
  * identification.
  */
	METHOD_ID(METHOD_ID const &id);

	// Create a method ID.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param name
 * Method name.
 * @param arg_id
 * Argument list ID.
 */
	METHOD_ID(
		const char *name,		// Method name
		const char *arg_id		// Argument list ID
		);

	// Destructor
/**
 * C++ destructor, deleting a <tt>METHOD_ID</tt>.
 */
	~METHOD_ID();

// STI aed: end

// STI aed: make query methods const and add entry method
	// Get the method name
	//const char *name() { return Entry->name(); }
/**
 * Returns the method name.
 */
	const char *name() const { return Entry->name(); }

	// Get the argument list ID
	//const char *arg_id() { return Entry->arg_id(); }
/**
 * Locates an argument identification.
 */
	const char *arg_id() const { return Entry->arg_id(); }

	// Get the method table index
	//int index() { return Entry->index(); }
/**
 * Returns the method table index.
 */
	int index() const { return Entry->index(); }

	// Get the method entry pointer
/**
 * Locates an entry for a method.
 */
	METHOD_ENTRY *entry() const { return Entry; }
// STI aed: end
};


// Define a class to hold the table of runtime method implementation functions
// for a class. Each class which supports runtime methods declares a method
// table and provides a mechanism to register and execute methods in the table.

/**
 * Holds the table of runtime method implementation functions for a class.
 *<br><br>
 * <b>Role:</b> This class holds the table of runtime method implementation functions for a class.
 * Each class that supports runtime methods declares a method table and provides a mechanism to
 * register and execute methods in the table.
 */
class DECL_KERN METHOD_TABLE : public ACIS_OBJECT
{
private:

	// Number of entries currently in the function table.
	int Size;

	// Dynamically array of function pointers.
	MethodFunction *Funcs;

public:
	// Must provide a default constructor because we provide a destructor.
	// But, explicitly initializing elements would create a dependency on
	// order of initialization of static variables.
/**
 * C++ constructor, creating a <tt>METHOD_TABLE</tt>.
 */
	METHOD_TABLE() {}

	// Destructor - frees function array
/**
 * C++ destructor, deleting a <tt>METHOD_TABLE</tt>.
 */
	~METHOD_TABLE();

// STI aed: pass METHOD_IDs by reference
	// Register an implementation function for a method in this table
	// Returns the current implementation function or NULL
	//MethodFunction add(METHOD_ID id, MethodFunction func);
/**
  * Registers an implementation function for a method in this table and returns the current implementation function or <tt>NULL</tt>.
  *<br><br>
  * @param id
  * method ID.
  * @param func
  * method function.
  */
	MethodFunction add(METHOD_ID const &id, MethodFunction func);

	// Find a implementation function for a method in this table.
	// Returns NULL if no function is registered for the method.
	//MethodFunction find(METHOD_ID id);
/**
  * Finds an implementation function for a method in this table and returns <tt>NULL</tt> if no function is registered for the method.
  *<br><br>
  * @param id
  * method ID.
  */
	MethodFunction find(METHOD_ID const &id);
// STI aed: end
};

/*! @} */
#endif
