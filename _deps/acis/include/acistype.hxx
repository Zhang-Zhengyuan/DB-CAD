/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare procedures for checking ACIS types
#ifndef acistype_hxx
#define acistype_hxx
class ENTITY;
class FACE;
class surface;
class curve;
class helix;
#include "dcl_kern.h"
#include "logical.h"
/**
 * \defgroup ACISQUERIES Topology and Geometry Queries
 * \ingroup KERNAPI
 */

/**
 * @file acistype.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */
/**
 * Determines if an <tt>ENTITY</tt> is a <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>BODY</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_BODY(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>WIRE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>WIRE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_WIRE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>LUMP</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>LUMP</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_LUMP(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SHELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>SHELL</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_SHELL(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SUBSHELL</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a SUBSHELL, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_SUBSHELL(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>FACE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>FACE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_FACE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>LOOP</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>LOOP</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_LOOP(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is an <tt>EDGE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_EDGE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>COEDGE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>COEDGE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_COEDGE(const ENTITY* ent);

/**
 * Determines if the <tt>ENTITY</tt> is a <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>VERTEX</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_VERTEX(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>APOINT</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is an <tt>APOINT</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_APOINT(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>CURVE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_CURVE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>PCURVE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is an <tt>PCURVE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_PCURVE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>STRAIGHT</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>STRAIGHT</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_STRAIGHT(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>ELLIPSE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is an <tt>ELLIPSE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_ELLIPSE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>INTCURVE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is an <tt>INTCURVE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_INTCURVE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>HELIX</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>HELIX</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_HELIX(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SURFACE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>SURFACE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_SURFACE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>PLANE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>PLANE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_PLANE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>CONE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>CONE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_CONE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SPHERE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>SPHERE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_SPHERE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>TORUS</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>TORUS</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_TORUS(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>SPLINE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is a <tt>SPLINE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_SPLINE(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>ATTRIB</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is an <tt>ATTRIB</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_ATTRIB(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is an <tt>ATTRIB_ST</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the <tt>ENTITY</tt> is an <tt>ATTRIB_ST</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for
 * a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_ATTRIB_ST(const ENTITY* ent);

//----------------------------------------------------------------------
// These procedures check to see if a curve or surface is a specified type.

/**
 * Determines if a <tt>surface</tt> is a <tt>plane<tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_plane(const surface* surf);

/**
 * Determines if a <tt>surface</tt> is a <tt>cone</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_cone(const surface* surf);

/**
 * Determines if a <tt>surface</tt> is a <tt>sphere</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_sphere(const surface* surf);

/**
 * Determines if a <tt>surface</tt> is a <tt>torus</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_torus(const surface* surf);

/**
 * Determines if a <tt>surface</tt> is a <tt>spline</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_spline(const surface* surf);

/**
 * This composite test checks for a <tt>cone</tt> that is cylindrical and circular.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param surf
 * surface to test.
 **/

DECL_KERN logical is_cylinder(const surface* surf);

/**
 * Determines if a <tt>curve</tt> is a <tt>straight</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_straight(const curve* crv);

/**
 * Determines if a <tt>curve</tt> is an <tt>ellipse</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_ellipse(const curve* crv);

/**
 * Determines if a <tt>curve</tt> is a circular <tt>ellipse</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_circular(const curve* crv);

/**
 * Determines if a <tt>curve</tt> is a <tt>helix</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_helix(const curve* crv);

/**
 * Determines if a <tt>curve</tt> is an <tt>intcurve</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_intcurve(const curve* crv);

/**
 * Determines if a <tt>curve</tt> is a composite curve constructed from multiple <tt>law</tt>s.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve to test.
 **/

DECL_KERN logical is_composite_curve(const curve* crv);

//----------------------------------------------------------------------
// These procedures check for specific kinds of BODYs.  
// A solid BODY
//	1. should have at least one LUMP that contains SHELL having FACE(s),
//	2. should not have WIRE,
//	3. should not have any LUMP that contains SHELL having WIRE(s).
// 
// A wire BODY
//	1. should have either a WIRE or a LUMP that contains SHELL having WIRE(s),
//	2. should not have any LUMP that contains SHELL having FACE(s)
// 
// A mixed BODY 
//	1. should have at least one LUMP that contains SHELL having FACE(s),
//	2. should have at least one LUMP that contains SHELL having WIRE(s),
//	3. no WIREs is attached to the BODY directly.
//
// If the ENTITY is not a BODY or is NULL, then these procedures will return FALSE.

