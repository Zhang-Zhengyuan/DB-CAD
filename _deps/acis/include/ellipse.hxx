/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for ELLIPSE.
// An ELLIPSE is defined by an ellipse which in turn is given by
// a centre, a unit SPAvector for the direction of the normal to the
// plane of the ellipse, a SPAvector giving size and direction of
// the major axis, and a real giving the eccentricity of
// the ellipse i.e. length minor-axis/length major-axis (defaults to
// 1.0 for a circle).  The inherent direction
// of the ellipse is given by the normal (using right-hand screw rule).
#ifndef ELLIPSE_CLASS
#define ELLIPSE_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "curve.hxx"
#include "elldef.hxx"
/**
 * @file ellipse.hxx
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
ENTITY_IS_PROTOTYPE(ELLIPSE, KERN)
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
extern DECL_KERN int ELLIPSE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define ELLIPSE_LEVEL 2


// ELLIPSE declaration proper.
/**
 * Defines an ellipse as an object in the model.
 * <br>
 * <b>Role:</b> <tt>ELLIPSE</tt> is a model geometry class that contains a pointer to a
 * (lowercase) ellipse, the corresponding construction geometry class. In general,
 * a model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
 * model object. It provides model management functionality, in addition to the
 * geometry definition.
 * <br><br>
 * <tt>ELLIPSE</tt> is one of several classes derived from <tt>CURVE</tt> to define a specific
 * type of curve. The ellipse class defines an ellipse by its center, a unit normal
 * vector, a major-axis vector, and a double specifying the eccentricity ratio of the ellipse.
 * <br><br>
 * Along with the usual <tt>CURVE</tt> and <tt>ENTITY</tt> class methods, <tt>ELLIPSE</tt> has member methods
 * to provide access to specific implementations of the geometry. For example, methods are
 * available to set and retrieve the axes, center, and other information about an ellipse.
 * <br><br>
 * A use count allows multiple references to an <tt>ELLIPSE</tt>. The construction of a new
 * <tt>ELLIPSE</tt> initializes the use count to 0. Methods are provided to increment and decrement
 * the use count, and after the use count returns to 0, the entity is deleted.
 * @see ellipse
 */

class DECL_KERN ELLIPSE : public CURVE {

    // Record an ELLIPSE as an ellipse.

    ellipse def;


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
    ENTITY_FUNCTIONS(ELLIPSE, KERN)
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

    // Now the functions specific to ELLIPSE.

    // Make a bare ELLIPSE to be filled in later.
/**
 * Constructs an <tt>ELLIPSE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    ELLIPSE();
    ELLIPSE(const char* gme);
    // Create an ELLIPSE from given centre, normal, major axis (SPAvector
    // gives its size and direction), and radius ratio.
/**
 * Constructs an <tt>ELLIPSE</tt> with specified center, normal, major axis, and radius ratio.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as argumenta. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param center
 * center of the constructed ELLIPSE.
 * @param normal
 * normal to the constructed ELLIPSE.
 * @param major_axis
 * major axis of the constructed ELLIPSE.
 * @param radius_ratio
 * ratio of the radii of the constructed ELLIPSE.
 */
    ELLIPSE(
        const SPAposition& center,
        const SPAunit_vector& normal,
        const SPAvector& major_axis,
        double radius_ratio = 1.0
    );
    ELLIPSE(
        const char* gme,
        const SPAposition& center,
        const SPAunit_vector& normal,
        const SPAvector& major_axis,
        double radius_ratio = 1.0
    );
    // Create an ELLIPSE from an ellipse.
/**
 * Constructs an <tt>ELLIPSE</tt> from the specified ellipse.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param ell
 * ellipse to be wrapped by the constructed ELLIPSE.
 */
    ELLIPSE(const ellipse& ell);
    ELLIPSE(const char* gme, const ellipse& ell);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>ELLIPSE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>ELLIPSE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>ELLIPSE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"ellipse"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>ELLIPSE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>ELLIPSE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 * <tt>ellipse::restore_data</tt>&nbsp;&nbsp;&nbsp;&nbsp;low-level geometry definition of the ellipse.
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the center position of this <tt>ELLIPSE</tt>.
 */
    const SPAposition& centre() const { return def.centre; }
    /**
     * Returns the normal to the plane of this <tt>ELLIPSE</tt>.
     */
    const SPAunit_vector& normal() const { return def.normal; }
    /**
     * Returns the major axis of this <tt>ELLIPSE</tt>.
     */
    const SPAvector& major_axis() const { return def.major_axis; }
    /**
     * Returns the radius ratio of this <tt>ELLIPSE</tt>.
     */
    double radius_ratio() const { return def.radius_ratio; }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>ELLIPSE</tt>'s center point to the given position.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param center
 * the new center.
 */
    void set_centre(const SPAposition& center);
    /**
     * Sets this <tt>ELLIPSE</tt>'s planar normal to the given unit vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param normal
     * the new normal.
     */
    void set_normal(const SPAunit_vector& normal);
    /**
     * Sets this <tt>ELLIPSE</tt>'s major axis to the given vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param major_axis
     * the new major axis.
     */
    void set_major_axis(const SPAvector& major_axis);
    /**
     * Sets this <tt>ELLIPSE</tt>'s radius ratio to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param ratio
     * the new radius ratio.
     */
    void set_radius_ratio(double ratio);

    // Return the curve equation for reading only.
/**
 * Returns the curve's equation of this <tt>ELLIPSE</tt>, for reading only.
 */
    const curve& equation() const;
    // Return the curve equation, checking first for backup.
/**
 * Returns the curve equation for update operations, backing it up first.
 */
    curve& equation_for_update();
    // Get a new (lower-case) curve being the ellipse of the
    // ELLIPSE, transformed if the given SPAtransf is non-null and
    // reversed in sense if the logical is true.
/**
 * Transforms the curve equation of this <tt>ELLIPSE</tt>.
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
    // Transform the stored ellipse in place.
/**
 * Transforms this <tt>ELLIPSE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Make a SPAbox enclosing a segment of this ellipse between
    // two positions.
/**
 * Makes a bounding box.
 * <br><br>
 * <b>Role:</b> Makes a bounding box for the portion of this <tt>ELLIPSE</tt>
 * delimited by two specified points that lie within its plane, and transforms it.
 * The box has sides parallel to the principal axes of the <tt>ELLIPSE</tt>.
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
    ellipse gme_get_def();
};

/** @} */
#endif
