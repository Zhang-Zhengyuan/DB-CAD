/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for the VERTEX class.

// A vertex is a 0-dimensional, topological entity that is used
// to bound an edge or a tedge.  Generally, an edge is bounded by
// two distinct vertices; however, if the edge is closed, both ends
// of the edge are bounded by the same vertex.  A vertex embodies
// the user's view of a corner of a face or the junction point of
// two edges. Each vertex is represented by a point in the geometric model.
// Two vertices can not exist at the same point.

// A vertex maintains pointers to one or more of the edges which it
// bounds.  Generally, a vertex will contain a pointer to one of the
// edges it bounds.  The other edges can be found by following the
// next, previous and partner pointers of the coedges of the edges.
// When a body is non-manifold at a vertex, i.e., more than one
// manifold group of faces meet at a vertex or a wire attaches to a face
// at a vertex, then the vertex must maintain a pointer to an edge
// in each group of faces and wires.  The other edges in a manifold
// group of faces or a wire can be found by following coedge pointers.

#if !defined( VERTEX_CLASS )
#define VERTEX_CLASS
/**
 * @file vertex.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
#include "dcl_kern.h"
#include "usecount.hxx"
#include "tophdr.hxx"
#include "en_macro.hxx"
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(VERTEX, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int VERTEX_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define VERTEX_LEVEL 1


// VERTEX declaration proper
/**
 * Represents a corner of a <tt>FACE</tt> and/or the end of an <tt>EDGE</tt>.
 * <br>
 * <b>Role:</b> A vertex is a 0-dimensional, topological entity that is used to bound an edge or a
 * tedge. Generally, an edge is bounded by two distinct vertices; however, if the edge is closed,
 * both ends of the edge are bounded by the same vertex. A vertex embodies the user's view of a
 * corner of a face or the junction point of two edges. Each vertex is represented by a point in the
 * geometric model. Two vertices can not exist at the same point.
 * <br><br>
 * A vertex maintains pointers to one or more of the edges which it bounds. Generally, a vertex
 * contains a pointer to one of the edges it bounds. The other edges can be found by following the
 * next, previous, and partner pointers of the coedges of the edges. When a body is nonmanifold at
 * a vertex (i.e., more than one manifold group of faces meet at a vertex or a wire attaches to a
 * face at a vertex), then the vertex must maintain a pointer to an edge in each group of faces and
 * wires. The other edges in a manifold group of faces or a wire can be found by following coedge
 * pointers.
 * <br><br>
 * Users may be interested in the topological traversal function
 * <tt>sg_q_edges_around_vertex</tt> or <tt>sg_q_coedges_around_vertex</tt>, which return
 * a list of edges or coedges that share a given vertex. Users may also find the topological
 * traversal functions located in <tt>q_vert.hxx</tt> useful for generating lists of
 * vertices on other topological entities.
 * @see APOINT, EDGE
 */
class DECL_KERN VERTEX : public ENTITY {

    friend DECL_KERN void set_vertex_tolerance(VERTEX*, double);

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
    // An entity is a "pattern child" when it is not responsible for
    // creating new entities when the pattern is applied.  Instead,
    // some owning entity takes care of this.
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
     * <b>Role:</b> Removes this entity from the list of entities maintained by its pattern,
     * if any. Returns <tt>FALSE</tt> if no pattern is found, otherwise <tt>TRUE</tt>.
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

protected:

    // Pointer to one edge on which the vertex lies.  If null, may
    // indicate that the vertex has a vertedge attribute that holds
    // pointers to several edges at the vertex.
/**
 * Pointer to one <tt>EDGE</tt> on which this <tt>VERTEX</tt> lies.
 * <br><br>
 * <b>Role:</b> A <tt>NULL</tt> value may indicate that the <tt>VERTEX</tt> has
 * a vertedge attribute that holds pointers to several <tt>EDGEs</tt> at the
 * <tt>VERTEX</tt>.
 */
    EDGE* edge_ptr;

