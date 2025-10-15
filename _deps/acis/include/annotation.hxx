// $Id: annotation.hxx,v 1.40 2002/08/09 17:15:16 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// =============================================================================
// IncludeFile: annotation.hxx		Master Entity for Code Annotations
// -----------------------------------------------------------------------------

// Annotation entities are used to add semantic information to the
// bulletin boards.  The information is added in the form of create
// bulletins for derived types of this entity.  The actual semantic
// information depends on the algorithm being annotated, so there will
// be a number of derived types.

#if !defined( ANNOTATION_CLASS )
#define ANNOTATION_CLASS

#include "dcl_kern.h"
#include "entity.hxx"
#include "bullsmal.hxx"
#include "attrib.hxx"
#include "at_sys.hxx"
#include "option.hxx"
#include "att_tag.hxx"
#include "api.hxx"
/**
* \defgroup ACISANNOTATIONS Annotations
* \ingroup KERNAPI 
*
*/
/**
 * @file annotation.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISANNOTATIONS
 *
 * @{
 */

// The ARRAY_BASED_ANNOTATION == FALSE branch of several #if's was in
// the code on the main trunk as of 19980720, but has been removed.
// Use cvs update -D if you need to see it for any reason.
// AlanG of tsl has verified that ARRAY_BASED_ANNOTATION == TRUE works for them.

//#if defined( TSL )
//#define ARRAY_BASED_ANNOTATION FALSE
//#else
/**
 * @nodoc
 */
#define ARRAY_BASED_ANNOTATION TRUE
//#endif

#include "annotation_mac.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ANNOTATION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
#define ANNOTATION_LEVEL ( ENTITY_LEVEL + 1)

extern DECL_KERN int ANNOTATION_TYPE;


class ENTITY_LIST;
class ENTITY_LIST_DArray;

#ifdef THREAD_SAFE_ACIS
extern DECL_KERN safe_object_pointer<ENTITY_LIST> annotation_list;
#else
extern DECL_KERN ENTITY_LIST *annotation_list;
#endif


/**
 * @nodoc
 */
#define DEBUG_ASSERT

/**
 * @nodoc
 */
#define DEF_DARRAY_SIZE 4

/**
 * @nodoc
 */
class DECL_KERN ENTITY_LIST_DArray : public ACIS_OBJECT {
protected:
	ENTITY_LIST** m_pData;
	int m_nSize;
	int m_nMaxSize;

public:

	ENTITY_LIST_DArray(int nSize=0);
	~ENTITY_LIST_DArray();

	int GetSize() const { return m_nSize; }
	int GetMaxSize() const { return m_nMaxSize; }

	void SetSize(int nNewSize);
	void FreeExtra();

	ENTITY_LIST* operator [] (int nIndex) const;
	ENTITY_LIST*& operator [] (int nIndex);

	ENTITY_LIST* operator *() const;
	ENTITY_LIST*& operator *();

	ENTITY_LIST*& operator ++();
	ENTITY_LIST*& operator ++(int);

	ENTITY_LIST*& operator --();
	ENTITY_LIST*& operator --(int);

private: //not allowed to be used..
	ENTITY_LIST_DArray(const ENTITY_LIST_DArray&) {}
	ENTITY_LIST_DArray& operator = (const ENTITY_LIST_DArray&) { return *this; }
};



// MASTER_ENTITY_DECL( ANNOTATION, KERN ) is almost what we need, but
// we also need to define the virtual member_lost function, which is called
// by ATTRIB_ANNOTATION::lose()

class ATTRIB_ANNOTATION;

/**
 * Defines a base class for adding information to the bulletin boards.
 * <br>
 * <b>Role:</b> The <tt>ANNOTATION</tt> class is a base class derived from <tt>ENTITY</tt>. ACIS code has been
 * enhanced to add calls to construct the <tt>ANNOTATION</tt> entities when the information is known.
 * <br><br>
 * The member data in the annotations is classified into inputs and outputs. Query
 * functions within <tt>ANNOTATION</tt> populate an <tt>ENTITY_LIST</tt> with the inputs or outputs. Each
 * derived <tt>ANNOTATION</tt> type has more specific query functions for full detail.
 * <br><br>
 * Annotation entities are used to add semantic information to the bulletin boards.
 * The information is added in the form of create bulletins for derived types of this entity.
 * The actual semantic information depends on the algorithm being annotated, so there will be
 * a number of derived types.
 * @see ATTRIB_ANNOTATION
 */

