/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for SPACOLLECTION.

// A collection is a special entity class that points to a list of entities.
// It may point to entities on more that one body.  Each entity it
// points to is required to have a back pointer through an ATTRIB_SPACOLLECTION.
// collections are useful for representing special relationships between
// entities.


#if !defined( SPACOLLECTION_CLASS )
#define SPACOLLECTION_CLASS


#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
#include "lists.hxx"
#include "entity.hxx"

/**
* @file collection.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISLISTS
 *
 * @{
 */

/**
* Text specifies what to do with members involved in a split.
* @param SplitMemberAdd
* Both entities are in the collection after a split.
* @param SplitMemberRemove
* Both entities are not in the collection after a split.
* @param SplitMemberLose
* Un-collect the entities in collection and delete the collection.
* @param SplitMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	SplitMemberAdd,	
	SplitMemberRemove,	
	SplitMemberLose,
	SplitMemberIgnore
} split_member_action;

/**
* Text specifies what to do with members involved in a merge.
* @param MergeMemberAdd
* The merged entity is in the collection after a merge.
* @param MergeMemberRemove
* The merged entity is not in the collection after a merge.
* @param MergeMemberLose
* Un-collect the entities in collection and delete the collection.
* @param MergeMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	MergeMemberAdd,
	MergeMemberRemove,
	MergeMemberLose,
	MergeMemberIgnore
} merge_member_action;

/**
* Text specifies what to do with members involved in a copy.
* @param CopyMemberKeep
* Keep the original entity in collection. 
* The copy of the entity will not be added to the collection. (same as CopyMemberIgnore).
* @param CopyMemberAdd
* The copy of the entity is added to the collection after a copy.
* @param CopyMemberRemove
* The entity being copied is removed from the collection after a copy.
* @param CopyMemberSwap
* The entity being copied is removed from the collection after a copy.
* The copy of the entity is added to the collection after a copy.
* @param CopyMemberLose
* Un-collect the entities in collection and delete the collection.
* @param CopyMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	CopyMemberKeep,
	CopyMemberAdd,
	CopyMemberRemove,
	CopyMemberSwap,
	CopyMemberLose,
	CopyMemberIgnore
} copy_member_action;

/**
* Text specifies what to do with members involved in a transform.
* @param TransMemberKeep. (same as TransMemberIgnore)
* The transformd entity is in the collection after a transform.
* @param TransMemberRemove
* The transformd entity is not in the collection after a transform.
* @param TransMemberLose
* Un-collect the entities in collection and delete the collection.
* @param TransMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	TransMemberKeep,
	TransMemberRemove,
	TransMemberLose,
	TransMemberIgnore
} trans_member_action;

/**
* Text specifies what to do with members involved in a replace.
* @param ReplaceMemberSwap
* The replaced entity is in the collection after a replace.
* The original entity is not in the collection after a replace.
* @param ReplaceMemberRemove
* The replaced entity and original entity are not in the collection after a replace.
* @param ReplaceMemberLose
* Un-collect the entities in collection and delete the collection.
* @param ReplaceMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	ReplaceMemberSwap,
	ReplaceMemberRemove,
	ReplaceMemberLose,
	ReplaceMemberIgnore
} replace_member_action;

/**
* Text specifies what to do with members involved in a from/to tolerant operation.
* @param TolerantMemberSwap
* The replaced entity is in the collection after a from/to tolerant operation.
* The original entity is not in the collection after a from/to tolerant operation.
* @param TolerantMemberRemove
* The replaced entity and original entity are not in the collection after a from/to tolerant operation.
* @param TolerantMemberLose
* Un-collect the entities in collection and delete the collection.
* @param TolerantMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	TolerantMemberSwap,
	TolerantMemberRemove,
	TolerantMemberLose,
	TolerantMemberIgnore
} tolerant_member_action;

/**
* Text specifies what to do with members involved in a geometry change.
* @param GeomChangedMemberKeep
* The geometry changed entity is in the collection after a geometry change.
* @param GeomChangedMemberRemove
* The geometry changed entity is not in the collection after a geometry change.
* @param GeomChangedMemberLose
* Un-collect the entities in collection and delete the collection.
* @param GeomChangedMemberIgnore
* Do nothing with the collection
**/
typedef enum 
{
	GeomChangedMemberKeep,
	GeomChangedMemberRemove,
	GeomChangedMemberLose,
	GeomChangedMemberIgnore
} geomchanged_member_action;


/**
* Text specifies what to do with collection and members in the save process
* @param SavePartial
* Don't save across the collection
* @param SaveAll
* Save across the collection
**/
typedef enum
{
	SavePartial,
	SaveAll
} collection_save_behavior;

