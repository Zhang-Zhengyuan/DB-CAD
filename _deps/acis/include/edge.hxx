/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for the EDGE class.

// The edge class represents the physical edge as recognised by the
// user.  It consists of a bounded portion of a space curve, the bounds
// being given as object-space vertices. (Any parametric bounds required
// for a parametric curve may be recorded within the edge.  These bounds are
// calculated from the vertex information and may be NULL if they
// have not been regenerated since the edge was last modified or
// retrieved.)

// The vertex pointer at either or both ends may be NULL,
// in which case the edge is taken to be unbounded in that direction.
// If the underlying curve is infinite, then so is the unbounded edge;
// if the curve is closed, then the vertex pointers must both be present
// or both NULL, and in the latter case the edge is the whole curve.

// As a special case, the geometry pointer may be NULL, while both
// vertex pointers point to the same vertex.  This is taken to mean that
// the "edge" is an isolated point (for example, the apex of a cone).
// Isolated points often represent non-manifold vertices and therefore
// require special care when encountered.  Edges with NULL geometry
// pointers should not be connected to edges with geometry; however,
// some algorithms (for example, Boolean operations) may use them
// during the course of the algorithm.

#ifndef EDGE_CLASS
#define EDGE_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "en_macro.hxx"
#include "vector.hxx"
#include "interval.hxx"
#include "usecount.hxx"
/**
 * @file edge.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

class SPAbox;
class SPAinterval;
class SPAposition;
class SPAparameter;

class pattern_holder;
class pattern;
class VOID_LIST;

class tolerize_options;

// STI TOL_MOD jmg: convexity enum
// convexity - these values are used for subscripts into the scoring array
/*
// tbrv
*/
/**
 * @nodoc
 */
enum EDGE_cvty
{
    EDGE_cvty_concave = 0,
    EDGE_cvty_tangent_concave = 1,
    EDGE_cvty_tangent = 2,  // dont know what sort
    EDGE_cvty_tangent_convex = 3,
    EDGE_cvty_convex = 4,
    EDGE_cvty_knife_convex = 5,
    EDGE_cvty_knife = 6,   // dont know what sort
    EDGE_cvty_knife_concave = 7,
    EDGE_cvty_tangent_inflect = 8,  // here to keep symmetry as far as pos
    EDGE_cvty_unknown = 9,
    EDGE_cvty_mixed = 10
};
// STI TOL_MOD jmg: end

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int EDGE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define EDGE_LEVEL 1