/**
 * Determines if an <tt>ENTITY</tt> is a solid <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> A solid <tt>BODY</tt> 
 * <br><br>
 * &nbsp;&nbsp;&nbsp;&nbsp; 1. should have at least one <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>FACE</tt>(s),
 * &nbsp;&nbsp;&nbsp;&nbsp; 2. should not have <tt>WIRE</tt>,
 * &nbsp;&nbsp;&nbsp;&nbsp; 3. should not have any <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>WIRE</tt>(s).
 * 
 * <br><br>
 * If the <tt>ENTITY</tt> is not a solid <tt>BODY</tt> or is <tt>NULL</tt>, then this function returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_solid_body(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a closed solid <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> This functions checks to see if the faces of a body are closed.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_closed_solid_body(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a sheet <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> This functions checks to see if the ENTITY is a sheet body.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_sheet_body(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a wire <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if:
 * <br><br>
 * &nbsp;&nbsp;&nbsp;&nbsp; 1. should have either a <tt>WIRE</tt> or a <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>WIRE</tt>(s),
 * &nbsp;&nbsp;&nbsp;&nbsp; 2. should not have any <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>FACE</tt>(s) 
 * <br><br>
 * 
 * If the <tt>ENTITY</tt> is not a wire <tt>BODY</tt> or is <tt>NULL</tt>, then this function returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_wire_body(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a mixed <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> A mixed <tt>BODY</tt>
 * <br><br>
 * &nbsp;&nbsp;&nbsp;&nbsp; 1. should have at least one <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>FACE</tt>(s),
 * &nbsp;&nbsp;&nbsp;&nbsp; 2. should have at least one <tt>LUMP</tt> that contains <tt>SHELL</tt> having <tt>WIRE</tt>(s),
 * &nbsp;&nbsp;&nbsp;&nbsp; 3. no <tt>WIRE</tt>s is attached to the <tt>BODY</tt> directly.
 *
 * <br><br>
 * If the <tt>ENTITY</tt> is not a mixed <tt>BODY</tt> or is <tt>NULL</tt>, then this function returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_mixed_body(const ENTITY* ent);

/**
 * Determines if an <tt>ENTITY</tt> is a connected <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the <tt>BODY</tt> has exactly one <tt>LUMP</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_connected_body(const ENTITY* ent);


// do not doc
/*
// tbrv
 */
/**
 * @nodoc
 */

DECL_KERN logical is_curve_tcoedge(const ENTITY*);

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical is_elliptical_tcoedge(const ENTITY*);

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical is_circular_tcoedge(const ENTITY*);

//----------------------------------------------------------------------
// These procedures check to see if an ENTITY is an EDGE which has a specific
// kind of curve for its geometry.  An EDGE with no geometry pointer will
// return FALSE for all of these queries.

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>EDGE</tt> with a <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> An edge with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_curve_edge(const ENTITY*ent);

