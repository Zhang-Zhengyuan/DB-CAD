/* ORIGINAL: acis2.1/ct_husk/classes/cell.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// cup 17Oct05 Added internal functionaly for CT data recreation on copy
// dal 31Mar03 Documentation.
/*******************************************************************/

// Header for CELL.

// The cell is a minimal connected subportion of a lump.  Usually a
// lump will consist of a single cell (which is bounded by an outer cshell
// and zero, one or more inner cshells representing voids within the cell).
// However when an operation that leaves internal faces in a lump
// returns a lump with fully enclosed internal regions, each region
// will be represented by a cell.

#if !defined( CELL_CLASS )
#define CELL_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

#include "en_macro.hxx"
#include "lists.hxx"
/**
* @file cell.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup CELLTOPAPI
 * @{
 */
class SPAbox;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CELL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_CELL( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( CELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int CELL_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define CELL_LEVEL 1

// CELL declaration proper.
/**
 * Represents an <i>n</i>-dimensional cell, representing an <i>n</i>-dimensional manifold.
 * <br>
 * <b>Role:</b> The <tt>CELL</tt> is an abstract class for <tt>CELL2D</tt> and <tt>CELL3D</tt>. It provides the data
 * and member functions for the list pointers, the lump back pointer, the supercell pointer,
 * and the bounding box. It should never be instanced.
 * <br><br>
 * The cell is a minimal connected subportion of a lump. 
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see ATTRIB_CELL, SPAbox, LUMP
 */

class DECL_CT CELL: public ENTITY {


	// List pointer linking all the cells in a lump (last member
	// of list has this pointer NULL).

	CELL *next_ptr;

	// Back pointer to owning lump.

	LUMP *lump_ptr;

	// Back pointer to owning supercell.

	SUPERCELL *supercell_ptr;

	// Validity flag

	logical validity_data;

	// Pointer to a geometric bounding region (a SPAbox), within which the
	// entire cell lies (with respect to the internal coordinate
	// system of its body).
	// It may be NULL if no such bound has been calculated since the
	// cell was last changed.

	SPAbox *bound_ptr;

    // Internally used mapping array
    ENTITY_LIST copymap_list[2];

protected:

	/**
	 * Sets the bounding pointer to <tt>NULL</tt>.
	 */
	void kill_bound() { bound_ptr = NULL; }

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    // Include the fullsize functionality

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
	 * Returns the entity that owns the cell.
	 */
	virtual ENTITY *owner() const
	{
		return (supercell_ptr != NULL) ? (ENTITY *) supercell_ptr :
										(ENTITY *) lump_ptr;
	}
// STI aed: end

	/**
	 * Returns <tt>TRUE</tt> if the <tt>CELL</tt> is deletable; otherwise, it returns <tt>FALSE</tt>.
	 */
	virtual logical deletable() const;

	// Now the functions specific to CELL.

	// Basic constructor, used internally for bulletin board activity.

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should call
	 * this constructor only with the overloaded new operator inherited from the <tt>ENTITY</tt> class (for example,
	 * <tt>x=new CELL</tt>), because this reserves the memory on the heap, a requirement to support roll
	 * back and history management.
	 */
	CELL();

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator inherited
	 * from the <tt>ENTITY</tt> class (for example, <tt>x=new CELL(...)</tt>), because this reserves the memory on the heap, a
	 * requirement to support roll back and history management.
	 * <br><br>
	 * @param CELL
	 * next cell.
	 * @param LUMP
	 * lump.
	 * @param SPAbox
	 * bounding box.
	 */
	CELL(CELL*, LUMP*, SPAbox* );

	// Data reading routines.

	/**
	 * Returns a pointer to the next cell in the list maintained by an <tt>ATTRIB_CELL</tt> or a <tt>SUPERCELL</tt>.
	 */
	CELL *next_in_list() const { return next_ptr; }

	/**
	 * Returns the pointer to the next cell in a complete enumeration of all the cells owned by the lump.
	 */
	CELL *next() const;

	/**
	 * Returns the pointer to the owning lump.
	 */
	LUMP *lump() const { return lump_ptr; }

	/**
	 * Returns the pointer to the owning supercell, if one exists. 
	 */
	SUPERCELL *supercell() const { return supercell_ptr; }

	/**
	 * Returns <tt>TRUE</tt> if the <tt>CELL</tt> is valid; otherwise, it returns <tt>FALSE</tt>.
	 */
	logical valid() const { return validity_data; }

	/**
	 * Returns the bound pointer.
	 */
	SPAbox *bound() const { return bound_ptr; }

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.

	/**
	 * Changes the pointer to the next cell.
	 * <br><br>
	 * <b>Role:</b> This method checks that the record has been posted on the bulletin board before
	 * performing the change. If not, the method returns an error, so that the omission (forgetting to call
	 * <tt>backup</tt> first) can be rectified in the program.
	 * <br><br>
	 * @param CELL
	 * cell.
	 */
	void set_next( CELL * );

	/**
	 * Changes the pointer to the owning lump.
	 * <br><br>
	 * <b>Role:</b> This method checks that the record has been posted on the bulletin board before
	 * performing the change. If not, the method returns an error, so that the omission (forgetting
	 * to call <tt>backup</tt> first) can be rectified in the program.
	 * <br><br>
	 * @param LUMP
	 * lump.
	 */
	void set_lump( LUMP * );

	/**
	 * Changes the pointer an owning supercell. (Usually there are no supercells.)
	 * <br><br>
	 * <b>Role:</b> This method checks that the record has been posted on the bulletin board before
	 * performing the change. If not, the method returns an error, so that the omission (forgetting
	 * to call <tt>backup</tt> to call backup first) can be rectified in the program.
	 * <br><br>
	 * @param SUPERCELL
	 * supercell.
	 */
	void set_supercell( SUPERCELL * );

	/**
	 * Changes the data for the bounding box.
	 * <br><br>
	 * <b>Role:</b> This method checks that the record has been posted on the bulletin board before
	 * performing the change. If not, the method returns an error, so that the omission (forgetting
	 * to call <tt>backup</tt> first) can be rectified in the program.
	 * <br><br>
	 * @param SPAbox
	 * bounding box.
	 */
	void set_bound( SPAbox * );

	// Set the update data...usually for use only by ct_husk automatic
	// update code...calling these member functions will affect the
	// actions of the automatic cell update computation.

	/**
	 * Validates the update data.
	 */
	void validate();

	/**
	 * Invalidates the update data.
	 */
	void invalidate();

    /**
    * @nodoc
    */
    void register_in_copy_map( ENTITY * src_ent, ENTITY * target_ent);

    /**
    * @nodoc
    */
    ENTITY * retrieve_mapped_target_from_source( ENTITY *source);

    /**
    * @nodoc
    */
    ENTITY * retrieve_mapped_source_from_target( ENTITY *target);

    /**
    * @nodoc
    */
    void flush_copy_map();
};

/** @} */
#endif
