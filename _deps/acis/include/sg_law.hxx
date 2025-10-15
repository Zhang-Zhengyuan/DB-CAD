// $Id: sg_law.hxx,v 1.17 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SG_LAW_HXX
#define SG_LAW_HXX

#include "dcl_kern.h"
#include "option.hxx"
#include "bs3curve.hxx" // defines bs3_curve_form and bs3_curve // defines bs3_curve_form and bs3_curve
#include "bs2curve.hxx" // defines bs2_curve
#include "bs3surf.hxx" // defines bs3_surf_form and bs3_surface // defines bs3_surf_form and bs3_surface
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo 08Jan01: added the classes.
class SPAvector;
class SPAposition;
class SPAinterval;
// ywoo: end
class law;
class curve;
class surface;
class sw_curve;
class discontinuity_info;

DECL_KERN curve *sg_curve_law(
    law    *in_law,
	double start,
	double end,
	double tol,
    int    law_number=0,
	law    **other_laws=NULL,
	discontinuity_info& disc_info = *(discontinuity_info*) NULL_REF,
	surface const & surf1 = *(const surface *)NULL_REF,
	surface const & surf2 = *(const surface *)NULL_REF,
	bs2_curve pcurve1 = NULL,
	bs2_curve pcurve2 = NULL,
	logical simplify = TRUE);

DECL_KERN curve *make_law_helix(
	const SPAvector&   axis,
	const SPAposition& root,
	const SPAposition& start_point,
	const SPAposition& end_point,
	double   pitch,
	double   radius,
	logical  right_handed,
	double   &end_param
	);

DECL_KERN curve *sg_composite_curve(
	int no_crv,
	curve **cur_arr,
	SPAinterval *cur_domains = NULL,
	logical need_c1 = TRUE,
	logical join_splines = FALSE,
	logical approx_ok = FALSE,
	double *junction_params = NULL // size should be no_crv + 1
	);
#endif

