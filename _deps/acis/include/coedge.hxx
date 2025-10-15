/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for the COEDGE class.

// The coedge is closely related to an edge and provides room to
// store its relationships with adjacent edges and with superior owning
// entities.  (In some contexts, the coedge may be viewed as the use
// of an edge by a face or wire.)  The data structures formed by these
// relationships (stored as pointers) and their interpretation depends
// upon the nature of the owning entity.

// The normal case is when the associated edge is part of a well-formed,
// manifold, solid body shell, being adjacent to exactly two faces.
// There are two coedges, each associated with a loop in one of the
// faces (in principle the two faces could be the same, and even the
// loops could be the same).  All the coedges in each loop are linked
// into a doubly-linked circular list through the next and previous
// pointers, and the two coedges for each edge are linked through
// their partner pointers.

// There are several extensions possible to this simple arrangement.
// Firstly, a loop may not necessarily be closed, for either a
// partially-defined or infinite face boundary.  In this case, the
// next and previous lists are not circular, but terminate with NULL
// pointers.  Secondly, a shell may not be closed, and have "free"
// edges at its boundary.  For such edges, there is only one coedge,
// with a NULL partner pointer.

// A third extension is to non-manifold shells, in which more than
// two faces meet in an edge.  In this case, the partner pointers for
// the coedges (still one for each face) are linked in a circular list.

// Coedges on faces whose underlying geometry is a parametric surface
// must maintain a pointer to a pcurve, which represents the curve
// underlying the edge in the parameteric space of the surface.
// Coedges on analytic surfaces are not required to have pcurves.

// An entirely different owning entity is the wire.  An object may
// be a directed or undirect graph, made up of one or more disjoint
// wires, each of which is a collection of connected edges.  In this
// case, each edge has exactly one coedge and the coedges are linked
// in circular lists around each vertex, using next and previous
// pointers according to which end of the coedge lies at the vertex.
// The next or previous pointer of a coedge on an open edge may be
// set to itself, indicating that there is no next or previous coedge
// on this branch of the wire.

// Finally, a shell may be of mixed dimensionality, containing both
// faces and unembedded edges.  The unembedded edges are connected
// together in wires and belong to the shell. Where they meet
// faces of the shell the vertices have multiple edge pointers, one
// for each face group, and one for each wire attached.

#ifndef COEDGE_CLASS
#define COEDGE_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "tophdr.hxx"
#include "en_macro.hxx"
#include "usecount.hxx"
/**
 * @file coedge.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

class pattern_holder;
class pattern;
class VOID_LIST;

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int COEDGE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define COEDGE_LEVEL 1

class SPAparameter;
class SPAposition;
class SPAinterval;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(COEDGE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// COEDGE declaration proper

/**
 * Relates <tt>EDGEs</tt> to adjacent <tt>EDGEs</tt> and to owning <tt>ENTITYs</tt>.
 * <br>
 * <b>Role:</b> A coedge is closely related to an edge. A coedge stores its relationships
 * with adjacent edges and with superior owning entities. (In some contexts, the coedge
 * may be viewed as the use of an edge by a face or wire.) The data structures formed by
 * these relationships (stored as pointers) and their interpretation depend upon the
 * nature of the owning entity.
 * <br><br>
 * The typical case is when the coedge's associated edge is part of a well-formed,
 * manifold, solid body shell and when that edge is adjacent to exactly two faces. This
 * results in two coedges, each associated with a loop in one of the faces. (In principle
 * the two faces could be the same, and even the loops could be the same.) All the coedges
 * in each loop are linked into a doubly-linked circular list using the next and previous
 * pointers. The two coedges for each edge are linked through their partner pointers.
 * <br><br>
 * Several deviations are possible from the typical case.
 * <br><br>
 * - A loop may not necessarily be closed for either a partially defined or infinite face
 * boundary. In this case, the next and previous lists are not circular, but terminate with
 * <tt>NULL</tt> pointers.
 * <br><br>
 * - A shell may not be closed and have "free" edges at its boundary. For such edges, there
 * is only one coedge with a <tt>NULL</tt> partner pointer.
 * Nonmanifold shells, where more than two faces meet in an edge, links the partner pointers
 * for the coedges (still one for each face) in a circular list.
 * <br><br>
 * - Coedges on faces whose underlying geometry is a parametric surface must maintain a pointer
 * to a pcurve, which represents the curve underlying the edge in the parametric space of the
 * surface. Coedges on analytic surfaces are not required to have pcurves.
 * <br><br>
 * - Wires as owning entities are handled differently. An object may be a directed or undirected
 * graph made up of one or more disjoint wires, each of which is a collection of connected
 * edges. In this case, each edge has exactly one coedge. The coedges are linked in circular
 * lists around each vertex using next and previous pointers according to which end of the
 * coedge lies at the vertex. The next or previous pointer of a coedge on an open edge may be set
 * to itself, indicating that there is no next or previous coedge on this branch of the wire.
 * <br><br>
 * - A shell may be of mixed dimensionality, containing both faces and unembedded edges.
 * The unembedded edges are connected together in wires and belong to the shell. Where they meet
 * faces of the shell, the vertices have multiple edge pointers, one for each face group, and one
 * for each wire attached.
 * <br><br>
 * The <tt>sg_get_coedges_of_wire</tt> and <tt>sg_q_coedges_around_vertex</tt> functions
 * may be useful for generating lists of coedges on other topological entities. The
 * <tt>geometry.hxx</tt> file contains several other geometric inquiry functions.
 * @see EDGE, ENTITY, PCURVE, EDGE, LOOP, WIRE
 */

