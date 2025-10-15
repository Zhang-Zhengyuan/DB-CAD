/* ORIGINAL: acis2.1/kerndata/geom/torus.hxx */
/* $Id: torus.hxx,v 1.13 2002/08/09 17:15:18 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file describing the modeller data structures

// A TORUS is defined by a torus which is in turn given by centre
// point, a normal and two radii. The torus is a circular thickening
// of a circular spine, which has the given centre and normal, and
// whose radius is the "major radius" of the torus. The radius of
// the thickening circle is the "minor radius".

// For a normal "doughnut", the major radius is always positive, and
// is greater than the magnitude of the minor radius. For an "apple",
// which is a circular arc of more than 180 degrees swept round an
// axis through its ends, the major radius is less than the magnitude
// of the minor radius, but still positive. For a "lemon", which is a
// circular arc of less than 180 degrees swept around an axis through
// its ends, the major radius is negative and less in magnitude than
// the minor radius. The major radius may never be zero or less than
// minus the magnitude of the minor radius.

// In all cases, ths torus is "convex" (i.e. encloses a finite region
// of space) if the minor radius is positive, and "concave" (i.e.
// encloses all but a finite region) if the minor radius is negative.
// The minor radius may not be zero.


#ifndef TORUS_CLASS
#define TORUS_CLASS

#include "dcl_kern.h"
#include "surface.hxx"

#include "tordef.hxx"

/**
* @file torus.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */

