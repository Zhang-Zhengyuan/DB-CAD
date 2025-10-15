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
#if !defined( BS3_CURVE )
#define BS3_CURVE
/**
 * \defgroup BS3CURVEAPI 3D B-spline curves 
 * \ingroup BSGEOMETRY
 *
 */
/**
 * @file bs3curve.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3CURVEAPI 3D B-spline curves 
 *
 * @{
 */
// STI dgh begin - moved from bs3c_def.hxx
// Classify the end conditions of the curve, for Acis. In this
// context, "periodic" simply means that the curve closes smoothly,
// that is there is tangent as well as SPAposition continuity around
// the ends. [Note that this is not the same as "periodic end
// conditions" in B-spline parlance.] "Closed" means SPAposition but not
// tangent continuity.
/**
* Classifies the end conditions of a 3D B-spline curve.
* @param bs3_curve_open_ends
* The curve has open ends.
* @param bs3_curve_closed_ends
* The curve has position continuity but not tangent
* continuity.
* @param bs3_curve_periodic_ends
* The curve closes smoothly, and there is tangent
* as well as position continuity around the ends.
* @param bs3_curve_unknown_ends
* unknown end conditions.
**/
enum bs3_curve_form {
    bs3_curve_open_ends,
    bs3_curve_closed_ends,
    bs3_curve_periodic_ends,
    bs3_curve_unknown_ends
};

/**
 * Define some possible invalid return values for knot multiplicity at a seam, in preparation
 * to allowing periodic splines into ACIS
 * @param BS_UNKNOWN_MULTIPLICITY
 * The curve was not input with a periodic knot vector
 * @param BS_INVALID_MULTIPLICITY
 * 
 */
enum bs_seam_multiplicity_e {
  BS_UNKNOWN_MULTIPLICITY = -1,  
  BS_INVALID_MULTIPLICITY =  0   
};

/**
 *  Define expected values for seam knot removal
 */
enum  bs_knot_removal_mode {
  BS_FORCE_KNOT_REMOVAL   = 2, 
  BS_REMOVE_WHEN_POSSIBLE = 1,
  BS_CHECK_IF_POSSIBLE    = 0
};
/**
 * Defines a 3D B-spline curve.
 * <br>
 * <b>Role:</b> Defines a 3D B-spline curve that is used to approximate true curves, and provides
 * insulation between the modeler and the underlying curve package.
 */
typedef class bs3_curve_def *bs3_curve;

/** @} */
#endif

