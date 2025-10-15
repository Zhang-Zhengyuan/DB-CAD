/* ORIGINAL: acis2.1/kerngeom/surface/srfperp.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// $Id 

// Header for srfperp

//
// Modification---
//	01-Apr-99 bd	Pass input args as const ref.
//

#if !defined( SRFPERP )
#define SRFPERP

#include "dcl_kern.h"
#include "position.hxx"
#include "vector.hxx"

class surface;
class SPApar_pos;

//
// Drop a perpendicular from a given space point to a surface.
// The point need not be on the surface.
//
// This routine is designed for parametric (i.e. spline) surfaces.
// For analytics, it simply calls their own point_perp() methods.
//
// This routine will return solutions that are outside the surface's
// basic SPAparameter range, if that's where the perpendicular is, so it
// behaves the same as the point_perp()'s for the analytic surfaces.
// If the caller should desire the closest point within the SPAparameter
// range, then they can check which domain limit is exceeded, and
// then point_perp to the appropriate edge.
// Similarly, this routine pays no attention to crossing seams on
// periodic surfaces.
//

DECL_KERN logical surface_perp(	
		surface  const &srf,				// input args
		SPAposition const &pt,
		SPApar_pos  const &uv_guess,			// may be NULL
		double tol,

		SPApar_pos &uv_actual = *(SPApar_pos *)NULL_REF,	// output args
		SPAposition &foot = *(SPAposition *)NULL_REF,
		SPAvector derivs1[2] = NULL,
		SPAvector derivs2[3] = NULL,
	    logical constrained = TRUE  // TRUE if constrained min wanted
						            // FALSE if unconstrained min wanted
	);

#endif
