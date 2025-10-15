/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Definition of a 2D B-spline curve, used to approximate to true
// curves in the SPAparameter space of a surface. This provides insulation
// between the modeller and the underlying curve package.
/*******************************************************************/
#if !defined( BS2_CURVE )
#define BS2_CURVE

/**
 * \defgroup BS2CURVEAPI 2D B-spline curves 
 * \ingroup BSGEOMETRY
 *
 */
/**
 * @file bs2curve.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS2CURVEAPI
 *
 * @{
 */

// STI dgh begin - moved from bs2c_def.hxx
// Classify the end conditions of the curve, for Acis. These represent
// the object-space form of the curve (since a bs2_curve always
// represents a curve in the SPAparameter space of a surface). In this
// context, "periodic" simply means that the curve closes smoothly,
// that is there is tangent as well as SPAposition continuity around
// the ends. [Note that this is not the same as "periodic end
// conditions" in B-spline parlance.] "Closed" means SPAposition but not
// tangent continuity.

/**
 * Classifies the end conditions of a 2D B-spline curve.
 * @param bs2_curve_open_ends
 * The curve has open ends.
 * @param bs2_curve_closed_ends
 * The curve has position continuity but not tangent
 * continuity.
 * @param bs2_curve_periodic_ends
 * The curve closes smoothly, and there is tangent
 * as well as position continuity around the ends.
 * @param bs2_curve_unknown_ends
 * Unknown end conditions.
 **/

enum bs2_curve_form {
    bs2_curve_open_ends,
    bs2_curve_closed_ends,
    bs2_curve_periodic_ends,
    bs2_curve_unknown_ends
};
// STI dgh end

/**
 * Defines a 2D B-spline curve.
 * <br>
 * <b>Role:</b> Defines a 2D B-spline curve that is used to approximate true curves, and provides
 * insulation between the modeler and the underlying curve package.
 */
typedef class bs2_curve_def *bs2_curve;

/** @} */
#endif

