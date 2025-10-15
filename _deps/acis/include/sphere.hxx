/* ORIGINAL: acis2.1/kerndata/geom/sphere.hxx */
/* $Id: sphere.hxx,v 1.13 2002/08/09 17:15:17 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file describing the modeller data structures

// A SPHERE is defined by a sphere which is in turn given by centre
// point and a radius.  The sign of the radius determines the direction
// of the surface - positive for a conves sphere, negative for a
// concave one.


#ifndef SPHERE_CLASS
#define SPHERE_CLASS

#include "dcl_kern.h"
#include "surface.hxx"

#include "sphdef.hxx"

/**
* @file sphere.hxx
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
ENTITY_IS_PROTOTYPE(SPHERE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int SPHERE_TYPE;

/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define SPHERE_LEVEL 2


 // SPHERE declaration proper.
 /**
  * Defines a sphere as an object in the model.
  * <br>
  * <b>Role:</b> <tt>SPHERE</tt> is a model geometry class that contains a pointer to a
  * (lowercase) <tt>sphere</tt>, the corresponding construction geometry class. In general, a
  * model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
  * model object. It provides model management functionality, in addition to the
  * geometry definition.
  * <br><br>
  * <tt>SPHERE</tt> is one of several classes derived from <tt>SURFACE</tt> to define a specific type of
  * surface. The sphere class defines a sphere by its center point and radius.
  * <br><br>
  * Along with the usual <tt>SURFACE</tt> and <tt>ENTITY</tt> class methods, <tt>SPHERE</tt> has member methods to
  * provide access to specific implementations of the geometry. For example, methods are
  * available to set and retrieve the center and radius of a sphere.
  * <br><br>
  * A use count allows multiple references to a <tt>SPHERE</tt>. The construction of a new <tt>SPHERE</tt>
  * initializes the use count to 0. Methods are provided to increment and decrement the use
  * count, and after the use count returns to 0, the entity is deleted.
  * @see sphere
  */
class DECL_KERN SPHERE : public SURFACE {

    // Record a SPHERE as a sphere.

    sphere def;

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
    ENTITY_FUNCTIONS(SPHERE, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    // Now the functions specific to SPHERE.

    // Make a bare SPHERE to be filled in later.
/**
 * Constructs a <tt>SPHERE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    SPHERE();
    SPHERE(const char* gme);

    // Create a SPHERE centred on a given SPAposition and with a
    // given radius.
/**
 * Constructs a <tt>SPHERE</tt> having the given center and radius.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as arguments. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param center
 * center position of the constructed SPHERE.
 * @param radius
 * radius of the constructed SPHERE.
 */
    SPHERE(const SPAposition& center, double radius);
    SPHERE(const char* gme, const SPAposition& center, double radius);
    // Create a SPHERE from a sphere.
/**
 * Constructs a <tt>SPHERE</tt> from the specified sphere.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param s
 * sphere to be wrapped by the constructed SPHERE.
 */
    SPHERE(const sphere& s);
    SPHERE(const char* gme, const sphere& s);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>SPHERE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>SPHERE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>SPHERE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"sphere"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>SPHERE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>SPHERE</tt> from a SAT file.
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
 * <td width = 30%><tt>sphere::restore_data</tt></td>
 * <td>low-level geometry definition of the sphere.</td>
 * </tr>
 * </table>
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the center of this <tt>SPHERE</tt>.
 */
    const SPAposition& centre() const { return def.centre; }
    /**
     * Returns the radius of this <tt>SPHERE</tt>.
     */
    double radius() const { return def.radius; }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>SPHERE</tt>'s center to the given position.
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
     * Sets this <tt>SPHERE</tt>'s radius to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param radius
     * the new radius.
     */
    void set_radius(double radius);

    // Return the surface equation for reading only.
/**
 * Returns the <tt>surface</tt> equation of this <tt>SPHERE</tt>.
 */
    const surface& equation() const;

    // Return the surface equation, checking first for backup.
/**
 * Returns the <tt>surface</tt> equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    surface& equation_for_update();

    // Get a new (lower-case) surface being the sphere of the SPHERE,
    // transformed if the given SPAtransf is non-null and reversed
    // in sense if the logical is true.
/**
 * Returns the transformed <tt>surface</tt> equation of this <tt>SPHERE</tt>.
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

    // Transform the stored sphere in place.
/**
 * Transforms this <tt>SPHERE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Construct a SPAbox containing a face on the SPHERE bounded by the
    // given LOOP list.

    //wseibold (Feb`02) added extra argument to create tighter face bounding boxes
/**
 * Returns a bounding box for this <tt>SPHERE</tt>.
 * <br><br>
 * <b>Role:</b> Returns a <tt>SPAbox</tt> bounding the complete surface.  If
 * tight_box is TRUE, then a tighter box bounding the supplied <tt>LOOP</tt> list
 * is computed.  If untransformed_box is supplied, it will be expanded to include
 * the untransformed box just computed.
 * <br><br>
 * @param loop
 * list of <tt>LOOP</tt>s on the surface
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
    sphere gme_get_def();
};

/** @} */
#endif
