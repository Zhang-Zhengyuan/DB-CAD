/* ORIGINAL: acis2.1/sg_husk/vis/sur_utl.hxx */
/* $Id: sur_utl.hxx,v 1.8 2002/08/09 17:15:38 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (SG_SURFACE_UTIL)
#define SG_SURFACE_UTIL

#include "dcl_kern.h"
#include "param.hxx"
#include "position.hxx"
#include "transf.hxx"

#include "curdef.hxx"

#include "surdef.hxx"

#include "face.hxx"
#include "coedge.hxx"

DECL_KERN surface*
sg_wcs_surface(
	FACE*			face,
	SPAtransf const&	Btrans
	);

DECL_KERN logical
sg_surface_pole(
	const SPAposition&  pos,
	const surface&	    srf
	);

//  Return a list of poles on the surface.

enum pole_bndry_type
{
	pole_none,
	pole_low_u,
	pole_low_v,
	pole_high_u,
	pole_high_v,
	pole_both_u,
	pole_both_v
};

DECL_KERN void
sg_surface_get_poles(
    surface const&  srf,
    int& n_poles,
    SPAposition poles[],
	pole_bndry_type * b_type = NULL
    );

#endif