/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(EDGE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// EDGE declaration proper
/**
 * Represents a physical edge.
 * <br>
 * <b>Role:</b> An edge represents a physical edge as recognized by the user. It consists
 * of a bounded portion of a space curve, the bounds being given as object-space vertices.
 * (Any parametric bounds required for a parametric curve may be recorded with the
 * <tt>EDGE</tt>. These bounds are calculated from the vertex information and may be
 * <tt>NULL</tt> if they have not been regenerated since the edge was last modified
 * or retrieved.)
 * <br><br>
 * The vertex pointer at either or both ends can be <tt>NULL</tt>, in which case the
 * <tt>EDGE</tt> is taken to be unbounded in that direction. If the underlying curve
 * is infinite, so is the unbounded edge. If the curve is closed, the vertex pointers
 * must both be present or both <tt>NULL</tt>. In the latter case, the edge is the
 * whole curve.
 * <br><br>
 * As a special case, the geometry pointer can be <tt>NULL</tt> while both vertex pointers
 * reference the same <tt>VERTEX</tt>. This means that the <tt>edge</tt> is an
 * isolated point, such as the apex of a cone. Isolated points often represent
 * nonmanifold vertices and therefore require special care when encountered. Edges
 * with <tt>NULL</tt> geometry pointers should not be connected to edges with geometry.
 * Some algorithms, such as Boolean operations, may use them during the course of the algorithm.
 * <br><br>
 * The <tt>query.hxx</tt> file contains topological traversal functions that
 * are useful for generating lists of edges on other topological entities. Similarly,
 * <tt>sg_get_edges_of_wire</tt> or <tt>sg_q_edges_around_vertex</tt> may be of interest.
 * Several geometric inquiry functions are located in <tt>geometry.hxx</tt>. When
 * splitting an edge, <tt>sg_split_edge_at_vertex</tt> may be helpful, and
 * <tt>get_edge_box</tt> may be useful to retrieve or recalculate an the bounding box of
 * an edge.
 */

class DECL_KERN EDGE : public ENTITY {

    // The replace_x_with_tx functions need special access to protected parts of EDGE so they have been
    // made friends. This became necessary when the TVERTEX versions of the set_start/end methods were
    // merged with the VERTEX versions. AL 07Mar2007.

    friend DECL_KERN logical replace_edge_with_tedge_internal(
        EDGE*, logical, logical, TEDGE*&, double const&, logical, tolerize_options*);

    friend DECL_KERN void replace_vertex_with_tvertex(VERTEX*, TVERTEX*&);

    friend DECL_KERN void set_edge_tolerance(EDGE*, double);

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
     * <b>Role:</b> Returns all patterns.
     * <br><br>
     * @param list
     * list of patterns.
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
     * <b>Role:</b> Returns <tt>TRUE</tt> if this is a <tt>pattern child</tt>. An entity is a
     * <tt>pattern child</tt> when it is not responsible for creating new entities
     * when the pattern is applied.  Instead, some owning entity takes care of this.
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

    // Pointers to the vertices of the edge. They may be NULL, with
    // meanings as described above.
/**
 * Pointer to the start <tt>VERTEX</tt> of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>VERTEX</tt> pointer at either or both ends may be <tt>NULL</tt>.
 */
    VERTEX* start_ptr;
    /**
     * Pointer to the end <tt>VERTEX</tt> of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The <tt>VERTEX</tt> pointer at either or both ends may be <tt>NULL</tt>.
     */
    VERTEX* end_ptr;

    // Pointer to one of the coedges lying on this edge.
    // No significance attaches to the choice of coedge,
    // which may change during a modelling operation.
/**
 * Pointer to one of the <tt>COEDGEs</tt> lying on this <tt>EDGE</tt>.
 */
    COEDGE* coedge_ptr;

    // The geometric shape of the edge.  Every curve description is of
    // a directed curve; the sense defines whether the edge shares this
    // direction, or has the opposite one.
/**
 * Pointer to the <tt>CURVE</tt> describing the geometry of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> The geometry pointer can be <tt>NULL</tt> while both <tt>VERTEX</tt>
 * pointers point to the same <tt>VERTEX</tt>.
 */
    CURVE* geometry_ptr;
    /**
     * The sense of the <tt>CURVE</tt> describing this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Every curve description is of a directed curve; the sense defines
     * whether or not this <tt>EDGE</tt> shares its underlying <tt>CURVE</tt>'s direction.
     */
    REVBIT sense_data;

    // Pointer to a geometric bounding region (a SPAbox), within which the
    // entire edge lies (with respect to its body's internal
    // coordinate system).
    // It may be NULL if no such bound has been calculated since the
    // edge was last changed.

    // Contains the use counted box.
    entity_box_container box_container;

    // STI TOL_MOD jmg: changed the range from a pointer to an SPAinterval to
    // an SPAinterval.

    // Interval bounding the parametric region, within
    // which the entire edge lies (with respect to its body's internal
    // coordinate system).
    // It may be infinite if no such bound has been calculated since the
    // edge was last changed.
/**
 * The parametric bounding interval for this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> Specifies an interval that includes the entire <tt>EDGE</tt> (with respect to
 * the internal coordinate system of the <tt>BODY</tt>). The interval may be infinite
 * if no such bound has been calculated since the <tt>EDGE</tt> was last changed.
 */
    SPAinterval param_bound;
    // STI TOL_MOD jmg: end

    // The convexity of an edge is now stored with the edge. The default value is
    // EDGE_cvty_unknown. ONLY internal ACIS function should change this value.
/**
 * The convexity of this <tt>EDGE</tt>.
 */
    mutable EDGE_cvty cvty;

    // Tolerance value
    // Negative value indicates if the tolerance value needs to be recalculated.
/**
 * @nodoc
 */
    double tolerance;

    /**
     * Assigns a pointer to the start <tt>VERTEX</tt> of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The <tt>VERTEX</tt> pointer at either or both ends can be <tt>NULL</tt>,
     * in which case the <tt>EDGE</tt> is taken to be unbounded in that direction.
     * If the underlying <tt>CURVE</tt> is infinite, then so is the unbounded <tt>EDGE</tt>;
     * if the <tt>CURVE</tt> is closed, then the <tt>VERTEX</tt> pointers must both
     * be present or both <tt>NULL</tt>, and in the latter case the <tt>EDGE</tt> is
     * the entire curve.
     * <br><br>
     * @param vertex
     * the new start VERTEX.
     * @param reset_pattern
     * internal use only.
     */
    virtual void set_start_ptr(VERTEX*, logical reset_pattern = TRUE);
    /**
     * Assigns a pointer to the end <tt>VERTEX</tt> of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The <tt>VERTEX</tt> pointer at either or both ends can be <tt>NULL</tt>,
     * in which case the <tt>EDGE</tt> is taken to be unbounded in that direction.
     * If the underlying <tt>CURVE</tt> is infinite, then so is the unbounded <tt>EDGE</tt>;
     * if the <tt>CURVE</tt> is closed, then the <tt>VERTEX</tt> pointers must both
     * be present or both <tt>NULL</tt>, and in the latter case the <tt>EDGE</tt> is
     * the entire curve.
     * <br><br>
     * @param vertex
     * the new end VERTEX.
     * @param reset_pattern
     * internal use only.
     */
    virtual void set_end_ptr(VERTEX* vertex, logical reset_pattern = TRUE);

    /**
     * Sets the pointer to the underlying <tt>CURVE</tt> geometry for this <tt>EDGE</tt>.
     * <br><br>
     * @param crv
     * the new CURVE.
     * @param reset_pattern
     * internal use only.
     */
    virtual void set_geometry_ptr(CURVE* crv);// internal use only

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
    ENTITY_FUNCTIONS(EDGE, KERN)
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


    // Now the functions specific to EDGE.

    // Basic constructor, used internally for bulletin board activity.
/**
 * Constructs an <tt>EDGE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    EDGE();


    // Public constructor which initialises the record and interfaces
    // with the bulletin board system.  The arguments initialise
    // start_ptr, end_ptr, geometry_ptr and sense_data respectively.
    // If necessary, the edge pointers in the two end vertices are set,
    // and the use count in the curve geometry is incremented.
/**
 * Constructs an <tt>EDGE</tt> from a start <tt>VERTEX</tt>, an end <tt>VERTEX</tt>, and a <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * The arguments initialize the start <tt>VERTEX</tt>, the end <tt>VERTEX</tt>, the
 * <tt>CURVE</tt>, and the sense of the <tt>EDGE</tt> relative to the underlying
 * <tt>CURVE</tt> geometry. If necessary, the <tt>EDGE</tt> pointers in the two
 * <tt>VERTEXes</tt> are set to the <tt>EDGE</tt>, and the use count in the
 * <tt>CURVE</tt> geometry is incremented.
 * <br><br>
 * @param start
 * start VERTEX for the constructed EDGE.
 * @param end
 * end VERTEX for the constructed EDGE.
 * @param geometry
 * geometry of the constructed EDGE.
 * @param sense
 * sense of the constructed EDGE relative to its underlying CURVE.
 * @param cvty
 * convexity of the constructed EDGE (optional).
 * @param domain
 * parametric domain of the constructed EDGE (optional).
 */
    EDGE(VERTEX* start,
        VERTEX* end,
        CURVE* geometry,
        REVBIT sense,
        EDGE_cvty cvty = EDGE_cvty_unknown,
        SPAinterval const& domain = *(const class SPAinterval*)NULL_REF);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>EDGE</tt> from a SAT file.
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
 *    read_ptr  // start vertex
 *    if (old_version) {  // if pre-tolerant modeling
 *       read_real  // skip start parameter value
 *    else if (tol_mod_version) {  // else tolerant modeling
 *       read_real  // set start parameter
 *       read_real  // start parameter
 *    read_ptr  // end vertex
 *    if (old_version) {  // if pre-tolerant modeling
 *       read_real  // skip end parameter value
 *    else if (tol_mod_version) {  // else tolerant modeling
 *       read_real  // set end parameter
 *       read_real  // end parameter
 *    read_ptr  // coedge pointer
 *    read_ptr  // geometry pointer
 *    read_logical  // rev bit for sense
 *    if ( tol_mod_version ) {  // if tolerant modeling
 *       read_string  // convexity </pre>
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
 * <tt>EDGE_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>EDGE_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>EDGE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>edge</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>EDGE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // Data reading routines.

/**
 * Returns a pointer to the start <tt>VERTEX</tt> of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> The pointer can be <tt>NULL</tt>.
 */
    VERTEX* start() const;
    /**
     * Returns a pointer to the end <tt>VERTEX</tt> of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The pointer can be <tt>NULL</tt>.
     */
    VERTEX* end() const;
    /**
     * Returns a pointer to one of the <tt>COEDGEs</tt> lying on this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> No significance is attached to the choice of <tt>COEDGE</tt>, which can change
     * during a modeling operation.
     */
    COEDGE* coedge() const { return coedge_ptr; }
    /**
     * Returns a pointer to the underlying <tt>CURVE</tt> geometry.
     */
    CURVE* geometry() const { return geometry_ptr; }
    /**
     * Returns the sense of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Indicates whether this <tt>EDGE</tt> shares the direction of the underlying
     * <tt>CURVE</tt>, or has the opposite direction.
     */
    REVBIT sense() const { return sense_data; }
    /**
     * Returns a geometric bounding region (box) for this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Returns a box that includes the entire <tt>EDGE</tt> (with respect to
     * the internal coordinate system of the <tt>BODY</tt>). The return may be <tt>NULL</tt>
     * if the bound was not calculated since the <tt>EDGE</tt> was last modified.
     */
     /*@todo:entity_box_container类未实现*/
    SPAbox* bound() const { return box_container.get_box(); }
    /**
     * Returns a pointer to the owning entity.
     */
    ENTITY* owner() const;
    /**
     * Returns the tolerance of the tolerant <tt>TEDGE</tt> made from this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This <tt>const</tt> version of this function is less efficient than the
     * non-<tt>const</tt> version because it cannot store the computed parameter range in the
     * <tt>EDGE</tt> for future use, but is required for constant <tt>EDGEs</tt>.
     */
    virtual double get_tolerance() const;
    /**
     * Returns the tolerance of the tolerant <tt>TEDGE</tt> made from this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This routine is for backwards compatibility. It saves the
     * evaluated range and cannot be declared <tt>const</tt>.
     */
    virtual double get_tolerance();

    // Extended tolerance routines which return the current value of the
    // tolerance that should be used.  For ordinary edges it is resabs/2 in
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

    /**
     * Returns a pointer to one of the <tt>COEDGEs</tt> lying on this <tt>EDGE</tt>, associated with the given <tt>FACE</tt>.
     * <br><br>
     * @param face
     * the FACE upon which the returned COEDGE is to lie.
     */
    COEDGE* coedge(FACE* face) const;

    /**
     * Returns the convexity of this <tt>EDGE</tt>.
     */
    EDGE_cvty get_convexity() const;

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets the <tt>EDGE</tt>'s start <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param vertex
 * the new start VERTEX.
 * @param reset_pattern
 * internal use only.
 * @param update_pr
 * internal use only.
 */
    void set_start(VERTEX* vertex, logical reset_pattern = TRUE, int update_pr = 1);

    // This member has been merged with the VERTEX version. AL 07Mar2007
    //    void set_start( TVERTEX *tvertex, logical reset_pattern = TRUE );

    /**
     * Sets the <tt>EDGE</tt>'s end <tt>VERTEX</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure is
     * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param vertex
     * the new end VERTEX.
     * @param reset_pattern
     * internal use only.
     * @param update_pr
     * internal use only.
     */
    void set_end(VERTEX* vertex, logical reset_pattern = TRUE, int update_pr = 1);
    // This member has been merged with the VERTEX version. AL 07Mar2007  
    //    void set_end( TVERTEX *tvertex, logical reset_pattern = TRUE );

    /**
     * Sets the start of a list of <tt>COEDGEs</tt> corresponding to this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure is
     * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param coedge
     * the new parent COEDGE.
     * @param reset_pattern
     * internal use only.
     */
    void set_coedge(COEDGE* coedge, logical reset_pattern = TRUE);

    /**
     * Sets this <tt>EDGE</tt>'s geometry to the given <tt>CURVE</tt>.
     * <br><br>
     * <b>Role:</b> Side effects of this method are adjusting the use counts of the existing
     * and new geometries, and deleting the existing geometry if it is no longer referenced.
     * Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
     * entry on the bulletin board.
     * <br><br>
     * @param crv
     * the new CURVE.
     * @param reset_pattern
     * internal use only.
     */
    virtual void set_geometry(CURVE* crv, logical reset_pattern, logical reset_tol_geom);
    virtual void set_geometry(CURVE* crv, logical reset_pattern = TRUE);

    /**
     * Sets the sense of this <tt>EDGE</tt> with respect to the underlying <tt>CURVE</tt>.
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
    virtual void set_sense(REVBIT sense, logical reset_pattern = TRUE);

    /**
     * Sets the bounding region (box) of this <tt>EDGE</tt> to the specified box.
     * <br><br>
     * <b>Role:</b> Sets the <tt>EDGE's SPAbox</tt> pointer to point to the given
     * <tt>SPAbox</tt>. Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt> to
     * put an entry on the bulletin board.
     * <br><br>
     * @param box
     * the new SPAbox.
     */
     /*@todo:entity_box_container类未实现*/
    void set_bound(SPAbox* in_box) { box_container.set_box(this, in_box); }

    /**
     * Sets the convexity of this <tt>EDGE</tt> to the given value.
     * <br><br>
     * @param cvty
     * the new convexity.
     * @param reset_pattern
     * internal use only.
     */
    void set_convexity(EDGE_cvty cvty, logical reset_pattern = TRUE);

    // This function checks the given range with the start
    // and end vertexes if they do not agree it returns
    // FALSE and does not set the range.  If the range could
    // be set it returns TRUE
/**
 * Checks the parameter range of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b>  This method checks the given range against the start and end
 * <tt>VERTEXes</tt>. If these are not in agreement it returns <tt>FALSE</tt> and
 * does not set the range. If the range could be set it returns <tt>TRUE</tt>.
 * <br><br>
 * @param range
 * the parameter range to check.
 */
    logical set_param_range(SPAinterval const& range);

    // Routines for finding the positions of the start and end of
    // the edge. Used a lot, so convenient to have.

/**
 * Returns the start position of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> This function does not take into account an optional transformation
 * on the owning body. That is the responsibility of the calling function.
 */
    virtual SPAposition start_pos() const;
    /**
     * Returns the end position of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This function does not take into account an optional transformation
     * on the owning body. That is the responsibility of the calling function.
     */
    virtual SPAposition end_pos() const;
    /**
     * Returns the midpoint of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The midpoint of an edge can be calculated either by
     * evaluating at the edge mid-parameter value, or by finding the arc-length
     * midpoint of the edge. Setting <tt>approx</tt> to <tt>FALSE</tt> returns
     * the exact <i>geometrical</i> midpoint of the edge using arc-length. Setting
     * <tt>approx</tt> to <tt>TRUE</tt> returns the midpoint by evaluating
     * at the mid-parameter value. Depending on the parameterization of the
     * underlying curve, the mid-parameter value may not correspond to the
     * geometrical midpoint of the edge. Default value is <tt>TRUE</tt>.
     *
     * This function does not take into account an optional transformation
     * on the owning body. That is the responsibility of the calling function.
     * <br><br>
     * @param approx
     * flag to use approximate (parametric) midpoint.
     */
    virtual SPAposition mid_pos(logical approx = TRUE) const;
    /**
     * Returns the derivative at the start parameter of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This function does not take into account an optional transformation
     * on the owning body. That is the responsibility of the calling function.
     */
    virtual SPAvector start_deriv() const;
    /**
     * Returns the derivative at the end parameter of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This function does not take into account an optional transformation
     * on the owning body. That is the responsibility of the calling function.
     */
    virtual SPAvector end_deriv() const;
    /**
     * Returns the derivative at the midpoint of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> The midpoint of an edge can be calculated either by
     * evaluating at the edge mid-parameter value, or by finding the arc-length
     * midpoint of the edge. Setting <tt>approx</tt> to <tt>FALSE</tt> returns
     * the exact <i>geometrical</i> midpoint of the edge using arc-length. Setting
     * <tt>approx</tt> to <tt>TRUE</tt> returns the midpoint by evaluating
     * at the mid-parameter value. Depending on the parameterization of the
     * underlying curve, the mid-parameter value may not correspond to the
     * geometrical midpoint of the edge. Default value is <tt>TRUE</tt>.
     *
     * This function does not take into account an optional transformation
     * on the owning body. That is the responsibility of the calling function.
     * <br><br>
     * @param approx
     * flag to use approximate (parametric) midpoint.
     */
    virtual SPAvector mid_point_deriv(logical approx = TRUE) const;

    // Routines for finding the SPAparameter bounds of the edge on its
    // curve. The first two are for backwards compatibility, the third
    // is to be preferred in cases where both parameters are needed
    // (which means almost always). They all save the evaluated range
    // in case it is wanted again, so cannot be declared const.

/**
 * Returns the parameter defining the start of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> This routine is for backwards compatibility. It saves the
 * evaluated range and cannot be declared const.
 */
    SPAparameter start_param();
    /**
     * Returns the parameter defining the end of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This is the preferred routine when there is a need for both
     * start and end parameters. It saves the evaluated range and cannot be declared <tt>const</tt>.
     */
    SPAparameter end_param();

    logical param_bounded() const;

    /**
     * Returns the parameter range of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This routine is for backwards compatibility. It saves the
     * evaluated range and cannot be declared <tt>const</tt>.
     */
    SPAinterval param_range();
    // For emergency use, we also have a const version which does not
    // save the value. Since this can be expensive, one hopes it is
    // not used too often.

/**
 * Returns the parameter defining the start of this <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> This const version of this function is less efficient than the
 * non-<tt>const</tt> version because it cannot store the computed parameter range in
 * the <tt>EDGE</tt> for future use, but is required for constant <tt>EDGEs</tt>.
 */
    SPAparameter start_param() const;
    /**
     * Returns the parameter defining the end of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This const version of this function is less efficient than the
     * non-<tt>const</tt> version because it cannot store the computed parameter range in
     * the <tt>EDGE</tt> for future use, but is required for constant <tt>EDGEs</tt>.
     */
    SPAparameter end_param() const;
    /**
     * Returns the parameter range of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This <tt>const</tt> version of this function is less efficient than the
     * non-<tt>const</tt> version because it cannot store the computed parameter range in
     * the <tt>EDGE</tt> for future use, but is required for constant <tt>EDGEs</tt>.
     */
    SPAinterval param_range() const;
    /**
     * Returns the parameter range of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> This routine simply returns the parameter range of the <tt>EDGE</tt>,
     * whether it has been computed or not. This is a <tt>const</tt> method and does not
     * change the edge.
     */
    SPAinterval get_param_range() const { return param_bound; }
    /**
     * @nodoc
     */
    SPAinterval calculate_param_range() const;

    // Make a tolerant TEDGE out of this edge.
/**
 * Makes a tolerant <tt>TEDGE</tt> from this <tt>EDGE</tt>.
 */
    TEDGE* make_tolerant(double tol, logical approx_ok);

    // Check to see if we have an edge bounded by vertices
/**
 * Returns <tt>TRUE</tt> if this <tt>EDGE</tt> is bounded by <tt>VERTEXes</tt>.
 */
    logical vertex_bounded() const;

    /**
     * Returns a string representing the convexity of this <tt>EDGE</tt>.
     * <br><br>
     * <b>Role:</b> Returns a string representing the convexity of this <tt>EDGE</tt>,
     * for example, <tt>"concave"</tt>, <tt>"tangent"</tt>, and <tt>"unknown"</tt>.
     */
    char const* convexity_string() const;
    /**
     * Assigns a convexity to this <tt>EDGE</tt>, for example, <tt>"concave"</tt>, <tt>"tangent"</tt>, and <tt>"unknown"</tt>.
     * <br><br>
     * @param string
     * string indicating the convexity.
     */
    EDGE_cvty string_convexity(char* string) const;

    // Utility methods
/**
 * Returns the length of the underlying geometry <tt>CURVE</tt>. If approx_ok is true,
 * and the edge curve has a B-spline approximation, then this approximation is used in the length calculation.
 */
    double	length(logical approx_ok = TRUE)	const;

    /**
     * Returns <tt>TRUE</tt> if this <tt>EDGE</tt> is closed.
     */
    logical closed()	const;
    /**
     * Returns <tt>TRUE</tt> if this <tt>EDGE</tt> is periodic.
     */
    logical periodic()	const;

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
    void** gme_get_start_ptr();
    void** gme_get_end_ptr();
    void** gme_get_coedge_ptr();
    void** gme_get_geometry_ptr();
};
/** @} */
#endif
