// $Id: net_utl.hxx,v 1.5 2001/01/11 18:01:37 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Utility routines, which are used in the cover/skin/loft sub-system. Inorder to 
// keep cover and skin husks independent of each other these routines are kept here.
// IMPORTANT: 
// These routines should not be called by any-one unless one understands the routines 
// behaviour completely and also willing to take the chance of possible un-documented 
// side-effects.

#if !defined( NET_UTL_HDR_DEF )
#define   NET_UTL_HDR_DEF

#include "dcl_cstr.h"
// ywoo 08Jan01: uncoomented "logical.h"
#include "logical.h"
//#include "baseutil/vector/position.hxx"

class COEDGE;
class curve;
class netspl_corner;


DECL_CSTR curve *
jg_coedge_curve( 
				COEDGE	*c 	// Given coedge
);

DECL_CSTR logical
sg_create_corner_intersections ( netspl_corner **corners,
						    int no_crv_v, int no_crv_u,
							curve **v_curves, curve **u_curves, double tolerance );

DECL_CSTR void
sg_cull_curves ( netspl_corner **corners,
						    int no_crv_v, int no_crv_u,
							curve **v_curves, curve **u_curves );
#endif
