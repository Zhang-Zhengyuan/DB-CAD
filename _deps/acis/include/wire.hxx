/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for WIRE.
// A wire represents a connected collection of unembedded edges.
// It is owned by a body or a shell, the latter necessarily if the
// wire is attached to corners of faces in the shell, or if it is
// internal to a lump (which will own the shell). Isolated wires
// used to be owned by bodies, and may remain so, though they might
// logically go into a shell owned by a lump owned by the body.

// Usually each edge has one coedge, and the coedges are linked
// in circular lists around each vertex, using next and previous
// pointers according to which end of the coedge lies at the vertex.
// A wire returned when booleans are used to slice (imprint) one
// body with another, has two coedges per edge; the coedges bear
// attributes giving details of the intersections of the body
// boundaries at each edge.
#if !defined( WIRE_CLASS )
#define WIRE_CLASS
/**
 * @file wire.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
class SPAbox;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(WIRE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
extern DECL_KERN int WIRE_TYPE;
/**
 * @nodoc
 * Type for a marker to distinguish the type of a wire, i.e. a genuine
 * wire or a wormhole in material.
 */
typedef logical WIRECONTBIT;
#define ALL_OUTSIDE FALSE
#define ALL_INSIDE TRUE
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define WIRE_LEVEL 1
/**
 * Represents a collection of edges that are owned by a body but are not attached to faces and do not enclose any volume.
 * <br>
 * <b>Role:</b> A wire represents a connected collection of unembedded edges. It is owned by a body
 * or a shell, the latter necessarily if the wire is attached to corners of faces in the shell, or
 * if it is internal to a lump (which will own the shell). Isolated wires used to be owned by bodies,
 * and may remain so, though they might logically go into a shell owned by a lump owned by the body.
 * <br><br>
 * Usually each edge has one coedge. Coedges are linked in circular lists around each vertex using
 * next and previous pointers according to which end of the coedge lies at the vertex. When Booleans
 * are used to slice (imprint) one body with another, a wire is returned that has two coedges per
 * edge; the coedges have attributes giving details of the intersections of the body boundaries at
 * each edge.
 * @see COEDGE, ENTITY, SUBSHELL, BODY, SHELL, SUBSHELL
 */
class DECL_KERN WIRE : public ENTITY {
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
     * <b>Role:</b> Sets the pattern to be the specified pattern.
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

private:

    // Next wire in list of all wires of the owning body.

    WIRE* next_ptr;			// list pointer of wires in a body


    // One "edge" in the wire.  No significance is placed upon the
    // choice of this edge, which may change during modelling
    // operations.

    COEDGE* coedge_ptr;


    // Back pointer to owning entity.  Each wire may belong to only
    // one body or shell.

    ENTITY* owner_ptr;


    // Pointer to subshell containing wire directly, or NULL if the
    // wire belongs directly to the owning shell or a body.

    SUBSHELL* subshell_ptr;


    // Marker to distinguish wires which exist in empty space and
    // those which form infinitesimally thin wormholes through
    // material. Wire edges are always split when they pass through
    // the boundary of a shell, so it is well-defined to place this
    // on the wire rather than on individual edges.

    WIRECONTBIT cont_data;


    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire body lies (with respect to its internal coordinate system).
    // It may be NULL if no such bound has been calculated since the
    // body was last changed.

