/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare procedures for working with work coordinate systems

#ifndef wcs_utl_hxx
#define wcs_utl_hxx
#include "dcl_kern.h"
#include "api.hxx"
#include "wcs.hxx"
/**
 * @file wcs_utl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISWCS
 *
 * @{
 */

class SPAtransf;
class outcome;
class SPAposition;
class SPAvector;
class SPAunit_vector;

//======================================================================

// The procedures in this file help in mapping things between model space
// and the active WCS.  In all cases, if no active WCS is defined (which
// means that it is the same as model space), these procedures will do the
// right thing, so you do not have to perform extra checking to make sure
// that the active WCS is not NULL.

// Get the SPAtransf which maps from the active WCS to model space.

/**
* Gets the transformation that maps from the active WCS to model space.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAtransf wcs_active_to_model();

// Get the SPAtransf which maps from model space to the active WCS.

/**
* Gets the transformation that maps from model space to the active WCS.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAtransf wcs_model_to_active();

// Map a SPAposition from the active WCS to model space

/**
* Gets a model space position from a WCS position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pt
* position.
*/

DECL_KERN SPAposition to_model(const SPAposition& pt);

// Map a SPAvector from the active WCS to model space

/**
* Gets a model space position from a WCS position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* vector.
*/

DECL_KERN SPAvector to_model(const SPAvector& v);

// Map a SPAunit_vector from the active WCS to model space

/**
* Gets a model space position from a WCS position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* unit vector.
*/

DECL_KERN SPAunit_vector to_model(const SPAunit_vector& v);

// Map a SPAposition from model space to the active WCS

/**
* Gets a WCS position from a model space position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param pt
* position.
*/

DECL_KERN SPAposition to_wcs(const SPAposition& pt);

// Map a SPAvector from model space to the active WCS

/**
* Gets a WCS position from a model space position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* vector.
*/

DECL_KERN SPAvector to_wcs(const SPAvector& v);

// Map a SPAunit_vector from model space to the active WCS

/**
* Gets a WCS position from a model space position.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* unit vector.
*/

DECL_KERN SPAunit_vector to_wcs(const SPAunit_vector& v);

//----------------------------------------------------------------------
// Get the origin of the active WCS

/**
* Gets the origin of the active WCS.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAposition wcs_origin();

// Get the x axis of the active WCS

/**
* Gets the x-axis of the active WCS.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAunit_vector wcs_x_axis();

// Get the y axis of the active WCS

/**
* Gets the y-axis of the active WCS.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAunit_vector wcs_y_axis();

// Get the z axis of the active WCS

/**
* Gets the z-axis of the active WCS.
* <br><br>
* <b>Role:</b> When there is no active WCS, the default is model space, and a
* one-to-one mapping occurs; therefore, verification that the active WCS is not
* <tt>NULL</tt> is unnecessary.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN SPAunit_vector wcs_z_axis();

//======================================================================
// These procedures are used by the API routines.  Application code can use
// api_wcs_create, api_wcs_set_active or api_wcs_get_active.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN outcome wcs_create(
    const SPAposition&, // origin of the WCS
    const SPAposition&, // SPAposition on the x axis
    const SPAposition&, // SPAposition in the positive XY plane
    WCS*&            // the WCS created
    );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN outcome wcs_create(
    const SPAposition&,    // origin of the WCS
    const SPAunit_vector&, // direction of the x axis
    const SPAunit_vector&, // direction in the positive XY plane
    WCS*&               // the WCS created
    );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN WCS* wcs_set_active(WCS*);

/**
* Gets the active WCS.
* <br><br>
* <b>Role:</b> Only zero or one WCS can be active at any time.
* <br><br>
* <b>Effect:</b> Read-only
*/

DECL_KERN WCS* wcs_get_active();

/**
* Sets the point or origin of the given WCS.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param wcs
* given WCS.
* @param new_origin
* new point of origin.
*/

DECL_KERN outcome wcs_set_origin(WCS* wcs, const SPAposition& new_origin);

//======================================================================
/** @} */
#endif
