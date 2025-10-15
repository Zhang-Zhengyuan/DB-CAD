// $Id: pcur_rtn.hxx,v 1.4 2002/08/09 17:15:25 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PCURVE_ROUTINES_HXX
#define PCURVE_ROUTINES_HXX

//
// Declaration of routines that deal with pcurves in a general
// sense, i.e., curves in the SPAparameter space of a surface.
//

//
// Modification---
//	19-Sep-00 bd	 New code: make fit_pcurve_bs2() non-local.
//

#include "acis.hxx"

#include "dcl_kern.h"
#include "logical.h"
#include <math.h>

#include "bs2curve.hxx"
#include "curdef.hxx"
#include "surdef.hxx"
#include "param.hxx"
#include "interval.hxx"


// Fit a bs2_curve as a pcurve to a curve and a surface.

DECL_KERN bs2_curve fit_pcurve_bs2(
	curve const &cur,
	SPAinterval const &crv_rng,
	surface const &sur,
	double tol,

	double &achieved_tol,
	double &achieved_partol,
	SPApar_vec &off_vec
);

#endif
