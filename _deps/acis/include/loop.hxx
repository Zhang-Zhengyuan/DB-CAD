/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the LOOP class.
// A loop represents a connected portion of the boundary of a FACE.
// Despite the name, it may not be a closed loop - see the description
// of FACE for details.  A loop maintains a pointer to a coedge
// of the loop.  Other coedges of the loop can be found by following
// the next or previous pointers of the coedges.
#ifndef LOOP_CLASS
#define LOOP_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "face.hxx"
#include "en_macro.hxx"
#include "ptfcenum.hxx"
/**
 * @file loop.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

class SPAbox;
class ps_polygon;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(LOOP, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int LOOP_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define LOOP_LEVEL 1
/**
 * Represents a set of connected <tt>EDGEs</tt> that bound a <tt>FACE</tt>.
 * <br>
 * <b>Role:</b> A loop represents a connected portion of the boundary of a face.
 * Loops may be open or closed. Refer to the description of <tt>FACE</tt> for
 * details. A loop maintains a pointer to a coedge of the loop. Other coedges of
 * the loop can be found by following the next or previous pointers of the coedges.
 * @see COEDGE, FACE
 */
class DECL_KERN LOOP : public ENTITY {

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
     * <b>Obsolete:</b> <tt>api_pattern_apply_to_entity</tt> should be used to apply patterns to loops.
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
     * <b>Role:</b> Removes the pattern element associated with this entity from the
     * pattern. Returns <tt>FALSE</tt> if this entity is not part of a pattern element,
     * otherwise <tt>TRUE</tt>.
     * <br><br>
     * <i><b>Note:</b> The affected entities are not destroyed, but are merely made
     * independent of the pattern. The pattern itself is correspondingly modified to
     * "drop out" the newly disassociated element.</i>
     */
    logical remove_from_pattern();// internal use only
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
     * <b>Role:</b> Removes the pattern on this and all associated entities. Returns
     * <tt>FALSE</tt> if no pattern is found, otherwise <tt>TRUE</tt>.
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
     */
    logical remove_from_pattern_ignore_partners();// internal use only
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

    void fix_pattern_group();
    friend FACE* FACE::next_in_list(PAT_NEXT_TYPE next_type) const;

private:

    // Next loop in list bounding a particular face (NULL if this is
    // the last loop in the list).

    LOOP* next_ptr;


    // One "edge" in the loop.  If the loop is closed, no significance
    // is placed upon the choice of this edge, which may change during
    // modelling operations.  If a loop is not closed, then its start
    // coedge pointer always points to the first coedge in the "next"
    // list, so that a loop may always be traversed completely from this
    // start coedge simply by following the next pointer in each coedge
    // until this becomes NULL or returns to the start coedge.

    COEDGE* start_ptr;


    // Back pointer to containing face.  Each loop may belong to
    // only one face.

    FACE* face_ptr;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // loop lies (with respect to the internal coordinate system of its
    // owning body).
    // It may be NULL if no such bound has been calculated since the
    // body was last changed.

    // Contains the use counted box.
    entity_box_container box_container;

    entity_ps_polygon_container ps_polygon_container;

    // Loop classification.
    loop_type this_lt;
    SURFACE* lt_surf_ptr;
    REVBIT lt_rev_flag;
protected:

    // access methods for loop type
    loop_type get_loop_type(const LOOP* in_loop) const;
    void set_loop_type(loop_type lt_in, LOOP* in_loop = NULL);

    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For the <tt>identical_comparator</tt> argument to be <tt>TRUE</tt> requires
     * an exact match when comparing doubles and returns the result of <tt>memcmp</tt> as a
     * default (for non-overridden subclasses). <tt>FALSE</tt> indicates tolerant
     * compares and returns <tt>FALSE</tt> as a default.
     * <br><br>
     * @param other
     * other entity.
     * @param identical_comparator
     * comparator.
     */
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;

    // STI ROLL end

        // Include the standard member functions for all entities.

        /**
         * @nodoc
         */
    ENTITY_FUNCTIONS(LOOP, KERN)
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

    /**
     * @nodoc
     */
    ROLL_NOTIFY_FUNCTION
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif

    // Now the functions specific to LOOP.

