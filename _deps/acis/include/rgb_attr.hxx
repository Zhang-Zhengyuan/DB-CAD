/* ORIGINAL: 3dt2.1/gihusk/rgb_attr.hxx */
// $Id: rgb_attr.hxx,v 1.6 2001/05/11 15:04:14 jenglund Exp $
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
//    Define an RGB color attribute.
//
// usage---
//	Define ATTRIB_RGB to store RGB color information for an ENTITY.
//	This attribute takes precedence over ATTRIB_COL when displaying
//	ENTITIES in the 3D Toolkit.
//
//----------------------------------------------------------------------

#ifndef rgb_attr_hxx
#define rgb_attr_hxx

#include "dcl_ga.h"
#include "at_macro.hxx"
#include "attr_sti.hxx"
#include "rgbcolor.hxx"

//======================================================================
/**
* @file rgb_attr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup GAAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_RGB, GA )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_RGB_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_RGB_LEVEL (ATTRIB_ST_LEVEL + 1)


// Declare the class.
/**
 * Stores rgb color information for an <tt>ENTITY</tt>.
 * <br>
 * <b>Role:</b> This class defines <tt>RGB</tt> color attribute and stores the <tt>RGB</tt> color
 * information for an <tt>ENTITY</tt>. This attribute takes precedence over the
 * <tt>ATTRIB_COL</tt> attribute when displaying entities using the Rendering Base Component.
 * <br><br>
 * @see rgb_color
 */
class DECL_GA ATTRIB_RGB: public ATTRIB_ST {
private:
	rgb_color rgb;

public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_RGB</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and
 * history management.
 */
	ATTRIB_RGB();
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * new operator inherited from the <tt>ENTITY</tt> class (for example,
 * <tt>x=new ATTRIB_RGB(...)</tt>), because this reserves the memory on the heap, a
 * requirement to support roll back and history management.
 * <br><br>
 * @param ent
 * entity.
 * @param rgb
 * rgb color value.
 */
	ATTRIB_RGB( ENTITY *ent, rgb_color rgb);
/**
 * Returns the <tt>rgb_color</tt> values.
 */
	rgb_color color() const { return rgb; }
/**
 * Sets the <tt>rgb_color</tt> values.
 * <br><br>
 * @param rgb
 * rgb color value.
 */
	void set_color( rgb_color rgb);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Functions called to aid attribute migration during modelling
	// operations.

/**
 * Notifies the <tt>ATTRIB_RGB</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the
 * attribute. After the merge, this owner will be deleted if the logical deleting
 * owner is <tt>TRUE</tt>, otherwise it will be retained and other entity will be deleted.
 * The default action is to do nothing. This function is supplied by the
 * application whenever it defines a new attribute, and is called when a merge occurs.
 * <br><br>
 * @param ent
 * given entity.
 * @param owner
 * deleting owner.
 */
	virtual void merge_owner( ENTITY *ent, logical owner);
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.


// rollback notification
#ifdef ACIS_1_41

/**
 * @nodoc
 */
    virtual void roll_notification(BULLETIN_TYPE, const ENTITY*) const;
#else
/**
 * @nodoc
 */
    virtual void roll_notify(BULLETIN_TYPE, ENTITY*);
#endif

	// Declare all the standard attribute functions.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_RGB, GA )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

//======================================================================
 /** @} */
#endif
