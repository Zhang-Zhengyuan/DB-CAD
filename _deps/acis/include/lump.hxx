/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the LUMP class.
// The lump is a connected portion of a body.  Usually a body will
// consist of a single lump (which is bounded by an outer shell and
// zero, one, or more inner shells representing voids within the body.)
// However when an operation such as boolean subtraction or intersection
// returns a body composed of several unconnected pieces, each piece
// will be represented by a lump.
#ifndef LUMP_CLASS
#define LUMP_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
/**
 * @file lump.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
class SPAbox;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(LUMP, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int LUMP_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define LUMP_LEVEL 1


// LUMP declaration proper.
/**
 * Represents a bounded, connected portion of space.
 * <br>
 * <b>Role:</b> A lump represents a connected portion of a body. Usually a body consists
 * of a single lump that is bounded by an outer shell and zero, one, or more inner shells
 * representing voids within the body. However, when an operation such as a Boolean
 * subtraction or intersection returns a body composed of several unconnected pieces,
 * each piece is represented by a lump.
 * @see BODY, SHELL
 */
class DECL_KERN LUMP : public ENTITY {

    // Overrides of virtual ENTITY methods relating to patterns
    // (see entity.hxx for descriptions):
protected:
    /**
     * Returns TRUE if this entity can have a pattern_holder. For internal use only.
     */
    logical holds_pattern() const { return TRUE; }
public:
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt>.
     */
    logical patternable() const { return TRUE; }// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> <tt>api_pattern_apply_to_entity</tt> should be used to apply patterns to lumps.
     * <br><br>
     * <b>Role:</b> Sets the current pattern.
     * <br><br>
     * @param in_pat
     * pattern.
     * @param keep_pointer
     * internal use only.
     */
    void set_pattern(pattern* in_pat, logical keep_pointer = TRUE);// internal use only

    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Returns all patterns in the list.
     * <br><br>
     * @param list
     * pattern list.
     */
    void get_all_patterns(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    logical attach_pattern_attrib(pattern* pat, logical include_children = TRUE);// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Copies the pattern through all children of the target entity.
     * <br><br>
     * @param target
     * target.
     */
    logical copy_pattern_down(ENTITY* target) const;// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if this is a pattern child. An entity is a "pattern child"
     * when it is not responsible for creating new entities when the pattern is applied.
     * Instead, some owning entity takes care of this.
     */
    logical is_pattern_child() const;// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes the pattern element associated with this entity from the pattern.
     * Returns <tt>FALSE</tt> if this entity is not part of a pattern element, otherwise <tt>TRUE</tt>.
     * The affected entities are not destroyed, but are merely made independent of the pattern. The
     * pattern itself is correspondingly modified to "drop out" the newly disassociated
     * element.
     */
    logical remove_from_pattern();// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes this entity from the list of entities maintained by its pattern,
     * if any. Returns <tt>FALSE</tt> if no pattern is found, otherwise returns <tt>TRUE</tt>.
     */
    logical remove_from_pattern_list();// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes the pattern on this and all associated entities. Returns <tt>FALSE</tt>
     * if no pattern is found, otherwise returns <tt>TRUE</tt>.
     */
    logical remove_pattern();// internal use only
    // End overrides


    // Other pattern-related methods:
/**
 * @nodoc
 */
    void get_all_pattern_holders(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    logical get_parent_pattern_holders(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    logical process_all_pat_holder_attribs(pattern_holder* in_ph);// internal use only
    /**
     * @nodoc
     */
    logical transfer_all_pat_holder_attrib_caches();// internal use only
    /**
     * @nodoc
     */
    logical contains_pattern() const;// internal use only

    // End other pattern-related

    //
    // DEBUG FUNCTIONS:
    //

    // method to write scheme commands to file for visualization
    // [in] pFp   - the file to write
    // [in] Color - color of the entity drawn
    //
/**
 * @nodoc
 */
    virtual void debug_scheme(FILE* pFp, int Color) const;

private:

    // List pointer linking all the lumps in a body (last member
    // of list has this pointer NULL).

    LUMP* next_ptr;


    // Start of a list of shells that comprise the lump.

    SHELL* shell_ptr;


    // Pointer to the owning body.

    BODY* body_ptr;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire lump lies (with respect to the internal coordinate
    // system of its body).
    // It may be NULL if no such bound has been calculated since the
    // lump was last changed.

    // Contains the use counted box.
    entity_box_container box_container;

    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * @nodoc
     */
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;

    // STI ROLL end

        // Include the standard member functions for all entities.

        /**
         * @nodoc
         */
    ENTITY_FUNCTIONS(LUMP, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

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


    // Now the functions specific to LUMP.

    // Basic constructor, used internally for bulletin board activity.

/**
 * Constructs a LUMP (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    LUMP();
    LUMP(const char* gme);


    // Public constructor, which initialises all the class data, and
    // records the creation in the bulletin board.  The first argument
    // is the start of a lists of shells contained in the lump, and
    // the second argument sets the pointer to the next lump in the
    // owning body.  The constructor sets the backpointers in the
    // shells in the given list.  The calling routine must set
    // body_ptr and if desired, bound_ptr, using set_body() and
    // set_bound() [declared below].

/**
 * Constructs a <tt>LUMP</tt> from a list of <tt>SHELL</tt>s and a connected <tt>LUMP</tt> already in the
 * owning <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * This constructor initializes all the class data, and records the creation in the
 * bulletin board. The first argument is the start of a list of <tt>SHELL</tt>s contained in
 * the <tt>LUMP</tt>, and the second argument sets the pointer to the next <tt>LUMP</tt> in the owning
 * <tt>BODY</tt>. The constructor sets the backpointers in the <tt>SHELL</tt>s in the given list. The
 * calling routine must set <tt>body_ptr</tt> and, if desired, <tt>bound_ptr</tt>,
 * using <tt>set_body</tt> and <tt>set_bound</tt>.
 * <br><br>
 * @param shell
 * pointer to a list of SHELLs to be contained by the constructed LUMP.
 * @param lump
 * next LUMP in the owning BODY.
 */
    LUMP(SHELL* shell, LUMP* lump);
    LUMP(const char* gme, SHELL* shell, LUMP* lump);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores <tt>this</tt> <tt>LUMP</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br><tt>
 * &nbsp;&nbsp;&nbsp;if (restore_version_number >= PATTERN_VERSION<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_ptr  </tt>APATTERN index<br><tt>
 * &nbsp;&nbsp;&nbsp;if (apat_idx != (APATTERN*)(-1)))<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;restore_cache();<br>
 * &nbsp;&nbsp;&nbsp;read_ptr  </tt>Pointer to record in save file for next LUMP in body<br><tt>
 * &nbsp;&nbsp;&nbsp;read_ptr  </tt>Pointer to record in save file for first SHELL in lump<br><tt>
 * &nbsp;&nbsp;&nbsp;read_ptr  </tt>Pointer to record in save file for BODY containing lump
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for <tt>this</tt> object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>LUMP_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>LUMP_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>LUMP_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"lump"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating <tt>this</tt> <tt>LUMP</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the next <tt>LUMP</tt> in the list of <tt>LUMP</tt>s contained in a body.
 * <br><br>
 * <b>Role:</b> The <tt>next_type</tt> argument controls how the next method treats
 * patterns, and can take any one of three values:
 * <br><br>
 * <dl>
 * <dt><tt>PAT_CAN_CREATE</tt>:</dt><dd>If the next lump is to be generated from a pattern, create it
 *                    if it doesn't yet exist and return its pointer.</dd>
 * <dt><tt>PAT_NO_CREATE</tt>:</dt><dd>If the next lump is to be generated from a pattern, but hasn't
 *                    yet been created, bypass it and return the pointer of the next
 *                    already-created lump (if any).</dd>
 * <dt><tt>PAT_IGNORE</tt>:</dt><dd>behave as though there is no pattern on the lump.</dd>
 * </dl><br>
 * @param next_type
 * (see Role).
 */
    LUMP* next(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;
    /**
     * Returns a pointer to the first <tt>SHELL</tt> contained in <tt>this</tt> <tt>LUMP</tt>.
     */
    SHELL* shell() const;
    /**
     * Returns a pointer to the containing <tt>BODY</tt>.
     */
    BODY* body() const { return body_ptr; }
    /**
     * Returns a geometric bounding region (box) for <tt>this</tt> <tt>LUMP</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>LUMP</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The pointer is <tt>NULL</tt> if a bound was not
     * calculated since the <tt>LUMP</tt> was last modified.
     */
    SPAbox* bound() const { return box_container.get_box(); }

    /**
     * Returns a pointer to the owning <tt>BODY</tt>.
     */
    ENTITY* owner() const;


    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets <tt>this</tt> <tt>LUMP</tt>'s next <tt>LUMP</tt> pointer to the given <tt>LUMP</tt>.
 * <br><br>
 * <b>Role:</b> Before performing a change, it checks if the data structure is posted
 * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
 * bulletin board.
 * <br><br>
 * @param lump
 * the new next LUMP.
 * @param reset_pattern
 * internal use only.
 */
    void set_next(LUMP* lump, logical reset_pattern = TRUE);
    /**
     * Sets <tt>this</tt> <tt>LUMP</tt>'s <tt>SHELL</tt> pointer to the given <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param shell
     * the new SHELL.
     * @param reset_pattern
     * internal use only.
     */
    void set_shell(SHELL* shell, logical reset_pattern = TRUE);
    /**
     * Sets <tt>this</tt> <tt>LUMP</tt>'s <tt>BODY</tt> backpointer to the specified <tt>BODY</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param body
     * the new owning BODY.
     * @param reset_pattern
     * internal use only.
     */
    void set_body(BODY* body, logical reset_pattern = TRUE);
    /**
     * Sets the bounding region (box) of <tt>this</tt> <tt>LUMP</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>LUMP</tt>'s <tt>SPAbox</tt> pointer to point to the given <tt>SPAbox</tt>.
     * Before performing the change, it checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }

public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_shell_ptr();
    void** gme_get_body_ptr();
};
/** @} */
#endif
