/* ORIGINAL: acis2.1/ga_husk/attrib/at_name.hxx */
// $Id: at_name.hxx,v 1.16 2002/08/09 17:19:22 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define class ATTRIB_GEN_NAME to support generic named attributes

// STI let (7/99): Modified class and global funcitons to add
// attribute migration support for copy_owner.

#ifndef ATTRIB_GEN_NAME_CLASS
#define ATTRIB_GEN_NAME_CLASS


// include files---

#include "dcl_ga.h"
#include "at_gen.hxx"
#include "logical.h"

/**
* @file at_name.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup GAAPI
 *
 * @{
 */
class enum_table;
class SPAposition;
class SPAvector;
class ATTRIB_GEN_NAME;

// The following typedefs define the signatures for custom functions
// to be called for the split/merge/trans/copy_owner methods.

/**
 * Defines the signature for custom <tt>split_owner</tt> methods for generic attributes
 * @param ganame
 * attribute whose owner was split.
 * @param newent
 * new entity created by split.
 */
typedef void (*split_func)(ATTRIB_GEN_NAME *ganame, ENTITY *newent);

/**
 * Defines the signature for custom <tt>merge_owner</tt> methods for generic attributes.
 * @param ganame
 * attribute whose owner is being merged.
 * @param otherent
 * other entity involved in merge.
 * @param lose_attr
 * attributes owner will be lost?
 */
typedef void (*merge_func)(ATTRIB_GEN_NAME *ganame, ENTITY *otherent, logical lose_attr);

/**
 * Defines the signature for custom <tt>trans_owner</tt> methods for generic attributes.
 * @param ganame
 * attribute whose owner is being transformed.
 * @param tran
 * transformation being applied.
 */
typedef void (*trans_func)(ATTRIB_GEN_NAME *ganame, SPAtransf const & tran);

/**
 * Defines the signature for custom <tt>copy_owner</tt> methods for generic attributes.
 * @param ganame
 * attribute whose owner is being copied.
 * @param ent
 * entity created by copy.
 */
typedef void (*copy_func)(ATTRIB_GEN_NAME * ganame, ENTITY * ent);


// Now define the attribute class

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_NAME, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_NAME_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_GEN_NAME_LEVEL (ATTRIB_GENERIC_LEVEL + 1)

/**
 * Defines a named attribute for the Generic Attributes Component.
 */
class DECL_GA ATTRIB_GEN_NAME: public ATTRIB_GENERIC {

    // Whether this attribute was created from a deprecated NAMED_ATTRIB instance.
    
    logical FromDeprAttr;

protected:

	/**
	 * Name assigned to this attribute.
	 */
	char *Name;

	/**
	 * Transforms the entity owned by this attribute in response to the <tt>trans_owner</tt> method.
	 */
	virtual void do_transform(SPAtransf const &, ENTITY_LIST &);

public:

	// Default constructor - used in backup and restore
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore.
	 */
	ATTRIB_GEN_NAME();

	// Create a named attribute
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param owner
	 * owning entity.
	 * @param name
	 * name.
	 * @param spl_act
	 * split action.
	 * @param mer_act
	 * merge action.
	 * @param tran_act
	 * transformation action.
	 * @param cop_act
	 * copy action.
	 */
	ATTRIB_GEN_NAME(
			ENTITY *owner,
			char const *name,
			split_action spl_act = SplitKeep,
			merge_action mer_act = MergeKeepKept,
			trans_action tran_act = TransIgnore,
			copy_action cop_act = CopyCopy
		);

	/**
	 * Returns <tt>TRUE</tt> if this is pattern compatible.
	 */
    virtual logical pattern_compatible() const;

	/**
	 * Gets the attribute name.
	 */
	char const *name() const { return Name; }

	/**
	 * Changes the name assigned to this attribute.
	 * <br><br>
	 * @param name
	 * name.
	 */
	void set_name(char const *name);

    /**
     * @nodoc
     */
    logical from_named_attrib() const { return FromDeprAttr; }

    /**
     * @nodoc
     */
    void set_from_named_attrib( logical from_depr );

	/**
	 * Notifies the <tt>ATTRIB_GEN_NAME</tt> that its owner is about to be split into two parts.
	 * <br><br>
	 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
	 * The default action is to do nothing. This function is supplied by the application whenever
	 * it defines a new attribute, and is called when a split occurs.
	 * <br><br>
	 * @param ent
	 * new entity.
	 */
	virtual void split_owner(ENTITY *ent);

	/**
	 * Notifies the <tt>ATTRIB_GEN_NAME</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
	 * <br><br>
	 * <b>Role:</b> The application has the chance to delete or otherwise modify the attribute.
	 * After the merge, this owner will be deleted if the logical deleting owner is <tt>TRUE</tt>, otherwise
	 * it will be retained and other entity will be deleted. The default action is to do nothing.
	 * This function is supplied by the application whenever it defines a new attribute, and is called
	 * when a merge occurs.
	 * <br><br>
	 * @param ent
	 * given entity.
	 * @param owner
	 * deleting owner.
	 */
	virtual void merge_owner(ENTITY *ent, logical owner);

	/**
	 * Notifies the <tt>ATTRIB_GEN_NAME</tt> that its owner is about to be transformed.
	 * <br><br>
	 * <b>Role:</b> The application has the chance to transform the attribute. The default action
	 * is to do nothing. This function is supplied by the application whenever it defines a new
	 * attribute, and is called when a transformation occurs.
	 * <br><br>
	 * @param trans
	 * transformation.
	 * @param ent_list
	 * entity list.
	 */
	virtual void trans_owner_list(SPAtransf const &trans, ENTITY_LIST &ent_list);

