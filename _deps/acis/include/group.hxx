/* ORIGINAL: acis2.1/ct_husk/classes/group.hxx */
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
// amt 02Nov02 Changing GROUP to SPAGROUP.
// jdh 06Nov02 Adding BASE_COMPAT for SPAGROUP and ATTRIB_SPAGROUP
/*******************************************************************/

// Header for SPAGROUP.

// A group is a special entity class that points to a list of entities.
// It may point to entities on more that one body.  Each entity it
// points to is required to have a back pointer through an ATTRIB_SPAGROUP.
// Groups are useful for representing special relationships between
// entities.


#if !defined( SPAGROUP_CLASS )
#define SPAGROUP_CLASS

#if defined(__cplusplus) && defined(BASE_COMPAT)

/*
// This is a temporary directive that allows
// a transition period to the new type names.
//
// ATTRIB_SPAGROUP is declared in ct_husk/classes/at_grp.hxx
// which includes this header. We can therefore define both here.
*/

#define GROUP			SPAGROUP

// Fang. ATTRIB_SPAGROUP is being replaced by ATTRIB_SPACOLLECTION.
#define ATTRIB_GROUP	ATTRIB_SPACOLLECTION
//#define ATTRIB_GROUP	ATTRIB_SPAGROUP

#endif /* BASE_COMPAT and __cplusplus */


#include "dcl_ct.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
#include "lists.hxx"
#include "collection.hxx"