class DECL_KERN ANNOTATION : public ENTITY
{
    logical m_members_are_hooked;
	void *m_user_data; // Not saved copied or rolled

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS(ANNOTATION, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    /*
	// tbrv
	*/
	/**
	 * @nodoc
 	*/
	void member_lost_helper(ENTITY*& entToCheck, const ENTITY* entity);
	/*
	// tbrv
	*/
	/**
	 * @nodoc
 	*/
	void inputs_helper(ENTITY* parent, ENTITY_LIST& list, logical no_tags = TRUE) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
 	*/
	void debug_helper(const char* ent_name, ENTITY* ent, FILE* fp) const;

public:

/**
 * Constructs an <tt>ANNOTATION</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
	ANNOTATION();
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
	ROLL_NOTIFY_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL_NO_ENTS()
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	// We have no extra data at this level,
	// but derived classes will use these stubs
	/**
	 * @nodoc
	 */
	ANNOTATION_EXTRA_DATA_FUNCTIONS(ANNOTATION)
#if 0
;
#endif
// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>ANNOTATION</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method, 
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    if (restore_version_number >= ANNO_HOOKED_VERSION)
 *       read_logical        m_members_are_hooked (unhooked or hooked) 
 * </pre>
 */
    void restore_common();
#endif

// These function are hidden from mkman in the ANNOTATION_EXTRA_DATA_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Reads information regarding this <tt>ANNOTATION</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>ANNOTATION_RESTORE_EXTRA_DEF</tt> macro expands to the <tt>restore_extra</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 *    <tt>No data</tt>&nbsp;&nbsp;&nbsp;&nbsp;This class does not save any data
 */
	void restore_extra();
#endif

// These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>ANNOTATION_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>ANNOTATION_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>ANNOTATION_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string <tt>"annotation"</tt>.
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating this <tt>ANNOTATION</tt> is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

// These function are hidden from mkman in the ANNOTATION_MAP_DECL_NO_ENTS macro; to have them documented,
// we include them here:
#if 0
/**
 * Re-implements <tt>get_entity_by_name</tt> and <tt>set_entity_by_name</tt> so they don't have to be virtual stacks.
 * <br><br>
 * <b>Role:</b> The default for <tt>isInput</tt> is <tt>FALSE</tt>.
 * <br><br>
 * @param name
 * name of the ENTITY.
 * @param isInput
 * flag to get (FALSE) or set (TRUE) the ENTITY.
 */
	virtual ENTITY*& find_entity_ref_by_name(const char* name, logical& isInput);
/**
 * Returns a pointer to the <tt>ENTITY</tt> specified by the given name.
 * <br><br>
 * @param name
 * name of the ENTITY.
 */
	virtual ENTITY* get_entity_by_name(const char* name);
/**
 * Returns a list of <tt>ENTITYs</tt> that are the inputs to the modeling operation.
 * <br><br>
 * @param list
 * list of inputs.
 * @param no_tags
 * flag to use tags or not.
 */
	virtual void inputs(ENTITY_LIST& list, logical no_tags = TRUE) const;
/**
 * Returns <tt>TRUE</tt> if the named <tt>ENTITY</tt> is the given <tt>ENTITY</tt>.
 * <br><br>
 * @param name
 * name of the ENTITY.
 * @param entity
 * the ENTITY to test.
 */
	virtual logical is_entity_by_name( const char* name, ENTITY* entity);
/**
 * Returns the name of the member.
 * <br><br>
 * @param entity
 * the ENTITY whose name is desired.
 */
	virtual const char* member_name(const ENTITY* entity) const;
/**
 * Returns an <tt>ENTITY</tt> list that is the output result of the modeling operation.
 * <br><br>
 * @param list
 * list of outputs.
 */
	virtual void outputs(ENTITY_LIST& list) const;
/**
 * Specifies the name for the given <tt>ENTITY</tt>.
 * <br><br>
 * @param name
 * name of the ENTITY.
 * @param value
 * the ENTITY whose name is specified.
 */
	virtual void set_entity_by_name( const char* name, ENTITY* value);
#endif

/**
 * Returns whether or not the output array is empty.
 */
 	logical empty() const;

/**
 * Returns either "unhooked" or "hooked".
 */
    logical members_are_hooked() const { return m_members_are_hooked; }

	// The ANNOTATION is owned by it's ANNOTATION_ATTRIBs so say not deletable
/**
 * Queries whether or not this <tt>ANNOTATION</tt> can be deleted.
 * <br><br>
 * <b>Role:</b> If the annotation is owned by its <tt>ANNOTATION_ATTRIB</tt>, then it is not deletable.
 */
	virtual logical deletable() const;

/**
 * Returns a pointer to user data.
 */
	void* user_data() const { return m_user_data; }

/**
 * Specifies a pointer to user data.
 * <br><br>
 * @param ud
 * pointer to the data.
 */
	void set_user_data(void* ud) { backup(); m_user_data = ud; }

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void member_lost_hook(const ENTITY* /* member */) {}

protected:
	// Add an attribute to the specified entity, that points to this annotation
	// don't allow more than one attribute on the same entity that points here.
/**
 * Adds an attribute to the specified <tt>ENTITY</tt> that points to this <tt>ANNOTATION</tt>.
 * <br><br>
 * @param e
 * the ENTITY to which the attribute is added.
 */
	void add_attribute(const ENTITY* e);

	// Hook an attribute to the entity, taking into account if it is an ATTRIB_TAG
	// and adding to the owner instead of so
/*
// tbrv
*/
/**
 * @nodoc
 */
	void hook(ENTITY* e);

	// Remove the attribute from the specified entity (or owner of the ATTRIB_TAG)
	// w/o calling the member_lost function.
/**
 * Removes the attribute from the specified entity (or owner of the <tt>ATTRIB_TAG</tt>) without calling the <tt>member_lost</tt> function.
 * <br><br>
 * @param entity
 * the ENTITY from which the attribute is unhooked.
 */
	void unhook(ENTITY* entity);

	// Is there an ATTRIB_ANNOTATION for this ANNOTATION on the given ENTITY?
	// If so, return it.  Else, return NULL
/**
 * Returns the <tt>ATTRIB_ANNOTATION</tt> for this <tt>ANNOTATION</tt> on the given <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>NULL</tt> otherwise.
 * <br><br>
 * @param entity
 * the ENTITY to check.
 */
	ATTRIB_ANNOTATION* find_attrib_annotation(const ENTITY* entity);

	// Set an entity pointer
/**
 * Specifies a pointer to an output <tt>ENTITY</tt>.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param newEnt
 * the ENTITY pointer.
 */
	void set_output_entity(ENTITY*& eToSet, ENTITY* newEnt);
/**
 * Specifies a pointer to an input <tt>ENTITY</tt>.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param newEnt
 * the ENTITY pointer.
 */
	void set_input_entity(ENTITY*& eToSet, ENTITY* newEnt);

	// Add another one, converting an ENTITY* to an EE_LIST
/**
 * Adds another output entity to the list.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param newEnt
 * the ENTITY to add.
 */
	void add_output_entity(ENTITY*& eToSet, ENTITY* newEnt);
/**
 * Adds another input entity to the list.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param newEnt
 * the ENTITY to add.
 */
	void add_input_entity(ENTITY*& eToSet, ENTITY* newEnt);

	// Remove an entity, converting an EE_LIST with two entries
	// to a simple ENTITY* holding the remaining entity
/**
 * Removes an output entity.
 * <br><br>
 * <b>Role:</b> Converts an <tt>EE_LIST</tt> with two entries to a simple <tt>ENTITY*</tt> holding the
 * remaining ENTITY.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param oldEnt
 * the ENTITY to remove.
 */
	void remove_output_entity(ENTITY*& eToSet, ENTITY* oldEnt);
/**
 * Removes an input entity.
 * <br><br>
 * <b>Role:</b> Converts an <tt>EE_LIST</tt> with two entries to a simple <tt>ENTITY*</tt> holding the
 * remaining <tt>ENTITY</tt>.
 * <br><br>
 * @param eToSet
 * pointer to the ENTITY list.
 * @param oldEnt
 * the ENTITY to remove.
 */
	void remove_input_entity(ENTITY*& eToSet, ENTITY* oldEnt);

public:
	// Query functions.
/**
 * Returns <tt>TRUE</tt> if one <tt>ENTITY</tt> contains the other.
 * <br><br>
 * @param eToCheck
 * the ENTITY to check.
 * @param entity
 * pointer to an ENTITY.
 * @param eToCheck_guaranteed_alive
 * check alive flag.
 */
	logical contains_this_entity(ENTITY*const& eToCheck, const ENTITY* entity,
                                 logical eToCheck_guaranteed_alive = FALSE) const;

/*
// tbrv
*/
/**
 * @nodoc
 */
 	void remove_member(ENTITY* entity) { member_lost(entity); unhook(entity); }

	// The attrib notifications are passed on via the ATTRIB_ANNOTATION

	// Two virtual functions aid annotation migration during modelling
	// operations.  These functions are supplied by the application
	// whenever it defines a new annotation.  They are called via the ATTRIB_ANNOTATION
	// whenever a split or merge occurs, e.g. during booleans.

	// split_member: some member of this annotation is being split
	// into two parts, the new piece being "new_entity".  The
	// application has the chance to delete, duplicate or otherwise
	// modify the annotations.  The default action is to do nothing.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void split_member(
				ENTITY *,	// the member entity being split
				ENTITY *	// "new entity" arising from split
			);


