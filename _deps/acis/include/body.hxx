/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for the BODY class.

// This class models a wire, sheet, or solid body (which may be several
// disjoint bodies treated as a collection of lumps).

// Lumps represent solids, sheets, and wires.  In a manifold solid, every
// edge is adjacent to two faces.  A non-manifold solid may have edges
// that are adjacent to more than two faces (or it may have more than one
// set of faces at a vertex.)  Edges in a sheet may bound any number of
// faces.  Edges of a wire do not bound any faces.

// A pure wire body contains wires, edges, coedges and vertices, but
// no faces.  Wires can represent isolated points, open or closed profiles,
// and general wire-frame models that are unsurfaced, i.e., have no faces.
// In recent versions of ACIS, wires are attached as a component of a shell
// and are not directly attached to the body.

// A solid body is represented by the boundary of the region of space
// enclosed by and modelled as a single lump composed of one or more disjoint
// SHELLs, containing no wires.

// The geometry of body is given in a local coordinate system, related
// to the universal one by a transformation stored with the body.

// ATTENTION Tech Pubs:
// Users may also find the topological traversal functions located in
// kernel/kerndata/top/query.hxx useful for generating lists of faces, edges
// and vertices on other topological entities.  Other functions of note
// include: get_body_box() to retrieve or recalculate the bounding SPAbox of
// a body, point_in_body() to determine the containment of a point versus
// a body, and raytest_body() to determine the intersections of a ray
// with a body.

#ifndef BODY_CLASS
#define BODY_CLASS

#include "dcl_kern.h"
#include "tophdr.hxx"

#include "en_macro.hxx"

/**
 * @file body.hxx
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
class BODY_data;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(BODY, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

/**
 * @nodoc
 */
extern DECL_KERN int BODY_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define BODY_LEVEL 1


// BODY declaration proper.

/**
 * Represents a wire, sheet, or solid body. If you derive from BODY, the <tt>owner()</tt> method must still return NULL.
 * <br>
 * <b>Role:</b> A <tt>BODY</tt> models a wire, sheet, or solid body. A body may be several disjoint
 * bodies treated as a collection of lumps.
 * <br><br>
 * Lumps represent solids, sheets, and wires. In a manifold solid, every edge is adjacent
 * to two faces. A nonmanifold solid may have edges that are adjacent to more than two faces.
 * A nonmanifold solid may also have more than one set of faces at a vertex. Edges in a sheet
 * may bound any number of faces. Edges of a wire do not bound any faces.
 * <br><br>
 * A pure wire body contains wires, edges, coedges, and vertices, but no faces. Wires
 * can represent isolated points, open or closed profiles, and general wireframe models that
 * are unsurfaced, i.e., have no faces. Wires are attached as a component of a shell and are
 * not directly attached to the body.
 * <br><br>
 * A solid body is represented by the boundary of the region of space that is enclosed
 * by a single lump. The lump is composed of one or more disjoint shells that contain no wires.
 * <br><br>
 * The geometry of body is given in a local coordinate system. This relates to the universal
 * one by a transformation stored with the body.
 * <br><br>
 * Functions for traversing the topology are located in <tt>query.hxx</tt>.
 * These are useful for generating lists of faces, edges, and vertices on other topological
 * entities. Other functions of note include: <tt>get_body_box</tt> to retrieve or recalculate the
 * bounding box of a body; <tt>point_in_body</tt> to determine the containment of a point versus a body;
 * and <tt>raytest_body</tt> to determine the intersections of a ray with a body.
 * @see LUMP, TRANSFORM, WIRE, LUMP
 */

