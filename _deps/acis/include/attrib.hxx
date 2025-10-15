/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for attrib.
/*******************************************************************/
// User attribute class definition. This will be the base class
// for the user to derive a range of useful types.

// Every entity may have one or more attributes.  These hang off
// the attribute pointer of an entity, and are arranged as a
// doubly-linked chain.

// Specific attributes holding data and/or pointers are set up as
// derived classes of class ATTRIB.  To avoid clashes of attribute
// names, sub-attributes will be allocated centrally for major
// users and the like.  At present there is an ATTRIB_TSL and an
// ATTRIB_SYS.  Specific attributes such as density, colour,
// dimensions or whatever will be held as sub-sub-attributes or
// at yet more remote levels of derivation.
#ifndef ATTRIB_CLASS
#define ATTRIB_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "at_macro.hxx"
/**
 * \defgroup ACISATTRIBUTES Attributes
 * \ingroup KERNAPI
 *
 */
/**
 * @file attrib.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */
class SPAtransf;
class BODY;
class law;
class ENTITY_LIST;
/**
 * \enum copy_action
 * ATTRIB actions for a copy_owner notification.
 * <br><br>
 * @param CopyLose
 * Lose the attribute if its owner is copied.
 * @param CopyKeep
 * Do nothing if the owner is copied.
 * @param CopyCopy
 * Create a new instance of the attribute on the new owner (using ATTRIB::duplicate()).
 * @param CopyCustom
 * Reserved for Generic Attributes. Call application supplied routine.
 */
typedef enum   
{
	CopyLose = 0,			// lose attribute
	CopyKeep = 1,			// keep attribute on old, do not add to new
	CopyCopy = 2,			// Copy the attribute to the new entity
	CopyCustom = 3			// Reserved for Generic Attributes.
} copy_action;

/**
 * \enum merge_action
 * ATTRIB actions for a merge_owner notification.
 * <br><br>
 * @param MergeLose
 * Lose the attribute if its owner is merged.
 * @param MergeKeepKept
 * Do nothing if owner is merged.
 * @param MergeKeepLost
 * Transfer attribute from entity being lost to entity being kept.
 * @param MergeKeepOne
 * Reserved for Generic Attributes. Transfer from entity being lost
 * to entity being kept if none on entity being kept.
 * Lose any on kept.
 * @param MergeKeepAll
 * Move non-duplicate attribute from entity being lost to entity being kept.
 * @param MergeCustom
 * Reserved for Generic Attributes. Call application supplied routine.
 */
typedef enum 
{
	MergeLose = 0,			// lose attr on kept-ent
	MergeKeepKept = 1,		// keep attr on kept-ent
	MergeKeepLost = 2,		// transfer from lost to kept. Lose any on kept.
	MergeKeepOne= 3,		// Reserved for Generic Attributes.
	MergeKeepAll = 4,		// move attr from go-ent to kept-ent
	MergeCustom = 5			// Reserved for Generic Attributes.
} merge_action;

/**
 * \enum split_action
 * ATTRIB actions for a split_owner notification.
 * <br><br>
 * @param SplitLose
 * Lose the attribute if its owner is split.
 * @param SplitKeep
 * Do nothing if owner is split.
 * @param SplitCopy
 * Create a new instance of the attribute on the new entity (using ATTRIB::duplicate()).
 * @param SplitCustom
 * Reserved for Generic Attributes. Call application supplied routine.
 */
typedef enum 
{
	SplitLose = 0,			// lose attribute
	SplitKeep = 1,			// keep attribute on old ent
	SplitCopy = 2,			// Copy the attribute to the new entity
	SplitCustom = 3			// Reserved for Generic Attributes.
} split_action;	

/**
 * \enum trans_action
 * ATTRIB actions for a trans_owner notification.
 * <br><br>
 * @param TransLose
 * Lose the attribute if its owner is transformed.
 * @param TransIgnore
 * Do nothing if its owner is transformed.
 * @param TransApply
 * Reserved for Generic Attributes. Apply given transform.
 * @param TransCustom
 * Reserved for Generic Attributes. Call application supplied routine.
 */
typedef enum 
{
	TransLose = 0,			// lose attribute
	TransIgnore = 1,		// keep attribute
	TransApply = 2,			// Reserved for Generic Attributes.
	TransCustom = 3			// Reserved for Generic Attributes.
} trans_action;

/**
 * \enum tolerant_action
 * ATTRIB actions for to_tolerant_owner and from_tolerant_owner 
 * notifications.
 * <br><br>
 * @param TolerantLose
 * Lose the attribute if it's owner is being lost and is being replaced
 * by a new tolerant/non-tolerant owner.
 * @param TolerantMove
 * Move the attribute to the new tolerant/non-tolerant owner.
 */
typedef enum 
{
	TolerantLose,		// do nothing; old attr goes away
	TolerantMove		// move the attribute from old to new
} tolerant_action;

/**
 * \enum replace_action
 * ATTRIB actions for a replace_owner notification.
 * <br><br>
 * @param ReplaceLose
 * Lose the attribute if its owner is replaced.
 * @param ReplaceKeepKept
 * Keep the attribute on the entity being kept.
 * @param ReplaceKeepLost
 * Move the attribute from the entity going away to the entity being 
 * kept, lose any on kept.
 * @param ReplaceKeepAll
 * Move the attribute from the entity going away to the entity being 
 * kept, keep any on kept.
 */