class LOOP;
class SPAposition;
class SPAtransf;
class SPAbox;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(TORUS, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int TORUS_TYPE;


/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define TORUS_LEVEL 2


 // TORUS declaration proper.
 /**
  * Defines a torus as an object in the model.
  * <br>
  * <b>Role:</b> <tt>TORUS</tt> is a model geometry class that contains a pointer
  * to a (lowercase) torus, the corresponding construction geometry class. In
  * general, a model geometry class is derived from <tt>ENTITY</tt> and is used to
  * define a permanent model object. It provides model management functionality,
  * in addition to the geometry definition.
  * <br><br>
  * <tt>TORUS</tt> is one of several classes derived from <tt>SURFACE</tt> to define
  * a specific type of surface. The <tt>torus</tt> class defines a torus as a circular
  * thickening of a circular spine, defined by a center, normal, major radius, and
  * minor radius.
  * <br><br>
  * Along with the usual <tt>SURFACE</tt> and <tt>ENTITY</tt> class methods,
  * <tt>TORUS</tt> has member methods to provide access to specific implementations
  * of the geometry. For example, methods are available to set and retrieve the
  * radius, center, and other information about a torus.
  * <br><br>
  * A use count allows multiple references to a <tt>TORUS</tt>. The construction of
  * a new <tt>TORUS</tt> initializes the use count to 0. Methods are provided to
  * increment and decrement the use count, and after the use count returns to 0,
  * the entity is deleted.
  * @see torus
  */
class DECL_KERN TORUS : public SURFACE {

    // Record a TORUS as a torus.

    torus def;


    // STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    /**
     * Virtual function for comparing subclass data - called by <tt>bulletin_no_change</tt>.
     * <br><br>
     * <b>Role:</b> For identical_comparator to be <tt>TRUE</tt> requires an exact match
     * when comparing doubles, and returns the result of memcmp as a default (for
     * non-overridden subclasses). <tt>FALSE</tt> indicates tolerant compares and returns
     * <tt>FALSE</tt> as a default.
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
    ENTITY_FUNCTIONS(TORUS, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


    // Now the functions specific to TORUS.

    // Make a bare TORUS to be filled in later.
/**
 * Constructs a <tt>TORUS</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    TORUS();
    TORUS(const char* gme);

    // Create a TORUS centred on a given SPAposition and with
    // given normal and radii.
/**
 * Constructs a <tt>TORUS</tt> with specified center, normal, major radius, and minor radius.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as arguments. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param center
 * center of the constructed TORUS.
 * @param normal
 * normal to the constructed TORUS.
 * @param major_radius
 * major radius of the constructed TORUS.
 * @param minor_radius
 * minor radius of the constructed TORUS.
 */
    TORUS(const SPAposition& center, const SPAunit_vector& normal, double major_radius, double minor_radius);
    TORUS(const char* gme, const SPAposition& center, const SPAunit_vector& normal, double major_radius, double minor_radius);

    // Create a TORUS from a torus.
/**
 * Constructs a <tt>TORUS</tt> from the specified torus.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param tor
 * torus to be wrapped by the constructed TORUS.
 */
    TORUS(const torus& tor);
    TORUS(const char* gme, const torus& tor);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier
 * <tt>TORUS_TYPE</tt>. If <tt>level</tt> is specified, returns <tt>TORUS_TYPE</tt>
 * for that level of derivation from <tt>ENTITY</tt>. The level of this class is
 * defined as <tt>TORUS_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string "torus".
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>TORUS</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>TORUS</tt> from a <tt>SAT</tt> file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a <tt>SAT</tt> file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the <tt>SAT</tt> file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the <tt>SAT</tt> file.
 * <pre>
 *    torus::restore_data      low-level geometry definition of the torus </pre>
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the center position of this <tt>TORUS</tt>.
 */
    const SPAposition& centre() const { return def.centre; }
    /**
     * Returns the normal vector of this <tt>TORUS</tt>.
     */
    const SPAunit_vector& normal() const { return def.normal; }
    /**
     * Returns the major radius of this <tt>TORUS</tt>.
     */
    double major_radius() const { return def.major_radius; }
    /**
     * Returns the minor radius of this <tt>TORUS</tt>.
     */
    double minor_radius() const { return def.minor_radius; }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>TORUS's</tt> center point to the given position.
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
     * Sets this <tt>TORUS's</tt> normal to the given unit vector.
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
     * Sets this <tt>TORUS's</tt> major radius to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param major_radius
     * the new major radius.
     */
    void set_major_radius(double major_radius);
    /**
     * Sets this <tt>TORUS's</tt> minor radius to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param minor_radius
     * the new minor radius.
     */
    void set_minor_radius(double minor_radius);

    // Return the surface equation for reading only.
/**
 * Returns the surface equation of this <tt>TORUS</tt>, for reading only.
 */
    const surface& equation() const;

    // Return the surface equation, checking first for backup.
/**
 * Returns the surface equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    surface& equation_for_update();

    // Get a new (lower-case) surface being the torus of the TORUS,
    // transformed if the given SPAtransf is non-null and reversed
    // in sense if the logical is true.
/**
 * Returns the transformed surface equation of this <tt>TORUS</tt>.
 * <br><br>
 * <b>Role:</b> If the logical <tt>negate</tt> is <tt>TRUE</tt>, the surface is reversed.
 * <br><br>
 * @param t
 * transform to apply.
 * @param negate
 * flag to reverse the surface.
 */
    surface* trans_surface(
        const SPAtransf& t = *(SPAtransf*)NULL_REF,
        logical negate = FALSE
    ) const;
    // Transform the stored torus in place.
/**
 * Transforms this <tt>TORUS</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Construct a SPAbox containing a face on the TORUS bounded by the
    // given LOOP list.

    // wseibold (Feb`02) added argument to compute tighter bounding boxes
/**
 * Returns a bounding box for this <tt>TORUS</tt>.
 * <br><br>
 * <b>Role:</b> Returns a <tt>SPAbox</tt> bounding the complete surface.  If
 * tight_box is TRUE, then a tighter box bounding the supplied <tt>LOOP</tt>
 * list is computed.  If untransformed_box is supplied, it will be expanded
 * to include the untransformed box just computed.
 * <br><br>
 * @param loop
 * list of <tt>LOOP<tt>s on the surface
 * @param t
 * transform to apply to the box
 * @param tight_box
 * flag to compute a tighter box
 * @param untransformed_box
 * accumulating box
 */
    SPAbox make_box(LOOP* loop = NULL, // STL amt 26Jun03: default value added
        const SPAtransf* t = NULL,
        logical tight_box = FALSE,
        SPAbox* untransformed_box = NULL) const;

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

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL

    // lookup is done using SURFACE::lookup()
    // int lookup( logical ) const;
public:
    torus gme_get_def();
};



/** @} */
#endif
