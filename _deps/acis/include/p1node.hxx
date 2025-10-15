/* ORIGINAL: acis2.1/meshhusk/mesh/p1node.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( P1NODE_CLASS )
#define P1NODE_CLASS

// Header for node with single SPAparameter value.

#include "dcl_kern.h"
#include "node.hxx"
#include "param.hxx"

// Identifier constant.

extern DECL_KERN int P1NODE_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(P1NODE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Level of derivation from ENTITY.

#define P1NODE_LEVEL 2

// Define P1NODE class.

class DECL_KERN P1NODE:public NODE {

	// Store a SPAparameter value.

	double param_data;

public:

	// Standard member functions.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( P1NODE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	// Constructor...

	// No arguments.

	P1NODE():NODE()
	{
		param_data = 0.;
	}

	// Basic constructor.

	P1NODE(const SPAposition& pos, double par):NODE(pos)
	{
		param_data = par;
	}

	// Copy.  Bitwise works.

	P1NODE(const P1NODE *old )
	{
		*this = *old;
	}

	// Parameter value reading function.

	double t() const { return param_data; }

	// Setting function.

	void set_t(const double &par){

		backup();
		param_data = par;
	}

	// Comparison function.  Intended to compare nodes of (exactly)
	// alike class, and must be defined in each new node class.

	virtual logical operator== (const NODE *) const;

	// Specialized fix, save and copy.

	virtual void fix_pointers(scan_list &);

	virtual NODE *copy(scan_list &) const;

	virtual void save(scan_list &) const;

};

#endif