typedef enum 
{
	ReplaceLose,		// lost attr on kept ent
	ReplaceKeepKept,	// keep attr on kept-ent
	ReplaceKeepLost,	// move the attribute from go-ent to kept-ent, lose any on kept
	ReplaceKeepAll		// move the attribute from go-ent to kept-ent, keep any on kept
} replace_action;

/**
 * \enum geometry_changed_action
 * ATTRIB actions for lop_change_owner, replace_owner_geometry,
 * reverse_owner, and warp_owner notifications.
 * <br><br>
 * @param GeomChangedLose
 * Lose the attribute if its owner's geometry is changed.
 * @param GeomChangedKeep
 * Keep the attribute if its owner's geometry is changed.
 */
typedef enum 
{
	GeomChangedLose,	// lose attribute
	GeomChangedKeep		// keep attribute
} geometry_changed_action;

/**
 * @nodoc
 */
const copy_action default_copy_action = CopyKeep;
/**
 * @nodoc
 */
const merge_action default_merge_action = MergeKeepKept;
/**
 * @nodoc
 */
const split_action default_split_action = SplitKeep;
/**
 * @nodoc
 */
const trans_action default_trans_action = TransIgnore;
/**
 * @nodoc
 */
const tolerant_action default_to_tolerant_action = TolerantMove;
/**
 * @nodoc
 */
const tolerant_action default_from_tolerant_action = TolerantMove;
/**
 * @nodoc
 */
const replace_action default_replace_action = ReplaceKeepKept;
/**
 * @nodoc
 */
const geometry_changed_action default_lop_change_action = GeomChangedKeep;
/**
 * @nodoc
 */
const geometry_changed_action default_rep_geom_action = GeomChangedKeep;
/**
 * @nodoc
 */
const geometry_changed_action default_reverse_action = GeomChangedKeep;
/**
 * @nodoc
 */
const geometry_changed_action default_warp_action = GeomChangedKeep;

/**
 * @nodoc
 */
const logical default_moveable = TRUE;
/**
 * @nodoc
 */
const logical default_pattern_compatible = FALSE;
/**
 * @nodoc
 */
const logical default_pattern_copyable = TRUE;
/**
 * @nodoc
 */
const logical default_deletable = FALSE;
/**
 * @nodoc
 */
const logical default_duplicatable = TRUE;
/**
 * @nodoc
 */
const logical default_copyable = TRUE;
/**
 * @nodoc
 */
const logical default_savable = TRUE;

/**
 * @nodoc
 */
const unsigned int num_copy_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_merge_action_bits = 3;
/**
 * @nodoc
 */
const unsigned int num_split_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_trans_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_to_tolerant_action_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_from_tolerant_action_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_replace_action_bits = 2;
/**
 * @nodoc
 */
const unsigned int num_lop_change_action_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_rep_geom_action_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_reverse_action_bits = 1;
/**
 * @nodoc
 */
const unsigned int num_warp_action_bits = 1;


/**
 * @nodoc
 */
typedef struct
{
	unsigned int copy_action			: num_copy_action_bits;
	unsigned int merge_action			: num_merge_action_bits;
	unsigned int split_action			: num_split_action_bits;
	unsigned int trans_action			: num_trans_action_bits;
	unsigned int to_tolerant_action		: num_to_tolerant_action_bits;
	unsigned int from_tolerant_action	: num_from_tolerant_action_bits;
	unsigned int replace_action			: num_replace_action_bits;
	unsigned int lop_change_action		: num_lop_change_action_bits;
	unsigned int rep_geom_action		: num_rep_geom_action_bits;
	unsigned int reverse_action			: num_reverse_action_bits;
	unsigned int warp_action		 	: num_warp_action_bits;

	unsigned int moveable				: 1;
	unsigned int pattern_compatible		: 1;
	unsigned int pattern_copyable		: 1;
	unsigned int copyable				: 1;
	unsigned int deletable				: 1;
	unsigned int duplicatable			: 1;
	unsigned int savable				: 1;
} behaviors_t;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int ATTRIB_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define ATTRIB_LEVEL 1

/**
 * Represents common data and functionality for all attributes.
 * <br>
 * <b>Role:</b> <tt>ATTRIB</tt> is the generic class for user and system attributes.
 * It provides housekeeping to maintain <tt>ENTITY</tt> attribute lists. Every
 * entity may have one or more attributes. These hang off the <tt>ENTITY</tt>'s
 * attribute pointer, and are arranged as a doubly-linked chain.
 * <br><br>
 * Macros simplify the definition of derived user attributes. Many of the methods
 * required for any attribute are the same for all attributes at the source level.
 * Other methods have elements in common. All of these methods are defined in the nested
 * macros in the <tt>at_macro.hxx</tt> file.
 * @see ENTITY
 */

class DECL_KERN ATTRIB: public ENTITY {

    friend class pattern_holder;
    friend class ATTRIB_PAT_HOLDER;

	friend DECL_KERN logical post_restore_fixup( restore_data& );

	ATTRIB *next_ptr;		// pointers for 2-way attribute list
	ATTRIB *previous_ptr;

	ENTITY *entity_ptr;		// entity that owns attribute

	behaviors_t behaviors;	// how the attrib behaves, both notifications and ables

	logical set_moveable( logical );
	logical set_pattern_compatible( logical );
	logical set_pattern_copyable( logical );
	logical set_copyable( logical );
	logical set_deletable( logical );
	logical set_duplicatable( logical );
	logical set_savable( logical );

protected:
	/**
	 * @nodoc
	 */
	void private_set_next( ATTRIB *att ) 
			{ next_ptr = att; }

