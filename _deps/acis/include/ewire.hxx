/* ORIGINAL: acis2.1/kernwire/ewire/ewire.hxx */
/* $Id: ewire.hxx,v 1.11 2002/08/09 17:21:36 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header defining routines for constructing wires from edges.

#if !defined( EWIRE_HDR_DEF )
#define  EWIRE_HDR_DEF

#include "dcl_cstr.h"
#include "bs3curve.hxx"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end.

class ENTITY_LIST;
class BODY;
class EDGE;
class SPAposition;
class SPAvector;
class SPAunit_vector;

// STIPORT TAC added osf1, vms
#if defined( ultrix ) || defined( osf1 ) || defined( vms )
#include "position.hxx"
#endif

DECL_CSTR void make_ewire(
			int,				// number of edges
			EDGE *[],			// array of edges to construct wire
			BODY *&				// wire body constructed
		);
DECL_CSTR EDGE *make_edge_line(
			SPAposition const &,	// start point
			SPAposition const &	// end point
		);
DECL_CSTR EDGE *make_edge_ellipse(
			SPAposition const &,	// centre of ellipse
			SPAunit_vector const &,// normal to plane of ellipse
			SPAvector const &,		// major axis direction and magnitude
			double,				// ratio between minor and major axis
								// lengths
			double,				// angular SPAposition of start point,
								// measured in the plane of the ellipse
								// clockwise around the normal,
								// starting at the end of the major
								// axis SPAvector
			double				// angular SPAposition of end point
		);
DECL_CSTR EDGE *make_edge_intcurve(
			int,				// number of points to fit/interpolate
			SPAposition const *,	// the points themselves
			SPAunit_vector const &,// tangent to the desired curve at the
								// start point
			SPAunit_vector const &,// same at the end point
			double fitol		// fit tolerance, exactly zero for
								// interpolation
		);
DECL_CSTR EDGE *make_edge_bs3_curve(
			bs3_curve,			// curve for edge
			SPAposition const & = *(SPAposition *)NULL_REF,
								// start point, NULL if the start of
								// the curve is to be the start of the
								// edge
			SPAposition const & = *(SPAposition *)NULL_REF
								// end point
		);

#endif
