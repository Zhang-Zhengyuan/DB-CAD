/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( NODE_CLASS_ACIS )
#define NODE_CLASS_ACIS

// Header for NODE class.

#include "dcl_kern.h"
#include "logical.h"
#include "position.hxx"

#include "en_macro.hxx"

/**
* @file node.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

class scan_list;
class ELEM;
class SPApar_pos;
class large_ptr_array;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(NODE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier constant

/*
 // tbrv
 */
 /**
  * @nodoc
 */
extern DECL_KERN int NODE_TYPE;

// Level of derivation from ENTITY

#define NODE_LEVEL 1

// Define a NODE class.  This is a base class, but it is not
// abstract.  It can be used as is if sufficient.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN NODE: public ENTITY {

	// Store node coordinates.  Avoid using a SPAposition class to
	// save a little memory.

	double coords_data[3];

	// Store a single back pointer of an owning element to save
	// on element/node search time.  Only one element is stored
	// as the remainder can be found via element adjacency.
	// NOTE:  FOR SIMPLICITY WE DO NOT ALLOW NON-MANIFOLD NODES
	// WITHIN A MESH AT THE MOMENT.

	ELEM *element_ptr;

	// Standard member functions.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( NODE , KERN)
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

	// Nodes are not deletable because they are subservient to meshes.

	virtual logical deletable() const;

	// Constructors

	// Make a generic node to be filled in later.

	NODE() { element_ptr = NULL; }

	// Copy constructor.

	NODE( const NODE *old )
	{
		*this = *old;
	}

	// Basic constructor (from a SPAposition).

	NODE(const SPAposition& pos)
	{
		coords_data[0] = pos.x();
		coords_data[1] = pos.y();
		coords_data[2] = pos.z();

		element_ptr = NULL;
	}

	// Reading functions.

	SPAposition coords() const
	{
		return SPAposition(coords_data[0],coords_data[1],coords_data[2]);
	}
	ELEM *element() const { return element_ptr; }
	virtual void adjacent_nodes(ENTITY_LIST &) const;
	virtual void adjacent_elements(ENTITY_LIST &) const;
	virtual logical open() const;

	// Setting functions.

	void set_coords( const SPAposition& pos )
	{
		backup();
		coords_data[0] = pos.x();
		coords_data[1] = pos.y();
		coords_data[2] = pos.z();
	}

	void set_element(ELEM *new_el) {
		backup();
		element_ptr = new_el;
	}

	// Transform a node in place.  This may need to be defined
	// in a derived class.  (For example, if the node includes
	// a normal).

	virtual void operator*= (const SPAtransf &);

	// Node comparison operator.  No tolerance is used.

	virtual logical operator== (const NODE *) const;

	// Real node fixing function.

	virtual void fix_pointers(scan_list &);

	// Make a separate copy routine

	virtual NODE *copy(scan_list &) const;

	// Do the pointer stuff in a separate function.

	void copy_data(const NODE *, scan_list &);

	// Make a new save function too.

	virtual void save(scan_list &) const;

};


/** @} */
#endif
