/* ORIGINAL: acis2.1/sg_husk/vis/vis.hxx */
/* $Id: vis.hxx,v 1.10 2002/08/09 17:19:06 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined ( SG_VISUALIZATION )
#define SG_VISUALIZATION

#include "logical.h"

#include "acis.hxx"
#include "dcl_intr.h"

class SPAtransf;
class SPAposition;
class curve;
class FACE;
class ENTITY_LIST;
class POLYGON_LIST;
class silhouette_options;

// Return a list of silhouette edges trimmed to a face.

DECL_INTR void
sg_make_silhouette_edges(
	FACE*,							// face for which silhouette edges are calculated
	const SPAtransf&,					// transformation for face 
	const SPAposition&,				// from point
	const SPAposition&,				// to point
	int,							// projection type (0=>parallel, 1=>prespective)
	ENTITY_LIST*,					// list of silhouette edges
	silhouette_options*	= NULL			// silhouette options object (tolerance)
	);

// Return a list of triangular polygons covering a face.

DECL_INTR void
sg_facet_face(
	FACE*,							// face for which silhouette edges are calculated
	const SPAtransf&,					// transformation for face 
	double,							// tolerance
	POLYGON_LIST*					// list of polygons
	);

// Return a list of positions and corresponding SPAparameter values on a curve giving
// a linear approximation to within a specified tolerance.

DECL_INTR void
sg_facet_curve( 
	const curve&,					// curve
	double,							// starting SPAparameter
	double,							// ending SPAparameter
	double,							// tolerance
	int,							// max number of points to generate
	int&,							// number of pts generated (set to max+1 if 
									// max exceeded)
	SPAposition[],						// array of positions on curve
	double[]						// array of SPAparameter values for point
	);


#endif
