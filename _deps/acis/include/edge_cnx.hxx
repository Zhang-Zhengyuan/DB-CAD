// $Id: edge_cnx.hxx,v 1.8 2002/08/09 17:15:22 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for Finding the convextity points of an edge.

#if !defined( EDGE_CNV )
#define EDGE_CNV

#include "dcl_intr.h"
#include "logical.h"
#include "base.hxx"

class EDGE;
class SPAposition;

// A convexity point is a point on an edge (i.e. on a curve and two surfaces) at
// which the edge changes convexity. In geometric terms, the cross product of
// the two surface normals reverses direction abruptly. This class finds such
// points by finding the roots of the equation N1 * N2 . T = 0. 

DECL_INTR logical edge_convexity( 
	EDGE *edge, 
	int &no_pts, 
	SPAposition **cnx_points,
	double **cnx_params,
	logical force_check = FALSE,
	double angle_tolerance = SPAresnor,
	logical range = FALSE, 		// Logical to tell whether we are interested in just zeros of the convexity function
					// i.e., range = FALSE, or if we are interested in knowing all crossings of the convexity function
					// with angle tolerance i.e. range = TRUE. Typically we should use range = TRUE only when angle_tol >> resnor.
	logical verify = TRUE,		// Tells us whether to verify the roots or simply trust them
	logical skip_ends = TRUE	// Tells us whether to include the convexity change points at the start/end of the edge or to skip them.
);

#endif

