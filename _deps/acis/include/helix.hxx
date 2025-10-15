/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for HELIX.
// A HELIX is defined by a helix which in turn is given by
// a point on the axis (its root point), axis direction, displacement vector 
// from root point to a point on the curve,
// pitch, taper, handedness, parameter scaling and parameter range.
/*******************************************************************/
#ifndef HELIX_CLASS
#define HELIX_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "curve.hxx"
#include "heldef.hxx"
/**
* @file helix.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */

class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HELIX, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int HELIX_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define HELIX_LEVEL 2


// HELIX declaration proper.
/**
 * Defines a helix as an object in the model.
 * <br>
 * <b>Role:</b> <tt>HELIX</tt> is a model geometry class that contains a pointer to a
 * (lowercase) helix, the corresponding construction geometry class. In general,
 * a model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
 * model object. It provides model management functionality, in addition to the
 * geometry definition.
 * <br><br>
 * <tt>HELIX</tt> is one of several classes derived from <tt>CURVE</tt> to define a specific
 * type of curve. The helix class defines a helix by its root point (a point on the axis),
 * its axis direction, displacement vector from root point to a point on the curve,
 * pitch, taper, handedness, parameter scaling, and parameter range.
 * <br><br>
 * Along with the usual <tt>CURVE</tt> and <tt>ENTITY</tt> class methods, <tt>HELIX</tt> has member methods
 * to provide access to specific implementations of the geometry. For example, methods are
 * available to set and retrieve the axis, root point, and other information about a helix.
 * <br><br>
 * A use count allows multiple references to a <tt>HELIX</tt>. The construction of a new
 * <tt>HELIX</tt> initializes the use count to 0. Methods are provided to increment and decrement
 * the use count, and after the use count returns to 0, the entity is deleted.
 * @see helix
 */
class DECL_KERN HELIX : public CURVE {

    // Record a HELIX as a helix.