	// merge_member: some member of this annotaion is about to be merged
	// with the "other_entity".  After the merge, the member will be
	// deleted if the logical "deleting_member" is TRUE, otherwise it
	// will be retained and "other_entity" will be deleted.
	// The default action is to do nothing.  Processing of the actual delete
	// can be defered to the member_lost function.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void merge_member(
				ENTITY *,	// the first entity being merged
				ENTITY *,	// "other entity"
				logical		// TRUE if first entity will be deleted
			);


	// A further function to inform the annotation that one of its
	// members it being transformed.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void trans_member(
				ENTITY *,	// the entity being transformed
				SPAtransf const &
			);

	// A further function to inform the annotation that one of its
	// members it being warped.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void warp_member(
				ENTITY *,	// the entity being warped
				law *
			);
};

// This attribute gets attatched to the ENTITY(s) being annotated
// and points to the annotation.  The lose method calls the virtual
// member delete method on the annotation, letting it know the
// ENTITY did not survive to the end of the BULLETIN_BOARD.

/**
 * @nodoc
 */
#define ATTRIB_ANNOTATION_LEVEL (ATTRIB_SYS_LEVEL + 1)
extern DECL_KERN int ATTRIB_ANNOTATION_TYPE;

/**
 * Attaches an annotation to an entity.
 * <br>
 * <b>Role:</b> During a modeling operation, annotations are attached to the annotated
 * entities via <tt>ATTRIB_ANNOTATION</tt>. This allows quick updating of the annotations as the
 * operation progresses. These attributes are normally removed when the operation completes,
 * but they may be retained by setting the <tt>unhook_annotations</tt> option to <tt>FALSE</tt>.
 * <br><br>
 * This attribute gets attached to the <tt>ENTITY</tt>(s) being annotated and points to the annotation.
 * The <tt>lose</tt> method calls the virtual member <tt>delete</tt> method on the annotation, letting it know
 * the <tt>ENTITY</tt> did not survive to the end of the <tt>BULLETIN_BOARD</tt>.
 * @see ANNOTATION
 */

