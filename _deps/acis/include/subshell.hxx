/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SUBSHELL.
// This represents a subdivision of a shell or superior subshell.  It
// will allow the system to improve the efficiency of many-to-many
// comparisons, by allowing quantities of faces to be excluded by a
// single SPAbox test.  The subdivision is determined by the system, and
// may change at any time, so the subshell has no significance to the
// user (though the application program may find the spatial
// subdivision implied useful).
#ifndef SUBSHELL_CLASS
#define SUBSHELL_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
/**
 * @file subshell.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
class SPAbox;
// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
extern DECL_KERN int SUBSHELL_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SUBSHELL, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define SUBSHELL_LEVEL 1
/**
 * Represents a subdivision of a <tt>SHELL</tt> or <tt>SUBSHELL</tt>.
 * <br>
 * <b>Role:</b> A subshell represents a subdivision of a shell or superior subshell. The
 * efficiency of many-to-many comparisons is improved by allowing quantities of faces to
 * be excluded by a single box test. The subdivision is determined by the system and may
 * change at any time. Thus, the subshell has no significance to the user, although the
 * application program may find the implied spatial subdivision useful.
 * @see FACE, SHELL, WIRE
 */
class DECL_KERN SUBSHELL : public ENTITY {

    // Pointer to the next superior subshell, or NULL if this subshell
    // belongs directly to a shell.

    SUBSHELL* parent_ptr;


    // Next subshell in list contained by superior shell or subshell.

    SUBSHELL* sibling_ptr;


    // Start of list of subshells contained within this one - allows a
    // hierarchy of subdivisions.

    SUBSHELL* child_ptr;


    // Start of list of faces contained in this subshell.  Each face
    // in a shell must be in the face list of the shell itself or
    // in exactly one of its subshells.

    FACE* face_ptr;


    // Same for wires.

    WIRE* wire_ptr;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // subshell lies (with respect to the internal coordinate system of
    // its body).
    // It may be NULL if no such bound has been calculated since the
    // body was last changed.

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
    ENTITY_FUNCTIONS(SUBSHELL, KERN)
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


    // Now the functions specific to SUBSHELL.

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
    SUBSHELL();
    SUBSHELL(const char* gme);

    // Public constructor which initialises the record and interacts
    // with the bulletin board.  The arguments initialise face_ptr,
    // child_ptr and sibling_ptr respectively.  The calling routine
    // must set parent_ptr and if desired, bound_ptr, using
    // set_parent() and set_bound() [declared below].
/**
 * Constructs a <tt>SUBSHELL</tt> from lists of <tt>FACEs</tt>, child <tt>SUBSHELLs</tt> and a sibling <tt>SUBSHELL</tt> already in the owning <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * Constructs a <tt>SUBSHELL</tt>, initializing the record and interacting with the bulletin board.
 * It sets the back pointers in child <tt>SUBSHELLs</tt> and <tt>FACEs</tt>. The first two arguments define
 * the starts of lists of <tt>FACEs</tt> and <tt>SUBSHELLs</tt> contained within the <tt>SUBSHELL</tt>, and the last
 * is a list of sibling <tt>SUBSHELLs</tt> already in the current <tt>BODY</tt>. The arguments initialize
 * <tt>face_ptr</tt>, <tt>child_ptr</tt>, and <tt>sibling_ptr</tt>, respectively. The
 * calling routine must set <tt>parent_ptr</tt> and, if desired, <tt>bound_ptr</tt>, using
 * <tt>set_parent</tt> and <tt>set_bound</tt>.
 * <br><br>
 * @param face
 * pointer to a list of FACEs to be contained by the constructed SUBSHELL.
 * @param child
 * pointer to a list of SUBSHELLs to be contained by the constructed SUBSHELL.
 * @param sibling
 * a sibling SUBSHELL in the owning BODY.
 */
    SUBSHELL(FACE* face, SUBSHELL* child, SUBSHELL* sibling);
    SUBSHELL(const char* gme, FACE* face, SUBSHELL* child, SUBSHELL* sibling);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>SUBSHELL</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 15%><tt>read_ptr</tt></td>
 * <td>Pointer to record in save file for next <tt>SUBSHELL</tt> belonging to parent (sibling).</td>
 * </tr>
 * <tr>
 * <td><tt>read_ptr</tt></td>
 * <td>Pointer to record in save file for first child <tt>SUBSHELL</tt>.</td>
 * </tr>
 * <tr>
 * <td><tt>read_ptr</tt></td>
 * <td>Pointer to record in save file for first <tt>FACE</tt> in subshell.</td>
 * </tr>
 * </table>
 * <tt>if (restore_version_number >= WIREBOOL_VERSION)</tt><br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>read_ptr</tt>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pointer to record in save file for first <tt>WIRE</tt> in subshell.<br>
 * <tt>else</tt><br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>read_ptr</tt>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pointer for first <tt>WIRE</tt> in subshell is set to <tt>NULL</tt>.
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>SUBSHELL_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>SUBSHELL_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>SUBSHELL_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"subshell"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>SUBSHELL</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the next superior <tt>SUBSHELL</tt>.
 * <br><br>
 * <b>Role:</b> The return is <tt>NULL</tt> if this <tt>SUBSHELL</tt> belongs directly to a <tt>SHELL</tt>.
 */
    SUBSHELL* parent() const { return parent_ptr; }
    /**
     * Returns a pointer to the next <tt>SUBSHELL</tt> in the list contained by a superior <tt>SHELL</tt> or <tt>SUBSHELL</tt>.
     */
    SUBSHELL* sibling() const { return sibling_ptr; }
    /**
     * Returns a pointer to the start of list of <tt>SUBSHELLs</tt> contained within this <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> This data scheme allows a hierarchy of subdivisions.
     */
    SUBSHELL* child() const { return child_ptr; }
    /**
     * Return a pointer to the first <tt>FACE</tt> in the list of <tt>FACEs</tt> contained in this <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Each <tt>FACE</tt> in a <tt>SHELL</tt> must be in the face list of the <tt>SHELL</tt>, or exactly
     * one of its <tt>SUBSHELLs</tt>.
     */
    FACE* face_list() const { return face_ptr; }
    /**
     * Return a pointer to the first <tt>WIRE</tt> in the list of <tt>WIREs</tt> contained in this <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Each <tt>WIRE</tt> in a <tt>SHELL</tt> must be in the face list of the <tt>SHELL</tt>, or exactly
     * one of its <tt>SUBSHELLs</tt>.
     */
    WIRE* wire_list() const { return wire_ptr; }
    /**
     * Returns a geometric bounding region (box) for this <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>SUBSHELL</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The return may be <tt>NULL</tt> if the bound
     * was not calculated since the <tt>SUBSHELL</tt> was last modified.
     */
    SPAbox* bound() const { return box_container.get_box(); }

