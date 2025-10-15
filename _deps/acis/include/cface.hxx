/* ORIGINAL: acis2.1/ct_husk/classes/cface.hxx */
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

// Header for CFACE.

// A CFACE is a reference to one "side" of a FACE.  If a face is
// double-sided, it should be pointed to by two cfaces.  If it is
// single-sided, one cface uses it in the FORWARD direction.

// The normal of a CFACE points AWAY FROM the material contained
// in the CELL3D, just as the normal of a FACE points away from the
// material contained in a lump.  A CFACE whose normal points in
// the same direction as the FACE is termed the "back cface" and
// its sense is FORWARD/INSIDE.  A CFACE whose normal points in
// the opposite direction as the FACE is termed the "front cface"
// and its sense is REVERSED/OUTSIDE.  In other words, the CFACE
// that sits on the front of a FACE is outside the material of the
// FACE and its normal is reversed with respect to the normal of
// the FACE.

#if !defined( CFACE_CLASS )
#define CFACE_CLASS

#include "dcl_ct.h"
#include "tophdr.hxx"

#include "en_macro.hxx"
/**
* @file cface.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

class SPAbox;

//STI HDR_INDEP: nvp
class CSHELL;
class CELL2D;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CFACE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CFACE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_CFACE( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(CFACE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int CFACE_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define CFACE_LEVEL 1


// CFACE declaration proper
/**
 * A reference to one <i>side</i> of a <tt>FACE</tt>.
 * <br>
 * <b>Role:</b> A <tt>CFACE</tt> is a reference to one <i>side</i> of a <tt>FACE</tt>. It represents a
 * use of the face by a cell. If a face is double-sided, the face should be pointed to by two cfaces: 
 * if the face is single-sided, one cface uses it in the <tt>FORWARD</tt> direction. 
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see ATTRIB_FACECFACE, CELL2D, CSHELL, ENTITY, FACE
 */
	class DECL_CT CFACE: public ENTITY {

	// Pointer to next cface in list contained directly
	// by CSHELL.

	CFACE *next_ptr;

	// Pointer to face.

	FACE *face_ptr;

	// Pointer to owning entity, be it cell2d or cshell.

	ENTITY *owner_ptr;

	// Side of face used by the cface.  (OUTSIDE or INSIDE which
	// correspond to REVERSED and FORWARD respectively).

	REVBIT sense_data;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CFACE, CT )
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

    /**
     * @nodoc
     */
    virtual logical deletable() const;

	// Now the functions specific to CFACE.

	// Basic constructor, used internally for bulletin board activity.
/**
 * C++ constructor, creating a <tt>CFACE</tt>.
 * <br><br>
 * <b>Role:</b> Applications should never directly call this constructor, but should instead
 * call the overloaded <tt>new</tt> operator. Used by restore and roll back.
 */
	CFACE();


	// Public constructor which initialises the record and interfaces
	// with the bulletin board.  The arguments initialise the members
	// face_ptr, next_ptr, sense_data respectively.
	// The calling routine must set owner_ptr to refer
	// to the owning cshell, and if desired, bound_ptr,
	// using set_cshell() and set_bound()
	// [declared below].
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new CFACE(...)</tt>), because this reserves the
 * memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * The calling method must set <tt>owner_ptr</tt> to refer to the owning cshell, and, if desired,
 * <tt>bound_ptr</tt>, using <tt>set_cshell</tt> and <tt>set_bound</tt>.
 * <br><br>
 * @param list_sibling
 * list of sibling cfaces.
 * @param own_face
 * owned face.
 * @param face_dir
 * with respect to face direction.
 */
	CFACE( CFACE *list_sibling, FACE *own_face, REVBIT face_dir );


	// Data reading routines.
/**
 * Returns the pointer to the next <tt>cface</tt> in the list contained directly by <tt>CSHELL</tt> or <tt>CELL2D</tt>.
 */
 	CFACE *next() const { return next_ptr; }
/**
 * Returns the pointer to the face.
 */
 	FACE *face() const { return face_ptr; }
// STI aed: make owner method virtual
	//ENTITY *owner() const { return owner_ptr; };
/**
 * Returns the pointer to the owning entity.
 */
 	virtual ENTITY *owner() const { return owner_ptr; };
// STI aed: end
/**
 * Returns the side of the face used by the <tt>cface</tt> (<tt>OUTSIDE</tt> or <tt>INSIDE</tt> correspond to <tt>REVERSED</tt> or <tt>FORWARD</tt>, respectively).
 */
 	REVBIT sense() const { return sense_data; }
/**
 * Returns the bounding box of the face for the <tt>cface</tt>.
 */
 	SPAbox *bound() const;


	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
/**
 * Changes the data for the next <tt>cface</tt>.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method returns an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cface
 * cface.
 */
	void set_next( CFACE * cface);
/**
 * Changes the data for the face.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method returns an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param face
 * face
 */
	void set_face( FACE * face );
/**
 * Changes the data for the cshell.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method returns an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cshell
 * cshell.
 */
	void set_cshell( CSHELL * cshell );
/**
 * Changes the data for the cell2d.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method returns an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param cell
 * cell2d.
 */
	void set_cell2d( CELL2D * cell);
/**
 * Changes the data for the sense data.
 * <br><br>
 * <b>Role:</b> This method checks that the record has been posted on the bulletin board
 * before performing the change. If not, the method returns an error, so that the omission
 * (forgetting to call <tt>backup</tt> first) can be rectified in the program.
 * <br><br>
 * @param sense
 * sense to set.
 */
	void set_sense( REVBIT sense );

};

/** @} */
#endif
