/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Define a color attribute.
//
// usage---
//    This is the same as the color attribute used in the ACIS testbed,
//    but it is copied here to avoid dependencies on the testbed code.
//
//----------------------------------------------------------------------

#ifndef col_attr_hxx
#define col_attr_hxx

// This attribute is a private attribute for Spatial Corp and
// so is derived from its master attribute class.

#include "dcl_rb.h"
#include "at_tsl.hxx"

/**
 * @file col_attr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 * @{
 */
//======================================================================
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_COL, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_RB int ATTRIB_COL_TYPE;
#define ATTRIB_COL_LEVEL (ATTRIB_TSL_LEVEL + 1)

// Declare the class.
/**
 * Stores color information for an <tt>ENTITY</tt>.
 * <br>
 * <b>Role:</b> This class defines the color attribute and stores the color
 * information for an <tt>ENTITY</tt>.
 */
class DECL_RB ATTRIB_COL: public ATTRIB_TSL {
private:
	int colour_data;

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example,
 * <tt>x=new ATTRIB_COL(...)</tt>), because this reserves the memory on the heap, a
 * requirement to support roll back and history management.
 * <br><br>
 * @param ent
 * entity.
 * @param colval
 * color value.
 */
	ATTRIB_COL( ENTITY * ent = NULL, int colval = 0 );
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
/**
 * Returns the <tt>rgb_color</tt> values.
 */
	int colour() const { return colour_data; }
/**
 * Sets the color value.
 * <br><br>
 * @param colval
 * color value.
 */
	void set_colour( int colval);

	// Functions called to aid attribute migration during modelling
	// operations.
/**
 * Notifies the <tt>ATTRIB_COL</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify
 * the attribute. The default action is to do nothing. This function is supplied
 * by the application whenever it defines a new attribute, and is called when a
 * split occurs.
 * <br><br>
 * @param ent
 * new entity.
 */
	virtual void split_owner( ENTITY *ent );
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.
/**
 * Notifies the <tt>ATTRIB_COL</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify
 * the attribute. After the merge, this owner will be deleted if the logical
 * deleting owner is <tt>TRUE</tt>, otherwise it will be retained and other entity will
 * be deleted. The default action is to do nothing. This function is supplied
 * by the application whenever it defines a new attribute, and is called when a
 * merge occurs.
 * <br><br>
 * @param ent
 * new entity.
 * @param del
 * deleting owner.
 */
	virtual void merge_owner( ENTITY *ent, logical del);
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.
/**
 * Virtual function called when an owner entity is being copied.
 * <br><br>
 * @param ent
 * entity being copied.
 */
	virtual void copy_owner( ENTITY *ent );
									// the owner of this attribute is
									// about to be copied - the
									// argument is the copy.

// rollback notification
	/**
	 * Fields notification of a rollback operation.
	 * <br><br>
	 * <b>Role:</b> The first argument specifies the type of bulletin
	 * being rolled back (create, delete or change). In the case of a change notification, the second
	 * argument contains the information to be restored into this object.
	 * <br><br>
	 * @param bulletin_type
	 * type of bulletin being rolled back.
	 * @param ent
	 * info to be restored into this argument.
	 */
	virtual void roll_notify( BULLETIN_TYPE bulletin_type, ENTITY * ent);

	// Declare all the standard attribute functions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_COL, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/** @} */
#endif