	/**
	 * @nodoc
	 */
	void private_set_previous( ATTRIB *att ) 
			{ previous_ptr = att; }

	/**
	 * @nodoc
	 */
	void private_set_entity( ENTITY *ent ) 
			{ entity_ptr = ent; }

public:

	/**
	 * Constructs an <tt>ATTRIB</tt> having the given owner <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b> Chain an attribute to the head of the given <tt>ENTITY</tt>, making
	 * it the attribute's owner.
	 * <br><br>
	 * @param entity
	 * the owning ENTITY.
	 */
	ATTRIB( ENTITY * entity = NULL );

	// These function are hidden from mkman in the ATTRIB_FUNCTIONS macro; to have them documented,
	// we include them here:
#if 0
	/**
	 * Returns a type identifier for this object.
	 * <br><br>
	 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier
	 * <tt>ATTRIB_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>ATTRIB_TYPE</tt>
	 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
	 * defined as <tt>ATTRIB_LEVEL</tt>.
	 * <br><br>
	 * @param level
	 * the derivation level at which the object is to be identified.
	 */
	virtual int identity( int level = 0 ) const;

	/**
	 * Returns the string <tt>"attrib"</tt>.
	 */
	virtual const char *type_name() const;

	/**
	 * Posts a delete bulletin to the bulletin board indicating this <tt>ATTRIB</tt> is no longer 
	 * used in the active model.
	 * <br><br>
	 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
	 */
	virtual void lose();
#endif

	// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
	// we include it here:
#if 0
	/**
	 * Restores this <tt>ATTRIB</tt> from a <tt>SAT</tt> file.
	 * <br><br>
	 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
	 * which is used in reading information from a <tt>SAT</tt> file. This method is never called
	 * directly. It is called by a higher hierarchical function if an item in the <tt>SAT</tt> file
	 * is determined to be of this class type. An instance of this class will already have
	 * been created through the allocation constructor. This method then populates the class
	 * instance with the appropriate data from the <tt>SAT</tt> file.
	 * <pre>
	 *    read_ptr  Pointer to record in save file for next attribute owned by entity
	 *    read_ptr  Pointer to record in save file for previous attribute owned by entity
	 *    read_ptr  Pointer to record in save file for owning entity</pre>
	 */
    void restore_common();
#endif

	// Remove the attribute from the chain of attributes of its owning
	// entity.
	/**
	 * Removes this <tt>ATTRIB</tt> from the chain of attributes of its owning <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b> Since the process of unhooking sets the base class pointers (entity_ptr, 
	 * previous_ptr, and next_ptr) to NULL, therby changing the entity, unhook must first call 
	 * backup to preserve the prior state of the attribute for rollback purposes. This then 
	 * allows one to freely modify the pointers, presumably in a derived class method, 
	 * without worrying whether or not to call backup.
	 */
	ATTRIB *unhook();

	/**
	 * Duplicate this attribute onto the given <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b> Using an attribute's <tt>copy_data</tt>, <tt>copy_common,</tt>,
	 * <tt>copy_scan</tt>, and <tt>fix_pointer</tt> methods, this <tt>ATTRIB</tt> base 
	 * class method will duplicate 'this' attribute onto the given entity. (Assuming
	 * 'this' returns copyable() == TRUE).
	 * <br><br>
	 * Simple attribute data will be propagated onto the new attribute according to
	 * the attribute's <tt>COPY_DEF</tt>.
	 * <br><br>
	 * Complex attribute data (pointers to entities other than the attribute's owner)
	 * will not be copied to the new attribute via this method.  Instead, the complex
	 * attribute data will be set to <tt>NULL</tt>.
	 * <br><br>
	 * @param new_ent
	 * the new <tt>ENTITY</tt> onto which a duplicate of 'this' attribute will be placed.
	 */
	ATTRIB * duplicate( ENTITY * new_ent ) const;

	/**
	 * Moves this <tt>ATTRIB</tt> to a new owning <tt>ENTITY</tt>.
	 * <br><br>
	 * @param new_entity
	 * the new owning ENTITY.
	 */
	virtual ATTRIB *move( ENTITY *new_entity );

	/**
	 * Returns the next attribute in this <tt>ATTRIB</tt>'s linked list.
	 */
	ATTRIB *next() const { return next_ptr; }

	/**
	 * Returns the previous attribute in this <tt>ATTRIB</tt>'s linked list.
	 */
	ATTRIB *previous() const { return previous_ptr; }

	/**
	 * Returns a pointer to the <tt>ENTITY</tt> that owns this <tt>ATTRIB</tt>.
	 */
	ENTITY *entity() const { return entity_ptr; }

	/**
	 * Identifies the owning <tt>ENTITY</tt>, if any, of this <tt>ATTRIB</tt>.
	 * <br><br>
	 * <b>Role:</b>  A return of <tt>NULL</tt> indicates that this attribute has either
	 * no owner or an unidentifiable one. Objects of the base <tt>ATTRIB</tt> class
	 * have no identifiable owner.
	 */
	virtual ENTITY *owner() const;

