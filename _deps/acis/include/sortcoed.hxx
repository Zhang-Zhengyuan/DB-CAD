/* ORIGINAL: acis2.1/kerndata/sortcoed/sortcoed.hxx */
/* $Id: sortcoed.hxx,v 1.9 2002/08/09 17:15:19 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Function to sort the coedges around a given edge such that their attached
// faces are in a clockwise order around the edge direction, starting with
// the coedge originally pointer to by the edge.

// For obvious reasons only has any action on non-manifold edges (i.e. ones
// with more than two coedges).


#if !defined( SORTCE_HDR_DEF )
#define  SORTCE_HDR_DEF

#include "dcl_kern.h"

class EDGE;
class COEDGE;
class SPAposition;
class SPAunit_vector;

//---------------------------------------------------------------
//	sort_coedges - do a radial sort of the COEDGEs on this EDGE.
//---------------------------------------------------------------
DECL_KERN void sort_coedges(
			EDGE *
			);

//--------------------------------------------------------------------------
//  into_face - find INWARD face tangent SPAvector given a COEDGE and
//      either a SPAposition or a SPAparameter on the corresponding EDGE.
//      May optionally supply the direction of the EDGE at this point.
//--------------------------------------------------------------------------
DECL_KERN SPAunit_vector into_face(
					const COEDGE *coedge, 			// Coedge of EDGE on FACE
					const double &ed_param, 		// Edge SPAparameter
					const SPAposition&pt,				// Edge SPAposition
					const SPAunit_vector &edge_dir		// Edge direction
					);
#endif
