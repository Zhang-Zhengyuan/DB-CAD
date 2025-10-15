/* ORIGINAL: acis2.1/ct_husk/classes/at_cell.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// cup 17Oct05 Implemented copy_owner to cope with down_copy functionality
// dal 03Apr03 Remove docs for inherited methods such as copy_owner
// dal 31Mar03 Documentation.
/*******************************************************************/

#if !defined( ATTRIB_CELL_CLASS )
#define ATTRIB_CELL_CLASS

#include "dcl_ct.h"
#include "at_ct.hxx"

/**
* @file at_cell.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

#define INSIDE 0
#define OUTSIDE 1

class LUMP;
class CELL;
class SUPERCELL;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_CELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_CELL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_ATTRIB_CELL( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE ( ATTRIB_CELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_CELL_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_CELL_LEVEL (ATTRIB_CT_LEVEL + 1)

/**
 * Implements an attribute pointing to a list of cells belonging to a lump.
 * <br>
 * <b>Role:</b> One of two attribute classes (the other being @href ATTRIB_FACECFACE) that connect the
 * Cellular Topology Data Structure to the standard ACIS Topology Structure. This attribute
 * is attached to a lump and contains a pointer to the list of cells for that lump. It can optionally 
 * contain a list of supercells (a cell hierarchy very similar to subshells). An <tt>auto_update</tt> flag 
 * indicates to the automatic cell re-computation mechanism whether or not to update the cell data of the attribute.
 * Each lump with Cellular Topology will have one <tt>ATTRIB_CELL</tt>.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see CELL
 */

class DECL_CT ATTRIB_CELL: public ATTRIB_CT {

//	Pointer to list of cells (2D and 3D contained) in the
//	cell attribute.

	CELL *cell_ptr;

//	Pointer to (optional) list of supercells in the cell
//	attribute.

	SUPERCELL *supercell_ptr;

//	Data that indicates if the cell data should be recalculated
//	at the end of an api (TRUE) or suspended until later (FALSE);

	logical auto_update_data;

public:

	// Construct the initial attribute
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
     * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_CELL(...)</tt>), because
     * this reserves the memory on the heap, a requirement to support roll back and history management.
     * <br><br>
     * @param lmp
     * lump.
     * @param cel
     * cell.
     */
	ATTRIB_CELL( LUMP *lmp = NULL, CELL *cel= NULL );

	// Data reading routines

    /**
     * Returns the list of cells.
     * <br><br>
     * <b>Role:</b> If there are no subshells, this returns the same pointer as <tt>cell</tt>.
	 * If there are subshells, this returns the first cell in the list of cells maintained by this attribute; 
	 * that is, the first cell that is not in any subshell.
	 */
	CELL *cell_list() const { return cell_ptr; }

    /**
     * Returns the (optional) list of supercells in the cell attribute.
     */
	SUPERCELL *supercell() const { return supercell_ptr; }

    /**
     * Indicates if the cell data should be recalculated at the end of an API.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if the cell data should be recalculated at the end of an API;
     * otherwise, it returns <tt>FALSE</tt>, which means suspend the recalculation until later.
     */
	logical auto_update_on() const { return auto_update_data; }

	// Return the first cell in a complete enumeration of all cells
	// in the attribute.  If there are no subshells, this returns
	// the same thing as cell_list().
    /**
     * Returns the first cell in a complete enumeration of all cells in the lump.
     * <br><br>
     * <b>Role:</b> If there are no subshells, this returns the same pointer as <tt>cell_list</tt>.
     */
	CELL *cell() const;

	// Data changing routines

    /**
     * Sets the first cell in a complete enumeration of all cells in the attribute.
     * <br><br>
     * @param cel
     * cell.
     */
	void set_cell(CELL *cel);

    /**
     * Sets a list of supercells in the cell attribute.
     * <br><br>
     * @param scel
     * super cell.
     */
	void set_supercell(SUPERCELL * scel);

    /**
     * Sets the automatic update on or off.
     * <br><br>
     * @param state
     * on or off.
     */
	void set_auto_update(logical state);

	// Split, merge, trans and copy functions
    /**
     * @nodoc
     */
	virtual void split_owner( ENTITY * entity);

    /**
     * @nodoc
     */
	virtual void merge_owner( ENTITY * entity, logical del_owner );

    /**
     * @nodoc
     */
	virtual void trans_owner( SPAtransf const &trans );

    /**
     * @nodoc
     */
	//virtual void copy_owner( ENTITY * copy_entity) { SPAUNUSED(copy_entity) };	// Don't copy these!
    // Implemented to cope with down_copy functionality
    virtual void copy_owner( ENTITY * copy_entity);
	// Standard housekeeping functions

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_CELL, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/** @} */
#define ATTRIB_CELL_CLASS
#endif

