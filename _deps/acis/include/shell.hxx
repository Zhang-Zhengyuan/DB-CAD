/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the SHELL class.
// The shell is a connected portion of a lump's boundary.  It has no
// physical or topological connection with any other shell.  (The latter
// imples that one can not traverse the topological structure of one
// shell and end up on another shell.) If a lump has no voids, then
// exactly one shell gives its overall extent; any other shells bound
// voids wholly within the lump.  There is no distinction made in the
// data structure between peripheral and void shells.  In this
// context a shell is closed and bounded.

// It is technically possible for a shell to be open and bounded,
// or unbounded.  In the former case, the containing lump (and body)
// is said to be "incomplete" (or, more accurately "incompletely
// bounded").  It interacts with other bodies only insofar as the
// defined portions of their shells interact, and there are
// configurations of that interaction which are disallowed.  If the
// shell is unbounded, it may be semi-infinite (for example a plane
// bounded by a single infinite straight line) or infinite (two such
// half-infinite planes joined at their boundaries).  In the former
// case, again the body is "incomplete", while in the latter case it
// is completely defined, though of infinite extent.  Although it
// is possible to represent such incomplete lumps and bodies, most
// algorithms in ACIS do not operate on such entities.

// The concepts of "peripheral" and "void" shells, and of "connected"
// and "disjoint" bodies, have no meaning when applied to incomplete
// lumps or bodies.

// A shell is constructed from a collection of "faces" and "wires".
// This collection, if large, may be subdivided into a hierarchy of
// "subshells", each containing a proper subcollection.  A shell
// subdivided into subshells may also contain faces and wires directly;
// in this case, these entities are not contained in any subshell.
#ifndef SHELL_CLASS
#define SHELL_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
/**
 * @file shell.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

class SPAbox;
// ROP
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SHELL, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int SHELL_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define SHELL_LEVEL 1


// SHELL declaration proper.
/**
 * Represents the external boundary of a <tt>LUMP</tt>, or the internal boundary of a void (unoccupied space) within a <tt>LUMP</tt>.
 * <br>
 * <b>Role:</b> The shell is a connected portion of a lump's boundary. It has no
 * physical or topological connection with any other shell. It is not possible to
 * traverse the topological structure of one shell and end up on another shell.
 * If a lump has no voids, then exactly one shell gives its overall extent. Any
 * other shells bound voids wholly within the lump. There is no distinction made
 * in the data structure between peripheral and void shells. In this context a
 * shell is closed and bounded.
 * <br><br>
 * It is technically possible for a shell to be open and bounded or unbounded.
 * If bounded, the containing lump (and body) is considered <i>incomplete</i>, or more
 * accurately, <i>incompletely bounded</i>. It interacts with other bodies only so far as
 * the defined portions of their shells interact. There are configurations of that
 * interaction that are disallowed. If the shell is unbounded, it can be
 * semi-infinite (e.g., a plane bounded by a single infinite straight line) or
 * infinite (e.g., two half-infinite planes joined at their boundaries). If the
 * shell is semi-infinite, the body is incomplete, while an infinite shell is
 * completely defined, though of infinite extent.
 * <br><br>
 * The concepts of <i>peripheral</i> and <i>void</i> shells, and of <i>connected</i>
 * and <i>disjoint</i> bodies have no meaning when applied to incomplete lump or body.
 * <br><br>
 * A shell is constructed from a collection of "faces" and "wires." Large collections
 * may be subdivided into a hierarchy of "subshells," each containing a proper
 * subcollection. A shell subdivided into subshells may also contain faces and wires
 * directly; in this case, these entities are not contained in any subshell.
 * @see FACE, LUMP, SUBSHELL, WIRE
 */
