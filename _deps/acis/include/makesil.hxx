/* ORIGINAL: acis2.1/kernint/makesil/makesil.hxx */
/* $Id: makesil.hxx,v 1.10 2002/08/09 17:18:52 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Routines for evaluating silhouette and SPAparameter lines.

#if !defined( MAKESIL_HEADER )
#define MAKESIL_HEADER

#include "dcl_intr.h"
#include "position.hxx"
#include "unitvec.hxx"

class surface;

class SPAbox;

class SPAvector;
class SPAtransf;

class surf_surf_int;


// Class to hold a general parallel/perspective view specification.

class DECL_INTR view_spec : public ACIS_OBJECT {
public:
	SPAunit_vector dir;	// the direction from the "to-point" towards
						// the view point

	double inv_dist;	// the reciprocal of the distance from the
						// "to-point" to the view point, zero for a
						// parallel projection

	SPAposition to_point;	// SPAposition which anchors the view

	view_spec( view_spec const & );

	view_spec( SPAunit_vector const & );
		// Construct a parallel projection

	view_spec( SPAposition const & );
		// Construct a perspective projection

	view_spec(
			SPAunit_vector const &,
			double,
			SPAposition const &
		);
		// Construct a general projection
};


// Compute the silhouette lines on a given surface, for a conical
// projection.

DECL_INTR surf_surf_int *make_surf_persp_sil(
			surface const &,
			SPAposition const &,
			SPAbox const & = *(SPAbox *)NULL_REF,
			surface *& = *(surface **)NULL_REF,
			double const = SPAresabs
		);


// Compute the silhouette lines on a given surface, for a parallel
// projection.

DECL_INTR surf_surf_int *make_surf_parallel_sil(
			surface const &,
			SPAvector const &,
			SPAbox const & = *(SPAbox *)NULL_REF,
			surface *& = *(surface **)NULL_REF,
			double const = SPAresabs
		);


// Compute the silhouette lines on a given surface, for a general
// projection.

DECL_INTR surf_surf_int *make_surf_sil(
			surface const &,
			SPAunit_vector const &,
			double,
			SPAposition const &,
			SPAbox const & = *(SPAbox *)NULL_REF,
			surface *& = *(surface **)NULL_REF
		);


// Compute the silhouette lines on a given surface, for a general
// projection.

DECL_INTR surf_surf_int *
make_sil(
			surface const &,
			view_spec const &,
			SPAbox const & = *(SPAbox *)NULL_REF,
			surface *& = *(surface **)NULL_REF,
			double const = SPAresabs
		);


// Compute SPAparameter lines on a surface

DECL_INTR surf_surf_int *make_surf_par(
			surface const &,
			int,			// Number of lines in the u direction
			int,			// Number of lines in the v direction
			SPAbox const &
		);

#endif
