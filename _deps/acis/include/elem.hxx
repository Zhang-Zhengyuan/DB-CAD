/* ORIGINAL: acis2.1/meshhusk/mesh/elem.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ELEM_CLASS )
#define ELEM_CLASS

// Header for base element class.

#include "dcl_kern.h"
#include "logical.h"

#include "en_macro.hxx"

class SPAtransf;
class SPAbox;
class large_ptr_array;
class NODE;
class scan_list;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ELEM , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier constant

extern DECL_KERN int ELEM_TYPE;

// Level of derivation from ENTITY

#define ELEM_LEVEL 1

// Define the element base class.

class DECL_KERN ELEM: public ENTITY{

protected:

	// Doubly-linked list

	ELEM *next_ptr;
	ELEM *previous_ptr;

	// Standard ENTITY member functions.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( ELEM , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

//	TRANSFORM_FUNCTION

	// Elements are not deletable because they are subservient to meshes.

	virtual logical deletable() const;

	// Generic constructor.

	ELEM(){
		next_ptr = NULL;
		previous_ptr = NULL;
	}

	// Copy (bitwise).

	ELEM(const ELEM *old)
	{
		*this = *old;
	}

	// Read the link pointers.

	ELEM *next() const { return next_ptr; }
	ELEM *previous() const { return previous_ptr; }

	// Link setting functions.

	void set_next(ELEM *);

	void set_previous(ELEM *);

	// Virtual access functions.

//	virtual int node_count() const;
//	virtual int element_count() const;

	// Transform operator.  Does nothing at this level.  Placeholder.

	virtual void operator*= (const SPAtransf &);

	// Comparison operator.  Placeholder.

	virtual logical operator==(const ELEM *) const;

	// Element checker.

	virtual void check(logical = FALSE) const;

	// Const SPAbox computation.  Handy for the intersectors, since they
	// take const elements.

	virtual SPAbox bound() const;

	// Non-const SPAbox computation.

	virtual SPAbox bound();

	// Pointer fixing.

	virtual void fix_pointers( scan_list &);

	// Specialized copy routine one global and three local (in
	// the scan attribute) entity lists.

	virtual ELEM *copy(  scan_list &) const;

	void copy_data(const ELEM *,  scan_list &);

	// Save routine.

	virtual void save( scan_list &) const;

};

#endif
