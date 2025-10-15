/* ORIGINAL: acis2.1/ct_husk/classes/cell2d.hxx */
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

// Header for CELL2D.

// The cell2d is a maximal connected sheet.  All of the faces stored are
// double-sided both-outside faces.

#if !defined( CELL2D_CLASS )
#define CELL2D_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

#include "en_macro.hxx"
#include "cell.hxx"

/**
 * @file cell2d.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

class SPAbox;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CELL2D</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CELL2D</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_CELL2D( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( CELL2D, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int CELL2D_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define CELL2D_LEVEL 2


// CELL2D declaration proper.
/**
 * Represents a two-dimensional cell, consisting of one or more sheet faces.
 * <br>
 * <b>Role:</b> A <tt>CELL2D</tt> is a maximal connected set of sheet faces that do not cross an edge
 * that is adjacent to more than two faces (that is, a non-manifold edge). All constituent faces are 
 * double-sided, both-outside faces.
 * The definition of a two-dimensional cell can be modified using the <tt>cell_granularity</tt> option.
 * It is directly derived from <tt>CELL</tt>.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see CFACE
 */

class DECL_CT CELL2D: public CELL {

	// Start of a list of cfaces that comprise the cell.

	CFACE *cface_ptr;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CELL2D, CT )
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


	// Now the functions specific to CELL2D.

	// Basic constructor, used internally for bulletin board activity.
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
 * call this constructor only with the overloaded <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class
 * (for example, <tt>x=new CELL2D</tt>), because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 */

	CELL2D();


	// Public constructor, which initialises all the class data, and
	// records the creation in the bulletin board.  The first argument
	// is the start of a lists of cshells contained in the cell, and
	// the second argument sets the pointer to the next cell in the
	// owning lump.  The constructor sets the backpointers in the
	// cshells in the given list.  The calling routine must set
	// set_owner() if desired, bound_ptr, using set_lump() and
	// set_bound() [declared below].
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new CELL2D(...)</tt>), because this reserves the
 * memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * Initializes all the class data, and records the creation in the bulletin
 * board.  <tt>CFACE</tt> starts the lists of cshells contained in the cell,
 * and <tt>CELL</tt> sets the pointer to the next cell in the owning lump. The
 * constructor sets the backpointers in the cshells in the given list.  The
 * calling routine must set the lump which owns the cell using
 * <tt>set_lump</tt> and, if desired, may set a bound using
 * <tt>set_bound</tt>.
 * <br><br>
 * @param list_cell
 * list of faces in cell.
 * @param list_sibling
 * list of sibling cells.
 */

	CELL2D( CFACE *list_cell, CELL *list_sibling );


	// Data reading routines.
/**
 * Returns the pointer to the list of cfaces that comprise the cell.
 */
 	CFACE *cface() const { return cface_ptr; }


	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
/**
 * Sets the <tt>CFACE</tt> for the <tt>CELL2D</tt>.
 * <br><br>
 * <b>Role:</b> Data changing routine that checks that the record is posted on the bulletin board before performing a change.
 * <br><br>
 * @param cface
 * cface.
 */

	void set_cface( CFACE * cface);

};

/** @} */
#endif
