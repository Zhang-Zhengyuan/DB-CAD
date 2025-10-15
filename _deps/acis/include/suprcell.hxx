/* ORIGINAL: acis2.1/ct_husk/classes/suprcell.hxx */
// $Id: suprcell.hxx,v 1.9 2002/08/09 17:20:42 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for SUPERCELL.

// This represents a grouping of cells or inferior supercells.  It 
// will allow the system to improve the efficiency of many-to-many 
// comparisons, by allowing quantities of cells to be excluded by a 
// single SPAbox test.  The subdivision is determined by the system, and 
// may change at any time, so the supercell has no significance to the 
// user (though the application program may find the spatial 
// subdivision implied useful).


#if !defined( SUPERCELL_CLASS )
#define SUPERCELL_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"

#include "en_macro.hxx"
#include "cell.hxx"
#include "at_cell.hxx"

class SPAbox;

/**
* @file suprcell.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SUPERCELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>SUPERCELL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_SUPERCELL( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SUPERCELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

/**
 * @nodoc
 */
extern DECL_CT int SUPERCELL_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

/**
 * @nodoc
 */
#define SUPERCELL_LEVEL 1


// SUPERCELL declaration proper.

/**
 * Provides a hierarchical cellular data structure. 
 * <br>
 * <b>Role:</b> A <tt>SUPERCELL</tt> functions much like a <tt>SUBSHELL</tt> in the standard topological structure. 
 * <tt>SUPERCELL</tt>s are optional in the cellular topology data structure. They provide a spatial decomposition 
 * of a cellular topology data structure. <tt>SUPERCELL</tt>s can be expanded or flattened (in a manner similar to 
 * <tt>SUBSHELL</tt>s). The following API functions demonstrate how hierarchies of <tt>SUPERCELL</tt>s can be expanded
 * and flattened: @href api_ct_expand, @href api_ct_flatten, @href api_ct_expand_cells, and @href api_ct_flatten_cells.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see ATTRIB_FACECFACE, CELL2D, CSHELL, ENTITY, FACE
 */
class DECL_CT SUPERCELL: public ENTITY {

	// Pointer to the next superior supercell, or NULL if this supercell 
	// belongs directly to an ATTRIB_CELL.

	SUPERCELL *parent_ptr;


	// Next supercell in list contained by superior ATTRIB_CELL or supercell.

	SUPERCELL *sibling_ptr;


	// Start of list of supercells contained within this one - allows a 
	// hierarchy of subdivisions.

	SUPERCELL *child_ptr;


	// Start of list of CELLS contained in this supercell.  Each cell
	// must be under the ATTRIB_CELL or in one supercell's list.

	CELL *cell_ptr;


	// Pointer to a geometric bounding region (a SPAbox), within which the 
	// supercell lies (with respect to the internal coordinate system of
	// its body). 
	// It may be NULL if no such bound has been calculated since the 
	// body was last changed.

	SPAbox *bound_ptr;


	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( SUPERCELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	// Search a private list for this object, used for debugging.

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

// STI aed: add owner method
	/**
	 * Returns the pointer to the owning entity.
	 */
	virtual ENTITY *owner() const { return parent_ptr; }
// STI aed: end

    /**
     * @nodoc
     */
	virtual logical deletable() const;

	// Now the functions specific to SUPERCELL.

	// Basic constructor, used internally for bulletin board activity.

	/**
	 * C++ constructor, creating a <tt>SUPERCELL</tt>.
	 * <br><br>
	 * <b>Role:</b> Applications should never directly call this constructor, but should instead
	 * call the overloaded <tt>new</tt> operator. Used by restore and roll back.
	 */
	SUPERCELL();

	// Public constructor which initialises the record and interacts 
	// with the bulletin board.  The arguments initialise face_ptr, 
	// child_ptr and sibling_ptr respectively.  The calling routine 
	// must set parent_ptr and if desired, bound_ptr, using 
	// set_parent() and set_bound() [declared below].

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
	 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x = new SUPERCELL(...)</tt>), because this reserves the
	 * memory on the heap, a requirement to support roll back and history management.
	 * <br><br>
	 * The calling routine must call @href set_parent and, if desired, @href set_bound.
	 * <br><br>
	 * @param first_cell
	 * contained cell list.
	 * @param first_supercell
	 * head of contained supercell list (i.e., the first child supercell).
	 * @param next_supercell
	 * next sibling supercell.
	 */
	SUPERCELL( CELL *first_cell, SUPERCELL *first_supercell, SUPERCELL *next_supercell );


	// Data reading routines.

	/**
	 * Returns the pointer to the parent <tt>SUPERCELL</tt>.
	 * Pointer to the next superior <tt>SUPERCELL</tt>, or <tt>NULL</tt> 
	 * if this <tt>SUPERCELL</tt> belongs directly to an <tt>ATTRIB_CELL</tt>.
	 */
	SUPERCELL *parent() const { return parent_ptr; }

	/**
	 * Returns the pointer to the next sibling <tt>SUPERCELL</tt>.
	 * Next <tt>SUPERCELL</tt> in list contained by superior <tt>ATTRIB_CELL</tt> or <tt>SUPERCELL</tt>.
	 */
	SUPERCELL *sibling() const { return sibling_ptr; }

	/**
	 * Returns the pointer to the first child <tt>SUPERCELL</tt>.
	 * Start of list of supercells contained within this one - allows a hierarchy of subdivisions.
	 */
	SUPERCELL *child() const { return child_ptr; }

	/**
	 * Returns the pointer to the first contained <tt>CELL</tt>.
	 * Each cell must be in the <tt>ATTRIB_CELL</tt>'s list of cells <i>or</i> in one <tt>SUPERCELL</tt>'s list of cells.
	 */
	CELL *cell_list() const { return cell_ptr; }

	/**
	 * Returns the pointer to the bounding box.
	 */
	SPAbox *bound() const { return bound_ptr; }


	// Data changing routines.  Each of these routines checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	/**
	 * Sets the pointer to the parent <tt>SUPERCELL</tt>.
	 */
	void set_parent( SUPERCELL *parent );

	/**
	 * Sets the pointer to the next sibling <tt>SUPERCELL</tt>.
	 */
	void set_sibling( SUPERCELL *sibling );

	/**
	 * Sets the pointer to the first child <tt>SUPERCELL</tt>.
	 */
	void set_child( SUPERCELL *child );

	/**
	 * Sets the pointer to the first contained <tt>CELL</tt>.
	 */
	void set_cell( CELL *cell );

	/**
	 * Sets the pointer to the bounding box.
	 */
	void set_bound( SPAbox *bound );

	/** 
	 * Returns the first cell in a complete enumeration of all the cells
	 * in the owning <tt>ATTRIB_CELL</tt>, continued by repeated use of <tt>CELL::next()</tt>.
	 * This is not generally very useful, but makes the corresponding
	 * <tt>ATTRIB_CELL</tt> function somewhat simpler.
	 */

	CELL *cell() const;
};

/** @} */
#endif
