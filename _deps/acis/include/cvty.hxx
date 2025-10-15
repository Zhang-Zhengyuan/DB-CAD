/* $Id: cvty.hxx,v 1.8 2002/08/09 17:27:00 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "logical.h"

#ifndef LTCV
#define LTCV

#include "dcl_lopt.h"

#include "position.hxx"
#include "errorsys.hxx"

class	EDGE;
class	COEDGE;
class	VERTEX;
class	curve;

// convexity  
enum LOPT_EDGE_cvty
{ 
	LOPT_EDGE_cvty_concave = 0, 
		LOPT_EDGE_cvty_tangent_concave = 1, 
		LOPT_EDGE_cvty_tangent = 2,  // dont know what sort
		LOPT_EDGE_cvty_tangent_convex = 3,
		LOPT_EDGE_cvty_convex  = 4,
		LOPT_EDGE_cvty_knife_convex = 5, 
		LOPT_EDGE_cvty_knife = 6,	// dont know what sort 
		LOPT_EDGE_cvty_knife_concave = 7, 
		LOPT_EDGE_cvty_tangent_inflect = 8,  // here to keep symmetry as far as pos
		LOPT_EDGE_cvty_unknown = 9,
		LOPT_EDGE_cvty_near_tangent = 10 // wolf (10/03) added for approximation project
};

// Convexity functions. 

DECL_LOPT LOPT_EDGE_cvty lopt_calc_convexity( 
											 SPAunit_vector const &norm0, 
											 SPAunit_vector const &norm1,
											 SPAunit_vector const &dir, 
											 double const tol = SPAresnor,
											 double const &K1 = * ( double * ) NULL_REF, 
											 double const &K2 = * ( double * ) NULL_REF,
											 double const &loose_tol = * ( double * ) NULL_REF,
											 logical & loose_tan = * ( logical * ) NULL_REF);

// convexity of edge
DECL_LOPT LOPT_EDGE_cvty lopt_calc_convexity( EDGE *edge, 
											 double const tol = SPAresnor,
											 double const &loose_tol = *(double*)NULL_REF,
											 logical &loose_tan = *(logical*)NULL_REF,
											 logical check_extra_points = FALSE,
											 logical check_near_tangent_portion = FALSE);

// convexity of edge at a VERTEX, at coedge start 
DECL_LOPT LOPT_EDGE_cvty lopt_calc_convexity( COEDGE *coedge, 
											 double const tol = SPAresnor,
											 double const &loose_tol = *(double*)NULL_REF,
											 logical &loose_tan = *(logical*)NULL_REF);

// convexity of coedge at a SPAparameter
DECL_LOPT LOPT_EDGE_cvty lopt_calc_convexity( double const &t, 
											 COEDGE *coedge, 
											 double const tol = SPAresnor);

// to test the convexity enum for general types
DECL_LOPT logical lopt_concave_convexity( LOPT_EDGE_cvty const &cvty );
DECL_LOPT logical lopt_convex_convexity( LOPT_EDGE_cvty const &cvty );
DECL_LOPT logical lopt_tangent_convexity( LOPT_EDGE_cvty const &cvty );
DECL_LOPT logical lopt_knife_convexity( LOPT_EDGE_cvty const &cvty );

#ifdef D3_DEBUG
class D3_ostream;
DECL_LOPT D3_ostream& operator<<( D3_ostream& os, const LOPT_EDGE_cvty cvty );
#endif


#endif

