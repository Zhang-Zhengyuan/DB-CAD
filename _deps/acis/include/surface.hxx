/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SURFACE.
// The SURFACE provides the basic framework for the range of surface
// geometries implemented at any time in the modeller.  It provides a
// use-count, so that surfaces may be multiply referenced, plus a range
// of virtual functions which provide access to the specific geometries'
// implementations.
#ifndef SURFACE_CLASS
#define SURFACE_CLASS
#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "en_macro.hxx"
#include "usecount.hxx"
/**
 * \defgroup ACISGEOMETRY Geometry
 * \ingroup KERNAPI
 *
 */
 /**
  * @file surface.hxx
  * @CAA2Level L1
  * @CAA2Usage U2
  * \addto ACISGEOMETRICENTITIES
  *
  * @{
  */
class LOOP;
class FACE;
class surface;
class SPAtransf;
class SPAbox;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SURFACE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int SURFACE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define SURFACE_LEVEL 1

/**
 * Defines a generic surface as an object in the model.
 * <br><br>
 * <b>Role:</b> <tt>SURFACE</tt> is a model geometry class that contains a pointer to a
 * (lowercase) surface, the corresponding construction geometry class. In general,
 * a model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
 * model object. It provides model management functionality, in addition to the
 * geometry definition.
 * <br><br>
 * A <tt>SURFACE</tt> provides the basic framework for the range of surface geometries
 * implemented in the modeler. Additional classes are derived from <tt>SURFACE</tt> to
 * define specific types of surfaces, such as <tt>CONE</tt>, <tt>MESHSURF</tt>,
 * <tt>PLANE</tt>, <tt>SPHERE</tt> and <tt>TORUS</tt>.
 * <br><br>
 * Along with the usual <tt>ENTITY</tt> class methods, <tt>SURFACE</tt> has member
 * methods to provide access to specific implementations of the geometry. For example,
 * a surface can be transformed by a given transform operator, resulting in another surface.
 * <br><br>
 * A use count allows multiple references to a <tt>SURFACE</tt>. The construction of a new
 * <tt>SURFACE</tt> initializes the use count to 0. Methods are provided to increment and
 * decrement the use count, and after the use count returns to 0, the entity is deleted.
 * @see ENTITY, FACE
 */
class DECL_KERN SURFACE : public ENTITY {

private:
    // The use-count holds the number of faces referencing the SURFACE

// STI jmb: Handle save/restore of use counted histories
//	unsigned int use_count_data;
// STI jmb: end


    // Track the ENTITYs owning the SURFACE
private:
    ENTITY** owners_ptr;
    int num_owners;
    int num_owners_alloc;

protected:
    /**
     * Returns TRUE if this entity can have a pattern_holder. For internal use only.
     */
    logical holds_pattern() const { return TRUE; }
public:
    /**
     * Adds the <tt>owner</tt> argument to the list of owners.
     * <br><br>
     * @param owner
     * owner to be added.
     * @param count
     * flag to increment use count or not.
     */
    void add_owner(ENTITY* owner, logical count = TRUE);

    /**
     * Removes the <tt>owner</tt> argument from the list of owners.
     * <br><br>
     * @param owner
     * owner to be removed.
     * @param count
     * flag to decrement the use count.
     * @param lose
     * flag to lose if use count is zero.
     */
    void remove_owner(ENTITY* owner, logical count = TRUE, logical lose = TRUE);
    /**
     * Copies the list of owners of this <tt>SURFACE</tt> to the <tt>list</tt> argument.
     * <br><br>
     * <b>Role:</b> Returns the number of owners copied.
     * <br><br>
     * @param list
     * list of owners.
     */
    int get_owners(ENTITY_LIST& list) const;

    /**
     * Returns the first owner of this <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> If there are no owners, NULL is returned.
     */
    ENTITY* owner() const;


    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For <tt>identical_comparator</tt> to be <tt>TRUE</tt> requires an
     * exact match when comparing doubles, and returns the result of memcmp as a
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
    ENTITY_FUNCTIONS(SURFACE, KERN)
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

    // Indicate whether this entity is normally destroyed by lose(),
    // or whether it gets destroyed implicitly when every owner has
    // been lost. Most entities are destroyed explicitly using lose(),
    // and so the default returns TRUE.
/**
 * Indicates whether this <tt>SURFACE</tt> is normally destroyed by lose.
 * <br><br>
 * <b>Role:</b> The method returns <tt>FALSE</tt>, indicating that <tt>SURFACEs</tt>
 * are, by default, shared among multiple owners by means of use-count tracking,
 * and so are destroyed implicitly when every owner has been lost. Classes derived
 * from <tt>SURFACE</tt> may of course override this behavior
 */
    virtual logical deletable() const;


    // Now the functions specific to SURFACE.