class DECL_KERN SHELL : public ENTITY {

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
     * <b>Obsolete:</b> <tt>api_pattern_apply_to_entity</tt> should be used to apply patterns to shells.
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
     * list.
     */
    void get_all_patterns(VOID_LIST& list);// internal use only
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
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes the pattern on this and all associated entities. Returns <tt>FALSE</tt>
     * if no pattern is found, otherwise <tt>TRUE</tt>.
     */
    logical remove_pattern();// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes this entity from the list of entities maintained by its pattern,
     * if any. Returns <tt>FALSE</tt> if no pattern is found, otherwise <tt>TRUE</tt>.
     */
    logical remove_from_pattern_list();// internal use only
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
     */
    logical attach_pattern_attrib(pattern* pat, logical include_children = TRUE);// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes the pattern element associated with this entity from the pattern.
     * Returns <tt>FALSE</tt> if this entity is not part of a pattern element, otherwise <tt>TRUE</tt>.
     * <br><br>
     * <i><b>Note:</b> The affected entities are not destroyed, but are merely made independent of
     * the pattern. The pattern itself is correspondingly modified to "drop out" the newly disassociated
     * element.</i>
     */
    logical remove_from_pattern();// internal use only

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

    // List pointer linking all the shells in a body (last member
    // of list has this pointer NULL).

    SHELL* next_ptr;


    // Start of a list of immediately-contained subshells (which in
    // turn contain faces and wires, and further subshells).

    SUBSHELL* subshell_ptr;


    // Start of a list of immediately-contained faces.

    FACE* face_ptr;


    // Start of a list of immediately-contained wires.

    WIRE* wire_ptr;


    // Pointer to the owning lump - shells in separate lumps are
    // themselves entirely separate.

