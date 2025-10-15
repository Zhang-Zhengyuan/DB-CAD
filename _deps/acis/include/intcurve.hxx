/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for INTCURVE.

// An INTCURVE, itself a derived class of CURVE, records an
// interpolated approximation to a general curve of intersection
// between two surfaces, as an intcurve (and a use-count within the
// parent CURVE record).  The intcurve record consists
// of a pointer to an int_cur and a logical denoting reversal of the
// int_cur.  The int-cur in turn contains a use-count, a 3d-spline
// termed a bs3_curve, a real for a fit tolerance, two pointers to
// the (lower-case) surfaces meeting in the intersection curve, and
// zero, one or two 2d-splines, termed bs2_splines, that record
// interpolated curves in SPAparameter space, depending on whether
// zero, one or both surfaces are parametric surfaces.

#ifndef INTCURVE_CLASS
#define INTCURVE_CLASS

/**
* @file intcurve.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */
#include "dcl_kern.h"
#include "logical.h"

#include "curve.hxx"

#include "intdef.hxx"

class SPAtransf;
class HELIX;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(INTCURVE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int INTCURVE_TYPE;


/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define INTCURVE_LEVEL 2


 // INTCURVE declaration proper.

 /**
  * 	Defines a parametric curve as an object in the model.
  * <br>
  * <b>Role:</b> <tt>INTCURVE</tt> is a model geometry class that contains a pointer to a (lowercase)
  * <tt>intcurve</tt>, the corresponding construction geometry class. In general, a model geometry class
  * is derived from <tt>ENTITY</tt> and is used to define a permanent model object. It provides model
  * management functionality, in addition to the geometry definition.
  * <br><br>
  * <tt>INTCURVE</tt> is one of several classes derived from <tt>CURVE</tt> to define a specific type of curve.
  * The <tt>intcurve</tt> record consists of a pointer to an <tt>int_cur</tt> and a logical denoting its sense.
  * <br><br>
  * An <tt>INTCURVE</tt> is the general representation of any curve that is not defined by an explicit
  * equation, but by reference to other geometric entities. This includes the intersection between
  * two surfaces, the projection of a curve onto a surface, an exact spline curve, or any other
  * general curve.
  * <br><br>
  * A use count allows multiple references to an <tt>INTCURVE</tt>. The construction of a new <tt>INTCURVE</tt>
  * initializes the use count to 0. Methods are provided to increment and decrement the use count,
  * and after the use count returns to 0, the entity is deleted.
  * @see intcurve
  */
class DECL_KERN INTCURVE : public CURVE {

    // Record an INTCURVE as an intcurve.

    intcurve def;

    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For the <tt>identical_comparator</tt> argument to be <tt>TRUE</tt>
     * requires an exact match when comparing doubles and returns the result of
     * <tt>memcmp</tt> as a default (for non-overridden subclasses). <tt>FALSE</tt>
     * indicates tolerant compares and returns <tt>FALSE</tt> as a default.
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
    friend class HELIX;

    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS(INTCURVE, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    // Now the functions specific to INTCURVE.

    // Make a bare INTCURVE to be filled in later.
/**
 * Constructs an <tt>INTCURVE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    INTCURVE();

    // Create an INTCURVE from an intcurve.
/**
 * Constructs an <tt>INTCURVE</tt> containing a specified <tt>intcurve</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param icrv
 * intcurve to be wrapped by the constructed INTCURVE.
 */
    INTCURVE(const intcurve& icrv);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier
 * <tt>INTCURVE_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>INTCURVE_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>INTCURVE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"intcurve"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>INTCURVE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>INTCURVE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    intcurve::restore_data      intcurve low-level geometry definition </pre>
 */
    void restore_common();
#endif

    // Data changing routine.
/**
 * Sets this <tt>INTCURVE's</tt> definition curve to the given intcurve.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param icrv
 * intcurve to which <tt>def</tt> is to be set.
 */
    void set_def(const intcurve& icrv);

    // Return the curve equation, for reading only.
/**
 * Returns the curve's equation, for reading only.
 */
    const curve& equation() const;

    // Return the curve equation, checking for backup first.
/**
 * Returns the curve's equation.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 */
    curve& equation_for_update();

    // Get a new (lower-case) curve being the intcurve of the
    // INTCURVE, transformed if the given SPAtransf is non-null and
    // reversed in sense if the logical is true.
/**
 * Transforms the curve's equation.
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

    // Transform the stored intcurve in place.
/**
 * Transforms the equation of this <tt>INTCURVE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Make a SPAbox enclosing a segment of this intersection curve
    // between two positions.
/**
 * Makes a bounding box for the portion of this <tt>INTCURVE</tt> delimited by two specified points, and transforms it.
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

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;// internal use only
    // STI ROLL

    // lookup is done using CURVE::lookup()
    // int lookup( logical ) const;
public:
    // get and set functions for access.
    intcurve gme_get_def();
    void gme_set_fit(int_cur* fit);
    void** gme_get_fit_ptr();
};

/** @} */
#endif