class DECL_KERN COEDGE : public ENTITY {

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
     * <b>Role:</b> Returns <tt>TRUE</tt> if this is a pattern child. An entity is a
     * "pattern child" when it is not responsible for creating new entities when the
     * pattern is applied.  Instead, some owning entity takes care of this.
     */
    logical is_pattern_child() const { return TRUE; }// internal use only
    /**
     * @nodoc
     * <b>Obsolete:</b> do not use.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> if this is removed from the pattern list.
     */
    logical remove_from_pattern_list();// internal use only
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
    // End other pattern-related

private:
    // used by constructors to initialize data
    void init(EDGE*, REVBIT, COEDGE*, COEDGE*);
protected:

    /**
     * Pointer to provide a doubly-linked list of <tt>COEDGEs</tt> in a <tt>LOOP</tt>, or circular lists at each end in a general unembedded graph.
     */
    COEDGE* next_ptr;
    /**
     * Pointer to provide a doubly-linked list of <tt>COEDGEs</tt> in a <tt>LOOP</tt>, or circular lists at each end in a general unembedded graph.
     */
    COEDGE* previous_ptr;
    /**
     * Pointer to a partner <tt>COEDGE</tt>, or <tt>NULL</tt> if this <tt>COEDGE</tt> is unembedded or attached to a free EDGE.
     */
    COEDGE* partner_ptr;
    /**
     * Pointer to the single <tt>EDGE</tt> on which this <tt>COEDGE</tt> and all its partners lie.
     */
    EDGE* edge_ptr;
    /**
     * Relationship between the direction of this <tt>COEDGE</tt> and that of the underlying <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> When embedded in a <tt>FACE</tt>, the <tt>COEDGEs</tt> must run
     * clockwise about the (outward) <tt>FACE</tt> normal; that is, at any point on
     * the <tt>COEDGE</tt>, if the <tt>FACE</tt> normal is "upwards" and the <tt>COEDGE</tt>
     * tangent is "forwards", then the <tt>FACE</tt> lies to the "left".
     */
    REVBIT sense_data;
    /**
     * Pointer to the owning <tt>LOOP</tt> or <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> There is always a <tt>LOOP</tt> if the <tt>COEDGE</tt> is embedded
     * in a <tt>FACE</tt>, or a <tt>WIRE</tt> if it is part of an unembedded graph.
     * If this <tt>COEDGE</tt> is an unembedded one in a mixed-dimensionality
     * <tt>SHELL</tt>, then this pointer is <tt>NULL</tt>.
     */
    ENTITY* owner_ptr;
    /**
     * Pointer to the description of the <tt>EDGE</tt> geometry in reference to the parametric space of the <tt>FACE</tt> in which it is embedded.
     * <br><br>
     * <b>Role:</b> This will be <tt>NULL</tt> if the <tt>EDGE</tt> is not embedded,
     * or if the <tt>FACE</tt> is not parametrically described. It may be <tt>NULL</tt>
     * even if the <tt>FACE</tt> is parametric.
     */
    PCURVE* geometry_ptr;
    /**
     * Sets this <tt>COEDGE</tt>'s parameter-space geometry to the given <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param pcrv
     * the new PCURVE.
     */
    virtual void set_geometry_ptr(PCURVE* pcrv);// internal use only

    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual compare function for <tt>api_get_modified_faces</tt>.
     * <br><br>
     * @param other
     * other entity.
     * @param identical_comparator
     * comparator.
     */
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;