class DECL_KERN ATTRIB_ANNOTATION : public ATTRIB_SYS
{
	ANNOTATION* m_annotation;
public:
/**
 * Constructs an ATTRIB_ANNOTATION from a given ANNOTATION, and attaches it to a given
 * owning ENTITY.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * the owning ENTITY for the constructed attribute.
 * @param a
 * the ANNOTATION contained in the constructed attribute.
 */
	ATTRIB_ANNOTATION(const ENTITY* e = NULL, ANNOTATION* a = NULL);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_ANNOTATION, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>ATTRIB_ANNOTATION</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    read_ptr        Pointer to annotation data. </pre>
 */
    void restore_common();
#endif

// These function are hidden from mkman in the ATTRIB_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>ATTRIB_ANNOTATION_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>ATTRIB_ANNOTATION_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>ATTRIB_ANNOTATION_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string <tt>"attrib_annotation"</tt>.
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating this <tt>ATTRIB_ANNOTATION</tt> is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

/**
 * Returns a pointer to the <tt>ANNOTATION</tt> stored in this <tt>ATTRIB_ANNOTATION</tt>.
 */
	ANNOTATION* annotation() const { return m_annotation; }

/**
 * Specifies the <tt>ANNOTATION</tt> to associate with this <tt>ATTRIB_ANNOTATION</tt>.
 * <br><br>
 * @param a
 * the ANNOTATION to use.
 */
	void set_annotation(ANNOTATION* a) { backup(); m_annotation = a; }

/**
 * @nodoc
 */
    virtual logical pattern_compatible() const; // documented in base class

