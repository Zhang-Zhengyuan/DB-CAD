/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Construct a  standard  wiggly surface, for testing spline handling
// without having to go to the lengths  of full  surface construction
// facilities.
/*******************************************************************/
#if !defined( CC_ROUT_HDR_DEF )
#define  CC_ROUT_HDR_DEF
#include "dcl_spl.h"
#include "bs3surf.hxx"
/**
* @file cc_rout.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BS3SURFACEAPI
 *
 * @{
 */
/**
* Creates four splines for the edges or a wiggle.
* <br><br>
* <b>Role:</b> Creates  four  splines  for  the edges. Each is a cubic
* B-spline with two spans, passing through three colinear points.  The
* shape is specified by the appropriate integer argument, as  follows:
*<table border="0" width="100%">
*<tr>
*<td width="5%" align="right"><tt>0</tt></td>
*<td width="5%"></td>
*<td width="90%"> <tt>Straight line</tt></td>
*</tr>
*<tr>
*<td width="5%" align="right"><tt>1</tt></td>
*<td width="5%"></td>
*<td width="90%"> <tt>S shape. Starting at low parameter value with a 45 degree upward
*(positive z) tangent,  and  ending at high parameter in the same direction.</tt></td>
* </tr>
* <tr>
*<td width="5%" align="right"><tt>2</tt></td>
*<td width="5%"></td>
*<td width="90%"><tt>Double hump. Starting going upwards and ending downwards.</tt></td>
*</tr>
*<tr>
*<td width="5%" align="right"><tt>-1</tt></td>
*<td width="5%"></td>
*<td width="90%"><tt>Same as 1, but inverted.</tt></td>
*</tr>
*<tr>
*<td width="5%" align="right"><tt>-2</tt></td>
*<td width="5%"></td>
*<td width="90%"><tt>Same as 2, but inverted.</tt></td>
*</tr>
*</table>
* <b>Errors:</b> None.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* @param width
* width.
* @param depth
* depth.
* @param height
* height.
* @param low_v_type
* low v type.
* @param high_v_type
* high v type.
* @param low_u_type
* low u type.
* @param high_u_type
* high u type.
**/
DECL_SPLINE bs3_surface bs3_surface_wiggle(
			double width,
			double depth,
			double height,
			int low_v_type = 1,
			int high_v_type = -2,
			int low_u_type = 2,
			int high_u_type = -1
		);

/** @} */
#endif