/**
* @file group.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */
class SPAbox;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SPAGROUP</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>SPAGROUP</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_SPAGROUP( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SPAGROUP, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_CT int SPAGROUP_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY
// Fang. SPAGROUP is now derived from SPACOLLECTION, so up the level.
#define SPAGROUP_LEVEL 2
//#define SPAGROUP_LEVEL 1


// SPAGROUP declaration proper.
/**
 * An implementation of a SPACOLLECTION with predefined actions and behaviors.
 * <br>
 * <b>Role:</b> A <tt>SPAGROUP</tt> is a an implementation of a <tt>SPACOLLECTION</tt>. 
 * Like a <tt>SPACOLLECTION</tt>, a <tt>SPAGROUP</tt> is an <tt>ENTITY</tt> designed to 
 * store a list of entities. However, a <tt>SPAGROUP</tt> has slightly different 
 * <i>behaviors</i> and <i>actions</i> when its members are modeled upon, copied, 
 * deleted, etc.
 * <br><br>
 * The specific actions, behaviors, and "ables" used by <tt>SPAGROUP</tt>:<br>
 * <br>
 * <b>Actions:</b><br>
 * split_member_action:  SplitMemberAdd<br>
 * merge_member_action:  MergeMemberAdd<br>
 * copy_member_action:  CopyMemberAdd<br>
 * trans_member_action:  TransMemberKeep<br>
 * replace_member_action:  ReplaceMemberIgnore<br>
 * tolerant_member_action:  TolerantMemberSwap<br>
 * geomchanged_member_action:  GeomChangedMemberKeep<br>
 * <br>
 * <b>Behaviors:</b><br>
 * collection_save_behavior:  SaveAll<br>
 * collection_copy_behavior:  CopyPartial<br>
 * collection_empty_behavior:  EmptyRemove<br>
 * <br>
 * <b>"Ables":</b><br>
 * savable:  TRUE<br>
 * copyable:  TRUE<br>
 * <br>
 * The <tt>SPAGROUP</tt>'s default behaviors require the application to consider its workflow 
 * when saving, copying, and deleting the <tt>SPAGROUP</tt>. Failure to take these behaviors 
 * into consideration will lead to memory/resource leaks.  Consider the following example: 
 * create two independent bodies and put both into a <tt>SPAGROUP</tt>. The following use-cases 
 * apply:
 * <UL>
 * <LI>If you save the <tt>SPAGROUP</tt> to SAT, the <tt>SPAGROUP</tt> and it's two bodies will
 * be saved. You could call this a "strong save", as the <tt>SPAGROUP</tt> pulls its 
 * members into the save.<br>
 * <LI>If you copy the <tt>SPAGROUP</tt> using <tt>api_copy_entity</tt>, you'll get a new 
 * <tt>SPAGROUP</tt> with no members. You could call this a "weak copy", as the <tt>SPAGROUP</tt>
 * does not pull its members into the copy.<br>
 * <LI>If you delete the <tt>SPAGROUP</tt> using <tt>api_del_entity</tt>, the <tt>SPAGROUP</tt> 
 * will be deleted but its two members will not be deleted and will be left dangling in history. 
 * You could call this a "weak delete", as the <tt>SPAGROUP</tt> does not pull its members into 
 * the delete.<br>
 * </UL>
 * Please refer to the "SPACOLLECTION Overview" Technical Article for a detailed explanation 
 * of each action, behavior, and "able", as well as numerous other use-cases.
 * <br><br>
 * @see SPACOLLECTION, is_SPAGROUP, api_ct_add_to_group, api_ct_remove_from_group, 
 * api_ct_lose_group, api_ct_return_groups, api_ct_make_group, split_member_action, 
 * merge_member_action, copy_member_action, trans_member_action, replace_member_action, 
 * tolerant_member_action, geomchanged_member_action, collection_save_behavior, 
 * collection_copy_behavior, collection_empty_behavior
 */
class DECL_CT SPAGROUP: public SPACOLLECTION {

	// Pointer to a geometric bounding region (a SPAbox), within which the
	// entire group lies (with respect to the internal coordinate
	// system of its body).
	// It may be NULL if no such bound has been calculated since the
	// group was last changed.
	SPAbox *bound_ptr;

	// Init group behaviors.
	void set_default_group();

protected:

	void kill_bound() { bound_ptr = NULL; }

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( SPAGROUP, CT )
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

#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Now the functions specific to SPAGROUP.

	// Basic constructor, used internally for bulletin board activity.

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should call
	 * this constructor only with the overloaded <tt>new</tt> operator inherited from the <tt>ENTITY</tt>
	 * class (for example, <tt>x=new SPAGROUP</tt>), because this reserves the memory on the heap, a
	 * requirement to support roll back and history management.
	 */
	SPAGROUP();

	//	Other constructors
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
	 * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new SPAGROUP(...)</tt>),
	 * because this reserves the memory on the heap, a requirement to support roll back and history
	 * management.
	 * <br><br>
	 * @param ent
	 * entity.
	 */
	SPAGROUP(ENTITY* ent);

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
	 * operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new SPAGROUP(...)</tt>),
	 * because this reserves the memory on the heap, a requirement to support roll back and history
	 * management.
	 * <br><br>
	 * @param entlist
	 * list of entities.
	 */
	 SPAGROUP(ENTITY_LIST& entlist);

	// Data reading routines.

	/**
	 * Returns the pointer to the bounding box.
	 */
	SPAbox *bound() const { return bound_ptr; }

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.

	/**
	 * Sets the bounding box for a group.
	 * <br><br>
	 * @param b_box
	 * bounding box.
	 */
	void set_bound( SPAbox * b_box );
	
	/**
	* @nodoc
	*/
	void add( ENTITY * ent);
	/**
	* @nodoc
	*/
	void remove( ENTITY * );

	/**
	 * Returns the number of entities in the collection list, minus the use count.
	 */
	int tombstones() const { return collection_list.count() - collection_list.iteration_count(); }

	// virtual void ENTITY::add() is hidden by add(ENTITY*).
	// Expose it again by defining a pass thru here.
	/**
	 * @nodoc
	 */
	void add() { ENTITY::add(); }

	// virtual void ENTITY::remove() is hidden by remove(ENTITY*).
	// Expose it again by defining a pass thru here.
	/**
	 * @nodoc
	 */
	void remove(logical i) { ENTITY::remove(i); }
	//STIBUGFIX let (r36011): added deletable method
	// A SPAGROUP should not be deleted when found in the copy scan list.
	// It should only be deleted when its use count drops to zero.
	/**
	 * @nodoc
	 */
	virtual logical deletable() const;
	//STI let: end

};

#endif
