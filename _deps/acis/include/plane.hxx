/* ORIGINAL: acis2.1/kerndata/geom/plane.hxx */
/* $Id: plane.hxx,v 1.12 2002/08/09 17:15:17 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file describing the modeller data structures

// A PLANE is defined by a plane which is in turn given by point
// on the plane and a unit normal.  The direction of the normal is
// regarded as the inherent direction of the surface.


#ifndef PLANE_CLASS
#define PLANE_CLASS

#include "dcl_kern.h"
#include "surface.hxx"

#include "pladef.hxx"

/**
* @file plane.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */

class LOOP;
class SPAposition;
class SPAunit_vector;
class SPAtransf;
class SPAbox;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PLANE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int PLANE_TYPE;

/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define PLANE_LEVEL 2


 // PLANE declaration proper.
 /**
  * Defines a plane as an object in the model.
  * <br>
  * <b>Role:</b> <tt>PLANE</tt> is a model geometry class that contains a pointer to a (lowercase)
  * <tt>plane</tt>, the corresponding construction geometry class. In general, a model geometry
  * class is derived from <tt>ENTITY</tt> and is used to define a permanent model object. It
  * provides model management functionality, in addition to the geometry definition.
  * <br><br>
  * <tt>PLANE</tt> is one of several classes derived from <tt>SURFACE</tt> to define a specific type of
  * surface. The <tt>plane</tt> class defines a plane by a point on the plane and its unit normal.
  * <br><br>
  * Along with the usual <tt>SURFACE</tt> and <tt>ENTITY</tt> class methods, <tt>PLANE</tt> has member methods to
  * provide access to specific implementations of the geometry. For example, methods are
  * available to set and retrieve the root point and normal of a plane.
  * <br><br>
  * A use count allows multiple references to a <tt>PLANE</tt>. The construction of a new <tt>PLANE</tt>
  * initializes the use count to 0. Methods are provided to increment and decrement the
  * use count, and after the use count returns to 0, the entity is deleted.
  * @see plane
  */
class DECL_KERN PLANE : public SURFACE {

    // Record a PLANE as a plane.

    plane def;

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
    ENTITY_FUNCTIONS(PLANE, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    // Now the functions specific to PLANE.

    // Make a bare PLANE to be filled in later.
/**
 * Constructs a <tt>PLANE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    PLANE();
    PLANE(const char* gme);
    // Create a PLANE that passes through given SPAposition and with
    // given unit SPAvector as normal.
/**
 * Constructs a <tt>PLANE</tt> that passes through the given position with the given unit-vector normal.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as argumenta. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param pos
 * position on the constructed PLANE.
 * @param normal
 * normal to the constructed PLANE.
 */
    PLANE(const SPAposition& pos, const SPAunit_vector& normal);
    PLANE(const char* gme, const SPAposition& pos, const SPAunit_vector& normal);
    // Create a PLANE from a plane.
/**
 * Constructs a <tt>PLANE</tt> from the specified plane.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param p
 * plane to be wrapped by the constructed PLANE.
 */
    PLANE(const plane& p);
    PLANE(const char* gme, const plane& p);
    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>PLANE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>PLANE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>PLANE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"plane"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>PLANE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>PLANE</tt> from a SAT file.
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
 * <td width = 30%><tt>plane::restore_data</tt></td>
 * <td>low-level geometry definition of the plane.</td>
 * </tr>
 * </table>
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the point defining this <tt>PLANE</tt>.
 */
    const SPAposition& root_point() const { return def.root_point; }

    /**
     * Returns the normal defining this <tt>PLANE</tt>.
     */
    const SPAunit_vector& normal() const { return def.normal; }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>PLANE</tt>'s root point to the given position.
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
     * Sets this <tt>PLANE</tt>'s normal to the given unit vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param normal
     * the new normal.
     */
    void set_normal(const SPAunit_vector& normal);

    // Return the surface equation for reading only.
/**
 * Returns the surface equation of this <tt>PLANE</tt>.
 */
    surface const& equation() const;
    // Return the surface equation, checking first for backup.
/**
 * Returns the <tt>surface</tt> equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    surface& equation_for_update();
    // Get a new (lower-case) surface being the plane of the PLANE,
    // transformed if the given SPAtransf is non-null and reversed
    // in sense if the logical is true.
/**
 * Returns the transformed <tt>surface</tt> equation of this <tt>PLANE</tt>.
 * <br><br>
 * <b>Role:</b> If the logical <tt>negate</tt> is <tt>TRUE</tt>, the <tt>surface</tt> is reversed.
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
    // Transform the stored plane in place.
/**
 * Transforms this <tt>PLANE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);
    // The generic SURFACE version of make_box is sufficient
    // so version for plane is left in abeyance.

    //	SPAbox make_box( LOOP * ) const;

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

    // lookup is done using SURFACE::lookup()
    // int lookup( logical ) const;

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL
public:
    plane gme_get_def();
};



/** @} */
#endif