    LUMP* lump_ptr;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire shell lies (with respect to the internal coordinate
    // system of its body).
    // It may be NULL if no such bound has been calculated since the
    // shell was last changed.

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
    ENTITY_FUNCTIONS(SHELL, KERN)
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


    // Now the functions specific to SHELL.

    // Basic constructor, used internally for bulletin board activity.

/**
 * Constructs a <tt>SHELL</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    SHELL();
    SHELL(const char* gme);


    // Public constructor, which initialises all the class data, and
    // records the creation in the bulletin board.  The first two
    // arguments are the starts of lists of faces and subshells
    // contained, and the last is a list of "sister" shells already
    // in the current lump.  The calling routine must set lump_ptr and
    // if desired, wire_ptr and bound_ptr, using set_lump(), set_wire()
    // and set_bound() [declared below].

/**
 * Constructs a <tt>SHELL</tt> from lists of <tt>FACE</tt>s and <tt>SUBSHELL</tt>s and a connected <tt>SHELL</tt>
 * already in the owning <tt>LUMP</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * This constructor initializes all the class data, and records the creation in the
 * bulletin board. The first two arguments are the starts of lists of <tt>FACE</tt>s and
 * <tt>SUBSHELL</tt>s contained in the <tt>SHELL</tt>, and the third argument sets the pointer to the
 * next <tt>SHELL</tt> in the owning <tt>LUMP</tt>. The calling routine must set <tt>lump_ptr</tt> and,
 * if desired, <tt>bound_ptr</tt>, using <tt>set_lump</tt> and <tt>set_bound</tt>.
 * <br><br>
 * @param face
 * pointer to a list of FACEs to be contained by the constructed SHELL.
 * @param subshell
 * pointer to a list of SUBSHELLs to be contained by the constructed SHELL.
 * @param shell
 * next SHELL in the owning LUMP.
 */
    SHELL(FACE* face, SUBSHELL* subshell, SHELL* shell);
    SHELL(const char* gme, FACE* face, SUBSHELL* subshell, SHELL* shell);

    /**
     * Constructs a <tt>SHELL</tt> from lists of <tt>WIRE</tt>s and <tt>SUBSHELL</tt>s and a connected <tt>SHELL</tt>
     * already in the owning <tt>LUMP</tt>.
     * <br><br>
     * <b>Role:</b> Requests memory for this object and populates it with the data
     * supplied as the argument. Applications should call this constructor only with
     * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
     * a requirement to support roll back and history management.
     * <br><br>
     * This constructor initializes all the class data, and records the creation in the
     * bulletin board. The first two arguments are the starts of lists of <tt>WIRE</tt>s and
     * <tt>SUBSHELL</tt>s contained in the <tt>SHELL</tt>, and the third argument sets the pointer to the
     * next <tt>SHELL</tt> in the owning <tt>LUMP</tt>. The calling routine must set <tt>lump_ptr</tt> and,
     * if desired, <tt>bound_ptr</tt>, using <tt>set_lump</tt> and <tt>set_bound</tt>.
     * <br><br>
     * @param wire
     * pointer to a list of WIREs to be contained by the constructed SHELL.
     * @param subshell
     * pointer to a list of SUBSHELLs to be contained by the constructed SHELL.
     * @param shell
     * next SHELL in the owning LUMP.
     */
    SHELL(WIRE* wire, SUBSHELL* subshell, SHELL* shell);
    SHELL(const char* gme, WIRE* wire, SUBSHELL* subshell, SHELL* shell);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>SHELL</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 * In versions before 1.6, the next tag will be for a body, but put it in the lump
 * pointer for now anyway, and fix it up later (in <tt>fix_pointers</tt>).
 * <pre>
 *    if (restore_version_number >= PATTERN_VERSION
 *       read_ptr  APATTERN index
 *       if (apat_idx != (APATTERN*)(-1)))
 *          restore_cache();
 *    read_ptr  Pointer to record in save file for next SHELL in lump
 *    read_ptr  Pointer to record in save file for first SUBSHELL in shell
 *    read_ptr  Pointer to record in save file for first FACE in shell
 *    if (restore_version_number >= WIREBOOL_VERSION)
 *       read_ptr  Pointer to record in save file for first WIRE in shell
 *    else
 *       Pointer for first WIRE in shell is set to NULL
 *    read_ptr  Pointer to record in save file for body owning the LUMP containing shell</pre>
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>SHELL_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>SHELL_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>SHELL_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"shell"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>SHELL</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.
// ROP
/**
 * Returns a pointer to the next <tt>SHELL</tt> in the list of <tt>SHELL</tt>s contained in a <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>next_type</tt> argument controls how the next method treats
 * patterns, and can take any one of three values:
 * <br><br><dl>
 *    <dt><tt>PAT_CAN_CREATE</tt>:</dt><dd>If the next shell is to be generated from a pattern, create it
 *                    if it doesn't yet exist and return its pointer.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt>:</dt><dd>If the next shell is to be generated from a pattern, but hasn't
 *                    yet been created, bypass it and return the pointer of the next
 *                    already-created shell (if any).</dd>
 *    <dt><tt>PAT_IGNORE</tt>:</dt><dd>Behave as though there is no pattern on the shell.</dd>
 * </dl><br>
 * @param next_type
 * (see Role).
 */
    SHELL* next(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;
    /**
     * Returns a pointer to the first <tt>SUBSHELL</tt> in a list of <tt>SUBSHELL</tt>s immediately contained within this <tt>SHELL</tt>.
     */
    SUBSHELL* subshell() const;
    /**
     * Returns a pointer to the first <tt>FACE</tt> of a list of <tt>FACE</tt>s immediately contained in this <tt>SHELL</tt>.
     */
    FACE* face_list() const;
    /**
     * Returns a pointer to the first <tt>WIRE</tt> of a list of <tt>WIRE</tt>s immediately contained in this <tt>SHELL</tt>.
     */
    WIRE* wire_list() const;
    /**
     * Returns a pointer to the owning <tt>LUMP</tt>.
     * <br><br>
     * <b>Role:</b> <tt>SHELL</tt>s in separate <tt>LUMP</tt>s are entirely separate.
     */
    LUMP* lump() const { return lump_ptr; }
    /**
     * Returns a geometric bounding region (box) for this <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>SHELL</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The return may be <tt>NULL</tt> if the bound
     * was not calculated since the <tt>SHELL</tt> was last modified.
     */
    SPAbox* bound() const { return box_container.get_box(); }

    /**
     * Returns a pointer to the owning <tt>LUMP</tt>.
     */
    ENTITY* owner() const;


    // Data changing routines. Each ensures that the SHELL object has
    // been saved for rollback before modifying it.
/**
 * Sets this <tt>SHELL</tt>'s next <tt>SHELL</tt> pointer to the given <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> Before performing a change, it checks if the data structure is posted
 * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
 * bulletin board.
 * <br><br>
 * @param shell
 * the new next SHELL.
 * @param reset_pattern
 * internal use only.
 */
    void set_next(SHELL* shell, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>SHELL</tt>'s <tt>SUBSHELL</tt> pointer to the given <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param subshell
     * the new SUBSHELL.
     * @param reset_pattern
     * internal use only.
     */
    void set_subshell(SUBSHELL* subshell, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>SHELL</tt>'s <tt>FACE</tt> pointer to the given <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param face
     * the new FACE.
     * @param reset_pattern
     * internal use only.
     */
    void set_face(FACE* face, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>SHELL</tt>'s <tt>WIRE</tt> pointer to the given <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param wire
     * the new WIRE.
     * @param reset_pattern
     * internal use only.
     */
    void set_wire(WIRE* wire, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>SHELL</tt>'s <tt>LUMP</tt> pointer to the specified <tt>LUMP</tt>.
     * <br><br>
     * <b>Role:</b> Before performing a change, it checks if the data structure is posted
     * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on the
     * bulletin board.
     * <br><br>
     * @param lump
     * the new owning LUMP.
     * @param reset_pattern
     * internal use only.
     */
    void set_lump(LUMP* lump, logical reset_pattern = TRUE);
    /**
     * Sets the bounding region (box) of this <tt>SHELL</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>SHELL</tt>'s <tt>SPAbox</tt> pointer to point to the given <tt>SPAbox</tt>.
     * Before performing the change, it checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }


    // Return the first face in a complete enumeration of all the faces
    // in the shell, continued by repeated use of FACE::next().
    // In the presence of subshells, the order of enumeration is not
    // defined, and in general will change whenever the subshell
    // subdivision is changed. The first version is what it will be
    // called eventually, to hide the subshell structure, but we are
    // currently in transition, because it was the old name for face_list().
    // The temporary withdrawal will by now (R1.7) have served its purpose
    // of ensuring all old-style references have disappeared, so we can
    // restore "face()" to use in its new role, with the aim eventually
    // of removing "first_face".

/**
 * Returns the first <tt>FACE</tt> in a complete enumeration of all the <tt>FACE</tt>s in the <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> The enumeration is continued by repeated use of <tt>FACE::next</tt>. The
 * undefined order of <tt>SUBSHELL</tt>s fluctuates with each change of the <tt>SUBSHELL</tt> subdivision.
 */
    FACE* face() const;

    // STI let (12/98): SHELL::first_face() is officially OBSOLETE in ACIS 5.0
/**
 * @nodoc
 */
    [[deprecated("Deprecated method : Please use face() instead")]]
    FACE* first_face() const;

    // Same for wires. This pointer didn't exist before R1.7, so there
    // is no need for the extra name.

/**
 * Returns the first <tt>WIRE</tt> in a complete enumeration of all the <tt>WIRE</tt>s in this <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> The enumeration is continued by repeated use of <tt>WIRE::next</tt>. The
 * undefined order of <tt>SUBSHELL</tt>s fluctuates with each change of the <tt>SUBSHELL</tt> subdivision.
 */
    WIRE* wire() const;
    // STI let (07/00): Added a new method
    // Determine if the shell is closed or not.  This method considers
    // only single-sided faces.  It ignores all double-sided faces and wires.
/**
 * Determine if the shell is closed or not.
 * <br><br>
 * <b>Role:</b> This method considers only single-sided faces. It ignores all
 * double-sided faces and wires.
 */
    logical is_closed() const;

public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_subshell_ptr();
    void** gme_get_face_ptr();
    void** gme_get_wire_ptr();
    void** gme_get_lump_ptr();
};
/** @} */
#endif
