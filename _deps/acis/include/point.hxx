/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for APOINT.
// A point records the object space SPAposition of a vertex.  There is
// no need for derived types. (This record class could be combined
// with the VERTEX but is kept separate, however, for consistency.)
// Cartesian coordinates are assumed.
#ifndef APOINT_CLASS
#define APOINT_CLASS
/**
 * \defgroup ACISGEOMETRICENTITIES Geometric Entities
 * \ingroup ACISGEOMETRY
 *
 */

 /**
  * @file point.hxx
  * @CAA2Level L1
  * @CAA2Usage U2
  * \addtogroup ACISGEOMETRICENTITIES
  *
  * @{
  */
#include "dcl_kern.h"
#include "logical.h"
#include "en_macro.hxx"
#include "position.hxx"
#include "usecount.hxx"
class SPAtransf;
class VERTEX;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(APOINT, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int APOINT_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define APOINT_LEVEL 1


// Point definition proper. There is only one sort of point, so we
// make it directly a derived class of ENTITY.

/**
 * Represents the position of a point as an object in the model.
 * <br>
 * <b>Role:</b> <tt>APOINT</tt> is a model geometry class that records the object space position of a
 * point. In general, a model geometry class is derived from <tt>ENTITY</tt> and is used to define a
 * permanent model object. It provides model management functionality, in addition to the
 * geometry definition.
 * <br><br>
 * The geometric definition of a point is stored as a <tt>SPAposition</tt> in the <tt>APOINT</tt> class. Cartesian
 * coordinates are assumed, though in principle other coordinate systems might be implemented.
 * <br><br>
 * Along with the usual <tt>ENTITY</tt> class methods, <tt>APOINT</tt> has member methods to provide access to
 * specific implementations of the geometry. For example, methods are available to set and
 * retrieve the coordinates of a point.
 * <br><br>
 * A use count allows multiple references to an <tt>APOINT</tt>. The construction of a new <tt>APOINT</tt>
 * initializes the use count to 0. Methods are provided to increment and decrement the use count,
 * and after the use count returns to 0, the entity is deleted.
 * <br><br>
 * This class is named <tt>APOINT</tt> to avoid compile and link conflicts with the Microsoft class <tt>POINT</tt>.
 * @see ENTITY, VERTEX, SPAposition
 */
class DECL_KERN APOINT : public ENTITY {

private:

    // Use count for record, allowing multiple use.  Starts at zero, and
    // if ever it reaches zero again, the record can be deleted.
    // Holds the number of edges referencing the CURVE.

// STI jmb: Handle save/restore of use counted histories
//	unsigned int use_count_data;
// STI jmb: end


    // Coordinates of point, recorded as a SPAposition.

    SPAposition coords_data;

    // Track the ENTITYs owning the APOINT
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
     * @param increment
     * flag to increment use count or not.
     */
    void add_owner(ENTITY* owner, logical increment = TRUE);
    /**
     * Removes the <tt>owner</tt> argument from the list of owners.
     * <br><br>
     * @param owner
     * owner to be removed.
     * @param decrement
     * flag to decrement the use count.
     * @param zero_flag
     * flag to lose if use count is zero.
     */
    void remove_owner(ENTITY* owner, logical decrement = TRUE, logical zero_flag = TRUE);
    /**
     * Returns the first owner of this <tt>APOINT</tt>.
     * <br><br>
     * <b>Role:</b> If there are no owners, NULL is returned.
     */
    ENTITY* owner() const;

    /**
     * Copies the list of owners of this <tt>APOINT</tt> to the <tt>list</tt> argument.
     * <br><br>
     * <b>Role:</b> Returns the number of owners copied.
     * <br><br>
     * @param list
     * list of owners.
     */
    int get_owners(ENTITY_LIST& list) const;

    // method to write scheme commands to file for visualization
    // [in] pFp - file pointer
    // [in] Color - color of the entity drawn
    //
/**
 * @nodoc
 */
    void debug_scheme(FILE* pFp, int Color) const;

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
    ENTITY_FUNCTIONS(APOINT, KERN)
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
    FULLSIZE_FUNCTION
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif

    // Indicate whether this entity is normally destroyed by lose(),
    // or whether it gets destroyed implicitly when every owner has
    // been lost. Most entities are destroyed explicitly using lose(),
    // and so the default returns TRUE.
/**
 * Indicates whether this <tt>APOINT</tt> is normally destroyed by <tt>lose</tt>.
 * <br><br>
 * <b>Role:</b> The method returns <tt>FALSE</tt>, indicating that <tt>APOINTs</tt> are, by default,
 * shared among multiple owners by means of use-count tracking, and so are destroyed
 * implicitly when every owner has been lost.  Classes derived from <tt>APOINT</tt> may of
 * course override this behavior
 */
    virtual logical deletable() const;


    // Now the functions specific to APOINT.

    // Constructor for a null point, which has to be filled in later.
/**
 * Constructs an <tt>APOINT</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    APOINT();
    APOINT(const char* gme);

    // Public constructor for a point, given its cartesian coordinates
/**
 * Constructs an <tt>APOINT</tt> with the specified Cartesian coordinates.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as arguments. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param x
 * x-coordinate of the constructed APOINT.
 * @param y
 * y-coordinate of the constructed APOINT.
 * @param z
 * z-coordinate of the constructed APOINT.
 */
    APOINT(double x, double y, double z);
    APOINT(const char* gme, double x, double y, double z);
    // Constructor for a point, given a SPAposition.
/**
 * Constructs an <tt>APOINT</tt> at the specified position.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as arguments. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param pos
 * position of the constructed APOINT.
 */
    APOINT(const SPAposition& pos);
    APOINT(const char* gme, const SPAposition& pos);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>APOINT_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>APOINT_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>APOINT_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"point"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>APOINT</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // These functions is hidden from mkman in the USE_COUNTED_DECL macro; to have them documented,
    // we include them here:
#if 0
/**
 * Increments the use count of this <tt>APOINT</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
 * an entry on the bulletin board.
 */
    virtual void add();
    /**
     * Indicates if this <tt>APOINT</tt> is use-counted or not.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> for <tt>APOINTs</tt> unless overridden in a derived class.
     */
    virtual logical is_use_counted() const;
    /**
     * Decrements the use count of this <tt>APOINT</tt>.
     * <br><br>
     * <b>Role:</b> If the use count reaches 0, the <tt>APOINT</tt> is deleted. Before performing
     * the change, it checks if the data structure is posted on the bulletin board. If not,
     * the method calls <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param lose_if_zero
     * flag to lose if use count is zero.
     */
    virtual void remove(logical lose_if_zero = TRUE);
    /**
     * Sets the reference use count of this <tt>APOINT</tt>.
     * <br><br>
     * <b>Role:</b> This is used by the <tt>lose</tt> method. Refinements are not
     * destructed until the use count goes to zero.
     * <br><br>
     * @param val
     * new use count.
     */
    virtual void set_use_count(int val);
    /**
     * Returns the use count of this <tt>APOINT</tt>.
     */
    virtual int use_count() const;
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>APOINT</tt> from a SAT file.
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
 *    if (!get_standard_save_flag())
 *       read_int  use count data </pre>
 */
    void restore_common();
#endif

    // Data reading routines.

// STI jmb: Handle save/restore of use counted histories
//	int use_count() const { return use_count_data; }
// STI jmb: end
/**
 * Returns the coordinates of this <tt>APOINT</tt> as an <tt>SPAposition</tt>.
 */
    const SPAposition& coords() const { return coords_data; }

    // STI jmb: Handle save/restore of use counted histories
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

    // Set SPAposition into existing point.
/**
 * Sets the coordinates of this <tt>APOINT</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param pos
 * the new position.
 */
    void set_coords(const SPAposition& pos);
    // Transform a point
/**
 * Transforms this <tt>APOINT</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Routine used for system debugging
/**
 * @nodoc
 */
    int lookup(logical) const;
};
/** @} */
#endif
