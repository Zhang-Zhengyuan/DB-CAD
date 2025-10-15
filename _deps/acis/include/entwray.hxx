/* ORIGINAL: 3dt2.1/geomhusk/entwray.hxx */
// $Id: entwray.hxx,v 1.8 2002/08/09 17:15:14 jeff Exp $
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
//    define class for ENTITY with pick ray
//
//----------------------------------------------------------------------

#ifndef entwray_hxx
#define entwray_hxx

#include "dcl_kern.h"
#include "pick_ray.hxx"

/**
* @file entwray.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class ENTITY;
class SPAposition;
class SPAunit_vector;

class acis_ostream;

//======================================================================

// An entity_with_ray is a combination of an ENTITY and a pick_ray.
// It is used in some of the API routines in the Geometric Design Husk
// to indicate which SPAposition of a curve you want to operate on.  For
// example, in the routine api_trim_curve, an entity_with_ray is used
// to specify the porition of the curve that you want to keep.  It is
// also used to compute a SPAposition on the curve near the desired solution
// for procedures for which there can be more than one solution.
/**
 * Combines an <tt>ENTITY</tt> and a <tt>pick_ray</tt>.
 * <br>
 * <b>Role</b>: An <tt>entity_with_ray</tt> is a combination of an <tt>ENTITY</tt> and a <tt>pick_ray</tt>.
 * Some API routines use this class to indicate the position on a curve where the
 * user wants to operate. In <tt>api_trim_curve</tt>, for example, an <tt>entity_with_ray</tt> specifies
 * the portion of the curve to be kept. It also computes a position on the curve near
 * the desired solution for procedures when there are multiple solutions.
 *<br><br>
 * @see ENTITY, pick_ray
 */
 class DECL_KERN entity_with_ray : public ACIS_OBJECT
{
  private:
    ENTITY* The_Entity;
    pick_ray The_Ray;

  public:

    //----------------------------------------
    // Constructors
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
     entity_with_ray();
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param ent
 * entity.
 * @param ray
 * pick ray.
 */
      entity_with_ray(ENTITY* ent, const pick_ray& ray);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param ent
 * entity.
 * @param posi
 * position.
 * @param vec
 * unit vector.
 */
     entity_with_ray(ENTITY* ent, const SPAposition& posi, const SPAunit_vector& vec);

    //----------------------------------------
    // Inquiry Functions

    // Get the ENTITY
/**
 * Gets the <tt>ENTITY</tt>.
 */
     ENTITY* entity()const {return The_Entity;}

    // Get the pick_ray
/**
 * Gets the <tt>pick_ray</tt>.
 */
     pick_ray ray() const  {return The_Ray;}

    //----------------------------------------
    // Change Functions

    // Set the ENTITY
/**
 * Sets the <tt>ENTITY</tt>.
 *<br><br>
 * @param ent
 * entity.
 */
     void set_entity(ENTITY* ent) {The_Entity = ent;}

    // Set the pick_ray
/**
 * Sets the <tt>pick_ray</tt>.
 *<br><br>
 * @param ray
 * pick ray.
 */
     void set_ray(const pick_ray& ray) {The_Ray = ray;}
};


//----------------------------------------------------------------------

//======================================================================
/*! @} */
#endif
