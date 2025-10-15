/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the FACE class.
// A face is a bounded portion of a geometric surface, the 2-dimensional
// analogue of a body.  The boundary of a face is represented by one or
// more loops of edges.  Each face is simply connected, implying that
// one can traverse from any point on the interior of the face to any
// other point on the interior of the face without crossing the boundary
// of the face.  In general, it is not meaningful to distinguish exterior
// and interior loops of edges, though for certain surface types this
// may be possible and some algorithms may do so.

// Face "loops" need not necessarily be closed, and if not, either
// open end may be finite or infinite.  If either end is infinite, then
// the face is infinite; if either end is finite, then the face is
// "incompletely-bounded", or just "incomplete".  Although such faces
// can be represented in ACIS, most algorithms cannot handle such faces.
#ifndef FACE_CLASS
#define FACE_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
#include "usecount.hxx"
class SPAbox;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @file face.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

 /**
  * @nodoc
  */
ENTITY_IS_PROTOTYPE(FACE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int FACE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define FACE_LEVEL 1


// Type for a marker to distinguish single-sided (i.e. boundary) faces
// and double-sided ones (i.e. sheets or embedded).
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef logical SIDESBIT;
#define SINGLE_SIDED FALSE
#define DOUBLE_SIDED TRUE

// Type for a marker to distinguish the type of a double-sided face.
// Meaningless for a single-sided face.

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef logical CONTBIT;
#define BOTH_OUTSIDE FALSE
#define BOTH_INSIDE TRUE


// FACE declaration proper
/**
 * Represents a bounded portion of a <tt>SURFACE</tt>.
 * <br>
 * <b>Role:</b> A face is a bounded portion of a single geometric surface,
 * the two-dimensional analog of a body. The boundary is represented by one or
 * more loops or edges. Each face is simply connected, implying that one can
 * traverse from any point on the interior of the face to any other point on the
 * interior of the face without crossing the boundary of the face. In general, it
 * is not meaningful to distinguish exterior and interior loops of edges, though
 * for certain surface types this may be possible and some algorithms may do so.
 * <br><br>
 * Face loops need not necessarily be closed, and if not, either open end may be
 * finite or infinite. If either end is infinite, then the face is infinite; if
 * either end is finite, then the face is "incompletely-bounded", or just
 * "incomplete." Although such faces can be represented in ACIS, most algorithms
 * cannot handle such faces.
 * <br><br>
 * Users may find the topological traversal functions located in <tt>query.hxx</tt>
 * useful for generating lists of faces on other topological entities or lists of
 * edges and vertices on faces. The <tt>get_face_box</tt> function may be useful
 * to retrieve or recalculate a face's bounding box. The <tt>reset_boxes</tt>
 * function may be useful to reset the bounding box of a face and its parents. Other
 * functions of note include:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 20%><tt>point_in_face</tt></td>
 * <td>determines the containment of a point versus a face.</td>
 * </tr>
 * <tr>
 * <td><tt>raytest_face</tt></td>
 * <td>determines the intersection of a ray with a face.</td>
 * </tr>
 * <tr>
 * <td><tt>sg_get_face_normal</tt></td>
 * <td>calculates a normal at a point on a face.</td>
 * </tr>
 * <tr>
 * <td><tt>find_cls_ptto_face</tt></td>
 * <td>finds the closest point to a specified point on a face.</td>
 * </tr>
 * </table>
 * @see LOOP, SHELL, SUBSHELL, SURFACE
 */
class DECL_KERN FACE : public ENTITY {

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
     * <b>Obsolete:</b> <tt>api_pattern_apply_to_entity</tt> should be used to apply patterns to faces.
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
     * <b>Role:</b> Returns all patterns.
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
     * <b>Role:</b> Removes the pattern element associated with this entity from the pattern.
     * Returns <tt>FALSE</tt> if this entity is not part of a pattern element, otherwise <tt>TRUE</tt>.
     * <br><br>
     * <i><b>Note:</b> The
     * affected entities are not destroyed, but are merely made independent of the pattern. The
     * pattern itself is correspondingly modified to "drop out" the newly disassociated
     * element.</i>
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
     * <b>Role:</b> Removes the pattern on this and all associated entities. Returns <tt>FALSE </tt>
     * if no pattern is found, otherwise <tt>TRUE</tt>.
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

    // Pointer to next face in list contained directly
    // by SHELL or SUBSHELL.

    FACE* next_ptr;


    // Pointer to first loop of edges bounding face.

    LOOP* loop_ptr;


    // Pointer to shell containing face, either directly or through a
    // hierarchy of subshells.

    SHELL* shell_ptr;


    // Pointer to subshell containing face directly, or NULL if the
    // face belongs directly to the owning shell.

    SUBSHELL* subshell_ptr;


    // Face geometry.  Every surface is directed - that is its normal
    // direction is a continuous function of SPAposition.  The normal to
    // the face may be the same as that of the surface at any SPAposition,
    // or may be the reverse of it, as determined by sense_ptr.  This
    // member is of an enumerated type, with values FORWARD or REVERSED.
    // When a face bounds a region of space, then its normal is always
    // chosen to point away from the region bounded - that is it is an
    // outward normal.

    // Note that 2-bit fields are used to accommodate systems in which
    // bit fields and REVBITs are signed. 1 bit would be more obvious.

    SURFACE* geometry_ptr;
    REVBIT sense_data : 2;

    // Faces may also be double-sided, either free surfaces in space
    // or embedded in material. Distinguish between single- and double-
    // sided faces, and in the latter case between in-space and in-
    // material.

    SIDESBIT sides_data : 2;
    CONTBIT cont_data : 2;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire body lies (with respect to its internal coordinate system).
    // It may be NULL if no such bound has been calculated since the
    // body was last changed.

    // Contains the versioned box.
    entity_ver_box_container ver_box_container;

    // Contains the par_box.
    entity_par_box_container par_box_container;

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
    ENTITY_FUNCTIONS(FACE, KERN)
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

    /**
     * @nodoc
     */
    USE_COUNT_REFERENCE_DECL
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif

    // Now the functions specific to FACE.

    // Basic constructor, used internally for bulletin board activity.
/**
 * Constructs a <tt>FACE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    FACE();
    FACE(const char* gme);


    // Public constructor which initialises the record and interfaces
    // with the bulletin board.  The arguments initialise the members
    // loop_ptr, next_ptr, geometry_ptr and sense_data respectively.
    // It also sets the backpointers (to the face) in the loops which
    // therefore must be correctly chained together before this
    // constructor is called.
    // The calling routine must set shell_ptr or subshell_ptr to refer
    // to the owning shell or subshell, and if desired, bound_ptr,
    // using set_shell() or set_subshell() and set_bound()
    // [declared below].

/**
 * Constructs a <tt>FACE</tt> from a <tt>SURFACE</tt>, a list of <tt>LOOPs</tt>, and a connected <tt>FACE</tt> already in the owning <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * This constructor creates a <tt>FACE</tt>, initializing the record and interfacing with the
 * bulletin board. The arguments initialize the first <tt>LOOP</tt> on the face, the next <tt>FACE </tt>
 * on the <tt>BODY</tt>, the underlying <tt>SURFACE</tt> geometry, and the sense of the <tt>FACE</tt> relative
 * to the surface (<tt>FORWARD</tt> or <tt>REVERSED</tt>) respectively. It increments the <tt>SURFACE</tt> use
 * count to reflect this new use. It also sets the backpointers (to the <tt>FACE</tt>) in the
 * <tt>LOOPs</tt> that must be correctly chained together before this constructor is called. The
 * calling routine must set <tt>shell_ptr</tt> or <tt>subshell_ptr</tt> to refer to
 * the owning <tt>SHELL</tt> or <tt>SUBSHELL</tt>, and if desired, <tt>bound_ptr</tt>, using
 * <tt>set_shell</tt> or <tt>set_subshell</tt> and <tt>set_bound</tt>.
 * <br><br>
 * @param loop
 * pointer to a list of LOOPs to be contained by the constructed FACE.
 * @param face
 * next FACE in the owning SHELL.
 * @param surf
 * the geometry of the constructed face.
 * @param sense
 * the sense (FORWARD or REVERSED) of the constructed face relative to surf.
 */
    FACE(LOOP* loop, FACE* face, SURFACE* surf, REVBIT sense);
    FACE(const char* gme, LOOP* loop, FACE* face, SURFACE* surf, REVBIT sense);


    // Constructor to be used when splitting a face into two pieces.
    // Uses the given loop list, but takes everything else from the
    // old face: geometry, senses, and shell and subshell owners.
    // Optionally updates the shell or subshell face list to contain
    // the new face (by default).

/**
 * Constructs a <tt>FACE</tt> from a list of <tt>LOOPs</tt> and a connected <tt>FACE</tt> already in the owning <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * Creates a <tt>FACE</tt>, using the given <tt>LOOP</tt> list, but taking geometry, senses, and shell
 * and subshell owners from the old <tt>FACE</tt>. Optionally updates the <tt>SHELL</tt> or <tt>SUBSHELL</tt> <tt>FACE</tt>
 * list to contain the new <tt>FACE</tt> (by default).
 * <br><br>
 * @param face
 * next FACE in the owning SHELL.
 * @param loop
 * pointer to a list of LOOPs to be contained by the constructed FACE.
 * @param update
 * flag to update the SHELL or SUBSHELL FACE list with the constructed FACE.
 */
    FACE(FACE* face, LOOP* loop, logical update = TRUE);
    FACE(const char* gme, FACE* face, LOOP* loop, logical update = TRUE);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>FACE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    if (restore_version_number >= PATTERN_VERSION
 *       read_ptr  APATTERN index
 *       if (apat_idx != (APATTERN*)(-1)))
 *          restore_cache();
 *    read_ptr  Pointer to record in save file for next FACE in shell or subshell
 *    read_ptr  Pointer to record in save file for first LOOP bounding face
 *    read_ptr  Pointer to record in save file for SHELL containing face
 *    read_ptr  Pointer to record in save file for SUBSHELL containing face
 *    read_ptr  Pointer to record in save file for SURFACE on which face lies
 *    read_logical  ("forward", "reversed") Direction of face normal with respect to the surface
 *    if (restore_version_number >= TWOSIDE_VERSION)
 *       read_logical  ("single", "double") Double sided face
 *       if (sides_data)  if double sided face
 *          read_logical  ("out", "in"), Double sided face containment. Containment data.
 *       else
 *          Containment data is FALSE.
 *    else
 *       Side data is SINGLE_SIDED and containment data is FALSE.</pre>
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>FACE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>FACE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>FACE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"face"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>FACE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the next <tt>FACE</tt> in the list of <tt>FACEs</tt> contained directly by a <tt>SHELL</tt> or <tt>SUBSHELL</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>next_type</tt> argument controls how the <tt>next</tt> method treats
 * patterns, and can take any one of three values:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 10%><tt>PAT_CAN_CREATE</tt>:</td>
 * <td>If the next face is to be generated from a pattern, create it
 * if it doesn't yet exist and return its pointer.</td>
 * </tr>
 * <tr>
 * <td><tt>PAT_NO_CREATE</tt>:</td>
 * <td>If the next face is to be generated from a pattern, but hasn't
 * yet been created, bypass it and return the pointer of the next already-created face (if any).</td>
 * </tr>
 * <tr>
 * <td><tt>PAT_IGNORE</tt>:</td>
 * <td>behave as though there is no pattern on the face.</td>
 * </tr>
 * </table>
 * <br>
 * @param next_type
 * how the next method treats patterns.
 */
    FACE* next_in_list(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;
    /**
     * Returns a pointer to the first <tt>LOOP</tt> of <tt>COEDGEs</tt> bounding this <tt>FACE</tt>.
     */
    LOOP* loop() const;
    /**
     * Returns a pointer to the <tt>SHELL</tt> containing this <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Returns the pointer directly, or through a hierarchy of <tt>SUBSHELLs</tt>.
     */
    SHELL* shell() const { return shell_ptr; }
    /**
     * Returns a pointer to the <tt>SUBSHELL</tt> containing this <tt>FACE</tt> directly.
     * <br><br>
     * <b>Role:</b> The return is <tt>NULL</tt> if the <tt>FACE</tt> belongs directly to the owning <tt>SHELL</tt>.
     */
    SUBSHELL* subshell() const { return subshell_ptr; }
    /**
     * Returns a pointer to the underlying <tt>SURFACE</tt> defining this <tt>FACE</tt>.
     */
    SURFACE* geometry() const;
    /**
     * Returns the sense of this <tt>FACE</tt> relative to its <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> Returns <tt>FORWARD</tt> or <tt>REVERSED</tt>. Every <tt>SURFACE</tt> has a direction sense.
     * Its normal direction is a continuous function of position. The normal to the
     * <tt>FACE</tt> can be the same as that of the <tt>SURFACE</tt> at any position, or can be the
     * reverse of it, as determined by <tt>sense</tt>. When a <tt>FACE</tt> bounds a region
     * of space, its normal always points away from the region bounded.
     */
    REVBIT sense() const { return sense_data; }
    /**
     * Indicates whether this <tt>FACE</tt> is single-sided or double-sided.
     * <br><br>
     * <b>Role:</b> Returns either <tt>SINGLE_SIDED</tt> or <tt>DOUBLE_SIDED</tt>.
     */
    SIDESBIT sides() const { return sides_data; }
    /**
     * Returns the containment of this <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Returns <tt>BOTH_OUTSIDE</tt> or <tt>BOTH_INSIDE</tt>. This value is meaningless if
     * the face is single-sided.
     */
    CONTBIT cont() const { return cont_data; }
    /**
     * Returns a geometric bounding region (box) for this <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>FACE</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The return may be <tt>NULL</tt> if the bound
     * was not calculated since the <tt>FACE</tt> was last modified.
     */
     /*@todo:entity_ver_box_container类未实现*/
    SPAbox* bound() const { return ver_box_container.get_ver_box(); }
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
     /*@todo:entity_par_box_container类未实现*/
    SPApar_box* uv_bound() const { return par_box_container.get_par_box(); }
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
 * Sets the next pointer of this <tt>FACE</tt> to the specified <tt>FACE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param face
 * the new next FACE.
 * @param reset_pattern
 * internal use only.
 */
    void set_next(FACE* face, logical reset_pattern = TRUE);
    /**
     * Sets the loop pointer of this <tt>FACE</tt> to the specified <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param loop
     * the new LOOP.
     * @param reset_pattern
     * internal use only.
     */
    void set_loop(LOOP* loop, logical reset_pattern = TRUE);
    /**
     * Sets the shell pointer of this <tt>FACE</tt> to the specified <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param shell
     * the new parent SHELL.
     * @param reset_pattern
     * internal use only.
     */
    void set_shell(SHELL* shell, logical reset_pattern = TRUE);
    /**
     * Sets the subshell pointer of this <tt>FACE</tt> to the specified <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param subshell
     * the new parent SUBSHELL.
     * @param reset_pattern
     * internal use only.
     */
    void set_subshell(SUBSHELL* subshell, logical reset_pattern = TRUE);
    /**
     * Sets the geometry pointer of this <tt>FACE</tt> to the specified <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> A side effect of this method is that the routine adjusts the use
     * counts of the existing and new geometry and deletes the old if it is no longer
     * referenced. Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param surf
     * the new SURFACE.
     * @param reset_pattern
     * internal use only.
     */
    void set_geometry(SURFACE* surf, logical reset_pattern = TRUE);
    /**
     * Sets the sense of this <tt>FACE</tt> with respect to its <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> Sets the <tt>FACE</tt>'s sense to <tt>FORWARD</tt> or <tt>REVERSED</tt>,
     * indicating the direction of the face with respect to the underlying <tt>SURFACE</tt>.
     * Before performing the change, it checks if the data structure is posted on the bulletin board.
     * If not, the method calls <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param rev
     * the new sense.
     * @param reset_pattern
     * internal use only.
     */
    void set_sense(REVBIT rev, logical reset_pattern = TRUE);
    /**
     * Sets the sides bit of this <tt>FACE</tt> to single-sided or double-sided.
     * <br><br>
     * <b>Role:</b> Indicates whether the <tt>FACE</tt> bounds a solid region of not.
     * Valid values for the <tt>SIDESBIT</tt> are <tt>SINGLE_SIDED</tt> or <tt>DOUBLE_SIDED</tt>.
     * Before performing the change, it checks if the data structure is
     * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
     * an entry on the bulletin board.
     * <br><br>
     * @param sides
     * the new sides bit value.
     * @param reset_pattern
     * internal use only.
     */
    void set_sides(SIDESBIT sides, logical reset_pattern = TRUE);
    /**
     * Sets the containment bit of this <tt>FACE</tt>.
     * <br><br>
     * <b>Role:</b> Indicates whether the <tt>FACE</tt> is to be considered fully contained within
     * a solid region, or not. Valid values for the <tt>CONTBIT</tt> are <tt>BOTH_OUTSIDE</tt> or
     * <tt>BOTH_INSIDE</tt>. This parameter is meaningless if the face is single-sided.
     * Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param cont
     * the new containment bit value.
     * @param reset_pattern
     * internal use only.
     */
    void set_cont(CONTBIT cont, logical reset_pattern = TRUE);
    /**
     * Sets the bounding region (box) of this <tt>FACE</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>FACE</tt>'s <tt>SPAbox</tt> pointer to point to the given <tt>SPAbox</tt>.
     * Before performing the change, it checks if the data structure is posted on
     * the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
     /*@todo:entity_par_box_container类未实现 */
    void set_uv_bound(SPApar_box* in_par_box) { par_box_container.set_par_box(this, in_par_box); }

    // Data reading sensitive to an extra reversal. Note that REVBIT
    // is in fact a logical, which simplifies this no end.

/**
 * Returns the sense of this <tt>FACE</tt> relative to its <tt>SURFACE</tt>, and compounded with the supplied argument.
 * <br><br>
 * <b>Role:</b> Useful when traversing the <tt>FACE</tt> in a reverse direction.
 * <br><br>
 * @param rev
 * flags whether the return value represents the internal sense of the FACE, or
 * its reverse.
 */
    REVBIT sense(
        REVBIT rev
    ) const {
        return rev ? !sense_data : sense_data;
    }


    // Return the next face in a complete enumeration of all the faces
    // in the shell.  The order is not defined, and will in general
    // change whenever subshells are recomputed for whatever reason.
    // The first name is the new one (which used to be used for
    // "next_in_list", which just gives the next face in this list), so
    // that the majority of algorithms need not know about subshells.
    // It was not allowed in recent releases, to force all applications
    // to switch to either next_in_list (rarely, except for a temporary
    // face list not currently owned by a shell), or next_face. This
    // release (1.7), both names are valid. Next release, or the release
    // after that the "next_face" version may be withdrawn.

/**
 * Returns the next <tt>FACE</tt> in a complete enumeration of all the <tt>FACEs</tt> in the <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>next_type</tt> argument controls how the next method treats
 * patterns, and can take any one of three values:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 10%><tt>PAT_CAN_CREATE</tt>:</td>
 * <td>If the next face is to be generated from a pattern, create it
 * if it doesn't yet exist and return its pointer.</td>
 * </tr>
 * <tr>
 * <td><tt>PAT_NO_CREATE</tt>:</td>
 * <td>If the next face is to be generated from a pattern, but hasn't
 * yet been created, bypass it and return the pointer of the next already-created face (if any).</td>
 * </tr>
 * <tr>
 * <td><tt>PAT_IGNORE</tt>:</td>
 * <td>behave as though there is no pattern on the face.</td>
 * </tr>
 * </table>
 * <br>
 * @param next_type
 * how the next method treats patterns.
 */
    FACE* next(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;

    // STI let (12/98): FACE::next_face() is officially OBSOLETE in ACIS 5.0
    /**
     * @nodoc
     */
    [[deprecated("Deprecated method : Please use next() instead")]]
    FACE* next_face() const;

public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_loop_ptr();
    void** gme_get_shell_ptr();
    void** gme_get_subshell_ptr();
    void** gme_get_geometry_ptr();
};
/** @} */

#endif