	/**
	 * Indicates whether this <tt>ATTRIB</tt> is independently deletable of its owner.
	 * <br><br>
	 * <b>Role:</b> The default and suggested behavior for this method is to return <tt>FALSE</tt>.
	 * This is an indication that the attribute's lifetime is controlled by its owner; e.g.,
	 * when the owner is lost, so too is this attribute.
	 * <br><br>
	 * An exception to this default behavior is if the attribute has no owner, e.g., its owner
	 * pointer is <tt>NULL</tt>.  In this case, ATTRIB::deletable will return TRUE.
	 * <br><br>
	 * You may want to return <tt>TRUE</tt> for this method if, as suggested, you want
	 * the attribute to be lost before it's owner. Note that api_del_entity will guarantee
	 * that all deletable attributes are lost before their owner is lost.
	 */
	virtual logical deletable() const;

	/**
	 * Indicates whether this <tt>ATTRIB</tt> can be duplicated.
	 * <br><br>
	 * <b>Role:</b> Indicates whether this attribute may be duplicated via either a copy
	 * or a save/restore of the owning <tt>ENTITY</tt>. The default is <tt>TRUE</tt>.
	 */
	virtual logical duplicatable() const;

	// The next two methods (copyable and savable) give finer control.  The default is 
	// to call duplicatable(), but either may overriden to allow attribs that can
	// be saved but not copied or vice versa.

	// Indicate whether this attribute should be included when the owning
	// entity is copied.  The default is to call duplicatable().
	/**
	 * Indicates whether this attribute should be included when the owning entity is copied.
	 * <br><br>
	 * <b>Role:</b>  The default is to call <tt>duplicatable()</tt>.
	 */
	virtual logical copyable() const;

	/**
	 * Indicates whether this <tt>ATTRIB</tt> should be included when the owning entity is saved.
	 * <br><br>
	 * <b>Role:</b>  The default is to call <tt>duplicatable()</tt>.
	 */
	virtual logical savable() const;

	// Indicate whether this attribute should be included when the owning
	// entity is moved.  The default is to call copyable().
	/**
	 * Returns <tt>TRUE</tt> if this <tt>ATTRIB</tt> is moveable from one owning <tt>ENTITY</tt> to another.
	 * <br><br>
	 * <b>Role:</b>  The default is to call <tt>copyable()</tt>.
	 */
	virtual logical moveable() const;

	/**
	 * Returns <tt>TRUE</tt> if the <tt>ATTRIB</tt> is pattern-copyable.
	 * <br><br>
	 * <b>Role:</b>  This virtual method indicates whether this attribute should be
	 * copied when its owning entity is copied for the purpose of creating a new 
	 * pattern element (i.e., when reason == SCAN_PATTERN).
	 * <br><br>
	 * By default, this virtual method returns the value returned by <tt>copyable</tt>.  
	 * This behavior should be overridden in derived classes that are deemed to be not
	 * pattern-copyable.
	 */
	virtual logical pattern_copyable() const;

	// During a save operation, ENTITYs that were generated by
	// a pattern are not saved, but are instead regenerated during a subsequent
	// restore.  This method is used to tell whether this attribute's owner will be
	// saved.
	/**
	 * @nodoc
	 */
	logical owner_is_saved() const;