    // Contains the use counted box.
    entity_box_container box_container;
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For the <tt>identical_comparator</tt> argument to be <tt>TRUE</tt>,
     * it requires an exact match when comparing doubles and returns the result of
     * <tt>memcmp</tt> as a default (for non-overridden subclasses). <tt>FALSE</tt>
     * indicates tolerant compares and returns <tt>FALSE</tt> as a default.
     * <br><br>
     * @param other
     * other entity.
     * @param identical_comparator
     * comparator.
     */
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS(WIRE, KERN)
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
    /**
     * @nodoc
     */
    LOOKUP_FUNCTION
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * Constructs a <tt>WIRE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    WIRE();
    WIRE(const char* gme);
    /**
     * Constructs a <tt>WIRE</tt> from a list of <tt>COEDGEs</tt> and a connected <tt>WIRE</tt> already in the owning <tt>BODY</tt>.
     * <br><br>
     * <b>Role:</b> Requests memory for this object and populates it with the data
     * supplied as the argument. Applications should call this constructor only with
     * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
     * a requirement to support roll back and history management.
     * <br><br>
     * Creates and initializes a <tt>WIRE</tt>, setting its <tt>COEDGE</tt> back pointers.
     * The arguments define the first <tt>COEDGE</tt> in this <tt>WIRE</tt> and the
     * next <tt>WIRE</tt> in a list of <tt>WIREs</tt> on the <tt>BODY</tt>, respectively.
     * <br><br>
     * @param coedge
     * the first COEDGE in the constructed WIRE.
     * @param wire
     * the next WIRE in the owning BODY.
     */
    WIRE(COEDGE* coedge, WIRE* wire);
    WIRE(const char* gme, COEDGE* coedge, WIRE* wire);
#if 0
    /**
     * Restores this <tt>WIRE</tt> from a <tt>SAT</tt> file.
     * <br><br>
     * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
     * which is used in reading information from a <tt>SAT</tt> file. This method is never called
     * directly. It is called by a higher hierarchical function if an item in the <tt>SAT</tt> file
     * is determined to be of this class type. An instance of this class will already have
     * been created through the allocation constructor. This method then populates the class
     * instance with the appropriate data from the <tt>SAT</tt> file.
     * <pre>
     *    if (restore_version_number >= PATTERN_VERSION
     *       read_ptr  APATTERN index
     *       if (apat_idx != (APATTERN*)(-1)))
     *          restore_cache();
     *    read_ptr  Pointer to record in save file for next WIRE in body, shell or subshell
     *    read_ptr  Pointer to record in save file for first COEDGE in wire
     *    read_ptr  Pointer to record in save file for (ENTITY) body or shell containing wire
     *    if (restore_version_number >= WIREBOOL_VERSION)
     *       read_ptr  Pointer to record in save file for SUBSHELL containing wire
     *       read_logical  ("out" "in") Containment of wire
     *    else
     *       Pointer to SUBSHELL set to NULL and containment of wire set to ALL_OUTSIDE.</pre>
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
 * <tt>WIRE_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>WIRE_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>WIRE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"wire"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>WIRE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif
    /**
     * Returns a pointer to the next <tt>WIRE</tt> in the list of <tt>WIREs</tt> contained directly by a <tt>BODY</tt>, <tt>SHELL</tt>, or <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> The <tt>next_type</tt> argument controls how the next method treats
     * patterns, and can take any one of three values:
     * <br><br><dl>
     *     <dt>PAT_CAN_CREATE:</dt><dd>If the next wire is to be generated from a pattern, create it
     *                    if it doesn't yet exist and return its pointer.</dd>
     *    <dt>PAT_NO_CREATE:</dt>  <dd>If the next wire is to be generated from a pattern, but hasn't
     *                    yet been created, bypass it and return the pointer of the next
     *                    already-created wire (if any).</dd>
     *    <dt>PAT_IGNORE:</dt>    <dd>Behave as though there is no pattern on the wire.</dd></dl>
     * <br>
     * @param next_type
     * (see Role).
     */
    WIRE* next_in_list(PAT_NEXT_TYPE /*next_type*/ = PAT_CAN_CREATE) const
    {
        return next_ptr;
    }
    /**
     * Returns a pointer to a <tt>COEDGE</tt> in this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> No significance is placed upon the choice of this <tt>COEDGE</tt>, which
     * may change during modeling operations.
     */
    COEDGE* coedge() const { return coedge_ptr; }
    /**
     * Returns the owner of this <tt>WIRE</tt> if the former is a <tt>BODY</tt>, otherwise <tt>NULL</tt>.
     * <br><br>
     * <b>Role:</b> Each <tt>WIRE</tt> may belong to only one <tt>BODY</tt>.
     */
    BODY* body() const;
    /**
     * Return a pointer to the owning <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Returns <tt>NULL</tt> if the owning <tt>ENTITY</tt> is not a <tt>SHELL</tt>.
     */
    SHELL* shell() const;
    /**
     * Returns a pointer to the <tt>SUBSHELL</tt> directly containing this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Returns <tt>NULL</tt> if the <tt>WIRE</tt> is directly contained
     * by its <tt>ENTITY</tt>, <tt>BODY</tt>, or <tt>SHELL</tt>.
     */
    SUBSHELL* subshell() const { return subshell_ptr; }
    /**
     * Returns the containment information for this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> A return value of <tt>ALL_OUTSIDE</tt> indicates that the <tt>WIRE</tt>
     * represents material in space, while <tt>ALL_INSIDE</tt> indicates that the
     * <tt>WIRE</tt> represents an infinitesimally small hole through material.
     */
    WIRECONTBIT cont() const { return cont_data; }
    /**
     * Returns a geometric bounding region (box) for this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>WIRE</tt> (with respect
     * to the internal coordinate system of the <tt>BODY</tt>). The return may be
     * <tt>NULL</tt> if the bound was not calculated since the <tt>WIRE</tt> was last
     * modified.
     */
    SPAbox* bound() const { return box_container.get_box(); }
    /**
     * Returns a pointer to the owning entity.
     */
    ENTITY* owner() const;
    /**
     * Sets the next <tt>WIRE</tt> in the <tt>BODY's</tt> list of <tt>WIREs</tt> to the given <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param wire
     * the new next WIRE.
     * @param reset_pattern
     * internal use only.
     */
    void set_next(WIRE* wire, logical reset_pattern = TRUE);
    /**
     * Sets the first <tt>COEDGE</tt> in the list of <tt>COEDGEs</tt> belonging to this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param coedge
     * the new COEDGE.
     */
    void set_coedge(COEDGE* coedge);
    /**
     * Set this <tt>WIRE's</tt> owner to be the specified <tt>BODY</tt> or <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param owner
     * the new owning ENTITY.
     */
    void set_owner(ENTITY* owner);
    /**
     * Sets the <tt>WIRE</tt>'s owning <tt>BODY</tt> to the specified <tt>BODY</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param body
     * the new parent BODY.
     */
    void set_body(BODY* body);
    /**
     * Sets the <tt>WIRE</tt>'s owning <tt>SHELL</tt> to the specified <tt>SHELL</tt>.
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
     * Sets the <tt>SUBSHELL</tt> directly containing this <tt>WIRE</tt> to be the given <tt>SUBSHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param subshell
     * the new parent SUBSHELL.
     */
    void set_subshell(SUBSHELL* subshell);
    /**
     * Sets the containment for this <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Indicates whether the <tt>WIRE</tt> is to be considered fully contained within
     * a solid region, or not. A value of <tt>ALL_OUTSIDE</tt> indicates that the <tt>WIRE</tt>
     * represents material in space, while <tt>ALL_INSIDE</tt> indicates that the
     * <tt>WIRE</tt> represents an infinitesimally small hole through material.
     * Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param cont
     * the new containment.
     */
    void set_cont(WIRECONTBIT cont);
    /**
     * Sets the bounding region (box) of this <tt>WIRE</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>WIRE's</tt> <tt>SPAbox</tt> pointer to point to the
     * given <tt>SPAbox</tt>. Before performing the change, it checks if the data
     * structure is posted on the bulletin board. If not, the method calls
     * <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }
    /**
     * Returns a pointer to the next <tt>WIRE</tt> in a complete enumeration of all the <tt>WIREs</tt> in a <tt>BODY</tt> or <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> The order is not defined and may change when <tt>SUBSHELLs</tt> are recomputed.
     * <br><br>
     * The <tt>next_type</tt> argument controls how the next method treats
     * patterns, and can take any one of three values:
     * <br><br>
     * <tt>PAT_CAN_CREATE:</tt> If the next wire is to be generated from a pattern, create it
     *                    if it doesn't yet exist and return its pointer.<br>
     *    <tt>PAT_NO_CREATE:</tt>  If the next wire is to be generated from a pattern, but hasn't
     *                    yet been created, bypass it and return the pointer of the next
     *                    already-created wire (if any).<br>
     *    <tt>PAT_IGNORE:</tt>     behave as though there is no pattern on the wire.
     * <br><br>
     * @param next_type
     * (see Role).
     */
    WIRE* next(PAT_NEXT_TYPE next_type = PAT_CAN_CREATE) const;

public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_coedge_ptr();
    void** gme_get_owner_ptr();
    void** gme_get_subshell_ptr();
};
/** @} */
#endif
