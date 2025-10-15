/*******************************************************************/
/*    Copyright (c) 2008-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 4-Mar-09 Added enum SPA_approx_degree for use in SPAapprox_options 
//              get/set degree methods
#ifndef SPA_APPROX_ENUM
#define SPA_APPROX_ENUM
/**
 * @file spa_approx_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BSGEOMETRY
 *
 * @{
 */
/**
* This enumeration is used by the SPA_approx_options class to set or query the mode of the approximation paramterization.
 * @param SPA_approx_param_mode_BASE_PARAMETERIZATION
 * Approximate the parameterization of incoming geometry.
 * @param SPA_approx_param_mode_APPROX_ARC_LENGTH
 * The speed of the approximation is everywhere almost 1.
**/

enum SPA_approx_param_mode
{
    SPA_approx_param_mode_BASE_PARAMETERIZATION = 0,  // approximate the parameterization of incoming geometry
    SPA_approx_param_mode_APPROX_ARC_LENGTH           // speed of the curve is approximately = 1
                                                      
};

/**
* This enumeration is used by the SPA_approx_options class to set or query the degree of the approximation.
 * @param SPA_approx_degree_CUBIC
 * Approximation degree 3.
 * @param SPA_approx_degree_QUINTIC
 * Approximation degree 5.
**/


enum SPA_approx_degree
{
    SPA_approx_degree_CUBIC   = 3,
    SPA_approx_degree_QUINTIC = 5
};


/** @} */
#endif // SPA_APPROX_ENUM