	// Annotations attached via this attributes can be saved, but not copied
	// If they copied, they would end up refering to data from the source
	// entity.  No way to translate the data to equivalent stuff in the target.
/**
 * Returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Role:</b> Annotations attached via this attribute can be saved, but not copied.
 * If copied, they would end up referring to data from the source <tt>ENTITY</tt>.
 */
	virtual logical copyable() const;

/**
 * Indicates whether this <tt>ATTRIB</tt> should be included when the owning entity is saved.
 * <br><br>
 * <b>Role:</b>  Returns <tt>TRUE</tt> unless <tt>this</tt> attribute references a
 * <tt>PATTERN_ANNOTATION</tt>.
 */
	virtual logical savable() const;
	// Two virtual functions aid attribute migration during modelling
	// operations.  These functions are supplied by the application
	// whenever it defines a new attribute.  They are called by Acis
	// whenever a split or merge occurs, e.g. during booleans.

	// split_owner: the owner entity of this attribute is being split
	// into two parts, the new piece being "new_entity".  The
	// application has the chance to delete, duplicate or otherwise
	// modify the attribute.  The default action is to do nothing.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void split_owner(
				ENTITY *	// new entity arising from split
			);


	// merge_owner: the owner of this attribute is about to be merged
	// with the "other_entity".  After the merge, this owner will be
	// deleted if the logical "deleting_owner" is TRUE, otherwise it
	// will be retained and "other_entity" will be deleted.
	// The default action is to do nothing.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void merge_owner(
				ENTITY *,	// "other entity"
				logical		// TRUE if owner of this attribute
							// will be deleted
			);


	// A further function to allow geometric attributes to transform
	// themselves when their owner is transformed.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void trans_owner(
				SPAtransf const &
			);

	// A further function to allow geometric attributes to warp
	// themselves when their owner is warpped.
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void warp_owner(
				law *
			);

};

/**
 * @nodoc
 */
DECL_KERN void push_annotation_list(void);

/**
 * @nodoc
 */
DECL_KERN void pop_annotation_list(void);

/**
 * @nodoc
 */
DECL_KERN ANNOTATION* find_annotation(const ENTITY* entity, int type, int level);

/**
 * @nodoc
 */
DECL_KERN ANNOTATION* find_annotation(const ENTITY* entity, logical (*is_func)(const ENTITY*),
									  const char* name = NULL, ENTITY* value = NULL);
/**
 * @nodoc
 */
DECL_KERN void find_all_annotations(ENTITY_LIST& matching_annotations,
									const ENTITY* entity, logical (*is_func)(const ENTITY*),
									const char* name = NULL, ENTITY* value = NULL);

// A function to clear annotations from the currently open bulletin board.
/**
 * @nodoc
 */
DECL_KERN outcome clear_annotations_from_curr_bb();

// A flag to control whether annotations are generated
//class option_header;
//extern DECL_KERN option_header annotations;


inline ANNOTATION* __hookup__(ANNOTATION* a) {
	a->hook_members();
	return a;
}

#define ANNOTATE(x) \
( \
	annotations.on() \
	? \
	( \
		__hookup__(ACIS_NEW x) \
	) \
	: \
	( \
		NULL \
	) \
)

// We thought about it and this is intentionally not exported.
/**
 * @nodoc
 */
int initialize_annotations();

// Not exported - use API
/**
 * @nodoc
 */
int clear_annotations();
/** @} */
#endif