/**
* Text specifies what to do with collection and members in the copy process
* @param CopyPartial
* Don't copy across the collection
* @param CopyAll
* Copy across the collection
**/
typedef enum
{
	CopyPartial,
	CopyAll
} collection_copy_behavior;

/**
* Text specifies what to do with collection when the collection is empty
* @param EmptyKeep
* Keep the empty collection
* @param EmptyRemove
* Remove the empty collection
**/
typedef enum
{
	EmptyKeep,
	EmptyRemove
} collection_empty_behavior;


/**
 * @nodoc
 */
const copy_member_action col_default_copy_action = CopyMemberKeep;
/**
 * @nodoc
 */
const merge_member_action col_default_merge_action = MergeMemberAdd;
/**
 * @nodoc
 */
const split_member_action col_default_split_action = SplitMemberAdd;
/**
 * @nodoc
 */
const trans_member_action col_default_trans_action = TransMemberKeep;
/**
 * @nodoc
 */
const replace_member_action col_default_replace_action = ReplaceMemberSwap;
/**
 * @nodoc
 */
const tolerant_member_action col_default_tolerant_action = TolerantMemberSwap;
/**
 * @nodoc
 */
const geomchanged_member_action col_default_geomchanged_action = GeomChangedMemberKeep;
/**
 * @nodoc
 */
const unsigned int num_col_copy_action_bits = 3;
/**
 * @nodoc
 */
const unsigned int num_col_merge_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_col_split_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_col_trans_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_col_replace_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_col_tolerant_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_col_geomchanged_action_bits = 2;
/**
 * @nodoc
 */
const collection_copy_behavior col_default_copy_behavior = CopyAll;
/**
 * @nodoc
 */
const collection_save_behavior col_default_save_behavior = SaveAll;
/**
 * @nodoc
 */
const collection_empty_behavior col_default_empty_behavior = EmptyKeep;
/**
 * @nodoc
 */
const unsigned int num_col_copy_behavior_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_col_save_behavior_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_col_empty_behavior_bits = 1;
/**
 * @nodoc
 */
const logical col_default_copyable = TRUE;
/**
 * @nodoc
 */
const logical col_default_savable = TRUE;

/**
 * @nodoc
 **/
typedef struct
{
	// actions
	unsigned int SplitAction	: num_col_split_action_bits;
	unsigned int MergeAction	: num_col_merge_action_bits;
	unsigned int CopyAction		: num_col_copy_action_bits;
	unsigned int TransAction	: num_col_trans_action_bits;
	unsigned int ReplaceAction	: num_col_replace_action_bits;
	unsigned int TolerantAction : num_col_tolerant_action_bits;
	unsigned int GeomChangedAction	: num_col_geomchanged_action_bits;

	// behaviors
	unsigned int SaveBehavior	: num_col_save_behavior_bits;
	unsigned int CopyBehavior	: num_col_copy_behavior_bits;
	unsigned int EmptyBehavior	: num_col_empty_behavior_bits;

	// ables
	unsigned int Savable		: 1;
	unsigned int Copyable		: 1;
} collection_behaviors_t;
/** @} */
/**
 * \addtogroup ACISQUERIES
 * @{
 */
/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SPACOLLECTION</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>SPACOLLECTION</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_KERN is_SPACOLLECTION( const ENTITY *ent );
/** @} */
/**
 * \addtogroup ACISLISTS
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SPACOLLECTION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// tbrv
/**
 * @nodoc
 */
extern DECL_KERN int SPACOLLECTION_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define SPACOLLECTION_LEVEL 1