	// Indicates whether this attribute can be saved without having to
	// remove the pattern, if any, from the owning entity.  All
    // non-savable attributes are compatible.  For savable attributes,
    // compatibility can be determined by the answer to the question:
    // "Does saving this attribute create references to entities (other
    // than the owning entity) that will be lost if only the seed element
    // of the pattern is saved?"  If not, the attribute in question should
    // be marked as pattern-compatible, by overriding this function, to
    // allow the use of patterns as widely as possible.  For safety's sake,
    // savable attributes are assumed to be incompatible with patterns
    // unless this function is overridden to return TRUE.
	/**
	 * Returns <tt>TRUE</tt> if the <tt>ATTRIB</tt> is pattern-compatible.
	 * <br><br>
	 * <b>Role:</b>  An attribute is pattern-compatible if it does not refer
	 * to another pattern-generated entity (other than its owner).
	 * <br><br>
	 * By default, this virtual method returns <tt>FALSE</tt> unless the attribute is not 
	 * <tt>savable</tt>.  
	 * This behavior should be overridden in derived classes that are deemed to be
	 * pattern-compatible, however, in order to facilitate pattern performance enhancements.
	 */
    virtual logical pattern_compatible() const;

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
	 * The default action is to do nothing.
	 * <br><br>
	 * @param new_entity
	 * the new entity arising from the split.
	 * @see ATTRIB#set_split_owner_action, get_split_owner_action, split_action, split_attrib
	 */
	virtual void split_owner( ENTITY *new_entity );

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
	 * The default action is to do nothing.
	 * <br><br>
	 * @param entity
	 * the ENTITY with which this attribute's owner is being merged.
	 * @param del_owner
	 * flag that the owner of this attribute will be deleted after the merge.
	 * @see ATTRIB#set_merge_owner_action, get_merge_owner_action, merge_action, merge_attrib
	 */
	virtual void merge_owner( ENTITY * entity, logical del_owner );

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owner is about to be transformed.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to transform the attribute.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_trans_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>trans_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @param transf
	 * the transformation being applied.
	 * @see ATTRIB#set_trans_owner_action, get_trans_owner_action, trans_action, trans_attrib
	 */
	virtual void trans_owner( const SPAtransf &transf );

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owner is about to be transformed.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to transform the attribute.
	 * The default action is to call <tt>trans_owner</tt>.
	 * <br><br>
	 * The list argument is a list of already transformed entities, which can 
	 * be used to stop infinite recursions by checking whether entities pointed 
	 * to by the attribute are already in the list.
	 * <br><br>
	 * @param transf
	 * the transformation being applied.
	 * @param e_list
	 * ENTITY list.
	 */
	virtual void trans_owner_list(
				const SPAtransf &transf,
				ENTITY_LIST &e_list	);

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning entity's sense bit is about to 
	 * be reversed.
	 * <b>Role:</b> The application has the chance to do something relevant. For example,
	 * faceter attributes reverse the mesh's face normals (or delete the mesh) when the
	 * owning face is reversed.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_reverse_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>reverse_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @see ATTRIB#set_reverse_owner_action, get_reverse_owner_action, geometry_changed_action, reverse_attrib
	 */
 	virtual void reverse_owner();

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> will be modified by warping.
	 * <br><br>
	 * <b>Role:</b>  This member function allows geometric attributes to warp themselves when
	 * their owners are warped.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_warp_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>warp_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @param warp
	 * the law being used by the warping operation.
	 * @see ATTRIB#set_warp_owner_action, get_warp_owner_action, geometry_changed_action, warp_attrib
	 */
	virtual void warp_owner( law *warp );

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning ENTITY will be changed by a local operation, 
	 * face removal, or shelling.
	 * <br><br>
	 * <b>Role:</b> This member function informs the attribute that the its owning ENTITY has been
	 * geometrically modified during the course of a local operation. No other details as to how the
	 * owner is being modified are provided.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_lop_change_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>lop_change_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @see ATTRIB#set_lop_change_owner_action, get_lop_change_owner_action, geometry_changed_action, lop_change_attrib
	 */
	virtual void lop_change_owner();

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is being given new geometry, 
	 * possibly changing its shape.
	 * <br><br>
	 * <b>Role:</b> This member function informs the attribute that the its owning ENTITY is being 
	 * given new geometry.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_rep_owner_geom_action</tt> method to specify how your attribute should act within 
	 * <tt>replace_owner_geometry</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @param new_geom
	 * the new geometry.
	 * @see ATTRIB#set_rep_owner_geom_action, get_rep_owner_geom_action, geometry_changed_action, replace_geometry_attrib
	 */
	virtual void replace_owner_geometry(ENTITY *new_geom);

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is being replaced.
	 * <br><br>
	 * <b>Role:</b>  When the argument <tt>replace_owner</tt> is <tt>TRUE</tt>, the
	 * owner of this attribute is being replaced by <tt>other_entity</tt>; when it
	 * is <tt>FALSE</tt>, <tt>other_entity</tt> is being replaced by this attribute's owner.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize 
	 * the <tt>set_replace_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>replace_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @param other_entity
	 * the other ENTITY involved in the replacement.
	 * @param replace_owner
	 * flag that the owning ENTITY is being replaced.
	 * @see ATTRIB#set_replace_owner_action, get_replace_owner_action, replace_action, replace_attrib
	 */
	virtual void replace_owner(
					   ENTITY *other_entity,
					   logical replace_owner
					   );


	/**
 	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> has been copied.
	 * <br><br>
	 * <b>Role:</b> This method is used to notify this attribute that it's owner has been copied.
	 * <br><br>
	 * The predominant fashion by which <tt>copy_owner</tt> is called is during the course of ACIS 
	 * operations that are constructing, deconstructing, or modifying either topology and/or 
	 * geometry. This method is also called at the end of the api_down_copy_entity algorithm.
	 * As a general rule, the attribute implementing this method cannot assume the validity
	 * of the model "above" or "below" its owner, only that its owner is valid and that the
	 * <tt>copy_ent</tt> is valid.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_copy_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>copy_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * The default action is to do nothing.
	 * <br><br>
	 * @param copy_ent
	 * the new <tt>ENTITY</tt> that is a copy of this attribute's owner.
	 * @see ATTRIB#set_copy_owner_action, get_copy_owner_action, copy_action, copy_attrib
	 */
	virtual void copy_owner( ENTITY *copy_ent );
	
	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is being replaced
	 * with a tolerant <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b> The default action is to move the attribute onto the given tolerant
	 * <tt>ENTITY</tt> only if <tt>moveable()</tt> returns <tt>TRUE</tt>.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_to_tolerant_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>to_tolerant_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * @param tol_ent
	 * the new, tolerant owner.
	 * @see ATTRIB#set_to_tolerant_owner_action, get_to_tolerant_owner_action, tolerant_action, to_tolerant_attrib
	 */
	virtual void to_tolerant_owner( ENTITY *tol_ent );

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is being replaced
	 * with a non-tolerant <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b> The default action is to move the attribute onto the given non-tolerant
	 * <tt>ENTITY</tt> only if <tt>moveable()</tt> returns <tt>TRUE</tt>.
	 * <br><br>
	 * Depending on the application needs of your attribute, it may be possible to utilize the 
	 * <tt>set_from_tolerant_owner_action</tt> method to specify how your attribute should act within 
	 * <tt>from_tolerant_owner</tt>, which would preclude the need to actually implement this method.
	 * <br><br>
	 * @param tol_ent
	 * the new, tolerant owner.
	 * @see ATTRIB#set_from_tolerant_owner_action, get_from_tolerant_owner_action, tolerant_action, from_tolerant_attrib
	 */
	virtual void from_tolerant_owner( ENTITY *non_tol_ent );

	// Entry for the model debugging system - maintains a list of
	// all attributes seen, and returns the index number of the
	// current one.
	/**
	 * @nodoc
	 */
	virtual int lookup( logical ) const;

