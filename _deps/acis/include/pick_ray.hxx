/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare the pick_ray class for representing a SPAposition and direction.
//
// design---
//    This is similar to the ray that is already defined in ACIS, except
//    that it does not have a radius, or carry other information about
//    what it intersects.  Maybe we should just use the ACIS ray and ignor
//    the stuff in it that we do not want.  I think that ACIS should have
//    really defined ray like this class, and then defined another class
//    which had a ray plus the other information.
//
//----------------------------------------------------------------------

#ifndef pick_ray_hxx
#define pick_ray_hxx

/**
 * @file pick_ray.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */
#include "dcl_kern.h"
#include "position.hxx"
#include "unitvec.hxx"

class SPAvector;
class SPAtransf;

//======================================================================

// A pick_ray is a combination of a SPAposition and a direction.  It will
// typically be used allow mapping a 2D graphic pick onto a SPAposition on
// an ENTITY such as an EDGE which is defined in model space.  In this
// context, the SPAposition would be the 2D pick location mapped into 3D space,
// and the direction will be the direction in which you are looking at
// the model in the view in which the pick was performed.

/**
 * 	Maps a <tt>2D</tt> graphic pick on an entity position defined in model space.
 * <br>
 * <b>Role:</b> A <tt>pick_ray</tt> is a combination of a position and a direction. It typically allows
 * mapping a <tt>2D</tt> graphic pick on an <tt>ENTITY</tt> position, such as an <tt>EDGE</tt>, that is defined in model
 * space. In this context, the position is the <tt>2D</tt> pick location mapped into <tt>3D</tt> space, and the
 * direction is the direction the user looks at (on the model and in the view) when the pick is
 * executed.
 * <br><br>
 * @see entity_with_ray, SPAposition, SPAunit_vector
 */
class DECL_KERN pick_ray : public ACIS_OBJECT
{
  private:

    SPAposition root_point;
    SPAunit_vector ray_direction;

  public:

    //----------------------------------------
    // Constructors

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
    pick_ray();
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pos
 * position.
 * @param vec
 * unit vector.
 */
    pick_ray(const SPAposition& pos, const SPAunit_vector& vec);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pos
 * position.
 * @param vec
 * unit vector.
 */
    pick_ray(const SPAposition& pos, const SPAvector& vec);
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param ray
 * pick ray.
 */
    pick_ray(const pick_ray& ray);

    //----------------------------------------
    // Inquiry Functions

    // Get the SPAposition from the pick_ray
/**
 * Gets the position from the <tt>pick_ray</tt>.
 */
    SPAposition point() const;

    // Get the direction (SPAunit_vector) from the pick_ray

/**
 * Gets the direction (<tt>SPAunit_vector</tt>) from the <tt>pick_ray</tt>.
 */
    SPAunit_vector direction() const;

    //----------------------------------------
    // Change Functions

    // Set the SPAposition

/**
 * Sets the position.
 * <br><br>
 * @param pos
 * position.
 */
    void set_point(const SPAposition& pos);

    // Set the direction
/**
 * Sets the direction with a <tt>SPAunit_vector</tt>.
 * <br><br>
 * @param vec
 * unit vector.
 */
    void set_direction(const SPAunit_vector& vec);
/**
 * Sets the direction with a vector.
 * <br><br>
 * @param vec
 * vector.
 */
    void set_direction(const SPAvector& vec);

    // Apply a transform to the pick_ray

/**
 * Applies a transformation to the <tt>pick_ray</tt>.
 * <br><br>
 * @param trans
 * transformation.
 */
    pick_ray& operator*=(const SPAtransf& trans);

    //----------------------------------------
    // Miscellaneous Functions

    // Return a new pick_ray which is the result of applying a transform
    // to this pick_ray
/**
 * Returns a new <tt>pick_ray</tt> that is the result of applying a transformation to this <tt>pick_ray</tt>.
 * <br><br>
 * @param trans
 * transformation.
 */
    pick_ray operator*(const SPAtransf& trans) const;

    // Compute the distance from the pick_ray to a SPAposition
/**
 * Computes the distance from the <tt>pick_ray</tt> to a position.
 * <br><br>
 * @param pt
 * position.
 */
    double distance_to(const SPAposition& pt) const;

};

//----------------------------------------------------------------------

//======================================================================
/** @} */
#endif