    // Pointer to SPAposition of vertex in space.
/**
 * Pointer to the position of this <tt>VERTEX</tt> in space.
 */
    APOINT* geometry_ptr;

    // Tolerance value
    // Negative value indicates if the tolerance value needs to be recalculated
/**
 * @nodoc
 */
    double tolerance;

    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For <tt>identical_comparator</tt> to be <tt>TRUE</tt>, it requires an
     * exact match when comparing doubles, and returns the result of <tt>memcmp</tt>
     * as a default (for non-overridden subclasses). <tt>FALSE</tt> indicates tolerant
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
    ENTITY_FUNCTIONS(VERTEX, KERN)
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

    // Now the functions specific to VERTEX.

    // Basic constructor, used internally for bulletin board activity.
/**
 * Constructs a <tt>VERTEX</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    VERTEX();
    VERTEX(const char* gme);

    // Public constructor, which initialises the record and interfaces
    // with the bulletin board.  Increments the point's use count to
    // reflect this new use.
/**
 * Constructs a <tt>VERTEX</tt> described by the specified <tt>APOINT</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * Creates a <tt>VERTEX</tt> using the specified parameters. The argument specifies
 * the <tt>APOINT</tt> entity defining the position of this <tt>VERTEX</tt> in
 * space. The constructor increments the <tt>APOINT's</tt> use count to reflect
 * this new use.
 * <br><br>
 * @param point
 * the APOINT describing the constructed VERTEX.
 */
    VERTEX(APOINT* point);
    VERTEX(const char* gme, APOINT* point);

    /*
    // Copy Constructor
    VERTEX( const VERTEX& );
    */

    // ENTITY *make_tolerant(double) const;

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>VERTEX</tt> from a <tt>SAT</tt> file.
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
 *    read_ptr  Pointer to record in save file for an edge which uses vertex
 *    read_ptr  Pointer to record in save file for point at which vertex lies </pre>
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
 * <tt>VERTEX_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>VERTEX_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>VERTEX_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"vertex"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>VERTEX</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

    // Get the i-th edge pointed to by the vertex.  This function
    // must be used when the vertex contains pointers to more than
    // one edge.  edge(0) returns the expected result if the edge
    // contains one pointer.  Returns null if i is too great or
    // negative.

/**
 * Gets the <i>i</i>th <tt>EDGE</tt> pointed to by this <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> Use this function when the <tt>VERTEX</tt> contains pointers to more than
 * one <tt>EDGE</tt>. Given an argument of zero (<tt>0</tt>), it returns the expected
 * result if the <tt>EDGE</tt> contains one pointer. It returns <tt>NULL</tt> if
 * <tt>i</tt> is either too great or negative.
 * <br><br>
 * @param i
 * zero-based index of the desired EDGE.
 */
    EDGE* edge(int i) const;

    // Get an edge at a vertex known to contain at most one pointer
    // to an edge.  NB It will return null if the vertex contains
    // more than one edge pointer.
/**
 * Gets an <tt>EDGE</tt> at a <tt>VERTEX</tt> known to contain at most one pointer to an <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>NULL</tt> if this <tt>VERTEX</tt> contains more than
 * one <tt>EDGE</tt> pointer.
 */
    EDGE* edge() const { return edge_ptr; }

    // Count the number of edges pointed to by the vertex.
    // NB The number of edges ending in the vertex will be equal to
    // or, more usually, greater than the number of pointers in the
    // vertex.
/**
 * Counts the number of <tt>EDGEs</tt> pointed to by this <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> The number of <tt>EDGEs</tt> ending in a <tt>VERTEX</tt> is equal
 * to or, greater than, the number of pointers in the <tt>VERTEX</tt>.
 */
    int count_edges() const;

    // Test if the given edge is pointed to by the vertex.
/**
 * Tests whether or not the specified <tt>EDGE</tt> is pointed to by this <tt>VERTEX</tt>.
 * <br><br>
 * @param edge
 * the EDGE to test.
 */
    logical edge_linked(EDGE* edge) const;
    /**
     * Returns the <tt>APOINT</tt> that defines the position of this <tt>VERTEX</tt>.
     */
    APOINT* geometry() const { return geometry_ptr; }
    /**
     * Returns a pointer to the owning entity.
     */
    ENTITY* owner() const;