	 /**
	  * Set the <tt>copy_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the copy_owner action on an attribute that implements copy_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its copy_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever copy_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	copy_action set_copy_owner_action( const copy_action action );

	 /**
	  * Get the current copy_owner action for this attribute.
	  */
	copy_action get_copy_owner_action( void ) const;

	 /**
	  * Set the <tt>merge_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the merge_owner action on an attribute that implements merge_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its merge_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever merge_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	merge_action set_merge_owner_action( const merge_action action );
	 /**
	  * Get the current merge_owner action for this attribute.
	  */
	merge_action get_merge_owner_action( void ) const;

	 /**
	  * Set the <tt>split_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the split_owner action on an attribute that implements split_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its split_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever split_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	split_action set_split_owner_action( const split_action action );
	 /**
	  * Get the current split_owner action for this attribute.
	  */
	split_action get_split_owner_action( void ) const;

	 /**
	  * Set the <tt>trans_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the trans_owner action on an attribute that implements trans_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its trans_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever trans_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	trans_action set_trans_owner_action( const trans_action action );
	 /**
	  * Get the current trans_owner action for this attribute.
	  */
	trans_action get_trans_owner_action( void ) const;

	 /**
	  * Set the <tt>to_tolerant_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the to_tolerant_owner action on an attribute that implements to_tolerant_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its to_tolerant_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever to_tolerant_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	tolerant_action set_to_tolerant_owner_action( const tolerant_action action );
	 /**
	  * Get the current to_tolerant_owner action for this attribute.
	  */
	tolerant_action get_to_tolerant_owner_action( void ) const;

	 /**
	  * Set the <tt>from_tolerant_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the from_tolerant_owner action on an attribute that implements 
	  * from_tolerant_owner will have no effect when that attribute is running in 
	  * its native application. However, when the attribute is saved to SAT/SAB, 
	  * its from_tolerant_owner action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever from_tolerant_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	tolerant_action set_from_tolerant_owner_action( const tolerant_action action );
	 /**
	  * Get the current from_tolerant_owner action for this attribute.
	  */
	tolerant_action get_from_tolerant_owner_action( void ) const;

	 /**
	  * Set the <tt>replace_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the replace_owner action on an attribute that implements replace_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its replace_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever replace_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	replace_action set_replace_owner_action( const replace_action action );
	 /**
	  * Get the current replace_owner action for this attribute.
	  */
	replace_action get_replace_owner_action( void ) const;

	 /**
	  * Set the <tt>lop_change_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the lop_change_owner action on an attribute that implements lop_change_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its lop_change_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever lop_change_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	geometry_changed_action set_lop_change_owner_action( const geometry_changed_action action );
	 /**
	  * Get the current lop_change_owner action for this attribute.
	  */
	geometry_changed_action get_lop_change_owner_action( void ) const;

	 /**
	  * Set the <tt>replace_owner_geometry</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the replace_owner_geometry action on an attribute that implements replace_owner_geometry
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its replace_owner_geometry
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever replace_owner_geometry action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	geometry_changed_action set_rep_owner_geom_action( const geometry_changed_action action );
	 /**
	  * Get the current replace_owner_geometry action for this attribute.
	  */
	geometry_changed_action get_rep_owner_geom_action( void ) const;

	 /**
	  * Set the <tt>reverse_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the reverse_owner action on an attribute that implements reverse_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its reverse_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever reverse_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	geometry_changed_action set_reverse_owner_action( const geometry_changed_action action );
	 /**
	  * Get the current reverse_owner action for this attribute.
	  */
	geometry_changed_action get_reverse_owner_action( void ) const;

	 /**
	  * Set the <tt>warp_owner</tt> action for this attribute. Returns
	  * the value actually set. If an invalid action is specified, a
	  * <tt>sys_warning(VALUE_OUT_OF_RANGE)</tt> will be issued and the action 
	  * will remain unchanged.
	  * <br><br>
	  * Setting the warp_owner action on an attribute that implements warp_owner
	  * will have no effect when that attribute is running in its native application.
	  * However, when the attribute is saved to SAT/SAB, its warp_owner
	  * action will also be saved. In the event the attribute is restored
	  * in a foreign application, we will treat the attribute as 'unknown'
	  * but will honor whatever warp_owner action setting is restored. (Note that
	  * support for save/restore of attribute actions was introduced in R16; persistence and
	  * recognition of attribute actions prior to R16 is not supported.)
	  * <br><br>
	  * @param action
	  * the specified action.
	  */
	geometry_changed_action set_warp_owner_action( const geometry_changed_action action );
	 /**
	  * Get the current warp_owner action for this attribute.
	  */
	geometry_changed_action get_warp_owner_action( void ) const;