    // Make a generic surface. Usually created in one of its derived
    // incarnations, but can be made stand-alone when reading in a
    // saved file containing unrecognised sub-classes of SURFACE.
    // Initialises the use count to zero.
/**
 * Constructs a <tt>SURFACE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    SURFACE();

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>SURFACE</tt> from a <tt>SAT</tt> file.
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
 *    if (!get_standard_save_flag())
 *       read_int  use count data
 *    // Nothing to copy or restore under normal circumstances.
 * </pre>
 */
    void restore_common();
#endif

    // STI jmb: Handle save/restore of use counted histories
    // 	int use_count() const { return use_count_data; }
    //
    //
    // 	// Use count manipulation. Either add or subtract one use, and if
    // 	// subtraction causes the use count to fall to zero, then delete
    // 	// the record.
    //
    // 	void add();
    // 	void remove();
        /**
         * @nodoc
         */
    USE_COUNTED_DECL
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end

// These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier
 * <tt>SURFACE_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>SURFACE_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>SURFACE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string "surface".
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>SURFACE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // These functions is hidden from mkman in the USE_COUNTED_DECL macro; to have them documented,
    // we include them here:
#if 0
/**
 * Increments the use count of this <tt>SURFACE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
 * an entry on the bulletin board.
 */
    virtual void add();
    /**
     * Indicates if this <tt>SURFACE</tt> is use-counted or not.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> for <tt>SURFACEs</tt> unless overridden in a derived class.
     */
    virtual logical is_use_counted() const;
    /**
     * Decrements the use count of this <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> If the use count reaches 0, the <tt>SURFACE</tt> is deleted. Before performing
     * the change, it checks if the data structure is posted on the bulletin board. If not,
     * the method calls <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param lose_if_zero
     * flag to lose if use count is zero.
     */
    virtual void remove(logical lose_if_zero = TRUE);
    /**
     * Sets the reference use count of this <tt>SURFACE</tt>.
     * <br><br>
     * @param val
     * new use count.
     */
    virtual void set_use_count(int val);
    /**
     * Returns the use count of this <tt>SURFACE</tt>.
     */
    virtual int use_count() const;
#endif

    // Return the surface equation for reading only.
/**
 * Returns the equation of this <tt>SURFACE</tt>.
 */
    virtual const surface& equation() const;

    // Return the surface equation.
/**
 * Returns the equation of this <tt>SURFACE</tt> for update operations.
 * <br><br>
 * <b>Role:</b> Before a change is performed the change, the bulletin board is
 * checked to see if the data structure has been posted. If not, <tt>backup</tt>
 * is called to put an entry on the bulletin board.
 */
    virtual surface& equation_for_update();


    // Transform a surface (action taken only by derived classes).
/**
 * Returns the transformed surface.
 * <br><br>
 * <b>Role:</b> If the logical argument is <tt>TRUE</tt>, the returned surface is reversed.
 * <br><br>
 * @param t
 * transform to apply.
 * @param reverse
 * flag to reverse the surface.
 */
    virtual surface* trans_surface(
        const SPAtransf& t = *(SPAtransf*)NULL_REF, logical reverse = FALSE) const;
    /**
     * Transforms the equation of this <tt>SURFACE</tt>.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param t
     * transform to apply.
     */
    virtual void operator*=(const SPAtransf& t);


    // Construct a bounding SPAbox for a face.  Although the generic record
    // type should never exist, this function is defined for it, to
    // return a SPAbox enclosing all the edges of the given list of loops.
    // This is sufficient for any ruled surface type, for which any point
    // in a face must be a linear combination of some two points on its
    // boundary.

    // wseibold (Feb`02) added argument to compute tighter bounding boxes
/**
 * Constructs a bounding box for a <tt>FACE</tt>.
 * <br><br>
 * <b>Role:</b> Although the generic record type should never exist, this function
 * is defined for it, to return a box enclosing all the edges of the given list of
 * <tt>LOOPs</tt>. This is sufficient for any ruled surface type, for which any point in a
 * <tt>FACE</tt> must be a linear combination of some two points on its boundary.
 * <br><br>
 * @param loop
 * list of LOOPs.
 * @param t
 * for future use.
 * @param tight_box
 * for future use.
 * @param untransformed_box
 * for future use.
 */
    virtual SPAbox make_box(LOOP* loop = NULL, // STL amt 26Jun03: default value added
        const SPAtransf* t = NULL,
        logical tight_box = FALSE,
        SPAbox* untransformed_box = NULL) const;

    // STI swa 19Jul02 - New method for detection of clash with SPAbox.
    //                   Note, the default methods will assume the
    //                   SPAbox and surface interfere when no test is
    //                   available.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual logical box_clash(const SPAbox& test_box,
        const SPAtransf& surf_transf = *(SPAtransf*)NULL_REF,
        double tol = SPAresabs) const;


    // Routine used for system debugging
/**
 * @nodoc
 */
    virtual int lookup(logical) const;// internal use only

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;// internal use only
    // STI ROLL
};
/** @} */
#endif