/**
 * Determines if an <tt>ENTITY</tt> is a linear edge; that is, an <tt>EDGE</tt> whose underlying geometry is a <tt>straight</tt>.
 * <br><br>
 * <b>Role:</b> An <tt>EDGE</tt> with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_linear_edge(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is an elliptical edge; that is, an <tt>EDGE</tt> whose underlying geometry is an <tt>ellipse</tt>.
 * <br><br>
 * <b>Role:</b> An <tt>EDGE</tt> with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_elliptical_edge(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is a helical edge; that is, an <tt>EDGE</tt> whose underlying geometry is a <tt>helix</tt>.
 * <br><br>
 * <b>Role:</b> An <tt>EDGE</tt> with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_helical_edge(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is a circular edge; that is, an <tt>EDGE</tt> whose underlying geometry is a circular <tt>ellipse</tt>.
 * <br><br>
 * <b>Role:</b> An edge with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_circular_edge(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is an intcurve edge; that is, an <tt>EDGE</tt> whose underlying geometry is an <tt>intcurve</tt>.
 * <br><br>
 * <b>Role:</b> An edge with no geometry pointer returns <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_intcurve_edge(const ENTITY *ent);

//----------------------------------------------------------------------
// These procedures check to see if an ENTITY is a FACE which has specific
// kind of surface for its geometry.

/**
 * Determines if an <tt>ENTITY</tt> is a planar face; that is, a <tt>FACE</tt> whose underlying geometry is a <tt>plane</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_planar_face(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is a cylindrical face; that is, a <tt>FACE</tt> whose underlying geometry is a cylindrical <tt>cone</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_cylindrical_face(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is a conical face; that is, a <tt>FACE</tt> whose underlying geometry is a <tt>cone</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_conical_face(const ENTITY *ent);

/**
 * Determines if an <tt>ENTITY</tt> is a spherical face; that is, a <tt>FACE</tt> whose underlying geometry is a <tt>sphere</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_spherical_face(const ENTITY *ent);

/**
 * Determines if the <tt>ENTITY</tt> is a toroidal face; that is, a <tt>FACE</tt> whose underlying geometry is a <tt>torus</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_toroidal_face(const ENTITY *ent);

/**
 * Determines if the <tt>ENTITY</tt> is a spline face; that is, a <tt>FACE</tt> whose underlying geometry is a <tt>spline</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_spline_face(const ENTITY *ent);

/**
 * Determines if the <tt>ENTITY</tt> is an analytic face; that is, a <tt>FACE</tt> whose underlying geometry is an analytic <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is not a <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/

DECL_KERN logical is_analytic_face(const ENTITY*);

/**
* Determines if the <tt>ENTITY</tt> is a mesh face; that is, a <tt>FACE</tt> whose underlying geometry is a mesh or triangulation.
* <br><br>
* <b>Role:</b> Returns <tt>TRUE</tt> if the face has its geometry in the form of triangulation and not as a parametric surface.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param ent
* entity to test.
**/
DECL_KERN logical is_mesh_face(const ENTITY* ent);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural rot-spl face; that is, a <tt>FACE</tt> whose underlying geometry is a rot_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a rot_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_rot_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural blend face; that is, a <tt>FACE</tt> whose underlying geometry is a blend_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a blend_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_blend_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural net face; that is, a <tt>FACE</tt> whose underlying geometry is a net_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a net_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_net_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural offset face; that is, a <tt>FACE</tt> whose underlying geometry is a off_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a off_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_offset_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural ruled face; that is, a <tt>FACE</tt> whose underlying geometry is a rule_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a rule_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_rule_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural skin face; that is, a <tt>FACE</tt> whose underlying geometry is a skin_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a skin_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_skin_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural sum face; that is, a <tt>FACE</tt> whose underlying geometry is a sum_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a sum_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_sum_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural sweep face; that is, a <tt>FACE</tt> whose underlying geometry is a sweep_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a sweep_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_sweep_face(const ENTITY*);

/**
 * Determines if the <tt>ENTITY</tt> is a procedural taper face; that is, a <tt>FACE</tt> whose underlying geometry is a taper_spl_sur <tt>surface</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the face is a taper_spl_sur <tt>SPLINE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to test.
 **/
DECL_KERN logical is_procedural_taper_face(const ENTITY*);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical is_open_single_sided(const ENTITY* ent);


//----------------------------------------------------------------------
// The following procedures get the type of the specified kinds of
//  ATTRIBUTEs.

/**
 * Gets the type of <tt>TSL</tt> attribute.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/

DECL_KERN int get_ATTRIB_TSL_TYPE();

/**
 * Gets the type of system attribute.
 * <br><br>
 * <b>Effect:</b> Read-only
 **/

DECL_KERN int get_ATTRIB_SYS_TYPE();

//----------------------------------------------------------------------
// Get a string that tells what kind of ENTITY you have in more detail
// than you get from ENTITY::type_name

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN const char* get_entity_type_string(const ENTITY*);

//======================================================================
/** @} */
#endif