	/**
	 * Specifies the copy's owner.
	 * <br><br>
	 * @param ent
	 * owning entity.
	 */
	virtual void copy_owner(ENTITY *ent);

    /**
     * Returns <tt>TRUE</tt> when the attribute will be copied with the copy scan mechanism.
     * <br><br>
     * <b>Role:</b>  This method returns <tt>TRUE</tt> when the copy action is <tt>CopyCopy</tt>, and <tt>FALSE</tt>
     * in all other circumstances. There is no way to get <tt>CopyLose</tt> or <tt>CopyCustom</tt>
     * behavior with the copy scan mechanism, so in these cases copyable() returns <tt>FALSE</tt>
     * and sets a warning to notify the application of possibly undesired behavior.
     */
	virtual logical copyable() const;

    /**
	 * Returns the attribute class identification.
	 */
	static int id();

	/**
	 * Determines if the attribute class is the specified type.
	 * <br><br>
	 * @param t
	 * type to check.
	 */
	virtual logical isa(int t) const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_NAME, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};


// Define classes to contain table of custom split/merge/trans/copy methods
/**
 * @nodoc
 */
class func_entry : public ACIS_OBJECT {

public:
	func_entry *Next;
	char *Name;
	split_func Split;
	merge_func Merge;
	trans_func Trans;
	copy_func Copy;

	func_entry(
		func_entry *next,
		char const *name,
		split_func split,
		merge_func merge,
		trans_func trans,
		copy_func copy
		);

	~func_entry();
};


/**
 * @nodoc
 */
class func_table : public ACIS_OBJECT {
	func_entry *Head;

public:
	func_table() { Head = NULL; }

	~func_table()
	{
		while (Head != NULL) {
			func_entry *next = Head->Next;
			ACIS_DELETE Head;
			Head = next;
		}
	}

	func_entry *
	lookup(
		char const *name,
		logical insert
		)
	{
		func_entry *cur = Head;
		while (cur != NULL) {
			if (strcmp(cur->Name, name) == 0)
				return cur;
			cur = cur->Next;
		}

		if (insert) {
			cur =  ACIS_NEW func_entry(Head, name, NULL, NULL, NULL, NULL);
			Head = cur;
			return cur;
		} else {
			return NULL;
		}
	}
};



/**
 * Returns a pointer to the first generic attribute with the given name in an <tt>ENTITY</tt>'s list of attributes.
 * <br><br>
 * <b>Role:</b> <tt>NULL</tt> is returned if no generic attribute with the requested name is found.
 * <br><br>
 * @param ent
 * entity on which to find attribute.
 * @param name
 * name of attribute to be found.
 */
DECL_GA ATTRIB_GEN_NAME *find_named_attrib(ENTITY const *ent, char const *name);


/**
 * Returns a pointer to the next generic attribute with the same name and owner as the supplied attribute.
 * <br><br>
 * <b>Role:</b> <tt>NULL</tt> is returned if no such attribute is found.
 * <br><br>
 * @param ganame
 * generic attribute from which to start search.
 */
DECL_GA ATTRIB_GEN_NAME *find_next_named_attrib(ATTRIB_GEN_NAME const *ganame);


/**
 * Registers a function which implements a custom <tt>split_owner</tt> method for generic attributes with the specified name.
 * <br><br>
 * <b>Role:</b> Returns old split method, if any.
 * <br><br>
 * @param name
 * attribute name to which method applies.
 * @param sfunc
 * function implementing custom split method.
 */
DECL_GA split_func set_split_method(char const *name, split_func sfunc);


/**
 * Registers a function which implements a custom <tt>merge_owner</tt> method for generic attributes with the specified name.
 * <br><br>
 * <b>Role:</b> Returns old merge method, if any.
 * <br><br>
 * @param name
 * attribute name to which method applies.
 * @param mfunc
 * function implementing custom merge method.
 */
DECL_GA merge_func set_merge_method(char const * name, merge_func mfunc);


/**
 * Registers a function which implements a custom <tt>trans_owner</tt> method for generic attributes with the specified name.
 * <br><br>
 * <b>Role:</b> Returns old trans method, if any.
 * <br><br>
 * @param name
 * attribute name to which method applies.
 * @param tfunc
 * function implementing custom transform method.
 */
DECL_GA trans_func set_trans_method(char const *name, trans_func tfunc);


/**
 * Registers a function which implements a custom <tt>copy_owner</tt> method for generic attributes.
 * <br><br>
 * <b>Role:</b> Returns old copy method, if any.
 * <br><br>
 * @param name
 * attribute name to which method applies.
 * @param cfunc
 * function implementing custom copy method.
 */
DECL_GA copy_func set_copy_method(char const *name, copy_func cfunc);


// Retrieve enumeration value/string tables for use in the test harness, etc.
/**
 * @nodoc
 */
DECL_GA void get_attrib_enum_tables(enum_table const *&,
									enum_table const *&,
									enum_table const *&,
									enum_table const *&);


///////////////////////////////////////////////////////////////////////////////
//
//  Other functions to work with generic ATTRIB_GEN_NAMEs
//
///////////////////////////////////////////////////////////////////////////////
/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	int,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	double,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	const char *,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	const SPAWCHAR_T *,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	SPAposition &,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	SPAvector &,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	ENTITY *,
	split_action=SplitKeep,
	merge_action=MergeKeepKept,
	trans_action=TransIgnore,
	copy_action=CopyCopy
    );

/**
 * @nodoc
 */
DECL_GA void
remove_generic_named_attribute(
    ENTITY* ent,
    const char* name
    );

/**
 * @nodoc
 */
DECL_GA int new_attribute_id();

 /** @} */
#endif