    /**
     * Returns the tolerance of the tolerant <tt>TVERTEX</tt> made from this <tt>VERTEX</tt>.
     * <br><br>
     * <b>Role:</b> This <tt>const</tt> version of this function is less efficient than the
     * non-<tt>const</tt> version because it cannot store the computed parameter range in the
     * <tt>EDGE</tt> for future use, but is required for constant <tt>EDGEs</tt>.
     */
    virtual double get_tolerance() const;
    /**
     * Returns the tolerance of the tolerant <tt>TVERTEX</tt> made from this <tt>VERTEX</tt>.
     * <br><br>
     * <b>Role:</b> This routine is for backwards compatibility. It saves the evaluated
     * range and cannot be declared <tt>const</tt>.
     */
    virtual double get_tolerance();

    // Extended tolerance routines which return the current value of the
    // tolerance that should be used.  For ordinary vertices it is resabs/2 in
    // order to be consistent with the interpretation that the tolerance of an
    // entity is a radius, not a diameter.
/**
 * @nodoc
 */
    virtual double get_curr_tolerance() const;
    /**
     * @nodoc
     */
    virtual double get_curr_tolerance();

    /**
     * @nodoc
     */
    virtual logical is_tolerant() const;

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

    // Delete any existing pointers to edges in the vertex and then
    // place a pointer to the given edge in the vertex.
/**
 * Sets the owning <tt>EDGE</tt> of this <tt>VERTEX</tt> to the specified <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> Deletes any existing pointers to <tt>EDGEs</tt> in this <tt>VERTEX</tt>
 * and then places a pointer to the given <tt>EDGE</tt> in the <tt>VERTEX</tt>.
 * Before performing the change, it checks if the data structure is posted on
 * the bulletin board. If not, the method calls <tt>backup</tt> to put an entry
 * on the bulletin board.
 * <br><br>
 * @param edge
 * the new parent EDGE.
 * @param reset_pattern
 * internal use only.
 */
    void set_edge(EDGE* edge, logical reset_pattern = TRUE);

    // Add a pointer to the given edge to those pointers already in
    // the vertex.
/**
 * Adds a pointer to the given <tt>EDGE</tt> to those pointers already in this <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param edge
 * the EDGE whose pointer is to be added.
 */
    void add_edge(EDGE* edge);

    // Remove a pointer to the given edge from the vertex.  Does
    // nothing if no such pointer exists.
/**
 * Removes the pointer to the specified <tt>EDGE</tt> from this <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> Does nothing if no such pointer exists.
 * <br><br>
 * @param edge
 * the EDGE whose pointer is to be deleted.
 */
    void delete_edge(EDGE* edge);
    /**
     * Sets the geometry of this <tt>VERTEX</tt> to the specified <tt>APOINT</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param point
     * the new APOINT.
     * @param reset_pattern
     * internal use only.
     */
    void set_geometry(APOINT* point, logical reset_pattern = TRUE);

    // Make a tolerant TVERTEX out of this vertex.
/**
 * Makes a tolerant <tt>TVERTEX</tt> from this <tt>VERTEX</tt>.
 * <br><br>
 * @param tol
 * the specified tolerance.
 */
    TVERTEX* make_tolerant(double tol);

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
    void** gme_get_edge_ptr();
    void** gme_get_geometry_ptr();
};

// Jeff 01.30.07 AUTO_MERGE_PERIODIC_VERTICES
/**
 * Attempts the merge of the current list of periodic seam vertex candidates.
 * <br><br>
 * <b>Role:</b> Candidate vertices are recorded when the option "auto_merge_periodic_vertices"
 * is enabled, and periodic edges are split during the current operation. The vertices are normally
 * merged at the end of the operation but can be merged at an earlier time with this function.
 */
void DECL_KERN auto_merge_periodic_vertex_candidates();
/** @} */
#endif

