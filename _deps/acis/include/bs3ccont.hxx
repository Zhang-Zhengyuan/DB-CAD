/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( BS3_CURVE_CONT_CLASS )
#define BS3_CURVE_CONT_CLASS
#include "dcl_spl.h"
#include "mmgr.hxx"
/**
 * @file bs3ccont.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRY
 *
 * @{
 */

/**
 * @nodoc
 * A curve may have either
 * acceleration_direction (G2) or acceleration_magnitude (C2) discontinuities,
 * or tangent_direction (G1) or tangent_magnitude (C1) discontinuities.
 */
enum irr_type {
	tangent_direction,
	tangent_magnitude,
	acceleration_direction,
	acceleration_magnitude
};

/**
 * Implements a linked list of parameter values at which a curve has a C1 or G1 discontinuity.
 * <br>
 * <b>Role:</b> This class implements a linked list of parameter values at which a curve has a
 * tangent direction (G1) or tangent magnitude (C1) discontinuity.
 */
class DECL_SPLINE curve_irregularities : public ACIS_OBJECT {
public:
/**
 * The parameter value at which the discontinuity exists.
 */
 	double par_val;				// parameter value at which the discontinuity exists
/**
 * Type of irregularity: either tangent direction (G1) or tangent magnitude (C1) discontinuity.
 */
 	irr_type ir;				// C2/G2/C1/G1 discontinuity
/**
 * The next irregularity in the list, which is a singly-linked, <tt>NULL</tt> terminated list.
 */
 	curve_irregularities *next;	// next in list; NULL terminated list.
};

/** @} */
#endif