// SPACOLLECTION declaration proper.
/**
 * An ENTITY that contains and manages a collection of entities.
 * <br>
 * <b>Role:</b> A <tt>SPACOLLECTION</tt> is an <tt>ENTITY</tt> that can contain and 
 * manage a collection of entities. The members of the <tt>SPACOLLECTION</tt> may be 
 * any sort of <tt>ENTITY</tt>, and may be from from different bodies. Note that the 
 * members of the <tt>SPACOLLECTION</tt> should, however, all reside in the same 
 * history stream as the <tt>SPACOLLECTION</tt>.
 * <br><br>
 * A <tt>SPACOLLECTION</tt> provides application-definable <i>behaviors</i> and 
 * <i>actions</i> when its members are modeled upon, copied, deleted, etc. Applications
 * can also derive from <tt>SPACOLLECTION</tt> and overide the notification functions 
 * to specify the behaviors that satisfy their specific application needs.
 * <br><br>
 * The default actions, behaviors, and "ables" used by <tt>SPACOLLECTION</tt>:<br>
 * <br>
 * <b>Actions:</b><br>
 * split_member_action:  SplitMemberAdd<br>
 * merge_member_action:  MergeMemberAdd<br>
 * copy_member_action:  CopyMemberKeep<br>
 * trans_member_action:  TransMemberKeep<br>
 * replace_member_action:  ReplaceMemberSwap<br>
 * tolerant_member_action:  TolerantMemberSwap<br>
 * geomchanged_member_action:  GeomChangedMemberKeep<br>
 * <br>
 * <b>Behaviors:</b><br>
 * collection_save_behavior:  SaveAll<br>
 * collection_copy_behavior:  CopyAll<br>
 * collection_empty_behavior:  EmptyKeep<br>
 * <br>
 * <b>"Ables":</b><br>
 * savable:  TRUE<br>
 * copyable:  TRUE<br>
 * <br>
 * Iterating the members of a <tt>SPACOLLECTION</tt> is similar to iterating an 
 * <tt>ENTITY_LIST</tt>. The <tt>init</tt> and <tt>next</tt> access functions are 
 * used for traversing the member list of entities.
 * <br><br>
 * Please refer to the "SPACOLLECTION Overview" Technical Article for a detailed 
 * explanation of each action, behavior, and "able", as well as numerous use-cases.
 * <br><br>
 * @see is_SPACOLLECTION, api_add_to_collection, api_delete_collection_entities, 
 * api_remove_from_collection, api_return_collection_ents, api_return_collections, 
 * split_member_action, merge_member_action, copy_member_action, trans_member_action, 
 * replace_member_action, tolerant_member_action, geomchanged_member_action, 
 * collection_save_behavior, collection_copy_behavior, collection_empty_behavior
 */
class DECL_KERN SPACOLLECTION : public ENTITY {

	// The following friend class will access the private data member collection_list 
	// directly by design.
	friend class ATTRIB_SPACOLLECTION;
	friend class SPAGROUP;

	// Embedded entity list that contains all of the collection's entities.
	ENTITY_LIST collection_list;

	// predefined collection behaviors
	collection_behaviors_t collection_behaviors;


	// set the default behaviors of a collection. Used in collection constructor
	/**
	 * @nodoc
	 */
	void set_defaults();

protected:

