/* ORIGINAL: acis2.1/ct_husk/classes/at_vcol.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// dal 03Apr03 Remove docs for inherited methods such as copy_owner
// dal 31Mar03 Documentation.
/*******************************************************************/

#if !defined( ATTRIB_VOL_COL_CLASS )
#define ATTRIB_VOL_COL_CLASS

#include "dcl_ct.h"
#include "at_cfv.hxx"

/**
* @file at_vcol.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

 /**
  * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_VOL_COL</tt>.
  * <br><br>
  * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
  * if the entity is a <tt>ATTRIB_VOL_COL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
  * <tt>NULL</tt> pointer.
  * <br><br>
  * <b>Effect:</b> Read-only
  * @param ent entity.
  **/
logical DECL_CT is_ATTRIB_VOL_COL(const ENTITY *ent);

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_VOL_COL, CT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_VOL_COL_TYPE;
#define ATTRIB_VOL_COL_LEVEL (ATTRIB_CFACE_VOL_LEVEL + 1)
/**
 * Assigns a color to a volumetric region; that is, a three-dimensional cell.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_VOL_COL</tt> attribute assigns a color to a three-dimensional cell, and resides on all
 * <tt>CFACE</tt>s of the three-dimensional cell. Instances of <tt>ATTRIB_VOL_COL</tt> participate in <tt>CFACE</tt>
 * volume attribute propagation in <tt>api_ct_propagate_cface_attribs</tt>.
 * This class is provided as an example to demonstrate the propagation of volumetric properties.
 * <br><br>
 * For additional information, refer to the Cellular Topology Technical Article on <i>Volume Attribute Propagation</i>.
 */

class DECL_CT ATTRIB_VOL_COL : public ATTRIB_CFACE_VOL {
private:

	//	Data member that contains the attribute color

	int color_data;

public:

	// Construct the initial attribute - add up to two cfaces
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
	 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x = new ATTRIB_VOL_COL(...)</tt>), because this reserves
	 * the memory on the heap, a requirement to support roll back and history management. Generally,
	 * applications should call the constructor with using the <tt>ACIS_NEW</tt> macro, which calls the
	 * overloaded <tt>new</tt> operator.
	 * @param cface cface.
	 * @param col color.
	 */

	ATTRIB_VOL_COL(CFACE * cface = NULL, int col = 0);

	/**
	 * @nodoc
	 */
	virtual logical pattern_compatible() const override;
	// returns TRUE

   //	Read color data
   /**
	* Returns the color data.
	*/
	int color() const { return color_data; }

	//	Set color data
	/**
	 * Sets the color data.
	 * @param col color value.
	 */
	void set_color(int col);

	//	ATTRIB_CFACE_VOL propagation functions

	/**
	 * Indicates if this attribute can be propagated to the specified <tt>CFACE</tt>.
	 * Checks whether the given <tt>CFACE</tt> entity has an attribute of class <tt>ATTRIB_VOL_COL</tt>.
	 * If an <tt>ATTRIB_VOL_COL</tt> already exists on the given <tt>CFACE</tt> and it has the same
	 * color value as this attribute, then this method returns <tt>FALSE</tt>; otherwise it returns <tt>TRUE</tt>.
	 * @param cface given <tt>CFACE</tt>.
	 */
	virtual logical can_propagate(CFACE * cface) override;

	/**
	 * Attaches a new <tt>ATTRIB_VOL_COL</tt> to the given <tt>CFACE</tt> entity.
	 * @param cface given <tt>CFACE</tt>.
	 */
	virtual void propagate_to_cface(CFACE * cface) override;

	//	Attribute split and copy functions...use parent's merge

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owner is about to be split into two parts.
	 * <br><br>
	 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the
	 * attribute. This function is supplied by the application whenever it derives a new attribute,
	 * and is called when a split occurs.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the
	 * <tt>set_split_owner_action</tt> method to specify how your attribute should act within
	 * <tt>split_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * For an <tt>ATTRIB_VOL_COL</tt> the default behavior is to attach a new <tt>ATTRIB_VOL_COL</tt>
	 * with this attribute's color value to the new <tt>CFACE</tt> arising from the split.
	 * @param new_entity the new <tt>CFACE</tt> arising from the split.
	 * @see ATTRIB#set_split_owner_action, get_split_owner_action, split_action, split_attrib
	 */
	virtual void split_owner(ENTITY * new_entity) override;

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> has been copied.
	 * <br><br>
	 * <b>Role:</b> This method is used to notify this attribute that it's owner has been copied.
	 * <br><br>
	 * The predominant fashion by which <tt>copy_owner</tt> is called is during the course of ACIS
	 * operations that are constructing, deconstructing, or modifying either topology and/or
	 * geometry. This method is also called at the end of the @href api_down_copy_entity algorithm.
	 * As a general rule, the attribute implementing this method cannot assume the validity
	 * of the model "above" or "below" its owner, only that its owner is valid and that the
	 * <tt>copy_ent</tt> is valid.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the
	 * <tt>set_copy_owner_action</tt> method to specify how your attribute should act within
	 * <tt>copy_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * For an <tt>ATTRIB_VOL_COL</tt> the default behavior is to attach a new <tt>ATTRIB_VOL_COL</tt>
	 * with this attribute's color value to the new <tt>CFACE</tt> arising from the copy.
	 * @param copy_entity the new <tt>CFACE</tt> that is a copy of this attribute's owner.
	 * @see ATTRIB#set_copy_owner_action, get_copy_owner_action, copy_action, copy_attrib
	 */
	virtual void copy_owner(ENTITY * copy_entity) override;

	/**
	* Override the default implementation so that the returned hash value only depends on
	* the color of this ATTRIB_VOL_COL attribute.
	*/
	virtual size_t hash() const override;

	/**
	* Override the default implementation so that two ATTRIB_VOL_COL attributes are
	* same if their color is same.
	*/
	virtual bool operator==(const ATTRIB_CFACE_VOL&) const override;

	// Standard housekeeping functions
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_VOL_COL, CT)
#if 0
		; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/** @} */
#endif