class DECL_KERN BODY : public ENTITY {

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
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Performs the transform on the pattern.
     * <br><br>
     * @param tform
     * transform.
     */
    void transform_patterns(const SPAtransf& tform);// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> <tt>api_pattern_apply_to_entity</tt> should be used to apply patterns to bodies.
     * <br><br>
     * <b>Role:</b> Sets the pattern.
     * <br><br>
     * @param in_pat
     * pattern.
     * @param keep_pointer
     * internal use only.
     */
    void set_pattern(pattern* in_pat, logical keep_pointer = TRUE);// internal use only
    /**
     * @nodoc
     */
    void get_all_patterns(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    void get_all_pattern_holders(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    void find_pattern_seeds(VOID_LIST& pats, ENTITY_LIST*& seeds);// internal use only
    /**
     * @nodoc
     */
    logical get_parent_pattern_holders(VOID_LIST& list);// internal use only
    /**
     * @nodoc
     */
    logical unshare_pat_holder_attribs();// internal use only
    /**
     * @nodoc
     */
    logical process_all_pat_holder_attribs(pattern_holder* in_ph = NULL);// internal use only
    /**
     * @nodoc
     */
    logical transfer_all_pat_holder_attrib_caches();// internal use only
    /**
     * @nodoc
     */
    logical attach_pattern_attrib(pattern* pat, logical include_children = TRUE);// internal use only
    // An entity is a "pattern child" when it is not responsible for
    // creating new entities when the pattern is applied.  Instead,
    // some owning entity takes care of this.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if this is a pattern child.
 */
    logical is_pattern_child() const;// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes the pattern on this and all associated entities. Returns <tt>FALSE</tt>
     * if no pattern is found, otherwise returns <tt>TRUE</tt>.
     */

    logical remove_pattern();// internal use only
    /**
     * @nodoc
     */
    logical remove_from_pattern();// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Removes this entity from the list of entities maintained by its
     * pattern, if any. Returns <tt>FALSE</tt> if no pattern is found, otherwise returns <tt>TRUE</tt>.
     */
    logical remove_from_pattern_list();// internal use only
    /**
     * @nodoc
     */
    logical remove_pattern_down_if_incompatible();// internal use only
    // Returns TRUE if the body or one or more of its patternable sub-entities
    // has a non-NULL pattern_holder_ptr.
/**
 * @nodoc
 */
    logical contains_pattern() const;// internal use only
    /**
     * @nodoc
     */
    logical undo_patterns(
        int seed_index = 0,
        logical mark_pending = FALSE,
        logical pattern_copy = FALSE);// internal use only
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
    friend class BODY_data_mgr;

    // Pointer to BODY_data class that holds detailed information.

    BODY_data* pbody_data;

    // Pointer to the start of a list of lumps.

    LUMP* lump_ptr;


    // Pointer to the start of a list of wires.

    WIRE* wire_ptr;


    // This transformation relates the local coordinate system to the
    // global one in which the body resides.

    TRANSFORM* transform_ptr;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire body lies (with respect to its internal coordinate system).
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
    ENTITY_FUNCTIONS(BODY, KERN)

        /**
         * Constructs a <tt>BODY</tt> containing a specified <tt>WIRE</tt>.
         * <br><br>
         * <b>Role:</b> Requests memory for this object and populates it with the data
         * supplied as the argument. Applications should call this constructor only with
         * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
         * a requirement to support roll back and history management.
         * <br><br>
         * @param wire
         * WIRE to be contained by the constructed BODY.
         */
        BODY(WIRE* wire);		// for a body containing a wire initially
    BODY(const char* gme, WIRE* wire);		// for a body containing a wire initially

    /**
     * Sets the <tt>WIRE</tt> pointer of <tt>this</tt> <tt>BODY</tt> to the specified <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param wire
     * the new WIRE.
     * @param reset_pattern
     * internal use only.
     */
    void set_wire(WIRE* wire, logical reset_pattern = TRUE);

#if 0
    ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
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


    // Now the functions specific to BODY.

    // Basic constructor, used internally for bulletin board activity.

/**
 * Constructs a <tt>BODY</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    BODY();
    BODY(const char* gme);


    // The body constructor initialises the record, and makes a new
    // bulletin entry in the current bulletin board to record the
    // creation of the body.

/**
 * Constructs a <tt>BODY</tt> containing a specified list of <tt>LUMP</tt>s.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param lump
 * pointer to a list of LUMPSs to be contained by the constructed BODY.
 */
    BODY(LUMP* lump);		// for a body containing a lump initially
    BODY(const char* gme, LUMP* lump);		// for a body containing a lump initially

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores <tt>this</tt> <tt>BODY</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br><tt>
 * &nbsp;&nbsp;&nbsp;if (restore_version_number &gt;= PATTERN_VERSION<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_ptr</tt>   APATTERN index<br><tt>
 * &nbsp;&nbsp;&nbsp;if (apat_idx != (APATTERN*)(-1)))<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;restore_cache();<br>
 * &nbsp;&nbsp;&nbsp;if (restore_version_number &lt; LUMP_VERSION)<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_ptr  // Pointer to shell tag<br>
 * &nbsp;&nbsp;&nbsp;if ((int)shell_tag &gt;= 0)
 * </tt>if the shell_tag is not NULL, then create a new LUMP pointer.<br>
 * &nbsp;&nbsp;&nbsp;   else  if the shell_tag is NULL, then the LUMP pointer is also NULL.<br>
 * &nbsp;&nbsp;&nbsp;   else if the lump is not NULL<br><tt>
 * &nbsp;&nbsp;&nbsp;read_ptr </tt> Pointer to record in save file for first LUMP shell in body<tt><br>
 * &nbsp;&nbsp;&nbsp;read_ptr </tt>Pointer to record in save file for first WIRE in body.<tt><br>
 * &nbsp;&nbsp;&nbsp;read_ptr </tt>Pointer to record in save file for body TRANSFORM<br>
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for <tt>this</tt> object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>BODY_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>BODY_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>BODY_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"body"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating <tt>this</tt> <tt>BODY</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the beginning of the list of bounding lumps of <tt>this</tt> <tt>BODY</tt>.
 */
    LUMP* lump() const;
    /**
     * Returns a pointer to the beginning of the list of wires comprising <tt>this</tt> <tt>BODY</tt>.
     */
    WIRE* wire() const;
    /**
     * Returns the <tt>TRANSFORM</tt> on <tt>this</tt> <tt>BODY</tt>.
     * <br><br>
     * <b>Role:</b> Returns a pointer to the transformation that relates the local coordinate
     * system to the global one in which the body resides.
     */
    TRANSFORM* transform() const { return transform_ptr; }
    /**
     * Returns a geometric bounding region (box) for <tt>this</tt> <tt>BODY</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the complete <tt>BODY</tt> with respect to
     * its internal coordinate system. The pointer is <tt>NULL</tt> if a bound was not
     * calculated since the <tt>BODY</tt> was last changed.
     */
     /*@todo:entity_box_container类未实现*/
    SPAbox* bound() const { return box_container.get_box(); }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets the <tt>LUMP</tt> pointer of <tt>this</tt> <tt>BODY</tt> to the specified <tt>LUMP</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param lump
 * the new LUMP.
 * @param reset_pattern
 * internal use only.
 */
    void set_lump(LUMP* lump, logical reset_pattern = TRUE);
    /**
     * Sets the <tt>TRANSFORM</tt> pointer of <tt>this</tt> <tt>BODY</tt> to the specified <tt>TRANSFORM</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param transf
     * the new TRANSFORM.
     * @param reset_owner
     * internal use only.
     */
    void set_transform(TRANSFORM* transf, logical reset_owner = TRUE);
    /**
     * Sets the bounding region (box) of <tt>this</tt> <tt>BODY</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>BODY</tt>'s <tt>SPAbox</tt> pointer to point to the given <tt>SPAbox</tt>.
     * This method is generally called internally in conjunction with the
     * <tt>get_body_box</tt> function. Before performing the change, it checks if
     * the data structure is posted on the bulletin board. If not, the method
     * calls <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
     /*@todo:entity_box_container类未实现*/
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }

public:
    // get and set functions for access.
    void** gme_get_lump_ptr();
    void** gme_get_wire_ptr();
    void** gme_get_transform_ptr();
};
/** @} */
#endif