    // Include the standard member functions for all entities.

    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS(COEDGE, KERN)
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


    // Now the functions specific to COEDGE.

    // Basic constructor, used internally for bulletin board activity.

/**
 * Constructs a <tt>COEDGE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    COEDGE();
    COEDGE(const char* gme);
    // Public constructor, which initialises the record, and interfaces
    // with the bulletin board system.  The arguments initialise
    // edge_ptr (and indirectly partner_ptr), sense_data, next_ptr and
    // previous_ptr respectively.  Coedge back-pointers are also set in
    // the two argument coedges, but presently only validly if all the
    // coedges are part of a conventional simple loop.

/**
 * Constructs a <tt>COEDGE</tt> from a specified <tt>EDGE</tt> and the next and previous <tt>COEDGES</tt> in the owning <tt>LOOP</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * The arguments initialize the <tt>EDGE</tt> (and indirectly the partner <tt>EDGE</tt>),
 * the <tt>sense_data</tt>, the previous <tt>COEDGE</tt>, and the next <tt>COEDGE</tt>.
 * <tt>COEDGE</tt> back-pointers are also set in the two argument <tt>COEDGEs</tt>,
 * but are only valid if all the <tt>COEDGEs</tt> are part of a conventional
 * simple <tt>LOOP</tt>.
 * <br><br>
 * @param edge
 * the EDGE in the constructed COEDGE.
 * @param sense
 * sense of the COEDGE relative to its EDGE.
 * @param prev
 * previous COEDGE in the owning LOOP.
 * @param next
 * next COEDGE in the owning LOOP.
 */
    COEDGE(EDGE* edge, REVBIT sense, COEDGE* prev, COEDGE* next);
    COEDGE(const char* gme, EDGE* edge, REVBIT sense, COEDGE* prev, COEDGE* next);


    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>COEDGE</tt> from a <tt>SAT</tt> file.
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
 *    read_ptr  Pointer to record in save file for next COEDGE in loop or wire
 *    read_ptr  Pointer to record in save file for previous COEDGE in loop or wire
 *    read_ptr  Pointer to record in save file for partner COEDGE on edge
 *    read_ptr  Pointer to record in save file for EDGE on which coedge lies
 *    if (restore_version_number &lt; COEDGE_SENSE_VERSION)
 *       read_int  Direction of coedge with respect to the edge enumeration
 *    else
 *       read_logical  either "forward" or "reversed"
 *    read_ptr  Pointer to record in save file for LOOP or wire to which coedge belongs
 *    read_ptr  Pointer to record in save file for parameter space curve PCURVE or geometry</pre>
 */
    void restore_common();
#endif

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>COEDGE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>COEDGE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>COEDGE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"coedge"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>COEDGE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns the next <tt>COEDGE</tt> in a doubly-linked list of <tt>COEDGEs</tt>.
 */
    COEDGE* next() const { return next_ptr; }
    /**
     * Returns the previous <tt>COEDGE</tt> in a doubly-linked list of <tt>COEDGEs</tt>.
     */
    COEDGE* previous() const { return previous_ptr; }
    /**
     * Returns the pointer to the partner <tt>COEDGE</tt> of this <tt>COEDGE</tt>.
     * <br><br>
     * <b>Role:</b> The return will be <tt>NULL</tt> if this <tt>COEDGE</tt> is unembedded or attached
     * to a free <tt>EDGE</tt>.
     */
    COEDGE* partner() const { return partner_ptr; }
    /**
     * Returns a pointer to the single <tt>EDGE</tt> on which this <tt>COEDGE</tt> and all its partners lie.
     */
    EDGE* edge() const { return edge_ptr; }
    /**
     * Returns the relationship between the direction of this <tt>COEDGE</tt> and that of the underlying <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> At any point on a <tt>COEDGE</tt>, if the <tt>FACE</tt> normal is
     * upwards and the <tt>COEDGE</tt> tangent is forward, then the <tt>FACE</tt> lies
     * to the left. This implies that the outer <tt>LOOPs</tt> are counterclockwise
     * and the inner <tt>LOOPs</tt> are clockwise with respect to the <tt>FACE</tt> normal.
     */
    REVBIT sense() const { return sense_data; }
    /**
     * Returns the pointer to the <tt>LOOP</tt> or <tt>WIRE</tt> that owns this <tt>COEDGE</tt>.
     * <br><br>
     * <b>Role:</b> There is always a <tt>LOOP</tt> if the <tt>COEDGE</tt> is embedded
     * in a <tt>FACE</tt>, or a <tt>WIRE</tt> if it is part of an unembedded graph.
     */
    ENTITY* owner() const;
    /**
     * Returns the owner of this <tt>COEDGE</tt> if the former is a <tt>LOOP</tt>, otherwise <tt>NULL</tt>.
     */
    LOOP* loop() const;
    /**
     * Returns the owner of this <tt>COEDGE</tt> if the former is a <tt>WIRE</tt>, otherwise <tt>NULL</tt>.
     */
    WIRE* wire() const;
    /**
     * Returns the owner of this <tt>COEDGE</tt> if the former is a <tt>SHELL</tt>, otherwise <tt>NULL</tt>.
     * <br><br>
     * <b>Role:</b> This method is not for general use. It supports an antiquated wire data struture.
     */
    SHELL* shell() const;
    /**
     * Returns the pointer to the description of the <tt>COEDGE</tt> geometry in reference to the parametric space of the <tt>FACE</tt> in which it is embedded.
     * <br><br>
     * <b>Role:</b> The pointer is <tt>NULL</tt> if the <tt>COEDGE</tt> is not embedded,
     * or if the <tt>FACE</tt> is not parametric.
     */
    PCURVE* geometry() const { return geometry_ptr; }

