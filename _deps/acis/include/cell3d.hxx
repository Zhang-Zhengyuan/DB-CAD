/* ORIGINAL: acis2.1/ct_husk/classes/cell3d.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// dal 31Mar03 Documentation.
/*******************************************************************/

// Header for CELL3D.

// The cell3d is a connected subportion of a lump with a minimal boundary.
// Usually a lump will consist of a single cell (which is bounded by an
// outer cshell and zero, one or more inner cshells representing voids
// within the cell). However when an operation that leaves internal faces
// in a lump returns a lump with fully enclosed internal regions, each region
// will be represented by a 3D cell.


#if !defined( CELL3D_CLASS )
#define CELL3D_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

#include "en_macro.hxx"

//STI HDR_INDEP: nvp
#include "cell.hxx"

/**
 * @file cell3d.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

class SPAbox;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CELL3D</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CELL3D</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_CELL3D( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( CELL3D, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int CELL3D_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define CELL3D_LEVEL 2


// CELL3D declaration proper.
/**
 * Represents a three-dimensional cell, whose interior is a connected, 3-manifold (i.e.,volumetric) region.
 * <br>
 * <b>Role:</b> The <tt>CELL3D</tt> is a three-dimensional sub-portion of a lump bounded by either <tt>SINGLE-SIDED</tt>
 * or <tt>DOUBLE-SIDED</tt> <tt>BOTH-INSIDE</tt> faces. Usually a lump consists of a single cell that is bounded
 * by an outer <i>cshell</i> and zero, one or more inner cshells representing voids within the cell.
 * However, when an operation that leaves internal faces in a lump returns a lump with fully
 * enclosed internal regions, each region is a three-dimensional cell. 
 * (A 3D cell is bounded by one or more cshells similar to the way a lump is bounded by one or more shells.
 * That is, a cshell is analogous to a shell.) <tt>CELL3D</tt> is derived from <tt>CELL</tt>.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * <br><br>
 * @see CSHELL, CSHELL
 */

class DECL_CT CELL3D: public CELL {

	// Start of a list of cshells that comprise the cell.

	CSHELL *cshell_ptr;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CELL3D, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	// Search a private list for this object, used for debugging.

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Now the functions specific to CELL3D.

	// Basic constructor, used internally for bulletin board activity.

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class
 * (for example, <tt>x=new CELL3D</tt>), because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 */

	CELL3D();


	// Public constructor, which initialises all the class data, and
	// records the creation in the bulletin board.  The first argument
	// is the start of a lists of cshells contained in the cell, and
	// the second argument sets the pointer to the next cell in the
	// owning lump.  The constructor sets the backpointers in the
	// cshells in the given list.  The calling routine must set
	// lump_ptr and if desired, bound_ptr, using set_lump() and
	// set_bound() [declared below].
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new CELL3D(...)</tt>), because this reserves the
 * memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * <tt>CSHELL</tt> starts the lists of cshells contained in the cell, and
 * <tt>CELL</tt> sets the pointer to the next cell in the owning lump. The
 * constructor sets the backpointers in the cshells in the given list. The
 * calling routine must set the lump which owns the cell using
 * <tt>set_lump</tt> and, if desired, may set a bound using
 * <tt>set_bound</tt>.
 * <br><br>
 * @param cshell
 * cshell.
 * @param cell
 * cell.
 */

	CELL3D( CSHELL *cshell, CELL * cell);


	// Data reading routines.
/**
 * Returns the pointer to the list of cshells that comprise the shell.
 */

	CSHELL *cshell() const { return cshell_ptr; }


	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
/**
 * Sets the <tt>CSHELL</tt> for the <tt>CELL3D</tt>.
 * <br><br>
 * <b>Role:</b> This data changing routine checks that the record has been posted on the bulletin
 * board before performing a change.
 * <br><br>
 * @param cshell
 * cshell.
 */

	void set_cshell( CSHELL * cshell );

};

/** @} */
#endif