    helix def;


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
    ENTITY_FUNCTIONS(HELIX, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    /**
     * @nodoc
     */
    FULLSIZE_FUNCTION
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif

    // Now the functions specific to HELIX.

    // Make a bare HELIX to be filled in later.
/**
 * Constructs a <tt>HELIX</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    HELIX();
    HELIX(const char* gme);

    // Create a helix from given axis root, direction, start displacement, pitch,
    // handedness, parameter scaling and taper.
/**
 * Constructs a <tt>HELIX</tt> with specified root point (on the axis), axis direction,
 * displacement vector from root point to a point on the helix,
 * thread distance, handedness, parameter range, parameter scaling and taper.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as arguments. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param axis_root
 * axis root position.
 * @param axis_dir
 * axis direction.
 * @param start_disp
 * displacement to start point.
 * @param pitch
 * pitch (distance between turns along axis direction).
 * @param handedness
 * handedness (TRUE is right, FALSE is left).
 * @param helix_range
 * maximum parameter range (must be bounded).
 * @param par_scaling
 * parameter scaling (one turn corresponds to parameter distance of <tt> 2*pi*par_scaling </tt>).
 * @param taper
 * taper (radial distance between turns).
 */
    HELIX(
        SPAposition const& axis_root,
        SPAunit_vector const& axis_dir,
        SPAvector const& start_disp,
        double                 pitch,
        logical                handedness,
        SPAinterval const& helix_range,
        double                 par_scaling = 1.0,
        double                 taper = 0.0
    );
    HELIX(
        const char* gme,
        SPAposition const& axis_root,
        SPAunit_vector const& axis_dir,
        SPAvector const& start_disp,
        double                 pitch,
        logical                handedness,
        SPAinterval const& helix_range,
        double                 par_scaling = 1.0,
        double                 taper = 0.0
    );

    // Create a HELIX from a helix.
/**
 * Constructs a <tt>HELIX</tt> from the specified helix.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param hel
 * helix to be wrapped by the constructed HELIX.
 */
    HELIX(const helix& hel);
    HELIX(const char* gme, const helix& hel);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>HELIX_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>HELIX_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>HELIX_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"helix"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>HELIX</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>HELIX</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 * <tt>helix::restore_data</tt>&nbsp;&nbsp;&nbsp;&nbsp;low-level geometry definition of the helix.
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the axis root position of this <tt>HELIX</tt>.
 */
    const SPAposition& axis_root() const { return def.axis_root(); }
    /**
     * Returns the axis direction of this <tt>HELIX</tt>.
     */
    const SPAunit_vector& axis_dir() const { return def.axis_dir(); }
    /**
     * Returns the start displacement of this <tt>HELIX</tt>.
     */
    const SPAvector& start_disp() const { return def.start_disp(); }
    /**
     * Returns the pitch of this <tt>HELIX</tt>.
     */
    double pitch() const { return def.pitch(); }
    /**
     * Returns the handedness of this <tt>HELIX</tt>.
     */
    logical handedness() const { return def.handedness(); }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>HELIX</tt>'s axis root position to the given position.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param axis_root
 * the new root position.
 */
    void set_axis_root(const SPAposition& axis_root);
    /**
     * Sets this <tt>HELIX</tt>'s axis direction to the given unit vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param axis_dir
     * the new axis direction.
     */
    void set_axis_dir(const SPAunit_vector& axis_dir);
    /**
     * Sets this <tt>HELIX</tt>'s start displacement to the given vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param start_disp
     * the new start displacement.
     */
    void set_start_disp(const SPAvector& start_disp);
    /**
     * Sets this <tt>HELIX</tt>'s pitch to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param pitch
     * the new pitch.
     */
    void set_pitch(double pitch);
    /**
     * Sets this <tt>HELIX</tt>'s handedness to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param handedness
     * the new handedness.
     */
    void set_handedness(logical handedness);

    // Return the curve equation for reading only.
/**
 * Returns the curve's equation of this <tt>HELIX</tt>, for reading only.
 */
    const curve& equation() const;

    // Sets the maximum range.

    void set_helix_range(SPAinterval const& helix_range);
    // Return the curve equation, checking first for backup.
/**
 * Returns the curve equation for update operations, backing it up first.
 */
    curve& equation_for_update();

    // Get a new (lower-case) curve being the helix of the
    // HELIX, transformed if the given SPAtransf is non-null and
    // reversed in sense if the logical is true.
/**
 * Transforms the curve equation of this <tt>HELIX</tt>.
 * <br><br>
 * <b>Role:</b> If the logical <tt>negate</tt> is <tt>TRUE</tt>, the curve is reversed.
 * <br><br>
 * @param t
 * transform to apply.
 * @param negate
 * flag to reverse the curve.
 */
    curve* trans_curve(
        const SPAtransf& t = *(SPAtransf*)NULL_REF,
        logical negate = FALSE
    ) const;

    // Transform the stored helix in place.
/**
 * Transforms this <tt>HELIX</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Make a SPAbox enclosing a segment of this helix between
    // two positions.
/**
 * Makes a bounding box.
 * <br><br>
 * <b>Role:</b> Makes a bounding box for the portion of this <tt>HELIX</tt>
 * delimited by two specified points that lie on it, and transforms it.
 * The box has sides parallel to the principal axes of the <tt>HELIX</tt>.
 * <br><br>
 * @param start
 * first point on the curve.
 * @param end
 * second point on the curve.
 * @param t
 * transform.
 * @param tol
 * tolerance.
 */
    SPAbox make_box(APOINT* start, APOINT* end, const SPAtransf* t, double tol = 0.0) const;

    // STI swa 19Jul02 - New method for detection of clash with SPAbox.
/*
// tbrv
*/
/**
 * @nodoc
 */
    logical box_clash(const SPAbox& test_box,
        const SPAtransf& surf_transf = *(SPAtransf*)NULL_REF,
        double tol = SPAresabs) const;

    // lookup is done using CURVE::lookup()
    // int lookup( logical ) const;
public:
    helix gme_get_def();
};

/** @} */
#endif
