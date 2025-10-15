/* ORIGINAL: acis2.1/sg_husk/inter/inteded.hxx */
/* $Id: inteded.hxx,v 1.8 2002/08/09 17:19:02 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef INTEDED_HXX
#define INTEDED_HXX

#include "dcl_intr.h"
#include "base.hxx"

class EDGE;
class curve_curve_int;
class ENTITY_LIST;

// The following functions are NOT supported and are intended for
// use by ACIS internal functionalities only.

DECL_INTR int 
sg_inter_ed_ed( 
		EDGE*,
		EDGE*, 
		curve_curve_int*&,
		double dist_tol = SPAresabs,
		double angle_tol = SPAresnor
	 );

DECL_INTR void 
sg_delete_cci_list( 
		curve_curve_int* 
	);

DECL_INTR int 
sg_ed_inters_to_ents( 
		EDGE*, 
		curve_curve_int*,
		ENTITY_LIST&,
		EDGE* = NULL,
		VOID_LIST * matching_ints = NULL
	);

// New Edge Edge intersector, with an optional tolerance
DECL_INTR curve_curve_int *
eded_int(
		EDGE*,
		EDGE*,
		double dist_tol = SPAresabs,
		double angle_tol = SPAresnor
	);

#endif
