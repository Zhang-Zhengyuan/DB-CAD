/* ORIGINAL: acis2.1/kernapi/api/kernapi.hxx */
/* $Id: ptcont.hxx,v 1.12 2000/12/26 18:41:57 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/**
* @file ptcont.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#if !defined( POINT_CONTAINMENT_DEF )
#define POINT_CONTAINMENT_DEF

/** \addtogroup INTRAPICONTAINMENT
 *  \brief Declared at <ptcont.hxx>
 *  @{
 */

// Result values for api_point_in_body

/**
 * Specifies whether a point lies inside, outside or on the boundary of the body.
 * <br>
 * <b>Role:</b> Used by <tt>api_point_in_body()</tt>.
 * @param point_unknown
 * unknown.
 * @param point_inside
 * point lies inside body.
 * @param point_boundary
 * point lies on the boundary of the body.
 * @param point_outside
 * point lies outside the body.
 **/
enum point_containment {
	point_unknown,
	point_inside,
	point_boundary,
	point_outside
};

/** @} */

#endif