    // Routines for finding the positions of the start and end of
    // the edge. Used a lot, so convenient to have.

/**
 * Returns either the start or end position of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start position if the sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's end position. This function does not take into
 * account a transformation on the owning body. That is the responsibility of
 * the calling function.
 */
    virtual SPAposition start_pos() const;
    /**
     * Returns either the start or end position of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense.
     * <br><br>
     * <b>Role:</b> Returns the edge's end position if the sense is <tt>FORWARD</tt>;
     * otherwise, returns the edge's start position. This function does not take into
     * account a transformation on the owning body. That is the responsibility of
     * the calling function.
     */
    virtual SPAposition end_pos() const;
    /**
     * Returns either the start or end parameter of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense.
     * <br><br>
     * <b>Role:</b> Returns the edge's start parameter if the sense is <tt>FORWARD</tt>;
     * otherwise, returns the edge's end parameter.
     */
    virtual SPAparameter start_param() const;
    /**
     * Returns either the start or end parameter of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense.
     * <br><br>
     * <b>Role:</b> Returns the edge's end parameter if the sense is <tt>FORWARD</tt>;
     * otherwise, returns the edge's start parameter.
     */
    virtual SPAparameter end_param() const;
    /**
     * Returns the parameter range of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * as an interval, adjusted for sense.
     * <br><br>
     * <b>Role:</b> If the coedge sense is <tt>REVERSED</tt>, the returned interval is
     * the negated edge parameter range.
     */
    virtual SPAinterval param_range() const;

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>COEDGE</tt>'s next <tt>COEDGE</tt> pointer.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param next
 * the new next COEDGE.
 * @param reset_pattern
 * internal use only.
 */
    void set_next_no_rev(COEDGE* next, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>COEDGE</tt>'s previous <tt>COEDGE</tt> pointer.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param previous
     * the new previous COEDGE.
     * @param reset_pattern
     * internal use only.
     */
    void set_previous_no_rev(COEDGE* previous, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>COEDGE</tt>'s partner to be the given <tt>COEDGE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param partner
     * the new partner COEDGE.
     * @param reset_pattern
     * internal use only.
     */
    void set_partner(COEDGE* partner, logical reset_pattern = TRUE);
    /**
     * Sets the underlying <tt>EDGE</tt> of this <tt>COEDGE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param edge
     * the new underlying EDGE.
     * @param reset_pattern
     * internal use only.
     */
    void set_edge(EDGE* edge, logical reset_pattern = TRUE);
    /**
     * Sets the sense of this <tt>COEDGE</tt> with respect to the underlying <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param sense
     * the new sense.
     * @param reset_pattern
     * internal use only.
     */
    void set_sense(REVBIT sense, logical reset_pattern = TRUE);
    /**
     * Sets the owning <tt>ENTITY</tt> of this <tt>COEDGE</tt> to the specified <tt>ENTITY</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param owner
     * the new parent ENTITY.
     * @param reset_pattern
     * internal use only.
     */
    void set_owner(ENTITY* owner, logical reset_pattern = TRUE);
    /**
     * Sets the owning <tt>ENTITY</tt> of this <tt>COEDGE</tt> to the specified <tt>LOOP</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param loop
     * the new parent LOOP.
     * @param reset_pattern
     * internal use only.
     */
    void set_loop(LOOP* loop, logical reset_pattern = TRUE);	// sets owner to a loop
    /**
     * Sets the owning <tt>ENTITY</tt> of this <tt>COEDGE</tt> to the specified <tt>WIRE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param wire
     * the new parent WIRE.
     * @param reset_pattern
     * internal use only.
     */
    void set_wire(WIRE* wire, logical reset_pattern = TRUE);	// sets owner to a wire
    /**
     * Sets the owning <tt>ENTITY</tt> of this <tt>COEDGE</tt> to the specified <tt>SHELL</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * This method is not for general use. It supports an antiquated wire data struture.
     * <br><br>
     * @param shell
     * the new parent SHELL.
     * @param reset_pattern
     * internal use only.
     */
    void set_shell(SHELL* shell, logical reset_pattern = TRUE);	// sets owner to a shell
    /**
     * Sets this <tt>COEDGE</tt>'s parameter-space geometry to the given <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param pcrv
     * the new PCURVE.
     * @param reset_pattern
     * internal use only.
     */
    virtual void set_geometry(PCURVE* pcrv, logical reset_pattern = TRUE);

    // "Pseudo-member" functions, returning vertex pointers from the
    // associated edge, if any.  These take into account the sense of
    // the coedge.

/**
 * Returns either the start or end <tt>VERTEX</tt> of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
 * depending upon the coedge's sense.
 * <br><br>
 * <b>Role:</b> Returns the edge's start <tt>VERTEX</tt> if the sense is <tt>FORWARD</tt>;
 * otherwise, returns the edge's end <tt>VERTEX</tt>.
 */
    VERTEX* start() const;
    /**
     * Returns either the start or end <tt>VERTEX</tt> of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense.
     * <br><br>
     * <b>Role:</b> Returns the edge's end <tt>VERTEX</tt> if the sense is <tt>FORWARD</tt>;
     * otherwise, returns the edge's start <tt>VERTEX</tt>.
     */
    VERTEX* end() const;

    // Data access sensitive to an additional sense bit. Note that
    // REVBIT is in fact a logical value, so we can simplify these
    // quite a lot.

/**
 * Returns a pointer to the next <tt>COEDGE</tt> if the sense argument is <tt>FORWARD</tt>; otherwise returns a pointer to the previous <tt>COEDGE</tt>.
 * <br><br>
 * @param rev
 * sense argument.
 */
    COEDGE* next(
        REVBIT rev
    ) const {
        return rev ? previous_ptr : next_ptr;
    }
    /**
     * Returns a pointer to the previous <tt>COEDGE</tt> if the sense argument is <tt>FORWARD</tt>; otherwise returns a pointer to the next <tt>COEDGE</tt>.
     * <br><br>
     * @param rev
     * sense argument.
     */
    COEDGE* previous(
        REVBIT rev
    ) const {
        return rev ? next_ptr : previous_ptr;
    }
    /**
     * Returns the sense of this <tt>COEDGE</tt> compounded with the sense argument.
     * <br><br>
     * <b>Role:</b> Useful when traversing <tt>COEDGEs</tt> in the reverse direction.
     */
    REVBIT sense(
        REVBIT rev
    ) const {
        return rev ? !sense_data : sense_data;
    }

    /**
     * Sets this <tt>COEDGE</tt>'s next <tt>COEDGE</tt> pointer, taking into account the specified sense.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param next
     * the new next COEDGE.
     * @param rev
     * sense argument.
     * @param reset_pattern
     * internal use only.
     */
    void set_next(COEDGE* next, REVBIT rev = FALSE, logical reset_pattern = TRUE);
    /**
     * Sets this <tt>COEDGE</tt>'s previous <tt>COEDGE</tt> pointer, taking into account the specified sense.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param previous
     * the new previous COEDGE.
     * @param rev
     * sense argument.
     * @param reset_pattern
     * internal use only.
     */
    void set_previous(COEDGE* previous, REVBIT rev = FALSE, logical reset_pattern = TRUE);

    /**
     * Returns either the start or end <tt>VERTEX</tt> of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense and a sense argument.
     * <br><br>
     * <b>Role:</b> Returns the edge's start <tt>VERTEX</tt> if the product of the coedge's
     * sense and the sense argument <tt>rev</tt> is <tt>FORWARD</tt>; otherwise, returns
     * the edge's end <tt>VERTEX</tt>.
     * <br><br>
     * @param rev
     * sense argument.
     */
    VERTEX* start(REVBIT rev)
    {
        return rev ? end() : start();
    }

    /**
     * Returns either the start or end <tt>VERTEX</tt> of the <tt>EDGE</tt> underlying this <tt>COEDGE</tt>,
     * depending upon the coedge's sense and a sense argument.
     * <br><br>
     * <b>Role:</b> Returns the edge's end <tt>VERTEX</tt> if the product of the coedge's
     * sense and the sense argument <tt>rev</tt> is <tt>FORWARD</tt>; otherwise, returns
     * the edge's start <tt>VERTEX</tt>.
     * <br><br>
     * @param rev
     * sense argument.
     */
    VERTEX* end(REVBIT rev)
    {
        return rev ? start() : end();
    }

    //Added two methods for internal use and it is specific to boolean merge operation.
    logical ends_at_mergable_singularity() const;
    logical starts_at_mergable_singularity() const;


    // STI let (10/97): Added two method

    // Determine if the coedge ends at a surface singularity
/**
 * Determines if this coedge ends at a surface singularity.
 */
    logical ends_at_singularity() const;

    // Determine if the coedge starts at a surface singularity
/**
 * Determines if this coedge starts at a surface singularity.
 */
    logical starts_at_singularity() const;

    // STI let: end

    // Make a tolerant <tt>TCOEDGE</tt> out of this edge.
/**
 * Makes a tolerant <tt>TCOEDGE</tt> out of this <tt>COEDGE</tt>.
 */
    TCOEDGE* make_tolerant();
    // STI jmb: Handle save/restore of use counted histories
        /**
         * @nodoc
         */
    USE_COUNT_REFERENCE_DECL
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end


public:
    // get and set functions for access.
    void** gme_get_next_ptr();
    void** gme_get_previous_ptr();
    void** gme_get_partner_ptr();
    void** gme_get_edge_ptr();
    void** gme_get_owner_ptr();
    void** gme_get_geometry_ptr();
};

/** @} */
#endif