    // Basic constructor, used internally for bulletin board activity.

/**
 * Constructs a <tt>LOOP</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    LOOP();
    LOOP(const char* gme);


    // Public constructor which initialises the record and interfaces
    // with the bulletin board.  The arguments initialise coedge_ptr
    // and loop_ptr respectively.  If the given coedge is not NULL,
    // the constructor also sets back pointers to the new loop in
    // each coedge of the loop (hence the coedges must be correctly
    // linked together before this constructor is called).

/**
 * Constructs a <tt>LOOP</tt> from a sequence of <tt>COEDGEs</tt> and another <tt>LOOP</tt> already in the owning <tt>FACE</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * Creates and initializes a new <tt>LOOP</tt>, setting owner pointers in <tt>COEDGEs</tt>.
 * The arguments initialize the first <tt>COEDGE</tt> on this <tt>LOOP</tt> and the
 * next <tt>LOOP</tt> on the list of <tt>LOOPs</tt> bounding a <tt>FACE</tt>,
 * respectively. If the given <tt>COEDGE</tt> is not <tt>NULL</tt>, the constructor
 * also sets back pointers to the new <tt>LOOP</tt> in each <tt>COEDGE</tt> of the
 * <tt>LOOP</tt>; therefore, the <tt>COEDGEs</tt> must be correctly linked together
 * before this constructor is called.
 * <br><br>
 * @param coedge
 * first COEDGE in the constructed LOOP.
 * @param loop
 * next LOOP in the owning FACE.
 */
    LOOP(COEDGE* coedge, LOOP* loop);
    LOOP(const char* gme, COEDGE* coedge, LOOP* loop);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>LOOP</tt> from a <tt>SAT</tt> file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt>
 * method, which is used in reading information from a <tt>SAT</tt> file. This
 * method is never called directly. It is called by a higher hierarchical function
 * if an item in the <tt>SAT</tt> file is determined to be of this class type.
 * An instance of this class will already have been created through the allocation
 * constructor. This method then populates the class instance with the appropriate
 * data from the <tt>SAT</tt> file.
 * <pre>
 *    if (restore_version_number >= PATTERN_VERSION
 *       read_ptr  APATTERN index
 *       if (apat_idx != (APATTERN*)(-1)))
 *          restore_cache();
 *    read_ptr  Pointer to record in save file for next LOOP in boundary of face
 *    read_ptr  Pointer to record in save file for first COEDGE in loop
 *    read_ptr  Pointer to record in save file for FACE which loop bounds </pre>
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier
 * <tt>LOOP_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>LOOP_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>LOOP_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>loop</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>LOOP</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the next <tt>LOOP</tt> in the list of <tt>LOOPs</tt> bounding a <tt>FACE</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>next_type</tt> argument controls how the next method treats
 * patterns, and can take any one of three values:
 * <br><br>
 *
 *    <tt>PAT_CAN_CREATE:</tt> If the next loop is to be generated from a pattern, create it
 *                    if it doesn't yet exist and return its pointer.
 *    <tt>PAT_NO_CREATE:</tt>  If the next loop is to be generated from a pattern, but hasn't
 *                    yet been created, bypass it and return the pointer of the next
 *                    already-created loop (if any).
 *    <tt>PAT_IGNORE:</tt>     behave as though there is no pattern on the loop.
 * <br><br>
 * @param next_type
 * (see Role).
 */
    LOOP* next(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;
    /**
     * Returns a pointer to one <tt>COEDGE</tt> in this <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> If the <tt>LOOP</tt> is closed, any <tt>COEDGE</tt> may be chosen
     * for its traversal. If the <tt>LOOP</tt> is not closed, its start <tt>COEDGE</tt>
     * pointer always points to the first <tt>COEDGE</tt> in the <tt>COEDGE</tt> list.
     * A <tt>LOOP</tt> can always be traversed completely from the start <tt>COEDGE</tt>
     * by following the <tt>COEDGE</tt> next pointer until a <tt>NULL</tt> is returned in
     * an open <tt>LOOP</tt>, or until the start <tt>COEDGE</tt> is returned the second
     * time in a closed <tt>LOOP</tt>.
     */
    COEDGE* start() const { return start_ptr; }
    /**
     * Returns a pointer to the <tt>FACE</tt> containing this <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> Each <tt>LOOP</tt> may belong to only one <tt>FACE</tt>.
     */
    FACE* face() const { return face_ptr; }
    /**
     * Returns a geometric bounding region (box) for this <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>LOOP</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The return may be
     * <tt>NULL</tt> if the bound was not calculated since the <tt>LOOP</tt> was last modified.
     */
    SPAbox* bound() const { return box_container.get_box(); }

    ps_polygon* loop_polygon(logical skip_scars = FALSE);

    /**
     * @nodoc
     */
    loop_type get_classification() const;

    /**
     * Returns a pointer to the owning entity.
     */
    ENTITY* owner() const;


    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets the next pointer of this <tt>LOOP</tt> to the specified <tt>LOOP</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param loop
 * the new next LOOP.
 * @param reset_pattern
 * internal use only.
 */
    void set_next(LOOP* loop, logical reset_pattern = TRUE);
    /**
     * Sets the first <tt>COEDGE</tt> in the list of <tt>COEDGEs</tt> belonging to this <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param coedge
     * the new COEDGE.
     * @param reset_pattern
     * internal use only.
     */
    void set_start(COEDGE* coedge, logical reset_pattern = TRUE);
    /**
     * Sets the face pointer of this <tt>LOOP</tt> to the specified <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param face
     * the new parent FACE.
     * @param reset_pattern
     * internal use only.
     */
    void set_face(FACE* face, logical reset_pattern = TRUE);
    /**
     * Sets the bounding region (box) of this <tt>LOOP</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>LOOP's</tt> <tt>SPAbox</tt> pointer to point to the
     * given <tt>SPAbox</tt>. Before performing the change, it checks if the data
     * structure is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box);

    void set_ps_polygon(ps_polygon* in_ps_poly);

    /**

     * @nodoc
     */
    void set_classification(loop_type);


    // STI let (04/00): Added a new method
    // Reverse the sense of a loop of coedges and their pcurves.

/**
 * Reverses the sense of a loop of coedges and their pcurves.
 */
    void reverse();

public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_start_ptr();
    void** gme_get_face_ptr();
};

/**
 * @nodoc
 */
DECL_KERN void transform_face_list(
    ENTITY_LIST& faces,
    ENTITY_LIST& loops,
    SPAtransf& the_trans,
    ENTITY_LIST& no_simplify_ents = *(ENTITY_LIST*)NULL_REF);

/** @} */
#endif // LOOP_CLASS
