/* ORIGINAL: acis2.1/kernbody/primtive/primtive.hxx */
/* $Id: primtive.hxx,v 1.8 2000/12/26 18:30:40 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for primitive creation routines.

//STI bd merge STI 1.7 into TSL 2.0:
#if !defined(  PRIMTIVE_HDR_DEF )
#define  PRIMTIVE_HDR_DEF

#include "acis.hxx"
#include "dcl_cstr.h"

#include "bs3surf.hxx"

class BODY;
class spline;
class surface;

// Make an elliptical prism of given height, radii and number of sides.
// Centre it at the origin, aligned with axis along the z-axis
// and one face perpendicular to the positive x-axis.

DECL_CSTR BODY *make_prism(
			double,			// height
			double,			// x-radius
			double,			// y-radius
			int				// number of sides
		);


// Make an elliptical pyramid of given height, radii and number of
// sides.  The apex is at given height above the origin.
// Centre it at the origin, aligned with axis along the z-axis
// and one face perpendicular to the positive x-axis.

DECL_CSTR BODY *make_pyramid(
			double,			// height
			double,			// x-radius
			double,			// y-radius
			double,			// top x-radius
			int				// number of sides
		);


// Make a rectangular block aligned with the axes and centered at the
// origin.

DECL_CSTR BODY *make_parallelepiped(
			double,			// size in x
			double,			// size in y
			double			// size in z
		);


// Make an elliptical cylinder of given height and radii.
// Centre it at the origin, aligned with axis along the z-axis
// and one face perpendicular to the positive x-axis.

DECL_CSTR BODY *make_cylinder(
			double,			// height
			double,			// x-radius
			double			// y-radius
		);


// Make an elliptical frustum of given height and radii.
// Centre it at the origin, aligned with axis along the z-axis
// and one face perpendicular to the positive x-axis.

DECL_CSTR BODY *make_frustum(
			double,			// height
			double,			// x-radius at bottom
			double,			// y-radius at bottom
			double			// x-radius at top
		);


// Make a sphere centred on the origin, of given radius.

DECL_CSTR BODY *make_sphere(
			double			// radius
		);


// Make a torus centred on the origin, in the x-y plane, and with
// given radii.

DECL_CSTR BODY *make_torus(
			double,			// major radius
			double			// minor radius
		);


// Make a thick slab with a periphery specified by the lines joining
// a series of 2D coordinate positions that are swept along the z-axis.
// The slab is centered about the plane z=0.

DECL_CSTR BODY *make_slab(
			double,			// height in z
			double [],		// x-coordinates
			double [],		// y-coordinates
			int				// number of sides
		);

// Construct a single face which is the whole of a given B-spline
// surface. The face normal follows the sense of the spline surface,
// and the face has one loop with four coedges.

DECL_CSTR BODY *make_spline(
			spline const &
		);

// Make a block with a wiggly top. The shape of the wiggle is supplied
// as the first argument, but is required to be a surface whose
// projection on the x-y plane is a rectangle of width (i.e. length in
// the x-direction) equal to the first double argument and depth
// (length in the y direction) equal to the second, centred on the
// origin. Its boundary curves must project into the boundaries of the
// rectangle, with its u SPAparameter corresponding to the x axis and
// v SPAparameter corresponding to the y axis. Finally, it must not
// project anywhere below z = - height / 2, where height is the
// third double argument.

DECL_CSTR BODY *make_wiggle(
			bs3_surface,
			double,
			double,
			double
		);

//STI bd merge STI 1.7 into TSL 2.0:
#endif