	// Standard attribute functions for save, restore, and general
	// housekeeping.

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


/**
 * Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given type, subtype, 
 * etc., starting at the beginning of the list.
 * <br><br>
 * <b>Role:</b>  Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given
 * type, subtype, etc., starting at the beginning of the list. The integer arguments
 * are successive subtypes of <tt>ATTRIB</tt> to be matched. Zero means "don't care at this level." -1 
  * means "don't care at this or any deeper level." All attribute type codes are guaranteed
 * unique for any run of ACIS. If no matching <tt>ATTRIB</tt> is found, <tt>NULL</tt> is returned.
 * <br><br>
 * Use find_attrib to find the first instance of an attribute that matches your search criteria.
 * Use find_next_attrib iteratively to search for the next attrib that meets your search criteria.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param owner
 * the owning entity.
 * @param subtype
 * the subtype to be matched.
 * @param subsubtype
 * the subsubtype to be matched.
 * @param subsubsubtype
 * the subsubsubtype to be matched.
 * @param subsubsubsubtype
 * the subsubsubsubtype to be matched.
 * @see ATTRIB, find_attrib, find_next_attrib, find_leaf_attrib, find_next_leaf_attrib
 */
DECL_KERN ATTRIB *find_attrib(
			const ENTITY *owner,
			int subtype = -1,
			int subsubtype = -1,
			int subsubsubtype = -1,
			int subsubsubsubtype = -1
		);

/**
 * Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given type, subtype, 
 * etc., starting at a given attribute.
 * <br><br>
 * <b>Role:</b>  Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given
 * type, subtype, etc., starting at a given attribute. The integer arguments
 * are successive subtypes of <tt>ATTRIB</tt> to be matched. Zero means "don't care at this level." -1
 * means "don't care at this or any deeper level." All attribute type codes are guaranteed
 * unique for any run of ACIS. If no matching <tt>ATTRIB</tt> is found, <tt>NULL</tt> is returned.
 * <br><br>
 * Use find_attrib to find the first instance of an attribute that matches your search criteria.
 * Use find_next_attrib iteratively to search for the next attribute that meets your search criteria.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param att
 * the attribute from which to start the search.
 * @param subtype
 * the subtype to be matched.
 * @param subsubtype
 * the subsubtype to be matched.
 * @param subsubsubtype
 * the subsubsubtype to be matched.
 * @param subsubsubsubtype
 * the subsubsubsubtype to be matched.
 * @see ATTRIB, find_attrib, find_next_attrib, find_leaf_attrib, find_next_leaf_attrib
 */
DECL_KERN ATTRIB *find_next_attrib(
			ATTRIB const *att,
			int subtype = -1,
			int subsubtype = -1,
			int subsubsubtype = -1,
			int subsubsubsubtype = -1
		);

// Functions to search an attribute list for attributes of a
// specific "leaf" type. Because of a change at V1.9, all attribute
// type codes are guaranteed unique for any run of Acis, so these
// will find you what you want. Formerly only the complete hierarchy
// of type codes was unique.

/**
 * Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given type, starting 
 * at the beginning of the list.
 * <br><br>
 * <b>Role:</b>  Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given
 * type, starting at the beginning of the list. All attribute type codes are guaranteed unique for 
 * any run of ACIS. If no matching <tt>ATTRIB</tt> is found, <tt>NULL</tt> is returned.
 * <br><br>
 * Use find_leaf_attrib to find the first instance of an attribute that matches your search criteria.
 * Use find_next_leaf_attrib iteratively to search for the next attribute that meets your search criteria.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param owner
 * the owning entity.
 * @param attrib_type
 * the attribute type code.
 * @see ATTRIB, find_attrib, find_next_attrib, find_leaf_attrib, find_next_leaf_attrib
 */
DECL_KERN ATTRIB *find_leaf_attrib(
			const ENTITY * owner,
			int attrib_type
		);

/**
 * Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given type, starting 
 * at a given attribute.
 * <br><br>
 * <b>Role:</b>  Searches an <tt>ENTITY</tt>'s attribute list for an <tt>ATTRIB</tt> of a given
 * type, starting at the given attribute. All attribute type codes are guaranteed unique for 
 * any run of ACIS. If no matching <tt>ATTRIB</tt> is found, <tt>NULL</tt> is returned.
 * <br><br>
 * Use find_leaf_attrib to find the first instance of an attribute that matches your search criteria.
 * Use find_next_leaf_attrib iteratively to search for the next attribute that meets your search criteria.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param attrib
 * the starting attribute.
 * @see find_attrib, find_next_attrib, find_leaf_attrib, find_next_leaf_attrib
 */
DECL_KERN ATTRIB *find_next_leaf_attrib(
			ATTRIB const * attrib
		);

/**
 * Provides a convenient way to have the "split_owner" method called for every attribute
 * on an entity being split.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on keep_ent and calls
 * attrib->split_owner(new_ent) on each.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param old_ent
 * the entity being split.
 * @param new_ent
 * the new entity that represents "the other side" of the split.
 * @param gap_ent
 * the entity "between" the old_ent and new_ent.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib 
 */
DECL_KERN void split_attrib(
			ENTITY * old_ent,
			ENTITY * new_ent,
            ENTITY * gap_ent = NULL
		);

/**
 * Provides a convenient way to "move" all attributes from an entity that is "going away"
 * to an entity that is being "kept".
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on keep_ent and calls
 * attrib->merge_owner(go_ent, FALSE) on each. Then walks the list of attributes 
 * on the go_ent and calls attrib->merge_owner(keep_ent, TRUE) on each.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param keep_ent
 * the entity being "kept".
 * @param go_ent
 * the entity "going away".
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void merge_attrib(
			ENTITY *keep_ent,	// entity to be kept
			ENTITY *go_ent		// entity that disappears
		);

/**
 * Provides a convenient way for attributes to be notified when their owner is being 
 * transformed.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the owning entity and calls 
 * attrib->trans_owner_list(trans, list) on each.
 * <br><br>
 * The list argument is a list of already transformed entities that can 
 * be used to stop infinite recursions by checking whether entities pointed 
 * to by the attribute are already in the list.
 * <br><br>
 * The implementation of ATTRIB::trans_owner_list calls ATTRIB::trans_owner.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param ent
 * the owning entity.
 * @param trans
 * the transform to apply.
 * @param list
 * list of entities already transformed.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void trans_attrib(
			ENTITY * ent,					// owning entity
			SPAtransf const & trans,		// applied transformation
			ENTITY_LIST * list = NULL		// list of entities already transformed.
		);

/**
 * Provides a convenient way to copy all attributes from a source entity to a target entity.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the source entity and calls
 * attrib->copy_owner(target) on each. 
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param source
 * the source entity.
 * @param target
 * the target entity.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void copy_attrib(
			ENTITY *source,
			ENTITY *target
		);

/**
 * Provides a convenient way for attributes to be notified when their owner's sense
 * bit is being reversed.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the given entity and calls 
 * attrib->reverse_owner() on each. 
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param ent
 * the owning entity.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void reverse_attrib(
			ENTITY * ent			// owning entity
		);

/**
 * Provides a convenient way for attributes to be notified when their owning entity
 * is being warped.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the given entity and calls 
 * attrib->warp_owner() on each. 
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param ent
 * the owning entity.
 * @param warp_law
 * the warp law being applied.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, lop_change_attrib, 
 * replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void warp_attrib(
			ENTITY * ent,		// owning entity
			law * warp_law		// warpping law
		);

/**
 * Provides a convenient way for attributes to be notified when their owning entity
 * is changed during a local operation, remove faces, or shelling.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the given entity and calls 
 * attrib->lop_change_owner() on each. 
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param ent
 * the owning entity.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void lop_change_attrib(
						 ENTITY * ent
						 );

/**
 * Provides a convenient way for attributes to be notified when their owning entity
 * is getting new geometry, which possibly changes its shape.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the given entity and calls 
 * attrib->replace_owner_geometry(new_geom) on each.
 * <br><br>
 * If the flag only_when_owning_geom_null is TRUE, this call does nothing 
 * when the geometry of the owning entity is not NULL. The flag is TRUE 
 * when a subsequent call to set_geometry is made, so multiple attribute 
 * notifications are not made.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param ent
 * the owning entity.
 * @param new_geom
 * the new geometry.
 * @param only_when_owning_geom_null
 * If TRUE, do nothing when ent geometry is non-NULL.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void replace_geometry_attrib(
							   ENTITY * ent,
							   ENTITY * new_geom,
                               logical only_when_owning_geom_null = TRUE
							   );

/**
 * Provides a convenient way to "replace" all attributes from an entity that is being "replaced"
 * to an entity that is being "kept".
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on keep_ent and calls
 * attrib->replace_owner(replace_ent, FALSE) on each. Then walks the list of attributes 
 * on the replace_ent and calls attrib->replace_owner(keep_ent, TRUE) on each.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param keep_ent
 * the entity that is being "kept".
 * @param replace_ent
 * the entity that is being "replaced".
 * @param only_when_replace_ent_null
 * If TRUE, do nothing if replace_ent is non-NULL.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void replace_attrib(
			ENTITY * keep_ent,
			ENTITY * replace_ent,
            logical only_when_replace_ent_null = TRUE
		);

/**
 * Provides a convenient way for attributes to be notified when their owning entity
 * has been replaced with a tolerant entity and will be deleted.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on orig_ent and calls 
 * attrib->to_tolerant_owner(tol_ent) on each.
 * <br><br>
 * The ATTRIB::to_tolerant_owner method gives the application the chance 
 * to delete, duplicate or otherwise modify their attribute.  The default 
 * action is to move the attribute onto the new tolerant entity as long as 
 * moveable() is TRUE.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param orig_ent
 * the original entity that is being deleted.
 * @param tol_ent
 * the newly created tolerant entity.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void to_tolerant_attrib(
                ENTITY * orig_ent,
				ENTITY * tol_ent
			);

/**
 * Provides a convenient way for attributes to be notified when their owning entity
 * has been replaced with a non-tolerant entity and will be deleted.
 * <br><br>
 * <b>Role:</b> Walks the list of attributes on the given entity and calls 
 * attrib->from_tolerant_owner(non_tol_ent) on each. 
 * <br><br>
 * The ATTRIB::from_tolerant_owner method gives the application the chance 
 * to delete, duplicate or otherwise modify their attribute.  The default 
 * action is to move the attribute onto the new non-tolerant entity as long as 
 * moveable() is TRUE.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * @param orig_ent
 * the original entity that is being deleted.
 * @param non_tol_ent
 * the newly created non-tolerant entity.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */
DECL_KERN void from_tolerant_attrib(
                ENTITY * orig_ent,		// original entity
				ENTITY * non_tol_ent	// new non-tolerant entity
			);
// --------------------------------------------------------------------------------
// notification callbacks
// --------------------------------------------------------------------------------
/**
 * @nodoc
 */
typedef void (*to_tolerant_attrib_callback)(ENTITY*,ENTITY*);

/**
 * @nodoc
 */
typedef void (*from_tolerant_attrib_callback)(ENTITY*,ENTITY*);

/**
 * @nodoc
 */
DECL_KERN to_tolerant_attrib_callback register_to_tolerant_attrib_callback( to_tolerant_attrib_callback);
/**
 * @nodoc
 */
DECL_KERN from_tolerant_attrib_callback register_from_tolerant_attrib_callback( from_tolerant_attrib_callback);
/** @} */


#endif