    /**
     * Returns a pointer to the owning parent.
     */
    ENTITY* owner() const;


    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>SUBSHELL</tt>'s parent pointer to the given <tt>SUBSHELL</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, this method checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param parent
 * the new parent SUBSHELL.
 */
    void set_parent(SUBSHELL* parent);
    /**
     * Sets this <tt>SUBSHELL</tt>'s sibling pointer to the given <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, this method checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param sibling
     * the new sibling SUBSHELL.
     */
    void set_sibling(SUBSHELL* sibling);
    /**
     * Sets this <tt>SUBSHELL</tt>'s child pointer to the given this <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> The pointer is to the start of a list of child <tt>SUBSHELLs</tt>.  Before
     * performing the change, this method checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param child
     * pointer to the first child SUBSHELL in the list.
     */
    void set_child(SUBSHELL* child);
    /**
     * Sets this <tt>SUBSHELL</tt>'s <tt>FACE</tt> pointer to the given <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> The pointer is to the start of a list of <tt>FACEs</tt>.  Before
     * performing the change, this method checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param face
     * pointer to the first FACE in the list.
     */
    void set_face(FACE* face);
    /**
     * Sets this <tt>SUBSHELL</tt>'s <tt>WIRE</tt> pointer to the given <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> The pointer is to the start of a list of <tt>WIREs</tt>.  Before
     * performing the change, this method checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param wire
     * pointer to the first WIRE in the list.
     */
    void set_wire(WIRE* wire);
    /**
     * Sets the bounding region (box) of this <tt>SUBSHELL</tt> to the specified <tt>box</tt>.
     * <br><br>
     * <b>Role:</b> Sets the <tt>SUBSHELL</tt>'s <tt>SPAbox </tt>pointer to point to the given <tt>SPAbox</tt>.
     * Before performing the change, it checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }


    // Return the first face in a complete enumeration of all the faces
    // in the owning shell, continued by repeated use of FACE::next().
    // This is not generally very useful, but makes the corresponding
    // SHELL function rather simpler. As with SHELLs, we now have two
    // synonymous functions, with the simpler ("face") one preferred.
    // Eventually the other one is likely to be withdrawn.
/**
 * Returns the first <tt>FACE</tt> in a complete enumeration of all the <tt>FACEs</tt> in the owning <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> The enumeration is continued by repeated use of <tt>FACE::next</tt>.
 */
    FACE* face() const;

    // STI let (12/98): SUBSHELL::first_face() is officially OBSOLETE in ACIS 5.0
/**
 * @nodoc
 */
    FACE* first_face() const; // internal use only

    // Same for wires. This pointer didn't exist before R1.7, so there
    // is no need for the extra name.
/**
 * Returns the first <tt>WIRE</tt> in a complete enumeration of all the <tt>WIREs</tt> in the owning <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> The enumeration is continued by repeated use of <tt>WIRE::next</tt>.
 */
    WIRE* wire() const;

public:
    // get and set functions for access.
    void** gme_get_parent_ptr();
    void** gme_get_sibling_ptr();
    void** gme_get_child_ptr();
    void** gme_get_face_ptr();
    void** gme_get_wire_ptr();
};

/** @} */
#endif
