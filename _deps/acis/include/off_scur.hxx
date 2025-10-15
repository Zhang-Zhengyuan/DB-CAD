/* ORIGINAL: acis2.1/sg_husk/offsur/off_scur.hxx */
/* $Id: off_scur.hxx,v 1.4 2002/08/09 17:27:15 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SG_OFF_SUR_CUR )
#define SG_OFF_SUR_CUR

#include "dcl_ofst.h"

// Forward Declarations
class intcurve;
class pcurve;
class SPAposition;
class surface;
class SPAvector;
class curve;

// Offset the curve SPAposition and tangent
DECL_OFST void
sg_offset_pos_tan(
	const intcurve *,	// original intersection curve
	const pcurve *,		// parametric representation of original curve
	const surface *,	// original surface
	const SPAposition &,	// point on surface
	double,				// SPAparameter of point on curve
	double,				// distance to offset
	SPAposition &,			// offset SPAposition
	SPAvector & );			// offset tangent

DECL_OFST curve *
sg_offset_surface_curve(
    const curve*    original_curve,
    const pcurve*   original_pcurve,
    const surface*  surf,
    const surface*  offset_surf,
    double          start_param,
    double          end_param,
    double          offset_distance);

#endif
