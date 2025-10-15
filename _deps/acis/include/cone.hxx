/* ORIGINAL: acis2.1/kerndata/geom/cone.hxx */
/* $Id: cone.hxx,v 1.13 2002/08/09 17:15:17 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file describing the cone geometry entity.
//
// This describes a right elliptical cone, by specifying a
// cross-sectional ellipse, in the same form as for the ellipse curve,
// together with the sine and cosine of the major half-angle (i.e.
// the half-angle measured in the plane of the major axis of the ellipse).
//
// The CONE is defined by a cone which is given by a right elliptical
// cone recorded as a cross-sectional ellipse, in the same form as for
// the ellipse curve, together with the sine and cosine of the major
// half-angle (i.e. the half-angle measured in the plane of the major
// axis of the ellipse).
//
// The signs of the trigonometric functions disclose which way the
// cone is enlarging, and whether the surface is convex or concave. The
// rules are:
//	if cosine_angle is +ve, the surface direction is outwards,
//		away from the axis, so the surface is convex. Conversely
//		if cosine_angle is negative.
//	if sine_angle and cosine_angle have the same sign, then
//		the cone increases in cross-section in the direction of
//		the axis (the normal to the base ellipse), and conversely
//		if they have opposite sign.
// If sine_angle is 0, the "cone" is a cylinder, and the direction
// of increasing cross-section is meaningless. If cosine_angle is
// zero, the "cone" is planar, and neither convex nor concave. The
// outward normal is then simply minus the base normal multiplied by
// sine_angle.
//
// The surface stops at the apex (if any) - i.e. this surface type
// does not represent a double cone.


#ifndef CONE_CLASS
#define CONE_CLASS

#include "dcl_kern.h"
#include "surface.hxx"

#include "condef.hxx"

/**
* @file cone.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */

class LOOP;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class SPAbox;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(CONE, KERN)
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
extern DECL_KERN int CONE_TYPE;


/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define CONE_LEVEL 2


 // CONE declaration proper.
 /**
  * Defines a cone as an object in the model.
  * <br>
  * <b>Role:</b> <tt>CONE</tt> is a model geometry class that contains a pointer to a (lowercase) <tt>cone</tt>,
  * the corresponding construction geometry class. In general, a model geometry class is derived
  * from <tt>ENTITY</tt> and is used to define a permanent model object. It provides model management
  * functionality, in addition to the geometry definition.
  * <br><br>
  * <tt>CONE</tt> is one of several classes derived from <tt>SURFACE</tt> to define a specific type of surface.
  * The <tt>cone</tt> class defines a cone by the base ellipse and the sine and cosine of the major half-angle.
  * <br><br>
  * Along with the usual <tt>SURFACE</tt> and <tt>ENTITY</tt> class methods, <tt>CONE</tt> has member methods to provide
  * access to specific implementations of the geometry. For example, methods are available to set
  * and retrieve the axes, center, and other information about a cone.
  * <br><br>
  * A use count allows multiple references to a <tt>CONE</tt>. The construction of a new <tt>CONE</tt>
  * initializes the use count to 0. Methods are provided to increment and decrement the use count,
  * and after the use count returns to 0, the entity is deleted.
  * @see cone
  */
class DECL_KERN CONE : public SURFACE {

    // Record a CONE as a cone.

