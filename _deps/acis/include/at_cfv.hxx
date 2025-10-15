/* ORIGINAL: acis2.1/ct_husk/classes/at_cfv.hxx */
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

#if !defined( ATTRIB_CFACE_VOL_CLASS )
#define ATTRIB_CFACE_VOL_CLASS

#include "dcl_ct.h"
#include "at_ct.hxx"

/**
* @file at_cfv.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

 //STI HDR_INDEP: nvp
class CFACE;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_CFACE_VOL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_CFACE_VOL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_ATTRIB_CFACE_VOL(const ENTITY * ent);

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_CFACE_VOL, CT)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_CFACE_VOL_TYPE;

// tbrv
/**
 * @nodoc
 */
#define ATTRIB_CFACE_VOL_LEVEL (ATTRIB_CT_LEVEL + 1)

 /**
  * Implements an attribute attached to a <tt>CFACE</tt> entity.
  * <br>
  * <b>Role:</b> This class implements an attribute attached to a <tt>CFACE</tt> entity. The <tt>ATTRIB_CFACE_VOL</tt>
  * attribute is used as a base class for volume property attributes placed on <tt>CFACE</tt>s.
  * They participate in <tt>CFACE</tt> volume attribute propagation in <tt>api_ct_propagate_cface_attribs</tt>.
  * This class is intended to be a base class for application developers who need to propagate volume information.
  * <br><br>
  * For additional information, refer to the Cellular Topology Technical Article on <i>Volume Attribute Propagation</i>.
  * @see ATTRIB_VOL_COL
  */

class DECL_CT ATTRIB_CFACE_VOL : public ATTRIB_CT 
{
public:

	// Construct the initial attribute - add up to two cfaces
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
	 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x = new ATTRIB_CFACE_VOL(...)</tt>), because this
	 * reserves the memory on the heap, a requirement to support roll back and history management.
	 * @param face cface.
	 */
	ATTRIB_CFACE_VOL(CFACE * face = NULL);

	/**
	 * @nodoc
	 */
	virtual logical pattern_compatible() const;
	// returns TRUE

	//	Volume attribute propagation member functions
	/**
	 * Indicates if this attribute can be propagated to the specified <tt>CFACE</tt>.
	 * Checks whether the given <tt>CFACE</tt> entity has an attribute of class <tt>ATTRIB_CFACE_VOL</tt>.
	 * If an <tt>ATTRIB_CFACE_VOL</tt> already exists on the given <tt>CFACE</tt>, this method returns
	 * <tt>FALSE</tt>; otherwise it returns <tt>TRUE</tt>. This method is frequently overloaded in derived classes.
	 * @param face given <tt>CFACE</tt>.
	 */
	virtual logical can_propagate(CFACE *face);

	/**
	 * Attaches a new <tt>ATTRIB_CFACE_VOL</tt> to the given <tt>CFACE</tt> entity.
	 * <br><br>
	 * @param face
	 * given <tt>CFACE</tt>.
	 */
	virtual void propagate_to_cface(CFACE * face);

	// Split, merge and copy functions

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
	 * For an <tt>ATTRIB_CFACE_VOL</tt> the default behavior is to attach a new <tt>ATTRIB_CFACE_VOL</tt>
	 * to the new <tt>CFACE</tt> arising from the split. This method is frequently overloaded in derived classes.
	 * @param new_entity the new <tt>CFACE</tt> arising from the split.
	 * @see ATTRIB#set_split_owner_action, get_split_owner_action, split_action, split_attrib
	 */
	virtual void split_owner(ENTITY * new_entity);

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is about to be merged
	 * with a given <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to delete or otherwise modify the attribute.
	 * After the merge, the owner of this attribute will be deleted if the logical argument
	 * <tt>del_owner</tt> is <tt>TRUE</tt>; otherwise, it will be retained and the other owner will
	 * be deleted. The default action is to do nothing. This function is supplied by the
	 * application whenever it defines a new, derived attribute, and is called when a merge occurs.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the
	 * <tt>set_merge_owner_action</tt> method to specify how your attribute should act within
	 * <tt>merge_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * For an <tt>ATTRIB_CFACE_VOL</tt> the default behavior is to move the <tt>ATTRIB_CFACE_VOL</tt>
	 * to the other <tt>CFACE</tt>, if this <tt>CFACE</tt> is being deleted and <tt>can_propagate</tt>
	 * returns <tt>TRUE</tt> for the other <tt>CFACE</tt> involved in the merge.
	 * This method is occasionally overloaded in derived classes.
	 * @param entity the <tt>CFACE</tt> with which this attribute's owner is being merged.
	 * @param del_owner flag that the owner of this attribute will be deleted after the merge.
	 * @see ATTRIB#set_merge_owner_action, get_merge_owner_action, merge_action, merge_attrib
	 */
	virtual void merge_owner(ENTITY * entity, logical del_owner);

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
	 * For an <tt>ATTRIB_CFACE_VOL</tt> the default behavior is to attach a new <tt>ATTRIB_CFACE_VOL</tt>
	 * to the new <tt>CFACE</tt> arising from the copy. This method is frequently overloaded in derived classes.
	 * @param copy_entity the new <tt>CFACE</tt> that is a copy of this attribute's owner.
	 * @see ATTRIB#set_copy_owner_action, get_copy_owner_action, copy_action, copy_attrib
	 */
	virtual void copy_owner(ENTITY * copy_entity);

	/**
	* This member function enables an efficient implementation of
	* @href api_ct_propagate_cface_attribs. It computes a unique hash value for this attribute.
	* The default implementation returns <tt>std::hash<ATTRIB_CFACE_VOL>{}(this)</tt>,
	* i.e., the standard hash value for the <tt>this</tt> pointer. That gives (ideally) different hash values
	* for each attribute. Ideally this function should be overloaded so that the hash value depends
	* only on properties/content of derived attributes. For more information refer to the technical
	* article [Deriving Classes from ATTRIB_CFACE_VOL](https://doc.spatial.com/get_doc_page/articles/d/e/r/Deriving_Classes_from_ATTRIB_CFACE_VOL_f75e.html)
	*/
	virtual size_t hash() const;

	/**
	* This member function enables an efficient implementation of
	* @href api_ct_propagate_cface_attribs. The default implementation returns <tt>true</tt> if two
	* ATTRIB_CFACE_VOL attributes being compared have the same memory address.
	* Ideally this function should be overloaded so that two attributes are the same if they
	* have the same properties/content. For more information refer to the technical
	* article [Deriving Classes from ATTRIB_CFACE_VOL](https://doc.spatial.com/get_doc_page/articles/d/e/r/Deriving_Classes_from_ATTRIB_CFACE_VOL_f75e.html)
	*/
	virtual bool operator==(const ATTRIB_CFACE_VOL& other) const;

	// Standard housekeeping functions

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_CFACE_VOL, CT)
#if 0
		; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/** @} */
#endif

