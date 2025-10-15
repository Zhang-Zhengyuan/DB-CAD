/* ORIGINAL: acis2.1/ct_husk/classes/cshell.hxx */
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

// Header for CSHELL.

// The cshell is one portion of a cell's boundary - it is in a different
// shell from any other cshell in that cell.  If a cell has no voids,
// then exactly one cshell gives its overall extent; any other cshells bound
// voids wholly within the cell.  There is no distinction made in the
// data structure between peripheral and void cshells, indeed, some
// cshells may be open (e.g. a single face floating within a cell).

// A cshell is constructed from a collection of "cfaces", each of
// which is one side of an existing face.

#if !defined( CSHELL_CLASS )
#define CSHELL_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

#include "en_macro.hxx"

/**
* @file cshell.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

class SPAbox;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CSHELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CSHELL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_CSHELL( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( CSHELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int CSHELL_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define CSHELL_LEVEL 1


// CSHELL declaration proper.
/**
 * Represents one portion of a 3D cell's boundary.
 * <br>
 * <b>Role:</b> The <tt>CSHELL</tt> is one portion of a 3D cell's boundary; it is in a different shell from
 * any other <tt>CSHELL</tt> in that 3D cell. If a cell has no voids, then exactly one <tt>CSHELL</tt> gives its
 * overall extent; any other <tt>CSHELL</tt>s bound voids wholly within the cell. There is no distinction
 * made in the data structure between peripheral and void <tt>CSHELL</tt>s; indeed, some <tt>CSHELL</tt>s may be
 * open; e.g., a single face floating within a cell.
 * <br><br>
 * A <tt>CSHELL</tt> is constructed from a collection of <tt>CFACE</tt>s, each of which is one side of
 * an existing face.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see CELL3D, CFACE, CELL3D, SPAbox
 */

class DECL_CT CSHELL: public ENTITY {


	// List pointer linking all the cshells in a body (last member
	// of list has this pointer NULL).

	CSHELL *next_ptr;

	// Start of a list of immediately-contained cfaces.

	CFACE *cface_ptr;

	// Pointer to the owning cell - cshells in separate cells are
	// themselves entirely separate.

	CELL3D *cell_ptr;


	// Pointer to a geometric bounding region (a SPAbox), within which the
	// entire cshell lies (with respect to the internal coordinate
	// system of its body).
	// It may be NULL if no such bound has been calculated since the
	// cshell was last changed.

	SPAbox *bound_ptr;


	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CSHELL, CT )
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
 * Returns the owner of the <tt>CSHELL</tt>.
 */
 	virtual ENTITY *owner() const { return (ENTITY *) cell_ptr; }
// STI aed: end
/**
 * @nodoc
 */
 	virtual logical deletable() const;

	// Now the functions specific to CSHELL.

	// Basic constructor, used internally for bulletin board activity.
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
 * call this constructor only with the overloaded <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class
 * (for example, <tt>x=new CSHELL</tt>), because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 */
 	CSHELL();


	// Public constructor, which initialises all the class data, and
	// records the creation in the bulletin board.  The first
	// argument is the start of list of cfaces
	// contained, and the last is a list of "sister" cshells already
	// in the current cell.  The calling routine must set cell_ptr and
	// if desired, bound_ptr, using set_cell() and set_bound()
	// [declared below].
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new CSHELL(...)</tt>), because this reserves the
 * memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param list_face
 * cfaces list in cshell.
 * @param list_sibling
 * sibling list cshells.
 */

	CSHELL( CFACE *list_face, CSHELL *list_sibling );


	// Data reading routines.
/**
 * Returns the next <tt>CSHELL</tt>.
 */

	CSHELL *next() const { return next_ptr; }

/**
 * Returns the start of a list of immediately contained cfaces.
 */
	CFACE *cface() const { return cface_ptr; }

/**
 * Returns the pointer to owning cell.
 */

	CELL3D *cell() const { return cell_ptr; }

/**
 * Returns the bounding box for the <tt>CSHELL</tt>.
 */

	SPAbox *bound() const { return bound_ptr; }


	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
/**
 * Sets the pointer to the next <tt>CSHELL</tt> in the <tt>CELL3D</tt>.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method generates an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cshell
 * cshell.
 */
	void set_next( CSHELL * cshell);
/**
 * Sets the pointer to the first <tt>CFACE</tt> in the <tt>CSHELL</tt>.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method generates an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cface
 * cface.
 */

	void set_cface( CFACE * cface );
/**
 * Sets the <tt>CELL3D</tt> of the <tt>CSHELL</tt>.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method generates an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cell
 * cell3d.
 */

	void set_cell( CELL3D * cell );
/**
 * Sets the bounding box of the <tt>CSHELL</tt>.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method generates an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param b_box
 * bounding box.
 */

	void set_bound( SPAbox * b_box);

};

/** @} */
#endif
