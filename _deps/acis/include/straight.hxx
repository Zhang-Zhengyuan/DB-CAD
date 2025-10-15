/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file describing the modeller data structures
// A straight line, a STRAIGHT, is defined by a straight which
// in turn records a point on the line and a unit tangent SPAvector.
// The direction of the tangent is regarded as the inherent
// direction of the curve.
#ifndef STRAIGHT_CLASS
#define STRAIGHT_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "curve.hxx"
#include "strdef.hxx"
/**
* @file straight.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */
class SPAposition;
class SPAunit_vector;
class SPAtransf;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(STRAIGHT, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int STRAIGHT_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define STRAIGHT_LEVEL 2


// STRAIGHT declaration proper.
/**
 * Defines a straight line as an object in the model.
 * <br>
 * <b>Role:</b> <tt>STRAIGHT</tt> is a model geometry class that contains a pointer to a
 * (lowercase) <tt>straight</tt>, the corresponding construction geometry class. In general,
 * a model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
 * model object. It provides model management functionality, in addition to the
 * geometry definition.
 * <br><br>
 * <tt>STRAIGHT</tt> is one of several classes derived from <tt>CURVE</tt> to define a specific type of
 * curve. The straight class defines an infinite line by a point (<tt>SPAposition</tt>) on the
 * line and its direction (<tt>SPAunit_vector</tt>).
 * <br><br>
 * Along with the usual <tt>CURVE</tt> and <tt>ENTITY</tt> class methods, <tt>STRAIGHT</tt> has member methods to
 * provide access to specific implementations of the geometry. For example, methods are
 * available to set and retrieve the root point and direction of a line.
 * <br><br>
 * A use count allows references to multiple <tt>STRAIGHTs</tt>. The construction of a new <tt>STRAIGHT</tt>
 * initializes the use count to 0. Methods are provided to increment and decrement the use
 * count, and after the use count returns to 0, the entity is deleted.
 * @see straight
 */
class DECL_KERN STRAIGHT : public CURVE {

    // Record a STRAIGHT as a straight.

    straight def;

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
    ENTITY_FUNCTIONS(STRAIGHT, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    // Now the functions specific to STRAIGHT.

    // Make a bare STRAIGHT to be filled in later.
/**
 * Constructs a <tt>STRAIGHT</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    STRAIGHT();
    STRAIGHT(const char* gme);

    // Create a STRAIGHT line that passes through given SPAposition
    // and in direction of given unit SPAvector.
/**
 * Constructs a <tt>STRAIGHT</tt> that passes through the given position and has the given direction.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as argumenta. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param pos
 * position on the constructed STRAIGHT.
 * @param dir
 * direction of the constructed STRAIGHT.
 */
    STRAIGHT(const SPAposition& pos, const SPAunit_vector& dir);
    STRAIGHT(const char* gme, const SPAposition& pos, const SPAunit_vector& dir);

    // Create a STRAIGHT from a straight.
/**
 * Constructs a <tt>STRAIGHT</tt> from the specified straight.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param str
 * straight to be wrapped by the constructed STRAIGHT.
 */
    STRAIGHT(const straight& str);
    STRAIGHT(const char* gme, const straight& str);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>STRAIGHT_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>STRAIGHT_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>STRAIGHT_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"straight"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>STRAIGHT</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>STRAIGHT</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
    * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 30%><tt>straight::restore_data</tt></td>
 * <td>low-level geometry definition of the line.</td>
 * </tr>
 * </table>
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the position on this <tt>STRAIGHT</tt> used in its definition.
 */
    const SPAposition& root_point() const { return def.root_point; }
    /**
     * Returns the unit vector defining the direction of this <tt>STRAIGHT</tt>.
     */
    const SPAunit_vector& direction() const { return def.direction; }


    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>STRAIGHT</tt>'s root point to the given position.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param pos
 * the new root point.
 */
    void set_root_point(const SPAposition& pos);
    /**
     * Sets this <tt>STRAIGHT</tt>'s direction to the given unit vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param dir
     * the new direction.
     */
    void set_direction(const SPAunit_vector& dir);

    // Return the curve equation for reading only.
/**
 * Returns the <tt>curve</tt> equation of this <tt>STRAIGHT</tt>, for reading only.
 */
    const curve& equation() const;

    // Return the curve equation, checking first for backup.
/**
 * Returns the <tt>curve</tt> equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    curve& equation_for_update();

    // Get a new (lower-case) curve being the straight of the
    // STRAIGHT, transformed if the given SPAtransf is non-null and
    // reversed in sense if the logical is true.
/**
 * Transforms the curve equation of this <tt>STRAIGHT</tt>.
 * <br><br>
 * <b>Role:</b> If the logical <tt>negate</tt> is <tt>TRUE</tt>, the <tt>curve</tt> is reversed.
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

    // Transform the stored straight in place.
/**
 * Transforms this <tt>STRAIGHT</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL

    // The generic CURVE version of make_box is good enough
    // so the version for straight is left in abeyance.

    // SPAbox make_box( APOINT *, APOINT *, SPAtransf const * ) const;

    // STI swa 19Jul02 - New method for detection of clash with SPAbox.
/*
// tbrv
*/
/**
 * @nodoc
 */
    logical box_clash(SPAbox const& test_box,
        SPAtransf const& surf_transf = *(SPAtransf*)NULL_REF,
        double tol = SPAresabs) const;

    // lookup is done using CURVE::lookup()
    // int lookup( logical ) const;
public:
    straight gme_get_def();
};


/** @} */
#endif
