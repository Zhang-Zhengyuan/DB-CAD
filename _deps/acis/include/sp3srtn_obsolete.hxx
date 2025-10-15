/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// This file contains Obsolete routines for processing 3D B-spline surfaces.
/*******************************************************************/
#ifndef sp3srtn_obsolete_hxx
#define sp3srtn_obsolete_hxx
#include "dcl_spl.h"
#include "sp3srtn.hxx"

// This function is now OBSOLETE.  Use d3_sf_sf_int().

// Intersect a plane and a B-spline surface. Convert the plane to a
// B-spline surface, find intersection point lists, and then interpolate
// curves.
// A logical is set to show whether the B-spline surface is to be
// considered negated or not, and another to show whether we want
// to intersect the B-spline with the plane or plane with B-spline.


// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE surf_surf_int* bs3_surface_pla_int(
					bs3_surface,		// given B-spline surface
					logical,			// Set TRUE for the B-spline surface negated
					plane const&,		// given plane
					logical,			// Set TRUE for intersection of plane with
										// B-spline, FALSE for vice-versa
					SPAbox const&		// region of interest
					);



// This function is now OBSOLETE.  Use d3_sf_sf_int().

// Intersect a cone and a B-spline surface.
// A logical is set to show whether the B-spline surface is to be
// considered negated or not, and another to show whether we want
// to intersect the B-spline with the cone or cone with B-spline.

/*
// tbrv
*/
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE surf_surf_int* bs3_surface_con_int(
						bs3_surface,	// given B-spline surface
						logical,		// Set TRUE for the B-spline surface negated
						cone const&,	// given cone
						logical,		// Set TRUE for intersection of cone with
										// B-spline, FALSE for vice-versa
						SPAbox const&	// region of interest
						);

// This function is now OBSOLETE.  Use d3_sf_sf_int().

// Intersect a sphere and a B-spline surface.
// A logical is set to show whether the B-spline surface is to be
// considered negated or not, and another to show whether we want
// to intersect the B-spline with the sphere or sphere with B-spline.


// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE surf_surf_int* bs3_surface_sph_int(
						bs3_surface,	// given B-spline surface
						logical,		// Set TRUE for the B-spline surface negated
						sphere const&,	// given sphere
						logical,		// Set TRUE for intersection of sphere with
										// B-spline, FALSE for vice-versa
						SPAbox const&	// region of interest
						);


// This function is now OBSOLETE.  Use d3_sf_sf_int().

// Intersect a torus and a B-spline surface.
// A logical is set to show whether the B-spline surface is to be
// considered negated or not, and another to show whether we want
// to intersect the B-spline with the torus or torus with B-spline.

// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE surf_surf_int* bs3_surface_tor_int(
						bs3_surface,	// given B-spline surface
						logical,		// Set TRUE for the B-spline surface negated
						torus const&,	// given torus
						logical,		// Set TRUE for intersection of torus with
										// B-spline, FALSE for vice-versa
						SPAbox const&	// region of interest
						);


// This function is now OBSOLETE.  Use d3_sf_sf_int().

// Intersect two B-spline surfaces together.
// Logicals are set to show whether one or both surfaces is to be
// considered negated.


// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE surf_surf_int* bs3_surface_bs3_int(
						bs3_surface,	// given B-spline surface-a
						logical,		// Set TRUE for B-spline surface-a negated
						bs3_surface,	// given B-spline surface-b
						logical,		// Set TRUE for B-spline surface-b negated
						SPAbox const&	// region of interest
						);

// OBSOLETE:
// *****************************************************************
//  Obsolete Routines ( These will go away in a future release. )
// *****************************************************************


// Make a surface by sweeping a shape curve along a planar path curve. Both
// shape and path curve are in (the same) object space.


// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE bs3_surface
bs3_surface_sweep_planar(
	const bs3_curve shape,  // curve to be swept
	const bs3_curve path    // planar curve to sweep along
);



// Routine, added to specialise the top curve construction
// for perpendicular sweep, which results in a exact sum-surface as
// the lateral_surface.
// The output is the projection of the low_u curve of the sum surface
// on to the given plane along the given straight path.

/**
* Specializes the top %curve construction for perpendicular sweep.
* <br><br>
* <b>Role:</b> Specializes the top %curve construction for perpendicular sweep,
* which results in an exact sum %surface as the <tt>lateral_surface</tt>. The output is the
* projection of the <tt>low_u</tt> %curve of the sum %surface on to the miter plane along the
* given straight path.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param in_sur
* given sum %surface.
* @param start
* path start.
* @param path_dir
* path direction.
* @param plane_root
* miter plane root point.
* @param plane_nor
* miter plane normal.
**/
[[deprecated("Deprecated API")]]
DECL_SPLINE bs3_curve
bs3_surface_sum_x_pla_str(
	const bs3_surface in_sur,		// Input sum surface
	const SPAposition& start,		// path start
	const SPAunit_vector& path_dir,	// path direction
	const SPAposition& plane_root,		// mitre_plane root point
	const SPAunit_vector& plane_nor		// mitre plane normal.
);


// REMOVED:
// *****************************************************************
//  Removed Routines ( These routines are removed in this release ).
// *****************************************************************


// New routine, added to specialise the sweep surface construction for
// for perpendicular sweep with draft.


// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE bs3_surface
bs3_surface_sweep_w_draft(
	const bs3_curve,		// Input Profile to be swept
	const curve*,			// path along which it is swept
	const SPAunit_vector&,		// Path normal
	double,					// Total Draft angle
	int,					// Number of sample points
	const SPAposition[],		// Array of Sample points on the path
	const SPAvector[],			// Array of Tangents at the sample points
	const double[],			// Array of SPAparameter values
	const double[]			// Array of Incremental Draft angle
);

// Skin two 3D planar curves, Use a Linear parametrization, make a
// simple ruled surface.

// For internal use only
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE bs3_surface
bs3_surface_simple_skin(
	bs3_curve,				// Curve 1
	bs3_curve				// Curve 2
);

// Set the form of a surface. This is only used in the IG spline
// package. It is a stub in the AG spline package.
/**
 * @nodoc
 */
[[deprecated("Deprecated API")]]
DECL_SPLINE void bs3_surface_set_form(	bs3_surface bs3);

#endif

