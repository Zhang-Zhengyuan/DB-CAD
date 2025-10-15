/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header to interface between Acis and AGlib (or in principle any
// other spline package). This simply provides a definition for a
// spline curve type for the major part of Acis to use.
#if !defined( BS3_SURFACE )
#define BS3_SURFACE
/**
 * \defgroup BSGEOMETRY B-spline geometry
 * \ingroup ACISGEOMETRY
 *
 * \defgroup BS3SURFACEAPI 3D B-spline surfaces
 * \ingroup BSGEOMETRY
 *
 */
/**
 * @file bs3surf.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3SURFACEAPI
 * \ingroup BSGEOMETRY
 *
 * @{
 */
/**
 * Classifies the end conditions of a 3D B-spline surface.
 * @param bs3_surf_open_ends
 * The surface has open ends.
 * @param bs3_surf_closed_ends
 * The surface has position continuity but not tangent continuity.
 * @param bs3_surf_periodic_ends
 * The surface closes smoothly, and there is tangent as well as position continuity
 * around the ends.
 * @param bs3_surf_unknown_ends
 * unknown end conditions.
 **/
enum bs3_surf_form {
    bs3_surf_open_ends,
    bs3_surf_closed_ends,
    bs3_surf_periodic_ends,
    bs3_surf_unknown_ends
};
/**
 * Defines a 3D B-spline surface.
 * <br>
 * <b>Role:</b> Defines a 3D B-spline surface that is used to approximate true surfaces, and
 * provides insulation between the modeler and the underlying curve package.
 */
typedef class bs3_surf_def *bs3_surface;

/** @} */
#endif