	/**
	 * @nodoc
	 */
	ENTITY_LIST &get_collection_list() {return collection_list;}

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( SPACOLLECTION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
	
	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
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

public:

	// Now the functions specific to SPACOLLECTION.

	// Basic constructor, used internally for bulletin board activity.

	/**
	 * Constructs an empty collection. 
	 */
	SPACOLLECTION();

	//	Other constructors
	/**
	 * Constructs a collection and adds an entity to it.
	 * <br><br>
	 * @param ent
	 * entity to add to the collection.
	 */
	SPACOLLECTION(ENTITY* ent);

	/**
	 * Constructs a collection and adds a list of entities to it.
	 * <br><br>
	 * @param entlist
	 * list of entities to add to the collection.
	 */
	SPACOLLECTION(ENTITY_LIST& entlist);

	// set collection "ables"
	/**
	 * Specify if the collection is savable or not. Returns the value set.
	 * @param b
	 * logical indicates is savable or not
	 */
	logical set_savable( logical b);
	/**
	 * Specify if the collection is copyable or not. Returns the value set.
	 * @param b
	 * logical indicates is copyable or not
	 */
	logical set_copyable( logical b);

	// get/set collection actions
	/**
	 * Get the split action for a collection.
	 */
	split_member_action get_split_member_action( void ) const;
	/**
	 * Set the split action for a collection. Returns the value set.
	 * @param a
	 * split_member_action
	 */
	split_member_action set_split_member_action( split_member_action a );

	/**
	 * Get the merge action for a collection.
	 */
	merge_member_action get_merge_member_action( void ) const;
	/**
	 * Set the merge action for a collection. Returns the value set.
	 * @param a
	 * merge_member_action
	 */
	merge_member_action set_merge_member_action( merge_member_action a );

	/**
	 * Get the trans action for a collection.
	 */
	trans_member_action get_trans_member_action( void ) const;
	/**
	 * Set the trans action for a collection. Returns the value set.
	 * @param a
	 * trans_member_action
	 */
	trans_member_action set_trans_member_action( trans_member_action a );

	/**
	 * Get the copy action for a collection.
	 */
	copy_member_action get_copy_member_action( void ) const;
	/**
	 * Set the copy action for a collection. Returns the value set.
	 * @param a
	 * copy_member_action
	 */
	copy_member_action set_copy_member_action( copy_member_action a );

	/**
	 * Get the replace action for a collection.
	 */
	replace_member_action get_replace_member_action( void ) const;
	/**
	 * Set the replace action for a collection. Returns the value set.
	 * @param a
	 * replace_member_action
	 */
	replace_member_action set_replace_member_action( replace_member_action a );

	/**
	 * Get the tolerant action for a collection.
	 */
	tolerant_member_action get_tolerant_member_action( void ) const;
	/**
	 * Set the tolerant action for a collection. Returns the value set.
	 * @param a
	 * tolerant_member_action
	 */
	tolerant_member_action set_tolerant_member_action( tolerant_member_action a );

	/**
	 * Get the geometry changed action for a collection.
	 */
	geomchanged_member_action get_geomchanged_member_action( void ) const;
	/**
	 * Set the geometry changed action for a collection. Returns the value set.
	 * @param a
	 * geomchanged_member_action
	 */
	geomchanged_member_action set_geomchanged_member_action( geomchanged_member_action a );

	// get/set behaviors
	/**
	 * Get the save behavior for a collection.
	 */
	collection_save_behavior get_collection_save_behavior( void ) const;
	/**
	 * Set the save behavior for a collection. Returns the value set.
	 * @param b
	 * collection_save_behavior
	 */
	collection_save_behavior set_collection_save_behavior( collection_save_behavior b );

	/**
	 * Get the copy behavior for a collection.
	 */
	collection_copy_behavior get_collection_copy_behavior( void ) const;
	/**
	 * Set the copy behavior for a collection. Returns the value set.
	 * @param b
	 * collection_copy_behavior
	 */
	collection_copy_behavior set_collection_copy_behavior( collection_copy_behavior );

	/**
	 * Get the empty behavior for a collection.
	 */
	collection_empty_behavior get_collection_empty_behavior( void ) const;
	/**
	 * Set the empty behavior for a collection. Returns the value set.
	 * @param b
	 * collection_empty_behavior
	 */
	collection_empty_behavior set_collection_empty_behavior( collection_empty_behavior );

	// notification functions

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member has been copied.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with 
	 * collection. The default action is to keep the copied entity in the collection.
	 * If an application derives a new collection and the predefined 
	 * <tt>copy_member_actions</tt> don't satisfy the application needs, this method 
	 * should be implemented by the application in its derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that has been copied.
	 * @param copy_ent
	 * the new <tt>ENTITY</tt> that is a copy of the member <tt>ENTITY</tt>.
	 * @see copy_member_action, SPACOLLECTION#set_copy_member_action, SPACOLLECTION#get_copy_member_action
	 */
	virtual void copy_member( ENTITY *ent, ENTITY *copy_ent );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member is about to be transformed.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with collection.
	 * The default action is to keep the transformed entity in the collection.
	 * If an application derives a new collection and the predefined <tt>trans_member_action</tt>'s don't 
	 * satisfy the application needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is about to be transformed
	 * @param transf
	 * the transformation being applied.
	 * @see trans_member_action, SPACOLLECTION#set_trans_member_action, SPACOLLECTION#get_trans_member_action
	 */
	virtual void trans_member( ENTITY *ent, const SPAtransf& transf );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member has been split into two parts.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection.
	 * If an application derives a new collection and the predefined <tt>split_member_actions</tt> don't 
	 * satisfy the application needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is splitted
	 * @param new_entity
	 * the new entity arising from the split.
	 * @see split_member_action, SPACOLLECTION#set_split_member_action, SPACOLLECTION#get_split_member_action
	 */
	virtual void split_member( ENTITY *ent, ENTITY *new_ent );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt> is about to be merged 
	 * with a given <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection.
	 * After the merge, the member of this collection will be deleted if the logical argument
	 * <tt>del_member</tt> is <tt>TRUE</tt>; otherwise, it will be retained and the other entity will
	 * be deleted. The default action is to keep the merged entity in the collection. 
	 * If an application derives a new collection and the predefined <tt>merge_member_actions</tt> don't 
	 * satisfy the application needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is merged
	 * @param other_ent
	 * the ENTITY with which collectin's member entity is being merged.
	 * @param del_member
	 * flag that the member of this collection will be deleted as part of the merge process.
	 * @see merge_member_action, SPACOLLECTION#set_merge_member_action, SPACOLLECTION#get_merge_member_action
	 */
	virtual void merge_member( ENTITY *ent, ENTITY *other_ent, logical del_member );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt> is about to be replaced 
	 * by a given <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection
	 * After the replace, the member of this collection will be replaced if the logical argument
	 * <tt>replace_member</tt> is <tt>TRUE</tt>; otherwise, it will be retained and the other entity will
	 * be replaced. The default action is to keep the replaced entity in the collection. 
	 * If an application derives a new collection and the predefined <tt>replace_member_actions</tt> don't 
	 * satisfy the application needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is replaced
	 * @param other_ent
	 * the ENTITY by which collectin's member entity is being replaced.
	 * @param del_member
	 * flag that the member of this collection will be replaced
	 * @see replace_member_action, SPACOLLECTION#set_replace_member_action, SPACOLLECTION#get_replace_member_action
	 */
	virtual void replace_member( ENTITY *ent, ENTITY *other_ent, logical replace_member );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt>'s geometry changed during a
	 * modeling operation.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection when the member
	 * entity's geometry changed during a modeling operation. The default action is to keep the entity in the collection.
	 * If an application derives a new collection and the predefined <tt>geomchanged_member_actions</tt> don't 
	 * satisfy the application needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> whose geometry changed
	 * @see geomchanged_member_action, SPACOLLECTION#set_geomchanged_member_action, SPACOLLECTION#get_geomchanged_member_action
	 */
	virtual void geomchanged_member( ENTITY *ent );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt> is removed from the collection.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection when the member
	 * entity is removed from the collection. The default action is to do nothing.
	 * If the default action doesn't meet the application's needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is removed from the collection
	 */
	virtual void remove_member( ENTITY *ent );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt> is added to the collection.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to determine what to do with the collection when the member
	 * entity is added to the collection. The default action is to do nothing.
	 * If the default action doesn't meet the application's needs, this method should be implemented by the application in its 
	 * derived collection.
	 * <br><br>
	 * @param ent
	 * the member <tt>ENTITY</tt> that is added to the collection
	 */
	virtual void add_member( ENTITY *ent );

	/**
	 * Notifies this <tt>SPACOLLECTION</tt> that a member <tt>ENTITY</tt> is replaced by a tolerant <tt>ENTITY</tt> if the <tt>ENTITY</tt>
	 * is a non-tolerant <tt>ENTITY</tt>, or is replace by a non-tolerant <tt>ENTITY</tt> if the <tt>ENTITY</tt> is a tolerant <tt>ENTITY</tt>.
	 * The default action is to let the replaced entity in the collection.
	 * @param ent
	 * the member <tt>ENTITY</tt> that is being replaced.
	 * @param other_ent
	 * the <tt>ENTITY</tt> that will replace the current member <tt>ENTITY</tt>
	 */
	virtual void tolerant_member( ENTITY *ent, ENTITY *other_ent );

	// Data reading routines.

	/**
	 * Lookup the specified ENTITY in the collection and return its index. 
	 * <br><br>
	 * @param ent
	 * entity
	 */
	int lookup(ENTITY const *ent) const {return collection_list.lookup( ent );}

	/**
	 * Returns the number of entities in the collection including the tombstones.
	 */
	int count() const { return collection_list.count(); }

	/**
	 * Returns the number of entities in the collection excluding the tombstones.
	 */
	int iteration_count() const { return collection_list.iteration_count(); }

	/**
	 * Initializes an iteration over the collection using <tt>next</tt>.
	 */
	void init() const { collection_list.init(); }

	/**
	 * Returns the next undeleted (live) entity in the collection.
	 */
	ENTITY *next() const { return collection_list.next(); }

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.

	/**
	 * Adds an entity to the collection.
	 * <br><br>
	 * @param ent
	 * entity.
	 */
	void add_ent( ENTITY * ent);
	
	/**
	 * Removes an entity from the collection.
	 * <br><br>
	 * @params ent
	 * entity
	 */
	void remove_ent( ENTITY * );

	/**
	 * Indicates whether this collection should be included when the member entity is saved.
	 */
	virtual logical savable() const;

	/**
	 * Indicates whether this collection should be included when the member entity is copied.
	 */
	virtual logical copyable() const;

	/**
	 * Removes the tombstones in the collection and returns a collection of live entities
	 */
	void rehash();

	/**
	 * Returns the collection list specified by the given number.
	 */
	ENTITY *operator[]( int i ) const { return collection_list[i]; }

	/**
	 * Remove the entities in the collection
	 */
	void clear();

	/**
	 * @nodoc
	 */
	int byte_count() const {return collection_list.byte_count(FALSE); };

	/**
	 * Returns the next non-deleted entity after the index given without affecting the 
	 * member variables used by init and next.
	 * @params ind
	 * index
	 */
	ENTITY *next_from( int &ind ) { return collection_list.next_from( ind ); } 
};
/** @} */
#endif
