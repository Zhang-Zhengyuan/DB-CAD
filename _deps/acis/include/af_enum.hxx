/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_ENUM
#define _AF_ENUM
/**
* @file af_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <af_enum.hxx>
 *  @{
 */
/**
 * Specifies the surface mode.
 * @param AF_SURF_ALL
 * All surfaces.
 * @param AF_SURF_REGULAR
 * regular surfaces.
 * @param AF_SURF_IRREGULAR
 * irregular surfaces.
 * @param AF_SURF_PLANE
 * planes.
 * @param AF_SURF_CONE
 * cones.
 * @param AF_SURF_SPHERE
 * spheres.
 * @param AF_SURF_TORUS
 * Tori.
 * @param AF_SURF_SPLINE
 * splines.
 * @param AF_SURF_MODE_ARRAY_DIM
 * dimension arrays.
 */

typedef enum {
		AF_SURF_ALL,		// All surfaces
		AF_SURF_REGULAR,	// Surfaces for which all 4 corners of
							//	a rectangle in SPAparameter space are
							//	coplanar in real space.
		AF_SURF_IRREGULAR,	// Surfaces for which the 4 corners of
							//	a rectangle in SPAparameter space are
							//	not necessarily coplanar in real space.
		AF_SURF_PLANE,		// Planes
		AF_SURF_CONE,		// Conical surfaces
		AF_SURF_SPHERE,		// Spheres
		AF_SURF_TORUS,		// Tori
		AF_SURF_SPLINE,		// Splines
		AF_SURF_MODE_ARRAY_DIM	// use this to dimension arrays
} AF_SURF_MODE ;

/* @} */
#endif