    cone def;

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
    ENTITY_FUNCTIONS(CONE, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    // Now the functions specific to CONE.

    // Make a CONE to be filled in later.
/**
 * Constructs a <tt>CONE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    CONE();
    CONE(const char* gme);
    // Make a cone with axis through given SPAposition and along given
    // unit SPAvector.  The SPAvector specifies the radius of the cone in
    // the plane normal to its axis and through the axis SPAposition.
    // The remaining reals are for the radius ratio of its
    // cross-sectional ellipse, and the sine and cosine of the cone
    // half-angle.
/**
 * Constructs a <tt>CONE</tt> with specified center, normal, major axis, radius ratio, and half-angle.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as argumenta. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param center
 * center of the constructed CONE.
 * @param normal
 * normal (axis) of the constructed CONE.
 * @param major_axis
 * major axis of the constructed CONE's base.
 * @param radius_ratio
 * radius ratio of the constructed CONE.
 * @param sine_angle
 * sine of the constructed CONE's half-angle.
 * @param cos_angle
 * cosine of the constructed CONE's half-angle.
 */
    CONE(
        const SPAposition& center,
        const SPAunit_vector& normal,
        const SPAvector& major_axis,
        double radius_ratio = 1,		// default to circular cone
        double sine_angle = 0,		// default to cylinder
        double cos_angle = 1
    );
    CONE(const char* gme,
        const SPAposition& center,
        const SPAunit_vector& normal,
        const SPAvector& major_axis,
        double radius_ratio = 1,		// default to circular cone
        double sine_angle = 0,		// default to cylinder
        double cos_angle = 1
    );

    // Make a CONE from a cone.
/**
 * Constructs a <tt>CONE</tt> from the specified cone.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param c
 * cone to be wrapped by the constructed CONE.
 */
    CONE(const cone& c);
    CONE(const char* gme, const cone& c);
    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for <tt>this</tt> object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>CONE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>CONE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>CONE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"cone"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>CONE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>CONE</tt> from a SAT file.
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
 * <td width = 30%><tt>cone::restore_data</tt></td>
 * <td>low-level geometry definition of the cone.</td>
 * </tr>
 * </table>
 */
    void restore_common();
#endif

    // Data reading routines.

/**
 * Returns the center of the ellipse defining this <tt>CONE</tt>.
 */
    const SPAposition& root_point() const { return def.base.centre; }
    /**
     * Returns the normal to the plane of the ellipse defining this <tt>CONE</tt>, i.e., the cone axis.
     */
    const SPAunit_vector& direction() const { return def.base.normal; }
    /**
     * Returns the major axis of the ellipse defining this <tt>CONE</tt>.
     */
    const SPAvector& major_axis() const { return def.base.major_axis; }
    /**
     * Returns the ratio of the minor-axis length to the major-axis length of the ellipse defining this <tt>CONE</tt>.
     */
    double radius_ratio() const { return def.base.radius_ratio; }
    /**
     * Returns the sine of the half-angle defining this <tt>CONE</tt>.
     */
    double sine_angle() const { return def.sine_angle; }
    /**
     * Returns the cosine of the half-angle defining this <tt>CONE</tt>.
     */
    double cosine_angle() const { return def.cosine_angle; }

    // Data changing routines.  Each of these routines checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.

/**
 * Sets this <tt>CONE</tt>'s root point to the given position.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param center
 * the new root point.
 */
    void set_root_point(const SPAposition& center);
    /**
     * Sets this <tt>CONE</tt>'s direction to the given unit vector.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param normal
     * the new direction.
     */
    void set_direction(const SPAunit_vector& normal);
    /**
     * Sets this <tt>CONE</tt>'s major axis to the given vector.
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
     * Sets this <tt>CONE</tt>'s major-to-minor radius ratio to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param radius_ratio
     * the new radius ratio.
     */
    void set_radius_ratio(double radius_ratio);
    /**
     * Sets the sine of this <tt>CONE</tt>'s half-angle to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param sine_angle
     * the new sine angle.
     */
    void set_sine_angle(double sine_angle);
    /**
     * Sets the cosine of this <tt>CONE</tt>'s half-angle to the given value.
     * <br><br>
     * <b>Role:</b> Before performing the change, it checks if the data structure
     * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
     * to put an entry on the bulletin board.
     * <br><br>
     * @param cosine_angle
     * the new cosine angle.
     */
    void set_cosine_angle(double cosine_angle);

    // Return the cone equation, for read only.
/**
 * Returns the <tt>surface</tt> equation of this <tt>CONE</tt>, for reading only.
 */
    const surface& equation() const;
    // Return the cone equation, checking for backup first.
/**
 * Returns the <tt>surface</tt> equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    surface& equation_for_update();

    // Get a new (lower-case) surface being the cone of the CONE,
    // transformed if the given SPAtransf is non-null and reversed
    // in sense if the logical is true.
/**
 * Returns the transformed <tt>surface</tt> equation of this <tt>CONE</tt>.
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
    // Transform the stored cone in place.
/**
 * Transforms this <tt>CONE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);
    //	The generic SURFACE version of make_box() is enough
    //	for any ruled surface.
    //	SPAbox make_box( LOOP * ) const;

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

    //	lookup is done using SURFACE::lookup()
    //	int lookup( logical ) const;

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL
public:
    cone gme_get_def();
};
/** @} */
#endif
