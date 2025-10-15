/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define classes to map enum values <-> strings
#ifndef ENUM_CLASSES
#define ENUM_CLASSES
#include "base.hxx"
#include "dcl_base.h"
#include <string.h>
// EXAMPLE usage:
//
// In a .hxx file:
//
//	typedef enum { FOO_THIS, FOO_THAT, FOO_OTHER } foo;
//
// In a .cxx file:
//
//	static struct enum_entry foo_entries[] = {
//		{FOO_THIS,  "this"},
//		{FOO_THAT,  "that"},
//		{FOO_OTHER, "other"},
//		{-9999,		NULL}		// required terminating entry
//	};
//
/**
* @file enum_tbl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSTRINGS
 *
 * @{
 */
/**
 * Structure defining a pair of value/string and used in the <tt>enum_table</tt> class.
 * <br>
 * <b>Role:</b> The enum_entry stores an enum value and it's string representation.
 * An array of enum_entry types is used by the <tt>enum_table</tt> class.
 * The terminating entry in the array must have a value of -9999 and a NULL string.
*/
struct enum_entry {
	int value;
	char const *string;
};
/**
 * Defines objects for storing the mapping between the enum values and their string representation.
 * <br>
 * <b>Role:</b> An enumeration table stores a mapping between the enum values and their string representation.
 * The mapping is used when writing enumerated values to a sat file in their string representation, or when
 * reading strings from a SAT file that need to be converted back to enumerated values.
 */
class DECL_BASE enum_table : public ACIS_OBJECT {

	// Array of value/string mappings.
	// Terminated with entry containing NULL string pointer.

	const enum_entry *entry;

public:

/**
 * C++ constructor, creates an <tt>enum_table</tt> object and initializes it with the specified enum entry.
 * <br><br>
 * @param ent
 * initialization value.
 */
	enum_table(const enum_entry *ent) { entry = ent; };

/**
 * Retrieves value corresponding to given string.
 * <br><br>
 * <b>Role:</b> If no match, value in terminating entry (generally -9999) is returned.
 * <br><br>
 * @param string
 * input string.
 */
	int value( char const *string ) const;


/**
 * Retrieves string corresponding to given value.
 * <br><br>
 * <b>Role:</b> If no match, returns <tt>NULL</tt>.
 * <br><br>
 * @param value
 * input value.
 */
	char const *string( int value ) const;
};

/** @} */
#endif

